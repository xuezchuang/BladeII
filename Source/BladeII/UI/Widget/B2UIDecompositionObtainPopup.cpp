// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDecompositionObtainPopup.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2LobbyInventory.h"
#include "B2UIDocHelper.h"


void UB2UIDecompositionObtainPopup::CacheAssets()
{
	GET_SLOT(UTextBlock, X_TB_Title);
	GET_SLOT(UTextBlock, X_TB_ObtainDesc);

	X_VB_Obtain.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UVerticalBox, X_VB_Obtain[0], X_VB_Obtain_1);
	GET_SLOT_BYNAME(UVerticalBox, X_VB_Obtain[1], X_VB_Obtain_2);
	GET_SLOT_BYNAME(UVerticalBox, X_VB_Obtain[2], X_VB_Obtain_3);

	X_IMG_Obtain.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UImage, X_IMG_Obtain[0], X_IMG_Obtain_1);
	GET_SLOT_BYNAME(UImage, X_IMG_Obtain[1], X_IMG_Obtain_2);
	GET_SLOT_BYNAME(UImage, X_IMG_Obtain[2], X_IMG_Obtain_3);

	X_TB_ObtainCount.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UTextBlock, X_TB_ObtainCount[0], X_TB_ObtainCount_1);
	GET_SLOT_BYNAME(UTextBlock, X_TB_ObtainCount[1], X_TB_ObtainCount_2);
	GET_SLOT_BYNAME(UTextBlock, X_TB_ObtainCount[2], X_TB_ObtainCount_3);

	GET_SLOT(UB2Button, X_BTN_Yes);
	GET_SLOT(UTextBlock, X_TB_Btn);
}

void UB2UIDecompositionObtainPopup::UpdateStaticText()
{
	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_DecompostionResult")));
	}
	if (X_TB_ObtainDesc.IsValid())
	{
		X_TB_ObtainDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_DecompostionResultDesc")));
	}
	if (X_TB_Btn.IsValid())
	{
		X_TB_Btn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Confirm")));
	}
}

void UB2UIDecompositionObtainPopup::BindDelegates()
{
	if (X_BTN_Yes.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Yes, &UB2UIDecompositionObtainPopup::OnClickClose)
	}
}

void UB2UIDecompositionObtainPopup::SetEtherPieceInfo(int32 ObtainAmount)
{
	for (int i = 0; i < MAX_DECOMPOSITION_COUNT; ++i)
	{
		if (X_VB_Obtain[i].IsValid())
		{
			X_VB_Obtain[i]->SetVisibility(i == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	if (UB2ItemInfo* ItemInfoTable = StaticFindItemInfo())
	{
		FSingleItemInfoData* const SingleItemInfoData = StaticFindItemInfo()->GetInfoData(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE);
		if (SingleItemInfoData)
		{
			if (X_IMG_Obtain[0].IsValid())
				X_IMG_Obtain[0]->SetBrushFromMaterial(SingleItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}

	if (X_TB_ObtainCount[0].IsValid())
	{
		X_TB_ObtainCount[0]->SetText(FText::AsNumber(ObtainAmount));
	}
}

void UB2UIDecompositionObtainPopup::SetEquipItemInfo(TArray<int32> ObtainItemType, TArray<int32> ObtainAmount)
{
	for (int i = 0; i < MAX_DECOMPOSITION_COUNT; ++i)
	{
		if (X_VB_Obtain[i].IsValid())
		{
			X_VB_Obtain[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UB2ItemInfo* ItemInfoTable = StaticFindItemInfo())
	{
		for (int32 i = 0; i < ObtainItemType.Num(); ++i)
		{
			if (X_VB_Obtain[i].IsValid())
			{
				X_VB_Obtain[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			FSingleItemInfoData* const SingleItemInfoData = StaticFindItemInfo()->GetInfoData(ObtainItemType[i]);
			if (SingleItemInfoData)
			{
				if (X_IMG_Obtain[i].IsValid())
					X_IMG_Obtain[i]->SetBrushFromMaterial(SingleItemInfoData->GetIconMaterial(ItemInfoTable));

				if (X_TB_ObtainCount[i].IsValid())
				{
					X_TB_ObtainCount[i]->SetText(FText::AsNumber(ObtainAmount[i]));
				}
			}
		}
	}
}

void UB2UIDecompositionObtainPopup::OnClickClose()
{
	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
	{
		DocUser->SetEtherPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE));
	}

	UB2UIManager::GetInstance()->CloseUI(UIFName::DecompositionObtainPopup);
}

void UB2UIDecompositionObtainPopup::CloseWidgetDelegate()
{
	OnClickClose();
}
