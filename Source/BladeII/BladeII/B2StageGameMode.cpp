// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Runtime/Engine/Public/Engine.h"
#if WITH_EDITOR
#include "Editor.h"
#endif
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "B2StageInfo.h"
//#include "B2MonsterSpawnPool.h"
#include "B2StageEventDirector.h"
#include "B2AutoWayPoint.h"
//#include "B2TreasureChestBase.h"
#include "Retailer.h"
#include "Event.h"
//#include "B2UIManager.h"
//#include "B2UIBattleMain.h"
#include "B2PCClassInfoBox.h"
//#include "B2UIStore.h"
//#include "B2UISummonItem.h"
#include "B2UIDocHelper.h"
//#include "B2UIGoodsShortcutTooltipPopup.h"
//#include "B2SkillRTManager.h"
#include "BladeIIPlayerController.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
//#include "B2GameLoadingProgress.h"
//#include "B2DialogTriggerActorBase.h"
#include "B2ScenarioDialogInfo.h"
#include "Global.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
//#include "B2UIUtil.h"
#include "BladeIIGameMode.h"
//#include "B2BGMManager.h"


///////////////////////////////////////////////////////////////////////////////////////////
//
//	B2StageGameModeBase
//

AB2StageGameModeBase::AB2StageGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StageManager = NULL;
	LastRequestedStageBuffType = EResurrectBuffType::RBT_None;
}

void AB2StageGameModeBase::PreBeginPlay()
{
	check(GetStageManager());
	// StageManager 의 필수 정보들(CurrentStageId 등)을 이 시점에서 가져오면 될 듯.
	GetStageManager()->PreBeginPlay();

	auto* GameRule = new StageGameRule;
	//SetGameRule(GameRule);

	Super::PreBeginPlay();	

	// Add RemoveBGLevel
	UWorld* World = GetWorld();
	BII_CHECK(World);
	FString RemoveBGName = World->GetOutermost()->GetName() + RemoveLevelSuffix;
	ULevelStreaming* StreamingLevel = World->GetLevelStreamingForPackageName(FName(*RemoveBGName));
	if (StreamingLevel)
	{
		StreamingLevel->SetShouldBeLoaded(true);
		StreamingLevel->SetShouldBeVisible(true);
		World->FlushLevelStreaming(EFlushLevelStreamingType::Full);
	}

	bFlagDeferredGoToMap = bFlagDeferredGoToNextArea = false;
	this->SetTickableWhenPaused(true);
}

void AB2StageGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//if (IsUsingMobileRendering(GetWorld())) { // 모바일 렌더러의 dynamic point light 개수 제한을 위해.
	//	DeactivateWorldMovablePointLight();
	//}
}

AB2StageManager* AB2StageGameModeBase::GetStageManager()
{
	UWorld* TheWorld = GetWorld();
	if (StageManager == NULL && TheWorld)
	{
		// 걍 기본 정보 넣고 spawn 시킴
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;
		StageManager = TheWorld->SpawnActor<AB2StageManager>(AB2StageManager::StaticClass(), SpawnInfo);
	}
	check(StageManager);
	if (StageManager)
	{
		StageManager->SetGameMode(this);
	}

	return StageManager;
}

void AB2StageGameModeBase::SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer)
{
	if (auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	{
		DocBattleStage->SetAllServival(true);
	}
}

void AB2StageGameModeBase::NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);

	if (auto* Player = Cast<ABladeIIPlayer>(TargetCharacter))
	{
		if (auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
		{
			DocBattleStage->SetAllServival(false);
		}

		if (IsDefeated(Player))
		{
			PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_CombatDefeat);
		}
	}
}

void AB2StageGameModeBase::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer)
{
	if (!(StageManager && StageManager->IsStageCleared())) // 패배 메뉴를 열게 되는데 스테이지 클리어 이후에는 작동하지 않게..
	{
		CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
		// 부활 메뉴 시작. RequestResurrect 에서 부활을 하거나, 혹은 다시 시작하면 끝날 것.
		StartDefeatMenuClass<EB2GameMode>::GetInstance().Signal(EB2GameMode::Scenario);
	}
}

void AB2StageGameModeBase::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLiveMobs)
{
	GetAllWorldMobList(OutList, bOnlyLiveMobs);
		
	TArray<ABladeIICharacter*> RemoveList;

	for (ABladeIICharacter* pChar : OutList)
	{
		// 탑승된몹은 제거
		if(pChar->IsControlledByQTEPlayer())
			RemoveList.Add(pChar);
	}

	for (ABladeIICharacter* pChar : RemoveList)
	{
		OutList.Remove(pChar);
	}
}

float AB2StageGameModeBase::GetCurrentStageDifficultyScale()
{
	auto* LocalSM = GetStageManager();
	// 현재 선택된 DifficultyLevel 과 Info 세팅 (내지는 서버 마스터 데이터) 에서 비롯되는 실제 적용되는 scale 값 리턴
	return LocalSM ? LocalSM->GetStageDifficultyScale() : 0.0f;
}

float AB2StageGameModeBase::GetDamageRandomRate()
{
	return (1.f + FMath::FRandRange(-0.025f, 0.025f));
}

bool AB2StageGameModeBase::IsDefeated(ABladeIIPlayer* Player)
{
	//if (Player && Player->bIsDying)
	//{
	//	auto* SkillRTManager = Player->GetSkillRTManager();
	//	auto* GameRule = GetGameRule();

	//	if (SkillRTManager && GameRule)
	//	{
	//		const bool HasAliveTagCharacter = GameRule->AllowToTag() && SkillRTManager->IsTagPossibleAtDeadMoment();
	//		return !HasAliveTagCharacter;
	//	}
	//}

	return false;
}

void AB2StageGameModeBase::NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	Super::NotifySpawnPoolMobSpawn(SpawnedMob, InBirthplaceInfo);

	if (StageManager){
		StageManager->NotifySpawnPoolMobSpawn(SpawnedMob, InBirthplaceInfo);
	}
}

