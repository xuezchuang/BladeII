// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "B2UIEventPageTenLotterySaving.generated.h"

UCLASS()
class BLADEII_API UB2UIEventPageTenLotterySaving : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void SubscribeEvents_EventPage();

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII EventPageTenLotterySaving")
	void RepeatRewardPossibleAnimation_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII EventPageTenLotterySaving")
	void StopRewardPossibleAnimation_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII EventPageTenLotterySaving")
	void StartTransparencyBlock_BP();
	
private:
	void InitEventSlotList(const b2network::B2mdTenLotterySavingsEvent& EventRewardInfo);
	void InitRewardIconList(const b2network::B2mdTenLotterySavingsEvent& EventRewardInfo);

	UFUNCTION()
	void OnClickBTN_GetReward();

	UFUNCTION()
	void OnClickBTN_GotoEquipShop();

	void ResponseRewardTenLotterySavingsEvent(FB2ResponseRewardTenLotterySavingsEventPtr ReciveEventReward);

private:
	void UpdateEventPageFromLotteryCount(const int32 CurrentLotteryCount);
	void UpdateRewardPossibleAnimation(const int32 CurrentLotteryCount);
	void UpdateCurrentLotteryCountText(const int32 CurrentLotteryCount);
	void UpdateBTN_GetRewardState(const int32 CurrentLotteryCount);
	void UpdateSlotListState(const int32 CurrentLotteryCount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotHB>					EventSlotHBClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotTenLotterySaving>	EventSlotTenLotterySavingClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32												MAX_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIRewardIcon>					RewardIconClass;

private:
	TWeakObjectPtr<UVerticalBox>	VB_EventSlot;

	TWeakObjectPtr<UHorizontalBox>			HB_RewardIconList;

	TWeakObjectPtr<UB2Button>				BTN_GetReward;
	TWeakObjectPtr<UTextBlock>				TB_GetRewardBtn;

	TWeakObjectPtr<UTextBlock>				TB_ExplainEvent;
	TWeakObjectPtr<UTextBlock>				TB_ExplainPoint;
	TWeakObjectPtr<UTextBlock>				TB_CurrentLotteryCount;

	TWeakObjectPtr<UOverlay>				O_EffectRewardPossible;

	TWeakObjectPtr<UB2Button>				BTN_GotoEquipShop;
	TWeakObjectPtr<UTextBlock>				TB_GotoEquipShop;

private:
	b2network::B2mdTenLotterySavingsEvent	ChachedEventRewardInfo;
};
