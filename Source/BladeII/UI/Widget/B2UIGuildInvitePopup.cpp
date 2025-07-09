// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildInvitePopup.h"
#include "B2UIGuildInfoPart.h"
#include "Retailer.h"


void UB2UIGuildInvitePopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Surch);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UEditableTextBox, ETB_SearchGuild);
	GET_SLOT(UVerticalBox, VB_SlotList);
}

void UB2UIGuildInvitePopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Surch, &UB2UIGuildInvitePopup::OnClickBTN_Surch);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildInvitePopup::OnClickClose);
}

void UB2UIGuildInvitePopup::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIGuildInvitePopup::Init()
{
	Super::Init();
}

void UB2UIGuildInvitePopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIGuildInvitePopup::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	DeliveryFindUserTicket = DeliveryFindUserClass<FB2FindUser>::GetInstance().Subscribe([this](const FB2FindUser& FindUser) {
		this->SetFindUser(FindUser);
	});

}


void UB2UIGuildInvitePopup::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);

	DeliveryFindUserClass<FB2FindUser>::GetInstance().Unsubscribe(DeliveryFindUserTicket);
}

void UB2UIGuildInvitePopup::SetInviteList(const FB2ResponseGetGuildMemberCandidatesPtr& MemberCandidtates)
{
	VB_SlotList->ClearChildren();

	for (auto& El : MemberCandidtates->friends)
	{
		auto InviteWidgetSlot = CreateWidget<UB2UIGuildInviteSlot>(GetWorld(), InviteSlot);

		check(InviteWidgetSlot)
		VB_SlotList->AddChild(InviteWidgetSlot);

		InviteWidgetSlot->Init();

		InviteWidgetSlot->SetGuildInfo(El->user->character_summary->account_name , El->user->character_summary->power,
			IntToPCClass(El->user->character_summary->character_type), El->user->character_summary->character_level, b2network::B2GuildMemberRank::NORMAL);
		InviteWidgetSlot->SetGuildState(El->user->guild_name.Len() ? b2network::B2GuildInviteState::JOINED_GUILD : b2network::B2GuildInviteState::INVITABLE, El->user->character_summary->account_id);
	}
}


void UB2UIGuildInvitePopup::SetFindUser(const FB2FindUser& FindUser)
{
	/*VB_SlotList->ClearChildren();

	auto UserInfo = GET_TUPLE_DATA(FB2ResponseFindUser::user_index, FindUser);
	int32 UserState = GET_TUPLE_DATA(FB2ResponseFindUser::guild_invite_state_index, FindUser);

	auto InviteWidgetSlot = CreateWidget<UB2UIGuildInviteSlot>(GetWorld(), InviteSlot);

	check(InviteWidgetSlot)

	VB_SlotList->AddChild(InviteWidgetSlot);
	InviteWidgetSlot->Init();

	InviteWidgetSlot->SetGuildInfo(UserInfo->character_summary->account_name, UserInfo->character_summary->power,
		IntToPCClass(UserInfo->character_summary->character_type), UserInfo->character_summary->character_level, b2network::B2GuildMemberRank::NORMAL);
	InviteWidgetSlot->SetGuildState(UserState, UserInfo->character_summary->account_id);*/

}

void UB2UIGuildInvitePopup::OnClickBTN_Surch()
{
	

	data_trader::Retailer::GetInstance().RequestFindUser(ETB_SearchGuild->GetText().ToString(), true);

}

void UB2UIGuildInvitePopup::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildInvite);
}


