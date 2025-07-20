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
#include "../UI/Widget/B2UITutorialBattleMain.h"

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
	BattleMainUI = nullptr;
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
	// Async 肺爹 鉴辑牢单 角力肺 捞扒 譬配府倔 儒抚阑 蝶扼啊具 窃. 
	// 泅犁 SetNextPlayerClass 龋免 矫痢捞 Async 肺爹俊 鞘夸茄 矫痢苞 崔扼辑 蝶肺 持霸 登菌澜.
	// 捞芭 寇俊 促弗 侥栏肺 瘤沥且 拌裙捞 乐栏搁 捞扒 企扁..
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

	// PCClassData 吝 啊瓷茄 何盒阑 Async 肺爹窍搁辑 促弗 巴甸狼 Preload 鳖瘤 阜霸 登骨肺 (GetPCClassesToPreLoad 俊辑 府畔 措惑 临烙)
	// async 肺爹 措惑捞 酒囱 巴甸篮 咯扁辑 流立 肺爹茄促. 农瘤绰 臼摆瘤父
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		TArray<EPCClass> ClassesToLoad;
		// 捞扒 喊肺 矫埃 救 冈阑 巴捞绊 PCClassInfo 吝俊辑 蝶肺 都绢辰 InGameOnlyInfo 尔 SkillAnim 捞 腹捞 冈阑 波. 弊吧 async 肺爹 且 巴烙.
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
	// 耽摹 奴 PCClass 甫 Async 肺爹窍骨肺 NPC 甸篮 Async 措惑捞菌带 巴甸档 葛滴 固府 肺爹秦 狄.
	//GameContextAsyncFlush();

	Super::BeginPlay();

	MarkBeginAsyncLoadNextPCClass(); // BeginPlay 俊辑搁 滴锅掳 唱棵 某腐磐 单捞磐俊 措茄 肺爹 夸没捞 凳. 捞饶俊绰 阿阿狼 背眉 捞饶啊 瞪 巴..
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
	// BattleMain 荤侩 X
	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TutorialBattleMain);

	if (BattleMainUI == nullptr)
	{
		BattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorialBattleMain>(UIFName::TutorialBattleMain);
		ensure(BattleMainUI != nullptr);

		BattleMainUI->OnStageBegin();
	}

	InitTutorialUI();

	SetPlayerAutoType(AutoBattleType::NONE);
	TogglePlayerAutoStateClass<>::GetInstance().Signal();
}

void AB2TutorialGameMode::BeginDestroy()
{
	//// BeginDestroy 矫痢俊辑狼 UIWidget destroy 绰 林狼且 鞘夸啊 乐澜.
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
	////咀记 瘤钎 肺弊 (譬配府倔 肯丰)
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

	//// 炼扒 绝栏搁 官肺 促澜 窜拌 柳青
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
	//		// Open且锭 suggest
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

	//// 备匡 Attack
	//else if (Number == ETutorialNumber::Tutorial_2)
	//{
	//	if (BattleMainUI)
	//	{
	//		BattleMainUI->SetUIEnable(false);
	//		BattleMainUI->SetAttackEnable(true);
	//	}
	//}

	//// 某腐磐 胶困莫 (八捧荤 -> 付过荤)
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

	//// 籍惑 馆拜
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

	//// 某腐磐 胶困莫 (付过荤 -> 拜捧啊)
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

	//// 某腐磐 胶困莫 (拜捧啊 -> 鞠混磊)
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

	//// 固畴 QTE 捞饶
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
	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (TutorialPlayer && BattleMainUI)
	{
		BattleMainUI->SetMoveWidgetState(ETutorialWidgetState::Enable);
	}
}

void AB2TutorialGameMode::OnInputAttack()
{
	if (CheckTutorialCondition(ETutorialConditionType::InputAttack))
	{
		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_2)
		{
			if (BattleMainUI)
			{
				BattleMainUI->SetAttackWidgetState(ETutorialWidgetState::Enable);
				BattleMainUI->SetAttackEnable(true);
			}
		}
		else if (GetCurrentTutorial() == ETutorialNumber::Tutorial_7)
		{
			if (BattleMainUI)
			{
				BattleMainUI->SetMountAttackWidgetState(ETutorialWidgetState::Enable);
			}
		}

		ActionNextTutorial();
	}
}

