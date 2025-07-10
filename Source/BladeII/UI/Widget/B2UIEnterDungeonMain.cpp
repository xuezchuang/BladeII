// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIEnterDungeonMain.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIEnterDungeonSlot.h"

#include "B2RedDotManager.h"
#include "B2UIDocHeroTower.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "TutorialManager.h"
#include "B2StageManager.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

#define Dungeon_SLOTSETTING(Name)																\
	if(Name.IsValid()){																			\
	Name->Init();																				\
	Name->FOnClickDelegate.BindUObject(this, &UB2UIEnterDungeonMain::OnClick_DungeonButton);	\
	}

void UB2UIEnterDungeonMain::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_Counter);
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_HeroTower);
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_Raid);
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_Dimension);

	Dungeon_SLOTSETTING(UIP_Counter);
	Dungeon_SLOTSETTING(UIP_HeroTower);
	Dungeon_SLOTSETTING(UIP_Raid);
	Dungeon_SLOTSETTING(UIP_Dimension);
}

void UB2UIEnterDungeonMain::BindDelegates()
{
	Super::BindDelegates();
}

bool UB2UIEnterDungeonMain::RedDotCounterDungeon()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Dungeon_Counter();
}

bool UB2UIEnterDungeonMain::RedDotHeroTower()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Dungeon_Hero();
}

bool UB2UIEnterDungeonMain::RedDotRaidDungeon()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Dungeon_Raid();
}

bool UB2UIEnterDungeonMain::RedDotDimension()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Dungeon_Dimension();
}

void UB2UIEnterDungeonMain::Init()
{
	Super::Init();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Dungeon"));

	CAPTURE_UOBJECT(UB2UIEnterDungeonMain);

	DeliveryGetHeroTowerTicket = DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetHeroTower, const FB2ResponseGetHeroTowerPtr& msgPtr)
		Capture->ResponseGetHeroTower(msgPtr);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetCounterTicket = DeliveryGetCounterDundeonClass<FB2GetCounterDungeon>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetCounter, const FB2GetCounterDungeon& msg)
		Capture->ResponseGetCounter(msg);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetRaidTicket = DeliveryGetRaidClass<FB2ResponseGetRaidPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetRaid, const FB2ResponseGetRaidPtr& msgPtr)
		Capture->ResponseGetRaid(msgPtr);
	END_CAPTURE_OBJECT()
		);	
	
	DeliveryGetDimensionTicket = DeliveryGetDimensionClass<FB2ResponseGetDimensionPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetDimension, const FB2ResponseGetDimensionPtr& msgPtr)
		Capture->ResponseGetDimension(msgPtr);
	END_CAPTURE_OBJECT()
		);

	DeliveryCheckRedDotTicket = DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCheckRedDot, const FB2MessageInfoResponseCheckRedDotPtr& msgPtr)
		Capture->DoMarkRedDot();
	END_CAPTURE_OBJECT()
		);
}

void UB2UIEnterDungeonMain::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Unsubscribe(DeliveryGetHeroTowerTicket);
	DeliveryGetCounterDundeonClass<FB2GetCounterDungeon>::GetInstance().Unsubscribe(DeliveryGetCounterTicket);
	DeliveryGetRaidClass<FB2ResponseGetRaidPtr>::GetInstance().Unsubscribe(DeliveryGetRaidTicket);
	DeliveryGetDimensionClass<FB2ResponseGetDimensionPtr>::GetInstance().Unsubscribe(DeliveryGetDimensionTicket);
	DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Unsubscribe(DeliveryCheckRedDotTicket);
}

void UB2UIEnterDungeonMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_Counter.Get()), &UB2UIEnterDungeonMain::RedDotCounterDungeon);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_HeroTower.Get()), &UB2UIEnterDungeonMain::RedDotHeroTower);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_Raid.Get()), &UB2UIEnterDungeonMain::RedDotRaidDungeon);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_Dimension.Get()), &UB2UIEnterDungeonMain::RedDotDimension);
}

void UB2UIEnterDungeonMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_DUNGEON });
}

