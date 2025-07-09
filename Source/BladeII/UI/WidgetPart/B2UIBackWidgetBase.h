// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIBackWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIBackWidgetBase : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	void OpenPopup();
	void ClosePopup();


protected:
	virtual void CloseWidgetDelegate();
	
};
