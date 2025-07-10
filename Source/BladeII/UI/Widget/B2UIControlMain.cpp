// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlMain.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIDocControl.h"
#include "B2UIPVPEntryHero.h"
#include "B2UIRankerInfo.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIManager.h"
#include "B2UIChapterStarRewardBox.h"
#include "B2UIRewardEventSet.h"
#include "B2UIModWeeklyReward.h"
#include "B2UIModRewardBox.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupBoxRewardConfirm.h"
#include "B2UIModWeekRewardPopup.h"
#include "TutorialManager.h"
#include "B2ControlGameMode.h"

#include "BladeIIGameImpl.h"


void UB2UIControlMain::Init()
{
	Super::Init();

	if (UIP_MyControlGrade.IsValid())
	{
		int32 nGrade = UB2UIDocHelper::GetDocControl()->GetBattleGrade();
		int32 nStsrCount = UB2UIDocHelper::GetDocControl()->GetBattleStarCount();
		UIP_MyControlGrade->SetGradeRank(nGrade, nStsrCount);
	}

	if (TB_MyRanking.IsValid())
	{
		int32 nMyRank = UB2UIDocHelper::GetDocControl()->GetMyRanking();
		TB_MyRanking->SetText(nMyRank != 0 ? FText::AsNumber(nMyRank) : FText::FromString(TEXT("-")));
	}

	if (TB_RankPercentage.IsValid())
	{
		float fMyRankPercent = UB2UIDocHelper::GetDocControl()->GetMyRankingPercent();
		FString strMyRankPercent = FString::Printf(TEXT("(%2.1f%%)"), fMyRankPercent);
		TB_RankPercentage->SetText(FText::FromString(strMyRankPercent));
	}

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_ControlMatch"));

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ControlMain);
}

void UB2UIControlMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("ChallengeMod_ControlMatch"));

	UB2UIDocControl* DocControl = UB2UIDocHelper::GetDocControl();
	if (DocControl)
	{
		bool IsPopup = DocControl->GetSubmersion();
		if (IsPopup)
		{
			DocControl->SetSubmersion(false);

			UB2UIManager::GetInstance()->OpenMsgPopup(
				EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_WarningNotice")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
			);
		}
	}
}

void UB2UIControlMain::BindDocAuto()
{
	UnbindDoc();

	auto* pDoc = UB2UIDocHelper::GetDocControl();
	BII_CHECK(pDoc);
	if (!pDoc)
		return;

	SetDoc(pDoc);

	pDoc->OnRewardBoxStatusChanged.AddUObject(this, &UB2UIControlMain::OnChangedRewardBox);
	pDoc->OnMVPPointChanged.AddUObject(this, &UB2UIControlMain::OnChangedMVPPoint);

	int32 iContinueWinMax = 0;
	int32 iContinueWin = 0;

	if (auto pGradeInfo = pDoc->GetGradeInfo())
	{
		iContinueWinMax = pGradeInfo->continue_win_max;
		iContinueWin = pGradeInfo->continue_win;
	}

	if (TB_WinMaxNumber.IsValid())
		TB_WinMaxNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(iContinueWinMax)));

	if (TB_CurrentWinNumber.IsValid())
		TB_CurrentWinNumber->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_PlayerTopContinuedWins")), FText::AsNumber(iContinueWin)));

	// 남은시간 설정
	InitRemainTime();
}

void UB2UIControlMain::UnbindDoc()
{
	SetDoc(nullptr);
}

void UB2UIControlMain::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIControlMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UB2UIDocControl* DocControl = UB2UIDocHelper::GetDocControl();

	if (!DocControl)
		return;

	UIP_PVPSeason_Reward->SetRemainTime(FDateTime::UtcNow().ToUnixTimestamp(), (DocControl->GetSettlementTime() / 1000));

	FTimespan RemainTime = FDateTime::FromUnixTimestamp(DocControl->GetSettlementTime() / 1000) - FDateTime::FromUnixTimestamp(FDateTime::UtcNow().ToUnixTimestamp());
	int32 nRemainTimeSec = RemainTime.GetTotalSeconds();
	int32 nTotalTimeSec = DocControl->GetSettlementPeriodHour() * 3600;

	UIP_PVPSeason_Reward->SetForceProgressBar(nRemainTimeSec, nTotalTimeSec);
}

