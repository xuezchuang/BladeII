// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotPointConsume.h"

#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "BladeIIUtil.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"

void UB2UIEventSlotPointConsume::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);
	GET_SLOT(UB2RichTextBlock, RTB_ItemName);
	GET_SLOT(UB2RichTextBlock, RTB_NeedPoint);
	GET_SLOT(UB2Button, BTN_DecreaseReward);
	GET_SLOT(UB2Button, BTN_IncreaseReward);

	GET_SLOT(UWidgetSwitcher, WS_EventSlot);

	//progress
	GET_SLOT(UB2RichTextBlock, RTB_BtnProgress);

	//complete
	GET_SLOT(UB2RichTextBlock, RTB_BtnComplete);
	GET_SLOT(UB2Button, BTN_GetReward);
}

void UB2UIEventSlotPointConsume::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIEventSlotPointConsume::OnClickBTN_GetReward);
	BIND_PRESS_FUNC_TO_BTN(BTN_DecreaseReward, &UB2UIEventSlotPointConsume::OnPressBTN_DecreaseRewardAmount);
	BIND_PRESS_FUNC_TO_BTN(BTN_IncreaseReward, &UB2UIEventSlotPointConsume::OnPressBTN_IncreaseRewardAmount);
	BIND_RELEASE_FUNC_TO_BTN(BTN_DecreaseReward, &UB2UIEventSlotPointConsume::OnReleaseBTN_ChangeRewardAmount);
	BIND_RELEASE_FUNC_TO_BTN(BTN_IncreaseReward, &UB2UIEventSlotPointConsume::OnReleaseBTN_ChangeRewardAmount);
}

void UB2UIEventSlotPointConsume::UpdateStaticText()
{
	if (RTB_BtnProgress.IsValid())
		RTB_BtnProgress->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventButtonDesc")));

	if (RTB_BtnComplete.IsValid())
		RTB_BtnComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventButtonDesc")));

	if (RTB_ItemName.IsValid())
	{
		int32 itemID = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(CachedEventRewardInfo.reward_id);
		RTB_ItemName->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, FString::FromInt(itemID)));
	}
}

void UB2UIEventSlotPointConsume::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckPressedChangeRewardBTN(InDeltaTime);
}

void UB2UIEventSlotPointConsume::OnClickBTN_GetReward()
{
	if (iEventState != b2network::B2MissionStateType::COMPLETED)
		return;

	iEventState = b2network::B2MissionStateType::PROGRESS;

	data_trader::Retailer::GetInstance().RequestRewardPointShopEvent(iEventID, iEventIndex, CurrentRewardAmount);
}

void UB2UIEventSlotPointConsume::OnPressBTN_DecreaseRewardAmount()
{
	if(ChangeRewardAmountBTNState == EEventSlotPointConsumeBTNState::None)
		ChangeRewardAmountBTNState = EEventSlotPointConsumeBTNState::Decrease;
}

void UB2UIEventSlotPointConsume::OnPressBTN_IncreaseRewardAmount()
{
	if (ChangeRewardAmountBTNState == EEventSlotPointConsumeBTNState::None)
		ChangeRewardAmountBTNState = EEventSlotPointConsumeBTNState::Increase;
}

void UB2UIEventSlotPointConsume::OnReleaseBTN_ChangeRewardAmount()
{
	if (ChangeRewardAmountBTNState != EEventSlotPointConsumeBTNState::None)
		ChangeRewardAmountBTNState = EEventSlotPointConsumeBTNState::None;
}

