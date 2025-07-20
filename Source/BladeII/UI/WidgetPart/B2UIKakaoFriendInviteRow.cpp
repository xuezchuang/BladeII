// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIKakaoFriendInviteRow.h"
#include "B2UIFriendKakaoInvite.h"

#include "BladeIIGameImpl.h"
#include "B2UIManager.h"
#include "B2PlatformAPI.h"

UB2UIKakaoFriendInviteRow::UB2UIKakaoFriendInviteRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RowIndex = -1;
	OwnerUI = nullptr;
}
void UB2UIKakaoFriendInviteRow::Init()
{
	Super::Init();
}


void UB2UIKakaoFriendInviteRow::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Character);
	GET_SLOT(UB2Button, BTN_WholeArea);
	GET_SLOT(UB2Button, BTN_KakaoInvite);
	GET_SLOT(UTextBlock, TB_KakaoInvite);
	GET_SLOT(UTextBlock, TB_PlayerName);
}

void UB2UIKakaoFriendInviteRow::UpdateStaticText()
{
	if (TB_KakaoInvite.IsValid())
		TB_KakaoInvite->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_KakaoFriendInvite_InviteBtn")));
}

void UB2UIKakaoFriendInviteRow::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_KakaoInvite, &UB2UIKakaoFriendInviteRow::OnInvite);
}

void UB2UIKakaoFriendInviteRow::SetUserInfo(const FB2KakaoFriendInfo &UserInfo)
{
	if (TB_PlayerName.IsValid())
		TB_PlayerName->SetText(FText::FromString(UserInfo.sKakaoNickName));

	SetKakaoInviteBtn(!UserInfo.bInvited);
}

void UB2UIKakaoFriendInviteRow::SetUserProfileImage(class UTexture2DDynamic *Tex)
{
	if (IMG_Character.IsValid())
	{
		IMG_Character->SetBrushFromTextureDynamic(Tex, true);
		IMG_Character->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UB2UIKakaoFriendInviteRow::OnInvite()
{
	if (OwnerUI)
	{
		OwnerUI->OnKakaoFriendInvite(RowIndex);
	}
}

void UB2UIKakaoFriendInviteRow::SetOwnerUI(UB2UIFriendKakaoInvite *InOwner)
{
	OwnerUI = InOwner;
}

void UB2UIKakaoFriendInviteRow::SetKakaoInviteBtn(bool bEnabled)
{
	if (BTN_KakaoInvite.IsValid())
	{
		BTN_KakaoInvite->SetIsEnabled(bEnabled);
	}
}