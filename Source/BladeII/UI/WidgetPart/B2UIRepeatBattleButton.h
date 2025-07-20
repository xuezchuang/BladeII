// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIRepeatBattleButton.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRepeatBattleButton : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	void RepeatBattleLoopOneChanged(class UB2UIDocBase* Sender, bool newBool, bool oldBool);
	void RepeatBattleLoopAllChanged(class UB2UIDocBase* Sender, bool newBool, bool oldBool);
	void RepeatBattleBoostOnChanged(class UB2UIDocBase* Sender, bool newFactor, bool oldFactor);
	void RepeatBattleChanged();

	UFUNCTION()
		void OnClickBtnRepeatStart();
	UFUNCTION()
		void OnClickBtnRepeatSetting();

	TWeakObjectPtr<UImage>							IMG_RB_Lock_Icon;
	TWeakObjectPtr<UB2Button>						BTN_RepeatStart;
	TWeakObjectPtr<UB2Button>						BTN_RepeatSetting;
	TWeakObjectPtr<UCanvasPanel>					CV_REPEAT_OPTION;
	TWeakObjectPtr<UOverlay>						CV_LoopOne;
	TWeakObjectPtr<UOverlay>						CV_LoopAll;
	TWeakObjectPtr<UOverlay>						CV_3Times;
	TWeakObjectPtr<UTextBlock>						TB_3Times;
	TWeakObjectPtr<UTextBlock>						TB_RepeatStart;
	TWeakObjectPtr<UTextBlock>						TB_RepeatSetting;
	TWeakObjectPtr<UTextBlock>						TB_3Times_Title;
	TWeakObjectPtr<UTextBlock>						TB_LoopOne;
	TWeakObjectPtr<UTextBlock>						TB_LoopAll;
};
