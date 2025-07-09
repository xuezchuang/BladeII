// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIEnterAdventure.h"
#include "B2UIDocHelper.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameImpl.h"

void UB2UIEnterAdventure::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIEnterAdventureSlot, UIP_15Act)
	GET_SLOT(UB2UIEnterAdventureSlot, UIP_6Act)
	GET_SLOT(UB2UIEnterAdventureSlot, UIP_7Act)


	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	if(UIP_15Act.IsValid())
	{
		UIP_15Act->Init();
		UIP_15Act->SetAdventureLock(false);																							//1~5는 항상 열려있음
		UIP_15Act->SetAdventureType(AdventureNewChapterNumber::Act1_5);
		UIP_15Act->FOnClickDelegate.BindUObject(this, &UB2UIEnterAdventure::OnClickAct15);
	}

	if (UIP_6Act.IsValid())
	{
		UIP_6Act->Init();
		UIP_6Act->SetAdventureLock(!StageDataStore.GetNewChapterOpenState(AdventureNewChapterNumber::Act6) , AdventureNewChapterNumber::Act6);	
		UIP_6Act->SetAdventureType(AdventureNewChapterNumber::Act6);
		UIP_6Act->FOnClickDelegate.BindUObject(this, &UB2UIEnterAdventure::OnClickAct6);
	}

	if (UIP_7Act.IsValid())
	{
		UIP_7Act->Init();
		UIP_7Act->SetAdventureLock(!StageDataStore.GetNewChapterOpenState(AdventureNewChapterNumber::Act7), AdventureNewChapterNumber::Act7);				
		UIP_7Act->SetAdventureType(AdventureNewChapterNumber::Act7);
		UIP_7Act->FOnClickDelegate.BindUObject(this, &UB2UIEnterAdventure::OnClickAct7);
	}
}

void UB2UIEnterAdventure::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIEnterAdventure::Init()
{
	Super::Init();
}

void UB2UIEnterAdventure::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIEnterAdventure::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("AdventureChoice"));
}

void UB2UIEnterAdventure::RegisterUIMarkForRedDot()
{

}

void UB2UIEnterAdventure::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);
}

void UB2UIEnterAdventure::ChangeChaperScene(int32 ChapterNum)
{
	LobbyChangeToChapterSceneByUISceneClass<int32>::GetInstance().Signal(ChapterNum);
}

void UB2UIEnterAdventure::UpdateStaticText()
{
	if (UIP_15Act.IsValid())
	{
		UIP_15Act->SetDungeonNameText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("ChapterEnterName_1")));
		UIP_15Act->SetDungeonDescText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("ChapterEnterText_1")));
	}

	if (UIP_6Act.IsValid())
	{
		UIP_6Act->SetDungeonNameText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("ChapterEnterName_2")));
		UIP_6Act->SetDungeonDescText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("ChapterEnterText_2")));
		UIP_6Act->SetNoEnterText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("Act6_Open")));
	}

	if (UIP_7Act.IsValid())
	{
		UIP_7Act->SetDungeonNameText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("ChapterEnterName_3")));
		UIP_7Act->SetDungeonDescText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("ChapterEnterText_3")));
		UIP_7Act->SetNoEnterText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, TEXT("Act7_Open")));
	}
}

void UB2UIEnterAdventure::UpdateServercmdGetAccount()
{
	Init();
}

void UB2UIEnterAdventure::OnClickAct15()
{
	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

	EStageDifficulty StageDifficulty = StageStore.GetMaxOpenedStageDifficulty();
	FServerStageID ServerStageID = StageStore.GetLastClearServerStageId(StageDifficulty);
	int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetLastClearClientId(StageDifficulty));
	
	if (ServerStageID == 0)// 서버에 저장된 최근 플레이 정보가 0 일때. 강제로 1-1로 세팅
	{
		ChapterNumber = 1;
		StageDifficulty = EStageDifficulty::ESD_Normal;
	}
	else if (ChapterNumber > 5)
	{
		ChapterNumber = 5;
		StageDifficulty = EStageDifficulty::ESD_Hell;
	}
	else
	{
		ChapterNumber = BladeIIGameImpl::GetStageDataStore().GetMaxReachedActNumber(StageDifficulty);
		
		if (StageStore.IsClearedFinalClientStageId(StageDifficulty) || ChapterNumber > 5)
			ChapterNumber = 5;
	}

	BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(ChapterNumber, StageDifficulty);
}

	

