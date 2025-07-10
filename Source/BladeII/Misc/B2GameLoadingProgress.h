// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "CommonStruct.h"

/** Functionalities for collecting game loading progress after pure map resource data loading has been completed.
 * regarding BladeII game resources, other than level data itself. */
namespace B2GMLoadingProgCollector
{

	/** Pure map loading share (proportion) except game content specific resource (i.e InfoAsset) loading. */
	float GetPureMapLoadShareForGM(UObject* WorldContextObject, EB2GameMode InGameModeToLoad);

	/** The one need to be called before map loading begins. */
	void PrepareWithPureMapLoadShareForGM(UObject* WorldContextObject, EB2GameMode InGameModeToLoad);

	void PrepareWithPureMapLoadShareForPreRenderGM(UObject* WorldContextObject);

	/** You must set the proper value to be increased at each Step call. 
	 * It is relative proportion to remaining game resource progress share. */
	void SetStepRate(float InNewValue);

	/** The main method that being used at actual spots to increase loading progress.
	 * StepRate must be set before this call. */
	void Step();
	
	/** In most cases, you might want to use SetStepRate and Step in pair. 
	 * In addition, you may optionally specify the minimum progress (within in-game resource only share) so far. */
	void StepCustom(float InSpecifiedRate, float DesiredMinGameResourceLoadingProgress = -1.0f);

	/** Almost like it's done.. but it is meant to be used before the actual loading complete */
	void StepToMax();

	// Addition mode, instead of rate scaling.. InAmount is still proportional to total in-game resource progress share, but it can go over 1.0
	//void StepWithAmount(float InAmount);
}