// Fill out your copyright notice in the Description page of Project Settings.


#include "UB2UIRelicIngredientIcon.h"

void UUB2UIRelicIngredientIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UTextBlock, TB_Count);
}

void UUB2UIRelicIngredientIcon::SetIngredient(int32 type, int32 count)
{
	if (TB_Count.IsValid())
	{
		TB_Count->SetText(FText::AsNumber(count));
	}
	if (IMG_Icon.IsValid())
	{

	}
}

