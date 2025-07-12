// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "B2UIDonationEvent.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationEvent : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
private:
	void SetRemainingTime();
	void UpdateUIInfo();
	void UpdateBottomUIInfo();
	void OpenPopup(int32 InStep);
	void InitDonationRemainingTimer();
	void EndDonationTimer();
	void InitRefreshTimer();
	void EndRefreshTimer();
	void SetRefreshTime();
	bool IsRefresh();
	void InitUIP();
	void InitRankingRewardPopup();

	UFUNCTION()
		void OnClickTenSummon();
	UFUNCTION()
		void OnClickOneSummon();
	UFUNCTION()
		void OnClickRefresh();
	UFUNCTION()
		void OnClickRewardList();

private:
	TArray<TWeakObjectPtr<class UB2UIDonationPointStepReward>> UIP_StepRewards;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_RemainingText;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_RemainingTime;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MyInfo;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MyRankingText;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MyRanking;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MyPointText;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_MyPoint;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Refresh;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_RewardList;
	TWeakObjectPtr<UTextBlock>TB_TenSummon;
	TWeakObjectPtr<UTextBlock>TB_OneSummon;
	TWeakObjectPtr<UTextBlock>TB_GiftFreeGuide;
	TWeakObjectPtr<UB2Button>BTN_TenSummon;
	TWeakObjectPtr<UB2Button>BTN_OneSummon;
	TWeakObjectPtr<UB2Button>BTN_Refresh;
	TWeakObjectPtr<UB2Button>BTN_RewardList;
	TWeakObjectPtr<class UB2UIDonationRankingScroll> UIP_RankingScroll;
	TWeakObjectPtr<class UB2UIDonationProductCost> UIP_OneSummonCost;
	TWeakObjectPtr<class UB2UIDonationProductCost> UIP_TenSummonCost;
	TWeakObjectPtr<class UB2UIDonationPointStepRewardPopup> UIP_DonationPointStepRewardPopup;
	TWeakObjectPtr<class UB2UIDonationRankingRewardPopup> UIP_DonationRankingRewardPopup;
	FDateTime EventEndTime;
	FTimerHandle RemainingTimeUpdate;
	TArray<issue_ptr>	Issues;
	TWeakObjectPtr<UCanvasPanel> CP_EventEnd;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_EndEvent;
	FTimerHandle RefreshTimer;
	int32 RefreshTime = 0;
	const int32 MAXREFRESHTIME = 3;
};