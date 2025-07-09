// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2FloatingHPBarForControlGameMode.h"
#include "B2ControlModePlayer.h"

UB2FloatingHPBarForControlGameMode::UB2FloatingHPBarForControlGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AdditionalZAdjust = 0.0f;
}

void UB2FloatingHPBarForControlGameMode::UpdatePosition()
{
	ForceLayoutPrepass();

	FinalScreenOffset.X = -GetDesiredSize().X / 2;
	FinalScreenOffset.Y = -GetDesiredSize().Y / 2;

	Super::UpdatePosition();
}

void UB2FloatingHPBarForControlGameMode::UpdateFloatingOffset()
{
	// Calculate the HPbar offset based on the character's bound size
	if (OwnerCharacter)
	{
		UCapsuleComponent* pComp = Cast<UCapsuleComponent>(OwnerCharacter->GetRootComponent());

		if (pComp)
		{
			LocOffsetFromOwner.X = LocOffsetFromOwner.Y = 0.0f;
			LocOffsetFromOwner.Z = 1.0f * (pComp->GetScaledCapsuleHalfHeight()) + AdditionalZAdjust;
		}
	}
}

uint8 UB2FloatingHPBarForControlGameMode::GetOwnerPlayerTeamIndex() const
{
	AB2ControlModePlayer* pPlayer = Cast<AB2ControlModePlayer>(OwnerCharacter);
	if (pPlayer)
	{
		return pPlayer->GetTeamNum();
	}

	return 0;
}
