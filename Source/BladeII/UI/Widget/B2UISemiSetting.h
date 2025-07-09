// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UISettingGame.h"
#include "B2UIBackWidget.h"
#include "B2UISemiSetting.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISemiSetting : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UISemiSetting(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnclickBtnExit();

protected:
	virtual void CloseWidgetDelegate() override;

	TWeakObjectPtr<UTextBlock> TB_SettingTitle;

	TWeakObjectPtr<UButton>			BTN_Exit;

	TWeakObjectPtr<UB2UISettingGame> UIP_Setting_Game;
};
