// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "UObject/Object.h"
#include "UMG.h"
#include "Styling/SlateBrush.h"
#include "B2InGameHUDInfo.h"
#include "B2InGameHUDInfo_Player.generated.h"

/**
 * To allow using a single In-game HUD class for all PCs, define some of widget styles which will be different for each classes here.
 * Each PC class will has its own HUDInfo asset.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2InGameHUDInfo_Player : public UB2InGameHUDInfo
{
	GENERATED_BODY()
	
public:
		
protected:

	// Player portrait is separated. Each of them is a part of different atlas texture.
	// Better not mix their use for saving memory.

	/** Player portrait material for any UI used for in-game battle. It is a part of in game HUD atlas texture. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftObjectPtr<UMaterialInterface> PlayerPortrait_InGameBattle;

	/** Player portrait material for any UI used for lobby. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftObjectPtr<UMaterialInterface> PlayerPortrait_Lobby;

	/** Player portrait material for any UI used for lobby. */
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftObjectPtr<UMaterialInterface> PlayerDeathPortrait_InGameBattle;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSoftObjectPtr<UTexture2D> PlayerLargePortrait;

public:
	// WorldContextObject is needed to check if it is currently lobby or in-game battle.
	UMaterialInterface* GetPlayerPortrait(UObject* WorldContextObject);
	//UObject* WorldContextObject는 없어도 되지만 추후를 위해 남겨 놓음
	UMaterialInterface* GetPlayerDeathPortrait(UObject* WorldContextObject);
	UTexture2D* GetPlayerLargePortrait();

	UPROPERTY(EditAnywhere, Category = "BladeII")
	uint32 bOverrideAttackButtonStyle : 1;

	/** Need at least image resource assigned for actual overriding. */
	UPROPERTY(EditAnywhere, Category = "BladeII", meta = (EditCondition = "bOverrideAttackButtonStyle"))
	FButtonStyle AttackButtonStyle;
	
protected:
	// The default attack button style, if not specially specified for some game mode.
	UPROPERTY(EditAnywhere, Category = "BladeII", meta = (EditCondition = "bOverrideAttackButtonStyle"))
	FMargin AttackButtonStyle_Margin;

	// Different margin setting are specially provided for some game mode..

	UPROPERTY(EditAnywhere, Category = "BladeII", meta = (EditCondition = "bOverrideAttackButtonStyle"))
	FMargin AttackButtonStyle_Margin_PVP;

	UPROPERTY(EditAnywhere, Category = "BladeII", meta = (EditCondition = "bOverrideAttackButtonStyle"))
	FMargin AttackButtonStyle_Margin_Raid;

	UPROPERTY(EditAnywhere, Category = "BladeII", meta = (EditCondition = "bOverrideAttackButtonStyle"))
	FMargin AttackButtonStyle_Margin_Counter;
public:
	void ApplyAttackBtnMargin(FButtonStyle& InOutButtonStyleToModify, UObject* WorldContextObject);

public:
	
	UB2InGameHUDInfo_Player(const FObjectInitializer& ObjectInitializer);
};
