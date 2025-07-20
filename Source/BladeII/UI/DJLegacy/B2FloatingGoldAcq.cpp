// Fill out your copyright notice in the Description page of Project Settings.
#include "B2FloatingGoldAcq.h"
//#include "BladeII.h"


UB2FloatingGoldAcq::UB2FloatingGoldAcq(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VisibleDuration = 1.0f;
	RandomPlaceDistanceFromOwner = 100.0f;
	bIsVisible = false;
}

void UB2FloatingGoldAcq::NotifyPlayerSuckedGold(int32 AcquiredAmount)
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		bIsVisible = true;
		
		if (VisibilityTimerHandle.IsValid())
		{
			OwningPC->GetWorldTimerManager().ClearTimer(VisibilityTimerHandle);
		}
		// Set the timer to turn the visibility off. -> 捞扒 Manager 率俊辑 鞍篮 矫埃栏肺 力芭 鸥捞赣甫 汲摹窍聪 绝矩促. 
		//OwningPC->GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &UB2FloatingGoldAcq::SuckedGoldTimerCB, VisibleDuration, false);

		// 概锅 距埃究 促弗 困摹俊 谎府扁 困秦 LocOffsetFromOwner 甫 粱 炼沥.

		float RandDirX = FMath::FRandRange(-1.0f, 1.0f);
		float RandDirY = FMath::FRandRange(-1.0f, 1.0f);
		FVector2D RandDir(RandDirX, RandDirY);
		RandDir.Normalize();

		LocOffsetFromOwner.X = RandomPlaceDistanceFromOwner * RandDir.X;
		LocOffsetFromOwner.Y = RandomPlaceDistanceFromOwner * RandDir.Y;
	}

	ReceivePlayerSuckedGold(AcquiredAmount); // Call the BP event.
}

void UB2FloatingGoldAcq::SuckedGoldTimerCB()
{
	bIsVisible = false;

	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		// Not sure if this is necessary.
		OwningPC->GetWorldTimerManager().ClearTimer(VisibilityTimerHandle);
	}
}

ESlateVisibility UB2FloatingGoldAcq::GetGoldAcqVisibility() const
{
	return bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}
