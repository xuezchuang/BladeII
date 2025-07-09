// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2RichTextBlock.h"
#include "QuestDataTable.h"
#include "QuestManager.h"
#include "B2UIQuestDialog.h"
#include "B2UIQuestSlot.h"


#include "BladeIIUtil.h"

#include "BladeIIGameImpl.h"

UB2UIQuestSlot::UB2UIQuestSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CurrentSlotState = EQuestSlotState::QuestHideAll;
	QuestSlot = 0;
	bInitQuest = false;

	QuestGoalCount = 1;
	CurrentProgressCount = 0;

	CompressTextLimitLength = 10;

	QuestProgressMaterial = nullptr;
}

void UB2UIQuestSlot::Init()
{
	Super::Init();
}

void UB2UIQuestSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIQuestSlot::SetSlotHideTimer()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC && SlotHideTime > 0.f)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(SlotHideTimer);
		OwningPC->GetWorldTimerManager().SetTimer(SlotHideTimer, this, &UB2UIQuestSlot::OnHideQuestSlotByTimer, SlotHideTime, false);
	}
}

void UB2UIQuestSlot::OnHideQuestSlotByTimer()
{
	if (CurrentSlotState == EQuestSlotState::QuestOpen)
		SetQuestSlotState(EQuestSlotState::QuestSimple);
}

void UB2UIQuestSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_QuestSlot);
	GET_SLOT(UImage, IMG_NPCICon);

	GET_SLOT(UImage, IMG_QuestProgress);
	
	GET_SLOT(UB2RichTextBlock, RTB_QuestDistrict);
	GET_SLOT(UB2RichTextBlock, RTB_QuestSummary);
	GET_SLOT(UB2RichTextBlock, RTB_QuestProgress);
	GET_SLOT(UB2RichTextBlock, RTB_LeftTime);

	GET_SLOT(UB2Button, BTN_QuestDetailLink);
	GET_SLOT(UB2Button,BTN_AreaQuest);

	GET_SLOT(UCanvasPanel, CP_QuestState);
	GET_SLOT(UCanvasPanel, CP_QuestNew);
	GET_SLOT(UCanvasPanel, CP_QuestComplete);
	GET_SLOT(UCanvasPanel, Slide);

	GET_SLOT(UPanelWidget, P_MainQuestEffect);
	GET_SLOT(UWidgetAnimation, ANIM_MainQ);
}

void UB2UIQuestSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_AreaQuest, &UB2UIQuestSlot::OnClickBtn_AreaQuest);
	BIND_CLICK_FUNC_TO_BTN(BTN_QuestDetailLink, &UB2UIQuestSlot::OnClickBtn_QuestDetailLink);
}

void UB2UIQuestSlot::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(), &UB2UIQuestSlot::AlarmCondition_QuestState);
}

void UB2UIQuestSlot::OnClickBtn_AreaQuest()
{

	switch (CurrentQuestState)
	{
	case EQuestState::InProgress:
	case EQuestState::New :
	case EQuestState::Complete:
		{
			auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
			if (QuestDialogUI != nullptr)
			{
				QuestDialogUI->OpenDialogBySlotIndex(QuestSlot);
				return;
			}
		}
		break;
	default:
		// 뭔가 ?
			break;
	}

	SetSlotHideTimer();
}

void UB2UIQuestSlot::OnClickBtn_QuestDetailLink()
{
	auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
	if (QuestDialogUI != nullptr)
	{
		QuestDialogUI->OpenDialogBySlotIndex(QuestSlot);
		return;
	}
}

bool UB2UIQuestSlot::AlarmCondition_QuestState()
{
	return CurrentQuestState == EQuestState::New ? true : false;
}

void UB2UIQuestSlot::SetQuestSlotState(EQuestSlotState NewSlotState)
{
	DoMarkRedDot();

	if (CurrentSlotState == NewSlotState)
		return;

	switch (NewSlotState)
	{
	case EQuestSlotState::QuestOpen :
		SetVisibility_QuestSlot(true);
		if (BTN_QuestDetailLink.IsValid()) BTN_QuestDetailLink->SetVisibility(ESlateVisibility::Visible);
		break;

	case EQuestSlotState::QuestSimple :
	case EQuestSlotState::QuestDefault :
		SetVisibility_QuestSlot(true);
		if (BTN_QuestDetailLink.IsValid()) BTN_QuestDetailLink->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EQuestSlotState::QuestHideAll :
		SetVisibility_QuestSlot(false); 
		break;

	default: return;
	}

	OnQuestSlotStateChanged(NewSlotState);
	CurrentSlotState = NewSlotState;
}

