// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UILogin.generated.h"

/*
* Login UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UILogin : public UB2UIWidget
{
	GENERATED_BODY()

public:
	//AccountTestCode
	void PopupCreateAccountMenu(); // To be invoked on some like "no such user so try create new user" event
	void EndCreateAccountMenu();
	UFUNCTION(BlueprintImplementableEvent)
		void ShowTestAccountMenu(bool bShow);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickBtnSimpleRun();
	UFUNCTION()
		void OnClickBtnGameStart();


	//AccountTestCode
	UFUNCTION()
		void OnClickBtnCreateAccount();

protected:
	TWeakObjectPtr<UButton>				BTN_SimpleRun;
	TWeakObjectPtr<UButton>				BTN_GameStart;
	TWeakObjectPtr<UEditableTextBox>	ETB_ID;
	TWeakObjectPtr<UEditableTextBox>	ETB_PW;

	//AccountTestCode
	TWeakObjectPtr<UButton>				BTN_CreateAccount;
	TWeakObjectPtr<UEditableTextBox>	ETB_NewAccountID;
	TWeakObjectPtr<UEditableTextBox>	ETB_NewAccountPW;
};
