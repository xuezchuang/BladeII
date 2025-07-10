// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialManager.h"

#include "Event.h"
#include "Retailer.h"
#include "OnlineWholesaler.h"
#include "CharacterDataStore.h"
//#include "B2StageDataStore.h"
#include "B2StageManager.h"
//#include "B2UIManager.h"
//#include "B2UITutorial.h"
//#include "B2UIMsgPopupReward.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"
#include "B2LobbyGameMode.h"
//#include "B2UIManager_Lobby.h"
//#include "B2UILobbyMain.h"

//#include "B2UILinkManager.h"
//#include "B2HighlightWidget.h"
//#include "B2UIManager_InGameCombat.h"

#include "BladeIIGameImpl.h"


UB2TutorialInfo::UB2TutorialInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TutorialInfoTable(nullptr)
	, TutorialStepTable(nullptr)
{

}

const FTutorialInfo* UB2TutorialInfo::GetTutorialInfo(int32 TutorialID)
{
	if(TutorialID <= 0)
	{ return nullptr; }

	// Device에서 TutorialInfoTable을 초기화 호출 시점에 제대로 Load하지 못하는 문제가 있음
	// 일단 여기서 처리로 타이밍 이슈인지 확실히 한 후 진행
	if (TutorialInfoTable == nullptr)
		LoadTutorialInfoTable();

	if(TutorialInfoTable)
		return TutorialInfoTable->FindRow<FTutorialInfo>(GetTutorialInfoName(TutorialID), TEXT(""));

	return nullptr;
}

const FTutorialStep* UB2TutorialInfo::GetTutorialStep(int32 TutorialID, int32 TutorialStep)
{
	// Device에서 TutorialInfoTable을 초기화 호출 시점에 제대로 Load하지 못하는 문제가 있음
	// 일단 여기서 처리로 타이밍 이슈인지 확실히 한 후 진행
	if (TutorialInfoTable == nullptr)
		LoadTutorialInfoTable();
	if (TutorialStepTable == nullptr)
		LoadTutorialStepTable();

	if (const FTutorialInfo* TutorialInfo = GetTutorialInfo(TutorialID))
	{
		const TArray<FTutorialStep>& TutorialStepList = TutorialInfo->StepList;
		if (TutorialStepList.IsValidIndex(TutorialStep))
		{
			return &(TutorialStepList[TutorialStep]);
		}
	}

	return nullptr;
}

void UB2TutorialInfo::GenerateTutorialSteps()
{
	if (TutorialInfoTable && TutorialStepTable)
	{
		if (bGeneratedStepList)
		{
			TArray<FName> TutorialKeys = TutorialInfoTable->GetRowNames();
			for (const FName& TutorialKey : TutorialKeys)
			{
				if (FTutorialInfo* TutorialInfo = TutorialInfoTable->FindRow<FTutorialInfo>(TutorialKey, TEXT("")))
				{
					TutorialInfo->StepList.Empty();
				}
			}
		}

		TArray<FName> StepNameKeys = TutorialStepTable->GetRowNames();
		for (const FName& StepNameKey : StepNameKeys)
		{
			if (FTutorialStep* TutorialStep = TutorialStepTable->FindRow<FTutorialStep>(StepNameKey, TEXT("")))
			{
				TutorialStep->SetStepBasicInfoFromKey(StepNameKey);
				FName TutorialInfoKey = GetTutorialInfoName(TutorialStep->TutorialID);
				if (FTutorialInfo* TutorialInfo = TutorialInfoTable->FindRow<FTutorialInfo>(TutorialInfoKey, TEXT("")))
				{
					TutorialInfo->StepList.Add(*TutorialStep);
				}
			}
		}

		bGeneratedStepList = true;
	}
}

void UB2TutorialInfo::InitTutorialInfo()
{
	LoadTutorialInfoTable();
	LoadTutorialStepTable();
	LoadTutorialControlBotInfos();
}

void UB2TutorialInfo::LoadTutorialInfoTable()
{
	if (TutorialInfoTable == nullptr)
	{
		FString TutorialInfoTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2TutorialInfo"), TEXT("TutorialInfoTable"), TutorialInfoTablePath, GGameIni);
		TutorialInfoTable = LoadObject<UDataTable>(nullptr, *TutorialInfoTablePath);
	}
}

void UB2TutorialInfo::LoadTutorialStepTable()
{
	if (TutorialStepTable == nullptr)
	{
		FString TutorialStepTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2TutorialInfo"), TEXT("TutorialStepTable"), TutorialStepTablePath, GGameIni);
		TutorialStepTable = LoadObject<UDataTable>(nullptr, *TutorialStepTablePath);
	}
}

void UB2TutorialInfo::LoadTutorialControlBotInfos()
{
	//if (TutorialControlBotInfos == nullptr)
	//{
	//	TutorialControlBotInfos = StaticFindTutorialControlBotInfos();
	//}
}

const TArray<FTutorialControlBotInfo>* UB2TutorialInfo::GetTutorialControlBotInfos()
{
	if (TutorialControlBotInfos == nullptr)
	{
		LoadTutorialControlBotInfos();
	}

	if (TutorialControlBotInfos)
	{		
		return &(TutorialControlBotInfos->TutorialControlBotInfos);
	}

	return nullptr;
}

TutorialManager::TutorialManager()
	: bTestTutorialAvailable(true)
	, CurrentTutorialID(INDEX_NONE)
	, CurrentTutorialStep(INDEX_NONE)
	, LastClearTutorialID(INDEX_NONE)
	, bTutorialAvailable(true)
	, bSubscribed(false)
	, TutorialInfoStore(nullptr)
	, bIsSkipNewTutorial(false)
{

	m_epcRequestPrepareClass = EPCClass::EPC_Gladiator;

	bHighlightRenderCheck = true;

	eNeedToCachingResponse = ENeedToCachingResponse::None;
}

TutorialManager::~TutorialManager()
{
}

TutorialManager& TutorialManager::GetInstance()
{
	static TutorialManager Instance;
	return Instance;
}

void TutorialManager::Init()
{
	if (TutorialInfoStore == nullptr)
	{
		TutorialInfoStore = NewObject<UB2TutorialInfo>(GetTransientPackage(), FName(TEXT("TutorialTable")), RF_MarkAsRootSet);
		TutorialInfoStore->AddToRoot();
		check(TutorialInfoStore);
	}

	CachingTableInit();

	if (TutorialInfoStore)
	{
		TutorialInfoStore->GenerateTutorialSteps();
	}

	SubscribeEvents();
	if (bSubscribed == false)
	{
		RegisterExceptionFunc();
		bSubscribed = true;
	}

	InitHardCodingProperties();
	SetCurrentTutorialID(INDEX_NONE);
	SetCurrentTutorialStep(INDEX_NONE);
}

void TutorialManager::CachingTableInit()
{
	if(TutorialInfoStore)
		TutorialInfoStore->InitTutorialInfo();
}

void TutorialManager::SetTestTutorial(int32 TestTutorialID)
{
	InitHardCodingProperties();

	SetLastClearTutorialID((TestTutorialID != INDEX_NONE) ? GetPrevTutorialID(TestTutorialID) : INDEX_NONE);
	SetCurrentTutorialID(INDEX_NONE);
	SetCurrentTutorialStep(INDEX_NONE);
}

bool TutorialManager::IsLockTutorial(const int32 TutorialID, const int32 TutorialStep)
{
	// test
	if (TutorialManager::GetInstance().bTestTutorialAvailable == false)
		return false;

	if (TutorialManager::GetInstance().IsTutorialAvailable() == false)
		return false;

	const int32 LastClearId = TutorialManager::GetInstance().GetLastClearTutorialID();
	const int32 CurrTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	const int32 CurrTutorialStep = TutorialManager::GetInstance().GetCurrentTutorialStep();
	
	if (LastClearId != INDEX_NONE)
	{
		if (LastClearId >= TutorialID)
			return false;
	}

	if (CurrTutorialID != INDEX_NONE && CurrTutorialStep != INDEX_NONE)
	{
		if (TutorialID == CurrTutorialID)
		{
			if (TutorialStep <= CurrTutorialStep)
				return false;
		}
	}

	return true;
}

bool TutorialManager::IsCurrentTutorial(const int32 TutorialID, const int32 TutorialStep)
{
	if (TutorialManager::GetInstance().IsTutorialAvailable() == false)
		return false;

	const int32 CurrTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	const int32 CurrTutorialStep = TutorialManager::GetInstance().GetCurrentTutorialStep();

	if (CurrTutorialID == INDEX_NONE || CurrTutorialStep == INDEX_NONE)
		return false;

	return (TutorialID == CurrTutorialID && TutorialStep == CurrTutorialStep);
}

bool TutorialManager::IsCurrentTutorial(const int32 TutorialID)
{
	if (TutorialManager::GetInstance().IsTutorialAvailable() == false)
		return false;

	const int32 CurrTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();

	if (CurrTutorialID == INDEX_NONE)
		return false;

	return TutorialID == CurrTutorialID;
}

