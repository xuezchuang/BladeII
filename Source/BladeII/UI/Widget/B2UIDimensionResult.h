// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDimensionResult.generated.h"


UCLASS()
class BLADEII_API UB2UIDimensionResult : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void SetButtonEnable(bool bEnable);
	void OnShowResult();

	UFUNCTION(BlueprintImplementableEvent)
	void AnimPlay_Open();

	UFUNCTION(BlueprintCallable, Category = "UI Herotower")
	void AnimEnd_End();
	
	UFUNCTION(BlueprintImplementableEvent)
	void AnimPlay_NewRecord();

	UFUNCTION(BlueprintCallable, Category = "UI Herotower")
	void AnimEnd_NewRecord();

private:
	virtual void CloseWidgetDelegate() override;

protected:
	UFUNCTION()
	void OnClickBTN_Ok();

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2UIRewardIcon> RewardItemClass;

private:
	TWeakObjectPtr<UHorizontalBox> HB_Rewards;
	TWeakObjectPtr<UB2Button> BTN_Ok;
	TWeakObjectPtr<UTextBlock> TB_Step;
	TWeakObjectPtr<UTextBlock> TB_RecordSec;

	TWeakObjectPtr<UTextBlock> STB_RecordSec;
	TWeakObjectPtr<UTextBlock> STB_NewRecord;
	TWeakObjectPtr<UTextBlock> STB_Reward;
	TWeakObjectPtr<UTextBlock> STB_Ok;

	int32 DeliveryGetDimensionTicket;
};
