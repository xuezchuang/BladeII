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
		// Set the timer to turn the visibility off. -> 이건 Manager 쪽에서 같은 시간으로 제거 타이머를 설치하니 없앤다. 
		//OwningPC->GetWorldTimerManager().SetTimer(VisibilityTimerHandle, this, &UB2FloatingGoldAcq::SuckedGoldTimerCB, VisibleDuration, false);

		// 매번 약간씩 다른 위치에 뿌리기 위해 LocOffsetFromOwner 를 좀 조정.

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
