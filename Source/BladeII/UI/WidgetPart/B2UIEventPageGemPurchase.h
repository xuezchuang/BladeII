// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "B2UIEventPageGemPurchase.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPageGemPurchase : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void SubscribeEvents_EventPage();

protected:
	void UpdateAccrueGemPointText(const int32 iAccrueGemPoint);

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	virtual void SortEventSlotList();
	void ResponseGemPurchaseEventReward(FB2ResponseReceiveGemPurchaseEventRewardPtr ReciveEventReward);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotGemPurchase>	EventSlotClass;

private:
	TWeakObjectPtr<UVerticalBox>	VB_EventSlotList;

	TWeakObjectPtr<UB2RichTextBlock> RTB_AccrueGemPurchase;
	TWeakObjectPtr<UB2RichTextBlock> RTB_AccrueGemPoint;
};
