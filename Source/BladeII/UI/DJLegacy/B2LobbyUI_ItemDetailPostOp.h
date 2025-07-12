// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2LobbyUI_ItemDetailPostOp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmButtonClicked);

/**
 * Most resembles ItemDetail UI, showing final item stat changes after Levelup and compose stuff.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemDetailPostOp : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;
	
	ELobbyInvenItemOpMode MyItemOpMode; // As this is used by all item Op mode.. Need this info somehow.

	/** Copied item data. The main stuff to be displayed here. */
	FB2Item NativeItemData_Before; // Before Levelup. This item itself does not exist at this point. Just copied data.
	FB2Item NativeItemData_After; // After Levelup
public:
	TWeakObjectPtr<UButton> BTN_Confirm;
	TWeakObjectPtr<UButton> BTN_Receive;
	TWeakObjectPtr<UWidgetAnimation> Anim_ItemRaffle;
	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> RaffleIcon;

	//Temporary for SummonItemDisplay
	FOnConfirmButtonClicked OnConfirmButtonClickedDelegate;

protected:

	//////////////////////////////////////////////////
	// For item icon.

	/** Widget class for a single item icon. It will be dynamically created inside */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;
	
	/** Parent canvas panel to contains dynamically created item icon. */
	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconPanel;

	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedItemIcon;
	//////////////////////////////////////////////////

	/** The font color implying that this item will increase character stat somewhat compared to what's equipped currently. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor ItemStatIncColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor ItemStatSameColor;

	/** The font color implying that this item will decrease (or just steady) character stat somewhat compared to what's equipped currently. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor ItemStatDecColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor	HeaderTextColor_EnhanceSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemStat")
	FSlateColor	HeaderTextColor_EnhanceFail;
	//////////////////////////////////////////////////
	// Native references for various item stat texts
	
	TWeakObjectPtr<UTextBlock> TB_ResultType;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPointType;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPoint;
	TWeakObjectPtr<UTextBlock> TB_PrimaryPointDelta;	
	TWeakObjectPtr<UTextBlock> TB_ItemLevel;
	TWeakObjectPtr<UTextBlock> TB_MaxItemLevel;
	TWeakObjectPtr<UProgressBar> PB_ItemExpProgress;
	TWeakObjectPtr<UProgressBar> PB_ItemJackPotExpProgress;
	TWeakObjectPtr<UTextBlock> TB_ItemExpPercent;	
	TWeakObjectPtr<UTextBlock> TB_ConfirmBtn;
	TWeakObjectPtr<UTextBlock> TB_EnhanceResult;	

	TWeakObjectPtr<UTextBlock>	TB_Combat;
	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<UTextBlock> TB_CombatPowerDelta;
	TWeakObjectPtr<UTextBlock>	TB_Enhance;
	TWeakObjectPtr<UTextBlock> TB_EnhanceLevel;
	TWeakObjectPtr<UTextBlock> TB_EnhanceLevelDelta;
								
	TWeakObjectPtr<UPanelWidget> X_P_ItemInfoDisplayPanel;
	TWeakObjectPtr<UPanelWidget> X_P_ItemDescriptionPanel;

	TWeakObjectPtr<UB2RichTextBlock> RTB_ItemDescription;

	TWeakObjectPtr<UCanvasPanel> X_CP_Jackpot;
	TWeakObjectPtr<UOverlay> X_O_ExpDelta;
	TWeakObjectPtr<UTextBlock> TB_ExpDelta;

	// For dynamic item options listing.

	/** One of major part here. It will display item's intrinsic options and random options dynamically. */
	TWeakObjectPtr<UVerticalBox> VB_SpecialInfoDisplayPanel;
	TWeakObjectPtr<class UB2UIItemDetailOption> UIP_ItemDetailOption;

	TWeakObjectPtr<UPanelWidget> EnhanceExpProgressSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2LobbyUI_ItemDetailSetEffect> SetEffectInfoDisplayClass; // For set effect enabled equipped protections
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2UICostumeItemDetailSetEffect> CostumeSetEffectInfoDisplayClass; 
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// For Levelup exp progress anim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jackpot")
	FProgressAnimateUtil ExpProgressAnim;

	/** Defines the time after widget creation that the first ingredient start to getting sucked to the target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jackpot")
	float JackpotFxBeginSeconds;

	FTimerHandle JackpotBeginHandle;

	bool IsLevelupDuringJackpot;

	UPROPERTY(EditAnywhere, Category = "Jackpot")
	TMap<EItemLevelUpJackpotType, FSlateBrush> JackPotFillImages;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	bool bEnhanceTest;
#endif

	virtual void CloseWidgetDelegate() override;

public:

	UB2LobbyUI_ItemDetailPostOp(const FObjectInitializer& ObjectInitializer);

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	virtual void NativeConstruct() override;
	virtual void CacheAssets() override;
	virtual void DestroySelf() override;

	void StartFromRaffle(const FB2Item& ItemInfo);
	/** Update the main data container here. It also updates ItemInfo data container according to the ItemRefID. */
	void UpdateItemData(const FB2Item& InBeforeLevelupItem, const FB2Item& InAfterLevelupItem);
	void OnUpdateProgressLevel(int32 CurrentLevel);
	void OnUpdateProgressPercent(float CurrentPercent);
	void OnFinishProgressAnimation();
	void OnFinishJackpotAnimation();

	void OnVisibleUpdate();
	void StartJackpotAnimation();

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnUpdateJackpotFX_BP(bool GreatJackpot);

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginJackpotAnimation_BP(bool GreatJackpot);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveItemIconByJackpotLevelup_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnChangeItemIconByJackpotLevelup();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnRaffleAnimationFinished();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnConfirmButtonClicked();

	void SetShowMainItemIcon(bool bShow);

	void ClosePopup();
	void ShowBackButton();

private:
	class UB2ItemInfo* GetItemInfoTable(); // Get ItemInfoTable from BladeII module.

	bool IsJackpot();

	void GetPreviewLevelupProgressItem(bool InIsJackpot, const FB2Item& InBeforeItem, const FB2Item& InAfterItem, FB2Item& OutPreviewITem);
	void DestroyItemIcon();
	void UpdateItemIcon(const FB2Item& InItem); // Dynamically create item icon for current NativeItemData, to be a part of UpdateItemData
	void UpdateSubWidgets(const FB2Item& InBeforeItem, const FB2Item& InAfterItem, bool IsChangeExpBar = true); // Another part of UpdateItemData, update placed widgets (by native references) to display item information.
	void UpdateExpProcess(int32 InCurItemLevel, float InCurItemExp); // Another part of UpdateItemData, update placed widgets (by native references) to display item information.

	void UpdateEssence(const FB2Item& InItem);
	/** A part of UpdateSubWidgets. */
	void UpdataItemSetEffect();
	void UpdateItemOptions();
	void CloseMe();

	void SetResultTypeText(ELobbyInvenItemOpMode InItemOpMode);
};
