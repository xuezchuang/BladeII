// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeIIRaidNetCharacter.h"
#include "B2ControlGameMode.h"

ABladeIIRaidNetCharacter::ABladeIIRaidNetCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void ABladeIIRaidNetCharacter::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser)
{
	Super::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);
	
}
