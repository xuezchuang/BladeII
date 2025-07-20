// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIDonationProductCost.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationProductCost : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetCost(int32 InCost, int32 FreeCount = 0);

protected:
	virtual void CacheAssets() override;

private:
#if WITH_EDITOR
	void EditorOnPostLoad();
#endif

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		TSoftObjectPtr<class UMaterialInterface> GemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		TSoftObjectPtr<class UMaterialInterface> GoldIcon;

private:
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UTextBlock> TB_Cost;
	TWeakObjectPtr<UB2UIWidgetBase> UIP_RedDot;
};
