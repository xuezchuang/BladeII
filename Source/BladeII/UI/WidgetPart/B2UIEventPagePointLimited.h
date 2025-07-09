// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetPart/B2UIEventPage.h"
#include "B2UIEventPagePointLimited.generated.h"

UCLASS()
class BLADEII_API UB2UIEventPagePointLimited : public UB2UIEventPage
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	virtual void SubscribeEvents_EventPage();
	virtual void InitEventPageMasterData();
	virtual void UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent) override;

public:
	void ResponsePointShopLimited(FB2ResponseRewardPointShopEventdPtr ReciveEventReward);

private:
	void InitEventTabPointShopLimited();
	void InitEventSlotPointShopLimited();

private:
	void UpdateEventPage_FromTapGroupID(const int32 iTapGroupID);
	void UpdateArrayEventTab_FromTapGroupID(const int32 iTapGroupID);
	void UpdateArrayEventSlot_FromTapGroupID(const int32 iTapGroupID);

	void UpdateAccruePointText(const int32 iAccruePoint);
	void UpdateArrayEventSlotRewardedCount(const int32 iIndex, const int32 iRewardedCount);

	void UpdateEventPage_FromAccruePoint(const int32 iAccruePoint);
	void UpdateArrayEventSlot_FromAccruePoint(const int32 iAccruePoint);

private:
	void OpenPointShopLimitedExplainPopup();

public:
	UFUNCTION()
	void OnClickBTN_Explain();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventTabPointShopLimited>	EventTabPointShopLimitedClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIEventSlotPointShopLimited>	EventSlotPointShopLimitedClass;

private:
	TWeakObjectPtr<UHorizontalBox>			HB_TabList;
	TWeakObjectPtr<UB2ScrollBox>			SB_EventSlotList;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_AccruePoint;

	TWeakObjectPtr<UB2Button>				BTN_Explain;
	TWeakObjectPtr<UB2RichTextBlock>		RTB_ExplainBtn;

	TWeakObjectPtr<class UB2UIPointShopLimitedEventPopup>	pPointShopLimitedEventPopup;

private:
	int32 iSelectedTabGroupID = -1;
	TArray<TWeakObjectPtr<class UB2UIEventTabPointShopLimited>>		ArrayEventTabShopLimited;
};
