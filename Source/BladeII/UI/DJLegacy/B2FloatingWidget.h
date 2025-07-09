// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2FloatingWidget.generated.h"

/**
 * Base for in-game widgets floating around screen, possibly around a specific character.
 * Basically assumed to provide some information, not like button.
 */
UCLASS()
class BLADEII_API UB2FloatingWidget : public UBladeIIUserWidget
{
	GENERATED_BODY()

protected:

	/** Owner character that this widget will floating around. */
	UPROPERTY(Transient, BlueprintReadOnly)
	class ABladeIICharacter* OwnerCharacter;
	
	/** Location offset from owner character in world space */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FVector LocOffsetFromOwner;

	/** Set this value if want to adjust based on on screen size, or adjust for widget origin. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FVector2D FinalScreenOffset;
		
public:

	UB2FloatingWidget(const FObjectInitializer& ObjectInitializer);
	
	/** Set the owner and do some initialization if necessary. */
	virtual void SetOwnerCharacter(class ABladeIICharacter* InOwner);

	/** Update its floating screen position to follow owner character. */
	virtual void UpdatePosition();

	void DestroySelf() override;

	virtual void SetCurrentlyRent(bool bInRent, UObject* InRenter) override;
};
