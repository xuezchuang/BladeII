// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherLotteryScroll.h"
#include "B2UIEtherLotteryRow.h"
#include "BladeIIUtil.h"
#include "B2EtherInfo.h"

const int32 ETHERLOTTERY_MAX_SLOT = 4;

void UB2UIEtherLotteryScroll::SetEtherLotteryInfo(int32 InStar, int32 InTier)
{
	TArray<int32> Types;

	if (StaticFindEtherInfoTable() != NULL)
	{
		Types = StaticFindEtherInfoTable()->GetTypes();
		int MaxSlot = Types.Num();

		if (X_SB_Scroll.IsValid() && MaxSlot > 0)
		{
			X_SB_Scroll->SetScrollOffset(0);

			int MaxRow = GetMaxRow(MaxSlot);

			for (int i = 0; i < MaxRow; i++)
			{
				UB2UIEtherLotteryRow* NewItem;
				if (UIP_EtherLotteryRowSlots.IsValidIndex(i))
				{
					NewItem = UIP_EtherLotteryRowSlots[i].Get();
				}
				else
				{
					NewItem = CreateWidget<UB2UIEtherLotteryRow>(GetWorld(), EtherLotteryRowSlotClass);
					UIP_EtherLotteryRowSlots.Add(NewItem);
				}

				NewItem->SetVisibility(ESlateVisibility::Visible);
				NewItem->Init();
				NewItem->SetEtherLotteryRowInfo(GetSlotIDs(Types, i, MaxSlot), InStar, InTier, this, X_SB_Scroll.Get());
				X_SB_Scroll->AddChild(NewItem);
			}
		}
	}
}

void UB2UIEtherLotteryScroll::DestroySelf(class UB2UIManager* InUIManager)
{
	for (TWeakObjectPtr<UB2UIEtherLotteryRow> Elem : UIP_EtherLotteryRowSlots)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}

	//SelectEtherLotterySlotClass<int32>::GetInstance().Unsubscribe(SelectEtherLotterySlotTicket);
}

void UB2UIEtherLotteryScroll::CacheAssets()
{
	GET_SLOT(UB2ScrollBox, X_SB_Scroll);
}

void UB2UIEtherLotteryScroll::BindDelegates()
{
	//SelectEtherLotterySlotTicket = SelectEtherLotterySlotClass<int32>::GetInstance().Subscribe([this](int32 InTypeID)
	//{
	//	for (TWeakObjectPtr<UB2UIEtherLotteryRow>Elem : UIP_EtherLotteryRowSlots)
	//	{
	//		if (Elem.IsValid())
	//		{
	//			Elem->SetSelectedVisibility(InTypeID);
	//		}
	//	}
	//});
}

int32 UB2UIEtherLotteryScroll::GetMaxRow(int MaxSlot)
{
	int32 Result = 0;

	if (MaxSlot % ETHERLOTTERY_MAX_SLOT > 0)
	{
		Result = MaxSlot / ETHERLOTTERY_MAX_SLOT + 1;
	}
	else
	{
		Result = MaxSlot / ETHERLOTTERY_MAX_SLOT;
	}

	return Result;
}

TArray<int32> UB2UIEtherLotteryScroll::GetSlotIDs(const TArray<int32>& InTypes, int32 RowIndex, int32 MaxSlot)
{
	int StartIDX = RowIndex * ETHERLOTTERY_MAX_SLOT;
	int EndIDX = ((RowIndex + 1) * ETHERLOTTERY_MAX_SLOT) - 1 > (MaxSlot - 1) ? (MaxSlot - 1) : ((RowIndex + 1) * ETHERLOTTERY_MAX_SLOT) - 1;

	TArray<int32> Result;

	for (int IDX = StartIDX; IDX <= EndIDX; ++IDX)
	{
		Result.Add(InTypes[IDX]);
	}

	return Result;
}
