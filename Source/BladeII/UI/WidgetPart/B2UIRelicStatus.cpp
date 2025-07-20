// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRelicStatus.h"


void UB2UIRelicStatus::CacheAssets()
{
	GET_SLOT(UTextBlock,			TB_Status);
	GET_SLOT(UTextBlock,			TB_CurLevel);
	GET_SLOT(UTextBlock,			TB_MaxLevel);
	GET_SLOT(UTextBlock,			TB_AddValue);
	GET_SLOT(UProgressBar,			PB_Status_Relic);
}

void UB2UIRelicStatus::SetRelicStatus(int32 cur, int32 max, int32 add)
{
	if (TB_CurLevel.IsValid())
	{
		TB_CurLevel->SetText(FText::AsNumber(cur+ add));
	}
	if (TB_MaxLevel.IsValid())
	{
		TB_MaxLevel->SetText(FText::AsNumber(max));
	}
	if (TB_AddValue.IsValid())
	{
		if (add == 0)
			TB_AddValue->SetVisibility(ESlateVisibility::Hidden);
		else
			TB_AddValue->SetVisibility(ESlateVisibility::Visible);
		TB_AddValue->SetText(FText::FromString(FString::Printf(TEXT("%+d"), add)));
	}
	if (PB_Status_Relic.IsValid())
	{
		if (max <= 0.0f)
			max = 1.0f;
		PB_Status_Relic->SetPercent((float)(cur+add) / float(max));
	}
}
