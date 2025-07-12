// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIEtherHelpSlot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherHelpSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetInfo(int32 InType);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UB2Image>								X_IMG_SetImage;
	TWeakObjectPtr<UTextBlock>								X_TB_SetName;
	TWeakObjectPtr<UB2RichTextBlock>						X_RTB_SetOptionName;
	TWeakObjectPtr<UTextBlock>								X_TB_SetOptionDesc;
	TWeakObjectPtr<UB2RichTextBlock>						X_RTB_SetOption;
};
