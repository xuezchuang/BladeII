// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEventPointConditionPart.h"

#include "BladeIIUtil.h"

void UB2UIEventPointConditionPart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_AccruePointConditon);
	GET_SLOT(UB2RichTextBlock, RTB_AccruePointCount);
}

void UB2UIEventPointConditionPart::UpdateStaticText()
{
	if (RTB_AccruePointConditon.IsValid())
	{
		//FString DescriptionFormat = ;
		FString DescriptionKey = FString::Printf(TEXT("PointEvent%d"), CahcedEventConditionInfo.event_type);
		RTB_AccruePointConditon->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, DescriptionKey));
	}

	if (RTB_AccruePointCount.IsValid())
		RTB_AccruePointCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("AcquirePoint")), FText::AsNumber(CahcedEventConditionInfo.point)));
}

void UB2UIEventPointConditionPart::InitEventPointConditionPart(const b2network::B2mdPointEvent& EventConditionInfo)
{
	CahcedEventConditionInfo = EventConditionInfo;

	Super::Init();
}