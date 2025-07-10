// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageLevelup.h"

#include "B2UIEventSlotLevelup.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "RewardEventManager.h"

void UB2UIEventPageLevelup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_EventList);
}

void UB2UIEventPageLevelup::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPageLevelup);

	Issues_EventPage.Add(DeliveryLevelUpEventRewardClass<FB2ResponseReceiveLevelUpEventRewardPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseReceiveLevelUpEventRewardPtr ReceiveLevelUpEventReward)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseLevelupEventReward(ReceiveLevelUpEventReward);
		}
	}
	));
}

void UB2UIEventPageLevelup::InitEventPageMasterData()
{
	if (VB_EventList.IsValid() == false)
		return;

	VB_EventList->ClearChildren();

	if (const TArray<b2network::B2mdLevelUpEvent>* pArrayEventRewardInfo = 
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_Levelup(iEventID))
	{
		const int32 idxMax = pArrayEventRewardInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventRewardInfo->IsValidIndex(idx))
			{
				auto* CreatedEventSlot = CreateWidget<UB2UIEventSlotLevelup>(GetOwningPlayer(), EventSlotClass);

				CreatedEventSlot->InitEventLevelupPart((*pArrayEventRewardInfo)[idx]);

				VB_EventList->AddChild(CreatedEventSlot);

				ArrayEventSlotList.Add(CreatedEventSlot);
			}
		}
	}
}

void UB2UIEventPageLevelup::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
		return;

	const auto& LevelupEventInfos = ReciveGetEvent->level_up_event_infos;

	for(int32 i = 0; i < LevelupEventInfos.Num(); ++i)
	{
		if(LevelupEventInfos.IsValidIndex(i))
		{
			for(int32 j = 0; j < ArrayEventSlotList.Num(); ++j)
			{	
				if(ArrayEventSlotList[j]->GetEventIndex() == LevelupEventInfos[i]->index)
				{
					ArrayEventSlotList[j]->SetEventState(LevelupEventInfos[i]->state);
					break;
				}
			}	
		}
	}
}

void UB2UIEventPageLevelup::ResponseLevelupEventReward(FB2ResponseReceiveLevelUpEventRewardPtr ReceiveLevelUpEventReward)
{
	OpenMailPopup();
}