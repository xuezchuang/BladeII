// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_EnhLevelProgElem.generated.h"

/**
 * Single enhance level piece of enhancement level progress for wing enhancement menu.
 */
UCLASS()
class BLADEII_API UB2WingUIP_EnhLevelProgElem : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
protected:

	int32 MyEnhanceLevel; // The enhance level that this piece represents.

	UPROPERTY(EditAnywhere)
	float LevelUpAnimPlayTime;

	FTimerHandle LevelUpAnimStopTH;

	TWeakObjectPtr<UImage> IMG_Filled; // Visible when enhancement level is same or higher than MyEnhanceLevel
	TWeakObjectPtr<UImage> IMG_Unfilled; // Visible when enhancement level is lower than MyEnhanceLevel (Or could be always visible)

	TWeakObjectPtr<UImage> FX_Enhance; // The image for level up anim.

	TWeakObjectPtr<UTextBlock> TB_MyEnhanceLevelNum;

public:

	UB2WingUIP_EnhLevelProgElem(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	void SetMyEnhanceLevel(int32 InLevel);
	// Update for the main job here.
	void UpdateByWingEnhanceLevel(int32 InWingLevel);

	FORCEINLINE int32 GetMyEnhanceLevel() const { return MyEnhanceLevel; }

	void PlayLevelupAnim();

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void PlayLevelupAnim_BP();
		
	void StopLevelupAnim();
};

