// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHallOfFameSkipButton.h"
#include "B2AndroidBackManager.h"
#include "Event.h"

void UB2UIHallOfFameSkipButton::SetActive(bool InActive)
{
	if (InActive == true && GetVisibility() != ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
	else if(InActive == false && GetVisibility() != ESlateVisibility::Collapsed)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIHallOfFameSkipButton::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_MatineeBack);
}

void UB2UIHallOfFameSkipButton::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_MatineeBack, &UB2UIHallOfFameSkipButton::SkipMatinee);
}

void UB2UIHallOfFameSkipButton::CloseWidgetDelegate()
{
	SkipMatinee();
}

void UB2UIHallOfFameSkipButton::SkipMatinee()
{
	EndHallOfFameMatineeClass<>::GetInstance().Signal();
}
