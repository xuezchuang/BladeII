// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DJLegacy/BladeIIUserWidget.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "UB2DynItemIcon_Hammer.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_Hammer : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
	
public:

	UB2DynItemIcon_Hammer(const FObjectInitializer& ObjectInitializer);

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
