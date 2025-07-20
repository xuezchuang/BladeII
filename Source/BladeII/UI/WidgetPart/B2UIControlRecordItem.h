// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlRecordItem.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlRecordItem : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	void SetTitle(const FText& Text);
	void SetAmount(const FText& Text);

private:
	virtual void CacheAssets() override;

protected:
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Amount;
	
};
