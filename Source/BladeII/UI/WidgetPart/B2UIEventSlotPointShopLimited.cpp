// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlotPointShopLimited.h"

#include "B2UIRewardEventSet.h"
#include "BladeIIUtil.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "Retailer.h"
#include "B2RewardInfo.h"

void UB2UIEventSlotPointShopLimited::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UB2RichTextBlock, RTB_BtnProgress);
	GET_SLOT(UB2RichTextBlock, RTB_BtnComplete);

	GET_SLOT(UB2RichTextBlock, RTB_ItemName);
	GET_SLOT(UB2RichTextBlock, RTB_ItemCount);

	GET_SLOT(UB2RichTextBlock, RTB_RewardExplain);
	GET_SLOT(UB2RichTextBlock, RTB_RewardPossibleCount);

	GET_SLOT(UB2Image, IMG_RewardIcon);
	GET_SLOT(UB2RichTextBlock, RTB_RewardIconItemCount);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_RewardIcon);

	GET_SLOT(UWidgetSwitcher, WS_EventState);
}

void UB2UIEventSlotPointShopLimited::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIEventSlotPointShopLimited::OnClickBTN_GetReward);
}

void UB2UIEventSlotPointShopLimited::UpdateStaticText()
{
	if (RTB_ItemName.IsValid())
	{
		int32 itemID = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(CachedEventRewardInfo.reward_id);
		RTB_ItemName->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, FString::FromInt(itemID)));
	}

	if (RTB_ItemCount.IsValid())
	{
		RTB_ItemCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemCount")), FText::AsNumber(CachedEventRewardInfo.reward_count)));
	}

	if (RTB_RewardExplain.IsValid())
	{
		RTB_RewardExplain->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("PointEvnetShop_Limit")));
	}

	FText ftRewardNeedPoint = FText::FromString(FString::Printf(TEXT("%d"), CachedEventRewardInfo.need_point));

	if (RTB_BtnProgress.IsValid())
	{
		RTB_BtnProgress->SetText(ftRewardNeedPoint);
	}

	if (RTB_BtnComplete.IsValid())
	{
		RTB_BtnComplete->SetText(ftRewardNeedPoint);
	}

	//default
	UpdateRewardPossibleCountText(CachedRewardedCount);
}

void UB2UIEventSlotPointShopLimited::InitEventSlotPointShopLimited(const b2network::B2mdPointEventReward& EventRewardInfo)
{
	iEventID = EventRewardInfo.event_id;
	iEventIndex = EventRewardInfo.index;

	CachedEventRewardInfo = EventRewardInfo;
	SetRewardedCount(0);

	this->Init();

	InitRewardIcon();
}

void UB2UIEventSlotPointShopLimited::InitRewardIcon()
{
	if (IMG_RewardIcon.IsValid())
	{
		UMaterialInterface* pMaterial = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetRewardIconMaterial(CachedEventRewardInfo.reward_id) : nullptr;
		if (pMaterial)
		{
			IMG_RewardIcon->SetBrushFromMaterial(pMaterial);
		}
	}

	if (RTB_RewardIconItemCount.IsValid())
	{
		FText ftRewardCount = FText::FromString(FString::Printf(TEXT("%d"), CachedEventRewardInfo.reward_count));
		RTB_RewardIconItemCount->SetText(ftRewardCount);
	}

	if (BTN_RewardIcon.IsValid())
	{
		FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(CachedEventRewardInfo.reward_id) : nullptr;
		if (RewardInfo != nullptr)
		{
			BTN_RewardIcon->SetItemInfo(RewardInfo->ItemInfoIndex);
		}
	}
}

void UB2UIEventSlotPointShopLimited::UpdateVisibleState()
{
	if (WS_EventState.IsValid())
	{
		//default, progress, completed
		WS_EventState->SetActiveWidgetIndex(GetEventState());
	}	
}

void UB2UIEventSlotPointShopLimited::Update_FromTapGroupID(const int32 iTapGroupID)
{
	bool IsVisible = iTapGroupID == CachedEventRewardInfo.tab_group_id ? true : false;

	if (IsVisible)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIEventSlotPointShopLimited::Update_FromAccruePoint(const int32 iAccruePoint)
{
	SetEventState(b2network::B2MissionStateType::PROGRESS);

	bool IsPossibleReward = (CachedEventRewardInfo.buy_limit_count - CachedRewardedCount) > 0 ? true : false;
	if (IsPossibleReward)
	{
		if (iAccruePoint >= CachedEventRewardInfo.need_point)
		{
			SetEventState(b2network::B2MissionStateType::COMPLETED);
		}
	}

	UpdateRewardPossibleCountText(CachedRewardedCount);

	UpdateVisibleState();
}

void UB2UIEventSlotPointShopLimited::UpdateRewardPossibleCountText(const int32 iRewardedCount)
{
	int32 iPossibleCount = CachedEventRewardInfo.buy_limit_count - iRewardedCount;
	if (iPossibleCount < 0)
	{
		iPossibleCount = 0;
	}

	FText ftRewardPossibleCount = FText::FromString(FString::Printf(TEXT("%d/%d"), iPossibleCount, CachedEventRewardInfo.buy_limit_count));

	if (RTB_RewardPossibleCount.IsValid())
	{
		RTB_RewardPossibleCount->SetText(ftRewardPossibleCount);
	}
}

void UB2UIEventSlotPointShopLimited::OnClickBTN_GetReward()
{
	//if (GetEventState() != b2network::B2MissionStateType::COMPLETED)
	//	return;

	//StratAnimEventMainTransparencyBlockClass<>::GetInstance().Signal();

	//data_trader::Retailer::GetInstance().RequestRewardPointShopEvent(iEventID, iEventIndex);
}

