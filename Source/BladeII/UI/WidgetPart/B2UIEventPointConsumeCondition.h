// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIEventPointConsumeCondition.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPointConsumeCondition : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	void InitEventPointConsimeCondition(const b2network::B2mdPointEvent& EventConditionInfo);

private:
	TWeakObjectPtr<UB2RichTextBlock>	RTB_AcquirePointConditon;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_AcquirePointCount;

private:
	b2network::B2mdPointEvent			CahcedEventConditionInfo;
};
