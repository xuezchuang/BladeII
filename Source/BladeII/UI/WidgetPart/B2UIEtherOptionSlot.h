// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIEtherOptionSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherOptionSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetInfo(EItemOption Option, float Value, EPCClass PCClass);
	void SetDefault();
	void PlaySpotLightAnimation();

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UTextBlock>					X_RTB_OptionName;
	TWeakObjectPtr<UTextBlock>					X_RTB_OptionValue;
	TWeakObjectPtr<UWidgetAnimation>			ANIM_SpotLight;
	
};
