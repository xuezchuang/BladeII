// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIMsgPopupSimple.generated.h"


UENUM(BlueprintType)
enum class EUIMsgPopupButton : uint8
{
	Positive		UMETA(DisplayName = "Positive"),
	Negative		UMETA(DisplayName = "Negative"),
	Close			UMETA(DisplayName = "Close"),
};


/*
* The base class of messaged popup. it has two buttons.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupSimple : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupSimple(const FObjectInitializer& ObjectInitializer);

	void AddHandler(EUIMsgPopupButton ButtonType, const FMsgPopupOnClick& Handler);

	void ShowButton(EUIMsgPopupButton ButtonType, bool bShow);
	void SetButtonText(EUIMsgPopupButton ButtonType, const FText& InText);
	void SetButtonGroup(EUIMsgPopupButtonGroup ButtonType);

	void SetAutoCloseOnClick(bool bAutoClose);
	void SetSubContent(const FText& InText);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
		void OnClickBtnPositive();
	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
		void OnClickBtnNegative();

	UFUNCTION(BlueprintCallable)
		void OnClickDestorySelf();

protected:
	TWeakObjectPtr<UButton>		BTN_Positive;
	TWeakObjectPtr<UButton>		BTN_Negative;
	TWeakObjectPtr<UButton>		BTN_Close;
	TWeakObjectPtr<UTextBlock>	TB_BtnPositive;
	TWeakObjectPtr<UTextBlock>	TB_BtnNegative;

	TWeakObjectPtr<UB2RichTextBlock>	TB_SubContent; // if you use this, need to add textfield in asset.
private:
	TMap<EUIMsgPopupButton, FMsgPopupOnClick> ClickHandlerMap;

	bool IsAutoCloseOnClick;
	bool ButtonState;
};
