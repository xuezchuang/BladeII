// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildMark.h"
#include "B2UIGuildBattleHistorySlot.h"


void UB2UIGuildBattleHistorySlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIGuildMark, UIP_MyMark);
	if(UIP_MyMark.IsValid()) UIP_MyMark->Init();
	GET_SLOT(UB2RichTextBlock, TB_MyGuildName);
	GET_SLOT(UProgressBar, PB_MyMedal);
	GET_SLOT(UTextBlock, TB_MyMedal);
	GET_SLOT(UB2UIGuildMark, UIP_OtherMark);
	if(UIP_OtherMark.IsValid()) UIP_OtherMark->Init();
	GET_SLOT(UB2RichTextBlock, TB_OtherGuildName);
	GET_SLOT(UProgressBar, PB_OtherMedal);
	GET_SLOT(UTextBlock, TB_OtherMedal);
	GET_SLOT(UB2RichTextBlock, TB_Time);
	GET_SLOT(UB2Image, IMG_Win);
	GET_SLOT(UB2Image, IMG_Lose);
	GET_SLOT(UB2Image, IMG_Draw);

	GET_SLOT(UB2RichTextBlock, RTB_BattleResult);

	GET_SLOT(UTextBlock, TB_MyMedalPercent);
	GET_SLOT(UTextBlock, TB_OtherMedalPercent);

	GET_SLOT(UB2RichTextBlock, RTB_BattleLooking);
}

void UB2UIGuildBattleHistorySlot::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIGuildBattleHistorySlot::Init()
{
	Super::Init();
}

void UB2UIGuildBattleHistorySlot::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_MyMark.IsValid()) UIP_MyMark->DestroySelf(InUIManager);
	if(UIP_OtherMark.IsValid()) UIP_OtherMark->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UIGuildBattleHistorySlot::SetMyGuild(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor, FString GuildName, int32 Medal, int32 MaxMedal , int32 Result)
{
	if (UIP_MyMark.IsValid())
		UIP_MyMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);

	if (TB_MyGuildName.IsValid())
		TB_MyGuildName->SetText(FText::FromString(GuildName));

	if (TB_MyMedal.IsValid())
		TB_MyMedal->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildInfoMedalCount")), FText::AsNumber(Medal), FText::AsNumber(MaxMedal)));

	float Percent = float(Medal) / float(MaxMedal);

	if (PB_MyMedal.IsValid())
		PB_MyMedal->SetPercent(Percent);

	if (TB_MyMedalPercent.IsValid())
		TB_MyMedalPercent->SetText(FText::Format(FText::FromString(TEXT("{0}%")), GetAutoFractionalFormattedText(Percent * 100, 2)));

	if (!RTB_BattleResult.IsValid())
		return;

	//승무패 표시해주는 부분
	if (Result == b2network::B2GuildBattleResultType::BGBRT_LOSE)
	{
		IMG_Win->SetVisibility(ESlateVisibility::Hidden);
		IMG_Lose->SetVisibility(ESlateVisibility::Visible);
		IMG_Draw->SetVisibility(ESlateVisibility::Hidden);

		RTB_BattleResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MatchResult_Lose")));
	}
	else if (Result == b2network::B2GuildBattleResultType::BGBRT_WIN)
	{
		IMG_Win->SetVisibility(ESlateVisibility::Visible);
		IMG_Lose->SetVisibility(ESlateVisibility::Hidden);
		IMG_Draw->SetVisibility(ESlateVisibility::Hidden);

		RTB_BattleResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MatchResult_Win")));
	}
	else if (Result == b2network::B2GuildBattleResultType::BGBRT_DRAW)
	{
		IMG_Draw->SetVisibility(ESlateVisibility::Visible);
		IMG_Win->SetVisibility(ESlateVisibility::Hidden);
		IMG_Lose->SetVisibility(ESlateVisibility::Hidden);

		RTB_BattleResult->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MatchResult_Draw")));
	}
}


void UB2UIGuildBattleHistorySlot::SetOtherGuild(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor, FString GuildName, int32 Medal, int32 MaxMedal)
{
	if (UIP_OtherMark.IsValid())
		UIP_OtherMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);

	if (TB_OtherGuildName.IsValid())
		TB_OtherGuildName->SetText(FText::FromString(GuildName));

	if (TB_OtherMedal.IsValid())
		TB_OtherMedal->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildInfoMedalCount")), FText::AsNumber(Medal), FText::AsNumber(MaxMedal)));

	float Percent = float(Medal) / float(MaxMedal);

	if (PB_OtherMedal.IsValid())
		PB_OtherMedal->SetPercent(Percent);

	if (TB_OtherMedalPercent.IsValid())
		TB_OtherMedalPercent->SetText(FText::Format(FText::FromString(TEXT("{0}%")), GetAutoFractionalFormattedText(Percent * 100, 2)));

}


void UB2UIGuildBattleHistorySlot::SetTime(int64 UnixTime)
{
	FString StrDay, StrHour, StrMin;

	FDateTime BattleTime = FDateTime::FromUnixTimestamp(UnixTime / 1000);


	TB_Time->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}/{2}")),
		FText::FromString(FString::Printf(TEXT("%02d"), BattleTime.GetYear())), 
		FText::FromString(FString::Printf(TEXT("%02d"), BattleTime.GetMonth())),
		FText::FromString(FString::Printf(TEXT("%02d"), BattleTime.GetDay()))));
	
}

void UB2UIGuildBattleHistorySlot::SetBattleLooking(int32 MyMaxMedal, int32 OtherMaxMedal)
{
	if (RTB_BattleLooking.IsValid())
	{
		RTB_BattleLooking->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleLookingMsg")), FText::AsNumber(int32(OtherMaxMedal/8)), FText::AsNumber(int32(MyMaxMedal/8))));
	}
}

void UB2UIGuildBattleHistoryRecord::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, TB_Win)
	GET_SLOT(UB2RichTextBlock, TB_Lose)
	GET_SLOT(UB2RichTextBlock, TB_Draw)
	GET_SLOT(UB2RichTextBlock, TB_Rank)
	GET_SLOT(UB2RichTextBlock, TB_Score)
}

void UB2UIGuildBattleHistoryRecord::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIGuildBattleHistoryRecord::Init()
{
	Super::Init();
}

void UB2UIGuildBattleHistoryRecord::SetRecord(int32 Win, int32 Lose, int32 Draw, int32 Rank, int32 TotalRank, int32 Score)
{
	if (TB_Win.IsValid())
		TB_Win->SetText(FText::AsNumber(Win));

	if (TB_Lose.IsValid())
		TB_Lose->SetText(FText::AsNumber(Lose));

	if (TB_Draw.IsValid())
		TB_Draw->SetText(FText::AsNumber(Draw));

	if (TB_Rank.IsValid())
	{
		float Percent = TotalRank ? (float(Rank) / float(TotalRank)) * 100 : 0;
		TB_Rank->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildRankNumber")), FText::AsNumber(Rank), FText::FromString(FString::Printf(TEXT("%.1f"), Percent))));
	}

	if (TB_Score.IsValid())
	{
		TB_Score->SetText(FText::AsNumber(Score));
	}
}
