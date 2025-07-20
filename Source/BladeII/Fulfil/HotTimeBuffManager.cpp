// Fill out your copyright notice in the Description page of Project Settings.
#include "HotTimeBuffManager.h"

#include "Event.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2GameInstance.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "Retailer.h"
#include "RewardEventManager.h"


FHotTimeEventDetailInfo & FHotTimeEventDetailInfo::operator=(const b2network::B2mdHotTimeEvent& InHotTimeInfo)
{
	event_id = InHotTimeInfo.event_id;
	index = InHotTimeInfo.index;
	day_of_week = InHotTimeInfo.day_of_week;
	buff_type = SvrToCliHotTimeBuffType(InHotTimeInfo.buff_type);
	begin_hour = InHotTimeInfo.begin_hour;
	begin_min = InHotTimeInfo.begin_min;
	end_hour = InHotTimeInfo.end_hour;
	end_min = InHotTimeInfo.end_min;
	increase_rate = InHotTimeInfo.increase_rate;

	return *this;
}

FDateTime FHotTimeEventDetailInfo::GetEndTime(FDateTime InCompareTime) const
{
	return GetTime(InCompareTime, ETimeType::End);
}

FDateTime FHotTimeEventDetailInfo::GetBeginTime(FDateTime InCompareTime) const
{
	return GetTime(InCompareTime, ETimeType::Begin);
}

FDateTime FHotTimeEventDetailInfo::GetTime(FDateTime InCompareTime, ETimeType TimeType) const
{
	int32 Year, Month, Day, Hour = 0, Minute = 0;
	
	switch (TimeType)
	{
	case ETimeType::Begin:
		Hour = begin_hour;
		Minute = begin_min;
		break;
	case ETimeType::End:
		Hour = end_hour;
		Minute = end_min;
		break;
	}

	InCompareTime.GetDate(Year, Month, Day);
	int32 NextDay = Hour / 24;
	Hour = NextDay > 0 ? Hour % 24 : Hour;
	FDateTime CalcTime = FDateTime(Year, Month, Day, Hour, Minute);

	if (NextDay > 0)
		CalcTime += FTimespan(NextDay, 0, 0, 0, 0);

	return CalcTime;
}


HotTimeBuffManager & HotTimeBuffManager::GetInstance()
{
	static HotTimeBuffManager instance;
	return instance;
}

HotTimeBuffManager::~HotTimeBuffManager()
{
	UnsubscribeEvents();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance && GameInstance->GetWorld())
		GameInstance->GetWorld()->GetTimerManager().ClearTimer(RefreshHotTimeHandler);
}

void HotTimeBuffManager::Init()
{
	SubscribeEvents();
}

void HotTimeBuffManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(ChangeUserAccountInfoClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnChangeAccountInfo();
	}
	));

	Issues.Add(DeliveryBuyHotTimeBuffClass<FB2ResponseBuyHotTimeBuffPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseBuyHotTimeBuffPtr BuffPtr) 
	{
		this->OnBuyHotTimeBuff(BuffPtr);
	}
	));

	Issues.Add(GetLatestEventInfoSuccessfulClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnRefreshLatestEventInfo();
	}
	));
}

void HotTimeBuffManager::UnsubscribeEvents()
{
	Issues.Empty();
}

bool HotTimeBuffManager::IsActiveHotTimeBuff(EHotTimeEventType EventType)
{
	switch (EventType)
	{
	case EHotTimeEventType::Event:
	case EHotTimeEventType::Product:
		return IsActiveHotTimeBuff(EventType, EHotTimeBuffType::Exp) || IsActiveHotTimeBuff(EventType, EHotTimeBuffType::Gold);
	case EHotTimeEventType::All:
		return IsActiveHotTimeBuff(EHotTimeEventType::Event) || IsActiveHotTimeBuff(EHotTimeEventType::Product);
	}
	return false;
}

bool HotTimeBuffManager::IsActiveHotTimeBuff(EHotTimeEventType EventType, EHotTimeBuffType BuffType)
{
	switch (EventType)
	{
	case EHotTimeEventType::Event:
		return IsInHotTimeEventPeriod(BuffType, UB2GameInstance::GetKoreaTimeNow());
	case EHotTimeEventType::Product:
		return IsActiveHotTimeProduct(BuffType, UB2GameInstance::GetUTCNow());
	case EHotTimeEventType::All:
		return IsActiveHotTimeBuff(EHotTimeEventType::All);
	}
	return false;
}

bool HotTimeBuffManager::GetProductBuffInfo(EHotTimeBuffType BuffType, FHotTimeProductBuffInfo& OutBuffInfo) const
{
	for (const auto& BuffInfo : HotTimeProductBuffInfo)
	{
		if (BuffInfo.buff_type == BuffType)
		{
			OutBuffInfo = BuffInfo;
			return true;
		}
	}
	return false;
}

