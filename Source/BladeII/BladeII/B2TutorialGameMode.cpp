// Fill out your copyright notice in the Description page of Project Settings.

#include "B2TutorialGameMode.h"
#include "Event.h"
#include "Trigger.h"
#include "MoviePlayer.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIManager_InGameCombat.h"

#include "BladeIIPlayerController.h"
#include "B2PreLoadingScreen.h"
#include "B2StageManager.h"
#include "B2MonsterSpawnPool.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillRTManager.h"
#include "B2SkillInfo.h"
#include "TutorialManager.h"
#include "B2CodeTable.h"

#include "BladeIIGameImpl.h"

#define REGISTE_PHASECLEAR_CALLBACK(TutorialNum)											 		\
		{																							\
			FOnTutorialPhaseClear CallbackHandler =  FOnTutorialPhaseClear::CreateLambda([this](){	\
				this->UpdateConditionTutorial##TutorialNum(this->GetCurrentTutorialPhase());});		\
			int32 TutorialIndex = TutorialNum -1;													\
			OnPhaseClearCallbackList[TutorialIndex] = CallbackHandler;								\
		}																							\



AB2TutorialGameMode::AB2TutorialGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//BattleMainUI = nullptr;
	GameRule = nullptr;

	bVisibleVirtualJoystick = false;

	bTutorialPhaseDone = false;

	bReadyForQTEAction = false;
	bSuggestQTEAction = false;
	bTutorialQTEComplete = false;

	bReadyForCounterAction = false;
	bTutorial4CounterAction = false;
	bCountAttackSuccess = false;
	bSetMinoZombie = false;
	bMinoAttackVibration = false;
	bTutorialUnitySkillAction = false;

	CounterPhaseBlockingVolume = nullptr;

	DefaultPawnClass = ABladeIITutorialPlayer::StaticClass();

	PendingAsyncLoadRequestPCClass = EPCClass::EPC_End;
	// Async 로딩 순서인데 실제로 이건 튜토리얼 흐름을 따라가야 함. 
	// 현재 SetNextPlayerClass 호출 시점이 Async 로딩에 필요한 시점과 달라서 따로 넣게 되었음.
	// 이거 외에 다른 식으로 지정할 계획이 있으면 이건 폐기..
	PCClassAsyncLoadOrder.Add(EPCClass::EPC_Wizard);
	PCClassAsyncLoadOrder.Add(EPCClass::EPC_Fighter);
	PCClassAsyncLoadOrder.Add(EPCClass::EPC_Assassin);
}


void AB2TutorialGameMode::PreBeginPlay()
{
	GetStageManager()->GetCacheStageKeepEssentialData().SetStageBasisData(9001, EStageDifficulty::ESD_Normal, EPCClass::EPC_Gladiator, EPCClass::EPC_Wizard);

	// BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(EPCClass::EPC_Gladiator);
	// BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(EPCClass::EPC_Wizard);
	if (GameRule == nullptr)
	{
		GameRule = new TutorialGameRule;
	}

	SetTutorialGameRule();

	Super::PreBeginPlay();

	// PCClassData 중 가능한 부분을 Async 로딩하면서 다른 것들의 Preload 까지 막게 되므로 (GetPCClassesToPreLoad 에서 리턴 대상 줄임)
	// async 로딩 대상이 아닌 것들은 여기서 직접 로딩한다. 크지는 않겠지만
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		TArray<EPCClass> ClassesToLoad;
		// 이건 별로 시간 안 먹을 것이고 PCClassInfo 중에서 따로 떼어낸 InGameOnlyInfo 랑 SkillAnim 이 많이 먹을 꺼. 그걸 async 로딩 할 것임.
		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		{
			ClassesToLoad.Add(IntToPCClass(PCI));
		}
		PCBox->PreloadClassAssets(ClassesToLoad);
	}

	PreBuildCharacterMeshes();
}

void AB2TutorialGameMode::BeginPlay()
{
	// 덩치 큰 PCClass 를 Async 로딩하므로 NPC 들은 Async 대상이었던 것들도 모두 미리 로딩해 둠.
	//GameContextAsyncFlush();

	Super::BeginPlay();

	MarkBeginAsyncLoadNextPCClass(); // BeginPlay 에서면 두번째 나올 캐릭터 데이터에 대한 로딩 요청이 됨. 이후에는 각각의 교체 이후가 될 것..
}

void AB2TutorialGameMode::SetTutorialGameRule(bool bIsQTE)
{
	GameRule->SetQTEEnabled(bIsQTE);
	//SetGameRule(GameRule);
}

void AB2TutorialGameMode::SetDesiredPCClassEnum()
{
	CurrentPCClassEnum = EPCClass::EPC_Gladiator;
}


void AB2TutorialGameMode::SubscribeEvents()
{
	if (bEventsSubscribed)
		return;

	Super::SubscribeEvents();

	Issues.Add(OpenDialogClass<FName, bool>::GetInstance().Subscribe2(
		[this](FName InDialogCodeName, bool bMaintainBattleUI)
	{
		this->OnOpenScenarioDialog(InDialogCodeName);
	}
	));
	Issues.Add(CloseDialogClass<FName>::GetInstance().Subscribe2(
		[this](FName InDialogCodeName)
	{
		this->OnCloseScenarioDialog(InDialogCodeName);
	}
	));
	Issues.Add(PlayerStartAttackClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputAttack();
	}
	));
	Issues.Add(PlayerStartSkill01Class<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputSkill(0);
	}
	));
	Issues.Add(PlayerStartSkill02Class<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputSkill(1);
	}
	));
	Issues.Add(PlayerStartSkill03Class<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputSkill(2);
	}
	));
	Issues.Add(PlayerStartWeaponSkillClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputUnitySkill();
	}
	));
	Issues.Add(QTEAttackClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputQte();
	}
	));
	Issues.Add(PlayerStartGuardClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputGuard();
	}
	));
	Issues.Add(PlayerStartTagClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnInputTag();
	}
	));
	Issues.Add(PlayerCounterAttackSucceedClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnCounterAttackSuccessed();
	}
	));
	Issues.Add(CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Subscribe2(
		[this](ABladeIICharacter* Victim, float Damage, bool bCritical)
	{
		this->OnCharacterDamaged(Victim, Damage);
	}
	));
	Issues.Add(MarkBeginAsyncLoadClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->MarkBeginAsyncLoadNextPCClass();
	}
	));
	Issues.Add(AutoMinoRiddingClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->StartAutoMinoRidding();
	}
	));

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnQTEEnableStateChanged.AddUObject(this, &AB2TutorialGameMode::OnChangedQTEEnableState);
	}

	OnPhaseClearCallbackList.AddDefaulted(static_cast<int32>(ETutorialNumber::Max));

	REGISTE_PHASECLEAR_CALLBACK(1);
	REGISTE_PHASECLEAR_CALLBACK(2);
	REGISTE_PHASECLEAR_CALLBACK(3);
	REGISTE_PHASECLEAR_CALLBACK(4);
	REGISTE_PHASECLEAR_CALLBACK(5);
	REGISTE_PHASECLEAR_CALLBACK(6);
	REGISTE_PHASECLEAR_CALLBACK(7);
	REGISTE_PHASECLEAR_CALLBACK(8);
	REGISTE_PHASECLEAR_CALLBACK(9);
}

void AB2TutorialGameMode::SetupUIScene()
{
	//// BattleMain 사용 X
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TutorialBattleMain);

	//if (BattleMainUI == nullptr)
	//{
	//	BattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorialBattleMain>(UIFName::TutorialBattleMain);
	//	ensure(BattleMainUI != nullptr);

	//	BattleMainUI->OnStageBegin();
	//}

	//InitTutorialUI();

	//SetPlayerAutoType(AutoBattleType::NONE);
	////TogglePlayerAutoStateClass<>::GetInstance().Signal();
}

