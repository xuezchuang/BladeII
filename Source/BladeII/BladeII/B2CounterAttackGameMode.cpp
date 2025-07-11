//
//#include "B2CounterAttackGameMode.h"
////#include "BladeII.h"
////#include "B2UIManager.h"
////#include "B2UIBattleMain.h"
////#include "B2UIBattleSkill.h"
//#include "BladeIICounterPlayer.h"
////#include "B2SkillRTManager.h"
////#include "Retailer.h"
//#include "B2StageManager.h"
////#include "B2UIDocHelper.h"
//#include "Components/InputComponent.h"
//
//
//#include "BladeIIGameImpl.h"
//
//AB2CounterAttackGameMode::AB2CounterAttackGameMode(const FObjectInitializer& ObjectInitializer) :
//	Super(ObjectInitializer)
//{
//	DefaultPawnClass = ABladeIICounterPlayer::StaticClass();
//}
////
////void AB2CounterAttackGameMode::PreBeginPlay()
////{
////	auto* GameRule = new CounterAttackGameRule;
////	//SetGameRule(GameRule);
////
////	Super::PreBeginPlay();
////}
////
////void AB2CounterAttackGameMode::SetupUIScene()
////{
////	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::CounterDGBattleMain);
////}
////
////void AB2CounterAttackGameMode::RestartPlayer(class AController* NewPlayer)
////{
////	Super::RestartPlayer(NewPlayer);
////
////	if (NewPlayer == nullptr)
////		return;
////
////	auto* PlayerController = Cast<APlayerController>(NewPlayer);
////	if (PlayerController)
////		PlayerController->SetIgnoreMoveInput(true);
////
////	PlayerController->ActivateTouchInterface(nullptr);
////
////	auto* Player = Cast<ABladeIIPlayer>(NewPlayer->GetPawn());
////	if (Player)
////	{
////		Player->SetEnableAttack(false);
////		//Unbind any attack key..
////		ModifyInputCommand(Player);
////
////		auto* CameraBoom = Player->GetCameraBoom();
////		if (CameraBoom)
////		{
////			CameraBoom->bEnableCameraLag = true;
////			CameraBoom->CameraLagSpeed = 5.f; //0.2초. TotalInterpolatedTime = 1/CameraLagSpeed
////			CameraBoom->CameraLagMaxDistance = 1000.f;
////			CameraBoom->bUseCameraLagSubstepping = false;
////		}
////
////		Player->CounterCheckTime = GoodCheckTime;
////	}
////}
////
////void AB2CounterAttackGameMode::SetDifficultyFactor(EUIBattleDifficulty Difficulty, float MobAttackRateScale)
////{
////	//DifficultyMobAttackRateMap.Add(Difficulty, MobAttackRateScale);
////}
////
////void AB2CounterAttackGameMode::SubscribeEvents()
////{
////	Super::SubscribeEvents();
////
////	//Issues.Add(CounterFinishAttackClass<>::GetInstance().Subscribe2(
////	//	[this]()
////	//	{
////	//		this->FinishAttack();
////	//	}
////	//));
////	//Issues.Add(PlayerGuardStartedClass<>::GetInstance().Subscribe2(
////	//	[this]()
////	//	{
////	//		this->GuardStartedTime = this->GetWorld()->GetTimeSeconds();
////	//	}
////	//));
////	//Issues.Add(PlayerCounterAttackSucceedClass<>::GetInstance().Subscribe2(
////	//	[this]()
////	//	{
////	//		this->GetWorldTimerManager().ClearTimer(this->BadAccuracyHandle);
////	//		auto* BattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//		if (BattleUI)
////	//		{
////	//			float CheckTime = this->GetWorld()->GetTimeSeconds() - this->GuardStartedTime;
////	//			int32 Accuracy = Accuracy_Bad;
////	//			if (CheckTime < this->PerfectCheckTime)
////	//				Accuracy=Accuracy_Perfect;
////	//			else if (CheckTime < this->GreatCheckTime)
////	//				Accuracy=Accuracy_Great;
////	//			else if (CheckTime < this->GoodCheckTime)
////	//				Accuracy=Accuracy_Good;
////
////	//			auto* DocBattle = UB2UIDocHelper::GetDocBattle();
////	//			if (DocBattle)
////	//			{
////	//				/*
////	//				// 콤보 효과 아래와 같이 수정
////	//				// Good		:  0 % -> 10%
////	//				// Great	: 10 % -> 20%
////	//				// Perfect	: 20 % -> 40%
////	//				*/
////	//				float RealIncCounterDamage = 0.0f;
////	//				if (Accuracy < Accuracy_Bad)
////	//				{
////	//					float IncCounterDamage = 0.1f;
////	//					if (Accuracy < Accuracy_Good)
////	//						IncCounterDamage = 0.1f * static_cast<float>(Accuracy_Good - Accuracy) * 2.0f;
////	//					RealIncCounterDamage = IncCounterDamage;
////	//				}
////	//				DocBattle->SetIncCounterDamageCombo(DocBattle->GetIncCounterDamageCombo() + RealIncCounterDamage);
////	//			}
////
////	//			BattleUI->ShowAccuracy(Accuracy);
////	//		}		
////
////	//		this->GuardStartedTime = 0;
////	//	}
////	//));
////	//Issues.Add(PlayerTakeAttackClass<float>::GetInstance().Subscribe2(
////	//	[this](float f)
////	//	{
////	//		this->ReserveShowBadAccuracy(f);
////	//	}
////	//));
////}
////
////void AB2CounterAttackGameMode::NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase)
////{
////	Super::NotifySpawnPoolWaveMobPhaseChange(SpawnPool, ChangedMob, DestroyedPrevRef, WaveNum, MobObjIndex, NewPhase);
////	DisablePlayerNormalAttack();
////	
////	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_Counter2Phase);
////}
////
////TArray<EPCClass> AB2CounterAttackGameMode::GetPCClassesToPreLoad()
////{
////	TArray<EPCClass> RetArray;
////
////	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
////	if (DocBattle)
////	{
////		RetArray.Add(IntToPCClass(DocBattle->GetCurPCClass()));
////	}
////
////#if WITH_EDITOR
////	if (GIsEditor)
////	{ // 이게 메모리 아끼려고 하는거라 에디터에선 의미도 없고 프리뷰에 방해만 됨. 다 넣어준다.
////		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
////		{
////			RetArray.AddUnique(IntToPCClass(PCI));
////		}
////	}
////#endif
////
////	return RetArray;
////}
////
//////TArray<FCombinedPCSkillAnimID> AB2CounterAttackGameMode::GetPCSkillAnimsToPreLoad()
//////{
//////	TArray<FCombinedPCSkillAnimID> RetArray;
//////	// AnimBP 에서 Skill Anim 을 분리하려는 구조상 반격 던전은 정해진 단일 스킬을 여전히 AnimBP 에 배치해서 쓸 것이므로 Pre-load 의 필요가 없음.
//////	return RetArray;
//////}
////
////void AB2CounterAttackGameMode::SetDecideAccuracyTime(float Perfect, float Great, float Good)
////{
////	//PerfectCheckTime = Perfect;
////	//GreatCheckTime = Great;
////	//GoodCheckTime = Good;
////
////	//auto* Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(GetWorld()));
////	//if (Player)
////	//{
////	//	Player->CounterCheckTime = GoodCheckTime;
////	//}
////}
////
////void AB2CounterAttackGameMode::OnCounterGuideNotify()
////{
////}
////
////void AB2CounterAttackGameMode::ReserveShowBadAccuracy(float time)
////{
////	GetWorldTimerManager().SetTimer(BadAccuracyHandle, this, &AB2CounterAttackGameMode::ShowBadAccuracy, time);
////}
////
////void AB2CounterAttackGameMode::ShowBadAccuracy()
////{
////	//auto* BattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//if (BattleUI)
////	//{
////	//	BattleUI->ShowAccuracy(Accuracy_Bad);
////	//}
////}
////
////void AB2CounterAttackGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
////{
////	Super::NotifyUIWidgetOpenComplete(InOpenedUI);
////
////	if (InOpenedUI == UIFName::CounterAttackDGResult)
////		ShowCachedCompleteQuestNotify();
////}
////
////void AB2CounterAttackGameMode::ModifyInputCommand(class ABladeIIPlayer* NewPlayer)
////{
////	UInputComponent* PlayerInputComponent = NewPlayer ? NewPlayer->InputComponent : nullptr;
////	if (PlayerInputComponent)
////	{
////		PlayerInputComponent->ClearActionBindings();
////
////		if (bEnableNormalAttack)
////		{
////			FInputActionBinding& FirePressed = PlayerInputComponent->BindAction("Attack", IE_Pressed, NewPlayer, &ABladeIIPlayer::StartAttackFromInput);
////			FInputActionBinding& FireReleased = PlayerInputComponent->BindAction("Attack", IE_Released, NewPlayer, &ABladeIIPlayer::StopAttackFromInput);
////		}		
////
////		FInputActionBinding& GuardPressed = PlayerInputComponent->BindAction("Guard", IE_Pressed, NewPlayer, &ABladeIIPlayer::StartGuardFromInput);
////		FInputActionBinding& GuardReleased = PlayerInputComponent->BindAction("Guard", IE_Released, NewPlayer, &ABladeIIPlayer::StopGuardFromInput);
////
////		// handle touch devices
////		PlayerInputComponent->BindTouch(IE_Pressed, NewPlayer, &ABladeIIPlayer::TouchStarted);
////		PlayerInputComponent->BindTouch(IE_Released, NewPlayer, &ABladeIIPlayer::TouchStopped);
////	}
////}
////
////void AB2CounterAttackGameMode::GoToVillage() 
////{
////	if (GetStageManager() && !StageManager->IsStageCleared())
////	{
////		data_trader::Retailer::GetInstance().RequestFinishCounterDungeon(StageManager->GetServerStageID(), StageManager->GetCacheStageKeepEssentialData().GetPlayToken(), false, true);
////	}
////
////	Super::GoToVillage();
////}
////
////void AB2CounterAttackGameMode::GoToVillageDefeated()
////{
////	//반격던전 로비로..
////	GoToMap();
////}
////
////void AB2CounterAttackGameMode::GoToMap()
////{
////	//GoToVillage(); // GoToVillage 에서 LobbyMain UI 를 여는 바람에.. 돌아가는 길에 약간 낭비가 있다.
////
////	//FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
////	//	// 전체 씬 전환을 하면 실 로딩 시간이 늘어나므로 원하는 화면 직전까지 UI History 만 만들어준다. 
////	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
////	//	if (UIMgrInst) {
////	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterDungeonMain);
////	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::CounterAttackDGMain);
////	//	}
////	//	data_trader::Retailer::GetInstance().RequestGetCounterDungeon();
////	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ElobbyScene_CounterAttackSelect);
////	//});
////}
////
////void AB2CounterAttackGameMode::GetFullStackSkillId(EPCClass InCharacterClass, TArray<int32>& OutSkillArray)
////{
////	//우선 이렇게.. 
////	// 이건 반격던전 용 특정 스킬이 단 하나만 존재한다는 가정이다. 
////	// 반격 던전 용으로 AnimBP 스테이트가 별도로 존재하고 있으며 거기서 반격 던전용 AnimSequence 가 별도로 특수하게 링크되어 있으므로 
////	// 만일 반격던전에서도 플레이어 레벨 등에 따라 여러 스킬들을 플레이하거나 일반 전투용 스킬 애니메이션을 공유해서 사용하거나 하는 등의 변경이 발생하게 된다면 AnimBP쪽도 마찬가지로 손질을 해 주어야 한다.
////	OutSkillArray.Add((PCClassToInt(InCharacterClass)+1) * 10000);
////}
////
////void AB2CounterAttackGameMode::PlayVictoryBGM()
////{
////	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_CounterDGVictory);
////}
////
////#include "Animation/AnimTypes.h"
////#include "Animation/AnimSequenceBase.h"
////#include "Animation/AnimInstanceProxy.h"
//////BossMob_master_Anim_BP의 Idle->Attack trasition 종료 트리거로 불리게됨.
////void AB2CounterAttackGameMode::MobAttackStarted(class ABladeIICharacter* Mob)
////{
////	//auto* MobSkelMeshComp = Mob ? Mob->GetMesh() : nullptr;
////	//if (MobSkelMeshComp == nullptr)
////	//	return;
////	//
////	//auto* MobAnimInstance = MobSkelMeshComp->GetAnimInstance();
////	//if (MobAnimInstance == nullptr)
////	//	return;
////
////	//auto& AnimInstanceProxy = MobAnimInstance->GetProxyOnAnyThread<FAnimInstanceProxy>();
////	//const TArray<FAnimTickRecord>& UngroupedActivePlayers = AnimInstanceProxy.GetUngroupedActivePlayersRead();
////
////	//if (UngroupedActivePlayers.Num() > 0 && UngroupedActivePlayers[0].SourceAsset)
////	//{
////	//	MobAttackAnim = Cast<UAnimSequenceBase>(UngroupedActivePlayers[0].SourceAsset);
////	//	if (!MobAttackAnim.IsValid())
////	//		return;
////
////	//	auto* BattelMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//	if (BattelMainUI == nullptr)
////	//		return;
////
////	//	auto* DocCounterDG = UB2UIDocHelper::GetDocCounterAttack();
////	//	if (DocCounterDG)
////	//	{
////	//		if (DocCounterDG->GetNewDifficulty() == 1)
////	//		{
////	//			// 기존 난이도별 적용되는 RateScale은 사용하지않음.
////	//			// 디폴트로 애님에 세팅되어있는 RateScale값은 계산해주어야함.
////	//			const float rateScale(1 / MobAttackAnim->RateScale); //여기서 한번만 나누고 및에서는 곱하기
////
////	//			BattelMainUI-> InitCounterSuggestTimer();
////	//			for (auto& AnimNotify : MobAttackAnim->Notifies)
////	//			{
////	//				const float triggerTime(AnimNotify.GetTriggerTime() * rateScale); //정확한 타이밍
////	//				if (AnimNotify.NotifyName.ToString().Find(TEXT("B2DamageNotify_StunCountable")) != INDEX_NONE)
////	//				{
////	//					BattelMainUI->AddCounterSuggestTimer(triggerTime);
////	//				}
////	//				else if (AnimNotify.NotifyName.ToString().Find(TEXT("B2DamageNotify")) != INDEX_NONE)
////	//				{
////	//					BattelMainUI->AddCounterSuggestTimer(triggerTime);
////	//				}
////	//			}
////	//		}
////	//	}
////	//}
////}
////
//////BossMob_master_Anim_BP의 Attack end 트리거로 불리게됨.
////void AB2CounterAttackGameMode::MobAttackFinished(class ABladeIICharacter* Mob)
////{
////	//auto* DocCounterDG = UB2UIDocHelper::GetDocCounterAttack();
////	//if (DocCounterDG)
////	//{
////	//	if (DocCounterDG->GetDifficulty() == 2)
////	//	{
////	//		if (!MobAttackAnim.IsValid())
////	//			return;
////
////	//		MobAttackAnim->RateScale = DefaultMobAnimPlayRate;
////	//	}
////	//	else if (DocCounterDG->GetDifficulty() == 1)
////	//	{
////	//		auto* BattelMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//		if (BattelMainUI == nullptr)
////	//			return;
////	//		BattelMainUI->RemoveAllCounterSuggestRing();
////	//	}
////	//}
////
////	//MobAttackAnim = nullptr;
////}
////
////
////void AB2CounterAttackGameMode::OnBeginMobStun()
////{
////	EnablePlayerNormalAttack();
////}
////
////void AB2CounterAttackGameMode::OnFinishMobStun()
////{
////	DisablePlayerNormalAttack();
////}
////
////void AB2CounterAttackGameMode::OnIntoDownStay()
////{
////	EnablePlayerNormalAttack();
////}
////
////void AB2CounterAttackGameMode::OnOutofDownStay()
////{
////	DisablePlayerNormalAttack();
////}
////
////bool AB2CounterAttackGameMode::SetFinishState(ABladeIICharacter* Mob)
////{
////	//if (bEndFinishAttack)
////	//	return false;
////
////	//check(BossMob == nullptr || BossMob == Mob);
////
////	//if (bTriggeredFinishAttack)
////	//	return true;
////
////	//auto* BattelMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//if (BattelMainUI == nullptr)
////	//	return false;
////
////	//OnFinishMobStun();
////
////	////UI 숨김.
////	//BattelMainUI->SetCounterFinishMode();
////
////	//bTriggeredFinishAttack = true;
////
////	//BossMob = Mob;
////
////	return true;
////}
////
////void AB2CounterAttackGameMode::FinishAttack()
////{
////	BII_CHECK(bTriggeredFinishAttack);
////
////	bEndFinishAttack = true;
////	if (BossMob.IsValid())
////	{
////		BossMob->Suicide();
////	}
////}
////
////////#include "Matinee/MatineeActor.h"
//////#include "Matinee/InterpGroup.h"
//////#include "Matinee/InterpGroupInst.h"
//////#include "Matinee/InterpTrack.h"
////
////void AB2CounterAttackGameMode::MoveCharacters(const FVector& OriginDiffMove, bool bPauseCameraLag, float PauseTime, float TimeCameraMove)
////{
////	//FVector DiffMove = OriginDiffMove;
////
////	//if (!LimitMoveCharacter.IsInside(MovedDelta + DiffMove))
////	//{
////	//	DiffMove = LimitMoveCharacter.GetClosestPointTo(MovedDelta + DiffMove) - MovedDelta;
////	//}
////	//
////	//MovedDelta += DiffMove;
////	//
////
////	//for (TActorIterator<AMatineeActor> It(GetWorld()); It; ++It)
////	//{
////	//	//AMatineeActor::ApplyWorldOffset을 부르려했는데,
////	//	//Attache된 child actor의 경우, movement가 relative라 원하는 그림이 나오지 않음..
////	//	AMatineeActor* Matinee = (*It);
////	//	if (Matinee && Matinee->MatineeData)
////	//	{
////	//		if (!Matinee->bIsPlaying)
////	//			Matinee->InitInterp();
////
////	//		for (auto* InterpGroup : Matinee->GroupInst)
////	//		{
////	//			AActor* IGGroupActor = InterpGroup->GetGroupActor();
////	//			if (IGGroupActor == nullptr || IGGroupActor->GetAttachParentActor() != nullptr)
////	//				continue;
////
////	//			UInterpGroup* Group = InterpGroup->Group;
////	//			for (int32 TrackIndex = 0; TrackIndex < Group->InterpTracks.Num(); TrackIndex++)
////	//			{
////	//				Group->InterpTracks[TrackIndex]->ApplyWorldOffset(DiffMove, false);
////	//			}
////	//		}
////
////	//		if (!Matinee->bIsPlaying)
////	//			Matinee->TermInterp();
////	//	}
////	//}
////
////	//for (TActorIterator<ABlockingVolume> It(GetWorld()); It; ++It)
////	//{
////	//	if (It->IsRootComponentMovable())
////	//		It->AddActorWorldOffset(DiffMove);
////	//}
////
////	//for (TActorIterator<ABladeIICharacter> It(GetWorld()); It; ++It)
////	//{
////	//	It->AddActorWorldOffset(DiffMove);
////	//}
////
////	//auto* Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(GetWorld()));
////	//if (Player)
////	//{
////	//	if (bPauseCameraLag)
////	//	{
////	//		CameraLagSpeed = 1.f / TimeCameraMove;
////
////	//		GetWorldTimerManager().ClearTimer(CameraLagStartTimeHandle);
////	//		Player->CameraBoom->CameraLagSpeed = 0.01f;
////	//		GetWorldTimerManager().SetTimer(CameraLagStartTimeHandle, this, &AB2CounterAttackGameMode::CameraLagStart, PauseTime, false);
////	//	}
////	//	else
////	//		Player->CameraBoom->CameraLagSpeed = 5.f;
////	//}	
////}
////
////void AB2CounterAttackGameMode::SetLimitMoveCharacters(const FBox& LimitSpace)
////{
////	LimitMoveCharacter = LimitSpace;
////}
////
////void AB2CounterAttackGameMode::CameraLagStart()
////{
////	//auto* Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(GetWorld()));
////	//if (Player)
////	//{
////	//	GetWorldTimerManager().ClearTimer(CameraLagStartTimeHandle);
////	//	Player->CameraBoom->CameraLagSpeed = CameraLagSpeed;
////	//}
////}
////
////float AB2CounterAttackGameMode::GetCurrentStageDifficultyScale()
////{
////	//auto* DocBS = UB2UIDocHelper::GetDocBattleStage();
////	//BII_CHECK(DocBS);
////
////	int32 DifficultyScale = 1;
////
////	//auto CounterDungeonData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonData();
////	//for (auto& Ele : CounterDungeonData)
////	//{
////	//	if (DocBS->GetServerStageId() != Ele.ServerStageId)
////	//		continue;
////
////	//	DifficultyScale = Ele.DifficultyScale;
////	//}
////
////	return static_cast<float>(DifficultyScale) / 100.0f;
////}
////
////#include "B2CombatStatEvaluator.h"
////
////float AB2CounterAttackGameMode::GetTotalAttack(ICharacterDataStore* CharacterDataStore)
////{
////	//const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();
////
////	//float Attack				= CombatStatEval::GetPCAttack(MainPCClass, CharacterDataStore);
////	//float ModSpecializeAttack	= CombatStatEval::GetPCAttackByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);
////
////	//float TotalAttack = Attack + ModSpecializeAttack;
////	//TotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalAttack);
////
////	//return TotalAttack;
////	return 0.0;
////}
////
////float AB2CounterAttackGameMode::GetTotalDefense(ICharacterDataStore* CharacterDataStore)
////{
////	//const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();
////
////	//float Defense				= CombatStatEval::GetPCDefense(MainPCClass, CharacterDataStore);
////	//float ModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);
////
////	//float TotalDefense = Defense + ModSpecializeDefense;
////	//TotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalDefense);
////
////	//return TotalDefense;
////	return 0.0;
////}
////
////void AB2CounterAttackGameMode::OverrideDamageInfo(FDamageInfo& OutDamageInfo, const ABladeIICharacter* B2Char)
////{
////	const auto* B2Player = Cast<ABladeIIPlayer>(B2Char);
////	if (!B2Player)
////		return;
////
////	if (B2Player->bCounterCauseStun && B2Player->IsCounterAttacking())
////	{
////		OutDamageInfo.DamageType			= EAttackType::EAT_Default;
////		OutDamageInfo.StateType				= EStateAttackType::EDT_Stun;
////		OutDamageInfo.StateDamageDuration	= 5.f;
////	}
////}
////
////void AB2CounterAttackGameMode::OverridePauseForDuration(float pausetime, const ABladeIICharacter* B2Char)
////{
////	//const auto* B2Player = Cast<ABladeIIPlayer>(B2Char);
////	//if (!B2Player)
////	//	return;
////
////	//auto* BattelMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//if (BattelMainUI && pausetime > 0)
////	//	BattelMainUI->OnPauseCounterSuggestRing(pausetime);
////}
////
////void AB2CounterAttackGameMode::PlayBGMByStageContext(int32 InClientStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, bool bForceStopCurrent)
////{
////	//if (bForceStopCurrent) 
////	//	StopBGM();
////
////	//PlayBGMByCommonSoundID( ECommonSoundID::ECSID_BGM_Counter1Phase, bForceStopCurrent );
////}
////
////void AB2CounterAttackGameMode::EnablePlayerNormalAttack()
////{
////	//bEnableNormalAttack = true;
////
////	//auto* BattelMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//if (BattelMainUI == nullptr)
////	//	return;
////
////	//BattelMainUI->SetAttackEnable(!bTriggeredFinishAttack);
////
////	//if(bTriggeredFinishAttack)
////	//	return;
////
////	//auto* Player = Cast<ABladeIICounterPlayer>(UGameplayStatics::GetLocalPlayerCharacter(GetWorld()));
////	//if (Player)
////	//	Player->EnableNormalAttack();
////}
////
////void AB2CounterAttackGameMode::DisablePlayerNormalAttack()
////{
////	//bEnableNormalAttack = false;
////
////	//auto* BattelMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
////	//if (BattelMainUI == nullptr)
////	//	return;
////
////	//BattelMainUI->SetAttackEnable(false);
////
////	//auto* Player = Cast<ABladeIICounterPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
////	//if (Player)
////	//	Player->DisableNormalAttack();
////}