// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UI_TotemSmeltingStatus.h"

#include "B2UIManager.h"
#include "BladeIIGameImpl.h"
#include "B2UI_TotemSmeltingSlot.h"

void UB2UI_TotemSmeltingStatus::DestroySelf(class UB2UIManager* InUIManager)
{
	for (int i = 0; i < UIP_TotemSmeltingSlotBPs.Num(); ++i)
	{
		if (UIP_TotemSmeltingSlotBPs.IsValidIndex(i))
		{
			UIP_TotemSmeltingSlotBPs[i]->OnSlotSmeltingClickDelgate.Unbind();
			UIP_TotemSmeltingSlotBPs[i]->DestroySelf(InUIManager);
		}
	}

	UIP_TotemSmeltingSlotBPs.Empty();
	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemSmeltingStatus::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_IncreaseOption);
	GET_SLOT(UB2RichTextBlock, RTB_DecreaseOption);

	UIP_TotemSmeltingSlotBPs.Empty();
	UIP_TotemSmeltingSlotBPs.SetNum(MAX_TOTEM_SUB_OPTION);
	GET_SLOT_BYNAME(UB2UI_TotemSmeltingSlot, UIP_TotemSmeltingSlotBPs[0], UIP_TotemSmeltingSlotBP_0);
	GET_SLOT_BYNAME(UB2UI_TotemSmeltingSlot, UIP_TotemSmeltingSlotBPs[1], UIP_TotemSmeltingSlotBP_1);
	GET_SLOT_BYNAME(UB2UI_TotemSmeltingSlot, UIP_TotemSmeltingSlotBPs[2], UIP_TotemSmeltingSlotBP_2);
	for (int32 i = 0; i < UIP_TotemSmeltingSlotBPs.Num(); ++i)
	{
		if (UIP_TotemSmeltingSlotBPs[i].IsValid())
		{
			UIP_TotemSmeltingSlotBPs[i]->Init();
			UIP_TotemSmeltingSlotBPs[i]->SetParentTotemStatue(this);
		}
	}
}

void UB2UI_TotemSmeltingStatus::BindDelegates()
{
	Super::BindDelegates();
	for (int32 i = 0; i < UIP_TotemSmeltingSlotBPs.Num(); ++i)
	{
		if (UIP_TotemSmeltingSlotBPs[i].IsValid())
		{
			UIP_TotemSmeltingSlotBPs[i]->OnSlotSmeltingClickDelgate.BindDynamic(this, &UB2UI_TotemSmeltingStatus::SmeltingTotem);
		}
	}
}

void UB2UI_TotemSmeltingStatus::UpdateStaticText()
{
	if (RTB_IncreaseOption.IsValid())
	{
		RTB_IncreaseOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Increase_Abbrev")));
	}

	if (RTB_DecreaseOption.IsValid())
	{
		RTB_DecreaseOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Decrease_Abbrev")));
	}
}

void UB2UI_TotemSmeltingStatus::UpdateTotemSmeltingStatus(const FB2Totem& InTotem)
{
	CachedB2TotemInfo = InTotem;

	for (int i = 0; i < MAX_TOTEM_SUB_OPTION; ++i)
	{
		if (UIP_TotemSmeltingSlotBPs.IsValidIndex(i) && InTotem.SubOptionDetails.IsValidIndex(i) && SubOptionType.IsValidIndex(i))
		{
			int32 IconIndex = i + 1;
			UIP_TotemSmeltingSlotBPs[i]->UpdateIndexIcon(IconIndex);

			FB2TotemInfo TotemInfo;
			// 마스터데이터에서 얻은 최대 제련 횟수
			const int32 MaxSmeltingCount = (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(InTotem.RefID, TotemInfo) ? TotemInfo.RefineLimitCounts[i] : 0);
			// 제련을 시도한 횟수 = 서브 옵션에 들어있는 제련 상태가 NONE 이 아닌 횟수
			int32 SmeltingTryCount = 0;
			for (int j = 0; j < InTotem.SubOptionDetails[i].RefineStates.Num(); ++j)
			{
				if (InTotem.SubOptionDetails[i].RefineStates[j].RefineState != ETotemRefineState::NONE)
				{
					++SmeltingTryCount;
				}
			}
			// 남은 제련 횟수 = 최대 제련 횟수 - 제련을 시도한 횟수
			const int32 SmeltingChance = FMath::Max(MaxSmeltingCount - SmeltingTryCount, 0);
			UIP_TotemSmeltingSlotBPs[i]->UpdateOptionText(InTotem.SubOptionDetails[i].SubOption, SubOptionType[i],SmeltingChance);
			UIP_TotemSmeltingSlotBPs[i]->UpdateWidgetVisible(SmeltingChance);
			UIP_TotemSmeltingSlotBPs[i]->UpdateSlotHole(MaxSmeltingCount, InTotem.SubOptionDetails[i]);

			// 업데이트 시 잠금 해제
			// 제련을 요청 시 잠금이 걸리며 응답이 와서 업데이트가 되면서 잠금이 해제되는 방식
			IsLockSmelting = false;
		}
	}
}

void UB2UI_TotemSmeltingStatus::SetTotemSmeltingMaterial(ETotemRefineCostType SmeltingMaterial)
{
	CachedB2SmeltingMaterial = SmeltingMaterial;
	const int32 TargetSmeltingChance = CachedB2TotemInfo.RefineTryCount + 1;

	switch (SmeltingMaterial)
	{
	case ETotemRefineCostType::TRC_GOLD:
	{
		for (int i = 0; i < MAX_TOTEM_SUB_OPTION; ++i)
		{
			if (UIP_TotemSmeltingSlotBPs.IsValidIndex(i))
			{
				UIP_TotemSmeltingSlotBPs[i]->UpdateSmeltingCost(SmeltingMaterial, CachedB2TotemInfo.Grade, TargetSmeltingChance);
			}
		}
	}break;
	case ETotemRefineCostType::TRC_DIMENTIONAL_PIECE:
	{
		for (int i = 0; i < MAX_TOTEM_SUB_OPTION; ++i)
		{
			if (UIP_TotemSmeltingSlotBPs.IsValidIndex(i))
			{
				UIP_TotemSmeltingSlotBPs[i]->UpdateSmeltingCost(SmeltingMaterial, CachedB2TotemInfo.Grade, TargetSmeltingChance);
			}
		}
	}break;
	default: break;
	}
}	

void UB2UI_TotemSmeltingStatus::SmeltingTotem(int32 OptionIndex)
{
	int32 SubOptionIndexArrayIndex = OptionIndex - 1;

	if (CachedB2TotemInfo.SubOptionDetails.IsValidIndex(SubOptionIndexArrayIndex))
	{
		int32 ServerOptionType = CliToSvrOptionType(CachedB2TotemInfo.SubOptionDetails[SubOptionIndexArrayIndex].SubOption.OptionType);
		int32 ServerRefineCostType = CliToSvrRefineCostType(CachedB2SmeltingMaterial);

		bool LastTry = (CachedB2TotemInfo.GetMaxSmeltingCount() - CachedB2TotemInfo.RefineTryCount == 1);

		LobbyReqSmeltingTotemClass<int64, int32, int32, bool>::GetInstance().Signal(CachedB2TotemInfo.ID, ServerOptionType, ServerRefineCostType, LastTry);
	}
}