// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIFairyLevelUpStatRow.h"


void UB2UIFairyLevelUpStatRow::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_ConditionLevel);
	GET_SLOT(UB2RichTextBlock, RTB_PreviewStat);
	GET_SLOT(UB2RichTextBlock, RTB_PreviewStatValue);
}

void UB2UIFairyLevelUpStatRow::UpdateStaticText()
{

}

void UB2UIFairyLevelUpStatRow::BindDelegates()
{

}

void UB2UIFairyLevelUpStatRow::SetContent(FText ConditionLevel, FText PreviewStat, FText PreviewStatValue)
{
	if (TB_ConditionLevel.IsValid())
		TB_ConditionLevel->SetText(ConditionLevel);
	if(RTB_PreviewStat.IsValid())
		RTB_PreviewStat->SetText(PreviewStat);
	if (RTB_PreviewStatValue.IsValid())
		RTB_PreviewStatValue->SetText(PreviewStatValue);
}
