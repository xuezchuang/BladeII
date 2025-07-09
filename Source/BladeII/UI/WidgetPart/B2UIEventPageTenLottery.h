// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "B2UIEventPageTenLottery.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPageTenLottery : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void SubscribeEvents_EventPage();

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	virtual void SortEventSlotList();
	void ResponseTenLotteryEventReward(FB2ResponseRewardTenLotteryEventPtr ReciveEventReward);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotTenLottery>	EventSlotClass;
	
private:
	TWeakObjectPtr<UVerticalBox>				VB_EventSlotList;

};