void AB2StageGameModeBase::NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool)
{
	Super::NotifySpawnPoolComplete(CompletedSpawnPool);

	if (StageManager){
		StageManager->NotifySpawnPoolComplete(CompletedSpawnPool);
	}
}

void AB2StageGameModeBase::NotifySpawnPoolSingleWaveStart(AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum)
{
	Super::NotifySpawnPoolSingleWaveStart(StartedSpawnPool, WaveNum);

	if (StageManager){
		StageManager->NotifySpawnPoolSingleWaveStart(StartedSpawnPool, WaveNum);
	}
}

void AB2StageGameModeBase::NotifySpawnPoolSingleWaveComplete(AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum)
{
	Super::NotifySpawnPoolSingleWaveComplete(CompletedSpawnPool, WaveNum);

	if (StageManager){
		StageManager->NotifySpawnPoolSingleWaveComplete(CompletedSpawnPool, WaveNum);
	}
}

void AB2StageGameModeBase::NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex)
{
	if (StageManager){ // 혹 모르니 리스트에서 제거하기 전에 처리하는 게 좋을 듯?
		StageManager->NotifySpawnPoolWaveMobDead(SpawnPool, DyingMob, WaveNum, MobIndex);
	}

	//// 보스 막타 진동
	//if (DyingMob && DyingMob->IsSpawnedAsBossMob()){
	//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (B2Player)
	//	{
	//		B2Player->ForceFeedback();
	//	}
	//}

	Super::NotifySpawnPoolWaveMobDead(SpawnPool, DyingMob, WaveNum, MobIndex);
}

void AB2StageGameModeBase::NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase)
{
	Super::NotifySpawnPoolWaveMobPhaseChange(SpawnPool, ChangedMob, DestroyedPrevRef, WaveNum, MobObjIndex, NewPhase);

	if (StageManager){
		StageManager->NotifySpawnPoolWaveMobPhaseChange(SpawnPool, ChangedMob, DestroyedPrevRef, WaveNum, MobObjIndex, NewPhase);
	}
}

void AB2StageGameModeBase::NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob)
{
	Super::NotifySpawnPoolSummonedMobDead(SpawnPool, DyingMob);

	if (StageManager){
		StageManager->NotifySpawnPoolSummonedMobDead(SpawnPool, DyingMob);
	}
}

void AB2StageGameModeBase::NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed)
{
	Super::NotifyForgetACharacter(CharacterAboutToBeDestroyed);

	if (StageManager){
		StageManager->NotifyForgetACharacter(CharacterAboutToBeDestroyed);
	}
}

void AB2StageGameModeBase::NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType)
{
	Super::NotifyStageEventSceneBegin(BegunDirector, EventType);

	if (StageManager){
		StageManager->NotifyStageEventSceneBegin(BegunDirector, EventType);
	}
}
void AB2StageGameModeBase::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{
	Super::NotifyStageEventSceneEnd(EndedDirector);

	if (StageManager){
		StageManager->NotifyStageEventSceneEnd(EndedDirector);
	}
}

void AB2StageGameModeBase::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	Super::NotifyUIWidgetOpenComplete(InOpenedUI);

	if (StageManager){
		StageManager->NotifyUIWidgetOpenComplete(InOpenedUI);
	}
}


void AB2StageGameModeBase::AddSpawnPool(class AB2MonsterSpawnPool* InNewSpawnPool)
{
	//Super::AddSpawnPool(InNewSpawnPool);
	//if (InNewSpawnPool){
	//	InNewSpawnPool->SetStageManager(StageManager);
	//}
}

void AB2StageGameModeBase::AddStageEventDirector(class AB2StageEventDirector* InNewEventDirector)
{
	Super::AddStageEventDirector(InNewEventDirector);

	if (InNewEventDirector){
		InNewEventDirector->SetStageManager(StageManager);
	}
}

void AB2StageGameModeBase::AddActiveCameraActor(class AB2ActiveCameraActor* InNewActiveCameraActor)
{
	Super::AddActiveCameraActor(InNewActiveCameraActor);
}

void AB2StageGameModeBase::AddAutoWayPoint(AB2AutoWayPoint* InNewWayPoint)
{
	Super::AddAutoWayPoint(InNewWayPoint);

	if (InNewWayPoint){
		InNewWayPoint->SetStageManager(StageManager);
	}
}

//void AB2StageGameModeBase::AddTreasureChest(class AB2TreasureChestBase* InNewTreasureChest)
//{
//	Super::AddTreasureChest(InNewTreasureChest);
//
//	if (InNewTreasureChest){
//		InNewTreasureChest->SetStageManager(StageManager);
//	}
//}
//
//void AB2StageGameModeBase::RemoveTreasureChest(class AB2TreasureChestBase* InNewTreasureChest)
//{
//	Super::RemoveTreasureChest(InNewTreasureChest);
//
//	if (InNewTreasureChest){
//		InNewTreasureChest->SetStageManager(NULL);
//	}
//}

bool AB2StageGameModeBase::RequestResurrect(EResurrectBuffType InResurrectBuffType)
{
	int32 ServerResurrectBuffType = CliToSvrResurrectBuffType(InResurrectBuffType);
	if (ServerResurrectBuffType != b2network::B2ResurrectBuffType::BRBT_None)
	{
		LastRequestedStageBuffType = InResurrectBuffType; // 부활 성공시에 그전까지 선택한 부활버프들 모두 모여서 올 건데 이 타입만 적용할 거.
		data_trader::Retailer::GetInstance().RequestResurrection(AB2StageManager::GetCacheStageKeepEssentialData().GetPlayToken(), ServerResurrectBuffType);
		return true;
	}
	return false;
}

