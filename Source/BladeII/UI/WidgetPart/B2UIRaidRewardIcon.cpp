// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidRewardIcon.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"

void UB2UIRaidRewardIcon::Init()
{
	Super::Init();
}

void UB2UIRaidRewardIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_RewardMainIcon);
	GET_SLOT(UTextBlock, TB_RewardDetailInfo);
}

UMaterialInterface* UB2UIRaidRewardIcon::GetIconMaterialFromItemTable(int32 RewardID)
{
	return StaticFindItemInfo() ? StaticFindItemInfo()->GetItemIcon(RewardID) : nullptr;
}

void UB2UIRaidRewardIcon::UpdateRewardIcon(class UMaterialInterface* Image, const FText& Text)
{
	if (IMG_RewardMainIcon.IsValid())
	{
		IMG_RewardMainIcon->SetBrushFromMaterial(Image);
	}
	if (TB_RewardDetailInfo.IsValid())
	{
		TB_RewardDetailInfo->SetText(Text);
	}
}

void UB2UIRaidRewardIcon::UpdateRewardIcon(int32 RewardID, const FText& Text)
{
	if (IMG_RewardMainIcon.IsValid())
	{
		IMG_RewardMainIcon->SetBrushFromMaterial(GetIconMaterialFromItemTable(RewardID));
	}
	if (TB_RewardDetailInfo.IsValid())
	{
		TB_RewardDetailInfo->SetText(Text);
	}
}
