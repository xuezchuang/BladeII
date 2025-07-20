// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UISelectiveLotteryCostumeSlot.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSlotCostumeClickDelgate, UB2UISelectiveLotteryCostumeSlot*, ClickedCostume);

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UISelectiveLotteryCostumeSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	// UWidget interface
	virtual void Init() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	// Costume info
	void BindDoc(const struct FB2Item& InItemData);
	void UnbindDoc();

	void SetCostumeGrade(const int32 InValue);
	void SetCostumeIcon(UMaterialInterface* InMaterial);
	void SetCostumeName(const FText& InText, const int32 InGrade);
	int32 GetItemRefID() { return BindedStruct.ItemRefID; }
	FB2Item* GetBindedItem() { return &BindedStruct; }

	// Select Info
	void SetSelected(bool InbSelected);
	bool GetSelected() const { return bSelected; }

	// Control
	void SetEnableReadingGlasses(bool Enable);

protected:
	virtual void CacheAssets() override;
	void SetDocStruct(const struct FB2Item& ItemStruct) { BindedStruct = ItemStruct; }

private:
	UFUNCTION()
	void OnClickBtnArea();

public:
	FOnSlotCostumeClickDelgate OnSlotCostumeClickDelgate;

protected:
	TWeakObjectPtr<UImage>						    IMG_CostumeGrade;
	TWeakObjectPtr<UImage>						    IMG_CostumeIcon;
	TWeakObjectPtr<UImage>						    IMG_Reading_Glasses;

	TWeakObjectPtr<UOverlay>						O_Selected;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_CostumeName;

	TWeakObjectPtr<UB2Button>						BTN_CostumeItemSlot;

private:
	struct FB2Item BindedStruct;

	bool bSelected;
};
