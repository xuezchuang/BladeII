// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "NetPlay/BladeIINetCharacter.h"
#include "BladeIIRaidNetCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API ABladeIIRaidNetCharacter : public ABladeIINetCharacter
{
	GENERATED_BODY()
	
public:
	ABladeIIRaidNetCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser) override;
};
