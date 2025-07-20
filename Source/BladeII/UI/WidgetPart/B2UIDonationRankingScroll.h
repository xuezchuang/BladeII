// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIDonationRankingScroll.generated.h"

/**
 *
 */
UCLASS()
class UB2UIDonationRankingScroll : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void UpdateScroll();

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	TSubclassOf<class UB2UIDonationRankingScrollSlot> RankingSlotClass;

private:
	TArray<TWeakObjectPtr<class UB2UIDonationRankingScrollSlot>> UIP_RankingScrollSlots;
	TWeakObjectPtr<UB2ScrollBox> X_SB_RankingScrollBox;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_ScrollTitle;
};
