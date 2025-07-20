// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "B2UIWidget.h"
#include "B2UICBTClear.generated.h"

DECLARE_DELEGATE_TwoParams(FOnCBTClearFinish, bool, bool);

/**
*
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICBTClear : public UB2UIWidget
{

	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SetLevelUpState(bool InbMain, bool InbSub);

private:
	TWeakObjectPtr<UB2Button> B2Button_0;
	
private:
	bool MainLevelUp;
	bool SubLevelUp;

public:
	UFUNCTION()
		void OnClickClose();

public:
	FOnCBTClearFinish DelegateCBTFinish;

};
