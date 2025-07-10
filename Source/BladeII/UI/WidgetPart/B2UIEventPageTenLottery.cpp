// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageTenLottery.h"

#include "B2UIEventSlotTenLottery.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "RewardEventManager.h"

void UB2UIEventPageTenLottery::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_EventSlotList);
}

void UB2UIEventPageTenLottery::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPageTenLottery);

	Issues_EventPage.Add(DeliveryRewardTenLotteryEventClass<FB2ResponseRewardTenLotteryEventPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseRewardTenLotteryEventPtr ReciveEventReward)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseTenLotteryEventReward(ReciveEventReward);
		}
	}
	));
}

void UB2UIEventPageTenLottery::InitEventPageMasterData()
{
	if (VB_EventSlotList.IsValid() == false)
		return;

	VB_EventSlotList->ClearChildren();
	ArrayEventSlotList.Empty();

	if (const TArray<b2network::B2mdTenLotteryEvent>* pArrayEventRewardInfo =
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_TenLottery(iEventID))
	{
		const int32 idxMax = pArrayEventRewardInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventRewardInfo->IsValidIndex(idx))
			{
				auto* CreatedEventSlotTenLottery = CreateWidget<UB2UIEventSlotTenLottery>(GetOwningPlayer(), EventSlotClass);

				if (CreatedEventSlotTenLottery)
				{
					CreatedEventSlotTenLottery->InitEventSlotTenLottery((*pArrayEventRewardInfo)[idx]);
					VB_EventSlotList->AddChild(CreatedEventSlotTenLottery);
					ArrayEventSlotList.Add(CreatedEventSlotTenLottery);
				}
			}
		}
	}
}

void UB2UIEventPageTenLottery::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
	{
		return;
	}

	const int32 UseStatuesIndex = 0;
	const auto& TenLotteryEventStates = ReciveGetEvent->ten_lottery_event_statuses;

	if (TenLotteryEventStates.IsValidIndex(UseStatuesIndex) == false)
	{
		return;
	}

	const int32 EventSlotMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < EventSlotMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i))
		{
			if (auto* EventSlotTenLottery = Cast<UB2UIEventSlotTenLottery>(ArrayEventSlotList[i].Get()))
			{
				EventSlotTenLottery->SetEventSlotState(TenLotteryEventStates[UseStatuesIndex]->lottery_count, TenLotteryEventStates[UseStatuesIndex]->rewarded_indexs);
			}
		}
	}
}

void UB2UIEventPageTenLottery::SortEventSlotList()
{
	if (VB_EventSlotList.IsValid() == false)
	{
		return;
	}

	VB_EventSlotList->ClearChildren();

	ArrayEventSlotList.StableSort(
		[](const TWeakObjectPtr<class UB2UIEventSlot>& A, const TWeakObjectPtr<class UB2UIEventSlot>& B)
	{
		//보상 완료 슬롯은 가장 뒤로 정렬
		bool Result = false;
		if (A->GetEventIndex() < B->GetEventIndex())
		{
			Result = true;
		}
		else
		{
			if (A->GetEventState() != b2network::B2MissionStateType::REWARDED && B->GetEventState() == b2network::B2MissionStateType::REWARDED)
			{
				Result = true;
			}
		}
		return Result;
	}
	);

	const int32 EventSlotMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < EventSlotMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i))
		{
			VB_EventSlotList->AddChild(ArrayEventSlotList[i].Get());
		}
	}
}

void UB2UIEventPageTenLottery::ResponseTenLotteryEventReward(FB2ResponseRewardTenLotteryEventPtr ReciveEventReward)
{
	OpenMailPopup();
}
