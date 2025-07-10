#include "B2UIMsgPopupHeroTowerSweep.h"


#include "BladeIIGameImpl.h"

void UB2UIMsgPopupHeroTowerSweep::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_TipText);
	GET_SLOT(UB2RichTextBlock, TB_CounterDungeonTip);
	
	const int SweepRewardCount = 3;
	TB_AddItems.SetNum(SweepRewardCount);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_AddItems[0], TB_AddComment01);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_AddItems[1], TB_AddComment02);
	GET_SLOT_BYNAME(UB2RichTextBlock, TB_AddItems[2], TB_AddComment03);

	BTN_Goods.SetNum(SweepRewardCount);
	GET_SLOT_BYNAME(UB2ButtonGoodInfoToolTip, BTN_Goods[0], BTN_Goods01);
	GET_SLOT_BYNAME(UB2ButtonGoodInfoToolTip, BTN_Goods[1], BTN_Goods02);
	GET_SLOT_BYNAME(UB2ButtonGoodInfoToolTip, BTN_Goods[2], BTN_Goods03);

	P_RewardItem.SetNum(SweepRewardCount);
	GET_SLOT_BYNAME(UPanelWidget, P_RewardItem[0], P_SweepItem01);
	GET_SLOT_BYNAME(UPanelWidget, P_RewardItem[1], P_SweepItem02);
	GET_SLOT_BYNAME(UPanelWidget, P_RewardItem[2], P_SweepItem03);

	IMG_Items.SetNum(SweepRewardCount);
	GET_SLOT_BYNAME(UImage, IMG_Items[0], IMG_Item01);
	GET_SLOT_BYNAME(UImage, IMG_Items[1], IMG_Item02);
	GET_SLOT_BYNAME(UImage, IMG_Items[2], IMG_Item03);

	for (auto& SweepRewardItem : P_RewardItem)
	{
		if (SweepRewardItem.IsValid())
		{
			SweepRewardItem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIMsgPopupHeroTowerSweep::SetAddComment(const FText& comment)
{
	if (TB_AddItems.IsValidIndex(0) && TB_AddItems[0].IsValid())
	{
		TB_AddItems[0]->SetText(comment);
	}
	if (BTN_Goods.IsValidIndex(0) && BTN_Goods[0].IsValid())
	{
		BTN_Goods[0]->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);
	}
}

void UB2UIMsgPopupHeroTowerSweep::SetAddComment2(const FText& comment)
{
	if (TB_AddItems.IsValidIndex(1) && TB_AddItems[1].IsValid())
	{
		TB_AddItems[1]->SetText(comment);
	}
	if (BTN_Goods.IsValidIndex(1) && BTN_Goods[1].IsValid())
	{
		BTN_Goods[1]->SetItemInfo(FItemRefIDHelper::GetGoodsID_EXP());
	}
}


void UB2UIMsgPopupHeroTowerSweep::SetAddCommentForHeroTower(const TMap<int, FRewardItem>& Rewards)
{
	int Index = 0;
	for (auto& SweepItemReward : Rewards)
	{
		int32 itemid = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(SweepItemReward.Key);

		if (P_RewardItem.IsValidIndex(Index) && P_RewardItem[Index].IsValid())
		{
			P_RewardItem[Index]->SetVisibility(ESlateVisibility::Visible);
		}
		if (TB_AddItems.IsValidIndex(Index) && TB_AddItems[Index].IsValid())
		{
			if (SweepItemReward.Value.MaxCount == 0
				|| SweepItemReward.Value.Count == SweepItemReward.Value.MaxCount)
			{
				TB_AddItems[Index]->SetText(FText::AsNumber(SweepItemReward.Value.Count));
			}
			else
			{
				FText IconText;
				IconText = FText::FromString(FString::Printf(TEXT("%d ~ %d"), SweepItemReward.Value.Count, SweepItemReward.Value.MaxCount));
				TB_AddItems[Index]->SetText(IconText);
			}

		}
		if (BTN_Goods.IsValidIndex(Index) && BTN_Goods[Index].IsValid())
		{
			BTN_Goods[Index]->SetItemInfo(itemid);
		}
		if (IMG_Items.IsValidIndex(Index) && IMG_Items[Index].IsValid())
		{
			IMG_Items[Index]->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(SweepItemReward.Key));
		}
		++Index;
	}
}

void UB2UIMsgPopupHeroTowerSweep::SetTipText(const FText& comment)
{
	if (TB_TipText.IsValid())
	{
		TB_TipText->SetText(comment);
	}
}

void UB2UIMsgPopupHeroTowerSweep::SetCounterDungeonTip(const FText& comment)
{
	if (TB_CounterDungeonTip.IsValid())
		TB_CounterDungeonTip->SetText(comment);
}