bool HotTimeBuffManager::GetEventBuffInfo(FDateTime LocalDateTime, EHotTimeBuffType BuffType, TArray<FHotTimeEventDetailInfo>& OutBuffInfo) 
{
	FEventInfo CurrentHotTimeEventInfo;
	if (!GetCurrentEventInfo(LocalDateTime, CurrentHotTimeEventInfo))
		return false;
	
	int32 EventID = CurrentHotTimeEventInfo.event_id;
	if (!HotTimeEventDetailInfoMap.Contains(EventID))
		return false;

	int32 CurrentDayOfWeek = UB2GameInstance::ConvertToDayOfWeek(LocalDateTime);

	for (const auto& BuffInfo : HotTimeEventDetailInfoMap[EventID])
	{
		if (BuffInfo.buff_type == BuffType && BuffInfo.day_of_week == CurrentDayOfWeek)
			OutBuffInfo.Add(BuffInfo);
	}
	return OutBuffInfo.Num() > 0;
}

bool HotTimeBuffManager::GetEventBuffInfoInTime(FDateTime LocalDateTime, EHotTimeBuffType BuffType, FHotTimeEventDetailInfo & OutCurrent, FHotTimeEventDetailInfo & OutNext)
{
	TArray<FHotTimeEventDetailInfo> EventBuffInfos;
	
	if (!GetEventBuffInfo(LocalDateTime, BuffType, EventBuffInfos))
		return false;

	EventBuffInfos.Sort([](const FHotTimeEventDetailInfo& A, const FHotTimeEventDetailInfo& B)
							{ return A.GetSortValueAtBeginTime() < B.GetSortValueAtBeginTime(); });

	bool FoundNextInfo = false;

	for (const auto& Element : EventBuffInfos)
	{
		if (IsInEventTime(LocalDateTime, Element))
			OutCurrent = Element;

		FDateTime Compare = FDateTime(1, 1, 1, LocalDateTime.GetHour(), LocalDateTime.GetMinute());
		FDateTime Begin = Element.GetBeginTime(FDateTime(1, 1, 1));
		if (Compare < Begin)
		{
			OutNext = Element;
			FoundNextInfo = true;
			break;
		}
	}
	return FoundNextInfo;
}

HotTimeBuffManager::HotTimeBuffManager()
	: HaveEventData(false), HaveProductData(false)
{
}

UGameInstance * HotTimeBuffManager::GetGameInstance()
{
	if (GEngine)
	{
		TArray<APlayerController*> PlayerList;
		GEngine->GetAllLocalPlayerControllers(PlayerList); 
		
		if (PlayerList.Num() > 0 && PlayerList[0] && PlayerList[0]->GetWorld())
			return PlayerList[0]->GetWorld()->GetGameInstance();
	}
	return nullptr;
}

void HotTimeBuffManager::OnChangeAccountInfo()
{
	RefreshHotTimeUIClass<>::GetInstance().Signal();
}

void HotTimeBuffManager::OnBuyHotTimeBuff(FB2ResponseBuyHotTimeBuffPtr BuyHotTimeBuffPtr)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	if (CliDataStore.GetAccountInfo())
	{
		CliDataStore.ReplaceUserDataWithDoc(EDocUserDataType::Gem, BuyHotTimeBuffPtr->current_gem);

		EHotTimeBuffType BuffType = SvrToCliHotTimeBuffType(BuyHotTimeBuffPtr->buff_type);
		switch (BuffType)
		{
		case EHotTimeBuffType::Gold:
			CliDataStore.GetAccountInfo()->gold_buff_end_time = BuyHotTimeBuffPtr->buff_end_time;
			CliDataStore.GetAccountInfo()->gold_buff_ticket = BuyHotTimeBuffPtr->buff_ticket;
			break;
		case EHotTimeBuffType::Exp:
			CliDataStore.GetAccountInfo()->exp_buff_end_time = BuyHotTimeBuffPtr->buff_end_time;
			CliDataStore.GetAccountInfo()->exp_buff_ticket = BuyHotTimeBuffPtr->buff_ticket;
			break;
		}
	}
	RefreshHotTimeUIClass<>::GetInstance().Signal();
	//Open Popup

	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bought_Product")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);

	BuyHotTimeBuffSuccessfulClass<>::GetInstance().Signal();
}

