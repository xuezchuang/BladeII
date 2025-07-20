// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "../B2RichTextBlock.h"
#include "B2UIEventSlotPointConsume.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotPointConsume : public UB2UIEventSlot
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnClickBTN_GetReward();
	UFUNCTION()
	void OnPressBTN_DecreaseRewardAmount();
	UFUNCTION()
	void OnPressBTN_IncreaseRewardAmount();
	UFUNCTION()
	void OnReleaseBTN_ChangeRewardAmount();

	void CheckPressedChangeRewardBTN(const float InDeltaTime);

private:
	void UpdateRewardSlotText();
	void UpdateRewardAmount(const bool bTimeOver);

public:
	virtual void UpdateVisibleState() override;
	void InitEventSlotPointConsume(const b2network::B2mdPointEventReward& EventRewardInfo);
	void CheckEventSlotState(const int32 iAcquireEventPoint);

private:
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_ItemName;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_NeedPoint;

	TWeakObjectPtr<UB2Button>				BTN_DecreaseReward;
	TWeakObjectPtr<UB2Button>				BTN_IncreaseReward;

	TWeakObjectPtr<UWidgetSwitcher>			WS_EventSlot;

	//progress
	TWeakObjectPtr<UB2RichTextBlock>		RTB_BtnProgress;

	//complete
	TWeakObjectPtr<UB2RichTextBlock>		RTB_BtnComplete;
	TWeakObjectPtr<UB2Button>				BTN_GetReward;

private:
	int32									CurrentRewardAmount;
	float									SumPressedTime;
	float									ChangeRewardAmountTime;
	EEventSlotPointConsumeBTNState			ChangeRewardAmountBTNState;

	b2network::B2mdPointEventReward			CachedEventRewardInfo;
	int32									CachedAcquireEventPoint;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float									ChangeRewardAmountTickTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32									ChangeRewardAmountTickMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32									ChangeRewardAmountMax;
};
