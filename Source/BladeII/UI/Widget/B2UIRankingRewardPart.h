// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2ButtonGoodInfoToolTip.h"
#include "B2UIRankingRewardPart.generated.h"


UCLASS()
class BLADEII_API UB2UIRankingRewardPart : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetItemInfo(const b2network::B2RewardPtr& RewardItem);
protected:

private:
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UImage>	   IMG_Item;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_Goods;
};
