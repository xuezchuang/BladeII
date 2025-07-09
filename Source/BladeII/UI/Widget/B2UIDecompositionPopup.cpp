// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDecompositionPopup.h"
#include "BladeIIUtil.h"
#include "B2ItemInfo.h"
#include "B2UIManager.h"
#include "CommonStruct.h"
#include "B2UIDecompositionObtainPopup.h"


void UB2UIDecompositionPopup::SetEtherInfo(const int32 InCount, const FOnClickPositive& InPositiveCallBack)
{
	PositiveCallBack = InPositiveCallBack;

	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Decomposition_Title"))));
	}
	if (X_TB_Description.IsValid())
	{
		X_TB_Description->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Decomposition_Note_1"))));
	}

	for (int i = 0; i < MAX_DECOMPOSITION_COUNT; ++i)
	{
		if (X_VB_Decomposition[i].IsValid())
		{
			X_VB_Decomposition[i]->SetVisibility(i == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	if (UB2ItemInfo* ItemInfoTable = StaticFindItemInfo())
	{
		FSingleItemInfoData* const SingleItemInfoData = StaticFindItemInfo()->GetInfoData(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE);
		if (SingleItemInfoData)
		{
			if (X_IMG_Decomposition[0].IsValid())
				X_IMG_Decomposition[0]->SetBrushFromMaterial(SingleItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}

	if (X_TB_Count[0].IsValid())
	{
		X_TB_Count[0]->SetText(FText::AsNumber(InCount));
	}
}

void UB2UIDecompositionPopup::SetEquipItemInfo(TArray<int32> ItemType, TArray<int32> ItemCount, const FOnClickPositive& InPositiveCallBack)
{
	PositiveCallBack = InPositiveCallBack;

	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("EquipItem_Decomposition_Title"))));
	}
	if (X_TB_Description.IsValid())
	{
		X_TB_Description->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("EquipItem_Decomposition_Note_1"))));
	}

	for (int i = 0; i < MAX_DECOMPOSITION_COUNT; ++i)
	{
		if (X_VB_Decomposition[i].IsValid())
		{
			X_VB_Decomposition[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UB2ItemInfo* ItemInfoTable = StaticFindItemInfo())
	{
		for (int32 i = 0; i < ItemType.Num(); ++i)
		{
			if (X_VB_Decomposition[i].IsValid())
			{
				X_VB_Decomposition[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			FSingleItemInfoData* const SingleItemInfoData = StaticFindItemInfo()->GetInfoData(ItemType[i]);
			if (SingleItemInfoData)
			{
				if (X_IMG_Decomposition[i].IsValid())
					X_IMG_Decomposition[i]->SetBrushFromMaterial(SingleItemInfoData->GetIconMaterial(ItemInfoTable));

				if (X_TB_Count[i].IsValid())
				{
					X_TB_Count[i]->SetText(FText::AsNumber(ItemCount[i]));
				}
			}
		}
	}
}

void UB2UIDecompositionPopup::SetTotemInfo(const int32 InCount, const FOnClickPositive& InPositiveCallBack)
{
	PositiveCallBack = InPositiveCallBack;

	if (X_TB_Title.IsValid())
	{
		X_TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("EquipItem_Decomposition_Title"))));
	}
	if (X_TB_Description.IsValid())
	{
		X_TB_Description->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("EquipItem_Decomposition_Note_1"))));
	}

	for (int i = 0; i < MAX_DECOMPOSITION_COUNT; ++i)
	{
		if (X_VB_Decomposition[i].IsValid())
		{
			X_VB_Decomposition[i]->SetVisibility(i == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	if (UB2ItemInfo* ItemInfoTable = StaticFindItemInfo())
	{
		FSingleItemInfoData* const SingleItemInfoData = StaticFindItemInfo()->GetInfoData(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE);
		if (SingleItemInfoData)
		{
			if (X_IMG_Decomposition[0].IsValid())
				X_IMG_Decomposition[0]->SetBrushFromMaterial(SingleItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}

	if (X_TB_Count[0].IsValid())
	{
		X_TB_Count[0]->SetText(FText::AsNumber(InCount));
	}
}

void UB2UIDecompositionPopup::OnClickPositive()
{
	PositiveCallBack.ExecuteIfBound();
}

void UB2UIDecompositionPopup::CloseWidgetDelegate()
{
	OnClickNegative();
}

void UB2UIDecompositionPopup::OnClickNegative()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::DecompositionPopup);
}

void UB2UIDecompositionPopup::CacheAssets()
{
	GET_SLOT(UTextBlock, X_TB_Title);
	GET_SLOT(UTextBlock, X_TB_Description);

	X_VB_Decomposition.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UVerticalBox, X_VB_Decomposition[0], X_VB_Decomposition_1);
	GET_SLOT_BYNAME(UVerticalBox, X_VB_Decomposition[1], X_VB_Decomposition_2);
	GET_SLOT_BYNAME(UVerticalBox, X_VB_Decomposition[2], X_VB_Decomposition_3);

	X_IMG_Decomposition.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UImage, X_IMG_Decomposition[0], X_IMG_Decomposition_1);
	GET_SLOT_BYNAME(UImage, X_IMG_Decomposition[1], X_IMG_Decomposition_2);
	GET_SLOT_BYNAME(UImage, X_IMG_Decomposition[2], X_IMG_Decomposition_3);

	X_TB_Count.SetNum(MAX_DECOMPOSITION_COUNT);
	GET_SLOT_BYNAME(UTextBlock, X_TB_Count[0], X_TB_Count_1);
	GET_SLOT_BYNAME(UTextBlock, X_TB_Count[1], X_TB_Count_2);
	GET_SLOT_BYNAME(UTextBlock, X_TB_Count[2], X_TB_Count_3);

	GET_SLOT(UB2Button, X_BTN_Negative);
	GET_SLOT(UB2Button, X_BTN_Positive);
	GET_SLOT(UTextBlock, X_TB_BtnNegative);
	GET_SLOT(UTextBlock, X_TB_BtnPositive);
}

