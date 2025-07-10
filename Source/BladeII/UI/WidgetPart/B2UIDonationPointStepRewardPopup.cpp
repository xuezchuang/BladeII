// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationPointStepRewardPopup.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"
#include "Retailer.h"
#include "B2AndroidBackManager.h"
#include "B2UIManager.h"


void UB2UIDonationPointStepRewardPopup::Init()
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

void UB2UIDonationPointStepRewardPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	Issues.Empty();

	for (TWeakObjectPtr<UB2UIRewardIcon> Elem : UIP_RewardIcons)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}
}

void UB2UIDonationPointStepRewardPopup::SetInfo(int32 InStep)
{
	DonationRewardStep = InStep;

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
		FDonationPointStepRewardInfo RewardInfo = DocDonation->GetDonationPointStepRewardInfo(DonationRewardStep);
		
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

		if (BTN_GetReward.IsValid())
		{
			if (DocDonation->GetDonationPointStepState(DonationRewardStep) == EDonationPointStepState::Reward)
			{
				BTN_GetReward->SetIsEnabled(true);
			}
			else
			{
				BTN_GetReward->SetIsEnabled(false);
			}
		}
	}
}

void UB2UIDonationPointStepRewardPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDonationPointStepRewardPopup::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIDonationPointStepRewardPopup::OnClickGetReward);

	Issues.Add(UpdateDonationPointRewardUIClass<>::GetInstance().Subscribe2(
		[this]()
	{
		UB2UIManager::GetInstance()->OpenUI(UIFName::RewardMailPopup);
		this->OnClickClose();
	}
	));
}

void UB2UIDonationPointStepRewardPopup::CacheAssets()
{
	Super::CacheAssets();

	UIP_RewardIcons.SetNum(MAXREWARD);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[0], UIP_RewardIcon_0);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[1], UIP_RewardIcon_1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, UIP_RewardIcons[2], UIP_RewardIcon_2);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UIDonationPointStepRewardPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_MileageTitle")));
	}

	if (TB_GetReward.IsValid())
	{
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterStarRewardGet")));
	}
}

void UB2UIDonationPointStepRewardPopup::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIDonationPointStepRewardPopup::OnClickGetReward()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		data_trader::Retailer::GetInstance().RequestDonationPointReward(DocDonation->GetDonationID(), DonationRewardStep);
	}
}

void UB2UIDonationPointStepRewardPopup::OnClickClose()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	SetVisibility(ESlateVisibility::Collapsed);
}
