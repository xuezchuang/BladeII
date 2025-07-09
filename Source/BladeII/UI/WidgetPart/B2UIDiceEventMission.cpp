// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDiceEventMission.h"

#include "BladeIIUtil.h"
#include "RewardEventManager.h"

void UB2UIDiceEventMission::InitDiceEventMission(const b2network::B2mdDiceEventMission& DiceEventMission)
{
	CachedMissionMD = DiceEventMission;

	InitMissionState();

	this->Init();

	UpdateMissionState();
}

void UB2UIDiceEventMission::InitMissionState()
{
	FB2ResponseGetEventPtr pReciveGetEvent = RewardEventManager::GetInstance().GetCahcedGetEventStatus();
	if (pReciveGetEvent == nullptr)
	{
		return;
	}

	const int32 iUseOnlyThisIndex = 0;
	const TArray<b2network::B2DiceEventStatusPtr> ArrayEventStatus = pReciveGetEvent->dice_event_statuses;
	if (ArrayEventStatus.IsValidIndex(iUseOnlyThisIndex) == false)
	{
		return;
	}
	
	const TArray<b2network::B2DiceEventMissionPtr>& ArrayEventMission = ArrayEventStatus[iUseOnlyThisIndex]->missions;
	const int32 iArrayMax = ArrayEventMission.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayEventMission.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayEventMission[i]->event_type == CachedMissionMD.event_type)
		{
			CachedMissinState = *(ArrayEventMission[i]);
			break;
		}
	}
}

void UB2UIDiceEventMission::UpdateMissionState()
{
	if (WS_MissionState.IsValid())
	{
		bool IsComplete = false;

		if (CachedMissinState.state == b2network::B2MissionStateType::COMPLETED)
		{
			IsComplete = true;
		}

		WS_MissionState->SetActiveWidgetIndex((int32)IsComplete);
	}
}

void UB2UIDiceEventMission::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_MissionProgress);
	GET_SLOT(UTextBlock, TB_MissionPoint);

	GET_SLOT(UWidgetSwitcher, WS_MissionState);
}

void UB2UIDiceEventMission::UpdateStaticText()
{
	if (TB_MissionProgress.IsValid())
	{
		//FString DescriptionFormat = ;
		FString DescriptionKey = FString::Printf(TEXT("DiceEvent_GetPoint%d"), CachedMissionMD.event_type);
		FText ftMissionText = BladeIIGetLOCText(B2LOC_CAT_EVENT, DescriptionKey);
		FText ftMissionProgress = FText::FromString(FString::Printf(TEXT("(%d/%d)"), CachedMissinState.progress_count, CachedMissionMD.goal_count));

		FString FinalString = ftMissionText.ToString();
		FinalString.Append(ftMissionProgress.ToString());

		TB_MissionProgress->SetText(FText::FromString(FinalString));
	}

	if (TB_MissionPoint.IsValid())
	{
		FText ftMissionPoint = FText::FromString(FString::Printf(TEXT("%d"), CachedMissionMD.point));
		TB_MissionPoint->SetText(ftMissionPoint);
	}
}