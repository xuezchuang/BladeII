// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageStageClear.h"

#include "B2UIEventSlotStageClear.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "RewardEventManager.h"
#include "../Common/Event.h"

void UB2UIEventPageStageClear::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_EventSlotList);
}

void UB2UIEventPageStageClear::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPageStageClear);

	Issues_EventPage.Add(DeliveryStageClearEventRewardClass<FB2ResponseReceiveStageClearEventRewardPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseReceiveStageClearEventRewardPtr ReceiveEventReward)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseStageClearEventReward(ReceiveEventReward);
		}
	}
	));
}

void UB2UIEventPageStageClear::InitEventPageMasterData()
{
	if (VB_EventSlotList.IsValid() == false)
		return;

	VB_EventSlotList->ClearChildren();
	ArrayEventSlotList.Empty();

	if (const TArray<b2network::B2mdStageClearEvent>* pArrayEventRewardInfo =
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_StageClear(iEventID))
	{
		const int32 idxMax = pArrayEventRewardInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventRewardInfo->IsValidIndex(idx))
			{
				auto* CreatedEventSlotStageClear = CreateWidget<UB2UIEventSlotStageClear>(GetOwningPlayer(), EventSlotClass);

				CreatedEventSlotStageClear->InitEventSlotStageClear((*pArrayEventRewardInfo)[idx]);

				VB_EventSlotList->AddChild(CreatedEventSlotStageClear);

				ArrayEventSlotList.Add(CreatedEventSlotStageClear);
			}
		}
	}
}

void UB2UIEventPageStageClear::UpdateEventSlotListState(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
		return;

	const auto& StageClearStates = ReciveGetEvent->stage_clear_event_statuses;

	const int32 iMax = StageClearStates.Num();
	for (int32 i = 0; i < iMax; ++i)
	{
		if (StageClearStates.IsValidIndex(i))
		{
			const int32 jMax = ArrayEventSlotList.Num();
			for (int32 j = 0; j < jMax; ++j)
			{
				if (ArrayEventSlotList.IsValidIndex(j) &&
					ArrayEventSlotList[j]->GetEventIndex() == StageClearStates[i]->index)
				{
					ArrayEventSlotList[j]->SetEventState(StageClearStates[i]->state);
				}
			}
		}
	}
}

void UB2UIEventPageStageClear::ResponseStageClearEventReward(FB2ResponseReceiveStageClearEventRewardPtr ReceiveEventReward)
{
	OpenMailPopup();
}