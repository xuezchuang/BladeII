// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2FloatingWidget.h"
#include "B2FloatingGoldAcq.generated.h"

/**
 * Player's gold acquisition effect display widget
 */
UCLASS()
class BLADEII_API UB2FloatingGoldAcq : public UB2FloatingWidget
{
	GENERATED_BODY()
	
protected:

	/** Visible duration from the moment of gold acquisition. It is also the lifetime of an instance of this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float VisibleDuration;

	/** Visibility flag based on gold acquisition timing. */
	bool bIsVisible;

	FTimerHandle VisibilityTimerHandle;

	/** Based on this distance, X and Y of LocOffsetFromOwner will be randomly calculated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float RandomPlaceDistanceFromOwner;

public:

	UB2FloatingGoldAcq(const FObjectInitializer& ObjectInitializer);
	
	/** Called at the moment of gold acquisition, to do some native handling such as timer stuff */
	void NotifyPlayerSuckedGold(int32 AcquiredAmount);

	/** Gold acquisition event for blueprint. */
	UFUNCTION(BlueprintImplementableEvent)
	void ReceivePlayerSuckedGold(int32 AcquiredAmount);

	void SuckedGoldTimerCB();
	
	FORCEINLINE float GetVisibleDuration() { return VisibleDuration; }

	/** Should not use GetVisibility which is declared by UWidget */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	ESlateVisibility GetGoldAcqVisibility() const;
};
