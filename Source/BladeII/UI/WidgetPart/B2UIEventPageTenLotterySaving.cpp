// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventPageTenLotterySaving.h"

#include "RewardEventManager.h"
#include "B2UIEventSlotTenLotterySaving.h"
#include "B2UIEventSlotHB.h"
#include "B2UIRewardEventSet.h"
#include "BladeIIUtil.h"
#include "Retailer.h"

void UB2UIEventPageTenLotterySaving::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_EventSlot);

	GET_SLOT(UHorizontalBox, HB_RewardIconList);

	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UTextBlock, TB_GetRewardBtn);

	
	GET_SLOT(UTextBlock, TB_ExplainEvent);
	GET_SLOT(UTextBlock, TB_ExplainPoint);
	GET_SLOT(UTextBlock, TB_CurrentLotteryCount);

	GET_SLOT(UOverlay, O_EffectRewardPossible);

	GET_SLOT(UB2Button, BTN_GotoEquipShop);
	GET_SLOT(UTextBlock, TB_GotoEquipShop);
}

void UB2UIEventPageTenLotterySaving::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIEventPageTenLotterySaving::OnClickBTN_GetReward);

	BIND_CLICK_FUNC_TO_BTN(BTN_GotoEquipShop, &UB2UIEventPageTenLotterySaving::OnClickBTN_GotoEquipShop);
}

void UB2UIEventPageTenLotterySaving::UpdateStaticText()
{
	//default
	UpdateCurrentLotteryCountText(0);

	if (TB_ExplainEvent.IsValid())
	{
		TB_ExplainEvent->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("TenLotterySaving_Desc")));
	}

	if (TB_ExplainPoint.IsValid())
	{
		TB_ExplainPoint->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("TenLotterySaving_HavePoint")));
	}

	if (TB_GetRewardBtn.IsValid())
	{
		TB_GetRewardBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("TenLotterySaving_ExchangeReward")));
	}

	if (TB_GotoEquipShop.IsValid())
	{
		TB_GotoEquipShop->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("TenLotterySaving_MoveStore")));
	}
}

void UB2UIEventPageTenLotterySaving::SubscribeEvents_EventPage()
{
	UnsubscribeEvents_EventPage();

	CAPTURE_UOBJECT(UB2UIEventPageTenLotterySaving);

	Issues_EventPage.Add(
		DeliveryRewardTenLotterySavingsEventClass <FB2ResponseRewardTenLotterySavingsEventPtr>::GetInstance().Subscribe2(
		[Capture](FB2ResponseRewardTenLotterySavingsEventPtr ReciveEventReward)
		{
			if (Capture.IsValid())
			{
				Capture->ResponseRewardTenLotterySavingsEvent(ReciveEventReward);
			}
		}
	));
}

void UB2UIEventPageTenLotterySaving::InitEventPageMasterData()
{
	ArrayEventSlotList.Empty();

	if (const TArray<b2network::B2mdTenLotterySavingsEvent>* pArrayEventRewardInfo =
		RewardEventManager::GetInstance().GetArrayEventRewardInfo_TenLotterySaving(iEventID))
	{
		const int32 UseOnlyThisIndex = 0;

		if (pArrayEventRewardInfo->IsValidIndex(UseOnlyThisIndex))
		{
			ChachedEventRewardInfo = (*pArrayEventRewardInfo)[UseOnlyThisIndex];

			InitEventSlotList(ChachedEventRewardInfo);
			InitRewardIconList(ChachedEventRewardInfo);
		}
	}
}

void UB2UIEventPageTenLotterySaving::UpdateEventPage(FB2ResponseGetEventPtr ReciveGetEvent)
{
	if (ReciveGetEvent == nullptr)
	{
		return;
	}

	const auto& TenLotterySavingEventStates = ReciveGetEvent->ten_lottery_savings_event_statuses;

	int32 CurrentLotteryCount = 0;

	const int32 UseOnlyThisIndex = 0;
	if (TenLotterySavingEventStates.IsValidIndex(UseOnlyThisIndex))
	{
		CurrentLotteryCount = TenLotterySavingEventStates[UseOnlyThisIndex]->current_point;
	}

	UpdateEventPageFromLotteryCount(CurrentLotteryCount);
}

void UB2UIEventPageTenLotterySaving::InitEventSlotList(const b2network::B2mdTenLotterySavingsEvent& EventRewardInfo)
{
	TWeakObjectPtr<UB2UIEventSlotHB> pEventSlotHB = nullptr;

	for (int32 i = 0; i < EventRewardInfo.need_point; ++i)
	{
		if (i % MAX_Horizontal == 0)
		{
			auto* pCreateHBWidget = CreateWidget<UB2UIEventSlotHB>(GetOwningPlayer(), EventSlotHBClass);
			pCreateHBWidget->Init();
			VB_EventSlot->AddChild(pCreateHBWidget);
			pEventSlotHB = pCreateHBWidget;
		}

		if (pEventSlotHB.IsValid())
		{
			auto* pEventSlot = CreateWidget<UB2UIEventSlotTenLotterySaving>(GetOwningPlayer(), EventSlotTenLotterySavingClass);
			pEventSlot->Init();
			pEventSlotHB->AddChildWidget(pEventSlot);
			ArrayEventSlotList.Add(pEventSlot);
		}
	}
}

