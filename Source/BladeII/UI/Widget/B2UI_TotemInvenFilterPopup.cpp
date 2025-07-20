// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UI_TotemInvenFilterPopup.h"

#include "B2UIManager.h"
#include "B2LobbyInventory.h"
#include "B2UI_TotemInvenFilter.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"

void UB2UI_TotemInvenFilterPopup::InitTotemInvenFilterPopup()
{
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2UI_TotemInvenFilterPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock,				TB_PopupTitle);

	GET_SLOT(UTextBlock,				TB_SellItemCount);
	GET_SLOT(UTextBlock,				TB_SellItemNumber);
	GET_SLOT(UTextBlock,				TB_SellItemSelect);
	GET_SLOT(UTextBlock,				TB_GoldToAcquire);
	GET_SLOT(UTextBlock,				TB_GoldReceive);

	GET_SLOT(UTextBlock,				TB_PopupNote);

	GET_SLOT(UB2Button,					BTN_Confirm);
	GET_SLOT(UTextBlock,				TB_Confirm);

	GET_SLOT(UB2Button,					BTN_Reset);
	GET_SLOT(UTextBlock,				TB_Reset);

	GET_SLOT(UB2UI_TotemInvenFilter,	UIP_TotemInventoryFilter);
	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->Init();
		UIP_TotemInventoryFilter->BindWarningPopup();
	}
}

void UB2UI_TotemInvenFilterPopup::BindDelegates()
{
	Super::BindDelegates();

	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->OnCheckStateChanged.AddDynamic(this, &UB2UI_TotemInvenFilterPopup::OnAnyTotemFilterCBChanged);
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UI_TotemInvenFilterPopup::OnClickBTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Reset, &UB2UI_TotemInvenFilterPopup::OnClickBTN_Reset);
}

void UB2UI_TotemInvenFilterPopup::UpdateStaticText()
{
	if (TB_SellItemNumber.IsValid())
	{
		TB_SellItemNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_CountUnit"))));
	}
	if (TB_SellItemSelect.IsValid())
	{
		TB_SellItemSelect->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Select"))));
	}
	if (TB_GoldReceive.IsValid())
	{
		TB_GoldReceive->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Receive"))));
	}
	if (TB_Reset.IsValid())
	{
		TB_Reset->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Reset"))));
	}
}

void UB2UI_TotemInvenFilterPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemInvenFilterPopup::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

bool UB2UI_TotemInvenFilterPopup::GetAllTargetTotems(TArray<FB2Totem> &OutList)
{
	if (UIP_TotemInventoryFilter.IsValid() == false)
	{
		return false;
	}

	UIP_TotemInventoryFilter->GetAllTargetTotems(OutList);
	return (OutList.Num() > 0);
}

void UB2UI_TotemInvenFilterPopup::UpdateConfirmButtonState()
{
	if (BTN_Confirm.IsValid() == false)
	{
		return;
	}

	TArray<FB2Totem> AllTargetTotems;
	bool IsEnabledState = GetAllTargetTotems(AllTargetTotems);

	BTN_Confirm->SetIsEnabled(IsEnabledState);
}

void UB2UI_TotemInvenFilterPopup::OnCloseButtonClicked()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UI_TotemInvenFilterPopup::OnAnyTotemFilterCBChanged(bool InCheck)
{
	UpdateConfirmButtonState();
	UpdateAcquireText();
}

void UB2UI_TotemInvenFilterPopup::OnClickBTN_Reset()
{
	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->ResetFilter();
	}
}

///////////////////////////////////////////////////////////////////////////////////////

void UB2UI_TotemInvenSellAll::InitTotemInvenFilterPopup()
{
	Super::InitTotemInvenFilterPopup();

	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->LoadFilterInfo(TEXT("SellAllTotems"));
	}
}

