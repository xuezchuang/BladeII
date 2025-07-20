// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UILotteryTab.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(OnClickLotteryTab, EPCClass)

UCLASS()
class BLADEII_API UB2UILotteryTab : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetTabPCClass(EPCClass InEPCClass);
	void SetOnClickLotteryTabDelegate(OnClickLotteryTab InDelegate);
	void SetTabVisible(bool IsSelect);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
		void OnClickTab();

public:
	UPROPERTY(EditAnywhere, Category = "TabIcon")
		TSoftObjectPtr<class UMaterialInterface> TabActiveIcon;
	UPROPERTY(EditAnywhere, Category = "TabIcon")
		TSoftObjectPtr<class UMaterialInterface> TabPassiveIcon;

private:
	TWeakObjectPtr<UButton> BTN_Tab;
	TWeakObjectPtr<UImage> IMG_Active;
	TWeakObjectPtr<UImage> IMG_Passive;
	OnClickLotteryTab OnClickLotteryTabDelegate;
	EPCClass TabPCClass;
};
