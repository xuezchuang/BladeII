// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIEventNewUser.h"
#include "B2UIEventAttendacnePart.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"

void UB2UIEventNewUser::CacheAssets()
{
	Super::CacheAssets();

	if (UIP_EventNewUserParts.Num() == 0)
	{
		UIP_EventNewUserParts.SetNum(static_cast<int32>(14));

		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[0], UIP_EventNewUwerParts0);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[1], UIP_EventNewUwerParts1);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[2], UIP_EventNewUwerParts2);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[3], UIP_EventNewUwerParts3);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[4], UIP_EventNewUwerParts4);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[5], UIP_EventNewUwerParts5);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[6], UIP_EventNewUwerParts6);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[7], UIP_EventNewUwerParts7);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[8], UIP_EventNewUwerParts8);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[9], UIP_EventNewUwerParts9);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[10], UIP_EventNewUwerParts10);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[11], UIP_EventNewUwerParts11);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[12], UIP_EventNewUwerParts12);
		GET_SLOT_BYNAME(UB2UIEventAttendacnePart, UIP_EventNewUserParts[13], UIP_EventNewUwerParts13);
	}

	for (auto& EventPart : UIP_EventNewUserParts)
	{
		EventPart->Init();
	}

	SetEventSpecialDay();

	GET_SLOT(UTextBlock, TB_RemainDay);
}

void UB2UIEventNewUser::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIEventNewUser::Init()
{
	Super::Init();

	SubscribeEvents();
	SetEventNewParts();
}

void UB2UIEventNewUser::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


bool UB2UIEventNewUser::IsEndEvent() const
{
	FTimespan RemainTime = FDateTime::FromUnixTimestamp(BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().FinishTime / 1000) - UB2GameInstance::GetUTCNow();
	return RemainTime.GetTicks() < 0;
}

bool UB2UIEventNewUser::IsNewUserEvent() const
{
	FTimespan RemainTime = FDateTime::FromUnixTimestamp(BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().NextAttendanceTime / 1000) - UB2GameInstance::GetUTCNow();
	bool EnableReceive = BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().TotalAttendanceDays > BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().AttendanceDay;
	return RemainTime.GetTicks() < 0 && EnableReceive;
}

void UB2UIEventNewUser::ReceiveEventNewUserAttendance(FB2ResponseBeginnerAttendancePtr InNewUserInfo)
{
	int32 AttendanceIndex = InNewUserInfo->attendance_day - 1;

	if (UIP_EventNewUserParts.IsValidIndex(AttendanceIndex) && UIP_EventNewUserParts[AttendanceIndex].IsValid())
	{
		BladeIIGameImpl::GetClientDataStore().SetNewUserAttendanceInfo(InNewUserInfo->attendance_day, InNewUserInfo->next_attendance_time);
	
		UIP_EventNewUserParts[AttendanceIndex]->UpdateRewardState(EEventRewardState::REWARD_COMPLETE, InNewUserInfo->reward);
	}
}


void UB2UIEventNewUser::SetEventSpecialDay()
{
	for (int32 i = 0; i < SpecialDay.Num(); ++i)
	{
		int32 EventSpecialDay = SpecialDay[i] - 1;

		if (UIP_EventNewUserParts.IsValidIndex(EventSpecialDay) && UIP_EventNewUserParts[EventSpecialDay].IsValid())
		{
			UIP_EventNewUserParts[EventSpecialDay]->SetSpecialDay(true);
		}
	}
}

void UB2UIEventNewUser::SetEventNewParts()
{
	const auto& EventNewUserInfo =  BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus();
	const int32 AttendanceType = static_cast<int32>(EDailyAttendanceType::BEGINNER);
	const int32 PartsCount = UIP_EventNewUserParts.Num();
	const int32 CurrentRewardDay = EventNewUserInfo.AttendanceDay;

	if (TB_RemainDay.IsValid())
	{
		FTimespan RemainTime = FDateTime::FromUnixTimestamp(BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().FinishTime / 1000) - UB2GameInstance::GetUTCNow();
		int32 RemainDays = RemainTime.GetDays();
		TB_RemainDay->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Attendance_Beginner_Event_TimeRemaining")), FText::AsNumber(RemainDays)));
	}

	for (int i = 0; i < PartsCount; i++)
	{
		FAttendanceDayData AttendanceDayInfo = BladeIIGameImpl::GetClientDataStore().GetAttendanceDayInfo(AttendanceType, i);

		if (UIP_EventNewUserParts[i].IsValid())
		{
			UIP_EventNewUserParts[i]->SetNewUserInfo(AttendanceDayInfo);
			UIP_EventNewUserParts[i]->SetEventPartsType(EAttendanceTab::EventNewUser);
			
			if (i < CurrentRewardDay)
			{
				UIP_EventNewUserParts[i]->UpdateRewardState(EEventRewardState::REWARD_COMPLETE);
			}
			else if (i == CurrentRewardDay)
			{
				UIP_EventNewUserParts[i]->UpdateRewardState(IsNewUserEvent() ? EEventRewardState::REWARD_ENABLE : EEventRewardState::REWARD_WAIT);
			}
			else
			{
				UIP_EventNewUserParts[i]->UpdateRewardState(EEventRewardState::REWARD_WAIT);
			}
		}
	}
}

void UB2UIEventNewUser::SubscribeEvents()
{
	UnsubscribeEvents();

	CAPTURE_UOBJECT(UB2UIEventNewUser);

	Issues.Add(DeliveryBeginnerAttendanceClass<FB2ResponseBeginnerAttendancePtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseBeginnerAttendancePtr InNewUserPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ReceiveEventNewUserAttendance(InNewUserPtr);
		}
	}
	));
}

void UB2UIEventNewUser::UnsubscribeEvents()
{
	Issues.Empty();
}


