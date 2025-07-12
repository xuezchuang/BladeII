// Fill out your copyright notice in the Description page of Project Settings.


#include "B2TotemDataStore.h"
#include "B2TotemRefineInfo.h"
#include "B2CombatStatEvaluator.h"

#include "BladeIIUtil.h"

FTotemDataStore::FTotemDataStore()
{
}

FTotemDataStore::~FTotemDataStore()
{
}

void FTotemDataStore::SetMasterData(const FB2MasterDatas& InMasterDatas)
{
	FB2ExtendMessagePtr	ExtendMessage;
	/*ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	CachedMDTotemInfos.Empty();
	for (const b2network::B2mdTotemInfoPtr& ThisInfo : ExtendMessage->md_totem_infos)
	{
		CachedMDTotemInfos.Add(FB2TotemInfo(ThisInfo));
	}

	CachedMDSelectTotemOptions.Empty();
	for (const b2network::B2mdSelectTotemOptionPtr& ThisInfo : ExtendMessage->md_select_totem_options)
	{
		CachedMDSelectTotemOptions.Add(FB2SelectTotemOption(ThisInfo));
	}

	CachedMDTotemRefineInfos.Empty();
	for (const b2network::B2mdTotemRefineInfoPtr& ThisInfo : ExtendMessage->md_totem_refine_infos)
	{
		CachedMDTotemRefineInfos.Add(FB2TotemRefineInfo(ThisInfo));
	}

	CachedMDTotemRefineRateInfos.Empty();
	for (const b2network::B2mdTotemRefineRateInfoPtr& ThisInfo : ExtendMessage->md_totem_refine_rate_infos)
	{
		CachedMDTotemRefineRateInfos.Add(FB2TotemRefineRateInfo(ThisInfo));
	}

	CachedMDTotemBonusOptionInfoes.Empty();
	for (const b2network::B2mdTotemBonusOptionInfoPtr& ThisInfo : ExtendMessage->md_totem_bonus_option_infos)
	{
		CachedMDTotemBonusOptionInfoes.Add(FB2TotemBonusOptionInfo(ThisInfo));
	}

	CachedMDDimensionInfos.Empty();
	for (const b2network::B2mdDimensionInfoPtr& ThisInfo : ExtendMessage->md_dimension_infos)
	{
		CachedMDDimensionInfos.Add(ThisInfo->difficulty_level,FB2DimensionInfo(ThisInfo));
	}

	CachedMDDimensionWeekRewardInfoes.Empty();
	for (const b2network::B2mdDimensionWeekRewardPtr& ThisInfo : ExtendMessage->md_dimension_week_rewards)
	{
		CachedMDDimensionWeekRewardInfoes.Add(FB2DimensionWeekRewardInfo(ThisInfo));
	}

	CachedMDTotemDismantleInfos.Empty();
	for (const b2network::B2mdTotemDismantleInfoPtr& ThisInfo : ExtendMessage->md_totem_dismantle_infos)
	{
		CachedMDTotemDismantleInfos.Add(FB2TotemDismantleInfo(ThisInfo));
	}

	CachedMDSealCost.Empty();
	CachedMDSealCost = ExtendMessage->md_item_seal_cost;

	CachedMDItemSealOptions.Empty();
	for (b2network::B2mdSelectItemSealOptionPtr ThisInfo : ExtendMessage->md_select_item_seal_option)
	{
		
		if (!CachedMDItemSealOptions.Contains(ThisInfo->slot))
		{
			TArray<b2network::B2mdSelectItemSealOptionPtr> ArrayInfo;
			CachedMDItemSealOptions.Add(ThisInfo->slot, ArrayInfo);
		}
			

		CachedMDItemSealOptions[ThisInfo->slot].Add(ThisInfo);
		
	}*/
}

