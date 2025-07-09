// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_ItemLevelupMain.generated.h"

const int32 LOBBY_EQUIPMENT_LEVELUP_MENU_INGREDIENT_ICON_PER_ROW = 5;

/**
 * The first (in fact, the main) step window of levelup item feature.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemLevelupMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** The item data to be the target of levelup. It is not supposed to be changed once pop-up this page. */
	FB2Item NativeTargetItem;
	int32 PostItemLevel;

protected:
	TWeakObjectPtr<UTextBlock> TB_EnhanceTitle;
	TWeakObjectPtr<UTextBlock> TB_EnhanceCommitBtn;
	TWeakObjectPtr<UB2Button> BTN_EnhanceCommit;
	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UTextBlock> TB_EnhanceLevelText;
	TWeakObjectPtr<UTextBlock> TB_EnhanceLevelNum;
	TWeakObjectPtr<UProgressBar> PB_EnhanceExpProgress;
	TWeakObjectPtr<UTextBlock> TB_EnhanceExpPercent;

	TWeakObjectPtr<UTextBlock> TB_EnhanceCostNum;

	TWeakObjectPtr<UTextBlock> TB_EnhanceIngredNote;
	TWeakObjectPtr<UTextBlock> TB_EnhanceIngredCount;
	TWeakObjectPtr<UTextBlock> TB_SelectEnhanceItemText;

	TWeakObjectPtr<UB2ScrollBox> SB_EnhanceIngredIcons;
	/** Contains common part for both enhancement and upgrade menu.
	 * UMG script is responsible for setting this at runtime. */
	UPROPERTY(Transient, BlueprintReadWrite)
	class UB2LobbyUI_ItemEnhUpgCommon* EnhUpgCommonNRef;

	//////////////////////////////////////////////////////////////////////
	// For levelup item ingredient item icon

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_EnhanceIngred> ItemIconWidgetClass;

	/** Horizontal margin between ingredient icons. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	float IngredientIconPlacementMargin;

	/** Multiple rows are placed at same vertical coordinate with this offset  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	float IngredientIconNextRowOffset;

	UPROPERTY(Transient)
	TArray<class UB2DynItemIcon_EnhanceIngred*> CreatedIngredientItemIcons;

	//////////////////////////////////////////////////////////////////////

	/** To sync levelup commit animation and the real enhancement operation start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float EnhanceCommitAnimLength;

	//////////////////////////////////////////////////////////////////////
	// For enhance exp progress anim
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	FProgressAnimateUtil LevelExpProgressAnim;
	//////////////////////////////////////////////////////////////////////

public:

	UB2LobbyUI_ItemLevelupMain(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;

	/** Mainly to be called when levelup ingredient selection is changed. */
	void UpdateOnLevelupStateChange();

	int32 GetPostItemLevel() const { return PostItemLevel; }
	int32 GetMaxItemLevel() const { return NativeTargetItem.MaxLevel;}
private:
	void UpdateStaticTexts();
	void UpdateDynamicTexts();
	void UpdateLevelupIngredientItemIcons(); // To be updated whenever enhance ingredients are changed.
	void UpdatePostItemLevelupExpValues(); // To be updated whenever enhance ingredients are changed.
	void UpdatePostItemLevelupOptionStatTexts(); // To be updated whenever enhance ingredients are changed.
	void UpdateItemLevelupCommitBtnState();	
	
	void OnUpdateProgressLevel(int32 CurrentLevel);
	void OnUpdateProgressPercent(float CurrentPercent);

	void DestroyItemLevelupIngredientItemIcon();

	class UB2DynItemIcon_EnhanceIngred* CreateItemIcon();

public:

	UFUNCTION()
	void OnEnhanceCommit();

	TUTORIAL void Tutorial_FinalItemLevelUpCommit();

	UFUNCTION()
	void OnCloseButtonPressed();

private:
	void CloseAndQuitItemOpMode();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginEnhanceCommitAnim();

	FTimerHandle ItemLevelupCommintTH;
	void FinalItemLevelupCommit();

	virtual void CloseWidgetDelegate() override;
};