void AB2TutorialGameMode::OnInputGuard()
{
	if (CheckTutorialCondition(ETutorialConditionType::InputGuard))
	{
		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_4)
		{
			// do something..
			if (!bReadyForCounterAction)
			{
				ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				if (TutorialPlayer)
				{
					TutorialPlayer->ForceCounterAttack();
				}

				if (BattleMainUI)
				{
					BattleMainUI->SetAttackEnable(true);
					BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Enable);
				}

				bReadyForCounterAction = true;

				SetTutorialPhaseInputControl(true);
				StopSlowTime();
			}
		}

		ActionNextTutorial();
	}
}

void AB2TutorialGameMode::OnInputSkill(int32 SkillIndex)
{
	if (CheckTutorialCondition(ETutorialConditionType::InputSKill))
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
	}
}

void AB2TutorialGameMode::OnInputQte()
{
	if (CheckTutorialCondition(ETutorialConditionType::InputQTE))
	{
		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_7)
		{
			ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			if (TutorialPlayer)
			{
				TutorialPlayer->SetEnableAttack(true);
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
	}
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

	if (bReadyForCounterAction && bTutorial4CounterAction)
	{
		ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (TutorialPlayer)
		{
			TutorialPlayer->ActiveTutorialCounterGuard();
			TutorialPlayer->StopRepeatingInput();
			TutorialPlayer->SetAttackState(EAttackState::ECS_None);

			SetTutorialPhaseInputControl(false); // Input阑 钱绢具 Guard甫 窍瘤
		}

		if (BattleMainUI)
		{
			BattleMainUI->SetGuardWidgetState(ETutorialWidgetState::Suggest);
		}

		bReadyForCounterAction = false;
		bTutorial4CounterAction = false;

		StartSlowTime(0.01f, 2.0f);
	}
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

		// QTE 啊瓷窍霸 窍搁辑 呕荐 乐绰 矫埃 公力茄
		VictimCharacter->SetQTEEnabled(true);
		VictimCharacter->SetQTELimitEnabled(true);

		if (GetCurrentTutorialPhase()->CompleteConditionType == ETutorialConditionType::WaitingForQTEPreparing)
			ActionNextTutorial();
	}
}

void AB2TutorialGameMode::StartAutoMinoRidding()
{
	if (BattleMainUI)
	{
		BattleMainUI->SetUIEnable(false);
		BattleMainUI->SetAttackEnable(false);
		BattleMainUI->SetGuardEnable(false);
		BattleMainUI->SetSkillWidgetClickable(false);
	}

	float DelayTime = 3.0f;

	GetWorldTimerManager().SetTimer(AutoMinoRiddingTH, this, &AB2TutorialGameMode::SetAutoMinoRidding, DelayTime, false);
}

void AB2TutorialGameMode::SetAutoMinoRidding()
{
	GetWorldTimerManager().ClearTimer(AutoMinoRiddingTH);

	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (TutorialPlayer)
	{
		TutorialPlayer->StartQTEByKeyPress();
	}

	OnInputQte();
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


	//	SetTutorialPhaseInputControl(true); // Input阑 Lock茄促
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
	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (TutorialPlayer)
	{
		TutorialPlayer->OnTutorialCounterActivated();

		if (GetCurrentTutorial() == ETutorialNumber::Tutorial_4)
		{
			float DelayTime = 0.3f;
			GetWorldTimerManager().SetTimer(DealyKillTH, this, &AB2TutorialGameMode::SetForceKillEnemies, DelayTime, false);

			TutorialPlayer->bComboEnabled = true;
			bCountAttackSuccess = true;
		}
	}
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
			// Mob 赣府困俊 QTE付农啊 躲锭鳖瘤 扁崔覆
			GetWorldTimerManager().SetTimer(QTEPerfectTH, this, &AB2TutorialGameMode::ActiveQTESuggest, 0.2f, false);
		}
	}
}

void AB2TutorialGameMode::SpawnWaveEnemies()
{
	if (GetSpawnPoolArray().Num() > 0 && GetSpawnPoolArray()[0]) // SpawnPool 咯矾俺 汲摹啊 啊瓷窍瘤父 抄捞档 喊 譬配府倔捞 乐绰 芭 酒聪搁 窍唱观俊绰..
	{
		// 烙矫 备泅
		GetSpawnPoolArray()[0]->CustomSpawnWave(MinoSpawnWaveNumber);
	}
}

