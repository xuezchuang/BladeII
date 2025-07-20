
#include "B2UITeamRankerInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UITeamEntryCharacter.h"
#include "B2UIGradeRankAndStar.h"
#include "Retailer.h"
#include "BladeIILocText.h"

void UB2UITeamRankerInfo::Init()
{
	Super::Init();
}

void UB2UITeamRankerInfo::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Ranking);
	GET_SLOT(UB2UIGradeRankAndStar, UIP_GradeIcon);
	if (UIP_GradeIcon.IsValid())
		UIP_GradeIcon->Init();

	GET_SLOT(UB2UITeamEntryCharacter, UIP_Team_Entry_Characters1);

	if (UIP_Team_Entry_Characters1.IsValid())
		UIP_Team_Entry_Characters1->Init();

	GET_SLOT(UB2UITeamEntryCharacter, UIP_Team_Entry_Characters2);

	if (UIP_Team_Entry_Characters2.IsValid())
		UIP_Team_Entry_Characters2->Init();

	GET_SLOT(UB2UITeamEntryCharacter, UIP_Team_Entry_Characters3);

	if (UIP_Team_Entry_Characters3.IsValid())
		UIP_Team_Entry_Characters3->Init();

	GET_SLOT(UTextBlock, TB_TotalAttackPoint);
	GET_SLOT(UTextBlock, TB_TotalDefencePoint);
	GET_SLOT(UTextBlock, TB_CombatPower);

	// [@AKI, 170419] M16俊辑 力芭 登菌栏唱 UI俊辑 父距 昏力甫 救沁阑 版快甫 措厚秦 Visible阑 救窍霸 窃
	GET_SLOT(UTextBlock, TB_PlayerLevel);
	if (TB_PlayerLevel.IsValid())
	{
		TB_PlayerLevel->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UTextBlock, TB_PlayerName);

	GET_SLOT(UImage, IMG_RankNumber_1);
	GET_SLOT(UImage, IMG_RankNumber_2);
	GET_SLOT(UImage, IMG_RankNumber_3);
	GET_SLOT(UImage, IMG_RankBG_1);
	GET_SLOT(UImage, IMG_RankBG_2);
	GET_SLOT(UImage, IMG_RankBG_3);
	GET_SLOT(UImage, IMG_BestBG);
	GET_SLOT(UImage, IMG_Line);

	GET_SLOT(UB2Button, BTN_Select);
}

void UB2UITeamRankerInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UITeamRankerInfo::OnClickSelect);
}

void UB2UITeamRankerInfo::SetRankerInfo(const FModRankerInfo& RankerInfo, bool bNotResult)
{
	check(RankerInfo.Entry.Num() == 3 && RankerInfo.EntryLevels.Num() == 3);

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

	//if (TB_PlayerLevel.IsValid()) //蜡历饭骇俺充.
	//	TB_PlayerLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("PVP1on1_RankerLevel")), FText::AsNumber(RankerInfo.EntryLevels[0])));

	if (TB_PlayerName.IsValid())
		TB_PlayerName->SetText(FText::FromString(RankerInfo.UserName));


	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(RankerInfo.TotalCombatPower));

	if (UIP_GradeIcon.IsValid())
	{
		UIP_GradeIcon->SetGradeRank(RankerInfo.Grade, RankerInfo.GradeStar);
	}

	PCClass1 = RankerInfo.Entry[0];
	PCClass2 = RankerInfo.Entry[1];
	PCClass3 = RankerInfo.Entry[2];

	if (UIP_Team_Entry_Characters1.IsValid())
		UIP_Team_Entry_Characters1->SetEntryCharacter(PCClass1, RankerInfo.EntryLevels[0]);

	if (UIP_Team_Entry_Characters2.IsValid())
		UIP_Team_Entry_Characters2->SetEntryCharacter(PCClass2, RankerInfo.EntryLevels[1]);

	if (UIP_Team_Entry_Characters3.IsValid())
		UIP_Team_Entry_Characters3->SetEntryCharacter(PCClass3, RankerInfo.EntryLevels[2]);

	AccountId = RankerInfo.AccountID;

	if (bNotResult)
	{
		if (IMG_BestBG.IsValid())
			IMG_BestBG->SetVisibility(RankerInfo.Rank < 4 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (IMG_Line.IsValid())
			IMG_Line->SetVisibility(RankerInfo.Rank == 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	else
	{
		if (IMG_BestBG.IsValid())
			IMG_BestBG->SetVisibility(ESlateVisibility::Collapsed);

		if (IMG_Line.IsValid())
			IMG_Line->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UB2UITeamRankerInfo::OnClickSelect()
{
	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(AccountId, PCClass1, PCClass2, PCClass3);

}
