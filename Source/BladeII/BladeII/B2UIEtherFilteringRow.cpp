// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIEtherFilteringRow.h"
#include "B2UIEtherFilteringSlot.h"
#include "B2UIEther.h"


void UB2UIEtherFilteringRow::Init()
{
	Super::Init();
	for (int i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsValid())
		{
			Slots[i]->Init();
		}
	}
}

void UB2UIEtherFilteringRow::SetReceiver(class UB2UIEther* InReceiver, UB2ScrollBox* HandlingScrollBox)
{
	for (int i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsValid())
		{
			Slots[i]->SetupManualScrollBoxSender_byReceiver(InReceiver, HandlingScrollBox);
			Slots[i]->SetReceiver(InReceiver);
		}
	}
}

void UB2UIEtherFilteringRow::SetSlotIcon(TArray<int32>& InTypes)
{
	for (int i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsValid())
		{
			if (InTypes.IsValidIndex(i))
			{
				Slots[i]->SetVisibility(ESlateVisibility::Visible);
				Slots[i]->SetIcon(InTypes[i]);
			}
			else
			{
				Slots[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UIEtherFilteringRow::SetSlotCount(TArray<int32>& InTypes)
{
	for (int i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsValid() && InTypes.IsValidIndex(i))
		{
			Slots[i]->SetCount(InTypes[i]);
		}
	}
}

void UB2UIEtherFilteringRow::DestroySelf(class UB2UIManager* InUIManager)
{
	for (int i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsValid())
		{
			Slots[i]->DestroySelf(InUIManager);
		}
	}
}

void UB2UIEtherFilteringRow::CacheAssets()
{
	Slots.SetNum(FilTERING_MAX_SLOT);
	GET_SLOT_BYNAME(UB2UIEtherFilteringSlot, Slots[0], X_UIP_Slot0);
	GET_SLOT_BYNAME(UB2UIEtherFilteringSlot, Slots[1], X_UIP_Slot1);
	GET_SLOT_BYNAME(UB2UIEtherFilteringSlot, Slots[2], X_UIP_Slot2);
	GET_SLOT_BYNAME(UB2UIEtherFilteringSlot, Slots[3], X_UIP_Slot3);
}
