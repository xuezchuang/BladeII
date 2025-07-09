// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIEtherLotteryPopupBase.h"
#include "B2UIBackWidget.h"
#include "B2UIEtherLotteryResultPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLotteryResultPopup : public UB2UIEtherLotteryPopupBase, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	void SetPopupInfo(const b2network::B2AetherServerInfoPtr EtherPtr);

private:
	UFUNCTION()
		virtual void OnClickBtnClose();

	virtual void CloseWidgetDelegate() override;
	virtual void BindDelegates() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

private:
	TWeakObjectPtr<UB2RichTextBlock> TB_Receive;
	TWeakObjectPtr<UTextBlock> TB_ResultType;
};
