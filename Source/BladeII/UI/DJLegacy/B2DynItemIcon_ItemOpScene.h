// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_TotemInventorySlot.h"
#include "B2DynItemIcon_ItemOpScene.generated.h"

/**
 * Extended primarily for enhancement progress scene icon, to have some effect on the icon
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_ItemOpScene : public UB2DynItemIcon_LobbyInven
{
	GENERATED_BODY()

protected:

	/** Major part showing the basic item information. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UB2DynItemIcon_LobbyInven* ItemIconCommonPart_NRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UB2LobbyUI_TotemInventorySlot* TotemIconCommonPart_NRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* IMG_TargetItemBlink_NRef;
	
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* IMG_TargetItemFinalBlow_NRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* IMG_IngredItemFinalBlow_NRef;
	
	/** Material that blinks and slow settling down for each ingredient sucking. */
	UPROPERTY(Transient, BlueprintReadWrite)
	class UMaterialInstanceDynamic* EnhanceTargetBlinkingMID;

	/** How fast the EnhanceTargetBlinking material parameter will be varying for fade out? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	float EnhanceTargetBlinkParamVaryingRate;

	float EnhanceTargetBlinkMIDParam;

	int32 TotalEnhanceIngredNum; // Just the initial total number, not be changed.

	/** Base starting value of progressively increasing target MID parameter. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	float EnhanceTargetProgressiveBaseValue;

	/** It will start from EnhanceTargetProgressiveBaseValue then be increased along the sucking process. */
	float EnhanceTargetProgressiveParam;
	/** Increase rate per-ingredient sucking. */
	float EnhanceTargetProgressiveParamRate;

public:

	UB2DynItemIcon_ItemOpScene(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	void UpdateEnhanceTargetBlinkingMIDParam(float InDeltaTime);
	/** To highlight it then let it start fade out again */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void ResetEnhanceTargetBlinkingMIDParam();

public:
	virtual void UpdateItemData(const FB2Item& InItem) override;
	void UpdateTotemData(const FB2Totem& InTotem);

	/** Better set before call UpdateItemData */
	virtual void SetIconUsageType(ELobbyInvenItemIconUsage InUsageType) override;
	
public:
	/** As one of ingredient item, at the moment of start getting sucked to the target.. */
	void OnIngredItemBeginSucking(int32 IngredIndex);
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnIngredItemBeginSucking_BP(int32 IngredIndex);

public:
	/** As a target item, at the moment of one of ingredient item start getting sucked to me.. */
	void OnTargetItemBeginSucking(int32 IngredIndex);
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnTargetItemBeginSucking_BP(int32 IngredIndex);

public:
	/** As a target item, at the moment of one of ingredient item just arrived to me to get sucked. */
	void OnTargetItemSuckedIngred(int32 IngredIndex);
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnTargetItemSuckedIngred_BP(int32 IngredIndex);

public:
	/** As a target item, at the moment of last ingredient item just arrived to me to get sucked. */
	void OnTargetItemLastSucking();
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnTargetItemLastSucking_BP();

public:

	/** True sets background and icon image invisible. It is meant to be used when the icon body is displayed using 3D mesh. */
	void SetShowOverlayInfoOnly(bool IsVisibleBGImg = false, bool IsVisibleGrade = false);

	FORCEINLINE void SetTotalEnhanceIngredNum(int32 InNum) { TotalEnhanceIngredNum = InNum; }

	virtual class UMaterialInstanceConstant* GetBGPanelMIC() override;
	virtual class UMaterialInstanceConstant* GetMainIconMIC() override;

	class UMaterialInstanceConstant* GetBGPanelMIC_Totem();
	class UMaterialInstanceConstant* GetMainIconMIC_Totem();
};
