// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "../B2RichTextBlock.h"
#include "EventSubsystem.h"
#include "B2UIConnectReward.generated.h"


UCLASS()
class BLADEII_API UB2UIConnectReward : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	bool IsNewReward() const;
	void CheckRewardState();
	void StartReward(const TArray<b2network::B2RewardPtr>& Rewards);
	void InitConnectRewardPart();
	void UpdateRewardState();
	virtual void UpdateStaticText() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBTN_Recieve();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	void FinishRewardAnim();
	void PopUpConnectReward(const TArray<b2network::B2RewardPtr>& Rewards);
	void RenderProgressBar(const float PlayTime);
	TWeakObjectPtr<class UB2UIConnectRewardPart>* FindRewardItem(const b2network::B2RewardPtr& RewardItem);

private:
	bool   IsWatingReward;
	bool   CalculateProgressBar;
	float  MaxRewardPlayTime;

	TArray<issue_ptr> Issues;

	TArray<TWeakObjectPtr<class UB2UIConnectRewardPart>> ConnectRewardParts;
	TArray<b2network::B2RewardPtr>   SavedRewards;
	TWeakObjectPtr<UB2Button>		 BTN_Recieve;
	TWeakObjectPtr<UImage>			 IMG_ProgressBar;
	TWeakObjectPtr<UImage>		     IMG_ButtonEffect;
	TWeakObjectPtr<UTextBlock>		 TB_Receive;
	TWeakObjectPtr<UB2RichTextBlock> TB_TimeMessage;
	TWeakObjectPtr<UB2RichTextBlock> TB_CumulativeTime;
};
