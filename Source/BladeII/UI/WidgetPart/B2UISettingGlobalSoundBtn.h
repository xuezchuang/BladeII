// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UISettingGlobalSoundBtn.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UISettingGlobalSoundBtn : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

protected:
	UFUNCTION()
	void OnClickBTN_GlobalSound();

protected:
	void UpdateNationBtnState();
	
private:
	TWeakObjectPtr<UB2Button>	BTN_GlobalSound;

	TWeakObjectPtr<UTextBlock>	TB_GlobalSound;

	TWeakObjectPtr<UTextBlock>	TB_Nation;

	TWeakObjectPtr<UWidgetSwitcher>	WS_NationalFlag;

private:
	int32	CachedSoundLocType;
};
