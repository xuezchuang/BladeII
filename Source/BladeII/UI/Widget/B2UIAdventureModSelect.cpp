// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIDocHelper.h"
#include "B2UIDocChapter.h"
#include "B2ChapterInfo.h"
#include "B2UIAdventureModSelect.h"
#include "BladeIIGameMode.h"
#include "B2UISelectAdventureDifficulty.h"


#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"

void UB2UIAdventureModSelect::Init()
{
	Super::Init();
}

void UB2UIAdventureModSelect::BindDocAuto()
{
	UnbindDoc();

	auto DocChapter = UB2UIDocHelper::GetDocChapter();
	SetDoc(DocChapter);

	if (DocChapter)
	{	
		// 선택된 하드 스테이지가 없다면 가장 낮은 모험 난이도로 설정
		if (DocChapter->GetSelectHardStageDifficulty() == EStageDifficulty::ESD_None)
		{
			EStageDifficulty MaxOpenedStageDifficulty = DocChapter->GetMaxOpenedStageDifficulty();
			if (MaxOpenedStageDifficulty <= EStageDifficulty::ESD_Normal)
				MaxOpenedStageDifficulty = EStageDifficulty::ESD_Hard;
			DocChapter->SetSelectHardStageDifficulty(MaxOpenedStageDifficulty);
		}

		UpdateScenarioChapterInfo();
		UpdateAdventureChapterInfo();
		UpdateChapterLock();

		// Bind
		DocChapter->OnSelectHardStageDifficultyChanged.AddUObject(this, &UB2UIAdventureModSelect::OnChangeDifficulty);
		DocChapter->OnNextChapterNumChanged.AddUObject(this, &UB2UIAdventureModSelect::OnChangeChapterNum);
		DocChapter->OnNextSubChapterNumChanged.AddUObject(this, &UB2UIAdventureModSelect::OnChangeChapterNum);
	}

	LobbyUpdateChapterDataClass<>::GetInstance().Signal();
}

void UB2UIAdventureModSelect::UnbindDoc()
{
	if (auto DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		DocChapter->OnSelectHardStageDifficultyChanged.RemoveAll(this);
		DocChapter->OnNextChapterNumChanged.RemoveAll(this);
		DocChapter->OnNextSubChapterNumChanged.RemoveAll(this);
	}	

	SetDoc(nullptr);
}

void UB2UIAdventureModSelect::BindDelegates()
{
}

void UB2UIAdventureModSelect::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("AdventureMod_AdventureTitle"));
}

void UB2UIAdventureModSelect::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UCanvasPanel, CP_ThreeDummySlot);
	GET_SLOT(UCanvasPanel, CP_TwoDummySlot);
	
	ThreeDummyChildList.AddUninitialized(3);
	GET_SLOT_BYNAME(class UB2UIAdventureModSlot, ThreeDummyChildList[0], AdventureSlot1);
	GET_SLOT_BYNAME(class UB2UIAdventureModSlot, ThreeDummyChildList[1], AdventureSlot2);
	GET_SLOT_BYNAME(class UB2UIAdventureModSlot, ThreeDummyChildList[2], AdventureSlot3);

	TwoDummyChildList.AddUninitialized(2);
	GET_SLOT_BYNAME(class UB2UIAdventureModSlot, TwoDummyChildList[0], AdventureSlot4);
	GET_SLOT_BYNAME(class UB2UIAdventureModSlot, TwoDummyChildList[1], AdventureSlot5);

	InitAdventureModeSlot();
}

void UB2UIAdventureModSelect::UpdateStaticText()
{
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("AdventureMod_AdventureTitle"));
}

void UB2UIAdventureModSelect::SetAdventureDifficulty(EStageDifficulty Difficulty)
{	
	if (CurAdventureSlotMap.Contains(EAdventureModeType::Adventure) == false ||
		CurAdventureSlotMap[EAdventureModeType::Adventure].IsValid() == false)
		return;

	CurAdventureSlotMap[EAdventureModeType::Adventure]->SetLastStageDifficulty(Difficulty);
}

