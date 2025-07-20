#include "B2UITeamMatchMain.h"
#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "B2UIWidget.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIDocHelper.h"
#include "B2UIPVPEntryHero.h"
#include "B2UIModWeekRewardPopup.h"
#include "B2UIChapterStarRewardBox.h"
#include "B2UIRewardEventSet.h"
#include "B2UIModWeeklyReward.h"
#include "B2UIMsgPopupReward.h"
#include "B2ItemInfo.h"
#include "B2UIStartButton.h"
#include "Retailer.h"
#include "B2UIModRewardBox.h"
#include "B2UIMsgPopupBoxRewardConfirm.h"
#include "B2UICommonTab.h"
#include "TutorialManager.h"

#include "BladeIIGameImpl.h"

void UB2UITeamMatchMain::Init()
{
	Super::Init();

	ShowRankerList(true);

	SetEntryInfo();

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::TeamMatchUI);
	data_trader::Retailer::GetInstance().RequestCheckChargePoints(false, false, true);
}

void UB2UITeamMatchMain::BindDocAuto()
{
	UnbindDoc();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_TeamMatch"));

	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (!TeamMatchDoc)
		return;

	SetDoc(TeamMatchDoc);

	if (UIP_MyPVPGrade.IsValid())
	{
		UIP_MyPVPGrade->SetGradeRank(TeamMatchDoc->GetPCGrade(), TeamMatchDoc->GetPCGradeStar());
	}

	TeamMatchDoc->OnPCGradeChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedGrade);
	TeamMatchDoc->OnPCGradeStarChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedGrade);
	TeamMatchDoc->OnPCRankChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedRank);
	TeamMatchDoc->OnRewardBoxStatusChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedRewardBox);
	TeamMatchDoc->RankerInfoContainer.OnRankingListChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedRankingList);

	if (TB_MyRanking.IsValid())
		TB_MyRanking->SetText(TeamMatchDoc->GetPCRank() != 0 ? FText::AsNumber(TeamMatchDoc->GetPCRank()) : FText::FromString(TEXT("-")));
	if (TB_RankPercentage.IsValid())
		TB_RankPercentage->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_RankPercentage")), FText::AsNumber(TeamMatchDoc->GetPCRankPercentage(), UB2UIManager::GetInstance()->GetNumberFormattingOption())));

	if (TB_WinMaxNumber.IsValid())
		TB_WinMaxNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(TeamMatchDoc->GetPCCurrTopContinuousWins())));

	if (TB_CurrentWinNumber.IsValid())
		TB_CurrentWinNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(TeamMatchDoc->GetPCCurrContinuousWins())));
}

void UB2UITeamMatchMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 客 鞍捞 Scene 栏肺 备己窍绰 版快 Init 矫痢俊 窍搁 Scene 俊 器窃等 header 啊 积己捞 救等 惑炔老 荐 乐澜.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_TeamMatch"));
	TAB_AllRank->ForcedClickTab();
}

void UB2UITeamMatchMain::UnbindDoc()
{
	auto* TeamMatchDoc = Cast<UB2UIDocTeamMatch>(GetDoc());
	if (TeamMatchDoc)
	{
		TeamMatchDoc->OnPCGradeChanged.RemoveAll(this);
		TeamMatchDoc->OnPCGradeStarChanged.RemoveAll(this);
		TeamMatchDoc->OnPCRankChanged.RemoveAll(this);
		TeamMatchDoc->OnRewardBoxStatusChanged.RemoveAll(this);
		TeamMatchDoc->RankerInfoContainer.OnRankingListChanged.RemoveAll(this);
	}

	SetDoc(nullptr);
}

void UB2UITeamMatchMain::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (VB_RankerList.IsValid())
	{
		for (auto& RankerInfo : RankerList)
		{
			if (RankerInfo.IsValid())
				RankerInfo->DestroySelf(InUIManager);
		}

		VB_RankerList->ClearChildren();

		RankerList.Empty();
	}
}

void UB2UITeamMatchMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen();

	SetModRewardBox();
	ShowWeekReward();
	SubscribeEvents();
	SetRewardList();
}

void UB2UITeamMatchMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose();

	UnsubscribeEvents();
}

