// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIRankingRewardPart.h"
#include "B2ItemInfo.h"


#include "BladeIIGameImpl.h"

void UB2UIRankingRewardPart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Item);
	GET_SLOT(UTextBlock, TB_Count);

	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);
}

void UB2UIRankingRewardPart::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIRankingRewardPart::Init()
{
	Super::Init();
}

void UB2UIRankingRewardPart::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIRankingRewardPart::SetItemInfo(const b2network::B2RewardPtr& RewardItem)
{
	int32 itemid = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(RewardItem->raw_reward->id); 

	if (IMG_Item.IsValid())
	{
		UMaterialInterface* IconMat = StaticFindItemInfo() ? StaticFindItemInfo()->GetItemIcon(itemid) : nullptr;
		if (IconMat)
			IMG_Item->SetBrushFromMaterial(IconMat);
		if (BTN_Goods.IsValid())
			BTN_Goods->SetItemInfo(itemid);
	}

	if (TB_Count.IsValid())
	{
		TB_Count->SetText(FText::FromString(FString::Printf(TEXT(" x %d"), RewardItem->raw_reward->count)));
	}

}