void UB2UIAdventureModSelect::SetAdventureModLock(bool IsUnLock, bool IsUnLockAnimation)
{	
	if (CurAdventureSlotMap.Contains(EAdventureModeType::Adventure) == false ||
		CurAdventureSlotMap[EAdventureModeType::Adventure].IsValid() == false)
		return;

	// 언락 애니메이션은 아직...(IsUnLockAnimation)
	CurAdventureSlotMap[EAdventureModeType::Adventure]->SetLockSlot(!IsUnLock);
	CurAdventureSlotMap[EAdventureModeType::Adventure]->SetVisiblityAdventureDifficultyButton(IsUnLock);
}

void UB2UIAdventureModSelect::SetScenarioComplete(bool IsComplete)
{
	if (CurAdventureSlotMap.Contains(EAdventureModeType::Scenario) == false ||
		CurAdventureSlotMap[EAdventureModeType::Scenario].IsValid() == false)
		return;

	CurAdventureSlotMap[EAdventureModeType::Scenario]->SetIsCompleteScenarioMode(IsComplete);
}

void UB2UIAdventureModSelect::UpdateAdventureChapterInfo()
{
	// 모험난이도 오픈가능한 스테이지 안내문 설정
	UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo();
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	if (ChapterInfoTable && StageInfoTable)
	{
		int32 MaxChapterNum = StageInfoTable->GetStartAdventureModeChapterNum();
		int32 MaxSubChapterNum = ChapterInfoTable->GetStagesNumInChapter(MaxChapterNum);

		if (CurAdventureSlotMap.Contains(EAdventureModeType::Adventure) && CurAdventureSlotMap[EAdventureModeType::Adventure].IsValid())
			CurAdventureSlotMap[EAdventureModeType::Adventure]->SetNoticeText(EAdventureModeType::Adventure, MaxChapterNum, MaxSubChapterNum);
	}

}


void UB2UIAdventureModSelect::UpdateScenarioChapterInfo()
{	
	if (auto DocChapter = Cast<UB2UIDocChapter>(GetDoc()))
	{
		int32 NextChapterNum = DocChapter->GetNextChapterNum();
		int32 NextSubChapterNum = DocChapter->GetNextSubChapterNum();		
		if (CurAdventureSlotMap.Contains(EAdventureModeType::Scenario) && CurAdventureSlotMap[EAdventureModeType::Scenario].IsValid())
			CurAdventureSlotMap[EAdventureModeType::Scenario]->SetNoticeText(EAdventureModeType::Scenario, NextChapterNum, NextSubChapterNum);
	}
}

void UB2UIAdventureModSelect::UpdateChapterLock(EStageDifficulty InStageDifficulty)
{
	if (InStageDifficulty == EStageDifficulty::ESD_None)
	{
		if (auto DocChapter = Cast<UB2UIDocChapter>(GetDoc()))
			InStageDifficulty = DocChapter->GetMaxOpenedStageDifficulty();
	}

	if (InStageDifficulty == EStageDifficulty::ESD_None)
		return;
	
	bool IsCompleteScenario = false;
	bool IsAdventureUnLock = false;
	bool IsAdventureUnLockAni = false;
	GetAdventureLockState(InStageDifficulty, IsCompleteScenario, IsAdventureUnLock, IsAdventureUnLockAni);

	SetScenarioComplete(IsCompleteScenario);
	SetAdventureModLock(IsAdventureUnLock, IsAdventureUnLockAni);
	SetAdventureDifficulty(InStageDifficulty);
}

void UB2UIAdventureModSelect::GetAdventureLockState(EStageDifficulty InLastDifficulty, bool& OutCompleteScenario, bool& OutAdventureUnLock, bool& OutAdventureUnLockAni)
{	
	OutCompleteScenario = false;
	OutAdventureUnLock = false;
	OutAdventureUnLockAni = false;

	if (InLastDifficulty == EStageDifficulty::ESD_None)
		return;

	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	if (StageDataStore.IsClearedStartAdventureModeClientStageId(EStageDifficulty::ESD_Normal))
	{
		OutAdventureUnLock = true;
		if (StageDataStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Normal))
			OutCompleteScenario = true;
	}
	else
	{
		if (InLastDifficulty == EStageDifficulty::ESD_Normal)
		{
			if (StageDataStore.IsClearedStartAdventureModeClientStageId(EStageDifficulty::ESD_Normal, true))
			{
				OutAdventureUnLock = true;
				OutAdventureUnLockAni = true;
			}
		}
	}
}

