// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRelicCharacterStatus.h"


void UB2UIRelicCharacterStatus::CacheAssets()
{
	GET_SLOT(UImage,			IMG_Status);
	GET_SLOT(UTextBlock,		TB_Status_Hero);
	GET_SLOT(UTextBlock,		TB_Status_Relic);
	GET_SLOT(UTextBlock,		TB_Status_Add);
	GET_SLOT(UProgressBar,		PB_Status_Hero);
	GET_SLOT(UProgressBar,		PB_Status_Relic);
}

void UB2UIRelicCharacterStatus::SetHeroStatus(int32 cur, int32 max)
{
	if (TB_Status_Hero.IsValid())
	{
		TB_Status_Hero->SetText(FText::AsNumber(cur));
	}
	if (PB_Status_Hero.IsValid())
	{
		PB_Status_Hero->SetPercent((float)cur / float(max));
	}
}

void UB2UIRelicCharacterStatus::SetRelicStatus(int32 cur, int32 max, int32 add)
{
	if (TB_Status_Relic.IsValid())
	{
		TB_Status_Relic->SetText(FText::AsNumber(cur + add));
	}
	if (TB_Status_Add.IsValid())
	{
		if (add == 0)
			TB_Status_Add->SetVisibility(ESlateVisibility::Hidden);
		else
			TB_Status_Add->SetVisibility(ESlateVisibility::Visible);
		TB_Status_Add->SetText(FText::FromString(FString::Printf(TEXT("%+d"), add)));
	}
	if (PB_Status_Relic.IsValid())
	{
		PB_Status_Relic->SetPercent((float)(cur + add) / float(max));
	}
}

