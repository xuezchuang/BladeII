// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "HotTimeBuffManager.h"
#include "B2UIHotTimeBuffPopup.h"
#include "B2UIHotTimeBuffPart.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHotTimeBuffPart : public UB2UIWidget 
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void ChangeActiveBuff(bool Active);
	void SetVisibleUIForEventType();

	void UpdateEventTypeUI();
	void UpdateProductTypeUI();

	FText GetHotTimeHourMinText(const FHotTimeEventDetailInfo& InEventDetailInfo);

	UFUNCTION()
		void OnClickBuyProductBuff();
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetHotTimeData(EHotTimeEventType InEventType, EHotTimeBuffType InBuffType, const FB2HotTimeBuffUIData& InBuffUIData);
	void UpdateBuffUI();

protected:
	EHotTimeEventType	 EventType;
	EHotTimeBuffType	 BuffType;
	FB2HotTimeBuffUIData BuffUIData;

protected:
	TWeakObjectPtr<UOverlay>	O_Active;
	
	TWeakObjectPtr<UTextBlock>	TB_ActiveBuffTitle;
	TWeakObjectPtr<UTextBlock>	TB_DeActiveBuffTitle;
	TWeakObjectPtr<UB2Image>	IMG_BuffImage;

	TWeakObjectPtr<UTextBlock>	TB_IncreaseRatio;
	TWeakObjectPtr<UTextBlock>	TB_BuffDescription;

	TWeakObjectPtr<UOverlay>	O_Time;
	TWeakObjectPtr<UTextBlock>	TB_RemainTime;

	TWeakObjectPtr<UTextBlock>	TB_BuffState;

	TWeakObjectPtr<UOverlay>	O_CurrentHotTime;
	TWeakObjectPtr<UOverlay>	O_NextHotTime;

	TWeakObjectPtr<UTextBlock>	TB_CurrentHotTime;
	TWeakObjectPtr<UTextBlock>	TB_NextHotTimeTitle;
	TWeakObjectPtr<UTextBlock>	TB_NextHotTime;

	TWeakObjectPtr<UB2Button>	BTN_BuyProductBuff;
	TWeakObjectPtr<UB2Image>	IMG_GoodsImage;
	TWeakObjectPtr<UTextBlock>	TB_Price;

	TWeakObjectPtr<UHorizontalBox>	HB_IncreaseRatio;
};

