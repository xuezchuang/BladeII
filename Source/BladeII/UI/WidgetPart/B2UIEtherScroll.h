// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIEtherScroll.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherScroll : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ether")
		TSubclassOf<class UB2UIEtherScrollRowSlot> EtherRowSlotClass;
	
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetEtherScroll(class UB2UIEther* InReceiver, int32 EtherSetName, float CachedScrollOffset);
	void UpdateLock(int64 EtherID, bool IsLock);

	FORCEINLINE float GetEtherScrollOffset() { return X_SB_EtherScrollBox->GetScrollOffset(); }

protected:
	virtual void CacheAssets() override;

private:
	TArray<TWeakObjectPtr<class UB2UIEtherScrollRowSlot>>		UIP_EtherScrollRowSlots;
	TWeakObjectPtr<UB2ScrollBox>								X_SB_EtherScrollBox;
};
