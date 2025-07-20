// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIEnterBattleMain.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIEnterDungeonSlot.h"
#include "B2RedDotManager.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"

#include "TutorialManager.h"

#include "BladeIIGameImpl.h"

#define Dungeon_SLOTSETTING(Name)																\
	if(Name.IsValid()){																			\
	Name->Init();																				\
	Name->FOnClickDelegate.BindUObject(this, &UB2UIEnterBattleMain::OnClick_SlotButton);		\
	}

void UB2UIEnterBattleMain::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_PVPMatch);
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_TeamMatch);
	GET_SLOT(UB2UIEnterDungeonSlot, UIP_Control);

	Dungeon_SLOTSETTING(UIP_PVPMatch);
	Dungeon_SLOTSETTING(UIP_TeamMatch);
	Dungeon_SLOTSETTING(UIP_Control);
}

void UB2UIEnterBattleMain::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIEnterBattleMain::Init()
{
	Super::Init();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Battle"));

	CAPTURE_UOBJECT(UB2UIEnterBattleMain);
	DeliveryGetMatchStatusTicket = DeliveryGetMatchStatusClass<FB2TagMatchStatus>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetMatchStatus, const FB2TagMatchStatus& msg)
		Capture->ResponseGetMatchStatus(msg);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetTeamBattleStatusTicket = DeliveryGetTeamBattleStatusClass<FB2TeamBattleStatus>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetTeamBattleStatus, const FB2TeamBattleStatus& msg)
		Capture->ResponseGetTeamBattleStatus(msg);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetAssaultBattleStatusTicket = DeliveryGetAssaultBattleStatusClass<FB2ResponseGetAssaultBattleStatusPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetAssaultBattleStatus, const FB2ResponseGetAssaultBattleStatusPtr& msgPtr)
		Capture->ResponseGetAssaultBattleStatus(msgPtr);
	END_CAPTURE_OBJECT()
		);

	DeliveryCheckRedDotTicket = DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCheckRedDot, const FB2MessageInfoResponseCheckRedDotPtr& msgPtr)
		Capture->DoMarkRedDot();
	END_CAPTURE_OBJECT()
		); 	
}

void UB2UIEnterBattleMain::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	DeliveryGetMatchStatusClass<FB2TagMatchStatus>::GetInstance().Unsubscribe(DeliveryGetMatchStatusTicket);
	DeliveryGetTeamBattleStatusClass<FB2TeamBattleStatus>::GetInstance().Unsubscribe(DeliveryGetTeamBattleStatusTicket);
	DeliveryGetAssaultBattleStatusClass<FB2ResponseGetAssaultBattleStatusPtr>::GetInstance().Unsubscribe(DeliveryGetAssaultBattleStatusTicket);
	DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Unsubscribe(DeliveryCheckRedDotTicket);
}

void UB2UIEnterBattleMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_PVPMatch.Get()), &UB2UIEnterBattleMain::RedDotCondition_PVPMatch);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_TeamMatch.Get()), &UB2UIEnterBattleMain::RedDotCondition_TeamMatch);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UIP_Control.Get()), &UB2UIEnterBattleMain::RedDotCondition_Control);
}

void UB2UIEnterBattleMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_PVP });
}

void UB2UIEnterBattleMain::ResponseGetMatchStatus(const FB2TagMatchStatus& msg)
{
	LobbyEnterPVPMainMenuClass<>::GetInstance().Signal();
}

void UB2UIEnterBattleMain::ResponseGetTeamBattleStatus(const FB2TeamBattleStatus& msg)
{
	LobbyEnterTeamMatchMenuClass<>::GetInstance().Signal();
}

void UB2UIEnterBattleMain::ResponseGetAssaultBattleStatus(const FB2ResponseGetAssaultBattleStatusPtr& msgPtr)
{
	LobbyEnterControlMenuClass<>::GetInstance().Signal();
}

bool UB2UIEnterBattleMain::RedDotCondition_PVPMatch()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Battle_PvP();
}

bool UB2UIEnterBattleMain::RedDotCondition_TeamMatch()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Battle_Team();
}

