// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEventTapPart.h"

#include "Event.h"
#include "B2RedDotManager.h"
#include "BladeIIGameImpl.h"
#include "B2ClientDataStore.h"

void UB2UIEventTapPart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_EventTap);

	GET_SLOT(UTextBlock, TB_Selected);
	GET_SLOT(UTextBlock, TB_UnSelected);

	GET_SLOT(UOverlay, O_Selected);
	GET_SLOT(UOverlay, O_Unselected);
}

void UB2UIEventTapPart::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_EventTap, &UB2UIEventTapPart::OnClickBTN_EventTap);
}

void UB2UIEventTapPart::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(), &UB2UIEventTapPart::HasRedDot_EventTap);
}

void UB2UIEventTapPart::InitEventTap(const b2network::B2mdEventInfo& fEventInfo)
{
	CachedEventInfo = fEventInfo;

	Super::Init();

	FText ftEventTabName = FText::FromString(fEventInfo.event_tab_name);

	if(TB_Selected.IsValid())
		TB_Selected->SetText(ftEventTabName);

	if(TB_UnSelected.IsValid())
		TB_UnSelected->SetText(ftEventTabName);
}

void UB2UIEventTapPart::SetSelected(bool bSelected)
{
	if (O_Selected.IsValid())
		O_Selected->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (O_Unselected.IsValid())
		O_Unselected->SetVisibility(bSelected ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

bool UB2UIEventTapPart::HasRedDot_EventTap()
{
	switch (GetEventCategory())
	{
	case b2network::B2EventCategory::POINT_REWARD:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_Point();
		break;

	case b2network::B2EventCategory::LEVEL_UP:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_Levelup();
		break;

	case b2network::B2EventCategory::STAGE_CLEAR:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_StageClear();
		break;

	case b2network::B2EventCategory::POINT_SHOP:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_PointConsume();
		break;

	case b2network::B2EventCategory::GEM_PURCHASE:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_GemPurchase();
		break;

	case b2network::B2EventCategory::SHOP_LOTTERY:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_TenLottery();
		break;

	case b2network::B2EventCategory::SPEND_GEM:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_GemSpend();
		break;

	case b2network::B2EventCategory::SHOP_LOTTERY_SAVINGS:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_TenLotterySaving();
		break;

	case b2network::B2EventCategory::DICE:
		return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Event_Dice();
		break;
	}

	return false;
}

void UB2UIEventTapPart::OnClickBTN_EventTap()
{
	SelectEventPageTabClass<int32>::GetInstance().Signal(CachedEventInfo.event_id);
}