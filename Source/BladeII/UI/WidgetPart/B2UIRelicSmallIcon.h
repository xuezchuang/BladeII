// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIRelicSmallIcon.generated.h"

DECLARE_DELEGATE_OneParam(FOnRelicIconClickedDelgate, int32);
/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRelicSmallIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	void SetIngredient(int32 RelicID);
	FOnRelicIconClickedDelgate OnRelicIconClickedDelgate;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickBtn();

private:
	TWeakObjectPtr<UImage>				IMG_Icon;
	TWeakObjectPtr<UB2Button>			BTN_Icon;
	int Index;
};