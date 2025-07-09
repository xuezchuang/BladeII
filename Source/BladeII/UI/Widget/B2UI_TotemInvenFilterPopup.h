// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIUtil.h"

#include "B2UI_TotemInvenFilterPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UI_TotemInvenFilterPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitTotemInvenFilterPopup();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

	virtual void UpdateAcquireText() {}
private:
	void UpdateConfirmButtonState();

public:
	bool GetAllTargetTotems(TArray<FB2Totem> &OutList);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();

	UFUNCTION()
	virtual void OnClickBTN_Confirm() {};

private:
	UFUNCTION()
	void OnAnyTotemFilterCBChanged(bool InCheck);
	
	UFUNCTION()
	void OnClickBTN_Reset();

protected:
	TWeakObjectPtr<UTextBlock>							TB_PopupTitle;
	TWeakObjectPtr<UTextBlock>							TB_PopupNote;

	TWeakObjectPtr<UB2Button>							BTN_Confirm;
	TWeakObjectPtr<UTextBlock>							TB_Confirm;

	TWeakObjectPtr<class UB2UI_TotemInvenFilter>		UIP_TotemInventoryFilter;

	TWeakObjectPtr<UB2Button>							BTN_Close;

	TWeakObjectPtr<UTextBlock>							TB_SellItemCount;
	TWeakObjectPtr<UTextBlock>							TB_SellItemNumber;
	TWeakObjectPtr<UTextBlock>							TB_SellItemSelect;
	TWeakObjectPtr<UTextBlock>							TB_GoldToAcquire;
	TWeakObjectPtr<UTextBlock>							TB_GoldReceive;

	TWeakObjectPtr<UB2Button>							BTN_Reset;
	TWeakObjectPtr<UTextBlock>							TB_Reset;
};

UCLASS()
class BLADEII_API UB2UI_TotemInvenSellAll : public UB2UI_TotemInvenFilterPopup
{
	GENERATED_BODY()

public:
	virtual void InitTotemInvenFilterPopup() override;
	
	void GetAllTargetItemSellingPrice(int32& ItemCount, int32& ItemPrice);

protected:
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void UpdateAcquireText() override;

protected:
	//UFUNCTION()
	virtual void OnClickBTN_Confirm() override;
};

UCLASS()
class BLADEII_API UB2UI_TotemInvenDecompositionAll : public UB2UI_TotemInvenFilterPopup
{
	GENERATED_BODY()

public:
	virtual void InitTotemInvenFilterPopup() override;

	void GetAllTargetItemSellingPrice(int32& ItemCount, int32& ItemPrice);

protected:
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void UpdateAcquireText() override;

protected:
	//UFUNCTION()
	virtual void OnClickBTN_Confirm() override;
};
