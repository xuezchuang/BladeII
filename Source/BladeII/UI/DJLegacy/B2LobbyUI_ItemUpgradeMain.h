// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2LobbyUI_ItemUpgradeMain.generated.h"

const int32 LOBBY_ITEM_Upgrade_MENU_INGREDIENT_ICON_PER_ROW = 5;

/**
* The first (in fact, the main) step window of item Upgradement feature.
*/
UCLASS()
class BLADEII_API UB2LobbyUI_ItemUpgradeMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** The item data to be the target of Upgrade. It is not supposed to be changed once pop-up this page. */
	FB2Item NativeUpgradeTargetData;
	
protected:

	/** To sync commit animation and the real operation start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float UpgradeCommitAnimLength;

	TWeakObjectPtr<UTextBlock> TB_UpgradeTitle;
	TWeakObjectPtr<UTextBlock> TB_UpgradeCommitBtn;
	TWeakObjectPtr<UB2Button> BTN_UpgradeCommit;
	TWeakObjectPtr<UB2Button> BTN_Close;
		
	TWeakObjectPtr<UTextBlock> TB_UpgradeCostNum;
	TWeakObjectPtr<UImage> IMG_UpgradeStone;
	TWeakObjectPtr<UTextBlock> TB_CurrentUpgradeStone;
	TWeakObjectPtr<UTextBlock> TB_RequiredUpgradeStone;

	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_UpgradeGoods;

	TWeakObjectPtr<UTextBlock> TB_UpgradeNote;
	TWeakObjectPtr<UTextBlock> TB_UpgradeNote2;

	/** Contains common part for both Upgrade and upgrade menu.
	* UMG script is responsible for setting this at runtime. */
	UPROPERTY(Transient, BlueprintReadWrite)
	class UB2LobbyUI_ItemEnhUpgCommon* EnhUpgCommonNRef;

public:

	UB2LobbyUI_ItemUpgradeMain(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;

	/** Mainly to be called when Upgrade ingredient selection is changed. */
	void UpdateOnUpgradeStateChange();
private:
	void UpdateStaticTexts();
	void UpdateDynamicTexts();
	void UpdatePostUpgradeOptionStatTexts(); // To be updated whenever Upgrade ingredients are changed.
	void UpdateUpgradeCommitBtnState();

public:

	UFUNCTION()
	void OnUpgradeCommit();

	UFUNCTION()
	void OnCloseButtonPressed();

private:
	void CloseAndQuitItemOpMode();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginUpgradeCommitAnim();

	FTimerHandle UpgradeCommintTH;
	void FinalUpgradeCommit();

	virtual void CloseWidgetDelegate() override;
};
