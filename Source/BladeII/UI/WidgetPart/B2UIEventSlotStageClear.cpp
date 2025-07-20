// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotStageClear.h"

#include "B2UIRewardEventSet.h"
#include "Retailer.h"
#include "BladeIIUtil.h"

void UB2UIEventSlotStageClear::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);

	//progress
	GET_SLOT(UTextBlock, TB_EventExplainProgress);
	GET_SLOT(UTextBlock, TB_GetRewardBtnProgress);

	//possible
	GET_SLOT(UTextBlock, TB_EventExplainPossible);
	GET_SLOT(UTextBlock, TB_GetRewardBtnPossible);
	GET_SLOT(UB2Button, BTN_GetRewardPossible);

	//complete
	GET_SLOT(UTextBlock, TB_EventExplainComplete);
	GET_SLOT(UTextBlock, TB_GetRewardBtnComplete);

	GET_SLOT(UWidgetSwitcher, WS_EventSlot);
}

void UB2UIEventSlotStageClear::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetRewardPossible, &UB2UIEventSlotStageClear::OnClickBTN_GetRewardPossible);
}

void UB2UIEventSlotStageClear::UpdateStaticText()
{
	int32 iStageID = ChachedEventRewardInfo.stage;

	int32 iStageDifficulty = iStageID / 1000;
	int32 iChapterNum = 1;
	int32 iStageNum = 1;

	int32 iComputeStage = (iStageID % 100);
	if (iComputeStage % 10 == 0)	//10
	{
		iChapterNum = iComputeStage / 10;
		iStageNum = 10;
	}
	else //1, 2...9
	{
		iChapterNum = iComputeStage / 10 + 1;
		iStageNum = iComputeStage % 10;
	}

	//FString DescriptionFormat = TEXT("StageDifficulty%d");
	FString DescriptionKey = FString::Printf(TEXT("StageDifficulty%d"), iStageDifficulty);

	FText ftStageDifficulty = BladeIIGetLOCText(B2LOC_CAT_GENERAL, DescriptionKey);
	FText ftStageFullText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageNum")), ftStageDifficulty, FText::AsNumber(iChapterNum), FText::AsNumber(iStageNum));

	FText ftEventSlot = FText::Format(FText::FromString(TEXT("{0} {1}")), ftStageFullText, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DungeonClearDesc")));

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

void UB2UIEventSlotStageClear::OnClickBTN_GetRewardPossible()
{
	if (iEventState != b2network::B2MissionStateType::COMPLETED)
		return;

	data_trader::Retailer::GetInstance().RequestReceiveStageClearEventReward(iEventID, iEventIndex);

	iEventState = b2network::B2MissionStateType::REWARDED;

	UpdateVisibleState();
}

void UB2UIEventSlotStageClear::InitEventSlotStageClear(const b2network::B2mdStageClearEvent& EventRewardInfo)
{
	iEventID = EventRewardInfo.event_id;
	iEventIndex = EventRewardInfo.index;

	ChachedEventRewardInfo = EventRewardInfo;

	Super::Init();

	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->Init();
		UIP_RewardIcon->UpdateRewardIcon(EventRewardInfo.reward_id, EventRewardInfo.reward_count);
	}
}

void UB2UIEventSlotStageClear::UpdateVisibleState()
{
	if (WS_EventSlot.IsValid())
		WS_EventSlot->SetActiveWidgetIndex(iEventState);
}
