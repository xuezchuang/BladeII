#include "B2UIStageBegin.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2StageInfo.h"
#include "B2HeroTowerGameMode.h"

void UB2UIStageBegin::CacheAssets()
{
	/*GET_SLOT(UTextBlock, TB_StageNum)
	GET_SLOT(UTextBlock, TB_AreaNameLocalized)
	GET_SLOT(UTextBlock, TB_AreaNameSub)*/
	GET_SLOT(UB2UIRepeatBattleInGameInfo, UIP_RepeatBattleInGameInfoP)
	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->Init();
	}
	/*GET_SLOT(UTextBlock, TB_TextStage)
	GET_SLOT(UTextBlock, TB_CounterDGStageName)
	GET_SLOT(UOverlay,	OV_CounterDG)*/
}

void UB2UIStageBegin::UpdateStaticText()
{
	//¹¹¾øÀ½
}

void UB2UIStageBegin::BindDelegates()
{
}

void UB2UIStageBegin::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_RepeatBattleInGameInfoP.IsValid())
	{
		UIP_RepeatBattleInGameInfoP->DestroySelf(InUIManager);
	}
}

void UB2UIStageBegin::CloseWidgetDelegate()
{

}

void UB2UIStageBegin::BindDocAuto()
{
	/*auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBattleStage);
	if (!DocBattleStage) return;

	if (GetB2GameModeType(this) == EB2GameMode::HeroTower)
	{
		SetHeroTowerInfo();
		return;
	}

	SetStageNum(DocBattleStage->GetClientStageId());
	SetAreaNameLocalized(DocBattleStage->GetLocalizedAreaName());
	SetAreaNameSub(DocBattleStage->GetLocalizedAreaSubName());*/
}

void UB2UIStageBegin::UnbindDoc()
{
	SetDoc(nullptr);
}

/*
void UB2UIStageBegin::SetStageNum(int32 InValue)
{
	bool bIsCounterDG = UB2StageInfo::IsCounterDGStage(InValue);
	bool bIsTutorial = GetB2GameModeType(this) == EB2GameMode::Tutorial;
		
	if (TB_StageNum.IsValid())
		TB_StageNum->SetText(bIsTutorial ? FText() : FText::AsNumber(InValue));

	if (bIsTutorial)
	{
		if (TB_TextStage.IsValid())
			TB_TextStage->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (TB_StageNum.IsValid()) {
			TB_StageNum->SetVisibility(ESlateVisibility::Collapsed);
		}
		TB_TextStage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialTitle_Begin")));
		//TB_TextStage->SetText(FText::FromName(TEXT("TUTORIAL")));
	}
	else
	{
		if (TB_TextStage.IsValid())
			TB_TextStage->SetVisibility(bIsCounterDG ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		if (TB_StageNum.IsValid())
			TB_StageNum->SetVisibility(bIsCounterDG ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		if (TB_TextStage.IsValid())
			TB_TextStage->SetVisibility(bIsCounterDG ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		if (TB_StageNum.IsValid())
			TB_StageNum->SetVisibility(bIsCounterDG ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
	
	if (OV_CounterDG.IsValid())
		OV_CounterDG->SetVisibility(bIsCounterDG ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (TB_CounterDGStageName.IsValid())
		TB_CounterDGStageName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_CounterDG_FullName")));
}
void UB2UIStageBegin::SetAreaNameLocalized(const FText& InText)
{
	if (TB_AreaNameLocalized.IsValid())
		TB_AreaNameLocalized->SetText(InText);
}
void UB2UIStageBegin::SetAreaNameSub(const FText& InText)
{
	if (TB_AreaNameSub.IsValid())
		TB_AreaNameSub->SetText(InText);
}

void UB2UIStageBegin::SetHeroTowerInfo()
{
	OV_CounterDG->SetVisibility(ESlateVisibility::Collapsed);

	AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(this));

	if (!pGM)
		return;
		
	int32 nHeroTowerFloor = pGM->GetHeroTowerFloor();

	if (TB_TextStage.IsValid())
		TB_TextStage->SetText(FText::FromString(TEXT("FLOOR")));
	
	if (TB_StageNum.IsValid())
		TB_StageNum->SetText(FText::AsNumber(nHeroTowerFloor));

	SetAreaNameLocalized(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_HeroTop")));

	SetAreaNameSub(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_NFloor")), FText::AsNumber(nHeroTowerFloor)));
}*/

//====================================================================================
//									Click Handler
//====================================================================================

