// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"
#include "B2UIManager.h"
#include "CommonStruct.h"
#include "B2UIShortagePopupManager.h"
#include "B2UI_TotemSmeltingStatus.h"
#include "B2UI_TotemSmeltingSlot.h"

//////////////////////////////////////////////////////////////
//
// UB2UI_TotemSmeltingSlotHole
//
//////////////////////////////////////////////////////////////

void UB2UI_TotemSmeltingSlotHole::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_SmeltingSlotHole);
}

void UB2UI_TotemSmeltingSlotHole::InitSlotHole()
{
	CacheAssets();
	SetSlotHole(ETotemRefineState::NONE);
}

void UB2UI_TotemSmeltingSlotHole::SetSlotHole(ETotemRefineState HoleState)
{
	if (WS_SmeltingSlotHole.IsValid())
	{
		WS_SmeltingSlotHole->SetActiveWidgetIndex(static_cast<int32>(HoleState));
	}
}

//////////////////////////////////////////////////////////////
//
// UB2UI_TotemSmeltingSlot
//
//////////////////////////////////////////////////////////////

void UB2UI_TotemSmeltingSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_SlotIcon);
	GET_SLOT(UB2RichTextBlock, RTB_SmeltingBonusOption);
	GET_SLOT(UB2RichTextBlock, RTB_SmeltingChance);

	GET_SLOT(UB2Button, BTN_Smelting);

	GET_SLOT(UHorizontalBox, HB_SmeltingSlotHole);

	GET_SLOT(UImage, IMG_MaterialIcon);
	GET_SLOT(UB2RichTextBlock, RTB_SmeltingCost);
}

void UB2UI_TotemSmeltingSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Smelting, &UB2UI_TotemSmeltingSlot::OnClickBTN_Smelting);
}

void UB2UI_TotemSmeltingSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	ClearCachedTotemSmeltingSlotHole();

	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemSmeltingSlot::UpdateIndexIcon(const int32& InIndex)
{
	CachedOptionIndex = InIndex;

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_SlotIcon.IsValid())
	{
		IMG_SlotIcon->SetBrushFromMaterial(ItemMiscInfo->GetTotemOptionIndexIcon(InIndex));
	}
}

