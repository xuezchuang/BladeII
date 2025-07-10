// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UI_TotemSmeltingBonus.h"

#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"
#include "B2DynText_Multiple.h"

void UB2UI_TotemSmeltingBonus::DestroySelf(class UB2UIManager* InUIManager)
{
	for(int i = 0 ; i < ArraySmeltingBonusText.Num() ; ++i)
	{
		if (ArraySmeltingBonusText.IsValidIndex(i))
		{
			ArraySmeltingBonusText[i]->DestroySelf();
		}
	}

	ArraySmeltingBonusText.Empty();
	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemSmeltingBonus::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_SmeltingBonusTitle);

	ArraySmeltingBonusText.AddDefaulted(MAX_TOTEM_BONUS_OPTION);
	GET_SLOT_BYNAME(UB2DynText_Multiple, ArraySmeltingBonusText[0], UIP_ItemSetOption0);
	GET_SLOT_BYNAME(UB2DynText_Multiple, ArraySmeltingBonusText[1], UIP_ItemSetOption1);
	GET_SLOT_BYNAME(UB2DynText_Multiple, ArraySmeltingBonusText[2], UIP_ItemSetOption2);
}

void UB2UI_TotemSmeltingBonus::UpdateStaticText()
{
	if (RTB_SmeltingBonusTitle.IsValid())
	{
		RTB_SmeltingBonusTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_Bonus")));
	}
}

void UB2UI_TotemSmeltingBonus::InitTotemSmeltingBonus(const FB2Totem& InTotem)
{
	this->Init();

	FB2TotemInfo TotemInfo;
	if (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(InTotem.RefID, TotemInfo))
	{
		FB2TotemBonusOptionInfo TotemBonusInfo;
		if (BladeIIGameImpl::GetTotemDataStore().GetTotemBonusOptionInfo(TotemInfo.BonusID, TotemBonusInfo))
		{
			UpdateSmeltingBonusEffectText(InTotem.RefineLevel, TotemBonusInfo);
		}
	}
}

void UB2UI_TotemSmeltingBonus::UpdateSmeltingBonusEffectText(const int32& SmeltingLevel, struct FB2TotemBonusOptionInfo& BonusOptionInfo)
{
	int32 BonusConditionMax = -1;

	for(int32 i = 0; i < ArraySmeltingBonusText.Num() ; ++i)
	{
		if (ArraySmeltingBonusText[i].IsValid() == false)
		{
			continue;
		}

		FText BonusSuccessCountFormat = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_BonusCount"));
		FText BonusSuccessText = FText::Format(BonusSuccessCountFormat, FText::AsNumber(BonusOptionInfo.OptionDatas[i].Condition));

		FItemOption OptionData = FItemOption(SvrToCliOptionType(BonusOptionInfo.OptionDatas[i].BonusOptionID), BonusOptionInfo.OptionDatas[i].BonusValue);

		FText BonusOptionText = (IsItemOptionPerPCClass(OptionData.OptionType) ? 
			BladeIIGameImpl::GetTotemDataStore().GetLOCTextOfSkillOption(OptionData.OptionType) : 
			GetLOCTextOfItemOption(OptionData.OptionType, EPCClass::EPC_Fighter));

		FText OptionValueText = GetItemOptionValueDisplayText(OptionData.OptionType, OptionData.RawOptionAmount, true);
		
		FText BonusTextFormat = FText::FromString(TEXT("{0} {1}"));
		FText BonusText = FText::Format(BonusTextFormat, BonusOptionText, OptionValueText);
		
		ArraySmeltingBonusText[i]->SetDynText(0, BonusSuccessText, BonusText);

		if (SmeltingLevel >= BonusOptionInfo.OptionDatas[i].Condition)
		{
			ArraySmeltingBonusText[i]->SetDynTextColor(0, ApplySetTextColor);
			ArraySmeltingBonusText[i]->SetDynTextColor(1, ApplySetTextColor);
		}
	}
}