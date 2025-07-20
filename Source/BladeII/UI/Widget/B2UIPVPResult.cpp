#include "B2UIPVPResult.h"
#include "B2UIDocHelper.h"
#include "B2UIGradeRankAndStar.h"
#include "B2TMGameMode.h"
#include "B2UIRankChanged.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2UIManager.h"
#include "Event.h"
#include "B2UIModRewardBox.h"
#include "B2UIStageResultLevelUp.h"
#include "B2UIRewardEventSet.h"
#include "B2UIShortagePopupManager.h"
#include "B2PVPGameMode.h"

#include "BladeIIGameImpl.h"
#include "B2UIUserReport.h"

UB2UIPVPResult::UB2UIPVPResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsFriendshipGame = false;
}

void UB2UIPVPResult::Init()
{
	Super::Init();

	auto* PVPGameMode = Cast<AB2PVPGameMode>(UGameplayStatics::GetGameMode(this));
	if (PVPGameMode && PVPGameMode->IsValidFriendshipGame())
	{
		if (P_RewardInfo.IsValid())
			P_RewardInfo->SetVisibility(ESlateVisibility::Collapsed);

		if (VB_RankInfo.IsValid())
			VB_RankInfo->SetVisibility(ESlateVisibility::Collapsed);

		IsFriendshipGame = true;
	}


	PvPMatchDoc = TWeakObjectPtr<UB2UIDocBattle>(UB2UIDocHelper::GetDocBattle());
	TeamMatchDoc = TWeakObjectPtr<UB2UIDocTeamMatch>(UB2UIDocHelper::GetDocTeamMatch());
	DocSome = TWeakObjectPtr<UB2UIDocSome>(UB2UIDocHelper::GetDocSome());


	CAPTURE_UOBJECT(UB2UIPVPResult);

	StartRankUpAnimationTicket = StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(StartRankUpAnimation, const EStateAfterGradeUp GradeUpState)
		Capture->StartRankupAnimation(GradeUpState);
	END_CAPTURE_OBJECT()
		);

	UserReportTicket = DeliveryReportUserClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UserReport)
		Capture->UserReportComplete();
		END_CAPTURE_OBJECT()
		);

	SetRewardBoxStatus();

	if (TB_SendMail.IsValid())
	{
		TB_SendMail->SetVisibility(ESlateVisibility::HitTestInvisible);
		//TB_SendMail->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_ItemAcquired"))));
	}

	if (BTN_OpponentInfo.IsValid() && PvPMatchDoc.IsValid() && TeamMatchDoc.IsValid())
	{
		BTN_OpponentInfo->SetIsEnabled(GameModeEnum == EB2GameMode::PVP_Tag ? !PvPMatchDoc->GetIsAIMe() : !TeamMatchDoc->GetIsAIMe());
	}

	if (BTN_UserReport.IsValid())
	{
		BTN_UserReport->SetVisibility(GameModeEnum == EB2GameMode::PVP_Tag ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        BTN_UserReport->SetIsEnabled(!PvPMatchDoc->GetIsAIMe());
	}
}

void UB2UIPVPResult::StartRankupAnimation(const EStateAfterGradeUp GradeUpState)
{
	if (EStateAfterGradeUp::GRADE_ANIM_START == GradeUpState && ANIM_UP_01.IsValid()) // 殿鞭诀
	{
		PlayAnimation(ANIM_UP_01.Get());

		if(BTN_BlockTouch.IsValid())
			BTN_BlockTouch->SetVisibility(ESlateVisibility::Visible);
	}
	else if (EStateAfterGradeUp::STATE_END == GradeUpState && UIP_PVPGrade.IsValid())
	{
		UIP_PVPGrade->SetGradeRank(GetGrade(GameModeEnum), GetStarCount(GameModeEnum));
	}
	else if (EStateAfterGradeUp::ADD_STAR_AFTER_GRADE_UP == GradeUpState)
	{
		if (UIP_PVPGrade.IsValid())
			UIP_PVPGrade->StartStarAnimation(0, GetStarCount(GameModeEnum), true);
	}
	else if (EStateAfterGradeUp::GRADE_UP_END == GradeUpState && ANIM_UP_02.IsValid())
	{
		ForcedCloseToolTipPopupClass<>::GetInstance().Signal(); // 酒捞袍 焊惑 砒屏 栋乐栏搁 瘤框
		PlayAnimation(ANIM_UP_02.Get());
	}

	if (!IsAnyAnimationPlaying() && DocSome.IsValid() && DocSome->GetRankingChanged() > 0)
	{
		ForcedCloseToolTipPopupClass<>::GetInstance().Signal(); // 酒捞袍 焊惑 砒屏 栋乐栏搁 瘤框
		ShowRankingChanged(DocSome->GetRankingChanged());
		DocSome->SetRankingChanged(0);
	}
	else
	{
		ForcedCloseToolTipPopupClass<>::GetInstance().Signal(); // 酒捞袍 焊惑 砒屏 栋乐栏搁 瘤框
		CheckAndOpenLevelUpPart();
	}

	if (TeamMatchDoc.IsValid())
	{
		TeamMatchDoc->SetPCBeforeGradeStar(TeamMatchDoc->GetPCGradeStar());
	}
	if (PvPMatchDoc.IsValid())
	{
		PvPMatchDoc->SetTagMatchBeforeGradeStar(PvPMatchDoc->GetTagMatchGradeStar());
	}
}

