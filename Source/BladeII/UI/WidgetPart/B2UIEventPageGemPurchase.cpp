// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageGemPurchase.h"

#include "B2UIEventSlotGemPurchase.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "RewardEventManager.h"

void UB2UIEventPageGemPurchase::CacheAssets()
{
	Super::CacheAssets();

	//GET_SLOT(UVerticalBox, VB_EventSlotList);

	//GET_SLOT(UB2RichTextBlock, RTB_AccrueGemPurchase);
	//GET_SLOT(UB2RichTextBlock, RTB_AccrueGemPoint);
}

void UB2UIEventPageGemPurchase::UpdateStaticText()
{
	Super::UpdateStaticText();

	//if (RTB_AccrueGemPurchase.IsValid())
	//	RTB_AccrueGemPurchase->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("BuyGemEvent_GemCount")));

	////초기화
	//UpdateAccrueGemPointText(0);
}

void UB2UIEventPageGemPurchase::SubscribeEvents_EventPage()
{
	//UnsubscribeEvents_EventPage();

	//CAPTURE_UOBJECT(UB2UIEventPageGemPurchase);

	//Issues_EventPage.Add(DeliveryRewardGemPurchaseEventClass<FB2ResponseReceiveGemPurchaseEventRewardPtr>::GetInstance().Subscribe2(
	//	[Capture](FB2ResponseReceiveGemPurchaseEventRewardPtr ReciveEventReward)
	//{
	//	if (Capture.IsValid())
	//	{
	//		Capture->ResponseGemPurchaseEventReward(ReciveEventReward);
	//	}
	//}
	//));
}

void UB2UIEventPageGemPurchase::UpdateAccrueGemPointText(const int32 iAccrueGemPoint)
{
	//if (RTB_AccrueGemPoint.IsValid())
	//	RTB_AccrueGemPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), iAccrueGemPoint)));
}

void UB2UIEventPageGemPurchase::InitEventPageMasterData()
{
	if (VB_EventSlotList.IsValid() == false)
		return;

	VB_EventSlotList->ClearChildren();
	ArrayEventSlotList.Empty();

	if (const TArray<b2network::B2mdGemPurchaseEventReward>* pArrayEventRewardInfo =
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_GemPurchase(iEventID))
	{
		const int32 idxMax = pArrayEventRewardInfo->Num();
		for (int32 idx = 0; idx < idxMax; ++idx)
		{
			if (pArrayEventRewardInfo->IsValidIndex(idx))
			{
				auto* CreatedEventSlotGemPurchase = CreateWidget<UB2UIEventSlotGemPurchase>(GetOwningPlayer(), EventSlotClass);

				if (CreatedEventSlotGemPurchase)
				{
					CreatedEventSlotGemPurchase->InitEventSlotGemPurchase((*pArrayEventRewardInfo)[idx]);
					VB_EventSlotList->AddChild(CreatedEventSlotGemPurchase);
					ArrayEventSlotList.Add(CreatedEventSlotGemPurchase);
				}
			}
		}
	}
}

void UB2UIEventPageGemPurchase::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
	{
		return;
	}

	const int32 UseStatuesIndex = 0;
	const auto& GemPurchaseEventStates = ReciveGetEvent->gem_purchase_event_statuses;

	if (GemPurchaseEventStates.IsValidIndex(UseStatuesIndex) == false)
	{
		return;
	}

	const int32 RewardStateValue = GemPurchaseEventStates[UseStatuesIndex]->reward_state_value;
	const int32 AccureGemCount = GemPurchaseEventStates[UseStatuesIndex]->gem_count;

	UpdateAccrueGemPointText(AccureGemCount);

	const int32 EventSlotMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < EventSlotMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i))
		{
			if (auto* EventSlotGemPurchase = Cast<UB2UIEventSlotGemPurchase>(ArrayEventSlotList[i].Get()))
			{
				EventSlotGemPurchase->SetGemProgressAndGoalCountText(AccureGemCount);

				int32 iRewardBitCheck = 2 << i;
				//보상 완료 //reward_state_value 0000001 첫번째 보상 완료 reward_state_value 0000011 첫번째, 두번째 보상 완료
				if (RewardStateValue & iRewardBitCheck)
				{
					EventSlotGemPurchase->SetEventState(b2network::B2MissionStateType::REWARDED);
				}
				//보상 미완료
				else if(AccureGemCount >= EventSlotGemPurchase->GetGoalCount())
				{
					EventSlotGemPurchase->SetEventState(b2network::B2MissionStateType::COMPLETED);
				}
			}
		}
	}
}

void UB2UIEventPageGemPurchase::SortEventSlotList()
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

void UB2UIEventPageGemPurchase::ResponseGemPurchaseEventReward(FB2ResponseReceiveGemPurchaseEventRewardPtr ReciveEventReward)
{
	OpenMailPopup();
}