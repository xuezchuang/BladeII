// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotTenLotterySaving.h"

void UB2UIEventSlotTenLotterySaving::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_State);
}

void UB2UIEventSlotTenLotterySaving::UpdateVisibleState()
{
	if (WS_State.IsValid())
	{
		WS_State->SetActiveWidgetIndex(iEventState);
	}
}
