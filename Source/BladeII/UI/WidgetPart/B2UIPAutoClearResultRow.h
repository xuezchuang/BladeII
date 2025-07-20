// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPAutoClearResultRow.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPAutoClearResultRow : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetInfo(int RepeatCount, int TotalGold, const TArray<struct FB2Item>& ItemInfos);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
		TSubclassOf<class UB2UISlotItem> ItemSlotClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
		int32 MaxColumn;

private:
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UTextBlock> TB_Gold;
	TWeakObjectPtr<UTextBlock> TB_GoldCount;
	TWeakObjectPtr<UUniformGridPanel> UG_Grid;

	TArray<TWeakObjectPtr<class UB2UISlotItem>> ItemSlots;
};
