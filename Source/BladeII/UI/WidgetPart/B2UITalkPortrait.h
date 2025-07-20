// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UITalkPortrait.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITalkPortrait : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetVisible(bool IsVisible);
	bool GetVisible();

	void SetVisibleName();

	void SetPortraitImage(UTexture2D *Texture);
	void SetSpeakerName(const FText &Name);

	void SetReversePortrait(bool IsReverse);

	UFUNCTION()
	void SetActivate();

	UFUNCTION()
	void SetInactivate();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivate_BP(bool IsActivity);

protected:
	TWeakObjectPtr<UImage> IMG_Portrait;
	TWeakObjectPtr<UImage> IMG_NameBG;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Name;

	bool bIsVisible;
};
