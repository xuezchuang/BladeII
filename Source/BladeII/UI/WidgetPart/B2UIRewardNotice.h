// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIRewardNotice.generated.h"


UCLASS()
class BLADEII_API UB2UIRewardNotice : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetTextAndStartAnimation(const FString& TextString, float DelayTime = 0);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII AdventureModSelect")
	void StartAnimation_BP(float DelayTime);

private:
	TWeakObjectPtr<UTextBlock>				TB_Main;
	FTimerHandle							ShowStartDelayTimerHandle;
};
