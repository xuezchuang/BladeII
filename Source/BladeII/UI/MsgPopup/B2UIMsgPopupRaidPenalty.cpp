// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupRaidPenalty.h"

#include "B2GameInstance.h"

UB2UIMsgPopupRaidPenalty::UB2UIMsgPopupRaidPenalty(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIMsgPopupRaidPenalty::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIMsgPopupRaidPenalty::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIMsgPopupRaidPenalty::Init()
{
	Super::Init();

	UB2GameInstance* B2GameInstance = GetWorld()->GetGameInstance<UB2GameInstance>();
	B2GameInstance->ResumeDelegate.AddUniqueDynamic(this, &UB2UIMsgPopupRaidPenalty::OnPlatformResume);
}

void UB2UIMsgPopupRaidPenalty::DestroySelf(UB2UIManager* InUIManager)
{
	UB2GameInstance* B2GameInstance = GetWorld()->GetGameInstance<UB2GameInstance>();
	B2GameInstance->ResumeDelegate.RemoveDynamic(this, &UB2UIMsgPopupRaidPenalty::OnPlatformResume);
	
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
		TheWorld->GetTimerManager().ClearTimer(LeftTimeHandler);
	
	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupRaidPenalty::OnPlatformResume()
{
	UB2GameInstance* B2GameInstance = GetWorld()->GetGameInstance<UB2GameInstance>();
	int32 nSecond = B2GameInstance->GetPauseResumeIntervalTime();
	StartTimer(nCurrentLeftTime - nSecond);
}

void UB2UIMsgPopupRaidPenalty::SetRemainTimeText(int32 nSecond)
{
	FTimespan testspan = FTimespan::FromSeconds(nSecond);
	int32 Min = testspan.GetMinutes();
	int32 Sec = testspan.GetSeconds();
	FString Msg = TEXT("");
	
	Msg += BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_RemainTime")).ToString();

	if (Min > 0)
		Msg += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::AsNumber(Min)).ToString();

	Msg += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), FText::AsNumber(Sec)).ToString();
	
	SetSubContent(FText::FromString(Msg));
}

void UB2UIMsgPopupRaidPenalty::StartTimer(int32 nSecond)
{
	nCurrentLeftTime = nSecond;
	SetRemainTimeText(nCurrentLeftTime);

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(LeftTimeHandler);
		TheWorld->GetTimerManager().SetTimer(LeftTimeHandler, this, &UB2UIMsgPopupRaidPenalty::CheckLimittime, 1.0f, true);
	}
}

void UB2UIMsgPopupRaidPenalty::CheckLimittime()
{
	nCurrentLeftTime -= 1;

	if (nCurrentLeftTime < 1)
	{
		this->CloseSelf();
	}
	else
	{
		SetRemainTimeText(nCurrentLeftTime);
	}
}