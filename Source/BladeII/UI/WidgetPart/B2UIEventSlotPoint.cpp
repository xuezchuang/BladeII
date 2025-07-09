// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEventSlotPoint.h"

#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "BladeIIUtil.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"

void UB2UIEventSlotPoint::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);
	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UB2RichTextBlock, RTB_ReceiveBtn);
	GET_SLOT(UB2RichTextBlock, RTB_ItemName);
	GET_SLOT(UOverlay, O_Complete);
	GET_SLOT(UImage, IMG_GetRewardEffect);

	if (O_Complete.IsValid())
		O_Complete->SetVisibility(ESlateVisibility::Collapsed);

	if (IMG_GetRewardEffect.IsValid())
		IMG_GetRewardEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIEventSlotPoint::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIEventSlotPoint::OnClickBTN_GetReward);
}

void UB2UIEventSlotPoint::OnClickBTN_GetReward()
{
	if (iEventState != b2network::B2MissionStateType::COMPLETED)
		return;

	iEventState = b2network::B2MissionStateType::REWARDED;

	UpdateVisibleState();

	OnGetReward_BP();
}

void UB2UIEventSlotPoint::GetRewardAnimEnd()
{
	data_trader::Retailer::GetInstance().RequestReceivePointEventReward(iEventID, iEventIndex);
}

void UB2UIEventSlotPoint::InitEventPointPart(const b2network::B2mdPointEventReward& EventRewardInfo)
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

	if (RTB_ReceiveBtn.IsValid())
		RTB_ReceiveBtn->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("AcquirePoint")), FText::AsNumber(EventRewardInfo.need_point)));

	int32 itemID = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(EventRewardInfo.reward_id);

	if (RTB_ItemName.IsValid())
		RTB_ItemName->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, FString::FromInt(itemID)));
}

void UB2UIEventSlotPoint::UpdateVisibleState()
{
	if (BTN_GetReward.IsValid())
		BTN_GetReward->SetIsEnabled(false);

	switch (iEventState)
	{
	case b2network::B2MissionStateType::COMPLETED:
	{
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetIsEnabled(true);

		if (IMG_GetRewardEffect.IsValid())
			IMG_GetRewardEffect->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (RTB_ReceiveBtn.IsValid())
			RTB_ReceiveBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
	}break;

	case b2network::B2MissionStateType::REWARDED:
	{
		if (IMG_GetRewardEffect.IsValid())
			IMG_GetRewardEffect->SetVisibility(ESlateVisibility::Collapsed);

		if (RTB_ReceiveBtn.IsValid())
			RTB_ReceiveBtn->SetVisibility(ESlateVisibility::Collapsed);

		if (O_Complete.IsValid())
			O_Complete->SetVisibility(ESlateVisibility::HitTestInvisible);
	}break;

	}

}