FText FTotemDataStore::GetTotemName(const FB2Totem& InTotem, bool IsMultiLine)
{
	ETotemBasePrefix TargetTotemBasePrefix = ETotemBasePrefix::TBP_MAX;

	for (FB2TotemInfo &TotemInfo : CachedMDTotemInfos)
	{
		if (TotemInfo.TemplateID == InTotem.RefID)
		{
			TargetTotemBasePrefix = SvrToCliTotemBasePrefix(TotemInfo.GroupID);
		}
	}

	if (InTotem.RefineLevel > 0)
	{
		FText ItemNameFormatWithPrefix = BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsMultiLine ? TEXT("Totem_Prefix_Complete") : TEXT("Totem_Prefix_Smelt"));
		return FText::Format(ItemNameFormatWithPrefix, GetTotemRefineText(InTotem.RefineLevel), GetTotemMainOptionText(TargetTotemBasePrefix));
	}
	else
	{
		FText ItemNameFormatWithPrefix = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Prefix_Smelting"));
		return FText::Format(ItemNameFormatWithPrefix, GetTotemMainOptionText(TargetTotemBasePrefix));
	}
}

bool FTotemDataStore::GetTotemInfo(const int32& ItemTemplateID, FB2TotemInfo& OutTotemInfo)
{
	for (FB2TotemInfo &TotemInfo : CachedMDTotemInfos)
	{
		if (TotemInfo.TemplateID == ItemTemplateID)
		{
			OutTotemInfo = TotemInfo;
			return true;
		}
	}	
	return false;
}

int32 FTotemDataStore::GetTotemSellPrice(const FB2Totem InTotem)
{
	for (FB2TotemInfo &TotemInfo : CachedMDTotemInfos)
	{
		if (TotemInfo.TemplateID == InTotem.RefID)
		{
			return TotemInfo.SellPrice;
		}
	}
	return 0;
}

int32 FTotemDataStore::GetTotemGroupID(const FB2Totem InTotem)
{
	for (FB2TotemInfo &TotemInfo : CachedMDTotemInfos)
	{
		if (TotemInfo.TemplateID == InTotem.RefID)
		{
			return TotemInfo.GroupID;
		}
	}
	return 0;
}

bool FTotemDataStore::GetTotemBonusOptionInfo(const int32& BonusOptionID, FB2TotemBonusOptionInfo& OutTotemInfo)
{
	for (FB2TotemBonusOptionInfo &BonusOptionInfo : CachedMDTotemBonusOptionInfoes)
	{
		if (BonusOptionInfo.BonusID == BonusOptionID)
		{
			OutTotemInfo = BonusOptionInfo;
			return true;
		}
	}
	return false;
}

bool FTotemDataStore::GetTotemBonusOption(const FB2Totem& InTotem, TArray<FItemOption>& OutItemOption)
{
	OutItemOption.Empty();

	FB2TotemInfo TotemInfo;
	if (GetTotemInfo(InTotem.RefID, TotemInfo))
	{
		FB2TotemBonusOptionInfo TotemBonusInfo;
		if (GetTotemBonusOptionInfo(TotemInfo.BonusID, TotemBonusInfo))
		{
			for (int i = 0; i < TotemBonusInfo.OptionDatas.Num(); ++i)
			{
				if (InTotem.RefineLevel >= TotemBonusInfo.OptionDatas[i].Condition)
				{
					FItemOption ItemOption = FItemOption(SvrToCliOptionType(TotemBonusInfo.OptionDatas[i].BonusOptionID), TotemBonusInfo.OptionDatas[i].BonusValue);
					OutItemOption.Add(ItemOption);
				}
			}

			if (OutItemOption.Num() >= 0)
			{
				return true;
			}
		}
	}

	return false;
}

FText FTotemDataStore::GetLOCTextOfSkillOption(const EItemOption& ItemOption)
{
	switch (ItemOption)
	{
	case EItemOption::EIO_Skill_IncSkill1Damage:
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totme_Character_SkillOption")), 1);
	}
	break;
	case EItemOption::EIO_Skill_IncSkill2Damage:
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totme_Character_SkillOption")), 2);
	}
	break;
	case EItemOption::EIO_Skill_IncSkill3Damage:
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totme_Character_SkillOption")), 3);
	}
	break;
	case EItemOption::EIO_Skill_IncSkill5Damage:
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totme_Character_SkillOption")), 5);
	}
	break;
	}
	return FText::FromString(TEXT("Not Skill Option"));
}

