// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDiceEventMileageRewardPopup.h"

#include "B2UIDiceEventMileageRewardSlot.h"

void UB2UIDiceEventMileageRewardPopup::InitMileageRewardPopup(const TArray<b2network::B2mdDiceEventMileage>& ArrayMileageInfo)
{
	CachedArrayMileageInfo.Empty();
	CachedArrayMileageInfo = ArrayMileageInfo;

	this->Init();

	InitMileageRewardSlotList();
}

void UB2UIDiceEventMileageRewardPopup::UpdateMileageRewardPopup(const int32 iRolledDiceCount, const int32 MileageRewardedValue)
{
	const int32 iArrayMax = ArrayMileageReward.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayMileageReward.IsValidIndex(i) == false)
		{
			continue;
		}

		ArrayMileageReward[i]->UpdateMileageRewardSlot(iRolledDiceCount, MileageRewardedValue);
	}
}

void UB2UIDiceEventMileageRewardPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UHorizontalBox, HB_RewardMileageSlotList);
}

void UB2UIDiceEventMileageRewardPopup::UpdateStaticText()
{

}

void UB2UIDiceEventMileageRewardPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDiceEventMileageRewardPopup::OnClickBTN_Close);
}

void UB2UIDiceEventMileageRewardPopup::OnClickBTN_Close()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIDiceEventMileageRewardPopup::InitMileageRewardSlotList()
{
	if (HB_RewardMileageSlotList.IsValid() == false)
	{
		return;
	}

	ArrayMileageReward.Empty();

	const int32 iArrayMax = CachedArrayMileageInfo.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (CachedArrayMileageInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		auto* pCreateMileageRewardSlot = CreateWidget<UB2UIDiceEventMileageRewardSlot>(GetOwningPlayer(), UIDiceEventMileageRewardSlotClass);
		if (pCreateMileageRewardSlot)
		{
			pCreateMileageRewardSlot->InitMileageRewardSlot(CachedArrayMileageInfo[i]);
			HB_RewardMileageSlotList->AddChild(pCreateMileageRewardSlot);
			ArrayMileageReward.Add(pCreateMileageRewardSlot);
		}
	}
}



