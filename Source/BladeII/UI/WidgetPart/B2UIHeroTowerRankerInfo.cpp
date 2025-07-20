// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHeroTowerRankerInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIManager.h"
#include "B2UIPVPEntryCharacters.h"



void UB2UIHeroTowerRankerInfo::Init()
{
	Super::Init();
}

void UB2UIHeroTowerRankerInfo::SetRankerInfo(const FModRankerInfo& RankerInfo)
{
	check(RankerInfo.Entry.Num() == 2 && RankerInfo.EntryLevels.Num() == 2);

	AccountInfo = RankerInfo.AccountID;
	MainClass = RankerInfo.Entry[0];
	SubClass = RankerInfo.Entry[1];


	if (IMG_RankNumber_1.IsValid())
		IMG_RankNumber_1->SetVisibility(RankerInfo.Rank == 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankNumber_2.IsValid())
		IMG_RankNumber_2->SetVisibility(RankerInfo.Rank == 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankNumber_3.IsValid())
		IMG_RankNumber_3->SetVisibility(RankerInfo.Rank == 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankBG_1.IsValid())
		IMG_RankBG_1->SetVisibility(RankerInfo.Rank == 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankBG_2.IsValid())
		IMG_RankBG_2->SetVisibility(RankerInfo.Rank == 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankBG_3.IsValid())
		IMG_RankBG_3->SetVisibility(RankerInfo.Rank == 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (TB_Ranking.IsValid())
	{
		TB_Ranking->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("PVP1on1_RankerRank")), FText::AsNumber(RankerInfo.Rank)));
		TB_Ranking->SetVisibility(RankerInfo.Rank > 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (TB_PlayerLevel.IsValid()) //蜡历饭骇俺充.
		TB_PlayerLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("PVP1on1_RankerLevel")), FText::AsNumber(RankerInfo.EntryLevels[0])));

	if (TB_PlayerName.IsValid())
		TB_PlayerName->SetText(FText::FromString(RankerInfo.UserName));

	if (TB_TotalAttackPoint.IsValid())
		TB_TotalAttackPoint->SetText(FText::AsNumber(RankerInfo.TotalCombatPower)); // jwyim power

	//if (TB_TotalDefencePoint.IsValid())
		//TB_TotalDefencePoint->SetText(FText::AsNumber(RankerInfo.TotalDefencePoint));// jwyim power

	if (TB_ClearTime.IsValid())
		TB_ClearTime->SetText(FText::FromString(UB2UIManager::FormatMilliSecondsAsTime(RankerInfo.ClearTimeSec, TEXT("mm.ss.ms"))));

	if (TB_ClearTower.IsValid())
		TB_ClearTower->SetText(FText::FromString(FString::FromInt(RankerInfo.BestFloor)));

	if (UIP_GradeIcon.IsValid())
	{
		UIP_GradeIcon->SetGradeRank(RankerInfo.Grade, RankerInfo.GradeStar);
	}

	if (UIP_EntryCharacters.IsValid())
		UIP_EntryCharacters->SetEntryClasses(RankerInfo.Entry[0], RankerInfo.Entry[1]);
}

void UB2UIHeroTowerRankerInfo::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Ranking);
	GET_SLOT(UB2UIGradeRankAndStar, UIP_GradeIcon);
	if (UIP_GradeIcon.IsValid())
		UIP_GradeIcon->Init();

	GET_SLOT(UB2UIPVPEntryCharacters, UIP_EntryCharacters);
	if (UIP_EntryCharacters.IsValid())
		UIP_EntryCharacters->Init();

	GET_SLOT(UButton, BTN_ShowPlayerInfo);

	GET_SLOT(UTextBlock, TB_TotalAttackPoint);
	//GET_SLOT(UTextBlock, TB_TotalDefencePoint);
	GET_SLOT(UTextBlock, TB_PlayerLevel);
	GET_SLOT(UTextBlock, TB_PlayerName);
	GET_SLOT(UTextBlock, TB_ClearTime);
	GET_SLOT(UTextBlock, TB_ClearTower);

	GET_SLOT(UTextBlock, STB_CombatPower);
	GET_SLOT(UTextBlock, STB_HighScore);

	GET_SLOT(UImage, IMG_RankNumber_1);
	GET_SLOT(UImage, IMG_RankNumber_2);
	GET_SLOT(UImage, IMG_RankNumber_3);
	GET_SLOT(UImage, IMG_RankBG_1);
	GET_SLOT(UImage, IMG_RankBG_2);
	GET_SLOT(UImage, IMG_RankBG_3);

	GET_SLOT(UB2Button, BTN_Select);
}

void UB2UIHeroTowerRankerInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIHeroTowerRankerInfo::OnClickSelect);
}

void UB2UIHeroTowerRankerInfo::OnClickSelect()
{
	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(AccountInfo, MainClass, SubClass, EPCClass::EPC_End);
}

void UB2UIHeroTowerRankerInfo::UpdateStaticText()
{
	if (STB_CombatPower.IsValid()) // 傈捧仿
		STB_CombatPower->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
	if (STB_HighScore.IsValid()) // 弥绊扁废
		STB_HighScore->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_HighRecord")));
}