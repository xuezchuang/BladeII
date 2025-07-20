// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "UMG.h"
#include "Styling/SlateBrush.h"
#include "B2InGameHUDInfo.h"
#include "B2InGameHUDInfo_BossMob.generated.h"

////////////////////////////////////////////////////////////////////////////////
//
//
// This HUDInfo class for Boss mob is to be deprecated.
// As UI information of mobs is not that much, they are now directly defined in NPCClassInfo.
//
//
////////////////////////////////////////////////////////////////////////////////

/**
 * To allow using a single In-game HUD class for all Boss mobs, define some of widget styles which will be different for each classes here.
 * Each boss mob class will has its own HUDInfo asset.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2InGameHUDInfo_BossMob : public UB2InGameHUDInfo
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "BladeII")
	uint32 bOverridePortraitBrush : 1;

	/** Need at least image resource assigned for actual overriding. */
	UPROPERTY(EditAnywhere, Category = "BladeII", meta = (EditCondition = "bOverridePortraitBrush"))
	FSlateBrush PortraitBrush;
	
public:

	UB2InGameHUDInfo_BossMob(const FObjectInitializer& ObjectInitializer);
	
};
