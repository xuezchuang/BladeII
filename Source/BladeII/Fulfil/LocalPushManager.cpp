// Fill out your copyright notice in the Description page of Project Settings.
#include "LocalPushManager.h"

#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "B2PlatformAPI.h"


LocalPushManager::LocalPushManager()
{
	UE_LOG(LogBladeII, Log, TEXT("LocalPushManager::LocalPushManager()"));
}

LocalPushManager::~LocalPushManager()
{
}

void LocalPushManager::SetLocalPushSchedule_RestPoint()
{
	UE_LOG(LogBladeII, Log, TEXT("LocalPushManager::SetLocalPushSchedule_RestPoint()"));
	
	CancelLocalPushSchedule_RestPoint();

	int32 nGamePush = 1;
	LoadGameSetting_GamePush(nGamePush);

	if (nGamePush)
	{
		int32 nNextLeftTime = GetLocalPushTime_RestPoint() + DELAY_TIME;
		if (GetLocalPushTime_RestPoint() > 0)
		{
			UBlueprintPlatformLibrary::ScheduleLocalNotificationFromNow(nNextLeftTime,
				BladeIIGetLOCText(B2LOC_CAT_LOCALPUSH, TEXT("Title_RestPoint")),
				BladeIIGetLOCText(B2LOC_CAT_LOCALPUSH, TEXT("Body_RestPoint")),
				FText::GetEmpty(),
				FString("LP_RestPoint"));
		}
	}
}

void LocalPushManager::CancelLocalPushSchedule_RestPoint()
{
	UE_LOG(LogBladeII, Log, TEXT("LocalPushManager::CancelLocalPushSchedule_RestPoint()"));
	//UBlueprintPlatformLibrary::CancelLocalNotification(FString("LP_RestPoint"));
	UBlueprintPlatformLibrary::ClearAllLocalNotifications();
}

int32 LocalPushManager::GetLocalPushTime_RestPoint()
{
	int32 CurrentRestPoint = BladeIIGameImpl::GetClientDataStore().GetRestPoint();
	int32 Time = (100 - CurrentRestPoint) * 432;
	
	return Time;
}
