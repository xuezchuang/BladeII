#include "B2UIPVPMain.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIManager.h"
#include "B2UIRankerInfo.h"
#include "B2UIChapterStarRewardBox.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIHeader.h"
#include "Event.h"
#include "B2UIModWeekRewardPopup.h"
//#include "B2UIModDailyReward.h"
#include "B2UIModWeeklyReward.h"
#include "B2UIMsgPopupReward.h"
#include "B2ItemInfo.h"
#include "B2UIStartButton.h"
#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "B2UIPVPCharacterSelectPage.h"
#include "B2UIModRewardBox.h"
#include "B2UIMsgPopupBoxRewardConfirm.h"
#include "B2UICommonTab.h"
#include "B2UIGameInviteList.h"
#include "TutorialManager.h"

#include "BladeIIGameImpl.h"

void UB2UIPVPMain::CacheAssets()
{
	GET_SLOT(UVerticalBox, VB_RankerList);

	GET_SLOT(UB2UIGradeRankAndStar, UIP_MyPVPGrade);
	if (UIP_MyPVPGrade.IsValid())
		UIP_MyPVPGrade->Init();

	//GET_SLOT(UB2UIChapterStarRewardBox, UIP_PVPDayRewardBox);
	//if (UIP_PVPDayRewardBox.IsValid())
		//UIP_PVPDayRewardBox->Init();
	GET_SLOT(UB2UIChapterStarRewardBox, UIP_PVPWeekRewardBox);
	if (UIP_PVPWeekRewardBox.IsValid())
		UIP_PVPWeekRewardBox->Init();
	GET_SLOT(UTextBlock, TB_MyRanking);
	GET_SLOT(UTextBlock, TB_RankPercentage);

	GET_SLOT(UButton, BTN_StartPVPMatch);

	GET_SLOT(UTextBlock, TB_RankUnit);
	GET_SLOT(UTextBlock, TB_Weekly);
	GET_SLOT(UTextBlock, TB_EntenHero);

	GET_SLOT(UHorizontalBox, List_Reward);

	GET_SLOT(UB2UIStartButton, UIP_StartBT);
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->StartButtonDelegateBindLambda([this]() {this->OnClickBtnStartPVPMatch(); });
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatchBattleReady")));
	}

	GET_SLOT(UButton, BTN_BattleFriendly);
	GET_SLOT(UTextBlock, TB_BattleFriendly);


	GET_SLOT(UB2UIModRewardBox, UIP_ModReward)
	if (UIP_ModReward.IsValid())
		UIP_ModReward->Init();

	GET_SLOT(UB2UICommonTab, TAB_AllRank);
	GET_SLOT(UB2UICommonTab, TAB_FriendRank);

	GET_SLOT(UTextBlock, TB_WinMax);
	GET_SLOT(UTextBlock, TB_WinMaxNumber);
	GET_SLOT(UTextBlock, TB_CurrentWin);
	GET_SLOT(UTextBlock, TB_CurrentWinNumber);

	UpdateStaticText();
	
}

void UB2UIPVPMain::BindDelegates()
{
	REGISTER_UITAB_BINDING(TAB_AllRank.Get(), &UB2UIPVPMain::OnClickBtnAllRank);
	REGISTER_UITAB_BINDING(TAB_FriendRank.Get(), &UB2UIPVPMain::OnClickBtnFriendRank);
	BIND_CLICK_FUNC_TO_BTN(BTN_StartPVPMatch, &UB2UIPVPMain::OnClickBtnStartPVPMatch);

	BIND_CLICK_FUNC_TO_BTN(BTN_BattleFriendly, &UB2UIPVPMain::OnClickBtnBattleFriendly);

	/*if (UIP_PVPDayRewardBox.IsValid())
		UIP_PVPDayRewardBox->RewardListDelgate.BindDynamic(this, &UB2UIPVPMain::OnClickBtnDaillyReward);*/
	if (UIP_PVPWeekRewardBox.IsValid())
		UIP_PVPWeekRewardBox->RewardListDelgate.BindDynamic(this, &UB2UIPVPMain::OnClickBtnWeeklyReward);
}

void UB2UIPVPMain::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2UIPVPMain);

	DeliveryOpenDuelRewardBoxTicket = DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryOpenDuelRewardBox, FB2OpenDuelRewardBoxPtr RewardInfo)
		UB2UIDocPVP1on1Rival*	PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();
		if (!PvPDoc) return;
		PvPDoc->SetRewardBoxStatus(RewardInfo->reward_box_status);
		END_CAPTURE_OBJECT()
		);
}

