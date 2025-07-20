// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIEnterDungeonSlot.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "B2UIMsgPopupSimpleAddComment.h"

#include "B2ControlGameMode.h"

#include "BladeIIGameImpl.h"

void UB2UIEnterDungeonSlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Select);

	GET_SLOT(UTextBlock, TB_DungeonName);
	GET_SLOT(UTextBlock, TB_DungeonCount);
	GET_SLOT(UTextBlock, TB_DungeonDesc);
	GET_SLOT(UTextBlock, TB_Reward);

	GET_SLOT(UB2RichTextBlock, TB_ClosePossibilityTime);

	GET_SLOT(UB2RichTextBlock, STB_ClosePossible);
	GET_SLOT(UOverlay, OV_RaidWait);

	GET_SLOT(UImage, IMG_Reward);

	if (OV_RaidWait.IsValid())
		OV_RaidWait->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UOverlay, OV_NoEnter);

	if (OV_NoEnter.IsValid())
		OV_NoEnter->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UB2RichTextBlock, TB_NoEnterText);

	GET_SLOT(UOverlay, OV_ControlMode);

	if (OV_ControlMode.IsValid())
		OV_ControlMode->SetVisibility(ESlateVisibility::Collapsed);
	GET_SLOT(UB2RichTextBlock, TB_ControlMode);

	// Slot BG 绰 UMG 郴何 BGImage 函荐俊 殿废秦辑, Construct 俊辑 技泼窍霸 登绢乐澜.
	// RewardIcon, RewardName, DungeonName, DungeonDesc, STB_ClosePossible 器窃
}

void UB2UIEnterDungeonSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIEnterDungeonSlot::OnClickBTN_Select);
}

void UB2UIEnterDungeonSlot::Init()
{
	Super::Init();

	CurrentModeState = b2network::B2ContentsModeState::NONE;
}

void UB2UIEnterDungeonSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIEnterDungeonSlot::OnClickBTN_Select()
{
	switch (CurrentModeState)
	{
	case b2network::B2ContentsModeState::NONE:
	case b2network::B2ContentsModeState::RUN:
		// 鞠巴档 救窃. 

		break;
	case b2network::B2ContentsModeState::BLOCK_ENTER:
	case b2network::B2ContentsModeState::MODE_FINISH:
	case b2network::B2ContentsModeState::SETTLE_BEGIN:
	case b2network::B2ContentsModeState::SETTLE_END:
	{
		int32 nStartDay = -1;
		int32 nStartHour = -1;
		int32 nStartMin = -1;
		int32 nEndDay = -1;
		int32 nEndHour = -1;
		int32 nEndMin = -1;

		// RUN 场唱绰 矫埃
		BladeIIGameImpl::GetClientDataStore().GetDuelModeSettleSchedule(GetCurrentModeType(CurrentGameMode),
			b2network::B2ContentsModeState::RUN,
			nStartDay,
			nStartHour,
			nStartMin);

		// SETTLE_END 场唱绰 矫埃
		BladeIIGameImpl::GetClientDataStore().GetDuelModeSettleSchedule(GetCurrentModeType(CurrentGameMode),
			b2network::B2ContentsModeState::SETTLE_END,
			nEndDay,
			nEndHour,
			nEndMin);

		// BLOCK_ENTER老订 促弗膏飘
		FText CommentText;
		if (CurrentModeState == b2network::B2ContentsModeState::BLOCK_ENTER)
			CommentText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("RankingBlockEnter"));
		else
			CommentText = BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, TEXT("811"));

		// 沥魂吝UI
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		UB2UIMsgPopupSimpleAddComment* pPopupUI = pUIManager->OpenMsgPopup<UB2UIMsgPopupSimpleAddComment>(EUIMsgPopup::SimpleAddComment,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			CommentText,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
			);

		FNumberFormattingOptions FormattingOption;
		FormattingOption.MinimumIntegralDigits = 2;


		// 眠啊内膏飘 n矫n盒 ~ n矫n盒鳖瘤 沥魂窃
		pPopupUI->SetAddComment(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("RankCalculteTime")), FText::AsNumber(nStartHour, &FormattingOption), FText::AsNumber(nStartMin, &FormattingOption), FText::AsNumber(nEndHour, &FormattingOption), FText::AsNumber(nEndMin, &FormattingOption)));

		//FString::FromInt(nErrorCode)
		//UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(811);
		return;
		break;
	}
	case b2network::B2ContentsModeState::SYSTEM_CHECK:
		// 痢八吝UI
		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(812);
		return;
		break;
	default:
		break;
	}
	FOnClickDelegate.Execute(CurrentGameMode);
}