void UB2UIEnterAdventure::OnClickAct6()
{
	int32 InChapter = AdventureNewChapterNumber::Act6;

	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

	FServerStageID ServerStageID = StageStore.GetLastPlayedServerStageId();
	int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetClientStageId(ServerStageID));

	EStageDifficulty StageDifficulty;

	if(ChapterNumber == InChapter)
		StageDifficulty = StageStore.GetStageDifficulty(ServerStageID);
	else
		StageDifficulty = StageStore.GetMaxOpenedNewStageDifficulty(InChapter);

	BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(InChapter, StageDifficulty);
}

void UB2UIEnterAdventure::OnClickAct7()
{
	int32 InChapter = AdventureNewChapterNumber::Act7;

	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

	FServerStageID ServerStageID = StageStore.GetLastPlayedServerStageId();
	int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetClientStageId(ServerStageID));

	EStageDifficulty StageDifficulty;

	if (ChapterNumber == InChapter)
		StageDifficulty = StageStore.GetStageDifficulty(ServerStageID);
	else
		StageDifficulty = StageStore.GetMaxOpenedNewStageDifficulty(InChapter);

	BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(InChapter, StageDifficulty);
}

void UB2UIEnterAdventureSlot::CacheAssets()
{
	Super::CacheAssets();


	GET_SLOT(UB2Button, BTN_Select);

	GET_SLOT(UOverlay, OV_NoEnter);
	GET_SLOT(UOverlay, Act_6);
	GET_SLOT(UOverlay, Act_7);

	GET_SLOT(UImage, Img_ACT6NoEnter)
	GET_SLOT(UImage, Img_ACT7NoEnter)


	GET_SLOT(UTextBlock, TB_DungeonName);
	GET_SLOT(UTextBlock, TB_DungeonDesc);
	GET_SLOT(UB2RichTextBlock, TB_NoEnterText);
}

void UB2UIEnterAdventureSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIEnterAdventureSlot::OnClickAdventure);
}

void UB2UIEnterAdventureSlot::Init()
{
	Super::Init();
}

void UB2UIEnterAdventureSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIEnterAdventureSlot::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);
}

void UB2UIEnterAdventureSlot::SetAdventureLock(bool InState, int32 InChapter)
{

	if (!InState && InChapter)
	{
		bool bAniState = BladeIIGameImpl::GetStageDataStore().LoadOpenCahpter(InChapter);

		if (!bAniState)
		{
			OpenAnimation();
			BladeIIGameImpl::GetStageDataStore().SaveOpenCahpter(InChapter,true);
			return;
		}
	}
	else if(InChapter)
		BladeIIGameImpl::GetStageDataStore().SaveOpenCahpter(InChapter,false);

	if (OV_NoEnter.IsValid())
		OV_NoEnter->SetVisibility(InState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (BTN_Select.IsValid())
		BTN_Select->SetVisibility(InState ? ESlateVisibility::Hidden : ESlateVisibility::Visible);


	if (InState == true)
	{
		if (Img_ACT6NoEnter.IsValid())
			Img_ACT6NoEnter->SetVisibility(InChapter == AdventureNewChapterNumber::Act6 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (Img_ACT7NoEnter.IsValid())
			Img_ACT7NoEnter->SetVisibility(InChapter == AdventureNewChapterNumber::Act7 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UIEnterAdventureSlot::SetAdventureType(int32 InState)
{
	if (Act_6.IsValid())
	{
		Act_6->SetVisibility(InState == AdventureNewChapterNumber::Act6 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (Act_7.IsValid())
	{
		Act_7->SetVisibility(InState == AdventureNewChapterNumber::Act7 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UIEnterAdventureSlot::SetDungeonNameText(FText InText)
{
	if (TB_DungeonName.IsValid())
		TB_DungeonName->SetText(InText);
}

void UB2UIEnterAdventureSlot::SetDungeonDescText(FText InText)
{
	if (TB_DungeonDesc.IsValid())
		TB_DungeonDesc->SetText(InText);
}

void UB2UIEnterAdventureSlot::SetNoEnterText(FText InText)
{
	if (TB_NoEnterText.IsValid())
		TB_NoEnterText->SetText(InText);
}


void UB2UIEnterAdventureSlot::OnClickAdventure()
{
	FOnClickDelegate.Execute();
}

