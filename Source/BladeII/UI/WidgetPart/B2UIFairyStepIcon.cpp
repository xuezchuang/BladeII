// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFairyStepIcon.h"

void UB2UIFairyStepIcon::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Switch)
	GET_SLOT(UImage, FX_On)
}

void UB2UIFairyStepIcon::SetStatus(EFairyStepIconStatus::Type Status)
{
	int32 Index = Status == EFairyStepIconStatus::Type::OFF ? 0 : 1;

	if (WS_Switch.IsValid())
	{
		WS_Switch->SetActiveWidgetIndex(Index);
	}

	if (FX_On.IsValid())
	{
		FX_On->SetColorAndOpacity(Status == EFairyStepIconStatus::LEVEL_MAX ? FLinearColor::White : FLinearColor::Transparent);
	}
}