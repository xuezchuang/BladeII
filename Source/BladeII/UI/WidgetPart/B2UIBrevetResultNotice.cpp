// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIBrevetResultNotice.h"
#include "B2UIManager.h"


void UB2UIBrevetResultNotice::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UTextBlock, TB_SuccessKey);
	GET_SLOT(UTextBlock, TB_SuccessValue);
}

void UB2UIBrevetResultNotice::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIBrevetResultNotice::Init()
{
	Super::Init();
}

void UB2UIBrevetResultNotice::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIBrevetResultNotice::InitResultNotice(EBrevetStatusType StatusType, int32 StatusValue)
{	
	FText KeyText;
	switch (StatusType)
	{
	case EBrevetStatusType::EBST_Atk:
		KeyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack"));
		break;
	case EBrevetStatusType::EBST_Def:
		KeyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense"));
		break;
	case EBrevetStatusType::EBST_Hp:
		KeyText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health"));
		break;
	}

	if (TB_SuccessKey.IsValid())
		TB_SuccessKey->SetText(KeyText);

	if (TB_SuccessValue.IsValid())
		TB_SuccessValue->SetText(FText::FromString(TEXT("+") + FString::FromInt(StatusValue)));
}

void UB2UIBrevetResultNotice::StartNoticeAnimation()
{
	StartNoticeAnimation_BP();
}

void UB2UIBrevetResultNotice::OnFinishAnimationEvent_NoticeAnimation_BP()
{
	FFinishNoticeDelegate.ExecuteIfBound();
}
