// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "CommonStruct.h"
#include "EventSubsystem.h"

typedef int32 iEVENT_ID;

class BLADEII_API RewardEventManager
{
public:
	~RewardEventManager();

	static RewardEventManager& GetInstance();

public:
	void Init();
	void SubscribeEvents();
	void UnsubscribeEvents();

public:
	template<class TPtr, class T>
	void InitInfoFromMD(const TArray<TPtr>& EventInfos, TMap<iEVENT_ID, TArray<T>>& TMapEventInfo);

	void InitEventFromMD(const FB2MasterDatas& InMasterDatas);
	void InitEventRewardInfoFromMD(const FB2MasterDatas& InMasterDatas);
	void InitEventConditionInfoFromMD(const FB2MasterDatas& InMasterDatas);
	void InitEventDiceInfoFromMD(const FB2MasterDatas& InMasterDatas);

	void UpdateEnableEventInfo();
	void UpdateEnableEventPageInfo();
	void UpdateLatestEventInfo(FB2ResponseGetLatestEventDataPtr ResponseEventData);

public:
	bool HasEnableEvent();
	bool HasEnableEventPage();
	bool IsForUseEventPage(const b2network::B2mdEventInfo& rEventInfo);
	bool IsEnableEventCategory(const int32 iEventCategory);
	bool IsEnableEventID(const int32 iEventID);

public:
	void RequestLatestEventInfo();
	void RequestGetEventStatus();
	void RequestGemSpendEventGemCount();
	void RequestSeasonEventStatus();

public:
	void ResponseLatestEventInfo(FB2ResponseGetLatestEventDataPtr ReciveEventData);
	void ResponseGetEvent(FB2ResponseGetEventPtr ReciveGetEventStatus);
	void ResponseAttendSeasonMission(const FB2ResponseAttendSeasonMissionPtr& ReciveSeasonEventStatus);

public:	//set
	

public: //get
	const FB2ResponseGetEventPtr&			GetCahcedGetEventStatus() { return CahcedGetEventStatus; }
	const TArray<b2network::B2mdEventInfo>&	GetArrayEnableEventPageInfo() { return ArrayEnableEventPageInfo; }
	const TArray<b2network::B2mdEventInfo>& GetArrayEnableEventInfo() { return ArrayEnableEventInfo; }
	const b2network::B2mdEventInfo*			GetEnableEventInfo(const int32 iEventID);
	const b2network::B2mdEventInfo*			GetEnableEventInfoFromCategory(const int32 iEventCategory);
	const int32								GetEnableEventIDFromCategory(const int32 iEventCategory);
	const TArray<b2network::B2mdEventAttendance>*			GetArrayEventInfo_Attendance(const int32 iEventID);
	const TArray<b2network::B2SeasonMissionEventStatus>*	GetArrayEventStatusInfo_Season(const int32 iEventID);
	
	//EventRewardInfo
	const TArray<b2network::B2mdPointEventReward>*			GetArrayEventRewardInfo_Point(const int32 iEventID);
	const TArray<b2network::B2mdLevelUpEvent>*				GetArrayEventRewardInfo_Levelup(const int32 iEventID);
	const TArray<b2network::B2mdStageClearEvent>*			GetArrayEventRewardInfo_StageClear(const int32 iEventID);
	const TArray<b2network::B2mdGemPurchaseEventReward>*	GetArrayEventRewardInfo_GemPurchase(const int32 iEventID);
	const TArray<b2network::B2mdSpendGemEvent>*				GetArrayEventRewardInfo_GemSpend(const int32 iEventID);
	const TArray<b2network::B2mdTenLotteryEvent>*			GetArrayEventRewardInfo_TenLottery(const int32 iEventID);
	const TArray<b2network::B2mdSeasonMissionEvent>*		GetArrayEventRewardInfo_Season(const int32 iEventID);
	const TArray<b2network::B2mdTenLotterySavingsEvent>*	GetArrayEventRewardInfo_TenLotterySaving(const int32 iEventID);

	//EventConditionInfo
	const TArray<b2network::B2mdPointEvent>*				GetArrayEventConditionInfo_Point(const int32 iEventID);

