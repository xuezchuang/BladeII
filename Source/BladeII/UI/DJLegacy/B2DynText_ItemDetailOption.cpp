// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynText_ItemDetailOption.h"
#include "B2Inventory.h"



UB2DynText_ItemDetailOption::UB2DynText_ItemDetailOption(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2DynText_ItemDetailOption::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_OptionName);
	GET_SLOT(UTextBlock, TB_OptionValueL);
	GET_SLOT(UTextBlock, TB_OptionValueR);
}

void UB2DynText_ItemDetailOption::UpdateOptionTextSet(bool bIsForEquippedItem, int32 RowIndex, EPCClass InPCClass, const struct FItemOption* InDisplayOption, struct FItemOption* InEquippedOption,
	const FSlateColor& StatIncColor, const FSlateColor& StatSameColor, const FSlateColor& StatDecColor, bool IsAfterEvent)
{
	ModulateBGImageToRowIndex(RowIndex);

	if (InDisplayOption)
	{
		//ECombatOptionApplyType OptionApplyType = GetCombatOptionApplyType(InDisplayOption->OptionType); 咯扁辑 结 冈阑霸 乐唱..
		if (TB_OptionName.IsValid())
		{
			TB_OptionName->SetText(GetLOCTextOfItemOption(InDisplayOption->OptionType, InPCClass));
		}
		if (TB_OptionValueL.IsValid())
		{
			if (bIsForEquippedItem)
			{
				TB_OptionValueL->SetText(FText::FromString(TEXT(""))); // 厘馒 厘厚牢 版快 谅螟芒俊绰 酒公巴档 钎矫 救窃.
			}
			else
			{				
				TB_OptionValueL->SetText(
					GetItemOptionValueDisplayText(InDisplayOption->OptionType, InDisplayOption->RawOptionAmount, true)
					); 
			}
		}
		if (TB_OptionValueR.IsValid())
		{
			if (bIsForEquippedItem)
			{ // 厘馒 厘厚牢 版快 快螟 芒俊 瓷仿摹 钎矫.
				TB_OptionValueR->SetText(
					GetItemOptionValueDisplayText(InDisplayOption->OptionType, InDisplayOption->RawOptionAmount, true)
					);

				UScaleBoxSlot* SBSlot = Cast<UScaleBoxSlot>(TB_OptionValueR->Slot);
				if (SBSlot) // 去磊 叠尔 乐栏骨肺 快螟 align
				{
					SBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
				}
			}
			else
			{
				if (InEquippedOption) // 悼老 困摹俊 厘馒茄 巴捞 鞍篮 可记捞 乐促搁 厚背窃.
				{
					const float OptionDeltaToEquipped = InDisplayOption->RawOptionAmount - InEquippedOption->RawOptionAmount;
					const bool bAreAlmostSame = (FMath::Abs(OptionDeltaToEquipped) < KINDA_SMALL_NUMBER);
					
					FString DeltaString;
					if (bAreAlmostSame)
					{
						DeltaString = TEXT("( - )");
					}
					else
					{
						if (GetItemOptionValueByPercent(InDisplayOption->OptionType))
						{
							DeltaString = FString::Printf(TEXT("( %s%d )"), (OptionDeltaToEquipped > 0.0f) ? TEXT("+") : TEXT("-"), FMath::TruncToInt(FMath::Abs(OptionDeltaToEquipped)));
						}
						else if (GetItemOptionValueBySecond(InDisplayOption->OptionType))
						{
							DeltaString = FString::Printf(TEXT("( %s%.2f )"), (OptionDeltaToEquipped > 0.0f) ? TEXT("+") : TEXT("-"), FMath::Abs(OptionDeltaToEquipped));
						}
						else
						{
							DeltaString = FString::Printf(TEXT("( %s%.2f %% )"), (OptionDeltaToEquipped > 0.0f) ? TEXT("+") : TEXT("-"), FMath::Abs(OptionDeltaToEquipped));
						}
					}

					TB_OptionValueR->SetText(FText::FromString(DeltaString));
					TB_OptionValueR->SetColorAndOpacity(bAreAlmostSame ? StatSameColor : (OptionDeltaToEquipped > 0.0f ? StatIncColor : StatDecColor));

					UScaleBoxSlot* SBSlot = Cast<UScaleBoxSlot>(TB_OptionValueR->Slot);
					if (SBSlot) // 谅螟 波尔 嘎措档废 谅螟 align.
					{
						SBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
					}
				}
				else
				{
					if (IsAfterEvent)
					{
						TB_OptionValueR->SetText(FText::FromString(TEXT("(New)")));
						TB_OptionValueR->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
					}
					else
					{
						TB_OptionValueR->SetText(FText::FromString(TEXT("")));
					}
				}
			}
		}
	}
	else
	{ // 趣矫扼档 绝促搁 厚况初绰促.
		if (TB_OptionName.IsValid())
		{
			TB_OptionName->SetText(FText::FromString(TEXT("")));
		}
		if (TB_OptionValueL.IsValid())
		{
			TB_OptionValueL->SetText(FText::FromString(TEXT("")));
		}
		if (TB_OptionValueR.IsValid())
		{
			TB_OptionValueR->SetText(FText::FromString(TEXT("")));
		}
	}
}

