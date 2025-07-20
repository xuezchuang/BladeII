// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageGemSpend.h"

#include "B2UIEventSlotGemSpend.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "RewardEventManager.h"
#include "../../Common/CommonStruct.h"
#include "Event.h"

void UB2UIEventPageGemSpend::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_EventSlotList);

	GET_SLOT(UB2RichTextBlock, RTB_AccrueGemPurchase);
	GET_SLOT(UB2RichTextBlock, RTB_AccrueGemPoint);
}

void UB2UIEventPageGemSpend::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (RTB_AccrueGemPurchase.IsValid())
		RTB_AccrueGemPurchase->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("SpendGemEvent_GemCount")));

	//檬扁拳
	UpdateSpendGemPointText(0);
}

void UB2UIEventPageGemSpend::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPageGemSpend);

	Issues_EventPage.Add(DeliveryRewardGemSpendEventClass<FB2ResponseRewardSpendGemEventPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseRewardSpendGemEventPtr ReciveEventReward)
		{
			if (Capture.IsValid())
			{
				Capture->ResponseGemSpendEventReward(ReciveEventReward);
			}
		}
	));
}

void UB2UIEventPageGemSpend::UpdateSpendGemPointText(const int32 iSpendGemPoint)
{
	if (RTB_AccrueGemPoint.IsValid())
		RTB_AccrueGemPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), iSpendGemPoint)));
}

void UB2UIEventPageGemSpend::InitEventPageMasterData()
{
	if (VB_EventSlotList.IsValid() == false)
		return;

	VB_EventSlotList->ClearChildren();
	ArrayEventSlotList.Empty();

	if (const TArray<b2network::B2mdSpendGemEvent>* pArrayEventRewardInfo =
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_GemSpend(iEventID))
	{
		const int32 idxMax = pArrayEventRewardInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventRewardInfo->IsValidIndex(idx))
			{
				auto* CreatedEventSlotGemSpend = CreateWidget<UB2UIEventSlotGemSpend>(GetOwningPlayer(), EventSlotClass);

				if (CreatedEventSlotGemSpend)
				{
					CreatedEventSlotGemSpend->InitEventSlotGemSpend((*pArrayEventRewardInfo)[idx]);
					VB_EventSlotList->AddChild(CreatedEventSlotGemSpend);
					ArrayEventSlotList.Add(CreatedEventSlotGemSpend);
				}
			}
		}
	}
}

void UB2UIEventPageGemSpend::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
	{
		return;
	}

	const int32 UseStatuesIndex = 0;
	const auto& GemSpendEventStates = ReciveGetEvent->spend_gem_event_statuses;

	if (GemSpendEventStates.IsValidIndex(UseStatuesIndex) == false)
	{
		return;
	}

	const int32 RewardStateValue = GemSpendEventStates[UseStatuesIndex]->reward_state_value;
	const int32 SpendGemCount = GemSpendEventStates[UseStatuesIndex]->spend_gem_count;

	UpdateSpendGemPointText(SpendGemCount);

	const int32 EventSlotMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < EventSlotMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i))
		{
			if (auto* EventSlotGemSpend = Cast<UB2UIEventSlotGemSpend>(ArrayEventSlotList[i].Get()))
			{
				EventSlotGemSpend->SetGemProgressAndGoalCountText(SpendGemCount);

				int32 iRewardBitCheck = 2 << i;
				//焊惑 肯丰 //reward_state_value 0000001 霉锅掳 焊惑 肯丰 reward_state_value 0000011 霉锅掳, 滴锅掳 焊惑 肯丰
				if (RewardStateValue & iRewardBitCheck)
				{
					EventSlotGemSpend->SetEventState(b2network::B2MissionStateType::REWARDED);
				}
				//焊惑 固肯丰
				else if (SpendGemCount >= EventSlotGemSpend->GetGoalCount())
				{
					EventSlotGemSpend->SetEventState(b2network::B2MissionStateType::COMPLETED);	
				}

				EventSlotGemSpend->UpdateVisibleState();
			}
		}
	}

	TArray<TWeakObjectPtr<class UB2UIEventSlot>> TempSlotList;
	
	for (int32 i = 0; i < ArrayEventSlotList.Num(); i++)
	{
		if (auto* EventSlotGemSpend = Cast<UB2UIEventSlotGemSpend>(ArrayEventSlotList[i].Get()))
		{
			if (EventSlotGemSpend->GetEventState() == b2network::B2MissionStateType::COMPLETED || EventSlotGemSpend->GetEventState() == b2network::B2MissionStateType::PROGRESS)
			{
				TempSlotList.Add(ArrayEventSlotList[i]);
			}
		}
	}
	for (int32 i = 0; i < ArrayEventSlotList.Num(); i++)
	{
		if (auto* EventSlotGemSpend = Cast<UB2UIEventSlotGemSpend>(ArrayEventSlotList[i].Get()))
		{
			if (EventSlotGemSpend->GetEventState() == b2network::B2MissionStateType::REWARDED)
			{
				TempSlotList.Add(ArrayEventSlotList[i]);
			}
		}
	}

	for (int32 i = 0; i < ArrayEventSlotList.Num(); ++i)
	{
		if (TempSlotList.IsValidIndex(i))
		{
			VB_EventSlotList->AddChild(TempSlotList[i].Get());
		}
	}
}

void UB2UIEventPageGemSpend::SortEventSlotList()
{
	//林荤困 捞亥飘俊辑 器牢飘 备涝 锭巩俊 弊成 UpdateEventPage 俊辑 喊档肺 沥纺窍档废 货肺 芦.
	
	if (VB_EventSlotList.IsValid() == false)
	{
		return;
	}

	VB_EventSlotList->ClearChildren();

	ArrayEventSlotList.StableSort(
		[](const TWeakObjectPtr<class UB2UIEventSlot>& A, const TWeakObjectPtr<class UB2UIEventSlot>& B)
	{
		//焊惑 肯丰 浇吩篮 啊厘 第肺 沥纺
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

void UB2UIEventPageGemSpend::ResponseGemSpendEventReward(FB2ResponseRewardSpendGemEventPtr ReciveEventReward)
{
	OpenMailPopup();
}
