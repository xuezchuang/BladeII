// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherLotteryPopupBase.h"
#include "B2UIEtherScrollSlot.h"
#include "BladeIIUtil.h"
#include "B2ItemInfo.h"
#include "B2EtherManager.h"
#include "B2UIEtherOptionSlot.h"
#include "B2GameDefine.h"


void UB2UIEtherLotteryPopupBase::Init()
{
	Super::Init();

	if (UI_EtherIcon.IsValid())
	{
		UI_EtherIcon->Init();
	}

	for (const TWeakObjectPtr<UB2UIEtherOptionSlot>Elem : UIP_EtherOptions)
	{
		if (Elem.IsValid())
		{
			Elem->Init();
			Elem->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIEtherLotteryPopupBase::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UI_EtherIcon.IsValid())
	{
		UI_EtherIcon->DestroySelf(InUIManager);
	}

	for (const TWeakObjectPtr<UB2UIEtherOptionSlot>Elem : UIP_EtherOptions)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}

	UIP_EtherSetOptionInfos.Empty();

	Super::DestroySelf(InUIManager);
}

void UB2UIEtherLotteryPopupBase::CacheAssets()
{
	GET_SLOT(UB2UIEtherScrollSlot, UI_EtherIcon);
	GET_SLOT(UTextBlock, X_TB_SetName);
	GET_SLOT(UTextBlock, X_TB_SetInfo);
	GET_SLOT(UB2Button, X_BTN_Close);
	GET_SLOT(UImage, X_ING_TierBG);
	GET_SLOT(UTextBlock, X_TB_EtherTierName);
	GET_SLOT(UB2RichTextBlock, X_RTB_MainOptionName);
	GET_SLOT(UB2RichTextBlock, X_RTB_MainOptionValue);

	UIP_EtherSetOptionInfos.SetNum(2);
	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[0].TB_Name, X_TB_SetOptionName_1);
	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[0].TB_Value, X_TB_SetOptionValue_1);
	GET_SLOT_BYNAME(UPanelWidget, UIP_EtherSetOptionInfos[0].PW_Slot, X_PW_Slot1);

	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[1].TB_Name, X_TB_SetOptionName_2);
	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[1].TB_Value, X_TB_SetOptionValue_2);
	GET_SLOT_BYNAME(UPanelWidget, UIP_EtherSetOptionInfos[1].PW_Slot, X_PW_Slot2);

	UIP_EtherOptions.SetNum(3);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_EtherOptions[0], X_UIP_SubOption1);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_EtherOptions[1], X_UIP_SubOption2);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_EtherOptions[2], X_UIP_SubOption3);
}

void UB2UIEtherLotteryPopupBase::SetEtherSetInfo(b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr)
{
	if (SetOptionPtr != nullptr)
	{
		if (X_TB_SetName.IsValid())
		{
			if (SetOptionPtr->min_apply_count == EtherSetCount::DEFENCE) //방어 세트
			{
				X_TB_SetName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_2"))));
			}
			else if (SetOptionPtr->min_apply_count == EtherSetCount::OFFENCE) // 공격 세트
			{
				X_TB_SetName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_1"))));
			}
		}

		if (X_TB_SetInfo.IsValid())
		{
			X_TB_SetInfo->SetText(B2EtherManager::GetInstance().GetEtherSetOptionText(SetOptionPtr->set_option_id));
		}

		UIP_EtherSetOptionInfos[0].SetText("Ether_SetOption_Detail1", "General_Stat_Percent", SetOptionPtr->trigger_rate * 0.01);
		UIP_EtherSetOptionInfos[1].SetText("Ether_SetOption_Detail2", "General_Sconds", SetOptionPtr->cooldown_second);
	}
}

void UB2UIEtherLotteryPopupBase::SetTierInfo(int32 InTier)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
	{
		if (X_ING_TierBG.IsValid())
		{
			X_ING_TierBG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierNameBGMtrl(InTier));
		}
	}

	if (X_TB_EtherTierName.IsValid())
	{
		X_TB_EtherTierName->SetText(B2EtherManager::GetInstance().GetEtherTierName(InTier));
	}
}

void UB2UIEtherLotteryPopupBase::FSetInfo::SetText(FString InNameKey, FString InOptionKey, float InValue)
{
	if (InNameKey != TEXT(""))
	{
		if (TB_Name.IsValid() && TB_Name.IsValid())
		{
			TB_Name->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), InNameKey));

			if (InValue > 0)
			{
				TB_Value->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, InOptionKey), FText::AsNumber(InValue)));
			}
			else
			{
				TB_Value->SetText(FText::FromString(TEXT("-")));
			}
		}
	}
	else
	{
		if (PW_Slot.IsValid())
		{
			PW_Slot->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