void HotTimeBuffManager::OnRefreshLatestEventInfo()
{
	const TArray<b2network::B2mdEventInfo>& EnableEvents = RewardEventManager::GetInstance().GetArrayEnableEventInfo();
	TArray<FEventInfo> TempHotTimeEventInfos;
	TArray<FHotTimeEventDetailInfo> TempHotTimeDetailInfos;
	for (const b2network::B2mdEventInfo& MDEventInfo : EnableEvents)
	{
		if (MDEventInfo.event_category == b2network::B2EventCategory::HOT_TIME_BUFF)
		{
			FEventInfo EventInfo;
			EventInfo = MDEventInfo;
			TempHotTimeEventInfos.Add(EventInfo);

			const TArray<b2network::B2mdHotTimeEvent>* MDDetailInfos = RewardEventManager::GetInstance().GetArrayHotTimeEventInfo(EventInfo.event_id);

			if(!MDDetailInfos)
				continue;

			for (const b2network::B2mdHotTimeEvent& MDDetail : *MDDetailInfos)
			{
				FHotTimeEventDetailInfo DetailInfo;
				DetailInfo = MDDetail;
				TempHotTimeDetailInfos.Add(DetailInfo);
			}
		}
	}
	SetHotTimeEventInfo(TempHotTimeEventInfos, TempHotTimeDetailInfos);
}

bool HotTimeBuffManager::IsInHotTimeEventPeriod(FDateTime LocalDateTime)
{
	if (!HasEventData())
		return false;

	FEventInfo CurrentHotTimeEventInfo;
	GetCurrentEventInfo(LocalDateTime, CurrentHotTimeEventInfo);

	return InnerIsInHotTimeEventPeriod(CurrentHotTimeEventInfo, LocalDateTime);
}

bool HotTimeBuffManager::IsInHotTimeEventPeriod(EHotTimeBuffType BuffType, FDateTime LocalDateTime)
{
	if (!IsInHotTimeEventPeriod(LocalDateTime))
		return false;

	FEventInfo CurrentHotTimeEventInfo;
	if (!GetCurrentEventInfo(LocalDateTime, CurrentHotTimeEventInfo))
		return false;

	int32 EventID = CurrentHotTimeEventInfo.event_id;
	if (!HotTimeEventDetailInfoMap.Contains(EventID))
		return false;

	bool Result = false;
	for (const auto& Info : HotTimeEventDetailInfoMap[EventID])
	{
		if(Info.buff_type != BuffType)
			continue;

		if (IsInEventTime(LocalDateTime, Info))
		{
			Result = true;
			break;
		}
	}

	return Result;
}

bool HotTimeBuffManager::IsActiveHotTimeProduct(EHotTimeBuffType BuffType, FDateTime DateTime) const
{
	FDateTime EndTime;
	if (!GetProductBuffEndTime(BuffType, EndTime))
		return false;

	return EndTime - DateTime > 0;
}

int32 HotTimeBuffManager::GetBuffTicketCount(EHotTimeBuffType BuffType)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	if (CliDataStore.GetAccountInfo())
	{
		switch (BuffType)
		{
		case EHotTimeBuffType::Exp:
			return CliDataStore.GetAccountInfo()->exp_buff_ticket;
		case EHotTimeBuffType::Gold:
			return CliDataStore.GetAccountInfo()->gold_buff_ticket;
			break;
		}
	}
	return 0;
}

bool HotTimeBuffManager::GetProductBuffEndTime(EHotTimeBuffType BuffType, FDateTime& OutBuffEndTime) const
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	if (!CliDataStore.GetAccountInfo())
		return false;

	switch (BuffType)
	{
	case EHotTimeBuffType::Exp:
		OutBuffEndTime = GetServerTimeToDateTime(CliDataStore.GetAccountInfo()->exp_buff_end_time);
		break;
	case EHotTimeBuffType::Gold:
		OutBuffEndTime = GetServerTimeToDateTime(CliDataStore.GetAccountInfo()->gold_buff_end_time);
		break;
	}

	return true;
}

bool HotTimeBuffManager::RequestBuyProductBuff(EHotTimeBuffType BuffType)
{
	if (BuffType == EHotTimeBuffType::End)
		return false;

	data_trader::Retailer::GetInstance().RequestBuyHotTimeBuff(CliToSvrHotTimeBuffType(BuffType));
	return true;
}

int32 HotTimeBuffManager::GetBuffTicketID(EHotTimeBuffType InBuffType)
{
	switch (InBuffType)
	{
	case EHotTimeBuffType::Gold:
		return FItemRefIDHelper::GetGoodsID_GoldBuffTicket();
	case EHotTimeBuffType::Exp:
		return FItemRefIDHelper::GetGoodsID_ExpBuffTicket();
	}
	return 0;
}

bool HotTimeBuffManager::AvailablePurchaseBuffTicket(EHotTimeBuffType InBuffType)
{
	int32 TicketCount = GetBuffTicketCount(InBuffType);

	return TicketCount > 0;
}

