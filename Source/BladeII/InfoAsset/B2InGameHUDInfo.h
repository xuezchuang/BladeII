// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "B2InGameHUDInfo.generated.h"

/**
 * This is just to provide a common interface of setting HUD Info for both player character and boss mob classes.
 * Not much thing is really expected here. (probably none)
 */
UCLASS(BlueprintType)
class BLADEII_API UB2InGameHUDInfo : public UObject
{
	GENERATED_BODY()
	
	
public:

	UB2InGameHUDInfo(const FObjectInitializer& ObjectInitializer);
	
};