void AB2StageGameModeBase::DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty)
{
	//// 기본 동작과는 달리 StageId 와 Difficulty 를 고려해서 맞는 걸 가져온다.
	//// 스테이지 시작 시점에 한번 정하면 바뀌지 않는다.
	//if (StageManager)
	//{
	//	TheActiveSpawnPool = GetDesiredSpawnPoolOfSettingFromList(SpawnPools, 
	//		// 게임이든, PlayInEditor 든 StageManager 의 PreBeginPlay 이후라면 StageId 나 DifficultyLevel 등이 사용 가능해야.
	//		StageManager->GetCurrentClientStageId(), StageManager->GetStageDifficultyLevel(), bFallBackToLowerDifficulty);
	//}
	//else
	//{
	//	Super::DecideAndSetActiveSpawnPool(); // 상위단 기능으로 폴백
	//}
}

TArray<EPCClass> AB2StageGameModeBase::GetPCClassesToPreLoad()
{
	TArray<EPCClass> RetArray;

	FLocalCharacterData& ClientDataStore = BladeIIGameImpl::GetLocalCharacterData();
	RetArray.Add(ClientDataStore.GetMainPlayerClass());
	RetArray.Add(ClientDataStore.GetSubPlayerClass());

	return RetArray;
}

TArray<FCombinedPCSkillAnimID> AB2StageGameModeBase::GetPCSkillAnimsToPreLoad()
{
#if WITH_EDITOR
	if (GIsEditor)
	{ // 에디터에선 모든 skill 애니메이션을 로딩하도록 하는 상위 클래스 기능을 사용
		return ABladeIIGameMode::GetPCSkillAnimsToPreLoad();
	}
#endif

	TArray<FCombinedPCSkillAnimID> RetArray;

	//TArray<EPCClass> AllPCClassToPreLoad = GetPCClassesToPreLoad();

	//// 각 PCClass 별로 장착한 스킬 애니메이션들을 넣도록.
	//// StageGameMode 이므로 LocalCharacterData 만으로 가능하다.

	//FLocalCharacterData& LocalPlayerCharData = BladeIIGameImpl::GetLocalCharacterData();
	//
	//for (EPCClass ThisPCClass : AllPCClassToPreLoad)
	//{
	//	TArray<ESkillAnimType> SkillAnimsOfThisClass;
	//	UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, ThisPCClass, GetB2GameModeType(), SkillAnimsOfThisClass, &LocalPlayerCharData);

	//	// 이번 캐릭터 클래스에서 모은 SkillAnimType 들로 Combined ID 를 만들어 넣음.
	//	for (ESkillAnimType ThisSkillAnimType : SkillAnimsOfThisClass)
	//	{
	//		RetArray.Add(FCombinedPCSkillAnimID(ThisPCClass, ThisSkillAnimType));
	//	}
	//}
	return RetArray;
}

void AB2StageGameModeBase::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	// TAsset 로딩하는 InfoAsset 들이 대상임. 반대는 ManualUnloadBeforeNewBeginning

	//if (GetStageManager())
	//{
	//	StageManager->PreloadAdditionalNecessaryInfo();
	//}
	//
	////Synchronous 로딩의 Async 로딩 flush 여부에 따라.. 이걸 다른 async request 보다 먼저 혹은 이후에..
	//PreloadInGameInfoAsset_SynchronousOnly(bAboutToPlayLoadingMovie);

	//PreloadAllExpectedDialogAssets();

	//Super::PreloadAnyNecessaryInfo(bAboutToPlayLoadingMovie);
	//
	//PreloadInGameInfoAsset_AsyncAllowed(bAboutToPlayLoadingMovie);

	//// 태그 캐릭터 Mesh merge 를 미리 수행해 놓음. Preload 차원. 
	//// 그러나 B2CompositeMeshCache 의 RooSet 처리를 사용하게 되면 이건 유명무실.
	//PreBuildTagCharMesh(); 
}

void AB2StageGameModeBase::PreBuildTagCharMesh()
{
	// B2CompositeMeshCache 의 RooSet 처리를 사용하게 되면 이건 유명무실.
	//PrebuildLocalTagCharMesh();
}

void AB2StageGameModeBase::GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode)
{
	// 이번 스테이지 flow 상 발동 가능한 시나리오 다이얼로그 관련한 코드네임들 긁어모으기.

	B2_SCOPED_TRACK_LOG(TEXT("AB2StageGameModeBase::GetAllExpectedDialogCodeNames"));

	//AB2MonsterSpawnPool* ActiveSpwnPool = GetActiveSpawnPool();
	//if (ActiveSpwnPool)
	//{
	//	ActiveSpwnPool->GetAllExpectedDialogCodeNames(OutAllDlgCode);
	//}
	//for (AB2StageEventDirector* ThisEventDirector : EventDirectors)
	//{
	//	if (ThisEventDirector && ThisEventDirector->IsExpectedForGameMode(this))
	//	{
	//		ThisEventDirector->GetAllExpectedDialogCodeNames(OutAllDlgCode);
	//	}
	//}
	//for (AB2DialogTriggerActorBase* ThisDialogTrigger : DialogTriggers)
	//{
	//	if (ThisDialogTrigger && ThisDialogTrigger->IsExpectedForGameMode(this))
	//	{
	//		OutAllDlgCode.AddUnique(ThisDialogTrigger->DialogCodeName);
	//	}
	//}
}

void AB2StageGameModeBase::PreloadAllExpectedDialogAssets()
{
	// 다이얼로그에 사용하는 텍스쳐랑 사운드들.. 이들은 로딩 자체가 on-demand 로 하기 무겁다기보다 다른 async 로딩 flush 방지 차원이 더 큼.

	B2_SCOPED_TRACK_LOG(TEXT("AB2StageGameModeBase::PreloadAllExpectedDialogAssets"));

	//TArray<FName> AllDlgCodes;
	//GetAllExpectedDialogCodeNames(AllDlgCodes);

	//UB2ScenarioDialogInfo* DlgInfoTable = StaticFindDialogInfoTable();
	//if (DlgInfoTable)
	//{ // 일반적인 dialog 에 필요한 것만 로딩해 본다..
	//	DlgInfoTable->PreloadAssetsForStageGame(AllDlgCodes, this, false);

	//	// Talkdialog 꺼리도 preload 하려면 좀 손이 갈 듯.. 그건 보통 스테이지 시작이나 끝날 때 나오는 거라 딱히 필요할까 싶지만..
	//}
}

