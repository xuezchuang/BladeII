// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlRecordItem.h"




void UB2UIControlRecordItem::SetTitle(const FText& Text)
{
	if (TB_Title.IsValid())
		TB_Title->SetText(Text);
}

void UB2UIControlRecordItem::SetAmount(const FText& Text)
{
	if (TB_Amount.IsValid())
		TB_Amount->SetText(Text);
}

void UB2UIControlRecordItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Amount);
}