void AB2TutorialGameMode::BeginDestroy()
{
	//// BeginDestroy 시점에서의 UIWidget destroy 는 주의할 필요가 있음.
	//if (FinishScreenPtr && !FinishScreenPtr->IsPendingKillOrUnreachable() && !FinishScreenPtr->HasAnyFlags(RF_BeginDestroyed))
	//{
	//	FinishScreenPtr->DestroySelf();
	//}
	//FinishScreenPtr = nullptr;

	Super::BeginDestroy();

	//UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{
	//	DocBattle->OnQTEEnableStateChanged.RemoveAll(this);
	//}
}

void AB2TutorialGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PendingAsyncLoadRequestPCClass != EPCClass::EPC_End)
	{
		BeginAsyncLoadNextPCClass();
	}
}

float AB2TutorialGameMode::GetCurrentStageDifficultyScale()
{
	return DifficultyScale;
}

void AB2TutorialGameMode::NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType)
{
	Super::NotifyStageEventSceneBegin(BegunDirector, EventType);
}

void AB2TutorialGameMode::NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	Super::NotifySpawnPoolMobSpawn(SpawnedMob, InBirthplaceInfo);

	if (InBirthplaceInfo.WaveNumber == 2 && TutorialMainIndex == (int32)ETutorialNumber::Tutorial_5)
	{
		SetStartTutorial(ETutorialNumber::Tutorial_6);
	}
}

void AB2TutorialGameMode::NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum)
{
	Super::NotifySpawnPoolSingleWaveComplete(CompletedSpawnPool, WaveNum);

	if (WaveNum == 4 && TutorialMainIndex == (int32)ETutorialNumber::Tutorial_2)
	{
		ShowTutorialArrow(WaveNum);
	}
	else if (WaveNum == 0 && TutorialMainIndex == (int32)ETutorialNumber::Tutorial_2)
	{
		ShowTutorialArrow(WaveNum);
	}
	else if (WaveNum == 1 && TutorialMainIndex == (int32)ETutorialNumber::Tutorial_4)
	{
		ShowTutorialArrow(WaveNum);
	}
	else if (WaveNum == 2 && TutorialMainIndex == (int32)ETutorialNumber::Tutorial_6)
	{
		ShowTutorialArrow(WaveNum);
	}
	else if (WaveNum == 5 && TutorialMainIndex == (int32)ETutorialNumber::Tutorial_7)
	{
		ShowTutorialArrow(3);
		DisableBlockingComponent();

		bMinoAttackVibration = false;
	}
}

void AB2TutorialGameMode::SetOpenSkillButton(EPCClass TargetClass, int32 SkillIndex, bool bOpen)
{
	class UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(TargetClass));
	if (DocHero)
	{
		UB2UIDocSkill* SkillDoc = DocHero->GetSkillDocByInputIndex(SkillIndex);
		if (SkillDoc)
		{
			SkillDoc->SetIsExistingSkill(bOpen);
		}
	}
}

//void AB2TutorialGameMode::SetGuardLock(ETutorialWidgetState Lock)
//{
//	if (BattleMainUI != nullptr)
//	{
//		BattleMainUI->SetGuardWidgetState(Lock);
//	}
//}

void AB2TutorialGameMode::StartForceFeedback()
{
	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer)
	//{
	//	TutorialPlayer->ForceFeedback();
	//}
}

void AB2TutorialGameMode::SetVirtualJoystickVisible(bool bVisible)
{
	//if (UIManager_InGameCombat)
	//{
	//	bVisibleVirtualJoystick = bVisible;
	//	UIManager_InGameCombat->HideVirtualJoystick(!bVisibleVirtualJoystick);
	//}
}

void AB2TutorialGameMode::InitTutorialUI()
{
	TutorialMainIndex = 0;

	SetVirtualJoystickVisible(false);
}

void AB2TutorialGameMode::SetStartTutorial(ETutorialNumber Number)
{
	bTutorialQTEComplete = false;
	bTutorialPhaseDone = false;
	bSuggestQTEAction = false;
	TutorialSubIndex = -1;
	TutorialMainIndex = (int32)Number;

	InitSettingTutorialPhase(Number);
	StopRepeatingInput();

	ActionNextTutorial();
}

void AB2TutorialGameMode::SetFinishTutorial()
{
	//SetClearNewTutorial(true);

	//if (TutorialManager::GetInstance().GetLastClearTutorialID() < 0)
	//{
	//	TutorialManager::GetInstance().RequestRewardTutorial();

	//	SendFinishActionLog();
	//}

	//BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(EPCClass::EPC_Assassin);
	//BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(EPCClass::EPC_Fighter);

	//GoToVillageClass<>::GetInstance().Signal();
}

void AB2TutorialGameMode::SendFinishActionLog()
{
	////////////////////////////////////////////////////////////////////
	////액션 지표 로그 (튜토리얼 완료)
	//B2ActionLogSender Sender;
	//Sender.setCategory(FString("TUTORIAL"))
	//	.setAction(FString("COMPLETE"))
	//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	////////////////////////////////////////////////////////////////////
}

void AB2TutorialGameMode::SetAllSkillButtonEnable(bool IsEnabled)
{
	for (int32 i = 0; i < 3; i++)
	{
		SetOpenSkillButton(EPCClass::EPC_Fighter, i, IsEnabled);
	}
}

void AB2TutorialGameMode::ShowTutorialArrow(int32 WaveNumber)
{
	for (auto &Each : TutorialArrows)
	{
		if (Each.WaveNumber == WaveNumber && Each.TargetArrow && Each.IsShow == false)
		{
			Each.IsShow = true;
			Each.TargetArrow->GetParticleSystemComponent()->SetVisibility(true);
		}
	}
}

void AB2TutorialGameMode::ShowTutorialArrowDirect(AEmitter *TargetArrow)
{
	if (TargetArrow)
	{
		TargetArrow->GetParticleSystemComponent()->SetVisibility(true);
	}
}

void AB2TutorialGameMode::ShowTutorialFinishScene()
{
	//if (TutorialFinishScreenClass && FinishScreenPtr == nullptr)
	//{
	//	ABladeIIPlayerController* LocalPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));

	//	FinishScreenPtr = CreateWidget<UB2PreLoadingScreen>(LocalPC, PreLoadingScreenWidgetClass);
	//	if (FinishScreenPtr)
	//	{
	//		FinishScreenPtr->AddToViewport(BII_WIDGET_ZORDER_PRELOADINGSCREEN);
	//		FinishScreenPtr->SetLoadingText_Middle1(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialFinishLoadingText1")));
	//		FinishScreenPtr->SetLoadingText_Middle2(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TutorialFinishLoadingText2")));
	//	}
	//}
}

