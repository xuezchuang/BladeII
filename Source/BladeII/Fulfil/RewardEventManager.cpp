// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "RewardEventManager.h"
#include "B2UIDocDonation.h"
#include "B2GameInstance.h"
#include "Retailer.h"
#include "Event.h"
#include "BladeIIUtil.h"
#include "B2UIDocHelper.h"

RewardEventManager::RewardEventManager()
{
	
}

RewardEventManager::~RewardEventManager()
{
	UnsubscribeEvents();
}

RewardEventManager & RewardEventManager::GetInstance()
{
	static RewardEventManager instance;
	return instance;
}

void RewardEventManager::Init()
{
	SubscribeEvents();
}

void RewardEventManager::SubscribeEvents()
{
	if (bSubscribed == false)
	{
		UnsubscribeEvents();

		bSubscribed = true;

		Issues.Add(DeliveryGetLatestEventDataClass<FB2ResponseGetLatestEventDataPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseGetLatestEventDataPtr ReciveEventData)
		{
			this->ResponseLatestEventInfo(ReciveEventData);
		}
		));

		Issues.Add(DeliveryGetEventClass<FB2ResponseGetEventPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseGetEventPtr ResponseGetEvent)
		{
			this->ResponseGetEvent(ResponseGetEvent);
		}
		));

		Issues.Add(BuyHotTimeBuffSuccessfulClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->RequestGemSpendEventGemCount();
		}
		));

		Issues.Add(DeliveryAttendSeasonMissionClass<FB2ResponseAttendSeasonMissionPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseAttendSeasonMissionPtr ReciveSeasonEventStatus)
		{
			this->ResponseAttendSeasonMission(ReciveSeasonEventStatus);
		}
		));
	}
}

void RewardEventManager::UnsubscribeEvents()
{
	Issues.Empty();
}

template<class TPtr, class T>
void RewardEventManager::InitInfoFromMD(const TArray<TPtr>& EventInfos, TMap<int32, TArray<T>>& TMapEventInfo)
{
	TMapEventInfo.Empty();

	const int32 iEventInfoMax = EventInfos.Num();
	for (int32 i = 0; i < iEventInfoMax; ++i)
	{
		if (EventInfos.IsValidIndex(i) == false)
		{
			continue;
		}

		TArray<T>& ArrayEventInfo = TMapEventInfo.FindOrAdd(EventInfos[i]->event_id);
		ArrayEventInfo.Add(*EventInfos[i]);
	}
}

void RewardEventManager::InitEventFromMD(const FB2MasterDatas& InMasterDatas)
{
	InitEventRewardInfoFromMD(InMasterDatas);
	InitEventConditionInfoFromMD(InMasterDatas);
	InitEventDiceInfoFromMD(InMasterDatas);

	//이벤트 출석 등 빠른 시점에 response md 필요
	RequestLatestEventInfo();
}

void RewardEventManager::InitEventRewardInfoFromMD(const FB2MasterDatas& InMasterDatas)
{
	//InitInfoFromMD<b2network::B2mdPointEventRewardPtr, b2network::B2mdPointEventReward>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_point_event_rewards_index, InMasterDatas),
	//	TMapEventRewardInfo_Point);

	//InitInfoFromMD<b2network::B2mdLevelUpEventPtr, b2network::B2mdLevelUpEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_level_events_index, InMasterDatas),
	//	TMapEventRewardInfo_Levelup);

	//InitInfoFromMD<b2network::B2mdStageClearEventPtr, b2network::B2mdStageClearEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_stage_clear_events_index, InMasterDatas),
	//	TMapEventRewardInfo_StageClear);

	//InitInfoFromMD<b2network::B2mdGemPurchaseEventRewardPtr, b2network::B2mdGemPurchaseEventReward>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_gem_purchase_event_rewards_index, InMasterDatas),
	//	TMapEventRewardInfo_GemPurchase);

	//InitInfoFromMD<b2network::B2mdSpendGemEventPtr, b2network::B2mdSpendGemEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_spend_gem_event_rewards_index, InMasterDatas),
	//	TMapEventRewardInfo_GemSpend);

	//InitInfoFromMD<b2network::B2mdTenLotteryEventPtr, b2network::B2mdTenLotteryEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_ten_lottery_events_index, InMasterDatas),
	//	TMapEventRewardInfo_TenLottery);

	//InitInfoFromMD<b2network::B2mdSeasonMissionEventPtr, b2network::B2mdSeasonMissionEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_season_mission_events_index, InMasterDatas),
	//	TMapEventRewardInfo_Season);

	//InitInfoFromMD<b2network::B2mdTenLotterySavingsEventPtr, b2network::B2mdTenLotterySavingsEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_ten_lottery_savings_events_index, InMasterDatas),
	//	TMapEventRewardInfo_TenLotterySaving);
}

