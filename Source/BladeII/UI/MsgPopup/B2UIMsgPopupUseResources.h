// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/MsgPopup/B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupUseResources.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMsgPopupUseResources : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	
protected:
	TWeakObjectPtr<UTextBlock> TB_Cost;
	TWeakObjectPtr<UB2RichTextBlock> TB_ContentSub;

public:
	void SetCost(int32 nCost);
	void SetSubContent(FText txtSubContent);
};
