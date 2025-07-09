// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIMsgPopupItemCostConfirm.generated.h"

/*
* The base class of messaged popup. it has two buttons.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupItemCostConfirm : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupItemCostConfirm(const FObjectInitializer& ObjectInitializer);

	void AddHandler(const FMsgPopupOnClick& Handler);

	void SetButtonText(const FText& InText);
	void SetAutoCloseOnClick(bool bAutoClose);

	void SetItemCost(FIntPoint ItemCost);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	bool IsPayAbleItemCount(const FIntPoint& ItemCost);

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
	void OnClickBtnConfirm();

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
	void OnClickBtnClose();


protected:
	TWeakObjectPtr<UButton>		BTN_Confirm;
	TWeakObjectPtr<UButton>		BTN_Close;
	TWeakObjectPtr<UTextBlock>	TB_BtnConfirm;
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemCost;

private:
	FMsgPopupOnClick ClickHandler;
	bool ButtonState;
};