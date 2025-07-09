// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_PropOptionIcon.generated.h"

// Some widgets will be invisible depend on the define by this type
enum class EWingPropOptionUIPUsage : uint8
{
	WPOUU_WingPageMain,
	WPOUU_WingEvolveResult,
	WPOUU_PropOptionEnhance,
	
	WPOUU_CollectBookOnlyIcon,
	WPOUU_CollectBookPreview,

};

/**
 * A simple icon displaying a Wing's prop option info
 */
UCLASS()
class BLADEII_API UB2WingUIP_PropOptionIcon : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
protected:

	EPCClass WingOwnerClass;
	int32 WingOptionPropIndex;

	EWingPropOptionUIPUsage MyUsageType;

	FWingPropOption CachedOptionData; // The main information displayed by this icon.

	TWeakObjectPtr<UB2Button> BTN_Area;
	// Either one of IMG_Icon or IMG_Locked is visible at one time.
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UImage> IMG_Icon_Locked;
	TWeakObjectPtr<UImage> IMG_Locked;
	TWeakObjectPtr<UTextBlock> TB_PropOptionLevel;

public:

	UB2WingUIP_PropOptionIcon(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;
	
	void SetUsageType(EWingPropOptionUIPUsage InUsageType);
	void SetMyOptionInfo(const FWingPropOption& InOptionData);
	void SetWingPropExtraInfo(EPCClass OwnerClass, int32 PropIndex);

protected:

	virtual void RegisterUIMarkForRedDot() final;

	void UpdateWidgets(); // Update widgets based on the data set here.

	UFUNCTION()
	void OnClickBtnArea();

	UFUNCTION()
		bool RedDotCondition_Upgrade();
};

