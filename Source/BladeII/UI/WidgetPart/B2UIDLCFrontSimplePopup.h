// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIDLCFrontBase.h"
#include "B2UIDLCFrontSimplePopup.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FDLCMsgPopupOnClick)

UENUM(BlueprintType)
enum class EDLCPopupButtonType : uint8
{
	Positive		UMETA(DisplayName = "Positive"),
	Negative		UMETA(DisplayName = "Negative"),
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDLCFrontSimplePopup : public UB2UIDLCFrontBase
{
	GENERATED_BODY()

public:
	UB2UIDLCFrontSimplePopup(const FObjectInitializer& ObjectInitializer);

	void AddHandler(EDLCPopupButtonType ButtonType, const FDLCMsgPopupOnClick& Handler);

	void ShowButton(EDLCPopupButtonType ButtonType, bool bShow);
	void SetButtonText(EDLCPopupButtonType ButtonType, const FText& InText);
	void SetButtonGroup(EUIMsgPopupButtonGroup ButtonType);

	void SetSubContent(const FText& InText);
	void SetTitle(const FText& InText);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION(BlueprintCallable, Category = "DLCFront")
		void OnClickBtnPositive();
	UFUNCTION(BlueprintCallable, Category = "DLCFront")
		void OnClickBtnNegative();
private:
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UB2RichTextBlock>	TB_SubContent;

	TWeakObjectPtr<UButton>		BTN_Positive;
	TWeakObjectPtr<UButton>		BTN_Negative;
	TWeakObjectPtr<UTextBlock>	TB_BtnPositive;
	TWeakObjectPtr<UTextBlock>	TB_BtnNegative;

	TMap<EDLCPopupButtonType, FDLCMsgPopupOnClick> ClickHandlerMap;
};