void AB2TutorialGameMode::ActionNextTutorial()
{
	//if (TutorialPhases.Num() < 1 || TutorialPhases.Num() - 1 < TutorialMainIndex)
	//	return;

	//if (bTutorialPhaseDone)
	//	return;

	//const int32 ClearSubIndex = TutorialPhases[TutorialMainIndex].Phases.Num();
	//if (TutorialSubIndex < ClearSubIndex)
	//{
	//	++TutorialSubIndex;
	//}

	//if (TutorialSubIndex == ClearSubIndex)
	//{
	//	bTutorialPhaseDone = true;
	//	return;
	//}

	//if (GetCurrentTutorialPhase() == nullptr)
	//	return;

	//auto CurPhaseData = GetCurrentTutorialPhase();

	//SetTutorialPhaseInputControl(CurPhaseData->bLockInput);

	//switch (CurPhaseData->TutorialEventType)
	//{
	//case ETutorialEventType::OpenDialog:
	//	if (CurPhaseData->bPauseScenarioUntilClose)
	//	{
	//		if (CurPhaseData->ScenarioPauseDelay > 0.f)
	//		{
	//			GetWorldTimerManager().ClearTimer(ScenarioDialogTH);
	//			GetWorldTimerManager().SetTimer(ScenarioDialogTH, this, &AB2TutorialGameMode::PauseTutorialGame, CurPhaseData->ScenarioPauseDelay, false);
	//		}
	//		else
	//		{
	//			PauseTutorialGame();
	//		}
	//	}
	//	ShowScenarioDialog(FName(*(CurPhaseData->ContentData)));
	//	break;

	//case ETutorialEventType::WidgetHide:
	//	if (BattleMainUI)
	//		BattleMainUI->HideTutorialWidgetGroup(CurPhaseData->ContentData);
	//	break;

	//case ETutorialEventType::WidgetShow:
	//	if (BattleMainUI)
	//		BattleMainUI->RestoreTutorialWidgetGroup(CurPhaseData->ContentData);
	//	break;

	//case ETutorialEventType::OpenTag:
	//	PauseTutorialGame();

	//	if (BattleMainUI)
	//		BattleMainUI->SetTagWidgetState(ETutorialWidgetState::Suggest);
	//	break;

	//case ETutorialEventType::closeTag:
	//	ResumeTutorialGame();

	//	if (BattleMainUI)
	//		BattleMainUI->SetTagWidgetState(ETutorialWidgetState::Lock);
	//	break;

	//case ETutorialEventType::OpenSkill:
	//	SetAllSkillButtonEnable(true);

	//	if (BattleMainUI)
	//	{
	//		for (int32 i = 0; i < 3; i++)
	//		{
	//			BattleMainUI->SetSkillWidgetSuggest(i, true);
	//		}
	//		BattleMainUI->SetSkillWidgetClickable(true);
	//	}
	//	break;

	//case ETutorialEventType::OpenUnitySkill:
	//	SetUnitySkillUI();
	//	break;

	//case ETutorialEventType::OpenGuard:
	//	if (BattleMainUI)
	//		BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Suggest);
	//	break;

	//case ETutorialEventType::CloseGuard:
	//	if (BattleMainUI)
	//		BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Lock);
	//	break;

	//case ETutorialEventType::WaitingForCondition:
	//	if (GetCurrentTutorial() == ETutorialNumber::Tutorial_3)
	//	{
	//		if (BattleMainUI)
	//		{
	//			BattleMainUI->SetAttackEnable(false);
	//			BattleMainUI->SetSkillWidgetClickable(true);
	//		}
	//	}
	//	break;

	//case ETutorialEventType::VirtualJoystickShow:
	//	SetVirtualJoystickVisible(true);
	//	break;

	//case ETutorialEventType::VirtualJoystickHide:
	//	SetVirtualJoystickVisible(false);
	//	break;

	//case ETutorialEventType::delay:
	//	if (CurPhaseData->ContentData.IsNumeric())
	//	{
	//		float DelayTime = FCString::Atof(*CurPhaseData->ContentData);

	//		GetWorldTimerManager().ClearTimer(TutorialTH);
	//		GetWorldTimerManager().SetTimer(TutorialTH, this, &AB2TutorialGameMode::OnDelayTime, DelayTime, false);
	//	}
	//	break;

	//default:
	//	break;
	//}

	//if (OnPhaseClearCallbackList.IsValidIndex(TutorialMainIndex))
	//	OnPhaseClearCallbackList[TutorialMainIndex].ExecuteIfBound();

	//// 조건 없으면 바로 다음 단계 진행
	//if (bTutorialPhaseDone == false &&
	//	CurPhaseData->CompleteConditionType == ETutorialConditionType::None)
	//	ActionNextTutorial();
}

void AB2TutorialGameMode::SetUnitySkillUI()
{
	//if (BattleMainUI)
	//{
	//	for (int32 i = 0; i < 3; i++)
	//	{
	//		// Open할때 suggest
	//		BattleMainUI->SetSkillEnable(i, false);
	//	}
	//}

	//int32 TempSKillPoint = 1;
	//SetTutorialPhaseInputControl(false);
	//class UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(EPCClass::EPC_Assassin));
	//if (DocHero)
	//{
	//	UB2UIDocSkill* SkillDoc = DocHero->GetSkillDocByInputIndex(WEAPON_6STAR_SKILL_INDEX);
	//	if (SkillDoc)
	//	{
	//		SkillDoc->SetSkillStackMaxPoint(TempSKillPoint);
	//		SkillDoc->SetSkillStackPoint(TempSKillPoint);
	//	}
	//}

	//ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (LocalPlayerChar)
	//	LocalPlayerChar->SetWeaponSkillPointMax();

	//SetOpenSkillButton(EPCClass::EPC_Assassin, WEAPON_6STAR_SKILL_INDEX, true);
	//SetOpenSkillButton(EPCClass::EPC_Gladiator, WEAPON_6STAR_SKILL_INDEX, true);

	//if (BattleMainUI)
	//	BattleMainUI->SetSkillWidgetSuggest(WEAPON_6STAR_SKILL_INDEX, true);
}

// 1. Move Suggest Tutorial
void AB2TutorialGameMode::UpdateConditionTutorial1(FTutorialPhaseData* TutorialPhaseData)
{
	/*check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;

	if (CurrPhaseEventType == ETutorialEventType::VirtualJoystickShow)
	{
		if (BattleMainUI)
		{
			BattleMainUI->SetMoveWidgetState(ETutorialWidgetState::Suggest);
			StartGuideArrow();
		}

		ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
		if (TutorialPlayer)
		{
			TutorialPlayer->StartEanbleAttack(false);
			TutorialPlayer->StartEanbleGuard(false);
		}
	}*/
}

// 2. Attack Suggest Tutorial
void AB2TutorialGameMode::UpdateConditionTutorial2(FTutorialPhaseData* TutorialPhaseData)
{
	/*check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;

	if (CurrPhaseCompleteCondition == ETutorialConditionType::InputAttack)
	{
		if (BattleMainUI)
		{
			BattleMainUI->SetUIEnable(false);
			BattleMainUI->SetAttackEnable(true);
		}

		ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
		if (TutorialPlayer)
		{
			TutorialPlayer->StartEanbleAttack(true);
		}
	}*/
}

// 3. Character Switching
void AB2TutorialGameMode::UpdateConditionTutorial3(FTutorialPhaseData* TutorialPhaseData)
{
	check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;
}

// 4. Guard Suggest Tutorial
void AB2TutorialGameMode::UpdateConditionTutorial4(FTutorialPhaseData* TutorialPhaseData)
{
	/*check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;

	if (CurrPhaseCompleteCondition == ETutorialConditionType::InputGuard)
	{
		if (BattleMainUI)
		{
			BattleMainUI->SetAttackEnable(false);
			BattleMainUI->SetGuardEnable(true);
		}
	}*/
}

// 5. Character Switching
void AB2TutorialGameMode::UpdateConditionTutorial5(FTutorialPhaseData* TutorialPhaseData)
{
	check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;
}

// 6. Skill Suggest Tutorial
void AB2TutorialGameMode::UpdateConditionTutorial6(FTutorialPhaseData* TutorialPhaseData)
{
	//check(TutorialPhaseData != nullptr);
	//const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	//const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;

	//if (CurrPhaseCompleteCondition == ETutorialConditionType::InputSKill)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetSkillWidgetClickable(true);
	//	}
	//}
}

// 7. Character Switching
void AB2TutorialGameMode::UpdateConditionTutorial7(FTutorialPhaseData* TutorialPhaseData)
{
	check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;
}

// 8. Mino QTE Suggest Tutorial
void AB2TutorialGameMode::UpdateConditionTutorial8(FTutorialPhaseData* TutorialPhaseData)
{
	check(TutorialPhaseData != nullptr);
	const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;
}

// 9. Unity Skill Suggest Tutorial
void AB2TutorialGameMode::UpdateConditionTutorial9(FTutorialPhaseData* TutorialPhaseData)
{
	//check(TutorialPhaseData != nullptr);
	//const ETutorialEventType CurrPhaseEventType = TutorialPhaseData->TutorialEventType;
	//const ETutorialConditionType CurrPhaseCompleteCondition = TutorialPhaseData->CompleteConditionType;

	//if (CurrPhaseCompleteCondition == ETutorialConditionType::InputUnitySkill)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUnitySkillWidgetClickable(true, false);
	//	}
	//}
}

