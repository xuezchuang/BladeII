#include "B2UIMobAppearBossArea.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"


void UB2UIMobAppearBossArea::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_MobName)
	GET_SLOT(UTextBlock, TB_MobNickName)
}

void UB2UIMobAppearBossArea::UpdateStaticText()
{
}

void UB2UIMobAppearBossArea::BindDelegates()
{
}

void UB2UIMobAppearBossArea::BindDocAuto()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBattleStage);
	if (!DocBattleStage) return;

	SetMobName(DocBattleStage->GetCurAppearedMobName());
	SetMobNickName(DocBattleStage->GetCurAppearedMobNickName());
}

void UB2UIMobAppearBossArea::UnbindDoc()
{
	SetDoc(nullptr);
}

void UB2UIMobAppearBossArea::SetMobName(const FText& InText)
{
	if (TB_MobName.IsValid())
		TB_MobName->SetText(InText);
}

void UB2UIMobAppearBossArea::SetMobNickName(const FText& InText)
{
	if (TB_MobNickName.IsValid())
		TB_MobNickName->SetText(InText);
}


//====================================================================================
//									Click Handler
//====================================================================================
