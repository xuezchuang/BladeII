// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUI_ItemCUSProgBase.h"
#include "CommonStruct.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemCSProg.generated.h"

/**
 * Progress scene UI page for both Compose and Surpass.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemCSProg : public UB2LobbyUI_ItemCusProgBase
{
	GENERATED_BODY()
	
protected:
	
	FB2Item NativeItemData_Ingred; // Target and Result data are at base class, and Ingredient is available for compose and surpass.

	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconSet_Ingred;

	/** Created item icon inside X_CP_ItemIconSet_Ingred** 
	 * Even when it is not being used, need to be created if its relevant Fx are to be created, 
	 * as Fx material uses UI icon's material parameter to set up the same item outlook. */
	UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* CreatedItemIcon_Ingred;


	//================================================================================
	// Fx stuff
	
	/** Particle system showing target and ingredient getting close, for compose and surpass. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* GettingClosePS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector GettingCloseFxScale;	
	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedGettingCloseFx;
	
	/** Particle system showing target and ingredient are being united. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* UnitingPS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector UnitingFxScale;
	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedUnitingFx;

	//================================================================================


public:

	UB2LobbyUI_ItemCSProg(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	void DestroySelf() override;

private:
	
	void UpdateStaticWidgets();

	// Names defined for NamedMaterialSlots of particle system template.
	const static FName Name_FxMID_TargetBGPanel;
	const static FName Name_FxMID_TargetItemIcon;
	const static FName Name_FxMID_IngredBGPanel;
	const static FName Name_FxMID_IngredItemIcon;
	const static FName Name_FxMID_UnitingBGPanel;
	const static FName Name_FxMID_UnitingItemIcon;

protected:

	/** Event from Blueprint, notifying that target and ingredient item are start getting close then united soon. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartGettingCloseScene();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartUnitingScene();
		
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartBGSoundfx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowBattleScorePopup();

private:

	// Set data for each item, and create UMG icon representing it. Icons need to be set invisible from the first moment or at some point.
	void SetItemData_Ingred(FB2Item& InItem);

	void DestroyItemIcon_Ingred();

	virtual void DestroyAllFx() override;

	void DestroyGettingCloseFx();
	void DestroyUnitingFx();
		
	virtual bool IsSupportedOpMode(ELobbyInvenItemOpMode InMode) const { return (InMode == ELobbyInvenItemOpMode::EIVIOP_Composition || InMode == ELobbyInvenItemOpMode::EIVIOP_Surpass); }

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void ShowCloseButton();
};