bool TutorialManager::IsFinishTutorial(const int32 TutorialID)
{
	const int32 LastTutorialID = TutorialManager::GetInstance().GetLastClearTutorialID();

	return TutorialID <= LastTutorialID;
}

void TutorialManager::OpenLockContentPopup(const int32 TutorialID, const int32 TutorialStep)
{
	//버튼 영역 겹침 문제로 튜토리얼 스텝 진행 중에는 팝업 안열림 처리
	if (TutorialManager::GetInstance().HasAnyTutorial())
		return;

	//if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	//{
	//	FText TutorialTitle = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialCondition_OpenCondition_ColorSize")), GetTutorialTitle(TutorialID));
	//	FString DescString = TutorialTitle.ToString();
	//	DescString.Append(TEXT("\n\n"));
	//	DescString.Append(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_ContentOpenCondition")).ToString());
	//	DescString.Append(GetTutorialDesc(TutorialID).ToString());

	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		FText::FromString(DescString),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::GotoLastChapter,
	//		FMsgPopupOnClick::CreateLambda([]() {
	//		GoGameStageInfoLastReachedFromLobbyClass<>::GetInstance().Signal();
	//		TutorialManager::GetInstance().CloseLobbyMainCombineMenu();
	//	})
	//	);
	//}
}

bool TutorialManager::CheckOpenLockContentPopup(const int32 LockTutorialID)
{
	bool bCheck = false;

	TutorialManager& TutorialMgr = TutorialManager::GetInstance();

	if (LockTutorialID && TutorialMgr.IsLockTutorial(LockTutorialID, 0))
	{
		bCheck = true;
		TutorialMgr.OpenLockContentPopup(LockTutorialID, 0);
	}

	return bCheck;
}

bool TutorialManager::IsValidTutorialSet()
{
	if (GetLastClearTutorialID() == INDEX_NONE && GetCurrentTutorialID() == INDEX_NONE)
	{
		return false;
	}

	return true;
}

void TutorialManager::SetTutorialMasterData(const TArray<b2network::B2mdTutorialInfoPtr>& TutorialMasterDatas)
{
	ServerTutorialMasterDatas = TutorialMasterDatas;
}

void TutorialManager::SubscribeEvents()
{
	/*UnsubscribeEvents();

	Issues.Add(NotifyUIButtonClickedClass<UButton*>::GetInstance().Subscribe2(
		[this](UButton* ClickedButton)
	{
		this->TutorialStep_OnClickedButton(ClickedButton);
	}
	));

	Issues.Add(UIWidgetOpenCompleteClass<FName>::GetInstance().Subscribe2(
		[this](const FName& OpenedUIName)
	{
		this->TutorialStep_OnOpenUI(OpenedUIName);
	}
	));

	Issues.Add(OnClearStageClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnStageClear();
	}
	));
	
	Issues.Add(ProcessLobbyTutorialClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->ExternalProcessTutorial();
	}
	));
	
	Issues.Add(DeliveryGetTutorialProgressClass<FB2ResponseGetTutorialProgressPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseGetTutorialProgressPtr TutorialProgress)
	{
		this->OnGetTutorialProgress(TutorialProgress);
	}
	));

	Issues.Add(DeliverySetTutorialProgressClass<FB2ResponseSetTutorialProgressPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseSetTutorialProgressPtr TutorialProgress)
	{
		this->OnSetTutorialProgress(TutorialProgress);
	}
	));

	Issues.Add(DeliveryUpdateTutorialProgressClass<FB2ResponseUpdateTutorialProgressPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseUpdateTutorialProgressPtr TutorialProgress)
	{
		this->OnUpdateTutorialProgress(TutorialProgress);
	}
	));

	Issues.Add(DeliverySkipTutorialProgressClass<FB2ResponseSkipTutorialProgressPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseSkipTutorialProgressPtr TutorialProgress)
	{
		this->OnSkipTutorialProgress(TutorialProgress);
	}
	));
	
	Issues.Add(DeliveryDrawTutorialItemClass<FB2ResponseDrawTutorialItemPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseDrawTutorialItemPtr TutorialItem)
	{
		this->OnDrawTutorialItem(TutorialItem);
	}
	));

	Issues.Add(DeliveryPrepareTutorialClass<FB2ResponsePrepareTutorialPtr>::GetInstance().Subscribe2(
		[this](FB2ResponsePrepareTutorialPtr PrepareInfo)
	{
		this->OnPrepareTutorial(PrepareInfo);
	}
	));
	
	Issues.Add(DeliveryGiveTutorialRewardClass<FB2ResponseGiveTutorialRewardPtr>::GetInstance().Subscribe2(
		[this](FB2ResponseGiveTutorialRewardPtr TutorialReward)
	{
		this->OnGiveTutorialReward(TutorialReward);
	}
	));
	
	Issues.Add(CahcedCheckAndProgressTutorialClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnCahcedCheckAndProgressTutorial();
	}
	));*/
}

void TutorialManager::UnsubscribeEvents()
{
	Issues.Empty();
}

bool TutorialManager::HasAnyTutorial()
{
	if (IsTutorialAvailable() == false)
		return false;

	// 현재 진행중인 튜토리얼이 있거나 진행 가능한 다음 튜토리얼이 있다면
	if (IsProcessingTutorial() || GetProcessableNextTutorial() != nullptr)
		return true;

	return false;
}

int32 TutorialManager::GetPrevTutorialID(int32 TutorialID)
{
	for (b2network::B2mdTutorialInfoPtr& ServerTutorialInfo : ServerTutorialMasterDatas)
	{
		if (ServerTutorialInfo && ServerTutorialInfo->tutorial_id == TutorialID)
		{
			return ServerTutorialInfo->prev_tutorial_id;
		}
	}

	return INDEX_NONE;
}

int32 TutorialManager::GetNextTutorialID(int32 TutorialID)
{
	for (b2network::B2mdTutorialInfoPtr& ServerTutorialInfo : ServerTutorialMasterDatas)
	{
		if (ServerTutorialInfo && ServerTutorialInfo->prev_tutorial_id == TutorialID)
		{
			return ServerTutorialInfo->tutorial_id;
		}
	}

	return INDEX_NONE;
}

const FTutorialInfo* TutorialManager::GetNextTutorialInfo()
{
	int32 LastTutorialID = GetLastClearTutorialID();
	int32 NextTutorialID = GetNextTutorialID(LastTutorialID);
	const FTutorialInfo* NextInfo = GetTutorialInfo(NextTutorialID);

	return NextInfo;
}

int32 TutorialManager::GetFirstTutorialID()
{
	if (ServerTutorialMasterDatas.IsValidIndex(0))
	{
		b2network::B2mdTutorialInfoPtr& BeginTutorialPtr = ServerTutorialMasterDatas[0];
		if (BeginTutorialPtr && GetPrevTutorialID(BeginTutorialPtr->tutorial_id) == INDEX_NONE)
			return BeginTutorialPtr->tutorial_id;
	}

	return 100;
}

const FTutorialInfo* TutorialManager::GetProcessableNextTutorial()
{
	// 1. 현재 진행중인 Tutorial이 없어야 함
	if (CurrentTutorialID == INDEX_NONE)
	{
		// 2. 다음 튜토리얼 정보가 유효해야 함
		if (const FTutorialInfo* TutorialInfo = GetNextTutorialInfo())
		{
			// 3. 시작 요구조건을 충족해야 함
			if (IsProcessableTutorial(TutorialInfo->TutorialID))
			{
				return TutorialInfo;
			}
		}
	}

	return nullptr;
}

const FTutorialInfo* TutorialManager::GetTutorialInfo(const int32 TutorialID)
{ 
	if (TutorialInfoStore == nullptr)
		return nullptr;

	return TutorialInfoStore->GetTutorialInfo(TutorialID); 
}
const FTutorialStep* TutorialManager::GetTutorialStepInfo(const int32 TutorialID, const int32 TutorialStep) 
{ 
	if (TutorialInfoStore == nullptr)
		return nullptr;

	return TutorialInfoStore->GetTutorialStep(TutorialID, TutorialStep); 
}

const FTutorialInfo* TutorialManager::GetCurrentTutorialInfo() 
{ 
	return GetTutorialInfo(CurrentTutorialID); 
}

const FTutorialStep* TutorialManager::GetCurrentStepInfo() 
{ 
	return GetTutorialStepInfo(CurrentTutorialID, CurrentTutorialStep); 
}

const FTutorialStep* TutorialManager::GetPrevStepInfo() 
{ 
	return GetTutorialStepInfo(CurrentTutorialID, CurrentTutorialStep - 1); 
}

ETutorialAction TutorialManager::GetPrevStepAction()
{ 
	return GetPrevStepInfo() ? GetPrevStepInfo()->Action : ETutorialAction::None; 
}

void TutorialManager::ExternalProcessTutorial()
{
	// Test Dummy Code
	CachingTableInit();

	if (IsProcessingTutorial() == false)
	{
		RequestUpdateTutorial();
	}
	else
	{
		ProcessStep();
	}
}

