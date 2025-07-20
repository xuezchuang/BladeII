// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherInfoNotify.h"
#include "B2UIManager.h"



void UB2UIEtherInfoNotify::Init()
{
	PlayTime = 0;
	Super::Init();
}

void UB2UIEtherInfoNotify::SetInfoNotify(FText InEtherSetText)
{
	if (X_TB_SetInfo.IsValid())
	{
		X_TB_SetInfo->SetText(InEtherSetText);
	}
}

void UB2UIEtherInfoNotify::CacheAssets()
{
	GET_SLOT(UTextBlock, X_TB_SetInfo);
}

void UB2UIEtherInfoNotify::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	PlayTime += InDeltaTime;
	if (EndTime < PlayTime)
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::EtherInfoNotify);
	}
}