void AB2TutorialGameMode::SetMinoRidingMatinee()
{
	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetWorld(), TEXT("MinoRidingMatinee"), *GLog);
	//}

	//// 呕巴篮 固府 郴覆
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
	GetWorldTimerManager().ClearTimer(QTEPerfectTH);

	SetTutorialPhaseInputControl(false);

	// Skill篮 阜磊
	if (BattleMainUI)
		BattleMainUI->SetUIEnable(false);


	ABladeIITutorialPlayer* TutorialPlayer = Cast<ABladeIITutorialPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (TutorialPlayer)
		TutorialPlayer->SetEnableAttack(true);

	BattleMainUI->SetQTEWidgetState(ETutorialWidgetState::Suggest);

	TArray<ABladeIICharacter*> AllMobs;
	GetAllWorldMobList(AllMobs);
	//碍力肺 葛电 各 QTE惑怕肺 父惦
	for (ABladeIICharacter* EnemyCharacter : AllMobs)
		if (EnemyCharacter && EnemyCharacter->IsAlive())
			EnemyCharacter->SetQTEEnabled(true);

	//	PauseTutorialGame();
	bSuggestQTEAction = true;
	bReadyForQTEAction = false;

	if (GetCurrentTutorialPhase() && GetCurrentTutorialPhase()->CompleteConditionType == ETutorialConditionType::WaitingForQTESuggest)
		ActionNextTutorial();
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
	ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABladeIIPlayerController* LocalPlayerController = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());

	if (LocalPlayerChar && LocalPlayerController)
	{
		if (bLockInput)
			LocalPlayerChar->DisableInput(LocalPlayerController);
		else
			LocalPlayerChar->EnableInput(LocalPlayerController);
	}
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

	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (B2PC)
	{
		B2PC->KillAllEnemies();
	}

}

void AB2TutorialGameMode::SetDirectPlayerTag()
{
	SetOpenSkillButton(EPCClass::EPC_Gladiator, WEAPON_6STAR_SKILL_INDEX, false);
	SetOpenSkillButton(EPCClass::EPC_Wizard, WEAPON_6STAR_SKILL_INDEX, false);

	PlayerStartTagClass<>::GetInstance().Signal();
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
	// 脚痹 譬配府倔 胶懦 荤侩窍绰 努贰胶 Fighter 客 Assassin 父 胶懦 荤侩.. (?)
	// 咯扁辑 OutSkillArray 俊 淬酒林绰 SkillAnim 甸篮 肺爹 措惑捞 登骨肺 鞘夸绝绰 扒 力寇窍绰 霸 亮促.
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
	// 6 己 酒捞袍 彬嘎勉
	if (InClass != EPCClass::EPC_Assassin)
	{
		GetStandardDummyPCPartItemData(InClass, 6, OutItemData);
	}
	else
	{
		// 绢货脚篮 八篮祸 厘厚
		GetStandardDummyPCPartItemData(InClass, 6, OutItemData, 1);
	}

	// 朝俺 救 靖
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
	TSubclassOf<APawn> DefaultPawnClassBackup = DefaultPawnClass;
	// 歹固甸 spawn 矫难具 秦辑 泪瘪 谍 努贰胶肺 官层初绊..
	DefaultPawnClass = ABladeIITutorialFallbackPuppetPlayer::StaticClass();

	// 葛电 某腐磐 mesh 促 呼靛秦 初澜. 咯扁绰 B2CompositeMeshCache 狼 RootSet 贸府客 惑包绝捞 
	// 厘馒 厘厚啊 促甫 巴捞骨肺 霸烙葛靛 瞒盔俊辑 固府 呼靛啊 鞘夸窃.
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass ThisCharClass = IntToPCClass(PCI);

		TArray<FB2Item> ThisCharEquipItems;
		FB2Wing ThisCharWing;
		GetPCPartsForTutorialPlay(ThisCharClass, ThisCharEquipItems, ThisCharWing);

		// 咯扁辑 InGameOnlyInfo 甫 肺爹窍瘤绰 臼绰促.
		PrebuildAndCacheCharMesh(ThisCharClass, ThisCharEquipItems, &ThisCharWing,
			false // 老窜 扁夯利栏肺 冀记 唱床柳 老馆 葛胆肺.
		);

		// 冀记 捍钦等 葛胆阑 荤侩且 啊瓷己阑 积阿窍扁绰 绢菲瘤父.. 内靛绰 爱苗滴绊.
		if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
		{
			PrebuildAndCacheCharMesh(ThisCharClass, ThisCharEquipItems, &ThisCharWing, true);
		}

		// 眠啊肺 楷免 悸诀 矫 荤侩且 歹固甸档 积己秦 初绰促. Async 肺爹阑 且 AnimBP 肺爹捞 绝档废 窍绰 霸 吝夸.
		FTransform FallbackPuppetTransform(FVector(100000.0f, 100000.0f, 100.0f * (float)PCI));
		ABladeIITutorialFallbackPuppetPlayer* SpawnedFallbackPuppet = Cast<ABladeIITutorialFallbackPuppetPlayer>(
			ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, ThisCharClass, FallbackPuppetTransform, false));
		if (SpawnedFallbackPuppet)
		{
			FallbackPuppets.Add(ThisCharClass, SpawnedFallbackPuppet);
		}
	}

	DefaultPawnClass = DefaultPawnClassBackup;

	// GameMode 狼 糕滚 函荐肺 贸澜 Spawn 矫懦 PCClass 甫 疙矫窍绰 粱 官焊鞍篮 备炼 锭巩俊 捞吧 促矫 阂矾林绢具 窃.
	SetDesiredPCClassEnum(); // 恐 捞犯霸 父甸菌菌绰瘤 肋 扁撅捞 救唱绰单 备炼啊 捞犯霸 汗棱秦龙 临 隔耳摆瘤..
}

