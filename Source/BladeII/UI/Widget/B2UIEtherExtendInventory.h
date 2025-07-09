// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIUtil.h"
#include "B2UIEtherExtendInventory.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherExtendInventory : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

	void UpdateWidgets(int32 NewExtendCount);

	int32 GetSliderExtendCount();
	int32 GetRealExtendCount();

private:
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickExtend();
	UFUNCTION()
		void OnClickMaxExtend();
	UFUNCTION()
		void OnClickBtnDown();
	UFUNCTION()
		void OnClickBtnUp();
	UFUNCTION()
		void OnChangedExtendCount(float ChangeValue);


	TWeakObjectPtr<UTextBlock>						X_TB_Title;
	TWeakObjectPtr<UTextBlock>						X_TB_CurrentCount;
	TWeakObjectPtr<UTextBlock>						X_TB_PostExtendCount;
	TWeakObjectPtr<UTextBlock>						X_TB_MaxSlotCountInfo;
	TWeakObjectPtr<UTextBlock>						X_TB_ExtendBtn;
	TWeakObjectPtr<UTextBlock>						X_TB_ExtendCost;
	TWeakObjectPtr<UTextBlock>						X_TB_MaxExtendBtn;
	TWeakObjectPtr<UButton>							X_BTN_Close;
	TWeakObjectPtr<UButton>							X_BTN_Extend;
	TWeakObjectPtr<UButton>							X_BTN_MaxExtend;
	TWeakObjectPtr<UButton>							X_BTN_Down;
	TWeakObjectPtr<UButton>							X_BTN_Up;

	TWeakObjectPtr<USlider>							S_CashExtendSlider;

	B2SlotSliderLogic								SlotSliderLogic;

	int32											ExtendCost;
	int32											MaxExtendNum = 0;
	uint32											ExtendInventoryEventID = 0;
};