void TutorialManager::TutorialStep_OnOpenUI(const FName& OpenedUIName)
{
	//const FTutorialStep* CurrentStep = GetCurrentStepInfo();

	//if (CurrentStep == nullptr)
	//{ return; }

	//FName ConvertedUIName = OpenedUIName;
	//if (FB2UIWidgetData* WidgetData = UB2UIManager::GetInstance()->GetWidgetData(OpenedUIName))
	//	ConvertedUIName = WidgetData->WidgetAssetName;

	//if (CurrentStep->Requirement == ETutorialRequire::OpenUIComplete &&
	//	CurrentStep->RequireName == ConvertedUIName)
	//{
	//	SetTutorialStepState(ETutorialStepState::Passable);
	//	ProcessStep();
	//}
}

void TutorialManager::OnTutorialBeginUIClose(int32 TutorialID)
{
	if (const FTutorialStep* CurrentStep = GetCurrentStepInfo())
	{
		LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(false);
		SetTutorialStepState(ETutorialStepState::Passable);
		ProcessStep();
	}
}

void TutorialManager::OnTutorialRewardUIClose(int32 TutorialID)
{
	// Reward UI를 닫으면 다시 Update를 요청해서 새 Tutorial을 받아온다.
	OnTutorialEnd(TutorialID);

	RequestUpdateTutorial();

	bWaitForCompleteReward = false;
}

void TutorialManager::SkipCurrentTutorial()
{
	RequestSkipTutorial();
}

TArray<int32> TutorialManager::GetLobbyDisplayTutorials()
{
	// remember MAX_VISIBLE_CONTENTOPEN_COUNT == 2 and
	// this should be very very very very Serious Hardcoding

	const int32 NextTutorial1 = GetNextTutorialID(GetLastClearTutorialID());
	const int32 NextTutorial2 = GetNextTutorialID(NextTutorial1);

	TArray<int32> TempTutorialIDs;

	if (const FTutorialInfo* Tutorial1 = GetTutorialInfo(NextTutorial1))
	{
		if(Tutorial1->TutorialType == ETutorialType::ContentOpen)
			TempTutorialIDs.Add(NextTutorial1);
	}

	if (const FTutorialInfo* Tutorial2 = GetTutorialInfo(NextTutorial2))
	{
		if (Tutorial2->TutorialType == ETutorialType::ContentOpen)
			TempTutorialIDs.Add(NextTutorial2);
	}

	return TempTutorialIDs;
}

void TutorialManager::TutorialStep_OnClickedButton(UButton* ClickedButton)
{
	if (ClickedButton != nullptr)
	{
		if (const FTutorialStep* CurrentStep = GetCurrentStepInfo())
		{
			if (CurrentStep->Requirement == ETutorialRequire::ClickButton &&
				CurrentStep->RequireName == ClickedButton->GetFName())
			{
				SetTutorialStepState(ETutorialStepState::Passable);
				ProcessStep();
			}
		}
	}
}

void TutorialManager::ProcessStep()
{
	if (const FTutorialStep* CurrentStepInfo = GetCurrentStepInfo())
	{
		if (IsPassableStep(CurrentStepInfo))
		{
			// 이전 Step을 처리하는데 있어 따로 뺄 부분이 있을지 고민해보자
			if (const FTutorialStep* PrevStep = GetPrevStep(CurrentStepInfo))
			{
				if (PrevStep->Action == ETutorialAction::Highlight || PrevStep->Action == ETutorialAction::BlockHilight)
					ClearHighlightUI(PrevStep);
			}

			ProcessTutorialAction(CurrentStepInfo);
			OnTutorialStepClear(CurrentStepInfo);

			if (const FTutorialStep* NextStepInfo = GetNextStep(CurrentStepInfo))
			{
				// IsValid NextStepInfo - 다음 진행할 Step이 남아있다면
				OnTutorialStepBegin(NextStepInfo);
			}

		}
	}
}

const FTutorialStep* TutorialManager::GetNextStep(const FTutorialStep* CurrentStepInfo)
{
	if (CurrentStepInfo && CurrentStepInfo->TutorialStep == GetCurrentTutorialStep())
	{
		const int32 TutorialID = GetCurrentTutorialID();
		const int32 NextStepIndex = GetCurrentTutorialStep() + 1;
		if (const FTutorialStep* NextStepInfo = GetTutorialStepInfo(TutorialID, NextStepIndex))
		{
			return NextStepInfo;
		}
	}

	return nullptr;
}

const FTutorialStep* TutorialManager::GetPrevStep(const FTutorialStep* CurrentStepInfo)
{
	if (CurrentStepInfo && CurrentStepInfo->TutorialStep == GetCurrentTutorialStep())
	{
		const int32 TutorialID = GetCurrentTutorialID();
		const int32 PrevStepIndex = GetCurrentTutorialStep() - 1;
		if (const FTutorialStep* PrevStepInfo = GetTutorialStepInfo(TutorialID, PrevStepIndex))
		{
			return PrevStepInfo;
		}
	}

	return nullptr;
}

class AbsoluteSetnullptr
{
private:
	FB2ResponseGetTutorialProgressPtr* CheckPtr;
public:
	AbsoluteSetnullptr(FB2ResponseGetTutorialProgressPtr* _CheckPtr) { CheckPtr = _CheckPtr; }
	~AbsoluteSetnullptr()
	{
		if (*CheckPtr)
			*CheckPtr = nullptr;
	}
};

void TutorialManager::OnCahcedCheckAndProgressTutorial()
{
	if (IsTutorialAvailable() == false)
		return;

	if(eNeedToCachingResponse != ENeedToCachingResponse::Cahced)
	{
		eNeedToCachingResponse = ENeedToCachingResponse::NeedCahcing;
		RequestGetTutorial();
		return;
	}

	if (CachedResponse)
	{
		AbsoluteSetnullptr AbsoluteNull(&CachedResponse);

		const int32 ServerTutorialID = CachedResponse->tutorial_id;
		bool bTutorial_Finished = CachedResponse->is_finished;

		/*
		if (ServerTutorialID <= TutorialID_GDC && bTutorial_Finished == false)
		{
			RequestRewardTutorial();
			return;
		}
		*/

		if (bTutorial_Finished)
		{
			SetLastClearTutorialID(ServerTutorialID);
		}
		else
		{
			const int32 PrevTutorialID = GetPrevTutorialID(ServerTutorialID);
			SetLastClearTutorialID(PrevTutorialID);

			//새접속시 이전에 튜토리얼 아이템을 지급받고 강종한 상태라면 해당 튜토리얼 완료 처리
			if (CachedResponse->is_item_prepared || CachedResponse->is_lottery_drawn)
			{
				RequestRewardTutorial();
				SetCurrentTutorialID(ServerTutorialID);
				return;
			}
		}
	}

	if (const FTutorialStep* TutorialStep = GetCurrentStepInfo())
	{
		//최악의 경우 튜토리얼 중간 진행 상태로 로비에 왔을때 보상 요청
		if (TutorialStep->TutorialStep > 1 && TutorialStep->Action != ETutorialAction::TutorialClear)
			RequestRewardTutorial();
	}
	else
	{
		SetCurrentTutorialID(INDEX_NONE);
		SetCurrentTutorialStep(INDEX_NONE);
		ExternalProcessTutorial();	// 아마 자동적인 흐름으로 RequestUpdateTutorial을 하게될 것임
		RefreshContentLockUI();
	}
}

void TutorialManager::BeginTutorial(int32 TutorialID)
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::BeginTutorial"));

	if (const FTutorialInfo* NewTutorial = GetTutorialInfo(TutorialID))
	{
		B2_SCOPED_TRACK_LOG_L2(TEXT("TutorialManager::BeginTutorial - Begin Valid Tutorial"));

		OnTutorialBegin(TutorialID);
		SetTutorialStepState(ETutorialStepState::WaitForRequirement); // Waiting for TutorialBeginUI Close

		bHighlightRenderCheck = true;
	}
}

bool TutorialManager::IsPassableStep(const FTutorialStep* TutorialStepInfo)
{
	// Passable State 확인 및 !!!!!!! 하드코딩 한 조건을 만족하는지 체크 - 하드코딩 조건이 없으면 무조건 true 리턴
	if (GetTutorialStepState() == ETutorialStepState::Passable && ProcessExceptionRequire(TutorialStepInfo))
	{
		return true;
	}

	return false;
}

bool TutorialManager::IsProcessableTutorial(int32 TutorialID)
{
	const int32 NextTutorialID = GetNextTutorialID(LastClearTutorialID);
	if (NextTutorialID == TutorialID && IsSatisfiedRequirements(TutorialID))
		return true;

	return false;
}

bool TutorialManager::IsSatisfiedRequirements(int32 TutorialID)
{
	if (const FTutorialInfo* TutorialInfo = GetTutorialInfo(TutorialID))
	{
		if(IsSatisfiedOpenStage(TutorialInfo) && IsSatisfiedCharacterLevel(TutorialInfo))
		{
			return true;
		}
	}

	return false;
}

