// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIClientIndicator.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIClientIndicator : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayIndicatorAnimation();

	void IsActive(bool InValue);

private:
	virtual void Init() override;
};

float GetBackOffTime(float RequestTime, float ResponseTime);