void AB2TutorialGameMode::PreBuildTagCharMesh()
{
	// 惑困 努贰胶牢 StageGameMode 俊辑 角青登绰 扁瓷栏肺 咯扁辑绰 酒公巴档 窍瘤 臼档废 秦具 茄促. 措脚 PreBuildCharacterMeshes 俊辑 贸府.
}

TArray<EPCClass> AB2TutorialGameMode::GetPCClassesToPreLoad()
{
	// 扁夯 Pre-render 肺流栏肺绰 Pre-render 贸府啊 救 等 盲肺 傈捧 捞棋飘甫 荤侩窍霸 瞪 巩力啊 乐绰单 捞繁 备己阑 困秦辑扼档 捞芭 矫累 傈俊 Pre-render 甫 喊档肺 贸府窍霸 瞪 巴.
	TArray<EPCClass> RetArray;
	RetArray.Add(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	return RetArray;
#if 0
	// Async 肺爹阑 救 静妨搁 傈眉 努贰胶甫 茄锅俊 肺爹窍档废 咯扁辑 府畔窍搁 凳.
	TArray<EPCClass> RetArray;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
#endif
}
TArray<EPCClass> AB2TutorialGameMode::GetPCClassesToMatineePreLoad()
{ // 霉 PCClass 寇浚 async 肺爹阑 窍歹扼档 楷免 悸诀篮 葛滴 Preload 秦具 茄促. 扁夯 GetPCClassesToPreLoad 客 促福霸 啊绰 版快.
	TArray<EPCClass> RetArray;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}
void AB2TutorialGameMode::MarkBeginAsyncLoadNextPCClass()
{
	// 促澜 某腐磐 spawn 棺 背眉 楷免 唱坷绰 矫痢俊 弊 促澜 某腐磐 单捞磐 async 肺爹阑 矫累且 格利栏肺 捞吧 妮茄促.

	if (PCClassAsyncLoadOrder.Num() > 0)
	{
		// SetNextPlayerClass 龋免 矫痢捞 Async 肺爹俊 鞘夸茄 矫痢焊促 蠢妨辑 喊档肺 鉴辑 瘤沥窍绰 硅凯阑 父甸绢辑 咯扁辑 持霸 登菌澜.
		// 捞巴 寇俊 促弗 侥栏肺 瘤沥且 拌裙捞 乐栏搁 捞扒 企扁..
		BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(PCClassAsyncLoadOrder[0]); // 被捞 捞吧 SubPlayerClass 肺 技泼窍绰 捞蜡绰 狼固惑 利寸洒 富捞 登扁档 窍瘤父 捞犯霸 秦具 付瘤阜俊 绢截脚捞 技泼登绊 扁夯 急琶 努贰胶啊 登扁 锭巩捞瘤 偿偿
		PCClassAsyncLoadOrder.RemoveAt(0);

		PendingAsyncLoadRequestPCClass = BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass();
	}
}
void AB2TutorialGameMode::BeginAsyncLoadNextPCClass()
{
	checkSlow(PendingAsyncLoadRequestPCClass != EPCClass::EPC_End);

	// 概锅狼 促澜 努贰胶 肺爹 傈俊 瘤陛鳖瘤 肺爹登绢 乐绰 巴甸篮 朝府档废 茄促. 泅犁 某腐磐 单捞磐绰 捞固 spawn 等 捞饶俊绰 鞘夸绝绢具.
	// iOS 甫 困秦辑捞绊 Android 俊辑 ManagedPCClassUnload 荤侩窍绊 乐栏搁 风飘悸 贸府 锭巩俊 攫肺爹 救 瞪 淀..
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
	//	// FlushAsyncLoading 阑 蜡惯且 荐 乐绰 巴甸阑 固府 秦 初酒具 茄促.
	//	StaticFindPCClassInfoBox(this);
	//	StaticFindSkillAnimInfo(this);
	//	StaticFindDamageEffectInfo(this);

	//	//
	//	// 泅犁绰 PCClassInfo 狼 InGameOnlyInfo (AnimBP) 客 SkillAnim 甸阑 Async 肺爹 夸没窍绰单 
	//	// 吝埃俊 flush 啊 瞪 荐档 乐绊 秦辑 呈公 腹捞绰 给且 荐档.
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

	//	// 角力 Async 肺爹 夸没篮 盖 酒贰俊辑.. 捞贰档 拱沸 吝埃俊 flush 芭府啊 积扁搁 档风宫捞摆瘤父.
	//	if (ThisLoadedInfo)
	//	{ // 捞扒 快急鉴困甫 臭霸 霖促. 措眉肺 某腐磐 喊 档涝何 楷免 档吝俊 肺爹 肯丰甫 狼档窃. (臂疥 肋 瞪鳖..)
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
	return false; // 览 救磷绢
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
	checkSlow(!bMergeSections); /* Tutorial 俊 历镊府萍 MergeSection 葛胆阑 镜 老篮 酒付 绝阑 淀 秦辑.. */

	TArray<FB2Item> TutorialDummyItems;
	FB2Wing WingData;

	EPCClass CurrentPCClass = GetCurrentPlayerClass();
	AB2TutorialGameMode::GetPCPartsForTutorialPlay(CurrentPCClass, TutorialDummyItems, WingData);

	SetupComponentsForPartsCommon(TutorialDummyItems, &WingData, bMergeSections, false, OutGetBuiltResultOnly);
}

ICharacterDataStore* ABladeIITutorialPlayer::GetCharacterDataStore()
{
	//咯扁肺 坷绰 巴篮 Local侩 Player character肺 啊沥茄促.
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

	//// 钢府 spawn 阑 秦 初变 沁瘤父..
	//// 坊歹傅俊 荤侩且 芭 酒丛 己瓷俊 规秦啊 瞪 荐 乐绰 夸牢篮 弥措茄 瞒窜.
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

	//// 楷免阑 芭摹促 焊搁 Hidden 捞 钱府霸 瞪 荐 乐绰单.. 弊贰档 SetVisibility 档 沁绊 扁夯 spawn 磊眉档 历 钢府 出底栏聪 绢瘤埃秦辑绰 宝满阑 芭.
	//SetActorHiddenInGame(true);
}
void ABladeIITutorialFallbackPuppetPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	checkSlow(!bMergeSections); /* Tutorial 俊 历镊府萍 MergeSection 葛胆阑 镜 老篮 酒付 绝阑 淀 秦辑.. */

	TArray<FB2Item> TutorialDummyItems;
	FB2Wing WingData;

	EPCClass CurrentPCClass = GetCurrentPlayerClass();
	// ABladeIITutorialPlayer 客 付蛮啊瘤 厘厚甫 厘馒窍档废 悼老茄 窃荐 荤侩.
	AB2TutorialGameMode::GetPCPartsForTutorialPlay(CurrentPCClass, TutorialDummyItems, WingData);

	SetupComponentsForPartsCommon(TutorialDummyItems, &WingData, bMergeSections, false, OutGetBuiltResultOnly);
}
void ABladeIITutorialFallbackPuppetPlayer::SubscribeEvents()
{
	// 酒公巴档 窍瘤 臼档废
}
void ABladeIITutorialFallbackPuppetPlayer::UnsubscribeEvents()
{
	// 酒公巴档 窍瘤 臼档废
}
bool ABladeIITutorialFallbackPuppetPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return false;
}
bool ABladeIITutorialFallbackPuppetPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	return false;
}