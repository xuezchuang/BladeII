// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2FloatingWidget.h"
#include "B2FloatingHPBar.generated.h"

/**
 * Simple HP bar UI widget, for common mobs. It is expected to be floating, but could be static at some screen location too..?
 */
UCLASS()
class BLADEII_API UB2FloatingHPBar : public UB2FloatingWidget
{
	GENERATED_BODY()

protected:
	
	/** LocOffsetFromOwner is calculated internally, so provide this to additionally adjust its placed z coordinate, relative to the owner character.
	 * FinalScreenOffset is still applied after applying this */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float AdditionalZAdjust;

public:
	UB2FloatingHPBar(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyCharacterDeath(class ABladeIICharacter* InPlayer) {}
	virtual void SetOwnerCharacter(class ABladeIICharacter* InOwner) override;

	/** Get the value for HP progress bar in 0.0 ~ 1.0 scale */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float GetHPBarProgress() const;
	
private:
	/** It calculates LocOffsetFromOwner based on the character bound size. */
	virtual void UpdateFloatingOffset();

public:
	virtual void UpdatePosition() override;
};

