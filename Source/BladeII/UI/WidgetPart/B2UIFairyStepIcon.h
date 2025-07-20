// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIFairyStepIcon.generated.h"


namespace EFairyStepIconStatus
{
	enum Type : uint8
	{
		ON,
		OFF,
		LEVEL_MAX,
	};
}
/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyStepIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetStatus(EFairyStepIconStatus::Type Status);

	UFUNCTION(BlueprintImplementableEvent)
	void StartOnAnimation();
protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UImage>	FX_On;
	TWeakObjectPtr<UWidgetSwitcher>	WS_Switch;
};
