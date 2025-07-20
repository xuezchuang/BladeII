// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "EventSubsystem.h"
#include "B2UIDonationPointStepRewardPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationPointStepRewardPopup : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetInfo(int32 InStep);
	
protected:
	virtual void BindDelegates() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickGetReward();
	UFUNCTION()
		void OnClickClose();

private:
	const int32 MAXREWARD = 3;
	int32 DonationRewardStep;
	TArray<TWeakObjectPtr<class UB2UIRewardIcon>> UIP_RewardIcons;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_GetReward;
	TWeakObjectPtr<UB2Button> BTN_GetReward;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TArray<issue_ptr>	Issues;
};
