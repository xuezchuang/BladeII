// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIRankingRewardRow.h"
#include "B2UIManager.h"

#include "B2UIRankingRewardPart.h"

void UB2UIRankingRewardRow::CacheAssets()
{
	Super::CacheAssets();


	/*RankingRewardParts.AddUninitialized(3);
	//GET_SLOT_BYNAME(UB2UIRankingRewardPart, RankingRewardParts[0], RankingRewardPart0);
	//GET_SLOT_BYNAME(UB2UIRankingRewardPart, RankingRewardParts[1], RankingRewardPart1);
	//GET_SLOT_BYNAME(UB2UIRankingRewardPart, RankingRewardParts[2], RankingRewardPart2);

	for (auto& Ele : RankingRewardParts)
	{
		if (Ele.IsValid())
		{
			Ele->Init();
		}
	}*/

	GET_SLOT(UTextBlock, TB_Ranking);
	GET_SLOT(UVerticalBox, X_VerticalBox);
}

void UB2UIRankingRewardRow::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIRankingRewardRow::Init()
{
	Super::Init();
}

void UB2UIRankingRewardRow::DestroySelf(UB2UIManager* InUIManager)
{

	/*if(RankingRewardPart0.IsValid()) RankingRewardPart0->DestroySelf(InUIManager);
	if(RankingRewardPart1.IsValid()) RankingRewardPart1->DestroySelf(InUIManager);
	if(RankingRewardPart2.IsValid()) RankingRewardPart2->DestroySelf(InUIManager);*/

	Super::DestroySelf(InUIManager);
}
//void UB2UIRankingRewardRow::SetRowInfo(const FString& Grade, const TArray<b2network::B2RewardPtr>& rewardItems)
void UB2UIRankingRewardRow::SetRowInfo(const FModWeeklyReward& WeeklyReward)
{
	SetRankingText(WeeklyReward);
	if (X_VerticalBox.IsValid())
		X_VerticalBox->ClearChildren();


	for (int32 i = 0; i < WeeklyReward.rewardItems.Num(); ++i)
	{
		if (X_VerticalBox.IsValid())
		{
			TSubclassOf<UB2UIRankingRewardPart> SubPart = UIP_RankingRewardPart.LoadSynchronous();

			UB2UIRankingRewardPart* UIPPart = CreateWidget<UB2UIRankingRewardPart>(this->GetOwningPlayer(), SubPart);

			if (UIPPart)
			{
				UVerticalBoxSlot* VerticalSlot = Cast<UVerticalBoxSlot>(X_VerticalBox->AddChild(UIPPart));
				UIPPart->Init();
				UIPPart->SetItemInfo(WeeklyReward.rewardItems[i]);

				if (VerticalSlot)
				{
					VerticalSlot->SetSize(ESlateSizeRule::Fill);
					VerticalSlot->SetHorizontalAlignment(HAlign_Fill);
					VerticalSlot->SetVerticalAlignment(VAlign_Fill);
				}
			
			}
		}


		/*if (RankingRewardParts[i].IsValid())
		{
			if (WeeklyReward.rewardItems.IsValidIndex(i))
			{
				RankingRewardParts[i]->SetVisibility(ESlateVisibility::Visible);
				RankingRewardParts[i]->SetItemInfo(WeeklyReward.rewardItems[i]);
			}
			else
			{
				RankingRewardParts[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}*/
	}
}

void UB2UIRankingRewardRow::SetRankingText(const FModWeeklyReward& WeeklyReward)
{
	const int32 GradeOne = 1;

	if (WeeklyReward.Grade == GradeOne)
	{
		if (WeeklyReward.Ranking_Max == WeeklyReward.Ranking_Min)
			TB_Ranking->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking1"))), FText::FromString(FString::FromInt(WeeklyReward.Ranking_Max))));
		else if (WeeklyReward.Ranking_Min < WeeklyReward.Ranking_Max)
		{
			TB_Ranking->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking2"))), FText::FromString(FString::FromInt(WeeklyReward.Ranking_Min)),
				FText::FromString(FString::FromInt(WeeklyReward.Ranking_Max))));
		}
		else if (!WeeklyReward.Ranking_Max)
			TB_Ranking->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking3"))), FText::FromString(FString::FromInt(WeeklyReward.Ranking_Min))));
	}
}


