// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UI_TotemSmeltingStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UI_TotemSmeltingStatus : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	void UpdateTotemSmeltingStatus(const FB2Totem& InTotem);
	void SetTotemSmeltingMaterial(ETotemRefineCostType SmeltingMaterial);

	void SetSmeltingButtonLockState(bool LockValue) { IsLockSmelting = LockValue; }
	bool GetSmeltingButtonLockState() { return IsLockSmelting; }

	UFUNCTION()
	void SmeltingTotem(int32 OptionIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmeltingOptionType")
		TArray<ETotemSubOptionType> SubOptionType;

private:
	TWeakObjectPtr<UB2RichTextBlock>					RTB_IncreaseOption;
	TWeakObjectPtr<UB2RichTextBlock>					RTB_DecreaseOption;

	TArray<TWeakObjectPtr<class UB2UI_TotemSmeltingSlot>>		UIP_TotemSmeltingSlotBPs;

private:
	bool												IsLockSmelting;

	FB2Totem											CachedB2TotemInfo;
	ETotemRefineCostType								CachedB2SmeltingMaterial;
};
