// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDimensionWeekRewardPopup.generated.h"


UCLASS()
class BLADEII_API UB2UIDimensionWeekRewardPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void SetReward(b2network::B2DimensionRankingRewardPtr Rewards);

protected:
	UFUNCTION()
	void OnClickBtn_Close();

private:
	TWeakObjectPtr<UTextBlock> TB_MyRank;
	TWeakObjectPtr<UTextBlock> TB_RankPercent;
	TWeakObjectPtr<UTextBlock> TB_MaxStep;
	TWeakObjectPtr<UTextBlock> TB_Timer;

	TWeakObjectPtr<UTextBlock> STB_MyRanking;
	TWeakObjectPtr<UTextBlock> STB_MaxStep;
	TWeakObjectPtr<UTextBlock> STB_Step;
	TWeakObjectPtr<UTextBlock> STB_MyRankingUnit;
	TWeakObjectPtr<UTextBlock> STB_SendMail;
	TWeakObjectPtr<UTextBlock> STB_WeekRewardTitle;
	
	TWeakObjectPtr<UB2Button> Btn_Close;

	TArray<TWeakObjectPtr<class UB2UIRewardIcon>> RewardSlots;
};
