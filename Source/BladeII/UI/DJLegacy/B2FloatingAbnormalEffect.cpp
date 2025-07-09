// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2FloatingAbnormalEffect.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"


UB2FloatingAbnormalEffect::UB2FloatingAbnormalEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VisibleDuration = 1.0f; // UIManager_InGameHUD 쪽에서 사용.

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
		// 매번 약간씩 다른 위치에 뿌리기 위해 LocOffsetFromOwner 를 좀 조정.
		float RandDirX = FMath::FRandRange(-1.0f, 1.0f);
		float RandDirY = FMath::FRandRange(-1.0f, 1.0f);
		FVector2D RandDir(RandDirX, RandDirY);
		RandDir.Normalize();

		LocOffsetFromOwner.X = RandomPlaceDistanceFromOwner * RandDir.X;
		LocOffsetFromOwner.Y = RandomPlaceDistanceFromOwner * RandDir.Y;

		// 이 시점에서 바로 최종 screen 좌표를 정해서 세팅한다. UpdatePosition 에서 매 틱 OwnerCharacter 따라가게 했더니 좀 떨린다는 의견이 있어서..
		FVector WidgetWorldLocation = OwnerCharacter->GetActorLocation() + LocOffsetFromOwner;

		if (OwningPC->ProjectWorldLocationToScreen(WidgetWorldLocation, InitialProjectedLocation))
		{
			InitialProjectedLocation += FinalScreenOffset;
			SetPositionInViewport(InitialProjectedLocation);
		}
	}

	if (IMG_AbnormalEffectText.IsValid())
	{ // 각 이상상태 별 텍스트 material 은 외부 테이블에서 주어짐.
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
		AdditionalZOffset += InEffectSetupInfo.AdditionalZOffset; // Setup 의 AdditionalZOffset 은 여기의 AdditionalZOffset 에 추가로 더해짐.
		UpdateFloatingOffset(); // 이거 아마 매 틱 안불릴 거..
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