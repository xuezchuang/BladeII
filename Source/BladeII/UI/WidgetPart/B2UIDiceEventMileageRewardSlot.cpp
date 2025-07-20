// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDiceEventMileageRewardSlot.h"

#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "BladeIIUtil.h"
#include "Retailer.h"

void UB2UIDiceEventMileageRewardSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UTextBlock, TB_RewardComplete);
	GET_SLOT(UHorizontalBox, HB_RewardSlot);
	GET_SLOT(UTextBlock, TB_PopupTitle);
	GET_SLOT(UWidgetSwitcher, WS_State);
}

void UB2UIDiceEventMileageRewardSlot::UpdateStaticText()
{
	if (TB_GetReward.IsValid())
	{
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
	}
		
	if (TB_RewardComplete.IsValid())
	{
		TB_RewardComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));
	}

	if (TB_PopupTitle.IsValid())
	{
		TB_PopupTitle->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemCount")), FText::AsNumber(CachedMileageInfo.dice_count)));
	}
}

void UB2UIDiceEventMileageRewardSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIDiceEventMileageRewardSlot::OnClickBTN_GetReward);
}

void UB2UIDiceEventMileageRewardSlot::OnClickBTN_GetReward()
{
	StartBtnBlockAnim_BP();

	data_trader::Retailer::GetInstance().RequestRewardDiceEventMileage(CachedMileageInfo.event_id, CachedMileageInfo.index);
}

void UB2UIDiceEventMileageRewardSlot::InitMileageRewardSlot(const b2network::B2mdDiceEventMileage& MileageInfo)
{
	CachedMileageInfo = MileageInfo;

	this->Init();

	InitRewardSlot();
}

void UB2UIDiceEventMileageRewardSlot::InitRewardSlot()
{
	const int32 iArrayMax = CachedMileageInfo.rewards.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (CachedMileageInfo.rewards.IsValidIndex(i) == false)
		{
			continue;
		}

		if (CachedMileageInfo.rewards[i]->count > 0)
		{
			if (auto* pCreatedWidget = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardIconClass))
			{
				pCreatedWidget->Init();
				pCreatedWidget->UpdateRewardIcon(CachedMileageInfo.rewards[i]->id, FText::AsNumber(CachedMileageInfo.rewards[i]->count));
				HB_RewardSlot->AddChild(pCreatedWidget);
			}
		}
	}
}

void UB2UIDiceEventMileageRewardSlot::UpdateMileageRewardSlot(const int32 iRolledDiceCount, const int32 MileageRewardedValue)
{
	bool IsPossibleRequestReward = false;

	if (iRolledDiceCount >= CachedMileageInfo.dice_count)
	{
		const int32 iRewardBitCheck = 1 << CachedMileageInfo.index;
		if (MileageRewardedValue & iRewardBitCheck)
		{
			MileageState = b2network::B2MissionStateType::REWARDED;
		}
		else
		{
			MileageState = b2network::B2MissionStateType::COMPLETED;
			IsPossibleRequestReward = true;
		}
	}
	else
	{
		MileageState = b2network::B2MissionStateType::PROGRESS;
	}

	if(BTN_GetReward.IsValid())
	{
		BTN_GetReward->SetIsEnabled(IsPossibleRequestReward);
	}

	if (TB_GetReward.IsValid())
	{
		if (MileageState == b2network::B2MissionStateType::REWARDED)
		{
			TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));
		}
		else
		{
			TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
		}
	}

	if (WS_State.IsValid())
	{
		WS_State->SetActiveWidgetIndex(MileageState);
	}
}