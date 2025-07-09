// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2FloatingHPBar.h"
#include "BladeIICharacter.h"

UB2FloatingHPBar::UB2FloatingHPBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AdditionalZAdjust = 0.0f;
}

void UB2FloatingHPBar::SetOwnerCharacter(class ABladeIICharacter* InOwner)
{
	Super::SetOwnerCharacter(InOwner);

	UpdateFloatingOffset();
}

float UB2FloatingHPBar::GetHPBarProgress() const
{
	if (OwnerCharacter)
	{
		return OwnerCharacter->GetHealth() / OwnerCharacter->GetMaxHealth();
	}
	return 0.0f;
}

void UB2FloatingHPBar::UpdateFloatingOffset()
{
	// Calculate the HPbar offset based on the character's bound size
	if (OwnerCharacter)
	{
		USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
		if (OwnerMesh && !OwnerCharacter->IsHPBarAttachedToSocket())
		{
			LocOffsetFromOwner.X = LocOffsetFromOwner.Y = 0.0f;
			// 기본적으로 머리에 붙이고 필요에 따라 AdditionalZAdjust 만큼 추가 조절.
			LocOffsetFromOwner.Z = 1.0f * (OwnerMesh->Bounds.BoxExtent.Z * OwnerMesh->BoundsScale) + AdditionalZAdjust;
		}
	}
}

void UB2FloatingHPBar::UpdatePosition()
{
	UpdateFloatingOffset(); // 이게 첫 시점에는 Bound 값이 완전히 초기화 안 되는 거 같다. 결국 매 틱 계산으로..
	Super::UpdatePosition();
}