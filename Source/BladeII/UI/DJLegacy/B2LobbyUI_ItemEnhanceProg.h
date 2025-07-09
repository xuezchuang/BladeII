// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonStruct.h"

#include "UI/DJLegacy/B2LobbyUI_ItemCUSProgBase.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemEnhanceProg.generated.h"

/**
* Progress scene UI page for both Compose and Surpass.
*/
UCLASS()
class BLADEII_API UB2LobbyUI_ItemEnhanceProg : public UB2LobbyUI_ItemCusProgBase
{
	GENERATED_BODY()
		
public:
	UB2LobbyUI_ItemEnhanceProg(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	void DestroySelf() override;


protected:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartGettingCloseScene();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartUnitingScene();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartBGSoundfx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ShowBattleScorePopup();

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartShowResultItemIcon() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayResultSound(bool IsSuccess);
private:
	virtual void DestroyAllFx() override;

	void UpdateStaticWidgets();
	void SetItemData_Ingred(FB2Item& InItem);
	void DestroyItemIcon_Ingred();
	void DestroyGettingCloseFx();
	void DestroyUnitingFx();
	virtual bool IsSupportedOpMode(ELobbyInvenItemOpMode InMode) const { return (InMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement); }
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void ShowCloseButton();
protected:
	const static FName Name_FxMID_TargetBGPanel;
	const static FName Name_FxMID_TargetItemIcon;
	const static FName Name_FxMID_IngredBGPanel;
	const static FName Name_FxMID_IngredItemIcon;
	const static FName Name_FxMID_UnitingBGPanel;
	const static FName Name_FxMID_UnitingItemIcon;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* GettingClosePS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector GettingCloseFxScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* EnhanceSuccessUnitingPS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector EnhanceSuccessUnitingFxScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* EnhanceFailUnitingPS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector EnhanceFailUnitingFxScale;

	UPROPERTY(Transient)
	class UB2DynItemIcon_ItemOpScene* CreatedItemIcon_Ingred;

	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedGettingCloseFx;

	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedUnitingFx;

protected:

	/** Final result user widget page, as a part here. */
	TWeakObjectPtr<UCanvasPanel> X_CP_ItemIconSet_Ingred;

protected:
	FB2Item NativeItemData_Ingred;
};
