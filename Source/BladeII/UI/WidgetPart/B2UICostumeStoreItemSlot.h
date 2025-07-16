// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIWidgetBase.h"
#include "../../DataStore/B2ClientDataStore.h"

#include "B2UICostumeStoreItemSlot.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class BLADEII_API UB2UICostumeStoreItemSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	void SetIcon(class UMaterialInstance* ProductIcon);
	void SetCostumPackageData(const FStoreProductData& InStoreData, const FText& ProductName);
	void SetRestrictData(int32 ProductID, int32 PackageID);
	void SetPackageProductMark(int32 ProductId);

	UFUNCTION()
	void OnClickedSlotButton();

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

protected:
	int32 StoreProductId;
	FStoreProductData StoreData;

private:

	TWeakObjectPtr<UTextBlock> TB_HaveItem;
	TWeakObjectPtr<UTextBlock> TB_Limited;
	// Product
	TWeakObjectPtr<UOverlay> OV_SlotWidget;
	TWeakObjectPtr<UOverlay> O_Limited;
	TWeakObjectPtr<UOverlay> O_HaveItem;

	TWeakObjectPtr<UImage> IMG_ItemIcon;
	TWeakObjectPtr<UImage> IMG_ItemGrade;
	TWeakObjectPtr<UB2Button> BTN_ItemDetail;
	TWeakObjectPtr<UB2Button> BTN_ItemToBuy;
	TWeakObjectPtr<class UB2UIStoreProductCost> UIP_Cost;
	TWeakObjectPtr<UTextBlock> TB_ItemNameAndCount;
	TWeakObjectPtr<UTextBlock> TB_RestrictComplete;

	// Product Mark
	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_Store_Event_Label;
};
