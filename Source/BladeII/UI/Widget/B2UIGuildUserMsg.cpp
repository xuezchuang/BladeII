// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildUserMsg.h"
#include "B2UIManager.h"
#include "B2UIGuildMemberSlot.h"



void UB2UIGuildUserMsg::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2RichTextBlock, TB_Content);
	GET_SLOT(UImage, BottomBGImage);
	GET_SLOT(UImage, TitleBGImage);
	GET_SLOT(UTextBlock, TextBlock_2332);
	GET_SLOT(UImage, IMG_Content);
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_Cancle);
	GET_SLOT(UTextBlock, TB_BtnPositive);
	GET_SLOT(UB2UIGuildMemberSlot, UIP_GuildMemberSlot3);
	if(UIP_GuildMemberSlot3.IsValid()) UIP_GuildMemberSlot3->Init();

}

void UB2UIGuildUserMsg::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIGuildUserMsg::OnClickBTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancle, &UB2UIGuildUserMsg::OnClickBTN_Cancle);
}

void UB2UIGuildUserMsg::Init()
{
	Super::Init();
	SetOpenState(EUIOpenState::Opened);
}

void UB2UIGuildUserMsg::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_GuildMemberSlot3.IsValid()) UIP_GuildMemberSlot3->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildUserMsg::SetGuildUserMsg(b2network::B2GuildMemberPtr GuildMemberInfo, FText ContentText, FGuildUserPopup Delegate)
{
	UIP_GuildMemberSlot3->SetGuildMember(GuildMemberInfo, 0);
	TB_Content->SetText(ContentText);
	PopupDelegate = Delegate;
}

void UB2UIGuildUserMsg::OnClickBTN_Confirm()
{
	PopupDelegate.ExecuteIfBound();
	UB2UIManager::GetInstance()->CloseUI(UIFName::UserGuildMsg);
}

void UB2UIGuildUserMsg::OnClickBTN_Cancle()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::UserGuildMsg);
}


