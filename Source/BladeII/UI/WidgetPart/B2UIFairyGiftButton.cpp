// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFairyGiftButton.h"

void UB2UIFairyGiftButton::Init()
{
	Super::Init();

	IsSelected = false;

	UpdateFairyImage();
}

void UB2UIFairyGiftButton::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyGiftButton::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_On);
	GET_SLOT(UImage, IMG_Off);

	GET_SLOT(UB2Button, BTN_Fairy);
	GET_SLOT(UWidgetSwitcher, WS_Image);
}

void UB2UIFairyGiftButton::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Fairy, &UB2UIFairyGiftButton::OnClickBTN_Fairy);
}

void UB2UIFairyGiftButton::SetFairyGiftBtnHandler(const FFairyGiftButtonOnClick& InBtnHandler)
{
	OnClickBtnHandler = InBtnHandler;
}

void UB2UIFairyGiftButton::SetSelectState(bool bSelected)
{
	IsSelected = bSelected;

	WS_Image->SetActiveWidgetIndex(IsSelected ? 1 : 0);
}

void UB2UIFairyGiftButton::UpdateFairyImage()
{
	UMaterialInterface* Mat = FairyImage.LoadSynchronous();
	if (Mat)
	{
		if (IMG_On.IsValid())
			IMG_On->SetBrushFromMaterial(Mat);
		if (IMG_Off.IsValid())
			IMG_Off->SetBrushFromMaterial(Mat);
	}
}

void UB2UIFairyGiftButton::OnClickBTN_Fairy()
{
	OnClickBtnHandler.ExecuteIfBound(FairyType);
}