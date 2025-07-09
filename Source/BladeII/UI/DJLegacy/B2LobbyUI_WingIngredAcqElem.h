// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2LobbyUI_WingIngredAcqElem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWingIngredAcqElemClickDelgate, UB2LobbyUI_WingIngredAcqElem*, ClickedItem);

/**
 * A single element composing WingIngredAcqPath pop-up.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_WingIngredAcqElem : public UB2LobbyUserWidget
{
	GENERATED_BODY()

	// For which type of ingredient this element provide path info.
	EWingIngredAcqElemType MyType;

protected:

	TWeakObjectPtr<UImage> IMG_IngredIcon;
	TWeakObjectPtr<UTextBlock> TB_AcquirePath;
	TWeakObjectPtr<UTextBlock> TB_Go;
	TWeakObjectPtr<UB2Button> BTN_Go;

public:
	// For notifying its owning pop-up page.
	FOnWingIngredAcqElemClickDelgate OnGoBtnClickDelegate; 

public:

	UB2LobbyUI_WingIngredAcqElem(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;

	void SetAcqElemType(EWingIngredAcqElemType InType);

private:
	void UpdateStaticWidgets();

	void UpdateWidgetsOnTypeChange();

	UFUNCTION()
	void OnClickBtnGo();
};
