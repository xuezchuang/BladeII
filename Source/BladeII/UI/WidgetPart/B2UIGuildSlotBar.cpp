#include "B2UIGuildSlotBar.h"
#include "B2UIGuildInfoPart.h"
#include "Retailer.h"
#include "BladeIILocText.h"


void UB2UIGuildInfoSlot::CacheAssets()
{
	GET_SLOT(UB2UIGuildInfoPart1, UIP_GuildInfoPart1)
	if (UIP_GuildInfoPart1.IsValid())
		UIP_GuildInfoPart1->Init();
	GET_SLOT(UB2UIGuildInfoPart2, UIP_GuildInfoPart2)
	if (UIP_GuildInfoPart2.IsValid())
		UIP_GuildInfoPart2->Init();
	GET_SLOT(UB2UIGuildInfoPart3, UIP_GuildInfoPart3)
	if (UIP_GuildInfoPart3.IsValid())
		UIP_GuildInfoPart3->Init();

	GET_SLOT(UB2Button, BTN_Join)
	GET_SLOT(UB2Button, BTN_Deny)
	GET_SLOT(UB2RichTextBlock,	RTB_JoinText)
	GET_SLOT(UB2RichTextBlock,	RTB_JoinTime)
	GET_SLOT(UTextBlock,		TB_JoinTime)

	GET_SLOT(UB2RichTextBlock, RTB_GuildRank)

	GET_SLOT(UOverlay, OV_Rank1)
	GET_SLOT(UOverlay, OV_Rank2)
	GET_SLOT(UOverlay, OV_Rank3)

	GET_SLOT(UTextBlock, TB_Reject)
	GET_SLOT(UTextBlock, TB_Join)
}
void UB2UIGuildInfoSlot::UpdateStaticText()
{
	if (TB_Reject.IsValid())
		TB_Reject->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_InviteCancel")));

	if (TB_Join.IsValid())
		TB_Join->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Join")));
}




void UB2UIGuildInfoSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Join, &UB2UIGuildInfoSlot::OnClickGuildDetail)
	BIND_CLICK_FUNC_TO_BTN(BTN_Deny, &UB2UIGuildInfoSlot::OnClickGuildDeny)
}

void UB2UIGuildInfoSlot::SetGuildInfo(FB2ResponseGuildInfoPtr GuildInfo)
{

	UIP_GuildInfoPart1->SetGuildMark(GuildInfo->mark_index, GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color);
	UIP_GuildInfoPart1->SetGuildName(FText::FromString(GuildInfo->guild_name));
	UIP_GuildInfoPart1->SetGuildMasterName(FText::FromString(GuildInfo->master_name));
	UIP_GuildInfoPart1->SetGuildLv(GuildInfo->level);

	UIP_GuildInfoPart2->SetGuildNumber(GuildInfo->cur_member_count, GuildInfo->max_member_count);
	UIP_GuildInfoPart2->SetCombat(GuildInfo->members_avg_power);				

	UIP_GuildInfoPart3->SetGuildScore(GuildInfo->season_obtain_medal);

	GuildID = GuildInfo->guild_id;

	SetRank(GuildInfo->season_ranking);
}

void UB2UIGuildInfoSlot::SetGuildDelegate(FGuildDetail Guilddelegate)
{
	DelegateGuildDetailInfo = Guilddelegate;
}

void UB2UIGuildInfoSlot::SetGuildDelectDelegate(FGuildSlotDelect GuildDelegate)
{
	DelegateGuildSlotDelect = GuildDelegate;
}

void UB2UIGuildInfoSlot::OnClickGuildDetail()
{
	DelegateGuildDetailInfo.ExecuteIfBound(GuildID);
}

void UB2UIGuildInfoSlot::OnClickGuildDeny()
{
	DeliveryRejectInviteGuildTicket = DeliveryRejectInviteGuildClass<>::GetInstance().Subscribe([this](){

		DelegateGuildSlotDelect.ExecuteIfBound(this);
		DeliveryRejectInviteGuildClass<>::GetInstance().Unsubscribe(DeliveryRejectInviteGuildTicket);
	});

	data_trader::Retailer::GetInstance().ReqeustRejectInviteGuild(GuildID);
}

void UB2UIGuildInfoSlot::SetGuildRankingInfo(b2network::B2GuildRankingPtr RankingInfo)
{
	UIP_GuildInfoPart1->SetGuildMark(RankingInfo->mark, RankingInfo->mark_color, RankingInfo->mark_bg, RankingInfo->mark_bg_color);
	UIP_GuildInfoPart1->SetGuildName(FText::FromString(RankingInfo->guild_name));
	UIP_GuildInfoPart1->SetGuildMasterName(FText::FromString(RankingInfo->gm_nickname));
	UIP_GuildInfoPart1->SetGuildLv(RankingInfo->level);

	UIP_GuildInfoPart2->SetGuildNumber(RankingInfo->cur_member_count, RankingInfo->max_member_count);
	UIP_GuildInfoPart2->SetCombat(RankingInfo->members_avg_power);	

	UIP_GuildInfoPart3->SetGuildScore(RankingInfo->total_obtain_medal);

	SetRank(RankingInfo->ranking);

	GuildID = RankingInfo->guild_id;
}

void UB2UIGuildInfoSlot::SetRank(int32 Rank)
{
	if (OV_Rank1.IsValid())
		OV_Rank1->SetVisibility(Rank == 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (OV_Rank2.IsValid())
		OV_Rank2->SetVisibility(Rank == 2 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (OV_Rank3.IsValid())
		OV_Rank3->SetVisibility(Rank == 3 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (RTB_GuildRank.IsValid())
	{
		RTB_GuildRank->SetVisibility(ESlateVisibility::Visible);

		if (RTB_GuildRank.IsValid() && Rank >= 4)
		{
			RTB_GuildRank->SetText(FText::AsNumber(Rank));
			
		}
		else if (Rank == 0)
		{
			RTB_GuildRank->SetText(FText::FromString(FString::Printf(TEXT("-"))));
		}
		else 
			RTB_GuildRank->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIGuildInfoSlot::SetRemainTime(int64 InTime)
{
	if (!TB_JoinTime.IsValid())
		return;

	FTimespan RemainTime = FDateTime::FromUnixTimestamp(InTime / 1000) - FDateTime::UtcNow();

	FText FirstText;
	FText SecondText;

	if (RemainTime.GetDays())
	{
		FirstText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), RemainTime.GetDays());
	}
	if (RemainTime.GetHours())
	{
		
		FText& SelectText = RemainTime.GetDays() ? SecondText : FirstText; 
		SelectText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), RemainTime.GetHours());
	}
	if (RemainTime.GetMinutes())
	{
		if (!RemainTime.GetDays() || !RemainTime.GetHours())
		{
			SecondText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), RemainTime.GetMinutes());
		}
	}
	
	/*FText Hour = RemainTime.GetHours() ? FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), RemainTime.GetHours()) : FText::FromString(TEXT(""));
	FText Min = RemainTime.GetMinutes() ? FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), RemainTime.GetMinutes()) : FText::FromString(TEXT(""));

	FirstText = RemainTime.GetDays() ? Day : Hour;
	SecondText = RemainTime.GetDays() ? (RemainTime.GetHours() ? Hour : Min) : Min ? ;*/

	TB_JoinTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Invite_Time")), FirstText, SecondText));
}
