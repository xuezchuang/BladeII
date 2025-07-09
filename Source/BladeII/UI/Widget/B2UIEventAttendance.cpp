// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIEventAttendacnePart.h"
#include "B2UIEventAttendance.h"
#include "B2ClientDataStore.h"
#include "B2UIMsgPopupReward.h"
#include "B2PlatformAPI.h"
#include "B2GameInstance.h"
#include "Retailer.h"
#include "B2UIEventAttendancePakgeSlot.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "RewardEventManager.h"

void UB2UIEventAttendance::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_GotoURL);
	GET_SLOT(UButton, BTN_GotoURL);
	GET_SLOT(UHorizontalBox, HB_PakageSlot);

	if (EventAttendanceParts.Num() == 0)
	{
		EventAttendanceParts.SetNum(7);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[0], UIP_EventAttendancePart0);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[1], UIP_EventAttendancePart1);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[2], UIP_EventAttendancePart2);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[3], UIP_EventAttendancePart3);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[4], UIP_EventAttendancePart4);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[5], UIP_EventAttendancePart5);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, EventAttendanceParts[6], UIP_EventAttendancePart6);
	}

	for (auto& EventPart : EventAttendanceParts)
	{
		EventPart->Init();
	}
	SetEventSpecialDay();

	const int32 PackageSlotMax = 2;
	EventAttendancePakageSlots.Empty();
	EventAttendancePakageSlots.SetNum(PackageSlotMax);
	GET_SLOT_BYNAME(UB2UIEventAttendancePakgeSlot, EventAttendancePakageSlots[0], UIP_EventAttendancePakageSlot0);
	GET_SLOT_BYNAME(UB2UIEventAttendancePakgeSlot, EventAttendancePakageSlots[1], UIP_EventAttendancePakageSlot1);

	for (int32 i = 0; i < PackageSlotMax; ++i)
	{
		if (EventAttendancePakageSlots.IsValidIndex(i))
			EventAttendancePakageSlots[i]->Init();
	}

	SetPakageSlotVisibleState(false);
}

bool UB2UIEventAttendance::IsNewReward() const
{
	FTimespan RemainTime = FDateTime::FromUnixTimestamp(BladeIIGameImpl::GetClientDataStore().GetEventAttendanceStatus().next_attendance_time / 1000) - UB2GameInstance::GetUTCNow();
	return RemainTime.GetTicks() < 0;
}
void UB2UIEventAttendance::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GotoURL, &UB2UIEventAttendance::OnClickBtnGotoUrl);
}

void UB2UIEventAttendance::Init()
{
	Super::Init();

	SubscribeEvents();

	InitEventInfo();
}

void UB2UIEventAttendance::SetEventAttendanceParts(int32 iPartsIndex, int32 iRewardIndex, int32 iAttendanceDay, TArray<FEventAttendanceReward>& rAttendanceRewards)
{
	if (!EventAttendanceParts[iPartsIndex].IsValid() || !rAttendanceRewards.IsValidIndex(iRewardIndex))
		return;

	EventAttendanceParts[iPartsIndex]->SetCachedEventInfo(CachedEventInfo);
	EventAttendanceParts[iPartsIndex]->SetIsDoubleReward(IsDoubleReward);
	EventAttendanceParts[iPartsIndex]->InitRewardSlotInfo(rAttendanceRewards[iRewardIndex]);
	
	if (iAttendanceDay > iRewardIndex)
	{
		EventAttendanceParts[iPartsIndex]->UpdateRewardState(EEventRewardState::REWARD_COMPLETE);
	}
	else if (iAttendanceDay == iRewardIndex)
	{
		EventAttendanceParts[iPartsIndex]->UpdateRewardState(IsNewReward() ? EEventRewardState::REWARD_ENABLE : EEventRewardState::REWARD_WAIT);
	}
	else
	{
		EventAttendanceParts[iPartsIndex]->UpdateRewardState(EEventRewardState::REWARD_WAIT);
	}
}

