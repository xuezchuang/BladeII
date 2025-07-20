// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "B2UIEventSlotStageClear.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotStageClear : public UB2UIEventSlot
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
	void OnClickBTN_GetRewardPossible();

public:
	void InitEventSlotStageClear(const b2network::B2mdStageClearEvent& EventRewardInfo);
	virtual void UpdateVisibleState() override;
	
private:
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

	//progress
	TWeakObjectPtr<UTextBlock>				TB_EventExplainProgress;
	TWeakObjectPtr<UTextBlock>				TB_GetRewardBtnProgress;

	//possible
	TWeakObjectPtr<UTextBlock>				TB_EventExplainPossible;
	TWeakObjectPtr<UTextBlock>				TB_GetRewardBtnPossible;
	TWeakObjectPtr<UB2Button>				BTN_GetRewardPossible;

	//complete
	TWeakObjectPtr<UTextBlock>				TB_EventExplainComplete;
	TWeakObjectPtr<UTextBlock>				TB_GetRewardBtnComplete;
	
	TWeakObjectPtr<UWidgetSwitcher>			WS_EventSlot;

private:
	b2network::B2mdStageClearEvent			ChachedEventRewardInfo;
};
