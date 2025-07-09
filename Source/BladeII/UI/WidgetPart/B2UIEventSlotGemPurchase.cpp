// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEventSlotGemPurchase.h"

#include "B2UIRewardEventSet.h"
#include "BladeIIUtil.h"
#include "Retailer.h"

void UB2UIEventSlotGemPurchase::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, HB_RewardIconList);

	GET_SLOT(UTextBlock, TB_ProgressAndGoalCount);

	//progress
	GET_SLOT(UTextBlock, TB_EventExplainProgress);
	GET_SLOT(UTextBlock, TB_GetRewardBtnProgress);
	GET_SLOT(UTextBlock, TB_GoalCountProgress);

	//possible
	GET_SLOT(UTextBlock, TB_EventExplainPossible);
	GET_SLOT(UTextBlock, TB_GetRewardBtnPossible);
	GET_SLOT(UB2Button, BTN_GetRewardPossible);
	GET_SLOT(UTextBlock, TB_GoalCountPossible);

	//complete
	GET_SLOT(UTextBlock, TB_EventExplainComplete);
	GET_SLOT(UTextBlock, TB_GetRewardBtnComplete);
	GET_SLOT(UTextBlock, TB_GoalCountComplete);

	GET_SLOT(UWidgetSwitcher, WS_EventSlot);
}

void UB2UIEventSlotGemPurchase::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetRewardPossible, &UB2UIEventSlotGemPurchase::OnClickBTN_GetRewardPossible);
}

void UB2UIEventSlotGemPurchase::UpdateStaticText()
{
	//텍스트 초기화
	SetGemProgressAndGoalCountText(0);

	FText ftEventSlot = FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("BuyGemEvent_Desc")), FText::AsNumber(ChachedEventRewardInfo.goal_count));

	//progress
	if (TB_EventExplainProgress.IsValid())
		TB_EventExplainProgress->SetText(ftEventSlot);
	if (TB_GetRewardBtnProgress.IsValid())
		TB_GetRewardBtnProgress->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));

	//possible
	if (TB_EventExplainPossible.IsValid())
		TB_EventExplainPossible->SetText(ftEventSlot);
	if (TB_GetRewardBtnPossible.IsValid())
		TB_GetRewardBtnPossible->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));

	//complete
	if (TB_EventExplainComplete.IsValid())
		TB_EventExplainComplete->SetText(ftEventSlot);
	if (TB_GetRewardBtnComplete.IsValid())
		TB_GetRewardBtnComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));
}

void UB2UIEventSlotGemPurchase::OnClickBTN_GetRewardPossible()
{
	if (iEventState < b2network::B2MissionStateType::COMPLETED)
		return;

	iEventState = b2network::B2MissionStateType::REWARDED;

	data_trader::Retailer::GetInstance().RequestReceiveGemPurchaseEventReward(iEventID, iEventIndex);

	UpdateVisibleState();
}

void UB2UIEventSlotGemPurchase::InitRewardIconList(const b2network::B2mdGemPurchaseEventReward& EventRewardInfo)
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

void UB2UIEventSlotGemPurchase::InitEventSlotGemPurchase(const b2network::B2mdGemPurchaseEventReward& EventRewardInfo)
{
	iEventID = EventRewardInfo.event_id;
	iEventIndex = EventRewardInfo.index;

	ChachedEventRewardInfo = EventRewardInfo;

	Super::Init();

	InitRewardIconList(EventRewardInfo);
}

void UB2UIEventSlotGemPurchase::UpdateVisibleState()
{
	if (WS_EventSlot.IsValid())
		WS_EventSlot->SetActiveWidgetIndex(iEventState);
}

void UB2UIEventSlotGemPurchase::SetGemProgressAndGoalCountText(const int32 GemCount)
{
	int32 iProgressCount = GemCount;

	if (iProgressCount > ChachedEventRewardInfo.goal_count)
	{
		iProgressCount = ChachedEventRewardInfo.goal_count;
	}

	FText GoalCountText = FText::FromString(FString::Printf(TEXT("(%d/%d)"), iProgressCount, ChachedEventRewardInfo.goal_count));

	if (TB_GoalCountProgress.IsValid())
		TB_GoalCountProgress->SetText(GoalCountText);

	if (TB_GoalCountPossible.IsValid())
		TB_GoalCountPossible->SetText(GoalCountText);

	if (TB_GoalCountComplete.IsValid())
		TB_GoalCountComplete->SetText(GoalCountText);
}
