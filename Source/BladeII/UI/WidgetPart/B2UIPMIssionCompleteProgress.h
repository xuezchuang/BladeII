// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "MissionManager.h"
#include "B2UIPMIssionCompleteProgress.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPMIssionCompleteProgress : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void UpdateInfo(struct FMissionTab& MissionTab, EMissionType MissionType);
	void OnReceiveMissionPointReward(const FB2ReceiveCompleteMissionReward& CompleteReward);

protected:
	virtual void BindDelegates() override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

	void OnClickRewardBox(int32 RewardBoxIndex);

	void UpdateInfoGetAllButton(EMissionType MissionType);
private:
	UFUNCTION()
		void OnClickRewardBox1();
	UFUNCTION()
		void OnClickRewardBox2();
	UFUNCTION()
		void OnClickRewardBox3();
	UFUNCTION()
		void OnClickGetAllReward();

	void SubscribeEvent();
	void UnSubscribeEvent();

	void UpdateRewardBoxInfo(TWeakObjectPtr<class UB2UIChapterStarRewardBox> PrevWidget, TWeakObjectPtr<class UB2UIChapterStarRewardBox> RewardWidget, MissionPointRewardPtr RewardPtr);

	TWeakObjectPtr<class UB2UIChapterStarRewardBox>						UIP_RewardBox1;
	TWeakObjectPtr<class UB2UIChapterStarRewardBox>						UIP_RewardBox2;
	TWeakObjectPtr<class UB2UIChapterStarRewardBox>						UIP_RewardBox3;
	TWeakObjectPtr<UB2Button>											BTN_GetAllReward;
	TWeakObjectPtr<UTextBlock>											TB_CompleteProgress;
	TWeakObjectPtr<UTextBlock>											TB_GetAllReward;
	TWeakObjectPtr<UPanelWidget>										CP_CompleteProgress;
	TWeakObjectPtr<UPanelWidget>										CP_ProgressParts;
	TWeakObjectPtr<class UB2UIPMissionPointRewardPreview>				UIP_RewardPreview;
	EMissionType														SelectedMissionType;
	uint8																DeliveryReceiveCompleteMissionRewardTicket;
};