void UB2UIEventAttendance::SetPakageSlotVisibleState(bool IsVisible)
{
	if (HB_PakageSlot.IsValid())
	{
		HB_PakageSlot->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	const int32 iArrayMax = EventAttendancePakageSlots.Max();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (EventAttendancePakageSlots.IsValidIndex(i))
		{
			EventAttendancePakageSlots[i]->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEventAttendance::InitEventInfo()
{
	const int32 iEventInfoOnlyUseThisArrayIndex = 0;

	const auto& RewardStatus = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceStatus();
	const TArray<b2network::B2mdEventAttendance>* pArrayEventInfo = RewardEventManager::GetInstance().GetArrayEventInfo_Attendance(RewardStatus.event_id);
	if (pArrayEventInfo && pArrayEventInfo->IsValidIndex(iEventInfoOnlyUseThisArrayIndex))
	{
		CachedEventInfo = (*pArrayEventInfo)[iEventInfoOnlyUseThisArrayIndex];

		UpdateEventAttendanceParts();
	}
	else
	{
		return;
	}
}

void UB2UIEventAttendance::UpdateEventAttendanceParts()
{
	TArray<FEventAttendanceReward> AttendanceRewards;
	const auto& RewardStatus = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceStatus();
	const auto& RewardData = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceRewardData(AttendanceRewards, RewardStatus.event_id);

	if (AttendanceRewards.Max() <= 0)
		return;

	if (RewardStatus.next_attendance_time == -1)
	{
		data_trader::Retailer::GetInstance().RequestEventAttendance(CachedEventInfo.event_id);
		return;
	}

	for (int32 i = 0; i < CachedEventInfo.total_attendance_days; ++i)
	{
		int32 iPartsIndex = i;

		if (i == CachedEventInfo.total_attendance_days - 1)	//4일 이벤트일 경우 UIP_EventAttendancePart6가 4일째 보상으로 세팅이 되어야 하므로
			iPartsIndex = EventAttendanceParts.Num() - 1;

		SetEventAttendanceParts(iPartsIndex, i, RewardStatus.attendance_day, AttendanceRewards);
	}

	if (TB_GotoURL.IsValid())
		TB_GotoURL->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendanceGotoURL")));

	ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(true);
}

void UB2UIEventAttendance::UpdateEventAttendancePackageState()
{
	bool IsPackageSlotVisible = false;

	switch (CachedEventInfo.bonus_type)
	{
	case EEventBonusType::NORMAL:
		{
		}break;
	case EEventBonusType::DOUBLE:
		{
			IsPackageSlotVisible = true;
		}break;
	}

	SetPakageSlotVisibleState(IsPackageSlotVisible);
}

bool UB2UIEventAttendance::IsPossibleDoubleReward(const int32 BougthPackageCount)
{
	bool IsDouble = false;

	if (BougthPackageCount >= 2)
	{
		IsDouble = true;
	}

	return IsDouble;
}

void UB2UIEventAttendance::ResponseGetPeriodPackageState(FB2ResponseGetPeriodPackageStatePtr ReciveGetPeriodPackageState)
{
	int32	BoughtPackageCount = 0;

	FDateTime DateNow = UB2GameInstance::GetLocalNow();

	const TArray<b2network::B2PeriodicPackageStatePtr>& ArrayPeriodicPackageStates = ReciveGetPeriodPackageState->periodic_package_states;
	const int32 ArrayMax = ArrayPeriodicPackageStates.Num();
	for (int32 i = 0; i < ArrayMax; ++i)
	{
		if (ArrayPeriodicPackageStates.IsValidIndex(i) == false)
			continue;

		int32 PackageID = ArrayPeriodicPackageStates[i]->package_id;
		FDateTime ExpireTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(ArrayPeriodicPackageStates[i]->end_pay_time));
		bool bPackageState = DateNow < ExpireTime ? true : false;

		const int32 PackageSlotMax = EventAttendancePakageSlots.Num();
		for (int32 j = 0; j < PackageSlotMax; ++j)
		{
			if(EventAttendancePakageSlots.IsValidIndex(j) == false)
				continue;;

			if (EventAttendancePakageSlots[j]->GetPackageID() == PackageID)
			{
				EventAttendancePakageSlots[j]->UpdateSlotState(bPackageState);
				BoughtPackageCount++;
			}
		}
	}
	
	IsDoubleReward = IsPossibleDoubleReward(BoughtPackageCount);

	UpdateEventAttendanceParts();
	UpdateEventAttendancePackageState();
}

void UB2UIEventAttendance::DestroySelf(UB2UIManager* InUIManager)
{
	for (auto& RewardPart : EventAttendanceParts)
	{
		if (RewardPart.IsValid())
			RewardPart->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIEventAttendance::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Visible)
	{
		data_trader::Retailer::GetInstance().RequestGetPeriodPackageState();
	}
}

void UB2UIEventAttendance::SubscribeEvents()
{
	UnsubscribeEvents();

	CAPTURE_UOBJECT(UB2UIEventAttendance);

	Issues.Add(ReceiveEventAttendanceRewardClass<int32, int32, b2network::B2RewardPtr>::GetInstance().Subscribe2(
		[Capture](int32 eventId, int32 attendance_day, b2network::B2RewardPtr Rewards)
	{
		if (Capture.IsValid())
		{
			Capture->ReceiveEventAttendance(eventId, attendance_day, Rewards);
		}
	}
	));

	Issues.Add(UpdateEventAttendancePartsClass<>::GetInstance().Subscribe2(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->UpdateEventAttendanceParts();
		}
	}
	));

	Issues.Add(DeliveryGetPeriodPackageStateClass<FB2ResponseGetPeriodPackageStatePtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseGetPeriodPackageStatePtr ReciveGetPeriodPackageState)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseGetPeriodPackageState(ReciveGetPeriodPackageState);
		}
	}
	));
}

