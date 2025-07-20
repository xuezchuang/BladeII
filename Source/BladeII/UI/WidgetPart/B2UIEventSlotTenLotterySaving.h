// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventSlot.h"
#include "B2UIEventSlotTenLotterySaving.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotTenLotterySaving : public UB2UIEventSlot
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

protected:
	virtual void UpdateVisibleState() override;

private:
	TWeakObjectPtr<UWidgetSwitcher>		WS_State;
};