void UB2UIEventPageTenLotterySaving::InitRewardIconList(const b2network::B2mdTenLotterySavingsEvent& EventRewardInfo)
{
	if (HB_RewardIconList.IsValid() == false)
	{
		return;
	}

	HB_RewardIconList->ClearChildren();

	const int32 RewardCountMax = EventRewardInfo.rewards.Num();
	for (int32 i = 0; i < RewardCountMax; ++i)
	{
		if (EventRewardInfo.rewards.IsValidIndex(i))
		{
			if (EventRewardInfo.rewards[i]->count <= 0)
			{
				continue;
			}

			auto* CreatedRewardIcon = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardIconClass);

			if (CreatedRewardIcon)
			{
				CreatedRewardIcon->Init();
				CreatedRewardIcon->UpdateRewardIcon(EventRewardInfo.rewards[i]->id, EventRewardInfo.rewards[i]->count);
				HB_RewardIconList->AddChildToHorizontalBox(CreatedRewardIcon);
			}
		}
	}
}

void UB2UIEventPageTenLotterySaving::OnClickBTN_GetReward()
{
	StartTransparencyBlock_BP();

	data_trader::Retailer::GetInstance().RequestRewardTenLotterySavingsEvent(ChachedEventRewardInfo.event_id);
}

void UB2UIEventPageTenLotterySaving::OnClickBTN_GotoEquipShop()
{
	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::EquipStore));
}

void UB2UIEventPageTenLotterySaving::ResponseRewardTenLotterySavingsEvent(FB2ResponseRewardTenLotterySavingsEventPtr ReciveEventReward)
{
	if (ReciveEventReward != nullptr)
	{
		UpdateEventPageFromLotteryCount(ReciveEventReward->current_point);

		OpenMailPopup();
	}
}

void UB2UIEventPageTenLotterySaving::UpdateEventPageFromLotteryCount(const int32 CurrentLotteryCount)
{
	UpdateRewardPossibleAnimation(CurrentLotteryCount);
	UpdateCurrentLotteryCountText(CurrentLotteryCount);
	UpdateBTN_GetRewardState(CurrentLotteryCount);
	UpdateSlotListState(CurrentLotteryCount);
}

void UB2UIEventPageTenLotterySaving::UpdateRewardPossibleAnimation(const int32 CurrentLotteryCount)
{
	if (O_EffectRewardPossible.IsValid() == false)
	{
		return;
	}

	if (CurrentLotteryCount >= ChachedEventRewardInfo.need_point)
	{
		O_EffectRewardPossible->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		RepeatRewardPossibleAnimation_BP();
	}
	else
	{
		O_EffectRewardPossible->SetVisibility(ESlateVisibility::Collapsed);

		StopRewardPossibleAnimation_BP();
	}
}

void UB2UIEventPageTenLotterySaving::UpdateCurrentLotteryCountText(const int32 CurrentLotteryCount)
{
	FText CurrentLotteryCountText = FText::FromString(FString::Printf(TEXT("%d"), CurrentLotteryCount));

	if (TB_CurrentLotteryCount.IsValid())
		TB_CurrentLotteryCount->SetText(CurrentLotteryCountText);
}

void UB2UIEventPageTenLotterySaving::UpdateBTN_GetRewardState(const int32 CurrentLotteryCount)
{
	if (BTN_GetReward.IsValid() == false)
	{
		return;
	}
		
	if (CurrentLotteryCount >= ChachedEventRewardInfo.need_point)
	{
		BTN_GetReward->SetIsEnabled(true);
	}
	else
	{
		BTN_GetReward->SetIsEnabled(false);
	}
}

void UB2UIEventPageTenLotterySaving::UpdateSlotListState(const int32 CurrentLotteryCount)
{
	const int32 EventSlotMax = ArrayEventSlotList.Num();
	for (int32 iSlotIndex = 0; iSlotIndex < EventSlotMax; ++iSlotIndex)
	{
		if (ArrayEventSlotList.IsValidIndex(iSlotIndex) == false)
		{
			continue;
		}

		if (auto* EventSlot = Cast<UB2UIEventSlotTenLotterySaving>(ArrayEventSlotList[iSlotIndex].Get()))
		{
			if (iSlotIndex < CurrentLotteryCount)
			{
				EventSlot->SetEventState(b2network::B2MissionStateType::COMPLETED);
			}
			else
			{
				EventSlot->SetEventState(b2network::B2MissionStateType::PROGRESS);
			}
		}
	}

	UpdateEventSlotListVisibleState();
}
