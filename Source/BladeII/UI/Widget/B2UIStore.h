#pragma once

#include "BladeIIGameMode.h"
#include "B2UIWidget.h"
#include "B2UIStoreItemSlot.h"
#include "B2UIStoreMileage.h"
#include "B2UIMsgPopupGuestWarning.h"
#include "B2UIStoreMileageRewardPopup.h"
#include "B2UIStorePackageDetailPopup.h"
#include "B2UIStore.generated.h"


UENUM(BlueprintType)
enum class EB2LOC_PRODUCTNAME_CAT : uint8 // For Blueprint BladeIIGetLOCText util
{
	GENERAL			UMETA(DisplayName = "General Text"),
	STORE			UMETA(DisplayName = "Store Text"),
	ITEM_NAME		UMETA(DisplayName = "Item Name Text"),
	// Check EB2LOC_CAT_ToString once you add new entry here.
};

UENUM(BlueprintType)
enum class ESTORE_BUYPOPUP_WIDGET_TYPE : uint8
{
	LIST = 0,
	BOOST,

	END
};

UENUM(BlueprintType)
enum class ESTORE_SLOT_WIDGET_TYPE : uint8

{
	BLADE_POINT		UMETA(DisplayName = "Blade Point"),
	GEM				UMETA(DisplayName = "Gem"),
	GOLD			UMETA(DisplayName = "Gold"),
	DARKCRYSTAL		UMETA(DisplayName = "DarkCrystal"),
	PACKAGE			UMETA(DisplayName = "Package"),
	FAME_POINT		UMETA(DisplayName = "Fame Point"),

	NONE			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class ESTORE_SLOT_EFFECT_TYPE : uint8

{
	EFFECT_1		UMETA(DisplayName = "Effect 1"),
	EFFECT_2		UMETA(DisplayName = "Effect 2"),
	EFFECT_3		UMETA(DisplayName = "Effect 3"),

	NONE			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EPACKAGE_TYPE : uint8
{
	SevenDays_Summon	UMETA(DisplayName = "SevenDays Summon"),
	Gold_Boost			UMETA(DisplayName = "Gold Boost"),
	BladePoint_Boost	UMETA(DisplayName = "BladePoint Boost"),
	Exp_Boost			UMETA(DisplayName = "Exp Boost"),
	Time_Boost			UMETA(DisplayName = "Time Boost"),
	Weekly_Flatrate		UMETA(DisplayName = "Weekly Flatrate"),
	Monthly_Flatrate	UMETA(DisplayName = "Monthly Flatrate"),
	Monthly_Flatrate2	UMETA(DisplayName = "Monthly Flatrate2"),
	Levelup_Package		UMETA(DisplayName = "Levelup Package"),
	Normal_Package		UMETA(DisplayName = "Normal Package"),

	None				UMETA(DisplayName = "None"),
};

USTRUCT(BlueprintType)
struct FB2UIStoreProductIconElement
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	int32 ProductId;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	ESTORE_SLOT_WIDGET_TYPE SlotWidget;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	class UMaterialInstance* ProductIcon;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	EB2LOC_PRODUCTNAME_CAT LocCategory;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	FString ProductNameKey;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	FString ProductDescriptionKey;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	ESTORE_SLOT_EFFECT_TYPE IconEffect;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	ESTORE_BUYPOPUP_WIDGET_TYPE PopupType;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	EPACKAGE_TYPE PackageType;

};

USTRUCT(BlueprintType)
struct FB2UIStoreBonusIconElement
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	EStoreItemType BonusType;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	class UMaterialInstance* BonusIcon;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	EB2LOC_PRODUCTNAME_CAT LocCategory;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	FString BonusNameKey;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreProductIconData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TArray<FB2UIStoreProductIconElement> ProductIconDataArray;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TArray<FB2UIStoreBonusIconElement> BonusIconDataArray;

