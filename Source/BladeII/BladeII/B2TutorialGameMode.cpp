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
	// Async �ε� �����ε� ������ �̰� Ʃ�丮�� �帧�� ���󰡾� ��. 
	// ���� SetNextPlayerClass ȣ�� ������ Async �ε��� �ʿ��� ������ �޶� ���� �ְ� �Ǿ���.
	// �̰� �ܿ� �ٸ� ������ ������ ��ȹ�� ������ �̰� ���..
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

	// PCClassData �� ������ �κ��� Async �ε��ϸ鼭 �ٸ� �͵��� Preload ���� ���� �ǹǷ� (GetPCClassesToPreLoad ���� ���� ��� ����)
	// async �ε� ����� �ƴ� �͵��� ���⼭ ���� �ε��Ѵ�. ũ���� �ʰ�����
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		TArray<EPCClass> ClassesToLoad;
		// �̰� ���� �ð� �� ���� ���̰� PCClassInfo �߿��� ���� ��� InGameOnlyInfo �� SkillAnim �� ���� ���� ��. �װ� async �ε� �� ����.
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
	// ��ġ ū PCClass �� Async �ε��ϹǷ� NPC ���� Async ����̾��� �͵鵵 ��� �̸� �ε��� ��.
	//GameContextAsyncFlush();

	Super::BeginPlay();

	MarkBeginAsyncLoadNextPCClass(); // BeginPlay ������ �ι�° ���� ĳ���� �����Ϳ� ���� �ε� ��û�� ��. ���Ŀ��� ������ ��ü ���İ� �� ��..
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
	//// BattleMain ��� X
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
	//// BeginDestroy ���������� UIWidget destroy �� ������ �ʿ䰡 ����.
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
	////�׼� ��ǥ �α� (Ʃ�丮�� �Ϸ�)
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

	//// ���� ������ �ٷ� ���� �ܰ� ����
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
	//		// Open�Ҷ� suggest
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

	//// ���� Attack
	//else if (Number == ETutorialNumber::Tutorial_2)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//	}
	//}

	//// ĳ���� ����Ī (������ -> ������)
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

	//// ���� �ݰ�
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

	//// ĳ���� ����Ī (������ -> ������)
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

	//// ĳ���� ����Ī (������ -> �ϻ���)
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

	//// �̳� QTE ����
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

	//		SetTutorialPhaseInputControl(false); // Input�� Ǯ��� Guard�� ����
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

		// QTE �����ϰ� �ϸ鼭 Ż�� �ִ� �ð� ������
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


	//	SetTutorialPhaseInputControl(true); // Input�� Lock�Ѵ�
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
			// Mob �Ӹ����� QTE��ũ�� �㶧���� ��޸�
			GetWorldTimerManager().SetTimer(QTEPerfectTH, this, &AB2TutorialGameMode::ActiveQTESuggest, 0.2f, false);
		}
	}
}

void AB2TutorialGameMode::SpawnWaveEnemies()
{
	//if (GetSpawnPoolArray().Num() > 0 && GetSpawnPoolArray()[0]) // SpawnPool ������ ��ġ�� ���������� ���̵� �� Ʃ�丮���� �ִ� �� �ƴϸ� �ϳ��ۿ���..
	//{
	//	// �ӽ� ����
	//	GetSpawnPoolArray()[0]->CustomSpawnWave(MinoSpawnWaveNumber);
	//}
}

