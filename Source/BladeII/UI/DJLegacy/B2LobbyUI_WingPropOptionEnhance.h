// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_PropOptionDetail.h"
#include "B2UIBackWidget.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2LobbyUI_WingPropOptionEnhance.generated.h"

/**
 * Wing's prop option enhancement popup menu, showing all prop options of a wing.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_WingPropOptionEnhance : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
		
	
	UPROPERTY(Transient)
	class UB2Airport* CachedAirport;

protected:
	void CloseWidgetDelegate() override;

	TWeakObjectPtr<UB2Button> BTN_Close;
	
	TWeakObjectPtr<UTextBlock> TB_PropOptionEnhanceTitle;
	TWeakObjectPtr<UTextBlock> TB_PropOptionEnhanceNote;

	TWeakObjectPtr<UTextBlock> TB_AngelFeatherLabel;
	TWeakObjectPtr<UTextBlock> TB_AngelFeatherCount;
	TWeakObjectPtr<UImage> IMG_AngelFeather;

	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_Goods;

	/** Naming prefix to find PropOption icon part */
	UPROPERTY(EditAnywhere)
	FString PropOptionDetailSlotPrefix;
	TArray<UB2WingUIP_PropOptionDetail*> PropOptionDetailSlots;

public:

	UB2LobbyUI_WingPropOptionEnhance(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;


private:
	void UpdateStaticWidgets();
	
public:
	void UpdateByCurrentWingData();
	
private:

	UFUNCTION()
	void OnClickCloseBtn();

	void CloseMe();
};