void UB2UIQuestSlot::SetVisibility_QuestSlot(bool bVisible)
{
	if (CP_QuestSlot.IsValid())
		CP_QuestSlot->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UB2UIQuestSlot::UpdateProgressMaterial(float CurrentProgress)
{
	if (IMG_QuestProgress.IsValid())
		QuestProgressMaterial = IMG_QuestProgress->GetDynamicMaterial();

	if (QuestProgressMaterial)
	{
		QuestProgressMaterial->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, CurrentProgress);
	}
}

void UB2UIQuestSlot::UpdateQuestDetailLeftTime(int32 TimeValue)
{
	FText LeftTimeFormat = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_LeftTime"));
	FText LeftTime = FText::FromString(UB2UIManager::FormatSecondsAsTime(TimeValue, TEXT("hh:mm")));

	if (RTB_LeftTime.IsValid())
		RTB_LeftTime->SetText(FText::Format(LeftTimeFormat, LeftTime));
}

void UB2UIQuestSlot::UpdateQuestProgress(float CurrentProgress)
{
	UpdateProgressMaterial(CurrentProgress);
}

void UB2UIQuestSlot::GetVisualStateFromQuest(const int32 QuestState, EQuestSlotState& OutSlotState, EQuestState& OutQuestState)
{
	switch (QuestState)
	{
	case EQS_NOT_ACCEPTED:
		OutQuestState = EQuestState::New;
		OutSlotState = EQuestSlotState::QuestDefault;
		break;
	case EQS_PROGRESS:
		OutQuestState = EQuestState::InProgress;
		OutSlotState = EQuestSlotState::QuestOpen;
		break;

	case EQS_COMPLETED:
		OutQuestState = EQuestState::Complete;
		OutSlotState = EQuestSlotState::QuestDefault;
		break;

	default: 
		OutQuestState = EQuestState::None;
		OutSlotState = EQuestSlotState::QuestHideAll;
		break;
	}
}

void UB2UIQuestSlot::InitQuestDefault(int32 QuestSlotIndex, bool bDisplayOnly)
{
	QuestPtr TargetQuest = QuestManager::GetInstance().GetQuest(QuestSlotIndex);
	if (TargetQuest == nullptr)
		return;

	// DisplayOnly 상태는 강제로 Progress 상태의 Visual을 보여준다.
	const int32 QuestState = bDisplayOnly ? EQS_PROGRESS : TargetQuest->GetState();
	if (QuestState == EQS_REWARDED)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;		// 보상 완료된 상태는 노출하지 않음
	}
	SetVisibility(ESlateVisibility::Visible);

	QuestSlot = QuestSlotIndex;
	bInitQuest = true;

	EQuestSlotState SlotState = EQuestSlotState::QuestHideAll;
	GetVisualStateFromQuest(QuestState, SlotState, CurrentQuestState);
	UpdateQuestMaterial(CurrentQuestState);

	// 퀘스트 설명 Text 업데이트
	GenerateQuestInfo(TargetQuest);

	// 1 메인 2 서브 ect... 10 일퀘
	if (QuestSlot == 1)
	{
		SetMainQuestEffect(TargetQuest);
		MainQuestActFilter(TargetQuest->GetQuestId());
	}

	// Daily Quest 전용 Material 설정
	if (TargetQuest->GetType() == DAILY_QUEST)
	{
		SetMaterialDailyQuest();
		if (RTB_LeftTime.IsValid())
			RTB_LeftTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UpdateQuestDetailLeftTime(10000); // 일단 임시로 - 퀘스트 시간이 업데이트 되면 그때 Dynamic하게 수정
	}
	else
	{
		if (RTB_LeftTime.IsValid())
			RTB_LeftTime->SetVisibility(ESlateVisibility::Collapsed);
	}

	QuestGoalCount = QuestManager::GetInstance().GetQuestGoalCount(TargetQuest->GetQuestId(), TargetQuest->GetType());
	TargetQuest->GetProgressCount() > QuestGoalCount ? (CurrentProgressCount = QuestGoalCount) : (CurrentProgressCount = TargetQuest->GetProgressCount());

	float ProgressPercentage = (QuestGoalCount > 0) ? (static_cast<float>(CurrentProgressCount) / static_cast<float>(QuestGoalCount)) : 0.f;
	UpdateQuestProgress(ProgressPercentage);

	SetQuestSlotState(SlotState);

	if (bDisplayOnly)
	{
		if(BTN_AreaQuest.IsValid())
			BTN_AreaQuest->SetVisibility(ESlateVisibility::Collapsed);

		if (BTN_QuestDetailLink.IsValid())
			BTN_QuestDetailLink->SetVisibility(ESlateVisibility::Collapsed);

		if (CP_QuestState.IsValid())
			CP_QuestState->SetVisibility(ESlateVisibility::Collapsed);
	}


	SetSlotHideTimer();
}

