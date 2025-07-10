// Fill out your copyright notice in the Description page of Project Settings.


#include "B2TotemData.h"
//#include "BladeIIGameImpl.h"
//#include "B2CombatStatEvaluator.h"

FB2TotemRefineState::FB2TotemRefineState()
{
	Index = 0;
	RefineState = ETotemRefineState::NONE;
}

FB2TotemRefineState::FB2TotemRefineState(int32 InIndex, ETotemRefineState InRefineState)
{
	Index = InIndex;
	RefineState = InRefineState;
}

FB2TotemSubOptionDetail::FB2TotemSubOptionDetail()
{
	RefineCount = 0;
	RefineStates.Empty();
}

FB2TotemSubOptionDetail::FB2TotemSubOptionDetail(const b2network::B2TotemSubOptionDetailPtr ptr)
{
	SubOption.OptionType = SvrToCliOptionType(ptr->option_id);
	SubOption.RawOptionAmount = ptr->option_value;
	RefineCount = ptr->refine_count;

	RefineStates.Empty();
	for (int8 i = 0; i < ptr->refine_states.Num(); ++i)
	{
		RefineStates.Add(FB2TotemRefineState(ptr->refine_states[i]->index, static_cast<ETotemRefineState>(ptr->refine_states[i]->status)));
	}
}

FB2Totem::FB2Totem()
{
	ID = 0;
	RefID = 0;
	OwnerPCClass = EPCClass::EPC_End;
	EquipSlot = ETotemEquipSlot::TOTEM_NONE;
	Grade = ETotemGrade::TG_MAX;
	SubOptionDetails.Empty();
	RefineLevel = 0;
	RefineTryCount = 0;
	GoldSuccessRate = 0;
	PieceSuccessRate = 0;
	Power = 0;
	IsLock = false;
	IsNew = false;
}

FB2Totem::FB2Totem(const b2network::B2TotemServerInfoPtr ptr)
{
	ID = ptr->id;
	RefID = ptr->template_id;
	OwnerPCClass = SvrToCliPCClassType(ptr->character_type);
	EquipSlot = static_cast<ETotemEquipSlot>(ptr->equip_slot);
	Grade = SvrToCliTotemGrade(ptr->grade);
	MainOptionDetail.OptionType = SvrToCliOptionType(ptr->main_option_id);
	MainOptionDetail.RawOptionAmount = ptr->main_option_value;

	SubOptionDetails.Empty();
	for (int8 i = 0; i < ptr->sub_option_details.Num(); ++i)
	{
		SubOptionDetails.Add(FB2TotemSubOptionDetail(ptr->sub_option_details[i]));
	}

	// 토템의 랜덤 옵션 중 3번째 항목은 무조건 감소 옵션으로 적용된다.
	if (SubOptionDetails.IsValidIndex(TOTEM_DECREASE_VARIATION_INDEX))
	{
		SubOptionDetails[TOTEM_DECREASE_VARIATION_INDEX].SubOption.OptionVariationType = EOptionVariation::EOV_Decrease;
	}

	RefineLevel = ptr->refine_level;
	RefineTryCount = ptr->refine_try_count;
	GoldSuccessRate = ptr->gold_success_rate;
	PieceSuccessRate = ptr->piece_success_rate;
	Power = ptr->power;
	IsLock = ptr->locked;
	IsNew = ptr->is_new;
}

float FB2Totem::GetCombat()
{
	// 옵션 가지고 전투력 구해서 반환
	return 0.0f;
}

bool FB2Totem::IsEquipped() const
{
	return (EquipSlot != ETotemEquipSlot::TOTEM_NONE);
}

bool FB2Totem::IsSmeltingComplete() const
{
	int32 MaxSmeltingCount = 0;

	for (int8 OptionIndex = 0; OptionIndex < SubOptionDetails.Num(); ++OptionIndex)
	{
		FB2TotemInfo TotemInfo;
		//MaxSmeltingCount += (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(RefID, TotemInfo) ? TotemInfo.RefineLimitCounts[OptionIndex] : 0);
	}

	return (MaxSmeltingCount == RefineTryCount);
}

bool FB2Totem::IsSmelting() const
{
	return (RefineTryCount > 0);
}

int32 FB2Totem::GetMaxSmeltingCount() const
{
	int32 MaxSmeltingCount = 0;

	for (int8 OptionIndex = 0; OptionIndex < SubOptionDetails.Num(); ++OptionIndex)
	{
		FB2TotemInfo TotemInfo;
		//MaxSmeltingCount += (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(RefID, TotemInfo) ? TotemInfo.RefineLimitCounts[OptionIndex] : 0);
	}

	return MaxSmeltingCount;
}

int32 FB2Totem::GetSubOptionTryCount(int32 OptionIndex) const
{
	int32 OptionRefineTryCount = 0;

	if (SubOptionDetails.IsValidIndex(OptionIndex))
	{
		for (int8 RefineIndex = 0; RefineIndex < SubOptionDetails[OptionIndex].RefineStates.Num(); ++RefineIndex)
		{
			if (SubOptionDetails[OptionIndex].RefineStates.IsValidIndex(RefineIndex) == false)
			{
				continue;
			}

			if (SubOptionDetails[OptionIndex].RefineStates[RefineIndex].RefineState != ETotemRefineState::NONE)
			{
				++OptionRefineTryCount;
			}
		}
	}

	return OptionRefineTryCount;
}

void FB2Totem::GetItemFormat(FB2Item& OutItem)
{
	OutItem.InstanceUID = ID;
	OutItem.ItemRefID = RefID;

	OutItem.StarGrade = TotemGradeToItemStarGrade(Grade);
	OutItem.AllowedPCClass = PCClassToInt(OwnerPCClass);
	OutItem.EnhanceLevel = RefineLevel;

	OutItem.bIsLocked = IsLock;
	OutItem.Power = Power;
	OutItem.IsNew = IsNew;

	//OutItem.PrimaryPointType = CombatStatEval::ConvertItemOptionToItemPrimaryPointType(MainOptionDetail.OptionType);
	OutItem.SetPrimaryPoint(MainOptionDetail.RawOptionAmount);

	OutItem.RandomOptions.Empty();
	for (int8 i = 0; i < SubOptionDetails.Num(); ++i)
	{
		OutItem.RandomOptions.Add(SubOptionDetails[i].SubOption);
	}

	OutItem.bCurrentlyEquipped = (IsEquipped() ? 1 : 0);
}