void UB2DynText_ItemDetailOption::UpdateOptionTextSet(FText InText)
{
	if (TB_OptionName.IsValid())
	{
		TB_OptionName->SetText(InText);
	}
	if (TB_OptionValueL.IsValid())
	{
		TB_OptionValueL->SetText(FText::FromString(TEXT("")));
	}
	if (TB_OptionValueR.IsValid())
	{
		TB_OptionValueR->SetText(FText::FromString(TEXT("")));
	}
}

void UB2DynText_ItemDetailOption::UpdatePrimaryRangeOption(int32 RowIndex, EItemPrimaryPointType PrimaryType, int32 LowValue, int32 HighValue)
{
	ModulateBGImageToRowIndex(RowIndex);

	if (TB_OptionName.IsValid())
		TB_OptionName->SetText(GetLOCTextOfPrimPointType(PrimaryType));

	if (TB_OptionValueL.IsValid())
		TB_OptionValueL->SetText(FText::AsNumber(LowValue));

	if (TB_OptionValueR.IsValid())
		TB_OptionValueR->SetText(FText::AsNumber(HighValue));
}

bool UB2DynText_ItemDetailOption::GetItemOptionValueBySecond(EItemOption ItemOption)
{
	return (
		ItemOption == EItemOption::EIO_Skill_DecSkillCooltime ||
		ItemOption == EItemOption::EIO_Misc_DecTagCooltime
		);
}

bool UB2DynText_ItemDetailOption::GetItemOptionValueByPercent(EItemOption ItemOption)
{
	return (
		ItemOption == EItemOption::EIO_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_Health_IncHealth ||
		ItemOption == EItemOption::EIO_Defense_IncDefense||

		ItemOption == EItemOption::EIO_Tag_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_Tag_Defense_IncDefense ||
		ItemOption == EItemOption::EIO_Tag_Health_IncHealth ||

		ItemOption == EItemOption::EIO_HeroTower_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_HeroTower_Defense_IncDefense ||
		ItemOption == EItemOption::EIO_HeroTower_Health_IncHealth ||

		ItemOption == EItemOption::EIO_Assault_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_Assault_Defense_IncDefense ||
		ItemOption == EItemOption::EIO_Assault_Health_IncHealth ||

		ItemOption == EItemOption::EIO_Raid_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_Raid_Defense_IncDefense ||
		ItemOption == EItemOption::EIO_Raid_Health_IncHealth ||

		ItemOption == EItemOption::EIO_TeamMatch_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_TeamMatch_Defense_IncDefense ||
		ItemOption == EItemOption::EIO_TeamMatch_Health_IncHealth ||

		ItemOption == EItemOption::EIO_Guild_Offense_IncAttack ||
		ItemOption == EItemOption::EIO_Guild_Defense_IncDefense ||
		ItemOption == EItemOption::EIO_Guild_Health_IncHealth ||

		ItemOption == EItemOption::EIO_Health_PerSecRecoverHP ||

		ItemOption == EItemOption::EIO_Offense_IncAdditionalDamage ||
		ItemOption == EItemOption::EIO_Defense_DecAdditionalDamage 
		);
}