void UB2UIControlMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen();

	SubscribeEvents();
	SetModRewardBox();
	SetMVPRewardBox();
	SetPreRewardList();
	SetModePopup();
}

void UB2UIControlMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose();
	UnsubscribeEvents();
}

void UB2UIControlMain::UpdateStaticText()
{
	if (TB_WinMax.IsValid())
		TB_WinMax->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_TopContinuedWins")));

	if (TB_CurrentWin.IsValid())
		TB_CurrentWin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_WinningStreak")));

	if (TB_ControlMode.IsValid())
		TB_ControlMode->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_WeeklyButton")));
}

void UB2UIControlMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_StartGame);
	GET_SLOT(UButton, BTN_Ranking);
	GET_SLOT(UButton, BTN_ControlMode);

	GET_SLOT(UB2UIGradeRankAndStar, UIP_MyControlGrade);
	if (UIP_MyControlGrade.IsValid())
		UIP_MyControlGrade->Init();

	GET_SLOT(UB2UIChapterStarRewardBox, UIP_PVPSeason_Reward);
	if (UIP_PVPSeason_Reward.IsValid())
		UIP_PVPSeason_Reward->Init();

	GET_SLOT(UTextBlock, TB_ControlMode);

	GET_SLOT(UTextBlock, TB_MyRanking);
	GET_SLOT(UTextBlock, TB_RankPercentage);

	GET_SLOT(UTextBlock, TB_RemainTime);
	GET_SLOT(UTextBlock, TB_RemainTimeValue);

	GET_SLOT(UVerticalBox, List_Reward);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon_ScrollOnly);
	if (UIP_RewardIcon_ScrollOnly.IsValid())
	{
		UIP_RewardIcon_ScrollOnly->Init();
	}

	GET_SLOT(UB2UIModRewardBox, UIP_RewardBox);
	if (UIP_RewardBox.IsValid())
	{
		UIP_RewardBox->Init();
	}

	GET_SLOT(UB2UIMvpRewardBox, UIP_MVPRewardBox);
	if (UIP_MVPRewardBox.IsValid())
	{
		UIP_MVPRewardBox->Init();
	}

	GET_SLOT(UTextBlock, TB_WinMax);
	GET_SLOT(UTextBlock, TB_WinMaxNumber);
	GET_SLOT(UTextBlock, TB_CurrentWin);
	GET_SLOT(UTextBlock, TB_CurrentWinNumber);

	GET_SLOT(UCanvasPanel, CP_TimeInfo);

	GET_SLOT(UCanvasPanel, CP_ModePopup);
	GET_SLOT(UB2Button, Mode_BTN_Close);
	GET_SLOT(UTextBlock, Mode_TB_Info);
}

void UB2UIControlMain::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_StartGame, &UB2UIControlMain::OnClickBtnStartGame);
	BIND_CLICK_FUNC_TO_BTN(BTN_Ranking, &UB2UIControlMain::OnClickBtnRanking);
	BIND_CLICK_FUNC_TO_BTN(BTN_ControlMode, &UB2UIControlMain::OnClickBtnControlMode);
	BIND_CLICK_FUNC_TO_BTN(Mode_BTN_Close, &UB2UIControlMain::OnClickBtnControlModePopupClose);

	if (UIP_PVPSeason_Reward.IsValid())
		UIP_PVPSeason_Reward->RewardListDelgate.BindDynamic(this, &UB2UIControlMain::OnClickBtnSeasonReward);
}

void UB2UIControlMain::SetModRewardBox()
{
	UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();

	if (!ControlDoc)
		return;

	auto BoxInfo = ControlDoc->GetRewardBoxStatus();

	if (BoxInfo && UIP_RewardBox.IsValid())
		UIP_RewardBox->SetModRewardBox(BoxInfo->is_reward_state, b2network::B2DuelMode::DUEL_MODE_ASSAULT, BoxInfo->box_grade, BoxInfo->win_count, BoxInfo->upgrade_win_count, BoxInfo->defeat_count);

	// SeasonReward
	if (auto* pDoc = UB2UIDocHelper::GetDocControl())
	{
		if (pDoc->GetControlRankingReward())
		{
			auto* RewardUI = UB2UIManager::GetInstance()->OpenUI<UB2UIModWeekRewardPopup>(UIFName::ModMatchReward);
			RewardUI->OpenReward(UIFName::ControlMain);
		}
	}
}

