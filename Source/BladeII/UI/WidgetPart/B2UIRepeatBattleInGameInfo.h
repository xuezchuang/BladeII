// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2UiBackWidget.h"
#include "B2UIRepeatBattleInGameInfo.generated.h"

/*
 * This repeat battle info set to display repeat battle status for several in-game UI.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRepeatBattleInGameInfo : public UB2UIWidgetBase, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()


protected:
	virtual void CloseWidgetDelegate() override { }

	TWeakObjectPtr<UTextBlock> TB_RepeatBattleOnMessage;
	TWeakObjectPtr<UTextBlock> TB_RepeatBattleCount;
	TWeakObjectPtr<UTextBlock> TB_RepeatBattleStopBtn;
	TWeakObjectPtr<UB2Button> BTN_RepeatBattleStop;
	TWeakObjectPtr<UTextBlock> TB_RepeatBattleCount_Boost;
	TWeakObjectPtr<UPanelWidget> CV_RepeatBattleInfo;
	TWeakObjectPtr<UPanelWidget> CV_3TimesInfo;
	TWeakObjectPtr<UTextBlock> TB_RepeatBattleOnMessage_Boost;

	TWeakObjectPtr<UImage>			IMG_BoostFull;

	TWeakObjectPtr<class UB2UIDocBattleStage> m_DocBattleStage;

public:
	UB2UIRepeatBattleInGameInfo(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
	void UpdateDynamicWidgets(); // Stuff that need to be called when repeat battle state has been changed.

	UFUNCTION()
	void OnClickRepeatBattleStopBtn();

	UFUNCTION()
	void OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn);

	bool IsBackPopup;
};
