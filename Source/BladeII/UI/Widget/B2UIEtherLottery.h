// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIEtherLottery.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherLottery : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetEtherLotteryInfo(int64 InMailID, int32 InEtherLotteryType);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
	virtual void UpdateStaticText() override;

private:
	UFUNCTION()
		void OnClickBtnClose();
	UFUNCTION()
		void OnClickBtnDetail();
	UFUNCTION()
		void OnClickBtnConfirm();

private:
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_ScrollTitle;
	TWeakObjectPtr<UTextBlock> TB_Confirm;
	TWeakObjectPtr<UTextBlock> X_TB_Empty;
	TWeakObjectPtr<UPanelWidget> CP_Ether;
	TWeakObjectPtr<UPanelWidget> CP_NonSelect;
	TWeakObjectPtr<UB2Button> X_BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UB2Button> BTN_Detail;
	TWeakObjectPtr<class UB2UIEtherLotterySlot> UI_EtherLotteryIcon;
	TWeakObjectPtr<class UB2UIEtherLotteryScroll> UI_EtherLotteryScroll;
	TWeakObjectPtr<class UB2UIEtherLotteryDetailPopup> UIP_EtherLotteryDetailPopup;

	int32 EtherStar = 0;
	int32 EtherTier = 0;
	int32 EtherTypeID = 0;
	int64 MailID = 0;
	uint32 DeliveryOpenMailTicket;
	uint32 DeliveryOpenMailErrorTicket;
	uint32 SelectEtherLotterySlotTicket;
};
