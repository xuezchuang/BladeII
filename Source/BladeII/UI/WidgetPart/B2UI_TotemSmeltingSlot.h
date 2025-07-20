// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UI_TotemSmeltingStatus.h"
#include "B2UI_TotemSmeltingSlot.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSlotSmeltingClickDelgate, int32, OptionIndex);
/**
 * 
 */

 //////////////////////////////////////////////////////////////
 //
 // UB2UI_TotemSmeltingSlotHole
 //
 //////////////////////////////////////////////////////////////

UCLASS()
class BLADEII_API UB2UI_TotemSmeltingSlotHole : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	void InitSlotHole();
	void SetSlotHole(ETotemRefineState HoleState);

private:
	TWeakObjectPtr<UWidgetSwitcher>			WS_SmeltingSlotHole;
};

//////////////////////////////////////////////////////////////
//
// UB2UI_TotemSmeltingSlot
//
//////////////////////////////////////////////////////////////

UCLASS()
class BLADEII_API UB2UI_TotemSmeltingSlot : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetParentTotemStatue(class UB2UI_TotemSmeltingStatus* pTotemStatue) { TotemStatus = pTotemStatue; }

	void UpdateIndexIcon(const int32& InIndex);
	void UpdateOptionText(const FItemOption& TotemOption, const ETotemSubOptionType& TotemOptionType, const int32& ChanceCount);
	void UpdateSlotHole(const int32& MaxHoleNum, const FB2TotemSubOptionDetail& SubOptionDetail);
	void UpdateSmeltingCost(ETotemRefineCostType SmeltingMaterial, const ETotemGrade& Grade, const int32& SmeltingChance);
	void UpdateWidgetVisible(const int32& SmeltingChance);

private:
	void ClearCachedTotemSmeltingSlotHole();

	int32 GetSmeltingCostByGold(const ETotemGrade& Grade, const int32& SmeltingChance);
	int32 GetSmeltingCostByPiece(const ETotemGrade& Grade, const int32& SmeltingChance);

	FText GetSubOptionTypeText(ETotemSubOptionType InSubOptionType);

private:
	UFUNCTION()
	void OnClickBTN_Smelting();

public:
	FOnSlotSmeltingClickDelgate OnSlotSmeltingClickDelgate;

protected:
	UPROPERTY(EditAnywhere)
	FSlateColor SmeltingEnableColor;
	UPROPERTY(EditAnywhere)
	FSlateColor SmeltingNotEnoughConditionColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TotemSmeltingMaterialIcon")
	UMaterialInterface*									SmeltingMaterialIcon_Gold;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TotemSmeltingMaterialIcon")
	UMaterialInterface*									SmeltingMaterialIcon_Piece;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UITotemSmeltingSlotHole")
	TSubclassOf<class UB2UI_TotemSmeltingSlotHole>		UITotemSmeltingSlotHoleClass;

private:
	TWeakObjectPtr<UImage>									IMG_SlotIcon;
	TWeakObjectPtr<UB2RichTextBlock>						RTB_SmeltingBonusOption;
	TWeakObjectPtr<UB2RichTextBlock>						RTB_SmeltingChance;

	TWeakObjectPtr<UB2Button>								BTN_Smelting;

	TWeakObjectPtr<UHorizontalBox>							HB_SmeltingSlotHole;

	TWeakObjectPtr<UImage>									IMG_MaterialIcon;
	TWeakObjectPtr<UB2RichTextBlock>						RTB_SmeltingCost;
	
	TArray<TWeakObjectPtr<UB2UI_TotemSmeltingSlotHole>>		CachedTotemSmeltingSlotHole;

private:
	class UB2UI_TotemSmeltingStatus*						TotemStatus;

	ETotemSubOptionType										SubOptionType;
	
	bool													IsEnoughCost;
	
	int32													CachedOptionIndex;
	ETotemRefineCostType									CashedCostType;
};