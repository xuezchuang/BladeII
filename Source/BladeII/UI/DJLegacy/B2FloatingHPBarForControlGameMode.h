// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2FloatingHPBar.h"
#include "B2FloatingHPBarForControlGameMode.generated.h"

/**
 * Simple HP bar UI widget, for common mobs. It is expected to be floating, but could be static at some screen location too..?
 */
UCLASS()
class BLADEII_API UB2FloatingHPBarForControlGameMode : public UB2FloatingHPBar
{
	GENERATED_BODY()

public:
	UB2FloatingHPBarForControlGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void UpdatePosition() override;
	virtual void UpdateFloatingOffset() override;
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	uint8 GetOwnerPlayerTeamIndex() const;
};