void UB2UIControlMain::SetMVPRewardBox()
{
	UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();

	if (!ControlDoc)
		return;

	int32 MVPPoint = ControlDoc->GetMVPPoint();
	int32 MaxPoint = BladeIIGameImpl::GetClientDataStore().GetAssaultMVPMaxCount();

	if (UIP_MVPRewardBox.IsValid())
		UIP_MVPRewardBox->SetMvpBox(MVPPoint, MaxPoint);
}

void UB2UIControlMain::SetWinState()
{

}

void UB2UIControlMain::OnChangedRewardBox(class UB2UIDocBase* Sender, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo, b2network::B2DuelRewardBoxStatusPtr RewardBoxInfo2)
{
	SetModRewardBox();
}

void UB2UIControlMain::OnChangedMVPPoint(class UB2UIDocBase* Sender, int32 Point, int32 Point2)
{
	SetMVPRewardBox();
}

void UB2UIControlMain::OpenMVPRewardPopup(FB2AssaultMvpRewardPtr& MvpReward)
{
	auto* pDoc = UB2UIDocHelper::GetDocControl();
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager && pDoc)
	{
		auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		RewardPopup->AddRewardItems(MvpReward->received_rewards);
		RewardPopup->ShowRewardPopup(true);

		pDoc->SetMVPPoint(0);
	}
}

void UB2UIControlMain::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2UIControlMain);

	DeliveryOpenDuelRewardBoxTicket = DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryOpenDuelRewardBox, FB2OpenDuelRewardBoxPtr RewardInfo)
		UB2UIDocControl*	ControlDoc = UB2UIDocHelper::GetDocControl();
	if (!ControlDoc) return;
	ControlDoc->SetRewardBoxStatus(RewardInfo->reward_box_status);
	END_CAPTURE_OBJECT()
		);

	DeliveryAssaultMvpRewardTicket = DeliveryAssaultMvpRewardClass<FB2AssaultMvpRewardPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAssaultMvpReward, FB2AssaultMvpRewardPtr RewardInfo)
		Capture->OpenMVPRewardPopup(RewardInfo);
	END_CAPTURE_OBJECT()
		);
}

void UB2UIControlMain::UnsubscribeEvents()
{
	DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Unsubscribe(DeliveryOpenDuelRewardBoxTicket);
	DeliveryAssaultMvpRewardClass<FB2AssaultMvpRewardPtr>::GetInstance().Unsubscribe(DeliveryOpenDuelRewardBoxTicket);
}

void UB2UIControlMain::OnClickBtnStartGame()
{
	// 점검 정산 체크
	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_Occupy) && CheckContentsModeState(b2network::B2ContentsMode::ASSAULT))
		return;

	// 상자 보상 or Mvp보상 체크
	UB2UIDocControl* pDoc = UB2UIDocHelper::GetDocControl();

	if (!pDoc)
		return;

	if (pDoc->GetRewardBoxStatus()
		&& pDoc->GetRewardBoxStatus()->is_reward_state
		&& pDoc->GetCanViewModRewardBoxConfirm()/*한번만 보기 플래그*/
		&& !IsActiveControlTutorial())
	{
		// 팝업창 띄워주고 한번만 보기 플래그 오프
		UB2UIMsgPopupBoxRewardConfirm* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupBoxRewardConfirm>(UIFName::MsgPopupBoxRewardConfirm);

		if (pUI)
		{
			pUI->ApplyRewardBox(pDoc->GetRewardBoxStatus()->box_grade);
			pUI->SetAutoCloseOnClick(true);
			pUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]()
			{
				OnClickBtnStartGame();
			}
			));
		}

		pDoc->SetCanViewModRewardBoxConfirm(false);
		return;
	}

	if (pDoc->GetMVPPoint() >= BladeIIGameImpl::GetClientDataStore().GetAssaultMvpPointToOpenReward()
		&& pDoc->GetCanViewMvpRewardBoxConfirm()/*한번만 보기 플래그*/
		&& !IsActiveControlTutorial())
	{
		// 팝업창 띄워주고 한번만 보기 플래그 오프
		UB2UIMsgPopupBoxRewardConfirm* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupBoxRewardConfirm>(UIFName::MsgPopupBoxRewardConfirm);

		if (pUI)
		{
			pUI->ApplyMvpBox();
			pUI->SetAutoCloseOnClick(true);
			pUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]()
			{
				OnClickBtnStartGame();
			}
			));
		}

		pDoc->SetCanViewMvpRewardBoxConfirm(false);
		return;
	}

	if (!IsActiveControlTutorial() && !CheckControlAbleTime())
		return; // 점령전 입장시간이 아니면 팝업 출력후 리턴

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ControlCharacterSelectPage);
}

