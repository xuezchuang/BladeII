// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "EventSubsystem.h"
#include "B2LobbyUI_InventoryMain.generated.h"


/**
* Main inventory page user widget. Many other sub pop-ups can pop-up based on this.
*/
UCLASS()
class BLADEII_API UB2LobbyUI_InventoryMain : public UB2LobbyUserWidget
{
	GENERATED_BODY()

		/** Source of displayed information here. */
		UPROPERTY(Transient)
		class UB2LobbyInventory* CachedInventory;

protected:

	const int32 MaxEquipPlace = static_cast<int32>(EItemEquipPlace::EIEP_End);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FSlateColor StoredItemNumTextColor_Normal;

	/** Applied when stored item over the limit of lobby inventory. We got some occasion for that. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FSlateColor StoredItemNumTextColor_Over;


	/** Contains common part of several hero management UI pages.
	* UMG script is responsible for setting this at runtime. */
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite)
		class UB2LobbyUI_HeroMgmtCommon* HeroMgmtBase;

	/** Directly contained in ItemDisplayPanelNativeRef, each ItemIconRow contains certain number of item icons.
	* It is still supposed to be a canvas panel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		TSubclassOf<class UB2DynItemIconRow_LobbyInven> ItemIconRowWidgetClass;

	/** The "outermost" container panel (scroll box) of dynamically created item icons.
	* it directly contains item rows (ItemIconRowWidgetClass) and each row contains the final item icons.
	* UMG is responsible for setting this. */
	UPROPERTY(Transient, BlueprintReadWrite)
		class UB2ScrollBox* ItemDisplayPanelNativeRef;

	/** Dynamically created item icon rows for current tab. Updated for each UpdateItemIconsForCurrentSelection call */
	UPROPERTY(Transient)
		TArray<class UB2DynItemIconRow_LobbyInven*> CreatedItemRows;

	////////////////////////////////////////////////////////////////////////////////
	// For equipped items.

	/** Placed canvas panels for all equip position, to be set by UMG using SetEquipItemIconPanelNativeRef
	* Use GetEquipItemIconPanelNativeRef for each element's access. */
	UPROPERTY(Transient)
		class UCanvasPanel* EquipItemIconPanels[(int32)EItemEquipPlace::EIEP_End];

	/** Created item icons on each EquipItemIconPanels element only for currently equipped items.
	* An element of this array is valid only when the character has equipped an item at the related equip place.
	* Use Set/GetEquipItemIcon for each element access. */
	UPROPERTY(Transient)
		class UB2DynItemIcon_LobbyEquip* CreatedEquipItemIcons[(int32)EItemEquipPlace::EIEP_End];

	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	// For widget component naming.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Helmet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_BodyArmor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Glove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Shoes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Belt;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Necklace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipSlotSuffix_Ring;

	FString GetEquipSlotSuffix(int32 InEquipPlaceIdx);
	FString GetEquipSlotSuffix(EItemEquipPlace InEquipPlace);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString NoEquipIconImgPrefix;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipEffectImgPrefix;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		FString EquipButtonPrefix;

	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	// Equipping effect

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
		float EquipEffectPlayTime;

	FTimerHandle EquipEffectPlayTH;

	TWeakObjectPtr<UImage> IMG_EquipEffectParamAnimDummy; // For other component's material parameter animation.
	TWeakObjectPtr<UImage> IMG_EquipEffectParamAnimDummy2;

	TArray<UImage*> EquipEffectImgs;
	TArray<UImage*> EquipEffectImgs2;
	UImage* GetEquipEffectImg(EItemEquipPlace InEquipPlace);
	UImage* GetEquipEffectImg2(EItemEquipPlace InEquipPlace);

	/** Must contains valid members only when effect animation is being played. They don't always contain widgets of all equip places. */
	UPROPERTY()
		TArray<class UMaterialInstanceDynamic*> CurrentActiveEquipEffectMID;
	UPROPERTY()
		TArray<class UMaterialInstanceDynamic*> CurrentActiveEquipEffectMID2;

	////////////////////////////////////////////////////////////////////////////////

