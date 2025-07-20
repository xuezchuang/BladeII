// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "B2UIEventSlotLevelup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotLevelup : public UB2UIEventSlot
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
		void OnClickBTN_GetReward();

public:
	void InitEventLevelupPart(const b2network::B2mdLevelUpEvent& EventRewardInfo);
	virtual void UpdateVisibleState() override;

private:
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

	TWeakObjectPtr<UB2Button>				BTN_GetReward;

	TWeakObjectPtr<UTextBlock>				TB_GetReward;

	TWeakObjectPtr<UTextBlock>				TB_GetRewardPossible;

	TWeakObjectPtr<UTextBlock>				TB_GetRewardImPossible;

	TWeakObjectPtr<UTextBlock>				TB_CompleteReward;

	TWeakObjectPtr<UOverlay>				O_GetRewardPossible;

	TWeakObjectPtr<UOverlay>				O_GetRewardImPossible;

private:
	b2network::B2mdLevelUpEvent				CachedEventRewardInfo;
};
