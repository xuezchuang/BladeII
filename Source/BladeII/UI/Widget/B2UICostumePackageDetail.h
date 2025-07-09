// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UICostumePackageDetail.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEII_API UB2UICostumePackageDetail : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	void SetPosition(const FVector2D v2PopupPosition, const float fPopupSizeX);
	void SetPackageData(int32 ProductID, const FText& ProductName, const FText &ProductDescription);
	void RefreshData();
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClickClose();

	UFUNCTION()
		void OnClickBuyToItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIHeaderBezel")
		float fTopBezel_BP;

protected:
	int32 StoreProductID;

	TWeakObjectPtr<UCanvasPanel> CP_PopupMain;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_PackageName;
	TWeakObjectPtr<UTextBlock> TB_PackageDescription;
	TWeakObjectPtr<UTextBlock> TB_BuyDescription;
	TWeakObjectPtr<UTextBlock> TB_Buy;
	TWeakObjectPtr<UTextBlock> TB_Period;

	TWeakObjectPtr<UTextBlock> TB_RestrictComplete;

	TArray<TWeakObjectPtr<class UB2UICostumeItemSlot>> UIP_CostumeItemSlots;

	TWeakObjectPtr<UB2Button> BTN_ItemToBuy;
	TWeakObjectPtr<UB2Button> BTN_Close;
	
	TWeakObjectPtr<class UB2UIStoreProductCost> UIP_Cost;
	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_Store_Event_Label; 
};
