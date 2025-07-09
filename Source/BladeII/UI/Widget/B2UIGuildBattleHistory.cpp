// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildBattleHistory.h"
#include "B2UIGuildBattleHistorySlot.h"


#include "BladeIIGameImpl.h"

void UB2UIGuildBattleHistory::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2UIGuildBattleHistoryRecord, UIP_GuildWarSeasonRecord)
	if (UIP_GuildWarSeasonRecord.IsValid())
		UIP_GuildWarSeasonRecord->Init();

	GET_SLOT(UVerticalBox, VB_List)
}

void UB2UIGuildBattleHistory::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildBattleHistory::OnClickBTN_Close);
}

void UB2UIGuildBattleHistory::Init()
{
	Super::Init();
}

void UB2UIGuildBattleHistory::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIGuildBattleHistory::OpenSetting(const FB2ResponseGuildBattleHistoryPtr& HistoryData)
{
	b2network::B2GuildBattleRecordPtr Season_Record = HistoryData->season_record;

	if (UIP_GuildWarSeasonRecord.IsValid())
		UIP_GuildWarSeasonRecord->SetRecord(Season_Record->win_count, Season_Record->lose_count, Season_Record->draw_count, Season_Record->ranking, Season_Record->ranking_total_count, Season_Record->total_obtain_medal);
	

	VB_List->ClearChildren();

	for (auto El : HistoryData->battle_histories)
	{
		auto WidgetSlot = CreateWidget<UB2UIGuildBattleHistorySlot>(GetWorld(), GuildRankSlot);

		check(WidgetSlot)
		VB_List->AddChild(WidgetSlot);
		WidgetSlot->Init();
		auto MyGuild = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo();
		WidgetSlot->SetOtherGuild(El->target_mark, El->target_mark_color, El->target_mark_bg, El->target_mark_bg_color, El->target_guild_name, El->target_obtain_medal, El->target_medal_total_count);
		WidgetSlot->SetMyGuild(MyGuild.GuildInfo->mark_index, MyGuild.GuildInfo->mark_color, MyGuild.GuildInfo->mark_bg_index, MyGuild.GuildInfo->mark_bg_color, MyGuild.GuildInfo->guild_name, El->obtain_medal, El->medal_total_count, El->result);
		WidgetSlot->SetTime(El->battle_end_time);
		WidgetSlot->SetBattleLooking(El->medal_total_count, El->target_medal_total_count);

	}

}

void UB2UIGuildBattleHistory::OnClickBTN_Close()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildBattleHistory);
}


