// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUI_ItemCusProgBase.h"
#include "CommonStruct.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemUpgradeProg.generated.h"


/**
 * Progress scene UI page for Item Upgrade. Shares some common features with compose progress scene.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemUpgradeProg : public UB2LobbyUI_ItemCusProgBase
{
	GENERATED_BODY()
	
protected:

	//================================================================================
	// Fx stuff

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* BeginUpgradePS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector BeginUpgradeFxScale;
	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedBeginUpgradeFx;

	/** Particle system showing the upgrade complete (could be uniting of compose case) scene. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* CompleteUpgradePS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector CompleteUpgradeFxScale;
	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedCompleteUpgradeFx;

	//================================================================================

public:

	UB2LobbyUI_ItemUpgradeProg(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	void DestroySelf() override;

private:
	
	void UpdateStaticWidgets();

	// Names defined for NamedMaterialSlots of particle system template.
	const static FName Name_FxMID_TargetBGPanel;
	const static FName Name_FxMID_TargetItemIcon;
	//const static FName Name_FxMID_IngredBGPanel; // We might need to display ingredient for upgrade too, not just equip item, consumables.
	//const static FName Name_FxMID_IngredItemIcon;
	const static FName Name_FxMID_UpgradeBGPanel;
	const static FName Name_FxMID_UpgradeItemIcon;

protected:

	// Event Calls from Blueprint

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartBeginUpgradeScene();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartCompleteUpgradeScene();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowBattleScorePopup();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartUpgradeSoundfx();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayResultSound(bool IsSuccess);
private:


	virtual void DestroyAllFx() override;
	void DestroyBeginUpgradeFx();
	void DestroyCompleteUpgradeFx();
		
	virtual bool IsSupportedOpMode(ELobbyInvenItemOpMode InMode) const override { return (InMode == ELobbyInvenItemOpMode::EIVIOP_Upgrade); }

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void ShowCloseButton();
};
