// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "B2UIScenarioDialog.generated.h"

/*
* Scenario Dialog UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIScenarioDialog : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	
	/** It specifies how much the overall layout will get down along screen Y axis when battle UI is hidden. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float YOffsetForBattleUIHidden;

	/**
	 * Each character will be shown at automatically calculated interval, but cannot over this time even when dialog is short.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float MaxCharShowInterval;

	/** Manually counted delta time, for use in paused circumstance */
	float ManualRealDeltaTime;
	float ManualLastRealTime;
	float ManualPauseDiffTime;

	FProgressiveTextBuilder ProgressiveDialogText;
	
public:
	UB2UIScenarioDialog(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:
	void UpdateManualTimeCounter();
public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	void OnPause(bool Pause);

	void ChangeDialog();

	void SetSpeakerName(const FText& InText);
	void SetSpeakerPortrait(UMaterialInterface* InMaterial);
	void SetDialogText(const FText& InText, float InThisTextShowingTime);

private:
	void FinalApplyDialogText();
	void ResetTimeStates();
public:

	void ShowDialogBox(bool bShow);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ScenarioDialog")
		void ShowNextBtn(bool bShow);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ScenarioDialog")
		void ShowDialogBox_BP(bool bShow);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ScenarioDialog")
		void ChangeFadeOutDialog();
	UFUNCTION(BlueprintCallable, Category = "BladeII ScenarioDialog")
		void ChangeFadeOutDialogComplete();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ScenarioDialog")
		void ChangeFadeInDialog();
	UFUNCTION(BlueprintCallable, Category = "BladeII ScenarioDialog")
		void ChangeFadeInDialogComplete();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void UpdateDialogDatas();

	UFUNCTION()
		void OnClickBtnNext();
	UFUNCTION()
		void OnClickBtnSkip();
	UFUNCTION()
		void OnChangedSpeakerPortrait(class UB2UIDocBase* Sender, UMaterialInterface* SpeakerPortrait, UMaterialInterface* PrevSpeakerPortrait);
	UFUNCTION()
		void OnChangedSpeakerName(class UB2UIDocBase* Sender, FText SpeakerName, FText PrevSpeakerName);
	UFUNCTION()
		void OnChangedDialogShowingTime(class UB2UIDocBase* Sender, float ShowingTime, float PrevShowingTime);
	UFUNCTION()
		void OnChangedDialogText(class UB2UIDocBase* Sender, FText DialogText, FText PrevDialogText);
	UFUNCTION()
		void OnChangedShowNextButton(class UB2UIDocBase* Sender, bool ShowNextButton, bool PrevShowNextButton);
	UFUNCTION()
		void OnChangedLastBattleUIHidden(class UB2UIDocBase* Sender, bool LastBattleUIHidden, bool PrevLastBattleUIHidden);

protected:
	TWeakObjectPtr<UCanvasPanel>		CP_OverallLayout;
	TWeakObjectPtr<UButton>				BTN_Area;
	TWeakObjectPtr<UButton>				BTN_Next;
	TWeakObjectPtr<UButton>				BTN_Skip;
	TWeakObjectPtr<UImage>				IMG_Speaker;
	TWeakObjectPtr<UTextBlock>			TB_Speaker;
	TWeakObjectPtr<UTextBlock>			TB_Dialog;
	TWeakObjectPtr<UButton>				BTN_ScenarioTouch;

private:
	bool bPause;;
	bool bShownDialogBox;
	FString TempPrevSpeakerName;

	FVector2D CachedOriginalLayoutPos;
};
