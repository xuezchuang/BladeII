#include "B2UIMobAppearBossStage.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"


void UB2UIMobAppearBossStage::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_MobName)
}

void UB2UIMobAppearBossStage::UpdateStaticText()
{
}

void UB2UIMobAppearBossStage::BindDelegates()
{
}

void UB2UIMobAppearBossStage::BindDocAuto()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBattleStage);
	if (!DocBattleStage) return;

	SetMobName(DocBattleStage->GetCurAppearedMobName());
}

void UB2UIMobAppearBossStage::UnbindDoc()
{
	SetDoc(nullptr);
}

void UB2UIMobAppearBossStage::SetMobName(const FText& InText)
{
	if (TB_MobName.IsValid())
		TB_MobName->SetText(InText);
}

//====================================================================================
//									Click Handler
//====================================================================================
