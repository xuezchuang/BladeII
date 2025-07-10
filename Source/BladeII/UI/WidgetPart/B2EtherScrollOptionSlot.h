// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "B2EtherScrollOptionSlot.generated.h"


UCLASS()
class BLADEII_API UB2EtherScrollOptionSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual bool Initialize() override;
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetInfo(EItemOption Option, float Value, EPCClass PCClass);

private:
	TWeakObjectPtr<UTextBlock>					X_RTB_OptionName;
	TWeakObjectPtr<UTextBlock>					X_RTB_OptionValue;
};
