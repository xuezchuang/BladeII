#include "BladeII.h"
#include "B2UIDocManager.h"
//#include "B2UIDocHelper.h"
//#include "GameDelegates.h"
//#include "BladeIIPlayer.h"	//여기 있는 enum때문에 include. enum을 글로벌하게 저장하는 곳을 없을까?
//#include "B2ItemInfo.h"
//#include "B2PCClassInfo.h"
//#include "BladeIIGameMode.h"

UB2UIDocManager* UB2UIDocManager::Instance = nullptr;


UB2UIDocManager::~UB2UIDocManager()
{
}

void UB2UIDocManager::Init(ABladeIIGameMode* InGM)
{
////	// 여기로 넘어오는 GameMode 는 Init 단계에서의 참고를 위함. 그리고 매 GameMode 마다 Init 이 불린다는 것을 암시할 수도
////	// UIDocManager 가 static 인 한, 이 GameMode 를 캐싱해서 사용하지 말 것. 단지 여기서만..
////	BII_CHECK(InGM);
////
////	if (bInit)
////	{
////		InitResourcesOnNecessaryHeroDoc(InGM);
////	}
////	else
////	{
////		/*
////#if WITH_EDITOR
////		if (!FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) //중첩 바인딩 안되게
////			FGameDelegates::Get().GetEndPlayMapDelegate().AddRaw(this, &UB2UIDocManager::Destroy);
////#endif
////		*/
////		UB2UIDocHelper::CreateDocSome(); // 애매한 것들 넣을 썸닥썸닥
////
////		//플레이어독 생성(골드같은 글로벌한 데이터, Single)
////		UB2UIDocHelper::CreateDocUser();
////
////		//UI의 자질구레한 상태들을 저장 할 도큐먼트 생성(Single)
////		UB2UIDocHelper::CreateDocUICondition();
////
////		//4명의 히어로 도큐먼트 생성
////		DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Gladiator));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Assassin));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Wizard));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Fighter));
////		// 생성은 모두 하고 PCClassInfo 로딩이 필요한 작업은 필요한 애들만 모아서 따로 함.
////		InitResourcesOnNecessaryHeroDoc(InGM);
////
////		//배틀독 생성(Single)
////		UB2UIDocHelper::CreateDocBattle();
////
////		//배틀스테이지(Single)
////		UB2UIDocHelper::CreateDocBattleStage();
////
////		//시나리오 다이알로그(Single)
////		UB2UIDocHelper::CreateDocScenarioDialog();
////
////		//챕터(Single)
////		UB2UIDocHelper::CreateDocChapter();
////
////		//모드 공용(1대1대전, 팀매치, 점령전, 반격던전, 영웅의 탑, 레이드)
////		UB2UIDocHelper::CreateDocMode();
////
////		//1대1대전
////		UB2UIDocHelper::CreateDocPVP1on1Rival();
////
////		//팀매치
////		UB2UIDocHelper::CreateDocTeamMatch();
////
////		//반격던전
////		UB2UIDocHelper::CreateDocCounterAttack();
////
////		//레이드
////		UB2UIDocHelper::CreateDocRaid();
////
////		//영웅의 탑
////		UB2UIDocHelper::CreateDocHeroTower();
////
////		//길드
////		UB2UIDocHelper::CreateDocGuild();
////
////		//상점
////		UB2UIDocHelper::CreateDocStore();
////
////		//점령전
////		UB2UIDocHelper::CreateDocControl();
////
////		//로비매칭
////		UB2UIDocHelper::CreateDocLobbyMatch();
////
////		//친구
////		UB2UIDocHelper::CreateDocFriend();
////
////		//기부이벤트
////		UB2UIDocHelper::CreateDocDonation();
////
////		//차원의균열
////		UB2UIDocHelper::CreateDocDimension();
////
////		bInit = true;
////	}	
}

void UB2UIDocManager::InitResourcesOnNecessaryHeroDoc(class ABladeIIGameMode* InGM)
{
//	BII_CHECK(InGM);
//	// 아래 InitResources 에서 PCClassInfo 를 로딩하게 된다면 필요한 애들만 모아서 HeroDoc 을 생성할 필요가 있다. 적어도 PCClassInGameOnlyInfo 는 로딩 안하면 괜찮을 듯.
//	// 만일 필요한 애들만 긁어모아야 할 경우 GetPCClassesToPreLoad 는 좀 빠지는 클래스가 있을 가능성이 있어서 여기에 쓰면 안됨.
//	{		
//		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//		{
//			UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
//			if (DocHero)
//			{
//				DocHero->InitResources();
//			}
//		}
//	}
}

UB2UIDocManager* UB2UIDocManager::GetInstance()
{
	if (Instance == nullptr)
	{
		UPackage* NewObjOuter = GetTransientPackage();
		if (NewObjOuter)
		{
			Instance = NewObject<UB2UIDocManager>(NewObjOuter, UB2UIDocManager::StaticClass());
			if (Instance){
				Instance->AddToRoot();
			}
		}
	}	
	// check(Instance); GIsRequestingExit 동안에는 GetTransientPackage 가 없어서 Instance 가 NULL 일 수 있다.
	return Instance;
}

void UB2UIDocManager::Shutdown()
{
	for (auto each : SingleDoc)
	{
		if (each.Value)
		{
			each.Value->Destroy();
			each.Value = nullptr;
		}
	}

	for (auto each : MultipleDocs)
	{
		if (each.Value)
		{
			each.Value->Destroy();
			each.Value = nullptr;
		}
	}

	SingleDoc.Empty();
	MultipleDocs.Empty();

	if (Instance)
	{
		Instance->RemoveFromRoot();
		Instance = nullptr;
	}

	CollectGarbage(RF_NoFlags);

	bInit = false;
}

void UB2UIDocManager::Tick(float DeltaTime)
{
	//나중에 딜레이들 줘서 틱시킬것(최적화)

}

bool UB2UIDocManager::IsTickable() const
{
	return true;
}

TStatId UB2UIDocManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UB2UIDocManager, STATGROUP_Tickables);
}

bool UB2UIDocManager::IsTickableWhenPaused() const
{
	return true;
}

bool UB2UIDocManager::IsTickableInEditor() const
{
	return true;
}

bool UB2UIDocManager::IsExistSingleDoc(EUIDocType Type)
{
	for (auto each : SingleDoc)
	{
		if (each.Value->DocType == Type)
			return false;
	}

	return true;
}
