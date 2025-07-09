// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2LobbyUI_AutoReinforceResult.generated.h"

UCLASS()
class BLADEII_API UB2LobbyUI_AutoReinforceResult : public UB2LobbyUserWidget
{
	GENERATED_BODY()

protected:

	/** Directly contained in ItemDisplayPanel scrollbox, each ItemIconRow contains certain number of item icons.
	 * It is still supposed to be a canvas panel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	TSubclassOf<class UB2DynItemIconRow_LobbyInven> ItemIconRowWidgetClass;

	/** The "outermost" container panel (scroll box) of dynamically created item icons.
	 * it directly contains item rows (ItemIconRowWidgetClass) and each row contains the final item icons. */
	TWeakObjectPtr<UScrollBox> SB_ItemDisplayPanel;

	/** Dynamically created item icon rows */
	UPROPERTY(Transient)
	TArray<class UB2DynItemIconRow_LobbyInven*> CreatedItemRows;

	TWeakObjectPtr<UTextBlock> TB_AutoEnhanceResultTitle;
	TWeakObjectPtr<UTextBlock> TB_AutoEnhanceCompleteMsg;
	TWeakObjectPtr<UTextBlock> TB_ConfirmBtn;

public:
	
	UB2LobbyUI_AutoReinforceResult(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	virtual void DestroySelf() override;

private:

	void UpdateStaticTexts();

	void UpdateResultItemIcons(const TArray<FB2Item>& InResultItems);
	void DestroyResultItemIcons();

protected:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnConfirmBtnClicked();

private:

	void CloseMe();
};
