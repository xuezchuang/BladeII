// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "B2UIEventSlotTenLottery.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotTenLottery : public UB2UIEventSlot
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
	void OnClickBTN_GetRewardPossible();

	void InitRewardIconList(const b2network::B2mdTenLotteryEvent& EventRewardInfo);

public:
	void InitEventSlotTenLottery(const b2network::B2mdTenLotteryEvent& EventRewardInfo);
	void SetEventSlotState(const int32 TenLotteryCount, const TArray<int32>& ArrayRewardedIndex);
	virtual void UpdateVisibleState() override;
	void SetGoalCountText(const int32 Count);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIRewardIcon>	RewardIconClass;

private:
	TWeakObjectPtr<UHorizontalBox>		HB_RewardIconList;

	TWeakObjectPtr<UTextBlock>			TB_ProgressAndGoalCount;

	//progress
	TWeakObjectPtr<UTextBlock>			TB_EventExplainProgress;
	TWeakObjectPtr<UTextBlock>			TB_GetRewardBtnProgress;
	TWeakObjectPtr<UTextBlock>			TB_GoalCountProgress;

	//possible
	TWeakObjectPtr<UTextBlock>			TB_EventExplainPossible;
	TWeakObjectPtr<UTextBlock>			TB_GetRewardBtnPossible;
	TWeakObjectPtr<UB2Button>			BTN_GetRewardPossible;
	TWeakObjectPtr<UTextBlock>			TB_GoalCountPossible;

	//complete
	TWeakObjectPtr<UTextBlock>			TB_EventExplainComplete;
	TWeakObjectPtr<UTextBlock>			TB_GetRewardBtnComplete;
	TWeakObjectPtr<UTextBlock>			TB_GoalCountComplete;

	TWeakObjectPtr<UWidgetSwitcher>		WS_EventSlot;

private:
	b2network::B2mdTenLotteryEvent		ChachedEventRewardInfo;
};
