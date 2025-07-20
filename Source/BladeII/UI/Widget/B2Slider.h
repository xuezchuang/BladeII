// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "B2Slider.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2Slider : public USlider
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	bool BlockClickEventInCaptured;
	
protected:
	bool OneTimeBlockClickEvent;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void PreHandleOnMouseCaptureBegin();
	void PreHandleOnMouseCaptureEnd();

public:
	virtual void BeginDestroy() override;
	
};
