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
	// StageManager �� �ʼ� ������(CurrentStageId ��)�� �� �������� �������� �� ��.
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

	//if (IsUsingMobileRendering(GetWorld())) { // ����� �������� dynamic point light ���� ������ ����.
	//	DeactivateWorldMovablePointLight();
	//}
}

AB2StageManager* AB2StageGameModeBase::GetStageManager()
{
	UWorld* TheWorld = GetWorld();
	if (StageManager == NULL && TheWorld)
	{
		// �� �⺻ ���� �ְ� spawn ��Ŵ
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
	if (!(StageManager && StageManager->IsStageCleared())) // �й� �޴��� ���� �Ǵµ� �������� Ŭ���� ���Ŀ��� �۵����� �ʰ�..
	{
		CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
		// ��Ȱ �޴� ����. RequestResurrect ���� ��Ȱ�� �ϰų�, Ȥ�� �ٽ� �����ϸ� ���� ��.
		StartDefeatMenuClass<EB2GameMode>::GetInstance().Signal(EB2GameMode::Scenario);
	}
}

void AB2StageGameModeBase::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLiveMobs)
{
	GetAllWorldMobList(OutList, bOnlyLiveMobs);
		
	TArray<ABladeIICharacter*> RemoveList;

	for (ABladeIICharacter* pChar : OutList)
	{
		// ž�µȸ��� ����
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
	// ���� ���õ� DifficultyLevel �� Info ���� (������ ���� ������ ������) ���� ��ԵǴ� ���� ����Ǵ� scale �� ����
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
	if (StageManager){ // Ȥ �𸣴� ����Ʈ���� �����ϱ� ���� ó���ϴ� �� ���� ��?
		StageManager->NotifySpawnPoolWaveMobDead(SpawnPool, DyingMob, WaveNum, MobIndex);
	}

	//// ���� ��Ÿ ����
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
		LastRequestedStageBuffType = InResurrectBuffType; // ��Ȱ �����ÿ� �������� ������ ��Ȱ������ ��� �𿩼� �� �ǵ� �� Ÿ�Ը� ������ ��.
		data_trader::Retailer::GetInstance().RequestResurrection(AB2StageManager::GetCacheStageKeepEssentialData().GetPlayToken(), ServerResurrectBuffType);
		return true;
	}
	return false;
}

void AB2StageGameModeBase::DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty)
{
	//// �⺻ ���۰��� �޸� StageId �� Difficulty �� ����ؼ� �´� �� �����´�.
	//// �������� ���� ������ �ѹ� ���ϸ� �ٲ��� �ʴ´�.
	//if (StageManager)
	//{
	//	TheActiveSpawnPool = GetDesiredSpawnPoolOfSettingFromList(SpawnPools, 
	//		// �����̵�, PlayInEditor �� StageManager �� PreBeginPlay ���Ķ�� StageId �� DifficultyLevel ���� ��� �����ؾ�.
	//		StageManager->GetCurrentClientStageId(), StageManager->GetStageDifficultyLevel(), bFallBackToLowerDifficulty);
	//}
	//else
	//{
	//	Super::DecideAndSetActiveSpawnPool(); // ������ ������� ����
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
	{ // �����Ϳ��� ��� skill �ִϸ��̼��� �ε��ϵ��� �ϴ� ���� Ŭ���� ����� ���
		return ABladeIIGameMode::GetPCSkillAnimsToPreLoad();
	}
#endif

	TArray<FCombinedPCSkillAnimID> RetArray;

	//TArray<EPCClass> AllPCClassToPreLoad = GetPCClassesToPreLoad();

	//// �� PCClass ���� ������ ��ų �ִϸ��̼ǵ��� �ֵ���.
	//// StageGameMode �̹Ƿ� LocalCharacterData ������ �����ϴ�.

	//FLocalCharacterData& LocalPlayerCharData = BladeIIGameImpl::GetLocalCharacterData();
	//
	//for (EPCClass ThisPCClass : AllPCClassToPreLoad)
	//{
	//	TArray<ESkillAnimType> SkillAnimsOfThisClass;
	//	UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, ThisPCClass, GetB2GameModeType(), SkillAnimsOfThisClass, &LocalPlayerCharData);

	//	// �̹� ĳ���� Ŭ�������� ���� SkillAnimType ��� Combined ID �� ����� ����.
	//	for (ESkillAnimType ThisSkillAnimType : SkillAnimsOfThisClass)
	//	{
	//		RetArray.Add(FCombinedPCSkillAnimID(ThisPCClass, ThisSkillAnimType));
	//	}
	//}
	return RetArray;
}

