#pragma once
#include "B2UIWidget.h"
#include "B2UIPackageStore.h"
#include "B2UIBackWidget.h"
#include "B2UIStorePackageDetailPopup.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStorePackageDetailPopupSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetInfo(int32 ItemID, UMaterialInterface* IconImage, int32 ItemType, FText ItemName, int32 Period, bool IsDaily = false);
	void SetRewardInfo(int32 RewardID, int32 Period, int32 RewardCount);

	FORCEINLINE int32 GetProductCount() { return ProductCount; }
	FORCEINLINE int32 GetTotalProductCount() { return ProductCount * PayPeriod; }

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UImage>			IMG_ItemIcon;
	TWeakObjectPtr<UPanelWidget>	P_Bonus;
	TWeakObjectPtr<UPanelWidget>	P_Daily;
	TWeakObjectPtr<UTextBlock>		TB_Bonus;
	TWeakObjectPtr<UTextBlock>		TB_Daily;
	TWeakObjectPtr<UTextBlock>		TB_Count;
	TWeakObjectPtr<UTextBlock>		TB_Name;
	TWeakObjectPtr<class UB2ButtonGoodInfoToolTip>	BTN_Goods;

	int32 ProductCount;
	int32 PayPeriod;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStorePackageDetailPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void CloseMe();
	void ShowMe();
	void SetPopupInfo(int32 ProuctID, int32 popupType);
	void SetPackageTypeInfo(int32 PackageType);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void SetPopupVisible(bool IsVisible);

	void CreateRewardIcons();
	void DestroyRewardIcons();

	void SetMainPackageInfo(int32 PackageID, int32 CostType, int32 Cost, int32 CalculatedCost);
	void SetBasicListInfo(const FStoreProductData* ProductData);
	void SetBoostListInfo(const FStoreProductData* ProductData);

	UMaterialInstance* GetProductIcon(int32 ProductId);
	FText GetProductName(int32 ProductId);

	UFUNCTION()
		void OnClickBtnClose();

	UFUNCTION()
		void OnClickBTN_Buy();

	UFUNCTION()
		void OnClickBTN_Detail();

protected:
	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIStorePackageDetailPopupSlot> PopupSlot;
	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIStoreProductIconData> ProductIconData;

	/** Created based on actual reward items using RewardTemplate */
	TArray<class UB2UIStorePackageDetailPopupSlot*> CreatedRewardIcons;

private:
	TWeakObjectPtr<UImage>				IMG_Package;

	TWeakObjectPtr<UB2Button>			BTN_Buy;
	TWeakObjectPtr<UB2Button>			BTN_Detail;
	TWeakObjectPtr<UB2Button>			BTN_Close;

	TWeakObjectPtr<UTextBlock>			TB_Title;
	TWeakObjectPtr<UTextBlock>			TB_TotalPrice;
	TWeakObjectPtr<UB2RichTextBlock>	TB_BoostDesc;
	TWeakObjectPtr<UTextBlock>			TB_ItemName;

	TWeakObjectPtr<UTextBlock>			STB_PocketItem;
	TWeakObjectPtr<UTextBlock>			TB_UsedItem;
	TWeakObjectPtr<UB2RichTextBlock>	TB_WarningSummury;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_BuyDesc;
	TWeakObjectPtr<UTextBlock>			STB_Detail;
	TWeakObjectPtr<UTextBlock>			STB_Buy;

	TWeakObjectPtr<UWidgetSwitcher>		WS_Content;
	TWeakObjectPtr<UUniformGridPanel>	GP_PackageList;

	TWeakObjectPtr<class UB2UIStoreProductCost>	UIP_Cost;

	TArray<TWeakObjectPtr<class UB2UIStorePackageDetailPopupSlot>>	BoostItemWidgets;

public:
	FButtonOneParamDelegate ButtonOneParamDelegate;

private:
	int32 CurrentStoreID;

};
