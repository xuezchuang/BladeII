// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventTabPointShopLimited.h"

#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2ClientDataStore.h"

void UB2UIEventTabPointShopLimited::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_EventTap);
	GET_SLOT(UWidgetSwitcher, WS_State);
	GET_SLOT(UB2RichTextBlock, RTB_TapGroupNameSelected);
	GET_SLOT(UB2RichTextBlock, RTB_TapGroupNameUnSelected);
}

void UB2UIEventTabPointShopLimited::UpdateStaticText()
{
	int32 itemID = BladeIIGameImpl::GetClientDataStore().GetRewardItemId(CachedTapGroupRewardID);
	FText ftTapGroupName = BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, FString::FromInt(itemID));

	if (RTB_TapGroupNameSelected.IsValid())
	{
		RTB_TapGroupNameSelected->SetText(ftTapGroupName);
	}

	if (RTB_TapGroupNameUnSelected.IsValid())
	{
		RTB_TapGroupNameUnSelected->SetText(ftTapGroupName);
	}
}


void UB2UIEventTabPointShopLimited::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_EventTap, &UB2UIEventTabPointShopLimited::OnClickBTN_EventTap);
}

void UB2UIEventTabPointShopLimited::OnClickBTN_EventTap()
{
	SelectEventTabShopLimitedClass<int32>::GetInstance().Signal(CachedTapGroupID);
}

void UB2UIEventTabPointShopLimited::InitEventTab(const int32 iTapGroupID, const int32 iTapGroupRewarID)
{
	CachedTapGroupID = iTapGroupID;
	CachedTapGroupRewardID = iTapGroupRewarID;

	this->Init();
}

void UB2UIEventTabPointShopLimited::UpdateEventTabPointShopLimited(bool IsSelected)
{
	const int32 iWidgetSwitcherIndex = (int32)IsSelected;

	if (WS_State.IsValid())
	{
		WS_State->SetActiveWidgetIndex(iWidgetSwitcherIndex);
	}
}