void AB2StageGameModeBase::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets)
{
	//if (UB2NPCClassInfoBox* NPCInfoBox = StaticFindMobClassInfoBox())
	//{
	//
	////	NPCInfoBox->conditionalc

	//}

}


AActor* AB2StageGameModeBase::FindPlayerStartForCurrStage(AController* InController)
{
	FSingleStageInfoData* ThisStageInfo = GetStageManager() ? GetStageManager()->GetCurrentStageInfoData() : NULL;
	if (ThisStageInfo && !ThisStageInfo->PlayerStartTag.IsEmpty() && !StageManager->IsPlayingInExtraLevel()) // PlayerStartTag 는 MainMap 용 설정임.
	{
		return FindPlayerStart(InController, ThisStageInfo->PlayerStartTag);
	}
	return FindPlayerStart(InController);
}

void AB2StageGameModeBase::ResetAssumeNoWorldLoading()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageGameModeBase::ResetAssumeNoWorldLoading"));
//
//	// 월드 로딩 없이 게임 플레이에 필요한 데이터들만 리셋해서 새로 시작하려는 기능임.
//	// 제한적인 상황에서 사용을.. 일반적인 스테이지 게임에서 사용하려는 거 아님.
//	// StageNumber 나 Difficulty 는 필요하다면 이 시점에 이미 새로운 값으로 받아져 있어야 함. 
//
//#if !UE_BUILD_SHIPPING
//	CheckResetAssumeNoWorldLoadingCondition(this);
//#endif
//	
//	// OnPreLoadMap 에서 하던 로컬 상태 저장..
//	SaveLocalIniStateOnPreLoadMap();
//
//	UpdateManagedActorList();
//
//	//UI독매니저 초기화
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	if (UIDocManager) {
//		UIDocManager->Init(this);
//	}
//
//	// 사용될 SpawnPool 셋업이 바뀔 가능성을 생각한다면 Preload 도 필요할수밖에.. 안 그러면 중간에 툭툭 튀어버릴 테니..
//	// 이전 스테이지 것들을 Unload 하는 건 어찌할까.. 그것까지 들어가면 결국 로딩시간이 많이 걸리게 될텐데..
//	PreloadAnyNecessaryInfo(false);
//	
//	// AutoAiController에 스테이지 클리어 셋팅된거 리셋
//	ABladeIIPlayerController* pPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	if (pPC)
//	{
//		pPC->ResetAutoAiController();
//
//		// 입력 막은거 해제
//		pPC->SetStageEndNonCombatState(false);
//	}
//
//	// 상태 리셋 차원에서 플레이어 캐릭터도 Respawn.
//	ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	UB2SkillRTManager* LocalPlayerSkillMgr = LocalPlayerChar->GetSkillRTManager();
//	if (LocalPlayerSkillMgr)
//		LocalPlayerSkillMgr->SimpleRebirth();
//
//	// SimpleRebirth 에 성공했다면 LocalPlayerCharacter 가 바뀌어 있을 것.
//	LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalPlayerChar)
//	{
//		LocalPlayerChar->EnableInput(UGameplayStatics::GetLocalPlayerController(this)); // Rebirth 과정에서 DisableInput 하는 게 있어서 Enable 해줌.
//		AActor* ThePlayerStart = FindPlayerStartForCurrStage(UGameplayStatics::GetLocalPlayerController(this));
//		if (ThePlayerStart)
//		{
//			LocalPlayerChar->SetActorLocation(ThePlayerStart->GetActorLocation());
//			LocalPlayerChar->SetActorRotation(FRotator(0.0f, ThePlayerStart->GetActorRotation().Yaw, 0.0f));
//		}
//
//		if (LocalPlayerSkillMgr)
//			LocalPlayerSkillMgr->ResetInitialized(LocalPlayerChar);
//	}
//
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED)
//		{
//			CurrSED->ResetAssumeNoWorldLoading();
//		}
//	}
//
//	StageManager->StageBeginHandling();
//	StageManager->ResetPlayTime();
//
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED)
//		{
//			CurrSED->EssentialBeginHandlingCommon();
//		}
//	}
//
//	StartEventScene_GameBegin();
//
//	// Spawn 과 함께 연출이 시작될 수 있으니 Player 캐릭터랑 연출 등의 리셋처리를 한 후에 SpawnPool 리셋.
//	if (GetActiveSpawnPool())
//	{
//		GetActiveSpawnPool()->ResetAssumeNoWorldLoading();
//	}
//	
//	if (!bDeferAutoBeginAuto) // 이걸 쓰는 상황에서 DeferAutoBeginAuto 를 true 로 쓸 거 같진 않지만..
//	{
//		TryAutoBeginAutoAtBeginning();
//	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//	B2StageGameMode
//