FServerStageID CheckAdventureStageID(FServerStageID ServerStageID)
{
	//int32 iConvertStageID = BladeIIGameImpl::GetStageDataStore().GetClientStageId(ServerStageID);
	//EStageDifficulty eConvertStageDifficulty = BladeIIGameImpl::GetStageDataStore().GetStageDifficulty(ServerStageID);

	//return BladeIIGameImpl::GetStageDataStore().GetServerStageID(iConvertStageID, eConvertStageDifficulty);
	return FServerStageID();
}

bool TutorialManager::IsSatisfiedOpenStage(const FTutorialInfo* TutorialInfo)
{
	//if (TutorialInfo)
	//{
	//	FStageDataStore& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	//	const FServerStageID RequireServerStageID = FServerStageID(TutorialInfo->RequireOpenStage);

	//	// Lobby에서 확인
	//	if (StageDataStore.IsStageClearedOnce(RequireServerStageID) || RequireServerStageID == 0)
	//		return true;

	//	// StageClear UI (In Game) 에서 확인
	//	const FServerStageID CurrClearStageID = CheckAdventureStageID(AB2StageManager::GetCacheStageKeepEssentialData().GetServerStageID());
	//	if ((RequireServerStageID < CurrClearStageID) || RequireServerStageID == CurrClearStageID)
	//		return true;

	//	// CachedActData가 없을수도 있으니 AccountInfo에서 확인
	//	const FServerStageID LastClearStageID = CheckAdventureStageID(StageDataStore.GetLastClearServerStageIdByLastPlayedStageDifficulty());
	//	if ((RequireServerStageID < LastClearStageID) || RequireServerStageID == LastClearStageID)
	//		return true;
	//}

	return false;
}

bool TutorialManager::IsSatisfiedCharacterLevel(const FTutorialInfo* TutorialInfo)
{
	if (TutorialInfo)
	{
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetLocalCharacterData();
		if (CharacterDataStore.GetUserLevel() >= TutorialInfo->RequireLevel)
		{
			return true;
		}
	}

	return false;
}

void TutorialManager::ProcessTutorialAction(const FTutorialStep* TutorialStepInfo)
{
	//if (TutorialStepInfo == nullptr)
	//	return;

	//switch (TutorialStepInfo->Action)
	//{
	//case ETutorialAction::TutorialBegin:
	//	if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//		TutorialUI->OpenTutorialBegin(CurrentTutorialID);
	//	break;

	//case ETutorialAction::Highlight:
	//case ETutorialAction::BlockHilight:
	//	ActivateHighlightUI(TutorialStepInfo);
	//	break;

	//case ETutorialAction::TutorialClear:
	//	if(GetNextStep(TutorialStepInfo) == nullptr)
	//		OnTutorialClear(CurrentTutorialID); // 현재는 바로 보여주지만 서버 연동 후 Clear Request 날리고 Response에서 UI보여준다
	//	break;
	//}

	//SetVisibleSkipButton(TutorialStepInfo->ViewSkipButton);

	//// 하드코딩한 Action도 실행한다.
	//ProcessExceptionAction(TutorialStepInfo);

	//ProcessNPCSoundAction(TutorialStepInfo);
}

bool TutorialManager::CheckAndBeginTutorial(int32 ServerTutorialID)
{
	if(bTestTutorialAvailable == false)	//튜토리얼 테스트 off중일땐 다음 튜토리얼 진행 안되도록 수정
	{ return false; }

	//FString ScopeString = FString::Printf(TEXT("TutorialManager::CheckAndBeginTutorial : ServerTutorialID [%d]"), ServerTutorialID);
	//B2_SCOPED_TRACK_LOG(*ScopeString);

	//// Dummy Code
	//CachingTableInit();	// 모바일디바이스에서 TableInfo가 로드되지 않는 경우가 존재
	//// Dummy Code

	//// 진행중인 Tutorial이 없고 유요한 TutorialID 이어야 함
	//if (IsProcessingTutorial() == false && GetTutorialInfo(ServerTutorialID) != nullptr)
	//{
	//	// 실행 가능하며 현재 UIScene이 LobbyMain이어야함
	//	if (IsProcessableTutorial(ServerTutorialID) && UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::LobbyMain)
	//	{
	//		BeginTutorial(ServerTutorialID);
	//		return true;
	//	}
	//}

	return false;
}

//class UB2UITutorial* TutorialManager::GetTutorialUI()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::GetTutorialUI"));
//
//	if (UB2UITutorial* TutorialUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorial>(UIFName::Tutorial))
//	{
//		return TutorialUI;
//	}
//	
//	return UB2UIManager::GetInstance()->OpenUI<UB2UITutorial>(UIFName::Tutorial);
//}

void TutorialManager::ActivateHighlightUI(const FTutorialStep* TutorialStep)
{
	if(TutorialStep == nullptr)
		return;

	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//{
	//	TutorialUI->ProcessStepHighlight(TutorialStep);
	//	LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(true);
	//}
}

void TutorialManager::ClearHighlightUI(const FTutorialStep* TutorialStep)
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//{
	//	TutorialUI->ClearStepHighlight(TutorialStep);
	//	LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(false);
	//}
}

void TutorialManager::SetVisibleSkipButton(bool bVisible)
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//{
	//	TutorialUI->SetVisibleSkipButton(bVisible);
	//}
}

bool TutorialManager::ProcessExceptionRequire(const FTutorialStep* StepInfo)
{
	if (StepInfo != nullptr)
	{
		if (const FStepExceptionFunc* ExecException = StepExceptionRequireExecs.Find(StepInfo->StepKey))
		{
			return ExecException->Execute(StepInfo);
		}
	}

	return true;	// 없으면 true
}

bool TutorialManager::ProcessExceptionAction(const FTutorialStep* StepInfo)
{
	if (StepInfo != nullptr)
	{
		if (const FStepExceptionFunc* ExecException = StepExceptionActionExecs.Find(StepInfo->StepKey))
		{
			return ExecException->Execute(StepInfo);
		}
	}

	return true;	// 없으면 true
}


bool TutorialManager::ProcessExceptionEnd(const FTutorialInfo* TutorialInfo)
{
	if (TutorialInfo != nullptr)
	{
		FName TutorialKey(*GetTutorialKey(TutorialInfo->TutorialID));
		if (const FTutorialExceptionFunc* ExecException = TutorialExceptionEndExecs.Find(TutorialKey))
		{
			return ExecException->Execute(TutorialInfo);
		}
	}

	return true;	// 없으면 true
}

void TutorialManager::OnGetTutorialProgress(FB2ResponseGetTutorialProgressPtr TutorialProgress)
{
	BII_CHECK(TutorialProgress);
	
	CachedResponse = TutorialProgress;

	int32 LastTutorialID = TutorialProgress->tutorial_id;

	if (TutorialProgress->is_finished == false)
		LastTutorialID = GetPrevTutorialID(TutorialProgress->tutorial_id);

	SetLastClearTutorialID(LastTutorialID);

	if (eNeedToCachingResponse == ENeedToCachingResponse::NeedCahcing)
	{
		eNeedToCachingResponse = ENeedToCachingResponse::Cahced;
		OnCahcedCheckAndProgressTutorial();
	}
		
	eNeedToCachingResponse = ENeedToCachingResponse::Cahced;

	//BladeIIGameImpl::GetClientDataStore().CheckBeginCombatTutorialorEntryMainLobby();
}

void TutorialManager::OnSetTutorialProgress(FB2ResponseSetTutorialProgressPtr TutorialProgress)
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::OnSetTutorialProgress"));

	BII_CHECK(TutorialProgress);

	if (TutorialProgress->is_finished == false)
	{
		const int32 ServerTutorialID = TutorialProgress->tutorial_id;
		SetLastClearTutorialID(GetPrevTutorialID(ServerTutorialID));
		CheckAndBeginTutorial(ServerTutorialID);
	}
}

void TutorialManager::OnUpdateTutorialProgress(FB2ResponseUpdateTutorialProgressPtr TutorialProgress)
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::OnUpdateTutorialProgress"));

	//BII_CHECK(TutorialProgress);
	//
	//const int32 ServerTutorialID = TutorialProgress->tutorial_id;
	//if (TutorialProgress->is_finished)
	//{
	//	OnTutorialEnd(ServerTutorialID);
	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::LobbyMain)
	//		RequestUpdateTutorial();
	//}
	//else
	//{
	//	SetLastClearTutorialID(GetPrevTutorialID(ServerTutorialID));
	//	CheckAndBeginTutorial(ServerTutorialID);
	//}

	//UpdateLobbyMainContentOpenSlot();
}

void TutorialManager::OnSkipTutorialProgress(FB2ResponseSkipTutorialProgressPtr TutorialProgress)
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::OnSkipTutorialProgress"));

	BII_CHECK(TutorialProgress);

	//if (TutorialProgress->is_finished)
	//{
	//	OnTutorialEnd(TutorialProgress->tutorial_id);
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::Tutorial);
	//	LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(false);
	//}

	//if (HasAnyTutorial())
	//	RequestUpdateTutorial();

	//CloseLobbyMainCombineMenu();

	//SkipTutorialRefreshUIClass<>::GetInstance().Signal();
}

