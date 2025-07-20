// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherLotterySlot.h"
#include "B2EtherManager.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"
#include "B2EtherInfo.h"
#include "B2UIStarGradePart.h"
#include "Event.h"

void UB2UIEtherLotterySlot::Init()
{
	Super::Init();

	if (UIP_Star.IsValid())
	{
		UIP_Star->Init();
	}
}

void UB2UIEtherLotterySlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_Star.IsValid())
	{
		UIP_Star->DestroySelf(InUIManager);
	}
}

void UB2UIEtherLotterySlot::SetEtherLotterySlotInfo(int32 InTypeID, int32 InStar, int32 InTier)
{
	TypeID = InTypeID;

	SetEtherInfo(TypeID, InTier);
	SetTierInfo(InTier);
	SetStarInfo(InStar);

	if (PW_Selected.IsValid())
	{
		PW_Selected->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIEtherLotterySlot::SetSelectedVisibility(int32 InType)
{
	if (PW_Selected.IsValid())
	{
		if (InType == TypeID)
		{
			PW_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else if(InType != TypeID && GetVisibility() != ESlateVisibility::Collapsed)
		{
			PW_Selected->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEtherLotterySlot::CacheAssets()
{
	GET_SLOT(UB2UIStarGradePart, UIP_Star);
	
	GET_SLOT(UImage, X_IMG_SetImage);
	GET_SLOT(UImage, X_IMG_TierBG);
	GET_SLOT(UTextBlock, X_TB_EtherTierName);
	GET_SLOT(UTextBlock, X_TB_EtherName);
	GET_SLOT(UImage, X_IMG_BG);
	GET_SLOT(UPanelWidget, PW_Selected);
}

void UB2UIEtherLotterySlot::OnClickedSenderBTNArea()
{
	SelectEtherLotterySlotClass<int32>::GetInstance().Signal(TypeID);
	TouchEtherLotterySlotPlaySound();
}

void UB2UIEtherLotterySlot::SetEtherInfo(int32 InTypeID, int32 InTier)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
	{
		UB2EtherInfo* EtherInfo = StaticFindEtherInfoTable();
		if (X_IMG_SetImage.IsValid() && EtherInfo != NULL)
		{
			X_IMG_SetImage->SetBrushFromMaterial(EtherInfo->GetEtherIcon(TypeID));
			FSlateColor Color = ItemMiscInfo->GetEtherTierTextColor(InTier);
			X_IMG_SetImage->SetColorAndOpacity(Color.GetSpecifiedColor());
		}

		if (X_TB_EtherName.IsValid())
		{
			X_TB_EtherName->SetColorAndOpacity(ItemMiscInfo->GetEtherTierTextColor(InTier));
			X_TB_EtherName->SetText(B2EtherManager::GetInstance().GetEtherName(TypeID));
		}
	}
}

void UB2UIEtherLotterySlot::SetTierInfo(int32 InTier)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo && X_IMG_TierBG.IsValid())
	{
		X_IMG_TierBG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierNameBGMtrl(InTier));
	}

	if (X_TB_EtherTierName.IsValid())
	{
		X_TB_EtherTierName->SetText(B2EtherManager::GetInstance().GetEtherTierName(InTier));
	}

	if (X_IMG_BG.IsValid())
	{
		X_IMG_BG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierBGMtrl(InTier));
	}
}

void UB2UIEtherLotterySlot::SetStarInfo(int32 InStar)
{
	if (UIP_Star.IsValid())
	{
		UIP_Star->UpdateStar(InStar, false);
	}
}
