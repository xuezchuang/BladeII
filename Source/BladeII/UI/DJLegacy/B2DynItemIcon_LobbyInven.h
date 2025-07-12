// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2DynItemIcon.h"
#include "../../Lobby/B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.generated.h"

/** Differentiate some icon usage types by enum, instead of separate classes. They are almost similar. */
enum class ELobbyInvenItemIconUsage : uint8
{
	EEIIT_LobbyInven, // Most basic inventory store icon.
	EEIIT_ItemDetailWindow,
	EEIIT_ItemOpModeMenuTarget, // For Op mode (Levelup, upgrade, compose, surpass stuff) target icon
	EEIIT_ItemOpModeProgTarget,
	EEIIT_ItemLevelupMenuIngredient, // Enhance menu ingredient icons waiting for sucking
	EEIIT_ComposeOrSurpassIngredient, // Different from enhance ingredient. Compose or surpass requires only one.
	EEIIT_AcqItemEquipSuggest,
	EEIIT_AutoEnhanceResult,
	EEIIT_MailAllReceiveResult,
	EEIIT_ItemEnhanceMenuIngredient,
	EEIIT_ItemRafle,
};

/**
 * Dynamically created and deployed onto Lobby's inventory UI, per each displayed item.
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_LobbyInven : public UB2DynItemIcon
{
	GENERATED_BODY()

protected:
	
	/** Some displayed information and features can be different according to this. */
	ELobbyInvenItemIconUsage IconUsageType;

	/** UMG is responsible for setting this. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* ItemLevelTextBlockNRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* ItemAmountTextBlockNRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* ItemEnhanceLevelTextBlockNRef;

	/** Item quality information display panel. UMG is responsible for setting this. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UOverlay* OV_ItemQualitySet_NRef;
	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* ItemQualityTextBlockNRef;
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* ItemQualityBGImageNRef;

	/** Translucent background designating that the item is selected for Enhancement ingredient. UMG is responsible for setting this. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* EnhanceIngredSelectionImageNRef;

	/** Another Enhance ingredient selection image. This is the check display at center. UMG is responsible for setting this. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* EnhanceIngredCheckImageNRef;

	/** An image designating the primary stat by this item will be increased compared to what is equipped at the same place. 
	 * UMG is responsible for setting this. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* StatIncSignImageNRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UOverlay* OV_StatIncAmountSet_NRef;
	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* TB_StatIncAmountNRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* ItemLockedImageNRef_Store; // Locked state image for inventory stored item view.

	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* ItemLockedImageNRef_Detail; // Locked state image for item detail window view.

	// UPROPERTY **_NRef are old way to cache placed widgets. Better use GET_SLOT with matching name like below.
	TWeakObjectPtr<UPanelWidget> P_EquipInfo;
	TWeakObjectPtr<UImage> IMG_PresetBG;
	TWeakObjectPtr<UTextBlock> TB_Preset;
	TWeakObjectPtr<UImage> IMG_EquippedCheck;;
	TWeakObjectPtr<class UB2UIStarGradePart> StarGradePart;

	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* IMG_DisabledSlot;

	/** The button lies beneath images here, to invoke detailed info window. It also exists for detailed info window, then better be disabled. 
	 * Not much significant reason that this is TWeakObjectPtr, while others are hard-reference. */
	TWeakObjectPtr<UB2Button> BTN_Area;

	TWeakObjectPtr<class UB2UIWidgetBase>	UIP_RedDot;

	TWeakObjectPtr<UPanelWidget> P_DevUI;
	TWeakObjectPtr<UPanelWidget> P_StarGradeDisplayPanel;
#if !UE_BUILD_SHIPPING
	TWeakObjectPtr<UTextBlock> TB_ItemRefId_Dev;
	TWeakObjectPtr<UTextBlock> TB_ItemUId_Dev;
	TWeakObjectPtr<UB2Button> BTN_ItemRefId_Dev;
	TWeakObjectPtr<UB2Button> BTN_ItemUId_Dev;
#endif

private:
	bool IsContainsPreset;
	class FLocalCharacterData* CachedCharacterStore;

public:

	UB2DynItemIcon_LobbyInven(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void UpdateItemData(const FB2Item& InItem) override;

	virtual void SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables) override;

	/** Better set before call UpdateItemData */
	virtual void SetIconUsageType(ELobbyInvenItemIconUsage InUsageType);

	//UFUNCTION(BlueprintCallable, Category = "BladeII") // Try remove BP binding for performance
	FText GetItemLevelText() const;
	FText GetItemAmountText() const;
	FText GetItemEnhanceLevelText() const;
	FText GetItemRefId() const;
	FText GetItemUId() const;
	FText GetEquipPresetText() const;

	void SetSelectIcon(bool IsSelected);
private:
	void SetupStatIncInfo(); // Part of UpdateItemData
public:

	//UFUNCTION(BlueprintCallable, Category = "BladeII") // Try remove BP binding for performance
	ESlateVisibility GetItemLockedImageVisibility_Store() const;

	//UFUNCTION(BlueprintCallable, Category = "BladeII") // Try remove BP binding for performance
	ESlateVisibility GetItemLockedImageVisibility_Detail() const;
		
	// Button input handling interfaces for ManualScrollBox feature.
	virtual void OnClickedSenderBTNArea() override;
	virtual void OnPressedSenderBTNArea() override;
	virtual void OnReleasedSenderBTNArea() override;
	
	bool OnClickBTNForLevelup(UB2LobbyInventory* LobbyInven);
	bool OnClickBTNForHammer(UB2LobbyInventory* LobbyInven);
	bool OnClickBTNForComposition(UB2LobbyInventory* LobbyInven);
	bool OnClickBTNForSurpass(UB2LobbyInventory* LobbyInven);
	bool OnClickBTNForEnhance(UB2LobbyInventory* LobbyInven);
	bool OnClickBTNForEnhanceChange(UB2LobbyInventory* LobbyInven);

	UFUNCTION()
	void OnClickedBTNArea();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnClickedBTNArea_BP();
private:
	void AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode InModeForUnlock);
	void AskForUnEquipItemPopupCommon();

	bool IsContainsCurrentPreset(const FB2Item& InItem);

public:

	UFUNCTION()
	void OnPressedBTNArea();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPressedBTNArea_BP();
	
	UFUNCTION()
	void OnReleasedBTNArea();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnReleasedBTNArea_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayCommonIconClickSound();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayLevelupIngredSelectedSound();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayEnhanceIngredSelectedSound();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayComposeIngredSelectedSound();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlaySurpassIngredSelectedSound();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayEnhanceResultAnim();

	UFUNCTION()
	void OnPressedBTNItemUid();
	UFUNCTION()
	void OnPressedBTNItemRefid();


	//UFUNCTION(BlueprintCallable, Category = "BladeII") // Try remove BP binding for performance
	ESlateVisibility GetEnhanceIngredSelectionImageVisibility() const;
	bool ShouldShowDevUI() const;
	ESlateVisibility GetDisabledSlotImageVisibility() const;

	/** True sets background and icon image invisible. It is meant to be used when the icon body is displayed using 3D mesh. */
	void SetShowOverlayInfoOnly(bool IsVisibleBGImg = false, bool IsVisibleGrade = false);

	virtual class UMaterialInstanceConstant* GetBGPanelMIC();
	virtual class UMaterialInstanceConstant* GetMainIconMIC();

	bool IsBtnAreaUsed() const; // BTN_Area is not always necessary.
};
