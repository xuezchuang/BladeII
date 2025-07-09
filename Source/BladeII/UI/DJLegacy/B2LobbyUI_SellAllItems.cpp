// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_SellAllItems.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2UIFilterCheckBox.h"
#include "Event.h"
#include "B2UIDocHelper.h"

UB2LobbyUI_SellAllItems::UB2LobbyUI_SellAllItems(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ }

void UB2LobbyUI_SellAllItems::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_SellAllTitle);
	GET_SLOT(UTextBlock, TB_GoldToAcquire);
	GET_SLOT(UTextBlock, TB_GoldReceive);
	
	GET_SLOT(UTextBlock, TB_SellAllNote);
	GET_SLOT(UTextBlock, TB_Sell);
	GET_SLOT(UTextBlock, TB_SellItemCount);
	GET_SLOT(UTextBlock, TB_SellItemNumber);
	GET_SLOT(UTextBlock, TB_SellItemSelect);
	GET_SLOT(UTextBlock, TB_Reset);

	GET_SLOT(UB2Button, BTN_Sell);
	GET_SLOT(UB2Button, BTN_Reset);

	GET_SLOT(UB2Image, IMG_ClassPortrait);

	GET_SLOT(UB2UIFilterInventory, UIP_InventoryFilter);
	
}

void UB2LobbyUI_SellAllItems::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	UpdateSubWidgets();
	BindDelegates();
}

void UB2LobbyUI_SellAllItems::BindDelegates()
{
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_SellAllItems::OnAnyItemFilterCBChanged);

	BIND_CLICK_FUNC_TO_BTN(BTN_Reset, &UB2LobbyUI_SellAllItems::OnReset)
	BIND_CLICK_FUNC_TO_BTN(BTN_Sell , &UB2LobbyUI_SellAllItems::OnSellButtonClicked)
}
void UB2LobbyUI_SellAllItems::OnReset()
{
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->ResetFilter();
}

void UB2LobbyUI_SellAllItems::UpdateSubWidgets()
{
	if (UIP_InventoryFilter.IsValid())
	{
		UIP_InventoryFilter->Init();
		UIP_InventoryFilter->BindWarningPopup();
		UIP_InventoryFilter->LoadFilterInfo(TEXT("SellAllItems"));
	}

	UpdateStaticWidget();
	UpdateGoldToAcquireText();
	UpdateSellButtonState();
}

void UB2LobbyUI_SellAllItems::UpdateStaticWidget()
{
	if (TB_SellAllTitle.IsValid())
		TB_SellAllTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAll"))));
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->SetFilterTitle(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAll"))));
	if(TB_Reset.IsValid())
		TB_Reset->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Reset"))));
	
	if (TB_SellAllNote.IsValid())
		TB_SellAllNote->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAllNote"))));
	if (TB_Sell.IsValid())
		TB_Sell->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Sell"))));

	if (IMG_ClassPortrait.IsValid())
		IMG_ClassPortrait->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(ClassPortraitImage, UB2LobbyInventory::GetInventorySelectedPCClass()));

	if (TB_SellItemNumber.IsValid())
		TB_SellItemNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_CountUnit"))));
	if (TB_SellItemSelect.IsValid())
		TB_SellItemSelect->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Select"))));
	if (TB_GoldReceive.IsValid())
		TB_GoldReceive->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Receive"))));
}

void UB2LobbyUI_SellAllItems::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2LobbyUI_SellAllItems::OnCloseButtonClicked()
{
	CloseMe();
}

void UB2LobbyUI_SellAllItems::OnSellButtonClicked()
{
	// 본격 동작
	TArray<FB2Item> AllTargetItems;
	if (!GetAllTargetItems(AllTargetItems))
		return;
	
	TArray<int64> AllTargetItemIDs;
	for (FB2Item& CurTarget : AllTargetItems)
	{
		AllTargetItemIDs.AddUnique(CurTarget.InstanceUID);

		// 인벤토리 탭 확인 없이 보관함에서 제거될 수 있는 것인 만큼 신규 획득 표시 리스트에서도 제거. 보다 확실하게는 응답이 왔을 때 해야 되긴 함. 그렇게까지 철저하게 해야 될 필요까진 없을 거 같고..
		//BladeIIGameImpl::GetRedDotManager().RemoveNewItem(CurTarget);
	}
	UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);
	// 매 response 마다 아이콘들 업데이트하면 좀 느리겠는데.. ㄷㄷ
	LobbyReqSellItemsClass<const TArray<int64>&, bool>::GetInstance().Signal(AllTargetItemIDs, false);

	if (UIP_InventoryFilter.IsValid())
	{
		UIP_InventoryFilter->SaveFilterInfo(TEXT("SellAllItems"));
	}

	CloseMe();
}

void UB2LobbyUI_SellAllItems::OnAnyItemFilterCBChanged(bool InCheck)
{ 
	UpdateGoldToAcquireText();
	UpdateSellButtonState();
}

void UB2LobbyUI_SellAllItems::UpdateGoldToAcquireText()
{// 현재 filter 선택에 따라 아이템들 취합해서 액수 계산
	int32 SellPrice = 0, SellItemCount = 0;
	GetAllTargetItemSellingPrice(SellItemCount, SellPrice);

	if (TB_GoldToAcquire.IsValid())
	{
		TB_GoldToAcquire->SetText(FText::AsNumber(SellPrice));
	}
	if (TB_SellItemCount.IsValid())
	{
		TB_SellItemCount->SetText(FText::AsNumber(SellItemCount));
	}
}

void UB2LobbyUI_SellAllItems::UpdateSellButtonState()
{
	if (BTN_Sell.IsValid())
	{
		bool IsEnabledState = false;
		TArray<FB2Item> AllTargetItems;
		if (GetAllTargetItems(AllTargetItems))
		{
			IsEnabledState = true;
		}
		BTN_Sell->SetIsEnabled(IsEnabledState);
	}
}

void UB2LobbyUI_SellAllItems::GetAllTargetItemSellingPrice(int32& ItemCount, int32& ItemPrice)
{
	int32 RetPrice = 0;
	TArray<FB2Item> AllTargetItems;
	if (GetAllTargetItems(AllTargetItems))
	{
		for (FB2Item& CurrItem : AllTargetItems)
		{
			RetPrice += CurrItem.SellingPrice;
		}
	}
	ItemCount = AllTargetItems.Num();
	ItemPrice = RetPrice;
}

bool UB2LobbyUI_SellAllItems::GetAllTargetItems(TArray<FB2Item> &OutList)
{
	if (!UIP_InventoryFilter.IsValid())
		return false;

	UIP_InventoryFilter->GetAllTargetItems(OutList, UB2LobbyInventory::GetInventorySelectedPCClass());

	return OutList.Num() > 0;
}

void UB2LobbyUI_SellAllItems::CloseMe()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellAllItems);
}