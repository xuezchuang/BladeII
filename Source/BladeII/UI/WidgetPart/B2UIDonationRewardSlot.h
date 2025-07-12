// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIDonationRewardSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationRewardSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public	:
	void SetInfo(FText InName, UMaterialInterface* InIcon);

protected:
	virtual void CacheAssets() override;
	
private:
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Name;

};
