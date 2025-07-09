// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_ItemComposeMain.generated.h"

/**
 * Main window of inventory's item compose operation.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemComposeMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** The item data to be the target of composition. It is not supposed to be changed once pop-up this page. */
	FB2Item NativeComposeTargetData;

protected:

	/** To sync commit animation and the real operation start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float CompseCommitAnimLength;

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;

	TWeakObjectPtr<UCanvasPanel> CP_TargetItemIconSet;
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedTargetItemIcon;

	TWeakObjectPtr<UCanvasPanel> CP_IngredItemIconSet;
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedIngredItemIcon;

	TWeakObjectPtr<UB2Button> BTN_ComposeCommit;
	TWeakObjectPtr<UTextBlock> TB_ComposeCommitBtn;
	TWeakObjectPtr<UTextBlock> TB_ComposeCostNum;
	TWeakObjectPtr<UTextBlock> TB_ComposeTitle;
	TWeakObjectPtr<UTextBlock> TB_ComposeDescription;
	TWeakObjectPtr<UTextBlock> TB_ComposeDescription2;
	TWeakObjectPtr<UTextBlock> TB_ComposeDescription3;
	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UTextBlock> TB_ExpectedResult;
	TWeakObjectPtr<UTextBlock> TB_NoIngredMessage;
	TWeakObjectPtr<UB2Button> BTN_IngredUnselect;

	TWeakObjectPtr<class UB2UIStarGradePart> StarGradePart;

public:

	UB2LobbyUI_ItemComposeMain(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;
	
	/** Mainly to be called when compose ingredient selection is changed. */
	void UpdateOnComposeStateChange();

private:

	void UpdateStaticTexts();
	void UpdateDynamicWidgets();
	void UpdateTargetItemIcon();
	void UpdateIngredItemIcon();
	void UpdateComposeCommitBtnState();
	void UpdateStar();

	void DestroyTargetItemIcon();
	void DestroyIngredItemIcon();

	bool IsAnyIngredSelected();

public:

	UFUNCTION()
	void OnComposeCommit();

	UFUNCTION()
	void OnCloseButtonPressed();

	UFUNCTION()
	void OnClickIngredUnselectBtn();

private:
	void CloseAndQuitItemOpMode();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginComposeCommitAnim();

	FTimerHandle ComposeCommintTH;
	void FinalComposeCommit();

	virtual void CloseWidgetDelegate() override;
};
