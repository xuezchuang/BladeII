// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEtherLotteryRow.h"
#include "B2UIEtherLotterySlot.h"



void UB2UIEtherLotteryRow::Init()
{
	Super::Init();
	
	for (const TWeakObjectPtr<UB2UIEtherLotterySlot>Elem : X_EtherLotterySlots)
	{
		if (Elem.IsValid())
		{
			Elem->Init();
		}
	}
}

void UB2UIEtherLotteryRow::DestroySelf(class UB2UIManager* InUIManager)
{
	for (const TWeakObjectPtr<UB2UIEtherLotterySlot>Elem : X_EtherLotterySlots)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIEtherLotteryRow::SetEtherLotteryRowInfo(TArray<int32> InTypeIDs, int32 InStar, int32 InTier, UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox)
{
	for (int i = 0; i < X_EtherLotterySlots.Num(); ++i)
	{
		if (X_EtherLotterySlots[i].IsValid())
		{
			if (InTypeIDs.IsValidIndex(i))
			{
				X_EtherLotterySlots[i]->SetVisibility(ESlateVisibility::Visible);
				X_EtherLotterySlots[i]->SetEtherLotterySlotInfo(InTypeIDs[i], InStar, InTier);
			}
			else
			{
				X_EtherLotterySlots[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	for (const TWeakObjectPtr<UB2UIEtherLotterySlot>Elem : X_EtherLotterySlots)
	{
		if (Elem.IsValid())
		{
			Elem->SetupManualScrollBoxSender_byReceiver(InReceiver, HandlingScrollBox);
		}
	}
}

void UB2UIEtherLotteryRow::SetSelectedVisibility(int32 InType)
{
	for (const TWeakObjectPtr<UB2UIEtherLotterySlot>Elem : X_EtherLotterySlots)
	{
		if (Elem.IsValid())
		{
			Elem->SetSelectedVisibility(InType);
		}
	}
}

void UB2UIEtherLotteryRow::CacheAssets()
{
	X_EtherLotterySlots.SetNum(4);
	GET_SLOT_BYNAME(UB2UIEtherLotterySlot, X_EtherLotterySlots[0], X_UIP_Slot_1);
	GET_SLOT_BYNAME(UB2UIEtherLotterySlot, X_EtherLotterySlots[1], X_UIP_Slot_2);
	GET_SLOT_BYNAME(UB2UIEtherLotterySlot, X_EtherLotterySlots[2], X_UIP_Slot_3);
	GET_SLOT_BYNAME(UB2UIEtherLotterySlot, X_EtherLotterySlots[3], X_UIP_Slot_4);
}