void UB2UIAdventureModSelect::InitAdventureModeSlot()
{
	// 0. Check BluePrint Data
	if (InitAdventureModeSlot_CheckBluePrint() == false)
		return;

	// 1. Set Select AdventureSlot
	ShowAdventureSlotTypes.Sort();
	InitAdventureModeSlot_SetCurAdventureSlot();

	// 2. Init Widget
	for (auto& ThisSlot : CurAdventureSlotMap)
	{
		if (ThisSlot.Value.IsValid())
		{
			ThisSlot.Value->Init();
			ThisSlot.Value->FOnClickBTNSelect.BindUObject(this, &UB2UIAdventureModSelect::ClickAdventureBTNSelect);
			ThisSlot.Value->FOnClickBTNSub.BindUObject(this, &UB2UIAdventureModSelect::ClickAdventureBTNSelectSub);
			ThisSlot.Value->FOnClickBTNLock.BindUObject(this, &UB2UIAdventureModSelect::ClickAdventureBTNLock);
		}
	}

	// 3. Setting BluePrint Data To SlotData
	for (auto& ThisSlot : CurAdventureSlotMap)
	{
		if (ThisSlot.Value.IsValid())
		{
			FAdventureModeSetting SettingData = GetAdventureModeSetting(ThisSlot.Key);
			if (SettingData.ModeType != EAdventureModeType::Max)
				ThisSlot.Value->SetAdventureModeInfo(SettingData);
		}
	}

	// 4. StartBeginAnimation_BP
	StartBeginAnimation_BP(ShowAdventureSlotTypes.Num());
}

bool UB2UIAdventureModSelect::InitAdventureModeSlot_CheckBluePrint()
{
	// 1. Check ShowAdventureSlotTypes
	if (ShowAdventureSlotTypes.Num() != 2 && ShowAdventureSlotTypes.Num() != 3)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("UI_AdventureModSelect.uasset 에러 발생 / ShowAdventureSlotTypes 블루프린트를 설정해주세요.(주의 : 사이즈가 2 또는 3이 되어야함)"))
			));
#endif //WITH_EDITOR
		BII_CHECK(false);
		return false;
	}

	// 2. Check AdventureModeTypeDatas
	if (AdventureModeTypeDatas.Num() == 0)
	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("AdventureModeSlot 블루프린터 데이터 없음. UI_AdventureModSelect에 데이터(AdventureModeTypeDatas)를 추가해주세요."))
//			));
//#endif //WITH_EDITOR
		return false;
	}

	// 3. GDC 버전 슈퍼 하드코딩!!!!!! (todo : 이건 나중에 필히 지워주도록 한다. 혹시 내가 까먹을 수도 있으니 나중에 보는 사람이 있다면 지워주시길... by spivy)
	{
		bool IsGDCVersion = ShouldShowGDCVersionContent(); // [GDC] Feb 2017
		if (IsGDCVersion)
		{
			// GDC버전에는 튜토리얼과 시나리오모드만 오픈합니다요 ~
			ShowAdventureSlotTypes.Empty();
			ShowAdventureSlotTypes.Emplace(EAdventureModeType::Tutorial);
			ShowAdventureSlotTypes.Emplace(EAdventureModeType::Scenario);
		}
	}

	return true;
}