void UB2UITeamMatchMain::CacheAssets()
{
	GET_SLOT(UVerticalBox, VB_RankerList);

	GET_SLOT(UTextBlock, TB_CombatPower);
	
	GET_SLOT(UB2UIGradeRankAndStar, UIP_MyPVPGrade);
	if (UIP_MyPVPGrade.IsValid())
		UIP_MyPVPGrade->Init();

	GET_SLOT(UTextBlock, TB_MyRanking);
	GET_SLOT(UTextBlock, TB_RankPercentage);

	GET_SLOT(UB2UIChapterStarRewardBox, UIP_TeamWeekRewardBox);

	if (UIP_TeamWeekRewardBox.IsValid())
		UIP_TeamWeekRewardBox->Init();

	GET_SLOT(UTextBlock, TB_Weekly);

	GET_SLOT(UTextBlock, TB_EntenHero);

	GET_SLOT(UHorizontalBox, List_Reward);

	GET_SLOT(UB2UIStartButton, UIP_StartBT);
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->StartButtonDelegateBindLambda([this]() { OnClickStartMatch(); });
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatchBattleReady")));
	}

	GET_SLOT(UB2UIModRewardBox, UIP_ModReward);
	if(UIP_ModReward.IsValid())
		UIP_ModReward->Init();

	GET_SLOT(UTextBlock, TB_WinMax);
	GET_SLOT(UTextBlock, TB_WinMaxNumber);
	GET_SLOT(UTextBlock, TB_CurrentWin);
	GET_SLOT(UTextBlock, TB_CurrentWinNumber);

	GET_SLOT(UB2UICommonTab, TAB_AllRank);
	GET_SLOT(UB2UICommonTab, TAB_FriendRank);
}

void UB2UITeamMatchMain::BindDelegates()
{
	REGISTER_UITAB_BINDING(TAB_AllRank.Get(), &UB2UITeamMatchMain::OnClickAllRanker);
	REGISTER_UITAB_BINDING(TAB_FriendRank.Get(), &UB2UITeamMatchMain::OnClickFriendRanker);

	if (UIP_TeamWeekRewardBox.IsValid())
		UIP_TeamWeekRewardBox->RewardListDelgate.BindDynamic(this, &UB2UITeamMatchMain::OnClickBtnWeeklyReward);

	auto TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	TeamDoc->OnFormationAttLevelChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedFormationLevel);
	TeamDoc->OnFormationDefLevelChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedFormationLevel);
	TeamDoc->OnFormationNormalLevelChanged.AddUObject(this, &UB2UITeamMatchMain::OnChangedFormationLevel);

}

void UB2UITeamMatchMain::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2UITeamMatchMain);

	DeliveryOpenDuelRewardBoxTicket = DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryOpenDuelRewardBox, FB2OpenDuelRewardBoxPtr RewardInfo)
			UB2UIDocTeamMatch*	TeamDoc = UB2UIDocHelper::GetDocTeamMatch();
			if (!TeamDoc) 
				return;
			TeamDoc->SetRewardBoxStatus(RewardInfo->reward_box_status);
		END_CAPTURE_OBJECT()
	);

	DeliveryGetFriendRankingTicket = DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetFriendRanking, const FB2ResponseGetFriendRankingsPtr& FriendRank)
			Capture->SetFriendList(FriendRank);
		END_CAPTURE_OBJECT()
	);
}

void UB2UITeamMatchMain::UnsubscribeEvents()
{
	DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Unsubscribe(DeliveryOpenDuelRewardBoxTicket);
	DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Unsubscribe(DeliveryGetFriendRankingTicket);
}

void UB2UITeamMatchMain::SetEntryInfo()
{
	auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamDoc)
		return;

	int32 CombatPoint = 0;

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		auto* HeroDoc = UB2UIDocHelper::GetDocHero(i);

		if (!HeroDoc)
			break;

		if (i == PCClassToInt(TeamDoc->GetPCCharacterClass1()) ||
			i == PCClassToInt(TeamDoc->GetPCCharacterClass2()) ||
			i == PCClassToInt(TeamDoc->GetPCCharacterClass3()))
		{
			CombatPoint += HeroDoc->GetCombatPower();
		}
	}

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(CombatPoint));
}

void UB2UITeamMatchMain::UpdateStaticText()
{
	if (TB_Weekly.IsValid())
		TB_Weekly->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_Organizingparticipationrewards")));

	if (TB_WinMax.IsValid())
		TB_WinMax->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_TopContinuedWins")));

	if (TB_CurrentWin.IsValid())
		TB_CurrentWin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_WinningStreak")));
}

void UB2UITeamMatchMain::OnChangedFormationLevel(UB2UIDocBase* Doc, int32 CurrLv, int32 PreLv)
{
	SetEntryInfo();
}

void UB2UITeamMatchMain::OnChangedGrade(UB2UIDocBase* Doc, int32 CurrGrade, int32 PrevGrade)
{
	if (UIP_MyPVPGrade.IsValid())
	{
		auto* TeamMatchDoc = Cast<UB2UIDocTeamMatch>(GetDoc());
		if (TeamMatchDoc)
		{
			UIP_MyPVPGrade->SetGradeRank(TeamMatchDoc->GetPCGrade(), TeamMatchDoc->GetPCGradeStar());
		}
	}	
}

