// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIAttendance.h"
#include "Retailer.h"
#include "B2UIAttendanceParts.h"
#include "BladeIIGameImpl.h"

void UB2UIAttendance::CacheAssets()
{
	Super::CacheAssets();

	UIP_AttendanceParts.Empty();
	UIP_AttendanceParts.SetNum(14);

	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[0], UIP_AttendanceParts_0);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[1], UIP_AttendanceParts_1);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[2], UIP_AttendanceParts_2);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[3], UIP_AttendanceParts_3);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[4], UIP_AttendanceParts_4);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[5], UIP_AttendanceParts_5);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[6], UIP_AttendanceParts_6);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[7], UIP_AttendanceParts_7);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[8], UIP_AttendanceParts_8);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[9], UIP_AttendanceParts_9);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[10], UIP_AttendanceParts_10);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[11], UIP_AttendanceParts_11);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[12], UIP_AttendanceParts_12);
	GET_SLOT_BYNAME(UB2UIAttendanceParts, UIP_AttendanceParts[13], UIP_AttendanceParts_13);

	GET_SLOT(UB2RichTextBlock, TB_Noti);
}

void UB2UIAttendance::Init()
{
	Super::Init();

	for (auto& EventPart : UIP_AttendanceParts)
	{
		EventPart->Init();
	}

	SetAttendanceData();
}

void UB2UIAttendance::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (auto& EventPart : UIP_AttendanceParts)
	{
		EventPart->DestroySelf(InUIManager);
	}
}

void UB2UIAttendance::UpdateStaticText()
{
	if (TB_Noti.IsValid())
	{
		TB_Noti->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Attendance_Menu_Notice")));
	}
}

void UB2UIAttendance::ReceiveReward(const FAttendanceRewardInfo& DailyAttendanceRewardInfo)
{
	if (DailyAttendanceRewardInfo.bIsAttendance)
	{
		if (UIP_AttendanceParts.IsValidIndex(DailyAttendanceRewardInfo.nAttendanceDay - 1))
		{
			UIP_AttendanceParts[DailyAttendanceRewardInfo.nAttendanceDay - 1]->ReceiveReward(DailyAttendanceRewardInfo.DailyAttendanceRewardItem);
		}
	}
}

void UB2UIAttendance::SetAttendanceData()
{
	bool IsReturnUser = BladeIIGameImpl::GetClientDataStore().IsReturnUser();

	EDailyAttendanceType InType = IsReturnUser ? EDailyAttendanceType::RETURNED : EDailyAttendanceType::GENERAL;
	int32 AttendanceType = static_cast<int32>(InType);

	for (int i = 0; i < UIP_AttendanceParts.Num(); i++)
	{
		FAttendanceDayData AttendanceDayInfo = BladeIIGameImpl::GetClientDataStore().GetAttendanceDayInfo(AttendanceType, i);

		UIP_AttendanceParts[i]->SetAttendanceInfo(AttendanceDayInfo);
	}
}
