// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "B2GameLoadingProgress.h"
#include "BladeIIGameImpl.h"
#include "BladeIIGameMode.h"
#include "B2LobbyGameMode.h"
#include "B2PCClassInfoBox.h"
#include "B2NPCClassInfoBox.h"
#include "B2NetGameMode.h"
#include "B2DLCFrontGameMode.h"
#include "B2GameInstance.h"

namespace B2GMLoadingProgCollector
{
	/** 매 Step 콜 마다 지금까지 로딩된 거 제외한 나머지 progress 에서 이만큼의 비율이 차게 된다. 
	 * 구간 별로 이 값을 설정해 놓고 적절히 Step 콜을 하는 식 */
	float CurrentGameResLoadingStepRate = 0.1f;


	UB2PCClassInfoBox* GetPCClassInfoBox(UObject* WorldContextObject)
	{
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		return B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox();
	}
	UB2NPCClassInfoBox* GetNPCClassInfoBox(UObject* WorldContextObject)
	{
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		return B2GI ? B2GI->GetMobClassInfoBox() : StaticFindMobClassInfoBox();
	}

	float GetAllPCClassRootSetWeight(UObject* WorldContextObject, float InSinglePCClassInfoWeight)
	{ // 어떤 PCClass 를 로딩해야 할지 정보를 확실히 전달해 주기 어려운 모드에서 사용하기 위함.
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{
			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
			{
				EPCClass ThisPCClass = IntToPCClass(PCI);
				if (PCBox->IsClassDataAtRootSet(ThisPCClass, true))
				{
					PCLoadedWeight += InSinglePCClassInfoWeight;
				}
			}
		}
		return PCLoadedWeight;
	}


	//////////////////////////////////////////////////////////////////////////////////////

	/**
	 * 각 게임모드 타입 별로 InfoAsset 컨텐츠 로딩이 빠진 순수한 맵 로딩 지분을 리턴하는 함수들을 개별 구현한다.
	 * 아직 로딩인 안된 게임모드에 대한 정보가 필요해서 GameMode 쪽 인터페이스로 할 수가 없음.
	 * 그렇게 했더라도 구현은 관리하기 좋게 여기에 모아놓았을 것.
	 * 까먹은 게 있더라도 Fallback 이 있고, 게임이 오동작을 하게 되는 거 아님.
	 */

