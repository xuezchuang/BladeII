// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherScrollRowSlot.h"
#include "B2UIEtherScrollSlot.h"
#include "B2UIEther.h"
#include "B2EtherManager.h"


#include "BladeIIGameImpl.h"

void UB2UIEtherScrollRowSlot::CacheAssets()
{
	Slots.SetNum(MAX_ETHER_SLOT);

	GET_SLOT_BYNAME(UB2UIEtherScrollSlot, Slots[0], Slot1);
	GET_SLOT_BYNAME(UB2UIEtherScrollSlot, Slots[1], Slot2);
	GET_SLOT_BYNAME(UB2UIEtherScrollSlot, Slots[2], Slot3);
	GET_SLOT_BYNAME(UB2UIEtherScrollSlot, Slots[3], Slot4);
	
	for (int i = 0; i < MAX_ETHER_SLOT; i++)
	{
		if (Slots[i].IsValid())
		{
			Slots[i]->Init();
		}
	}
}

void UB2UIEtherScrollRowSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (int i = 0; i < MAX_ETHER_SLOT; i++)
	{
		if (Slots[i].IsValid())
		{
			Slots[i]->DestroySelf(InUIManager);
		}
	}
}

bool UB2UIEtherScrollRowSlot::UpdateLock(int64 EtherID, bool IsLock)
{
	bool Result = false;

	for (TWeakObjectPtr<UB2UIEtherScrollSlot> Elem : Slots)
	{
		if (Elem->GetEtherID() == EtherID)
		{
			Elem->UpdateLock(IsLock);
			Result = true;
			break;
		}
	}

	return Result;
}

void UB2UIEtherScrollRowSlot::SetRowInfo(class UB2UIEther* InReceiver, UB2ScrollBox* HandlingScrollBox, int32 StartIndex, int32 EndIndex, int32 SetName)
{
	//auto& StoredEthers = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetScrollEther(SetName);
	//for (int i = 0; i < MAX_ETHER_SLOT; i++)
	//{
	//	int CurIDX = StartIndex + i;
	//	if (Slots[i].IsValid())
	//	{
	//		if (CurIDX <= EndIndex)
	//		{
	//			Slots[i]->SetupManualScrollBoxSender_byReceiver(InReceiver, HandlingScrollBox);
	//			Slots[i]->SetRowSlotInfo(InReceiver, StoredEthers[CurIDX]);
	//		}
	//		else
	//		{
	//			Slots[i]->SetVisibility(ESlateVisibility::Hidden);
	//		}
	//	}
	//}
}