private:

	TArray<std::shared_ptr<issue>> Issues;

	uint32 bEventSubscribed : 1;
	uint32 bFirstHeroChange : 1;

	int32 CachedLevelupItemIngredsCount;

	/** The item inventory tab selection above item display panel */
	EItemInvenType CurSelectedItemInvenTab;
	EPCClass CurSelectedPCClass;
	
	EItemInvenType BeforeCurSelectedItemInvenTab;
	EPCClass BeforeCurSelectedPCClass;

	EItemInvenType CachedSelectedItemInvenTab;

	FTimerHandle	DelayButtonCallbackTH;

	class FLocalCharacterData* CachedCharacterStore;

	TWeakObjectPtr<UTextBlock> TB_ItemOpModeGuide;

	// They are turned on/off according to current equip state.
	TArray<UImage*> NoEquipIcons;
	UImage* GetNoEquipIconImageForEquipPlace(EItemEquipPlace InEquipPlace);

	TArray<UButton*> EquipSlotButtons;

	TWeakObjectPtr<UPanelWidget> CP_ContainerSizeTextSet;
	TWeakObjectPtr<UB2Button> BTN_ContainerExpansion;
	TWeakObjectPtr<UB2Button> BTN_SellAll;
	TWeakObjectPtr<UB2Button> BTN_AutoEnhancement;
	TWeakObjectPtr<UB2Button> BTN_AutoEquip;
	TWeakObjectPtr<UB2Button> BTN_AutoSelect;
	TWeakObjectPtr<UB2Button> BTN_AutoDecomposition;

	TWeakObjectPtr<UTextBlock> TB_AutoSelect;

	////////////////////////////////////////////////////////////////////////////////
	TWeakObjectPtr<UCanvasPanel>	X_LeftPopupPanel;
	TWeakObjectPtr<UCanvasPanel>	X_RightPopupPanel;
	TWeakObjectPtr<UImage>    IMG_BackGround;

	TWeakObjectPtr<class UB2UIWidgetBase> IMG_NewItemCheck_Weapon;
	TWeakObjectPtr<class UB2UIWidgetBase> IMG_NewItemCheck_Protection;
	TWeakObjectPtr<class UB2UIWidgetBase> IMG_NewItemCheck_Accessory;
	TWeakObjectPtr<class UB2UIWidgetBase> IMG_NewItemCheck_Consumables;

	TArray<TWeakObjectPtr<class UB2UIWidgetBase>>	NewitemCheckArr;

	TWeakObjectPtr<UTextBlock> TXT_Helmet;
	TWeakObjectPtr<UTextBlock> TXT_BodyArmor;
	TWeakObjectPtr<UTextBlock> TXT_Glove;
	TWeakObjectPtr<UTextBlock> TXT_Shoes;
	TWeakObjectPtr<UTextBlock> TXT_Weapon;
	TWeakObjectPtr<UTextBlock> TXT_Belt;
	TWeakObjectPtr<UTextBlock> TXT_Necklace;
	TWeakObjectPtr<UTextBlock> TXT_Ring;

	TWeakObjectPtr<class UB2UICommonTab> TAB_Weapon;
	TWeakObjectPtr<class UB2UICommonTab> TAB_Protection;
	TWeakObjectPtr<class UB2UICommonTab> TAB_Accessory;
	TWeakObjectPtr<class UB2UICommonTab> TAB_Consumables;

	TWeakObjectPtr<UTextBlock>		TB_AllSell;
	TWeakObjectPtr<UTextBlock>		TB_AutoEquip;
	TWeakObjectPtr<UTextBlock>		TB_AutoDecomposition;

