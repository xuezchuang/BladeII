// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "B2UIEventPageStageClear.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPageStageClear : public UB2UIEventPage
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void SubscribeEvents_EventPage();

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventSlotListState(FB2ResponseGetEventPtr ReciveGetEvent);
	void ResponseStageClearEventReward(FB2ResponseReceiveStageClearEventRewardPtr ReceiveEventReward);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotStageClear>		EventSlotClass;

private:
	TWeakObjectPtr<UVerticalBox>	VB_EventSlotList;
};