void TutorialManager::OnDrawTutorialItem(FB2ResponseDrawTutorialItemPtr TutorialItem)
{
	BII_CHECK(TutorialItem);

	//auto DrawResponse = std::make_shared<b2network::FB2MessageInfoResponseDrawShopLottery>();
	//if (DrawResponse->IsSuccess())
	//{
	//	DrawResponse->free_lottery = TutorialItem->free_lottery;
	//	DrawResponse->items = TutorialItem->items;
	//	DrawResponse->gem = TutorialItem->gem;
	//	DrawResponse->social_point = TutorialItem->social_point;
	//	DrawResponse->mileage = TutorialItem->mileage;
	//	DrawResponse->next_free_time = TutorialItem->next_free_time;
	//	DrawResponse->daily_lottery_count = TutorialItem->daily_lottery_count;
	//}

	//DeliveryDrawShopLotteryClass<FB2ResponseDrawShopLotteryPtr>::GetInstance().Signal(DrawResponse);
}

void TutorialManager::OnPrepareTutorial(FB2ResponsePrepareTutorialPtr PrepareInfo)
{
	BII_CHECK(PrepareInfo);

	//if (UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser())
	//	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, PrepareInfo->current_gold);

	//switch (GetCurrentTutorialID())
	//{
	//case TutorialID_ItemLevelup:
	//	PrePareTutorialID_ItemLevelup();
	//	break;
	//case TutorialID_ItemEnhance:
	//	PrePareTutorialID_ItemEnhance();
	//	break;
	//case TutorialID_Ether:
	//	PrePareTutorialID_Ether();
	//	break;
	//case TutorialID_Seal:
	//	PrePareTutorialID_Seal();
	//	break;
	//}
}

void TutorialManager::OnGiveTutorialReward(FB2ResponseGiveTutorialRewardPtr TutorialReward)
{
	BII_CHECK(TutorialReward);

	//const int32 RewardTutorialID = GetCurrentTutorialID();
	//FString ScopeString = FString::Printf(TEXT("TutorialReward : CurrID [%d]"), RewardTutorialID);
	//B2_SCOPED_TRACK_LOG(*ScopeString);

	//if (RewardTutorialID == TutorialID_GDC || RewardTutorialID == -1)
	//{
	//	// 전투 튜토리얼 일 경우 캐싱한 튜토리얼 정보 갱신 요청
	//	RequestGetTutorial();
	//}
	//else
	//{
	//	if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//		TutorialUI->OpenTutorialRewardUI(RewardTutorialID, TutorialReward->tutorial_rewards);
	//}
}

void TutorialManager::RequestGetTutorial()
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::RequestGetTutorial"));

	data_trader::Retailer::GetInstance().RequestGetTutorialProgress();
}

void TutorialManager::RequestUpdateTutorial()
{
	if (GetProcessableNextTutorial())
	{
		B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::RequestUpdateTutorial"));

		data_trader::Retailer::GetInstance().RequestUpdateTutorialProgress();
	}
}

void TutorialManager::RequestRewardTutorial()
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::RequestTutorialReward"));

	data_trader::Retailer::GetInstance().RequestGiveTutorialReward();

	bWaitForCompleteReward = true;
}

void TutorialManager::RequestSkipTutorial()
{
	B2_SCOPED_TRACK_LOG(TEXT("TutorialManager::RequestSkipTutorial"));

	data_trader::Retailer::GetInstance().RequestSkipTutorialProgress();
}

//#include "B2UITutorialLockContents.h"

void TutorialManager::RefreshContentLockUI()
{
	//if (UB2UILobbyMain* LobbyMainUI = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain))
	//{
	//	TArray<UWidget*> AllWidgets;
	//	LobbyMainUI->WidgetTree->GetAllWidgets(AllWidgets);

	//	for (UWidget* ChildWidget : AllWidgets)
	//	{
	//		if (UB2UITutorialLockContents* LockUI = Cast<UB2UITutorialLockContents>(ChildWidget))
	//		{
	//			LockUI->Init();
	//		}
	//	}
	//}
}

void TutorialManager::RefreshTutorialTestUI()
{
	//UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	//if (pUIManager)
	//{
	//	pUIManager->OpenUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//}
}

void TutorialManager::OnAttendanceUIClose()
{
	if(bTutorialAvailable)
		RequestGetTutorial();
}

/*
	SetTutorialProgress ServerCommand는 개발 버전(서버)에서만 사용 가능! 절대 엄수
*/
void TutorialManager::ForceBeginTutorial(int32 TutorialID, bool bFinish)
{
#if WITH_EDITOR
	SetTestTutorial(TutorialID);
	FString CommandString = FString::Printf(TEXT("SetTutorialProgress %d %d %d %d"), TutorialID, 0, 0, static_cast<int32>(bFinish));
	data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*CommandString);
#endif
}

// Tutorial Hardcoding - 로직이 하드코딩으로 바인딩 되어야 는 부분들


void TutorialManager::OnStageClear()
{
	// 현재 Clear한 StageID는 GetProcessableNextTutorial 내부에서 확인
	if (IsFirstStageClear(TutorialID_FirstStageClear, EStageDifficulty::ESD_Normal) ||
		IsFirstStageClear(TutorialID_ExpertDungon, EStageDifficulty::ESD_Hard))
	{
		bStage1Clear = true;
		bHasContentOpenTutorial = true;
	}

	if (const FTutorialInfo* NextTutorialInfo = GetProcessableNextTutorial())
	{
		if (NextTutorialInfo->TutorialType == ETutorialType::ContentOpen)
		{
			bHasContentOpenTutorial = true;
		}
	}
}

void TutorialManager::OnClickHighlightRect()
{
	if (const FTutorialStep* CurrentStep = GetCurrentStepInfo())
	{
		if (CurrentStep->Requirement == ETutorialRequire::ClickHighlight)
		{
			SetTutorialStepState(ETutorialStepState::Passable);
			ProcessStep();
		}
	}
}

void TutorialManager::OpenDialogContentsOpen()
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//{
	//	TutorialUI->OpenDialogContentsOpenUI();

	//	bHasContentOpenTutorial = false;
	//}
}

void TutorialManager::UpdateLobbyMainContentOpenSlot()
{
	//if (UB2UILobbyMain* LobbyMainUI = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain))
	//	LobbyMainUI->UpdateLobbyContentOpenSlots();
}

void TutorialManager::ProcessNPCSoundAction(const FTutorialStep* StepInfo)
{
	//if (StepInfo == nullptr)
	//	return;

	//USoundCue* pNPCSound = StepInfo->SoundAssetPtr.LoadSynchronous();

	//if (pNPCSound)
	//{
	//	if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//		UGameplayStatics::PlaySound2D(TutorialUI->GetWorld(), pNPCSound);
	//}
}

b2network::B2mdTutorialInfoPtr TutorialManager::GetServerTutorialInfo(int32 TutorialID)
{
	for (b2network::B2mdTutorialInfoPtr& ServerTutorialInfo : ServerTutorialMasterDatas)
	{
		if (ServerTutorialInfo && ServerTutorialInfo->tutorial_id == TutorialID)
			return ServerTutorialInfo;
	}

	return nullptr;
}

FText TutorialManager::GetTutorialConditionText(int32 TutorialID)
{
	//auto* TutorialInfo = GetTutorialInfo(TutorialID);
	//if (TutorialInfo == nullptr)
	//	return FText();

	//switch (TutorialInfo->TutorialType)
	//{
	//case ETutorialType::ContentOpen:
	//	{
	//		FString ConditionText;
	//		if (TutorialInfo->RequireOpenStage > 0)
	//		{
	//			FServerStageID ConvertServerStageId(TutorialInfo->RequireOpenStage);
	//			int32 ActId = BladeIIGameImpl::GetStageDataStore().GetActIdByServerStageId(ConvertServerStageId);
	//			int32 SubChapterNum = BladeIIGameImpl::GetStageDataStore().GetSubChapterNumByServerStageId(ConvertServerStageId);

	//			ConditionText += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialCondition_OpenStage")), FText::FromString(FString::Printf(TEXT("%d-%d"), ActId, SubChapterNum))).ToString();
	//		}

	//		if (TutorialInfo->RequireLevel > 0)
	//		{
	//			if (ConditionText.IsEmpty() == false)
	//				ConditionText.Append(TEXT("\n"));

	//			ConditionText += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialCondition_Level")), TutorialInfo->RequireLevel).ToString();
	//		}

	//		return FText::FromString(ConditionText);
	//	}
	//	break;
	//	// 기획의도로 인해 ETutorialType::Default일 경우 TutorialDesc를 보여준다.
	//case ETutorialType::Default:
	//	return GetTutorialDesc(TutorialID);
	//}
	//
	return FText();
}

