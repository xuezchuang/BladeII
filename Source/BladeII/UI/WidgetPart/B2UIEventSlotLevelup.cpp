// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotLevelup.h"

#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "BladeIIUtil.h"

void UB2UIEventSlotLevelup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);
	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UTextBlock, TB_GetRewardPossible);
	GET_SLOT(UTextBlock, TB_GetRewardImPossible);
	GET_SLOT(UTextBlock, TB_CompleteReward);
	GET_SLOT(UOverlay, O_GetRewardPossible);
	GET_SLOT(UOverlay, O_GetRewardImPossible);

	if (TB_CompleteReward.IsValid())
		TB_CompleteReward->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIEventSlotLevelup::UpdateStaticText()
{
	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));

	if (TB_CompleteReward.IsValid())
		TB_CompleteReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));

	if (TB_GetRewardPossible.IsValid())
		TB_GetRewardPossible->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_NotEnoughCharLevel")), FText::AsNumber(CachedEventRewardInfo.level)));

	if (TB_GetRewardImPossible.IsValid())
		TB_GetRewardImPossible->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_NotEnoughCharLevel")), FText::AsNumber(CachedEventRewardInfo.level)));
}

void UB2UIEventSlotLevelup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIEventSlotLevelup::OnClickBTN_GetReward);
}

void UB2UIEventSlotLevelup::UpdateVisibleState()
{
	bool bPossible = false;

	switch (iEventState)
	{
	case b2network::B2MissionStateType::COMPLETED:
	{
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetIsEnabled(true);

		bPossible = true;

	}break;

	case b2network::B2MissionStateType::REWARDED:
	{
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetVisibility(ESlateVisibility::Collapsed);

		if (TB_CompleteReward.IsValid())
			TB_CompleteReward->SetVisibility(ESlateVisibility::Visible);
	}break;

	default:
	{
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetIsEnabled(false);
	}break;
	}

	if (O_GetRewardPossible.IsValid())
		O_GetRewardPossible->SetVisibility(bPossible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (O_GetRewardImPossible.IsValid())
		O_GetRewardImPossible->SetVisibility(bPossible ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIEventSlotLevelup::OnClickBTN_GetReward()
{
	if (iEventState != b2network::B2MissionStateType::COMPLETED)
		return;

	iEventState = b2network::B2MissionStateType::REWARDED;

	data_trader::Retailer::GetInstance().RequestReceiveLevelUpEventReward(iEventID, iEventIndex);

	UpdateVisibleState();
}

void UB2UIEventSlotLevelup::InitEventLevelupPart(const b2network::B2mdLevelUpEvent& EventRewardInfo)
{
	iEventID = EventRewardInfo.event_id;

	iEventIndex = EventRewardInfo.index;

	CachedEventRewardInfo = EventRewardInfo;

	Super::Init();

	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->Init();
		UIP_RewardIcon->UpdateRewardIcon(EventRewardInfo.reward_id, EventRewardInfo.reward_count);
	}
}