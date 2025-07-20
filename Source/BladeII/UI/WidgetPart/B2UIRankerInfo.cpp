#include "B2UIRankerInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIPVPEntryCharacters.h"


#include "BladeIIUtil.h"

void UB2UIRankerInfo::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Ranking);
	GET_SLOT(UB2UIGradeRankAndStar, UIP_GradeIcon);
	if (UIP_GradeIcon.IsValid())
		UIP_GradeIcon->Init();

	GET_SLOT(UB2UIPVPEntryCharacters, UIP_EntryCharacters);
	if (UIP_EntryCharacters.IsValid())
		UIP_EntryCharacters->Init();

	GET_SLOT(UButton, BTN_ShowPlayerInfo);

	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_PlayerLevel);
	GET_SLOT(UTextBlock, TB_PlayerName);

	GET_SLOT(UImage, IMG_RankNumber_1);
	GET_SLOT(UImage, IMG_RankNumber_2);
	GET_SLOT(UImage, IMG_RankNumber_3);
	GET_SLOT(UImage, IMG_RankBG_1);
	GET_SLOT(UImage, IMG_RankBG_2);
	GET_SLOT(UImage, IMG_RankBG_3);

	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UImage, IMG_BestBG);
	GET_SLOT(UImage, IMG_Line);

    GET_SLOT(UB2Button, BTN_PVPReplay);
    GET_SLOT(UTextBlock, TB_PVPReplay);

	//[@AKI, 170420] M16 老措老&评措傈俺急 p.27 康旷 饭骇 昏力肺 牢秦 Visibility 函版
	// TB_PlayerLevel阑 促 瘤况滚妨档 登绰单...趣矫 隔扼辑 巢败 初澜
	if (TB_PlayerLevel.IsValid())
	{
		TB_PlayerLevel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIRankerInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIRankerInfo::OnClickSelect);
    BIND_CLICK_FUNC_TO_BTN(BTN_PVPReplay, &UB2UIRankerInfo::OnClickPVPReplay);
}
void UB2UIRankerInfo::OnClickSelect()
{
	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(AccountInfo, MainClass, SubClass,EPCClass::EPC_End);
}

void UB2UIRankerInfo::OnClickPVPReplay()
{
    UE_LOG(LogBladeII, Log, TEXT("OnClickPVPReplay URL(%s) Compressed(%d) Uncompressed(%d)"), *ReplayInfo.Url, ReplayInfo.SizeCompressed, ReplayInfo.SizeUncompressed);
    StartPVPReplayClass<FString, int32, int32>::GetInstance().Signal(ReplayInfo.Url, ReplayInfo.SizeCompressed, ReplayInfo.SizeUncompressed);
}

void UB2UIRankerInfo::Init()
{
	Super::Init();

	MainClass = EPCClass::EPC_End;
	SubClass = EPCClass::EPC_End;
}

void UB2UIRankerInfo::SetRankerInfo(const FModRankerInfo& RankerInfo,bool bNotResult)
{
	//check(RankerInfo.Entry.Num() == 2 && RankerInfo.EntryLevels.Num() == 2);

	AccountInfo = RankerInfo.AccountID;
    ReplayInfo = RankerInfo.ReplayInfo;

	if (RankerInfo.Entry.Num() == 1)
	{
		MainClass = RankerInfo.Entry[0];
	}

	if (RankerInfo.Entry.Num() == 2)
	{
		MainClass = RankerInfo.Entry[0];
		SubClass = RankerInfo.Entry[1];
	}

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

	//[@AKI, 170420] M16 老措老&评措傈俺急 p.27 康旷 饭骇 昏力肺 牢秦 林籍 贸府
//	if (TB_PlayerLevel.IsValid()) //蜡历饭骇俺充.
//		TB_PlayerLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("PVP1on1_RankerLevel")), FText::AsNumber(RankerInfo.EntryLevels[0])));

	if (TB_PlayerName.IsValid())
		TB_PlayerName->SetText(FText::FromString(RankerInfo.UserName));

    if (TB_CombatPower.IsValid())
    {
        TB_CombatPower->SetText(FText::AsNumber(RankerInfo.TotalCombatPower));
    }

    if (BTN_PVPReplay.IsValid())
    {
        BTN_PVPReplay->SetVisibility(ESlateVisibility::Collapsed);
    }

	if (UIP_GradeIcon.IsValid())
	{
		UIP_GradeIcon->SetGradeRank(RankerInfo.Grade, RankerInfo.GradeStar);
	}
		
	if (UIP_EntryCharacters.IsValid())
		UIP_EntryCharacters->SetEntryClasses(MainClass, SubClass);

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
			IMG_Line->SetVisibility( ESlateVisibility::Collapsed);
	}

}
