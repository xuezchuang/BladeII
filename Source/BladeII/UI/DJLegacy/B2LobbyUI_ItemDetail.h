// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "B2DynItemIcon_SealBoxPreview.h"
#include "B2UIStoreProductCost.h"
#include "B2LobbyUI_ItemDetail.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseButtonClicked);

/**
 * Pop-up window invoked when an item icon is clicked
 */

struct FSealBoxItem
{
	int32 itemRefID = -1;
	int32 minQuantity=0;
	int32 maxQuantity=0;
};
struct FSealBoxEquipment
{
	int32 starGrade;
	int32 groupID;
	int32 equipCategory;
	int32 equipPosition;
	int32 quality;
	int32 enhancedLevel;
	int32 surpassCount;
};

UCLASS()
class BLADEII_API UB2LobbyUI_ItemDetail : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

private:
	bool bIsContainsPresetItem;
	bool bIsContainsOtherPreset;

	/** For equipped item if true, stored (inventory) item if false. The viewport position and some of operations are different according to this. */
	uint32 bIsForEquippedItem : 1;

	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;
	
	//
	// Shares some common stuff with B2DynItemIcon.. but this has B2DynItemIcon inside.
	//

	/** Copied item data. The main stuff to be displayed through this item icon widget. */
	FB2Item NativeItemData;

	class FClientDataStore *CachedDataStore;

	FTimerHandle DelayCallbackTH;

	FSealBoxInfo originalSealBox;
	TArray<FSealBoxRewardRateInfo> sealboxRewardInfo;
