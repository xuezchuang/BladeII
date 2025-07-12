// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_EnhLevelProgElem.h"
#include "../B2RichTextBlock.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2WingUIP_Enhancement.generated.h"

/**
 * Wing enhancement menu on wing main management page. (Evolution menu can replace this menu at the same place).
 */
UCLASS()
class BLADEII_API UB2WingUIP_Enhancement : public UBladeIIUserWidget
{
	GENERATED_BODY()

protected:

	FB2Wing LastCachedWingData;

	TWeakObjectPtr<UTextBlock> TB_EnhancementTitle;

	/** They show current enhance level progress by each widget element's visibility setting. */
	UPROPERTY()
	TArray<UB2WingUIP_EnhLevelProgElem*> LevelProgElements;
	/** Used for finding LevelProgElements */
	UPROPERTY(EditAnywhere)
	FString LevelProgElemPrefix;

	TWeakObjectPtr<UTextBlock> TB_EnhanceLevelLabel;

	TWeakObjectPtr<UImage> IMG_EnhanceIngred;
	TWeakObjectPtr<UTextBlock> TB_CurrentIngredNum;
	TWeakObjectPtr<UTextBlock> TB_RequiredIngredNum;

	TWeakObjectPtr<UProgressBar> PB_EnhancePoint;
	TWeakObjectPtr<UTextBlock> TB_EnhancePointProgress;

	TWeakObjectPtr<UB2RichTextBlock> TB_EnhanceNote;

	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_EnhanceGoodInfo;

	// For enhance "point" progress effect. Placed over the PB_EnhancePoint progress bar.
	TWeakObjectPtr<UCanvasPanel> X_CP_ProgressEffectPanel;
	TWeakObjectPtr<UImage> IMG_ProgressEffect;
	UPROPERTY(EditAnywhere)
	float ProgressEffectFlashDuration;
	FTimerHandle ProgressEffectFlashTH;

public:

	UB2WingUIP_Enhancement(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	void InitFromWingMain();

	void PlayLevelupAnim();

	void UpdateByWingData(const FB2Wing& InWingData);
private:

	void UpdateStaticWidgets();

	void BeginShowProgressEffect(float InProgress); // InProgress 0.0 ~ 1.0
	void FinishShowProgressEffect(); // To be called by timer

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayEnhanceProgressFxAnim();
};