/* Setting */
void AB2TutorialGameMode::InitSettingTutorialPhase(ETutorialNumber Number)
{
	//// Move
	//if (Number == ETutorialNumber::Tutorial_1)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//	}
	//}

	//// 구울 Attack
	//else if (Number == ETutorialNumber::Tutorial_2)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//	}
	//}

	//// 캐릭터 스위칭 (검투사 -> 마법사)
	//else if (Number == ETutorialNumber::Tutorial_3)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);

	//		BattleMainUI->SetAttackWidgetState(ETutorialWidgetState::Enable);
	//	}

	//	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (TutorialPlayer)
	//	{
	//		TutorialPlayer->StartEanbleGuard(false);
	//	}

	//	SetNextPlayerClass(EPCClass::EPC_Fighter);
	//}

	//// 석상 반격
	//else if (Number == ETutorialNumber::Tutorial_4)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//		BattleMainUI->SetGuardEnable(true);
	//	}

	//	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (TutorialPlayer)
	//	{
	//		TutorialPlayer->StartEanbleGuard(true);
	//	}

	//	/*
	//	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (TutorialPlayer)
	//	{
	//	TutorialPlayer->SetInvincibleByTutorial(true);
	//	}
	//	*/

	//	bReadyForCounterAction = true;
	//	bTutorial4CounterAction = true;
	//}

	//// 캐릭터 스위칭 (마법사 -> 격투가)
	//else if (Number == ETutorialNumber::Tutorial_5)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//		BattleMainUI->SetGuardEnable(true);

	//		BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Enable);
	//	}

	//	SetNextPlayerClass(EPCClass::EPC_Assassin);
	//}

	//// Skill 
	//else if (Number == ETutorialNumber::Tutorial_6)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//		BattleMainUI->SetGuardEnable(true);
	//		BattleMainUI->SetSkillWidgetClickable(true);
	//	}
	//}

	//// 캐릭터 스위칭 (격투가 -> 암살자)
	//else if (Number == ETutorialNumber::Tutorial_7)
	//{
	//	if (BattleMainUI)
	//	{
	//		SetTutorialGameRule(true);

	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//		BattleMainUI->SetGuardEnable(true);
	//		BattleMainUI->SetSkillWidgetClickable(true);

	//		for (int32 i = 0; i < 3; i++)
	//		{
	//			BattleMainUI->SetSkillWidgetSuggest(i, false);
	//		}
	//	}

	//	bSetMinoZombie = true;
	//}

	//// 미노 QTE 이후
	//else if (Number == ETutorialNumber::Tutorial_8)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetSkillWidgetClickable(true);
	//		BattleMainUI->SetMountAttackWidgetState(ETutorialWidgetState::Enable);
	//	}

	//	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (TutorialPlayer)
	//	{
	//		TutorialPlayer->ResetSkillCooltime();
	//		TutorialPlayer->StopForceFeedback();
	//	}
	//}

	//// Weapon skill
	//else if (Number == ETutorialNumber::Tutorial_9)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetSkillWidgetClickable(false, false);
	//		BattleMainUI->SetAttackEnable(false);
	//		BattleMainUI->SetGuardEnable(false);
	//	}

	//	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (TutorialPlayer)
	//	{
	//		TutorialPlayer->ResetSkillCooltime();
	//	}

	//	SetTutorialPhaseInputControl(true);

	//	StartAutoUnitySkill();
	//}
}

void AB2TutorialGameMode::StopRepeatingInput()
{
	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer)
	//{
	//	TutorialPlayer->StopRepeatingInput();
	//}
}

/* Input */
void AB2TutorialGameMode::OnInputMove()
{
	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer && BattleMainUI)
	//{
	//	BattleMainUI->SetMoveWidgetState(ETutorialWidgetState::Enable);
	//}
}

void AB2TutorialGameMode::OnInputAttack()
{
	//if (CheckTutorialCondition(ETutorialConditionType::InputAttack))
	//{
	//	if (GetCurrentTutorial() == ETutorialNumber::Tutorial_2)
	//	{
	//		if (BattleMainUI)
	//		{
	//			BattleMainUI->SetAttackWidgetState(ETutorialWidgetState::Enable);
	//			BattleMainUI->SetAttackEnable(true);
	//		}
	//	}
	//	else if (GetCurrentTutorial() == ETutorialNumber::Tutorial_7)
	//	{
	//		if (BattleMainUI)
	//		{
	//			BattleMainUI->SetMountAttackWidgetState(ETutorialWidgetState::Enable);
	//		}
	//	}

	//	ActionNextTutorial();
	//}
}

void AB2TutorialGameMode::OnInputGuard()
{
	//if (CheckTutorialCondition(ETutorialConditionType::InputGuard))
	//{
	//	if (GetCurrentTutorial() == ETutorialNumber::Tutorial_4)
	//	{
	//		// do something..
	//		if (!bReadyForCounterAction)
	//		{
	//			ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//			if (TutorialPlayer)
	//			{
	//				TutorialPlayer->ForceCounterAttack();
	//			}

	//			if (BattleMainUI)
	//			{
	//				BattleMainUI->SetAttackEnable(true);
	//				BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Enable);
	//			}

	//			bReadyForCounterAction = true;

	//			SetTutorialPhaseInputControl(true);
	//			StopSlowTime();
	//		}
	//	}

	//	ActionNextTutorial();
	//}
}

void AB2TutorialGameMode::OnInputSkill(int32 SkillIndex)
{
	/*if (CheckTutorialCondition(ETutorialConditionType::InputSKill))
	{
		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_6)
		{
			for (int32 i = 0; i < 3; i++)
			{
				BattleMainUI->SetSkillWidgetSuggest(i, false);
			}

			if (BattleMainUI)
			{
				BattleMainUI->SetAttackEnable(true);
				BattleMainUI->SetSkillWidgetClickable(true);
				BattleMainUI->SetGuardEnable(true);
			}
		}

		ActionNextTutorial();
	}*/
}

void AB2TutorialGameMode::OnInputQte()
{
	/*if (CheckTutorialCondition(ETutorialConditionType::InputQTE))
	{
		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_7)
		{
			ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
			if (TutorialPlayer)
			{
				TutorialPlayer->bEnableNormalAttack = true;
			}

			if (BattleMainUI)
			{
				BattleMainUI->SetAttackEnable(true);
				BattleMainUI->SetGuardEnable(true);
				BattleMainUI->SetQTEWidgetState(ETutorialWidgetState::Enable);
				BattleMainUI->SetMountAttackWidgetState(ETutorialWidgetState::Suggest);
			}

			bMinoAttackVibration = true;
			bTutorialQTEComplete = true;
			bSetMinoZombie = false;

			OnSuccessQTE_Mino();
		}

		ActionNextTutorial();
	}*/
}

void AB2TutorialGameMode::OnInputUnitySkill()
{
	if (CheckTutorialCondition(ETutorialConditionType::InputUnitySkill))
	{
		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_9)
		{
			if (!bTutorialUnitySkillAction)
			{
				bTutorialUnitySkillAction = true;

				SetFinishTutorial();
			}
		}
	}
}

void AB2TutorialGameMode::OnInputTag()
{
	//if (CheckTutorialCondition(ETutorialConditionType::InputTag))
	//{
	//	// do something..

	//	SetOpenSkillButton(EPCClass::EPC_Gladiator, WEAPON_6STAR_SKILL_INDEX, false);
	//	SetOpenSkillButton(EPCClass::EPC_Wizard, WEAPON_6STAR_SKILL_INDEX, false);

	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetAttackEnable(true);
	//	}

	//	ActionNextTutorial();
	//}
}

