// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIBlockUser.h"
#include "B2UIManager.h"

#include "BladeIIGameImpl.h"


void UB2UIBlockUser::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_UnBlock);
	GET_SLOT(UTextBlock, TB_UnBlock);
	GET_SLOT(UTextBlock, TB_UserID);
}

void UB2UIBlockUser::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_UnBlock, &UB2UIBlockUser::OnClickUnBlock);
}

void UB2UIBlockUser::Init()
{
	Super::Init();
}

void UB2UIBlockUser::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIBlockUser::UpdateStaticText()
{
	if (TB_UserID.IsValid())
	{
		TB_UserID->SetText(FText::FromString(UserNickName));
	}

	if (TB_UnBlock.IsValid())
	{
		//TB_UnBlock->SetText(FText::FromString(UserNickName));
	}
}

void UB2UIBlockUser::SetInfo(const FString& InUserNickName, int64 InUserAccountId)
{
	UserNickName = InUserNickName;
	UserAccountId = InUserAccountId;

	UpdateStaticText();
}

void UB2UIBlockUser::OnClickUnBlock()
{
	BladeIIGameImpl::GetChatStore().RequestUnBlockUser(UserAccountId);
}


