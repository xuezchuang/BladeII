// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEventPointConditionPart.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPointConditionPart : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	void InitEventPointConditionPart(const b2network::B2mdPointEvent& EventConditionInfo);
	
private:
	TWeakObjectPtr<UB2RichTextBlock>	RTB_AccruePointConditon;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_AccruePointCount;

private:
	b2network::B2mdPointEvent			CahcedEventConditionInfo;
};
