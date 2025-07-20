// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "../B2RichTextBlock.h"
#include "B2UIEventPagePointConsume.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPagePointConsume : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void SubscribeEvents_EventPage();
	
public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	void ResponsePointConsumeEventReward(FB2ResponseRewardPointShopEventdPtr ReciveEventReward);

private:
	void InitEventPointConsumeSlotInfo();
	void InitEventPointConsumeConditionInfo();
	void CheckEventSlotListState(int32 iAccuireEventPoint);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventPointConsumeCondition>	EventPointConsumeConditionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotPointConsume>		EventSlotPointConsumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotHB>					EventSlotHBClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 iHorizontalMax;

private:
	TWeakObjectPtr<UB2RichTextBlock>		RTB_AccruePointTitle;

	TWeakObjectPtr<UB2ScrollBox>			SB_ConditionList;
	TWeakObjectPtr<UB2ScrollBox>			SB_EventSlotHBList;
	

	TWeakObjectPtr<UB2RichTextBlock>		RTB_EventConditionExplain;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_EventAcquirePointCount;
};
