// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIPAutoClearResultRow.h"
#include "B2UISlotItem.h"
#include "BladeIILocText.h"
#include "B2Inventory.h"



void UB2UIPAutoClearResultRow::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (TWeakObjectPtr<UB2UISlotItem> Elem : ItemSlots)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}
}

void UB2UIPAutoClearResultRow::SetInfo(int RepeatCount, int TotalGold, const TArray<FB2Item>& ItemInfos)
{
	for (TWeakObjectPtr<UB2UISlotItem> Elem : ItemSlots)
	{
		if (Elem.IsValid())
		{
			Elem->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	int CurRow = 0;
	int CurColumn = 0;

	RepeatCount += 1;

	if (TB_Count.IsValid())
	{
		TB_Count->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordDeadCount")), FText::AsNumber(RepeatCount)));
	}

	if (TB_GoldCount.IsValid())
	{
		TB_GoldCount->SetText(FText::AsNumber(TotalGold));
	}

	for (int i = 0; i < ItemInfos.Num(); i++)
	{
		UB2UISlotItem* NewItem;
		if (ItemSlots.IsValidIndex(i))
		{
			NewItem = ItemSlots[i].Get();
			NewItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			NewItem = CreateWidget<UB2UISlotItem>(GetWorld(), ItemSlotClass);
			ItemSlots.Add(NewItem);
		}
		
		if (UG_Grid.IsValid())
		{
			UUniformGridSlot* NewItemSlot = UG_Grid->AddChildToUniformGrid(NewItem);
			NewItemSlot->SetColumn(CurColumn);
			NewItemSlot->SetRow(CurRow);
		}

		NewItem->Init();
		NewItem->SetBTNArea(false);
		IsEtherItem(ItemInfos[i]) ? NewItem->BindEther(ItemInfos[i]) : NewItem->BindDoc(ItemInfos[i]);
		bool bInvisibleStar = IsGoldItem(ItemInfos[i]) || IsBossPieceItem(ItemInfos[i]) || IsMaterialItem(ItemInfos[i]);
		NewItem->SetVisibleStar(bInvisibleStar ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		NewItem->SetVisibleStageInfoItemIcon(ESlateVisibility::Collapsed, false);

		CurColumn++;
		if (CurColumn >= MaxColumn)
		{
			CurColumn = 0;
			CurRow++;
		}
	}
}

void UB2UIPAutoClearResultRow::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UTextBlock, TB_Gold);
	GET_SLOT(UTextBlock, TB_GoldCount);
	GET_SLOT(UUniformGridPanel, UG_Grid);
}

void UB2UIPAutoClearResultRow::UpdateStaticText()
{
	if (TB_Gold.IsValid())
	{
		TB_Gold->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageClear_AcquiredGold")));
	}
}
