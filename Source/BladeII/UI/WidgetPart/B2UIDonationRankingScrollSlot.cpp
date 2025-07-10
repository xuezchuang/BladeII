// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationRankingScrollSlot.h"
#include "B2UIDocDonation.h"


void UB2UIDonationRankingScrollSlot::SetData(FRankerData InRankerData)
{
	AccountID = InRankerData.AccountID;
	switch (InRankerData.Ranking)
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
			TB_Ranking->SetText(FText::AsNumber(InRankerData.Ranking));
		}
		break;
	}

	if (X_RTB_Name.IsValid())
	{
		X_RTB_Name->SetText(FText::FromString(InRankerData.NickName));
	}

	if (X_RTB_Point.IsValid())
	{
		X_RTB_Point->SetText(FText::AsNumber(InRankerData.DonationPoint));
	}
}

void UB2UIDonationRankingScrollSlot::OnClickUserInfo()
{
	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(AccountID, EPCClass::EPC_End, EPCClass::EPC_End, EPCClass::EPC_End);
}

void UB2UIDonationRankingScrollSlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_Ranking);
	GET_SLOT(UB2RichTextBlock, X_RTB_Name);
	GET_SLOT(UB2RichTextBlock, X_RTB_Point);
	GET_SLOT(UImage, IMG_RankBG_1);
	GET_SLOT(UImage, IMG_RankBG_2);
	GET_SLOT(UImage, IMG_RankBG_3);
	GET_SLOT(UB2Button, BTN_UserInfo);
}

void UB2UIDonationRankingScrollSlot::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_UserInfo, &UB2UIDonationRankingScrollSlot::OnClickUserInfo);
}
