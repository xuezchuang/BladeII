// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEtherExtendInventory.h"
#include "B2UIManager.h"
#include "B2EtherManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2UIEtherInfoNotify.h"



void UB2UIEtherExtendInventory::CacheAssets()
{
	GET_SLOT(UTextBlock, X_TB_Title);
	GET_SLOT(UTextBlock, X_TB_CurrentCount);
	GET_SLOT(UTextBlock, X_TB_PostExtendCount);
	GET_SLOT(UTextBlock, X_TB_MaxSlotCountInfo);
	GET_SLOT(UTextBlock, X_TB_ExtendBtn);
	GET_SLOT(UTextBlock, X_TB_ExtendCost);
	GET_SLOT(UTextBlock, X_TB_MaxExtendBtn);

	GET_SLOT(UButton, X_BTN_Close);
	GET_SLOT(UButton, X_BTN_Extend);
	GET_SLOT(UButton, X_BTN_MaxExtend);
	GET_SLOT(UButton, X_BTN_Down);
	GET_SLOT(UButton, X_BTN_Up);

	GET_SLOT(USlider, S_CashExtendSlider);
}

void UB2UIEtherExtendInventory::UpdateStaticText()
{
	int32 CurrentCount = B2EtherManager::GetInstance().GetEtherInventoryCount();
	int32 ExtendCount = BladeIIGameImpl::GetClientDataStore().GetEtherInvenExtendCount();
	int32 MaxCount = BladeIIGameImpl::GetClientDataStore().GetEtherInvenMaxCount();

	ExtendCost = BladeIIGameImpl::GetClientDataStore().GetEtherInvenExtendCost();

	SlotSliderLogic.SetData(CurrentCount, ExtendCount, MaxCount);

	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Inventory_Expansion"))));
	}
	if (X_TB_CurrentCount.IsValid())
	{
		X_TB_CurrentCount->SetText(FText::AsNumber(CurrentCount));
	}
	if (X_TB_MaxSlotCountInfo.IsValid())
	{
		X_TB_MaxSlotCountInfo->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenExtend_MaxSlotCountInfo"))), MaxCount));
	}
	if (X_TB_ExtendBtn.IsValid())
	{
		X_TB_ExtendBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenExtend_SingleExtendBtn"))));
	}
	if (X_TB_MaxExtendBtn.IsValid())
	{
		X_TB_MaxExtendBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Maximum"))));
	}

	UpdateWidgets(0);
}

void UB2UIEtherExtendInventory::BindDelegates()
{
	if (X_BTN_Close.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherExtendInventory::OnClickClose);
	}

	if (X_BTN_Extend.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Extend, &UB2UIEtherExtendInventory::OnClickExtend);
	}
	
	if (X_BTN_MaxExtend.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_MaxExtend, &UB2UIEtherExtendInventory::OnClickMaxExtend);
	}

	if (X_BTN_Down.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Down, &UB2UIEtherExtendInventory::OnClickBtnDown);
	}

	if (X_BTN_Up.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Up, &UB2UIEtherExtendInventory::OnClickBtnUp);
	}

	if (S_CashExtendSlider.IsValid())
		S_CashExtendSlider->OnValueChanged.AddDynamic(this, &UB2UIEtherExtendInventory::OnChangedExtendCount);

	ExtendInventoryEventID = DeliveryExtendEtherInventoryClass<FB2ExtendEtherInventoryPtr>::GetInstance().Subscribe([this](const FB2ExtendEtherInventoryPtr EtherPtr)
	{
		BladeIIGameImpl::GetClientDataStore().SetAccountInfo(EtherPtr->account);
		if (auto* DocUser = UB2UIDocHelper::GetDocUser())
		{
			DocUser->SetGem(EtherPtr->account->cash);
		}

		B2EtherManager::GetInstance().SetEtherInventoryCount(EtherPtr->current_slot_count);

		UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
		if (Popup)
		{
			Popup->SetInfoNotify(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Inventory_Expansion_Note_1"))));
		}

		this->OnClickClose();
	}
	);
}

void UB2UIEtherExtendInventory::DestroySelf(class UB2UIManager* InUIManager)
{
	DeliveryExtendEtherInventoryClass<FB2ExtendEtherInventoryPtr>::GetInstance().Unsubscribe(ExtendInventoryEventID);
}

void UB2UIEtherExtendInventory::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherExtendInventory);
}

void UB2UIEtherExtendInventory::OnClickExtend()
{
	data_trader::Retailer::GetInstance().RequestExtendAetherInventory(GetRealExtendCount());
}

void UB2UIEtherExtendInventory::OnClickMaxExtend()
{
	UpdateWidgets(SlotSliderLogic.GetMaxExtendCount());
}

void UB2UIEtherExtendInventory::OnClickBtnDown()
{
	UpdateWidgets(GetSliderExtendCount() - 1);
}

void UB2UIEtherExtendInventory::OnClickBtnUp()
{
	UpdateWidgets(GetSliderExtendCount() + 1);
}

void UB2UIEtherExtendInventory::OnChangedExtendCount(float ChangeValue)
{
	UpdateWidgets(GetSliderExtendCount());
}

int32 UB2UIEtherExtendInventory::GetSliderExtendCount()
{
	if (S_CashExtendSlider.IsValid())
		return SlotSliderLogic.SliderValueToExtendCount(S_CashExtendSlider->GetValue());

	return 0;
}

int32 UB2UIEtherExtendInventory::GetRealExtendCount()
{
	if (S_CashExtendSlider.IsValid())
		return SlotSliderLogic.GetRealExtendCount(S_CashExtendSlider->GetValue());

	return 0;
}


void UB2UIEtherExtendInventory::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIEtherExtendInventory::UpdateWidgets(int32 NewExtendCount)
{
	int32 PostExtendSlotCount = SlotSliderLogic.GetSlotCount(NewExtendCount);

	if (S_CashExtendSlider.IsValid())
	{
		S_CashExtendSlider->SetStepSize(SlotSliderLogic.GetSliderStepSize());
		S_CashExtendSlider->SetValue(SlotSliderLogic.ExtendCountToSliderValue(NewExtendCount));
	}

	if (X_TB_PostExtendCount.IsValid())
	{
		X_TB_PostExtendCount->SetText(FText::AsNumber(PostExtendSlotCount));
	}
	
	if (X_TB_ExtendCost.IsValid())
	{
		int32 RealExtendCount = GetRealExtendCount() * ExtendCost;
		
		X_TB_ExtendCost->SetText(FText::AsNumber(RealExtendCount));
		
		if (BladeIIGameImpl::GetClientDataStore().GetGemAmount() < RealExtendCount)
		{
			X_TB_ExtendCost->SetColorAndOpacity(FLinearColor::Red);
		}
		else
		{
			X_TB_ExtendCost->SetColorAndOpacity(FLinearColor::White);
		}
	}

	if (X_BTN_Extend.IsValid())
	{
		X_BTN_Extend->SetIsEnabled(!SlotSliderLogic.IsAlreadyMaxSlotCount()); 
	}
	if (X_BTN_MaxExtend.IsValid())
	{
		X_BTN_MaxExtend->SetIsEnabled(PostExtendSlotCount < SlotSliderLogic.GetMaxAllowedSlotCount()); 
	}
}
