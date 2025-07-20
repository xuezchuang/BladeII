// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIDimensionWeekRewardPopup.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDocHelper.h"


void UB2UIDimensionWeekRewardPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_MyRank);
	GET_SLOT(UTextBlock, TB_RankPercent);
	GET_SLOT(UTextBlock, TB_MaxStep);
	GET_SLOT(UTextBlock, TB_Timer);

	GET_SLOT(UTextBlock, STB_MyRanking);
	GET_SLOT(UTextBlock, STB_MaxStep);
	GET_SLOT(UTextBlock, STB_Step);
	GET_SLOT(UTextBlock, STB_MyRankingUnit);
	GET_SLOT(UTextBlock, STB_SendMail);
	GET_SLOT(UTextBlock, STB_WeekRewardTitle);
	
	GET_SLOT(UB2Button, Btn_Close);

	RewardSlots.SetNum(2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardSlots[0], Reward1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, RewardSlots[1], Reward2);

	for (int32 i = 0; i < RewardSlots.Num(); i++)
	{
		if (RewardSlots[i].IsValid())
			RewardSlots[i]->Init();
	}
}

void UB2UIDimensionWeekRewardPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(Btn_Close, &UB2UIDimensionWeekRewardPopup::OnClickBtn_Close);
}

void UB2UIDimensionWeekRewardPopup::Init()
{
	Super::Init();
}

void UB2UIDimensionWeekRewardPopup::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIDimensionWeekRewardPopup::OnClickBtn_Close()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIDimensionWeekRewardPopup::CloseWidgetDelegate()
{
	OnClickBtn_Close();
}

void UB2UIDimensionWeekRewardPopup::SetReward(b2network::B2DimensionRankingRewardPtr Rewards)
{	
	if (TB_MyRank.IsValid())
		TB_MyRank->SetText(FText::AsNumber(Rewards->ranking));

	if (TB_RankPercent.IsValid())
	{
		float nPercent = Rewards->ranker_count > 0 ? static_cast<float>(Rewards->ranking) / static_cast<float>(Rewards->ranker_count) * 100 : 0;
		TB_RankPercent->SetText(FText::FromString(FString::Printf(TEXT("(%.1f%%)"), nPercent)));
	}

	if (TB_MaxStep.IsValid())
		TB_MaxStep->SetText(FText::AsNumber(Rewards->best_difficulty_level));

	if (TB_Timer.IsValid())
		TB_Timer->SetText(FText::FromString(UB2UIManager::FormatMilliSecondsAsTime(Rewards->best_clear_time_millis, TEXT("hh:mm:ss.ms"))));

	for (int32 i = 0; i < Rewards->rewards.Num(); i++)
	{
		if (RewardSlots.IsValidIndex(i) && RewardSlots[i].IsValid())
		{
			RewardSlots[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RewardSlots[i]->UpdateRewardIcon(Rewards->rewards[i]);
		}
	}
}

void UB2UIDimensionWeekRewardPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_WeekRewardTitle.IsValid())
		STB_WeekRewardTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardTitle"))));
	if (STB_MyRanking.IsValid())
		STB_MyRanking->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardMyRanking"))));
	if (STB_MaxStep.IsValid())
		STB_MaxStep->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DimensionResult_PlayerTopDifficulty"))));
	if (STB_Step.IsValid())
		STB_Step->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_NDifficulty")), FText::GetEmpty()));
	if (STB_MyRankingUnit.IsValid())
		STB_MyRankingUnit->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("PVP1on1_RankUnit"))));
	if (STB_SendMail.IsValid())
		STB_SendMail->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_GetMailReward"))));
}