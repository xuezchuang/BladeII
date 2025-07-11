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
	/** �� Step �� ���� ���ݱ��� �ε��� �� ������ ������ progress ���� �̸�ŭ�� ������ ���� �ȴ�. 
	 * ���� ���� �� ���� ������ ���� ������ Step ���� �ϴ� �� */
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
	{ // � PCClass �� �ε��ؾ� ���� ������ Ȯ���� ������ �ֱ� ����� ��忡�� ����ϱ� ����.
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
	 * �� ���Ӹ�� Ÿ�� ���� InfoAsset ������ �ε��� ���� ������ �� �ε� ������ �����ϴ� �Լ����� ���� �����Ѵ�.
	 * ���� �ε��� �ȵ� ���Ӹ�忡 ���� ������ �ʿ��ؼ� GameMode �� �������̽��� �� ���� ����.
	 * �׷��� �ߴ��� ������ �����ϱ� ���� ���⿡ ��Ƴ����� ��.
	 * ����� �� �ִ��� Fallback �� �ְ�, ������ �������� �ϰ� �Ǵ� �� �ƴ�.
	 */

	float GetPureMapLoadShare_Lobby(UObject* WorldContextObject)
	{
		AB2DLCFrontGameMode* DLCGM = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
		if (DLCGM){ // DLCFrontGameMode ���� �κ�� �̵��ϴ� �� ���� ��ŸƮ�� ��Ȳ.. �� ������ ���� ���߾��.
			return 0.2f;
		}

		return FLobbySceneManager::HasDeferredChangeLobbyScenes() ? 0.4f : 0.6f;
	}
	float GetPureMapLoadShare_Scenario(UObject* WorldContextObject)
	{
		// ���� InfoAsset ���δ� ���� ū �������� PCClassInfo �� �󸶳� �ε��Ǿ� �ִ����� ���� ���� �� �ε������� ����ġ�� �ٸ��� ��.
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true))
			{
				// �� �������� ���� PC ������ �ε��� ������� �� ������ �ε� ���п� �󸶳� ������ ��ġ�°����� 
				// PC ������ ��ü�� ������ ���ϴ� �� �ƴ�. PC ������ ��ü ������ ��������� �� Ŭ ������ ����.
				PCLoadedWeight += 0.12f; 
			}
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass(), true))
			{
				PCLoadedWeight += 0.12f;
			}
		}
		float NPCLoadedWeight = 0.0f;
		// �̰͵� ����� Ŀ�� �ε� ���൵ ���� ������ ������ �ʿ�. ���� ���� �ε��� �󸶳� ���Ǵ��������� �𸣰�����.
		UB2NPCClassInfoBox* NPCBox = GetNPCClassInfoBox(WorldContextObject);
		if (NPCBox)
		{
			NPCLoadedWeight += FMath::Min(((float)NPCBox->GetEstimatedRootedAssetNum() * 0.01f), 0.2f);
		}

		// PCSkillAnim ��Ʈ�� ���δ� ���� PCClassInfo �� ���� ���ٰ� ��. �װ� �����ؼ��� PCClassInfo �� RootSet ���ο� ū ����ġ.
		// �������� ��Ȳ�� ���� DamageEffectInfo �� BattleMain ���� �� �� �� �ִµ�.. �װͱ��� �� ������� ������ ����
		
		return 0.3f + PCLoadedWeight + NPCLoadedWeight;
	}
	float GetPureMapLoadShare_CounterDungeon(UObject* WorldContextObject)
	{
		float PCLoadedWeight = 0.0f;
		UB2PCClassInfoBox* PCBox = GetPCClassInfoBox(WorldContextObject);
		if (PCBox)
		{ // ���� �ϳ���.
			if (PCBox->IsClassDataAtRootSet(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true))
			{
				PCLoadedWeight += 0.25f;
			}
		}

		return 0.4f + PCLoadedWeight;
	}
	float GetPureMapLoadShare_HeroTower(UObject* WorldContextObject)
	{
		// �ó����� �ʺ��� �� ��ü�� �۰� NPC �����Ͱ� ����
		return GetPureMapLoadShare_Scenario(WorldContextObject) * 0.7;
	}
	// ����Ʈ ĳ���Ͱ� �ִ� �ʵ��� ���������� PCClassInfo ������ �ܿ� �� ĳ���͵��� ��Ʈ�ε��� �ִ�. �̰� �뷫 �Ϲ� �ó����� ���� NPCClassInfo ��ŭ���� �����ϸ� �Ƿ���.. 
	float GetPureMapLoadShare_PVP(UObject* WorldContextObject)
	{
		// PVP �� Ư���� ������ ��Ʈ���� ������ ���߿� �ε��ؼ�.. �� ������ �Ƹ� �� ���� ��.

		float PCLoadedWeight = GetAllPCClassRootSetWeight(WorldContextObject, 0.1f);

		return FMath::Min(0.1f + PCLoadedWeight, 0.9f);
	}
	float GetPureMapLoadShare_TeamMatch(UObject* WorldContextObject)
	{
		// �ٵ� �ٸ� ���鵵.. ���� ���� ��� ��Ʈ �ε��ؾ� �ϴ� �� �����ϸ�.. �� �ε� ������ ���� �� ���� ���� �Ѥ�

		float PCLoadedWeight = GetAllPCClassRootSetWeight(WorldContextObject, 0.1f);

		return FMath::Min(0.15f + PCLoadedWeight, 0.9f); // Ȥ�ö� PCClass �þ�ٰ� �̰� 1.0 �� �Ѿ�� ���� ����..
	}
	float GetPureMapLoadShare_Raid(UObject* WorldContextObject)
	{
		// Raid �� �� �ε� ���� � ĳ���� ������ �ʿ����� �̸� �� �� ����.
		TArray<EPCClass> AllNecessaryPCClasses = AB2NetGameMode::GetAllNetGameMatchedPCClasses();
		check(AllNecessaryPCClasses.Num() > 0);
		// ���� ��� ����� �� ��Ȯ������ ������ �׷��� ��Ȯ�� �ʿ���� �ִ� �͵� �ƴϰ�..
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
		// Raid ó�� (�̸� Ŭ������ �˰� �ִ�) �ټ��� �÷��̾� ĳ���Ϳ� �ϳ��� ���� ����. 
		return GetPureMapLoadShare_Raid(WorldContextObject);
	}
	float GetPureMapLoadShare_Tutorial(UObject* WorldContextObject)
	{
		// ó�� �ϳ� ���� async �ε� ��. �׸��� ���� ��� ���� �� �� ũ�� ��� �� ���� ��.
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
		// �������ϰ� ����� �����ε�.. NPC �����Ͱ� �־ PureMap ������ �ణ �����.
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
		// ���� �� �ε� ������ �Ҹ��� ��
		// ������ �ε� �� ���Ӹ��� ���� �ֿ� ���� Rootset ������ ���� ���� �� �ε������� progress ������ "������" �����Ѵ�.

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
		//// ������ ����Ǵ� 0.0 ~ 1.0 ���� ���� �ִٸ� �̰ɷδ� 100% ���� ä���� �ʰ� �ȴ�.
		//float CurrentProgress = GlobalGetMapLoadingProgress();
		//float AddedValue = (1.0f - CurrentProgress) * CurrentGameResLoadingStepRate;

		//GlobalAddMapLoadingProgress(AddedValue);
	}

	void StepCustom(float InSpecifiedRate, float DesiredMinGameResourceLoadingProgress /*= -1.0f*/)
	{
		//// �̰� ���� ���� �� ���� ���� ���� �� ���Ƽ� ���� �� �� �ְ� �����ϴ� �Ű� ���� ��쿣 �̰ɷ� ���� �� ��.
		//SetStepRate(InSpecifiedRate);
		//Step();

		//// �߰��� ������ �ּ� progress �� ������ �� �ִ�. ���� �� �ε� ������ �ΰ��� ������ �ȿ����� ������.
		//if (DesiredMinGameResourceLoadingProgress > 0.0f)
		//{
		//	const float CurrentTotalProgress = GlobalGetMapLoadingProgress(); // ��¥ ���� ��ü ���൵
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
		// ������ ������ �ε��ٸ� 100% �� ä������ ����. ������ �����̶�� ��..
		// �ε��� �����δ� ������ ������ �ʾƵ� �񵿱� �ε� ȭ�鿡�� ������ �����ϴ� ���� �ε� ���൵ 100% �� �� �� �ְ�, �ű⼭ �� ������ �ʾ� ������ �ε��� ����Ǵ�.. ����
		// ���� Ȥ���� �ǵ�ġ �ʰ� �ð��� ���� �Ҹ��� �� �ִ� Async �ε��� ���� Flush �� ������ �ϰ�.. (���⼭ ���� ���� �ʾƵ� �ᱹ �̿� �����ϴ� ������ �Ͼ�� �ϰ�����)
		// �̰� �� �� ���Ŀ��� ������ ������ ������ �ð��� ��ƸԴ� ������ �Ͼ�� �Ѵ�. ���� ��� ���� UI ����.
		FlushAsyncLoading();
		StepCustom(1.0f, 1.0f);
	}

}