void UB2UIPVPMain::UnsubscribeEvents()
{
	DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Unsubscribe(DeliveryOpenDuelRewardBoxTicket);
}

void UB2UIPVPMain::UpdateStaticText()
{
	if (TB_Weekly.IsValid())
		TB_Weekly->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_Organizingparticipationrewards")));

	if (TB_EntenHero.IsValid())
		TB_EntenHero->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_EnterHero")));

	if (TB_WinMax.IsValid())
		TB_WinMax->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_TopContinuedWins")));

	if (TB_CurrentWin.IsValid())
		TB_CurrentWin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_WinningStreak")));

	if (TB_BattleFriendly.IsValid())
		TB_BattleFriendly->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_FriendshipGameStart")));
}

void UB2UIPVPMain::Init()
{
	Super::Init();

	ShowRankerList(true);

	if (UIP_MyPVPGrade.IsValid())
	{
		UIP_MyPVPGrade->SetGradeRank(8, 2);
	}

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("PVP1on1_TitleName"));

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::PVPOneOnOne);
	data_trader::Retailer::GetInstance().RequestCheckChargePoints(false, true, false);

	TAB_AllRank->ForcedClickTab();
}

void UB2UIPVPMain::DestroySelf(class UB2UIManager* InUIManager)
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

	//UIP_PVPEntries.Empty();
}

void UB2UIPVPMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto* PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	if (!PvPDoc)
		return;

	UIP_PVPWeekRewardBox->SetRemainTime(FDateTime::UtcNow().ToUnixTimestamp(), (PvPDoc->GetTagRewardTime()/1000));

}

void UB2UIPVPMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen();

	DeliveryGetFriendRankingTicket = DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Subscribe([this](const FB2ResponseGetFriendRankingsPtr& FriendRank)
	{
		SetFriendList(FriendRank);
	}
	);
	SetModRewardBox();
	ShowWeekReward();
	SubscribeEvents();
	SetRewardList();

	//SetDayRewardInfo();
}

void UB2UIPVPMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose();
	UnsubscribeEvents();

	DeliveryGetFriendRankingClass<FB2ResponseGetFriendRankingsPtr>::GetInstance().Unsubscribe(DeliveryGetFriendRankingTicket);
}

void UB2UIPVPMain::RequestUpdate()
{
	//서버 왔을때 업데이트 할경우
}

void UB2UIPVPMain::BindDocAuto()
{
	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (DocBattle && DocPVP)
	{
		DocBattle->OnTagMatchRankChanged.AddUObject(this, &UB2UIPVPMain::OnChangedRank);
		DocBattle->OnTagMatchGradeChanged.AddUObject(this, &UB2UIPVPMain::OnChangedGrade);
		DocBattle->OnTagMatchGradeStarChanged.AddUObject(this, &UB2UIPVPMain::OnChangedGrade);
		DocPVP->OnRewardBoxStatusChanged.AddUObject(this, &UB2UIPVPMain::OnChangedRewardBox);
		DocPVP->RankerInfoContainer.OnRankingListChanged.AddUObject(this, &UB2UIPVPMain::OnChangedRankingList);

		if (TB_MyRanking.IsValid())
			TB_MyRanking->SetText(DocBattle->GetTagMatchRank() != 0 ? FText::AsNumber(DocBattle->GetTagMatchRank()) : FText::FromString(TEXT("-")));
		if (TB_RankPercentage.IsValid())
			TB_RankPercentage->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_RankPercentage")), FText::AsNumber(DocBattle->GetTagMatchRankPercentage(), UB2UIManager::GetInstance()->GetNumberFormattingOption())));

		if (UIP_MyPVPGrade.IsValid())
			UIP_MyPVPGrade->SetGradeRank(DocBattle->GetTagMatchGrade(), DocBattle->GetTagMatchGradeStar());

		if (TB_WinMaxNumber.IsValid())
			TB_WinMaxNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL,TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(DocBattle->GetTagMatchCurrTopContinuousWins())));

		if (TB_CurrentWinNumber.IsValid())
			TB_CurrentWinNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")) ,FText::AsNumber(DocBattle->GetTagMatchCurrContinuousWins())));
	}
}

void UB2UIPVPMain::UnbindDoc()
{
	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (DocBattle)
	{
		DocBattle->OnTagMatchRankChanged.RemoveAll(this);
		DocBattle->OnTagMatchGradeChanged.RemoveAll(this);
		DocBattle->OnTagMatchGradeStarChanged.RemoveAll(this);
	}
	if (DocPVP)
	{
		DocPVP->OnRewardBoxStatusChanged.RemoveAll(this);
		DocPVP->RankerInfoContainer.OnRankingListChanged.RemoveAll(this);
	}
	
}

