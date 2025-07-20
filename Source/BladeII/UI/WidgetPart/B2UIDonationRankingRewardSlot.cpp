// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationRankingRewardSlot.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDocDonation.h"



void UB2UIDonationRankingRewardSlot::Init()
{
	Super::Init();

	if (UIP_RewardIcon1.IsValid())
	{
		UIP_RewardIcon1->Init();
	}

	if (UIP_RewardIcon2.IsValid())
	{
		UIP_RewardIcon2->Init();
	}

	if (UIP_RewardIcon3.IsValid())
	{
		UIP_RewardIcon3->Init();
	}

	if (UIP_RewardIcon4.IsValid())
	{
		UIP_RewardIcon4->Init();
	}

	if (UIP_RewardIcon5.IsValid())
	{
		UIP_RewardIcon5->Init();
	}
}

void UB2UIDonationRankingRewardSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_RewardIcon1.IsValid())
	{
		UIP_RewardIcon1->DestroySelf(InUIManager);
	}

	if (UIP_RewardIcon2.IsValid())
	{
		UIP_RewardIcon2->DestroySelf(InUIManager);
	}

	if (UIP_RewardIcon3.IsValid())
	{
		UIP_RewardIcon3->DestroySelf(InUIManager);
	}

	if (UIP_RewardIcon4.IsValid())
	{
		UIP_RewardIcon4->DestroySelf(InUIManager);
	}

	if (UIP_RewardIcon5.IsValid())
	{
		UIP_RewardIcon5->DestroySelf(InUIManager);
	}
}

void UB2UIDonationRankingRewardSlot::SetInfo(const FDonationPointRankingRewardInfo& InData)
{
	switch (InData.Rank)
	{
	case 1:
		if (IMG_RankBG_1.IsValid())
		{
			IMG_RankBG_1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (IMG_RankBG_2.IsValid())
		{
			IMG_RankBG_2->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_RankBG_3.IsValid())
		{
			IMG_RankBG_3->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (TB_Ranking.IsValid())
		{
			TB_Ranking->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case 2:
		if (IMG_RankBG_1.IsValid())
		{
			IMG_RankBG_1->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_RankBG_2.IsValid())
		{
			IMG_RankBG_2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (IMG_RankBG_3.IsValid())
		{
			IMG_RankBG_3->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (TB_Ranking.IsValid())
		{
			TB_Ranking->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case 3:
		if (IMG_RankBG_1.IsValid())
		{
			IMG_RankBG_1->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_RankBG_2.IsValid())
		{
			IMG_RankBG_2->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_RankBG_3.IsValid())
		{
			IMG_RankBG_3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (TB_Ranking.IsValid())
		{
			TB_Ranking->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	default:
		if (IMG_RankBG_1.IsValid())
		{
			IMG_RankBG_1->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_RankBG_2.IsValid())
		{
			IMG_RankBG_2->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_RankBG_3.IsValid())
		{
			IMG_RankBG_3->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (TB_Ranking.IsValid())
		{
			TB_Ranking->SetVisibility(ESlateVisibility::HitTestInvisible);
			TB_Ranking->SetText(FText::AsNumber(InData.Rank));
		}
		break;
	}

	if (CP_RewardItem1.IsValid())
	{
		if (InData.RewardID1 > 0)
		{
			CP_RewardItem1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			
			if (UIP_RewardIcon1.IsValid())
			{
				UIP_RewardIcon1->UpdateRewardIcon(InData.RewardID1, FText::GetEmpty());
			}
			
			if (X_RTB_Count1.IsValid())
			{
				X_RTB_Count1->SetText(FText::Format(FText::FromString(TEXT("x{0}")), FText::AsNumber(InData.RewardCount1)));
			}
		}
		else
		{
			CP_RewardItem1->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (CP_RewardItem2.IsValid())
	{
		if (InData.RewardID2 > 0)
		{
			CP_RewardItem2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (UIP_RewardIcon2.IsValid())
			{
				UIP_RewardIcon2->UpdateRewardIcon(InData.RewardID2, FText::GetEmpty());
			}

			if (X_RTB_Count2.IsValid())
			{
				X_RTB_Count2->SetText(FText::Format(FText::FromString(TEXT("x{0}")), FText::AsNumber(InData.RewardCount2)));
			}
		}
		else
		{
			CP_RewardItem2->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (CP_RewardItem3.IsValid())
	{
		if (InData.RewardID3 > 0)
		{
			CP_RewardItem3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (UIP_RewardIcon3.IsValid())
			{
				UIP_RewardIcon3->UpdateRewardIcon(InData.RewardID3, FText::GetEmpty());
			}

			if (X_RTB_Count3.IsValid())
			{
				X_RTB_Count3->SetText(FText::Format(FText::FromString(TEXT("x{0}")), FText::AsNumber(InData.RewardCount3)));
			}
		}
		else
		{
			CP_RewardItem3->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (CP_RewardItem4.IsValid())
	{
		if (InData.RewardID4 > 0)
		{
			CP_RewardItem4->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (UIP_RewardIcon4.IsValid())
			{
				UIP_RewardIcon4->UpdateRewardIcon(InData.RewardID4, FText::GetEmpty());
			}

			if (X_RTB_Count4.IsValid())
			{
				X_RTB_Count4->SetText(FText::Format(FText::FromString(TEXT("x{0}")), FText::AsNumber(InData.RewardCount4)));
			}
		}
		else
		{
			CP_RewardItem4->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (CP_RewardItem5.IsValid())
	{
		if (InData.RewardID5 > 0)
		{
			CP_RewardItem5->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (UIP_RewardIcon5.IsValid())
			{
				UIP_RewardIcon5->UpdateRewardIcon(InData.RewardID5, FText::GetEmpty());
			}

			if (X_RTB_Count5.IsValid())
			{
				X_RTB_Count5->SetText(FText::Format(FText::FromString(TEXT("x{0}")), FText::AsNumber(InData.RewardCount5)));
			}
		}
		else
		{
			CP_RewardItem5->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIDonationRankingRewardSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_RewardItem1);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon1);
	GET_SLOT(UB2RichTextBlock, X_RTB_Count1);
	GET_SLOT(UCanvasPanel, CP_RewardItem2);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon2);
	GET_SLOT(UB2RichTextBlock, X_RTB_Count2);
	GET_SLOT(UCanvasPanel, CP_RewardItem3);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon3);
	GET_SLOT(UB2RichTextBlock, X_RTB_Count3);
	GET_SLOT(UCanvasPanel, CP_RewardItem4);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon4);
	GET_SLOT(UB2RichTextBlock, X_RTB_Count4);
	GET_SLOT(UCanvasPanel, CP_RewardItem5);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon5);
	GET_SLOT(UB2RichTextBlock, X_RTB_Count5);
	GET_SLOT(UImage, IMG_RankBG_1);
	GET_SLOT(UImage, IMG_RankBG_2);
	GET_SLOT(UImage, IMG_RankBG_3);
	GET_SLOT(UTextBlock, TB_Ranking);
}
