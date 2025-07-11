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

	// Device���� TutorialInfoTable�� �ʱ�ȭ ȣ�� ������ ����� Load���� ���ϴ� ������ ����
	// �ϴ� ���⼭ ó���� Ÿ�̹� �̽����� Ȯ���� �� �� ����
	if (TutorialInfoTable == nullptr)
		LoadTutorialInfoTable();

	if(TutorialInfoTable)
		return TutorialInfoTable->FindRow<FTutorialInfo>(GetTutorialInfoName(TutorialID), TEXT(""));

	return nullptr;
}

const FTutorialStep* UB2TutorialInfo::GetTutorialStep(int32 TutorialID, int32 TutorialStep)
{
	// Device���� TutorialInfoTable�� �ʱ�ȭ ȣ�� ������ ����� Load���� ���ϴ� ������ ����
	// �ϴ� ���⼭ ó���� Ÿ�̹� �̽����� Ȯ���� �� �� ����
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
	//��ư ���� ��ħ ������ Ʃ�丮�� ���� ���� �߿��� �˾� �ȿ��� ó��
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

	// ���� �������� Ʃ�丮���� �ְų� ���� ������ ���� Ʃ�丮���� �ִٸ�
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
	// 1. ���� �������� Tutorial�� ����� ��
	if (CurrentTutorialID == INDEX_NONE)
	{
		// 2. ���� Ʃ�丮�� ������ ��ȿ�ؾ� ��
		if (const FTutorialInfo* TutorialInfo = GetNextTutorialInfo())
		{
			// 3. ���� �䱸������ �����ؾ� ��
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
	// Reward UI�� ������ �ٽ� Update�� ��û�ؼ� �� Tutorial�� �޾ƿ´�.
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
			// ���� Step�� ó���ϴµ� �־� ���� �� �κ��� ������ ����غ���
			if (const FTutorialStep* PrevStep = GetPrevStep(CurrentStepInfo))
			{
				if (PrevStep->Action == ETutorialAction::Highlight || PrevStep->Action == ETutorialAction::BlockHilight)
					ClearHighlightUI(PrevStep);
			}

			ProcessTutorialAction(CurrentStepInfo);
			OnTutorialStepClear(CurrentStepInfo);

			if (const FTutorialStep* NextStepInfo = GetNextStep(CurrentStepInfo))
			{
				// IsValid NextStepInfo - ���� ������ Step�� �����ִٸ�
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

			//�����ӽ� ������ Ʃ�丮�� �������� ���޹ް� ������ ���¶�� �ش� Ʃ�丮�� �Ϸ� ó��
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
		//�־��� ��� Ʃ�丮�� �߰� ���� ���·� �κ� ������ ���� ��û
		if (TutorialStep->TutorialStep > 1 && TutorialStep->Action != ETutorialAction::TutorialClear)
			RequestRewardTutorial();
	}
	else
	{
		SetCurrentTutorialID(INDEX_NONE);
		SetCurrentTutorialStep(INDEX_NONE);
		ExternalProcessTutorial();	// �Ƹ� �ڵ����� �帧���� RequestUpdateTutorial�� �ϰԵ� ����
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
	// Passable State Ȯ�� �� !!!!!!! �ϵ��ڵ� �� ������ �����ϴ��� üũ - �ϵ��ڵ� ������ ������ ������ true ����
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

	//	// Lobby���� Ȯ��
	//	if (StageDataStore.IsStageClearedOnce(RequireServerStageID) || RequireServerStageID == 0)
	//		return true;

	//	// StageClear UI (In Game) ���� Ȯ��
	//	const FServerStageID CurrClearStageID = CheckAdventureStageID(AB2StageManager::GetCacheStageKeepEssentialData().GetServerStageID());
	//	if ((RequireServerStageID < CurrClearStageID) || RequireServerStageID == CurrClearStageID)
	//		return true;

	//	// CachedActData�� �������� ������ AccountInfo���� Ȯ��
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
	//		OnTutorialClear(CurrentTutorialID); // ����� �ٷ� ���������� ���� ���� �� Clear Request ������ Response���� UI�����ش�
	//	break;
	//}

	//SetVisibleSkipButton(TutorialStepInfo->ViewSkipButton);

	//// �ϵ��ڵ��� Action�� �����Ѵ�.
	//ProcessExceptionAction(TutorialStepInfo);

	//ProcessNPCSoundAction(TutorialStepInfo);
}

bool TutorialManager::CheckAndBeginTutorial(int32 ServerTutorialID)
{
	if(bTestTutorialAvailable == false)	//Ʃ�丮�� �׽�Ʈ off���϶� ���� Ʃ�丮�� ���� �ȵǵ��� ����
	{ return false; }

	//FString ScopeString = FString::Printf(TEXT("TutorialManager::CheckAndBeginTutorial : ServerTutorialID [%d]"), ServerTutorialID);
	//B2_SCOPED_TRACK_LOG(*ScopeString);

	//// Dummy Code
	//CachingTableInit();	// ����ϵ���̽����� TableInfo�� �ε���� �ʴ� ��찡 ����
	//// Dummy Code

	//// �������� Tutorial�� ���� ������ TutorialID �̾�� ��
	//if (IsProcessingTutorial() == false && GetTutorialInfo(ServerTutorialID) != nullptr)
	//{
	//	// ���� �����ϸ� ���� UIScene�� LobbyMain�̾����
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

	return true;	// ������ true
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

	return true;	// ������ true
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

	return true;	// ������ true
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
	//	// ���� Ʃ�丮�� �� ��� ĳ���� Ʃ�丮�� ���� ���� ��û
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
	SetTutorialProgress ServerCommand�� ���� ����(����)������ ��� ����! ���� ����
*/
void TutorialManager::ForceBeginTutorial(int32 TutorialID, bool bFinish)
{
#if WITH_EDITOR
	SetTestTutorial(TutorialID);
	FString CommandString = FString::Printf(TEXT("SetTutorialProgress %d %d %d %d"), TutorialID, 0, 0, static_cast<int32>(bFinish));
	data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*CommandString);
#endif
}

// Tutorial Hardcoding - ������ �ϵ��ڵ����� ���ε� �Ǿ�� �� �κе�


void TutorialManager::OnStageClear()
{
	// ���� Clear�� StageID�� GetProcessableNextTutorial ���ο��� Ȯ��
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
	//	// ��ȹ�ǵ��� ���� ETutorialType::Default�� ��� TutorialDesc�� �����ش�.
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
		SetTutorialStepState(ETutorialStepState::WaitForRequirement); // Process Step�Ұ����ϰ� ����
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
//		// �Ϲ� UIData �� ���
//		if (FB2UIWidgetData* WidgetData = UB2UIManager::GetInstance()->GetWidgetDataByWidget(Cast<UB2UIWidget>(TargetWidget)))
//		{
//			WidgetName = WidgetData->WidgetFName.ToString();
//		}
//		// DJLegacy �� ���
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
	//ó�� �������� Ŭ����
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 1);	// Action1 Chapter���� Stage 1 - 1 ���� �� Scene�� Swipe���� �� Highlight ���ġ
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 2);	// Action2	Chapter���� Block�� Swipe����
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_FirstStageClear, 8);	// Require8 - Stage1 ���� �� �κ�� ���ƿͼ� ���� Clear Ȥ�� ����/��� Ȯ��
	REGISTERTUTORIAL_END_EXCEPTION(TutorialID_FirstStageClear);		// TutorialEnd - 01 ���� Skip�ع��� ��츦 ���� DocChapter Swipe false�� ����

	//��� ������
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 0);		// Action0 - ��� ������ �� ���� Item �� ��� Request	
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 3);		// Action4 - 400_5���� ��� ����� �Ҷ� B2lobbyInventory::SelectForIngred ���� Signal���� Event�� Subscribe
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemLevelup, 4);		// Require5 - ��� �������� ��ᰡ �����ϰ� ��ϵǾ����� Ȯ��
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 4);		// Action5 - ��� ������ ������ 0���� �����ϰ� Ʃ�丮�� ������ ���� �������� �غ� - ������ ��ư �������� ��� �ٲ�ġ��, 400_4 ���� Subscribe�ߴ� Event Unsubscribe
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 5);		// Action6 - ���̶���Ʈ ĳ�� ���� Ŭ����

	//�ݰ� ����
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 3);	// Action3 - �ݰݴ��� �ش� ���Ͽ� �°� Highlight ����
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 4);	// Action4 - �Ϲ����� Highlight Step�� �ƴϹǷ� DungeonSlot UI ĳ�� �� ������ ClearHighlight ȣ��

	//������
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_InitAndCenterCamera);	// Action - �ΰ��� ù�׼� ī�޶� �ű�� ��Ʋ���μ���� �� ���غ�
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_BotCamera);			// Action - ī�޶� �ű��. �ٿű�� ���Ӹ�忡�� ���� �Ѱ��ش�.
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_TopCamera);			// Action - ī�޶� �ű��. �ٿű�� ���Ӹ�忡�� ���� �Ѱ��ش�.
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_IngameCamera);			// Action - ī�޶� �ΰ��� ����.
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_SetIngame);			// Action - ���� ����Ҽ��ִ� ���·� UI�� ���Ӹ�� ��������

	//��� ��ȭ
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 0);		// Action0 - Ʃ�丮�� ���� �� ��ȭ �غ� �������� ����
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 1);		// Require2 - ���� �� Mainĳ���� �������� ���� Ȯ��,(�ֹ���, ���Ⱑ 4ĳ�� �� ������� ���̹��⵵) ���� ��û, Response������ true
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 5);		// Action6 - ĳ�� ���̶���Ʈ ���� Ȱ��ȭ
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 6);		// Require7 - ��ȭ�� ItemDetailPopup�� ���������� ���ȴ��� Ȯ��
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 6);		// Action7 - ��ȭ ��ư ������ �� ��ȭ Protocol Subscribe
	REGISTERSTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 7);		// Require8 - ��ȭ Protocol�� ����� Response�Ǿ����� Ȯ��
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 7);		// Action8 - TutorialUI Hide ��Ű�� 1100_7���� ��ȭ �������� subscribe�� unsubscribe

	//���� ����
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 3);		// Action2 Chapter���� Stage 1 - 1 ���� �� Scene�� Swipe���� �� Highlight ���ġ
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 4);		// Action3	Chapter���� Block�� Swipe����
	REGISTERTUTORIAL_END_EXCEPTION(TutorialID_ExpertDungon);		// TutorialEnd - 01 ���� Skip�ع��� ��츦 ���� DocChapter Swipe false�� ����

	//���׸�
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Ether, 0);				// Action0 - ���׸� Ʃ�丮��� ������ Request
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Ether, 13);			// Action13 - ���׸� ���� �ִϸ��̼� �۵� �� Ʃ�丮�� ���� hide

	//����
	REGISTERSTEP_ACTION_EXCEPTION(TutorialID_Seal, 0);				// Action0 - ���׸� Ʃ�丮��� ������ Request

	//�Ǹ� ����	//��ȹ ��û ����(����� ���ܴ޶�� �Ͻ�)
	//REGISTERSTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 2);		// Action2 Chapter���� Stage 1 - 1 ���� �� Scene�� Swipe���� �� Highlight ���ġ
	//REGISTERSTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 3);		// Action3	Chapter���� Block�� Swipe����
	//REGISTERTUTORIAL_END_EXCEPTION(TutorialID_NightmareDungeon);		// TutorialEnd - 01 ���� Skip�ع��� ��츦 ���� DocChapter Swipe false�� ����
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

// �ϵ��ڵ��ΰ� Ƽ���� ���� ���� ������ ����

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

// Action - Tutorial100_1 Chapter���� Stage 1 - 1 ���� �� Scene�� Swipe�Ǵ� ���� �����ϱ� ����
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

// Require - Tutorial100_8 - Stage1 ���� �� �κ�� ���ƿͼ� ���� Clear Ȥ�� ����/��� Ȯ��
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

// 400-5 ���� ������ ��� ���Step�� Event�� subscribe
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

	//		// �̵��г� ����
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

	//		// �̵��г�	����
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

// Require - Tutorial1100_1 - ���� �� Mainĳ���� �������� ���� Ȯ��,(�ֹ���, ���Ⱑ 4ĳ�� �� ������� ���̹��⵵) ���� ��û, Response������ true
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

	//// Response �޴� �ñⰡ UIOpen ������ �� �־� �ٽ� �ѹ� üũ����� ��
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

//�Ǹ� ����	//��ȹ ��û ����(����� ���ܴ޶�� �Ͻ�)
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
