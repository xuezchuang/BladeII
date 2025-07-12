// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_EvolveGradeStar.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "../B2RichTextBlock.h"
#include "B2WingUIP_Evolution.generated.h"

/**
 * Wing evolution menu on wing main management page. (Enhancement menu can replace this menu at the same place).
 */
UCLASS()
class BLADEII_API UB2WingUIP_Evolution : public UBladeIIUserWidget
{
	GENERATED_BODY()

protected:

	/** How long the evolution fail message will be visible, in second unit. */
	UPROPERTY(EditAnywhere)
	float EvolutionFailMessageShowTime;

	TWeakObjectPtr<UTextBlock> TB_EvolutionTitle;

	TWeakObjectPtr<UImage> IMG_EvolveIngred;
	TWeakObjectPtr<UTextBlock> TB_CurrentIngredNum;
	TWeakObjectPtr<UTextBlock> TB_RequiredIngredNum;

	TWeakObjectPtr<UOverlay> OV_NormalEvolveMenuSet;
		TWeakObjectPtr<UB2WingUIP_EvolveGradeStar> UIP_EvolveGradeStar;

		TWeakObjectPtr<UTextBlock> TB_EvolveSuccessRateLabel;
		TWeakObjectPtr<UTextBlock> TB_EvolveSuccessRate;

		TWeakObjectPtr<UProgressBar> PB_EvolvePoint;
		TWeakObjectPtr<UTextBlock> TB_EvolvePointLabel;

		TWeakObjectPtr<UTextBlock> TB_EvolveFailMessage;

		TWeakObjectPtr<UB2RichTextBlock> TB_EvolveNote;

	TWeakObjectPtr<UOverlay> OV_MaxEvolvedSet; // To be on when evolution is at maximum grade.
		TWeakObjectPtr<UB2WingUIP_EvolveGradeStar> UIP_EvolveGradeStarAtMax;
		TWeakObjectPtr<UB2RichTextBlock> TB_MaxEvolveNote;
		TWeakObjectPtr<UB2ButtonGoodInfoToolTip>	BTN_EvolutionGoodInfo;

public:

	UB2WingUIP_Evolution(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	void InitFromWingMain();

	void UpdateByWingData(const FB2Wing& InWingData);

	void BeginShowEvolutionFailMessage();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void BeginShowEvolutionFailMessage_BP();
private:
	FTimerHandle EvolutionFailMessageShowTH;
	void StopShowEvolutionFailMessage();
public:
	void ForceStopShowEvolutionFailMessage();

private:
	void UpdateStaticWidgets();
};