const TArray<FTutorialControlBotInfo>* TutorialManager::GetTutorialControlBotInfos()
{
	if (TutorialInfoStore)
		return TutorialInfoStore->GetTutorialControlBotInfos();

	return nullptr;
}

void TutorialManager::OnTutorialBegin(int32 TutorialID)
{
	InitHardCodingProperties();

	SetCurrentTutorialID(TutorialID);
	SetCurrentTutorialStep(0);

	//if (UB2UIManager* UIMgr = UB2UIManager::GetInstance())
	//{
	//	UIMgr->CloseAllStandaloneUIs();
	//}

	//CloseLobbyMainCombineMenu();

	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//{
	//	TutorialUI->OpenTutorialBegin(TutorialID);
	//	LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(true);
	//}
}


void TutorialManager::OnTutorialEnd(int32 TutorialID)
{
	const FTutorialInfo* TutorialInfo = GetTutorialInfo(TutorialID);
	ProcessExceptionEnd(TutorialInfo);

	//ClearHighlightUI(GetCurrentStepInfo());

	SetLastClearTutorialID(TutorialID);
	SetCurrentTutorialID(INDEX_NONE);
	SetCurrentTutorialStep(INDEX_NONE);

	UpdateLobbyMainContentOpenSlot();
}

void TutorialManager::OnTutorialClear(int32 TutorialID)
{
	ClearHighlightUI(GetCurrentStepInfo());
	RequestRewardTutorial();

	bHighlightRenderCheck = false;
}

void TutorialManager::OnTutorialStepBegin(const FTutorialStep* BeginStepInfo)
{
	if (BeginStepInfo != nullptr)
	{
		FString ScopeString = FString::Printf(TEXT("TutorialManager::OnTutorialStepBegin [ID : %d , Step : %d]"), BeginStepInfo->TutorialID, BeginStepInfo->TutorialStep);

		B2_SCOPED_TRACK_LOG(*ScopeString);

		SetCurrentTutorialStep(BeginStepInfo->TutorialStep);
		if (BeginStepInfo->Requirement != ETutorialRequire::None)
		{
			SetTutorialStepState(ETutorialStepState::WaitForRequirement);
		}

		ProcessStep();
	}
}

void TutorialManager::OnTutorialStepClear(const FTutorialStep* ClearStepInfo)
{
	if (ClearStepInfo && ClearStepInfo->Action == ETutorialAction::TutorialClear)
	{
		SetTutorialStepState(ETutorialStepState::WaitForRequirement); // Process Step불가능하게 막음
	}
}

FText GetTutorialTitle(int32 TutorialID)
{
	FString KeyString = FString::Printf(TEXT("TutorialTitle%d"), TutorialID);

	return BladeIIGetLOCText(B2LOC_CAT_TUTORIAL, KeyString);
}

FText GetTutorialDesc(int32 TutorialID)
{
	FString KeyString = FString::Printf(TEXT("TutorialDesc%d"), TutorialID);

	return BladeIIGetLOCText(B2LOC_CAT_TUTORIAL, KeyString);
}

FText GetTutorialStepDialog(int32 TutorialID, int32 TutorialStep)
{
	return BladeIIGetLOCText(B2LOC_CAT_TUTORIAL, GetTutorialStepKey(TutorialID, TutorialStep));
}

FString GetTutorialStepKey(int32 TutorialID, int32 TutorialStep)
{
	return FString::Printf(TEXT("TutorialStep%d_%d"), TutorialID, TutorialStep);
}

FString GetTutorialKey(int32 TutorialID)
{
	return FString::Printf(TEXT("Tutorial%d"), TutorialID);
}

//FString GetWidgetName(UB2UnitedWidgetBase* TargetWidget)
//{
//	FString WidgetName(TEXT("Empty"));
//	
//	if (TargetWidget)
//	{
//		// 일반 UIData 일 경우
//		if (FB2UIWidgetData* WidgetData = UB2UIManager::GetInstance()->GetWidgetDataByWidget(Cast<UB2UIWidget>(TargetWidget)))
//		{
//			WidgetName = WidgetData->WidgetFName.ToString();
//		}
//		// DJLegacy 일 경우
//		else if (UBladeIIUserWidget* DJLegacyWidget = Cast<UBladeIIUserWidget>(TargetWidget))
//		{
//			if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(TargetWidget)))
//			{
//				if (UB2UIManager_Lobby* LobbyUIManager = LobbyGM->DJLegacy_GetLobbyUIManager())
//				{
//					WidgetName = LobbyUIManager->GetDJLegacyUIName(DJLegacyWidget).ToString();
//				}
//			}
//		}
//	}
//
//	return WidgetName;
//}
//
//FName GetContentLockKey(UB2UnitedWidgetBase* TargetWidget, UButton* LockButton)
//{
//	if (TargetWidget && LockButton)
//	{
//		static FString NameFormat(TEXT("%s_%s"));
//
//		FString ContentLockKey = FString::Printf(*NameFormat, *GetWidgetName(TargetWidget), *LockButton->GetName());
//		return FName(*ContentLockKey);
//	}
//
//	return FName(TEXT("Empty"));
//}

UTexture2D* GetTutorialIconTexture(int32 TutorialID)
{
	if (const FTutorialInfo* TutorialInfo = TutorialManager::GetInstance().GetTutorialInfo(TutorialID))
	{
		TSoftObjectPtr<UTexture2D> IconTexture = TutorialInfo->TutorialTexture;
		return IconTexture.LoadSynchronous();
	}

	return nullptr;
}

UMaterialInterface* GetTutorialSpeakerMaterial(int32 TutorialID)
{
	if (const FTutorialInfo* TutorialInfo = TutorialManager::GetInstance().GetTutorialInfo(TutorialID))
	{
		TSoftObjectPtr<UMaterialInterface> SpekerMaterial = TutorialInfo->SpeakerMaterial;
		return SpekerMaterial.LoadSynchronous();
	}

	return nullptr;
}

void TutorialManager::RegisterExceptionFunc()
{
	//처음 스테이지 클리어
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 1);	// Action1 Chapter에서 Stage 1 - 1 입장 전 Scene이 Swipe방지 및 Highlight 재배치
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 2);	// Action2	Chapter에서 Block된 Swipe해제
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_FirstStageClear, 8);	// Require8 - Stage1 종료 후 로비로 돌아와서 정상 Clear 혹은 실패/취소 확인
	REGISTERTUTORIAL_END_EXCEPTION(TutorialID_FirstStageClear);		// TutorialEnd - 01 에서 Skip해버릴 경우를 위해 DocChapter Swipe false로 해제

	//장비 레벨업
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 0);		// Action0 - 장비 레벨업 전 재료용 Item 및 골드 Request	
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 3);		// Action4 - 400_5에서 재료 등록을 할때 B2lobbyInventory::SelectForIngred 에서 Signal날릴 Event를 Subscribe
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemLevelup, 4);		// Require5 - 장비 레벨업용 재료가 적절하게 등록되었는지 확인
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 4);		// Action5 - 장비 레벨업 가격을 0으로 수정하고 튜토리얼 레벨업 전용 프로토콜 준비 - 레벨업 버튼 눌렀을때 기능 바꿔치기, 400_4 에서 Subscribe했던 Event Unsubscribe
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 5);		// Action6 - 하이라이트 캐쉬 위젯 클리어

	//반격 던전
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 3);	// Action3 - 반격던전 해당 요일에 맞게 Highlight 변경
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 4);	// Action4 - 일반적인 Highlight Step이 아니므로 DungeonSlot UI 캐싱 후 강제로 ClearHighlight 호출

	//점령전
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_InitAndCenterCamera);	// Action - 인게임 첫액션 카메라 옮기고 배틀메인숨기고 뭐 막해봄
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_BotCamera);			// Action - 카메라 옮기기. 다옮기면 게임모드에서 스탭 넘겨준다.
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_TopCamera);			// Action - 카메라 옮기기. 다옮기면 게임모드에서 스탭 넘겨준다.
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_IngameCamera);			// Action - 카메라 인게임 복구.
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_SetIngame);			// Action - 게임 계속할수있는 상태로 UI랑 게임모드 설정해줌

	//장비 강화
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 0);		// Action0 - 튜토리얼 진행 전 강화 준비 프로토콜 보냄
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 1);		// Require2 - 입장 전 Main캐릭터 무기착용 여부 확인,(주문서, 무기가 4캐릭 다 없을경우 더미무기도) 서버 요청, Response받으면 true
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 5);		// Action6 - 캐쉬 하이라이트 위젯 활성화
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 6);		// Require7 - 강화용 ItemDetailPopup이 정상적으로 열렸는지 확인
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 6);		// Action7 - 강화 버튼 누르기 전 강화 Protocol Subscribe
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 7);		// Require8 - 강화 Protocol이 제대로 Response되었는지 확인
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 7);		// Action8 - TutorialUI Hide 시키고 1100_7에서 강화 프로토콜 subscribe를 unsubscribe

	//정예 던전
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 3);		// Action2 Chapter에서 Stage 1 - 1 입장 전 Scene이 Swipe방지 및 Highlight 재배치
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 4);		// Action3	Chapter에서 Block된 Swipe해제
	REGISTERTUTORIAL_END_EXCEPTION(TutorialID_ExpertDungon);		// TutorialEnd - 01 에서 Skip해버릴 경우를 위해 DocChapter Swipe false로 해제

	//에테르
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Ether, 0);				// Action0 - 에테르 튜토리얼용 아이템 Request
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Ether, 13);			// Action13 - 에테르 장착 애니메이션 작동 중 튜토리얼 위젯 hide

	//각성
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Seal, 0);				// Action0 - 에테르 튜토리얼용 아이템 Request

	//악몽 던전	//기획 요청 제거(기능은 남겨달라고 하심)
	//REGISTERSTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 2);		// Action2 Chapter에서 Stage 1 - 1 입장 전 Scene이 Swipe방지 및 Highlight 재배치
	//REGISTERSTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 3);		// Action3	Chapter에서 Block된 Swipe해제
	//REGISTERTUTORIAL_END_EXCEPTION(TutorialID_NightmareDungeon);		// TutorialEnd - 01 에서 Skip해버릴 경우를 위해 DocChapter Swipe false로 해제
}

