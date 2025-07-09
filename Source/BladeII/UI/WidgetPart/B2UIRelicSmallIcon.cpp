// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIRelicSmallIcon.h"

void UB2UIRelicSmallIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UB2Button, BTN_Icon);

}

void UB2UIRelicSmallIcon::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Icon, &UB2UIRelicSmallIcon::OnClickBtn);
}

void UB2UIRelicSmallIcon::SetIngredient(int32 RelicID)
{
	if (IMG_Icon.IsValid())
	{
		Index = RelicID;
	}
}

void UB2UIRelicSmallIcon::OnClickBtn()
{
	OnRelicIconClickedDelgate.ExecuteIfBound(Index);
}