public:

	UB2LobbyUI_InventoryMain(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void CacheAssets() override;

	/** Set canvas panel native ref, for dynamic equipped item icon creation. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void SetEquipItemIconPanelNativeRef(EItemEquipPlace InEquipPlace, class UCanvasPanel* InRef);
	/** Set canvas panel native ref for each equip place. */
	class UCanvasPanel* GetEquipItemIconPanelNativeRef(EItemEquipPlace InEquipPlace);
	/** To access element of CreatedEquipItemIcons, for each equip place. */
	void SetEquipItemIcon(EItemEquipPlace InEquipPlace, class UB2DynItemIcon_LobbyEquip* InCreatedIcon);
	class UB2DynItemIcon_LobbyEquip* GetEquipItemIcon(EItemEquipPlace InEquipPlace);
	class UB2DynItemIcon_LobbyEquip* GetEquipItemIconOfID(int64 ItemInstanceID);

	/** Simply called both update call for stored and equipped item icons.
	* The main function that you call in any case of item state and/or list update. */
	void UpdateAllItemIconsForCurrentSelection();
	/** Create and place item icons for currently selected player character and item class tab. */
	void UpdateStoredItemIconsForCurrentSelection();
	/** It also creates and place item icons, but equipped ones. */
	void UpdateEquipItemIconsForCurrentSelection();
	/** Find if it currently has an icon of ItemInstanceUID, and update it if found. Use it only when you know there's only one item's state change. In addition, do not use this for item remove or add. */
	void TryUpdateSingleItemIconOfID(int64 ItemInstanceUID);
	/** Returns a valid reference of item icon, if one of CreatedItemRows contains an item icon of designated instance UID. */
	class UB2DynItemIcon_LobbyInven* FindSingleItemIcon(int64 ItemInstanceUID);
	class UB2DynItemIcon_LobbyInven* FindSingleItemIconFromIndex(int32 iIndexX, int32 iIndexY);

	void DestroyAllItemRows(); // Eventually destroy all item icons.
	void DestroyAllEquipItemIcons();

	/** Return the allowed size (final scaled size) by the equip item panel size for each equip place.
	* Probably the same for all.. */
	FVector2D GetAllowedSingleEquipItemIconSize(EItemEquipPlace InEquipPlace);

	virtual void DestroySelf() override;

	void SetItemOpModeGuideText(const FText& InText); // Set and display
	void HideItemOpModeGuideText();

	void OnHeroMgmtPCChanged(EPCClass InSelectedPCClass);

	// It is assumed to be called on PCClass or InvenTab change. Might need previous info..
	virtual void DoMarkRedDot() final;

	void UpdateInvenControlButtonState();
	void UpdateAutoSelectButton();

private:

	void UpdateStaticText();
	void BindDelegates();

	void SubscribeEvents();
	void UnsubscribeEvents();

	FText GetAutoSelectButtonText() const;

	virtual void RegisterUIMarkForRedDot() final;
	virtual void UnRegisterUIMarkForRedDot() final;

	void OnDelayCallbackSellAll();
	void OnDelayCallbackAutoEquip();
	void OnDelayCallbackAutoLevelUp();
	void OnDelayCallbackAutoDecomposition();

	void TutorialHighlightCheck();

	void DeliveryModifyItemNewFlag();
	void UpdateInvenTabWidgetVisibilityState();

	void UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot);

	UFUNCTION()
		void OnClickTabWeapon();
	UFUNCTION()
		void OnClickTabProtection();
	UFUNCTION()
		void OnClickTabAccessory();
	UFUNCTION()
		void OnClickTabConsumables();

public:

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		FText GetContainerSizeText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		FText GetStoredItemNumText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		FSlateColor GetStoredItemNumTextColor() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		FText GetSellAllButtonText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		FText GetAutoLevelupButtonText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		FText GetAutoEquipmentButtonText() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnItemInvenTabButtonPressed(EItemInvenType InSelectedInvenTab);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnContainerExpansionButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnSellAllButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnAutoLevelupButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnAutoEquipButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnAutoSelectButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnAutoDecompositionButtonPressed();

	FORCEINLINE EItemInvenType GetCurrentInvenTabSelection() { return CurSelectedItemInvenTab; }
	FORCEINLINE EPCClass		GetPrevSelectedPCClass() { return CurSelectedPCClass; }

	/** Special purpose to set inventory UI tab selection from native */
	void NativeSetInvenTabSelection(EItemInvenType NewTabSelection);

	// Is possible to invoke sub menu like SellAll, AutoEquip, and such stuff.
	bool IsPossibleToInvokeSubMenu();
	bool IsPossibleToInvokeContainerExpansionMenu();
	bool IsPossibleToAutoSelctSubMenu();

	bool IsPossibleToInvokeSellAll();
	bool IsPossibleToInvokeAutoEnhancement();

	bool IsInventoryModeLevelup();

	void AllPlayEquipEffect();
	void PlayEquipEffect(EItemEquipPlace InEquippedPlace);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void BeginEquipEffectParamAnimBP();
	void UpdateEquipEffectParamAnim();
	void StopEquipEffect();
	void RedDotCondition_TapState();
	void RequestNewItem();

	// Enable/Disable inventory type tab buttons, could be used when you need to force the inventory tab selection, e.g. item levelup mode.
	void SetEnableAllTabButtons(bool bEnable);
	void SetEnableOnlyOneTabType(EItemInvenType InEnableTabType);

	void ShowHeroMgmtControlMenu(bool bShow);

	void OnChangeInvenItemOpMode(ELobbyInvenItemOpMode InvenItemOpMode);

	UCanvasPanel* GetItemDetailPopupParentPanel(bool bOfLeftSide) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "B2LobbyUI_InventoryMain")
	void PlayChangeAnimEquipSlotOrCostumeSlot_BP(bool bIEquipSlotVisible);
};