#if WITH_EDITOR
void UB2UIPVPResult::SetCommandRankUp(const int32 _Rank, const int32 _StarCount, const int32 _WantedStarCount)
{
	if (GameModeEnum != EB2GameMode::PVP_Team)
		return;

	auto Rank = _Rank;
	auto StarCount = _StarCount;
	auto WantedStarCount = _WantedStarCount;

	if (Rank != 1 && WantedStarCount > 2)
	{
		WantedStarCount = 2;
	}
	else if (WantedStarCount < -1)
	{
		WantedStarCount = -1;
	}

	if (Rank > 10)
	{
		Rank = 9;
	}
	else if (Rank < 0)
	{
		Rank = 0;
	}


	if (WantedStarCount > 0)
	{
		if (StarCount + WantedStarCount > 3)
		{
			if (Rank == 1)
			{
				TeamMatchDoc->SetPCGrade(Rank);
				TeamMatchDoc->SetPCGradeStar(StarCount + WantedStarCount);
			}
			else
			{
				auto ResultStarCount = WantedStarCount - (3 - StarCount);
				TeamMatchDoc->SetPCGrade(Rank - 1);
				TeamMatchDoc->SetPCGradeStar(ResultStarCount);
			}
		}
		else
		{
			TeamMatchDoc->SetPCGrade(Rank);
			TeamMatchDoc->SetPCGradeStar(StarCount + WantedStarCount);
		}
	}
	else if(WantedStarCount == 0)
	{
		return;
	}
	else
	{
		if (StarCount == 0)
		{
			if (Rank != 0)
			{
				return;
			}
		}
		else if (StarCount == 1)
		{
			TeamMatchDoc->SetPCGrade(Rank + 1);
			TeamMatchDoc->SetPCGradeStar(3);
		}
		else
		{
			TeamMatchDoc->SetPCGrade(Rank);
			TeamMatchDoc->SetPCGradeStar(StarCount - 1);
		}
	}
	TeamMatchDoc->SetPCBeforeGrade(Rank);
	TeamMatchDoc->SetPCBeforeGradeStar(StarCount);

	if (UIP_PVPGrade.IsValid())
	{
		auto Grade = GetGrade(GameModeEnum, false);
		auto Star = GetStarCount(GameModeEnum, false);
		UIP_PVPGrade->SetGradeRank(Grade, Star);
	}
	OnOpenAnimEnded();
}
#endif // WITH_EDITOR

void UB2UIPVPResult::UpdateStaticText()
{
	if (TB_WinningBonus.IsValid())
		TB_WinningBonus->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("WinningStreakBonus")));

	if(TB_PVPRankPrevent.IsValid())
		TB_PVPRankPrevent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPRankPrevent")));
}

void UB2UIPVPResult::OnOpenAnimEnded()
{
	auto GradeUp = GetGrade(GameModeEnum) <= GetGrade(GameModeEnum, false);

	if (GetGrade(GameModeEnum) < GetGrade(GameModeEnum, false)) // 殿鞭诀, 蜡瘤, 殿鞭促款 搬沥
	{
		GradeUp = true;
	}
	else if (GetGrade(GameModeEnum) == GetGrade(GameModeEnum, false))
	{
		GradeUp = GetStarCount(GameModeEnum) < GetStarCount(GameModeEnum, false) ? false : true;
	}
	else
	{
		GradeUp = false;
	}

	if(UIP_PVPGrade.IsValid())
		UIP_PVPGrade->StartStarAnimation(GetStarCount(GameModeEnum, false), GetStarCount(GameModeEnum), GradeUp);
}

