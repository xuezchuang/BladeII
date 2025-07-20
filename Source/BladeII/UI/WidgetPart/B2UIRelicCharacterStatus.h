// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIRelicCharacterStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRelicCharacterStatus : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void CacheAssets() override;
	void SetHeroStatus(int32 cur, int32 max);
	void SetRelicStatus(int32 cur, int32 max, int32 add);

private:
	TWeakObjectPtr<UImage>				IMG_Status;
	TWeakObjectPtr<UTextBlock>			TB_Status_Hero;
	TWeakObjectPtr<UTextBlock>			TB_Status_Relic;
	TWeakObjectPtr<UTextBlock>			TB_Status_Add;
	TWeakObjectPtr<UProgressBar>		PB_Status_Hero;
	TWeakObjectPtr<UProgressBar>		PB_Status_Relic;
};