void UB2UI_TotemInvenSellAll::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UI_TotemInvenSellAll::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_PopupTitle.IsValid())
	{
		TB_PopupTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAll"))));
	}
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Sell"))));
	}
	if (TB_PopupNote.IsValid())
	{
		TB_PopupNote->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_SellAllNote"))));
	}
}

void UB2UI_TotemInvenSellAll::OnClickBTN_Confirm()
{
	TArray<FB2Totem> AllTargetItems;
	if (!GetAllTargetTotems(AllTargetItems))
		return;

	TArray<int64> AllTargetItemIDs;
	for (FB2Totem& CurTarget : AllTargetItems)
	{
		AllTargetItemIDs.AddUnique(CurTarget.ID);
	}

	UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);

	LobbyReqSellItemsClass<const TArray<int64>&, bool>::GetInstance().Signal(AllTargetItemIDs, true);

	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->SaveFilterInfo(TEXT("SellAllTotems"));
	}

	OnCloseButtonClicked();
}

void UB2UI_TotemInvenSellAll::UpdateAcquireText()
{// 泅犁 filter 急琶俊 蝶扼 酒捞袍甸 秒钦秦辑 咀荐 拌魂
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

void UB2UI_TotemInvenSellAll::GetAllTargetItemSellingPrice(int32& ItemCount, int32& ItemPrice)
{
	int32 RetPrice = 0;
	TArray<FB2Totem> AllTargetItems;
	if (GetAllTargetTotems(AllTargetItems))
	{
		for (FB2Totem& CurrItem : AllTargetItems)
		{
			RetPrice += BladeIIGameImpl::GetTotemDataStore().GetTotemSellPrice(CurrItem);
		}
	}
	ItemCount = AllTargetItems.Num();
	ItemPrice = RetPrice;
}
///////////////////////////////////////////////////////////////////////////////////////

void UB2UI_TotemInvenDecompositionAll::InitTotemInvenFilterPopup()
{
	Super::InitTotemInvenFilterPopup();

	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->LoadFilterInfo(TEXT("DecompositionAllTotems"));
	}
}

void UB2UI_TotemInvenDecompositionAll::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UI_TotemInvenDecompositionAll::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_PopupTitle.IsValid())
	{
		TB_PopupTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_DecompostionAll"))));
	}
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Decompostion"))));
	}
	if (TB_PopupNote.IsValid())
	{
		TB_PopupNote->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_DecompostionAllNote"))));
	}
}

void UB2UI_TotemInvenDecompositionAll::OnClickBTN_Confirm()
{
	TArray<FB2Totem> AllTargetItems;
	if (!GetAllTargetTotems(AllTargetItems))
		return;

	TArray<int64> AllTargetItemIDs;
	for (FB2Totem& CurTarget : AllTargetItems)
	{
		AllTargetItemIDs.AddUnique(CurTarget.ID);
	}

	UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);

	LobbyReqDecompositionItemsClass<const TArray<int64>&, bool>::GetInstance().Signal(AllTargetItemIDs, true);

	if (UIP_TotemInventoryFilter.IsValid())
	{
		UIP_TotemInventoryFilter->SaveFilterInfo(TEXT("DecompositionAllTotems"));
	}

	OnCloseButtonClicked();
}

void UB2UI_TotemInvenDecompositionAll::UpdateAcquireText()
{// 泅犁 filter 急琶俊 蝶扼 酒捞袍甸 秒钦秦辑 咀荐 拌魂
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

void UB2UI_TotemInvenDecompositionAll::GetAllTargetItemSellingPrice(int32& ItemCount, int32& ItemPrice)
{
	int32 RetPrice = 0;
	TArray<FB2Totem> AllTargetItems;
	if (GetAllTargetTotems(AllTargetItems))
	{
		for (FB2Totem& CurrItem : AllTargetItems)
		{
			RetPrice += BladeIIGameImpl::GetTotemDataStore().GetDismantlePieceCount(CurrItem);
		}
	}
	ItemCount = AllTargetItems.Num();
	ItemPrice = RetPrice;
}