// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UISelectiveLotteryPopup.h"
#include "B2UISelectiveLotteryCostumeRow.h"
#include "BladeIIGameImpl.h"

void UB2UISelectiveLotteryCostumeRow::Init()
{
	Super::Init();

	for (auto& LotteryCostumeSlot : LotteryCostumeSlots)
	{
		if (LotteryCostumeSlot.IsValid())
		{
			LotteryCostumeSlot->Init();
			LotteryCostumeSlot->SetVisibility(ESlateVisibility::Hidden);
			LotteryCostumeSlot->SetEnableReadingGlasses(false);
		}
	}
}

void UB2UISelectiveLotteryCostumeRow::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto& LotteryCostumeSlot : LotteryCostumeSlots)
	{
		if (LotteryCostumeSlot.IsValid())
		{
			LotteryCostumeSlot->OnSlotCostumeClickDelgate.Unbind();
			LotteryCostumeSlot->DestroySelf(InUIManager);
		}
	}

	Super::DestroySelf(InUIManager);
}

UB2UISelectiveLotteryCostumeSlot* UB2UISelectiveLotteryCostumeRow::SetLotteryCostumeSlotInfo(int32 InIndex, int32 RefId)
{
	TWeakObjectPtr<UB2UISelectiveLotteryCostumeSlot> outCostumSlot = nullptr;

	for (int32 i = 0 ; i < LotteryCostumeSlots.Num() ; i++)
	{
		if (InIndex != i)
			continue;

		if (LotteryCostumeSlots[i].IsValid())
		{
			LotteryCostumeSlots[i]->BindDoc(FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(RefId)));
			LotteryCostumeSlots[i]->SetVisibility(ESlateVisibility::Visible);
			outCostumSlot = LotteryCostumeSlots[i];
		}
	}

	return outCostumSlot.Get();
}

void UB2UISelectiveLotteryCostumeRow::CacheAssets()
{
	LotteryCostumeSlots.SetNum(4);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryCostumeSlot, LotteryCostumeSlots[0], UIP_CostumeElem00);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryCostumeSlot, LotteryCostumeSlots[1], UIP_CostumeElem01);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryCostumeSlot, LotteryCostumeSlots[2], UIP_CostumeElem02);
	GET_SLOT_BYNAME(UB2UISelectiveLotteryCostumeSlot, LotteryCostumeSlots[3], UIP_CostumeElem03);
}

void UB2UISelectiveLotteryCostumeRow::BindDelegates()
{
	for (auto& LotteryCostumeSlot : LotteryCostumeSlots)
	{
		if (LotteryCostumeSlot.IsValid())
		{
			LotteryCostumeSlot->OnSlotCostumeClickDelgate.BindDynamic(this, &UB2UISelectiveLotteryCostumeRow::SelectItem);
		}
	}
}

void UB2UISelectiveLotteryCostumeRow::SelectItem(UB2UISelectiveLotteryCostumeSlot* ClickedItem)
{
	if (LotteryPopup && ClickedItem)
	{
		LotteryPopup->SetSelectedItemRefId(ClickedItem->GetItemRefID());
	}
}