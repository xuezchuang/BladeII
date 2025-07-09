// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIHotTimeIcon.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHotTimeIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void ActiveEffect_BP(bool bActive);
};

