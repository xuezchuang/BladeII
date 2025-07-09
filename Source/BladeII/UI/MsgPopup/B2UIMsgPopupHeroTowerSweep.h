// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupHeroTowerSweep.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupHeroTowerSweep : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	virtual void SetAddComment(const FText& comment);
	void SetAddComment2(const FText& comment);
	void SetAddCommentForHeroTower(const TMap<int, FRewardItem>& Rewards);
	void SetTipText(const FText& comment);
	void SetCounterDungeonTip(const FText& comment);

protected:
	TArray<TWeakObjectPtr<UB2RichTextBlock>>		 TB_AddItems;
	TArray<TWeakObjectPtr<UB2ButtonGoodInfoToolTip>> BTN_Goods;
	TArray <TWeakObjectPtr<UImage>>					 IMG_Items;
	TWeakObjectPtr<UTextBlock>						 TB_TipText;
	TArray<TWeakObjectPtr<UPanelWidget>>			 P_RewardItem;

	TWeakObjectPtr<UB2RichTextBlock>				TB_CounterDungeonTip;
};
