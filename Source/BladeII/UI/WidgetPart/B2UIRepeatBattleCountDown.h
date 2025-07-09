// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIRepeatBattleCountDown.generated.h"

DECLARE_DELEGATE(FRepeatBattleCountDownCB);

/*
 * This repeat battle standing-by count down part is used in several places 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRepeatBattleCountDown : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UIRepeatBattleCountDown(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	/** Called when the count down is done */
	FRepeatBattleCountDownCB OnAllCountedCB;

protected:
	virtual void CloseWidgetDelegate() override { }

	TWeakObjectPtr<UB2Button> Btn_Pause;
	TWeakObjectPtr<UTextBlock> TB_RepeatBattleOnState;
	TWeakObjectPtr<UTextBlock> TB_RepeatBattleCountDownNumber;
	TWeakObjectPtr<UPanelWidget> CP_RepeatBattleInfoSet;
	TWeakObjectPtr<UImage> IMG_BG;

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void BindDelegates() override;

public:
	void SetNotiText(FText NotiText);
	void StartCountdown(FRepeatBattleCountDownCB InAllCountedCB, float WholeCountTime = 3.0f); // Very first start of count down.
	void BattleCountDownSetVisibility(ESlateVisibility InVisibility);

	UFUNCTION()
	void OnClickBTN_Pause();

private:
	bool bIsPauseAble;
	bool bIsCounting;
	bool bIsFinishCount;
	float CountDownNumber; // Reduced for every 1 sec, to display number
	bool IsBackPopup;

	void CountdownTick(float InDeltaTime); // For every 1 sec (update count down number text)
public:
	void StopCountdown();
	void PauseCountdown();
	void ResumeCountdown();
	void SetPauseAble(bool IsEnable);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnStartCountdownBP();

	FORCEINLINE bool IsCounting() { return bIsCounting; }
};

void PopUpRepeatBattleStopMsg(class APlayerController* InPC);
