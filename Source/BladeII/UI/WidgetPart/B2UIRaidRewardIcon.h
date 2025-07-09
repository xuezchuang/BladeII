// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIRaidRewardIcon.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRaidRewardIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void CacheAssets() override;

	void UpdateRewardIcon(int32 RewardID, const FText& Text);
	void UpdateRewardIcon(class UMaterialInterface* Image, const FText& Text);

	UMaterialInterface* GetIconMaterialFromItemTable(int32 RewardID);
protected:
	TWeakObjectPtr<UImage>		IMG_RewardMainIcon;
	TWeakObjectPtr<UTextBlock>	TB_RewardDetailInfo;
};
