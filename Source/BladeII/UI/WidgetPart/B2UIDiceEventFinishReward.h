// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDiceEventFinishReward.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDiceEventFinishReward : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitDiceEventFinishReward(const b2network::B2mdDiceEventFinishReward& FinishReward);
	void UpdateState(int32 iFinishCount);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

private:
	void UpdateIMG_RewardIcon();

private:
	TWeakObjectPtr<UWidgetSwitcher>			WS_State;

	TWeakObjectPtr<UB2Image>				IMG_RewardIcon;

	TWeakObjectPtr<UTextBlock>				TB_FinishCount;
	TWeakObjectPtr<UTextBlock>				TB_RewardCount;

private:
	b2network::B2mdDiceEventFinishReward	CachedDiceEventFinishReward;
};