void UB2UIEventSlotPointConsume::CheckPressedChangeRewardBTN(const float InDeltaTime)
{
	if (ChangeRewardAmountBTNState == EEventSlotPointConsumeBTNState::None)
	{
		SumPressedTime = 0.0f;
		ChangeRewardAmountTime = 0.0f;
		return;
	}

	SumPressedTime += InDeltaTime;

	if (SumPressedTime >= ChangeRewardAmountTime)
	{
		const float ChangeRewardAmountTickMaxTime = ChangeRewardAmountTickMaxCount * ChangeRewardAmountTickTime;
		const bool TimeOver = (ChangeRewardAmountTime >= ChangeRewardAmountTickMaxTime ? true : false);

		UpdateRewardAmount(TimeOver);
		UpdateRewardSlotText();

		if (TimeOver == false)
		{
			ChangeRewardAmountTime += ChangeRewardAmountTickTime;
		}
		else
		{
			SumPressedTime = 0.0f;
			ChangeRewardAmountTime = 0.0f;
			ChangeRewardAmountBTNState = EEventSlotPointConsumeBTNState::None;
		}
	}
}

void UB2UIEventSlotPointConsume::UpdateRewardSlotText()
{
	const int32 iNeedPoint = CachedEventRewardInfo.need_point * CurrentRewardAmount;
	const int32 iRewardAmount = CachedEventRewardInfo.reward_count * CurrentRewardAmount;

	if (RTB_NeedPoint.IsValid())
		RTB_NeedPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemCount")), FText::AsNumber(iNeedPoint)));

	if (UIP_RewardIcon.IsValid())
		UIP_RewardIcon->SetAmount(FText::AsNumber(iRewardAmount));
}

void UB2UIEventSlotPointConsume::UpdateRewardAmount(const bool bTimeOver)
{
	const int32 iPossibleRewardAmountMax = FMath::Clamp<int32>(CachedAcquireEventPoint / CachedEventRewardInfo.need_point, 1, ChangeRewardAmountMax);

	if (ChangeRewardAmountBTNState == EEventSlotPointConsumeBTNState::Decrease)
		CurrentRewardAmount = (bTimeOver ? 1 : FMath::Clamp<int32>(--CurrentRewardAmount, 1, iPossibleRewardAmountMax));
	else if (ChangeRewardAmountBTNState == EEventSlotPointConsumeBTNState::Increase)
		CurrentRewardAmount = (bTimeOver ? iPossibleRewardAmountMax : FMath::Clamp<int32>(++CurrentRewardAmount, 1, iPossibleRewardAmountMax));
	else
		CurrentRewardAmount = FMath::Clamp<int32>(CurrentRewardAmount, 1, iPossibleRewardAmountMax);
}

void UB2UIEventSlotPointConsume::UpdateVisibleState()
{
	if (WS_EventSlot.IsValid())
		WS_EventSlot->SetActiveWidgetIndex(iEventState);
}

void UB2UIEventSlotPointConsume::InitEventSlotPointConsume(const b2network::B2mdPointEventReward& EventRewardInfo)
{
	iEventID = EventRewardInfo.event_id;
	iEventIndex = EventRewardInfo.index;
	
	CachedAcquireEventPoint = 0;
	CurrentRewardAmount = 1;
	SumPressedTime = 0.0f;
	ChangeRewardAmountTime = 0.0f;
	ChangeRewardAmountBTNState = EEventSlotPointConsumeBTNState::None;

	CachedEventRewardInfo = EventRewardInfo;

	Super::Init();

	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->Init();
		UIP_RewardIcon->UpdateRewardIcon(EventRewardInfo.reward_id, EventRewardInfo.reward_count);
	}

	UpdateRewardSlotText();
}

void UB2UIEventSlotPointConsume::CheckEventSlotState(const int32 iAcquireEventPoint)
{
	CachedAcquireEventPoint = iAcquireEventPoint;

	if (iAcquireEventPoint >= CachedEventRewardInfo.need_point)
		iEventState = b2network::B2MissionStateType::COMPLETED;
	else
		iEventState = b2network::B2MissionStateType::PROGRESS;

	const bool TimeOver = false;

	UpdateRewardAmount(TimeOver);
	UpdateRewardSlotText();
	UpdateVisibleState();
}

