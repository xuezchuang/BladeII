// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2Image.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "EventSubsystem.h"
#include "B2UISeasonEvent.generated.h"

struct tSeasonEventDayInfo
{
	TArray<FSeasonEventSlotInfo>	ArrSeasonEventSlotInfo;
};

UCLASS()
class BLADEII_API UB2UISeasonEvent : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void DoMarkRedDot() final;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	void SubscribeEvent();
	void UnSubscribeEvent();
	void InitEventIDSeasonEvent();
	void InitSeasonEventMasterData();
	void InitSeasonEventTabs();
	void InitSeasonEventSlots(int32 iSelectedSeasonEventDay);
	void InitBannerImage();

protected:
	UFUNCTION()
	void OnClickBTN_GetFinalReward();

	bool CheckFinalEventSlot(b2network::B2SeasonMissionInfoPtr SeasonMission);

protected:
	UFUNCTION()
	void OnSucessedDownloadBanner(class UTexture2DDynamic* pTexture);

	UFUNCTION()
	void OnFailedDownloadBanner(class UTexture2DDynamic* pTexture);

public:
	void UpdateSeasonEventState(int32 iSelectedSeasonEventDay);
	void UpdateSeasonEventTabs(int32 iSelectedSeasonEventDay);
	void UpdateSeasonEventSlots(int32 iSelectedSeasonEventDay);
	void UpdateSeasonEventGetFinalReward();
	void UpdateSeasonEventSlotState(b2network::B2SeasonMissionInfoPtr SeasonMission);
	void UpdateTitleAndPeriodText();

	void GoToLinkScene(int32 iSeasonEventDay, int32 iSeasonEventSlotIndex);

public:
	void ResponseGetSeasonMission(FB2ResponseGetSeasonMissionPtr GetSeasonMission);
	void ResponseRewardSeasonMission(FB2ResponseRewardSeasonMissionPtr RewardSeasonMission);

public:
	int32 GetSeasonEventFinalDay();
	int32 GetSeasonEventSlotMaxIndex();
	int32 GetSeasonEventProgressDay();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UISeasonEventTab>		SeasonEventTabPartClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UISeasonEventSlot>		SeasonEventSlotClass;

private:
	TWeakObjectPtr<UHorizontalBox>						HB_SeasonEventTabList;

	TWeakObjectPtr<UVerticalBox>						VB_SeasonEventMainSlot;

	TWeakObjectPtr<UVerticalBox>						VB_SeasonEventSlotList;

	TWeakObjectPtr<UB2Button>							BTN_GetFinalReward;

	TWeakObjectPtr<UTextBlock>							TB_GetFinalReward;

	TWeakObjectPtr<UTextBlock>							TB_CompleteReward;

	TWeakObjectPtr<UProgressBar>						PB_Progress;

	TWeakObjectPtr<UTextBlock>							TB_ProgressCount;

	TArray<TWeakObjectPtr<class UB2UISeasonEventTab>>	ArrSeasonEventTab;

	TArray<TWeakObjectPtr<class UB2UISeasonEventSlot>>	ArrSeasonEventSlot;

	TArray<tSeasonEventDayInfo>							ArrSeasonEventDayInfo;

	TArray<b2network::B2SeasonMissionEventStatus>		ArrSeasonEventStatus;

	FSeasonEventSlotInfo								FinalSeasonEventSlotInfo;

	int32 iChacedLastSelectedSeasonEventDay = 1;

	int32						SeasonEventID;

	TWeakObjectPtr<UTextBlock>							TB_EventTitle;
	TWeakObjectPtr<UTextBlock>							TB_EventPeriod;

private:
	UPROPERTY(Transient)
	class UAsyncTaskDownloadImage*	ImageDownloader;

	TWeakObjectPtr<UB2Image>			IMG_Banner;
	FString							strCahcedBannerUrl;
	int32							iTryCountDownBannerImage = 3;

private:
	bool						bSubscribed;
	TArray<issue_ptr>			Issues;
};