protected:

	//////////////////////////////////////////////////
	// For item icon.

	/** Widget class for a single item icon. It will be dynamically created inside */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;
	
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedItemIcon;
	//////////////////////////////////////////////////

	/** The font color implying that this item will increase character stat somewhat compared to what's equipped currently. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor ItemStatIncColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor ItemStatSameColor;

	/** The font color implying that this item will decrease character stat somewhat compared to what's equipped currently. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor ItemStatDecColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2DynItemIcon_SealBoxPreview>	SealBoxContainListBP;

	//////////////////////////////////////////////////
	// Native references for various item stat texts
		
	TWeakObjectPtr<UTextBlock> TB_PrimaryPointType;
		
	TWeakObjectPtr<UTextBlock> TB_PrimaryPoint;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPointDelta;
		
	TWeakObjectPtr<UTextBlock>	TB_Combat;
	TWeakObjectPtr<UTextBlock>	TB_CombatPower;
	TWeakObjectPtr<UTextBlock>  TB_CombatPowerDelta;

	TWeakObjectPtr<UTextBlock> TB_ItemLevel;
	TWeakObjectPtr<UProgressBar> PB_ItemExpProgress;
	TWeakObjectPtr<UTextBlock> TB_ItemExpPercent;

	TWeakObjectPtr<UTextBlock> TB_Enhance;
	TWeakObjectPtr<UTextBlock> TB_EnhanceLevel;
	TWeakObjectPtr<UTextBlock> TB_ItemDesc;

	TWeakObjectPtr<UTextBlock> TB_ShowChance;
	TWeakObjectPtr<UTextBlock> TB_SealBoxDesc;
	TWeakObjectPtr<UTextBlock> TB_SealBox_Contain;
	TWeakObjectPtr<UTextBlock> TB_SealBox_ButtonSet;
	TWeakObjectPtr<UB2RichTextBlock> RTB_SealBoxDescription;
	TWeakObjectPtr<UB2ScrollBox> SB_SealBox_ContainList;
	TWeakObjectPtr<UHorizontalBox> HB_ClassCycle;
	
	//////////////////////////////////////////////////
	// Bottom Button Set
	TWeakObjectPtr<UButton> BTN_Unseal;
	TWeakObjectPtr<UTextBlock> TB_Unseal;

	TWeakObjectPtr<UButton> BTN_Convert;
	TWeakObjectPtr<UTextBlock> TB_Convert;

	TWeakObjectPtr<UButton> BTN_Decomposition;
	TWeakObjectPtr<UTextBlock> TB_Decomposition;

	TWeakObjectPtr<UButton> BTN_Use;
	TWeakObjectPtr<UTextBlock> TB_UseBtn;

	TWeakObjectPtr<UButton> BTN_Sell;
	TWeakObjectPtr<UTextBlock> TB_SellBtn;

	TWeakObjectPtr<UButton> BTN_EquipOrRelease;
	TWeakObjectPtr<UTextBlock> TB_EquipOrReleaseBtn;
	//////////////////////////////////////////////////

	TWeakObjectPtr<UButton> BTN_ShowChance;
	TWeakObjectPtr<UButton> BTN_SealBoxClass;

	//////////////////////////////////////////////////
	// Left Button Set
	/** To be visible only when certain condition is met. 
	 * Item upgrade itself is not that special, just appearing at the same condition to composition so make separate button. */
	TWeakObjectPtr<USizeBox> SB_Upgrade;
	TWeakObjectPtr<UButton> BTN_Upgrade;
	TWeakObjectPtr<UTextBlock> TB_UpgradeBtn;

	/** Levelup or Composition or Surpass, depend on the item's state. */
	TWeakObjectPtr<UOverlay> OV_LevelupOrCompositionOrSurPass;
	TWeakObjectPtr<UButton> BTN_LevelupOrCompositionOrSurPass;
	TWeakObjectPtr<UTextBlock> TB_LevelupOrCompositionOrSurPassBtn;

	TWeakObjectPtr<USizeBox> SB_CompositionForSurpass;
	TWeakObjectPtr<UButton> BTN_CompositionForSurpass;
	TWeakObjectPtr<UTextBlock> TB_CompositionForSurpass;

	TWeakObjectPtr<USizeBox> SB_Enhance;
	TWeakObjectPtr<UButton> BTN_Enhance;
	TWeakObjectPtr<UTextBlock> TB_EnhanceBtn;

	//TWeakObjectPtr<USizeBox> SB_ItemOptionChange;
	//TWeakObjectPtr<UButton> BTN_ItemOptionChange;
	//TWeakObjectPtr<UTextBlock> TB_ItemOptionChange;

	TWeakObjectPtr<USizeBox> SB_EnhanceChange;
	TWeakObjectPtr<UButton> BTN_EnhanceChange;
	TWeakObjectPtr<UTextBlock> TB_EnhanceChange;

	TWeakObjectPtr<USizeBox>	SB_Seal;
	TWeakObjectPtr<UButton>		BTN_Seal;
	TWeakObjectPtr<UTextBlock>	TB_Seal;

	//////////////////////////////////////////////////

	TWeakObjectPtr<UB2RichTextBlock> RTB_ItemDescription;

	TWeakObjectPtr<UCanvasPanel> X_P_ItemIconPanel;
	TWeakObjectPtr<UPanelWidget> X_P_LeftButtonSet;
	TWeakObjectPtr<UPanelWidget> X_P_BottomButtonSet;
	TWeakObjectPtr<UPanelWidget> X_P_ItemLockButtonSet;
	TWeakObjectPtr<UPanelWidget> X_P_ItemInfoDisplayPanel;
	TWeakObjectPtr<UPanelWidget> X_P_ItemDescriptionPanel;
	TWeakObjectPtr<UPanelWidget> X_P_ItemSealBoxPanel;

	TWeakObjectPtr<UTextBlock> TB_SealBox_Class;

	TWeakObjectPtr<UButton> BTN_ItemLock;
	TWeakObjectPtr<UButton> BTN_Close;
	//////////////////////////////////////////////////

	// Additional info view at right bottom corner forset effect of equipped protections
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2LobbyUI_ItemDetailSetEffect> SetEffectInfoDisplayClass; // For set effect enabled equipped protections

	TWeakObjectPtr<UVerticalBox> VB_SpecialInfoDisplayPanel;

	TWeakObjectPtr<class UB2UIItemDetailOption> UIP_ItemDetailOption;

	/** Either by UB2LobbyUI_ItemDetailSetEffect */
	UB2LobbyUserWidget* CreatedSpecialInfoDisplay;

	EPCClass SealBoxClass;

	//////////////////////////////////////////////////

public:

	UB2LobbyUI_ItemDetail(const FObjectInitializer& ObjectInitializer);
	
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void NativeConstruct() override;

	virtual void CacheAssets() override;

	virtual void DestroySelf() override;


private:
	class UB2ItemInfo* GetItemInfoTable(); // Get ItemInfoTable from BladeII module.
public:

	/** Update the main data container here. It also updates ItemInfo data container according to the ItemRefID. */
	void UpdateItemData(const FB2Item& InItem);
private:
	void UpdateItemIcon(); // Dynamically create item icon for current NativeItemData, to be a part of UpdateItemData
	void DestroyItemIcon();

	void UpdateSubWidgets(); // Another part of UpdateItemData, update placed widgets (by native references) to display item information.

	bool GetUnsealableItem();
	bool GetSealboxClassCycleable();
	bool GetConvertableItem();
	bool GetConsumableItem();
	bool GetEssenceItem();

	void UpdateWidgetForSealBox();

	const struct FSingleGoodsTypeInfoData* GetItemGoodsTypeInfoData();

	/** A part of UpdateSubWidgets. EquippedOneAtSamePlace is non-null only when found.. */
	void UpdatePrimaryPointTextSet(FB2Item* EquippedOneAtSamePlace);

	void UpdateItemOptions();

	void UpdateSpecialInfoDisplay();
	void DestroySpecialInfoDisplay();

	/** Common function to pop-up item unlock message for some item action. */
	void AskForUnlockItemPopupCommon(const FText& InAskingItemActText);
	void AskForUnEquipItemPopupCommon(const FText& InAskingItemActText);

	void NoticeErrorPopupCommon(const FText& InAskingText);
