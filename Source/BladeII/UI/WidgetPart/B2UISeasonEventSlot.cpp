// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISeasonEventSlot.h"

#include "B2UIManager.h"
#include "B2UIRewardEventSet.h"
#include "MissionManager.h"
#include "B2MissionInfo.h"
#include "Retailer.h"
#include "TutorialManager.h"

const int32 iSEASONEVENT_MAINEVENTPROGRESS = 0;
const int32 iSEASONEVENT_PROGRESS = 1;
const int32 iSEASONEVENT_POSSIBLE = 2;
const int32 iSEASONEVENT_COMPLETE = 3;

void UB2UISeasonEventSlot::Init()
{
	Super::Init();
}

void UB2UISeasonEventSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISeasonEventSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GetReward);

	GET_SLOT(UTextBlock, TB_GetReward);

	GET_SLOT(UTextBlock, TB_CompleteReward);

	GET_SLOT(UB2Button, BTN_GotoLink);

	GET_SLOT(UTextBlock, TB_GotoLink);

	GET_SLOT(UWidgetSwitcher, WS_SeasonEventList);

	const int32 iArrayMax = WS_SeasonEventList->GetNumWidgets();
	ArrayRTB_SeasonEvent.SetNum(iArrayMax);
	GET_SLOT_BYNAME(UB2RichTextBlock, ArrayRTB_SeasonEvent[iSEASONEVENT_MAINEVENTPROGRESS], RTB_MainEvent);
	GET_SLOT_BYNAME(UB2RichTextBlock, ArrayRTB_SeasonEvent[iSEASONEVENT_PROGRESS], RTB_Progress);
	GET_SLOT_BYNAME(UB2RichTextBlock, ArrayRTB_SeasonEvent[iSEASONEVENT_POSSIBLE], RTB_Possible);
	GET_SLOT_BYNAME(UB2RichTextBlock, ArrayRTB_SeasonEvent[iSEASONEVENT_COMPLETE], RTB_Complete);

	GET_SLOT(UProgressBar, PB_Progress);

	GET_SLOT(UTextBlock, TB_ProgressCount);
	GET_SLOT(UTextBlock, TB_ProgressCountComplete);

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);

	if (UIP_RewardIcon.IsValid())
		UIP_RewardIcon->Init();
}

void UB2UISeasonEventSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UISeasonEventSlot::OnClickBTN_GetReward);

	BIND_CLICK_FUNC_TO_BTN(BTN_GotoLink, &UB2UISeasonEventSlot::OnClickBTN_Link);
}

void UB2UISeasonEventSlot::UpdateStaticText()
{
	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));

	if (TB_GotoLink.IsValid())
		TB_GotoLink->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mission_Link")));

	//if (TB_CompleteReward.IsValid())	//한글, 영어 모두 complete인데...
	//	TB_CompleteReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("")));
}

void UB2UISeasonEventSlot::OnClickBTN_GetReward()
{
	if (fSeasonEventSlotInfo.state != iSEASONEVENT_POSSIBLE)
		return;

	fSeasonEventSlotInfo.state = iSEASONEVENT_COMPLETE;

	data_trader::Retailer::GetInstance().RequestRewardSeasonMission(
		fSeasonEventSlotInfo.RewardInfo.event_id, fSeasonEventSlotInfo.RewardInfo.day, fSeasonEventSlotInfo.RewardInfo.index);
}

void UB2UISeasonEventSlot::OnClickBTN_Link()
{
	if (TutorialManager::GetInstance().CheckOpenLockContentPopup(fSeasonEventSlotInfo.RewardInfo.tutorial_id) == false)
	{
		SeasonEventGotoLinkSceneClass<int32, int32>::GetInstance().Signal(fSeasonEventSlotInfo.RewardInfo.day, fSeasonEventSlotInfo.RewardInfo.index);
	}
}

bool UB2UISeasonEventSlot::CheckDayMainMission()
{
	//지금은 인덱스가 1이면 메인미션 취급
	const int32 iDayMainMission = 1;

	if (fSeasonEventSlotInfo.RewardInfo.index == iDayMainMission)
		return true;

	return false;
}

void UB2UISeasonEventSlot::UpdateSeasonEventSlotInfo(const FSeasonEventSlotInfo &EventSlotInfo)
{
	fSeasonEventSlotInfo = EventSlotInfo;

	if (UIP_RewardIcon.IsValid())
		UIP_RewardIcon->UpdateRewardIcon(EventSlotInfo.RewardInfo.reward_id, EventSlotInfo.RewardInfo.reward_count);

	UpdateSeasonEventProgressBar();

	UpdateSeasonEventSlot();
}

void UB2UISeasonEventSlot::UpdateSeasonEventSlot()
{
	if (CheckDayMainMission())
		UpdateSeasonEventMainSlotState();
	else
		UpdateSeasonEventSlotState();
}

