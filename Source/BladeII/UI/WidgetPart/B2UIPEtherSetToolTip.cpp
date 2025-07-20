// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPEtherSetToolTip.h"
#include "BladeIIUtil.h"
#include "BladeIILocText.h"
#include "B2EtherManager.h"
#include "BladeIIGameImpl.h"


void UB2UIPEtherSetToolTip::SetInfo(int32 InSetID, EPCClass InPCClass)
{
	this->SetVisibility(ESlateVisibility::Visible);

	TMap<int64, FB2Ether> OwnerEthers = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEquippedEthers(InPCClass);

	float CoolDown = B2EtherManager::GetInstance().GetEtherSetOptionCoolDown(InSetID, OwnerEthers);
	float TriggerRate = B2EtherManager::GetInstance().GetEtherSetOptionTriggerRate(InSetID, OwnerEthers) * 100.0f;
	float Duration = B2EtherManager::GetInstance().GetEtherSetEffecttDuration(InSetID);
	float OptionValue1 = 0.0f;
	float OptionValue2 = 0.0f;
	B2EtherManager::GetInstance().GetEtherSetOptionValue(InSetID, OptionValue1, OptionValue2);
	FString OptionDesc = B2EtherManager::GetInstance().GetEtherSetOptionDescTextTableKey(InSetID);
	FString OptionUnit = B2EtherManager::GetInstance().GetEtherSetOptionUnitTableKey(InSetID);

	EtherSetOptionInfos[0].SetText("Ether_SetOption_Detail1", "General_Stat_Percent", TriggerRate, false);
	EtherSetOptionInfos[1].SetText("Ether_SetOption_Detail2", "General_Sconds", CoolDown, false);
	EtherSetOptionInfos[2].SetText("Ether_SetOption_Detail3", "General_Sconds", Duration, true);
	EtherSetOptionInfos[3].SetText(OptionDesc, OptionUnit, OptionValue1, true, OptionValue2);
}

void UB2UIPEtherSetToolTip::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UB2Button, BTN_Close);

	EtherSetOptionInfos.SetNum(4);
	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[0].TB_Name, X_TB_SetOptionName_1);
	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[0].TB_Value, X_TB_SetOptionValue_1);
	GET_SLOT_BYNAME(UPanelWidget, EtherSetOptionInfos[0].PW_Slot, X_PW_Slot1);

	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[1].TB_Name, X_TB_SetOptionName_2);
	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[1].TB_Value, X_TB_SetOptionValue_2);
	GET_SLOT_BYNAME(UPanelWidget, EtherSetOptionInfos[1].PW_Slot, X_PW_Slot2);

	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[2].TB_Name, X_TB_SetOptionName_3);
	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[2].TB_Value, X_TB_SetOptionValue_3);
	GET_SLOT_BYNAME(UPanelWidget, EtherSetOptionInfos[2].PW_Slot, X_PW_Slot3);

	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[3].TB_Name, X_TB_SetOptionName_4);
	GET_SLOT_BYNAME(UTextBlock, EtherSetOptionInfos[3].TB_Value, X_TB_SetOptionValue_4);
	GET_SLOT_BYNAME(UPanelWidget, EtherSetOptionInfos[3].PW_Slot, X_PW_Slot4);
}

void UB2UIPEtherSetToolTip::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT(""))));
	}
}

void UB2UIPEtherSetToolTip::BindDelegates()
{
	if (BTN_Close.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIPEtherSetToolTip::OnClickClose);
	}
}

void UB2UIPEtherSetToolTip::OnClickClose()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void FEtherSetSlotInfo::SetText(FString InNameKey, FString InOptionKey, float InValue1, bool IsVisible, float InValue2 /*= 0.0f*/)
{
	if (PW_Slot.IsValid())
	{
		PW_Slot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (InNameKey != TEXT(""))
	{
		if (TB_Name.IsValid() && TB_Name.IsValid())
		{
			TB_Name->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), InNameKey));

			if (InValue1 > 0)
			{
				if (InValue2 == 0)
				{
					TB_Value->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, InOptionKey), FText::AsNumber(InValue1)));
				}
				else
				{
					TB_Value->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, InOptionKey), FText::AsNumber(InValue1), FText::AsNumber(InValue2)));
				}
			}
			else
			{
				if (IsVisible == false)
				{
					TB_Value->SetText(FText::FromString(TEXT("-")));
				}
				else if (IsVisible == true && PW_Slot.IsValid())
				{
					PW_Slot->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
	else
	{
		if (PW_Slot.IsValid())
		{
			PW_Slot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}