void AB2TutorialGameMode::OnDelayTime()
{
	if (CheckTutorialCondition(ETutorialConditionType::delay))
	{
		// do something..

		ActionNextTutorial();
	}
}

void AB2TutorialGameMode::OnCloseScenarioDialog(FName CloseDialogName)
{
	//if (CheckTutorialCondition(ETutorialConditionType::CloseDialog))
	//{
	//	if (GetCurrentTutorial() != ETutorialNumber::Tutorial_6)
	//	{
	//		BattleMainUI->SetUIEnable(true);
	//	}
	//	else
	//	{
	//		if (BattleMainUI)
	//		{
	//			BattleMainUI->SetAttackEnable(true);
	//			BattleMainUI->SetSkillWidgetClickable(false, false);
	//			BattleMainUI->SetGuardEnable(false);
	//			BattleMainUI->SetEnabledTagBtn(false);
	//		}
	//	}

	//	GetWorldTimerManager().ClearTimer(ScenarioDialogTH);

	//	if (GetCurrentTutorial() != ETutorialNumber::Tutorial_1
	//		|| GetCurrentTutorial() != ETutorialNumber::Tutorial_2
	//		|| GetCurrentTutorial() != ETutorialNumber::Tutorial_6)
	//		ResumeTutorialGame();

	//	// do something..

	//	ActionNextTutorial();
	//}
}

void AB2TutorialGameMode::OnOpenScenarioDialog(FName OpenDialogName)
{
	if (CheckTutorialCondition(ETutorialConditionType::OpenDialog))
	{
		// do something..

		ActionNextTutorial();
	}
}

void AB2TutorialGameMode::StartGuideArrow()
{
	if (GEngine && GLog)
	{
		GEngine->HandleCeCommand(GetWorld(), TEXT("StartGuideArrow"), *GLog);
	}
}

void AB2TutorialGameMode::OnMobAttackNotify()
{
	UE_LOG(LogBladeII, Log, TEXT("@@ OnMobAttackNotify"));

	//if (bReadyForCounterAction && bTutorial4CounterAction)
	//{
	//	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (TutorialPlayer)
	//	{
	//		TutorialPlayer->ActiveTutorialCounterGuard();
	//		TutorialPlayer->StopRepeatingInput();
	//		TutorialPlayer->SetAttackState(EAttackState::ECS_None);

	//		SetTutorialPhaseInputControl(false); // Input을 풀어야 Guard를 하지
	//	}

	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Suggest);
	//	}

	//	bReadyForCounterAction = false;
	//	bTutorial4CounterAction = false;

	//	StartSlowTime(0.01f, 2.0f);
	//}
}

void AB2TutorialGameMode::OnMobStopAttackNotify()
{
	UE_LOG(LogBladeII, Log, TEXT("@@ OnMobStopAttackNotify"));

	if (GetCurrentTutorial() == ETutorialNumber::Tutorial_4)
	{
		if (!bCountAttackSuccess)
		{
			OnInputGuard();
		}
	}
}

void AB2TutorialGameMode::OnCharacterDamaged(ABladeIICharacter* VictimCharacter, float Damage)
{
	if (VictimCharacter && GetCurrentTutorialPhase())
	{
		if (GetCurrentTutorialPhase()->CompleteConditionType == ETutorialConditionType::WaitingForQTEPreparing &&
			GetCurrentTutorial() == ETutorialNumber::Tutorial_7)
		{
			SetMinoQTEState(VictimCharacter);
		}
	}

	if (bMinoAttackVibration)
	{
		StartForceFeedback();
	}
}

void AB2TutorialGameMode::SetMinoQTEState(ABladeIICharacter* VictimCharacter)
{
	if (VictimCharacter)
	{
		if (bSetMinoZombie)
		{
			VictimCharacter->Health = FMath::Clamp<float>(VictimCharacter->Health, 1, VictimCharacter->GetMaxHealth());
			VictimCharacter->SetInvincible(true);
		}

		// QTE 가능하게 하면서 탈수 있는 시간 무제한
		VictimCharacter->SetQTEEnabled(true);
		VictimCharacter->SetQTELimitEnabled(true);

		if (GetCurrentTutorialPhase()->CompleteConditionType == ETutorialConditionType::WaitingForQTEPreparing)
			ActionNextTutorial();
	}
}

void AB2TutorialGameMode::StartAutoMinoRidding()
{
	//if (BattleMainUI)
	//{
	//	BattleMainUI->SetUIEnable(false);
	//	BattleMainUI->SetAttackEnable(false);
	//	BattleMainUI->SetGuardEnable(false);
	//	BattleMainUI->SetSkillWidgetClickable(false);
	//}

	//float DelayTime = 3.0f;

	//GetWorldTimerManager().SetTimer(AutoMinoRiddingTH, this, &AB2TutorialGameMode::SetAutoMinoRidding, DelayTime, false);
}

void AB2TutorialGameMode::SetAutoMinoRidding()
{
	//GetWorldTimerManager().ClearTimer(AutoMinoRiddingTH);

	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer)
	//{
	//	TutorialPlayer->StartQTEByKeyPress();
	//}

	//OnInputQte();
}

void AB2TutorialGameMode::StartAutoUnitySkill()
{
	float DelayTime = 4.0f;

	GetWorldTimerManager().SetTimer(AutoUnitySkillTH, this, &AB2TutorialGameMode::SetAutoUnitySkill, DelayTime, false);
}

void AB2TutorialGameMode::SetAutoUnitySkill()
{
	GetWorldTimerManager().ClearTimer(AutoUnitySkillTH);

	OnInputUnitySkill();
}

bool AB2TutorialGameMode::CheckTutorialCondition(ETutorialConditionType EventType)
{
	if (IsInvalidTutorialIndex())
		return false;

	if (GetCurrentTutorialPhase() == nullptr)
		return false;

	ETutorialConditionType CurCondition = GetCurrentTutorialPhase()->CompleteConditionType;
	if (CurCondition == EventType)
	{
		return true;
	}
	return false;
}

void AB2TutorialGameMode::ShowScenarioDialog(FName DialogCodeName)
{
	OpenDialogClass<FName, bool>::GetInstance().Signal(DialogCodeName, true);
}

bool AB2TutorialGameMode::IsInvalidTutorialIndex()
{
	if (TutorialPhases.Num() < 1 ||
		TutorialPhases.Num() - 1 < TutorialMainIndex ||
		TutorialPhases[TutorialMainIndex].Phases.Num() < 1 ||
		TutorialPhases[TutorialMainIndex].Phases.Num() - 1 < TutorialSubIndex)
		return true;
	return false;
}

void AB2TutorialGameMode::OnChangedQTEEnableState(class UB2UIDocBase* Sender, EQTEUIAttackEnableTimingState CurrQTEState, EQTEUIAttackEnableTimingState PrevQTEState)
{
	return;

	//if ((GetCurrentTutorial() == ETutorialNumber::Tutorial_2 || GetCurrentTutorial() == ETutorialNumber::Tutorial_6)
	//	&& PrevQTEState == EQTEUIAttackEnableTimingState::None && CurrQTEState != EQTEUIAttackEnableTimingState::None && bTutorialQTEComplete == false)
	//{
	//	bReadyForQTEAction = true;

	//	PlayerStopAttackClass<>::GetInstance().Signal();
	//	if (BattleMainUI)
	//		BattleMainUI->SetUIEnable(false);


	//	SetTutorialPhaseInputControl(true); // Input을 Lock한다
	//	ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (LocalPlayerChar)
	//	{
	//		LocalPlayerChar->bEnableNormalAttack = false;
	//		LocalPlayerChar->SetKeepAttacking(false);
	//		LocalPlayerChar->SetSignalAttack(false);

	//		ActiveQTESuggest();
	//	}
	//}
}

