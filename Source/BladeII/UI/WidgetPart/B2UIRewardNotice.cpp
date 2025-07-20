// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIRewardNotice.h"
#include "B2UIManager.h"


void UB2UIRewardNotice::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Main);
}

void UB2UIRewardNotice::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIRewardNotice::Init()
{
	Super::Init();
}

void UB2UIRewardNotice::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIRewardNotice::SetTextAndStartAnimation(const FString& TextString, float DelayTime)
{	
	if (TB_Main.IsValid() == false)
		return;
	
	TB_Main->SetVisibility(ESlateVisibility::Collapsed);
	TB_Main->SetText(FText::FromString(TextString));

	StartAnimation_BP(DelayTime);
}

