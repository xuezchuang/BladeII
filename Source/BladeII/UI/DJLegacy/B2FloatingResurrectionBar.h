// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2FloatingHPBar.h"
#include "B2FloatingResurrectionBar.generated.h"

/**
 * Simple HP bar UI widget, for common mobs. It is expected to be floating, but could be static at some screen location too..?
 */
UCLASS()
class BLADEII_API UB2FloatingResurrectionBar : public UB2FloatingHPBar
{
	GENERATED_BODY()

public:
	UB2FloatingResurrectionBar(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void UpdatePosition() override;
	virtual void UpdateFloatingOffset() override;

	virtual void NotifyCharacterDeath(class ABladeIICharacter* InPlayer) override;
	
	/** Get the value for HP progress bar in 0.0 ~ 1.0 scale */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float GetResurrectionBarProgress() const;

private:
	TWeakObjectPtr<UOverlay>	O_HP;
	TWeakObjectPtr<UOverlay>	O_Avoid;
};

