// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDiceEventMileageSlot.h"

void UB2UIDiceEventMileageSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_State);
	GET_SLOT(UTextBlock, TB_Mileage);
	GET_SLOT(UProgressBar, PB_Progress);
}

void UB2UIDiceEventMileageSlot::InitMileageSlot(const b2network::B2mdDiceEventMileage& MileageInfo)
{
	CachedMileageInfo = MileageInfo;

	this->Init();
}

void UB2UIDiceEventMileageSlot::UpdateMileageSlot(const int32 iRolledDiceCount, int32 MileageRewardedValue)
{
	UpdateMileageState(iRolledDiceCount, MileageRewardedValue);
	UpdateMileageText(iRolledDiceCount);
}

void UB2UIDiceEventMileageSlot::UpdateMileageState(int32 iRolledDiceCount, int32 MileageRewardedValue)
{
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
		}
	}
	else
	{
		MileageState = b2network::B2MissionStateType::PROGRESS;
	}

	if (WS_State.IsValid())
	{
		WS_State->SetActiveWidgetIndex(MileageState);
	}
}

void UB2UIDiceEventMileageSlot::UpdateMileageText(int32 iRolledDiceCount)
{
	if (TB_Mileage.IsValid())
	{
		int32 iProgressCount = iRolledDiceCount;

		if (iProgressCount > CachedMileageInfo.dice_count)
		{
			iProgressCount = CachedMileageInfo.dice_count;
		}

		FText ftMileage = FText::FromString(FString::Printf(TEXT("<%d/%d>"), iProgressCount, CachedMileageInfo.dice_count));
		TB_Mileage->SetText(ftMileage);
	}
}

void UB2UIDiceEventMileageSlot::UpdateMileageProgressBar(int32 iRolledDiceCount)
{
	int32 iProgressBarCount = iRolledDiceCount;

	if (iProgressBarCount <= 0)
	{
		iProgressBarCount = 0;
	}
	else if (iProgressBarCount > ProgressBarMax)
	{
		iProgressBarCount = ProgressBarMax;
	}

	if (PB_Progress.IsValid())
	{
		float ProgressRate = static_cast<float>(iProgressBarCount) / static_cast<float>(ProgressBarMax);
		PB_Progress->SetPercent(ProgressRate);
	}
}
