// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherLottery.h"
#include "B2UIManager.h"
#include "B2UIEtherLotteryScroll.h"
#include "B2UIEtherLotterySlot.h"
#include "B2UIEtherLotteryDetailPopup.h"
#include "Retailer.h"
#include "B2EtherManager.h"
#include "BladeIIGameImpl.h"
#include "B2AndroidBackManager.h"
#include "B2UIEtherLotteryResultPopup.h"

void UB2UIEtherLottery::Init()
{
	Super::Init();

	if (UI_EtherLotteryScroll.IsValid())
	{
		UI_EtherLotteryScroll->Init();
	}

	if (UI_EtherLotteryIcon.IsValid())
	{
		UI_EtherLotteryIcon->Init();
	}

	if (CP_Ether.IsValid())
	{
		CP_Ether->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CP_NonSelect.IsValid())
	{
		CP_NonSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BTN_Confirm.IsValid())
	{
		BTN_Confirm->SetIsEnabled(false);
	}

	if (UIP_EtherLotteryDetailPopup.IsValid())
	{
		UIP_EtherLotteryDetailPopup->Init();
		UIP_EtherLotteryDetailPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIEtherLottery::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UI_EtherLotteryScroll.IsValid())
	{
		UI_EtherLotteryScroll->DestroySelf(InUIManager);
	}

	if (UI_EtherLotteryIcon.IsValid())
	{
		UI_EtherLotteryIcon->DestroySelf(InUIManager);
	}

	if (UIP_EtherLotteryDetailPopup.IsValid())
	{
		UIP_EtherLotteryDetailPopup->DestroySelf(InUIManager);
	}

	SelectEtherLotterySlotClass<int32>::GetInstance().Unsubscribe(SelectEtherLotterySlotTicket);
	DeliveryOpenMailClass<int64, FB2ResponseOpenMailPtr>::GetInstance().Unsubscribe(DeliveryOpenMailTicket);
	DeliveryOpenMailErrorClass<int32, FB2ResponseOpenMailPtr>::GetInstance().Unsubscribe(DeliveryOpenMailErrorTicket);

	Super::DestroySelf(InUIManager);
}

void UB2UIEtherLottery::SetEtherLotteryInfo(int64 InMailID, int32 InEtherLotteryType)
{
	MailID = InMailID;

	B2EtherManager::GetInstance().GetEtherLotteryInfo(InEtherLotteryType, EtherStar, EtherTier);
	
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_MAILCONTENT), FString(TEXT("Lottery_SelectItemEtherType"))), EtherStar, EtherTier));
	}

	if (UI_EtherLotteryScroll.IsValid())
	{
		UI_EtherLotteryScroll->SetEtherLotteryInfo(EtherStar, EtherTier);
	}
}

void UB2UIEtherLottery::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_Close);
	GET_SLOT(UB2Button, BTN_Detail);
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2UIEtherLotteryScroll, UI_EtherLotteryScroll);
	GET_SLOT(UPanelWidget, CP_Ether);
	GET_SLOT(UPanelWidget, CP_NonSelect);
	GET_SLOT(UB2UIEtherLotterySlot, UI_EtherLotteryIcon);
	GET_SLOT(UTextBlock, X_TB_Empty);
	GET_SLOT(UB2UIEtherLotteryDetailPopup, UIP_EtherLotteryDetailPopup);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_ScrollTitle);
	GET_SLOT(UTextBlock, TB_Confirm);
}

void UB2UIEtherLottery::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherLottery::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_Detail, &UB2UIEtherLottery::OnClickBtnDetail);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIEtherLottery::OnClickBtnConfirm);

	SelectEtherLotterySlotTicket = SelectEtherLotterySlotClass<int32>::GetInstance().Subscribe([this](int32 InTypeID)
	{
		EtherTypeID = InTypeID;

		if (CP_Ether.IsValid())
		{
			CP_Ether->SetVisibility(ESlateVisibility::Visible);
			UI_EtherLotteryIcon->SetEtherLotterySlotInfo(InTypeID, EtherStar, EtherTier);
		}

		if (CP_NonSelect.IsValid() && CP_NonSelect->GetVisibility() != ESlateVisibility::Collapsed)
		{
			CP_NonSelect->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Confirm.IsValid())
		{
			BTN_Confirm->SetIsEnabled(true);
		}
	});

	DeliveryOpenMailTicket = DeliveryOpenMailClass<int64, FB2ResponseOpenMailPtr>::GetInstance().Subscribe(
		[this](int64 MailId, const FB2ResponseOpenMailPtr& MailPtr)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().MakeDirty();
		UB2UIEtherLotteryResultPopup* Popup =UB2UIManager::GetInstance()->OpenUI<UB2UIEtherLotteryResultPopup>(UIFName::EtherLotteryResult);
		if (Popup)
		{
			Popup->SetPopupInfo(MailPtr->aether);
		}
		OnClickBtnClose();
	});
	
	DeliveryOpenMailErrorTicket = DeliveryOpenMailErrorClass<int32, FB2ResponseOpenMailPtr>::GetInstance().Subscribe(
		[this](const int32 ErrorCode, const FB2ResponseOpenMailPtr MailPtr)
	{
		OnClickBtnClose();
	});
}

void UB2UIEtherLottery::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIEtherLottery::UpdateStaticText()
{
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CharacterSelect_SelectComplete"))));
	}
	
	if (TB_ScrollTitle.IsValid())
	{
		TB_ScrollTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_MAILCONTENT), FString(TEXT("Lottery_SelectItemText"))));
	}
	
	if (X_TB_Empty.IsValid())
	{
		X_TB_Empty->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_MAILCONTENT), FString(TEXT("Lottery_SelectItemEther"))));
	}
}

void UB2UIEtherLottery::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherLottery);
}

void UB2UIEtherLottery::OnClickBtnDetail()
{
	if (UIP_EtherLotteryDetailPopup.IsValid())
	{
		UIP_EtherLotteryDetailPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_EtherLotteryDetailPopup.Get());
		UIP_EtherLotteryDetailPopup->SetPopupInfo(EtherTypeID, EtherStar, EtherTier);
	}
}

void UB2UIEtherLottery::OnClickBtnConfirm()
{
	data_trader::Retailer::GetInstance().RequestOpenMail(MailID, 0, 0, EtherTypeID);
}