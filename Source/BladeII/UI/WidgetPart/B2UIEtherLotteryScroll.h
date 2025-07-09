// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIEtherLotteryScroll.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLotteryScroll : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetEtherLotteryInfo(int32 InStar, int32 InTier);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	int32 GetMaxRow(int32 MaxSlot);
	TArray<int32> GetSlotIDs(const TArray<int32>& InTypes, int32 RowIndex, int32 MaxSlot);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ether")
		TSubclassOf<class UB2UIEtherLotteryRow> EtherLotteryRowSlotClass;

private:
	TArray<TWeakObjectPtr<class UB2UIEtherLotteryRow>> UIP_EtherLotteryRowSlots;
	TWeakObjectPtr<UB2ScrollBox> X_SB_Scroll;
	uint32 SelectEtherLotterySlotTicket;

};
