// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIWaitRevive.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIWaitRevive : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void InitTimer(float fWaitTime);

protected:
	virtual void CacheAssets() override;

	void ResetTimer();
	
private:
	TWeakObjectPtr<UTextBlock> TB_RemainTime;

	float m_fWaitTime;
	float m_fCheckStartTime;

	ESlateVisibility BattleMainUiOrgVisibility;
	
};