void FTutorialStep::SetStepBasicInfoFromKey(const FName& InStepKey)
{
	FString KeyString;
	InStepKey.ToString(KeyString);
	StepKey = InStepKey;

	static FString StaticStepKey(TEXT("TutorialStep"));

	FString LeftStr, RightStr;
	if (KeyString.Split(TEXT("_"), &LeftStr, &RightStr))
	{
		TutorialStep = FCString::Atoi(*RightStr);
		FString LeftTutorialIDStr = LeftStr.Mid(StaticStepKey.Len());
		TutorialID = FCString::Atoi(*LeftTutorialIDStr);
	}
}

void TutorialManager::CloseLobbyMainCombineMenu()
{
	//CloseCombineMenuClass<>::GetInstance().Signal();
}

// 하드코딩인걸 티내기 위해 보기 구리게 만듬

void TutorialManager::InitHardCodingProperties()
{
	bHasContentOpenTutorial = false;

	//CachedUIPStage1 = nullptr;

	bStage1Clear = false;
	LobbyTryUpdateItemOPMainMenuTicket = 0;
	EnterItemLevelupModeClassTicket = 0;

	CachedEnhanceWeapon.Empty();
	IsEquippedWeapon = false;

	//CachedDungeonSlot = nullptr;

	bEnhanceIngredItemRegistered = false;

	DeliveryEnhanceItemTicket = 0;
	bItemEnhancedSucessfully = false;

	//CachedUIItemIcon = nullptr;
}

// Action - Tutorial100_1 Chapter에서 Stage 1 - 1 입장 전 Scene이 Swipe되는 것을 방지하기 위함
//#include "B2UIChapter.h"
//#include "B2UIChapterFrame.h"
//#include "B2UIChapterStage.h"
DEFINESTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 1)
{
	ChapterWidgetActiveHiglight(true);

	return false;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 2)
{
	ChapterWidgetActiveHiglight(false);

	return false;
}

bool TutorialManager::IsFirstStageClear(int32 iTutorialID, EStageDifficulty eStageDifficulty)
{
	//const int32 CurrTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();

	//if (CurrTutorialID != iTutorialID)
	//	return false;

	//FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();

	//const FServerStageID FirstServerStageID = StageStore.GetServerStageID(1, eStageDifficulty); // 1-1
	//FServerStageID LastClearServerStageID = StageStore.GetLastClearServerStageId(eStageDifficulty);

	//if (StageStore.GetClientStageId(LastClearServerStageID) == StageStore.GetClientStageId(FirstServerStageID))
	//	return true;
	//
	return false;
}

// Require - Tutorial100_8 - Stage1 종료 후 로비로 돌아와서 정상 Clear 혹은 실패/취소 확인
DEFINESTEP_REQUIRE_EXCEPTION(TutorialID_FirstStageClear, 8)
{
	return bStage1Clear;
}
DEFINETUTORIAL_END_EXCEPTION(TutorialID_FirstStageClear)
{
	ChapterWidgetActiveHiglight(false);

	return false;
}

void TutorialManager::OnClickSummonItem()
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//	TutorialUI->HideAllTutorialUI();
}

#define InvenIconHighlightWidth 148
#define InvenIconHighlightHeight 178
//
void TutorialManager::SetCachedUIItemIcon(UB2UnitedWidgetBase* pItemIcon)
{
	if (CachedUIItemIcon != nullptr)
		return;

	CachedUIItemIcon = pItemIcon;
	CachedUIItemIcon->AddActiveHighlight(FVector2D(InvenIconHighlightWidth, InvenIconHighlightHeight));
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 0)
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	EPCClass MainClass = CharData.GetMainPlayerClass();

	m_epcRequestPrepareClass = MainClass;

	data_trader::Retailer::GetInstance().RequestPrepareTutorial(CliToSvrPCClassType(MainClass));

	return true;
}

// 400-5 에서 진행할 재료 등록Step용 Event를 subscribe
DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 3)
{
	//LobbyTryUpdateItemOPMainMenuTicket = LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Subscribe([this] {this->OnItemIngredRegistered(); });
	return true;
}

void TutorialManager::OnItemIngredRegistered()
{
	bIngredItemRegistered = true;
	//SetTutorialStepState(ETutorialStepState::Passable);
	//ProcessStep();
}

DEFINESTEP_REQUIRE_EXCEPTION(TutorialID_ItemLevelup, 4)
{
	return bIngredItemRegistered;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 4)
{
	//if(CachedUIItemIcon)
	//	CachedUIItemIcon->AddActiveHighlight(FVector2D(InvenIconHighlightWidth, InvenIconHighlightHeight));

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Unsubscribe(LobbyTryUpdateItemOPMainMenuTicket);
	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 5)
{
	//if (CachedUIItemIcon)
	//	CachedUIItemIcon->ClearHighlight();

	return true;
}


void TutorialManager::OnItemLevelupClicked()
{
	ClearHighlightUI(GetCurrentStepInfo());
}

//#include "B2UIMainCounterAttackDG.h"
DEFINESTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 3)
{
	//if (UB2UIMainCounterAttackDG* DungeonUI = UB2UIManager::GetInstance()->GetUI<UB2UIMainCounterAttackDG>(UIFName::CounterAttackDGMain))
	//{
	//	if (UB2UnitedWidgetBase* DungeonSlot = DungeonUI->GetActiveDungeonWidgetPart())
	//	{
	//		DungeonSlot->ActiveHighlight(TEXT("TutorialStep500"));
	//		CachedDungeonSlot = DungeonSlot;

	//		return true;
	//	}
	//}
	
	return false;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 4)
{
	//if (CachedDungeonSlot)
	//{
	//	CachedDungeonSlot->ClearHighlight();
	//	CachedDungeonSlot = nullptr;
	//}

	return true;
}

//#include "B2UIBattleMain.h"
//#include "B2UIControlBattleSub.h"
//#include "B2ControlGameMode.h"
DEFINESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_InitAndCenterCamera)
{
	//UB2UIBattleMain* pBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);

	//if (pBattleMainUI)
	//{
	//	pBattleMainUI->SetVisibility(ESlateVisibility::Hidden);
	//}

	//UB2UIControlBattleSub* pControlBattleSubUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pControlBattleSubUI)
	//{
	//	pControlBattleSubUI->SetTutorialUIState(false);
	//	pControlBattleSubUI->SetControlNpcUIState(EControlNpcUiState::Live);
	//}
	//
	//if (GetTutorialUI())
	//{
	//	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(GetTutorialUI()));

	//	if (pGM)
	//	{
	//		pGM->ProcessTutorialAction(EControlTutorialActionStep::ViewCenterCamera);

	//		// 이동패널 숨김
	//		pGM->GetUIManager_InGameCombat()->HideVirtualJoystick(true);
	//	}
	//}
	
	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_BotCamera)
{
	//if (GetTutorialUI())
	//{
	//	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(GetTutorialUI()));

	//	if (pGM)
	//		pGM->ProcessTutorialAction(EControlTutorialActionStep::ViewSanctuaryBottomCamera);
	//}

	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_TopCamera)
{
//	if (GetTutorialUI())
//	{
//		AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(GetTutorialUI()));
//
//		if (pGM)
//			pGM->ProcessTutorialAction(EControlTutorialActionStep::ViewSanctuaryTopCamera);
//	}

	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_IngameCamera)
{
	//if (GetTutorialUI())
	//{
	//	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(GetTutorialUI()));

	//	if (pGM)
	//		pGM->ProcessTutorialAction(EControlTutorialActionStep::ViewIngameCamera);
	//}

 	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_SetIngame)
{
	//UB2UIBattleMain* pBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);

	//if (pBattleMainUI)
	//{
	//	pBattleMainUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//}

	//UB2UIControlBattleSub* pControlBattleSubUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pControlBattleSubUI)
	//{
	//	pControlBattleSubUI->SetTutorialUIState(true);
	//	pControlBattleSubUI->SetControlNpcUIState(EControlNpcUiState::None);
	//}

	//if (GetTutorialUI())
	//{
	//	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(GetTutorialUI()));

	//	if (pGM)
	//	{
	//		pGM->ProcessTutorialAction(EControlTutorialActionStep::SetIngame);

	//		// 이동패널	보임
	//		pGM->GetUIManager_InGameCombat()->HideVirtualJoystick(false);
	//	}
	//}

	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 0)
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	EPCClass MainClass = CharData.GetMainPlayerClass();

	m_epcRequestPrepareClass = MainClass;

	data_trader::Retailer::GetInstance().RequestPrepareTutorial(CliToSvrPCClassType(MainClass));

	data_trader::Retailer::GetInstance().RequestGetAccountItem(b2network::B2CharacterType::Common, b2network::B2InventoryType::Consumables);

	return true;
}