void RewardEventManager::InitEventConditionInfoFromMD(const FB2MasterDatas& InMasterDatas)
{
	//InitInfoFromMD<b2network::B2mdPointEventPtr, b2network::B2mdPointEvent>(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_point_events_index, InMasterDatas),
	//	TMapEventConditionInfo_Point);
}

void RewardEventManager::InitEventDiceInfoFromMD(const FB2MasterDatas& InMasterDatas)
{
	/*InitInfoFromMD<b2network::B2mdDiceEventMissionPtr, b2network::B2mdDiceEventMission>(
		GET_TUPLE_DATA(FB2ResponseMasterDatas::md_dice_event_missions_index, InMasterDatas),
		TMapEventDiceInfo_Mission);

	InitInfoFromMD<b2network::B2mdDiceEventCellPtr, b2network::B2mdDiceEventCell>(
		GET_TUPLE_DATA(FB2ResponseMasterDatas::md_dice_event_cells_index, InMasterDatas),
		TMapEventDiceInfo_Cell);

	InitInfoFromMD<b2network::B2mdDiceEventFinishRewardPtr, b2network::B2mdDiceEventFinishReward>(
		GET_TUPLE_DATA(FB2ResponseMasterDatas::md_dice_event_finish_rewards_index, InMasterDatas),
		TMapEventDiceInfo_FinishReward);

	InitInfoFromMD<b2network::B2mdDiceEventBuyPointPtr, b2network::B2mdDiceEventBuyPoint>(
		GET_TUPLE_DATA(FB2ResponseMasterDatas::md_dice_event_buy_points_index, InMasterDatas),
		TMapEventDiceInfo_BuyPoint);

	InitInfoFromMD<b2network::B2mdDiceEventMileagePtr, b2network::B2mdDiceEventMileage>(
		GET_TUPLE_DATA(FB2ResponseMasterDatas::md_dice_event_mileages_index, InMasterDatas),
		TMapEventDiceInfo_Mileage);*/
}

void RewardEventManager::UpdateEnableEventInfo()
{
	ArrayEnableEventInfo.Empty();

	FDateTime DateNow = UB2GameInstance::GetLocalNow();
	const int32 ArrayEventUseOnlyThisIndex = 0;

	for (auto& Elem : TMapAllEventInfo)
	{
		TArray<b2network::B2mdEventInfo>& rEventInfo = Elem.Value;

		FDateTime StartTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(rEventInfo[ArrayEventUseOnlyThisIndex].start_time));
		FDateTime EndTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(rEventInfo[ArrayEventUseOnlyThisIndex].end_time));

		if (DateNow > StartTime && DateNow < EndTime)
		{
			ArrayEnableEventInfo.Add(rEventInfo[ArrayEventUseOnlyThisIndex]);
		}
	}
}

void RewardEventManager::UpdateEnableEventPageInfo()
{
	ArrayEnableEventPageInfo.Empty();

	int32 EnableEventInfoMax = ArrayEnableEventInfo.Num();
	for (int32 i = 0; i < EnableEventInfoMax; ++i)
	{
		if (ArrayEnableEventInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		if (IsForUseEventPage(ArrayEnableEventInfo[i]))
		{
			ArrayEnableEventPageInfo.Add(ArrayEnableEventInfo[i]);
		}
	}

	ArrayEnableEventPageInfo.Sort([](const b2network::B2mdEventInfo& A, const b2network::B2mdEventInfo& B) { return A.tab_order < B.tab_order; });
}

void RewardEventManager::UpdateLatestEventInfo(FB2ResponseGetLatestEventDataPtr ResponseEventData)
{
	if (ResponseEventData == nullptr)
	{
		return;
	}

	EventDataLastUpdateTime = ResponseEventData->last_update_time;

	InitInfoFromMD<b2network::B2mdEventInfoPtr, b2network::B2mdEventInfo>(
		ResponseEventData->md_event_infos,
		TMapAllEventInfo);

	InitInfoFromMD<b2network::B2mdHotTimeEventPtr, b2network::B2mdHotTimeEvent>(
		ResponseEventData->md_hot_time_events,
		TMapEventInfo_HotTime);

	InitInfoFromMD<b2network::B2mdEventAttendancePtr, b2network::B2mdEventAttendance>(
		ResponseEventData->md_event_attendances,
		TMapEventInfo_Attendance);

	//기부 이벤트는 따로 작업
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		DocDonation->SetDonationEventBaseData(ResponseEventData->md_donation_event_infos);
	}

	UpdateEnableEventInfo();
	UpdateEnableEventPageInfo();

	GetLatestEventInfoSuccessfulClass<>::GetInstance().Signal();
}

bool RewardEventManager::HasEnableEvent()
{
	bool IsEnableEvent = true;

	if (ArrayEnableEventInfo.Num() <= 0)
	{
		IsEnableEvent = false;
	}

	return IsEnableEvent;
}

