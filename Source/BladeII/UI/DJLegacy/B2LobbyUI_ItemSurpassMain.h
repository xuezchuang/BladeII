// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2UIBackWidget.h"
#include "CommonStruct.h"
#include "B2LobbyUI_ItemSurpassMain.generated.h"

/**
* Main window of inventory's item Surpass operation.
*/
UCLASS()
class BLADEII_API UB2LobbyUI_ItemSurpassMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	/** The item data to be the target of composition. It is not supposed to be changed once pop-up this page. */
	FB2Item NativeSurpassTargetData;
	FB2Item SumSurpassData;
	TArray<int64> CurrIngredIDs;

protected:

	/** To sync commit animation and the real operation start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float SurpassCommitAnimLength;

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;

	TWeakObjectPtr<UCanvasPanel> CP_TargetItemIconSet;
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedTargetItemIcon;

	TWeakObjectPtr<UCanvasPanel> CP_IngredItemIconSet;
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedIngredItemIcon;

	TWeakObjectPtr<UPanelWidget> P_Selected;
	TWeakObjectPtr<UTextBlock> TB_CurrentLabel;
	TWeakObjectPtr<UTextBlock> TB_PostSurpassLabel;
	TWeakObjectPtr<UTextBlock> TB_CurrMaxLevelLabel;
	TWeakObjectPtr<UTextBlock> TB_CurrMaxLevel;
	TWeakObjectPtr<UTextBlock> TB_PostSurpassMaxLevelLabel;
	TWeakObjectPtr<UTextBlock> TB_PostSurpassMaxLevel;

	TWeakObjectPtr<UTextBlock> TB_NoIngredMessage;

	TWeakObjectPtr<UB2Button> BTN_SurpassCommit;
	TWeakObjectPtr<UTextBlock> TB_SurpassCommitBtn;
	TWeakObjectPtr<UTextBlock> TB_SurpassTitle;
	TWeakObjectPtr<UTextBlock> TB_SurpassCostNum;
	TWeakObjectPtr<UButton> BTN_Close;
		
	TWeakObjectPtr<UB2Button> BTN_IngredUnselect;

	TWeakObjectPtr<UTextBlock> TB_SurpassDescription;
	TWeakObjectPtr<UTextBlock> TB_SurpassUnselectDescription;

public:

	UB2LobbyUI_ItemSurpassMain(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;

	/** Mainly to be called when Surpass ingredient selection is changed. */
	void UpdateOnSurpassStateChange();

private:
	void UpdateSurpassInfo();
	void UpdateStaticTexts();
	void UpdateDynamicWidgets();
	void UpdateTargetItemIcon();
	void UpdateIngredItemIcon();
	void UpdateSurpassCommitBtnState();

	void DestroyTargetItemIcon();
	void DestroyIngredItemIcon();

	bool IsAnyIngredSelected();

public:

	UFUNCTION()
	void OnSurpassCommit();

	UFUNCTION()
	void OnCloseButtonPressed();
	
	UFUNCTION()
	void OnClickIngredUnselectBtn();

private:
	void CloseAndQuitItemOpMode();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginSurpassCommitAnim();

	FTimerHandle SurpassCommintTH;
	void FinalSurpassCommit();

	virtual void CloseWidgetDelegate() override;
};