void AB2StageGameMode::BeginPlay()
{
	Super::BeginPlay();
//	
//	if (IsRepeatBattleOn())
//	{ // 매 반복전투 스테이지 시작시마다..
//		OnBeginPlayOfRepeatBattle();
//	}
//	
//	if (!IsInPreRenderPhase()) // Pre-render 시에는 그게 끝난 후 RestoreWholeGameTimeScale 을 함.
//	{
//		RestoreWholeGameTimeScale(); // BeginPlay 인데 웬 Restore 인가 싶지만 걍 필요한 기본 설정 하는 걸로 보면 됨.
//	}
//
//	DeferredClientStageId = INDEX_NONE;
//
//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (B2Player)
//	{
//		B2Player->SetUnityTick(true);
//	}
//
//#if WITH_EDITOR
//	FSingleStageInfoData* ThisStageInfo = StageManager->GetCurrentStageInfoData();
//	UPackage* OutermostPkg = GetOutermost();
//	if (GIsEditor && ThisStageInfo && OutermostPkg)
//	{		
//		FString CurrLevelName = OutermostPkg->FileName.ToString();
//		FString StageInfoMainMapName = ThisStageInfo->MainMap.ToString(); // 여긴 아마 경로는 없을 것.
//		FString StageInfoExtraMapName = ThisStageInfo->ExtraMap.ToString();
//		// 굳이 아닌 거 비교해서 불필요한 메시지 박스 안띄우려고..
//		const bool bIsProbablyMatchingMainLevel = (CurrLevelName.Len() >= StageInfoMainMapName.Len() && CurrLevelName.EndsWith(StageInfoMainMapName));
//		// 에디터에서 단독 플레이시 ExtraMap 판별.
//		const bool bEditor_IsExtraMap = (GIsEditor && CurrLevelName.EndsWith(StageInfoExtraMapName));
//		if (!ThisStageInfo->PlayerStartTag.IsEmpty() && !StageManager->IsPlayingInExtraLevel() && bIsProbablyMatchingMainLevel && !bEditor_IsExtraMap)
//		{
//			bool bFoundMatchingPlayerStart = false;
//			for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
//			{
//				APlayerStart* Start = *It;
//				if (Start && Start->PlayerStartTag == FName(*ThisStageInfo->PlayerStartTag))
//				{
//					bFoundMatchingPlayerStart = true;
//					break;
//				}
//			}
//			if (!bFoundMatchingPlayerStart)
//			{
//#if !PLATFORM_MAC
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(
//					TEXT("현재 플레이하는 레벨 %s 에서 StageInfo %d 에 설정된 %s 태그를 가진 PlayerStart를 찾을 수 없다. 컴퓨터가 곧 폭발한다."),
//					*CurrLevelName, StageManager->GetCurrentClientStageId(), *ThisStageInfo->PlayerStartTag
//					)));
//#endif
//			}
//		}
//		int32 TotalPlayerStart = 0;
//		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
//		{
//			APlayerStart* PlayerStart = *It;
//			APlayerStartPIE* PlayerStartPIE = Cast<APlayerStartPIE>(PlayerStart); // 대체 이건 뭐냐..
//			if (PlayerStart && !PlayerStartPIE)
//			{
//				++TotalPlayerStart;
//			}
//		}
//		if (TotalPlayerStart >= 2 &&
//			(ThisStageInfo->PlayerStartTag.IsEmpty() || StageManager->IsPlayingInExtraLevel() || bEditor_IsExtraMap))
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(
//				TEXT("현재 플레이하는 레벨 %s 의 PlayerStart 가 %d 개이나 StageInfo %d 에 PlayerStartTag 가 설정되어 있지 않거나 혹은 ExtraMap 입니다."),
//				*CurrLevelName, TotalPlayerStart, StageManager->GetCurrentClientStageId()
//				)));
//#endif
//		}
//	}
//#endif
}

void AB2StageGameMode::SetDesiredPCClassEnum()
{
	Super::SetDesiredPCClassEnum();

	// ExtraMap 로딩 시에는 직전에 플레이했던 클래스로 시작해야 한다.
	// PlayInExtraLevel 이나 LastPlayedClass 모두 extra map 로딩 시작 직전에 모두 세팅되어 있을 것.
	if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel()){
		CurrentPCClassEnum = AB2StageManager::GetCacheStageKeepEssentialData().GetPreExtraLastPlayedClass();
	}
#if WITH_EDITOR
	if (GetStageManager() && GetStageManager()->IsStandalonePIE())
	{ // StageManager 의 PreBeginPlay 가 이것보다 앞서 실행되었다면 로비를 통하지 않은 단순 Play-in-Editor 에서는 여기로 들어올 것.
		CurrentPCClassEnum = DefaultPCClassEnum;
	}
#endif
}

void AB2StageGameMode::SubscribeEvents()
{
	Super::SubscribeEvents();

	CAPTURE_UOBJECT(AB2StageGameMode);

	Issues.Add(LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Subscribe2(
		[this](EHeroMgmtSubMode sm)
		{
			this->EnterHeroMgmtMode(sm);
		}
	));
	Issues.Add(EndPlayOfSingleRepeatBattleStageClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->OnEndPlayOfSingleRepeatBattleStage();
		}
	));
}

void AB2StageGameMode::PreBeginPlay()
{
	Super::PreBeginPlay();

	// [모험모드] 일경우 월드바운드를 항상 체크하며, 일정거리(-50M) 이하로 액터가 떨어질 경우, 사망 처리
	AWorldSettings* WorldSettings = GetWorld() ? GetWorld()->GetWorldSettings() : nullptr;
	if ( WorldSettings )
	{
		WorldSettings->bEnableWorldBoundsChecks = true;
		WorldSettings->KillZ = -5000.0f;
	}

	bEnterInventory = bEnterShop = false;
	nRequestGeneralShop = -1;
}

void AB2StageGameMode::RestoreWholeGameTimeScale()
{
	if (HasStageBuff(EStageBuffType::SBT_BoostGameSpeed))
	{ // 게임 속도 높이기.
		float BoostScale = BladeIIGameImpl::GetClientDataStore().GetStageBuffIncreaseGamseSpeedRate();
		//SetWholeGameTimeScale(BoostScale); // SetGlobalTimeDilation 으로 들어가는 값의 기반에 적용됨.
	}
	else
	{
		Super::RestoreWholeGameTimeScale();
	}
}

void AB2StageGameMode::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);

	//// 첫 Spawnpool NPCClassInfo 는 preload로, InGame 진입하자마자 나머지 Spawnpool Asyncload
	//if (NewState == MatchState::InProgress && TheActiveSpawnPool)
	//{
	//	TheActiveSpawnPool->TryAsyncLoadReservedMonsters();
	//}
}

void AB2StageGameMode::EnterHeroMgmtMode(EHeroMgmtSubMode InSubMode)
{
	GoToVillage();

	/*FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory); 
	});*/
}

void AB2StageGameMode::RestartGame()
{
	RestartStage();
}

