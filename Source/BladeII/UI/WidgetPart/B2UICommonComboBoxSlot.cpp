// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICommonComboBoxSlot.h"
#include "B2UIManager.h"



void UB2UICommonComboBoxSlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Image, IMG_BG);
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UTextBlock, TB_MainText);
}

void UB2UICommonComboBoxSlot::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UICommonComboBoxSlot::OnClickBTN_Select);
}

void UB2UICommonComboBoxSlot::Init()
{
	Super::Init();
}

void UB2UICommonComboBoxSlot::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}


void UB2UICommonComboBoxSlot::SetMainText(FText InText)
{
	if (TB_MainText.IsValid())
		TB_MainText->SetText(InText);
}

void UB2UICommonComboBoxSlot::OnClickBTN_Select()
{

}


