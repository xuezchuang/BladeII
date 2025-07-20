// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISeasonEventCombineMenuBtn.h"

#include "B2UIManager.h"
#include "Retailer.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
#include "RewardEventManager.h"

void UB2UISeasonEventCombineMenuBtn::Init()
{
	Super::Init();

	InitEventIDSeasonEvent();

	SubscribeEvents();

	SeasonEventVisbleCheck();
}

void UB2UISeasonEventCombineMenuBtn::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UISeasonEventCombineMenuBtn::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_SeasonEvent.Get()), &UB2UISeasonEventCombineMenuBtn::HasRedDot_SeasonEvent);
}

void UB2UISeasonEventCombineMenuBtn::SubscribeEvents()
{
	if (bSubscribed == false)
	{
		UnsubscribeEvents();

		bSubscribed = true;

		Issues.Add(GetLatestEventInfoSuccessfulClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->SeasonEventVisbleCheck();
		}
		));

		Issues.Add(GetEventStatusSuccessfulClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->SeasonEventVisbleCheck();
		}
		));
	}
}

void UB2UISeasonEventCombineMenuBtn::UnsubscribeEvents()
{
	bSubscribed = false;
	Issues.Empty();
}

void UB2UISeasonEventCombineMenuBtn::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_SeasonEvent);

	GET_SLOT(UTextBlock, TB_SeasonEvent);
}

void UB2UISeasonEventCombineMenuBtn::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_SeasonEvent, &UB2UISeasonEventCombineMenuBtn::OnClickBTN_SeasonEvent);
}

void UB2UISeasonEventCombineMenuBtn::UpdateStaticText()
{
	if (TB_SeasonEvent.IsValid())
		TB_SeasonEvent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_SeasonEventMenu")));
}

void UB2UISeasonEventCombineMenuBtn::OnClickBTN_SeasonEvent()
{
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SeasonEvent);
}

bool UB2UISeasonEventCombineMenuBtn::HasRedDot_SeasonEvent()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_SeasonEvent();
}

void UB2UISeasonEventCombineMenuBtn::SeasonEventVisbleCheck()
{
	bool IsVisible = false;

	if (IsEnableDateSeasonEvent() == true)
	{
		const TArray<b2network::B2SeasonMissionEventStatus>* pArraySeasonEventStatus = RewardEventManager::GetInstance().GetArrayEventStatusInfo_Season(SeasonEventID);
		if (pArraySeasonEventStatus)
		{
			if (pArraySeasonEventStatus->Num() > 0)
			{
				IsVisible = true;
			}
		}
	}

	this->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

bool UB2UISeasonEventCombineMenuBtn::IsEnableDateSeasonEvent()
{
	return RewardEventManager::GetInstance().IsEnableEventID(SeasonEventID);
}

void UB2UISeasonEventCombineMenuBtn::InitEventIDSeasonEvent()
{
	SeasonEventID = RewardEventManager::GetInstance().GetEnableEventIDFromCategory(b2network::B2EventCategory::SEASON_MISSION);
}
