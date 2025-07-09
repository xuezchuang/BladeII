// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2DynItemIcon_EnhanceIngred.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_EnhanceIngred : public UBladeIIUserWidget
{
	GENERATED_BODY()
public:

	UB2DynItemIcon_EnhanceIngred(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	void UpdateItemData(const FB2Item& InItem);

	UFUNCTION()
	void OnClickedBTNArea();

	virtual void OnClickedSenderBTNArea() override;
protected:
	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> DynItemIcon;
	TWeakObjectPtr<class UB2Button> BTN_Area;

	FB2Item NativeItemData;
};
