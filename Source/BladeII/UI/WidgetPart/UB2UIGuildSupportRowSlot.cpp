// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGuildSupportRowSlot.h"
//#include "BladeII.h"
#include "B2DynItemIcon_GuildSupport.h"

void UB2UIGuildSupportRowSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemRowName);

	ItemIDs.SetNum(B2GuildRequestItem::SlotColumnMax);

	Items.SetNum(B2GuildRequestItem::SlotColumnMax);
	GET_SLOT_BYNAME(UB2DynItemIcon_GuildSupport, Items[0], ItemCol_0);
	GET_SLOT_BYNAME(UB2DynItemIcon_GuildSupport, Items[1], ItemCol_1);
	GET_SLOT_BYNAME(UB2DynItemIcon_GuildSupport, Items[2], ItemCol_2);

	for (auto &Each : Items)
	{
		if (Each.IsValid())
			Each->Init();
	}

	Buttons.SetNum(B2GuildRequestItem::SlotColumnMax);
	GET_SLOT_BYNAME(UB2Button, Buttons[0], BTN_Col_0);
	GET_SLOT_BYNAME(UB2Button, Buttons[1], BTN_Col_1);
	GET_SLOT_BYNAME(UB2Button, Buttons[2], BTN_Col_2);
}

void UB2UIGuildSupportRowSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(Buttons[0], &UB2UIGuildSupportRowSlot::OnCallbackCol_0);
	BIND_CLICK_FUNC_TO_BTN(Buttons[1], &UB2UIGuildSupportRowSlot::OnCallbackCol_1);
	BIND_CLICK_FUNC_TO_BTN(Buttons[2], &UB2UIGuildSupportRowSlot::OnCallbackCol_2);
}

void UB2UIGuildSupportRowSlot::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIGuildSupportRowSlot::SetCategoryName(const FText &InText)
{
	if (TB_ItemRowName.IsValid())
		TB_ItemRowName->SetText(InText);
}

void UB2UIGuildSupportRowSlot::UpdateItemIcons(const TArray<FB2Item>& AllItemsArray, const int32 InStartIndex)
{
	const int32 StartIndex = FMath::Clamp(InStartIndex, 0, AllItemsArray.Num() - 1);
	const int32 EndIndex = FMath::Clamp(StartIndex + B2GuildRequestItem::SlotColumnMax, 0, AllItemsArray.Num());

	int32 SlotIndex = 0;
	for (int32 II = StartIndex; II < EndIndex; ++II)
	{
		if (AllItemsArray.IsValidIndex(II) && Items[SlotIndex].IsValid())
		{
			Items[SlotIndex]->UpdateItemData(AllItemsArray[II]);

			ItemIDs[SlotIndex] = AllItemsArray[II].ItemRefID;
			++SlotIndex;
		}
	}

	for (int32 i = SlotIndex; i < B2GuildRequestItem::SlotColumnMax; ++i)
	{
		if (Items[SlotIndex].IsValid())
		{
			Items[SlotIndex]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIGuildSupportRowSlot::OnCallbackCol_0()
{
	int32 SlotIndex = 0;
	OnDelegate.ExecuteIfBound(Items[SlotIndex].Get(), ItemIDs[SlotIndex]);
}

void UB2UIGuildSupportRowSlot::OnCallbackCol_1()
{
	int32 SlotIndex = 1;
	OnDelegate.ExecuteIfBound(Items[SlotIndex].Get(), ItemIDs[SlotIndex]);
}

void UB2UIGuildSupportRowSlot::OnCallbackCol_2()
{
	int32 SlotIndex = 2;
	OnDelegate.ExecuteIfBound(Items[SlotIndex].Get(), ItemIDs[SlotIndex]);
}

