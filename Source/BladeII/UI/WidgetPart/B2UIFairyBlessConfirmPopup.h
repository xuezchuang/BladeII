// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIFairyBlessConfirmPopup.generated.h"

DECLARE_DELEGATE(FFairyBlessConfirmOnClick)

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyBlessConfirmPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetConfirmBtnHandler(const FFairyBlessConfirmOnClick& InConfirmBtnHandler);
	void SetBlessOption(int32 OptionId, float OptionValue);
private:
	UFUNCTION()
	void OnClickBTN_Confirm();

	UFUNCTION()
	void OnClickBTN_Cancel();

private:
	FFairyBlessConfirmOnClick OnClickConfirmBtnHandler;


	TWeakObjectPtr<UTextBlock> TB_Title;

	TWeakObjectPtr<UB2RichTextBlock> RTB_Stat;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Content;

	TWeakObjectPtr<UB2Button> BTN_Negative;
	TWeakObjectPtr<UB2Button> BTN_Positive;

	TWeakObjectPtr<UTextBlock> TB_BtnNegative;
	TWeakObjectPtr<UTextBlock> TB_BtnPositive;
	
};