void AB2TutorialGameMode::OnCounterAttackSuccessed()
{
	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer)
	//{
	//	TutorialPlayer->OnTutorialCounterActivated();

	//	if (GetCurrentTutorial() == ETutorialNumber::Tutorial_4)
	//	{
	//		float DelayTime = 0.3f;
	//		GetWorldTimerManager().SetTimer(DealyKillTH, this, &AB2TutorialGameMode::SetForceKillEnemies, DelayTime, false);

	//		TutorialPlayer->bComboEnabled = true;
	//		bCountAttackSuccess = true;
	//	}
	//}
}

void AB2TutorialGameMode::OnPlayerAttackStateChange(ABladeIITutorialPlayer* TutorialPlayer, EAttackState InAttackState)
{
	if (TutorialPlayer == nullptr)
		return;

	bool bQTETutorial = (GetCurrentTutorial() == ETutorialNumber::Tutorial_2 || GetCurrentTutorial() == ETutorialNumber::Tutorial_6) && bTutorialQTEComplete == false;
	if (bQTETutorial && bReadyForQTEAction)
	{
		if (InAttackState == EAttackState::ECS_None)
		{
			// Mob 머리위에 QTE마크가 뜰때까지 기달림
			GetWorldTimerManager().SetTimer(QTEPerfectTH, this, &AB2TutorialGameMode::ActiveQTESuggest, 0.2f, false);
		}
	}
}

void AB2TutorialGameMode::SpawnWaveEnemies()
{
	//if (GetSpawnPoolArray().Num() > 0 && GetSpawnPoolArray()[0]) // SpawnPool 여러개 설치가 가능하지만 난이도 별 튜토리얼이 있는 거 아니면 하나밖에는..
	//{
	//	// 임시 구현
	//	GetSpawnPoolArray()[0]->CustomSpawnWave(MinoSpawnWaveNumber);
	//}
}

void AB2TutorialGameMode::SetMinoRidingMatinee()
{
	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetWorld(), TEXT("MinoRidingMatinee"), *GLog);
	//}

	//// 탈것은 미리 내림
	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerController(this)->GetPawn());
	//if (TutorialPlayer && TutorialPlayer->IsInQTEState())
	//{
	//	if (TutorialPlayer->QTEType == EQTEType::EQT_Mount)
	//	{
	//		TutorialPlayer->StopQTEMount();
	//		TutorialPlayer->SetActorHiddenInGame(true);
	//	}
	//}
}

void AB2TutorialGameMode::DisableBlockingComponent()
{
	if (GEngine && GLog)
	{
		GEngine->HandleCeCommand(GetWorld(), TEXT("DisableBlockingComponent"), *GLog);
	}
}

void AB2TutorialGameMode::SetNextPlayerClass(EPCClass PCClass)
{
	BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(PCClass);
}

const FString AB2TutorialGameMode::GetClosingMovieName()
{
	return ConvertToSafeLocalizedMovieName(TEXT("DLC_Tutorial"));
}

void AB2TutorialGameMode::ActiveQTESuggest()
{
	//GetWorldTimerManager().ClearTimer(QTEPerfectTH);

	//SetTutorialPhaseInputControl(false);

	//// Skill은 막자
	//if (BattleMainUI)
	//	BattleMainUI->SetUIEnable(false);


	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer)
	//	TutorialPlayer->bEnableNormalAttack = true;

	//BattleMainUI->SetQTEWidgetState(ETutorialWidgetState::Suggest);

	//TArray<ABladeIICharacter*> AllMobs;
	//GetAllWorldMobList(AllMobs);
	////강제로 모든 몹 QTE상태로 만듬
	//for (ABladeIICharacter* EnemyCharacter : AllMobs)
	//	if (EnemyCharacter && EnemyCharacter->IsAlive())
	//		EnemyCharacter->SetQTEEnabled(true);

	////	PauseTutorialGame();
	//bSuggestQTEAction = true;
	//bReadyForQTEAction = false;

	//if (GetCurrentTutorialPhase() && GetCurrentTutorialPhase()->CompleteConditionType == ETutorialConditionType::WaitingForQTESuggest)
	//	ActionNextTutorial();
}

FTutorialPhaseData* AB2TutorialGameMode::GetCurrentTutorialPhase()
{
	if (IsInvalidTutorialIndex() == false)
	{
		if (TutorialPhases.IsValidIndex(TutorialMainIndex) &&
			TutorialPhases[TutorialMainIndex].Phases.IsValidIndex(TutorialSubIndex))
		{
			return &TutorialPhases[TutorialMainIndex].Phases[TutorialSubIndex];
		}
	}

	return nullptr;
}

void AB2TutorialGameMode::SetTutorialPhaseInputControl(bool bLockInput)
{
	/*ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	ABladeIIPlayerController* LocalPlayerController = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));

	if (LocalPlayerChar && LocalPlayerController)
	{
		if (bLockInput)
			LocalPlayerChar->DisableInput(LocalPlayerController);
		else
			LocalPlayerChar->EnableInput(LocalPlayerController);
	}*/
}

void AB2TutorialGameMode::PauseTutorialGame()
{
	if (UGameplayStatics::IsGamePaused(this) == false)
		UGameplayStatics::SetGamePaused(this, true);
}

void AB2TutorialGameMode::ResumeTutorialGame()
{
	if (UGameplayStatics::IsGamePaused(this) == true)
		UGameplayStatics::SetGamePaused(this, false);
}

void AB2TutorialGameMode::StartSlowTime(float SlowSpeed, float DuringTime)
{
	UGameplayStatics::SetGlobalTimeDilation(this, SlowSpeed);

	GetWorldTimerManager().ClearTimer(SlowTimeTH);
	GetWorldTimerManager().SetTimer(SlowTimeTH, this, &AB2TutorialGameMode::StopSlowTime, SlowSpeed * DuringTime, false);
}

void AB2TutorialGameMode::StopSlowTime()
{
	GetWorldTimerManager().ClearTimer(SlowTimeTH);

	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
}

void AB2TutorialGameMode::SetForceKillEnemies()
{
	GetWorldTimerManager().ClearTimer(DealyKillTH);

	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(GetWorld()));
	/*
	if (B2PC)
	{
	B2PC->KillAllEnemies();
	}
	*/
}

void AB2TutorialGameMode::SetDirectPlayerTag()
{
	SetOpenSkillButton(EPCClass::EPC_Gladiator, WEAPON_6STAR_SKILL_INDEX, false);
	SetOpenSkillButton(EPCClass::EPC_Wizard, WEAPON_6STAR_SKILL_INDEX, false);

	//PlayerStartTagClass<>::GetInstance().Signal();
}

int32 AB2TutorialGameMode::GetClientStageId()
{
	return 9001;
}

int32 AB2TutorialGameMode::GetStageDifficulty()
{
	return (int32)EStageDifficulty::ESD_Normal;
}

void AB2TutorialGameMode::GetFullStackSkillId(EPCClass InCharacterClass, TArray<int32>& OutSkillArray)
{
	// 신규 튜토리얼 스킬 사용하는 클래스 Fighter 와 Assassin 만 스킬 사용.. (?)
	// 여기서 OutSkillArray 에 담아주는 SkillAnim 들은 로딩 대상이 되므로 필요없는 건 제외하는 게 좋다.
	if (InCharacterClass == EPCClass::EPC_Fighter || InCharacterClass == EPCClass::EPC_Assassin)
	{
		int32 SkillNum1 = -1, SkillNum2 = -1, SkillNum3 = -1, UnitySkillNum = -1;

		int32 SkillNumForClass = (int32)InCharacterClass + 1;

		SkillNum1 = (SkillNumForClass * 10000) + 0;
		SkillNum2 = (SkillNumForClass * 10000) + 2;
		SkillNum3 = (SkillNumForClass * 10000) + 4;
		UnitySkillNum = (SkillNumForClass * 100) + 1;

		OutSkillArray.Empty();

		OutSkillArray.Add(SkillNum1);
		OutSkillArray.Add(SkillNum2);
		OutSkillArray.Add(SkillNum3);
		OutSkillArray.Add(UnitySkillNum);
	}
}

