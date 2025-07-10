// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIActOpen.h"

#include "B2UIManager.h"
#include "B2UIDocHelper.h"



#include "BladeIIGameImpl.h"

void UB2UIActOpen::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UImage, IMG_O_BG);
	GET_SLOT(UImage, FX_O_Back_02);
	GET_SLOT(UImage, FX_O_Back_01);
	GET_SLOT(UImage, IMG_O_BG02);
	GET_SLOT(UImage, IMG_O_Deco);
	GET_SLOT(UImage, IMG_O_Deco03);
	GET_SLOT(UImage, IMG_O_Deco04);
	GET_SLOT(UImage, FX_O_Line01);
	GET_SLOT(UImage, FX_O_Line02);

	GET_SLOT(UB2Button, BTN_Back);
	BTN_Back->SetVisibility(ESlateVisibility::Hidden);
	GET_SLOT(UCanvasPanel, ArenaOpen);
	ArenaOpen->SetVisibility(ESlateVisibility::Hidden);

	GET_SLOT(UTextBlock, TXT_Open);
}

void UB2UIActOpen::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIActOpen::Init()
{
	Super::Init();
}

void UB2UIActOpen::DestroySelf(UB2UIManager* InUIManager)
{	
	Super::DestroySelf(InUIManager);
}

void UB2UIActOpen::PlayOpenAnim()
{
	PlayOpenChapter_BP();
}

void UB2UIActOpen::OnPlayOpenAnimFinish()
{
	SetVisibility(ESlateVisibility::Collapsed);
	BladeIIGameImpl::GetStageDataStore().RequestCachedGetActInfoAndChangeChapter();
}

void UB2UIActOpen::SetTextData(int32 nChapterNum)
{
	FString sTemp;
	sTemp = FString::Printf(TEXT("ActOpen_Title_%d"), nChapterNum);	

	TXT_Open->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, sTemp));	
	
}