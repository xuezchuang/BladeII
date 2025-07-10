#include "B2UIFormationSelectButtonSlot.h"
#include "B2UIDocHelper.h"
#include "BladeIILocText.h"

void UB2UIFormationSelectButtonSlot::Init()
{
	Super::Init();

}

void UB2UIFormationSelectButtonSlot::BindDelegates()
{
	if (BTN_SelectButton.IsValid())
		BIND_CLICK_FUNC_TO_BTN(BTN_SelectButton, &UB2UIFormationSelectButtonSlot::OnClickFormationSelect);
}

void UB2UIFormationSelectButtonSlot::SetFormationType(ETMFormation FormationType)
{
	UB2UIDocTeamMatch* DocTeam = UB2UIDocHelper::GetDocTeamMatch();

	if (!DocTeam)
		return;

	EFormationInfo = FormationType;

	switch (FormationType)
	{
	case ETMFormation::ETM_Normal:
		TB_Formation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationNormal")));
		TB_Lv->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(DocTeam->GetFormationNormalLevel()))));
		break;
	case ETMFormation::ETM_Attack:
		TB_Formation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationAtt")));
		TB_Lv->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(DocTeam->GetFormationAttLevel()))));
		break;
	case ETMFormation::ETM_Defense:
		TB_Formation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationDef")));
		TB_Lv->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(DocTeam->GetFormationDefLevel()))));
		break;
	}

}

void UB2UIFormationSelectButtonSlot::SetFormationType(ETMFormation FormationType , int32 Lv)
{
	EFormationInfo = FormationType;

	switch (FormationType)
	{
	case ETMFormation::ETM_Normal:
		TB_Formation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationNormal")));
		break;
	case ETMFormation::ETM_Attack:
		TB_Formation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationAtt")));
		break;
	case ETMFormation::ETM_Defense:
		TB_Formation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationDef")));
		break;
	}

	TB_Lv->SetText(
		FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(Lv))));

}

void UB2UIFormationSelectButtonSlot::SetSlotState(ETMFormation FormationState)
{
	if (EFormationInfo == FormationState)
	{
		SetOnSlotState();
	}
	else
	{
		SetOffSlotState();
	}

}


void UB2UIFormationSelectButtonSlot::SetOnSlotState()
{
	IMG_Formation->SetBrushFromMaterial(FormationOnIcon[static_cast<int32>(EFormationInfo)]);
	OnSelect();
}

void UB2UIFormationSelectButtonSlot::SetOffSlotState()
{
	IMG_Formation->SetBrushFromMaterial(FormationOffIcon[static_cast<int32>(EFormationInfo)]);
	OffSelect();
}

void UB2UIFormationSelectButtonSlot::OnClickFormationSelect()
{
	SlotStateDelegate.ExecuteIfBound(EFormationInfo);
}

void UB2UIFormationSelectButtonSlot::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Lv);
	GET_SLOT(UTextBlock, TB_Formation);
	GET_SLOT(UImage, IMG_Formation);
	GET_SLOT(UButton, BTN_SelectButton);
}