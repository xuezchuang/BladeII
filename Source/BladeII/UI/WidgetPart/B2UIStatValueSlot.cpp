// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIStatValueSlot.h"
#include "B2UIManager.h"



void UB2UIStatValueSlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_CombatLable);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UWidgetSwitcher, SW_UpDown);
}

void UB2UIStatValueSlot::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIStatValueSlot::Init()
{
	Super::Init();
}

void UB2UIStatValueSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIStatValueSlot::SetInfo(FText LableText, FText ValueText,bool bIsUP)
{
	if(TB_CombatLable.IsValid())
		TB_CombatLable->SetText(LableText);
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(ValueText);
	if (SW_UpDown.IsValid())
		SW_UpDown->SetActiveWidgetIndex(bIsUP);

	if (bIsUP)
		TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Increase);
	else
		TB_CombatPower->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);

}

