// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventMileageSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventMileageSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	void InitMileageSlot(const b2network::B2mdDiceEventMileage& MileageInfo);

public:
	void UpdateMileageSlot(const int32 iRolledDiceCount, int32 MileageRewardedValue);
	void UpdateMileageState(int32 iRolledDiceCount, int32 MileageRewardedValue);
	void UpdateMileageText(int32 iRolledDiceCount);
	void UpdateMileageProgressBar(int32 iRolledDiceCount);

public:
	int32 GetGoalDiceCount() { return CachedMileageInfo.dice_count; }

public:
	void SetProgressBarMax(int32 iMax) { ProgressBarMax = iMax; }

private:
	TWeakObjectPtr<UWidgetSwitcher>			WS_State;

	TWeakObjectPtr<UTextBlock>				TB_Mileage;

	TWeakObjectPtr<UProgressBar>			PB_Progress;

private:
	b2network::B2mdDiceEventMileage			CachedMileageInfo;

	int32 MileageState;

	int32 ProgressBarMax;
};
