// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIUserReport.h"
#include "Retailer.h"
#include "B2UIDocSome.h"
#include "B2UIDocHelper.h"

UB2UIUserReport::UB2UIUserReport(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AccountID = 0;
	ModType = 0;
    ReasonFlags = 0;
}

void UB2UIUserReport::Init()
{
	Super::Init();
}

void UB2UIUserReport::CacheAssets()
{
	Super::CacheAssets();

	TB_ReasonArr.SetNum(6);
	CB_ReasonArr.SetNum(6);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Cancel);
	GET_SLOT(UTextBlock, TB_Report);

	GET_SLOT_BYNAME(UTextBlock, TB_ReasonArr[0], TB_Reason1);
	GET_SLOT_BYNAME(UTextBlock, TB_ReasonArr[1], TB_Reason2);
	GET_SLOT_BYNAME(UTextBlock, TB_ReasonArr[2], TB_Reason3);
	GET_SLOT_BYNAME(UTextBlock, TB_ReasonArr[3], TB_Reason4);
	GET_SLOT_BYNAME(UTextBlock, TB_ReasonArr[4], TB_Reason5);
	GET_SLOT_BYNAME(UTextBlock, TB_ReasonArr[5], TB_Reason6);

	GET_SLOT_BYNAME(UCheckBox, CB_ReasonArr[0], CB_Reason1);
	GET_SLOT_BYNAME(UCheckBox, CB_ReasonArr[1], CB_Reason2);
	GET_SLOT_BYNAME(UCheckBox, CB_ReasonArr[2], CB_Reason3);
	GET_SLOT_BYNAME(UCheckBox, CB_ReasonArr[3], CB_Reason4);
	GET_SLOT_BYNAME(UCheckBox, CB_ReasonArr[4], CB_Reason5);
	GET_SLOT_BYNAME(UCheckBox, CB_ReasonArr[5], CB_Reason6);

	GET_SLOT(UTextBlock, TB_NickName);
	GET_SLOT(UEditableText, E_EditBox);
	GET_SLOT(UTextBlock, TB_ReportTitle);

	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_Report);
}

void UB2UIUserReport::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Report, &UB2UIUserReport::OnClickReport);
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIUserReport::OnClickCancel);

    if (E_EditBox.IsValid())
    {
        E_EditBox->OnTextChanged.AddDynamic(this, &UB2UIUserReport::EditBoxTextChange);
    }
}

void UB2UIUserReport::UpdateStaticText()
{
    if (TB_Title.IsValid())
    {
        TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Report")));
    }

    if (TB_Report.IsValid())
    {
        TB_Report->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Report")));
    }

    if (E_EditBox.IsValid())
    {
        E_EditBox->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_ReportDesc")));
    }

    if (TB_ReportTitle.IsValid())
    {
        TB_ReportTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_ReportTarget")));
    }
}

void UB2UIUserReport::UpdatePopup(int32 InModType, int64 InAccountID, FString InNickName)
{
	AccountID = InAccountID;

	SetReason(InModType);
	SetNickName(InNickName);

    // Reset
    UnCheckAll();
}

void UB2UIUserReport::SetReason(int32 InModType)
{
	ModType = InModType;

    int32 SlotNumber = FCString::Atoi(
        *(BladeIIGetLOCText(B2LOC_CAT_USERREPORT,
            FString::FromInt((InModType * ModDivisionNumber) + 10)).ToString()));

	for (int32 i = 0 ; i < CB_ReasonArr.Num() ; ++i)
	{
		if (CB_ReasonArr[i].IsValid())
		{
			CB_ReasonArr[i]->SetCheckedState(ECheckBoxState::Unchecked);			//체크초기화
			CB_ReasonArr[i]->SetVisibility(i > SlotNumber - 1 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}

		if (TB_ReasonArr[i].IsValid())
		{
			TB_ReasonArr[i]->SetText(BladeIIGetLOCText(B2LOC_CAT_USERREPORT,
				FString::FromInt((InModType * ModDivisionNumber) + i + 1))); //PVP=101,102,103... ASSAULT= 201,202,203....
		}
	}

    if (BTN_Report.IsValid())
    {
        BTN_Report->SetIsEnabled(false);
    }

    if (E_EditBox.IsValid())
    {
        E_EditBox->SetIsEnabled(false);
    }
}

void UB2UIUserReport::SetNickName(FString InNickName)
{
    if (TB_NickName.IsValid())
    {
        TB_NickName->SetText(FText::FromString(InNickName));
    }
}

void UB2UIUserReport::OnClickReport()
{
    FString Explain(TEXT(""));

    data_trader::Retailer::GetInstance().RequestReportUser(AccountID, ModType, ReasonFlags, Explain);

	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
    if (DocSome)
    {
        DocSome->SetSendReportUserAccountID(AccountID);
    }
}

void UB2UIUserReport::OnClickCancel()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::UserReportPopup);
}

void UB2UIUserReport::EditBoxTextChange(const FText& InText)
{
    if (BTN_Report.IsValid())
    {
        BTN_Report->SetIsEnabled(InText.IsEmpty() == true ? false : true);
    }
}

void UB2UIUserReport::SetCheckNumber(int32 InCheckNumber, bool IsCheck)
{
 //   int Index = InCheckNumber - 1;

 //   if (Index < 0 || Index >= CB_ReasonArr.Num())
 //   {
 //       return;
 //   }

 //   CB_ReasonArr[Index]->SetCheckedState(IsCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

 //   // Update Reason Flags

 //   ReasonFlags = 0;

 //   for (int i = 0; i < CB_ReasonArr.Num(); i++)
 //   {
 //       const bool checked = CB_ReasonArr[i]->GetCheckedState() == ECheckBoxState::Checked;
 //       if (checked)
 //       {
 //           ReasonFlags += FMath::Pow(2, i);
 //       }
 //   }

	//if (BTN_Report.IsValid())
	//{
 //       if (ReasonFlags != 0)
 //       {
 //           BTN_Report->SetIsEnabled(true);
 //       }
 //       else
 //       {
 //           BTN_Report->SetIsEnabled(false);
 //       }
	//}
}

void UB2UIUserReport::UnCheckAll()
{
    for (int i = 0; i < CB_ReasonArr.Num(); i++)
    {
        CB_ReasonArr[i]->SetCheckedState(ECheckBoxState::Unchecked);
    }

    E_EditBox->SetIsEnabled(true);
    E_EditBox->SetText(FText::GetEmpty());

    ReasonFlags = 0;
}
