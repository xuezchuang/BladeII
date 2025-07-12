// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIFairyLevelUpStatRow.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFairyLevelUpStatRow : public UB2UIWidget
{
	GENERATED_BODY()
public:
	void SetContent(FText ConditionLevel, FText PreviewStat, FText PreviewStatValue);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
private:
	TWeakObjectPtr<UTextBlock> TB_ConditionLevel;
	TWeakObjectPtr<UB2RichTextBlock> RTB_PreviewStat;
	TWeakObjectPtr<UB2RichTextBlock> RTB_PreviewStatValue;
};
