// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIModDailyReward.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIModDailyReward : public UB2UIWidget
{
	GENERATED_BODY()

	TWeakObjectPtr<UTextBlock> TB_DailyRewardTitle;
	TWeakObjectPtr<UTextBlock> TB_DailyRewardRequire;

	TArray< TWeakObjectPtr<class UB2UIRewardSlot> > RewardSlotList;
	int32 CurrentSlotIndex;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:

	UB2UIModDailyReward(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	void OpenDailyReward(int32 MatchType);
	void SetRewardSlotFromServerData(MailGiftType RewardType, const FModDailyReward& ServerRewardInfo);

	UFUNCTION()
		void OnClickButtonClose();

protected:
	TWeakObjectPtr<UB2Button>				BTN_Close;
};
