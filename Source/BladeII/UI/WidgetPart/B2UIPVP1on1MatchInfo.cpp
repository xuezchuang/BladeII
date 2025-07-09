#include "BladeII.h"
#include "B2UIPVP1on1MatchInfo.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIGuildMark.h"


#include "BladeIIUtil.h"

UB2UIPVP1on1MatchInfo::UB2UIPVP1on1MatchInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIPVP1on1MatchInfo::Init()
{
	Super::Init();
}


void UB2UIPVP1on1MatchInfo::UpdateStaticText()
{
	if (TB_AttackPowerText.IsValid())
	{
		TB_AttackPowerText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack"))));
	}

	if (TB_DefenseText.IsValid())
	{
		TB_DefenseText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense"))));
	}
}

void UB2UIPVP1on1MatchInfo::CacheAssets()
{
	GET_SLOT(UB2UIGradeRankAndStar, UIP_GradeStar);

	GET_SLOT(UTextBlock, TB_AttackPowerText);
	GET_SLOT(UTextBlock, TB_AttackPowerValue);
	GET_SLOT(UTextBlock, TB_DefenseText);
	GET_SLOT(UTextBlock, TB_DefenseValue);
	GET_SLOT(UPanelWidget, P_AttackPowerInfo);
	GET_SLOT(UPanelWidget, P_DefenseInfo);

	GET_SLOT(UTextBlock,			TB_UserName);
	GET_SLOT(UTextBlock,			TB_WinningStreak);
	GET_SLOT(UTextBlock,			TB_CombatPower);
	GET_SLOT(UOverlay,				Talk);
	GET_SLOT(UTextBlock,			TB_BattleMent);
	GET_SLOT(UB2Image,				IMG_MyBadge);

	if (UIP_GradeStar.IsValid())
		UIP_GradeStar->Init();

	if (Talk.IsValid())
	{
		Talk->SetVisibility(ESlateVisibility::Hidden);
	}
/*
	if (TB_BattleMent.IsValid())
	{
		int32 RandIdx = FMath::RandRange(0, BattleMentTextKeys.Num() - 1);
		if (BattleMentTextKeys.IsValidIndex(RandIdx))
			TB_BattleMent->SetText(BladeIIGetLOCText(B2LOC_CAT_BATTLE_MENT, BattleMentTextKeys[RandIdx]));
	}
*/
}

void UB2UIPVP1on1MatchInfo::SetLeft(bool bSet)
{
	bLeft = bSet;
}

void UB2UIPVP1on1MatchInfo::SetAttackPower(int32 InAttackPower)
{
	if (InAttackPower == 0)
	{
		if (P_AttackPowerInfo.IsValid())
			P_AttackPowerInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (P_AttackPowerInfo.IsValid())
			P_AttackPowerInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (TB_AttackPowerValue.IsValid())
			TB_AttackPowerValue->SetText(FText::AsNumber(InAttackPower));
	}
}

void UB2UIPVP1on1MatchInfo::SetDefense(int32 InDefense)
{
	if (InDefense == 0)
	{
		if (P_DefenseInfo.IsValid())
			P_DefenseInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (P_DefenseInfo.IsValid())
			P_DefenseInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (TB_DefenseValue.IsValid())
			TB_DefenseValue->SetText(FText::AsNumber(InDefense));
	}
}

void UB2UIPVP1on1MatchInfo::SetUserName(const FString& UserName)
{
	if (TB_UserName.IsValid())
	{
		TB_UserName->SetText(FText::FromString(UserName));
	}
}

void UB2UIPVP1on1MatchInfo::SetGradeAndStar(int32 Grade, int32 Star)
{
	if (UIP_GradeStar.IsValid())
	{
		UIP_GradeStar->SetGradeRank(Grade, Star);
	}
}

void UB2UIPVP1on1MatchInfo::SetWinningStreak(int32 InWins)
{
	if (TB_WinningStreak.IsValid())
	{
		if (InWins < 1)
			TB_WinningStreak->SetVisibility(ESlateVisibility::Collapsed);
		else
		{
			TB_WinningStreak->SetVisibility(ESlateVisibility::HitTestInvisible);
			TB_WinningStreak->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_StreakWins")), FText::AsNumber(InWins)));
		}			
	}
}

void UB2UIPVP1on1MatchInfo::SetCombatPower(int32 InValue)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(InValue));
}

void UB2UIPVP1on1MatchInfo::SetMyBadgeImage(bool _isLocalPlayer)
{
	if (IMG_MyBadge.IsValid())
	{
		ESlateVisibility visible = _isLocalPlayer ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		IMG_MyBadge->SetVisibility(visible);
	}
}

void UB2UIPVP1on1MatchInfo::SetVisibilityStarGrade(bool bVisible)
{
	if (UIP_GradeStar.IsValid())
		UIP_GradeStar->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}
