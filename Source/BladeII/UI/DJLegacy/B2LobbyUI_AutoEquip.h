// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_AutoEquip.generated.h"

/**
 * One of Lobby inventory's pop-up. Suggests for best possible (maybe) equipment.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_AutoEquip : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
protected:

	virtual void CloseWidgetDelegate() override;

	////////////////////////////////////////////////////////////////////////////////
	// Dynamic equipment icons, just like that of LobbyUI_InventoryMain

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	TSubclassOf<class UB2DynItemIcon_LobbyEquip> EquipItemIconWidgetClass;

	/** Placed canvas panels for all equip position, to be set by UMG using SetEquipItemIconPanelNativeRef
	 * Use GetEquipItemIconPanelNativeRef for each element's access. */
	UPROPERTY(Transient)
	class UCanvasPanel* EquipItemIconPanels[(int32)EItemEquipPlace::EIEP_End];

	/** Created item icons on each EquipItemIconPanels element only for currently equipped or suggested items.
	 * An element of this array is valid only when the character has equipped an item or has suggested one at related equip place.
	 * Use Set/GetEquipItemIcon for each element access. */
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyEquip* CreatedEquipItemIcons[(int32)EItemEquipPlace::EIEP_End];

	////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// Text blocks

	
	TWeakObjectPtr<UTextBlock> TB_AutoEquipTitle;
	TWeakObjectPtr<UTextBlock> TB_AutoEquipButton;

	TWeakObjectPtr<UTextBlock> TB_AttackLabel;
	TWeakObjectPtr<UTextBlock> TB_DefenseLabel;
	TWeakObjectPtr<UTextBlock> TB_HealthLabel;

	TWeakObjectPtr<UTextBlock> TB_TotalAttackStat;
	TWeakObjectPtr<UTextBlock> TB_TotalDefenseStat;
	TWeakObjectPtr<UTextBlock> TB_TotalHealthStat;

	//////////////////////////////////////////////////

	/** Item IDs for equip suggestion, cached when this page is invoked and applied when auto equip button is clicked. */
	TArray<int64> PendingEquipSuggestedItemIDs;

	/** The AutoEquip window will be automatically closed when AutoEquip is done and this time has passed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PostAutoEquipCloseTime;

	FTimerHandle PostAutoEquipCloseTH;
	uint32 bAutoEquipRequested : 1;

public:
	
	UB2LobbyUI_AutoEquip(const FObjectInitializer& ObjectInitializer);

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

	void DestroyEquipItemIcon(EItemEquipPlace InEquipPlace); // Destroy icon at specific EquipPlace, if created.

	/** Return the allowed size (final scaled size) by the equip item panel size for each equip place.
	 * Probably the same for all.. */
	FVector2D GetAllowedSingleEquipItemIconSize(EItemEquipPlace InEquipPlace);

	void UpdateOnEquipChange();

private:
	void UpdateStaticTexts();

	void UpdateItemIcons();
	/** Set texts that summarize major 3 prim points' stat inc by equip suggestion. */
	void UpdateEquipSuggestionSummaryTexts();
	/** Set equip icon slot for current equip state. */
	void UpdateIconsForCurrentEquipState();
	/** Set equip icon slot for better equip suggestion, like overwriting current equip state. */
	void UpdateIconsForEquipSuggestion();
	
	void DestroyAllItemIcons();
public:

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnAutoEquipButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();

	bool DoAutoEquip();

	/** For Blueprint sound play, called only when at least one pending equipment presents. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayAutoEquipCommitSoundBP();

	void CloseMe();
};
