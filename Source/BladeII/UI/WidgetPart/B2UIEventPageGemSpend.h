// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "B2UIEventPageGemSpend.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventPageGemSpend : public UB2UIEventPage
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void SubscribeEvents_EventPage();

protected:
	void UpdateSpendGemPointText(const int32 iSpendGemPoint);

public:
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;
	virtual void SortEventSlotList();
	void ResponseGemSpendEventReward(FB2ResponseRewardSpendGemEventPtr ReciveEventReward);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotGemSpend>	EventSlotClass;

private:
	TWeakObjectPtr<UVerticalBox>	VB_EventSlotList;

	TWeakObjectPtr<UB2RichTextBlock> RTB_AccrueGemPurchase;
	TWeakObjectPtr<UB2RichTextBlock> RTB_AccrueGemPoint;
};
