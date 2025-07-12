// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "../B2RichTextBlock.h"
#include "../Widget/B2Image.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2UIEventSlotPointShopLimited.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotPointShopLimited : public UB2UIEventSlot
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	void InitEventSlotPointShopLimited(const b2network::B2mdPointEventReward& EventRewardInfo);

private:
	void InitRewardIcon();

public:
	virtual void UpdateVisibleState() override;
	void Update_FromTapGroupID(const int32 iTapGroupID);
	void Update_FromAccruePoint(const int32 iAccruePoint);

private:
	void UpdateRewardPossibleCountText(const int32 iRewardedCount);

public:
	void SetRewardedCount(const int32 iRewardedCount) { CachedRewardedCount = iRewardedCount; }

private:
	UFUNCTION()
	void OnClickBTN_GetReward();

private:
	TWeakObjectPtr<UB2Button>				BTN_GetReward;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_BtnProgress;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_BtnComplete;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_ItemName;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_ItemCount;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_RewardExplain;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_RewardPossibleCount;

	TWeakObjectPtr<UWidgetSwitcher>			WS_EventState;

	TWeakObjectPtr<UB2Image>					IMG_RewardIcon;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_RewardIconItemCount;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_RewardIcon;

private:
	b2network::B2mdPointEventReward			CachedEventRewardInfo;
	int32									CachedRewardedCount;
};
