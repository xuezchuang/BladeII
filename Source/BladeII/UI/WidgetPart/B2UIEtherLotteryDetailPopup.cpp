// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherLotteryDetailPopup.h"
#include "B2EtherManager.h"
#include "B2UIEtherScrollSlot.h"
#include "B2UIEtherOptionSlot.h"
#include "BladeIIUtil.h"
#include "B2AndroidBackManager.h"


void UB2UIEtherLotteryDetailPopup::SetPopupInfo(int32 InTypeID, int32 InStar, int32 InTier)
{
	if (UI_EtherIcon.IsValid())
	{
		UI_EtherIcon->SetInfo(InTypeID, InTier, InStar);
	}

	SetEtherSetInfo(B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(InTypeID, InTier));
	SetTierInfo(InTier);

	for (int i = 0; i < InTier - 1; i++)
	{
		if (UIP_EtherOptions.IsValidIndex(i) && UIP_EtherOptions[i].IsValid())
		{
			UIP_EtherOptions[i]->SetVisibility(ESlateVisibility::Visible);
			UIP_EtherOptions[i]->SetDefault();
		}
	}

	if (X_RTB_MainOptionName.IsValid())
	{
		X_RTB_MainOptionName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_RandomOption")));
	}

	if (X_RTB_MainOptionValue.IsValid())
	{
		X_RTB_MainOptionValue->SetText(FText::FromString(TEXT("?")));
	}
}

void UB2UIEtherLotteryDetailPopup::OnClickBtnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
}

void UB2UIEtherLotteryDetailPopup::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIEtherLotteryDetailPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherLotteryDetailPopup::OnClickBtnClose);
}