void AB2TutorialGameMode::GetPCPartsForTutorialPlay(EPCClass InClass, TArray<FB2Item>& OutItemData, FB2Wing& OutWingData)
{
	// 6 성 아이템 깔맞춤
	if (InClass != EPCClass::EPC_Assassin)
	{
		GetStandardDummyPCPartItemData(InClass, 6, OutItemData);
	}
	else
	{
		// 어새신은 검은색 장비
		GetStandardDummyPCPartItemData(InClass, 6, OutItemData, 1);
	}

	// 날개 안 씀
	OutWingData = FB2Wing();
	OutWingData.bShouldBeVisible = false;
}

ABladeIIPlayer* AB2TutorialGameMode::GetFallbackPuppetPlayer(EPCClass InCharClass)
{
	ABladeIITutorialFallbackPuppetPlayer** FoundCache = FallbackPuppets.Find(InCharClass);
	return FoundCache ? *FoundCache : nullptr;
}

void AB2TutorialGameMode::PreBuildCharacterMeshes()
{
	//TSubclassOf<APawn> DefaultPawnClassBackup = DefaultPawnClass;
	//// 더미들 spawn 시켜야 해서 잠깐 딴 클래스로 바꿔놓고..
	//DefaultPawnClass = ABladeIITutorialFallbackPuppetPlayer::StaticClass();

	//// 모든 캐릭터 mesh 다 빌드해 놓음. 여기는 B2CompositeMeshCache 의 RootSet 처리와 상관없이 
	//// 장착 장비가 다를 것이므로 게임모드 차원에서 미리 빌드가 필요함.
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	EPCClass ThisCharClass = IntToPCClass(PCI);

	//	TArray<FB2Item> ThisCharEquipItems;
	//	FB2Wing ThisCharWing;
	//	GetPCPartsForTutorialPlay(ThisCharClass, ThisCharEquipItems, ThisCharWing);

	//	// 여기서 InGameOnlyInfo 를 로딩하지는 않는다.
	//	PrebuildAndCacheCharMesh(ThisCharClass, ThisCharEquipItems, &ThisCharWing,
	//		false // 일단 기본적으로 섹션 나눠진 일반 모델로.
	//	);

	//	// 섹션 병합된 모델을 사용할 가능성을 생각하기는 어렵지만.. 코드는 갖춰두고.
	//	if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	//	{
	//		PrebuildAndCacheCharMesh(ThisCharClass, ThisCharEquipItems, &ThisCharWing, true);
	//	}

	//	// 추가로 연출 셋업 시 사용할 더미들도 생성해 놓는다. Async 로딩을 할 AnimBP 로딩이 없도록 하는 게 중요.
	//	FTransform FallbackPuppetTransform(FVector(100000.0f, 100000.0f, 100.0f * (float)PCI));
	//	ABladeIITutorialFallbackPuppetPlayer* SpawnedFallbackPuppet = Cast<ABladeIITutorialFallbackPuppetPlayer>(
	//		ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, ThisCharClass, FallbackPuppetTransform, false));
	//	if (SpawnedFallbackPuppet)
	//	{
	//		FallbackPuppets.Add(ThisCharClass, SpawnedFallbackPuppet);
	//	}
	//}

	//DefaultPawnClass = DefaultPawnClassBackup;

	//// GameMode 의 멤버 변수로 처음 Spawn 시킬 PCClass 를 명시하는 좀 바보같은 구조 때문에 이걸 다시 불러주어야 함.
	//SetDesiredPCClassEnum(); // 왜 이렇게 만들었었는지 잘 기억이 안나는데 구조가 이렇게 복잡해질 줄 몰랐겠지..
}

void AB2TutorialGameMode::PreBuildTagCharMesh()
{
	// 상위 클래스인 StageGameMode 에서 실행되는 기능으로 여기서는 아무것도 하지 않도록 해야 한다. 대신 PreBuildCharacterMeshes 에서 처리.
}

TArray<EPCClass> AB2TutorialGameMode::GetPCClassesToPreLoad()
{
	// 기본 Pre-render 로직으로는 Pre-render 처리가 안 된 채로 전투 이펙트를 사용하게 될 문제가 있는데 이런 구성을 위해서라도 이거 시작 전에 Pre-render 를 별도로 처리하게 될 것.
	TArray<EPCClass> RetArray;
	RetArray.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	return RetArray;
#if 0
	// Async 로딩을 안 쓰려면 전체 클래스를 한번에 로딩하도록 여기서 리턴하면 됨.
	TArray<EPCClass> RetArray;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
#endif
}
TArray<EPCClass> AB2TutorialGameMode::GetPCClassesToMatineePreLoad()
{ // 첫 PCClass 외엔 async 로딩을 하더라도 연출 셋업은 모두 Preload 해야 한다. 기본 GetPCClassesToPreLoad 와 다르게 가는 경우.
	TArray<EPCClass> RetArray;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}
void AB2TutorialGameMode::MarkBeginAsyncLoadNextPCClass()
{
	// 다음 캐릭터 spawn 및 교체 연출 나오는 시점에 그 다음 캐릭터 데이터 async 로딩을 시작할 목적으로 이걸 콜한다.

	if (PCClassAsyncLoadOrder.Num() > 0)
	{
		// SetNextPlayerClass 호출 시점이 Async 로딩에 필요한 시점보다 느려서 별도로 순서 지정하는 배열을 만들어서 여기서 넣게 되었음.
		// 이것 외에 다른 식으로 지정할 계획이 있으면 이건 폐기..
		BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(PCClassAsyncLoadOrder[0]); // 굳이 이걸 SubPlayerClass 로 세팅하는 이유는 의미상 적당히 말이 되기도 하지만 이렇게 해야 마지막에 어쌔신이 세팅되고 기본 선택 클래스가 되기 때문이지 낄낄
		PCClassAsyncLoadOrder.RemoveAt(0);

		PendingAsyncLoadRequestPCClass = BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass();
	}
}
void AB2TutorialGameMode::BeginAsyncLoadNextPCClass()
{
	checkSlow(PendingAsyncLoadRequestPCClass != EPCClass::EPC_End);

	// 매번의 다음 클래스 로딩 전에 지금까지 로딩되어 있는 것들은 날리도록 한다. 현재 캐릭터 데이터는 이미 spawn 된 이후에는 필요없어야.
	// iOS 를 위해서이고 Android 에서 ManagedPCClassUnload 사용하고 있으면 루트셋 처리 때문에 언로딩 안 될 듯..
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		PCBox->UnloadAllOrExceptSelectedIfAllowed();
	}

	BeginAsyncLoadPCClass(PendingAsyncLoadRequestPCClass);

	PendingAsyncLoadRequestPCClass = EPCClass::EPC_End;
}
void AB2TutorialGameMode::BeginAsyncLoadPCClass(EPCClass InClassToReqLoad)
{
	//if (InClassToReqLoad != EPCClass::EPC_End)
	//{
	//	// FlushAsyncLoading 을 유발할 수 있는 것들을 미리 해 놓아야 한다.
	//	StaticFindPCClassInfoBox(this);
	//	StaticFindSkillAnimInfo(this);
	//	StaticFindDamageEffectInfo(this);

	//	//
	//	// 현재는 PCClassInfo 의 InGameOnlyInfo (AnimBP) 와 SkillAnim 들을 Async 로딩 요청하는데 
	//	// 중간에 flush 가 될 수도 있고 해서 너무 많이는 못할 수도.
	//	//

	//	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//	UB2PCClassInfo* ThisLoadedInfo = PCBox ? PCBox->GetSingleClassInfo(InClassToReqLoad) : nullptr;

	//	TArray<FCombinedPCSkillAnimID> CombinedSkillAnimIDToReq;
	//	TArray<ESkillAnimType> SkillAnimsOfThisClass;
	//	UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, InClassToReqLoad, GetB2GameModeType(), SkillAnimsOfThisClass, &BladeIIGameImpl::GetLocalCharacterData());
	//	for (ESkillAnimType ThisSkillAnimType : SkillAnimsOfThisClass)
	//	{
	//		CombinedSkillAnimIDToReq.Add(FCombinedPCSkillAnimID(InClassToReqLoad, ThisSkillAnimType));
	//	}

	//	// 실제 Async 로딩 요청은 맨 아래에서.. 이래도 물론 중간에 flush 거리가 생기면 도루묵이겠지만.
	//	if (ThisLoadedInfo)
	//	{ // 이건 우선순위를 높게 준다. 대체로 캐릭터 별 도입부 연출 도중에 로딩 완료를 의도함. (글쎄 잘 될까..)
	//		ThisLoadedInfo->ReqAsyncLoad(this, FStreamableManager::AsyncLoadHighPriority);
	//	}
	//	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(this);
	//	if (SkillAnimInfo)
	//	{
	//		TArray<FB2AsyncRequestInfo> RequestAssets = SkillAnimInfo->GetAsyncRequestAssetList(CombinedSkillAnimIDToReq);
	//		FString RequestName = FString::Printf(TEXT("%d_SkillAnims"), static_cast<int32>(InClassToReqLoad));
	//		SkillAnimInfo->TryAsyncLoad(RequestName, RequestAssets);
	//	}
	//}
}