void AB2StageGameModeBase::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	// TAsset �ε��ϴ� InfoAsset ���� �����. �ݴ�� ManualUnloadBeforeNewBeginning

	//if (GetStageManager())
	//{
	//	StageManager->PreloadAdditionalNecessaryInfo();
	//}
	//
	////Synchronous �ε��� Async �ε� flush ���ο� ����.. �̰� �ٸ� async request ���� ���� Ȥ�� ���Ŀ�..
	//PreloadInGameInfoAsset_SynchronousOnly(bAboutToPlayLoadingMovie);

	//PreloadAllExpectedDialogAssets();

	//Super::PreloadAnyNecessaryInfo(bAboutToPlayLoadingMovie);
	//
	//PreloadInGameInfoAsset_AsyncAllowed(bAboutToPlayLoadingMovie);

	//// �±� ĳ���� Mesh merge �� �̸� ������ ����. Preload ����. 
	//// �׷��� B2CompositeMeshCache �� RooSet ó���� ����ϰ� �Ǹ� �̰� ������.
	//PreBuildTagCharMesh(); 
}

void AB2StageGameModeBase::PreBuildTagCharMesh()
{
	// B2CompositeMeshCache �� RooSet ó���� ����ϰ� �Ǹ� �̰� ������.
	//PrebuildLocalTagCharMesh();
}

void AB2StageGameModeBase::GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode)
{
	// �̹� �������� flow �� �ߵ� ������ �ó����� ���̾�α� ������ �ڵ���ӵ� �ܾ������.

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
	// ���̾�α׿� ����ϴ� �ؽ��Ķ� �����.. �̵��� �ε� ��ü�� on-demand �� �ϱ� ���̴ٱ⺸�� �ٸ� async �ε� flush ���� ������ �� ŭ.

	B2_SCOPED_TRACK_LOG(TEXT("AB2StageGameModeBase::PreloadAllExpectedDialogAssets"));

	//TArray<FName> AllDlgCodes;
	//GetAllExpectedDialogCodeNames(AllDlgCodes);

	//UB2ScenarioDialogInfo* DlgInfoTable = StaticFindDialogInfoTable();
	//if (DlgInfoTable)
	//{ // �Ϲ����� dialog �� �ʿ��� �͸� �ε��� ����..
	//	DlgInfoTable->PreloadAssetsForStageGame(AllDlgCodes, this, false);

	//	// Talkdialog ������ preload �Ϸ��� �� ���� �� ��.. �װ� ���� �������� �����̳� ���� �� ������ �Ŷ� ���� �ʿ��ұ� ������..
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
	if (ThisStageInfo && !ThisStageInfo->PlayerStartTag.IsEmpty() && !StageManager->IsPlayingInExtraLevel()) // PlayerStartTag �� MainMap �� ������.
	{
		return FindPlayerStart(InController, ThisStageInfo->PlayerStartTag);
	}
	return FindPlayerStart(InController);
}

