// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIRaidRoulette.generated.h"

/**
 * 
 */

struct FRaidRouletteInfo
{
	enum class EState : uint8
	{
		Win,
		Lose,
		Giveup,
		Max,
	};

	EState State;
	int32 Contribution;
	int32 PCIndex;
	FLinearColor PieceColor;
};

DECLARE_DELEGATE(FRouletteSpinFinished);

UCLASS()
class BLADEII_API UB2UIRaidRoulette : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:

	virtual void Init() override;
	
	void StartRoulette();
	void SetRoulette(const TArray<FRaidRouletteInfo> &InfoArray, int32 TargetIndex);
	void SkipRoulette();

	FORCEINLINE const int32 GetCurrentTargetIndex() {	return CurrentTargetIndex; }

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII RaidRoulette")
	void PlayRouletteSound_BP();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	void SpinRoulette(float InDeltaTime);
	float GetAccumulatedAngle(const int Index);
	float GetAngleRange(const int Index);

public:
	FRouletteSpinFinished RouletteSpinDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "BladeII")
	UCurveFloat* Curve;

private:
	enum class ERouletteState : uint8
	{
		None,
		Rotating,
		RremainingTarget,
		Max,
	};

	const float Degree360 = 360.0f;

	TWeakObjectPtr<UCanvasPanel> CP_Root;
	TWeakObjectPtr<UImage> IMG_Arrow;

	TArray<TWeakObjectPtr<class UB2UIRaidRoulettePiece>> PieceArray;
	TArray<float> RoulettePercents;

	ERouletteState RouletteState;
	
	const float MaxRotateSpeed = 2000.0f;
	float RotateSpeed = 2000.0f;
	float TargetAngle;
	float RotatedAngle;
	float ProgressTime;
	int32 CurrentTargetIndex = 0;
};

