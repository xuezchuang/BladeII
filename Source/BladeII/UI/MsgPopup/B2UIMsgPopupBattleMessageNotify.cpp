// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupBattleMessageNotify.h"


void UB2UIMsgPopupBattleMessageNotify::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_Title);
}

void UB2UIMsgPopupBattleMessageNotify::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIMsgPopupBattleMessageNotify::Init()
{
	Super::Init();
}

void UB2UIMsgPopupBattleMessageNotify::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupBattleMessageNotify::SetMessage(const FString& msg, const float duration)
{	
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_BATTLEMESSAGE, msg));

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if(duration > 0)
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UB2UIMsgPopupBattleMessageNotify::TimeEnd, duration, false);
}

void UB2UIMsgPopupBattleMessageNotify::TimeEnd()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::BattleMessageNotify);
}