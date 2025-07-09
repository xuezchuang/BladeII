// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIHeroTowerBattleSub.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHeroTowerBattleSub : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void CacheAssets() override;
	
protected:
	TWeakObjectPtr<UTextBlock> TB_Floor;
	TWeakObjectPtr<UTextBlock> TB_ElapsedTime;
	TWeakObjectPtr<UHorizontalBox> HB_Rewards;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UIRewardIcon> RewardItemClass;

};