void AB2StageGameMode::RestartGameNext()
{
	if ( !GetStageManager() ) return;	
 //   
	//UB2StageInfo* StageInfo = StaticFindStageInfo();
	//if ( !StageInfo ) return;

	//// [연속전투] 3배모험에서 티켓이 부족할경우 전투를 멈추고 팝업을 띄워준다
	//if ( auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage() )
	//{
	//	if ( DocBattleStage->GetRepeatBattleBoostOn() && DocBattleStage->GetRepeatBattleBoostRemainCount() < 1 )
	//	{
	//		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode( 4018 ); // FAILURE_NOT_ENOUGH_STAGE_BOOST_TICKET
	//		return;
	//	}
	//}
	//
	////연속전투
	//int32 CurrentClientStageID = StageManager->GetCurrentClientStageId();
	//bool FinishRepeatBattleLoop = StageManager != nullptr ? StageManager->IsEndStage(CurrentClientStageID) : true;
	//FServerStageID NextServerStageId = StageManager->GetNextServerStageID(CurrentClientStageID, AB2StageManager::GetCacheStageKeepEssentialData().GetStageDifficulty());

	//if ( IsRepeatBattleLoopAll() && NextServerStageId.Id == 0 && FinishRepeatBattleLoop )
	//{
	//	UB2UIManager::GetInstance()->OpenMsgPopup( EUIMsgPopup::Simple
	//		, BladeIIGetLOCText( B2LOC_CAT_GENERAL, TEXT( "SensitiveNoti_Notification" ) )
	//		, BladeIIGetLOCText( B2LOC_CAT_GENERAL, TEXT( "BattleStageInfo_ContBattleLastStage" ) )
	//		, 0.f
	//		, true
	//		, true
	//		, EUIMsgPopupButtonGroup::Confirm );

	//	StageManager->CancelOrStopRepeatBattle();
	//	return;
	//}
	//
	//auto& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	//StageData.RequestStartStageNext();
}

void AB2StageGameMode::RestartStage()
{
	if (GetStageManager())
	{
		auto& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
		StageData.RequestStartStageAgainAtPlace();
	}
}

void AB2StageGameMode::OverrideDefaultPawnStartLocation(AController* NewPlayerController, FVector& InOutStartLocation)
{
	AActor* PlayerStartOfStage = FindPlayerStartForCurrStage(NewPlayerController);
	if (PlayerStartOfStage)
	{
		InOutStartLocation = PlayerStartOfStage->GetActorLocation();
	}
}

void AB2StageGameMode::GoToMap()
{
	bFlagDeferredGoToMap = true;
	bRequestedGetActInfo = false;
	bRegistredDeferredUISceneChapter = false;	
}

void AB2StageGameMode::GoToDeferredStage(int32 InNextClientStageId)
{
	GoToNextArea();
	DeferredClientStageId = InNextClientStageId;
}

void AB2StageGameMode::GoToNextArea()
{
	GoToMap();
		
	bFlagDeferredGoToNextArea = true;
	bRequestedGetStageInfo = false;
	bRegistredDeferredUISceneStageInfo = false;
}

void AB2StageGameMode::GoToVillageDefeated()
{
	if (GetStageManager())
	{
		StageManager->RequestFailStageDefeat();
	}
	GoToVillage(); // 내지는 GoToNextArea 나 반복전투에서처럼 전투 대기 화면으로 가야할 수도.. 그런 경우 AB2StageGameMode::DeferredGoToLobby 쪽에서 GoGameStageInfoFromLobbyClass 에 편승을..
}

void AB2StageGameMode::GiveUpGame()
{
	if (GetStageManager())
	{
		StageManager->RequestFailStageGiveUP();
	}
}

void AB2StageGameMode::EnterShop(int32 nWhereShop)
{
	int32 NextClientStageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
	GoToDeferredStage(NextClientStageId);

	bEnterShop = true;
	
	nRequestGeneralShop = nWhereShop;

}

bool AB2StageGameMode::OpenEquipNumberNotice()
{
	//EPCClass FinalGoBackInvenPageDefClass;

	//if (!StageManager->GetCacheStageKeepEssentialData().EquipNumberCheck(FinalGoBackInvenPageDefClass))
	//	return false;

	//B2UIUtil::OpenInventoryFullPopup(FinalGoBackInvenPageDefClass, FMsgPopupOnClick::CreateLambda([this]() {
	//	bEnterInventory = true;
	//	int32 NextClientStageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
	//	GoToDeferredStage(NextClientStageId);
	//	}),
	//	false);

	return true;
}

void AB2StageGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DeferredGoToLobby();
}

const FString AB2StageGameMode::GetOpeningMovieName()
{
	if (GMinimalDLCFrontMode){ // DLC Front 모드 리소스로딩 최대한 제거. 정말 필요한 곳인지는 모르겠는데 여튼..
		FString(TEXT(""));
	}

	AB2StageManager* StageMgr = GetStageManager();
	if (StageMgr && StageMgr->IsScenarioMode()
		// 설정 구조 상 Extra 레벨(대체로 보스전) 전용 동영상 세팅이 없음. 이런 경우는 사실상 연이어 플레이하는 거니 Extra 가 아닌 메인 맵 로딩시에만 플레이
		&& !StageMgr->IsPlayingInExtraLevel() 
		)
	{
		// StageGameMode 플레이를 위한 리퀘스트를 통해서만 의미있는 StageId 가 들어가 있어야 함.
		int32 StageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
		FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(StageId) : NULL;
		if (StageInfoData && StageInfoData->HasScenarioMovie())
		{
			return StageInfoData->GetLocalizedMovieName();
		}		
	}
	return FString(TEXT(""));
}