	UFUNCTION(BlueprintPure, Category = "BladeII")
	class UMaterialInstance* GetProductIcon(int32 ProductId, int32 OptionalItemId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	ESTORE_SLOT_EFFECT_TYPE	GetProductIconEffect(int32 ProductId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	ESTORE_SLOT_WIDGET_TYPE GetProductWidget(int32 ProductId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	FText GetProductName(int32 ProductId, int32 OptionalItemId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	FText GetProductDescription(int32 ProductId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	void GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	ESTORE_BUYPOPUP_WIDGET_TYPE GetPopupWidgetType(int32 ProductId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	EPACKAGE_TYPE GetPackageType(int32 ProductId);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	int32 GetProductId(EPACKAGE_TYPE PakageType);

private:
	FB2UIStoreProductIconElement* GetProductIconData(int32 ProductId);
	FB2UIStoreProductIconElement* GetProductIconData(EPACKAGE_TYPE PakageType);
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStore : public UB2UIWidget
{
	GENERATED_BODY()

public:

	UB2UIStore(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void OnOpen(bool RightNow = false);
	virtual void OnClose(bool RightNow = false);

	virtual void RegisterUIMarkForRedDot() override;

	void CreateStoreCategoryTab();
	void SetGoodsTabEvent();

	////////////////////////////////////////////////////
	// Store Tab
	void OnClickTabButtonDelegate(int32 ButtonIndex);

	UFUNCTION()
	void OnClickTabPackage();
	UFUNCTION()
	void OnClickTabDarkCrystal();
	UFUNCTION()
	void OnClickTabGoods();
	UFUNCTION()
	void OnClickTabFamePoint();

	////////////////////////////////////////////////////
	// Goods Tab
	UFUNCTION()
	void OnClickGoodsTabBladePoint();
	UFUNCTION()
	void OnClickGoodsTabGem();
	UFUNCTION()
	void OnClickGoodsTabGold();

	FText GetStoreCategoryName(int32 StoreCategory);
	ESTORE_SLOT_WIDGET_TYPE GetProductWidget(int32 ProductId);
	class UMaterialInstance* GetProductIcon(int32 ProductId, int32 OptionalItemId);
	ESTORE_SLOT_EFFECT_TYPE GetProductIconEffect(int32 ProductId);
	FText GetProductName(int32 ProductId, int32 OptionalItemId);
	void GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId);

public:

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<class UB2UICommonTab> DefaultStoreTab;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIStoreItemSlot> DefaultItemSlot;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIStoreProductIconData> ProductIconData;

	void SetTab();

private:

	void SetProductListItem(const FStoreProductData* ProductItem);

	void SetGoodsCategory(int32 GoodsCategory);
	void SetSelectedStoreCategory(int StoreCategory);
	void SetSelectedGoodsCategory(int32 GoodsCategory);
	void SetTabStoreMileage();

protected:

	UFUNCTION()
	void OpenMileageGuidePopup();
	UFUNCTION()
	void CloseMileageGuidePopup();
	UFUNCTION()
	void SetOpenRewardPopupBClass();
	UFUNCTION()
	void SetOpenRewardPopupAClass();
	UFUNCTION()
	void SetOpenRewardPopupSClass();

	void OpenRewardPopup(EUIStoreMileageGrade MileageGrade);
	void OpenPopupConfirmToBuyItem(int32 ProductId);

	void SetCostData_ConfirmBuy(EStoreItemCost CostType, int32 Cost, int32 CalculatedCost);
	void SetBonusData_ConfirmBuy(EStoreItemCost CostType, bool bShowBonus, EStoreItemType BonusItemType, int32 InItemCount, int32 InBonusCount);

	void OnResponseRewardShopMileage(FB2ResponseRewardShopMileagePtr ResponseRewardShopMileage);
	void OnMileageChanged();

	void SetForcedBlockHeader(bool IsBlock);

	void ConfirmToBuyGem();
	void CompletedPurchaseBuyGem(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct);

	UFUNCTION()
	void ClosePopupConfirmToBuyItem();
	UFUNCTION()
	void CloseConfirmToBuyPopupGemPopup();
	UFUNCTION()
	void CloseConfirmToBuyPopupNormalPopup();
	UFUNCTION()
	void SubmitConfirmToBuyItem();
	UFUNCTION()
	void SubmitConfirmToBuyGem();
	UFUNCTION()
	void DetailConfirmToBuyItem();

	UFUNCTION()
	void OpenGuestWarningPopup();
	UFUNCTION()
	void CloseGuestWarningPopup();
	UFUNCTION()
	void BuyGuestWarningPopup();

	UFUNCTION()
	bool RedDotCondition_BladePoint();
	UFUNCTION()
	bool RedDotCondition_Gem();
	UFUNCTION()
	bool RedDotCondition_Gold();
	UFUNCTION()
	bool RedDotCondition_DarkCrystal();
	UFUNCTION()
	bool RedDotCondition_Package();

private:

	////////////////////////////////////////////////////
	// Store Tab
	TWeakObjectPtr<UVerticalBox> VB_TabList;

	////////////////////////////////////////////////////
	// Goods Tab
	TWeakObjectPtr<UPanelWidget>	O_GoodsTab;
	TWeakObjectPtr<class UB2UICommonTab> GoodsTab_BladePoint;
	TWeakObjectPtr<class UB2UICommonTab> GoodsTab_Gem;
	TWeakObjectPtr<class UB2UICommonTab> GoodsTab_Gold;

	// List
	TArray<TWeakObjectPtr<UB2UIWidgetBase>> ItemSlots;
	TWeakObjectPtr<UHorizontalBox> HB_ItemList;

	// Mileage
	TWeakObjectPtr<UCanvasPanel> CP_Mileage;
	TWeakObjectPtr<UB2UIStoreMileage> UIP_StoreMileage;

	TWeakObjectPtr<UB2Button> BTN_MileageGuide;
	TWeakObjectPtr<UB2Button> BTN_MileageGuidePopupClose;

	//////////////////////////////////////////////////////////////////
	// Dynamically loaded and created UIPs..

	TWeakObjectPtr<UCanvasPanel> X_CP_MileageRewardPopup_Panel;
	TWeakObjectPtr<UCanvasPanel> X_CP_ConfirmToBuyPopup_Gem_Panel;
	TWeakObjectPtr<UCanvasPanel> X_CP_ConfirmToBuyPopup_Normal_Panel;
	TWeakObjectPtr<UCanvasPanel> X_CP_MileageGuidePopup_Panel;
	TWeakObjectPtr<UCanvasPanel> X_CP_GuestWarningPopup_Panel;

protected:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2UIStoreMileageRewardPopup> UIP_MileageRewardPopup_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UIBackWidgetBase> UIP_ConfirmToBuyPopup_Gem_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UIBackWidgetBase> UIP_ConfirmToBuyPopup_Normal_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UIBackWidgetBase> UIP_MileageGuidePopup_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2UIMsgPopupGuestWarning> UIP_GuestWarningPopup_Class;

private:

	UPROPERTY(Transient)
	UB2UIStoreMileageRewardPopup* Created_UIP_MileageRewardPopup;
	UPROPERTY(Transient)
	class UB2UIBackWidgetBase* Created_UIP_ConfirmToBuyPopup_Gem;
	UPROPERTY(Transient)
	class UB2UIBackWidgetBase* Created_UIP_ConfirmToBuyPopup_Normal;
	UPROPERTY(Transient)
	class UB2UIBackWidgetBase* Created_UIP_MileageGuidePopup;
	UPROPERTY(Transient)
	UB2UIMsgPopupGuestWarning* Created_UIP_GuestWarningPopup;

protected:
	// For some dynamically created UIP..
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UIRewardIcon> RewardIconTemplate;

	void ConditionalCreateMileageRewardPopup();
	void ConditionalCreateConfirmToBuyPopup_Gem();
	void ConditionalCreateConfirmToBuyPopup_Normal();
	void ConditionalCreateMileageGuidePopup();
	void ConditionalCreateGuestWarningPopup();

	void DestroyMileageRewardPopup();
	void DestroyConfirmToBuyPopup_Gem();
	void DestroyConfirmToBuyPopup_Normal();
	void DestroyMileageGuidePopup();
	void DestroyGuestWarningPopup();

	//////////////////////////////////////////////////////////////////

	int32 CurrentStoreCategory;
	int32 CurrentGoodsCategory;

	int32 CurrentMileageType;

	int32 SelectedProductIdToBuy;
	uint32 SelectItemToBuyTicket;
	uint32 DeliveryBuyShopProductTicket;
	uint32 DeliveryRewardShopMileageClassTicket;
	FDelegateHandle MileageChangedHandler;

};