void AB2StageGameModeBase::ResetAssumeNoWorldLoading()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageGameModeBase::ResetAssumeNoWorldLoading"));
//
//	// ���� �ε� ���� ���� �÷��̿� �ʿ��� �����͵鸸 �����ؼ� ���� �����Ϸ��� �����.
//	// �������� ��Ȳ���� �����.. �Ϲ����� �������� ���ӿ��� ����Ϸ��� �� �ƴ�.
//	// StageNumber �� Difficulty �� �ʿ��ϴٸ� �� ������ �̹� ���ο� ������ �޾��� �־�� ��. 
//
//#if !UE_BUILD_SHIPPING
//	CheckResetAssumeNoWorldLoadingCondition(this);
//#endif
//	
//	// OnPreLoadMap ���� �ϴ� ���� ���� ����..
//	SaveLocalIniStateOnPreLoadMap();
//
//	UpdateManagedActorList();
//
//	//UI���Ŵ��� �ʱ�ȭ
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	if (UIDocManager) {
//		UIDocManager->Init(this);
//	}
//
//	// ���� SpawnPool �¾��� �ٲ� ���ɼ��� �����Ѵٸ� Preload �� �ʿ��Ҽ��ۿ�.. �� �׷��� �߰��� ���� Ƣ����� �״�..
//	// ���� �������� �͵��� Unload �ϴ� �� �����ұ�.. �װͱ��� ���� �ᱹ �ε��ð��� ���� �ɸ��� ���ٵ�..
//	PreloadAnyNecessaryInfo(false);
//	
//	// AutoAiController�� �������� Ŭ���� ���õȰ� ����
//	ABladeIIPlayerController* pPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	if (pPC)
//	{
//		pPC->ResetAutoAiController();
//
//		// �Է� ������ ����
//		pPC->SetStageEndNonCombatState(false);
//	}
//
//	// ���� ���� �������� �÷��̾� ĳ���͵� Respawn.
//	ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	UB2SkillRTManager* LocalPlayerSkillMgr = LocalPlayerChar->GetSkillRTManager();
//	if (LocalPlayerSkillMgr)
//		LocalPlayerSkillMgr->SimpleRebirth();
//
//	// SimpleRebirth �� �����ߴٸ� LocalPlayerCharacter �� �ٲ�� ���� ��.
//	LocalPlayerChar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalPlayerChar)
//	{
//		LocalPlayerChar->EnableInput(UGameplayStatics::GetLocalPlayerController(this)); // Rebirth �������� DisableInput �ϴ� �� �־ Enable ����.
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
//	// Spawn �� �Բ� ������ ���۵� �� ������ Player ĳ���Ͷ� ���� ���� ����ó���� �� �Ŀ� SpawnPool ����.
//	if (GetActiveSpawnPool())
//	{
//		GetActiveSpawnPool()->ResetAssumeNoWorldLoading();
//	}
//	
//	if (!bDeferAutoBeginAuto) // �̰� ���� ��Ȳ���� DeferAutoBeginAuto �� true �� �� �� ���� ������..
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
//	{ // �� �ݺ����� �������� ���۽ø���..
//		OnBeginPlayOfRepeatBattle();
//	}
//	
//	if (!IsInPreRenderPhase()) // Pre-render �ÿ��� �װ� ���� �� RestoreWholeGameTimeScale �� ��.
//	{
//		RestoreWholeGameTimeScale(); // BeginPlay �ε� �� Restore �ΰ� ������ �� �ʿ��� �⺻ ���� �ϴ� �ɷ� ���� ��.
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
//		FString StageInfoMainMapName = ThisStageInfo->MainMap.ToString(); // ���� �Ƹ� ��δ� ���� ��.
//		FString StageInfoExtraMapName = ThisStageInfo->ExtraMap.ToString();
//		// ���� �ƴ� �� ���ؼ� ���ʿ��� �޽��� �ڽ� �ȶ�����..
//		const bool bIsProbablyMatchingMainLevel = (CurrLevelName.Len() >= StageInfoMainMapName.Len() && CurrLevelName.EndsWith(StageInfoMainMapName));
//		// �����Ϳ��� �ܵ� �÷��̽� ExtraMap �Ǻ�.
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
//					TEXT("���� �÷����ϴ� ���� %s ���� StageInfo %d �� ������ %s �±׸� ���� PlayerStart�� ã�� �� ����. ��ǻ�Ͱ� �� �����Ѵ�."),
//					*CurrLevelName, StageManager->GetCurrentClientStageId(), *ThisStageInfo->PlayerStartTag
//					)));
//#endif
//			}
//		}
//		int32 TotalPlayerStart = 0;
//		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
//		{
//			APlayerStart* PlayerStart = *It;
//			APlayerStartPIE* PlayerStartPIE = Cast<APlayerStartPIE>(PlayerStart); // ��ü �̰� ����..
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
//				TEXT("���� �÷����ϴ� ���� %s �� PlayerStart �� %d ���̳� StageInfo %d �� PlayerStartTag �� �����Ǿ� ���� �ʰų� Ȥ�� ExtraMap �Դϴ�."),
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

	// ExtraMap �ε� �ÿ��� ������ �÷����ߴ� Ŭ������ �����ؾ� �Ѵ�.
	// PlayInExtraLevel �̳� LastPlayedClass ��� extra map �ε� ���� ������ ��� ���õǾ� ���� ��.
	if (AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel()){
		CurrentPCClassEnum = AB2StageManager::GetCacheStageKeepEssentialData().GetPreExtraLastPlayedClass();
	}
#if WITH_EDITOR
	if (GetStageManager() && GetStageManager()->IsStandalonePIE())
	{ // StageManager �� PreBeginPlay �� �̰ͺ��� �ռ� ����Ǿ��ٸ� �κ� ������ ���� �ܼ� Play-in-Editor ������ ����� ���� ��.
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

	// [������] �ϰ�� ����ٿ�带 �׻� üũ�ϸ�, �����Ÿ�(-50M) ���Ϸ� ���Ͱ� ������ ���, ��� ó��
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
	{ // ���� �ӵ� ���̱�.
		float BoostScale = BladeIIGameImpl::GetClientDataStore().GetStageBuffIncreaseGamseSpeedRate();
		//SetWholeGameTimeScale(BoostScale); // SetGlobalTimeDilation ���� ���� ���� ��ݿ� �����.
	}
	else
	{
		Super::RestoreWholeGameTimeScale();
	}
}

void AB2StageGameMode::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);

	//// ù Spawnpool NPCClassInfo �� preload��, InGame �������ڸ��� ������ Spawnpool Asyncload
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

	//// [��������] 3����迡�� Ƽ���� �����Ұ�� ������ ���߰� �˾��� ����ش�
	//if ( auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage() )
	//{
	//	if ( DocBattleStage->GetRepeatBattleBoostOn() && DocBattleStage->GetRepeatBattleBoostRemainCount() < 1 )
	//	{
	//		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode( 4018 ); // FAILURE_NOT_ENOUGH_STAGE_BOOST_TICKET
	//		return;
	//	}
	//}
	//
	////��������
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
	GoToVillage(); // ������ GoToNextArea �� �ݺ���������ó�� ���� ��� ȭ������ ������ ����.. �׷� ��� AB2StageGameMode::DeferredGoToLobby �ʿ��� GoGameStageInfoFromLobbyClass �� �����..
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
	if (GMinimalDLCFrontMode){ // DLC Front ��� ���ҽ��ε� �ִ��� ����. ���� �ʿ��� �������� �𸣰ڴµ� ��ư..
		FString(TEXT(""));
	}

	AB2StageManager* StageMgr = GetStageManager();
	if (StageMgr && StageMgr->IsScenarioMode()
		// ���� ���� �� Extra ����(��ü�� ������) ���� ������ ������ ����. �̷� ���� ��ǻ� ���̾� �÷����ϴ� �Ŵ� Extra �� �ƴ� ���� �� �ε��ÿ��� �÷���
		&& !StageMgr->IsPlayingInExtraLevel() 
		)
	{
		// StageGameMode �÷��̸� ���� ������Ʈ�� ���ؼ��� �ǹ��ִ� StageId �� �� �־�� ��.
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
	//			// ��ü �� ��ȯ�� �ϸ� �� �ε� �ð��� �þ�Ƿ� ���ϴ� ȭ�� �������� UI History �� ������ش�. 
	//			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//			if (UIMgrInst){
	//				UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//				// ���� �Ʒ� RequestGetActInfo ���� �������� Chapter Scene �� ������ �ϴµ� ������ ���� ������ ���ϹǷ� ��� ���ư�̶� �ִ� Chapter UIScene �̶� �̸� ����д�.
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
	//	{ // �ݺ��������� Stage ��ȣ ������ä �� ��ɿ� ��� �ܻ���. �Ʒ� GoGameStageInfo �̺�Ʈ ������ �ſ��� ����ؼ� �ܻ���.
	//		NextClientStageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
	//		bRequestedGetStageInfo = true; // �̹� ���������� StageInfo �� ������ ���ư� ���̹Ƿ� �� Request ���ص� ��.
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

	//		if (NextServerStageId.Id == 0 && FinishRepeatBattleLoop)		//������������ NextServerID�� 0�̸� ���̵� �����ؾ��ϴ°ɷ� ���� �����ش�
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
	//				//��ȥ���� ���ڸ���� �ٷΰ��� �˾�����
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
	//		// GetRepeatBattleRemainingCount ������ ���� ���� ���� ��� �Ϸ��� �������� ���� ���������� ���� �����ϹǷ� ������ �ݺ������� ���ؼ��� 1 ������ ���� �ش�.
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
	//	if (bEnterInventory) //�κ��丮�� �����Ѵٸ�
	//	{
	//		bFlagDeferredGoToMap = bFlagDeferredGoToNextArea = false;
	//		FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//			LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//		});
	//		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	//	}

	//	OpenBladeIILobbyCommon(this); // �� ��ȯ ���� �� ���� �κ� �� �ε�

	//	if (bEnterShop)					//�������ΰ����Ѵٸ�
	//	{
	//		auto* DocStore = UB2UIDocHelper::GetDocStore();

	//		UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	//		if (UIMgr)
	//		{ // ������ �˾��� �� ���¿��� ���� �����ϴ� ��찡 ���� �� �־ �װ� ó��.
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
	//B2GMLoadingProgCollector::StepToMax(); // ��ü�� SetupUIScene ������ �ε��� �� ä��� ������ ��
	//SetupUIScene();

	//if (auto* BattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain))
	//	BattleMainUI->OnStageBegin();
}

void AB2StageGameMode::OnBeginPlayOfRepeatBattle()
{
	
	// �ݺ����� auto ������ TryAutoBeginAutoAtBeginning ����..
	
}

void AB2StageGameMode::OnEndPlayOfSingleRepeatBattleStage()
{
	//FRepeatBattleStateSet RepeatBattleState = AB2StageManager::GetCacheStageKeepEssentialData().GetRepeatBattleState();

	//// [�ݺ�����] �ϰ��, �κ�� ���ư��� �ʰ�, �ش� ������� �ٷ� �ε� ó�� �Ѵ�
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
	if ( Super::ShouldStartAutoFromBeginning() || // ���� ������ �����ؾ� �Ѵ�.
		IsRepeatBattleOn() )
	{
		return true;
	}
	return false;
}
