// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotHB.h"

void UB2UIEventSlotHB::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, HorizontalBox);
}

void UB2UIEventSlotHB::AddChildWidget(UWidget* Content)
{
	if (HorizontalBox.IsValid())
		HorizontalBox->AddChild(Content);
}
