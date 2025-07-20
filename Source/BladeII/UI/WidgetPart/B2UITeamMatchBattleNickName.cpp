
#include "B2UITeamMatchBattleNickName.h"
#include "B2UIGradeRankAndStar.h"


UB2UITeamMatchBattleNickName::UB2UITeamMatchBattleNickName(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UB2UITeamMatchBattleNickName::Init()
{
	Super::Init();
}

void UB2UITeamMatchBattleNickName::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_NickName);
	GET_SLOT(UB2UIGradeRankAndStar, UIP_PVPGrade);
	if (UIP_PVPGrade.IsValid())
		UIP_PVPGrade->Init();

	GET_SLOT(UB2Image, IMG_MyBadge);
}

void UB2UITeamMatchBattleNickName::SetPlayerInfo(const FString& NickName, int32 Lv, int32 Grade, int32 Star)
{
	if (TB_NickName.IsValid())
		TB_NickName->SetText(FText::FromString(NickName));
	
	if (UIP_PVPGrade.IsValid())
		UIP_PVPGrade->SetGradeRank(Grade, Star);
}

void UB2UITeamMatchBattleNickName::SetMyPlayerBadge(bool _isVisible)
{
	if (IMG_MyBadge.IsValid())
	{
		IMG_MyBadge->SetVisibility(_isVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
