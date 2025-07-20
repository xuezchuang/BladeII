// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventMileageRewardPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventMileageRewardPopup : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitMileageRewardPopup(const TArray<b2network::B2mdDiceEventMileage>& ArrayMileageInfo);
	void UpdateMileageRewardPopup(const int32 iRolledDiceCount, const int32 MileageRewardedValue);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
	void OnClickBTN_Close();

	void InitMileageRewardSlotList();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIDiceEventMileageRewardSlot>		UIDiceEventMileageRewardSlotClass;

private:
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UHorizontalBox>	HB_RewardMileageSlotList;

	TArray<TWeakObjectPtr<class UB2UIDiceEventMileageRewardSlot>>	ArrayMileageReward;

private:
	TArray<b2network::B2mdDiceEventMileage>	CachedArrayMileageInfo;
};
