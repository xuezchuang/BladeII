// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDonationRewardInfoPopup.h"
#include "BladeIIUtil.h"
#include "B2UIDonationRewardSlot.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"
#include "B2UIDonationRankingRewardSlot.h"
#include "B2RewardInfo.h"


void UB2UIDonationRewardInfoPopup::Init()
{
	Super::Init();

	UpdateRankingRewardScroll();

	OnClickDonationReward();
}

void UB2UIDonationRewardInfoPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (TWeakObjectPtr<UB2UIDonationRewardSlot> Elem : DonationRewardSlots)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}

	for (TWeakObjectPtr<UB2UIDonationRankingRewardSlot> Elem : DonationRankingRewardSlots)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}
}

void UB2UIDonationRewardInfoPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_DonationSelected);
	GET_SLOT(UTextBlock, TB_DonationUnselected);
	GET_SLOT(UTextBlock, TB_RankingSelected);
	GET_SLOT(UTextBlock, TB_RankingUnselected);
	GET_SLOT(UTextBlock, TB_Talk);
	GET_SLOT(UTextBlock, TB_Name);
	GET_SLOT(UUniformGridPanel, UGP_Reward);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_DonationReward);
	GET_SLOT(UB2Button, BTN_RankingReward);
	GET_SLOT(UB2ScrollBox, SB_Reward);
	GET_SLOT(UB2ScrollBox, SB_RankingReward);
	GET_SLOT(UOverlay, OV_DonationUnselected);
	GET_SLOT(UOverlay, OV_DonationSelected);
	GET_SLOT(UOverlay, OV_RankingUnselected);
	GET_SLOT(UOverlay, OV_RankingSelected);
	GET_SLOT(UB2Button, BTN_FixedReward);
	GET_SLOT(UOverlay, OV_FixedUnselected);
	GET_SLOT(UOverlay, OV_FixedSelected);
	GET_SLOT(UTextBlock, TB_FixedUnselected);
	GET_SLOT(UTextBlock, TB_FixedSelected);
	GET_SLOT(UTextBlock, TB_FixedRewardDesc);
}

void UB2UIDonationRewardInfoPopup::UpdateStaticText()
{
	Super::UpdateStaticText();
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RewardInfoButton")));
	}

	if (TB_DonationSelected.IsValid())
	{
		TB_DonationSelected->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_ConRewardTitle")));
	}

	if (TB_DonationUnselected.IsValid())
	{
		TB_DonationUnselected->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_ConRewardTitle")));
	}

	if (TB_RankingSelected.IsValid())
	{
		TB_RankingSelected->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RankRewardTitle")));
	}

	if (TB_RankingUnselected.IsValid())
	{
		TB_RankingUnselected->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RankRewardTitle")));
	}

	if (TB_Talk.IsValid())
	{
		TB_Talk->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_NPCMessage")));
	}

	if (TB_Name.IsValid())
	{
		TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(100003)));
	}

	if (TB_FixedSelected.IsValid())
	{
		TB_FixedSelected->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_CertainRewardTitle")));
	}
	
	if (TB_FixedUnselected.IsValid())
	{
		TB_FixedUnselected->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_CertainRewardTitle")));
	}

	if (TB_FixedRewardDesc.IsValid())
	{
		TB_FixedRewardDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_CertainRewardNoti")));
	}
}

void UB2UIDonationRewardInfoPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDonationRewardInfoPopup::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_DonationReward, &UB2UIDonationRewardInfoPopup::OnClickDonationReward);
	BIND_CLICK_FUNC_TO_BTN(BTN_RankingReward, &UB2UIDonationRewardInfoPopup::OnClickRankingReward);
	BIND_CLICK_FUNC_TO_BTN(BTN_FixedReward, &UB2UIDonationRewardInfoPopup::OnClickFixedReward);
}

void UB2UIDonationRewardInfoPopup::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIDonationRewardInfoPopup::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::DonationRewardInfoPopup);
}

