// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIValueButton.h"


void UB2UIValueButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Enhance);
	GET_SLOT(UTextBlock, STB_Enhancebutton);
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UTextBlock, TB_EnhnceValue);
}

void UB2UIValueButton::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Enhance, &UB2UIValueButton::OnClickedButton);
}

void UB2UIValueButton::Init()
{
	Super::Init();
}

void UB2UIValueButton::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIValueButton::SetButtonName(FText ButtonName)
{
	if (STB_Enhancebutton.IsValid())
	{
		STB_Enhancebutton->SetText(ButtonName);
	}
}

void UB2UIValueButton::SetAmount(FText ValueAmount, bool bIsRed)
{
	if (IMG_Icon.IsValid() && TB_EnhnceValue.IsValid())
	{
		TB_EnhnceValue->SetText(ValueAmount);
		
		if (ValueAmount.IsEmpty())
		{
			IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			IMG_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (bIsRed)
			TB_EnhnceValue->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
		else
			TB_EnhnceValue->SetColorAndOpacity(NormalColor);
	}
}

void UB2UIValueButton::SetButtonDelegate(FValueButtonDelegate Delegate)
{
	ButtonDelegate = Delegate;
}

void UB2UIValueButton::OnClickedButton()
{
	ButtonDelegate.ExecuteIfBound();
}

void UB2UIValueButton::OnClicked_ValueButton()
{
	ButtonDelegate.ExecuteIfBound();
}

void UB2UIValueButton::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	if (BTN_Enhance.IsValid())
		BTN_Enhance->SetIsEnabled(bInIsEnabled);
}

