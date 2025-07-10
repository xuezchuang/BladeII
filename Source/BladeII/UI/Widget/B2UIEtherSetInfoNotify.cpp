// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherSetInfoNotify.h"
#include "B2UIManager.h"




void UB2UIEtherSetInfoNotify::SetSetText(FText Text)
{
	if (X_TB_SetInfo.IsValid())
	{
		X_TB_SetInfo->SetText(Text);
	}
}

void UB2UIEtherSetInfoNotify::Init()
{
	PlayTime = 0;
	Super::Init();
}

void UB2UIEtherSetInfoNotify::CacheAssets()
{
	GET_SLOT(UTextBlock, X_TB_SetInfo)
}

void UB2UIEtherSetInfoNotify::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	PlayTime += InDeltaTime;
	if (EndTime < PlayTime)
	{
		//UB2UIManager::GetInstance()->CloseUI(UIFName::EtherInfoNotify);
	}
}
