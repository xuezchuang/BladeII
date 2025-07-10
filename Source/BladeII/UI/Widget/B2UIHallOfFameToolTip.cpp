// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIHallOfFameToolTip.h"
#include "B2UIManager.h"



void UB2UIHallOfFameToolTip::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, TB_ToolTip1);
	GET_SLOT(UB2RichTextBlock, TB_ToolTip2);
	GET_SLOT(UB2Button, BTN_ClosePraiseToolTip);
}

void UB2UIHallOfFameToolTip::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_ClosePraiseToolTip, &UB2UIHallOfFameToolTip::OnClickBTN_ClosePraiseToolTip);
}

void UB2UIHallOfFameToolTip::Init()
{
	Super::Init();
}

void UB2UIHallOfFameToolTip::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIHallOfFameToolTip::OnClickBTN_ClosePraiseToolTip()
{
	DestroySelf(UB2UIManager::GetInstance());
}