void UB2UIPVPResult::OnRankUpAnimEnded_One()
{
	if(UIP_PVPGrade.IsValid())
		UIP_PVPGrade->StartRankUpAnimation(GetGrade(GameModeEnum, false), GetGrade(GameModeEnum), GetStarCount(GameModeEnum));

}

void UB2UIPVPResult::OnRankUpAnimEnded_Two()
{
	if (DocSome.IsValid() && DocSome->GetRankingChanged() > 0)
	{
		ShowRankingChanged(DocSome->GetRankingChanged());
		DocSome->SetRankingChanged(0);
	}

	if (TeamMatchDoc.IsValid())
		TeamMatchDoc->SetPCBeforeGrade(TeamMatchDoc->GetPCGrade());
	if (PvPMatchDoc.IsValid())
		PvPMatchDoc->SetTagMatchBeforeGrade(PvPMatchDoc->GetTagMatchGrade());
	if (UIP_PVPGrade.IsValid())
		UIP_PVPGrade->SetGradeRank(GetGrade(GameModeEnum), GetStarCount(GameModeEnum));

	if (BTN_BlockTouch.IsValid())
		BTN_BlockTouch->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIPVPResult::OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2)
{
	SetRewardBoxStatus();
}

void UB2UIPVPResult::UserReportComplete()
{
	if (BTN_UserReport.IsValid())
		BTN_UserReport->SetIsEnabled(false);

	UB2UIManager::GetInstance()->CloseUI(UIFName::UserReportPopup);
	DevShowNotReadyYetMessage(TEXT("General_ReportComplete"));
}

void UB2UIPVPResult::PreventRematch()
{
	if (BTN_Restart.IsValid())
		BTN_Restart->SetIsEnabled(false);
}

void UB2UIPVPResult::SetRankUpImamge_Implementation(const int32 Rank)
{
	if(UIP_PVPGrade.IsValid())
		UIP_PVPGrade->SetGradeRank(GetGrade(GameModeEnum), GetStarCount(GameModeEnum));
}

void UB2UIPVPResult::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	StartRankingUpAnimationClass<const EStateAfterGradeUp>::GetInstance().Unsubscribe(StartRankUpAnimationTicket);

	DeliveryReportUserClass<>::GetInstance().Unsubscribe(UserReportTicket);
}

void UB2UIPVPResult::ShowRankingChanged(int32 DiffChangedRanking)
{
	if (DocSome.IsValid() && UIP_PVPRankingChangedPopup.IsValid())
	{
		UIP_PVPRankingChangedPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UIP_PVPRankingChangedPopup->SetUsingGameMode(GameModeEnum);
		//UIP_PVPRankingChangedPopup->SetMyRankingData(DocSome->GetMyRankerInfo());
		//UIP_PVPRankingChangedPopup->SetOtherRankingData(DocSome->GetOtherRankerInfo()); //烙矫
		UIP_PVPRankingChangedPopup->SetChangedRanking(DiffChangedRanking);
	}
}



void UB2UIPVPResult::SetRewardBoxStatus()
{
	GameModeEnum = GetB2GameModeType(this);

	b2network::B2DuelRewardBoxStatusPtr RewardBosxStatus;
	FText RewardBoxNotice;

	if (GameModeEnum == EB2GameMode::PVP_Tag)
	{
		auto DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();

		if (!DocPVP)
			return;

		RewardBosxStatus  = DocPVP->GetRewardBoxStatus();

		RewardBoxNotice = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_Notice1"));
	}
	else if (GameModeEnum == EB2GameMode::PVP_Team)
	{
		if (TeamMatchDoc.IsValid())
		{
			RewardBosxStatus = TeamMatchDoc->GetRewardBoxStatus();
		}
		RewardBoxNotice = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_Notice2"));
	}

	if (RewardBosxStatus && UIP_ModRewardBox.IsValid())
	{
		UIP_ModRewardBox->SetModRewardBox(false, b2network::B2DuelMode::DUEL_MODE_PVP, RewardBosxStatus->box_grade
			, RewardBosxStatus->win_count, RewardBosxStatus->upgrade_win_count, RewardBosxStatus->defeat_count);
		UIP_ModRewardBox->SetModExplanation(RewardBoxNotice);
	}
}

