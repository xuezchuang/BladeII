// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIModRewardBoxGradeInfo.generated.h"


UCLASS()
class BLADEII_API UB2UIModRewardBoxGradeInfo : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	
	void SetBoxGrade(int32 InModeType, int32 Grade, int32 WinCount);

	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickBronze();
	UFUNCTION()
		void OnClickSilver();
	UFUNCTION()
		void OnClickGold();
	UFUNCTION()
		void OnClickPlatinum();
	UFUNCTION()
		void OnClickDia();
	UFUNCTION()
		void OnClickMaster();
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:

	TArray<TWeakObjectPtr<UTextBlock>>		CountTextArr;
	TWeakObjectPtr<UB2Button>				BTN_Close;

	TWeakObjectPtr<UB2Button>				BTN_Bronze;
	TWeakObjectPtr<UB2Button>				BTN_Silver;
	TWeakObjectPtr<UB2Button>				BTN_Gold;
	TWeakObjectPtr<UB2Button>				BTN_Platinum;
	TWeakObjectPtr<UB2Button>				BTN_Dia;
	TWeakObjectPtr<UB2Button>				BTN_Master;


private:
	int32 ModType;
};
