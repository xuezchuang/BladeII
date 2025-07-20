// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingWidget.h"
#include "BladeIICharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"


UB2FloatingWidget::UB2FloatingWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerCharacter = NULL;
	LocOffsetFromOwner = FVector(0.0f, 0.0f, 0.0f);
	FinalScreenOffset = FVector2D(0.0f, 0.0f);
}

void UB2FloatingWidget::SetOwnerCharacter(class ABladeIICharacter* InOwner)
{
	OwnerCharacter = InOwner; // We may expect OwnerCharacter is NULL.
}

void UB2FloatingWidget::UpdatePosition()
{
	//if (OwnerCharacter)
	//{
	//	// Visibility settings here assumes this widget is for information visualization.

	//	ESlateVisibility SelfVisibility = ESlateVisibility::Hidden;

	//	if (APlayerController* PC = UGameplayStatics::GetLocalPlayerController(OwnerCharacter))
	//	{
	//		// Follows this character position with specified offset.
	//		FVector WidgetWorldLocation = OwnerCharacter->GetFloatingBarPosition() + LocOffsetFromOwner;

	//		FVector2D ProjectedLocation;
	//		if (PC->ProjectWorldLocationToScreen(WidgetWorldLocation, ProjectedLocation))
	//		{
	//			ProjectedLocation += FinalScreenOffset * UWidgetLayoutLibrary::GetViewportScale(this);
	//			SetPositionInViewport(ProjectedLocation);
	//			SelfVisibility = ESlateVisibility::HitTestInvisible;
	//		}
	//	}

	//	// ForceHidden 牢 版快 蜡瘤. 咯扁辑 SetVisibility 甫 流立 荤侩窍绰 措脚 阿 widget 喊肺 visibility 官牢爹阑 窍绰 霸 亮阑 巴档 鞍促. 
	//	const bool bHideFloating = IsForceHidden() || ShouldBeHiddenAsPooledObject() || OwnerCharacter->bHidden;
	//	SetVisibility(bHideFloating ? ESlateVisibility::Hidden : SelfVisibility);
	//}
}

//void UB2FloatingWidget::DestroySelf()
//{
//	Super::DestroySelf();
//	OwnerCharacter = NULL;
//}

void UB2FloatingWidget::SetCurrentlyRent(bool bInRent, UObject* InRenter)
{
	Super::SetCurrentlyRent(bInRent, InRenter);

	if (IsPooledObject())
	{
		ABladeIICharacter* RenterChar = Cast<ABladeIICharacter>(InRenter);
		check(!IsCurrentlyRent() || RenterChar); // Renter 瘤沥篮 BladeIICharacter 肺 秦具 窃.
		SetOwnerCharacter(IsCurrentlyRent() ? RenterChar : nullptr);
	}
}