void UB2UI_TotemSmeltingSlot::UpdateOptionText(const FItemOption& TotemOption, const ETotemSubOptionType& TotemOptionType, const int32& ChanceCount)
{
	if (RTB_SmeltingBonusOption.IsValid())
	{
		FText OptionFormat = FText::FromString(TEXT("{0} {1}{2}"));

		FText OptionText = (IsItemOptionPerPCClass(TotemOption.OptionType) ?
			BladeIIGameImpl::GetTotemDataStore().GetLOCTextOfSkillOption(TotemOption.OptionType) :
			GetLOCTextOfItemOption(TotemOption.OptionType, EPCClass::EPC_Fighter));

		FText OptinoTypeText = GetSubOptionTypeText(TotemOptionType);
		FText ChanceCountText = GetItemOptionValueDisplayText(TotemOption.OptionType, TotemOption.RawOptionAmount, true);

		RTB_SmeltingBonusOption->SetText(FText::Format(OptionFormat, OptionText, OptinoTypeText, ChanceCountText));
	}

	if (RTB_SmeltingChance.IsValid())
	{
		FText ChanceCountFormat = FText::FromString(TEXT("({0})"));
		RTB_SmeltingChance->SetText(FText::Format(ChanceCountFormat, FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_ChanceCount")), ChanceCount)));
	}
}

void UB2UI_TotemSmeltingSlot::UpdateSlotHole(const int32& MaxHoleNum, const FB2TotemSubOptionDetail& SubOptionDetail)
{
	HB_SmeltingSlotHole->ClearChildren();
	ClearCachedTotemSmeltingSlotHole();

	for (int32 i = 0; i < MaxHoleNum; ++i)
	{
		if (CachedTotemSmeltingSlotHole.IsValidIndex(i) == false)
		{
			UB2UI_TotemSmeltingSlotHole* pSmeltingSlotHole = Cast<UB2UI_TotemSmeltingSlotHole>(DynCreateInHorizontalBox(UITotemSmeltingSlotHoleClass, this, HB_SmeltingSlotHole.Get(), FVector2D(0.0f, 0.0f), FMargin()));
			if (pSmeltingSlotHole)
			{
				pSmeltingSlotHole->InitSlotHole();

				if (SubOptionDetail.RefineStates.IsValidIndex(i))
				{
					ETotemRefineState HoleState = SubOptionDetail.RefineStates[i].RefineState;
					pSmeltingSlotHole->SetSlotHole(HoleState);
				}
			}
			CachedTotemSmeltingSlotHole.Add(pSmeltingSlotHole);
		}
	}
}

void UB2UI_TotemSmeltingSlot::UpdateSmeltingCost(ETotemRefineCostType SmeltingMaterial, const ETotemGrade& Grade, const int32& SmeltingChance)
{
	CashedCostType = SmeltingMaterial;

	switch (SmeltingMaterial)
	{
	case ETotemRefineCostType::TRC_GOLD:
	{
		if (IMG_MaterialIcon.IsValid())
		{
			IMG_MaterialIcon->SetBrushFromMaterial(SmeltingMaterialIcon_Gold);
		}
		if (RTB_SmeltingCost.IsValid())
		{
			int32 Cost = GetSmeltingCostByGold(Grade, SmeltingChance);
			IsEnoughCost = (Cost <= BladeIIGameImpl::GetClientDataStore().GetGoldAmount());
			RTB_SmeltingCost->SetText(FText::AsNumber(Cost));
			RTB_SmeltingCost->SetColorAndOpacity(IsEnoughCost ? SmeltingEnableColor.GetSpecifiedColor() : SmeltingNotEnoughConditionColor.GetSpecifiedColor());
		}
		if (BTN_Smelting.IsValid())
		{
			BTN_Smelting->SetIsEnabled(IsEnoughCost);
		}
	}break;
	case ETotemRefineCostType::TRC_DIMENTIONAL_PIECE:
	{
		if (IMG_MaterialIcon.IsValid())
		{
			IMG_MaterialIcon->SetBrushFromMaterial(SmeltingMaterialIcon_Piece);
		}
		if (RTB_SmeltingCost.IsValid())
		{
			int32 Cost = GetSmeltingCostByPiece(Grade, SmeltingChance);
			IsEnoughCost = (Cost <= UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE));
			RTB_SmeltingCost->SetText(FText::AsNumber(Cost));
			RTB_SmeltingCost->SetColorAndOpacity(IsEnoughCost ? SmeltingEnableColor.GetSpecifiedColor() : SmeltingNotEnoughConditionColor.GetSpecifiedColor());
		}
		if (BTN_Smelting.IsValid())
		{
			BTN_Smelting->SetIsEnabled(IsEnoughCost);
		}
	}break;
	default: break;
	}
}

void UB2UI_TotemSmeltingSlot::UpdateWidgetVisible(const int32& SmeltingChance)
{
	if (IMG_MaterialIcon.IsValid())
	{
		IMG_MaterialIcon->SetVisibility((SmeltingChance <= 0) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (RTB_SmeltingCost.IsValid())
	{
		RTB_SmeltingCost->SetVisibility((SmeltingChance <= 0) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (BTN_Smelting.IsValid())
	{
		BTN_Smelting->SetVisibility((SmeltingChance <= 0) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}

void UB2UI_TotemSmeltingSlot::ClearCachedTotemSmeltingSlotHole()
{
	for (int32 i = 0 ; i < CachedTotemSmeltingSlotHole.Num(); ++i)
	{
		if (CachedTotemSmeltingSlotHole.IsValidIndex(i))
		{
			CachedTotemSmeltingSlotHole[i]->DestroySelf(UB2UIManager::GetInstance());
		}
	}

	CachedTotemSmeltingSlotHole.Empty();
}

int32 UB2UI_TotemSmeltingSlot::GetSmeltingCostByGold(const ETotemGrade& Grade, const int32& SmeltingChance)
{
	TArray<FB2TotemRefineInfo>& TotemRefineInfos = BladeIIGameImpl::GetTotemDataStore().GetTotemRefineInfos();
	
	for ( FB2TotemRefineInfo TotemRefineInfo : TotemRefineInfos)
	{
		if (TotemRefineInfo.Grade == CliToSvrTotemGrade(Grade) && TotemRefineInfo.RefineChance == SmeltingChance)
		{
			return TotemRefineInfo.GoldCost;
		}
	}
	
 	return 0;
}

int32 UB2UI_TotemSmeltingSlot::GetSmeltingCostByPiece(const ETotemGrade& Grade, const int32& SmeltingChance)
{
	TArray<FB2TotemRefineInfo>& TotemRefineInfos = BladeIIGameImpl::GetTotemDataStore().GetTotemRefineInfos();

	for (FB2TotemRefineInfo TotemRefineInfo : TotemRefineInfos)
	{
		if (TotemRefineInfo.Grade == CliToSvrTotemGrade(Grade) && TotemRefineInfo.RefineChance == SmeltingChance)
		{
			return TotemRefineInfo.DimensionalPieceCost;
		}
	}

	return 0;
}

FText UB2UI_TotemSmeltingSlot::GetSubOptionTypeText(ETotemSubOptionType InSubOptionType)
{
	if (InSubOptionType == ETotemSubOptionType::OPTION_PLUS)
	{
		return FText::FromString(TEXT("+"));
	}
	else if (InSubOptionType == ETotemSubOptionType::OPTION_MINUS)
	{
		return  FText::FromString(TEXT("-"));
	}
	else
	{
		return  FText::FromString(TEXT(""));
	}
}

void UB2UI_TotemSmeltingSlot::OnClickBTN_Smelting()
{
	// 팝업 기획이 제거되고 버튼을 Enabled 시키는걸로 변경
// 	if (IsEnoughCost == false)
// 	{
// 		if (CashedCostType == ETotemRefineCostType::TRC_GOLD)
// 		{
// 			ShortageMGR->PopupGoToShopForGoldMessage();
// 		}
// 		else if (CashedCostType == ETotemRefineCostType::TRC_DIMENTIONAL_PIECE)
// 		{
// 			ShortageMGR->PopupGoToShopForGoldMessage();
// 		}
// 		return;
// 	}

	if (TotemStatus)
	{
		if (TotemStatus->GetSmeltingButtonLockState() == false)
		{
			TotemStatus->SetSmeltingButtonLockState(true);
			OnSlotSmeltingClickDelgate.ExecuteIfBound(CachedOptionIndex);
		}
	}
}

