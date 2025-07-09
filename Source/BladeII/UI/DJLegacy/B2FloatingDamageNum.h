// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2FloatingWidget.h"
#include "B2FloatingDamageNum.generated.h"

/**
 * Damages and even for healing, display for HP changes anyway.
 */
UCLASS()
class BLADEII_API UB2FloatingDamageNum : public UB2FloatingWidget
{
	GENERATED_BODY()
	
protected:

	/** Visible duration from damaged moment. It is also the lifetime of an instance of this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float VisibleDuration;

	/** Its LocOffsetFromOwner will be automatically set based on owner character's height, and set this if you want give additional z offset.. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float AdditionalZOffset;

	/** Based on this distance, X of LocOffsetFromOwner will be randomly calculated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float RandomPlaceXDistanceFromOwner;

	/** Based on this distance, Y of LocOffsetFromOwner will be randomly calculated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float RandomPlaceYDistanceFromOwner;
	
	/**
	 * Modulated font color for normal damaged circumstance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FColor BaseModulateColor_Damage;

	/**
	 * Modulated font color for healing circumstance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FColor BaseModulateColor_Healing;
	
	/**
	 * Modulated font color for special damaged circumstance, such as critical hit or finish action.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FColor BaseModulateColor_Critical;

	////////////////////////////////////////////////////////////////////////////////
	// For new way having image widget for each digit.

	TWeakObjectPtr<UWrapBox> WB_AllDisplaySet; // Gotta be a wrap-box to utilize draw batching optimization of 4.13.

	// For manual center placement of WrapBox.. We could not find proper alignment setting in overlay panel, so just putting it in canvas panel.
	UPROPERTY(EditAnywhere, Category = "BladeII New Digit Display")
	float SingleDigitWidth;

	/** Set it to match actually placed image widget number. */
	UPROPERTY(EditAnywhere, Category = "BladeII New Digit Display")
	int32 MaxDigitNum;

	FORCEINLINE int32 GetMaxDisplayableNum() const { return FMath::RoundToInt(FMath::Pow(10.0f, (float)MaxDigitNum)) - 1; }

	/** To find digit images by naming */
	UPROPERTY(EditAnywhere, Category = "BladeII New Digit Display")
	FString DigitImageNamePrefix;

	/** Set exactly 10 members, having 0'th member set for 0, 1'st for 1, 2'nd for 2, and so forth. 
	 * We use PaperSprites, instead of Material to utilize image draw batching in Wrap-box, introduced with 4.13 */
	UPROPERTY(EditAnywhere, Category = "BladeII New Digit Display")
	TArray<class UPaperSprite*> DigitSprites;
	FORCEINLINE class UPaperSprite* GetSpriteForDigit(int32 InNumber) { return DigitSprites.IsValidIndex(InNumber) ? DigitSprites[InNumber] : NULL; }

	UPROPERTY()
	TArray<UImage*> AllDigits;

	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////
	// Critical damage display

	TWeakObjectPtr<UPanelWidget> PN_CriticalDamage;
	
	// It might be changed to sequential text..
	TWeakObjectPtr<UImage> IMG_CriticalDamageText;

	////////////////////////////////////////////////////////////////////////////////


	/** Visibility flag based on recent damaged state. */
	bool bIsVisible;

	FTimerHandle VisibilityTimerHandle;

	float FadingProgress; // It will start counting when lastly damaged until visibility timer expires. 0.0 ~ 1.0

	/** Cached value when bIsVisible is turned on most recently. */
	float RecentDamageNum;

	/** It is initially calculated, and now being the final static screen position while it exists, instead of following owner character. */
	FVector2D InitialProjectedLocation;

	FVector m_vSpawnLocation;

public:

	UB2FloatingDamageNum(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	
	virtual void SetOwnerCharacter(class ABladeIICharacter* InOwner) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
	virtual void UpdatePosition() override;

	/** Set bInCritical to true for critical hit or finish action, any special damage. */
	void NotifyTakeDamage(float Damage, bool bInCritical);

	// For new digit display..
	void SetupDigitsForNumber(int32 InNumberToDisplay, const FLinearColor& InBlendColor);

	void SetupForCriticalDamage(bool bIsCritical);

	/** For additional blueprint damage event handling */
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveTakeDamage(float Damage, bool bInCritical);

	void TakeDamageTimerCB();

	/** Should not use GetVisibility which is declared by UWidget */
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	ESlateVisibility GetDamageNumVisibility() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool IsOwnerPC();

	FORCEINLINE float GetVisibleDuration() { return VisibleDuration; }

private:
	/** Update LocOffsetFromOwner based on the character bound size and additional parameter. */
	void UpdateFloatingOffset();
};