void UB2UIEnterDungeonMain::ResponseGetHeroTower(const FB2ResponseGetHeroTowerPtr& msgPtr)
{
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroTowerReady);
}

void UB2UIEnterDungeonMain::ResponseGetCounter(const FB2GetCounterDungeon& msg)
{
	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::CounterAttackDGMain);
}

void UB2UIEnterDungeonMain::ResponseGetRaid(const FB2ResponseGetRaidPtr& msgPtr)
{
	/*auto* UIDocRaid = Cast<UB2UIDocRaid>(GetDoc());
	BII_CHECK(UIDocRaid);
	if (UIDocRaid == nullptr)
		return;*/

	//UIDocRaid->SetLastRaidType(1);

	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_RaidMain);
}

void UB2UIEnterDungeonMain::ResponseGetDimension(const FB2ResponseGetDimensionPtr& msgPtr)
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (UIDocDimension)
		UIDocDimension->SetDimensionPtr(msgPtr);

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::DimensionMain);
}

void UB2UIEnterDungeonMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Dungeon"));
	
	UpdateDungeonSlot();
}

void UB2UIEnterDungeonMain::OnClick_DungeonButton(ELobbyModSceneElem eGameMode)
{
	switch (eGameMode)
	{
	case ELobbyModSceneElem::LMODIDX_HeroTop:
		data_trader::Retailer::GetInstance().RequestGetHeroTower();
		break;
	case ELobbyModSceneElem::LMODIDX_CounterDungeon:
		data_trader::Retailer::GetInstance().RequestGetCounterDungeon();		
		break;
	case ELobbyModSceneElem::LMODIDX_Raid:
		data_trader::Retailer::GetInstance().RequestGetRaid();
		break;
	case ELobbyModSceneElem::LMODIDX_Dimension:
		data_trader::Retailer::GetInstance().RequestGetDimension();
		break;
	}
}

void UB2UIEnterDungeonMain::UpdateDungeonSlot()
{
	auto DocMode = UB2UIDocHelper::GetDocMode();
	if (DocMode)
	{
		TutorialManager& TutorialMgr = TutorialManager::GetInstance();

		auto DocCounter = UB2UIDocHelper::GetDocCounterAttack();
		if (DocCounter && UIP_Counter.IsValid())
		{
			UIP_Counter->SetCountInfo(DocCounter->GetLeftTicket(), BladeIIGameImpl::GetClientDataStore().GetMaxCounterDungeonTicketCount());

			// 점검 정산 블록UI 해당모드 튜토리얼 끝냈을때만 표시
			if (TutorialMgr.IsFinishTutorial(TutorialID_CounterDungeon))
				UIP_Counter->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::COUNTER));
			else
				UIP_Counter->SetModeState(b2network::B2ContentsModeState::RUN);
		}

		auto DocHerotower = UB2UIDocHelper::GetDocHeroTower();
		if (DocHerotower && UIP_HeroTower.IsValid())
		{
			UIP_HeroTower->SetCountInfo(DocHerotower->GetDailyLeftFreeTryCount(), BladeIIGameImpl::GetClientDataStore().GetMaxHeroTowerTicketCount());

			// 점검 정산 블록UI 해당모드 튜토리얼 끝냈을때만 표시
			if (TutorialMgr.IsFinishTutorial(TutorialID_HeroTower))
				UIP_HeroTower->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::HERO_TOWER));
			else
				UIP_HeroTower->SetModeState(b2network::B2ContentsModeState::RUN);
		}

		auto DocRaid = UB2UIDocHelper::GetDocRaid();
		if (DocRaid && UIP_Raid.IsValid())
		{
			UIP_Raid->SetCountInfo(0, 0);

			// 점검 정산 블록UI 해당모드 튜토리얼 끝냈을때만 표시
			if (TutorialMgr.IsFinishTutorial(TutorialID_Raid))
				UIP_Raid->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::RAID));
			else
				UIP_Raid->SetModeState(b2network::B2ContentsModeState::RUN);

			// 입장가능 시간대 체크는 모드 열려있을때만한다.
			if (UIP_Raid->IsModeStateCurrentOpen())
			{
				//현재 스텝 진행 중인 튜토리얼이 레이드일 경우 무조건 입장 가능
				if (TutorialMgr.IsCurrentTutorial(TutorialID_Raid))
				{
					UIP_Raid->SetButtonEnable(true);
				}
				else
				{
					FText LockedTimeText = GetLockedTimeText();
					UIP_Raid->SetLockTimeInfo(LockedTimeText);
					UIP_Raid->SetButtonEnable(LockedTimeText.IsEmpty());

				}
			}
		}

		auto DocDimension = UB2UIDocHelper::GetDocDimension();
		if (DocDimension && UIP_Dimension.IsValid())
		{
			int32 MaxCount = DocDimension->GetDailyTryCount();
			int32 PossibleCount = DocDimension->GetDimensionTicket();

			UIP_Dimension->SetCountInfo(PossibleCount, MaxCount);
			
			// 디멘션은 악몽 5단 클리어시 오픈임..	
			if (DocDimension->GetDimensionOpen())
				UIP_Dimension->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::DIMENSION));
			else
			{
				UIP_Dimension->SetModeState(b2network::B2ContentsModeState::NONE);
				UIP_Dimension->SetBlockPage_Dimension();
			}
		}
	}

	UpdateDungeonSlot_RewardInfo();
}

