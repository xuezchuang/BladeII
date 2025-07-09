// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIRaidRoulettePiece.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRaidRoulettePiece : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	void SetPercent(const FLinearColor &Color, float Percent);
	void SetAngle(float Angle, float Length);
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	
protected:
	TWeakObjectPtr<UCanvasPanel> CP_Percent;
	TWeakObjectPtr<UImage> IMG_Piece;
	TWeakObjectPtr<UTextBlock> RTB_Percent;
	TWeakObjectPtr<UScaleBox> SB_Percent;
};
