// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRewardSlot.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"




UB2UIRewardSlot::UB2UIRewardSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UB2UIRewardSlot::Init()
{
	Super::Init();
}

void UB2UIRewardSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIRewardSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage,IMG_RewardIcon);

	GET_SLOT(UTextBlock,TB_RewardName);
	GET_SLOT(UTextBlock,TB_RewardAmount);
	
}


void UB2UIRewardSlot::InitRewardSlotTemplate(MailGiftType RewardType)
{
	for (const auto& DispInfo : RewardDisplayInfo)
	{
		if (DispInfo.RewardType == RewardType)
		{
			SetRewardSlotDefaultDisplay(DispInfo);
			return;
		}
	}
}

void UB2UIRewardSlot::SetRewardSlotDefaultDisplay(const FRewardDisplayInfo& InRewardDisplayInfo)
{
	if (IMG_RewardIcon.IsValid() && InRewardDisplayInfo.RewardIconMaterial != nullptr)
	{
		IMG_RewardIcon->SetBrushFromMaterial(InRewardDisplayInfo.RewardIconMaterial);
	}

	if (TB_RewardName.IsValid())
	{
		TB_RewardName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), InRewardDisplayInfo.RewardLocTextKey));
	}
}

void UB2UIRewardSlot::SetRewardSlotItem(int32 ItemID)
{
	// 클라이언트 아이템 리소스 테이블
	UB2ItemInfo* StaticItemTable = StaticFindItemInfo();
	FSingleItemInfoData* DisplayItemInfo = StaticItemTable ? StaticItemTable->GetInfoData(ItemID) : nullptr;

	if (IMG_RewardIcon.IsValid())
	{
		IMG_RewardIcon->SetBrushFromMaterial(DisplayItemInfo ? DisplayItemInfo->GetIconMaterial(StaticItemTable) : nullptr);
	}
	if (TB_RewardName.IsValid())
	{
		TB_RewardName->SetText(DisplayItemInfo ? DisplayItemInfo->GetLocalizedName() : FText::FromString(TEXT("Unknown Item")));
	}
}

void UB2UIRewardSlot::SetRewardAmount(int32 Amount)
{
	if (TB_RewardAmount.IsValid())
	{
		TB_RewardAmount->SetText(FText::FromString(FString::Printf(TEXT("X %d"), Amount)));
	}
}