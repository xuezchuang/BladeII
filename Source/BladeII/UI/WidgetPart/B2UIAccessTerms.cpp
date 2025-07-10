// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIAccessTerms.h"

#include "Misc/DateTime.h"

#include "B2DLCFrontGameMode.h"
#include "BladeIIUtil.h"


void UB2UIAccessTerms::CacheAssets()
{
	Super::CacheAssets();
	//GET_SLOT(UB2Image, B2Image_0);
}

void UB2UIAccessTerms::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIAccessTerms::Init()
{
	Super::Init();
}

void UB2UIAccessTerms::GotoTitleScreen()
{
	auto* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->GmaeModeGotoTitle();
	}

	// savedata
	SaveGameSetting_AccessTerm(true);
	DestroySelf();
}

FString UB2UIAccessTerms::SetDateString(int32 nYear, int32 nMonth, int32 nDay)
{
	FString sDate;

	int32 year;
	int32 month;
	int32 Dayyy;
	FDateTime thistime = FDateTime::Now();
	int32 thisyo = thistime.GetDay();
	thistime.GetDate(year, month, Dayyy);
	sDate = FString::Printf(TEXT("- %d/%d/%d -"), year, month, Dayyy);
	return sDate;
}





