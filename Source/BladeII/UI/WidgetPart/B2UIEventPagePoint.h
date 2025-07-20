// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "../B2RichTextBlock.h"
#include "B2UIEventPagePoint.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPagePoint : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void SubscribeEvents_EventPage();

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	void ResponsePointEventReward(FB2ResponseReceivePointEventRewardPtr ReceivePointEventReward);

private:
	void InitEventPointInfo();
	void InitEventPointConditionInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventPointConditionPart>	EventPointConditionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 iHorizontalMaxIndex;
	
private:
	TWeakObjectPtr<UVerticalBox> VB_EventPointCondition;

	TWeakObjectPtr<UScrollBox> SB_PointEventCondition;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_AccruePointTitle;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_AccurePointMethod;
};
