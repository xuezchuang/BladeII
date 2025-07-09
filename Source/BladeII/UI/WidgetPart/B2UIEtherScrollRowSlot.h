// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIEtherScrollRowSlot.generated.h"

const int32 MAX_ETHER_SLOT = 4;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherScrollRowSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetRowInfo(class UB2UIEther* InReceiver, UB2ScrollBox* HandlingScrollBox, int32 StartIndex, int32 EndIndex, int32 SetName);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	bool UpdateLock(int64 EtherID, bool IsLock);

protected:
	virtual void CacheAssets() override;
	
private:
	TArray<TWeakObjectPtr<class UB2UIEtherScrollSlot>> Slots;

	
};
