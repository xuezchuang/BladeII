// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationRankingRewardPopup.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"
#include "B2UIRewardEventSet.h"
#include "B2AndroidBackManager.h"
#include "BladeIILocText.h"

void UB2UIDonationRankingRewardPopup::Init()
{
	Super::Init();

	for (TWeakObjectPtr<UB2UIRewardIcon> Elem : UIP_RewardIcons)
	{
		if (Elem.IsValid())
		{
			Elem->Init();
			Elem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIDonationRankingRewardPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (TWeakObjectPtr<UB2UIRewardIcon> Elem : UIP_RewardIcons)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}
}

void UB2UIDonationRankingRewardPopup::SetInfo(int32 InRanking)
{
	for (TWeakObjectPtr<UB2UIRewardIcon> Elem : UIP_RewardIcons)
	{
		if (Elem.IsValid())
		{
			Elem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		FDonationPointRankingRewardInfo RewardInfo = DocDonation->GetDonationPointRankingRewardInfo(InRanking);

		if (RewardInfo.RewardID1 > 0 && UIP_RewardIcons.IsValidIndex(0) && UIP_RewardIcons[0].IsValid())
		{
			UIP_RewardIcons[0]->UpdateRewardIcon(RewardInfo.RewardID1, RewardInfo.RewardCount1);
			UIP_RewardIcons[0]->SetVisibility(ESlateVisibility::Visible);
		}

		if (RewardInfo.RewardID2 > 0 && UIP_RewardIcons.IsValidIndex(1) && UIP_RewardIcons[1].IsValid())
		{
			UIP_RewardIcons[1]->UpdateRewardIcon(RewardInfo.RewardID2, RewardInfo.RewardCount2);
			UIP_RewardIcons[1]->SetVisibility(ESlateVisibility::Visible);
		}

		if (RewardInfo.RewardID3 > 0 && UIP_RewardIcons.IsValidIndex(2) && UIP_RewardIcons[2].IsValid())
		{
			UIP_RewardIcons[2]->UpdateRewardIcon(RewardInfo.RewardID3, RewardInfo.RewardCount3);
			UIP_RewardIcons[2]->SetVisibility(ESlateVisibility::Visible);
		}

		if (RewardInfo.RewardID4 > 0 && UIP_RewardIcons.IsValidIndex(3) && UIP_RewardIcons[3].IsValid())
		{
			UIP_RewardIcons[3]->UpdateRewardIcon(RewardInfo.RewardID4, RewardInfo.RewardCount4);
			UIP_RewardIcons[3]->SetVisibility(ESlateVisibility::Visible);
		}

		if (RewardInfo.RewardID5 > 0 && UIP_RewardIcons.IsValidIndex(4) && UIP_RewardIcons[4].IsValid())
		{
			UIP_RewardIcons[4]->UpdateRewardIcon(RewardInfo.RewardID5, RewardInfo.RewardCount5);
			UIP_RewardIcons[4]->SetVisibility(ESlateVisibility::Visible);
		}

		if (TB_Desc.IsValid())
		{
			TB_Desc->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_SendRewardMsg")), InRanking));
		}
	}
}

void UB2UIDonationRankingRewardPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDonationRankingRewardPopup::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_OK, &UB2UIDonationRankingRewardPopup::OnClickClose);
}

void UB2UIDonationRankingRewardPopup::CacheAssets()
{
	Super::CacheAssets();

	UIP_RewardIcons.SetNum(MAXREWARD);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[0], UIP_RewardIcon_0);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[1], UIP_RewardIcon_1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[2], UIP_RewardIcon_2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[3], UIP_RewardIcon_3);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[4], UIP_RewardIcon_4);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_OK);
	GET_SLOT(UTextBlock, TB_Desc);
	GET_SLOT(UB2Button, BTN_OK);
	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UIDonationRankingRewardPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	}

	if (TB_OK.IsValid())
	{
		TB_OK->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}
}

void UB2UIDonationRankingRewardPopup::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIDonationRankingRewardPopup::OnClickClose()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	SetVisibility(ESlateVisibility::Collapsed);
}
