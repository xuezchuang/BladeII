// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingAbnormalEffect.h"
//#include "BladeII.h"

#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"


UB2FloatingAbnormalEffect::UB2FloatingAbnormalEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VisibleDuration = 1.0f; // UIManager_InGameHUD 率俊辑 荤侩.

	AdditionalZOffset = 100.0f;
	RandomPlaceDistanceFromOwner = 50.0f;

	InitialProjectedLocation = FVector2D(0.0f, 0.0f);

	UpArrowTexture = NULL;
	DownArrowTexture = NULL;
}

void UB2FloatingAbnormalEffect::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_AbnormalEffectText);
	GET_SLOT(UImage, IMG_Arrow);
}

void UB2FloatingAbnormalEffect::SetOwnerCharacter(class ABladeIICharacter* InOwner)
{
	Super::SetOwnerCharacter(InOwner);

	UpdateFloatingOffset();
}

void UB2FloatingAbnormalEffect::NotifyAbnormalEffect(const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo)
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwnerCharacter && OwningPC)
	{
		// 概锅 距埃究 促弗 困摹俊 谎府扁 困秦 LocOffsetFromOwner 甫 粱 炼沥.
		float RandDirX = FMath::FRandRange(-1.0f, 1.0f);
		float RandDirY = FMath::FRandRange(-1.0f, 1.0f);
		FVector2D RandDir(RandDirX, RandDirY);
		RandDir.Normalize();

		LocOffsetFromOwner.X = RandomPlaceDistanceFromOwner * RandDir.X;
		LocOffsetFromOwner.Y = RandomPlaceDistanceFromOwner * RandDir.Y;

		// 捞 矫痢俊辑 官肺 弥辆 screen 谅钎甫 沥秦辑 技泼茄促. UpdatePosition 俊辑 概 平 OwnerCharacter 蝶扼啊霸 沁歹聪 粱 冻赴促绰 狼斑捞 乐绢辑..
		FVector WidgetWorldLocation = OwnerCharacter->GetActorLocation() + LocOffsetFromOwner;

		if (OwningPC->ProjectWorldLocationToScreen(WidgetWorldLocation, InitialProjectedLocation))
		{
			InitialProjectedLocation += FinalScreenOffset;
			SetPositionInViewport(InitialProjectedLocation);
		}
	}

	if (IMG_AbnormalEffectText.IsValid())
	{ // 阿 捞惑惑怕 喊 咆胶飘 material 篮 寇何 抛捞喉俊辑 林绢咙.
		IMG_AbnormalEffectText->SetBrushFromTexture(InEffectSetupInfo.LoadedTextEffectTexture);
		IMG_AbnormalEffectText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (IMG_Arrow.IsValid())
	{
		IMG_Arrow->SetVisibility(InEffectSetupInfo.ArrowType != EFloatingAbnormalEffectArrowType::FAXAT_None ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		if (InEffectSetupInfo.ArrowType == EFloatingAbnormalEffectArrowType::FAXAT_Up)
		{
			IMG_Arrow->SetBrushFromTexture(UpArrowTexture);
		}
		else if (InEffectSetupInfo.ArrowType == EFloatingAbnormalEffectArrowType::FAXAT_Down)
		{
			IMG_Arrow->SetBrushFromTexture(DownArrowTexture);
		}
	}

	if (InEffectSetupInfo.AdditionalZOffset != 0.0f)
	{
		AdditionalZOffset += InEffectSetupInfo.AdditionalZOffset; // Setup 狼 AdditionalZOffset 篮 咯扁狼 AdditionalZOffset 俊 眠啊肺 歹秦咙.
		UpdateFloatingOffset(); // 捞芭 酒付 概 平 救阂副 芭..
	}

	NotifyAbnormalEffect_BP(InEffectSetupInfo.ArrowType); // Call the BP event too.
}

bool UB2FloatingAbnormalEffect::IsOwnerPC()
{
	return (Cast<ABladeIIPlayer>(OwnerCharacter) != NULL);
}

void UB2FloatingAbnormalEffect::UpdateFloatingOffset()
{
	// Calculate the offset based on the character's bound size
	if (OwnerCharacter)
	{
		if (USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh())
		{
			// Leave LocOffsetFromOwner X and Y. We can still configure it.
			LocOffsetFromOwner.Z = (OwnerMesh->Bounds.BoxExtent.Z * OwnerMesh->BoundsScale) + AdditionalZOffset; // Apply additional offset variable.
		}
	}
}