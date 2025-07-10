// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIUpgradeStatNotify.h"

#include "B2UIManager.h"

#include "B2UIBattleScoreNotify.h"
#include "B2UIStatValueSlot.h"

void UB2UIUpgradeStatNotify::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UVerticalBox, List_Stat);
	GET_SLOT(UTextBlock, STB_Title);
}

void UB2UIUpgradeStatNotify::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUpgradeStatNotify::Init()
{
	Super::Init();

	if (List_Stat.IsValid())
		List_Stat->ClearChildren();
	if (STB_Title.IsValid())
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StatUpgradeSuccess")));
}

void UB2UIUpgradeStatNotify::DestroySelf(UB2UIManager* InUIManager)
{
	GetWorld()->GetTimerManager().ClearTimer(DelayViewTimerHandle);
	Super::DestroySelf(InUIManager);
}

void UB2UIUpgradeStatNotify::AddListItem(FText LableText, FText ValueText, bool bIsUP)
{
	if (List_Stat.IsValid())
	{
		UB2UIStatValueSlot* CreatedWidget = CreateWidget<UB2UIStatValueSlot>(GetOwningPlayer(), StaValueSlotClass);
		if (CreatedWidget)
		{
			CreatedWidget->Init();
			CreatedWidget->SetInfo(LableText, ValueText, bIsUP);
			List_Stat->AddChild(CreatedWidget);
		}
	}
}

void UB2UIUpgradeStatNotify::ClearList()
{
	if (List_Stat.IsValid())
		List_Stat->ClearChildren();

	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(DelayViewTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(DelayViewTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DelayViewTimerHandle, this, &UB2UIUpgradeStatNotify::OnDelayViewTimerHandle, fDelayTime, false);
	}
}

void UB2UIUpgradeStatNotify::OnDelayViewTimerHandle()
{
	// 배틀스코어와 함께 출력되는 팝업의 경우 바로끄기 작동하지않고 리턴. 
	UB2UIBattleScoreNotify* BattleScoreNotifyUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleScoreNotify>(UIFName::BattleScoreNotify);
	if (BattleScoreNotifyUI && BattleScoreNotifyUI->IsVisible()) return;
	
	UB2UIManager::GetInstance()->CloseUI(UIFName::UpgradeStatNotify);
}

void UB2UIUpgradeStatNotify::SetTitleText(FText _Text)
{
	if (STB_Title.IsValid())
		STB_Title->SetText(_Text);
}