void UB2UIEventAttendance::UnsubscribeEvents()
{
	Issues.Empty();
}


bool UB2UIEventAttendance::IsPeriod() const
{
	return IsReceptiblePeriod() && IsEntirePeriod();
}

bool UB2UIEventAttendance::IsReceptiblePeriod() const
{
	TArray<FEventAttendanceReward> AttendanceRewards;
	const auto& RewardStatus = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceStatus();

	bool IsData = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceRewardData(AttendanceRewards, RewardStatus.event_id);
	return AttendanceRewards.Num() > RewardStatus.attendance_day;
}

bool UB2UIEventAttendance::IsEntirePeriod() const
{
	TArray<FEventAttendanceReward> AttendanceRewards;
	const auto& RewardStatus = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceStatus();

	return BladeIIGameImpl::GetClientDataStore().GetEventAttendanceRewardData(AttendanceRewards, RewardStatus.event_id);
}

void UB2UIEventAttendance::ReceiveEventAttendance(int32 EventId, int32 ReciveRewardDay, b2network::B2RewardPtr Reward)
{
	const int32 MaxPartIndex = EventAttendanceParts.Num();
	for (int32 i = 0; i < MaxPartIndex; ++i)
	{
		if (EventAttendanceParts.IsValidIndex(i) == false)
		{
			continue;
		}

		if (EventAttendanceParts[i]->GetRewardDay() == ReciveRewardDay)
		{
			EventAttendanceParts[i]->UpdateRewardState(EEventRewardState::REWARD_COMPLETE, Reward);
			break;
		}
	}

	DoMarkCombineRedDotClass<>::GetInstance().Signal();
	UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::AttendanceMain);
}

void UB2UIEventAttendance::SetEventSpecialDay()
{
	for (int32 i = 0; i < SpecialDay.Num(); ++i)
	{
		int32 EventSpecialDay = SpecialDay[i] - 1;

		if (EventAttendanceParts.IsValidIndex(EventSpecialDay) && EventAttendanceParts[EventSpecialDay].IsValid())
		{
			EventAttendanceParts[EventSpecialDay]->SetSpecialDay(true);
		}
	}
}

void UB2UIEventAttendance::OnClickBtnGotoUrl()
{
	if (!BTN_GotoURL.IsValid())
		return;

	B2P_ShowOpenURL(CachedEventInfo.event_url);
}
