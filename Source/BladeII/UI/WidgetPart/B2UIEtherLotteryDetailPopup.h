// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIEtherLotteryPopupBase.h"
#include "B2UIBackWidget.h"
#include "B2UIEtherLotteryDetailPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLotteryDetailPopup : public UB2UIEtherLotteryPopupBase, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	void SetPopupInfo(int32 InTypeID, int32 InStar, int32 InTier);

private:
	UFUNCTION()
		virtual void OnClickBtnClose();
	
	virtual void CloseWidgetDelegate() override;
	virtual void BindDelegates() override;
};