	//HotTime
	const TArray<b2network::B2mdHotTimeEvent>*				GetArrayHotTimeEventInfo(const int32 iEventID);

	//EventDiceInfo
	const TArray<b2network::B2mdDiceEventMission>*			GetArrayEventDiceInfo_Mission(const int32 iEventID);
	const TArray<b2network::B2mdDiceEventCell>*				GetArrayEventDiceInfo_Cell(const int32 iEventID);
	const TArray<b2network::B2mdDiceEventFinishReward>*		GetArrayEventDiceInfo_FinishReward(const int32 iEventID);
	const TArray<b2network::B2mdDiceEventBuyPoint>*			GetArrayEventDiceInfo_BuyPoint(const int32 iEventID);
	const TArray<b2network::B2mdDiceEventMileage>*			GetArrayEventDiceInfo_Mileage(const int32 iEventID);

private:
	RewardEventManager();
	RewardEventManager(RewardEventManager const&) = delete;
	RewardEventManager operator = (RewardEventManager const&) = delete;

private: //fixeddata

	//EventRewardInfo
	TMap<iEVENT_ID, TArray<b2network::B2mdPointEventReward>>		TMapEventRewardInfo_Point;
	TMap<iEVENT_ID, TArray<b2network::B2mdLevelUpEvent>>			TMapEventRewardInfo_Levelup;
	TMap<iEVENT_ID, TArray<b2network::B2mdStageClearEvent>>			TMapEventRewardInfo_StageClear;
	TMap<iEVENT_ID, TArray<b2network::B2mdGemPurchaseEventReward>>	TMapEventRewardInfo_GemPurchase;
	TMap<iEVENT_ID, TArray<b2network::B2mdSpendGemEvent>>			TMapEventRewardInfo_GemSpend;
	TMap<iEVENT_ID, TArray<b2network::B2mdTenLotteryEvent>>			TMapEventRewardInfo_TenLottery;
	TMap<iEVENT_ID, TArray<b2network::B2mdSeasonMissionEvent>>		TMapEventRewardInfo_Season;
	TMap<iEVENT_ID, TArray<b2network::B2mdTenLotterySavingsEvent>>	TMapEventRewardInfo_TenLotterySaving;

	//EventConditionInfo
	TMap<iEVENT_ID, TArray<b2network::B2mdPointEvent>>				TMapEventConditionInfo_Point;

	//EventDiceInfo
	TMap<iEVENT_ID, TArray<b2network::B2mdDiceEventMission>>		TMapEventDiceInfo_Mission;
	TMap<iEVENT_ID, TArray<b2network::B2mdDiceEventCell>>			TMapEventDiceInfo_Cell;
	TMap<iEVENT_ID, TArray<b2network::B2mdDiceEventFinishReward>>	TMapEventDiceInfo_FinishReward;
	TMap<iEVENT_ID, TArray<b2network::B2mdDiceEventBuyPoint>>		TMapEventDiceInfo_BuyPoint;
	TMap<iEVENT_ID, TArray<b2network::B2mdDiceEventMileage>>		TMapEventDiceInfo_Mileage;

private: //dynamicdata
	
	//EventInfo
	int64															EventDataLastUpdateTime;
	TMap<iEVENT_ID, TArray<b2network::B2mdEventInfo>>				TMapAllEventInfo;
	TArray<b2network::B2mdEventInfo>								ArrayEnableEventInfo;
	TArray<b2network::B2mdEventInfo>								ArrayEnableEventPageInfo;

	//EventStatusInfo
	FB2ResponseGetEventPtr											CahcedGetEventStatus;
	TMap<iEVENT_ID, TArray<b2network::B2SeasonMissionEventStatus>>	TMapEventStatusInfo_Season;
	
	//HotTime
	TMap<iEVENT_ID, TArray<b2network::B2mdHotTimeEvent>>			TMapEventInfo_HotTime;

	//EventAttendance
	TMap<iEVENT_ID, TArray<b2network::B2mdEventAttendance>>			TMapEventInfo_Attendance;

private:
	bool															bSubscribed;
	TArray<std::shared_ptr<issue>>									Issues;
};
