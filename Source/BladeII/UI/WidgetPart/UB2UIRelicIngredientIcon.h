// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/B2UIWidgetBase.h"
#include "UB2UIRelicIngredientIcon.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UUB2UIRelicIngredientIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	void SetIngredient(int32 type, int32 count);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UImage>				IMG_Icon;
	TWeakObjectPtr<UTextBlock>			TB_Count;
	
};
