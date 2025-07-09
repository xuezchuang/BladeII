// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2WingUIP_MainSymbol.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_WingEvolveResult.generated.h"

/**
 * UI page for Wing evolution result, after evolution scene.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_WingEvolveResult : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
	
	UPROPERTY(Transient)
	class UB2Airport* CachedAirport;

protected:

	/** Containing wing name, evolution grade, prop option icons */
	TWeakObjectPtr<UB2WingUIP_MainSymbol> UIP_MainSymbol;

	TWeakObjectPtr<UTextBlock> TB_AttackIncLabel;
	TWeakObjectPtr<UTextBlock> TB_AttackIncAmount;
	TWeakObjectPtr<UImage> IMG_AttackIncSign;

	TWeakObjectPtr<UTextBlock> TB_DefenseIncLabel;
	TWeakObjectPtr<UTextBlock> TB_DefenseIncAmount;
	TWeakObjectPtr<UImage> IMG_DefenseIncSign;

	TWeakObjectPtr<UTextBlock> TB_HealthIncLabel;
	TWeakObjectPtr<UTextBlock> TB_HealthIncAmount;
	TWeakObjectPtr<UImage> IMG_HealthIncSign;
		
	TWeakObjectPtr<UTextBlock> TB_PropOptionLabel;
	TWeakObjectPtr<UImage> IMG_NewPropOptionIcon;
	TWeakObjectPtr<UTextBlock> TB_NewPropOptionInfo;
	TWeakObjectPtr<UTextBlock> TB_NewPropOptionInfo2;
	
	TWeakObjectPtr<UTextBlock> TB_Suc;

	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UTextBlock> TB_Confirm;

	TWeakObjectPtr<UOverlay>	O_OpsionIcon;
	TWeakObjectPtr<UOverlay>	O_Complete;
	TWeakObjectPtr<UTextBlock>	TB_Complete;

public:

	UB2LobbyUI_WingEvolveResult(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	void DestroySelf() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowBattleScoreNotify();

private:
	
	void UpdateStaticWidgets();
	
	// The main shit here.
	void UpdateByWingEvolutionData(const FB2Wing& PrevWing, const FB2Wing& EvolvedWing);

protected:

	UFUNCTION()
	void OnClickedBtnConfirm();

	virtual void CloseWidgetDelegate() override;
};