void UB2UISeasonEventSlot::UpdateSeasonEventMainSlotState()
{
	if (!BTN_GetReward.IsValid() ||
		!BTN_GotoLink.IsValid() ||
		!WS_SeasonEventList.IsValid() ||
		!TB_ProgressCount.IsValid() ||
		!TB_ProgressCountComplete.IsValid())
		return;

	int32 iWidgetSwitcherIndex = fSeasonEventSlotInfo.state;

	BTN_GetReward->SetVisibility(ESlateVisibility::Collapsed);
	BTN_GotoLink->SetVisibility(ESlateVisibility::Collapsed);
	TB_ProgressCount->SetVisibility(ESlateVisibility::Visible);
	TB_ProgressCountComplete->SetVisibility(ESlateVisibility::Collapsed);

	switch (fSeasonEventSlotInfo.state)
	{

	case iSEASONEVENT_PROGRESS:
	{
		iWidgetSwitcherIndex = iSEASONEVENT_MAINEVENTPROGRESS;

		BTN_GetReward->SetVisibility(ESlateVisibility::Visible);
		BTN_GetReward->SetIsEnabled(false);
	}
	break;

	case iSEASONEVENT_POSSIBLE:
	{
		BTN_GetReward->SetVisibility(ESlateVisibility::Visible);
		BTN_GetReward->SetIsEnabled(true);

		TB_ProgressCount->SetVisibility(ESlateVisibility::Collapsed);
		TB_ProgressCountComplete->SetVisibility(ESlateVisibility::Visible);
	}
	break;

	}

	WS_SeasonEventList->SetActiveWidgetIndex(iWidgetSwitcherIndex);
}

void UB2UISeasonEventSlot::UpdateSeasonEventSlotState()
{
	if (!BTN_GetReward.IsValid() ||
		!BTN_GotoLink.IsValid() ||
		!WS_SeasonEventList.IsValid() ||
		!TB_ProgressCount.IsValid() ||
		!TB_ProgressCountComplete.IsValid())
		return;

	int32 iWidgetSwitcherIndex = fSeasonEventSlotInfo.state;

	BTN_GetReward->SetVisibility(ESlateVisibility::Collapsed);
	BTN_GotoLink->SetVisibility(ESlateVisibility::Collapsed);
	TB_ProgressCount->SetVisibility(ESlateVisibility::Visible);
	TB_ProgressCountComplete->SetVisibility(ESlateVisibility::Collapsed);

	switch (fSeasonEventSlotInfo.state)
	{

	case iSEASONEVENT_PROGRESS:
	{
		BTN_GotoLink->SetVisibility(ESlateVisibility::Visible);
	}
	break;

	case iSEASONEVENT_POSSIBLE:
	{
		BTN_GetReward->SetVisibility(ESlateVisibility::Visible);
		BTN_GetReward->SetIsEnabled(true);

		TB_ProgressCount->SetVisibility(ESlateVisibility::Collapsed);
		TB_ProgressCountComplete->SetVisibility(ESlateVisibility::Visible);
	}
	break;

	}

	WS_SeasonEventList->SetActiveWidgetIndex(iWidgetSwitcherIndex);
}

void UB2UISeasonEventSlot::UpdateSeasonEventProgressBar()
{
	if (TB_ProgressCount.IsValid())
	{
		int32 DispalyCurrCount = FMath::Clamp<int32>(fSeasonEventSlotInfo.progress_count, 0, fSeasonEventSlotInfo.RewardInfo.goal_count);
		FString ProgressString = FString::Printf(TEXT("(%d / %d)"), DispalyCurrCount, fSeasonEventSlotInfo.RewardInfo.goal_count);
		TB_ProgressCount->SetText(FText::FromString(ProgressString));
		TB_ProgressCountComplete->SetText(FText::FromString(ProgressString));
	}

	if (PB_Progress.IsValid())
	{
		float ProgressRate = static_cast<float>(fSeasonEventSlotInfo.progress_count) / static_cast<float>(fSeasonEventSlotInfo.RewardInfo.goal_count);
		PB_Progress->SetPercent(ProgressRate);
	}
}

void UB2UISeasonEventSlot::SetSeasonEventSlotText(FText fSeasonEventText)
{
	int32 iGoalCount = fSeasonEventSlotInfo.RewardInfo.goal_count;

	if (CheckDayMainMission())
		iGoalCount = fSeasonEventSlotInfo.RewardInfo.day;

	FText fSeasonEventSlotText = FText::Format(fSeasonEventText, FText::AsNumber(iGoalCount));

	const int32 idxMax = ArrayRTB_SeasonEvent.Num();
	for (int32 idx = 0; idx < idxMax; ++idx)
	{
		if (ArrayRTB_SeasonEvent.IsValidIndex(idx))
		{
			ArrayRTB_SeasonEvent[idx]->SetText(fSeasonEventSlotText);

#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SameLine====================================================================\n"));
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("UB2UISeasonEventSlot::SetSeasonEventSlotText ArrayRTB_SeasonEvent IsValid\n"));
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SameLine=====================================================================\n"));
#endif
		}
		else
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SameLine====================================================================\n"));
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("UB2UISeasonEventSlot::SetSeasonEventSlotText ArrayRTB_SeasonEvent Not Valid\n"));
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SameLine=====================================================================\n"));
#endif
		}
			
	}
}