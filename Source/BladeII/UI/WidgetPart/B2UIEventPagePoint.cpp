// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPagePoint.h"

#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "B2UIEventSlotPoint.h"
#include "B2UIEventPointConditionPart.h"
#include "BladeIIUtil.h"
#include "RewardEventManager.h"
#include "../Common/Event.h"

#define SET_EVENTPOINT_PART(Index); \
GET_SLOT_BYNAME(UB2UIEventSlotPoint, ArrayEventSlotList[Index], UIP_EventSlotPoint##Index); \

void UB2UIEventPagePoint::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UScrollBox, SB_PointEventCondition);
	GET_SLOT(UB2RichTextBlock, RTB_AccruePointTitle);
	GET_SLOT(UB2RichTextBlock, RTB_AccurePointMethod);

	int32 iEventPartMax = 6;
	ArrayEventSlotList.SetNum(iEventPartMax);
	SET_EVENTPOINT_PART(0)
		SET_EVENTPOINT_PART(1)
		SET_EVENTPOINT_PART(2)
		SET_EVENTPOINT_PART(3)
		SET_EVENTPOINT_PART(4)
		SET_EVENTPOINT_PART(5)
}

void UB2UIEventPagePoint::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPagePoint);

	Issues_EventPage.Add(DeliveryPointEventRewardClass<FB2ResponseReceivePointEventRewardPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseReceivePointEventRewardPtr ReceiveEventReward)
		{
			if (Capture.IsValid())
			{
				Capture->ResponsePointEventReward(ReceiveEventReward);
			}
		}
	));
}

void UB2UIEventPagePoint::InitEventPageMasterData()
{
	InitEventPointInfo();

	InitEventPointConditionInfo();
}

void UB2UIEventPagePoint::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
		return;

	const auto& PointEventInfo = ReciveGetEvent->point_event_statuses;

	if (!PointEventInfo.IsValidIndex(0))
		return;

	if (RTB_AccurePointMethod.IsValid())
		RTB_AccurePointMethod->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventNotice2")));

	if (RTB_AccruePointTitle.IsValid())
		RTB_AccruePointTitle->SetText(FText::FromString(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventNotice1")).ToString() + FString::Printf(TEXT(" : %d"), PointEventInfo[0]->point)));

	int32 iPointEventMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < iPointEventMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i))
		{
			auto* EventSlotPoint = Cast<UB2UIEventSlotPoint>(ArrayEventSlotList[i].Get());

			int32 iRewardBitCheck = 2 << i;

			//焊惑 肯丰 //reward_state_value 0000001 霉锅掳 焊惑 肯丰 reward_state_value 0000011 霉锅掳, 滴锅掳 焊惑 肯丰
			if (PointEventInfo[0]->reward_state_value & iRewardBitCheck)
			{
				EventSlotPoint->SetEventState(b2network::B2MissionStateType::REWARDED);
			}
			//焊惑 固肯丰
			else if (PointEventInfo[0]->point >= EventSlotPoint->GetNeedPoint())
			{
				EventSlotPoint->SetEventState(b2network::B2MissionStateType::COMPLETED);
			}

		}
	}
}

void UB2UIEventPagePoint::ResponsePointEventReward(FB2ResponseReceivePointEventRewardPtr ReceivePointEventReward)
{
	OpenMailPopup();
}

void UB2UIEventPagePoint::InitEventPointInfo()
{
	//器牢飘 焊惑 浇吩 沥利 俺荐 6俺 绊沥
	if (const TArray<b2network::B2mdPointEventReward>* pArrayEventRewardInfo =
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_Point(iEventID))
	{
		const int32 idxMax = pArrayEventRewardInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventRewardInfo->IsValidIndex(idx) == false)
			{
				continue;
			}

			if (ArrayEventSlotList.IsValidIndex(idx))
			{
				if(auto* EventSlotPoint = Cast<UB2UIEventSlotPoint>(ArrayEventSlotList[idx].Get()))
				{
					EventSlotPoint->InitEventPointPart((*pArrayEventRewardInfo)[idx]);
					EventSlotPoint->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
	}
}

void UB2UIEventPagePoint::InitEventPointConditionInfo()
{
	if (!SB_PointEventCondition.IsValid())
		return;

	if (const TArray<b2network::B2mdPointEvent>* pArrayEventConditionInfo =
		RewardEventManager::GetInstance().GetArrayEventConditionInfo_Point(iEventID))
	{
		const int32 idxMax = pArrayEventConditionInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventConditionInfo->IsValidIndex(idx) == false)
			{
				continue;
			}

			if (auto* CreatedPointEventConditionPart = CreateWidget<UB2UIEventPointConditionPart>(GetOwningPlayer(), EventPointConditionClass))
			{
				CreatedPointEventConditionPart->InitEventPointConditionPart((*pArrayEventConditionInfo)[idx]);
				SB_PointEventCondition->AddChild(CreatedPointEventConditionPart);
			}
		}
	}
}