bool UB2UIEnterBattleMain::RedDotCondition_Control()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Battle_Control();
}

void UB2UIEnterBattleMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Battle"));

	UpdateBattleSlot();
}

void UB2UIEnterBattleMain::OnClick_SlotButton(ELobbyModSceneElem eGameMode)
{
	switch (eGameMode)
	{
	case ELobbyModSceneElem::LMODIDX_PVP:
		data_trader::Retailer::GetInstance().RequestGetMatchStatus();
		break;
	case ELobbyModSceneElem::LMODIDX_TeamMatch:
		data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
		break;
	case ELobbyModSceneElem::LMODIDX_Control:
		data_trader::Retailer::GetInstance().RequestAssaultBattleStatus();		
		break;
	}
}

void UB2UIEnterBattleMain::UpdateBattleSlot()
{
	auto DocMode = UB2UIDocHelper::GetDocMode();

	if (DocMode)
	{
		auto DocPvp = UB2UIDocHelper::GetDocPVP1on1Rival();
		if (DocPvp && UIP_PVPMatch.IsValid())
		{
			UIP_PVPMatch->SetCountInfo(DocPvp->GetMatchPoint(), BladeIIGameImpl::GetClientDataStore().GetMaxPvpMatchPoint());

			// 痢八 沥魂 喉废UI 秦寸葛靛 譬配府倔 场陈阑锭父 钎矫
			if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPOneAndOne))
				UIP_PVPMatch->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_DUEL));
			else
				UIP_PVPMatch->SetModeState(b2network::B2ContentsModeState::RUN);
		}

		auto DocTeam = UB2UIDocHelper::GetDocTeamMatch();
		if (DocTeam && UIP_TeamMatch.IsValid())
		{
			UIP_TeamMatch->SetCountInfo(DocTeam->GetTeamMatchPoint(), BladeIIGameImpl::GetClientDataStore().GetMaxTeamMatchPoint());

			// 痢八 沥魂 喉废UI 秦寸葛靛 譬配府倔 场陈阑锭父 钎矫
			if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPTeam))
				UIP_TeamMatch->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_TEAM));
			else
				UIP_TeamMatch->SetModeState(b2network::B2ContentsModeState::RUN);
		}

		auto DocControl = UB2UIDocHelper::GetDocControl();
		if (DocControl && UIP_Control.IsValid())
		{
			UIP_Control->SetCountInfo(0, 0);

			// 痢八 沥魂 喉废UI 秦寸葛靛 譬配府倔 场陈阑锭父 钎矫
			if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_Occupy))
				UIP_Control->SetModeState(DocMode->GetContentsModeState(b2network::B2ContentsMode::ASSAULT));
			else
				UIP_Control->SetModeState(b2network::B2ContentsModeState::RUN);

			UIP_Control->SetModeText(b2network::B2ContentsMode::ASSAULT);

			// 涝厘啊瓷 矫埃措 眉农绰 葛靛 凯妨乐阑锭父茄促.
			if (UIP_Control->IsModeStateCurrentOpen() && !IsActiveControlTutorial())
			{
				FText LockedTimeText = GetLockedTimeText();

				UIP_Control->SetLockTimeInfo(LockedTimeText);
				UIP_Control->SetButtonEnable(LockedTimeText.IsEmpty() || IsActiveControlTutorial());
			}
		}
	}

	UpdateBattleSlot_RewardInfo();
}

void UB2UIEnterBattleMain::UpdateBattleSlot_RewardInfo()
{
	if (UIP_PVPMatch.IsValid())
		UIP_PVPMatch->SetReward(0);
	if (UIP_TeamMatch.IsValid())
		UIP_TeamMatch->SetReward(0);
	if (UIP_Control.IsValid())
		UIP_Control->SetReward(0);
}

FText UB2UIEnterBattleMain::GetLockedTimeText()
{
	TArray<FText> ta_OpenTimes;
	TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
	bool bIsOpen = false;
	if (auto DocControl = UB2UIDocHelper::GetDocControl())
	{
		bIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(DocControl->GetPeriodInfos(), OpenTimes);
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
			return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Close"));
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