void UB2UIEnterDungeonMain::UpdateDungeonSlot_RewardInfo()
{
	if (UIP_Counter.IsValid())
		UIP_Counter->SetReward(0);
	if (UIP_HeroTower.IsValid())
		UIP_HeroTower->SetReward(0);
	if (UIP_Dimension.IsValid())
		UIP_Dimension->SetReward(0);

	if (UIP_Raid.IsValid())
	{
		if (auto DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			int32 TodayReward = 1;

			TArray<int32> DayOpens;
			TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
			int32 nStartIndex = static_cast<int32>(ERaidBossIndex::BOSS_HARANG);
			int32 nEndInddex = static_cast<int32>(ERaidBossIndex::BOSS_MAX);

			for (int32 i = nStartIndex; i < nEndInddex; i++)
			{
				if (BladeIIGameImpl::GetClientDataStore().GetIsRaidBossOpen(DocRaid->GetPeriodInfos(), i, DayOpens))
				{
					TodayReward = FMath::Max(i - 1, 0);
					break;
				}
			}

			for (int32 i = nStartIndex; i < nEndInddex; i++)
			{
				if (BladeIIGameImpl::GetClientDataStore().GetRaidModeOpenTimes(DocRaid->GetPeriodInfos(), i, OpenTimes))
				{
					TodayReward = FMath::Max(i - 1, 0);
					break;
				}
			}

			UIP_Raid->SetReward(TodayReward);
		}
	}
}

FText UB2UIEnterDungeonMain::GetLockedTimeText()
{
	TArray<FText> ta_OpenTimes;
	TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
	bool bIsOpen = false;
	if (auto DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		bIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(DocRaid->GetPeriodInfos(), OpenTimes);
	}

	if (bIsOpen)
	{
		return  FText::GetEmpty();
	}
	else
	{
		if (OpenTimes.Num() > 0)
		{
			for (auto EachItem : OpenTimes)
			{
				FNumberFormattingOptions Options;
				Options.MinimumIntegralDigits = 2;
				ta_OpenTimes.Add(FText::Format(FText::FromString("{0} : {1} ~ {2} : {3}"),
					FText::AsNumber(EachItem->begin_hour, &Options), FText::AsNumber(EachItem->begin_min, &Options),
					FText::AsNumber(EachItem->end_hour, &Options), FText::AsNumber(EachItem->end_min, &Options)));
			}
		}
		else
		{
			return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_EnterDungeon"));
		}
	}

	FString TimeText = "";

	for (int32 i = 0; i < ta_OpenTimes.Num(); i++)
	{
		TimeText += ta_OpenTimes[i].ToString();
		if (i != (ta_OpenTimes.Num() - 1))
			TimeText += FString("\n");
	}

	return FText::FromString(TimeText);
}
#undef Dungeon_SLOTSETTING