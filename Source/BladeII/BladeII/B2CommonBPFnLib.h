// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "B2StageInfo.h"
#include "B2CommonBPFnLib.generated.h"

/**
 * Common static BlueprintCallable function library for BladeIIGame, except AnimBP functions.
 */
UCLASS()
class BLADEII_API UB2CommonBPFnLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	
	/** Blueprint BladeIIGetLOCText, for use of LocalizedTextTable. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "BladeIIGetLOCText")
	static FText B2GM_BladeIIGetLOCText(EB2LOC_CAT CategoryEnum, const FString& TextKey);

	UFUNCTION(BlueprintPure, Category = "BladeIIGame", DisplayName = "BladeIIGetLOCText_Pure")
	static FText B2GM_BladeIIGetLOCText_Pure(EB2LOC_CAT CategoryEnum, const FString& TextKey);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "CreateMovieUI")
	static void CreateMovieUI(class UMediaSource* InMediaSource, const TArray<struct FMovieSubtitle>& InSubtitleArray);
};