void UB2UIAdventureModSelect::InitAdventureModeSlot_SetCurAdventureSlot()
{
	// Note : 기획 의도에 따라 슬롯 개수가 자주 변경될 수가 있다. (마일스톤, GDC 등등..)
	const int32 MaxShowSlotCount = ShowAdventureSlotTypes.Num();

	TWeakObjectPtr<UCanvasPanel> CurRootObject = nullptr;
	TWeakObjectPtr<UCanvasPanel> HiddenRootObject = nullptr;
	TArray<TWeakObjectPtr<class UB2UIAdventureModSlot>>* ChildObjectList = nullptr;

	CurRootObject = (MaxShowSlotCount == 2) ? CP_TwoDummySlot : CP_ThreeDummySlot;
	HiddenRootObject = (MaxShowSlotCount == 2) ? CP_ThreeDummySlot : CP_TwoDummySlot;
	ChildObjectList = (MaxShowSlotCount == 2) ? &TwoDummyChildList : &ThreeDummyChildList;

	if (CurRootObject == nullptr || ChildObjectList == nullptr || HiddenRootObject == nullptr)
		return;

	if (ChildObjectList->Num() != MaxShowSlotCount)
		return;

	if (CurRootObject.IsValid())
		CurRootObject->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (HiddenRootObject.IsValid())
		HiddenRootObject->SetVisibility(ESlateVisibility::Hidden);

	// Set CurAdventureSlotMap!
	CurAdventureSlotMap.Empty();
	for (int32 i = 0; i < ShowAdventureSlotTypes.Num(); ++i)
	{
		if (ShowAdventureSlotTypes.IsValidIndex(i) == false || 
			ChildObjectList->IsValidIndex(i) == false)
			continue;

		CurAdventureSlotMap.Add(ShowAdventureSlotTypes[i], (*ChildObjectList)[i]);
	}
}

void UB2UIAdventureModSelect::ClickAdventureBTNSelect(EAdventureModeType AdventureModeType)
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (DocChapter == nullptr)
		return;

	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	switch (AdventureModeType)
	{
	case EAdventureModeType::Tutorial:
		GoToTutorialClass<bool>::GetInstance().Signal(true);
		break;
	case EAdventureModeType::Scenario:
		StageDataStore.RequestGetActInfoAndChangeChapter(DocChapter->GetCurChapterNum(), EStageDifficulty::ESD_Normal);
		break;
	case EAdventureModeType::Adventure:
		StageDataStore.RequestGetActInfoAndChangeChapter(DocChapter->GetCurChapterNum(), DocChapter->GetSelectHardStageDifficulty());
		break;
	}
}

void UB2UIAdventureModSelect::ClickAdventureBTNSelectSub(EAdventureModeType AdventureModeType)
{
	switch (AdventureModeType)
	{
	case EAdventureModeType::Tutorial:
		break;
	case EAdventureModeType::Scenario:
		break;
	case EAdventureModeType::Adventure:
		UB2UIManager::GetInstance()->OpenUI(UIFName::SelectAdventureDifficulty);
		break;
	}
}

void UB2UIAdventureModSelect::ClickAdventureBTNLock(EAdventureModeType AdventureModeType)
{
	switch (AdventureModeType)
	{
	case EAdventureModeType::Tutorial:
		break;
	case EAdventureModeType::Scenario:
		break;
	case EAdventureModeType::Adventure:
		ABladeIIGameMode::SetHUDCenterMessage(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("AdventureMod_HardOpenMessage")), 1.0f);
		break;
	}
}

FAdventureModeSetting UB2UIAdventureModSelect::GetAdventureModeSetting(EAdventureModeType InAdventureModeType)
{
	if (InAdventureModeType == EAdventureModeType::Max)
		return FAdventureModeSetting();

	// 몇개안되니까 이터레이팅..
	for (auto AdventureModeSettingData : AdventureModeTypeDatas)
	{
		if (AdventureModeSettingData.ModeType == InAdventureModeType)
			return AdventureModeSettingData;
	}

	return FAdventureModeSetting();
}

void UB2UIAdventureModSelect::OnChangeDifficulty(class UB2UIDocBase* Sender, EStageDifficulty NewDifficulty, EStageDifficulty PrevDiff)
{
	UpdateChapterLock(NewDifficulty);
}

void UB2UIAdventureModSelect::OnChangeChapterNum(class UB2UIDocBase* Sender, int32 ChapterNum, int32 PreChapterNum)
{
	UpdateScenarioChapterInfo();
	UpdateChapterLock();
}