void UB2UIPVPMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("PVP1on1_TitleName"));

}

void UB2UIPVPMain::OnChangedRank(class UB2UIDocBase* Sender, int32 CurRank, int32 PrevRank)
{
	auto* BattleDoc = Cast<UB2UIDocBattle>(Sender);
	if (BattleDoc)
	{
		if (TB_MyRanking.IsValid())
			TB_MyRanking->SetText(BattleDoc->GetTagMatchRank() != 0 ? FText::AsNumber(BattleDoc->GetTagMatchRank()) : FText::FromString(TEXT("-")));
		if (TB_RankPercentage.IsValid())
			TB_RankPercentage->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_RankPercentage")), FText::AsNumber(BattleDoc->GetTagMatchRankPercentage(), UB2UIManager::GetInstance()->GetNumberFormattingOption())));
	}
}

void UB2UIPVPMain::OnChangedGrade(class UB2UIDocBase* Sender, int32 CurGrade, int32 PrevGrade)
{
	auto* BattleDoc = Cast<UB2UIDocBattle>(Sender);
	if (BattleDoc)
	{
		if (UIP_MyPVPGrade.IsValid())
			UIP_MyPVPGrade->SetGradeRank(BattleDoc->GetTagMatchGrade(), BattleDoc->GetTagMatchGradeStar());
	}
}

void UB2UIPVPMain::OnChangedRankingList(const TArray<FModRankerInfo>& RankerInfos)
{
	UpdateList();
}

void UB2UIPVPMain::OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2)
{
	SetModRewardBox();
}

void UB2UIPVPMain::OnClickBtnAllRank()
{
	ShowRankerList(true);
}

void UB2UIPVPMain::OnClickBtnFriendRank()
{
	ShowRankerList(false);
}

void UB2UIPVPMain::OnClickBtnStartPVPMatch()
{
	// 점검 정산 체크
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPOneAndOne) && CheckContentsModeState(b2network::B2ContentsMode::PVP_DUEL))
		return;

	// 상자 보상 or Mvp보상 체크
	UB2UIDocPVP1on1Rival* pDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	if (!pDoc)
		return;

	if (pDoc->GetRewardBoxStatus()
		&& pDoc->GetRewardBoxStatus()->is_reward_state
		&& pDoc->GetCanViewModRewardBoxConfirm()/*한번만 보기 플래그*/)
	{
		// 팝업창 띄워주고 한번만 보기 플래그 오프
		UB2UIMsgPopupBoxRewardConfirm* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupBoxRewardConfirm>(UIFName::MsgPopupBoxRewardConfirm);

		if (pUI)
		{
			pUI->ApplyRewardBox(pDoc->GetRewardBoxStatus()->box_grade);
			pUI->SetAutoCloseOnClick(true);
			pUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]()
			{
				OnClickBtnStartPVPMatch();
			}
			)
			);
		}


		pDoc->SetCanViewModRewardBoxConfirm(false);
		return;
	}

	pDoc->SetIsFriendshipGame(false);
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_PVPCharacterSelectPage);
}


void UB2UIPVPMain::OnClickBtnBattleFriendly()
{
	if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		DocPVP->SetIsFriendshipGame(true);
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_PVPCharacterSelectPage);
	}
}

/*void UB2UIPVPMain::OnClickBtnDaillyReward()
{
	auto* DailyRewardList = UB2UIManager::GetInstance()->OpenUI<UB2UIModDailyReward>(UIFName::ModMatchRewardList_Day);
	if ( DailyRewardList != nullptr )
		DailyRewardList->OpenDailyReward(b2network::B2MatchInfoType::BMIT_PLAYER);
}*/

void UB2UIPVPMain::OnClickBtnWeeklyReward()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIPVPMain::OnClickBtnWeeklyReward"));
	auto* WeeklyRewardList = UB2UIManager::GetInstance()->OpenUI<UB2UIModWeeklyReward>(UIFName::ModMatchRewardList_Week);

	if (WeeklyRewardList != nullptr )
		WeeklyRewardList->SetOpenRewardListType(UIFName::PVPOneOnOne);
}

void UB2UIPVPMain::ShowRankerList(bool bShowAllRanker)
{
	if (bShowingAllRanker == bShowAllRanker)
		return;

	bShowingAllRanker = bShowAllRanker;
	
	bShowAllRanker ? UpdateList() : data_trader::Retailer::GetInstance().RequestGetFriendRankings(b2network::B2MatchInfoType::BMIT_PLAYER);
}