bool RewardEventManager::HasEnableEventPage()
{
	bool IsEnableEvent = true;

	if (ArrayEnableEventPageInfo.Num() <= 0)
	{
		IsEnableEvent = false;
	}

	return IsEnableEvent;
}

bool RewardEventManager::IsForUseEventPage(const b2network::B2mdEventInfo& rEventInfo)
{
	bool bIsEventPage = true;

	const int32 NotEventPageEvent = 0;

	if (rEventInfo.tab_order == NotEventPageEvent)
	{
		bIsEventPage = false;
	}
	else if (rEventInfo.event_category == b2network::B2EventCategory::HOT_TIME_BUFF)
	{
		bIsEventPage = false;
	}

	return bIsEventPage;
}

bool RewardEventManager::IsEnableEventCategory(const int32 iEventCategory)
{
	bool IsEnable = false;

	int32 EnableEventInfoMax = ArrayEnableEventInfo.Num();
	for (int32 i = 0; i < EnableEventInfoMax; ++i)
	{
		if (ArrayEnableEventInfo.IsValidIndex(i) == false)
		{
			continue;
		}
		
		if (ArrayEnableEventInfo[i].event_category == iEventCategory)
		{
			IsEnable = true;
			break;
		}
	}

	return IsEnable;
}

bool RewardEventManager::IsEnableEventID(const int32 iEventID)
{
	bool IsEnable = false;

	int32 EnableEventInfoMax = ArrayEnableEventInfo.Num();
	for (int32 i = 0; i < EnableEventInfoMax; ++i)
	{
		if (ArrayEnableEventInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayEnableEventInfo[i].event_id == iEventID)
		{
			IsEnable = true;
			break;
		}
	}

	return IsEnable;
}

void RewardEventManager::RequestLatestEventInfo()
{
	data_trader::Retailer::GetInstance().RequestGetLatestEventData(EventDataLastUpdateTime);
}

void RewardEventManager::RequestGetEventStatus()
{
	if (HasEnableEvent())
	{
		data_trader::Retailer::GetInstance().RequestGetEvent();
	}
}

void RewardEventManager::RequestGemSpendEventGemCount()
{
	data_trader::Retailer::GetInstance().RequestGetEvent();
}

void RewardEventManager::RequestSeasonEventStatus()
{
	data_trader::Retailer::GetInstance().RequestAttendSeasonMission();
}

void RewardEventManager::ResponseLatestEventInfo(FB2ResponseGetLatestEventDataPtr ReciveEventData)
{
	if(ReciveEventData == nullptr)
	{
		return;
	}

	bool bUpdateEventInfo = false;

	if (ReciveEventData->last_update_time)
	{
		if (EventDataLastUpdateTime != ReciveEventData->last_update_time)
		{
			bUpdateEventInfo = true;
		}
	}
	else
	{
		if (EventDataLastUpdateTime == 0)
		{
			//waring
			//최악의 경우 : 처음 RequestGetLatestEventData 했는데 empty온경우
			//이 경우 이벤트 페이지 진입시 진행중이 이벤트가 없다고 나올것
		}
	}

	if (bUpdateEventInfo)
	{
		UpdateLatestEventInfo(ReciveEventData);
	}
}

void RewardEventManager::ResponseGetEvent(FB2ResponseGetEventPtr ReciveGetEventStatus)
{
	CahcedGetEventStatus = ReciveGetEventStatus;

	GetEventStatusSuccessfulClass<>::GetInstance().Signal();
}

void RewardEventManager::ResponseAttendSeasonMission(const FB2ResponseAttendSeasonMissionPtr& ReciveSeasonEventStatus)
{
	TMapEventStatusInfo_Season.Empty();

	for (auto EventStatusInfo : ReciveSeasonEventStatus->event_statuses)
	{
		TArray<b2network::B2SeasonMissionEventStatus>& ArrayEventInfo = TMapEventStatusInfo_Season.FindOrAdd(EventStatusInfo->event_id);
		ArrayEventInfo.Add(*EventStatusInfo);
	}

	GetEventStatusSuccessfulClass<>::GetInstance().Signal();
}

const b2network::B2mdEventInfo* RewardEventManager::GetEnableEventInfo(const int32 iEventID)
{
	b2network::B2mdEventInfo* pEventInfo = nullptr;

	int32 EnableEventInfoMax = ArrayEnableEventInfo.Num();
	for (int32 i = 0; i < EnableEventInfoMax; ++i)
	{
		if (ArrayEnableEventInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayEnableEventInfo[i].event_id == iEventID)
		{
			pEventInfo = &(ArrayEnableEventInfo[i]);
			break;
		}
	}

	return pEventInfo;
}