void UB2UITeamMatchMain::OnChangedRank(UB2UIDocBase* Doc, int32 CurrRank, int32 PrevRank)
{
	auto* TeamMatchDoc = Cast<UB2UIDocTeamMatch>(GetDoc());
	if (TeamMatchDoc)
	{
		if (TB_MyRanking.IsValid())
			TB_MyRanking->SetText(TeamMatchDoc->GetPCRank() != 0 ? FText::AsNumber(TeamMatchDoc->GetPCRank()) : FText::FromString(TEXT("-")));
		if (TB_RankPercentage.IsValid())
			TB_RankPercentage->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_RankPercentage")), FText::AsNumber(TeamMatchDoc->GetPCRankPercentage(), UB2UIManager::GetInstance()->GetNumberFormattingOption())));
	}
}

void UB2UITeamMatchMain::OnClickStartMatch()
{
	// 痢八 沥魂 眉农
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPTeam) && CheckContentsModeState(b2network::B2ContentsMode::PVP_TEAM))
		return;

	// 惑磊 焊惑 or Mvp焊惑 眉农
	UB2UIDocTeamMatch* pDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!pDoc)
		return;

	if (pDoc->GetRewardBoxStatus()
		&& pDoc->GetRewardBoxStatus()->is_reward_state
		&& pDoc->GetCanViewModRewardBoxConfirm()/*茄锅父 焊扁 敲贰弊*/)
	{
		// 扑诀芒 剁况林绊 茄锅父 焊扁 敲贰弊 坷橇
		UB2UIMsgPopupBoxRewardConfirm* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupBoxRewardConfirm>(UIFName::MsgPopupBoxRewardConfirm);

		if (pUI)
		{
			pUI->ApplyRewardBox(pDoc->GetRewardBoxStatus()->box_grade);
			pUI->SetAutoCloseOnClick(true);
			pUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]()
			{
				OnClickStartMatch();
			}
			)
			);
		}


		pDoc->SetCanViewModRewardBoxConfirm(false);
		return;
	}

	EnterTMHeroChangeMenuClass<>::GetInstance().Signal();
}

void UB2UITeamMatchMain::OnClickAllRanker()
{
	ShowRankerList(true);
}

void UB2UITeamMatchMain::OnClickFriendRanker()
{
	ShowRankerList(false);
}

void UB2UITeamMatchMain::ShowRankerList(bool bShowAllRanker)
{
	if (bShowingAllRanker == bShowAllRanker)
		return;

	bShowingAllRanker = bShowAllRanker;

	bShowAllRanker ? UpdateList() : data_trader::Retailer::GetInstance().RequestGetFriendRankings(b2network::B2MatchInfoType::BMIT_TEAM);
}

void UB2UITeamMatchMain::SetModRewardBox()
{
	auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamDoc)
		return;

	auto RewardBoxInfo = TeamDoc->GetRewardBoxStatus();

	if (UIP_ModReward.IsValid() && RewardBoxInfo)
		UIP_ModReward->SetModRewardBox(RewardBoxInfo->is_reward_state, b2network::B2DuelMode::DUEL_MODE_TEAM, RewardBoxInfo->box_grade
			, RewardBoxInfo->win_count, RewardBoxInfo->upgrade_win_count, RewardBoxInfo->defeat_count);
}

void UB2UITeamMatchMain::SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank)
{
	TArray<FModRankerInfo>& ToSet = FriendRankingList;

	//if (!ToSet.IsValidIndex(0)) { VB_RankerList->ClearChildren(); return; }

	ToSet.Empty();
	int32 BeginRank = 0;
	for (auto RankerSvrInfo : FriendRank->rankers)
	{
		FModRankerInfo RankerCliInfo;
		RankerCliInfo.UserName = RankerSvrInfo->nickname;
		RankerCliInfo.Grade = RankerSvrInfo->battle_grade;
		RankerCliInfo.GradeStar = RankerSvrInfo->battle_star_count;
		RankerCliInfo.Rank = ++BeginRank;
		RankerCliInfo.TotalCombatPower = RankerSvrInfo->total_power;			// jwyim power
		RankerCliInfo.AccountID = RankerSvrInfo->account_id;

		for (auto EntryInfo : RankerSvrInfo->entries)
		{
			RankerCliInfo.Entry.Add(SvrToCliPCClassType(EntryInfo.character_type));
			RankerCliInfo.EntryLevels.Add(EntryInfo.level);
		}

		ToSet.Add(RankerCliInfo);
	}

	UpdateList();
}

