// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIEtherFilteringRow.generated.h"

/**
 * 
 */
const int FilTERING_MAX_SLOT = 4;

UCLASS()
class BLADEII_API UB2UIEtherFilteringRow : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	void SetReceiver(class UB2UIEther* InReceiver, UB2ScrollBox* HandlingScrollBox);
	void SetSlotIcon(TArray<int32>& InTypes);
	void SetSlotCount(TArray<int32>& InTypes);
	void DestroySelf(class UB2UIManager* InUIManager);
	
protected:
	virtual void CacheAssets() override;

private:
	TArray<TWeakObjectPtr<class UB2UIEtherFilteringSlot>> Slots;
	
};
