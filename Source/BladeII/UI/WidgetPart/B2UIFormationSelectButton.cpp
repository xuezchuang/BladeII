// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIFormationSelectButton.h"


void UB2UIFormationSelectButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIFormationSelectButtonSlot, UIP_FormationSelectButton);
	if (UIP_FormationSelectButton.IsValid())
	{
		UIP_FormationSelectButton->Init();

		if (FormationOnIcon.Num() && FormationOffIcon.Num())
		{
			UIP_FormationSelectButton->FormationOnIcon = FormationOnIcon;
			UIP_FormationSelectButton->FormationOffIcon = FormationOffIcon;
		}
	}
	GET_SLOT(UB2RichTextBlock, RTB_Ability1);

}
void UB2UIFormationSelectButton::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIFormationSelectButton::Init()
{
	Super::Init();
}

void UB2UIFormationSelectButton::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_FormationSelectButton.IsValid()) UIP_FormationSelectButton->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UIFormationSelectButton::SetFormationSetting(ETMFormation Formation, int32 Lv, int32 Param1, int32 Param2, FFormationSlotStateDelegate Delegate)
{
	FString FormationNameKey, FormationExplanationKey;

	UIP_FormationSelectButton->SetFormationType(Formation);
	UIP_FormationSelectButton->SlotStateDelegate = Delegate;

	switch (Formation)
	{
	case ETMFormation::ETM_Normal:
		FormationNameKey = TEXT("ChallengeMod_TeamMatch_FormationNormal");
		FormationExplanationKey = TEXT("TeamFormationBuff_Normal");
		break;
	case ETMFormation::ETM_Attack:
		FormationNameKey = TEXT("ChallengeMod_TeamMatch_FormationAtt");
		FormationExplanationKey = TEXT("TeamFormationBuff_Offensive");
		break;
	case ETMFormation::ETM_Defense:
		FormationNameKey = TEXT("ChallengeMod_TeamMatch_FormationDef");
		FormationExplanationKey = TEXT("TeamFormationBuff_Defensive");
		break;
	}

	if (Param1 && Param2)
		RTB_Ability1->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FormationExplanationKey), FText::AsNumber(Param1), FText::AsNumber(Param2)));
	
	else if (Param1)
		RTB_Ability1->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FormationExplanationKey), FText::AsNumber(Param1)));
	else if (Param2)
		RTB_Ability1->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FormationExplanationKey), FText::AsNumber(Param2)));
}

void UB2UIFormationSelectButton::SetOnSlotState()
{
	UIP_FormationSelectButton->SetOnSlotState();
	OnClickState();
}

void UB2UIFormationSelectButton::SetOffSlotState()
{
	UIP_FormationSelectButton->SetOffSlotState();
	OffClickState();
}