const b2network::B2mdEventInfo* RewardEventManager::GetEnableEventInfoFromCategory(const int32 iEventCategory)
{
	b2network::B2mdEventInfo* pEventInfo = nullptr;

	int32 EnableEventInfoMax = ArrayEnableEventInfo.Num();
	for (int32 i = 0; i < EnableEventInfoMax; ++i)
	{
		if (ArrayEnableEventInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayEnableEventInfo[i].event_category == iEventCategory)
		{
			pEventInfo = &(ArrayEnableEventInfo[i]);
			break;
		}
	}

	return pEventInfo;
}

const int32 RewardEventManager::GetEnableEventIDFromCategory(const int32 iEventCategory)
{
	int32 EventID = 0;

	if (const b2network::B2mdEventInfo* pEventInfo = GetEnableEventInfoFromCategory(iEventCategory))
	{
		EventID = pEventInfo->event_id;
	}

	return EventID;
}

const TArray<b2network::B2mdEventAttendance>* RewardEventManager::GetArrayEventInfo_Attendance(const int32 iEventID)
{
	return TMapEventInfo_Attendance.Find(iEventID);
}

const TArray<b2network::B2SeasonMissionEventStatus>* RewardEventManager::GetArrayEventStatusInfo_Season(const int32 iEventID)
{
	return TMapEventStatusInfo_Season.Find(iEventID);
}

const TArray<b2network::B2mdPointEventReward>* RewardEventManager::GetArrayEventRewardInfo_Point(const int32 iEventID)
{
	return TMapEventRewardInfo_Point.Find(iEventID);
}

const TArray<b2network::B2mdLevelUpEvent>* RewardEventManager::GetArrayEventRewardInfo_Levelup(const int32 iEventID)
{
	return TMapEventRewardInfo_Levelup.Find(iEventID);
}

const TArray<b2network::B2mdStageClearEvent>* RewardEventManager::GetArrayEventRewardInfo_StageClear(const int32 iEventID)
{
	return TMapEventRewardInfo_StageClear.Find(iEventID);
}

const TArray<b2network::B2mdGemPurchaseEventReward>* RewardEventManager::GetArrayEventRewardInfo_GemPurchase(const int32 iEventID)
{
	return TMapEventRewardInfo_GemPurchase.Find(iEventID);
}

const TArray<b2network::B2mdSpendGemEvent>* RewardEventManager::GetArrayEventRewardInfo_GemSpend(const int32 iEventID)
{
	return TMapEventRewardInfo_GemSpend.Find(iEventID);
}

const TArray<b2network::B2mdTenLotteryEvent>* RewardEventManager::GetArrayEventRewardInfo_TenLottery(const int32 iEventID)
{
	return TMapEventRewardInfo_TenLottery.Find(iEventID);
}

const TArray<b2network::B2mdSeasonMissionEvent>* RewardEventManager::GetArrayEventRewardInfo_Season(const int32 iEventID)
{
	return TMapEventRewardInfo_Season.Find(iEventID);
}

const TArray<b2network::B2mdTenLotterySavingsEvent>* RewardEventManager::GetArrayEventRewardInfo_TenLotterySaving(const int32 iEventID)
{
	return TMapEventRewardInfo_TenLotterySaving.Find(iEventID);
}

const TArray<b2network::B2mdPointEvent>* RewardEventManager::GetArrayEventConditionInfo_Point(const int32 iEventID)
{
	return TMapEventConditionInfo_Point.Find(iEventID);
}

const TArray<b2network::B2mdHotTimeEvent>* RewardEventManager::GetArrayHotTimeEventInfo(const int32 iEventID)
{
	return TMapEventInfo_HotTime.Find(iEventID);
}

const TArray<b2network::B2mdDiceEventMission>* RewardEventManager::GetArrayEventDiceInfo_Mission(const int32 iEventID)
{
	return TMapEventDiceInfo_Mission.Find(iEventID);
}

const TArray<b2network::B2mdDiceEventCell>* RewardEventManager::GetArrayEventDiceInfo_Cell(const int32 iEventID)
{
	return TMapEventDiceInfo_Cell.Find(iEventID);
}

const TArray<b2network::B2mdDiceEventFinishReward>* RewardEventManager::GetArrayEventDiceInfo_FinishReward(const int32 iEventID)
{
	return TMapEventDiceInfo_FinishReward.Find(iEventID);
}

const TArray<b2network::B2mdDiceEventBuyPoint>* RewardEventManager::GetArrayEventDiceInfo_BuyPoint(const int32 iEventID)
{
	return TMapEventDiceInfo_BuyPoint.Find(iEventID);
}

const TArray<b2network::B2mdDiceEventMileage>* RewardEventManager::GetArrayEventDiceInfo_Mileage(const int32 iEventID)
{
	return TMapEventDiceInfo_Mileage.Find(iEventID);
}