void UB2UIPVPResult::BindDocAuto()
{
	GameModeEnum = GetB2GameModeType(this);


	int32 TopWinsChanged = 0;
	switch (GameModeEnum)
	{
	case EB2GameMode::PVP_Tag:
	{
		auto* BattleDoc = UB2UIDocHelper::GetDocBattle();
		auto* PvpDoc = UB2UIDocHelper::GetDocPVP1on1Rival();
		if (BattleDoc && PvpDoc)
		{
			if (UIP_PVPGrade.IsValid())
				UIP_PVPGrade->SetGradeRank(BattleDoc->GetTagMatchBeforeGrade(), BattleDoc->GetTagMatchBeforeGradeStar());
			if (TB_WinningBonus.IsValid())
			{
				TB_WinningBonus->SetVisibility(BattleDoc->GetTagMatchCurrContinuousWins() >=
					BladeIIGameImpl::GetClientDataStore().GetWinCountForBonus() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);	//2楷铰捞惑老锭 剁快扁

				// 1殿鞭篮 楷铰焊呈胶 钎扁 救茄促.
				if (BattleDoc->GetTagMatchBeforeGrade() == 1)
				{
					TB_WinningBonus->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			if (TB_ResultWin_Count.IsValid())
				TB_ResultWin_Count->SetText(FText::AsNumber(BattleDoc->GetTagMatchCurrContinuousWins()));
			if (TB_ResultLose_Count.IsValid())
				TB_ResultLose_Count->SetText(FText::AsNumber(BattleDoc->GetTagMatchPrevContinuousWins() + 1));
			if (TB_SuccessiveWin.IsValid())
				TB_SuccessiveWin->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(BattleDoc->GetTagMatchCurrTopContinuousWins())));

			TopWinsChanged = BattleDoc->GetTagMatchCurrTopContinuousWins() - BattleDoc->GetTagMatchPrevTopContinuousWins();

			PvpDoc->OnRewardBoxStatusChanged.AddUObject(this, &UB2UIPVPResult::OnChangedRewardBox);

		}


		break;
	}
	case EB2GameMode::PVP_Team:
	{
		//auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
		if (TeamMatchDoc.IsValid())
		{
			if (UIP_PVPGrade.IsValid())
				UIP_PVPGrade->SetGradeRank(TeamMatchDoc->GetPCBeforeGrade(), TeamMatchDoc->GetPCBeforeGradeStar());

			if (TB_WinningBonus.IsValid())
			{
				TB_WinningBonus->SetVisibility(TeamMatchDoc->GetPCCurrContinuousWins() >=
					BladeIIGameImpl::GetClientDataStore().GetWinCountForBonus() ? ESlateVisibility::Visible : ESlateVisibility::Hidden); //2楷铰捞惑老锭 剁快扁

				// 1殿鞭篮 楷铰焊呈胶 钎扁 救茄促.																										 // 1殿鞭篮 楷铰焊呈胶 钎扁 救茄促.
				if (TeamMatchDoc->GetPCBeforeGrade() == 1)
				{
					TB_WinningBonus->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			if (TB_ResultWin_Count.IsValid())
				TB_ResultWin_Count->SetText(FText::AsNumber(TeamMatchDoc->GetPCCurrContinuousWins()));
			if (TB_ResultLose_Count.IsValid())
				TB_ResultLose_Count->SetText(FText::AsNumber(TeamMatchDoc->GetPCCurrContinuousWins() + 1));
			if (TB_SuccessiveWin.IsValid())
				TB_SuccessiveWin->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(TeamMatchDoc->GetPCCurrTopContinuousWins())));

			TopWinsChanged = TeamMatchDoc->GetPCCurrTopContinuousWins() - TeamMatchDoc->GetPCPrevTopContinuousWins();

			TeamMatchDoc->OnRewardBoxStatusChanged.AddUObject(this, &UB2UIPVPResult::OnChangedRewardBox);


		}
		break;
	}
	default:
		BII_CHECK(false);
		return;
	}

	SetDeltaTopWins(TopWinsChanged);

	if (TB_PVPRankPrevent.IsValid())
	{
		if (GetGrade(GameModeEnum, false) == GetGrade(GameModeEnum) &&						//殿鞭苞 喊俺荐啊 鞍栏搁 膏飘 剁快扁
			GetStarCount(GameModeEnum, false) == GetStarCount(GameModeEnum))
		{
			TB_PVPRankPrevent->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{

			TB_PVPRankPrevent->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();

	if (CP_Victory.IsValid())
		CP_Victory->SetVisibility((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_Lose.IsValid())
		CP_Lose->SetVisibility((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (CP_Draw.IsValid())
		CP_Draw->SetVisibility((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::Draw) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (OV_WinResult.IsValid())
		OV_WinResult->SetVisibility((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (OV_LoseResult.IsValid())
		OV_LoseResult->SetVisibility((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() != ENetMatchResult::LocalWin) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	// OV_DrawResult 眠啊瞪 荐 乐澜. 弊矾搁 公铰何 惑炔俊急 困俊 OV_LoseResult 钎矫 救登霸
	if (TB_PlayerRank.IsValid())
	//	TB_PlayerRank->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerRank")), FText::AsNumber(DocSome->GetMyRankerInfo().Rank)));
	//if (TB_SendMail.IsValid())
		TB_SendMail->SetVisibility((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose) ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	if (TB_LoseMent.IsValid())
	{
		TB_LoseMent->SetVisibility(DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose)
			TB_LoseMent->SetText(BladeIIGetDuelModLoseMentText());
	}


	if (TB_VictoryReward.IsValid())
	{
		if ((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin))
			TB_VictoryReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_VictoryReward")));
		else if ((DocSome.IsValid() && DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::Draw))
			TB_VictoryReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_DrawReward")));
		else
			TB_VictoryReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_LoseReward")));
	}

	if (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose)
	{
		if (List_Reward.IsValid())
		{
			List_Reward->SetVisibility(ESlateVisibility::Hidden);
			List_Reward->ClearChildren();
		}

		if (UIP_LoseReward.IsValid())
		{
			UIP_LoseReward->SetVisibility(ESlateVisibility::Visible);

			for (auto Reward : DocSome->GetRewards())
			{
				UIP_LoseReward->Init();
				UIP_LoseReward->UpdateRewardIcon(Reward);
				break;
			}
		}

	}
	else
	{
		if (UIP_LoseReward.IsValid())
		{
			UIP_LoseReward->SetVisibility(ESlateVisibility::Hidden);
		}

		if (List_Reward.IsValid())
		{
			List_Reward->SetVisibility(ESlateVisibility::Visible);
			List_Reward->ClearChildren();

			for (auto Reward : DocSome->GetRewards())
			{
				auto RewardIcon = CreateWidget<UB2UIRewardIcon>(GetWorld(), RewardIconClass);
				if (RewardIcon)
				{
					RewardIcon->Init();
					RewardIcon->UpdateRewardIcon(Reward);
					List_Reward->AddChild(RewardIcon);
				}
			}
		}
	}

	if (List_Reward.IsValid())
	{
		List_Reward->SetVisibility(DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		List_Reward->ClearChildren();
		
		for (auto Reward : DocSome->GetRewards())
		{
			auto RewardIcon = CreateWidget<UB2UIRewardIcon>(GetWorld(), RewardIconClass);
			if (RewardIcon)
			{
				RewardIcon->Init();
				RewardIcon->UpdateRewardIcon(Reward);
				List_Reward->AddChild(RewardIcon);
			}
		}
	}

	SetDeltaRank(DocSome->GetRankingChanged());
}

void UB2UIPVPResult::CacheAssets()
{
	GET_SLOT(UB2UIGradeRankAndStar, UIP_PVPGrade);
	if (UIP_PVPGrade.IsValid())
	{
		UIP_PVPGrade->Init();
		UIP_PVPGrade->SetUseAnimation();
	}

	GET_SLOT(UB2UIRankChanged, UIP_PVPRankingChangedPopup);
	if (UIP_PVPRankingChangedPopup.IsValid())
	{
		UIP_PVPRankingChangedPopup->Init();
		UIP_PVPRankingChangedPopup->ButtonDelegateBindLambda([this]() { CloseRankingChangePopup(); });
	}

	GET_SLOT(UCanvasPanel, CP_Victory);
	GET_SLOT(UCanvasPanel, CP_Lose);
	GET_SLOT(UCanvasPanel, CP_Draw);

	GET_SLOT(UPanelWidget, P_RewardInfo);
	GET_SLOT(UVerticalBox, VB_RankInfo);
	GET_SLOT(UPanelWidget, P_BottomButtons);
	
	GET_SLOT(UOverlay, OV_WinResult);
	GET_SLOT(UTextBlock, TB_ResultWin_Count);
	GET_SLOT(UOverlay, OV_LoseResult);
	GET_SLOT(UTextBlock, TB_ResultLose_Count);

	GET_SLOT(UHorizontalBox, List_Reward);

	GET_SLOT(UTextBlock, TB_SuccessiveWin);
	GET_SLOT(UImage,  IMG_IncreaseWins);

	GET_SLOT(UTextBlock, TB_PlayerRank);
	GET_SLOT(UImage, IMG_IncreaseRank);
	GET_SLOT(UImage, IMG_DecreaseRank);
	GET_SLOT(UTextBlock, TB_DeltaRank);
	GET_SLOT(UTextBlock, TB_SendMail);
	GET_SLOT(UTextBlock, TB_VictoryReward);

	GET_SLOT(UButton, BTN_OpponentInfo);
	GET_SLOT(UButton, BTN_Restart);
	GET_SLOT(UButton, BTN_Quit);
	GET_SLOT(UB2Button, BTN_BlockTouch);

	GET_SLOT(UTextBlock, TB_LoseMent);

	GET_SLOT(UB2UIModRewardBox, UIP_ModRewardBox);
	if (UIP_ModRewardBox.IsValid())
		UIP_ModRewardBox->Init();

	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp);
	if (UIP_LevelUp.IsValid())
		UIP_LevelUp->Init();

	GET_SLOT(UTextBlock, TB_WinningBonus);
	GET_SLOT(UTextBlock, TB_PVPRankPrevent);

	GET_SLOT(UB2Button, BTN_UserReport);

	GET_SLOT(UB2UIRewardIcon, UIP_LoseReward);

	ANIM_Open = GetAnimation(this, FName("ANIM_Open"));
	ANIM_UP_01 = GetAnimation(this, FName("ANIM_UP_01"));
	ANIM_UP_02 = GetAnimation(this, FName("ANIM_UP_02"));

	if (P_BottomButtons.IsValid())
		P_BottomButtons->SetIsEnabled(false);
}

void UB2UIPVPResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OpponentInfo, &UB2UIPVPResult::OnClickBtnOpponentInfo);
	BIND_CLICK_FUNC_TO_BTN(BTN_Restart, &UB2UIPVPResult::OnClickBtnRestart);
	BIND_CLICK_FUNC_TO_BTN(BTN_Quit, &UB2UIPVPResult::OnClickBtnQuit);
	BIND_CLICK_FUNC_TO_BTN(BTN_UserReport, &UB2UIPVPResult::OnClickReport);
}

void UB2UIPVPResult::OnClickBtnOpponentInfo()
{
	ReturnToFindAccountClass<>::GetInstance().Signal();
}

void UB2UIPVPResult::OnClickBtnRestart()
{
	if (GameModeEnum == EB2GameMode::PVP_Tag)
	{
		if (IsFriendshipGame)
		{
			auto* PVPGameMode = Cast<AB2PVPGameMode>(UGameplayStatics::GetGameMode(this));
			auto* DocForRival = UB2UIDocHelper::GetDocPVP1on1Rival();

			if (PVPGameMode && DocForRival)
			{
				const FString& RivalNickName = DocForRival->GetUserNickName();
				PVPGameMode->RequestGameInvite(RivalNickName, EB2GameMode::PVP_Tag);
			}

			return;
		}


		// 痢八 沥魂 眉农
		if (CheckContentsModeState(b2network::B2ContentsMode::PVP_DUEL))
			return;

		if (!OnCheckAdditionalPoint_PvP()) return;

		auto* PvPBattleDoc = UB2UIDocHelper::GetDocPVP1on1Rival();
		if (PvPBattleDoc)
		{
			PvPBattleDoc->SetUsedAdditionalPoint(false);
			PvPBattleDoc->SetMatchPoint(PvPBattleDoc->GetMatchPoint() - 1);
		}
	}
	else if(GameModeEnum == EB2GameMode::PVP_Team)
	{
		// 痢八 沥魂 眉农
		if (CheckContentsModeState(b2network::B2ContentsMode::PVP_TEAM))
			return;

		if (!OnCheckAdditionalPoint_Team()) return;

		auto* TeamBattleDoc = UB2UIDocHelper::GetDocTeamMatch();
		if (TeamBattleDoc)
			TeamBattleDoc->SetUsedAdditionalPoint(false);
	}

	ReturnToPVPRematchClass<>::GetInstance().Signal();
}

void UB2UIPVPResult::OnClickBtnQuit()
{
	ReturnToPVPMainMenuClass<bool>::GetInstance().Signal(false);
}

void UB2UIPVPResult::OnClickReport()
{
	UB2UIUserReport* UserReport = UB2UIManager::GetInstance()->OpenUI<UB2UIUserReport>(UIFName::UserReportPopup);

	if (UserReport)
	{
		auto PVPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

		if (!PVPDoc)
			return;

		UserReport->UpdatePopup(b2network::B2ReportModeType::PVP, PVPDoc->GetUserAccountID(), PVPDoc->GetUserNickName());
	}
}

bool UB2UIPVPResult::OnCheckAdditionalPoint_PvP()
{
	auto* PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	check(PvPDoc)
	if (PvPDoc->GetMatchPoint() == 0)
	{
		//萍南何练 舅覆扑诀
		UB2UIMsgPopupUseResources* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::UseGem,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPMatch_BuyTicketTitleMsg")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this](){ OnCheckGemForBuyTicket_PvP(); }),
			FMsgPopupOnClick::CreateLambda([this]() { BTN_Restart->SetIsEnabled(false); }));// 秒家滚瓢 喘范阑锭 滚瓢 厚劝己拳

		if (MsgUI)
		{
			MsgUI->SetCost(BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(PvPDoc->GetAdditionalMatchCount()));
			MsgUI->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketSubMsg")));
		}
		return false;
	}

	return true;
}

void UB2UIPVPResult::OnCheckGemForBuyTicket_PvP()
{
	auto* PvPBattleDoc = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (PvPBattleDoc)
	{
		int32 AdditonalCount = PvPBattleDoc->GetAdditionalMatchCount();

		if (BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(AdditonalCount) > BladeIIGameImpl::GetClientDataStore().GetGemAmount()) // 离何练
		{
			ShortageMGR->PopupGoToShopForGemMessage();
			BTN_Restart->SetIsEnabled(false);
		}
		else
		{
			/*
			if (auto* DocUser = UB2UIDocHelper::GetDocUser())
			{
				int32 TempGem = BladeIIGameImpl::GetClientDataStore().GetGemAmount() - BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(PvPBattleDoc->GetAdditionalMatchCount());
				BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, TempGem);
			}
			*/

			PvPBattleDoc->SetUsedAdditionalPoint(true);
			ReturnToPVPRematchClass<>::GetInstance().Signal();
		}
	}
}

bool UB2UIPVPResult::OnCheckAdditionalPoint_Team()
{
	auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	check(TeamDoc)
		if (TeamDoc->GetTeamMatchPoint() == 0)
		{
			// 萍南何练 舅覆扑诀
			UB2UIMsgPopupUseResources* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupUseResources>(EUIMsgPopup::UseGem,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TeamMatch_BuyTicketTitleMsg")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::ConfirmOrCancel,
				FMsgPopupOnClick::CreateLambda([this]() { OnCheckGemForBuyTicket_Team(); }),
				FMsgPopupOnClick::CreateLambda([this]() {BTN_Restart->SetIsEnabled(false); }));// 秒家滚瓢 喘范阑锭 滚瓢 厚劝己拳 // ClickNo Function

			if (MsgUI)
			{
				MsgUI->SetCost(BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(TeamDoc->GetTeamAdditionalMatchCount()));
				MsgUI->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_BuyTicketSubMsg")));
			}

			return false;
		}

	return true;
}

void UB2UIPVPResult::OnCheckGemForBuyTicket_Team()
{
	auto* TeamBattleDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (TeamBattleDoc)
	{
		int32 AdditonalCount = TeamBattleDoc->GetTeamAdditionalMatchCount();

		if (BladeIIGameImpl::GetClientDataStore().GetAdditionalMatchPointCost(AdditonalCount) > BladeIIGameImpl::GetClientDataStore().GetGemAmount()) // 离何练
		{
			ShortageMGR->PopupGoToShopForGemMessage();
			BTN_Restart->SetIsEnabled(false);
		}
		else
		{
			TeamBattleDoc->SetUsedAdditionalPoint(true);
			ReturnToPVPRematchClass<>::GetInstance().Signal();
		}
	}
}

int32 UB2UIPVPResult::GetGrade(const EB2GameMode GameMode, const bool IsCurrent)
{
	auto* PvPBattle = UB2UIDocHelper::GetDocBattle();
	auto* TeamBattle = UB2UIDocHelper::GetDocTeamMatch();

	if ((GameMode != EB2GameMode::PVP_Tag && GameMode != EB2GameMode::PVP_Team) || nullptr == PvPBattle || nullptr == TeamBattle)
	{
		return 0;
	}

	if (IsCurrent)
	{
		return GameMode == EB2GameMode::PVP_Tag ? PvPBattle->GetTagMatchGrade() : TeamBattle->GetPCGrade();
	}
	return GameMode == EB2GameMode::PVP_Tag ? PvPBattle->GetTagMatchBeforeGrade() : TeamBattle->GetPCBeforeGrade();
}

int32 UB2UIPVPResult::GetStarCount(const EB2GameMode GameMode, const bool IsCurrent)
{
	auto* PvPBattle = UB2UIDocHelper::GetDocBattle();
	auto* TeamBattle = UB2UIDocHelper::GetDocTeamMatch();

	if ((GameMode != EB2GameMode::PVP_Tag && GameMode != EB2GameMode::PVP_Team) || nullptr == PvPBattle || nullptr == TeamBattle)
	{
		return 0;
	}

	if (IsCurrent)
	{
		return GameMode == EB2GameMode::PVP_Tag ? PvPBattle->GetTagMatchGradeStar() : TeamBattle->GetPCGradeStar();
	}
	return GameMode == EB2GameMode::PVP_Tag ? PvPBattle->GetTagMatchBeforeGradeStar() : TeamBattle->GetPCBeforeGradeStar();
}

void UB2UIPVPResult::CheckAndOpenLevelUpPart()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	// 饭骇诀 窍绰瘤 魄窜窍绰 何盒篮 醚 2镑. 珐欧函版扑诀捞饶 客, 珐欧函版捞 绝阑版快 弊 捞傈.
	TArray<EPCClass> ThisPCClass;

	if (GameModeEnum == EB2GameMode::PVP_Tag)
	{
		ThisPCClass.Add(CharacterDataStore.GetMainPlayerClass());
		ThisPCClass.Add(CharacterDataStore.GetSubPlayerClass());
	}
	else if(GameModeEnum == EB2GameMode::PVP_Team)
	{
		auto DocTeamMatch = UB2UIDocHelper::GetDocTeamMatch();
		if (DocTeamMatch)
		{
			ThisPCClass.Add(DocTeamMatch->GetPCCharacterClass1());
			ThisPCClass.Add(DocTeamMatch->GetPCCharacterClass2());
			ThisPCClass.Add(DocTeamMatch->GetPCCharacterClass3());
		}
	}
	else
	{
		return;
	}
	
	for (auto pcclass : ThisPCClass)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
		//UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(pcclass));
		//bool bCharLevelUp = (FMath::Max(DocHero->GetBasicUpgradeLevel(), DocHero->GetExtraUpgradeLevel()) > DocHero->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(pcclass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(pcclass);
		if (ClearAfterLevel > ClearBeforeLevel)
		{
			if (UIP_LevelUp.IsValid())
			{
				UIP_LevelUp->StartMe(ThisPCClass);
				UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
				return;
			}
		}
	}

	OnLevelUpFXFinish();
}


void UB2UIPVPResult::OnLevelUpFXFinish()
{
	if(P_BottomButtons.IsValid())
		P_BottomButtons->SetIsEnabled(true);
}

void UB2UIPVPResult::CloseRankingChangePopup()
{
	CheckAndOpenLevelUpPart();
}