void AB2StageGameMode::GetMovieSubtitles(TArray<B2MovieSubtitle>* Subtitles)
{
	if (Subtitles)
	{
		int32 StageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
		FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(StageId) : NULL;

		if (StageInfoData && StageInfoData->MovieSubtitles.Num() > 0)
		{
			Subtitles->Empty();
			Subtitles->AddDefaulted(StageInfoData->MovieSubtitles.Num());
			for (int32 i = 0; i < StageInfoData->MovieSubtitles.Num(); ++i)
			{
				auto& Subtitle = StageInfoData->MovieSubtitles[i];
				(*Subtitles)[i].Text = BladeIIGetLOCText(B2LOC_CAT_DIALOGTEXT, Subtitle.Text);
				(*Subtitles)[i].Color = Subtitle.Color;
				(*Subtitles)[i].StartTime = Subtitle.StartTime;
				(*Subtitles)[i].EndTime = Subtitle.EndTime;
			}
		}
	}
}

void AB2StageGameMode::DeferredGoToLobby()
{
	//bool bGoToLobby = false;
	//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	//auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	//if (bFlagDeferredGoToMap)
	//{		
	//	int32 ActId = StageDataStore.GetActIdByClientStageId(AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId());
	//	EStageDifficulty Difficulty = AB2StageManager::GetCacheStageKeepEssentialData().GetStageDifficulty();

	//	bGoToLobby = bRegistredDeferredUISceneChapter;
	//	if (!bRegistredDeferredUISceneChapter)
	//	{
	//		FLobbySceneManager::DeferredRegistChangeLobbyScene([ActId, Difficulty](){
	//			// 전체 씬 전환을 하면 실 로딩 시간이 늘어나므로 원하는 화면 직전까지 UI History 만 만들어준다. 
	//			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//			if (UIMgrInst){
	//				UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//				// 원래 아래 RequestGetActInfo 이후 응답으로 Chapter Scene 이 열려야 하는데 응답이 오지 않으면 망하므로 적어도 백버튼이라도 있는 Chapter UIScene 이라도 미리 열어둔다.
	//				UIMgrInst->ChangeUIScene(EUIScene::AdventureEnterBattleMain);
	//				UIMgrInst->ChangeUIScene(EUIScene::Chapter); 
	//			}
	//			BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(ActId, Difficulty);
	//		});			

	//		bRegistredDeferredUISceneChapter = true;
	//	}
	//	else if (!bRequestedGetActInfo)
	//	{
	//		StageDataStore.RequestGetActInfoAndChangeChapter(ActId, Difficulty);
	//		bRequestedGetActInfo = true;
	//	}
	//}

	//if (bFlagDeferredGoToNextArea && bGoToLobby)
	//{
	//	int32 NextClientStageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId() + 1;
	//	EStageDifficulty CurStageDifficulty = AB2StageManager::GetCacheStageKeepEssentialData().GetStageDifficulty();

	//	if (IsRepeatBattleOn() && !IsRepeatBattleLoopAll())
	//	{ // 반복전투에서 Stage 번호 유지한채 이 기능에 편승 꿀빨기. 아래 GoGameStageInfo 이벤트 날리는 거에도 편승해서 꿀빨기.
	//		NextClientStageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
	//		bRequestedGetStageInfo = true; // 이번 스테이지의 StageInfo 를 가지고 돌아갈 것이므로 또 Request 안해도 됨.
	//	}

	//	if (DeferredClientStageId != INDEX_NONE)
	//		NextClientStageId = DeferredClientStageId;

	//	UB2StageInfo* StageInfoTable = StaticFindStageInfo();

	//	if (StageInfoTable == nullptr)
	//		return;
	//	
	//	FServerStageID NextServerStageId;
	//	if (IsRepeatBattleLoopAll())
	//	{
	//		int32 CurrentClientStageID = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
	//		bool FinishRepeatBattleLoop = StageManager != nullptr ? StageManager->IsEndStage(CurrentClientStageID) : true;
	//		NextServerStageId = StageManager->GetNextServerStageID(CurrentClientStageID, CurStageDifficulty);

	//		if (NextServerStageId.Id == 0 && FinishRepeatBattleLoop)		//연속전투에서 NextServerID가 0이면 난이도 변경해야하는걸로 간주 멈춰준다
	//		{
	//			/*if (UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupBase>(UIFName::MsgPopupSimple))
	//				return;*/

	//			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_ContBattleLastStage")),
	//				0.f,
	//				true,
	//				true,
	//				EUIMsgPopupButtonGroup::Confirm
	//			);
	//			StageManager->CancelOrStopRepeatBattle();
	//			bFlagDeferredGoToMap = bFlagDeferredGoToNextArea = false;
	//			return;
	//		}
	//	}
	//	else
	//	{
	//		NextServerStageId = StageDataStore.GetServerStageID(NextClientStageId, CurStageDifficulty);
	//	}


	//	bGoToLobby = bGoToLobby && bRegistredDeferredUISceneStageInfo;
	//	if (StageDataStore.IsCachedStageData(NextServerStageId) && !bRegistredDeferredUISceneStageInfo)
	//	{		
	//		
	//		if (!bEnterInventory && !bEnterShop && IsRepeatBattleOn())
	//		{
	//			if (!StageDataStore.IsPossibleToDoRepeatBattle(StageManager->GetServerStageID()) )
	//			{
	//				//영혼검이 모자를경우 바로가기 팝업띄우기
	//				CancelOrStopRepeatBattle();
	//				bFlagDeferredGoToMap = bFlagDeferredGoToNextArea = false;
	//				HandleServerErrorGoodsShortageClass<const uint32, const EGoodsButtonType>::GetInstance().Signal(FItemRefIDHelper::GetGoodsID_BladePoint(), EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
	//				return;
	//			}
	//			else if (OpenEquipNumberNotice())
	//			{
	//				CancelOrStopRepeatBattle();
	//				bFlagDeferredGoToMap = bFlagDeferredGoToNextArea = false;

	//				return;
	//			}

	//		}
	//		// GetRepeatBattleRemainingCount 에서는 아직 줄지 않은 방금 완료한 스테이지 시작 시점에서의 값을 리턴하므로 다음번 반복전투를 위해서는 1 감소한 값을 준다.
	//		FRepeatBattleStateSet RepeatBattleStateSet = StageManager->GetRepeatBattleState();
	//		RepeatBattleStateSet.CurrentRepeatCount += 1;
	//		FLobbySceneManager::DeferredRegistChangeLobbyScene([NextServerStageId, RepeatBattleStateSet](){
	//			GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Signal(NextServerStageId, RepeatBattleStateSet);
	//			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterPage);
	//		});

	//		bRegistredDeferredUISceneStageInfo = true;
	//	}
	//	else if (!bRequestedGetStageInfo)
	//	{
	//		StageDataStore.RequestGetStageInfo(NextServerStageId);
	//		bRequestedGetStageInfo = true;
	//	}
	//}

	//if (bGoToLobby)
	//{
	//	if (bEnterInventory) //인벤토리로 가야한다면
	//	{
	//		bFlagDeferredGoToMap = bFlagDeferredGoToNextArea = false;
	//		FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//			LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//		});
	//		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	//	}

	//	OpenBladeIILobbyCommon(this); // 씬 전환 예약 후 본격 로비 맵 로딩

	//	if (bEnterShop)					//상점으로가야한다면
	//	{
	//		auto* DocStore = UB2UIDocHelper::GetDocStore();

	//		UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	//		if (UIMgr)
	//		{ // 자잘한 팝업이 뜬 상태에서 상점 진입하는 경우가 생길 수 있어서 그거 처리.
	//			UIMgr->CloseAllStandaloneUIs();
	//		}

	//		switch (static_cast<EStorePageWhere>(nRequestGeneralShop))
	//		{
	//		case EStorePageWhere::Store:
	//		{
	//			auto* StoreUI = UIMgr->GetUI<UB2UIStore>(UIFName::Store);
	//			if (StoreUI)
	//				StoreUI->SetTab();
	//			else
	//				data_trader::Retailer::GetInstance().RequestGetGeneralShop();
	//		}
	//			break;
	//		case EStorePageWhere::EquipStore:
	//		{
	//			auto* SummonItemUI = UIMgr->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
	//			if (SummonItemUI)
	//				SummonItemUI->SetTab();
	//			else
	//				data_trader::Retailer::GetInstance().RequestGetLotteryShop();
	//		}
	//			break;
	//		case EStorePageWhere::PackageStore:
	//		{
	//			data_trader::Retailer::GetInstance().RequestGetPackageShop();
	//		}
	//			break;
	//		case EStorePageWhere::MagicStore:
	//		{
	//			data_trader::Retailer::GetInstance().RequestGetMagicShop();
	//		}
	//		break;
	//		}

	//		bEnterShop = false;
	//	}
	//}
}