void AB2TutorialGameMode::SetMinoRidingMatinee()
{
	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetWorld(), TEXT("MinoRidingMatinee"), *GLog);
	//}

	//// Ż���� �̸� ����
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

	//// Skill�� ����
	//if (BattleMainUI)
	//	BattleMainUI->SetUIEnable(false);


	//ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (TutorialPlayer)
	//	TutorialPlayer->bEnableNormalAttack = true;

	//BattleMainUI->SetQTEWidgetState(ETutorialWidgetState::Suggest);

	//TArray<ABladeIICharacter*> AllMobs;
	//GetAllWorldMobList(AllMobs);
	////������ ��� �� QTE���·� ����
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
	// �ű� Ʃ�丮�� ��ų ����ϴ� Ŭ���� Fighter �� Assassin �� ��ų ���.. (?)
	// ���⼭ OutSkillArray �� ����ִ� SkillAnim ���� �ε� ����� �ǹǷ� �ʿ���� �� �����ϴ� �� ����.
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
	// 6 �� ������ �����
	if (InClass != EPCClass::EPC_Assassin)
	{
		GetStandardDummyPCPartItemData(InClass, 6, OutItemData);
	}
	else
	{
		// ������� ������ ���
		GetStandardDummyPCPartItemData(InClass, 6, OutItemData, 1);
	}

	// ���� �� ��
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
	//// ���̵� spawn ���Ѿ� �ؼ� ��� �� Ŭ������ �ٲ����..
	//DefaultPawnClass = ABladeIITutorialFallbackPuppetPlayer::StaticClass();

	//// ��� ĳ���� mesh �� ������ ����. ����� B2CompositeMeshCache �� RootSet ó���� ������� 
	//// ���� ��� �ٸ� ���̹Ƿ� ���Ӹ�� �������� �̸� ���尡 �ʿ���.
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	EPCClass ThisCharClass = IntToPCClass(PCI);

	//	TArray<FB2Item> ThisCharEquipItems;
	//	FB2Wing ThisCharWing;
	//	GetPCPartsForTutorialPlay(ThisCharClass, ThisCharEquipItems, ThisCharWing);

	//	// ���⼭ InGameOnlyInfo �� �ε������� �ʴ´�.
	//	PrebuildAndCacheCharMesh(ThisCharClass, ThisCharEquipItems, &ThisCharWing,
	//		false // �ϴ� �⺻������ ���� ������ �Ϲ� �𵨷�.
	//	);

	//	// ���� ���յ� ���� ����� ���ɼ��� �����ϱ�� �������.. �ڵ�� ����ΰ�.
	//	if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	//	{
	//		PrebuildAndCacheCharMesh(ThisCharClass, ThisCharEquipItems, &ThisCharWing, true);
	//	}

	//	// �߰��� ���� �¾� �� ����� ���̵鵵 ������ ���´�. Async �ε��� �� AnimBP �ε��� ������ �ϴ� �� �߿�.
	//	FTransform FallbackPuppetTransform(FVector(100000.0f, 100000.0f, 100.0f * (float)PCI));
	//	ABladeIITutorialFallbackPuppetPlayer* SpawnedFallbackPuppet = Cast<ABladeIITutorialFallbackPuppetPlayer>(
	//		ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, ThisCharClass, FallbackPuppetTransform, false));
	//	if (SpawnedFallbackPuppet)
	//	{
	//		FallbackPuppets.Add(ThisCharClass, SpawnedFallbackPuppet);
	//	}
	//}

	//DefaultPawnClass = DefaultPawnClassBackup;

	//// GameMode �� ��� ������ ó�� Spawn ��ų PCClass �� ����ϴ� �� �ٺ����� ���� ������ �̰� �ٽ� �ҷ��־�� ��.
	//SetDesiredPCClassEnum(); // �� �̷��� ����������� �� ����� �ȳ��µ� ������ �̷��� �������� �� ��������..
}

void AB2TutorialGameMode::PreBuildTagCharMesh()
{
	// ���� Ŭ������ StageGameMode ���� ����Ǵ� ������� ���⼭�� �ƹ��͵� ���� �ʵ��� �ؾ� �Ѵ�. ��� PreBuildCharacterMeshes ���� ó��.
}

TArray<EPCClass> AB2TutorialGameMode::GetPCClassesToPreLoad()
{
	// �⺻ Pre-render �������δ� Pre-render ó���� �� �� ä�� ���� ����Ʈ�� ����ϰ� �� ������ �ִµ� �̷� ������ ���ؼ��� �̰� ���� ���� Pre-render �� ������ ó���ϰ� �� ��.
	TArray<EPCClass> RetArray;
	RetArray.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	return RetArray;
#if 0
	// Async �ε��� �� ������ ��ü Ŭ������ �ѹ��� �ε��ϵ��� ���⼭ �����ϸ� ��.
	TArray<EPCClass> RetArray;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
#endif
}
TArray<EPCClass> AB2TutorialGameMode::GetPCClassesToMatineePreLoad()
{ // ù PCClass �ܿ� async �ε��� �ϴ��� ���� �¾��� ��� Preload �ؾ� �Ѵ�. �⺻ GetPCClassesToPreLoad �� �ٸ��� ���� ���.
	TArray<EPCClass> RetArray;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}
