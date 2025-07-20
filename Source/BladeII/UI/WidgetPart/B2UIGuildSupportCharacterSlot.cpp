// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGuildSupportCharacterSlot.h"

void UB2UIGuildSupportCharacterSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_CharNumber); 
	GET_SLOT(UB2Button, BTN_Area);
}

void UB2UIGuildSupportCharacterSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIGuildSupportCharacterSlot::OnClickBtnArea);
	
}

void UB2UIGuildSupportCharacterSlot::SetHeroImage(EPCClass CharType)
{
	CharacterClass = CharType;
	SetHeroImage_BP(CharType);
}

void UB2UIGuildSupportCharacterSlot::SetSwitchState(bool IsEnabled)
{
	SetSwitchState_BP(IsEnabled);
}

void UB2UIGuildSupportCharacterSlot::SetMainHero(bool IsMain)
{
	if (TB_CharNumber.IsValid())
	{
		if(TB_CharNumber->GetVisibility() == ESlateVisibility::Hidden)
			TB_CharNumber->SetVisibility(ESlateVisibility::HitTestInvisible);

		TB_CharNumber->SetText(FText::AsNumber(IsMain ? 1 : 2)); //皋牢捞搁 1锅 辑宏搁 2锅
	}
}

void UB2UIGuildSupportCharacterSlot::SetDefaultHero()
{
	if (TB_CharNumber.IsValid())
	{
		TB_CharNumber->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIGuildSupportCharacterSlot::OnClickBtnArea()
{
	if (CharacterClass == EPCClass::EPC_End)
		return;

	OnClickedDelgate.ExecuteIfBound(CharacterClass);
}
