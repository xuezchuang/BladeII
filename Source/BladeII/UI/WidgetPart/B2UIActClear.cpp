// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIActClear.h"

#include "B2UIManager.h"
#include "QuestManager.h"
#include "B2UIQuestDialog.h"
#include "B2UIDocHelper.h"

#include "TutorialManager.h"


#include "BladeIIGameImpl.h"

void UB2UIActClear::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UImage, IMG_C_BG);
	GET_SLOT(UImage, FX_Back_02);
	GET_SLOT(UImage, FX_Back_01);
	GET_SLOT(UImage, IMG_C_BG02);
	GET_SLOT(UImage, IMG_C_Deco);
	GET_SLOT(UImage, FX_Back_03);
	GET_SLOT(UImage, IMG_Deco03);
	GET_SLOT(UImage, IMG_Deco04);

	GET_SLOT(UB2Button, BTN_Back);
	BTN_Back->SetVisibility(ESlateVisibility::Hidden);
	GET_SLOT(UCanvasPanel, ArenaClear);
	ArenaClear->SetVisibility(ESlateVisibility::Hidden);
	
	GET_SLOT(UTextBlock, TXT_Clear);
	GET_SLOT(UTextBlock, TXT_ClearDoc);
}

void UB2UIActClear::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIActClear::Init()
{
	Super::Init();
}

void UB2UIActClear::DestroySelf(UB2UIManager* InUIManager)
{	
	Super::DestroySelf(InUIManager);
}

void UB2UIActClear::PlayClearAnim()
{
	BTN_Back->SetVisibility(ESlateVisibility::HitTestInvisible);

	PlayClearChapter_BP();

	HasAnyTutorialActClearAnimStartCheck();
}

void UB2UIActClear::OnPlayClearAnimFinish()
{
	SetVisibility(ESlateVisibility::Collapsed);
	BladeIIGameImpl::GetStageDataStore().RequestCachedGetActInfoAndChangeChapter();

	HasAnyTutorialVisibleCheck();
}

void UB2UIActClear::SetTextData(int32 nChapterNum)
{
	//FString sTemp = FString::Printf(TEXT("ActClear_Title_%d"), nChapterNum);
	//FString sTempDoc = FString::Printf(TEXT("ActClear_Desc_%d"), nChapterNum);

	//// Note : GDC烙矫 内靛 (GDC瘤唱搁 瘤况林磊~~~ by spivy)
	//{
	//	if (ShouldShowGDCVersionContent())
	//	{
	//		if (UB2StageInfo* StageInfoTable = StaticFindStageInfo())
	//		{
	//			if (nChapterNum == StageInfoTable->GetChapterNumByClientStageId(StageInfoTable->GetFinalClientStageId()))
	//			{
	//				sTemp = FString(TEXT("ActClear_Title_GDC"));
	//				sTempDoc = FString(TEXT("ActClear_Desc_GDC"));
	//			}
	//		}
	//	}
	//}

	//UE_LOG(LogStreaming, Warning, TEXT("ActClear_Title_ ====== '%s' "), *sTemp);
	//UE_LOG(LogStreaming, Warning, TEXT("ActClear_Desc_ ====== '%s' "), *sTempDoc);
	//TXT_Clear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, sTemp));
	//TXT_ClearDoc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, sTempDoc));
	
}

void UB2UIActClear::HasAnyTutorialVisibleCheck()
{
	if (!TutorialManager::GetInstance().HasAnyTutorial())
		return;

	TutorialManager::GetInstance().ActClearAnimEnd();
}

void UB2UIActClear::HasAnyTutorialActClearAnimStartCheck()
{
	if (!TutorialManager::GetInstance().HasAnyTutorial())
		return;

	TutorialManager::GetInstance().ActClearAnimStart();
}