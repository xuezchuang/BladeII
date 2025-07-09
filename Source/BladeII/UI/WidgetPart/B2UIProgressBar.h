// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIProgressBar.generated.h"

/*
* ProgressBar the use of the original version is more easily made.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIProgressBar : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;

public:
	UB2UIProgressBar(const FObjectInitializer& ObjectInitializer);

	virtual void SetPercent(float InValue);
	virtual void SetPercentByValue(float CurValue, float MaxValue);

	float GetPercent() const;
	float GetPercentPrev() const;

	void ShowText(bool bShow);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBar")
		void SetPercent_BP(float InPercent, float InPrevPercent);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ProgressBar")
		void SetPercentByValue_BP(float CurValue, float MaxValue);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ProgressBar", meta = (UIMin = "0", UIMax = "1"))
		float Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ProgressBar")
		bool IsTextFormatPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ProgressBar")
		bool bShowText;

protected:
	virtual void CacheAssets() override;

	void SetText(const FText& InText);

protected:
	TWeakObjectPtr<UProgressBar>		PB;
	TWeakObjectPtr<UTextBlock>			TB;

	float PrevPercent;
};
