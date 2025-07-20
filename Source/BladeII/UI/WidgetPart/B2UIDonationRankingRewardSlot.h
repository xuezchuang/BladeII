// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIDonationRankingRewardSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationRankingRewardSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetInfo(const struct FDonationPointRankingRewardInfo& InData);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UCanvasPanel> CP_RewardItem1;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon1;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Count1;

	TWeakObjectPtr<UCanvasPanel> CP_RewardItem2;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon2;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Count2;

	TWeakObjectPtr<UCanvasPanel> CP_RewardItem3;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon3;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Count3;

	TWeakObjectPtr<UCanvasPanel> CP_RewardItem4;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon4;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Count4;

	TWeakObjectPtr<UCanvasPanel> CP_RewardItem5;
	TWeakObjectPtr<class UB2UIRewardIcon> UIP_RewardIcon5;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Count5;

	TWeakObjectPtr<UImage> IMG_RankBG_1;
	TWeakObjectPtr<UImage> IMG_RankBG_2;
	TWeakObjectPtr<UImage> IMG_RankBG_3;
	TWeakObjectPtr<UTextBlock> TB_Ranking;
};
