// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIProgressBar.h"
#include "B2UIProgressBarDivided.generated.h"

/*
* ProgressBar visible across the multiple lines, it does not appear on a single line.
* PB of the first layer represents the current divided values.
* and PB_Extra of the second layer represents the next divided values.
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIProgressBarDivided : public UB2UIProgressBar
{
	GENERATED_BODY()

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;

public:
	UB2UIProgressBarDivided(const FObjectInitializer& ObjectInitializer);

	virtual void SetPercent(float InValue) override; // Not the percent value in 0 ~ 100, 0.0 ~ 1.0 scale here.
	virtual void SetPercentByValue(float CurValue, float MaxValue)  override;

	void SetDivideNum(int32 InValue);
	void SetCurDividedNum(int32 InValue);

	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBarDivided")
		void SetPercentDivided_BP(float AllPercent, float CurDividedPercent, float PrevDividedPercent, int32 CurVisibleDividedNum, int32 PrevVisibleDividedNum);

public:
	/* Several dividing if you want to? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ProgressBarDivided")
		int32 TotalDivideNum;

	/*Images show the current value of Odd Even when divided */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ProgressBarDivided")
		FProgressBarStyle StyleOdd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ProgressBarDivided")
		FProgressBarStyle StyleEven;

protected:
	virtual void CacheAssets() override;

	/* Whether the underlying hide ProgressBar under? */
	void HideExtraProgressBar(bool bHide);

	void UpdateProgressBarStyle();

	/** set the image of ProgressBar */
	void SetProgressStyle(TWeakObjectPtr<UProgressBar> TargetPB, const FProgressBarStyle& InStyle);


protected:
	TWeakObjectPtr<UProgressBar>					PB_Extra;
	TWeakObjectPtr<UTextBlock>						TB_CurDivideNum;
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BladeII ProgressBarDivided")
	int32 CurDividedNum;
	UPROPERTY(BlueprintReadOnly, Category = "BladeII ProgressBarDivided")
	int32 PrevCurDivideNum;

	/* Is this current piece is applied even-odd? */
	bool IsOddCurDividedNum;

	/* Is this what percentage each of the pieces? */
	float PercentPerEachDivide;

	/* The previous percent value in the current divede piece. */
	float PrevCurDividedPercent;
};

/**
 * Extended B2UIProgressBarDivided to place another one at below..
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIProgressBarDividedDual : public UB2UIProgressBarDivided
{
	GENERATED_BODY()

	uint32 bShowSecondPB : 1;

public:
	UB2UIProgressBarDividedDual(const FObjectInitializer& ObjectInitializer);

	void SetShowSecond(bool bShow);

	void SetProgressOfSecond(float CurValue, float MaxValue);
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetProgressOfSecondBP(float CurValue, float MaxValue);

	virtual void CacheAssets() override;
public:
	virtual void Init() override;

protected:
	TWeakObjectPtr<UPanelWidget>	P_SecondPBSet; // The second ProgressBar can have extra related widgets within this panel.
	TWeakObjectPtr<UProgressBar>	PB_Second; // Just another one to show different value.

};