void UB2UIDonationRewardInfoPopup::OnClickDonationReward()
{
	if (SB_Reward.IsValid())
	{
		SB_Reward->SetVisibility(ESlateVisibility::Visible);
	}

	if (SB_RankingReward.IsValid())
	{
		SB_RankingReward->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_DonationSelected.IsValid())
	{
		OV_DonationSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_DonationUnselected.IsValid())
	{
		OV_DonationUnselected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_RankingUnselected.IsValid())
	{
		OV_RankingUnselected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_RankingSelected.IsValid())
	{
		OV_RankingSelected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_FixedSelected.IsValid())
	{
		OV_FixedSelected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_FixedUnselected.IsValid())
	{
		OV_FixedUnselected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (TB_FixedRewardDesc.IsValid())
	{
		TB_FixedRewardDesc->SetVisibility(ESlateVisibility::Collapsed);
	}

	UpdateDonationRewardScroll();
}

void UB2UIDonationRewardInfoPopup::OnClickRankingReward()
{
	if (SB_Reward.IsValid())
	{
		SB_Reward->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (SB_RankingReward.IsValid())
	{
		SB_RankingReward->SetVisibility(ESlateVisibility::Visible);
	}

	if (OV_DonationSelected.IsValid())
	{
		OV_DonationSelected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_DonationUnselected.IsValid())
	{
		OV_DonationUnselected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_RankingUnselected.IsValid())
	{
		OV_RankingUnselected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_RankingSelected.IsValid())
	{
		OV_RankingSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_FixedSelected.IsValid())
	{
		OV_FixedSelected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_FixedUnselected.IsValid())
	{
		OV_FixedUnselected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (TB_FixedRewardDesc.IsValid())
	{
		TB_FixedRewardDesc->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIDonationRewardInfoPopup::OnClickFixedReward()
{
	if (SB_Reward.IsValid())
	{
		SB_Reward->SetVisibility(ESlateVisibility::Visible);
	}

	if (SB_RankingReward.IsValid())
	{
		SB_RankingReward->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_DonationSelected.IsValid())
	{
		OV_DonationSelected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_DonationUnselected.IsValid())
	{
		OV_DonationUnselected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_RankingUnselected.IsValid())
	{
		OV_RankingUnselected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_RankingSelected.IsValid())
	{
		OV_RankingSelected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (OV_FixedSelected.IsValid())
	{
		OV_FixedSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (OV_FixedUnselected.IsValid())
	{
		OV_FixedUnselected->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (TB_FixedRewardDesc.IsValid())
	{
		TB_FixedRewardDesc->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	UpdateFixedRewardScroll();
}

void UB2UIDonationRewardInfoPopup::UpdateDonationRewardScroll()
{
	TArray<FDonationRewardSlotInfo> SlotInfos;
	SlotInfos.Empty();
	for (const FDonationRewardInfo& Elem : DonationRewardInfos)
	{
		FDonationRewardSlotInfo SlotInfo;
		SlotInfo.ItemName = Elem.RewardName;
		SlotInfo.Icon = Elem.RewardIcon.LoadSynchronous();
		SlotInfos.Add(SlotInfo);
	}
	UpdateRewardScroll(SlotInfos);
}

void UB2UIDonationRewardInfoPopup::UpdateRankingRewardScroll()
{
	TArray<FDonationPointRankingRewardInfo> RankingRewardInfos;
	RankingRewardInfos.Empty();
	
	if (SB_RankingReward.IsValid())
	{
		UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
		if (DocDonation)
		{
			DocDonation->GetDonationPointRankingRewardInfos(RankingRewardInfos);

			for (int i = 0; i < RankingRewardInfos.Num(); i++)
			{
				UB2UIDonationRankingRewardSlot* NewItem;
				if (DonationRankingRewardSlots.IsValidIndex(i) == false)
				{
					NewItem = CreateWidget<UB2UIDonationRankingRewardSlot>(GetWorld(), RankingRewardSlotClass);
					DonationRankingRewardSlots.Add(NewItem);
					NewItem->Init();
				}
				else
				{
					NewItem = DonationRankingRewardSlots[i].Get();
				}

				UPanelSlot* NewItemSlot = SB_RankingReward->AddChild(NewItem);
				
				NewItem->SetInfo(RankingRewardInfos[i]);
			}
		}
	}
}

void UB2UIDonationRewardInfoPopup::UpdateFixedRewardScroll()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		TArray<FDonationBundleFixedRewardInfo> BundleFixedRewardInfos;
		BundleFixedRewardInfos.Empty();
		DocDonation->GetDonationBundleFixedRewardInfos(BundleFixedRewardInfos);

		UB2RewardInfo* RewardInfo = StaticFindRewardInfo();
		if (RewardInfo)
		{
			TArray<FDonationRewardSlotInfo> FixedRewardInfos;
			FixedRewardInfos.Empty();
			for (const FDonationBundleFixedRewardInfo& Elem : BundleFixedRewardInfos)
			{
				FDonationRewardSlotInfo FixedRewardInfo;
				FixedRewardInfo.Icon = RewardInfo->GetRewardIconMaterial(Elem.RewardID);
				FixedRewardInfo.ItemName = RewardInfo->GetNameText(Elem.RewardID);
				FixedRewardInfos.Add(FixedRewardInfo);
			}

			UpdateRewardScroll(FixedRewardInfos);
		}
	}
}

void UB2UIDonationRewardInfoPopup::UpdateRewardScroll(TArray<FDonationRewardSlotInfo>& InDatas)
{
	if (UGP_Reward.IsValid())
	{
		for (TWeakObjectPtr<UB2UIDonationRewardSlot>& Elem : DonationRewardSlots)
		{
			if (Elem.IsValid())
			{
				Elem->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		int32 CurColumn = 0;
		int32 CurRow = 0;
		for (int i = 0; i < InDatas.Num(); i++)
		{
			UB2UIDonationRewardSlot* NewItem;
			if (DonationRewardSlots.IsValidIndex(i) == false)
			{
				NewItem = CreateWidget<UB2UIDonationRewardSlot>(GetWorld(), RewardSlotClass);
				DonationRewardSlots.Add(NewItem);
				NewItem->Init();
			}
			else
			{
				NewItem = DonationRewardSlots[i].Get();
			}

			UUniformGridSlot* NewItemSlot = UGP_Reward->AddChildToUniformGrid(NewItem);
			NewItemSlot->SetColumn(CurColumn);
			NewItemSlot->SetRow(CurRow);

			NewItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			NewItem->SetInfo(InDatas[i].ItemName, InDatas[i].Icon);

			CurColumn++;
			if (CurColumn >= MaxDonationRewardColumn)
			{
				CurColumn = 0;
				CurRow++;
			}
		}
	}
}

#if WITH_EDITOR
void FDonationRewardInfo::EditorOnPostLoad()
{
	RewardIcon.LoadSynchronous();
}
#endif