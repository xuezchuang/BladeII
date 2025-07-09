// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIGuildRewardBoxProgress.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildRewardBoxProgress : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void RegisterUIMarkForRedDot() final;

public:
	void SetGuildReward(int32 Grade, int32 CurrentMedal, int32 MaxMedal,int32 NextMedal);
	void SetRewardComplete(bool IsReward);
	void SetGuildRewardText(FText InText);

	bool HasRedDot_GuildReward();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void EventGradeBoxIcon(int32 Grade);

protected:
	UFUNCTION()
	void OnClickBTN_RewardClick();

private:
	TWeakObjectPtr<UProgressBar>	PB_MedalProgress;
	TWeakObjectPtr<UB2RichTextBlock>		TB_NextMedal;
	TWeakObjectPtr<UB2RichTextBlock>		TB_MedalProgress;
	TWeakObjectPtr<UB2Button>		BTN_RewardClick;
	TWeakObjectPtr<UPanelWidget>	CP_ProgressReward;
	TWeakObjectPtr<UPanelWidget>	CP_GetReward;

	TWeakObjectPtr<UB2UIWidgetBase>	Reddot_GuilldReward;

	int32 CurrentMedalCount;

	bool RewardState;

};
