// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherScroll.h"
#include "B2EtherManager.h"
#include "B2UIEtherScrollRowSlot.h"


#include "BladeIIGameImpl.h"

void UB2UIEtherScroll::SetEtherScroll(class UB2UIEther* InReceiver, int32 EtherSetName, float CachedScrollOffset)
{
	if (X_SB_EtherScrollBox.IsValid())
	{
		X_SB_EtherScrollBox->SetScrollOffset(CachedScrollOffset);

		int MaxSlot = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetScrollEther(EtherSetName).Num();

		int MaxRow = 0;
		if (MaxSlot % MAX_ETHER_SLOT > 0)
		{
			MaxRow = MaxSlot / MAX_ETHER_SLOT + 1;
		}
		else
		{
			MaxRow = MaxSlot / MAX_ETHER_SLOT;
		}

		for (int i = 0; i < MaxRow; i++)
		{
			UB2UIEtherScrollRowSlot* NewItem;
			if (UIP_EtherScrollRowSlots.IsValidIndex(i))
			{
				NewItem = UIP_EtherScrollRowSlots[i].Get();
			}
			else
			{
				NewItem = CreateWidget<UB2UIEtherScrollRowSlot>(GetWorld(), EtherRowSlotClass);
				UIP_EtherScrollRowSlots.Add(NewItem);
			}

			NewItem->SetVisibility(ESlateVisibility::Visible);
			NewItem->Init();
			int StartIDX = i * MAX_ETHER_SLOT;
			int EndIDX = ((i + 1) * MAX_ETHER_SLOT) - 1 > (MaxSlot - 1) ? (MaxSlot - 1) : ((i + 1) * MAX_ETHER_SLOT) - 1;
			X_SB_EtherScrollBox->AddChild(NewItem);
			NewItem->SetRowInfo(InReceiver, X_SB_EtherScrollBox.Get(), StartIDX, EndIDX, EtherSetName);
		}

		if (UIP_EtherScrollRowSlots.Num() > MaxRow)
		{
			for (int i = MaxRow; i < UIP_EtherScrollRowSlots.Num();i++)
			{
				UIP_EtherScrollRowSlots[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UIEtherScroll::UpdateLock(int64 EtherID, bool IsLock)
{
	for (TWeakObjectPtr<UB2UIEtherScrollRowSlot> Elem : UIP_EtherScrollRowSlots)
	{
		if (Elem->GetVisibility() == ESlateVisibility::Visible)
		{
			if (Elem->UpdateLock(EtherID, IsLock))
			{
				break;
			}
		}
	}
}

void UB2UIEtherScroll::CacheAssets()
{
	GET_SLOT(UB2ScrollBox, X_SB_EtherScrollBox);
}

void UB2UIEtherScroll::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (int i = 0; i < UIP_EtherScrollRowSlots.Num(); i++)
	{
		UIP_EtherScrollRowSlots[i]->DestroySelf(InUIManager);
	}
}

