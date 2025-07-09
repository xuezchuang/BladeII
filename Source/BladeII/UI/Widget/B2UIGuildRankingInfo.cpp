// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildRankingInfo.h"
#include "B2UIGuildSlotBar.h"
#include "B2UIGuildInfoPart.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"

void UB2UIGuildRankingInfo::CacheAssets()
{
	Super::CacheAssets();
	

	GET_SLOT(UB2Button, BTN_Close)
	GET_SLOT(UB2Button, BTN_GuildRank)
	GET_SLOT(UB2Button, BTN_RankReward)
	GET_SLOT(UB2Button, BTN_Surch)

	GET_SLOT(UEditableTextBox, ETB_SearchGuild)

	GET_SLOT(UB2UIGuildInfoSlot, UIP_MyGuildInfoSlotRank)
	if(UIP_MyGuildInfoSlotRank.IsValid())
		UIP_MyGuildInfoSlotRank->Init();

	GET_SLOT(UCanvasPanel, CP_GuildRanking)

	GET_SLOT(UVerticalBox, VB_RankingList)
	GET_SLOT(UVerticalBox, VB_LeftReward)
	GET_SLOT(UVerticalBox, VB_RightReward)
}

void UB2UIGuildRankingInfo::BindDelegates()
{
	Super::BindDelegates();
	
	BIND_CLICK_FUNC_TO_BTN(BTN_GuildRank, &UB2UIGuildRankingInfo::OnClickGuildRanking);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildRankingInfo::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_Surch, &UB2UIGuildRankingInfo::OnClickSuarch);

}

void UB2UIGuildRankingInfo::Init()
{
	Super::Init();
}

void UB2UIGuildRankingInfo::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIGuildRankingInfo::SetGuildRanking()
{

}




void UB2UIGuildRankingInfo::SetSuarchList(const FB2ResponseFindGuildPtr& GuildInfo)
{
	SetTapGuildRanking();



	auto GuildRank = CreateWidget<UB2UIGuildInfoSlot>(GetWorld(), GuildRankSlot);

	check(GuildRank)

	VB_RankingList->AddChild(GuildRank);
	GuildRank->Init();
	GuildRank->SetGuildInfo(GuildInfo->guild);
}

void UB2UIGuildRankingInfo::SetTapGuildRanking()
{

	CP_GuildRanking->SetVisibility(ESlateVisibility::Visible);

	if (UIP_MyGuildInfoSlotRank.IsValid())
		UIP_MyGuildInfoSlotRank->SetGuildInfo(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo);

	VB_RankingList->ClearChildren();
}

void UB2UIGuildRankingInfo::SetTapRankingReward()
{


	CP_GuildRanking->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGuildRankingInfo::RankingListReceive(const FB2ResponseGetGuildRankingListPtr& RankingInfo)
{
	RankingArr.Empty();
	RankingArr = RankingInfo->guild_rankings;
}

void UB2UIGuildRankingInfo::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	SetTapGuildRanking();

	DeliverytGetGuildRankingListTicket = DeliverytGetGuildRankingListClass<FB2ResponseGetGuildRankingListPtr>::GetInstance().Subscribe([this](const FB2ResponseGetGuildRankingListPtr& RankingInfo)
	{
		RankingListReceive(RankingInfo);
		OnClickGuildRanking();
	});

	DeliveryFindGuildTicket = DeliveryFindGuildClass<FB2ResponseFindGuildPtr>::GetInstance().Subscribe([this](const FB2ResponseFindGuildPtr& GuildInfo)
	{
		SetSuarchList(GuildInfo);
	});

	data_trader::Retailer::GetInstance().RequestGetGuildRankingList();
}

void UB2UIGuildRankingInfo::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose();
	DeliveryFindGuildClass<FB2ResponseFindGuildPtr>::GetInstance().Unsubscribe(DeliveryFindGuildTicket);
	DeliverytGetGuildRankingListClass<FB2ResponseGetGuildRankingListPtr>::GetInstance().Unsubscribe(DeliverytGetGuildRankingListTicket);
}

void UB2UIGuildRankingInfo::OnClickGuildRanking()
{
	SetTapGuildRanking();
	VB_RankingList->ClearChildren();
	for (auto El : RankingArr)
	{
		auto RankSlot = CreateWidget<UB2UIGuildInfoSlot>(GetWorld(), GuildRankSlot);
		check(RankSlot)

		if(!RankSlot)
			return;

		VB_RankingList->AddChild(RankSlot);
		RankSlot->Init();
		RankSlot->SetGuildRankingInfo(El);
	}

}


void UB2UIGuildRankingInfo::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildRanking);
}

void UB2UIGuildRankingInfo::OnClickSuarch()
{
	if (!ETB_SearchGuild.IsValid())
		return;

	data_trader::Retailer::GetInstance().RequestFindGuild(ETB_SearchGuild->GetText().BuildSourceString());
}