public:

	void SetIsForEquippedItem(bool bInForEquipped);
	void SetFindUserInfoButtonHidden();

public:
	
	// Bottom Button Visibility Set
	ESlateVisibility GetUnsealButtonVisibility();
	ESlateVisibility GetConvertButtonVisibility();
	ESlateVisibility GetDecompositionButtonVisibility() const;
	ESlateVisibility GetUseButtonVisibility();
	ESlateVisibility GetAllReleaseVisibility() const;
	ESlateVisibility GetSellButtonVisibility() const;

	// Left Button Visibility Set
	ESlateVisibility GetUpgradeButtonVisibility() const;
	ESlateVisibility GetLevelupOrCompositionOrSurPassButtonVisibility() const;
	ESlateVisibility GetCompositionForSurpassVisibility() const;
	ESlateVisibility GetEnhanceButtonVisibility() const;
	ESlateVisibility GetEnhanceChangeButtonVisibility() const;
	ESlateVisibility GetItemOptionChangeButtonVisibility() const;
	ESlateVisibility GetSealButtonVisibility() const;
	
	// Bottom Button Text Set
	FText GetUnsealButtonText() const;
	FText GetConvertButtonText() const;
	FText GetDecompositionButtonText() const;
	FText GetUseButtonText() const;
	FText GetSellButtonText() const;
	FText GetEquipOrReleaseButtonText() const;

	// Left Button Text Set
	FText GetUpgradeButtonText() const;
	FText GetLevelupOrCompositionOrSurPassButtonText() const;
	FText GetCompositionForSurpassButtonText() const;
	FText GetEnhanceButtonText() const;
	FText GetEnhanceChangeButtonText() const;
	FText GetItemOptionChangeText() const;
	FText GetSealButtonText() const;

	FText GetItemDescriptionText() const;

	FText GetSealBoxDescText(int32 sealBoxRefID) const;

	// Old way.. not a big problem in detail view
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	ESlateVisibility GetItemLockedImageVisibility() const;
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	ESlateVisibility GetItemUnLockedImageVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "B2LobbyUI_ItemDetail")
	bool GetItemIsLocked() const;

	FOnCloseButtonClicked OnCloseButtonClickedDelegate;

protected:
	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnUnsealButtonClicked();

	UFUNCTION()
	void OnConvertButtonClicked();

	UFUNCTION()
	void OnDecompositionButtonClicked();
	
	UFUNCTION()
	void OnUseButtonClicked();

	UFUNCTION()
	void OnSellButtonClicked();

	UFUNCTION()
	void OnEquipOrReleaseButtonClicked();

	/** For Blueprint sound play. Equip or Release according to the bIsForEquip parameter. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayEquipOrReleaseSound(bool bIsForEquip);

	UFUNCTION()
	void OnUpgradeButtonClicked();

	UFUNCTION()
	void OnLevelupOrCompositionOrSurPassButtonClicked();

	UFUNCTION()
	void OnCompositionForSurpassButtonClicked();

	UFUNCTION()
	void OnEnhanceButtonClicked();

	UFUNCTION()
	void OnItemLockButtonClicked();

	/*UFUNCTION()
	void OnItemOptionChangeButtonClicked();*/

	UFUNCTION()
	void OnEnhanceChangeButtonClicked();

	UFUNCTION()
	void OnSealButtonClicked();

	UFUNCTION()
	void OnShowChanceBTNClicked();

	UFUNCTION()
	void OnSealBoxClassBTNClicked();
	
	virtual void CloseWidgetDelegate() override;

public:
	FORCEINLINE int32 GetDisplayItemRefID() { return NativeItemData.ItemRefID; }
	FORCEINLINE int64 GetDisplayItemUID() { return NativeItemData.InstanceUID; }

private:
	void CloseMe();
	void Decomposition();
	void UnsubscribeDecomposition();

	void OnDelayUnsealCallback();
	void OnDelayConvertCallback();
	void OnDelayDecompositionCallback();
	void OnDelayCompositionForSurpassCallback();
	void OnDelaySellCallback();
	void OnDelayLevelupCallback();
	void OnDelayEnhanceCallback();
	void OnDelayEnhanceChangeCallback();
	void OnDelaySealCallback();
	void OnDelayShowChanceCallback();
	void OnDelaySealBoxClassCallback();

	void AddSealBoxContents();

private:
	uint32 DecompositionEquipItemEventID;
};