void UB2UIDecompositionPopup::BindDelegates()
{
	if (X_BTN_Negative.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Negative, &UB2UIDecompositionPopup::OnClickNegative);
	}

	if (X_BTN_Positive.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Positive, &UB2UIDecompositionPopup::OnClickPositive);
	}

	SubscribeEvents();
}

void UB2UIDecompositionPopup::UpdateStaticText()
{
	if (X_TB_BtnNegative.IsValid())
	{
		X_TB_BtnNegative->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_No"))));
	}

	if (X_TB_BtnPositive.IsValid())
	{
		X_TB_BtnPositive->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Yes"))));
	}
}

void UB2UIDecompositionPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	PositiveCallBack.Unbind();

	UnsubscribeEvents();
}

void UB2UIDecompositionPopup::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryDecompositionEtherClass<FB2DecompositionEtherPtr>::GetInstance().Subscribe2(
		[this](FB2DecompositionEtherPtr EtherPtr)
	{
		this->OnClickNegative();

		RefreshScrollUIClass<>::GetInstance().Signal();

		UB2UIDecompositionObtainPopup* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIDecompositionObtainPopup>(UIFName::DecompositionObtainPopup);
		Popup->SetEtherPieceInfo(EtherPtr->obtain_aether_piece_count);
	}
	));

	Issues.Add(DeliveryDismantleItemsClass<FB2DismantleItems>::GetInstance().Subscribe2(
		[this](FB2DismantleItems DismanttleItemsPtr)
	{
		this->OnClickNegative();

		RefreshScrollUIClass<>::GetInstance().Signal();
	}
	));
}

void UB2UIDecompositionPopup::UnsubscribeEvents()
{
	Issues.Empty();
}