void UB2UITeamMatchMain::SetRewardList()
{
	if (List_Reward.IsValid())
	{
		List_Reward->ClearChildren();
		auto Rewards = BladeIIGameImpl::GetClientDataStore().GetDuelModeRewardData(b2network::B2DuelMode::DUEL_MODE_TEAM);

		for (auto RewardItem : Rewards)
		{
			auto* CreatedWidget = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardInfoClass);
			if (CreatedWidget != NULL)
			{
				CreatedWidget->Init();
				CreatedWidget->UpdateRewardIcon(RewardItem);
				List_Reward->AddChild(CreatedWidget);
			}
		}
	}
}

void UB2UITeamMatchMain::UpdateList()
{
	//Todo 模备徘牢瘤 酒囱瘤
	if (VB_RankerList.IsValid())
	{
		VB_RankerList->ClearChildren();
		const TArray<FModRankerInfo>* ModRankerList;

		if (bShowingAllRanker)
		{
			auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

			if (!TeamDoc) return;

			ModRankerList = TeamDoc->RankerInfoContainer.GetRankerList();

			if (!ModRankerList)
				return;
		}
		else
		{
			ModRankerList = &FriendRankingList;
			if (!ModRankerList)
				return;
		}

		for (auto Ranker : *ModRankerList)
		{
			auto* CreatedWidget = CreateWidget<UB2UITeamRankerInfo>(GetOwningPlayer(), RankerInfoClass);
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_RankerList->AddChild(CreatedWidget));
				check(AddedSlot);
				RankerList.Add(CreatedWidget);
				CreatedWidget->Init();
				CreatedWidget->SetPadding(FMargin(0, 0, 0, 10.0));
				CreatedWidget->SetRankerInfo(Ranker, true);
			}
		}
	}
}

void UB2UITeamMatchMain::ShowWeekReward()
{
	auto TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamDoc)
		return;

	int32 BladePoint = 0;
	int32 Gem = 0;
	int32 Gold = 0;
	int32 ItemId = 0;
	int32 DailyRewardCount = 0;

	const TArray<FModDailyReward>& DailyReward = BladeIIGameImpl::GetClientDataStore().GetModDailyRewardList();


	for (auto& Ele : DailyReward)
	{
		if (Ele.Match_Type != b2network::B2MatchInfoType::BMIT_TEAM)
			continue;

		BladePoint = Ele.Reward_BladePoint;
		Gem = Ele.Reward_Gem;
		Gold = Ele.Reward_Gold;
		ItemId = Ele.Reward_ItemId;

	}

	if (TeamDoc->GetTeamTodayReward())
		++DailyRewardCount;
	if (TeamDoc->GetTeamLastReward())
		++DailyRewardCount;


	if (TeamDoc->GetIsTeamReward())
	{
		auto RewardUI = UB2UIManager::GetInstance()->OpenUI<UB2UIModWeekRewardPopup>(UIFName::ModMatchReward);
		RewardUI->OpenReward(UIFName::TeamMatchUI);
		TeamDoc->SetIsTeamReward(false);
	}
	else if (DailyRewardCount)
	{
		UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		if (RewardPopup != nullptr)
		{
			RewardPopup->AddSingleRewardItem(ERewardType::BladePoint, BladePoint);
			RewardPopup->AddSingleRewardItem(ERewardType::Gem, Gem);
			RewardPopup->AddSingleRewardItem(ERewardType::Gold, Gold);
			RewardPopup->AddSingleRewardItem(ERewardType::Item, ItemId);

			RewardPopup->ShowRewardPopup();
		}

		TeamDoc->SetTeamLastReward(false);
		TeamDoc->SetTeamTodayReward(false);
	}

}

void UB2UITeamMatchMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamDoc)
		return;

	UIP_TeamWeekRewardBox->SetRemainTime(FDateTime::UtcNow().ToUnixTimestamp(), (TeamDoc->GetTeamRewardTime() / 1000));
}

void UB2UITeamMatchMain::OnClickBtnWeeklyReward()
{
	auto* RewardList = UB2UIManager::GetInstance()->OpenUI<UB2UIModWeeklyReward>(UIFName::ModMatchRewardList_Week);

	if ( RewardList != nullptr )
		RewardList->SetOpenRewardListType(UIFName::TeamMatchUI);
}

void UB2UITeamMatchMain::OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2)
{
	SetModRewardBox();
}

void UB2UITeamMatchMain::OnChangedRankingList(const TArray<FModRankerInfo>& RankerInfos)
{
	UpdateList();
}
