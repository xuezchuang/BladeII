
#include "B2UITeamMatchInfo.h"
#include "B2UIGradeRankAndStar.h"
#include "BladeIILocText.h"

UB2UITeamMatchInfo::UB2UITeamMatchInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UITeamMatchInfo::Init()
{
	Super::Init();
}


void UB2UITeamMatchInfo::CacheAssets()
{
	GET_SLOT(UB2UIGradeRankAndStar, UIP_GradeStar);

	GET_SLOT(UTextBlock, TB_UserLevel);
	GET_SLOT(UTextBlock, TB_UserName);
	GET_SLOT(UTextBlock, TB_GuildName);
	GET_SLOT(UTextBlock, TB_WinningStreak);

	GET_SLOT(UB2Image, IMG_MyBadge);
}

void UB2UITeamMatchInfo::SetLeft(bool bSet)
{
	bLeft = bSet;
}

void UB2UITeamMatchInfo::SetChracterLevel(int32 InLevel)
{
	if (TB_UserLevel.IsValid())
	{
		TB_UserLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Hero_Level")), FText::AsNumber(InLevel)));
	}
}

void UB2UITeamMatchInfo::SetMyBadgeImage(bool _isLocalPlayer)
{
	if (IMG_MyBadge.IsValid())
	{
		ESlateVisibility visible = _isLocalPlayer ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		IMG_MyBadge->SetVisibility(visible);
	}
}