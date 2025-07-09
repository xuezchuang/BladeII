// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIChapterStarRewardPreview.h"
#include "MissionManager.h"
#include "B2UIPMissionPointRewardPreview.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIPMissionPointRewardPreview : public UB2UIChapterStarRewardPreview
{
	GENERATED_BODY()

protected:
	virtual void BindDelegates() override;

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OpenPreview(int32 InRewardBoxIndex, bool bInCanGetReward, EMissionType MissionType, int32 RequireMissionPoint);
	void CreateRewardIconCommon(const TArray<b2network::B2RewardPtr>& RewardInfos);

private:
	void ClearDynamicIcons(class UB2UIManager* InUIManager);

	UFUNCTION()
		void OnClickGetRewardBtn();
	
	EMissionType	RequestMissionType;
	int32			RequestRewardIndex;
};
