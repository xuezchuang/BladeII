// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UISeasonEventSlot.generated.h"

UCLASS()
class BLADEII_API UB2UISeasonEventSlot : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

protected:
	UFUNCTION()
	void OnClickBTN_GetReward();

	UFUNCTION()
	void OnClickBTN_Link();

	bool CheckDayMainMission();

public:
	void UpdateSeasonEventSlotInfo(const FSeasonEventSlotInfo &EventSlotInfo);

	void UpdateSeasonEventSlot();
	void UpdateSeasonEventMainSlotState();
	void UpdateSeasonEventSlotState();
	void UpdateSeasonEventProgressBar();

	void SetSeasonEventSlotText(FText fSeasonEventText);

private:
	TWeakObjectPtr<UB2Button>				BTN_GetReward;
	TWeakObjectPtr<UTextBlock>				TB_GetReward;

	TWeakObjectPtr<UTextBlock>				TB_CompleteReward;

	TWeakObjectPtr<UB2Button>				BTN_GotoLink;
	TWeakObjectPtr<UTextBlock>				TB_GotoLink;

	TArray<TWeakObjectPtr<UB2RichTextBlock>>	ArrayRTB_SeasonEvent;

	TWeakObjectPtr<UWidgetSwitcher>			WS_SeasonEventList;

	TWeakObjectPtr<UProgressBar>			PB_Progress;
	TWeakObjectPtr<UTextBlock>				TB_ProgressCount;
	TWeakObjectPtr<UTextBlock>				TB_ProgressCountComplete;

	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

private:
	FSeasonEventSlotInfo					fSeasonEventSlotInfo;
};
