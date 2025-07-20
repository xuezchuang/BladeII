// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIRelicStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRelicStatus : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void CacheAssets() override;
	void SetRelicStatus(int32 cur, int32 max, int32 add);

private:
	TWeakObjectPtr<UTextBlock>			TB_Status;
	TWeakObjectPtr<UTextBlock>			TB_CurLevel;
	TWeakObjectPtr<UTextBlock>			TB_MaxLevel;
	TWeakObjectPtr<UTextBlock>			TB_AddValue;
	TWeakObjectPtr<UProgressBar>		PB_Status_Relic;
};
