// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_MainSymbol.h"
#include "B2WingUIP_Enhancement.h"
#include "B2WingUIP_Evolution.h"
#include "B2LobbyUI_WingIngredAcqElem.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_WingMain.generated.h"

/**
 * Main wing management page user widget.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_WingMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
	/** Source of displayed information here. */
	UPROPERTY(Transient)
	class UB2Airport* CachedAirport;

protected:
	/** Contains common part of several hero management UI pages. */
	TWeakObjectPtr<class UB2LobbyUI_HeroMgmtCommon> UIP_HeroMgmtBase;

	/** Containing wing name, evolution grade, prop option icons */
	TWeakObjectPtr<UB2WingUIP_MainSymbol> UIP_WingMenuUpper_MainSymbol;
		
	TWeakObjectPtr<UTextBlock> TB_AttackIncLabel;
	TWeakObjectPtr<UTextBlock> TB_AttackIncAmount;

	TWeakObjectPtr<UTextBlock> TB_DefenseIncLabel;
	TWeakObjectPtr<UTextBlock> TB_DefenseIncAmount;

	TWeakObjectPtr<UTextBlock> TB_HealthIncLabel;
	TWeakObjectPtr<UTextBlock> TB_HealthIncAmount;

	TWeakObjectPtr<UB2Button> BTN_AutoEnhanceOrEvolve;
	TWeakObjectPtr<UB2Button> BTN_EnhanceOrEvolve;
	TWeakObjectPtr<UB2Button> BTN_WingBook;

	TWeakObjectPtr<UTextBlock> TB_AutoEnhanceOrEvolveBtn;
	TWeakObjectPtr<UTextBlock> TB_EnhanceOrEvolveBtn;
	TWeakObjectPtr<UTextBlock> TB_EnhanceOrEvolveCost;
	TWeakObjectPtr<UTextBlock> TB_WingBookBtn;


	//////////////////////////////////////////////////////////
	// Dynamically created Enhancement/Evolution part
	// Only one among Enhance or Evolution menu is visible at one time.
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2WingUIP_Enhancement> UIP_Enhancement_WidgetClass;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2WingUIP_Evolution> UIP_Evolution_WidgetClass;
	
	UPROPERTY(Transient)
	UB2WingUIP_Enhancement* CreatedEnhancementPart;
	UPROPERTY(Transient)
	UB2WingUIP_Evolution* CreatedEvolutionPart;

	TWeakObjectPtr<UCanvasPanel> X_CP_EnhanceOrEvolutionBase;

	//////////////////////////////////////////////////////////

public:

	UB2LobbyUI_WingMain(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;
	virtual void DestroySelf() override;
	virtual void BeginDestroy() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void RegisterUIMarkForRedDot() final;
	virtual void UnRegisterUIMarkForRedDot() final;
	virtual void DoMarkRedDot() final;

	/** Main entry point of update widgets here. 
	 * To be called when current character's wing data has been just changed, or character is just switched. */
	void UpdateByCurrentWingData();
	
	void PlayOnEnhancedAnim();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayOnEnhancedAnim_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayOnFailAnim_BP();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnFailEndAnimation_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayOnSoundEvolution_BP();

	void BeginShowEvolutionFailMessage();

	void UpdateOnAutoEnhanceComplete();
	void UpdateOnAutoEvolveComplete();

	void SetBattleScoreNotify();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowBattleScoreNotify();

	UFUNCTION()
	void BeginShowWingEvolutionSuccessMessage();
private:
	void CloseWidgetDelegate() override;

	void UpdateStaticWidgets();

	bool NeedToShowEvolveMenu() const;
	bool NeedToShowMaxEvolveMenu() const;

	void UpdateAutoEnhanceOrEvolveBtnText();

	UFUNCTION()
	void OnClickedAutoEnhanceOrEvolveBtn();

	UFUNCTION()
	void OnClickedEnhanceOrEvolveBtn();

	UFUNCTION()
	void OnClickedWingBookBtn();

	//////////////////////////////////////////////////////////
	// Enhancement/Evolution part dynamic creation.

	void SetOpenEnhancementPart(bool bInOpen); // Open or Close, dynamically create if not created yet.
	void SetOpenEvolutionPart(bool bInOpen);

	void ConditionalCreateEnhancementPart();
	void ConditionalCreateEvolutionPart();
	
	void DestroyEnhancementPart();
	void DestroyEvolutionPart();

	//////////////////////////////////////////////////////////

public:
	void OnHeroMgmtPCChanged(EPCClass InSelectedPCClass);
	void ShowUpgradeStatNotify();

	int32 TempBattleScoreBefore = -1;
	int32 TempBattleScoreAfter = -1;

	int32 BeforeAttPoint = -1;
	int32 BeforeDefPoint = -1;
	int32 BeforeHPPoint = -1;

private:
	uint32 LobbySetHeroMgmtModePCSelectionTicket;

};

// Util for use of each ingredient using spots, enhancement, evolution, prop-option enhancement. Whenever wanted ingredient is not enough.
void OpenWingIngredAcqPathPopup(EWingIngredAcqElemType InPopupMainReason);