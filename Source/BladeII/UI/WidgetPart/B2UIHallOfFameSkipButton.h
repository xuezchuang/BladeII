// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIBackWidgetBase.h"
#include "B2UIHallOfFameSkipButton.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIHallOfFameSkipButton : public UB2UIBackWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetActive(bool InActive);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void SkipMatinee();

	TWeakObjectPtr<UB2Button> BTN_MatineeBack;
};
