// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISeasonEventTab.h"

#include "B2UIManager.h"

void UB2UISeasonEventTab::Init()
{
	Super::Init();
}

void UB2UISeasonEventTab::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISeasonEventTab::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_SeasonEventTab.Get()), &UB2UISeasonEventTab::HasRedDot_SeasonEventTab);
}

void UB2UISeasonEventTab::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_IMG_EventTab);

	const int32 iArrayMax = (int32)ESEASONEVENT_TAB_STATE::Max;
	ArrayTB_SeasonEventTab.SetNum(iArrayMax);
	GET_SLOT_BYNAME(UTextBlock, ArrayTB_SeasonEventTab[0], TB_SeasonEventDisable);
	GET_SLOT_BYNAME(UTextBlock, ArrayTB_SeasonEventTab[1], TB_SeasonEventUnSelected);
	GET_SLOT_BYNAME(UTextBlock, ArrayTB_SeasonEventTab[2], TB_SeasonEventSelected);

	GET_SLOT(UB2Button, BTN_SeasonEventTab);
}

void UB2UISeasonEventTab::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_SeasonEventTab, &UB2UISeasonEventTab::OnClickBTN_SeasonEventTab);
}

void UB2UISeasonEventTab::UpdateStaticText()
{
	FText fSeasonEventTab = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), iSeasonEventDay);

	const int32 iArrayMaxTB = ArrayTB_SeasonEventTab.Num();
	for (int32 i = 0; i < iArrayMaxTB; ++i)
	{
		if (ArrayTB_SeasonEventTab[i].IsValid())
			ArrayTB_SeasonEventTab[i]->SetText(fSeasonEventTab);
	}
}

void UB2UISeasonEventTab::UpdateSeasonEventTab(int32 iSelectedSeasonEventDay)
{
	ESEASONEVENT_TAB_STATE	eSeasonEventTabState = ESEASONEVENT_TAB_STATE::UnSelected;

	if (iSeasonEventProgressDay < iSeasonEventDay)
		eSeasonEventTabState = ESEASONEVENT_TAB_STATE::Disable;
	else if (iSelectedSeasonEventDay == iSeasonEventDay)
		eSeasonEventTabState = ESEASONEVENT_TAB_STATE::Selected;

	if (WS_IMG_EventTab.IsValid())
		WS_IMG_EventTab->SetActiveWidgetIndex((int32)eSeasonEventTabState);
}

void UB2UISeasonEventTab::OnClickBTN_SeasonEventTab()
{
	if (iSeasonEventProgressDay < iSeasonEventDay)
		return;

	SelectSeasonEventTabClass<int32>::GetInstance().Signal(iSeasonEventDay);
}

bool UB2UISeasonEventTab::HasRedDot_SeasonEventTab()
{
	return bCheckRedDot;
}