void UB2UIPVPMain::ShowWeekReward()
{
	auto PvpDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	if (!PvpDoc)
		return;

	int32 BladePoint = 0;
	int32 Gem = 0;
	int32 Gold = 0;
	int32 ItemId = 0;
	int32 DailyRewardCount = 0;

	const TArray<FModDailyReward>& DailyReward = BladeIIGameImpl::GetClientDataStore().GetModDailyRewardList();


	for (auto& Ele : DailyReward)
	{
		if (Ele.Match_Type != b2network::B2MatchInfoType::BMIT_PLAYER)
			continue;

		BladePoint = Ele.Reward_BladePoint;
		Gem = Ele.Reward_Gem;
		Gold = Ele.Reward_Gold;
		ItemId = Ele.Reward_ItemId;

	}

	if (PvpDoc->GetTagTodayReward())
		++DailyRewardCount;
	if (PvpDoc->GetTagTodayReward())
		++DailyRewardCount;


	if (PvpDoc->GetIsTagReward())
	{
		auto RewardUI = UB2UIManager::GetInstance()->OpenUI<UB2UIModWeekRewardPopup>(UIFName::ModMatchReward);
		RewardUI->OpenReward(UIFName::PVPOneOnOne);
		PvpDoc->SetIsTagReward(false);
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

		PvpDoc->SetTagLastReward(false);
		PvpDoc->SetTagTodayReward(false);
	}
}

/*void UB2UIPVPMain::SetDayRewardInfo()
{
	const auto& DailyRewardData = BladeIIGameImpl::GetClientDataStore().GetModDailyRewardList();

	for (auto& Ele : DailyRewardData)
	{
		if (Ele.Match_Type == b2network::B2MatchInfoType::BMIT_PLAYER)
		{
			auto* PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();
			UIP_PVPDayRewardBox->SetStarMaxNum(Ele.Require_Count);
			UIP_PVPDayRewardBox->SetStarCurNum(PvPDoc->GetTagParticipationCount());
			return;
		}
	}

}*/

void UB2UIPVPMain::SetFriendList(const FB2ResponseGetFriendRankingsPtr& FriendRank)
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

void UB2UIPVPMain::SetRewardList()
{
	if (List_Reward.IsValid())
	{
		List_Reward->ClearChildren();
		auto Rewards = BladeIIGameImpl::GetClientDataStore().GetDuelModeRewardData(b2network::B2DuelMode::DUEL_MODE_PVP);

		for(auto RewardItem : Rewards)
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

void UB2UIPVPMain::ShowInviteUI(bool bShowInvite)
{
	UB2UIManager* UIManager = UB2UIManager::GetInstance();
	if (bShowInvite)
	{
		auto* InviteListUI = UIManager->OpenUI<UB2UIGameInviteList>(UIFName::RaidInviteList);
		InviteListUI->SetInviteUIByGameMode(EB2GameMode::PVP_Tag);
	}
}

void UB2UIPVPMain::UpdateList()
{
	//Todo 친구탭인지 아닌지..
	if (VB_RankerList.IsValid())
	{
		VB_RankerList->ClearChildren();
		if (bShowingAllRanker)
		{
			auto* PvpDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

			if (!PvpDoc) return;

			ModRankerList = PvpDoc->RankerInfoContainer.GetRankerList();
			
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
			auto* CreatedWidget = CreateWidget<UB2UIRankerInfo>(GetOwningPlayer(), RankerInfoClass);
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_RankerList->AddChild(CreatedWidget));
				check(AddedSlot);
				RankerList.Add(CreatedWidget);
				CreatedWidget->Init();
				CreatedWidget->SetPadding(FMargin(0, 0, 0, 10.0));
				CreatedWidget->SetRankerInfo(Ranker,true);
			}
		}
	}
}

void UB2UIPVPMain::SetModRewardBox()
{
	auto* PvpDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	if (!PvpDoc)
		return;

	auto RewardBoxInfo = PvpDoc->GetRewardBoxStatus();

	if (UIP_ModReward.IsValid() && RewardBoxInfo)
		UIP_ModReward->SetModRewardBox(RewardBoxInfo->is_reward_state, b2network::B2DuelMode::DUEL_MODE_PVP, RewardBoxInfo->box_grade
			, RewardBoxInfo->win_count, RewardBoxInfo->upgrade_win_count, RewardBoxInfo->defeat_count);
}

