// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIMsgPopupGuestWarning.h"
#include "B2UIPackageStore.generated.h"

DECLARE_DELEGATE_OneParam(FButtonOneParamDelegate, int32)

UENUM(BlueprintType)
enum class EPACKAGE_TAB_TYPE : uint8
{
	mNULL =0,
	/* Basic */
	BASIC = 1,
	EQUIPMENT,
	GROWTH,
	HERO,
	BOOST,

	LEVELUP,
	FLATRATE,

	NONE
};

UENUM(BlueprintType)
enum class EPACKAGE_PANEL_TYPE : uint8
{
	BASIC = 0,
	FLATRATE,
	LEVELUP,

	NONE
};

UCLASS()
class BLADEII_API UB2UIPackageStore : public UB2UIWidget
{
	GENERATED_BODY()

protected:

	UB2UIPackageStore(const FObjectInitializer& ObjectInitializer);
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	virtual void OnOpen(bool RightNow = false);
	virtual void OnClose(bool RightNow = false);

	void OnClickTabButtonDelegate(int32 ButtonIndex);
	void OpenPopupConfirmToBuyItem(int32 ProductId);
	void InitPanelWidgetByCategory();
	void UpdatePanelWidget();

	void DoMarkRedDot_Package();

private:

	void CreateStoreCategoryTab();
	FText GetStoreCategoryName(int32 StoreCategory);
	void SetSelectedStoreCategory(int32 StoreCategory);
	void RegisterRedDot(int32 CurrentTabType, class UB2UICommonTab* TabWidget);

	UFUNCTION()
		bool RedDotCondition_FlatRate();
	UFUNCTION()
		bool RedDotCondition_Level();
	UFUNCTION()
		void OnClickProbability();

protected:

	void ConditionalCreatePackageDetailPopup();
	void ConditionalCreateGuestWarningPopup();

	void DestroyPackageDetailPopup();
	void DestroyGuestWarningPopup();

	void SubmitConfirmToBuyItem(int32 StoreProductID);
	void SubmitConfirmToBuyGem(int32 StoreProductID);
	void ConfirmToBuyGem();

	void CompletedPurchaseBuyGem(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct);

	UFUNCTION()
		void SubmitConfirmToBuy(int32 StoreProductID);
	UFUNCTION()
		void ClosePopupConfirmToBuyItem();

	UFUNCTION()
		void OpenGuestWarningPopup();
	UFUNCTION()
		void CloseGuestWarningPopup();
	UFUNCTION()
		void BuyGuestWarningPopup();

protected:

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UICommonTab> DefaultStoreTab;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIStorePackageDetailPopup> UIP_PackageDetailPopup_Class;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<UB2UIMsgPopupGuestWarning> UIP_GuestWarningPopup_Class;
	
private:

	UPROPERTY(Transient)
		UB2UIStorePackageDetailPopup* Created_UIP_PackageDetailPopup;
	UPROPERTY(Transient)
		UB2UIMsgPopupGuestWarning* Created_UIP_GuestWarningPopup;

	int32 CurrentStoreCategory;

	TWeakObjectPtr<UCanvasPanel> X_CP_PackageDetailPopup_Panel;
	TWeakObjectPtr<UCanvasPanel> X_CP_GuestWarningPopup_Panel;

	TWeakObjectPtr<UWidgetSwitcher> WS_Content;
	TWeakObjectPtr<UVerticalBox> VB_TabList;

	TWeakObjectPtr<class UB2UIPackageTypePanel_Basic>		UI_Package_Basic;
	TWeakObjectPtr<class UB2UIPackageTypePanel_Flatrate>	UI_Package_Flatrate;
	TWeakObjectPtr<class UB2UIPackageTypePanel_Level>		UI_Package_LevelUp;

	TWeakObjectPtr<UB2Button> BTN_Probability;
	TWeakObjectPtr<UTextBlock> TB_Probability;

	int32 SelectedProductID;

	uint32 SelectItemToBuyTicket;
	uint32 DeliveryBuyShopProductTicket;
	uint32 DeliveryCheckRedDotTicket;
};
