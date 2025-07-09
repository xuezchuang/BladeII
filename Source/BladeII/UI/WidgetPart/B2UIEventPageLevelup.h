// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIEventPage.h"
#include "B2UIEventPageLevelup.generated.h"


UCLASS()
class BLADEII_API UB2UIEventPageLevelup : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void SubscribeEvents_EventPage();

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	void ResponseLevelupEventReward(FB2ResponseReceiveLevelUpEventRewardPtr ReceiveLevelUpEventReward);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotLevelup>		EventSlotClass;

private:
	TWeakObjectPtr<UVerticalBox>	VB_EventList;
};
