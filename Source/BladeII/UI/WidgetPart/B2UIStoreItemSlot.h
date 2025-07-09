#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStoreProductCost.h"
#include "B2UIStoreItemSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButtonToBuy, int32, ProductId);
DECLARE_DYNAMIC_DELEGATE(FOnClickedButton);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreItemSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:

	virtual void Init() override;

private:

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:

	void CreateSlotWidget(int32 SlotWidgetType, class UMaterialInstance* ProductIcon, int32 SlotIconEffectType);
	void SetData(int32 InStoreProductId, class UMaterialInstance* ProductIcon, const FText& ProductName, int32 InItemCount, bool bPackage = false);
	void SetBonusData(bool bShowBonus, class UMaterialInstance* BonusIcon, const FText& BonusName, int32 InItemCount, int32 InBonusCount);
	void SetRestrictData(int32 ProductId, int32 RestrictType, int32 RestrictCount);
	void SetPeriodData(int32 ProductId);
	void SetProductMark(int32 ProductId);
	void SetPackageProductMark(int32 ProductId);
	void SetFundsSale(int32 DiscountRate);
	void SetCashBonus(int32 IncreaseRate);
	void SetCost(EStoreItemCost InCostType, int32 InCost, int32 InCalculatedCost);
	void SetSalePeriodData(bool bIsLimited, int64 EndTime);

	UFUNCTION()
		void OnClickedSlotButton();

private:

	FText GetRestrictTypeName(int32 RestrictType);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		int32 StoreProductId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		EStoreItemCost CostType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		TArray<TSoftClassPtr<class UB2UIStoreItemSlotIcon>>	StoreSlotWidgetArray;

private:

	// Product
	TWeakObjectPtr<UOverlay> OV_SlotWidget;
	TWeakObjectPtr<UOverlay> OV_SlotWidget_Dark;
	TWeakObjectPtr<UOverlay> OV_SlotWidget_Fame;
	TWeakObjectPtr<UImage> IMG_ItemIcon;
	TWeakObjectPtr<UB2Button> BTN_ItemToBuy;
	TWeakObjectPtr<UB2Button> BTN_GetReward;
	TWeakObjectPtr<UB2UIStoreProductCost> UIP_Cost;
	TWeakObjectPtr<UTextBlock> TB_ItemNameAndCount;
	
	// Bonus
	TWeakObjectPtr<UOverlay> OV_Bonus;
	TWeakObjectPtr<UImage> IMG_BonusItemIcon;
	TWeakObjectPtr<UTextBlock> TB_BonusItemName;
	TWeakObjectPtr<UTextBlock> TB_BonusItemCount;
	TWeakObjectPtr<UTextBlock> TB_BonusText;

	// Cash Bonus
	TWeakObjectPtr<UOverlay> OV_PreBonusItemCount;
	TWeakObjectPtr<UB2RichTextBlock> RTB_PreBonusItemCount;
	TWeakObjectPtr<UImage> IMG_Arrow;

	// Restrict
	TWeakObjectPtr<UWidgetSwitcher> SW_Restrict;
	TWeakObjectPtr<UTextBlock> TB_RestrictType;
	TWeakObjectPtr<UTextBlock> TB_RestrictCount;
	TWeakObjectPtr<UTextBlock> TB_RestrictComplete;

	// Product Mark
	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_Store_Event_Label;

	// Restrict Only used Package
	TWeakObjectPtr<UTextBlock> TB_LeftDay;

	// limited item sale
	TWeakObjectPtr<UOverlay> OV_DailyLimit;
	TWeakObjectPtr<UOverlay> OV_SalePeriod;
	TWeakObjectPtr<UTextBlock> TB_SalePeriod;
};