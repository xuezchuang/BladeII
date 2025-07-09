// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2DynTextBase.generated.h"

/**
 * Base class for any kind of dynamically placed text block.. It has some common functionalities.
 */
UCLASS()
class BLADEII_API UB2DynTextBase : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
protected:
		
	TWeakObjectPtr<UImage> IMG_BG;

	/** To help differentiate adjacent rows, give some color to modulate for odd/even index row. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FLinearColor ModColor_OddRow;
	/** To help differentiate adjacent rows, give some color to modulate for odd/even index row. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FLinearColor ModColor_EvenRow;
	

public:
	UB2DynTextBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CacheAssets() override;

	/** Applies ModColor_OddRow or ModColor_EvenRow according to the RowIndex, according to whether it is odd or even number.
	 * It can help easily separate adjacent rows. */
	void ModulateBGImageToRowIndex(int32 RowIndex);
};