void UB2UIQuestSlot::GenerateQuestInfo(QuestPtr TargetQuest)
{

	UB2QuestInfoTable* QuestTable = StaticFindQuestTable();
	FQuestCommonInfo* QuestInfo = QuestTable != nullptr ? QuestTable->GetQuestInfo(TargetQuest) : nullptr;
	if (QuestInfo)
	{
		FText OrgQuestText = BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestInfo->OutLine));

		TArray<FText> ParsedTextList;
		ParseTextFromSpecialChar(OrgQuestText, TEXT("/div"), ParsedTextList);

		if (RTB_QuestDistrict.IsValid() && ParsedTextList.IsValidIndex(0))
		{
			RTB_QuestDistrict->SetText(ParsedTextList[0]);
		}

		if (RTB_QuestSummary.IsValid() && ParsedTextList.IsValidIndex(1))
		{
			FText CompressedText = GetCompressedText(ParsedTextList[1], CompressTextLimitLength);
			RTB_QuestSummary->SetText(CompressedText);
		}

		if (RTB_QuestProgress.IsValid())
		{
			FText ProgressText = GetProgressText(TargetQuest);
			RTB_QuestProgress->SetText(ProgressText);
		}

		int32 NPCIndex = QuestInfo->StartNPC % 100000;
		UpdateNPCMaterial(NPCIndex);
	}
}


void UB2UIQuestSlot::UpdateQuestMaterial(EQuestState QuestState)
{
	ESlateVisibility StateVisibility = ESlateVisibility::Collapsed;

	if (QuestState == EQuestState::New || QuestState == EQuestState::Complete)
	{
		if (CP_QuestState.IsValid())
			CP_QuestState->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (Slide.IsValid())
			Slide->SetVisibility(ESlateVisibility::Collapsed);

		if (QuestState == EQuestState::New)
		{
			if (CP_QuestNew.IsValid())
				CP_QuestNew->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (CP_QuestComplete.IsValid())
				CP_QuestComplete->SetVisibility(ESlateVisibility::Collapsed);
		}

		else if (QuestState == EQuestState::Complete)
		{
			if (CP_QuestComplete.IsValid())
				CP_QuestComplete->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (CP_QuestNew.IsValid())
				CP_QuestNew->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	else if (QuestState == EQuestState::InProgress)
	{
		if (CP_QuestState.IsValid())
			CP_QuestState->SetVisibility(ESlateVisibility::Collapsed);

		if (Slide.IsValid())
			Slide->SetVisibility(ESlateVisibility::HitTestInvisible);

	}

}

void UB2UIQuestSlot::UpdateNPCMaterial(int32 NPCIndex)
{
	if (IMG_NPCICon.IsValid())
	{
		TSoftObjectPtr<UMaterialInterface>* FoundAssetPtr = NPCIconMaterials.Find(NPCIndex);
		if (FoundAssetPtr)
		{
			if (UMaterialInterface* IconMaterial = (*FoundAssetPtr).LoadSynchronous())
			{
				IMG_NPCICon->SetBrushFromMaterial(IconMaterial);
			}
		}
	}
}

void UB2UIQuestSlot::MainQuestActFilter(int32 InQuestID)
{
	SetVisibility(ESlateVisibility::Visible);
	b2network::B2mdMainQuestInfoPtr MainQuestMasterData = QuestManager::GetInstance().GetMainQuestMasterInfo(InQuestID);

	if (MainQuestMasterData == nullptr)
		return;

	if (MainQuestMasterData->main_view_condition == 0)  //main_view_condition ==0일때는 거른다
		return;

	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();
	FServerStageID LastServerStageID;

	for (int32 i = 0; i < static_cast<int32>(EStageDifficulty::ESD_End); ++i)								//가장큰 ServerStageID를 찾는다 노말 Act6은 5-10지옥보다 StageID가 더 크다
	{
		FServerStageID DifficultyStageID = StageStore.GetLastClearServerStageId(static_cast<EStageDifficulty>(i));

		if (LastServerStageID < DifficultyStageID)
			LastServerStageID = DifficultyStageID;
	}

	SetVisibility(LastServerStageID.Id >= MainQuestMasterData->main_view_condition ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);


}

void UB2UIQuestSlot::SetMainQuestEffect(QuestPtr InTargetQuest)
{
	if (InTargetQuest->GetType() == MAIN_QUEST)
	{
		if (P_MainQuestEffect.IsValid())
			P_MainQuestEffect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (ANIM_MainQ.IsValid())
			PlayAnimation(ANIM_MainQ.Get(), 0.0f, 0 );
	}
}