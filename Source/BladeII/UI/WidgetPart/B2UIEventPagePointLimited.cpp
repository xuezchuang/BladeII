// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPagePointLimited.h"

#include "RewardEventManager.h"
#include "B2UIManager.h"
#include "BladeIIUtil.h"
#include "B2UIEventTabPointShopLimited.h"
#include "B2UIEventSlotPointShopLimited.h"
#include "B2UIPointShopLimitedEventPopup.h"
#include "../Common/Event.h"

void UB2UIEventPagePointLimited::DestroySelf(class UB2UIManager* InUIManager)
{
	if (pPointShopLimitedEventPopup.IsValid())
	{
		pPointShopLimitedEventPopup->DestroySelf(InUIManager);
		pPointShopLimitedEventPopup = nullptr;
	}

	Super::DestroySelf(InUIManager);
}


void UB2UIEventPagePointLimited::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, HB_TabList);
	GET_SLOT(UB2ScrollBox, SB_EventSlotList);

	GET_SLOT(UB2RichTextBlock, RTB_AccruePoint);

	GET_SLOT(UB2Button, BTN_Explain);
	GET_SLOT(UB2RichTextBlock, RTB_ExplainBtn);
}

void UB2UIEventPagePointLimited::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (RTB_ExplainBtn.IsValid())
	{
		RTB_ExplainBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventLimitedPointShop_GuidePopUpTitle")));
	}
}

void UB2UIEventPagePointLimited::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Explain, &UB2UIEventPagePointLimited::OnClickBTN_Explain);
}

void UB2UIEventPagePointLimited::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPagePointLimited);

	Issues_EventPage.Add(DeliveryRewardPointShopEventClass<FB2ResponseRewardPointShopEventdPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseRewardPointShopEventdPtr ReciveEventReward)
	{
		if (Capture.IsValid())
		{
			Capture->ResponsePointShopLimited(ReciveEventReward);
		}
	}
	));

	Issues_EventPage.Add(SelectEventTabShopLimitedClass<int32>::GetInstance().Subscribe2(
		[Capture](int32 iTapGroupID)
	{
		if (Capture.IsValid())
		{
			Capture->UpdateEventPage_FromTapGroupID(iTapGroupID);
		}
	}
	));
}

void UB2UIEventPagePointLimited::InitEventPageMasterData()
{
	InitEventTabPointShopLimited();
	InitEventSlotPointShopLimited();

	UpdateEventPage_FromTapGroupID(iSelectedTabGroupID);
}

void UB2UIEventPagePointLimited::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
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

	const TArray<b2network::B2PointShopEventRewardHistoryPtr>& ArrayRewardHistory = ReciveGetEvent->point_shop_event_reward_histories;
	const int32 iArrayMax = ArrayRewardHistory.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayRewardHistory.IsValidIndex(i) == false)
		{
			continue;
		}

		const int32 iRewardIndex = ArrayRewardHistory[i]->index;
		const int32 iRewardedCount = ArrayRewardHistory[i]->total_reward_count;

		UpdateArrayEventSlotRewardedCount(iRewardIndex, iRewardedCount);
	}

	UpdateEventPage_FromAccruePoint(iCurrentAccruePoint);
}

void UB2UIEventPagePointLimited::ResponsePointShopLimited(FB2ResponseRewardPointShopEventdPtr ReciveEventReward)
{
	if (ReciveEventReward == nullptr)
	{
		return;
	}

	if (b2network::B2PointShopEventStatusPtr EventStatus = ReciveEventReward->point_shop_event_status)
	{
		if (EventStatus->event_id == iEventID)
		{
			const int32 iCurrentAccruePoint = EventStatus->point;;
			const int32 iRewardIndex = ReciveEventReward->index;
			const int32 iRewardedCount = ReciveEventReward->total_reward_count;

			UpdateArrayEventSlotRewardedCount(iRewardIndex, iRewardedCount);
			UpdateEventPage_FromAccruePoint(iCurrentAccruePoint);

			OpenMailPopup();
		}
	}
}

void UB2UIEventPagePointLimited::InitEventTabPointShopLimited()
{
	if (HB_TabList.IsValid() == false)
	{
		return;
	}
		
	HB_TabList->ClearChildren();
	ArrayEventTabShopLimited.Empty();

	TMap<int32, TArray<b2network::B2mdPointEventReward>> TempTMapRewardInfo;

	const TArray<b2network::B2mdPointEventReward>* pArrayEventRewardInfo = RewardEventManager::GetInstance().GetArrayEventRewardInfo_Point(iEventID);
	if (pArrayEventRewardInfo == nullptr)
	{
		return;
	}

	const int32 iArrayMax = pArrayEventRewardInfo->Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (pArrayEventRewardInfo->IsValidIndex(i) == false)
		{
			continue;
		}

		const int32 iTapGroupID = (*pArrayEventRewardInfo)[i].tab_group_id;

		TArray<b2network::B2mdPointEventReward>& ArrayRewardInfo = TempTMapRewardInfo.FindOrAdd(iTapGroupID);
		ArrayRewardInfo.Add((*pArrayEventRewardInfo)[i]);
	}

	for (auto& Elem : TempTMapRewardInfo)
	{
		const int32 iUseOnlyThisIndex = 0;
		const TArray<b2network::B2mdPointEventReward>& ArrayRewardInfo = Elem.Value;

		int32 iTapGroupID = 0;
		int32 iTapGroupRewardID = 0;

		if (ArrayRewardInfo.IsValidIndex(iUseOnlyThisIndex))
		{
			iTapGroupID = ArrayRewardInfo[iUseOnlyThisIndex].tab_group_id;
			iTapGroupRewardID = ArrayRewardInfo[iUseOnlyThisIndex].reward_id;
		}

		auto* pCreatedEventTab = CreateWidget<UB2UIEventTabPointShopLimited>(GetOwningPlayer(), EventTabPointShopLimitedClass);

		if (pCreatedEventTab)
		{
			pCreatedEventTab->InitEventTab(iTapGroupID, iTapGroupRewardID);
			HB_TabList->AddChild(pCreatedEventTab);
			ArrayEventTabShopLimited.Add(pCreatedEventTab);

			if (auto HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(pCreatedEventTab->Slot))
			{
				HorizontalBoxSlot->SetSize(ESlateSizeRule::Fill);
			}
				
		}

		if (iSelectedTabGroupID == -1)
		{
			iSelectedTabGroupID = iTapGroupID;
		}
	}
}

