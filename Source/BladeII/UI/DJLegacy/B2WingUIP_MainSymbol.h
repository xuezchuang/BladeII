// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2WingUIP_PropOptionIcon.h"
#include "B2WingUIP_EvolveGradeStar.h"
#include "B2WingUIP_MainSymbol.generated.h"

/**
 * The big-ass wing symbol part with evolution grade and prop option icons displayed.
 */
UCLASS()
class BLADEII_API UB2WingUIP_MainSymbol : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
	/** Source of displayed information here. */
	UPROPERTY(Transient)
	class UB2Airport* CachedAirport;

protected:
	
	TWeakObjectPtr<UImage> IMG_BigFatIcon; // Big-Fat

	TWeakObjectPtr<UB2WingUIP_EvolveGradeStar> UIP_EvolveGradeStar;

	/** Naming prefix to find PropOption icon part */
	UPROPERTY(EditAnywhere)
	FString UIPPropOptionPrefix;

	UPROPERTY(EditAnywhere)
	FString ExtraActivatedImagePrefix;
	
	TArray<UB2WingUIP_PropOptionIcon*> PropOptionIcons;
	TArray<UImage*> ExtraActivatedImages; // Extra images for some effect, which cannot be in PropOpitonIcon

	TWeakObjectPtr<UTextBlock> TB_WingTitle;

public:

	UB2WingUIP_MainSymbol(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	void DestroySelf() override;

	void SetPropIconUsageType(EWingPropOptionUIPUsage InUsageType);

	/** Main entry point of update widgets here.
	 * To be called when current character's wing data has been just changed, or character is just switched. */
	void UpdateByCurrentWingData();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void UpdateByCurrentWingData_BP(EPCClass InOwnerClass, int32 InEvolveGrade, int32 InEnhanceLevel);

	/** For temporarily disable buttons for any reason like some auto execution task.. */
	void SetExtraDisableButtons(bool bInDisable);

private:
	
	void UpdateStaticWidgets();
		

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayOpeningAnim();

};
