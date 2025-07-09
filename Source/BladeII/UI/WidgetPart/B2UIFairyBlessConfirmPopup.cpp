// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIFairyBlessConfirmPopup.h"
#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "B2Inventory.h"

void UB2UIFairyBlessConfirmPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);

	GET_SLOT(UB2RichTextBlock, RTB_Stat); 
	GET_SLOT(UB2RichTextBlock, RTB_Content);

	GET_SLOT(UB2Button, BTN_Negative);
	GET_SLOT(UB2Button, BTN_Positive);

	GET_SLOT(UTextBlock, TB_BtnNegative);
	GET_SLOT(UTextBlock, TB_BtnPositive); 

}

void UB2UIFairyBlessConfirmPopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	if (TB_BtnNegative.IsValid())
		TB_BtnNegative->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_No")));
	if (TB_BtnPositive.IsValid())
		TB_BtnPositive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Yes")));

	if (RTB_Content.IsValid())
		RTB_Content->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessConfirmNotice")));
}

void UB2UIFairyBlessConfirmPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Positive, &UB2UIFairyBlessConfirmPopup::OnClickBTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Negative, &UB2UIFairyBlessConfirmPopup::OnClickBTN_Cancel);
}

void UB2UIFairyBlessConfirmPopup::CloseWidgetDelegate()
{
	CloseSelf();
}

void UB2UIFairyBlessConfirmPopup::Init()
{
	Super::Init();

}

void UB2UIFairyBlessConfirmPopup::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIFairyBlessConfirmPopup::SetBlessOption(int32 OptionId, float OptionValue)
{
	EItemOption CurrOption = SvrToCliOptionType(OptionId);

	FText OptionName;
	OptionName = GetLOCTextOfItemOption(CurrOption, EPCClass::EPC_End);

	FText OptionValueText = GetItemOptionValueDisplayText(CurrOption, OptionValue, true);

	if (RTB_Stat.IsValid())
		RTB_Stat->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessStat")), OptionName, OptionValueText));
}

void UB2UIFairyBlessConfirmPopup::SetConfirmBtnHandler(const FFairyBlessConfirmOnClick& InConfirmBtnHandler)
{
	OnClickConfirmBtnHandler = InConfirmBtnHandler;
}

void UB2UIFairyBlessConfirmPopup::OnClickBTN_Confirm()
{
	OnClickConfirmBtnHandler.ExecuteIfBound(); 

	CloseSelf();
}


void UB2UIFairyBlessConfirmPopup::OnClickBTN_Cancel()
{
	CloseSelf();
}



