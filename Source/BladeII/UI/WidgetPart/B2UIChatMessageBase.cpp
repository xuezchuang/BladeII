// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIChatMessageBase.h"
#include "BladeIILocText.h"

void UB2UIChatMessageBase::SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo)
{
}

void UB2UIChatMessageBase::CacheAssets()
{

}

void UB2UIChatMessageBase::UpdateStaticText()
{

}

void UB2UIChatMessageBase::BindDelegates()
{

}

void UB2UIChatMessageBase::UpdateMessageTime()
{

}

FText UB2UIChatMessageBase::GetElapsedTime(const int32 ElapsedSec)
{
	if (ElapsedSec < 60)
	{ // 정확한 시간 표시 없이 "방금" 정도로 표시
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_ElapsedTimeInfo_Short"));
	}
	else
	{
		return FText::Format(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_ElapsedTimeInfo_Minutes")), FText::AsNumber(ElapsedSec / 60)
		);
	}
}


FText UB2UIChatMessageBase::GetElapsedTime(const FTimespan RemainTime)
{

	FString StrDay, StrHour, StrMin;

	int32 RemainDay = RemainTime.GetDays();
	int32 RemainHour = RemainTime.GetHours();
	int32 RemainMin = RemainTime.GetMinutes();

	if (!RemainDay && !RemainHour && !RemainMin)
		RemainMin = 1;

	if (RemainMin == 1 && RemainHour == 0 && RemainDay == 0)
	{
		// 정확한 시간 표시 없이 "방금" 정도로 표시
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_ElapsedTimeInfo_Short"));
	}

	if (RemainDay != 0)
		StrDay = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), FText::FromString(FString::FromInt(RemainDay))).ToString();

	if (RemainHour != 0)
		StrHour = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), FText::FromString(FString::FromInt(RemainHour))).ToString();

	if (RemainMin != 0)
		StrMin = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::FromString(FString::FromInt(RemainMin))).ToString();

	FString strTime;

	if (RemainDay == 0)
	{
		if (RemainHour == 0)
			strTime = FString::Printf(TEXT("%s"), *StrMin);
		else
			strTime = FString::Printf(TEXT("%s %s"), *StrHour, *StrMin);
	}
	else
	{
		if (RemainHour == 0)
			strTime = FString::Printf(TEXT("%s"), *StrDay);
		else
			strTime = FString::Printf(TEXT("%s %s"), *StrDay, *StrHour);
	}

	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_ContactTime")), FText::FromString(strTime));

}