void AB2TutorialGameMode::MarkBeginAsyncLoadNextPCClass()
{
	// ���� ĳ���� spawn �� ��ü ���� ������ ������ �� ���� ĳ���� ������ async �ε��� ������ �������� �̰� ���Ѵ�.

	if (PCClassAsyncLoadOrder.Num() > 0)
	{
		// SetNextPlayerClass ȣ�� ������ Async �ε��� �ʿ��� �������� ������ ������ ���� �����ϴ� �迭�� ���� ���⼭ �ְ� �Ǿ���.
		// �̰� �ܿ� �ٸ� ������ ������ ��ȹ�� ������ �̰� ���..
		BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(PCClassAsyncLoadOrder[0]); // ���� �̰� SubPlayerClass �� �����ϴ� ������ �ǹ̻� ������ ���� �Ǳ⵵ ������ �̷��� �ؾ� �������� ��ؽ��� ���õǰ� �⺻ ���� Ŭ������ �Ǳ� �������� ����
		PCClassAsyncLoadOrder.RemoveAt(0);

		PendingAsyncLoadRequestPCClass = BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass();
	}
}
void AB2TutorialGameMode::BeginAsyncLoadNextPCClass()
{
	checkSlow(PendingAsyncLoadRequestPCClass != EPCClass::EPC_End);

	// �Ź��� ���� Ŭ���� �ε� ���� ���ݱ��� �ε��Ǿ� �ִ� �͵��� �������� �Ѵ�. ���� ĳ���� �����ʹ� �̹� spawn �� ���Ŀ��� �ʿ�����.
	// iOS �� ���ؼ��̰� Android ���� ManagedPCClassUnload ����ϰ� ������ ��Ʈ�� ó�� ������ ��ε� �� �� ��..
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
	//	// FlushAsyncLoading �� ������ �� �ִ� �͵��� �̸� �� ���ƾ� �Ѵ�.
	//	StaticFindPCClassInfoBox(this);
	//	StaticFindSkillAnimInfo(this);
	//	StaticFindDamageEffectInfo(this);

	//	//
	//	// ����� PCClassInfo �� InGameOnlyInfo (AnimBP) �� SkillAnim ���� Async �ε� ��û�ϴµ� 
	//	// �߰��� flush �� �� ���� �ְ� �ؼ� �ʹ� ���̴� ���� ����.
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

	//	// ���� Async �ε� ��û�� �� �Ʒ�����.. �̷��� ���� �߰��� flush �Ÿ��� ����� ���繬�̰�����.
	//	if (ThisLoadedInfo)
	//	{ // �̰� �켱������ ���� �ش�. ��ü�� ĳ���� �� ���Ժ� ���� ���߿� �ε� �ϷḦ �ǵ���. (�۽� �� �ɱ�..)
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
	return false; // �� ���׾�
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
	checkSlow(!bMergeSections); /* Tutorial �� ������Ƽ MergeSection ���� �� ���� �Ƹ� ���� �� �ؼ�.. */

	TArray<FB2Item> TutorialDummyItems;
	FB2Wing WingData;

	EPCClass CurrentPCClass = GetCurrentPlayerClass();
	AB2TutorialGameMode::GetPCPartsForTutorialPlay(CurrentPCClass, TutorialDummyItems, WingData);

	SetupComponentsForPartsCommon(TutorialDummyItems, &WingData, bMergeSections, false, OutGetBuiltResultOnly);
}

ICharacterDataStore* ABladeIITutorialPlayer::GetCharacterDataStore()
{
	//����� ���� ���� Local�� Player character�� �����Ѵ�.
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

	//// �ָ� spawn �� �� ���� ������..
	//// �������� ����� �� �ƴ� ���ɿ� ���ذ� �� �� �ִ� ������ �ִ��� ����.
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

	//// ������ ��ġ�� ���� Hidden �� Ǯ���� �� �� �ִµ�.. �׷��� SetVisibility �� �߰� �⺻ spawn ��ü�� �� �ָ� �������� �������ؼ��� ������ ��.
	//SetActorHiddenInGame(true);
}
void ABladeIITutorialFallbackPuppetPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	checkSlow(!bMergeSections); /* Tutorial �� ������Ƽ MergeSection ���� �� ���� �Ƹ� ���� �� �ؼ�.. */

	TArray<FB2Item> TutorialDummyItems;
	FB2Wing WingData;

	EPCClass CurrentPCClass = GetCurrentPlayerClass();
	// ABladeIITutorialPlayer �� �������� ��� �����ϵ��� ������ �Լ� ���.
	AB2TutorialGameMode::GetPCPartsForTutorialPlay(CurrentPCClass, TutorialDummyItems, WingData);

	SetupComponentsForPartsCommon(TutorialDummyItems, &WingData, bMergeSections, false, OutGetBuiltResultOnly);
}
void ABladeIITutorialFallbackPuppetPlayer::SubscribeEvents()
{
	// �ƹ��͵� ���� �ʵ���
}
void ABladeIITutorialFallbackPuppetPlayer::UnsubscribeEvents()
{
	// �ƹ��͵� ���� �ʵ���
}
bool ABladeIITutorialFallbackPuppetPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return false;
}
bool ABladeIITutorialFallbackPuppetPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	return false;
}