// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEventSlotHB.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlotHB : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;

public:
	void AddChildWidget(UWidget* Content);
	
private:
	TWeakObjectPtr<UHorizontalBox>	HorizontalBox;
};
