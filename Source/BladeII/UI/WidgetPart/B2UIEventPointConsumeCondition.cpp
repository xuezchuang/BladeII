// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEventPointConsumeCondition.h"

#include "BladeIIUtil.h"

void UB2UIEventPointConsumeCondition::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_AcquirePointConditon);
	GET_SLOT(UB2RichTextBlock, RTB_AcquirePointCount);
}

void UB2UIEventPointConsumeCondition::UpdateStaticText()
{
	if (RTB_AcquirePointConditon.IsValid())
	{
		//FString DescriptionFormat = );
		FString DescriptionKey = FString::Printf(TEXT("PointEvent%d"), CahcedEventConditionInfo.event_type);
		RTB_AcquirePointConditon->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, DescriptionKey));
	}

	if (RTB_AcquirePointCount.IsValid())
		RTB_AcquirePointCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemCount")), FText::AsNumber(CahcedEventConditionInfo.point)));
}


void UB2UIEventPointConsumeCondition::InitEventPointConsimeCondition(const b2network::B2mdPointEvent& EventConditionInfo)
{
	CahcedEventConditionInfo = EventConditionInfo;

	Super::Init();
}