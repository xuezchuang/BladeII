// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherOptionSlot.h"
#include "B2Inventory.h"
#include "BladeIIUtil.h"

void UB2UIEtherOptionSlot::SetInfo(EItemOption Option, float Value, EPCClass PCClass)
{
	if (X_RTB_OptionName.IsValid())
	{
		X_RTB_OptionName->SetText(GetLOCTextOfItemOption(Option, PCClass));
	}

	if (X_RTB_OptionValue.IsValid())
	{
		X_RTB_OptionValue->SetText(GetItemOptionValueDisplayText(Option, Value, true));
	}
}

void UB2UIEtherOptionSlot::SetDefault()
{
	if (X_RTB_OptionName.IsValid())
	{
		X_RTB_OptionName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_RandomOption")));
	}

	if (X_RTB_OptionValue.IsValid())
	{
		X_RTB_OptionValue->SetText(FText::FromString(TEXT("?")));
	}
}

void UB2UIEtherOptionSlot::CacheAssets()
{
	GET_SLOT(UTextBlock, X_RTB_OptionValue);
	GET_SLOT(UTextBlock, X_RTB_OptionName);
	GET_SLOT(UWidgetAnimation, ANIM_SpotLight);
}

void UB2UIEtherOptionSlot::PlaySpotLightAnimation()
{
	if(ANIM_SpotLight.IsValid())
		PlayAnimation(ANIM_SpotLight.Get());
}