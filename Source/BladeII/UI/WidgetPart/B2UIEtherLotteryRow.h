// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIEtherLotteryRow.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLotteryRow : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetEtherLotteryRowInfo(TArray<int32> InTypeIDs, int32 InStar, int32 InTier, UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox);
	void SetSelectedVisibility(int32 InType);

protected:
	virtual void CacheAssets() override;
	
private:
	TArray<TWeakObjectPtr<class UB2UIEtherLotterySlot>> X_EtherLotterySlots;

};
