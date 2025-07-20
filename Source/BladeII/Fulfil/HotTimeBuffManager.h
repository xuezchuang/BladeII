// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "B2MessageInfoCommonStruct.h"
#include "B2UIEnum.h"
#include "EventSubsystem.h"
#include "Misc/ScopeLock.h"
#include "../DataStore/B2ClientDataStore.h"

/**
 * 
 */
struct FHotTimeEventDetailInfo
{
	enum class ETimeType :uint8
	{
		Begin,
		End,
	};

	int32 event_id;
	int32 index;
	int32 day_of_week;
	EHotTimeBuffType buff_type;
	int32 begin_hour;
	int32 begin_min;
	int32 end_hour;
	int32 end_min;
	int32 increase_rate;

	FHotTimeEventDetailInfo& operator=(const b2network::B2mdHotTimeEvent& InHotTimeInfo);

	int32 GetSortValueAtBeginTime() const { return (begin_hour * 100) + begin_min; }

	FDateTime GetEndTime(FDateTime InCompareTime) const;
	FDateTime GetBeginTime(FDateTime InCompareTime) const;
	FDateTime GetTime(FDateTime InCompareTime, ETimeType TimeType) const;
};

struct FHotTimeProductBuffInfo
{
	EHotTimeBuffType buff_type;
	int32 price;
	int32 period_hour;
	int32 increaseRate;
};

class BLADEII_API HotTimeBuffManager
{
	
public:
	static HotTimeBuffManager& GetInstance();

	~HotTimeBuffManager();

	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();

	bool IsActiveHotTimeBuff(EHotTimeEventType EventType);
	bool IsActiveHotTimeBuff(EHotTimeEventType EventType, EHotTimeBuffType BuffType);

	bool GetProductBuffInfo(EHotTimeBuffType BuffType, FHotTimeProductBuffInfo& OutBuffInfo) const;
	bool GetEventBuffInfo(FDateTime LocalDateTime, EHotTimeBuffType BuffType, TArray<FHotTimeEventDetailInfo>& OutBuffInfo);

	bool GetEventBuffInfoInTime(FDateTime LocalDateTime, EHotTimeBuffType BuffType, FHotTimeEventDetailInfo& OutCurrent, FHotTimeEventDetailInfo& OutNext);

	bool IsInHotTimeEventPeriod(FDateTime LocalDateTime);
	bool IsInHotTimeEventPeriod(EHotTimeBuffType BuffType, FDateTime LocalDateTime);

	bool IsActiveHotTimeProduct(EHotTimeBuffType BuffType, FDateTime DateTime) const;

	int32 GetBuffTicketCount(EHotTimeBuffType BuffType);

	bool GetProductBuffEndTime(EHotTimeBuffType BuffType, FDateTime& OutBuffEndTime) const;

	bool RequestBuyProductBuff(EHotTimeBuffType BuffType);

	int32 GetBuffTicketID(EHotTimeBuffType InBuffType);

	bool AvailablePurchaseBuffTicket(EHotTimeBuffType InBuffType);
	bool AvailablePurchaseGem(EHotTimeBuffType InBuffType);

	void SetHotTimeEventInfo(const TArray<FEventInfo>& InEventInfos, const TArray<FHotTimeEventDetailInfo>& InDetailInfos);
	void SetHotTimeProductBuffInfo(const TArray<FHotTimeProductBuffInfo>& InProductBuffInfo);

	bool GetCurrentEventInfo(const FDateTime& LocalDateTime, FEventInfo& OutHotTimeEventInfo);

	FORCEINLINE const bool HasEventData() const { return HaveEventData; }
	FORCEINLINE const bool HasProductData() const { return HaveEventData; }

private:
	//2矫埃 -> 1矫埃
	const int32 REFRESH_TIME = 60 * 60 * 1;

	bool HaveEventData;
	bool HaveProductData;

	//盔单捞磐
	TArray<FEventInfo> HotTimeEventInfos;
	TArray<FHotTimeProductBuffInfo> HotTimeProductBuffInfo;
	
	//啊傍单捞磐
	TMap<int32, TArray<FHotTimeEventDetailInfo>> HotTimeEventDetailInfoMap;

	FTimerHandle RefreshHotTimeHandler;

private:

	HotTimeBuffManager();
	HotTimeBuffManager(HotTimeBuffManager const&) = delete;
	HotTimeBuffManager operator = (HotTimeBuffManager const&) = delete;

	UGameInstance* GetGameInstance();

	void OnChangeAccountInfo();
	void OnBuyHotTimeBuff(FB2ResponseBuyHotTimeBuffPtr BuyHotTimeBuffPtr);
	void OnRefreshLatestEventInfo();

	bool IsInEventTime(const FDateTime &LocalCompareTime, const FHotTimeEventDetailInfo& EventInfo) const;
	bool InnerIsInHotTimeEventPeriod(const FEventInfo& CheckEventInfo, const FDateTime& LocalDateTime) const;
	bool IsValidEventInfo(const FEventInfo& CheckEventInfo) const;
 
	void RequestRefreshHotTime();

private:
	TArray<std::shared_ptr<issue>> Issues;
	FCriticalSection DataSet;
};
