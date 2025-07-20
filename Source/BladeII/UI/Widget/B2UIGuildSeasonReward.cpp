// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildSeasonReward.h"
#include "B2UIManager.h"
#include "B2UIGuildMark.h"
#include "B2UIGuildInfoPart.h"



#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildSeasonReward::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIGuildMark, UIP_GuildMark);
	if(UIP_GuildMark.IsValid()) UIP_GuildMark->Init();
	GET_SLOT(UB2RichTextBlock, TB_GuildName);
	GET_SLOT(UB2RichTextBlock, TB_GuildScore);
	GET_SLOT(UB2RichTextBlock, TB_Rank);

	GET_SLOT(UB2Button, BTN_Confirm);

	GET_SLOT(UVerticalBox, VB_LeftList);
	GET_SLOT(UVerticalBox, VB_RightList);

}

void UB2UIGuildSeasonReward::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIGuildSeasonReward::OnClickBTN_Confirm);
}

void UB2UIGuildSeasonReward::Init()
{
	Super::Init();
}

void UB2UIGuildSeasonReward::DestroySelf(UB2UIManager* InUIManager)
{

	if (UIP_GuildMark.IsValid()) UIP_GuildMark->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildSeasonReward::OnOpen(bool RightNow /*= false*/)
{
	SetVisibility(ESlateVisibility::Visible);
	SetGuildInfo();
}

void UB2UIGuildSeasonReward::OnClickBTN_Confirm()
{
	SetVisibility(ESlateVisibility::Hidden);
}


void UB2UIGuildSeasonReward::SetGuildInfo()
{
	auto GuildInfo = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo;


	UIP_GuildMark->SetGuildMark(GuildInfo->mark_index, GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color);
	TB_GuildName->SetText(FText::FromString(GuildInfo->guild_name));
	TB_GuildScore->SetText(FText::AsNumber(GuildInfo->season_obtain_medal));

	float PerCent = 0;
	if (!GuildInfo->season_ranking || !GuildInfo->season_ranking_total)
		PerCent = 100.f;
	else
		PerCent = float(GuildInfo->season_ranking) / float(GuildInfo->season_ranking_total);


	TB_Rank->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildRankNumber")), FText::AsNumber(GuildInfo->season_ranking),
		FText::FromString(FString::Printf(TEXT("%.1f"), PerCent))));


}

void UB2UIGuildSeasonReward::SetGuildRewardInfo(int32 Cost)
{
	TArray<b2network::B2mdGuildBattleSeasonRewardPtr> RewardInfo = BladeIIGameImpl::GetClientDataStore().GetGuildSeasonRewardInfo();

	RewardInfo.Sort([](b2network::B2mdGuildBattleSeasonRewardPtr Ele1, b2network::B2mdGuildBattleSeasonRewardPtr Ele2)->bool{
		if (Ele1->order < Ele2->order) return true; else return false;
	});

	VB_LeftList->ClearChildren();
	VB_RightList->ClearChildren();

	FText RankText;

	for (auto& El : RewardInfo)
	{
		auto RewardSlotWidget = CreateWidget<UB2UIGuildSeasonRewardSlot>(GetWorld(), RewardSlot);

		check(RewardSlotWidget)

		if (El->ranking_max)
		{
			VB_LeftList->AddChild(RewardSlotWidget);
			RewardSlotWidget->Init();
			if (El->ranking_max == El->ranking_min)
				RankText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Rank")), FText::AsNumber(El->ranking_max));
			else
				RankText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Rank2")), FText::AsNumber(El->ranking_min), FText::AsNumber(El->ranking_max));
		}
		else
		{
			VB_RightList->AddChild(RewardSlotWidget);
			RewardSlotWidget->Init();
			if (El->ranking_rate_max == 100)
				RankText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_RankPercent2")), FText::AsNumber(El->ranking_rate_min));
			else
				RankText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_RankPercent")), FText::AsNumber(El->ranking_rate_max));
		}

		//RewardSlotWidget->SetIncludeRank(El->rewards[0]->count == Cost ? true : false);

		RewardSlotWidget->SetRewardSlot(RankText, El->rewards[0]->raw_reward->id, El->rewards[0]->raw_reward->count);
	}
}


