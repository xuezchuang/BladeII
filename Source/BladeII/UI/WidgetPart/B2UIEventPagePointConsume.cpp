// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPagePointConsume.h"

#include "BladeIIGameImpl.h"
#include "B2UIEventSlotPointConsume.h"
#include "B2UIEventPointConsumeCondition.h"
#include "BladeIIUtil.h"
#include "B2UIEventSlotHB.h"
#include "RewardEventManager.h"
#include "../Common/Event.h"

void UB2UIEventPagePointConsume::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_AccruePointTitle);

	GET_SLOT(UB2ScrollBox, SB_ConditionList);
	GET_SLOT(UB2ScrollBox, SB_EventSlotHBList);

	GET_SLOT(UB2RichTextBlock, RTB_EventConditionExplain);
	GET_SLOT(UB2RichTextBlock, RTB_EventAcquirePointCount);
}

void UB2UIEventPagePointConsume::UpdateStaticText()
{
	if (RTB_EventConditionExplain.IsValid())
		RTB_EventConditionExplain->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventNotice2")));

	if (RTB_EventAcquirePointCount.IsValid())
		RTB_EventAcquirePointCount->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventNotice3")));
}

void UB2UIEventPagePointConsume::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPagePointConsume);

	Issues_EventPage.Add(DeliveryRewardPointShopEventClass<FB2ResponseRewardPointShopEventdPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseRewardPointShopEventdPtr ReciveEventReward)
	{
		if (Capture.IsValid())
		{
			Capture->ResponsePointConsumeEventReward(ReciveEventReward);
		}
	}
	));
}

void UB2UIEventPagePointConsume::InitEventPageMasterData()
{
	InitEventPointConsumeSlotInfo();

	InitEventPointConsumeConditionInfo();
}

void UB2UIEventPagePointConsume::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
		return;

	int32 iCurrentAccruePoint = 0;

	const auto& EventStatuses = ReciveGetEvent->point_shop_event_statuses;
	const int32 iArrayStatusesMax = EventStatuses.Num();
	for (int32 i = 0; i < iArrayStatusesMax; ++i)
	{
		if (EventStatuses.IsValidIndex(i) == false)
		{
			continue;
		}

		if (EventStatuses[i]->event_id == iEventID)
		{
			iCurrentAccruePoint = EventStatuses[i]->point;
			break;
		}
	}

	CheckEventSlotListState(iCurrentAccruePoint);
}

void UB2UIEventPagePointConsume::ResponsePointConsumeEventReward(FB2ResponseRewardPointShopEventdPtr ReciveEventReward)
{
	if(ReciveEventReward == nullptr)
	{
		return;
	}

	if (b2network::B2PointShopEventStatusPtr PointConsumeEventStates = ReciveEventReward->point_shop_event_status)
	{
		if (PointConsumeEventStates->event_id == iEventID)
		{
			CheckEventSlotListState(PointConsumeEventStates->point);
			OpenMailPopup();
		}	
	}
}

void UB2UIEventPagePointConsume::InitEventPointConsumeSlotInfo()
{
	if (SB_EventSlotHBList.IsValid() == false ||
		EventSlotPointConsumeClass == nullptr ||
		EventSlotHBClass == nullptr)
		return;

	SB_EventSlotHBList->ClearChildren();
	ArrayEventSlotList.Empty();

	TWeakObjectPtr<UB2UIEventSlotHB> pEventSlotHB = nullptr;

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

			if (idx % iHorizontalMax == 0)
			{
				auto* pCreateWidget = CreateWidget<UB2UIEventSlotHB>(GetOwningPlayer(), EventSlotHBClass);
				pCreateWidget->Init();
				SB_EventSlotHBList->AddChild(pCreateWidget);
				pEventSlotHB = pCreateWidget;
			}

			if (pEventSlotHB.IsValid())
			{
				auto* pEventSlot = CreateWidget<UB2UIEventSlotPointConsume>(GetOwningPlayer(), EventSlotPointConsumeClass);

				pEventSlot->InitEventSlotPointConsume((*pArrayEventRewardInfo)[idx]);

				pEventSlotHB->AddChildWidget(pEventSlot);

				if (auto HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(pEventSlot->Slot))
					HorizontalBoxSlot->SetSize(FSlateChildSize());

				ArrayEventSlotList.Add(pEventSlot);
			}
		}
	}
}

void UB2UIEventPagePointConsume::InitEventPointConsumeConditionInfo()
{
	if (SB_ConditionList.IsValid() == false ||
		EventPointConsumeConditionClass == nullptr)
		return;

	SB_ConditionList->ClearChildren();

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

			auto* pEventConditionWidget = CreateWidget<UB2UIEventPointConsumeCondition>(GetOwningPlayer(), EventPointConsumeConditionClass);

			pEventConditionWidget->InitEventPointConsimeCondition((*pArrayEventConditionInfo)[idx]);

			SB_ConditionList->AddChild(pEventConditionWidget);
		}
	}
}

void UB2UIEventPagePointConsume::CheckEventSlotListState(int32 iAccuireEventPoint)
{
	if (RTB_AccruePointTitle.IsValid())
		RTB_AccruePointTitle->SetText(FText::FromString(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("PointEvnetShop_Goods")).ToString() + FString::Printf(TEXT(" : %d"), iAccuireEventPoint)));

	const int32 idxMax = ArrayEventSlotList.Num();
	for (int32 idx = 0; idx < idxMax; ++idx)
	{
		if (ArrayEventSlotList.IsValidIndex(idx) == false)
			continue;

		if (auto* pEventSlot = Cast<UB2UIEventSlotPointConsume>(ArrayEventSlotList[idx].Get()))
			pEventSlot->CheckEventSlotState(iAccuireEventPoint);
	}
}