bool UB2UIControlMain::CheckControlAbleTime()
{
	auto DocControl = UB2UIDocHelper::GetDocControl();
	if (DocControl)
	{
		TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
		bool bIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(DocControl->GetPeriodInfos(), OpenTimes);

		if (bIsOpen)
			return true;
	}

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, TEXT("7603")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);

	return false;
}

void UB2UIControlMain::OnClickBtnRanking()
{
	// 버튼 명예의 전당.
	UB2UIManager::GetInstance()->OpenUI(UIFName::ControlRanking);
}

void UB2UIControlMain::OnClickBtnControlMode()
{
	if (CP_ModePopup.IsValid())
		CP_ModePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIControlMain::OnClickBtnControlModePopupClose()
{
	if (CP_ModePopup.IsValid())
		CP_ModePopup->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIControlMain::OnClickBtnSeasonReward()
{
	auto* WeeklyRewardList = UB2UIManager::GetInstance()->OpenUI<UB2UIModWeeklyReward>(UIFName::ModMatchRewardList_Week);

	if (WeeklyRewardList != nullptr)
		WeeklyRewardList->SetOpenRewardListType(UIFName::ModMatchReward);
}

void UB2UIControlMain::InitRemainTime()
{
	if (!TB_RemainTime.IsValid() || !TB_RemainTimeValue.IsValid())
		return;

	if (UB2UIDocHelper::GetDocControl()->IsAlwaysOpenControlMatch() && CP_TimeInfo.IsValid())
	{
		CP_TimeInfo->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	auto DocControl = UB2UIDocHelper::GetDocControl();
	bool bIsOpen = false;
	if (DocControl)
	{
		TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
		bIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(DocControl->GetPeriodInfos(), OpenTimes);
	}

	TB_RemainTime->SetText(bIsOpen ? BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRemainCloseTime")) : BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRemainOpenTime")));

	if (BTN_StartGame.IsValid())
	{
		// 점령전 튜토리얼일땐 활성화
		if (IsActiveControlTutorial())
		{
			BTN_StartGame->SetIsEnabled(true);
		}
		else
		{
			BTN_StartGame->SetIsEnabled(bIsOpen);
		}
	}

	int32 RemainTimeMin = UB2UIDocHelper::GetDocControl()->GetControlMatchRemainTime();
	int32 RemainTimeHour = RemainTimeMin / 60;
	RemainTimeMin = RemainTimeMin % 60;

	FString strRemainTrim = TEXT("");

	if (RemainTimeHour != 0)
		strRemainTrim += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), FText::AsNumber(RemainTimeHour)).ToString();

	strRemainTrim += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::AsNumber(RemainTimeMin)).ToString();

	TB_RemainTimeValue->SetText(FText::FromString(strRemainTrim));
}

void UB2UIControlMain::SetPreRewardList()
{
	if (List_Reward.IsValid())
	{
		List_Reward->ClearChildren();
		auto Rewards = BladeIIGameImpl::GetClientDataStore().GetDuelModeRewardData(b2network::B2DuelMode::DUEL_MODE_ASSAULT);

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

void UB2UIControlMain::SetModePopup()
{
	int32 currentMode = UB2UIDocHelper::GetDocControl()->GetControlType();

	if (Mode_TB_Info.IsValid())
	{
		switch (currentMode)
		{
		case CONTROL_MODE_ORIGINAL:
			Mode_TB_Info->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_WeeklyInfo1")));
			break;
		case CONTROL_MODE_NOMINO:
			Mode_TB_Info->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_WeeklyInfo2")));
			break;
		}
	}
}