bool HotTimeBuffManager::AvailablePurchaseGem(EHotTimeBuffType InBuffType)
{
	FHotTimeProductBuffInfo ProductBuffInfo;
	GetProductBuffInfo(InBuffType, ProductBuffInfo);

	return ProductBuffInfo.price <= BladeIIGameImpl::GetClientDataStore().GetGemAmount();
}

void HotTimeBuffManager::SetHotTimeEventInfo(const TArray<FEventInfo>& InEventInfos, const TArray<FHotTimeEventDetailInfo>& InDetailInfos)
{
	HaveEventData = true;
	{
		FScopeLock Lock(&DataSet);
		HotTimeEventInfos.Empty();
		HotTimeEventDetailInfoMap.Empty();

		FDateTime CurrentUTC = UB2GameInstance::GetUTCNow();
		for (const FEventInfo& EventInfo : InEventInfos)
		{
			auto EndTime = GetServerTimeToDateTime(EventInfo.end_time);
			if (EndTime - CurrentUTC >= 0)
				HotTimeEventInfos.Add(EventInfo);
		}

		for (const FHotTimeEventDetailInfo& DetailInfo : InDetailInfos)
		{
			TArray<FHotTimeEventDetailInfo>& DetailInfos = HotTimeEventDetailInfoMap.FindOrAdd(DetailInfo.event_id);
			DetailInfos.Add(DetailInfo);
		}
	}

	RefreshHotTimeUIClass<>::GetInstance().Signal();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance && GameInstance->GetWorld())
	{
		GameInstance->GetWorld()->GetTimerManager().ClearTimer(RefreshHotTimeHandler);
		GameInstance->GetWorld()->GetTimerManager().SetTimer(RefreshHotTimeHandler, FTimerDelegate::CreateLambda([this]() 
		{
			this->RequestRefreshHotTime();
		}),
		REFRESH_TIME, true, REFRESH_TIME);
	}
}

void HotTimeBuffManager::SetHotTimeProductBuffInfo(const TArray<FHotTimeProductBuffInfo>& InProductBuffInfo)
{
	HaveProductData = true;
	HotTimeProductBuffInfo = InProductBuffInfo;
}

bool HotTimeBuffManager::IsInEventTime(const FDateTime & LocalCompareTime, const FHotTimeEventDetailInfo & EventInfo) const
{
	int32 Hour = LocalCompareTime.GetHour();
	int32 Minute = LocalCompareTime.GetMinute();
	int32 Second = LocalCompareTime.GetSecond();

	int32 CurrentDayofweek = UB2GameInstance::ConvertToDayOfWeek(LocalCompareTime);
	if (EventInfo.day_of_week != CurrentDayofweek)
		return false;
	
	FDateTime Compare = FDateTime(1, 1, 1, Hour, Minute, Second);
	FDateTime Begin = EventInfo.GetBeginTime(FDateTime(1, 1, 1));
	FDateTime End = EventInfo.GetEndTime(FDateTime(1, 1, 1));

	return (Compare - Begin >= 0) && (End - Compare >= 0);
}

bool HotTimeBuffManager::InnerIsInHotTimeEventPeriod(const FEventInfo & CheckEventInfo, const FDateTime& LocalDateTime) const
{
	if (!IsValidEventInfo(CheckEventInfo))
		return false;

	auto StartTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(CheckEventInfo.start_time));
	auto EndTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(CheckEventInfo.end_time));

	return (LocalDateTime - StartTime >= 0) && (EndTime - LocalDateTime >= 0);
}

bool HotTimeBuffManager::IsValidEventInfo(const FEventInfo & CheckEventInfo) const
{
	if (CheckEventInfo.event_id == INDEX_NONE || CheckEventInfo.start_time >= CheckEventInfo.end_time)
		return false;

	return true;
}

bool HotTimeBuffManager::GetCurrentEventInfo(const FDateTime& LocalDateTime, FEventInfo& OutHotTimeEventInfo)
{
	bool FirstSetting = false;
	bool result = false;
	{
		FScopeLock Lock(&DataSet);
		for (const FEventInfo& EventInfo : HotTimeEventInfos)
		{
			if (HaveEventData && !FirstSetting)
			{
				OutHotTimeEventInfo = EventInfo;
				OutHotTimeEventInfo.event_id = INDEX_NONE;
				OutHotTimeEventInfo.start_time = 0;
				OutHotTimeEventInfo.end_time = 0;
				FirstSetting = true;
			}

			if (InnerIsInHotTimeEventPeriod(EventInfo, LocalDateTime))
			{
				OutHotTimeEventInfo = EventInfo;
				result = true;
				break;
			}
		}
	}

	return result;
}

void HotTimeBuffManager::RequestRefreshHotTime()
{
	RewardEventManager::GetInstance().RequestLatestEventInfo();
}
