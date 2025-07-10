// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_DecompositionAllItems.h"
//#include "BladeII.h"

#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2UIFilterCheckBox.h"
#include "Event.h"
#include "B2UIDocHelper.h"

UB2LobbyUI_DecompositionAllItems::UB2LobbyUI_DecompositionAllItems(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2LobbyUI_DecompositionAllItems::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_DecompostionAllTitle);

	HB_Decomposition.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UHorizontalBox, HB_Decomposition[0], HB_Decomposition_1);
	GET_SLOT_BYNAME(UHorizontalBox, HB_Decomposition[1], HB_Decomposition_2);
	GET_SLOT_BYNAME(UHorizontalBox, HB_Decomposition[2], HB_Decomposition_3);
	IMG_Decomposition.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UImage, IMG_Decomposition[0], IMG_Decomposition_1);
	GET_SLOT_BYNAME(UImage, IMG_Decomposition[1], IMG_Decomposition_2);
	GET_SLOT_BYNAME(UImage, IMG_Decomposition[2], IMG_Decomposition_3);
	TB_DecompositionCount.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UTextBlock, TB_DecompositionCount[0], TB_DecompositionCount_1);
	GET_SLOT_BYNAME(UTextBlock, TB_DecompositionCount[1], TB_DecompositionCount_2);
	GET_SLOT_BYNAME(UTextBlock, TB_DecompositionCount[2], TB_DecompositionCount_3);
	GET_SLOT(UTextBlock, TB_DecompositionReceive);

	GET_SLOT(UTextBlock, TB_DecompostionAllNote);
	GET_SLOT(UTextBlock, TB_Decompostion);
	GET_SLOT(UTextBlock, TB_DecompostionItemCount);
	GET_SLOT(UTextBlock, TB_DecompostionItemNumber);
	GET_SLOT(UTextBlock, TB_DecompostionItemSelect);
	GET_SLOT(UTextBlock, TB_Reset);

	GET_SLOT(UB2Button, BTN_Decompostion);
	GET_SLOT(UB2Button, BTN_Reset);

	GET_SLOT(UB2Image, IMG_ClassPortrait);

	GET_SLOT(UB2UIFilterInventory, UIP_InventoryFilter);
}

void UB2LobbyUI_DecompositionAllItems::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	UpdateSubWidgets();
	BindDelegates();
}

void UB2LobbyUI_DecompositionAllItems::BindDelegates()
{
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_DecompositionAllItems::OnAnyItemFilterCBChanged);

	BIND_CLICK_FUNC_TO_BTN(BTN_Reset, &UB2LobbyUI_DecompositionAllItems::OnReset)
		BIND_CLICK_FUNC_TO_BTN(BTN_Decompostion, &UB2LobbyUI_DecompositionAllItems::OnDecompositionButtonClicked)
}
void UB2LobbyUI_DecompositionAllItems::OnReset()
{
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->ResetFilter();
}

void UB2LobbyUI_DecompositionAllItems::UpdateSubWidgets()
{
	if (UIP_InventoryFilter.IsValid())
	{
		//int32 MinGrade = 2;
		UIP_InventoryFilter->Init();
		UIP_InventoryFilter->SetMinGradeFilter();
		UIP_InventoryFilter->BindWarningPopup();
		UIP_InventoryFilter->SetVisibleEssenceCheckBox(ESlateVisibility::Collapsed);
		UIP_InventoryFilter->LoadFilterInfo(TEXT("DecompostionAllItems"));
	}

	UpdateStaticWidget();
	UpdateDecompositionObtainState();
	UpdateDecompositionButtonState();
}

void UB2LobbyUI_DecompositionAllItems::UpdateStaticWidget()
{
	if (TB_DecompostionAllTitle.IsValid())
		TB_DecompostionAllTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_DecompostionAll"))));
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->SetFilterTitle(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_DecompostionAll"))));
	if (TB_Reset.IsValid())
		TB_Reset->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Reset"))));

	if (TB_DecompostionAllNote.IsValid())
		TB_DecompostionAllNote->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_DecompostionAllNote"))));
	if (TB_Decompostion.IsValid())
		TB_Decompostion->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Decompostion"))));

	if (IMG_ClassPortrait.IsValid())
		IMG_ClassPortrait->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(ClassPortraitImage, UB2LobbyInventory::GetInventorySelectedPCClass()));

	if (TB_DecompostionItemNumber.IsValid())
		TB_DecompostionItemNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_CountUnit"))));
	if (TB_DecompostionItemSelect.IsValid())
		TB_DecompostionItemSelect->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Select"))));
	if (TB_DecompositionReceive.IsValid())
		TB_DecompositionReceive->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Receive"))));
}

void UB2LobbyUI_DecompositionAllItems::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2LobbyUI_DecompositionAllItems::OnCloseButtonClicked()
{
	CloseMe();
}