int32 FTotemDataStore::GetDismantlePieceCount(const FB2Totem& InTotem)
{
	for (int i = 0; i < CachedMDTotemDismantleInfos.Num(); ++i)
	{
		if (CachedMDTotemDismantleInfos[i].Grade == CliToSvrTotemGrade(InTotem.Grade) && CachedMDTotemDismantleInfos[i].RefineLevel == InTotem.RefineLevel)
		{
			return CachedMDTotemDismantleInfos[i].Amount;
		}
	}

	return 0;
}

bool FTotemDataStore::GetTotemSubOptionMinMax(const ETotemGrade& Grade, const EItemOption& ItemOption, float& OutMinValue, float& OutMaxValue)
{
	for (int i = 0; i < CachedMDSelectTotemOptions.Num(); ++i)
	{
		if (CachedMDSelectTotemOptions[i].Grade == CliToSvrTotemGrade(Grade) && SvrToCliOptionType(CachedMDSelectTotemOptions[i].OptionID) == ItemOption)
		{
			OutMinValue = CachedMDSelectTotemOptions[i].MinSubValue;
			OutMaxValue = CachedMDSelectTotemOptions[i].MaxSubValue;

			return true;
		}
	}
	return false;
}

ETotemEquipSlot FTotemDataStore::GetTotemEquipSlot(const int32& InRefID)
{
	for (FB2TotemInfo &TotemInfo : CachedMDTotemInfos)
	{
		if (TotemInfo.TemplateID == InRefID)
		{
			return static_cast<ETotemEquipSlot>(TotemInfo.EquipSlot);
		}
	}

	return ETotemEquipSlot::TOTEM_NONE;
}

FText FTotemDataStore::GetTotemRefineText(const int32& RefineLevel)
{
	//UB2TotemRefineInfo* TotemRefineInfo = StaticFindTotemRefineInfo();

	//if (RefineLevel >= TotemRefineInfo->GetTotemRefineValue(ETotemRefinePrefix::TRP_SACRED))
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Prefix_SmeltCount04"));
	//else if (RefineLevel >= TotemRefineInfo->GetTotemRefineValue(ETotemRefinePrefix::TRP_STRONG))
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Prefix_SmeltCount03"));
	//else if (RefineLevel >= TotemRefineInfo->GetTotemRefineValue(ETotemRefinePrefix::TRP_SHINING))
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Prefix_SmeltCount02"));
	//else if (RefineLevel >= TotemRefineInfo->GetTotemRefineValue(ETotemRefinePrefix::TRP_ECLIPSE))
	//	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Prefix_SmeltCount01"));
	//else
		return FText::FromString(TEXT(""));
}

FText FTotemDataStore::GetTotemMainOptionText(const ETotemBasePrefix& TotemBasePrefix)
{
	switch (TotemBasePrefix)
	{
	case ETotemBasePrefix::TBP_VALOROUS:
	{
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_prefix_valor"));
	}
	break;
	case ETotemBasePrefix::TBP_PATRON:
	{
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_prefix_protect"));
	}
	break;
	case ETotemBasePrefix::TBP_VIGOROUS:
	{
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_prefix_vitality"));
	}
	break;
	}
	return FText::FromString(TEXT(""));
}

FB2DimensionInfo* FTotemDataStore::GetDimensionInfo(int32 nDiff)
{
	return CachedMDDimensionInfos.Find(nDiff);
}

TArray<FB2DimensionWeekRewardInfo> FTotemDataStore::GetDimensionWeekRewardInfos()
{
	return CachedMDDimensionWeekRewardInfoes;
}

TArray<b2network::B2mdSelectItemSealOptionPtr> FTotemDataStore::GetSealOptionData(int32 InSlotNumber)
{

	if (CachedMDItemSealOptions.Contains(InSlotNumber))
	{
		return CachedMDItemSealOptions[InSlotNumber];
	}
	
	return TArray<b2network::B2mdSelectItemSealOptionPtr>();
}

b2network::B2mdItemSealCostPtr FTotemDataStore::GetSealCostData(int32 InSlotNumber)
{
	for (auto SealCostInfo : CachedMDSealCost)
	{
		if (SealCostInfo->slot == InSlotNumber)
			return SealCostInfo;
	}

	return nullptr;
}
