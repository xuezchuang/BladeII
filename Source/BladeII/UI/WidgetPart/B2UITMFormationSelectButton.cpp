// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UITMFormationSelectButton.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIFormationSelectButtonSlot.h"
#include "BladeIIGameImpl.h"

void UB2UITMFormationSelectButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT_BYNAME(UB2UIFormationSelectButtonSlot, UIP_SlotInfo, UIP_FormationSelectButton);
	if (UIP_SlotInfo.IsValid())
	{
		UIP_SlotInfo->Init();
	}

	GET_SLOT(UTextBlock, TB_Enhance);
	GET_SLOT(UButton, BTN_Enhance);
	GET_SLOT(UButton, BTNX_Select);

	RTB_Ability.SetNum(3);
	GET_SLOT_BYNAME(UB2RichTextBlock, RTB_Ability[0], RTB_FirstAbility);
	GET_SLOT_BYNAME(UB2RichTextBlock, RTB_Ability[1], RTB_SecondAbility);
	GET_SLOT_BYNAME(UB2RichTextBlock, RTB_Ability[2], RTB_ThirdAbility);
	
	O_Default.SetNum(3);
	GET_SLOT_BYNAME(UOverlay, O_Default[0], X_FirstDefault);
	GET_SLOT_BYNAME(UOverlay, O_Default[1], X_SecondDefault);
	GET_SLOT_BYNAME(UOverlay, O_Default[2], X_ThirdDefault);
	O_Select.SetNum(3);
	GET_SLOT_BYNAME(UOverlay, O_Select[0], X_FIrstSelect);
	GET_SLOT_BYNAME(UOverlay, O_Select[1], X_SecondSelect);
	GET_SLOT_BYNAME(UOverlay, O_Select[2], X_ThirdSelect);
}

void UB2UITMFormationSelectButton::BindDelegates()
{
	Super::BindDelegates();
	if (BTN_Enhance.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(BTN_Enhance, &UB2UITMFormationSelectButton::OnClickEnhance);
	}

	if (BTNX_Select.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(BTNX_Select, &UB2UITMFormationSelectButton::OnClickSelect);
	}
}

void UB2UITMFormationSelectButton::UpdateStaticText()
{
	if (TB_Enhance.IsValid())
	{
		TB_Enhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Teamformation_Upgrade"))); //碍拳
	}
}

void UB2UITMFormationSelectButton::OnClickSelect()
{
	SelectDelegate.ExecuteIfBound(FormationType);
}

void UB2UITMFormationSelectButton::OnClickEnhance()
{
	ButtonClickDelegate.ExecuteIfBound(FormationType, FormationLevel);
}

void UB2UITMFormationSelectButton::Init()
{
	Super::Init();
}

void UB2UITMFormationSelectButton::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	if (UIP_SlotInfo.IsValid())
	{ 
		UIP_SlotInfo->DestroySelf(InUIManager);
	}
}

void UB2UITMFormationSelectButton::SetDelegate(FFormationAnimationDelegate AnimaionDelegate, FFormationClickDelegate ClickDelegate)
{
	if (UIP_SlotInfo.IsValid())
	{
		UIP_SlotInfo->SlotStateDelegate = AnimaionDelegate;
	}
	SelectDelegate = AnimaionDelegate;
	ButtonClickDelegate = ClickDelegate;
}

void UB2UITMFormationSelectButton::SetFormation(ETMFormation Formation, int32 Level, TArray<float>& AttackBonus, TArray<float>& DefenseBonus)
{
	FormationLevel = Level;
	FormationType = Formation;

	if (UIP_SlotInfo.IsValid())
	{
		UIP_SlotInfo->SetFormationType(Formation, Level);
	}

	for (int i = 0; i < RTB_Ability.Num(); ++i)
	{
		if (RTB_Ability[i].IsValid())
		{
			if ((AttackBonus.IsValidIndex(i) == false && DefenseBonus.IsValidIndex(i) == false) 
				|| (AttackBonus[i] == 0 && DefenseBonus[i] == 0))
			{
				RTB_Ability[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				RTB_Ability[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
				if (AttackBonus[i] == 0)
				{
					RTB_Ability[i]->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TMBuff_Defensive")), FText::AsNumber(DefenseBonus[i])));
				}
				else if (DefenseBonus[i] == 0)
				{
					RTB_Ability[i]->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TMBuff_Offensive")), FText::AsNumber(AttackBonus[i])));
				}
				else
				{
					RTB_Ability[i]->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TMBuff_Both")), FText::AsNumber(DefenseBonus[i]), FText::AsNumber(AttackBonus[i])));
				}
			}
		}
	}

	if (Level >= BladeIIGameImpl::GetClientDataStore().GetMaxLevelTeamFormation(FormationType))
	{
		if(BTN_Enhance.IsValid())
			BTN_Enhance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UITMFormationSelectButton::SetOnSlotState()
{
	if (UIP_SlotInfo.IsValid())
	{
		UIP_SlotInfo->SetOnSlotState();
	}
	OnClickState();

	for (int i = 0; i < O_Select.Num();i++)
	{
		if (RTB_Ability[i]->GetVisibility() == ESlateVisibility::Collapsed)
		{
			O_Default[i]->SetVisibility(ESlateVisibility::Collapsed);
			O_Select[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			O_Default[i]->SetVisibility(ESlateVisibility::Collapsed);
			O_Select[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UB2UITMFormationSelectButton::SetOffSlotState()
{
	if (UIP_SlotInfo.IsValid())
	{
		UIP_SlotInfo->SetOffSlotState();
	}
	OffClickState();

	for (int i = 0; i < O_Select.Num();i++)
	{
		if (RTB_Ability[i]->GetVisibility() == ESlateVisibility::Collapsed)
		{
			O_Default[i]->SetVisibility(ESlateVisibility::Collapsed);
			O_Select[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			O_Default[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
			O_Select[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