	float GetPureMapLoadShare_Lobby(UObject* WorldContextObject)
	{
		AB2DLCFrontGameMode* DLCGM = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
		if (DLCGM){ // DLCFrontGameMode 에서 로비로 이동하는 건 게임 스타트업 상황.. 맵 지분을 많이 낮추어야.
			return 0.2f;
		}

		return FLobbySceneManager::HasDeferredChangeLobbyScenes() ? 0.4f : 0.6f;
	}
	float GetPureMapLoadShare_Scenario(UObject* WorldContextObject)
	{
		// 단일 InfoAsset 으로는 가장 큰 데이터인 PCClassInfo 가 얼마나 로딩되어 있는지에 따라 순수 맵 로딩에서의 가중치를 다르게 줌.
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true))
			{
				// 이 더해지는 값은 PC 데이터 로딩이 상대적인 맵 데이터 로딩 지분에 얼마나 영향을 미치는가이지 
				// PC 데이터 자체의 지분을 뜻하는 건 아님. PC 데이터 자체 지분은 통상적으로 더 클 것으로 예상.
				PCLoadedWeight += 0.12f; 
			}
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass(), true))
			{
				PCLoadedWeight += 0.12f;
			}
		}
		float NPCLoadedWeight = 0.0f;
		// 이것도 상당히 커서 로딩 진행도 지분 산정에 참고할 필요. 실제 다음 로딩에 얼마나 사용되는지까지는 모르겠지만.
		UB2NPCClassInfoBox* NPCBox = GetNPCClassInfoBox(WorldContextObject);
		if (NPCBox)
		{
			NPCLoadedWeight += FMath::Min(((float)NPCBox->GetEstimatedRootedAssetNum() * 0.01f), 0.2f);
		}

		// PCSkillAnim 루트셋 여부는 거의 PCClassInfo 와 같이 간다고 봄. 그걸 생각해서라도 PCClassInfo 의 RootSet 여부에 큰 가중치.
		// 나머지는 상황에 따라 DamageEffectInfo 나 BattleMain 같은 게 될 수 있는데.. 그것까지 다 따지기는 어려우니 무시
		
		return 0.3f + PCLoadedWeight + NPCLoadedWeight;
	}
	float GetPureMapLoadShare_CounterDungeon(UObject* WorldContextObject)
	{
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{ // 여긴 하나뿐.
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true))
			{
				PCLoadedWeight += 0.25f;
			}
		}

		return 0.4f + PCLoadedWeight;
	}
	float GetPureMapLoadShare_HeroTower(UObject* WorldContextObject)
	{
		// 시나리오 맵보다 맵 자체는 작고 NPC 데이터가 많음
		return GetPureMapLoadShare_Scenario(WorldContextObject) * 0.7;
	}
	// 리모트 캐릭터가 있는 맵들은 공통적으로 PCClassInfo 데이터 외에 각 캐릭터들의 파트로딩이 있다. 이걸 대략 일반 시나리오 맵의 NPCClassInfo 만큼으로 간주하면 되려나.. 
	float GetPureMapLoadShare_PVP(UObject* WorldContextObject)
	{
		// PVP 는 특히나 전투용 스트리밍 레벨은 나중에 로딩해서.. 맵 비중이 아마 젤 작을 듯.

		float PCLoadedWeight = GetAllPCClassRootSetWeight(WorldContextObject, 0.1f);

		return FMath::Min(0.1f + PCLoadedWeight, 0.9f);
	}
	float GetPureMapLoadShare_TeamMatch(UObject* WorldContextObject)
	{
		// 근데 다른 모드들도.. 상대방 장착 장비 파트 로딩해야 하는 거 생각하면.. 맵 로딩 지분을 많이 줄 수가 없군 ㅡㅡ

		float PCLoadedWeight = GetAllPCClassRootSetWeight(WorldContextObject, 0.1f);

		return FMath::Min(0.15f + PCLoadedWeight, 0.9f); // 혹시라도 PCClass 늘어났다가 이게 1.0 이 넘어가면 좋지 않지..
	}
	float GetPureMapLoadShare_Raid(UObject* WorldContextObject)
	{
		// Raid 는 맵 로딩 전에 어떤 캐릭터 정보가 필요한지 미리 알 수 있음.
		TArray<EPCClass> AllNecessaryPCClasses = AB2NetGameMode::GetAllNetGameMatchedPCClasses();
		check(AllNecessaryPCClasses.Num() > 0);
		// 뭔가 계산 방식이 썩 정확하지는 않은데 그렇게 정확할 필요까지 있는 것도 아니고..
		float PerPCWeight = 0.4f / AllNecessaryPCClasses.Num();
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{
			for (EPCClass ThisPCClass : AllNecessaryPCClasses)
			{
				if (PCBox->IsClassDataAtRootSet(ThisPCClass, true))
				{
					PCLoadedWeight += PerPCWeight;
				}
			}
		}
		return 0.15f + PCLoadedWeight;
	}
	float GetPureMapLoadShare_Control(UObject* WorldContextObject)
	{
		// Raid 처럼 (미리 클래스를 알고 있는) 다수의 플레이어 캐릭터와 하나의 몹이 있음. 
		return GetPureMapLoadShare_Raid(WorldContextObject);
	}
	float GetPureMapLoadShare_Tutorial(UObject* WorldContextObject)
	{
		// 처음 하나 빼고 async 로딩 함. 그리고 여긴 배경 쪽을 좀 더 크게 잡는 게 좋을 듯.
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{ 
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true))
			{
				PCLoadedWeight += 0.2f;
			}
		}

		return 0.5f + PCLoadedWeight;
	}
	float GetPureMapLoadShare_Guild(UObject* WorldContextObject)
	{
		// 팀대전하고 비슷한 성격인데.. NPC 데이터가 있어서 PureMap 지분을 약간 낮춘다.
		return GetPureMapLoadShare_TeamMatch(WorldContextObject) * 0.8f;
	}
	float GetPureMapLoadShare_PreRender()
	{
		return 0.2f;
	}
	float GetPureMapLoadShare_Fallback()
	{
		return 0.5f;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	float GetPureMapLoadShareForGM(UObject* WorldContextObject, EB2GameMode InGameModeToLoad)
	{
		// 다음 맵 로딩 직전에 불리는 거
		// 다음에 로딩 될 게임모드와 현재 주요 에셋 Rootset 사정에 따라서 순수 맵 로딩까지의 progress 지분을 "적당히" 리턴한다.

		if (WorldContextObject)
		{
			switch (InGameModeToLoad)
			{
			case EB2GameMode::Lobby: return GetPureMapLoadShare_Lobby(WorldContextObject);
			case EB2GameMode::Scenario: return GetPureMapLoadShare_Scenario(WorldContextObject);
			case EB2GameMode::CounterDungeon: return GetPureMapLoadShare_CounterDungeon(WorldContextObject);
			case EB2GameMode::HeroTower: return GetPureMapLoadShare_HeroTower(WorldContextObject);
			case EB2GameMode::PVP_Tag: return GetPureMapLoadShare_PVP(WorldContextObject);
			case EB2GameMode::PVP_Team: return GetPureMapLoadShare_TeamMatch(WorldContextObject);
			case EB2GameMode::Raid: return GetPureMapLoadShare_Raid(WorldContextObject);
			case EB2GameMode::Control: return GetPureMapLoadShare_Control(WorldContextObject);
			case EB2GameMode::Tutorial: return GetPureMapLoadShare_Tutorial(WorldContextObject);
			case EB2GameMode::Guild: return GetPureMapLoadShare_Guild(WorldContextObject);
			case EB2GameMode::PreRender: return GetPureMapLoadShare_PreRender();
			case EB2GameMode::Dimension: return GetPureMapLoadShare_Scenario(WorldContextObject);
			}
		}

		return GetPureMapLoadShare_Fallback();
	}

	void PrepareWithPureMapLoadShareForGM(UObject* WorldContextObject, EB2GameMode InGameModeToLoad)
	{
		//GlobalSetPureMapLoadingShareValue(GetPureMapLoadShareForGM(WorldContextObject, InGameModeToLoad));
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void SetStepRate(float InNewValue)
	{
		CurrentGameResLoadingStepRate = FMath::Clamp(InNewValue, 0.01f, 0.99f);
	}
	
	void Step()
	{
		//// 값들이 예상되는 0.0 ~ 1.0 범위 내에 있다면 이걸로는 100% 까지 채우지 않게 된다.
		//float CurrentProgress = GlobalGetMapLoadingProgress();
		//float AddedValue = (1.0f - CurrentProgress) * CurrentGameResLoadingStepRate;

		//GlobalAddMapLoadingProgress(AddedValue);
	}

	void StepCustom(float InSpecifiedRate, float DesiredMinGameResourceLoadingProgress /*= -1.0f*/)
	{
		//// 이걸 둘이 따로 쓸 만한 곳이 있을 거 같아서 따로 쓸 수 있게 제공하는 거고 많은 경우엔 이걸로 땜질 될 듯.
		//SetStepRate(InSpecifiedRate);
		//Step();

		//// 추가로 고정된 최소 progress 를 지정할 수 있다. 순수 맵 로딩 제외한 인게임 데이터 안에서의 비율로.
		//if (DesiredMinGameResourceLoadingProgress > 0.0f)
		//{
		//	const float CurrentTotalProgress = GlobalGetMapLoadingProgress(); // 진짜 완전 전체 진행도
		//	const float GameResourceShare = GlobalGetGameSpecificContentLoadingShare();
		//	const float PureMapLoadingShare = 1.0f - GameResourceShare;

		//	const float DesiredMinTotalProgress = PureMapLoadingShare + GameResourceShare * DesiredMinGameResourceLoadingProgress;
		//	if (DesiredMinTotalProgress > CurrentTotalProgress)
		//	{
		//		GlobalAddMapLoadingProgress(DesiredMinTotalProgress - CurrentTotalProgress);
		//	}
		//}
	}

	void StepToMax()
	{
		// 적당한 시점에 로딩바를 100% 로 채워놓기 위함. 적당한 시점이라는 건..
		// 로딩이 실제로는 완전히 끝나지 않아도 비동기 로딩 화면에서 유저가 인지하는 동안 로딩 진행도 100% 를 볼 수 있고, 거기서 얼마 지나지 않아 실제로 로딩이 종료되는.. ㄷㄷ
		// 따라서 혹여나 의도치 않게 시간을 많이 소모할 수 있는 Async 로딩에 대해 Flush 를 한차례 하고.. (여기서 굳이 하지 않아도 결국 이에 상응하는 동작은 일어나긴 하겠지만)
		// 이걸 콜 한 이후에는 길지는 않지만 나름의 시간을 잡아먹는 동작이 일어나야 한다. 예를 들어 전투 UI 띄우기.
		FlushAsyncLoading();
		StepCustom(1.0f, 1.0f);
	}

}