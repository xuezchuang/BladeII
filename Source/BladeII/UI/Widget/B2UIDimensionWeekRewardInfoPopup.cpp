// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIRewardEventSet.h"
#include "B2ItemInfo.h"
#include "B2RewardInfo.h"
#include "B2UIDimensionWeekRewardInfoPopup.h"

#include "BladeIIGameImpl.h"

void UB2UIDimensionWeekRewardInfoPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_WeeklyReward);
	GET_SLOT(UB2Button, BTN_Close);

	RewardSlots.SetNum(7);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[0], Part_1);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[1], Part_2);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[2], Part_3);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[3], Part_4);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[4], Part_5);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[5], Part_6);
	GET_SLOT_BYNAME(UB2UIDimensionWeekRewardInfoItem, RewardSlots[6], Part_7);

	for (int32 i = 0 ; i < RewardSlots.Num() ; i++)
	{
		if(RewardSlots[i].IsValid())
			RewardSlots[i]->Init();
	}
}

void UB2UIDimensionWeekRewardInfoPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDimensionWeekRewardInfoPopup::OnClickBTN_Close);
}

void UB2UIDimensionWeekRewardInfoPopup::Init()
{
	Super::Init();

	TArray<FB2DimensionWeekRewardInfo> RewardInfo = BladeIIGameImpl::GetTotemDataStore().GetDimensionWeekRewardInfos();

	for (int32 i = 0 ; i < RewardInfo.Num() ; i++)
	{
		if (RewardSlots.IsValidIndex(i))
		{
			bool bUseSub = (RewardInfo[i].Rewards.Num() == 1);
			
			if(bUseSub)
				RewardSlots[i]->SetItemInfo(true, RewardInfo[i].Ranking_min, RewardInfo[i].Ranking_max, RewardInfo[i].Rewards[0]->id, RewardInfo[i].Rewards[0]->count);
			else
				RewardSlots[i]->SetItemInfo(false, RewardInfo[i].Ranking_min, RewardInfo[i].Ranking_max, RewardInfo[i].Rewards[0]->id, RewardInfo[i].Rewards[0]->count, RewardInfo[i].Rewards[1]->id, RewardInfo[i].Rewards[1]->count);
		}
	}
}

void UB2UIDimensionWeekRewardInfoPopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIDimensionWeekRewardInfoPopup::OnClickBTN_Close()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIDimensionWeekRewardInfoPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

////////////////////////////////////////////////////////////////
// ListItem
///////////////////////////////////////////////////////////////

void UB2UIDimensionWeekRewardInfoItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Item);

	GET_SLOT(UTextBlock, TB_Rank);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UTextBlock, TB_SubCount);
	GET_SLOT(UTextBlock, TB_ItemName);

	GET_SLOT(UTextBlock, TB_OnlySubCount);

	GET_SLOT(UImage, IMG_RewardMainIcon);
	GET_SLOT(UImage, IMG_RewardSubIcon);
	GET_SLOT(UImage, IMG_OnlySub);

	GET_SLOT(UImage, IMG_OnlySub);

	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_ToolTip_Main);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_ToolTip_Sub);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTM_ToolTip_OnlySub);
}

void UB2UIDimensionWeekRewardInfoItem::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIDimensionWeekRewardInfoItem::Init()
{
	Super::Init();
}

void UB2UIDimensionWeekRewardInfoItem::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIDimensionWeekRewardInfoItem::SetItemInfo(bool bIsUseSub, int32 nMinRank, int32 nMaxRank, int32 nIndex1, int32 nCount1, int32 nIndex2, int32 nCount2)
{
	if (WS_Item.IsValid())
		WS_Item->SetActiveWidgetIndex(static_cast<int32>(bIsUseSub));

	if (TB_Rank.IsValid())
	{
		if (nMaxRank == 0)
			TB_Rank->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking3"))), FText::FromString(FString::FromInt(nMinRank))));
		else if (nMaxRank == nMinRank)
			TB_Rank->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking1"))), FText::FromString(FString::FromInt(nMaxRank))));
		else if (nMinRank < nMaxRank)
		{
			TB_Rank->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking2"))), FText::FromString(FString::FromInt(nMinRank)),
				FText::FromString(FString::FromInt(nMaxRank))));
		}
	}
	if (StaticFindRewardInfo())
	{
		if (bIsUseSub)
		{
			FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo()->GetInfoData(nIndex1);
			if (RewardInfo)
			{
				UMaterialInterface* Mat = StaticFindRewardInfo()->GetRewardIconMaterial(nIndex1);

				if (IMG_OnlySub.IsValid() && Mat)
					IMG_OnlySub->SetBrushFromMaterial(Mat);

				if (BTM_ToolTip_OnlySub.IsValid())
					BTM_ToolTip_OnlySub->SetItemInfo(RewardInfo->ItemInfoIndex);

				if (TB_OnlySubCount.IsValid())
					TB_OnlySubCount->SetText(FText::FromString("X " + FString::FromInt(nCount1)));
			}
		}
		else
		{
			FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo()->GetInfoData(nIndex1);
			if (RewardInfo)
			{
				UMaterialInterface* Mat = StaticFindRewardInfo()->GetRewardIconMaterial(nIndex1);
				if (IMG_RewardMainIcon.IsValid() && Mat)
					IMG_RewardMainIcon->SetBrushFromMaterial(Mat);

				if (BTN_ToolTip_Main.IsValid())
					BTN_ToolTip_Main->SetItemInfo(RewardInfo->ItemInfoIndex);

				if (TB_ItemName.IsValid())
					TB_ItemName->SetText(StaticFindItemInfo()->GetItemName(RewardInfo->ItemInfoIndex));

				if (TB_Count.IsValid())
					TB_Count->SetText(FText::FromString("X " + FString::FromInt(nCount1)));
			}

			FSingleRewardInfoData* RewardInfo2 = StaticFindRewardInfo()->GetInfoData(nIndex2);
			UMaterialInterface* Mat2 = StaticFindRewardInfo()->GetRewardIconMaterial(nIndex2);
			if (Mat2 && RewardInfo2)
			{
				if (IMG_RewardSubIcon.IsValid())
					IMG_RewardSubIcon->SetBrushFromMaterial(Mat2);

				if (TB_SubCount.IsValid())
					TB_SubCount->SetText(FText::FromString("X " + FString::FromInt(nCount2)));

				if (BTN_ToolTip_Sub.IsValid())
					BTN_ToolTip_Sub->SetItemInfo(RewardInfo2->ItemInfoIndex);
			}
		}
	}

}