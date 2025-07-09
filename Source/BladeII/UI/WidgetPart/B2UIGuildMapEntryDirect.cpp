// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildMark.h"
#include "B2UIGuildMapEntryDirect.h"
#include "B2UIGuildInfoPart.h"
#include "B2UIGuildMapMain.h"


void UB2UIGuildMapEntryDirect::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIGuildMark, UIP_MyGuildMark);
	if(UIP_MyGuildMark.IsValid()) UIP_MyGuildMark->Init();
	GET_SLOT(UB2RichTextBlock, TB_MyGuildName);
	GET_SLOT(UTextBlock, TB_MyGuildLv);
	GET_SLOT(UTextBlock, TB_MyGuildMember);

	GET_SLOT(UB2UIGuildMark, UIP_OtherGuildMark);
	if(UIP_OtherGuildMark.IsValid()) UIP_OtherGuildMark->Init();
	GET_SLOT(UB2RichTextBlock, TB_OtherGuildName);
	GET_SLOT(UTextBlock, TB_OtherGuildLv);
	GET_SLOT(UTextBlock, TB_OtherGuildMember);


	GET_SLOT(UPanelWidget, O_MyDirect);
	GET_SLOT(UPanelWidget, O_OtherDirect);


	GET_SLOT(UB2UIGuildInfoPart4, UIP_LeftGuildInfo);
	if (UIP_LeftGuildInfo.IsValid()) UIP_LeftGuildInfo->Init();
	GET_SLOT(UB2UIGuildInfoPart4, UIP_RightGuildInfo);
	if (UIP_RightGuildInfo.IsValid()) UIP_RightGuildInfo->Init();
}

void UB2UIGuildMapEntryDirect::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIGuildMapEntryDirect::Init()
{
	Super::Init();
}

void UB2UIGuildMapEntryDirect::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_MyGuildMark.IsValid()) UIP_MyGuildMark->DestroySelf(InUIManager);
	if(UIP_OtherGuildMark.IsValid()) UIP_OtherGuildMark->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildMapEntryDirect::SetMyGuildInfo(b2network::B2GuildBattleGuildPtr InGuildInfo)
{
	SetMyGuildMark(InGuildInfo->mark_index, InGuildInfo->mark_color, InGuildInfo->mark_bg_index, InGuildInfo->mark_bg_color);
	SetMyGuildName(FText::FromString(InGuildInfo->guild_name));
	SetMyGuildMemeber(InGuildInfo->members.Num());
	SetMyGuildLv(InGuildInfo->guild_level);

	UIP_LeftGuildInfo->SetGuildMark(InGuildInfo->mark_index, InGuildInfo->mark_color, InGuildInfo->mark_bg_index, InGuildInfo->mark_bg_color);
	UIP_LeftGuildInfo->SetGuildInfo(FText::FromString(InGuildInfo->guild_name), InGuildInfo->attack_count, InGuildInfo->members.Num() * UB2UIGuildMapMain::MaxAttackCount, InGuildInfo->obtain_medal, InGuildInfo->members.Num() * UB2UIGuildMapMain::MaxMedal);
}

void UB2UIGuildMapEntryDirect::SetOtherGuildInfo(b2network::B2GuildBattleGuildPtr InGuildInfo)
{
	SetOtherGuildMark(InGuildInfo->mark_index, InGuildInfo->mark_color, InGuildInfo->mark_bg_index, InGuildInfo->mark_bg_color);
	SetOtherGuildName(FText::FromString(InGuildInfo->guild_name));
	SetOtherGuildMemeber(InGuildInfo->members.Num());
	SetOtherGuildLv(InGuildInfo->guild_level);

	UIP_RightGuildInfo->SetGuildMark(InGuildInfo->mark_index, InGuildInfo->mark_color, InGuildInfo->mark_bg_index, InGuildInfo->mark_bg_color);
	UIP_RightGuildInfo->SetGuildInfo(FText::FromString(InGuildInfo->guild_name), InGuildInfo->attack_count, InGuildInfo->members.Num() * UB2UIGuildMapMain::MaxAttackCount, InGuildInfo->obtain_medal, InGuildInfo->members.Num() * UB2UIGuildMapMain::MaxMedal);
}

void UB2UIGuildMapEntryDirect::SetMyGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	if (UIP_MyGuildMark.IsValid())
	{
		UIP_MyGuildMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);
	}
}

void UB2UIGuildMapEntryDirect::SetMyGuildName(FText NameText)
{
	if (TB_MyGuildName.IsValid())
	{
		TB_MyGuildName->SetText(NameText);
	}
}



void UB2UIGuildMapEntryDirect::SetMyGuildMemeber(int32 GuildMember)
{
	if (TB_MyGuildMember.IsValid())
		TB_MyGuildMember->SetText(FText::AsNumber(GuildMember));
}

void UB2UIGuildMapEntryDirect::SetMyGuildLv(int32 Lv)
{
	if (TB_MyGuildLv.IsValid())
		TB_MyGuildLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")),FText::AsNumber(Lv)));
}

void UB2UIGuildMapEntryDirect::SetOtherGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	if (UIP_OtherGuildMark.IsValid())
	{
		UIP_OtherGuildMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);
	}
}

void UB2UIGuildMapEntryDirect::SetOtherGuildName(FText OtherNameText)
{
	if (TB_OtherGuildName.IsValid())
	{
		TB_OtherGuildName->SetText(OtherNameText);
	}
}

void UB2UIGuildMapEntryDirect::SetOtherGuildMemeber(int32 GuildMember)
{
	if (TB_OtherGuildMember.IsValid())
		TB_OtherGuildMember->SetText(FText::AsNumber(GuildMember));
}

void UB2UIGuildMapEntryDirect::SetOtherGuildLv(int32 Lv)
{
	if (TB_OtherGuildLv.IsValid())
		TB_OtherGuildLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(Lv)));
}

