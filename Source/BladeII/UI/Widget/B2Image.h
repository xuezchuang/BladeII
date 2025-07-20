// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Components/Image.h"
#include "B2Image.generated.h"

/**
 * For BladeII custom UI image widget features on top of original UImage.
 */
UCLASS()
class BLADEII_API UB2Image : public UImage
{
	GENERATED_BODY()

protected:

	/** When this is true, each assigned (by ColorToMIDParamMap_*) channel value of ColorAndOpacity is forced to 1.0,
	 * and animated ColorAndOpacity value is used for this image's assigned material parameter (if assigned) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	uint32 bMapColorToMIDParam : 1;

	/** For bMapColorToMIDParam, they designate which channel value will be mapped to which material parameter. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (EditCondition = "bMapColorToMIDParam"))
	FName ColorToMIDParamMap_R;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (EditCondition = "bMapColorToMIDParam"))
	FName ColorToMIDParamMap_G;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (EditCondition = "bMapColorToMIDParam"))
	FName ColorToMIDParamMap_B;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (EditCondition = "bMapColorToMIDParam"))
	FName ColorToMIDParamMap_A;

public:
	UB2Image(const FObjectInitializer& ObjectInitializer);

	/** Sets current ColorAndOpacity value to MID parameter mapped by ColorToMIDParamMap_*, then sets mapped ColorAndOpacity channel value to 1.0. */
	void TransferColorToMIDParam();

	FORCEINLINE bool IsUsingColorToMIDParamMapping() const { return bMapColorToMIDParam; }

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	
};
