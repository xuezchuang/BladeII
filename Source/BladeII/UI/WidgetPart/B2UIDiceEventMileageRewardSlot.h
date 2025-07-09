// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventMileageRewardSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventMileageRewardSlot : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
	void OnClickBTN_GetReward();

public:
	void InitMileageRewardSlot(const b2network::B2mdDiceEventMileage& MileageInfo);
	void InitRewardSlot();

public:
	void UpdateMileageRewardSlot(const int32 iRolledDiceCount, const int32 MileageRewardedValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIDiceEventMileageRewardSlot")
	void StartBtnBlockAnim_BP();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIRewardIcon> RewardIconClass;
	
private:
	TWeakObjectPtr<UB2Button>				BTN_GetReward;
	TWeakObjectPtr<UTextBlock>				TB_GetReward;
	TWeakObjectPtr<UTextBlock>				TB_RewardComplete;
	TWeakObjectPtr<UHorizontalBox>			HB_RewardSlot;
	TWeakObjectPtr<UTextBlock>				TB_PopupTitle;
	TWeakObjectPtr<UWidgetSwitcher>			WS_State;
	
private:
	b2network::B2mdDiceEventMileage	CachedMileageInfo;

	int32 MileageState;
};
