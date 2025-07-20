// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMsgPopupSelectFairyGift.h"

#include "B2UIFairyGiftButton.h"
#include "B2UIMsgPopupReward.h"
#include "B2AndroidBackManager.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"

void UB2UIMsgPopupSelectFairyGift::Init()
{
	Super::Init();

	SelectedFairy = EFairyType::FAIRY_NIECE;
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2UIMsgPopupSelectFairyGift::DestroySelf(class UB2UIManager* InUIManager)
{
	for (TPair<EFairyType, TWeakObjectPtr<UB2UIFairyGiftButton>>& Elem : UIP_FairyGiftButtons)
	{
		if(Elem.Value.IsValid())
		{
			Elem.Value->DestroySelf(InUIManager);
		}
	}
	UIP_FairyGiftButtons.Empty();

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupSelectFairyGift::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_GiftChoice);
	GET_SLOT(UTextBlock, TB_Confirm);

	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UB2UIFairyGiftButton, UIP_FairyGiftButton_Niece);
	if (UIP_FairyGiftButton_Niece.IsValid())
	{
		UIP_FairyGiftButton_Niece->Init();
		UIP_FairyGiftButtons.Add(EFairyType::FAIRY_NIECE, UIP_FairyGiftButton_Niece);
	}
	GET_SLOT(UB2UIFairyGiftButton, UIP_FairyGiftButton_Pang);
	if (UIP_FairyGiftButton_Pang.IsValid())
	{
		UIP_FairyGiftButton_Pang->Init();
		UIP_FairyGiftButtons.Add(EFairyType::FAIRY_PANG, UIP_FairyGiftButton_Pang);
	}
	GET_SLOT(UB2UIFairyGiftButton, UIP_FairyGiftButton_Rio);
	if (UIP_FairyGiftButton_Rio.IsValid())
	{
		UIP_FairyGiftButton_Rio->Init();
		UIP_FairyGiftButtons.Add(EFairyType::FAIRY_RIO, UIP_FairyGiftButton_Rio);
	}
}

void UB2UIMsgPopupSelectFairyGift::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	}
	if (TB_GiftChoice.IsValid())
	{
		TB_GiftChoice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_GiftChoice")));
	}
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}
}

void UB2UIMsgPopupSelectFairyGift::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIMsgPopupSelectFairyGift::OnClick_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupSelectFairyGift::OnClick_Close);

	for (TPair<EFairyType, TWeakObjectPtr<UB2UIFairyGiftButton>>& Elem : UIP_FairyGiftButtons)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->SetFairyGiftBtnHandler(FFairyGiftButtonOnClick::CreateLambda([this](EFairyType Fairy) {
				OnChangedFairy(Fairy);
			}));
		}
	}
}

void UB2UIMsgPopupSelectFairyGift::CloseWidgetDelegate()
{
	OnClick_Close();
}

void UB2UIMsgPopupSelectFairyGift::OnClick_Confirm()
{
	data_trader::Retailer::GetInstance().RequestReceiveFairyGift(CliToSvrFairyType(SelectedFairy));
}

void UB2UIMsgPopupSelectFairyGift::OnClick_Close()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	UB2UIManager::GetInstance()->CloseUI(UIFName::MsgPopupSelectFairyGift);
}

void UB2UIMsgPopupSelectFairyGift::OnChangedFairy(EFairyType FairyType)
{
	for (TPair<EFairyType, TWeakObjectPtr<UB2UIFairyGiftButton>>& Elem : UIP_FairyGiftButtons)
	{
		if (Elem.Value.IsValid())
		{
			SelectedFairy = FairyType;
			Elem.Value->SetSelectState(Elem.Key == FairyType ? true : false);
		}
	}
}