int32 AB2StageGameMode::GetCurrentStageNumber() const
{
	if (StageManager)
		return StageManager->GetCurrentClientStageId();

	return 1;
}

int32 AB2StageGameMode::GetCurrentServerStageNumber() const
{
	if (StageManager)
		return StageManager->GetServerStageID().Id;
	
	return 1;
}

void AB2StageGameMode::SetStageClearViewObjects(ACameraActor* InCamActor, UParticleSystemComponent* InPSWingComp, UParticleSystemComponent* InPSGemComp, UParticleSystemComponent* InPSBGComp)
{
	if (StageManager){
		StageManager->SetStageClearViewObjects(InCamActor, InPSWingComp, InPSGemComp, InPSBGComp);
	}
}

void AB2StageGameMode::PlayBGMCustomize(const FName& InName)
{
	//if (StageManager && BGMManager)
	//{
	//	StageManager->PlayCustomBGM(InName);
	//	BGMManager->PlayBGMByStageCustomize(InName);
	//}
}

void AB2StageGameMode::SetupUIScene()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::BattleMain);
}

void AB2StageGameMode::OnStageBegin()
{	
	//B2GMLoadingProgCollector::StepToMax(); // 대체로 SetupUIScene 직전에 로딩바 꽉 채우면 적절한 듯
	//SetupUIScene();

	//if (auto* BattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain))
	//	BattleMainUI->OnStageBegin();
}

void AB2StageGameMode::OnBeginPlayOfRepeatBattle()
{
	
	// 반복전투 auto 시작은 TryAutoBeginAutoAtBeginning 에서..
	
}

void AB2StageGameMode::OnEndPlayOfSingleRepeatBattleStage()
{
	//FRepeatBattleStateSet RepeatBattleState = AB2StageManager::GetCacheStageKeepEssentialData().GetRepeatBattleState();

	//// [반복전투] 일경우, 로비로 돌아가지 않고, 해당 모험맵을 바로 로딩 처리 한다
	//if ( RepeatBattleState.bIsOn && RepeatBattleState.bRepeatOne )
	//{
	//	RestartGame();
	//}
	//else if ( RepeatBattleState.bIsOn && RepeatBattleState.bRepeatAll )
	//{
	//	RestartGameNext();
	//}
	//else
	//{
	//	GoToNextArea();
	//}
}

bool AB2StageGameMode::IsRepeatBattleOn() 
{ 
	return GetStageManager() ? StageManager->GetRepeatBattleState().bIsOn : false; 
}

bool AB2StageGameMode::IsRepeatBattleLoopAll()
{
	return GetStageManager() ? StageManager->GetRepeatBattleState().bRepeatAll : false;
}

int AB2StageGameMode::GetRepeatBattleRemainingCount() 
{ 
	return GetStageManager() ? StageManager->GetRepeatBattleState().CurrentRepeatCount : 0;
}

void AB2StageGameMode::CancelOrStopRepeatBattle()
{
	if (GetStageManager() && IsRepeatBattleOn())
	{
		StageManager->CancelOrStopRepeatBattle();
		//SetPlayerAutoType(AutoBattleType::NONE);
	}
}

bool AB2StageGameMode::HasStageBuff(EStageBuffType InType)
{
	if (GetStageManager())
	{
		return StageManager->HasStageBuff(InType);
	}
	return false;
}

bool AB2StageGameMode::ShouldStartAutoFromBeginning()
{
	if ( Super::ShouldStartAutoFromBeginning() || // 상위 조건을 포함해야 한다.
		IsRepeatBattleOn() )
	{
		return true;
	}
	return false;
}
