// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIMsgPopupBase.generated.h"

/*
* The base class of messaged popup.
*/

DECLARE_DELEGATE(FMsgPopupOnClick)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupBase : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	friend class UB2UIManager;

public:
	UB2UIMsgPopupBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeAutoCloseTimeCallback(float InRemainAutoCloseTime);
	
	void SetTitle(const FText& InText);
	void SetContent(const FText& InText);

	void ShowModalArea(bool bShow);
	void SetModalWindow(bool bModal);

	void SetAutoCloseTime(float InAutoCloseTime);
	
	void SetMsgID(int32 InValue){ MsgID = InValue; }
	void SetPriority(EPopUpPriority InPriority) { PopupPriority = InPriority; }

	EPopUpPriority GetPriority() const { return PopupPriority;}
	int32 GetMsgID() const		{ return MsgID; }

	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override { }

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
		virtual void SetClickButtonState(bool ButtonType);
	UFUNCTION(BlueprintCallable, Category = "BladeII BattleSkill")
		virtual bool GetClickButtonState();

protected:
	TWeakObjectPtr<UButton> BTN_Modal;
	TWeakObjectPtr<UTextBlock>	TB_Title;
	TWeakObjectPtr<UB2RichTextBlock>	TB_Content;

	bool ButtonState;

private:
	bool bModalWindow;

	int32 MsgID;

	EUIMsgPopup MsgPopupType;

	/* If a value greater than 0 is activated automatically closed. */
	float AutoCloseTime;
	/* The time remaining until the automatic closing. (but, if AutoCloseTime greater than 0 ) */
	float RemainAutoCloseTime;
	EPopUpPriority PopupPriority;
};
