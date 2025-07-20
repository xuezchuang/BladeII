// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingHPBar.h"
//#include "BladeII.h"

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
			// 扁夯利栏肺 赣府俊 嘿捞绊 鞘夸俊 蝶扼 AdditionalZAdjust 父怒 眠啊 炼例.
			LocOffsetFromOwner.Z = 1.0f * (OwnerMesh->Bounds.BoxExtent.Z * OwnerMesh->BoundsScale) + AdditionalZAdjust;
		}
	}
}

void UB2FloatingHPBar::UpdatePosition()
{
	UpdateFloatingOffset(); // 捞霸 霉 矫痢俊绰 Bound 蔼捞 肯傈洒 檬扁拳 救 登绰 芭 鞍促. 搬惫 概 平 拌魂栏肺..
	Super::UpdatePosition();
}