#include "B2Inventory.h"

ABladeIITutorialPlayer::ABladeIITutorialPlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bEnableAttack = true;
	bEnableGuard = true;
	bWaitingCounterGuard = false;
	bWaitingMoveInput = true;
}

void ABladeIITutorialPlayer::InitializeCombatStats()
{
	Super::InitializeCombatStats();

	if (auto* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		TotalAttack = TutorialGM->PCTotalAttack;
		TotalDefense = TutorialGM->PCTotalDefense;
		MaxHealth = TutorialGM->PCHealth;
		Health = MaxHealth;
	}
}

bool ABladeIITutorialPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	return false; // 응 안죽어
}

float ABladeIITutorialPlayer::GetScaledAttackLevel()
{
	if (auto* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		return TutorialGM->ScaledAttackLevel;
	}

	return 0.0f;
}

void ABladeIITutorialPlayer::SetAttackState(EAttackState InAttackState)
{
	Super::SetAttackState(InAttackState);

	if (auto* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		TutorialGM->OnPlayerAttackStateChange(this, InAttackState);
	}
}

void ABladeIITutorialPlayer::StartAttack()
{
	if (!bEnableAttack)
		return;

	Super::StartAttack();
}

void ABladeIITutorialPlayer::StopAttack()
{
	Super::StopAttack();
}

void ABladeIITutorialPlayer::StartGuard(bool bFromInput/* = false*/)
{
	if (!bEnableGuard)
		return;

	Super::StartGuard(bFromInput);
}

void ABladeIITutorialPlayer::StopGuard(bool bForce/* = false*/)
{
	Super::StopGuard(bForce);
}

void ABladeIITutorialPlayer::StartWeaponSkill()
{
	return;
}

void ABladeIITutorialPlayer::AddMovementInput(FVector WorldDirection, float ScaleValue /*= 1.0f*/, bool bForce /*= false*/)
{
	if (bWaitingMoveInput)
	{
		if (auto* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			TutorialGM->OnInputMove();
			bWaitingMoveInput = false;
		}
	}

	else
	{
		Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	}
}

float ABladeIITutorialPlayer::GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds)
{
	return 0.0f;
}

void ABladeIITutorialPlayer::AddWeaponSkillPoint(float ToAddPoint)
{
	return;
}

void ABladeIITutorialPlayer::StopQTEMount()
{
	SetActorEnableCollision(true);

	if (QTEActor && QTEActor->IsAlive())
	{
		QTEActor->SetActorEnableCollision(false);
		QTEActor->GetMovementComponent()->StopMovementImmediately();
		QTEActor->SetActorHiddenInGame(true);
		OnQTEMountTargetEnd(QTEActor->GetMesh()->GetSocketTransform(ABladeIIPlayer::QTESocketName));
	}

	if (IsLocalPlayer())
	{
		auto* DocBattle = GetDocBattle();
		if (DocBattle)
		{
			DocBattle->SetbIsQTEMounted(false);
			DocBattle->SetbIsQTEMountedAndReadyForAttack(false/*this->IsQTEMountingAndReadyForAttack()*/);
		}
	}
}

void ABladeIITutorialPlayer::ActiveTutorialCounterGuard()
{
	bWaitingCounterGuard = true;
}

void ABladeIITutorialPlayer::OnTutorialCounterActivated()
{
	bWaitingCounterGuard = false;
	CounterCheckTime = 1.f;
}

void ABladeIITutorialPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	checkSlow(!bMergeSections); /* Tutorial 에 저퀄리티 MergeSection 모델을 쓸 일은 아마 없을 듯 해서.. */

	TArray<FB2Item> TutorialDummyItems;
	FB2Wing WingData;

	EPCClass CurrentPCClass = GetCurrentPlayerClass();
	AB2TutorialGameMode::GetPCPartsForTutorialPlay(CurrentPCClass, TutorialDummyItems, WingData);

	SetupComponentsForPartsCommon(TutorialDummyItems, &WingData, bMergeSections, false, OutGetBuiltResultOnly);
}

ICharacterDataStore* ABladeIITutorialPlayer::GetCharacterDataStore()
{
	//여기로 오는 것은 Local용 Player character로 가정한다.
	if (CharacterDataStore == nullptr)
		CharacterDataStore = &BladeIIGameImpl::GetTutorialCharacterData();

	return CharacterDataStore;
}

ABladeIITutorialFallbackPuppetPlayer::ABladeIITutorialFallbackPuppetPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}
void ABladeIITutorialFallbackPuppetPlayer::BeginPlay()
{
	//Super::BeginPlay();

	//// 멀리 spawn 을 해 놓긴 했지만..
	//// 렌더링에 사용할 거 아님 성능에 방해가 될 수 있는 요인은 최대한 차단.
	//USkeletalMeshComponent* TheMeshComp = GetMesh();
	//if (TheMeshComp)
	//{
	//	TheMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	//	TheMeshComp->CastShadow = false;
	//	TheMeshComp->SetVisibility(false);
	//}
	//if (CenterShadow) // Decal fake shadow
	//{
	//	CenterShadow->SetVisibility(false);
	//}

	//SetActorEnableCollision(false);

	//// 연출을 거치다 보면 Hidden 이 풀리게 될 수 있는데.. 그래도 SetVisibility 도 했고 기본 spawn 자체도 저 멀리 놔뒀으니 어지간해서는 괜찮을 거.
	//SetActorHiddenInGame(true);
}
void ABladeIITutorialFallbackPuppetPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	checkSlow(!bMergeSections); /* Tutorial 에 저퀄리티 MergeSection 모델을 쓸 일은 아마 없을 듯 해서.. */

	TArray<FB2Item> TutorialDummyItems;
	FB2Wing WingData;

	EPCClass CurrentPCClass = GetCurrentPlayerClass();
	// ABladeIITutorialPlayer 와 마찬가지 장비를 장착하도록 동일한 함수 사용.
	AB2TutorialGameMode::GetPCPartsForTutorialPlay(CurrentPCClass, TutorialDummyItems, WingData);

	SetupComponentsForPartsCommon(TutorialDummyItems, &WingData, bMergeSections, false, OutGetBuiltResultOnly);
}
void ABladeIITutorialFallbackPuppetPlayer::SubscribeEvents()
{
	// 아무것도 하지 않도록
}
void ABladeIITutorialFallbackPuppetPlayer::UnsubscribeEvents()
{
	// 아무것도 하지 않도록
}
bool ABladeIITutorialFallbackPuppetPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return false;
}
bool ABladeIITutorialFallbackPuppetPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	return false;
}