void UB2UIEnterDungeonSlot::SetModeState(int32 ModeState)
{
	CurrentModeState = ModeState;

	switch (ModeState)
	{
	case b2network::B2ContentsModeState::NONE:
	case b2network::B2ContentsModeState::RUN:
		// 鞠巴档 救窃. 
		
		break;
	case b2network::B2ContentsModeState::BLOCK_ENTER:
	case b2network::B2ContentsModeState::MODE_FINISH:
	case b2network::B2ContentsModeState::SETTLE_BEGIN:
	case b2network::B2ContentsModeState::SETTLE_END:
		// 沥魂吝UI
		OV_NoEnter->SetVisibility(ESlateVisibility::HitTestInvisible);
		TB_NoEnterText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("RankCalculte")));
		break;
	case b2network::B2ContentsModeState::SYSTEM_CHECK:
		// 痢八吝UI
		OV_NoEnter->SetVisibility(ESlateVisibility::HitTestInvisible);
		TB_NoEnterText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ContentsShutdown")));
		break;
	default:
		break;
	}
}

bool UB2UIEnterDungeonSlot::IsModeStateCurrentOpen()
{
	return CurrentModeState == b2network::B2ContentsModeState::RUN;
}

int32 UB2UIEnterDungeonSlot::GetCurrentModeType(ELobbyModSceneElem LobbyModScene)
{
	switch (LobbyModScene)
	{
	case ELobbyModSceneElem::LMODIDX_PVP:
		return b2network::B2ContentsMode::PVP_DUEL;
		break;
	case ELobbyModSceneElem::LMODIDX_TeamMatch:
		return b2network::B2ContentsMode::PVP_TEAM;
		break;
	case ELobbyModSceneElem::LMODIDX_Control:
		return b2network::B2ContentsMode::ASSAULT;
		break;
	case ELobbyModSceneElem::LMODIDX_CounterDungeon:
		return b2network::B2ContentsMode::COUNTER;
		break;
	case ELobbyModSceneElem::LMODIDX_HeroTop:
		return b2network::B2ContentsMode::HERO_TOWER;
		break;
	case ELobbyModSceneElem::LMODIDX_Raid:
		return b2network::B2ContentsMode::RAID;
		break;
	case ELobbyModSceneElem::LMODIDX_Dimension:
		return b2network::B2ContentsMode::DIMENSION;
		break;
	default:
		break;
	}

	return b2network::B2ContentsMode::NONE;
}

void UB2UIEnterDungeonSlot::SetCountInfo(int32 LastCount, int32 MaxCount)
{
	if (TB_DungeonCount.IsValid())// (带傈 涝厘冉荐 / MAX)
	{
		if (MaxCount > 0)
			TB_DungeonCount->SetText(FText::FromString("(" + FString::FromInt(LastCount) + "/" + FString::FromInt(MaxCount) + ")"));
		else
			TB_DungeonCount->SetText(FText::GetEmpty());

		if (MaxCount > 0 && LastCount == 0)
			TB_DungeonCount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
		else
			TB_DungeonCount->SetColorAndOpacity(FLinearColor::White);
	}
}

void UB2UIEnterDungeonSlot::SetLockTimeInfo(FText OpenTimeText)
{
	if (!OpenTimeText.IsEmpty())
	{
		if (TB_ClosePossibilityTime.IsValid())
			TB_ClosePossibilityTime->SetText(OpenTimeText);
		if (OV_RaidWait.IsValid())
			OV_RaidWait->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIEnterDungeonSlot::SetButtonEnable(bool bEnable)
{
	BTN_Select->SetIsEnabled(bEnable);
}

void UB2UIEnterDungeonSlot::SetReward(int32 ImageNumber)
{
	if (RewardArray.IsValidIndex(ImageNumber))
	{
		if(IMG_Reward.IsValid())
			IMG_Reward->SetBrushFromMaterial(RewardArray[ImageNumber].RewardImage);

		if (TB_Reward.IsValid())
			TB_Reward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, RewardArray[ImageNumber].RewardText));
	}
}

void UB2UIEnterDungeonSlot::SetBlockPage_Dimension()
{
	OV_NoEnter->SetVisibility(ESlateVisibility::HitTestInvisible);
	TB_NoEnterText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_Close")));
	BTN_Select->SetIsEnabled(false);
}

void UB2UIEnterDungeonSlot::SetModeText(int32 ContentMode)
{
	//if (IsActiveControlTutorial())
	//{
	//	if (OV_ControlMode.IsValid())
	//		OV_ControlMode->SetVisibility(ESlateVisibility::Collapsed);

	//	return;
	//}

	//switch (ContentMode)
	//{
	//case b2network::B2ContentsMode::ASSAULT:
	//	if (OV_ControlMode.IsValid())
	//		OV_ControlMode->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//	if (TB_ControlMode.IsValid())
	//	{
	//		if(UB2UIDocHelper::GetDocControl()->GetControlType() == CONTROL_MODE_ORIGINAL)
	//			TB_ControlMode->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_WeeklyInfo1")));
	//		else if (UB2UIDocHelper::GetDocControl()->GetControlType() == CONTROL_MODE_NOMINO)
	//			TB_ControlMode->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_WeeklyInfo2")));
	//	}
	//	break;
	//default:
	//	if (OV_ControlMode.IsValid())
	//		OV_ControlMode->SetVisibility(ESlateVisibility::Collapsed);
	//	break;
	//}
}