void UB2UIEventPagePointLimited::InitEventSlotPointShopLimited()
{
	if (SB_EventSlotList.IsValid() == false)
	{
		return;
	}

	SB_EventSlotList->ClearChildren();

	const TArray<b2network::B2mdPointEventReward>* pArrayEventRewardInfo = RewardEventManager::GetInstance().GetArrayEventRewardInfo_Point(iEventID);
	if (pArrayEventRewardInfo == nullptr)
	{
		return;
	}

	const int32 iArrayMax = pArrayEventRewardInfo->Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (pArrayEventRewardInfo->IsValidIndex(i) == false)
		{
			continue;
		}

		auto* CreatedEventSlot = CreateWidget<UB2UIEventSlotPointShopLimited>(GetOwningPlayer(), EventSlotPointShopLimitedClass);
		if (CreatedEventSlot)
		{
			CreatedEventSlot->InitEventSlotPointShopLimited((*pArrayEventRewardInfo)[i]);
			SB_EventSlotList->AddChild(CreatedEventSlot);
			ArrayEventSlotList.Add(CreatedEventSlot);
		}
	}
}

void UB2UIEventPagePointLimited::UpdateEventPage_FromTapGroupID(const int32 iTapGroupID)
{
	if (SB_EventSlotList.IsValid())
	{
		const float InitScrollOffset = 0.f;
		SB_EventSlotList->SetScrollOffset(InitScrollOffset);
	}

	UpdateArrayEventTab_FromTapGroupID(iTapGroupID);
	UpdateArrayEventSlot_FromTapGroupID(iTapGroupID);
}

void UB2UIEventPagePointLimited::UpdateArrayEventTab_FromTapGroupID(const int32 iTapGroupID)
{
	const int32 iArrayMax = ArrayEventTabShopLimited.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayEventTabShopLimited.IsValidIndex(i) == false)
		{
			continue;
		}

		bool IsSelected = false;

		if (ArrayEventTabShopLimited[i]->GetTapGroupID() == iTapGroupID)
		{
			IsSelected = true;
		}

		ArrayEventTabShopLimited[i]->UpdateEventTabPointShopLimited(IsSelected);
	}
}

void UB2UIEventPagePointLimited::UpdateArrayEventSlot_FromTapGroupID(const int32 iTapGroupID)
{
	int32 iArrayMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i) == false)
		{
			continue;
		}

		auto* EventSlot = Cast<UB2UIEventSlotPointShopLimited>(ArrayEventSlotList[i].Get());
		if (EventSlot)
		{
			EventSlot->Update_FromTapGroupID(iTapGroupID);
		}
	}
}

void UB2UIEventPagePointLimited::UpdateAccruePointText(const int32 iAccruePoint)
{
	if (RTB_AccruePoint.IsValid())
	{
		RTB_AccruePoint->SetText(FText::FromString(
			BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("PointEvnetShop_Point")).ToString() + FString::Printf(TEXT(" : %d"), iAccruePoint)));
	}
}

void UB2UIEventPagePointLimited::UpdateArrayEventSlotRewardedCount(const int32 iIndex, const int32 iRewardedCount)
{
	const int32 iArrayMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i) == false)
		{
			continue;
		}

		if (auto* EventSlot = Cast<UB2UIEventSlotPointShopLimited>(ArrayEventSlotList[i].Get()))
		{
			if (iIndex == EventSlot->GetEventIndex())
			{
				EventSlot->SetRewardedCount(iRewardedCount);
			}
		}
	}
}

void UB2UIEventPagePointLimited::UpdateEventPage_FromAccruePoint(const int32 iAccruePoint)
{
	UpdateAccruePointText(iAccruePoint);
	UpdateArrayEventSlot_FromAccruePoint(iAccruePoint);
}

void UB2UIEventPagePointLimited::UpdateArrayEventSlot_FromAccruePoint(const int32 iAccruePoint)
{
	const int32 iArrayMax = ArrayEventSlotList.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayEventSlotList.IsValidIndex(i) == false)
		{
			continue;
		}

		if (auto* EventSlot = Cast<UB2UIEventSlotPointShopLimited>(ArrayEventSlotList[i].Get()))
		{
			EventSlot->Update_FromAccruePoint(iAccruePoint);
		}
	}
}

void UB2UIEventPagePointLimited::OpenPointShopLimitedExplainPopup()
{
	if (pPointShopLimitedEventPopup.IsValid())
	{
		pPointShopLimitedEventPopup->OpenPopup();
	}
	else
	{
		if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
		{
			pPointShopLimitedEventPopup = pUIManagerInstance->OpenUI<UB2UIPointShopLimitedEventPopup>(UIFName::PointShopLimitedEventPopup);
			pPointShopLimitedEventPopup->InitPointShopLimitedEventPopup(iEventID);
			pPointShopLimitedEventPopup->OpenPopup();
		}
	}
}

void UB2UIEventPagePointLimited::OnClickBTN_Explain()
{
	OpenPointShopLimitedExplainPopup();
}