void UB2LobbyUI_DecompositionAllItems::OnDecompositionButtonClicked()
{
	TArray<FB2Item> AllTargetItems;
	if (!GetAllTargetItems(AllTargetItems))
		return;

	TArray<int64> AllTargetItemIDs;
	for (FB2Item& CurTarget : AllTargetItems)
	{
		AllTargetItemIDs.AddUnique(CurTarget.InstanceUID);
	}
	UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);

	LobbyReqDecompositionItemsClass<const TArray<int64>&, bool>::GetInstance().Signal(AllTargetItemIDs, false);

	if (UIP_InventoryFilter.IsValid())
	{
		UIP_InventoryFilter->SaveFilterInfo(TEXT("DecompostionAllItems"));
	}

	CloseMe();
}

void UB2LobbyUI_DecompositionAllItems::OnAnyItemFilterCBChanged(bool InCheck)
{
	UpdateDecompositionObtainState();
	UpdateDecompositionButtonState();
}

void UB2LobbyUI_DecompositionAllItems::UpdateDecompositionObtainState()
{
	int32 InItemCount = 0;
	TMultiMap<int32, int32> InDismantleItems;
	TArray<int32> InDismantleItemsKey;
	InDismantleItems.Empty();
	InDismantleItemsKey.Empty();

	GetAllTargetItemObtainDecomposition(InItemCount, InDismantleItems, InDismantleItemsKey);

	for (int32 i = 0; i < MAX_DECOMPOSITION_COUNT; ++i)
	{
		if (HB_Decomposition[i].IsValid())
		{
			HB_Decomposition[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UB2ItemInfo* ItemInfoTable = StaticFindItemInfo())
	{
		for (int32 i = 0; i < InDismantleItemsKey.Num(); ++i)
		{
			if (HB_Decomposition[i].IsValid())
			{
				HB_Decomposition[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			FSingleItemInfoData* const SingleItemInfoData = StaticFindItemInfo()->GetInfoData(InDismantleItemsKey[i]);
			if (SingleItemInfoData)
			{
				if (IMG_Decomposition[i].IsValid())
					IMG_Decomposition[i]->SetBrushFromMaterial(SingleItemInfoData->GetIconMaterial(ItemInfoTable));

				TArray<int32> DismantleItemsTemp;
				DismantleItemsTemp.Empty();
				InDismantleItems.MultiFind(InDismantleItemsKey[i], DismantleItemsTemp);

				int32 DismantlesAmount = 0;
				for (int32& Dis : DismantleItemsTemp)
				{
					DismantlesAmount += Dis;
				}

				if (TB_DecompositionCount[i].IsValid())
				{
					TB_DecompositionCount[i]->SetText(FText::AsNumber(DismantlesAmount));
				}
			}
		}
	}

	if (TB_DecompostionItemCount.IsValid())
	{
		TB_DecompostionItemCount->SetText(FText::AsNumber(InItemCount));
	}
	if (TB_DecompositionReceive.IsValid())
	{
		TB_DecompositionReceive->SetVisibility(InItemCount > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_DecompositionAllItems::UpdateDecompositionButtonState()
{
	if (BTN_Decompostion.IsValid())
	{
		bool IsEnabledState = false;
		TArray<FB2Item> AllTargetItems;
		if (GetAllTargetItems(AllTargetItems))
		{
			IsEnabledState = true;
		}
		BTN_Decompostion->SetIsEnabled(IsEnabledState);
	}
}

void UB2LobbyUI_DecompositionAllItems::GetAllTargetItemObtainDecomposition(int32& InItemCount, TMultiMap<int32, int32>& InDismantleItems, TArray<int32>& InDismantleItemsKey)
{
	TArray<FB2Item> AllTargetItems;

	if (GetAllTargetItems(AllTargetItems))
	{
		for (FB2Item& CurrItem : AllTargetItems)
		{
			FItemDismantleInfo DismantleItemInfo;
			BladeIIGameImpl::GetClientDataStore().GetItemDismantleInfoData(CurrItem.InventoryType, CurrItem.StarGrade, DismantleItemInfo);

			for (int32 i = 0; i < DismantleItemInfo.Matrial.Num(); ++i)
			{
				InDismantleItems.Add(DismantleItemInfo.Matrial[i], DismantleItemInfo.MatrialAmount[i]);
				InDismantleItemsKey.AddUnique(DismantleItemInfo.Matrial[i]);
			}
		}
	}

	InItemCount = AllTargetItems.Num();
}

bool UB2LobbyUI_DecompositionAllItems::GetAllTargetItems(TArray<FB2Item> &OutList)
{
	if (!UIP_InventoryFilter.IsValid())
		return false;

	UIP_InventoryFilter->GetAllTargetItems(OutList, UB2LobbyInventory::GetInventorySelectedPCClass());

	return OutList.Num() > 0;
}

void UB2LobbyUI_DecompositionAllItems::CloseMe()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_DecompositionAllItems);
}
