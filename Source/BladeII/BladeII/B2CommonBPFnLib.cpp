// Fill out your copyright notice in the Description page of Project Settings.


#include "B2CommonBPFnLib.h"
#include "BladeIIPlayer.h"
#include "BladeIIGameMode.h"
#include "BladeIILocText.h"
#include "B2UIManager.h"
#include "B2UIMoviePlayer.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"

FText UB2CommonBPFnLib::B2GM_BladeIIGetLOCText(EB2LOC_CAT CategoryEnum, const FString& TextKey)
{
	return BladeIIGetLOCText(EB2LOC_CAT_ToString(CategoryEnum), TextKey);
}

FText UB2CommonBPFnLib::B2GM_BladeIIGetLOCText_Pure(EB2LOC_CAT CategoryEnum, const FString& TextKey)
{
	return BladeIIGetLOCText(EB2LOC_CAT_ToString(CategoryEnum), TextKey);
}

void UB2CommonBPFnLib::CreateMovieUI(class UMediaSource* InMediaSource, const TArray<struct FMovieSubtitle>& InSubtitleArray)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		
	if (!pUIManager)
		return;

	AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(pUIManager));
	AB2StageManager* StageMgr = StageGM ? StageGM->GetStageManager() : NULL;

	bool ScenarioMode = StageMgr ? StageMgr->IsScenarioMode() : false;

	if (!ScenarioMode)
		return;

	UB2UIMoviePlayer* pUI = pUIManager->OpenUI<UB2UIMoviePlayer>(UIFName::MoviePlayer);

	if (pUI)
	{
		pUI->SetMovie(InMediaSource, InSubtitleArray);
	}
}
