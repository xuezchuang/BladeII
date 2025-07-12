// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIEtherFiltering.generated.h"


/**
 *
 */

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherFiltering : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void SetReceiver(class UB2UIEther* InReceiver);
	void SetSlotInfo(bool IsIcon);
	void DestroySelf(class UB2UIManager* InUIManager);

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ether")
	//TSubclassOf<class UB2UIEtherFilteringRow> FilteringRowSlotClass;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
	void OnClickClose();

	TWeakObjectPtr<UTextBlock>													X_TB_Title;
	TWeakObjectPtr<UB2Button>													X_BTN_Close;
	TWeakObjectPtr<UB2ScrollBox>												X_SB_Scroll;
	//TArray<TWeakObjectPtr<class UB2UIEtherFilteringRow>>						UIP_FilteringSlots;

	struct SlotInfo
	{
		int TypeID;
		int SetType;
		SlotInfo()
		{
			TypeID = 0;
			SetType = 0;
		}
	};
	TArray<SlotInfo>															SlotInfos;
};
