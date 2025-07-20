// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UICostumeItemOption.h"

#include "B2DynText_ItemDetailOption.h"
#include "BladeIIUtil.h"

void UB2UICostumeItemOption::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_Title);
	GET_SLOT(UVerticalBox, VB_OptionList);
}

void UB2UICostumeItemOption::UpdateStaticText()
{
	if (RTB_Title.IsValid())
	{
		RTB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyItemDetail_IntrinsicOptionPanelTitle")));
	}
}

void UB2UICostumeItemOption::InitCostumeItemOption(const EPCClass& ItemAllowedPCClass, const TArray<FItemOption>& ArrayItemOption,
	const TArray<FItemOption>* ArrayItemOptionAtSamePlace)
{
	this->Init();

	if (VB_OptionList.IsValid())
	{
		const int32 iArrayOptionMax = ArrayItemOption.Num();
		for (int32 idx = 0; idx < iArrayOptionMax; ++idx)
		{
			FItemOption SameOptionOfEquipped;
			bool bFoundSameOptionOfEquipped = false;
			if (ArrayItemOptionAtSamePlace)
			{
				bFoundSameOptionOfEquipped = IsSameItemOptionOfType(*ArrayItemOptionAtSamePlace, SameOptionOfEquipped, ArrayItemOption[idx].OptionType);
			}

			UB2DynText_ItemDetailOption* pItemDetailOption = Cast<UB2DynText_ItemDetailOption>(DynCreateInVerticalBox(UIItemDetailOptionClass, this, VB_OptionList.Get(), FMargin()));
			if (pItemDetailOption)
			{
				pItemDetailOption->UpdateOptionTextSet(false, idx, ItemAllowedPCClass, &ArrayItemOption[idx], bFoundSameOptionOfEquipped ? &SameOptionOfEquipped : NULL, ItemStatIncColor, ItemStatSameColor, ItemStatDecColor);
			}
		}
	}
}

bool UB2UICostumeItemOption::IsSameItemOptionOfType(const TArray<FItemOption>& ArrayItemOptionAtSamePlace, FItemOption& OutFoundOption, EItemOption InSearchOption)
{
	bool IsSame = false;

	const int32 iArrayMax = ArrayItemOptionAtSamePlace.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayItemOptionAtSamePlace.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayItemOptionAtSamePlace[i].OptionType == InSearchOption)
		{
			OutFoundOption = ArrayItemOptionAtSamePlace[i];
			IsSame = true;
			break;
		}
	}

	return IsSame;
}