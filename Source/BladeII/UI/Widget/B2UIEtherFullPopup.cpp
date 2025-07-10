// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherFullPopup.h"
#include "BladeIIUtil.h"
#include "BladeIILocText.h"
#include "B2EtherManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"
#include "Retailer.h"


void UB2UIEtherFullPopup::SetEtherFullPopupInfo(int InStoredCount, int MaxCount)
{
	if (TB_StoredItemCount.IsValid())
	{
		TB_StoredItemCount->SetText(FText::AsNumber(InStoredCount));
	}

	if (TB_ContainerItemCount.IsValid())
	{
		TB_ContainerItemCount->SetText(FText::AsNumber(MaxCount));
	}

	int32 InventoryMaxCount = BladeIIGameImpl::GetClientDataStore().GetEtherInvenMaxCount();

	if (InventoryMaxCount <= MaxCount && BTN_ExtensionInven.IsValid())
	{
		BTN_ExtensionInven->SetIsEnabled(false);
	}
}

void UB2UIEtherFullPopup::CacheAssets()
{
	GET_SLOT(UButton, BTN_ExtensionInven);
	GET_SLOT(UButton, BTN_MoveInven);
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT(UTextBlock, TB_ExtensionInven);
	GET_SLOT(UTextBlock, TB_MoveInven);
	GET_SLOT(UTextBlock, TB_StoredItemCount);
	GET_SLOT(UTextBlock, TB_ContainerItemCount);
	GET_SLOT(UTextBlock, TB_Content);
	GET_SLOT(UTextBlock, TB_Title);
}

void UB2UIEtherFullPopup::UpdateStaticText()
{
	if (TB_ExtensionInven.IsValid())
	{
		TB_ExtensionInven->SetText(BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_Inventory_Extend")));
	}
	
	if (TB_MoveInven.IsValid())
	{
		TB_MoveInven->SetText(BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_Inventory_Move")));
	}

	if (TB_Content.IsValid())
	{
		TB_Content->SetText(BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_Inventory_Full")));
	}

	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageStartErrorTitle")));
	}
}

void UB2UIEtherFullPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ExtensionInven, &UB2UIEtherFullPopup::OnClickBtnExtensionEtherInven);
	BIND_CLICK_FUNC_TO_BTN(BTN_MoveInven, &UB2UIEtherFullPopup::OnClickBtnMoveEther);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIEtherFullPopup::OnClickBtnClose);
}

void UB2UIEtherFullPopup::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIEtherFullPopup::OnClickBtnExtensionEtherInven()
{
	OnClickBtnClose();

	UB2UIManager::GetInstance()->OpenUI(UIFName::EtherExtendInventory);
}

void UB2UIEtherFullPopup::OnClickBtnMoveEther()
{
	if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::StageResult ||
		UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::CharacterSelectPage)
	{
		MoveEtherDelegate.ExecuteIfBound();
	}
	else
	{
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::EtherMain);
	}

	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherFullPopup);
}

void UB2UIEtherFullPopup::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::EtherFullPopup);
}