// Require - Tutorial1100_1 - 입장 전 Main캐릭터 무기착용 여부 확인,(주문서, 무기가 4캐릭 다 없을경우 더미무기도) 서버 요청, Response받으면 true
#include "CharacterDataStore.h"
#include "B2LobbyInventory.h"
DEFINESTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 1)
{
	return bReadyForEnhance;
}

bool TutorialManager::HasEquippedWeapon(EPCClass PCClass, FB2Item& OutWeaponItem)
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	TArray<FB2Item> EquippedItems;

	CharData.GetEquippedItems(PCClass, EquippedItems);
	for (FB2Item& Item : EquippedItems)
	{
		if (Item.ItemClass == EItemClass::EIC_Weapon)
		{
			OutWeaponItem = Item;
			return true;
		}
	}

	return false;
}

bool TutorialManager::HasInventoryWeapon(EPCClass PCClass, FB2Item& OutWeaponItem)
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	TArray<FB2Item> Weapons;

	UB2LobbyInventory::GetStoredItemListForEquipPlace(Weapons, EItemEquipPlace::EIEP_Weapon, PCClass);
	if (Weapons.IsValidIndex(0))
	{
		OutWeaponItem = Weapons[0];
		return true;
	}

	return false;
}

void TutorialManager::OnOpenItemDetailPopup()
{
	SetTutorialStepState(ETutorialStepState::Passable);
	ProcessStep();
}

DEFINESTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 6)
{
	return bEnhanceIngredItemRegistered;
}

void TutorialManager::OnEnhanceIngredRegistered(const FB2Item& IngredItem)
{
	bEnhanceIngredItemRegistered = true;
	SetTutorialStepState(ETutorialStepState::Passable);
	ProcessStep();
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 5)
{
	//if (CachedUIItemIcon)
	//	CachedUIItemIcon->AddActiveHighlight(FVector2D(InvenIconHighlightWidth, InvenIconHighlightHeight));

	return true;
}


DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 6)
{
	//if (CachedUIItemIcon)
	//	CachedUIItemIcon->ClearHighlight();

	DeliveryEnhanceItemTicket = DeliveryEnhanceItemClass<FB2EnhanceItem>::GetInstance().Subscribe([this](const FB2EnhanceItem& ServerEnhancedItem) {
		this->OnEnhancedItem(ServerEnhancedItem);
	});

	return true;
}

void TutorialManager::OnItemEnhancedClicked()
{
	ClearHighlightUI(GetCurrentStepInfo());
}

DEFINESTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 7)
{
	return bItemEnhancedSucessfully;
}

void TutorialManager::OnEnhancedItem(const FB2EnhanceItem& EnhancedItem)
{
	bItemEnhancedSucessfully = true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 7)
{
	DeliveryEnhanceItemClass<FB2EnhanceItem>::GetInstance().Unsubscribe(DeliveryEnhanceItemTicket);

	return true;
}

void TutorialManager::OnEnhanceDetailPopupClose()
{
	if (bItemEnhancedSucessfully)
	{
		SetTutorialStepState(ETutorialStepState::Passable);
		ProcessStep();
	}
}

void TutorialManager::ChapterWidgetActiveHiglight(bool bCheck)
{
	//if (bCheck == true)
	//{
	//	if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//	{
	//		UB2UIChapterFrame* ChapterFrame = UB2UIManager::GetInstance()->GetUI<UB2UIChapterFrame>(UIFName::Chapter);
	//		UB2UIChapter* Chapter1 = ChapterFrame ? ChapterFrame->GetCurChapter() : nullptr;

	//		if (UB2UIChapterStage* StageWidget = Chapter1->GetStageWidget(1))
	//		{
	//			StageWidget->AddActiveHighlight(FVector2D(90, 140));
	//			CachedUIPStage1 = StageWidget;
	//		}
	//	}
	//}
	//else
	//{
	//	if (CachedUIPStage1)
	//	{
	//		CachedUIPStage1->ClearHighlight();
	//		CachedUIPStage1 = nullptr;
	//	}
	//}
}

void TutorialManager::ActClearAnimStart()
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//	TutorialUI->ActiveTransparencyBlock(true);
}

void TutorialManager::ActClearAnimEnd()
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//	TutorialUI->ActiveTransparencyBlock(false);
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 3)
{
	ChapterWidgetActiveHiglight(true);

	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 4)
{
	ChapterWidgetActiveHiglight(false);

	return true;
}

bool TutorialManager::ExceptionTutorial_TutorialID_ExpertDungon_End(const FTutorialInfo* TutorialInfo)
{
	ChapterWidgetActiveHiglight(false);

	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_Ether, 0)
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	EPCClass MainClass = CharData.GetMainPlayerClass();

	data_trader::Retailer::GetInstance().RequestPrepareTutorial(CliToSvrPCClassType(MainClass));
	
	return true;
}

DEFINESTEP_ACTION_EXCEPTION(TutorialID_Ether, 13)
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//	TutorialUI->ActiveTransparencyBlock(true);

	return true;
}


DEFINESTEP_ACTION_EXCEPTION(TutorialID_Seal, 0)
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	EPCClass MainClass = CharData.GetMainPlayerClass();
	m_epcRequestPrepareClass = MainClass;

	data_trader::Retailer::GetInstance().RequestPrepareTutorial(CliToSvrPCClassType(MainClass));

	return true;
}

void TutorialManager::AtherEquipAnimEnd()
{
	//if (UB2UITutorial* TutorialUI = GetTutorialUI())
	//	TutorialUI->ActiveTransparencyBlock(false);
}

void TutorialManager::PrePareTutorialID_ItemLevelup()
{
	data_trader::Retailer::GetInstance().RequestGetAccountItem(CliToSvrPCClassType(GetTutorialRequestPrepareClass()), b2network::B2InventoryType::Weapon);
}

void TutorialManager::PrePareTutorialID_ItemEnhance()
{
	data_trader::Retailer::GetInstance().RequestGetAccountItem(CliToSvrPCClassType(GetTutorialRequestPrepareClass()), b2network::B2InventoryType::Weapon);
	data_trader::Retailer::GetInstance().RequestGetAccountItem(b2network::B2CharacterType::Common, b2network::B2InventoryType::Consumables);

	bReadyForEnhance = true;
	ProcessStep();

	//// Response 받는 시기가 UIOpen 이후일 수 있어 다시 한번 체크해줘야 함
	//if (GetTutorialUI()->GetCurrentDJLegacyUIName() == GetCurrentStepInfo()->RequireName)
	//{
	//	SetTutorialStepState(ETutorialStepState::Passable);
	//	ProcessStep();
	//}
}

void TutorialManager::PrePareTutorialID_Ether()
{
	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().MakeDirty();
}

void TutorialManager::PrePareTutorialID_Seal()
{
	FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	EPCClass MainClass = CharData.GetMainPlayerClass();
	data_trader::Retailer::GetInstance().RequestGetAccountItem(CliToSvrPCClassType(MainClass), b2network::B2InventoryType::Weapon);
}

//악몽 던전	//기획 요청 제거(기능은 남겨달라고 하심)
//DEFINESTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 2)
//{
//	ChapterWidgetActiveHiglight(true);
//
//	return true;
//}
//
//DEFINESTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 3)
//{
//	ChapterWidgetActiveHiglight(false);
//
//	return true;
//}
//
//bool TutorialManager::ExceptionTutorial_TutorialID_NightmareDungeon_End(const FTutorialInfo* TutorialInfo)
//{
//	ChapterWidgetActiveHiglight(false);
//
//	return true;
//}

bool TutorialManager::HighlightRenderCheck(int32 TutorialID)
{
	if (GetCurrentTutorialID() == TutorialID)
		return bHighlightRenderCheck;
	else
		return true;
}

bool TutorialManager::LockWidgetRedDotVisibleCheck(int32 LockTutorialID)
{
	if (IsTutorialAvailable() == false || GetLastClearTutorialID() >= LockTutorialID)
		return true;
	else
		return false;
}

void TutorialManager::GotoNextStep()
{
	SetTutorialStepState(ETutorialStepState::Passable);
	ProcessStep();
}
