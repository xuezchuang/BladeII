// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherLotteryResultPopup.h"
#include "B2EtherManager.h"
#include "B2UIEtherScrollSlot.h"
#include "B2UIEtherOptionSlot.h"
#include "B2Inventory.h"
#include "B2UIManager.h" 

void UB2UIEtherLotteryResultPopup::SetPopupInfo(const b2network::B2AetherServerInfoPtr EtherPtr)
{
	FB2Ether EtherInfo(EtherPtr);

	if (UI_EtherIcon.IsValid())
	{
		UI_EtherIcon->SetInfo(&EtherInfo);
	}

	SetEtherSetInfo(B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(EtherInfo.Type, EtherInfo.Tier));
	SetTierInfo(EtherInfo.Tier);

	if (X_RTB_MainOptionName.IsValid())
	{
		X_RTB_MainOptionName->SetText(GetLOCTextOfItemOption(EtherInfo.MainOption.OptionType, EPCClass::EPC_End));
	}

	if (X_RTB_MainOptionValue.IsValid())
	{
		X_RTB_MainOptionValue->SetText(GetItemOptionValueDisplayText(EtherInfo.MainOption.OptionType, EtherInfo.MainOption.RawOptionAmount, true));
	}

	for (int i = 0; i < EtherInfo.SubOption.Num(); ++i)
	{
		if (UIP_EtherOptions[i].IsValid())
		{
			UIP_EtherOptions[i]->SetVisibility(ESlateVisibility::Visible);
			UIP_EtherOptions[i]->SetInfo(EtherInfo.SubOption[i].OptionType, EtherInfo.SubOption[i].RawOptionAmount, EPCClass::EPC_End);
		}
	}
}

void UB2UIEtherLotteryResultPopup::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherLotteryResult);
}

void UB2UIEtherLotteryResultPopup::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIEtherLotteryResultPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherLotteryResultPopup::OnClickBtnClose);
}

void UB2UIEtherLotteryResultPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2RichTextBlock, TB_Receive);
	GET_SLOT(UTextBlock, TB_ResultType);
}

void UB2UIEtherLotteryResultPopup::UpdateStaticText()
{
	if (TB_Receive.IsValid())
	{
		TB_Receive->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
	}

	if (TB_ResultType.IsValid())
	{
		TB_ResultType->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_MAILCONTENT), FString(TEXT("MailTab_Ticket"))));
	}
}
