// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "BladeIIPlayer.h"
#include "B2UICharacterNickNameEdit.h"
#include "B2UICharacterObserve.generated.h"

/*
* The UI comes when observing character.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICharacterObserve : public UB2UIWidget
{
	GENERATED_BODY()

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	virtual void OnClose(bool RightNow = false) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

private:
	void SubscribeEvent();
	void UnsubscribeEvent();

public:
	void PopupCharNickEdit(EPCClass InEditClass);
	void CloseCharNickEdit();

protected:
	void OpenCharacterSimpleInfoUI();
	void CloseCharacterSimpleInfoUI();
	
private:
	UFUNCTION()
		void OnClickBtnBack();

	UFUNCTION()
		void OnClickBtnCharNickEdit();

	UFUNCTION()
		void OnClickBtnToggleCharInfo();

protected:
	TWeakObjectPtr<UButton>				BTN_Back;
	TWeakObjectPtr<UButton>				BTN_BackExtra; // To extend clickable area of BTN_Back

	TWeakObjectPtr<UB2Button>			BTN_CharNickEdit;
	TWeakObjectPtr<UTextBlock>			TB_CharNickEdit;
	TWeakObjectPtr<UB2UICharacterNickNameEdit> UIP_CharNickEditP;

	TWeakObjectPtr<UB2Button>			BTN_ToggleCharInfo;
	TWeakObjectPtr<UTextBlock>			TB_ToggleInfo;
	TWeakObjectPtr<class UB2UILobbyCharInfo>		UIP_CharSimpleInfo;

	TWeakObjectPtr<UB2Button>		BTN_Change;

	TWeakObjectPtr<UOverlay>		Overlay_36;
	TWeakObjectPtr<UOverlay>		OV_CharNickEditBtnSet_test;
	TWeakObjectPtr<UOverlay>		OV_ToggleCharInfo;
	TWeakObjectPtr<UCanvasPanel>	CP_BTN_BackSet;

	TWeakObjectPtr<UTextBlock> TBS_Back;
	
private:
	bool bSetToggleCharInfo;

	bool m_bChangeScreen;
	UFUNCTION()
	void OnClickBtnChangeScreen();
};
