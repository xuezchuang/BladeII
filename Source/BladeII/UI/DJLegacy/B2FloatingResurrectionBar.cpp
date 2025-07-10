// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingResurrectionBar.h"
//#include "BladeII.h"

#include "BladeIINetPlayer.h"

UB2FloatingResurrectionBar::UB2FloatingResurrectionBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AdditionalZAdjust = 0.0f;
}

void UB2FloatingResurrectionBar::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UOverlay, O_HP);
	GET_SLOT(UOverlay, O_Avoid);

	if (O_Avoid.IsValid())
		O_Avoid->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2FloatingResurrectionBar::UpdatePosition()
{
	ForceLayoutPrepass();

	FinalScreenOffset.X = -GetDesiredSize().X / 2;
	FinalScreenOffset.Y = -GetDesiredSize().Y / 2;

	Super::UpdatePosition();	
}

void UB2FloatingResurrectionBar::UpdateFloatingOffset()
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

void UB2FloatingResurrectionBar::NotifyCharacterDeath(class ABladeIICharacter* InPlayer)
{
	if (O_HP.IsValid())
		O_HP->SetVisibility(ESlateVisibility::Collapsed);

	if (O_Avoid.IsValid())
		O_Avoid->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


float UB2FloatingResurrectionBar::GetResurrectionBarProgress() const
{
	if (ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(OwnerCharacter))
	{
		if (pPlayer->bIsDying)
			return pPlayer->GetResurrectionTimerRate();
	}

	return 0.0f;		
}

