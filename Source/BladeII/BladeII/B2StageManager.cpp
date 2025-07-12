 //Fill out your copyright notice in the Description page of Project Settings.


#include "B2StageManager.h"
#include "Engine.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#include "B2StageGameMode.h"
#include "BladeIIPlayer.h"
#include "B2MonsterSpawnPool.h"
#include "B2StageEventDirector.h"
//#include "B2AutoWayPoint.h"
#include "GameFramework/GameState.h"
#include "BladeIIPlayerController.h"
#include "B2ActiveCameraActor.h"
#include "BladeIIGameMode.h"
#include "BladeIIWorldSettings.h"
#include "Event.h"
#include "EventSubsystem.h"
#include "B2ItemInfo.h"
#include "B2DropItem.h"
#include "B2TreasureChestBase.h"
#include "B2Inventory.h"
#include "B2UIDocHelper.h"
#include "B2ScenarioDialogInfo.h"
#include "B2UIScenarioDialog.h"
#include "B2UIManager.h"
//#include "Camera/CameraActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "BladeIIGameMode.h"
#include "Engine/ExponentialHeightFog.h"
#include "Atmosphere/AtmosphericFog.h"
#include "B2SkillRTManager.h"
#include "B2PCClassInfoBox.h"
#include "B2NPCClassInfoBox.h"
#include "B2SomeInfo.h"
#include "B2UITalkDialog.h"
#include "B2CodeTable.h"
#include "B2UIUtil.h"
#include "B2UIEtherFullPopup.h"
#include "B2CombatStatEvaluator.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "B2NetworkBridge.h"
#include "OnlineWholesaler.h"
#include "Retailer.h"
#include "MyAssetStore.h"
#include "BladeIIUtil.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "QuestManager.h"
#include "B2HeroTowerGameMode.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "TutorialManager.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
#include "B2UITutorial.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2UIMsgPopupInventoryFull.h"
#include "B2DimensionGameMode.h"
#include "Retailer.h"

#if !UE_BUILD_SHIPPING
/** For device performance related test. 0.0 spawn no gold. 1.0 spawns all required gold drop manifestation. */
float GCheat_DropGoldSpawnRate = 1.0f;
#endif

// Sets default values
AB2StageManager::AB2StageManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CachedGameMode = NULL;

	bCurrentLevelCleared = false;
	bStageCleared = false;
	IsPossiblePlayBossBgm = true;

	bStopAllMobs = false;

	LastUpdatedPlayTime = 0.0f;
	PlayTimeLastCheckedCache = 0.0f;
	bCountingPlayTime = true;

	DelayedDropSpawnSerialNumber = 0;

	StageClearViewCam = NULL;
	StageClearWingFxComp = NULL;
	StageClearGemFxComp = NULL;
	StageClearBGFxComp = NULL;

	CurrentBossMobCount = 0;

	StagePlayTimeAutoUpdateInterval = 0.2f; // �������� ��Ȯ���� ������ ��ġ���� ������ ���� UI ǥ�õ� �Ͱ� ��� UI ǥ�ÿ����� ���̸� ������ ���� �ʿ䰡 �־ ���� ������..

	CurrentPlayingDialogSound = NULL;
	
	CustomaizeBGM = false;
}

void AB2StageManager::PreBeginPlay()
{	
//	// �ٸ� Actor ���� BeginPlay �� �Ҹ��� ���� ó���Ǿ�� �� �͵��� ���⿡ ���ų־� ����.
//#if WITH_EDITOR
//	if (GEditor && GEditor->bIsPlayWorldQueued) // PlayInEditor �� �κ� ���� �� ���� ���⸦ ������� ���� ��.
//		StaticCacheStageData.Initialize();
//#endif
//	
//	ScenarioDialogManager.SetStageManager(this);
//
//	GetStageRewardManager().SetStageManager(this);
//	//[@AKI, 180213] �̰Ŵ� ���� �ȵǰ� �ϴ� �̰Ŵ� ������ �ҵ�(�̰� StageId���ٲٳ�....)
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!Utilities::FConnectionState::GetInstance().IsConnected()
//		// FConnectionState �� �˻��Ѵٸ� �ѹ��̶� �κ�� ���� ������ ���Ŀ��� ���� ������ �ɷ� ó���Ǿ� ������ ������ ����� ���� ����. 
//		// ������ �� �׳� Open �� ���� �÷��̿����� ���ǻ� standalone ��� ����
//		|| DevOnlyIsStandalonePlay()
//		)
//	{
//		StaticCacheStageData.SetStandaloneStageData(this);
//	}
//#endif
//
//	RefreshStagePlayTimeCheck(); // ExtraMap ������� ī��Ʈ�� ����
//
//	// ��Ÿ �ٸ� �����͵�..
//	
//	SyncDataToUIDoc();
//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//	{
//		UB2UIDocHero* UIDocClass = UB2UIDocHelper::GetDocHero(PCI);
//		if (UIDocClass)
//		{ // �������� Ŭ������ �� (�������ϸ�) ���õǴ� ������. ������ �� ����.
//			UIDocClass->SetLastAcquiredSkillPoint(0);
//		}
//	}

	SubscribeEvents(); // BeginPlay ������ �Ҹ��� �̺�Ʈ�� ���� �״� �� ������ ���� ����� ���ƾ�.
}

// Called when the game starts or when spawned
void AB2StageManager::BeginPlay()
{
	Super::BeginPlay();
	
	StageBeginHandling();

	BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = -1; // �� �������� �� �̻� �ʿ����� ���� �Ӵ��� ���� ������ ������ �÷��̿� ���ذ� ��
}

void AB2StageManager::BeginDestroy()
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogBladeII, Log, TEXT("!!! StageManager BeginDestroy !!!"));
#endif

//	UWorld* MyWorld = GetWorld();
//	if (MyWorld)
//	{
//		MyWorld->GetTimerManager().ClearAllTimersForObject(this);
//	}
//
//	UnsubscribeEvents();
//
//#if WITH_EDITOR
//	if (GEditor && GEditor->bRequestEndPlayMapQueued)
//		StaticCacheStageData.Initialize();
//#endif 

	Super::BeginDestroy();
}

void AB2StageManager::Destroyed()
{
	UnsubscribeEvents();
	Super::Destroyed();
}

void AB2StageManager::StageBeginHandling()
{
	//bCurrentLevelCleared = false;
	//bStageCleared = false;
	//IsPossiblePlayBossBgm = true;

	//// Note: �ٸ� actor ���� BeginPlay ������ ó���� �ʿ��� �κ��� ABladeIIGameMode::PreBeginPlay �� ����.
	//// �̰��� �Ҹ��� ������ �ܼ��� actor ���� BeginPlay �� ó���Ǵ� ������. ���İ���� ���ǵ��� ����.

	//SyncDataToUIDoc(); // �� �ѹ� ������ ��.

	//if (StaticCacheStageData.bPlayInExtraLevel) // ExtraLevel �ε� ���� ó��. SetReadyPlayingExtraMap �� �ݴ�
	//{
	//	//ABladeIIGameMode::HideHUDCenterMessage(); // Extralevel �ε� ��� �����ϸ鼭 �״� ���� �޽��� ����. Ȥ�� �� ���̿� �ٸ� ���� �޽����� �ʿ������ٸ�..?
	//	// ABladeIIGameMode::HideHUDCenterMessage �� ������ �ȸ����µ� ���� ���� �Ķ� ���ʿ��� ĳ���� ���� ���۷����� �ǹ̰� ���� �����̴�. ��¿ �� ����.
	//	UB2UIManager::GetInstance()->CloseAllMsgPopups();
	//	RetrieveDataForExtraMap();
	//}

	//PlayStageBGM();

	//UpdateStagePlayTime(false); // UI ǥ�ø� ���� ó�� �ѹ� Ȯ���� �ҷ��� �ʿ䰡 ����.
	//GetWorldTimerManager().SetTimer(AutoUpdateStagePlayTimeTH, this, &AB2StageManager::AutoUpdateStagePlayTimeCB, StagePlayTimeAutoUpdateInterval, false);

	//StageBeginClass<>::GetInstance().Signal(); // UI �� ��ȣ.
}

void AB2StageManager::SyncDataToUIDoc()
{ // UI ���� �� �����͵� ����.
	//UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	//if (DocBS)
	//{
	//	DocBS->SetClientStageId(StaticCacheStageData.ClientStageId);
	//	DocBS->SetStageDifficulty(StaticCacheStageData.StageDifficultyLevel);
	//	DocBS->SetServerStageId(StaticCacheStageData.ServerStageID);

	//	ABladeIIWorldSettings* B2WS = GetWorld() ? Cast<ABladeIIWorldSettings>(GetWorld()->GetWorldSettings()) : NULL;
	//	DocBS->SetLocalizedAreaName(B2WS ? B2WS->GetLocalizedAreaNameText() : FText::FromString(TEXT("")));
	//	DocBS->SetLocalizedAreaSubName(B2WS ? B2WS->GetLocalizedAreaSubNameText() : FText::FromString(TEXT("")));
	//	DocBS->SetStagePlayTime((int32)LastUpdatedPlayTime);

	//	DocBS->SetRepeatBattleCurrentOn(GetRepeatBattleState().bIsOn);
	//	DocBS->SetRepeatBattleCurrentCount(GetRepeatBattleState().CurrentRepeatCount);
	//	DocBS->SetRepeatBattleLoopAll(GetRepeatBattleState().bRepeatAll);
	//	DocBS->SetRepeatBattleLoopOne(GetRepeatBattleState().bRepeatOne);
	//	DocBS->SetRepeatBattleBoostOn(GetRepeatBattleState().bBoostOn);
	//}
}

void AB2StageManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(BeginStageClearScene_Phase1Class<>::GetInstance().Subscribe2(
		[this]()
	{
		this->CheckBeginStageClearScene_Phase1();
	}
	));

	Issues.Add(BeginStageClearScene_Phase2Class<>::GetInstance().Subscribe2(
		[this]()
	{
		this->BeginStageClearScene_Phase2();
	}
	));

	Issues.Add(ShowStageClearWingEffectClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->StageClearPhase1_StartVictoryWingFx();
	}
	));

	Issues.Add(ShowStageClearGemEffectClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->StageClearPhase1_StartVictoryGemFx();
	}
	));

	Issues.Add(DeliveryClearStageClass<FB2ClearStageInfo>::GetInstance().Subscribe2(
		[this](const FB2ClearStageInfo& info)
	{
		this->ResponseStageClear(info);
	}
	));

	Issues.Add(DeliveryFinishCounterDungeonClass<FB2FinishCounterDungeonInfo>::GetInstance().Subscribe2(
		[this](const FB2FinishCounterDungeonInfo& info)
	{
		this->ResponseFinishCounterDungeon(info);
	}
	));

	Issues.Add(OpenDialogClass<FName, bool>::GetInstance().Subscribe2(
		[this](FName DialogCodeName, bool bMaintainBattleUI)
	{
		this->ScenarioDialogManager.SetScenarioDialog(DialogCodeName, bMaintainBattleUI);
	}
	));

	Issues.Add(NextDialogClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->NextDialogLineWrapper();
	}
	));

	Issues.Add(SkipDialogClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->SkipDialogWrapper();
	}
	));

	Issues.Add(PlayStageBGMClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->PlayBGM();
	}
	));

	Issues.Add(DeliveryFinishHeroTowerClass<FB2ResponseFinishHeroTowerPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseFinishHeroTowerPtr& MsgPtr)
	{
		this->ResponseHeroTowerClear(MsgPtr);
	}
	));

	Issues.Add(DeliveryFinishDimensionClass<FB2ResponseFinishDimensionPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseFinishDimensionPtr& MsgPtr)
	{
		this->ResponseDimensionClear(MsgPtr);
	}
	));

	Issues.Add(StartPauseMenuClass<EB2GameMode>::GetInstance().Subscribe2(
		[this](EB2GameMode GameMode)
	{
		this->PauseDialog(true);
	}
	));

	Issues.Add(StopPauseMenuClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->PauseDialog(false);
	}
	));

	Issues.Add(StageEventTalkStartClass<int32, bool>::GetInstance().Subscribe2(
		[this](const int32 GroupID, const bool IsOpen)
	{
		this->OnTalkDialogStart(GroupID, IsOpen);
	}
	));

	Issues.Add(StageEventTalkEndClass<int32, bool>::GetInstance().Subscribe2(
		[this](const int32 GroupID, const bool IsOpen)
	{
		this->OnTalkDialogEnd(GroupID, IsOpen);
	}
	));
}

void AB2StageManager::UnsubscribeEvents()
{
	Issues.Empty();
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
bool AB2StageManager::DevOnlyIsStandalonePlay() const
{ // ������ ���� ���� ��Ż standalone �÷��� ����
#if WITH_EDITOR
	if (IsStandalonePIE()){
		return true;
	}
#endif

	if (FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode()) { // �̰� �����ϴ� �� �� ������ ��..
		return true;
	}

	if (GEngine)
	{
		FWorldContext* TheWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (TheWorldContext)
		{ // Open ��ɿ� Standalone ����. ���� ��� Open LV01_02?Standalone?StageNum=2 �̷� ������
			if (TheWorldContext->LastURL.HasOption(TEXT("Standalone")))
			{
				return true;
			}
		}
	}
	return false;
}
#endif

void AB2StageManager::NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	//if (InBirthplaceInfo.Birthplace && SpawnedMob)
	//{
	//	if (bStopAllMobs)
	//	{
	//		SpawnedMob->PreventMovementByStageEvent(); // StopOrRestoreAllWorldMobs ȣ�� ���� ���Ŀ� spawn �� �͵鵵 ó��.
	//	}

	//	if (InBirthplaceInfo.bSpawnedAsBossMob)
	//	{
	//		/*const auto* StageSetting = InBirthplaceInfo.Birthplace->GetDesiredStageSetting(StaticCacheStageData.ClientStageId, StaticCacheStageData.GetStageDifficulty());

	//		BII_CHECK(StageSetting)
	//		BII_CHECK(StageSetting->WaveSettings.IsValidIndex(InBirthplaceInfo.WaveNumber))

	//		auto& WaveInfo = StageSetting->WaveSettings[InBirthplaceInfo.WaveNumber];

	//		if(WaveInfo.Wave_AppointmentBGMName.IsNone())
	//		{
	//			CurrentBossMobCount++;
	//			PlayBossStageBGM();
	//		}
	//		else
	//		{
	//			PlayCustomBGM(WaveInfo.Wave_AppointmentBGMName);
	//		}*/

	//		CurrentBossMobCount++;
	//		PlayBossStageBGM();
	//	}
	//}
}

void AB2StageManager::NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool)
{
	CheckStageClear(); // SpawnPool ����� ��ü�� StageClear �� �̾����� �߰� ������ �ʿ��� �� ����.
}

void AB2StageManager::NotifySpawnPoolSingleWaveStart(AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum)
{
}

void AB2StageManager::NotifySpawnPoolSingleWaveComplete(AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum)
{
}

void AB2StageManager::NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex)
{
	//BII_CHECK(DyingMob && DyingMob->IsValidObj());
	//
	//if (DyingMob && DyingMob->IsSpawnedAsBossMob()){
	//	CurrentBossMobCount = FMath::Max(CurrentBossMobCount-1, 0);
	//}
}

void AB2StageManager::NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase)
{
	// DestroyedPrevRef �� NULL �� �ƴ� ���� IsValidObj ���� ���� ����.
}

void AB2StageManager::NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob)
{

}

void AB2StageManager::NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed)
{
	for (auto& CurrInfo : DelayedDropSpawnInfoMap)
	{
		if (CurrInfo.Value.DelaySpawnLocRefActor && CurrInfo.Value.DelaySpawnLocRefActor == CharacterAboutToBeDestroyed)
		{
			CurrInfo.Value.DelaySpawnLocRefActor = NULL;
		}
	}
}

void AB2StageManager::NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType)
{
	if (!bCurrentLevelCleared) // ���� ���� �Ŀ� �� �� �����Ƿ� ������Ʈ ���� �ʵ��� ����.
	{
		// �Ʒ� UpdateStagePlayTime �� true �� �־��ָ� ���� ��� ����Ǵ� ���� ��� �ð� ī������ �ߴܵ� ��.
		// ������ ���� ���߿��� �ð� ī���� �ϰ� �Ǿ���.
		UpdateStagePlayTime(false);
	}
}
void AB2StageManager::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{ // �� Notify �� �� ��ü ���� ����� �÷��̵Ǿ����� �������� �ʴ´�. �߰��� �ߴ��ߴ��� �̷��� ���� �� ��.
	if (!bCurrentLevelCleared) // ���� ���� �Ŀ� �� �� �����Ƿ� ����..
	{
		RefreshStagePlayTimeCheck();
	}
}

void AB2StageManager::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{

}

void AB2StageManager::PreloadAdditionalNecessaryInfo()
{
	// TAsset �ε��ϴ� InfoAsset ���� �����. �⺻������ AB2StageGameMode::PreloadAnyNecessaryInfo ���� ó���ϴ� �̿��� �͵�

	GetStageRewardManager().PreloadAllRewardItemInfo(); // ����.. ��Ÿ ������ ����� ���� ���� ������ �� �ε�.
}

void AB2StageManager::CheckStageClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::CheckStageClear"));
//
//	bool bSpawnPoolDone = true;
//
//	// ��� SpawnPool ���� �������� üũ -> �ϴ��� SpawnPool �ϳ� ���������� ��ġ ���ɼ��� ������ ���ܵ�.
//	check(CachedGameMode);
//
//	AB2MonsterSpawnPool* CheckSpawnPool = CachedGameMode->GetActiveSpawnPool();
//	if (CheckSpawnPool)
//	{
//		if (CheckSpawnPool->AreAllDoneForCurrentStage() == false)
//		{
//			bSpawnPoolDone = false;
//		}
//	}
//
//
//	//
//	// ��Ÿ ���ǵ� ����� ���..
//	//
//
//
//	bCurrentLevelCleared = bSpawnPoolDone; // �ٸ� ������ �ʿ信 ���� �߰�.
//
//	if (bCurrentLevelCleared) // ������ ���� �ǵ� Stage Ŭ��� �Ǳ� ���ؼ��� ���� �� �� üũ�� �� ����.
//	{		
//		// �÷��� ��ü�� �������Ƿ� ī��Ʈ �ߴ��ϸ鼭 PlayTime ����. ExtraMap �� �ִٸ� ���ݱ��� ī��Ʈ�� PlayTime �� ���޵� ��. 
//		UpdateStagePlayTime(true); // �̹� ���� ����� ���۵Ǹ鼭 �ߴܵ� ������ �� �ִ�. (�Ƹ� �װ� ��κ��ϵ�?)
//
//#if WITH_EDITOR
//		// �����Ϳ��� ExtraMap �ܵ� �÷����� ���� �ڱ��ڽ��� �ٽ� �ε����� �ʰ� �������� ���� ó���� �ϵ��� �Ѵ�.
//		bool bEDITOR_IsExtraMap = false;
//		if (GIsEditor && StaticCacheStageData.bHasExtraLevel && StaticCacheStageData.CachedCurrentStageInfoData)
//		{
//			UPackage* OuterMostPackage = GetWorld()->GetOutermost();
//			// ���� FIleName �� ExtraMap �� ������ �˻�. FileName �� ��θ� �����ϴµ� ExtraMap �� �׷��� ���� ���̹Ƿ� �� �޺κи� üũ.
//			// ���� ���� ���� ���̹����� ���� �����ϰ� ExtraMap ó���� �� ���� �ְ����� ������ �̽��̰� ������ �ȴٸ� �ݹ� �˰� �� ����
//			if (OuterMostPackage && OuterMostPackage->FileName.ToString().EndsWith(StaticCacheStageData.CachedCurrentStageInfoData->ExtraMap.ToString()))
//			{
//				bEDITOR_IsExtraMap = true;
//			}
//		}
//#endif
//		if (StaticCacheStageData.bPlayInExtraLevel == false && StaticCacheStageData.bHasExtraLevel && StaticCacheStageData.CachedCurrentStageInfoData
//#if WITH_EDITOR
//			&& bEDITOR_IsExtraMap == false
//#endif
//			)
//		{
//			// ExtraMap �� �ε��Կ� ���� ���� �ʿ� ������ �� �����͸� ����.. �ؾ� �ϴµ� �� �������� �ϸ� ExtraMapDelay �� ���� ��� ��ġ�� ���� ���� �� �ִ�.
//			// LoadExtraMap ������ ó����.
//			//SetReadyPlayingExtraMap();
//			
//			// ExtraMap ��ȯ �޽���.. ExtraMapDelay ���� ���� �ʰ� ���;� ��.
//			const float ExtraMapMessageDelay = StaticCacheStageData.CachedCurrentStageInfoData->GetFinalExtraMapMessageDelay(IsScenarioMode());
//			if (ExtraMapMessageDelay > 0.0f){
//				GetWorldTimerManager().SetTimer(ExtraMapMessageDelayTimer, this, &AB2StageManager::ExtraMapMessageDelayTimerCB, ExtraMapMessageDelay, false);
//			}
//			else{
//				ShowExtraMapMessage();
//			}
//
//			const float ExtraMapDelay = StaticCacheStageData.CachedCurrentStageInfoData->GetFinalExtraMapDelay(IsScenarioMode());
//			if (ExtraMapDelay > 0.0f)
//			{
//				GetWorldTimerManager().SetTimer(ExtraMapLoadingDelayTimer, this, &AB2StageManager::ExtraMapLoadingDelayTimerCB, ExtraMapDelay, false);
//			}
//			else
//			{
//				PreHandleLoadExtraMap(); // ���� ������ ���� PreHandleLoadExtraMap �� �ణ �մ���� ����.. ���������� �ʰ�����
//				LoadExtraMap(); // �ٷ� �ε�
//			}
//		}
//		else
//		{
//			// Extra �ʱ��� ����ؼ� ��¥ ���� ��.
//			bStageCleared = true;
//			StageCompleteClass<>::GetInstance().Signal();
//			if (GetB2GameModeType(CachedGameMode) == EB2GameMode::CounterDungeon)
//			{//�ݰݴ��� 
//				RequestCounterDungeonClear();
//			}
//			else if (GetB2GameModeType(CachedGameMode) ==  EB2GameMode::HeroTower)
//			{
//				RequestHeroTowerClear(true);
//			}
//			else if (GetB2GameModeType(CachedGameMode) == EB2GameMode::Dimension)
//			{
//				RequestDimensionClear(EDimensionResult::CLEAR);
//			}
//			else
//			{
//				if (GetB2GameModeType(CachedGameMode) == EB2GameMode::Tutorial)
//					return;
//
//				RequestStageClear(); // ������ �������� Ŭ���� �˸�
//			}			
//		}
//
//		// Stage ���� �� Transition ��� �� �Է� �Ұ����ϰ�
//		ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//		if (B2PC)
//		{
//			// Stage ���� �� Transition ��� �� �Է� �Ұ����ϰ�
//			B2PC->SetStageEndNonCombatState(true);
//		}
//	}
}

void AB2StageManager::RequestStageClear()
{
	StaticCacheStageData.PlayTimeInMainMap = LastUpdatedPlayTime;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// ���۽ÿ� ������ ������ �ʾ����� StageClear üũ�� ���ÿ��� �ٷ�. IsConnected üũ�� ���� �ʴµ� ����� ���¿��� �����Ϳ��� ������ �÷��̸� �ϰ� �� �� �����Ƿ�
	// ��, ��ŷ ���������� �� �̻� �̷� ����� Ŭ���� ó���� �������� �ʴ´�.
	if (DevOnlyIsStandalonePlay())
	{
		OnStageClear();
		return;
	}
#endif

	auto TotalKilledMonster = QuestManager::GetInstance().GetTotalKilledMonsters();
	auto KilledMonsterInfos = QuestManager::GetInstance().GetArrayKilledMonsters();

	auto b2SpotSaleOptionPtr = std::make_shared<b2network::B2ItemSpotSaleOption>();
	auto b2AetherSpotSaleOptionPtr = std::make_shared<b2network::B2AetherSpotSaleOption>();
	if (auto DocSome = UB2UIDocHelper::GetDocSome())
	{
		TArray<bool> AutoSellCategory = DocSome->GetOutAutoSellCatergoryFilter();
		for (int i = 0; i < AutoSellCategory.Num(); i++)
		{
			if (AutoSellCategory[i])
			{
				b2SpotSaleOptionPtr->inventory_types.Add(CliToSvrItemInvenType(static_cast<EItemInvenType>(i)));
			}
		}
		
		b2SpotSaleOptionPtr->item_grade = DocSome->GetOutAutoSellGradeFilter();
		b2AetherSpotSaleOptionPtr->aether_grade = (DocSome->GetOutAutoSellEtherCatergoryFilter() == true) ? DocSome->GetOutAutoSellEtherGradeFilter() : 0;
	}

	auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	BII_CHECK(DocBattleStage);

	data_trader::Retailer::GetInstance().RequestClearStage(
		StaticCacheStageData.ServerStageID,
		StaticCacheStageData.PlayToken,
		CliToSvrPCClassType(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass()),
		CliToSvrPCClassType(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass()),
		FMath::TruncToInt(LastUpdatedPlayTime), // Ŭ���� ����� ���ϴ� ��ġ�ε� ���� �ָ��� ��Ȳ���� �ݿø��ϴ� �ͺ��� ������ ���� �������� �ڸ��� �� ������..
		KilledMonsterInfos,
		TotalKilledMonster,
		DocBattleStage != nullptr ? DocBattleStage->GetAllServival() : false, 
		GetCacheStageKeepEssentialData().StartQTEAttackCount,	
		GetCacheStageKeepEssentialData().FinishQTEAttackCount, 
		b2SpotSaleOptionPtr, 
		b2AetherSpotSaleOptionPtr);
}

void AB2StageManager::ResponseStageClear(const FB2ClearStageInfo& Info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::ResponseStageClear"));
//
//	StaticCacheStageData.TotalExpOfCurrentStage = GET_TUPLE_DATA(FB2ResponseClearStage::reward_exp_index, Info);
//	auto characters = GET_TUPLE_DATA(FB2ResponseClearStage::characters_index, Info);
//
//	BII_CHECK(characters[0] && characters[1]);
//#if !UE_BUILD_SHIPPING
//	UE_LOG(LogBladeII, Log, TEXT("ResponseStageClear Character level %d %d"), characters[0]->level, characters[1]->level);
//#endif
//	
//	UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
//	BII_CHECK(DocBattleStage)
//
//	if (DocBattleStage)
//	{
//		/* Condition */
//		int32 StageClearGrade = GET_TUPLE_DATA(FB2ResponseClearStage::clear_grade_index, Info);
//		const auto StageClearCondition = GET_TUPLE_DATA(FB2ResponseClearStage::clear_mask_index, Info);
//		const auto MaxCondition = static_cast<int32>(EStageClearCondition::MaxCondition);
//		const auto ConstMask = static_cast<int32>(1);
//		TArray<bool> StageClearConditions;
//
//		for (int32 i = 0; i < MaxCondition; ++i)
//		{
//			const auto ClearCondition = (StageClearCondition & (ConstMask << i));
//			StageClearConditions.Add(ClearCondition == 0 ? false : true);
//		}
//		DocBattleStage->SetStageClearConditions(StageClearConditions);
//		DocBattleStage->SetStageClearGrade(StageClearGrade); // �������� ���� UI �� ��Ÿ�� �� ��� ��������.
//		
//		/* Item */
//		int32 MultipleValue = GET_TUPLE_DATA(FB2ResponseClearStage::multiple_value_index, Info);
//		// ���� ������.. �⺻ ������ ����̶�.. ������(������ �̿���)�� ������ ��������� ���·� �߰��Ѵ�.
//		GetB2ItemArray(FB2ResponseClearStage::items_index, Info);
//		// �������� ���� ������
//		const TArray<b2network::B2DropDataPtr> FixedItemInfos = GET_TUPLE_DATA(FB2ResponseClearStage::fixed_items_index, Info);
//		TArray<FB2Item>	BossItemArray;
//		for (auto FixedItemInfo : FixedItemInfos)
//		{
//			FB2Item BossItem;
//			BossItem.ItemRefID = FixedItemInfo->template_id;
//			BossItem.ConsumingAmount = FixedItemInfo->count * MultipleValue;
//			BossItem.ItemClass = EItemClass::EIC_BossPiece;
//			BossItemArray.Add(BossItem);
//		}
//		// ���׸� ���� ������
//		const TArray<b2network::B2AetherServerInfoPtr> EtherInfos = GET_TUPLE_DATA(FB2ResponseClearStage::aethers_index, Info);
//		TArray<FB2Item>	EtherItemArray;
//		if (EtherInfos.Num() > 0)
//		{
//			BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().MakeDirty();
//		}
//		for (const b2network::B2AetherServerInfoPtr& Elem : EtherInfos)
//		{
//			FB2Item ItemEther(Elem);
//			EtherItemArray.Add(ItemEther);
//		}
//		// ��� ���� ������
//		const TArray<b2network::B2DropDataPtr> MaterialItemInfos = GET_TUPLE_DATA(FB2ResponseClearStage::material_items_index, Info);
//		TArray<FB2Item>	MaterialItemArray;
//		for (const b2network::B2DropDataPtr& Elem : MaterialItemInfos)
//		{
//			FB2Item MaterialItem;
//			MaterialItem.ItemRefID = Elem->template_id;
//			MaterialItem.ConsumingAmount = Elem->count * MultipleValue;
//			MaterialItem.InventoryType = EItemInvenType::EIIVT_Consumables;
//			MaterialItem.ItemClass = EItemClass::EIC_End;
//			MaterialItemArray.Add(MaterialItem);
//		}
//
//		/* Auto Sell */
//		// �Ǹ� ��� ������
//		const TArray<b2network::B2SoldItemInfoPtr> SoldItemInfos = GET_TUPLE_DATA(FB2ResponseClearStage::sold_items_index, Info);
//		TArray<FB2Item>	SoldItemArray;
//		for (const b2network::B2SoldItemInfoPtr& Elem : SoldItemInfos)
//		{
//			FB2Item SoldItem;
//			SoldItem.ItemRefID = Elem->template_id;
//
//			FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
//			const FMD_ItemSingleElem* ItemMDElem = CliDataStore.GetItemMasterDetailInfo(SoldItem.ItemRefID);
//			
//			SoldItem.StarGrade = ItemMDElem ? ItemMDElem->ItemGrade : 1;
//			SoldItem.Quality = Elem->quality;
//			SoldItem.ConsumingAmount = 1;
//			SoldItem.InventoryType = EItemInvenType::EIIVT_End;
//			SoldItem.ItemClass = EItemClass::EIC_Weapon;
//			SoldItemArray.Add(SoldItem);
//		}
//		// �Ǹ� ���׸� ������
//		const TArray<b2network::B2SoldAetherInfoPtr> SoldEtherInfos = GET_TUPLE_DATA(FB2ResponseClearStage::sold_aethers_index, Info);
//		for (const b2network::B2SoldAetherInfoPtr& Elem : SoldEtherInfos)
//		{
//			// StarGrade = Ether->grade;
//			// Quality = Ether->type;
//			// Level = Ether->tier;
//			// SurpassCount = Ether->slot;
//						
//			FB2Item SoldItem;
//			SoldItem.StarGrade = Elem->aether_grade;
//
//			const TArray<FB2AetherItemPreviewInfo>& AetherItems = DocBattleStage->GetAetherItemList();
//			SoldItem.Quality = AetherItems.IsValidIndex(0) ? AetherItems[0].AetherType : FMath::FRandRange(1, 4);
//
//			SoldItem.Level = Elem->tier;
//			SoldItem.SurpassCount = FMath::FRandRange(1, 6);
//			SoldItem.ConsumingAmount = 1;
//			SoldItem.InventoryType = EItemInvenType::EIIVT_End;
//			SoldItem.ItemClass = EItemClass::EIC_Ether;
//			SoldItemArray.Add(SoldItem);
//		}
//
//		/* Gold */
//		// ����� �������� Drop Gold
//		int32 StageDropGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_drop_gold_index, Info);
//		// �������� Ŭ���� �� �������� �ټ� �ִ� Reward Gold
//		TArray<int32> StageRewardGolds = GET_TUPLE_DATA(FB2ResponseClearStage::reward_golds_index, Info);
//		// Reward Gold ���� (x3 ��� ����)
//		int32 StageRewardTotalGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_reward_gold_index, Info);
//		// QTE Gold 
//		int32 QTEGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_qte_gold_index, Info);		
//		// ���� Total Gold (��� �Ǹ� Gold ������)
//		int32 TotalGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_gold_index, Info);
//		// ���/���׸� �Ǹ� Gold
//		int32 SoldGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_sales_gold_index, Info);
//
//		int32 GoldBuffRate = GET_TUPLE_DATA(FB2ResponseClearStage::gold_buff_rate_index, Info);
//		int32 ExpBuffRate = GET_TUPLE_DATA(FB2ResponseClearStage::exp_buff_rate_index, Info);
//		
//		int32 GoldItemEquipRate = GET_TUPLE_DATA(FB2ResponseClearStage::equip_item_gold_rate_index, Info);
//		int32 GoldItemSetRate = GET_TUPLE_DATA(FB2ResponseClearStage::item_set_gold_rate_index, Info);
//
//		int32 TotalGoldRate = GoldBuffRate + GoldItemEquipRate + GoldItemSetRate;
//
//		if (StageRewardGolds.Num() > 0)
//		{
//			for (int32 StageRewardGold : StageRewardGolds)
//			{
//				FB2Item RewardGoldItem;
//				float GoldSum = StageRewardGold * MultipleValue * (1.0f + (TotalGoldRate / 100.0f));
//				GenerateGoldItem(RewardGoldItem, (int32)GoldSum);
//				B2ItemArray.Add(RewardGoldItem);
//			}
//		}
//		DocBattleStage->SetStageDropGold(StageDropGold);
//		DocBattleStage->SetStageRewardGold(StageRewardTotalGold);
//		DocBattleStage->SetStageQTEGold(QTEGold);
//		DocBattleStage->SetStageTotalGold(TotalGold);
//		DocBattleStage->SetStageSoldGold(SoldGold);
//		
//		// ���� ������ ����Ʈ
//		B2ItemArray.Sort([](const FB2Item& lhs, const FB2Item& rhs)->bool
//		{
//			if (lhs.StarGrade == rhs.StarGrade)
//				return lhs.ItemClass < rhs.ItemClass;
//			else
//				return lhs.StarGrade > rhs.StarGrade;
//		});
//
//		DocBattleStage->SetStageClearMultipleValue(MultipleValue);
//		DocBattleStage->SetStageClearRewardItemList(B2ItemArray);
//		DocBattleStage->SetStageClearEtherItemsList(EtherItemArray);
//		DocBattleStage->SetStageClearMaterialItemsList(MaterialItemArray);
//		DocBattleStage->SetStageClearSoldItemsList(SoldItemArray);
//		DocBattleStage->SetStageResultBossDropItemsList(BossItemArray);
//
//		DocBattleStage->SetEndGoldBuffRate(GoldBuffRate);
//		DocBattleStage->SetEndExpBuffRate(ExpBuffRate);
//	}
//
//	//������ ����. ���� �����۵� ���⼭ ��.
//	BladeIIGameImpl::GetStageDataStore().ClearStage(Info, StaticCacheStageData.ServerStageID);
//
//	ABladeIIPlayerController* PlayerController = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	FMyAssetStore* MyAssetStore = PlayerController? &PlayerController->GetMyAssetStore() : NULL;
//	if (MyAssetStore)
//	{ // Ŭ�� ���� �������� ����� ���� ���� �������� �޾Ұ� ��� ȭ�鿡 ����� �ѷ��ַ��� ���� ��ũ�� �ʿ���.
//		// �ٵ� ��� �� �ȵǰ� �ϸ� FB2MessageInfoResponseClearStage �� total_added_drop_gold �� total_added_reward_gold ������ �װɷδٰ�..
//		MyAssetStore->SyncCurrentTempGoldToAccountInfo(true); // Freeze ��Ų��.
//	}
//	OnStageClear();
}

void AB2StageManager::OnStageClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::OnStageClear"));

	//if (CachedGameMode)
	//{ // �⺻���� Auto ���¸� ���� �� �ƴ϶� �Ͻ������θ� ����. �⺻ auto ���´� ������ ������ �ʿ䰡 �����Ƿ�.
	//	CachedGameMode->TemporaryStopLocalPlayerAuto();
	//}

	//UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//if (DocBattleStage)
	//{
	//	DocBattleStage->SetStagePlayTime(StaticCacheStageData.GetStageClearTime());
	//	if (!Utilities::FConnectionState::GetInstance().IsConnected())
	//	{
	//		DocBattleStage->SetStageClearGrade(STAGE_CLEAR_GRADE_STAR_3); // ���� �ȵ� ���¶�� �� �� ���̰� ���� ä���ش�.
	//	}
	//}

	//// �������� Ŭ���� �� ���� ���� ���� ���� noti.
	//// StageEventDirector ������ notify �� �ϱ� ���� �������� ���� �ؾ� EventWidget �ʱ�ȭ �� ������ �����͵��� ä���� ����.
	//auto* BladeIIPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(BladeIIPlayer ? BladeIIPlayer->GetController() : UGameplayStatics::GetLocalPlayerController(this));
	//BII_CHECK(BladeIIPlayer && B2PC);

	//// �������� Ŭ���� ����� ���� �޸� ��� ���� ������ �͵��� ������. 
	//// GC �� ���� ������ ���ŷ�� �� �ɸ� �� �ִ�. ������ ��Ÿ �� ���ŷ�� �� �Ž����� ������ ���.
	//ManualUnloadOnStageClear(this); 
	//
	//// �� Ÿ�̹� ���� �ؼ� StageResult UI �� ��׶��� �ε��� ����.. ���߿� ��½ Ƣ�� ��.
	//UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::StageResult);

	//OnClearStageClass<>::GetInstance().Signal();

	//BII_CHECK(CachedGameMode);
	//CachedGameMode->StartEventScene_GameClear();

	//GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);
}

void AB2StageManager::RequestFailStage(EStageFailReason reason)
{
	if (bStageCleared)
		return;

	StaticCacheStageData.PlayTimeInMainMap = LastUpdatedPlayTime;

	data_trader::Retailer::GetInstance().RequestFailStage(
		StaticCacheStageData.ServerStageID,
		StaticCacheStageData.PlayToken,
		CliToSvrStageFailReason(reason),
		FMath::TruncToInt(LastUpdatedPlayTime));
}

void AB2StageManager::RequestCounterDungeonClear()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// ���۽ÿ� ������ ������ �ʾ����� StageClear üũ�� ���ÿ��� �ٷ�. IsConnected üũ�� ���� �ʴµ� ����� ���¿��� �����Ϳ��� ������ �÷��̸� �ϰ� �� �� �����Ƿ�
	// ��, ��ŷ ���������� �� �̻� �̷� ����� Ŭ���� ó���� �������� �ʴ´�.
	if (DevOnlyIsStandalonePlay())
	{
		OnCounterDungeonClear();
		return;
	}
#endif

	data_trader::Retailer::GetInstance().RequestFinishCounterDungeon(
		StaticCacheStageData.ServerStageID,
		StaticCacheStageData.PlayToken,
		true);
}

void AB2StageManager::ResponseFinishCounterDungeon(const FB2FinishCounterDungeonInfo& Info)
{
	if (!IsStageCleared())
		return;

	//auto RewardList = GET_TUPLE_DATA(FB2ResponseFinishCounterDungeon::received_rewards_index, Info);
	//int32 LeftTicket = GET_TUPLE_DATA(FB2ResponseFinishCounterDungeon::left_ticket_index, Info);
	//int32 Cost = GET_TUPLE_DATA(FB2ResponseFinishCounterDungeon::next_buy_ticket_gem_cost_index, Info);

	//auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	//if (CounterDoc)
	//{
	//	CounterDoc->SetRewardItems(RewardList);
	//	CounterDoc->SetLeftTicket(LeftTicket);
	//	CounterDoc->SetNextBuyTicketGemCost(Cost);

	//	for (auto reward : RewardList)
	//	{
	//		for (auto expInfo : reward->light_characters)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().EndGameIncreaseExp(expInfo);
	//		}
	//	}
	//}

	//OnCounterDungeonClear();
}

void AB2StageManager::OnCounterDungeonClear()
{
	/*OnClearStageClass<>::GetInstance().Signal();

	BII_CHECK(CachedGameMode);
	CachedGameMode->StartEventScene_GameClear();

	GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);*/
}

void AB2StageManager::RequestHeroTowerClear(bool bClear)
{
	/*FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();

	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

	int32 nPlayTimeMilliSecond = 0;
	
	AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(this));
	
	if (pGM)
		nPlayTimeMilliSecond = pGM->GetElapsedTime().GetTotalMilliseconds();

	if (pDoc)
	{
		pDoc->m_bIsClearHeroTower = bClear;
		pDoc->m_nCurrentFloorRecordTimeMilliSecForResult = nPlayTimeMilliSecond;
	}

	data_trader::Retailer::GetInstance().RequestFinishHeroTower(StageData.GetHeroTowerFloor(), StageData.PlayToken, nPlayTimeMilliSecond, bClear);

	GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);*/
}

void AB2StageManager::RequestAbandonHeroTower()
{
	//if (GetB2GameModeType(CachedGameMode) == EB2GameMode::HeroTower)
	//{
	//	FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	//	data_trader::Retailer::GetInstance().RequestAbandonHeroTower(StageData.GetHeroTowerFloor(), StageData.PlayToken);
	//}
}

void AB2StageManager::ResponseHeroTowerClear(const FB2ResponseFinishHeroTowerPtr& MsgPtr)
{
	//UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();
	//UB2UIDocUser* UIDocUser = UB2UIDocHelper::GetDocUser();
	//if (pDoc)
	//{
	//	pDoc->SetFinishHeroTowerPtr(MsgPtr);

	//	if (pDoc->m_bIsClearHeroTower)
	//	{
	//		// colosseum comment : reward list �� �����Ͽ� ������ Ÿ�Կ� ���� ������ ó���� �� �ֵ��� �۾��� �ʿ��մϴ�.

	//		// Ŭ����� ���� UI�� �����ϸ鼭 �����
	//		/*BII_CHECK(CachedGameMode);
	//		CachedGameMode->StartEventScene_GameClear();*/

	//		//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerResult);

	//		for (auto Reward : MsgPtr->rewards)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().ApplyRewardInfo(Reward);
	//		}
	//	}
	//	else
	//	{
	//		// �й�� ���� ������ ž ��� UI�� �ƴ� DefeatUI �� �����.
	//		StartDefeatMenuClass<EB2GameMode>::GetInstance().Signal(EB2GameMode::HeroTower);
	//	}
	//}	
}

void AB2StageManager::RequestDimensionClear(EDimensionResult bClear)
{
	/*FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();

	auto* pDoc = UB2UIDocHelper::GetDocDimension();

	int32 nPlayTimeMilliSecond = 0;

	AB2DimensionGameMode* pGM = Cast<AB2DimensionGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		nPlayTimeMilliSecond = pGM->GetElapsedTime().GetTotalMilliseconds();

	if (pDoc)
	{
		pDoc->m_bIsClear = bClear;
		pDoc->m_nCurrentRecordTimeMilliSecForResult = nPlayTimeMilliSecond;
		data_trader::Retailer::GetInstance().RequestFinishDimension(pDoc->GetDifficulty(), StageData.PlayToken, static_cast<int32>(bClear), FMath::Max(nPlayTimeMilliSecond,1));
	}

	GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);*/
}

void AB2StageManager::ResponseDimensionClear(const FB2ResponseFinishDimensionPtr& MsgPtr)
{
	//auto* pDoc = UB2UIDocHelper::GetDocDimension();
	//UB2UIDocUser* UIDocUser = UB2UIDocHelper::GetDocUser();
	//if (pDoc)
	//{
	//	pDoc->SetFinishDimensionPtr(MsgPtr);

	//	if (pDoc->m_bIsClear == EDimensionResult::CLEAR)
	//	{
	//		// ����ó��		
	//		for (auto Reward : MsgPtr->totems)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().AddTotem(Reward); // ����
	//		}

	//		for (auto Reward : MsgPtr->update_items)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE, Reward); // ���� ���� 
	//		}

	//		// ��ó��
	//		OnClearStageClass<>::GetInstance().Signal();
	//		BII_CHECK(CachedGameMode);
	//		CachedGameMode->StartEventScene_GameClear();
	//	}
	//	else if(pDoc->m_bIsClear == EDimensionResult::GIVEUP)
	//	{
	//		CachedGameMode->GoToMap();
	//	}
	//	else
	//	{
	//		StartDefeatMenuClass<EB2GameMode>::GetInstance().Signal(EB2GameMode::Dimension);
	//	}
	//}
}

void AB2StageManager::ExtraMapMessageDelayTimerCB()
{
	GetWorldTimerManager().ClearTimer(ExtraMapMessageDelayTimer);
	ShowExtraMapMessage();
}

void AB2StageManager::ShowExtraMapMessage()
{
	// ���⼱ DisplayTime �� �� ���� ���� ���尡 ���� �ε� �� �Ŀ� ���� ���� ��. �̰� �� �����ε�, ���⼭ ����� �˾��� ���� �������� �����ڵ忡 ĳ���� ���⵵ ���� �ʰ� �ؼ�. �� ���� ������ �� �˾����� ��� �����ϴ� ���� �Ǿ�� �� ��.
	//FText TransitionMessage = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("InGameHUD_TrasitToExtra")));
	//ABladeIIGameMode::SetHUDCenterMessage(TransitionMessage, 0.0f, 1); // AnimIndex 1 ��. �̰� �� �ϵ��ڵ���. ��ȣ Ʋ���ٰ� ū ���簡 �Ͼ �͵� �ƴϰ�..
}

void AB2StageManager::ExtraMapLoadingDelayTimerCB()
{
	GetWorldTimerManager().ClearTimer(ExtraMapLoadingDelayTimer);
	PreHandleLoadExtraMap(); // ���� ������ ���� PreHandleLoadExtraMap �� �ణ �մ���� ����.. ���������� �ʰ�����
	LoadExtraMap();
}

void AB2StageManager::PreHandleLoadExtraMap()
{
	// ���� �ٸ� �� �־��µ� ������ ���� SaveDataForExtraMap �� �ֱ�..

	SetReadyPlayingExtraMap(); // ExtraMap ���� �Ѱ��� ������ ����.
}

void AB2StageManager::LoadExtraMap()
{
	//FSingleStageInfoData* ThisStageInfo = GetCurrentStageInfoData();
	//if (ThisStageInfo)
	//{
	//	UGameplayStatics::OpenLevel(this, ThisStageInfo->ExtraMap, true);
	//}
}

void AB2StageManager::SetReadyPlayingExtraMap()
{
	//StaticCacheStageData.bPlayInExtraLevel = true;
	//StaticCacheStageData.PlayTimeInMainMap = GetStagePlayTime(false);
	//auto* PlayerCharacter	= Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//BII_CHECK(PlayerCharacter && PlayerCharacter->IsValidObj());
	//auto* PlayerController	= Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//BII_CHECK(PlayerController && PlayerController->IsValidObj());

	//if (PlayerCharacter && PlayerController && CachedGameMode)
	//{
	//	StaticCacheStageData.PreExtraLastPlayedCharClass = PlayerCharacter->GetCurrentPlayerClass(); // ���� Ŭ������ ����. �ٸ� �ϳ��� �� ���� ���� DataStore �� �ִ� Main Ȥ�� Sub ��.
	//	auto* MyAssetStore = &PlayerController->GetMyAssetStore();
	//	if (MyAssetStore)
	//	{
	//		StaticCacheStageData.MainMapStartingGold = MyAssetStore->GetInitialGoldAmount();
	//		StaticCacheStageData.MainMapLastGold = MyAssetStore->GetCurrentTempGoldAmount();
	//	}

	//	UB2SkillRTManager* SkillRTManager = PlayerCharacter->GetSkillRTManager();
	//	if (SkillRTManager)
	//	{
	//		StaticCacheStageData.SetReadyPreExtraCombatVariables(PlayerCharacter, SkillRTManager); // ������ ���� ���� ���� �������� ���� ��Ƽ�..
	//	}
	//}	
}

void AB2StageManager::RetrieveDataForExtraMap()
{
	//// SetReadyPlayingExtraMap ���� PreExtraLastPlayedCharClass �� �����ϴµ� �׷��ٰ� ���⼭ GetPreExtraLastPlayedClass �� ���� �ʴ´�. �װ� ���Ӹ�忡�� Player Character spawn �� �̸� �� ���Ҿ��.
	//// Health �鵵 ��������. �װ� BladeIIPlayer �� SkillRTManager �ʿ���..

	//ABladeIIPlayer* PlayerCharacter = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//BII_CHECK(PlayerCharacter && PlayerCharacter->IsValidObj());

	//if (PlayerCharacter)
	//{
	//	auto* PlayerController = Cast<ABladeIIPlayerController>(PlayerCharacter->GetController());
	//	auto* MyAssetStore = PlayerController ? &PlayerController->GetMyAssetStore() : NULL;
	//	if (MyAssetStore)
	//	{
	//		MyAssetStore->SetStartingGold(StaticCacheStageData.MainMapStartingGold);
	//		MyAssetStore->AddGold(StaticCacheStageData.MainMapLastGold - StaticCacheStageData.MainMapStartingGold); // ���� ���� MainMapLastGold ��.
	//	}
	//}

	//LastUpdatedPlayTime = StaticCacheStageData.PlayTimeInMainMap;
}

void AB2StageManager::BeginStageClearScene_Phase1()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageClear); // StageClear UI ���� ������ ��ȣ�� ���� BeginStageClearScene_Phase2 �� ���۵� ��.

	//// StageClear Phase1 �� ī�޶� ����.
	//UWorld* TheWorld = GetWorld();
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (TheWorld && B2PC)
	//{
	//	if (!StageClearViewCam){
	//		// ���극�� BP �� ���� ������ ���� �ʾҴٸ� ���������� ���� �����Ѵ�.
	//		// ������ sky box � ������ ���� � ����� �ٶ� �� �ְ�. �׷��ٰ� �ʹ� �ָ� �װʹ�� �ε��Ҽ� ���е� ������ ���� �� �ִ�.
	//		// ����ϴ� ����Ʈ�� ������ǥ���� align �Ǵ� �� ���Ƽ� ���� �Ʒ��� ������ ���ϰ� ������ �����ϸ� fog ������ �ָ����� ������� ������ �� �Ʒ��� fog �� ��
	//		FTransform CamSpawnTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(200000.0f, 0.0f, 0.0f));
	//		FActorSpawnParameters CamSpawnParam;
	//		StageClearViewCam = Cast<ACameraActor>(TheWorld->SpawnActor(ACameraActor::StaticClass(), &CamSpawnTransform, CamSpawnParam));
	//	}
	//	UCameraComponent* StageClearViewCamComp = StageClearViewCam ? StageClearViewCam->GetCameraComponent() : NULL;
	//	if (StageClearViewCam && StageClearViewCamComp)
	//	{
	//		// ���̵� ȭ�� ����.. �̰� �׳� ���� ���� ȭ�鿡�� ���� ȿ������ ©���� �Ǿ adaptive FOV �� �ʿ���.
	//		StageClearViewCamComp->SetConstraintAspectRatio(false);
	//		StageClearViewCamComp->SetAspectRatioAdaptiveFOV(true);

	//		EnsureTargetToStageClearCam();

	//		StageClearPhase1_StartVictoryBGFx(); // ī�޶� �����Ǹ� VictoryBGFx �� �ٷ� ����. ������ Wing �̶� Gem �� UIStageClear �ʿ��� ��ȣ�� ���� ��.
	//	}
	//	
	//	// StageClearPhase1 �� StageEventDirector ���� clnematic mode ������ ���� �����Ƿ� ���⼭ ������ �־���. ���� �����̴� CinematicMode ���� ������ �� ������ �ʿ�� ������..?
	//	B2PC->SetCinematicMode(true, false, true, false, true);
	//}

	//TurnOffAllFogs(); // ���� ������ ���ϴ� ��ο� ����� �ȳ����� ������.
}

void AB2StageManager::StageClearPhase1_StartVictoryWingFx()
{// StageClear UI ������ �̺�Ʈ�� ���� ���� �� ��.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearWingFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearWingFxTemplate() : NULL),
	//	StageClearWingFxComp, TEXT("StageClearWingFxRelativePos"), TEXT("StageClearWingFxRelativeRot"), TEXT("StageClearWingFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryGemFx()
{ // StageClear UI ������ �̺�Ʈ�� ���� ���� �� ��.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearGemFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearGemFxTemplate() : NULL),
	//	StageClearGemFxComp, TEXT("StageClearGemFxRelativePos"), TEXT("StageClearGemFxRelativeRot"), TEXT("StageClearGemFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryBGFx()
{// StageClear UI ������ �̺�Ʈ�� ���� ���� �� ��.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearBGFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearBGFxTemplate() : NULL),
	//	StageClearBGFxComp, TEXT("StageClearBGFxRelativePos"), TEXT("StageClearBGFxRelativeRot"), TEXT("StageClearBGFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryFxCommon(class UParticleSystem* InPSTemplate, class UParticleSystemComponent*& InOutCompForSet, const TCHAR* PosConfigKey, const TCHAR* RotConfigKey, const TCHAR* ScaleConfigKey)
{
	//UCameraComponent* StageClearViewCamComp = StageClearViewCam ? StageClearViewCam->GetCameraComponent() : NULL;
	//if (StageClearViewCamComp)
	//{
	//	// ���̵� ȭ�� ����.. �̰� �׳� ���� ���� ȭ�鿡�� ���� ȿ������ ©���� �Ǿ adaptive FOV �� �ʿ���.
	//	StageClearViewCamComp->SetConstraintAspectRatio(false);
	//	StageClearViewCamComp->SetAspectRatioAdaptiveFOV(true);
	//	
	//	EnsureTargetToStageClearCam(); // �β����� ��

	//	if (InOutCompForSet) // ī�޶�ó�� �ʿ� ��ġ�� ���� ���õǾ� �ִٸ� �װ� ����ϰ�
	//	{
	//		InOutCompForSet->Activate();
	//	}
	//	else // �ƴϸ� ���ο��� ����
	//	{
	//		if (InPSTemplate)
	//		{
	//			// ī�޶�κ����� ��� ��ġ �� ������ ���� �׽�Ʈ�� ���� ���ؼ� ini �� ���.
	//			FVector FxRelativePos(500.0f, 0.0f, 0.0f);
	//			GConfig->GetVector(TEXT("/Script/BladeII.B2StageManager"), PosConfigKey, FxRelativePos, GGameIni);
	//			FRotator FXRelativeRot(0.0f, 0.0f, 0.0f);
	//			GConfig->GetRotator(TEXT("/Script/BladeII.B2StageManager"), RotConfigKey, FXRelativeRot, GGameIni);

	//			InOutCompForSet = UGameplayStatics::SpawnEmitterAttached(
	//				InPSTemplate, StageClearViewCamComp, NAME_None, FxRelativePos, FXRelativeRot, EAttachLocation::KeepRelativeOffset);
	//			
	//			BII_CHECK(InOutCompForSet);
	//			if (InOutCompForSet)
	//			{
	//				float FxScale = 1.0f;
	//				GConfig->GetFloat(TEXT("/Script/BladeII.B2StageManager"), ScaleConfigKey, FxScale, GGameIni);
	//				InOutCompForSet->SetRelativeScale3D(FVector(FxScale, FxScale, FxScale));
	//			}
	//		}
	//	}
	//}
}

void AB2StageManager::BeginStageClearScene_Phase2()
{
	TurnBackOnAllFogs(); // Phase 1 ���� �� ���׵� �ٽ� �ѱ�.

	//check(CachedGameMode);
	//const TArray<class AB2StageEventDirector*>& EventDirectors = CachedGameMode->GetEventDirectorArray();
	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	// �̰� StageClear UI ���� ���� ��ȣ�� �޴´�. ���� ���â�� �Բ��ϴ� �¸� ���� ����
	//	// ESEV_Clear ��� �ű⼭ �ɰ��� �ٸ� enum ���� �˻��ϰ� �� ���� ����.
	//	if (CurrSED->GetDirectingEvent() == EStageEvent::ESEV_Clear){
	//		CurrSED->CustomBeginShow();
	//	}
	//}

	//EnsureTargetToStageClearCam(); // �β����� ��
	//
	//// �¸� BGM �� �ᱹ Phase2 �� �Բ�..
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//BII_CHECK(B2GM);
	//if (B2GM)
	//{
	//	B2GM->PlayStageVictoryBGM(IsGoldBonusStage(), IsBossStage());
	//}
}

void AB2StageManager::EnsureTargetToStageClearCam()
{
	// �⺻�����δ� StageClearScene Phase1 ���� �� StageClearViewCam ���� ViewTarget �ѹ��� �����ϸ� �Ǵ� �ǵ� �̷����� ī�޶�ȿ���� �ִ� ��ų�� �Բ� ��Ÿ�� ���� �� �װ� �� ������ ���� �ִ�.
	//// �β����Ե� �װ� ����� ��ġ�� ���ؼ� �� �ڵ带 ���� ��� ������ ȣ����. �׷��� �ؼ� ũ�� ���� �� �͵� �ƴϰ� �ؼ� 
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2PC && StageClearViewCam) {
	//	B2PC->SetViewTargetWithBlend(StageClearViewCam, 0.0f);
	//}
}

void AB2StageManager::TurnOffAllFogs()
{
	//for (FActorIterator FogIt(GetWorld()); FogIt; ++FogIt)
	//{
	//	AExponentialHeightFog* ExpFog = Cast<AExponentialHeightFog>(*FogIt);
	//	AAtmosphericFog* AtmosFog = Cast<AAtmosphericFog>(*FogIt);

	//	// �׳� ���� �ʰ� visibility �� ����� ���ٰ� ��. ���߿� �ٽ� �Ѿ� �ϴ�
	//	if (ExpFog && ExpFog->GetComponent()){
	//		const bool bWasVisible = (ExpFog->GetComponent()->IsVisible() && !ExpFog->bHidden);
	//		CachedExpFogVisMap.Add(ExpFog, bWasVisible);
	//		ExpFog->SetActorHiddenInGame(true);
	//	}
	//	else if (AtmosFog && AtmosFog->GetAtmosphericFogComponent()){
	//		const bool bWasVisible = (AtmosFog->GetAtmosphericFogComponent()->IsVisible() && !AtmosFog->bHidden);
	//		CachedAtmosFogVisMap.Add(AtmosFog, bWasVisible);
	//		AtmosFog->SetActorHiddenInGame(true);
	//	}
	//}
}

void AB2StageManager::TurnBackOnAllFogs()
{
	for (auto& ExpFogElem : CachedExpFogVisMap)
	{
		if (ExpFogElem.Key && ExpFogElem.Value){ // ������ visible �ߴ� ��츸 �ٽ� �Ҵ�.
			ExpFogElem.Key->SetActorHiddenInGame(false);
		}
	}
	for (auto& AtmosFogElem : CachedAtmosFogVisMap)
	{
		if (AtmosFogElem.Key && AtmosFogElem.Value){ // ������ visible �ߴ� ��츸 �ٽ� �Ҵ�.
			AtmosFogElem.Key->SetActorHiddenInGame(false);
		}
	}
}

void AB2StageManager::PlayStageBGM()
{
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//BII_CHECK(B2GM);

	//B2GM->PlayBGMByStageContext(StaticCacheStageData.ClientStageId, StaticCacheStageData.bPlayInExtraLevel,
	//	(CurrentBossMobCount > 0) ? EStageCombatSoundContext::ESCSC_BossCombat : EStageCombatSoundContext::ESCSC_NormalCombat);

	//CustomaizeBGM = false;
}

void AB2StageManager::PlayBossStageBGM()
{
	//[@SameLine, 180524] ������ Ÿ�� ������ ������ ���� bgm ��� ����
	//x-10(BossStage)�� �ƴ� ��� ùbgm->����bgm ������ ����X
	//x-10(BossStage)�� ��� ùbgm->����bgm->����bgm ���� ����

	if (IsBossStage() || IsPossiblePlayBossBgm)
	{
		PlayStageBGM();
	}

	IsPossiblePlayBossBgm = false;
}

void AB2StageManager::PlayCustomBGM(const FName& InName)
{
	CustomaizeBGM = true;
	CustomaizeBGMName = InName;
}


void AB2StageManager::PlayBGM()
{
	//if (CustomaizeBGM)
	//{
	//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//	BII_CHECK(B2GM);

	//	B2GM->PlayBGMCustomize(CustomaizeBGMName);
	//}
	//else
	//	PlayStageBGM();
}

void AB2StageManager::SetStageClearViewObjects(ACameraActor* InCamActor, UParticleSystemComponent* InPSWingComp, UParticleSystemComponent* InPSGemComp, UParticleSystemComponent* InPSBGComp)
{ // ���� ���� ���õȴٸ� ������ ��ġ�� �ֵ��� ��. StageClearPhase1_StartVictoryGemFx ��� ���� ó���� ���� �ʰ� �ȴ�.
	StageClearViewCam = InCamActor;
	StageClearWingFxComp = InPSWingComp;
	StageClearGemFxComp = InPSGemComp;
	StageClearBGFxComp = InPSBGComp;
}

void AB2StageManager::RefreshStagePlayTimeCheck()
{ // UpdateStagePlayTime �̿ܿ� ���� ī��Ʈ ���� ���� ������ �ʿ��� ��� ���. ������ Start �� Resume ��.
	UWorld* TheWorld = GetWorld();
	BII_CHECK(TheWorld);

	if (TheWorld)
	{
		PlayTimeLastCheckedCache = TheWorld->GetTimeSeconds();

		bCountingPlayTime = true; // Refresh �� �� �̻� ī��Ʈ ����.
	}
}
void AB2StageManager::UpdateStagePlayTime(bool bStopCounting)
{
	//UWorld* TheWorld = GetWorld();
	//BII_CHECK(TheWorld);

	//if (TheWorld && bCountingPlayTime)
	//{ // ������ PlayTimeLastCheckedCache �������κ��� ���� �ð� ����.

	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//	bool bCinematicMode = B2PC && B2PC->IsInCinematicMode();

	//	if(!bCinematicMode)
	//		LastUpdatedPlayTime += TheWorld->GetTimeSeconds() - PlayTimeLastCheckedCache;

	//	PlayTimeLastCheckedCache = TheWorld->GetTimeSeconds();

	//	if (bStopCounting) // �� ��� Stop �̳� Pause ������ ���.
	//	{
	//		bCountingPlayTime = false;
	//	}
	//	
	//	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	//	if (DocBS) // UI Doc ������Ʈ.
	//	{
	//		DocBS->SetStagePlayTime((int32)LastUpdatedPlayTime);
	//	}
	//}
}

float AB2StageManager::GetStagePlayTime(bool bUpdateNow)
{
	if (bUpdateNow)
	{
		UpdateStagePlayTime(false); // �߰� ���� ������ ������Ʈ �ϴ� ���. �ߴܵ� ���¿��� ������ ������Ʈ �� ��.
	}

	return LastUpdatedPlayTime;
}

void AB2StageManager::AutoUpdateStagePlayTimeCB()
{
	GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);

	UpdateStagePlayTime(false); // ������Ʈ �Ǹ鼭 UIDoc ������Ʈ �ϸ� �˾Ƽ� ǥ�� ���� �� �ߴ� ���߿��� �˾Ƽ� ��ī��Ʈ

	if (!bCurrentLevelCleared)
	{
		GetWorldTimerManager().SetTimer(AutoUpdateStagePlayTimeTH, this, &AB2StageManager::AutoUpdateStagePlayTimeCB, StagePlayTimeAutoUpdateInterval, false);
	}
}

//FSingleStageInfoData* AB2StageManager::GetCurrentStageInfoData()
//{
//	return StaticCacheStageData.CachedCurrentStageInfoData;
//}

float AB2StageManager::GetStageDifficultyScale()
{
//#if !UE_BUILD_SHIPPING
//	extern float GCheat_StageDifficultyOverride;
//	if (GCheat_StageDifficultyOverride > 0.0f){
//		return GCheat_StageDifficultyOverride;
//	}
//#endif
//
//	// ���� ���õ� DifficultyLevel �� Info ���� (������ ���� ������ ������) ���� ��ԵǴ� ���� ����Ǵ� scale �� ����
//	return BladeIIGameImpl::GetStageDataStore().GetStageDifficultyScale(
//		StaticCacheStageData.ClientStageId, 
//		StaticCacheStageData.StageDifficultyLevel,
//		// B2GameModeType �Ѱ��ִ� �� M15 ������ �ӽ�.
//		CachedGameMode ? CachedGameMode->GetB2GameModeType() : EB2GameMode::Scenario);
	return 0.0;
}

void AB2StageManager::StopOrRestoreAllWorldMobs(bool bStop)
{
	//bStopAllMobs = bStop;

	//TArray<ABladeIICharacter*> WorldMobList;
	//
	//check(CachedGameMode);
	//CachedGameMode->GetAllWorldMobList(WorldMobList);
	//
	//for (ABladeIICharacter* CurrMob : WorldMobList)
	//{
	//	if (bStopAllMobs)
	//	{
	//		CurrMob->PreventMovementByStageEvent(); // PreventMovementByStageEvent �� �ؾ� AnimNotify �� ���� AllowMovement �� �� �� ���ư� ����
	//	}
	//	else
	//	{
	//		CurrMob->AllowMovementByStageEvent();
	//	}
	//}
}

void AB2StageManager::ForceCancelAttackAllWorldMobs()
{
	//TArray<ABladeIICharacter*> WorldMobList;

	//check(CachedGameMode);
	//CachedGameMode->GetAllWorldMobList(WorldMobList);

	//for (ABladeIICharacter* CurrMob : WorldMobList)
	//{
	//	CurrMob->ForceCancelAttackMotion();
	//}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	FCacheStageEssentialDataToKeep
//

FCacheStageEssentialDataToKeep AB2StageManager::StaticCacheStageData;

FCacheStageEssentialDataToKeep::FCacheStageEssentialDataToKeep()
{
	Initialize();

	bStageRequestedWithSkipStoryCinematic = false;

	//HandleServerError4015Class<FB2StartStageInfo>::GetInstance().Subscribe([this](const FB2StartStageInfo& StageInfo){ this->PopUpImpossibleToStartStageByFullInven(GET_TUPLE_DATA(FB2ResponseStartStage::inventory_count_infos_index, StageInfo)); });
	//HandleServerError4030Class<FB2StartStageInfo>::GetInstance().Subscribe([this](const FB2StartStageInfo& StageInfo){ this->FullEtherInventory(StageInfo);	});
}

void FCacheStageEssentialDataToKeep::SetStandaloneStageData(AB2StageManager* StageManager)
{
//	// �⺻�����δ� ������ �÷��� �� ���� ������ �ȵ� ���¿����� ������ �÷��� ������ ���.
//	// ���� ���ӿ��� �������� ��Ȳ�̸� ����� ����� �ƴ����� �������� ���� ���� ��Ʈ��ũ�� ����ٸ� �ϴ� �÷��̴� �����ؾ� �ϹǷ� ��� ���� ������ �ʿ��ϴ�.
//
//	check(StageManager);
//
//	TotalExpOfCurrentStage = 1000; // ����, ���� ������ �ƴ�.
//
//	// �ϴ� WorldSettings �� ������ ���� �켱 ���� �õ�. �ε� �ɼ��� �־��ٸ� �Ʒ����� �װ� ����� ��. ���� ���߿� ����
//	ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(StageManager->GetWorldSettings());
//	BII_CHECK(B2WS);
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (B2WS && !bPlayInExtraLevel)
//	{
//		ClientStageId = B2WS->GetDevTestStageNumber();
//		StageDifficultyLevel = (EStageDifficulty)B2WS->GetDevTestStageDifficulty();
//		bScenarioMode = B2WS->GetDevTestIsScenarioMode();
//	}
//#endif
//
//	// ���� ������ �������� ��ȣ ���� �ɼ��� �������Ʈ OpenLevel �ɼ� ���ڿ�(Open ��� ����)�� ���� ������ �� �ֵ���. ���������δ� �������� �����;� ��.
//	if (GEngine)
//	{
//		FWorldContext* TheWorldContext = GEngine->GetWorldContextFromWorld(StageManager->GetWorld());
//		if (TheWorldContext)
//		{
//			if (TheWorldContext->LastURL.HasOption(TEXT("StageNum")))
//			{
//				const TCHAR* StageNumStr = TheWorldContext->LastURL.GetOption(TEXT("StageNum="), TEXT("1"));
//				ClientStageId = FCString::Atoi(StageNumStr);
//			}
//			else if (TheWorldContext->LastURL.HasOption(TEXT("SN"))) // ©�� ����
//			{
//				const TCHAR* StageNumStr = TheWorldContext->LastURL.GetOption(TEXT("SN="), TEXT("1"));
//				ClientStageId = FCString::Atoi(StageNumStr);
//			}
//
//			if (TheWorldContext->LastURL.HasOption(TEXT("Difficulty")))
//			{
//				const TCHAR* DifficultyNumStr = TheWorldContext->LastURL.GetOption(TEXT("Difficulty="), TEXT("1"));
//				StageDifficultyLevel = (EStageDifficulty)FCString::Atoi(DifficultyNumStr);
//			}
//			else if (TheWorldContext->LastURL.HasOption(TEXT("DF"))) // ª�� ����
//			{
//				const TCHAR* DifficultyNumStr = TheWorldContext->LastURL.GetOption(TEXT("DF="), TEXT("1"));
//				StageDifficultyLevel = (EStageDifficulty)FCString::Atoi(DifficultyNumStr);
//			}
//			
//			if (TheWorldContext->LastURL.HasOption(TEXT("ScenarioMode")))
//			{
//				const TCHAR* ScenarioModeStr = TheWorldContext->LastURL.GetOption(TEXT("ScenarioMode="), TEXT("1"));
//				bScenarioMode = (FCString::Atoi(ScenarioModeStr) != 0);
//			}
//			else if (TheWorldContext->LastURL.HasOption(TEXT("SM"))) // ª�� ����
//			{
//				const TCHAR* ScenarioModeStr = TheWorldContext->LastURL.GetOption(TEXT("SM="), TEXT("1"));
//				bScenarioMode = (FCString::Atoi(ScenarioModeStr) != 0);
//			}
//			
//			if (TheWorldContext->LastURL.HasOption(TEXT("TotalExp")))
//			{
//				const TCHAR* TotalExpStr = TheWorldContext->LastURL.GetOption(TEXT("TotalExp="), TEXT("100"));
//				TotalExpOfCurrentStage = FCString::Atoi(TotalExpStr);
//			}
//		}
//	}
//
//	// �ܵ� �÷��̶� ���̵��� ���� �ó����� ��带 ���� ���ϵ���.
//	// ���⼭ ���� ó�� ���� ������ ���� ���̵������� �ó����� ������ ���� �� �� �ִµ� ���� ������ �÷��̿��� ����� �����ϰ� Ȯ���� ������ ����.
//	if (!IsScenarioModeAllowedDifficulty(StageDifficultyLevel))
//	{
//		bScenarioMode = false;
//	}
//
//	if (BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId > 0)
//	{
//		ClientStageId = BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId;
//		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = -1; // �ѹ� ���� ������ ���������� ������� ���
//	}
//
//	CachingStageInfoData();
//
//	if (CachedCurrentStageInfoData)
//	{
//		bHasExtraLevel = CachedCurrentStageInfoData->bHasExtraMap;
//
//		//ExtraMap�� World�� �ٸ� �������� ���� ���� �߰� ��������� �Ѵ�.(���� ����Ÿ Ȯ���ñ��� �ӽ�)
//		StageReward.SetTotalStageRewardGold(bPlayInExtraLevel ? 700 : 500);
//
//		UB2ItemInfo* ItemDataTable = StageReward.GetItemInfoTable();
//		if (ItemDataTable)
//		{
//			// �ϴ��� �������������� �ϳ��� �����۸��� ��ӵȴٰ� ����. (��ȹ�� �ִ� 2��)
//			// StageReward.SetDefeatBossRewardItem(ItemDataTable->GetRandomItemRefID());
//		}
//	}
//
//#if WITH_EDITOR // For use play-in-editor stage settings
//	if (GIsEditor){
//		bStandalonePIE = true;
//	}
//
//	// �̰͵� ������ ������ ��������
//	FB2Item DummyReward;
//	DummyReward.ItemRefID = 1000100;
//	StageReward.SetDefeatBossRewardItem(DummyReward);
//#endif
}

void FCacheStageEssentialDataToKeep::Initialize()
{
	StageReward.Initialize();

	//
	// ���� �ް� �� ���� �� �̰� �Ҹ��Ƿ� Request ���� ĳ�� �� �������� ���⼭ �ʱ�ȭ �ϸ� �ȵ�. �ʿ��ϸ� �����ڿ���..
	//

	PreExtraLastPlayedCharClass = EPCClass::EPC_Gladiator;

	ClientStageId = INDEX_NONE;
	ServerStageID = FServerStageID();
	bScenarioMode = true;
	StageDifficultyLevel = EStageDifficulty::ESD_Normal;

	PlayToken.Empty();
	bSpawnGoldMob = false;


	bHasExtraLevel = false;
	bPlayInExtraLevel = false;
	PlayTimeInMainMap = 0.f;
	MainMapStartingGold = 0;
	MainMapLastGold = 0;

	PreExtraMainPlayerLastHP = 0.0f;
	PreExtraMainPlayerLastMaxHP = 0.0f;
	PreExtraMainPlayerLastArmor = 0.0f;
	PreExtraMainPlayerWeaponSkillPoint = 0;
	PreExtraSubPlayerLastHP = 0.0f;
	PreExtraSubPlayerLastMaxHP = 0.0;
	PreExtraSubPlayerLastArmor = 0.0f;
	PreExtraSubPlayerWeaponSkillPoint = 0;

	TotalExpOfCurrentStage = 0;

	//CachedCurrentStageInfoData = NULL;

	/*MaxResurrectionChance = RemainingResurrectionChance = 3;
	ResurrectionCost = 0;*/

	// EResurrectGameModeType::EResurrectType_OnlyClientMode�� ����Ʈ���� �������ش�.
	//ResurrectionList.Empty();
	//for (int32 i = (int32)EResurrectGameModeType::EResurrectType_Stage; i < (int32)EResurrectGameModeType::EResurrectType_Max; ++i)
	//	ResurrectionList.Add((EResurrectGameModeType)i, FResurrectData());

	//ServerSelectedStageBuff.Empty();

	/*
	//UI���� ������ ���� �ϱ� ���� Doc�� �����ͽ�ũ
	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		DocBattle->SetMaxResurrectionChance(MaxResurrectionChance);
		DocBattle->SetRemainingResurrectionChance(RemainingResurrectionChance);
		DocBattle->SetResurrectionCost(ResurrectionCost);
	}
	*/
#if WITH_EDITOR // For use play-in-editor stage settings
	bStandalonePIE = GIsEditor; // �����Ϳ����� �κ� ���� �ε��Ǿ� ���� ������ ���� ��� �ٽ� false ������ �� ��.
#endif
}

void FCacheStageEssentialDataToKeep::PopUpImpossibleToStartStageByFullInven(const TArray<b2network::B2InventoryCountServerInfoPtr>& InvenCountInfo)
{
	/*EPCClass FinalGoBackInvenPageDefClass = EPCClass::EPC_End;
	auto UIP_RepeatBattleCountDownP = UB2UIManager::GetInstance()->GetUI<UB2UIRepeatBattleCountDown>(UIFName::RepeatBattleCountDown);

	if (UIP_RepeatBattleCountDownP)
		UIP_RepeatBattleCountDownP->StopCountdown();

	CancelOrStopRepeatBattleClass<>::GetInstance().Signal();

	if (InvenCountInfo.Num() > 0)
	{
		FinalGoBackInvenPageDefClass = SvrToCliPCClassType(InvenCountInfo[0]->character_type);
	}

	if ( UB2UIManager::GetInstance() && UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::StageResult)
	{
		B2UIUtil::OpenInventoryFullPopup( FinalGoBackInvenPageDefClass, FMsgPopupOnClick::CreateLambda( [ this, FinalGoBackInvenPageDefClass ]()
		{
			FServerStageID InStageID = GetServerStageID();
			FRepeatBattleStateSet InStateSte = GetRepeatBattleState();

			FLobbySceneManager::DeferredRegistChangeLobbyScene( [InStageID, InStateSte, FinalGoBackInvenPageDefClass ]()
			{
				if ( auto* UIManager = UB2UIManager::GetInstance() )
				{
					UIManager->ArtificialAddUIHistory( EUIScene::LobbyMain );
					UIManager->ChangeUIScene(EUIScene::AdventureEnterBattleMain);
					UIManager->ChangeUIScene(EUIScene::Chapter);
					GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Signal(InStageID, InStateSte);
					LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterPage);
				}

				LobbyHeroMgmtSelectClass< EPCClass >::GetInstance().Signal( FinalGoBackInvenPageDefClass );
				LobbyEnterHeroMgmtModeClass< EHeroMgmtSubMode >::GetInstance().Signal( EHeroMgmtSubMode::EHMSM_Inventory );
				LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal( EItemInvenType::EIIVT_Weapon, false );
			} );

			OpenBladeIILobbyCommon( Cast< AGameMode >( UGameplayStatics::GetGameMode( GWorld ) ) );

		} ), false );
	}
	else
	{
		auto InventoryPopUp = B2UIUtil::OpenInventoryFullPopup(FinalGoBackInvenPageDefClass);
		if (InventoryPopUp.IsValid() && InvenCountInfo.Num() > 0)
		{
			InventoryPopUp->SetInventoryItemCount(InvenCountInfo[0]->count, InvenCountInfo[0]->max_count);
		}
	}*/
}

bool FCacheStageEssentialDataToKeep::EquipNumberCheck(EPCClass& PCClass)
{
	//TArray<EPCClass> AllPCClassNeedCleanInven;
	//UB2LobbyInventory::GetPCClassesNeedToCleanInventory(AllPCClassNeedCleanInven);

	///*
	//// AllPCClassNeedCleanInven �� ������� �ʾƾ� ���� ������ �������� ���� ������ ��� ������ ���ؼ� check �� �ɰ� ������ �ʱ�.
	//// �κ��丮�� ����� �ʿ䰡 �ִ� Ŭ�������� �̸� �����ؼ� �κ��丮 â ���� �� ���õǾ� �ִ� Ŭ������ �ݿ��Ѵ�.
	//const bool bIsMainClassNeedToCleanInven = (AllPCClassNeedCleanInven.Find(RequestedMainPlayerCharClass) != INDEX_NONE);
	//const bool bIsSubClassNeedToCleanInven = (AllPCClassNeedCleanInven.Find(RequestedSubPlayerCharClass) != INDEX_NONE);
	//// Main/Sub �� �� �������� �ٸ� ���� �ȵ� ���� inventory �� �� ������ �װ͵� ����Ұ���.
	//const EPCClass FallbackInvenCleanCharClass = (AllPCClassNeedCleanInven.Num() > 0) ? AllPCClassNeedCleanInven[0] : EPCClass::EPC_Gladiator;
	//PCClass = bIsMainClassNeedToCleanInven ? RequestedMainPlayerCharClass :
	//	(bIsSubClassNeedToCleanInven ? RequestedSubPlayerCharClass : FallbackInvenCleanCharClass);
	//if (PCClass == EPCClass::EPC_End) { // �̷� ���� ����߰�����..
	//	PCClass = EPCClass::EPC_Gladiator;
	//}

	//// �׸��� Ŭ���� ������ �Ǿ��ٸ� ������ �� �ִ� Ŭ���� �Ǳ���..
	//const bool bIsNeedToClean = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, PCClass);
	//if (!bIsNeedToClean)
	//	return false;
	//*/

	//if (AllPCClassNeedCleanInven.Num() > 0)
	//{
	//	// �� ������ PreLoadingScreen �� ���� ���� ���̹Ƿ� �ݵ���.
	//	ClosePreLoadingScreenAllClass<>::GetInstance().Signal();
	//	CancelStartGameStageClass<>::GetInstance().Signal();
	//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // ��Ÿ ���� ��û ó���� �Ұ��� ��� ������ �ݺ� ���� ��û�� ����ؾ� ��.

	//	const EPCClass FallbackInvenCleanCharClass = AllPCClassNeedCleanInven[0];
	//	PCClass = FallbackInvenCleanCharClass;
	//	return true;
	//}
	//else
	//{
		return false;
	//}
}

void FCacheStageEssentialDataToKeep::SetRepeatBattleStateBoostOn(bool InBoostOn)
{
	RepeatBattleState.bBoostOn = InBoostOn;
}

void FCacheStageEssentialDataToKeep::FullEtherInventory(const FB2StartStageInfo& StageInfo)
{
	/*UB2UIRepeatBattleCountDown* UIP_RepeatBattleCountDownP = UB2UIManager::GetInstance()->GetUI<UB2UIRepeatBattleCountDown>(UIFName::RepeatBattleCountDown);
	if (UIP_RepeatBattleCountDownP)
	{
		UIP_RepeatBattleCountDownP->StopCountdown();
	}

	CancelOrStopRepeatBattleClass<>::GetInstance().Signal();

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		int32 CurrentCount = GET_TUPLE_DATA(FB2ResponseStartStage::current_aether_count_index, StageInfo);
		int32 MaxCount = GET_TUPLE_DATA(FB2ResponseStartStage::current_aether_max_count_index, StageInfo);
		UB2UIEtherFullPopup* Popup = UIMgrInst->OpenUI<UB2UIEtherFullPopup>(UIFName::EtherFullPopup);
		Popup->SetEtherFullPopupInfo(CurrentCount, MaxCount);

		FServerStageID InStageID = GetServerStageID();
		FRepeatBattleStateSet InStateSte = GetRepeatBattleState();

		

		Popup->SetMoveEtherDelegate([InStageID , InStateSte]()
		{
			if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::StageResult)
			{
				FLobbySceneManager::DeferredRegistChangeLobbyScene([InStageID, InStateSte]()
				{
					if (auto* UIManager = UB2UIManager::GetInstance())
					{
						UIManager->ArtificialAddUIHistory(EUIScene::LobbyMain);
						UIManager->ChangeUIScene(EUIScene::AdventureEnterBattleMain);
						UIManager->ChangeUIScene(EUIScene::Chapter);
						GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Signal(InStageID, InStateSte);
						LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_CharacterPage);
						LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_EtherMain);
					}

				});

				OpenBladeIILobbyCommon(Cast< AGameMode >(UGameplayStatics::GetGameMode(GWorld)));
			}
			else
			{
				LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_EtherMain);
			}
		});
		
	}*/
}


void FCacheStageEssentialDataToKeep::InitQTECount()
{
	StartQTEAttackCount  = 0;
	FinishQTEAttackCount = 0;
	RecentQTEState       = -1;
}

void FCacheStageEssentialDataToKeep::AddQTECount(int32 QTEStateNum)
{
	RecentQTEState = QTEStateNum;

	switch (QTEStateNum)
	{
	case 0:
	{
		++StartQTEAttackCount;
	}
	break;
	case 1:
	{
		++FinishQTEAttackCount;
	}
	break;
	default:
		break;
	}
}

int32 FCacheStageEssentialDataToKeep::GetQTECount(int32 QTEStateNum) const
{
	return QTEStateNum ? FinishQTEAttackCount : StartQTEAttackCount;
}

int32 FCacheStageEssentialDataToKeep::GetQTEBonus(int32 QTEStateNum) const
{
	return QTEStateNum ? FinishQTEBonusValue : StartQTEBonusValue;
}

void FCacheStageEssentialDataToKeep::SetStageResponseData(FString Token, int32 DropGolds, int32 DropGoldGoldmob, const TArray<FB2Item>& Items, int32 InMaxResurrectionChance, const TArray<FB2Item>& _bossPieceItems)
{
	PlayToken = Token;

	bSpawnGoldMob = DropGoldGoldmob > 0;

	StageReward.SetTotalStageRewardGold(DropGolds + DropGoldGoldmob);
	for (auto& Item : Items)
		StageReward.SetDefeatBossRewardItem(Item);

	for (auto bossPieceItem : _bossPieceItems)
		StageReward.SetDefeatBossPieceRewardItem(bossPieceItem);

	//SetClearResurrection(EResurrectGameModeType::EResurrectType_Stage, InMaxResurrectionChance);

#if WITH_EDITOR // For use play-in-editor stage settings
	bStandalonePIE = false;
#endif
}

void FCacheStageEssentialDataToKeep::SetStageResponseDataQuest(int32 QuestItemID, int32 QuestItemCount)
{
	// �ϴ��� �ƹ��͵� ���� 
}

void FCacheStageEssentialDataToKeep::SetQTEBonus(int32 StartQTEBonus, int32 FinishQTEBonus)
{
	StartQTEBonusValue = StartQTEBonus;
	FinishQTEBonusValue = FinishQTEBonus;
}

void FCacheStageEssentialDataToKeep::SetStageResponseDataStageBuff(int32 BuffFlag)
{
	SvrToCliStageBuffTypeComposit(BuffFlag, ServerSelectedStageBuff);
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetSelectedStageBuffs(ServerSelectedStageBuff); // Server Response���� ������ Buff�� UIDoc�� ����
	}
}

void FCacheStageEssentialDataToKeep::RequestStartStage(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass, const TArray<EStageBuffType>& StageBuffTypes, FRepeatBattleStateSet RepeatBattle, bool bSkipStoryCinematic, int32 MainPresetId, int32 SubPresetId)
{
	/*int32 iRequestTutorialID = 0;

	if (TutorialManager::GetInstance().IsTutorialAvailable())
	{
		int32 iCurrentTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();

		if (iCurrentTutorialID == TutorialID_FirstStageClear || iCurrentTutorialID == TutorialID_ExpertDungon)
			iRequestTutorialID = iCurrentTutorialID;
	}
	
	RequestStartStage(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass, StageBuffTypes, RepeatBattle, bSkipStoryCinematic, iRequestTutorialID, MainPresetId, SubPresetId);*/
}

void FCacheStageEssentialDataToKeep::RequestStartStage(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass, const TArray<EStageBuffType>& StageBuffTypes, FRepeatBattleStateSet RepeatBattle, bool bSkipStoryCinematic, int32 TurorialId, int32 MainPresetId, int32 SubPresetId)
{
	//// [180712_YJ] ���̵� On/OFF��� �߰�.
	//// Ȥ�ó� ������ �������̵��� ����������쿡, �����˾� ȣ�� �� �κ�� �̵�.
	//// Ʃ�丮�� ��� ����.
	//if (static_cast<int32>(InStageDifficulty) > BladeIIGameImpl::GetClientDataStore().GetLimitOpenStageDifficulty())
	//{
	//	UB2UIMsgPopupSimple* MsgUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_ActOff")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm,
	//		FMsgPopupOnClick::CreateLambda([this]() { LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain); })
	//		);

	//	return;
	//}

	//InitQTECount();

	//RequestedMainPlayerCharClass = MainCharClass;
	//RequestedSubPlayerCharClass = SubCharClass;
	//RequestedClientStageId = InClientStageId;
	//RequestedStageDifficulty = InStageDifficulty;
	//RequestedStageOption = CliToSvrStageBuffTypeComposit(StageBuffTypes);
	//RequestServerStageId = BladeIIGameImpl::GetStageDataStore().GetServerStageID(RequestedClientStageId, InStageDifficulty);
	//RequestedTutorialId = TurorialId;
	//RequestMainPCPresetId = MainPresetId;
	//RequestSubPCPresetId = SubPresetId;

	//bStageRequestedWithSkipStoryCinematic = bSkipStoryCinematic; // ������ �� �� bScenarioMode ���� �Ǻ��� ����.

	//// ���⼭�� RepeatBattleState �� Response �� ���� �����Ѵ�.

	////ToDo ������ �����û ��Ŷ�� ������. (��������Id, ����/���� ĳ���� Ŭ����, ��Ÿ ������(����, �������߰��ǵ��)
	//
	//UE_LOG(LogBladeII, Log, TEXT("StageRequest"));
	////[@AKI, 180214] RequestServerStageId�� 0���� ������ ���� ������ ensure�ɾ� ����. Ȥ�� ���⿡ �ɸ� ��� �ȱ��Ϳ��� ���� �ٶ�.
	//ensure( !RequestServerStageId.IsEmpty() );
	//if (RequestServerStageId.IsEmpty())
	//	RequestServerStageId.Id = 1;
	//data_trader::Retailer::GetInstance().RequestStartStage(
	//	RequestServerStageId,
	//	CliToSvrPCClassType(RequestedMainPlayerCharClass),
	//	CliToSvrPCClassType(RequestedSubPlayerCharClass),
	//	RequestedStageOption,
	//	RepeatBattle.bRepeatOne,
	//	RepeatBattle.bBoostOn,
	//	RequestedTutorialId,
	//	RequestMainPCPresetId, 
	//	RequestSubPCPresetId);	// @TODO main / tag character's preset id
}

void FCacheStageEssentialDataToKeep::RequestStartStageAgainAtPlace()
{ 
	TArray< EStageBuffType > RequestBuffList;
	UB2UIDocBattleStage* DocBattleState = UB2UIDocHelper::GetDocBattleStage();
	BII_CHECK( DocBattleState );
	if ( DocBattleState )
	{
		RequestBuffList = DocBattleState->GetSelectedStageBuffs();
		
		if ( RepeatBattleState.bIsOn && RepeatBattleState.bRepeatOne )
		{
			// Increase Repeat Battle Count
			RepeatBattleState.CurrentRepeatCount += 1;
		}
	}

	int32 MainPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	int32 SubPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	RequestStartStage( ClientStageId
		, StageDifficultyLevel
		, BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass()
		, BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass()
		, RequestBuffList
		, RepeatBattleState
		, bStageRequestedWithSkipStoryCinematic
		, MainPCPresetId
		, SubPCPresetId );
}

void FCacheStageEssentialDataToKeep::RequestStartStageNext()
{
	int32 MainPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	int32 SubPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	TArray<EStageBuffType> ThisRequestBuff = ServerSelectedStageBuff; // Ȥ�ö� �ʱ� ���ð� ���������� �����ؼ� �־�� �Ѵٸ� AllSelectedStageBuff ��
	RequestStartStage( ClientStageId + 1
		, StageDifficultyLevel
		, BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass()
		, BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass()
		, ThisRequestBuff
		, ( RepeatBattleState.bIsOn && RepeatBattleState.bRepeatAll ) ? RepeatBattleState : FRepeatBattleStateSet()
		, bStageRequestedWithSkipStoryCinematic
		, MainPCPresetId
		, SubPCPresetId);
}

void FCacheStageEssentialDataToKeep::RequestStartCounterDungeon(FServerStageID InServerStageId, EPCClass MainCharClass, EPCClass SubCharClass)
{
	/*auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	RequestedMainPlayerCharClass = MainCharClass;
	RequestedSubPlayerCharClass = SubCharClass;
	
	RequestServerStageId = InServerStageId;
	RequestedStageDifficulty = StageDataStore.GetStageDifficulty(InServerStageId);
	RequestedClientStageId = StageDataStore.GetClientStageId(InServerStageId);

	data_trader::Retailer::GetInstance().RequestStartCounterDungeon(
		RequestServerStageId,
		CliToSvrPCClassType(RequestedMainPlayerCharClass));*/
}

void FCacheStageEssentialDataToKeep::PrepareForceStartCounterDungeon(FServerStageID InServerStageId)
{
	/*auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	auto CounterDungeonData = ClientDataStore.GetCounterDungeonData();
	for (auto& Ele : CounterDungeonData)
	{
		if (Ele.ServerStageId == InServerStageId)
		{
			auto* PeriodData = ClientDataStore.GetCounterDungeonPeriodData(Ele.DungeonType);
			if (PeriodData)
			{
				for (auto& ElePeriod : *PeriodData)
				{
					RequestedMainPlayerCharClass = ElePeriod.CharacterType;
					break;
				}
			}
			break;
		}
	}

	RequestServerStageId = InServerStageId;
	RequestedClientStageId = StageDataStore.GetClientStageId(InServerStageId);
	RequestedStageDifficulty = StageDataStore.GetStageDifficulty(InServerStageId);*/
}

void FCacheStageEssentialDataToKeep::SetStageBasisData(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass, int32* InStageOption, FRepeatBattleStateSet* InRepeatBattleState)
{
	//Initialize();

	//FStageDataStore& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	//ServerStageID = StageDataStore.GetServerStageID(InClientStageId, InStageDifficulty);

	//// ó�� �÷��� �ÿ��� true. �ι�° �̻��� ������ ���丮��忡�� ����ڰ� ���丮�ѱ��� �����߰ų�, ������(�Ϲ� �̻��� ���̵�)�̸� false.
	//bScenarioMode =
	//	// ���̵��� �޶����� �� ServerStageID �� �޶����� �ǹǷ� ù �÷��� �ÿ��� IsStageClearedOnce �� �ٽ� false �� �ȴ�. ���� IsScenarioModeAllowedDifficulty �� && ������ �Ǿ�� ��.
	//	IsScenarioModeAllowedDifficulty(InStageDifficulty) && 
	//	(StageDataStore.IsStageClearedOnce(ServerStageID) == false || !bStageRequestedWithSkipStoryCinematic);

	//SetStageBasisData_CommonInternal(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass);

	//if (InRepeatBattleState)
	//	RepeatBattleState = *InRepeatBattleState;
}

void FCacheStageEssentialDataToKeep::SetStageBasisData_CounterDungeon(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass)
{
	Initialize();

	// �ݰݴ����� ClientId�� ServerId�� ����.
	ServerStageID = FServerStageID(InClientStageId);
	bScenarioMode = false;

	SetStageBasisData_CommonInternal(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass);
}

void FCacheStageEssentialDataToKeep::SetStageBasisData_CommonInternal(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass)
{
	// ServerStageID �� �̰� ������ ���õǾ� �־�� �ϴµ�... TutorialGameMode ���� ���� ��Ȳ�� �־ üũ�� ���ϰڱ� �Ѥ�
	//check(!ServerStageID.IsEmpty());

	ClientStageId = InClientStageId;
	StageDifficultyLevel = InStageDifficulty;
	
	BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(MainCharClass);
	BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(SubCharClass);

	CachingStageInfoData();

	////if (CachedCurrentStageInfoData)
	//	bHasExtraLevel = CachedCurrentStageInfoData->bHasExtraMap;
}

void FCacheStageEssentialDataToKeep::ClearStageBasisData()
{
	ClientStageId = INDEX_NONE;
	ServerStageID = FServerStageID(); // �ϴ� �̰� �����ϴ� �� �� �߿�.
	bScenarioMode = true; // ��� �⺻�� true �� �� ��︮�ڴ�.
	bStageRequestedWithSkipStoryCinematic = false;
	ServerSelectedStageBuff.Empty();
	bHasExtraLevel = false;
	//[@AKI, 180208] Extra Map ���ٿ��� �ʱ�ȭ�� �Ƚ�Ű�� ����(ĳ�� �Ǿ� �ִ� ���簡 ��������)
	bPlayInExtraLevel = false;
}

void FCacheStageEssentialDataToKeep::CachingStageInfoData()
{
	//CachedCurrentStageInfoData = NULL;
	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	//if (StageInfoTable)
	//{
	//	CachedCurrentStageInfoData = StageInfoTable->GetInfoData(ClientStageId);
	//}
}

void FCacheStageEssentialDataToKeep::ResponseStartStage(AGameMode* InCurrentGameMode, const FB2StartStageInfo& Info)
{
	//AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(InCurrentGameMode); // �Ϲ����� ��� ����� ������ �� LobbyGameMode. �κ񿡼� �������� ���� ��ư ������ ���� ���� �� Ŭ���̾�Ʈ �� �ε� �� �� �����ϴ� �κ�.
	//FRepeatBattleStateSet NewRepeatBattleState = RepeatBattleState;
	//if (LobbyGM){
	//	NewRepeatBattleState = LobbyGM->GetRepeatBattleRequestState();
	//}

	//if (auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	//{
	//	DocBattleStage->SetAllServival(true);
	//	DocBattleStage->SetStartExpBuffRate(GET_TUPLE_DATA(FB2ResponseStartStage::exp_buff_rate_index, Info));
	//	DocBattleStage->SetStartGoldBuffRate(GET_TUPLE_DATA(FB2ResponseStartStage::gold_buff_rate_index, Info));
	//}

	//// FB2StartStageInfo �� ���� �㰡�� StageBuff ������ ����ִٸ� RequestedStageOption ��� �װɷ� �ؼ� SetStageResponseData ���� ������
	//SetStageBasisData(RequestedClientStageId, RequestedStageDifficulty, RequestedMainPlayerCharClass, RequestedSubPlayerCharClass, &RequestedStageOption, &NewRepeatBattleState);

	//TArray<FB2Item>	Items;
	//TU::CopyVectorHelper<FB2Item, b2network::B2ItemServerInfoPtr>	helper(&Items);

	//helper = GET_TUPLE_DATA(FB2ResponseStartStage::drop_items_index, Info);
	//TArray<b2network::B2DropDataPtr> dropinfos = GET_TUPLE_DATA(FB2ResponseStartStage::drop_datas_index, Info);
	//
	//FB2Item dropItemInfo;
	//TArray<FB2Item>	dropItemsInfo;
	//for (auto dropinfo : dropinfos)
	//{
	//	dropItemInfo.ItemRefID = dropinfo->template_id;
	//	dropItemInfo.ConsumingAmount = dropinfo->count;
	//	dropItemsInfo.Add(dropItemInfo);
	//}

	//SetStageResponseData(
	//	GET_TUPLE_DATA(FB2ResponseStartStage::token_index, Info),
	//	GET_TUPLE_DATA(FB2ResponseStartStage::drop_gold_index, Info),
	//	GET_TUPLE_DATA(FB2ResponseStartStage::drop_gold_by_gold_monster_index, Info),
	//	Items,
	//	GET_TUPLE_DATA(FB2ResponseStartStage::max_resurrection_count_index, Info),
	//	dropItemsInfo
	//);

	//SetQTEBonus(
	//	GET_TUPLE_DATA(FB2ResponseStartStage::qte_bonus_attack_index, Info),
	//	GET_TUPLE_DATA(FB2ResponseStartStage::qte_bonus_finish_index, Info)
	//);

	//BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::StageBoostTicket, GET_TUPLE_DATA(FB2ResponseStartStage::stage_boost_ticket_index, Info));

	//GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(GET_TUPLE_DATA(FB2ResponseStartStage::guild_skills_index, Info));//woong

	//SetStageResponseDataStageBuff(GET_TUPLE_DATA(FB2ResponseStartStage::selected_option_flags_index, Info));
	//SetStageResponseDataQuest(GET_TUPLE_DATA(FB2ResponseStartStage::quest_item_id_index, Info),
	//						  GET_TUPLE_DATA(FB2ResponseStartStage::quest_item_count_index, Info));

	//// �������� ���۰� �Բ� AccountInfo ���� �ʿ�� ���� ClientDataStore/StageDataStore �ʿ���.
	//BladeIIGameImpl::GetStageDataStore().StartStage(Info);

	//bool StartStageRet = StartBladeIIGameStage(InCurrentGameMode, ClientStageId);

	//if (TutorialManager::GetInstance().IsTutorialAvailable())
	//{
	//	UB2UITutorial* Popup = UB2UIManager::GetInstance()->GetUI<UB2UITutorial>(UIFName::Tutorial);
	//	if (Popup)
	//	{
	//		B2AndroidBackManager::GetInstance()->RemoveBackWidget(Popup);
	//	}
	//}

	//// ���� ������ �α�����
	//if (IsScenarioMode())
	//{
	//	B2ActionLogSender Sender;
	//	Sender.setCategory(FString("PLAY"))
	//		.setAction(FString("CINEMATIC"))
	//		.setLabel(FString::FromInt(ClientStageId))
	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//}
}

void FCacheStageEssentialDataToKeep::ResponseStartCounterDungeon(AGameMode* InCurrentGameMode, const FString& Token)
{
	//// �ݰݴ����� GetServerStageID�� ServerStageID�� ������ �� �����Ƿ�, SetStageBasisData_CounterDungeon() ���� ���� ó��
	//SetStageBasisData_CounterDungeon(RequestedClientStageId, RequestedStageDifficulty, RequestedMainPlayerCharClass, RequestedSubPlayerCharClass);

	//TArray<FB2Item> EmptyArray;
	//SetStageResponseData(Token, 0, 0, EmptyArray, 0, EmptyArray);

	//StartBladeIICounterDungeon(InCurrentGameMode, ClientStageId);
}

// colosseum comment : bUseGem ����
void FCacheStageEssentialDataToKeep::RequestStartHeroTower(AGameMode* InCurrentGameMode, int32 nStartFloor, EPCClass MainCharClass, EPCClass SubCharClass, FString szToken)
{
	//RequestedMainPlayerCharClass = MainCharClass;
	//RequestedSubPlayerCharClass = SubCharClass;

	//// ������ �����͵����Ϳ��� ���̵� ���Ͷ�
	//RequestedClientStageId = BladeIIGameImpl::GetClientDataStore().GetHeroTowerMapId(nStartFloor);
	//	
	//data_trader::Retailer::GetInstance().RequestStartHeroTower(nStartFloor, 
	//	CliToSvrPCClassType(RequestedMainPlayerCharClass),
	//	CliToSvrPCClassType(RequestedSubPlayerCharClass),
	//	szToken
	//	);
}

void FCacheStageEssentialDataToKeep::ResponseStartHeroTower(AGameMode* InCurrentGameMode, const FB2ResponseStartHeroTowerPtr& msgPtr)
{
//	SetStageBasisData(RequestedClientStageId, EStageDifficulty::ESD_Normal, RequestedMainPlayerCharClass, RequestedSubPlayerCharClass);
//
//	TArray<FB2Item> EmptyArray;
//	SetStageResponseData(msgPtr->play_token, 0, 0, EmptyArray, 0, EmptyArray);
//	
//	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();
//
//	if (pDoc)
//	{
//		// colosseum comment : daily_left_free_try_count ����
//		//pDoc->m_ptrGetHeroTower->daily_left_free_try_count = msgPtr->left_free_ticket;
//		pDoc->m_ptrGetHeroTower->daily_try_count = msgPtr->daily_enter_count;
//		// colosseum comment : current_gem ����
//		//BladeIIGameImpl::GetClientDataStore().SetGemAmount(msgPtr->current_gem);
//		pDoc->m_bIsClearHeroTower = false;
//	}	
//	
//#if !UE_BUILD_SHIPPING
//	extern bool gUseHeroTowerResetNoLoading;
//#endif
//	AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(InCurrentGameMode);
//	if (pGM 
//#if !UE_BUILD_SHIPPING // ������ ž�� ���� �� �ٷ� �̵��� �ε� ���� �������� ������ �ϴµ� �̰� ���ߵ��� �߰��Ǵ� �ڵ� ������ �ȵǸ� ������ ����Ű�� ����Ƿ� Ư�� ���� �߻��� �׽�Ʈ�� ���� �÷��׸� ����.
//		&& gUseHeroTowerResetNoLoading
//#endif
//		)
//		pGM->ResetAssumeNoWorldLoading();
//	else
//		StartBladeIIHerotowerGame(InCurrentGameMode, RequestedClientStageId);
//
//	GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(msgPtr->guild_skills);
}

void FCacheStageEssentialDataToKeep::RequestStartDimension(AGameMode* InCurrentGameMode, int32 nDiff, int32 nMap, EPCClass MainCharClass, EPCClass SubCharClass)
{
	/*RequestedMainPlayerCharClass = MainCharClass;
	RequestedSubPlayerCharClass = SubCharClass;

	auto DocDimension = UB2UIDocHelper::GetDocDimension();
	if (DocDimension)
	{
		RequestedClientStageId = DocDimension->GetClientStageID();
	}
	const int32 Combat = CombatStatEval::GetPCCombatPower(MainCharClass);

	data_trader::Retailer::GetInstance().RequestStartDimension(nDiff, 
																nMap,
																CliToSvrPCClassType(RequestedMainPlayerCharClass),
																Combat);*/
}

void FCacheStageEssentialDataToKeep::ResponseStartDimension(AGameMode* InCurrentGameMode, const FB2ResponseStartDimensionPtr& msgPtr)
{
	//SetStageBasisData(RequestedClientStageId, EStageDifficulty::ESD_Normal, RequestedMainPlayerCharClass, RequestedSubPlayerCharClass);

	//TArray<FB2Item> EmptyArray;
	//// ���۵���� 100% �̴� ������ �����Ұ� ì����
	//TArray<FB2Item> DefaultRewardArray;
	//FB2Item NewItem;
	//NewItem.ItemRefID = FItemRefIDHelper::ITEM_REF_ID_PREVIEW_TOTEM;
	//DefaultRewardArray.Add(NewItem);
	//SetStageResponseData(msgPtr->play_token, 0, 0, DefaultRewardArray, 0, EmptyArray);

	//int32 LoadMapID = -1;
	//UB2UIDocDimension* pDoc = UB2UIDocHelper::GetDocDimension();
	//if (pDoc)
	//{
	//	LoadMapID = pDoc->GetMapLoadID();
	//}

	//AB2DimensionGameMode* pGM = Cast<AB2DimensionGameMode>(InCurrentGameMode);
	//StartBladeIIDimension(InCurrentGameMode, LoadMapID);

	//GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(msgPtr->guild_skills);
}

//void FCacheStageEssentialDataToKeep::SetClearResurrection(EResurrectGameModeType ResurrectModeType, int32 InMaxResurrectionChance)
//{
//	//ResurrectionList[ResurrectModeType].ClearResurrectionChange(InMaxResurrectionChance);
//	//SetResurrectionCostByLookup(ResurrectModeType, ResurrectionList[ResurrectModeType].MaxResurrectionChance, ResurrectionList[ResurrectModeType].RemainingResurrectionChance);
//
//	////UI���� ������ ���� �ϱ� ���� Doc�� �����ͽ�ũ
//	//if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
//	//{
//	//	DocBattle->SetResurrectGameModeType(ResurrectModeType);
//	//	DocBattle->SetMaxResurrectionChance(ResurrectionList[ResurrectModeType].MaxResurrectionChance);
//	//	DocBattle->SetRemainingResurrectionChance(ResurrectionList[ResurrectModeType].RemainingResurrectionChance);
//	//	DocBattle->SetResurrectionCost(ResurrectionList[ResurrectModeType].ResurrectionCost);
//	//}
//}

//void FCacheStageEssentialDataToKeep::DecreaseRemainingResurrectionChance(EResurrectGameModeType ResurrectModeType)
//{ 
//	if (ResurrectModeType == EResurrectGameModeType::EResurrectType_OnlyClientMode)
//		return;
//
//	//BII_CHECK(ResurrectionList.Contains(ResurrectModeType));
//
//	//ResurrectionList[ResurrectModeType].DecreaseResurrectionChance();
//	//SetResurrectionCostByLookup(ResurrectModeType, ResurrectionList[ResurrectModeType].MaxResurrectionChance, ResurrectionList[ResurrectModeType].RemainingResurrectionChance);
//
//	//auto* BattleDoc = UB2UIDocHelper::GetDocBattle();
//	//if (BattleDoc)
//	//	BattleDoc->SetRemainingResurrectionChance(ResurrectionList[ResurrectModeType].RemainingResurrectionChance);
//}

//void FCacheStageEssentialDataToKeep::SetResurrectionCostByLookup(EResurrectGameModeType ResurrectModeType, int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance)
//{
//	//const int32 NextResurrectionNum = InMaxResurrectionChance - InRemainingResurrectionChance + 1;
//	//// ��Ȱ ���� ���� ������ �ִ� ����� ClientDataStore ���� ���.
//
//	//ResurrectionList[ResurrectModeType].ResurrectionCost = BladeIIGameImpl::GetClientDataStore().GetResurrectionCost(ResurrectModeType, NextResurrectionNum);
//	//
//	//// UIDoc���� ����
//	//if (auto* BattleDoc = UB2UIDocHelper::GetDocBattle())
//	//	BattleDoc->SetResurrectionCost(ResurrectionList[ResurrectModeType].ResurrectionCost);
//}

void FCacheStageEssentialDataToKeep::SetReadyPreExtraCombatVariables(class ABladeIIPlayer* CurrentActivePlayer, class UB2SkillRTManager* SkillRTMgr)
{
	/*if (CurrentActivePlayer && SkillRTMgr)
	{
		PreExtraMainPlayerLastHP = CurrentActivePlayer->AmIMainCharacter() ? CurrentActivePlayer->GetHealth() : SkillRTMgr->GetInactiveCharacterHealth();
		PreExtraMainPlayerLastMaxHP = CurrentActivePlayer->AmIMainCharacter() ? CurrentActivePlayer->GetMaxHealth() : SkillRTMgr->GetInactiveCharacterMaxHealth();
		PreExtraMainPlayerWeaponSkillPoint = CurrentActivePlayer->AmIMainCharacter() ? CurrentActivePlayer->GetWeaponSkillPoint() : SkillRTMgr->GetInactiveCharacterWeaponSkillPoint();
		PreExtraMainPlayerLastArmor = CurrentActivePlayer->AmIMainCharacter() ? CurrentActivePlayer->Armor : SkillRTMgr->GetInactiveCharacterArmor();

		if (SkillRTMgr->GetInactivePlayerNotCachedYet() && RequestedSubPlayerCharClass != EPCClass::EPC_End)
		{	
			if (auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(RequestedSubPlayerCharClass)))
			{
				PreExtraSubPlayerLastHP = DocHero->GetHealthPoint();
				PreExtraSubPlayerLastMaxHP = DocHero->GetHealthPoint();
			}
		}
		else
		{
			PreExtraSubPlayerLastHP = CurrentActivePlayer->AmIMainCharacter() ? SkillRTMgr->GetInactiveCharacterHealth() : CurrentActivePlayer->GetHealth();
			PreExtraSubPlayerLastMaxHP = CurrentActivePlayer->AmIMainCharacter() ? SkillRTMgr->GetInactiveCharacterMaxHealth() : CurrentActivePlayer->GetMaxHealth();
		}
		
		PreExtraSubPlayerWeaponSkillPoint = CurrentActivePlayer->AmIMainCharacter() ? SkillRTMgr->GetInactiveCharacterWeaponSkillPoint() : CurrentActivePlayer->GetWeaponSkillPoint();
		PreExtraSubPlayerLastArmor = CurrentActivePlayer->AmIMainCharacter() ? SkillRTMgr->GetInactiveCharacterArmor() : CurrentActivePlayer->Armor;
	}*/
}

void FCacheStageEssentialDataToKeep::SetPostExtraMapLoadSkillRTManager(class ABladeIIPlayer* CurrentActivePlayer, struct FInactivePlayerStateCache* StructToSet)
{
	//if (CurrentActivePlayer && StructToSet)
	//{// StructToSet �� ���� �����ִ� �±� ĳ���� ���� ������̶� AmIMainCharacter �� �ݴ��� ���� ����ؼ� ����.
	//	StructToSet->MaxHealth = CurrentActivePlayer->AmIMainCharacter() ? PreExtraSubPlayerLastMaxHP : PreExtraMainPlayerLastMaxHP;
	//	// ��ǻ� ExtraMap �ε� ������ CurrentHealth ���� �ǵ� LastHealth �� ���� �ʿ��Ѱ�?
	//	StructToSet->LastHealth = CurrentActivePlayer->AmIMainCharacter() ? PreExtraSubPlayerLastHP : PreExtraMainPlayerLastHP;
	//	StructToSet->CurrentHealth = StructToSet->LastHealth;
	//	StructToSet->LastArmor = CurrentActivePlayer->AmIMainCharacter() ? PreExtraSubPlayerLastArmor : PreExtraMainPlayerLastArmor;
	//	StructToSet->WeaponSkillPoint = CurrentActivePlayer->AmIMainCharacter() ? PreExtraSubPlayerWeaponSkillPoint : PreExtraMainPlayerWeaponSkillPoint;
	//}
}

void FCacheStageEssentialDataToKeep::SetPostExtraMapLoadB2Player(class ABladeIIPlayer* CurrentActivePlayer)
{
	if (CurrentActivePlayer)
	{
		CurrentActivePlayer->Health = CurrentActivePlayer->AmIMainCharacter() ? PreExtraMainPlayerLastHP : PreExtraSubPlayerLastHP;
		CurrentActivePlayer->Armor = CurrentActivePlayer->AmIMainCharacter() ? PreExtraMainPlayerLastArmor : PreExtraSubPlayerLastArmor;
		CurrentActivePlayer->SetWeaponSkillPoint(CurrentActivePlayer->AmIMainCharacter() ? PreExtraMainPlayerWeaponSkillPoint : PreExtraSubPlayerWeaponSkillPoint);
	}
}

void FCacheStageEssentialDataToKeep::SetRepeatBattleStateStop()
{ // �ΰ��ӿ��� �ݺ����� stop �Ϸ��� ���⼭ ���� �� StageManager ���� �߰� ó�� �ʿ�.
	/*auto BattleDoc = UB2UIDocHelper::GetDocBattleStage();

	if (!BattleDoc)
		return;

	BattleDoc->SetRepeatBattleLoopAll(false);
	BattleDoc->SetRepeatBattleLoopOne(false);*/


	RepeatBattleState.bIsOn = false;
	RepeatBattleState.CurrentRepeatCount = 0;
	RepeatBattleState.bRepeatAll = false;
	RepeatBattleState.bRepeatOne = false;

	/*
	RepeatBattleState.MultiplyingFactor = 1;
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Stage reward manager
//

void FStageRewardManager::Initialize()
{
	TotalStageRewardGold = 0;
	DefeatBossRewardItems.Empty();
	DefeatBossPieceRewardItems.Empty();
	BossItemCurrentIndex = 0;

	SumCurrentRewardGold = 0;
	StageManager = NULL;
}

int32 FStageRewardManager::GetQTEGold() const
{
	return AB2StageManager::GetCacheStageKeepEssentialData().GetQTEBonus(AB2StageManager::GetCacheStageKeepEssentialData().GetRecentQTE());
}

int32 FStageRewardManager::GetRewardGold(float weight, float TotalMobWeight, bool InQTE)
{
	//int32 rtnValue = 0;
	//float LeftGoldWeight = TotalMobWeight;

	//BII_CHECK(StageManager);
	//if (StageManager == NULL)
	//	return rtnValue;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(StageManager));
	//BII_CHECK(B2GM);
	//if (weight > 0 && B2GM)
	//{
	//	AB2MonsterSpawnPool* CheckSpawnPool = B2GM->GetActiveSpawnPool();
	//	if(CheckSpawnPool)
	//	{//���� ��� ȹ�� ����ġ�� ��� ���Ѵ�.
	//		LeftGoldWeight += CheckSpawnPool->GetLeftGoldWeight(StageManager->GetCurrentClientStageId(), StageManager->GetStageDifficultyLevel());
	//	}

	//	for (auto* TreasureChest : B2GM->GetTreasureChestArray())
	//	{
	//		LeftGoldWeight += TreasureChest->RewardGoldWeight;
	//	}

	//	if (LeftGoldWeight > weight)
	//	{//���� ��差�� ������ ���� ����ġ �������� ������ ���ذ��� ���Ѵ�.
	//		int32 randBarometer = (TotalStageRewardGold - SumCurrentRewardGold) / LeftGoldWeight * weight;

	//		rtnValue = FMath::Max(1, (int32)(randBarometer * (1.f + FMath::FRandRange(-0.2f, 0.2f))));
	//		rtnValue = (rtnValue + SumCurrentRewardGold > TotalStageRewardGold) ? TotalStageRewardGold - SumCurrentRewardGold : rtnValue;

	//		SumCurrentRewardGold += rtnValue;
	//	}
	//	else
	//	{//���� �� ����ġ�� ���� ���� ����ġ���� ���� ��� ���� ��� ��� ��ȯ
	//		rtnValue = TotalStageRewardGold - SumCurrentRewardGold;

	//		SumCurrentRewardGold = TotalStageRewardGold;
	//	}
	//}

	//return InQTE ? ((rtnValue + GetQTEGold()) / DropCoinCountForQTE) : rtnValue;
	return 0.0;
}

const FB2Item& FStageRewardManager::GetDefeatBossRewardItem()
{
	static FB2Item EmptyItem;

	if (DefeatBossRewardItems.IsValidIndex(BossItemCurrentIndex))
	{
		return DefeatBossRewardItems[BossItemCurrentIndex++];
	}

	return EmptyItem;
}

const FB2Item& FStageRewardManager::GetDefeatBossPieceRewardItem()
{
	static FB2Item EmptyItem;

	for(auto DefeatBossPieceRewardItem : DefeatBossPieceRewardItems)
	{
		if (DefeatBossPieceRewardItems.IsValidIndex(BossPieceItemCurrentIndex))
		{
			return DefeatBossPieceRewardItems[BossPieceItemCurrentIndex];
		}
		BossPieceItemCurrentIndex++;
	}

	return EmptyItem;
}

void FStageRewardManager::RequestSpawnDropItem(const FDropItemInfo& DropItemInfo, ABladeIICharacter* DelaySpawnRefActor, const FVector& SpawnLocation)
{
//	if (!StageManager)
//	{
//		BII_CHECK(0);
//		return;
//	}
//
//	FActorSpawnParameters CommonSpawnParam;
//	//CommonSpawnParam.bDeferConstruction = true; // bDeferConstruction �� bDeferPostSpawnInitialize �� SpawnDropItem ���߿� �ڵ����� ���� ��.
//	CommonSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;		
//	
//	bool IsQTEDeath = DelaySpawnRefActor ? DelaySpawnRefActor->IsInQTEState() : false;
//
//	uint32 GoldWieght = IsQTEDeath ? DropCoinCountForQTE : DropItemInfo.RewardGoldWeight;
//	uint32 NumGoldCoin = GoldWieght; //���ٸ� �̾߱Ⱑ ������ weight 1�� 1����
//	uint32 NumDropItems = NumGoldCoin + (DropItemInfo.bCanDropItem ? 1 : 0);
//
//#define AVAILABLE_EDITOR_PREVIEW_SPAWNITEM 1
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//	//����Ʈ ������ �뵵 - SpawnPool�� ������ ���� �ƴ� ���.
//	bool bEditorPreview = false;
//	if (NumDropItems == 0)
//	{
//		NumDropItems = DropItemInfo.TestNumSpawnGold + (DropItemInfo.TestNumSpawnItemRefID != 0 ? 1 : 0);
//		NumGoldCoin = DropItemInfo.TestNumSpawnGold;
//		bEditorPreview = true;
//	}
//#endif 
//	float FirstItemYawAngle = FMath::FRandRange(0.f, 360.f);
//	float ModAngle = (360.f / FMath::Max(1.0f, (float)NumDropItems));
//
//	const float WeightStep = GoldWieght / (float)NumGoldCoin;
//	float LeftGoldWeight = GoldWieght;
//	for (uint32 CoinIdx = 0; CoinIdx < NumGoldCoin; ++CoinIdx, LeftGoldWeight -= WeightStep)
//	{
//		float MinAngle = CoinIdx * ModAngle + FirstItemYawAngle;
//		float MaxAngle = MinAngle + ModAngle;
//		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f); // ���⼱ Rotation �� ���Ѵ�. Location �� delay �� ���� ��� SpawnRefActor �� ������Ʈ �� ��ġ���� ������.
//
//		StageManager->DeferredSpawnDropGold(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? 10 : 
//#endif
//			// SpawnTimeDelay �� spawn ���� �л��� ���� �߰� ������ �� ���� �ִ�.
//			GetRewardGold(WeightStep, LeftGoldWeight, IsQTEDeath), CommonSpawnParam, DelaySpawnRefActor, SpawnLocation, SpawnRotation, DropItemInfo.GoldSpawnDelay, IsQTEDeath);
//	}
//
//	if (DropItemInfo.bCanDropItem
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//		|| (bEditorPreview && DropItemInfo.TestNumSpawnItemRefID != 0)
//#endif
//		)
//	{
//		float MinAngle = GoldWieght * ModAngle + FirstItemYawAngle;
//		float MaxAngle = MinAngle + ModAngle;
//		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f); // ���⼱ Rotation �� ���Ѵ�. Location �� delay �� ���� ��� SpawnRefActor �� ������Ʈ �� ��ġ���� ������.
//
//		StageManager->DeferredSpawnDropItem(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? DropItemInfo.TestNumSpawnItemRefID :
//#endif
//			// SpawnTimeDelay �� spawn ���� �л��� ���� �߰� ������ �� ���� �ִ�.
//			GetDefeatBossRewardItem().ItemRefID, CommonSpawnParam, DelaySpawnRefActor, SpawnLocation, SpawnRotation, DropItemInfo.RewardSpawnDelay);
//		StageManager->DeferredSpawnDropItem(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? DropItemInfo.TestNumSpawnItemRefID :
//#endif
//			// SpawnTimeDelay �� spawn ���� �л��� ���� �߰� ������ �� ���� �ִ�.
//			GetDefeatBossPieceRewardItem().ItemRefID, CommonSpawnParam, DelaySpawnRefActor, SpawnLocation, SpawnRotation, DropItemInfo.RewardSpawnDelay);
//	}
}

UB2ItemInfo* FStageRewardManager::GetItemInfoTable()
{
	return StaticFindItemInfo();
}

void FStageRewardManager::AcquireGold(uint32 AcquireGold)
{
	//auto*			PlayerController	= Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(StageManager));
	//FMyAssetStore*	MyAssetStore		= PlayerController ? &PlayerController->GetMyAssetStore() : NULL;
	//BII_CHECK(MyAssetStore);

	//MyAssetStore->AddGold(AcquireGold);
}

void FStageRewardManager::AcquireItem(int32 AcquireItemRefID)
{

}

void FStageRewardManager::PreloadAllRewardItemInfo()
{
	UB2ItemInfo* ItemInfoTable = GetItemInfoTable();
	if (ItemInfoTable)
	{
		TArray<int32> PreloadIDs;

		PreloadIDs.Add(FItemRefIDHelper::GetGoodsID_Gold());

		for (auto& BossReward: DefeatBossRewardItems)
		{
			PreloadIDs.Add(BossReward.ItemRefID);
		}
		for (auto& StageClearReward : StageClearRewardItems)
		{
			PreloadIDs.Add(StageClearReward.ItemRefID);
		}
		for (auto& BossPieceReward : DefeatBossPieceRewardItems)
		{
			PreloadIDs.Add(BossPieceReward.ItemRefID);
		}

		// �̰� ID ���� ���� �ִ� �����͸� �� �ε��ϴ� ������ ItemInfo �ε��� ���� �ڷδ� �׸� ����Ǵ� ����� �ƴѵ�
		// Reward �������̶�� ��ǻ� ��κ��� ���ҽ����� �ʿ��ϱ� �� �� ����. �ϴ� ����. �� ���� �ȴ� ������ PreloadClassAssets �� ���ֺ��� ������ �� �غ� ����.
		ItemInfoTable->PreloadClassAssets(PreloadIDs);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Scenario Dialog Manager
//

void FB2ScenarioDialogManager::SetStageManager(AB2StageManager* StageManager)
{
	if (CachedStageManager)
	{
		CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
	}

	CachedStageManager = StageManager;

	if (CachedStageManager)
	{
		CachedStageManager->StopCurrentDialogSound();
	}
}

void FB2ScenarioDialogManager::Initialize()
{
	DialogInfoInstace = NULL;
	CurrentDialogLineIndex = 0;

	if (CachedStageManager)
	{
		CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
	}	
}

UB2DialogCharacterInfoSet* FB2ScenarioDialogManager::GetDialogCharacterInfoSet(UObject* WorldContextObject)
{
	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	//return B2GI;// ? B2GI->GetDialogCharacterInfoTable() : StaticFindDialogCharacterInfoTable();
	return NULL;
}

FDialogCharacterInfo* FB2ScenarioDialogManager::GetDialogCharacterInfo(FName ToGetCharacterCodeName, UObject* WorldContextObject)
{
	UB2DialogCharacterInfoSet* DialogCharacterInfoTable = GetDialogCharacterInfoSet(WorldContextObject);
	return DialogCharacterInfoTable ? DialogCharacterInfoTable->GetDialogCharacterInfo(ToGetCharacterCodeName, WorldContextObject) : NULL;
}

USoundCue* FB2ScenarioDialogManager::GetDialogSound(FName CharacterCodeName, FName SoundCodeName, UObject* WorldContextObject)
{
	//UB2DialogSoundInfoSet* DialogSoundInfoTable = StaticFindDialogSoundInfoTable();
	//return DialogSoundInfoTable ? DialogSoundInfoTable->GetInfoSound(CharacterCodeName, SoundCodeName, WorldContextObject) : NULL;
	return NULL;
}

void FB2ScenarioDialogManager::SetScenarioDialog(FName ToSetDialogCodeName, bool bMaintainBattleUI)
{
/*	auto* DialogInfoTable =  StaticFindDialogInfoTable();
	if (DialogInfoTable)
	{
		SetScenarioDialog(DialogInfoTable->GetDialogInfo(ToSetDialogCodeName), bMaintainBattleUI);
	}	*/	
}

void FB2ScenarioDialogManager::SetScenarioDialog(const FDialogInfoInstance* ToSetDialogInfoInstance, bool bMaintainBattleUI)
{
	//if (ToSetDialogInfoInstance == NULL)
	//	return;

	//ForceSkip();

	//DialogInfoInstace = ToSetDialogInfoInstance;
	//
	//if (DialogInfoInstace->bNeedPauseGame)
	//{
	//	UGameplayStatics::SetGamePaused(CachedStageManager, true);
	//}

	//SetDialogLine(CurrentDialogLineIndex);
	//
	//UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	//if (UIMgr)
	//{
	//	UIMgr->OpenUI(UIFName::ScenarioDialog);
	//	if (bMaintainBattleUI) // �⺻������ ���� widget ������ bHideBattleRelatedWidgets �� ���� �ǵ� ���� �Ѱ� ���� ��츦 ����..
	//	{
	//		UIMgr->HideBattleRelatedWidgets(false);
	//	}
	//}

	//UB2UIDocScenarioDialog* DocSDG = UB2UIDocHelper::GetDocScenarioDialog();
	//if (DocSDG)
	//{
	//	// bLastBattleUIHidden ���� ���� ��ȭâ ��ġ�� �� �ٲ� ��.
	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(CachedStageManager));
	//	if (B2PC && B2PC->IsInCinematicMode())
	//	{
	//		DocSDG->SetbLastBattleUIHidden(B2PC->IsHUDHidingCinematicMode()); // CinematicMode ������ �켱�� ��.
	//	}
	//	else
	//	{
	//		DocSDG->SetbLastBattleUIHidden(!bMaintainBattleUI);
	//	}
	//}
}

void FB2ScenarioDialogManager::SetDialogLine(int32 NextDialogLineIndex)
{
	if (DialogInfoInstace == NULL || CachedStageManager == NULL)
	{
		BII_CHECK(0);
		return;
	}

	if (!DialogInfoInstace->DialogSequence.IsValidIndex(NextDialogLineIndex))
	{
		// ����� INDEX_NONE
		// �ݵ�� ForceSkip() ������ ȣ�� �Ǿ�� �� - DialogInstance�� nullptr ��
		OnUpdatedDialogClass<FName, int32>::GetInstance().Signal(DialogInfoInstace->DialogCodeName, INDEX_NONE);
		CloseDialogClass<FName>::GetInstance().Signal(DialogInfoInstace->DialogCodeName);
		ForceSkip();
		return;
	}		

	auto* UIDocDialog = UB2UIDocHelper::GetDocScenarioDialog();
	if (UIDocDialog == NULL)
		return;
	
	CurrentDialogLineIndex = NextDialogLineIndex;
		
	const FDialogLine& DialogLineInfo = DialogInfoInstace->DialogSequence[CurrentDialogLineIndex];
	// DialogCharacterInfo �� Portrait material �� ��� ������ �װ� DialogCharacterInfo ���� ���� �������� �ʰ� InfoSet ���̺��� ���� �������̽��� ������.
	FDialogCharacterInfo* CharacterInfo = GetDialogCharacterInfo(DialogLineInfo.SpeakerCodeName, CachedStageManager);

	UB2DialogCharacterInfoSet* DialogCharacterInfoTable = GetDialogCharacterInfoSet(CachedStageManager);
	if (DialogCharacterInfoTable)
	{
		UMaterialInstance* LoadedPortraitMat = DialogCharacterInfoTable->GetDialogCharacterPortraitMtrl(DialogLineInfo.SpeakerCodeName, DialogLineInfo.PortraitType, CachedStageManager);
		if (LoadedPortraitMat)
		{
			UIDocDialog->SetSpeakerPortrait(LoadedPortraitMat);
		}
	}

	USoundCue* DialogSound = GetDialogSound(DialogLineInfo.SpeakerCodeName, DialogLineInfo.DialogSoundCodeName, CachedStageManager);
	if (DialogSound)
	{
		CachedStageManager->PlayDialogSound(DialogSound);
	}
	UIDocDialog->SetThisDialogShowingTime(DialogLineInfo.ShowingTime); // Dialog UI �ʿ��� ����.. Ÿ�̸Ӵ� ���⼭ ������. SetDialogText ���� �̰ź���..
	UIDocDialog->SetDialogText(BladeIIGetLOCText(B2LOC_CAT_DIALOGTEXT, DialogLineInfo.DialogLineCodeName.ToString()));
	
	UIDocDialog->SetShowNextButton(
		// ���� ���� ���� ���� NextButton �� ���δ�. ������ ��ü ������ư�̳� ���׸� NextButton �̳� ���� ��ü�� ������ ��..
		DialogInfoInstace->DialogSequence.Num() > 1 && DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex+1)
		);
	
	CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
	CachedStageManager->GetWorldTimerManager().SetTimer(GoToNextLineTimerHandle, CachedStageManager, &AB2StageManager::NextDialogLineWrapper, DialogLineInfo.ShowingTime, false);
		
	UIDocDialog->SetSpeakerName(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, CharacterInfo ? CharacterInfo->CharacterNameKey.ToString() : TEXT("")));
	
	OnUpdatedDialogClass<FName, int32>::GetInstance().Signal(DialogInfoInstace->DialogCodeName, CurrentDialogLineIndex);
}

void AB2StageManager::NextDialogLineWrapper()
{
	ScenarioDialogManager.NextDialogLine();
}

void AB2StageManager::NextDialogLineImmediateWrapper()
{
	ScenarioDialogManager.NextDialogLineImmediate();
}

void AB2StageManager::PauseDialog(bool Pause)
{
	//UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	//UB2UIScenarioDialog* DialogUI = UIMgr ? UIMgr->GetUI<UB2UIScenarioDialog>(UIFName::ScenarioDialog) : NULL;
	//if (DialogUI)
	//{
	//	DialogUI->OnPause(Pause);
	//	if (CurrentPlayingDialogSound)
	//	{
	//		if (Pause)
	//		{
	//			CurrentPlayingDialogSound->SetPaused(true);
	//			bPausedDialog = true;
	//		}
	//		else if (bPausedDialog) // ������ Pause������ ��  - Pause ���� ����� Dialog Sound�� Pause�� ������� ��� ����ǹǷ� ���� Resume��ų �ʿ� ����
	//		{
	//			CurrentPlayingDialogSound->SetPaused(false);
	//			bPausedDialog = false;
	//		}
	//	}
	//}
}

void FB2ScenarioDialogManager::NextDialogLine()
{
	//if (CachedStageManager)
	//{
	//	CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
	//}

	//bool bNextDialogDelayed = false;
	//// ������ ���� ������ �ִ� ���, �ٷ� �Ѿ�� �� Delay �� üũ
	//if (CachedStageManager && DialogInfoInstace 
	//	&& DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex)
	//	&& DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex + 1))
	//{
	//	const FDialogLine& CurrDialogSequence = DialogInfoInstace->DialogSequence[CurrentDialogLineIndex];
	//	if (CurrDialogSequence.DelayToNextLine > 0.0f)
	//	{ // GoToNextLineTimerHandle ��������
	//		CachedStageManager->GetWorldTimerManager().SetTimer(GoToNextLineTimerHandle, CachedStageManager, &AB2StageManager::NextDialogLineImmediateWrapper, CurrDialogSequence.DelayToNextLine, false);

	//		// ForceSkip ���� �� �ϸ� �ȵǰ� ��� ����⸸ ����.
	//		UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	//		UB2UIScenarioDialog* DialogUI = UIMgr ? UIMgr->GetUI<UB2UIScenarioDialog>(UIFName::ScenarioDialog) : NULL;
	//		if (DialogUI)
	//		{
	//			DialogUI->ForceHide();
	//		}

	//		bNextDialogDelayed = true;
	//	}
	//}
	//
	//if (!bNextDialogDelayed){
	//	NextDialogLineImmediate(); // ���� ������ ���� ����� skip ó���� �� ��.
	//}
}

void FB2ScenarioDialogManager::NextDialogLineImmediate()
{
//	if (CachedStageManager)
//	{
//		CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
//	}
//
//	// NextDialogLine ���� delay ���״� ���. hide ������.
//	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//	UB2UIScenarioDialog* DialogUI = UIMgr ? UIMgr->GetUI<UB2UIScenarioDialog>(UIFName::ScenarioDialog) : NULL;
//#if !UE_BUILD_SHIPPING
//	extern bool gbHACKInGameUIHiddenByDevCommand; // UI ����� ġƮ ��µ� ���� �� ������ �̰͵� �����..
//#endif
//	if (DialogUI
//#if !UE_BUILD_SHIPPING
//		&& !gbHACKInGameUIHiddenByDevCommand
//#endif
//		)
//	{
//		DialogUI->RestoreFromForceHidden();
//	}
//
//	SetDialogLine(CurrentDialogLineIndex + 1);
}

void AB2StageManager::SkipDialogWrapper()
{
	ScenarioDialogManager.ForceSkip();
}

void AB2StageManager::PlayDialogSound(USoundCue* InSoundCue)
{
	StopCurrentDialogSound(); // �ߴܺ���
	if (InSoundCue)
	{
		CurrentPlayingDialogSound = UGameplayStatics::SpawnSound2D(this, InSoundCue);
	}
}

void AB2StageManager::StopCurrentDialogSound()
{
	if (CurrentPlayingDialogSound)
	{
		if (!IsValid(CurrentPlayingDialogSound))
		{
			CurrentPlayingDialogSound->Stop();
			CurrentPlayingDialogSound->ConditionalBeginDestroy();
		}
	}
	CurrentPlayingDialogSound = NULL;
}

void AB2StageManager::SetGameMode(class AB2StageGameModeBase* InGameMode)
{
	CachedGameMode = InGameMode;
}

void AB2StageManager::ResetPlayTime()
{
	LastUpdatedPlayTime = 0;
}

bool AB2StageManager::IsEndStage(int32 CurrentStageID)
{
	UB2StageInfo* StageInfo = StaticFindStageInfo();
	if (StageInfo == nullptr)
		return true;

	int32 MaxStageNum = CurrentStageID <= (StageInfo->GetStartAdventureModeChapterNum() * 10) ? StageInfo->GetStartAdventureModeChapterNum() : StageInfo->GetEndAdventureModeChapterNum();
	MaxStageNum *= 10;

	return CurrentStageID == MaxStageNum;
}

FServerStageID AB2StageManager::GetNextServerStageID(int32 CurrentStageID, EStageDifficulty CurStageDifficulty)
{
	//if (IsEndStage(CurrentStageID))
		return FServerStageID();

	//return BladeIIGameImpl::GetStageDataStore().GetServerStageID(CurrentStageID + 1, CurStageDifficulty);
}

void AB2StageManager::DeferredSpawnDropGold(int32 Amount, const struct FActorSpawnParameters SpawnParam, ABladeIICharacter* DelaySpawnLocRefActor, const FVector& SpawnLocation, const FRotator SpawnRotation, float SpawnTimeDelay, bool InQTE)
{	
#if !UE_BUILD_SHIPPING
	// ����̽� �׽�Ʈ ���� ���� Gold spawn ������ ������ �� �ְ� ��.

	float FinalDropGoldSpawnRate = FMath::Clamp(GCheat_DropGoldSpawnRate, 0.0f, 1.0f);
	
	float ThisSpawnChanceInv = FMath::FRandRange(0.0f, 1.0f);
	if (ThisSpawnChanceInv > FinalDropGoldSpawnRate){
		return;
	}
#endif

	// �ٷ� Spawn �̶�� ����� �Ѿ�� SpawnLocation/Rotation ���� �������� delay �Ǵ� ���� DelaySpawnLocRefActor �� ��ġ�� ����ϰ� �� ��.

	if (SpawnTimeDelay > 0.0f)
	{
		FDelayedSpawnDropItemInfo NewDelayInfo;
		GetWorldTimerManager().SetTimer(NewDelayInfo.DelayTH,
			FTimerDelegate::CreateUObject(this, &AB2StageManager::SpawnDropGold, Amount, SpawnParam, SpawnLocation, SpawnRotation, DelayedDropSpawnSerialNumber, false),
			SpawnTimeDelay, false);
		
		NewDelayInfo.DelaySpawnLocRefActor = DelaySpawnLocRefActor;
		DelayedDropSpawnInfoMap.Add(DelayedDropSpawnSerialNumber, NewDelayInfo);
		++DelayedDropSpawnSerialNumber; // �Ϸù�ȣ�� ��� ����
	}
	else
	{ // �ٷ� spawn �� ������ SpawnTimerMapKey ���� ������
		SpawnDropGold(Amount, SpawnParam, SpawnLocation, SpawnRotation, -1, InQTE);
	}
}

void AB2StageManager::DeferredSpawnDropItem(int32 ItemRefID, const struct FActorSpawnParameters SpawnParam, ABladeIICharacter* DelaySpawnLocRefActor, const FVector& SpawnLocation, const FRotator SpawnRotation, float SpawnTimeDelay)
{
	// �ٷ� Spawn �̶�� ����� �Ѿ�� SpawnLocation/Rotation ���� �������� delay �Ǵ� ���� DelaySpawnLocRefActor �� ��ġ�� ����ϰ� �� ��.

	if (SpawnTimeDelay > 0.0f)
	{		
		FDelayedSpawnDropItemInfo NewDelayInfo;
		GetWorldTimerManager().SetTimer(NewDelayInfo.DelayTH,
			FTimerDelegate::CreateUObject(this, &AB2StageManager::SpawnDropItem, ItemRefID, SpawnParam, SpawnLocation, SpawnRotation, DelayedDropSpawnSerialNumber),
			SpawnTimeDelay, false);

		NewDelayInfo.DelaySpawnLocRefActor = DelaySpawnLocRefActor;
		DelayedDropSpawnInfoMap.Add(DelayedDropSpawnSerialNumber, NewDelayInfo);
		++DelayedDropSpawnSerialNumber; // �Ϸù�ȣ�� ��� ����
	}
	else
	{ // �ٷ� spawn �� ������ SpawnTimerMapKey ���� ������
		SpawnDropItem(ItemRefID, SpawnParam, SpawnLocation, SpawnRotation, -1);
	}
}

void AB2StageManager::SpawnDropGold(int32 Amount, const FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey, bool InQTE)
{
	//FTransform FinalSpawnTransform = SpawnDropDelayHandlingCommon(InitialSpawnLocation, InitialSpawnRotation, DelaySpawnInfoMapKey); // Delay ref actor �� �ִ� ��� �ش� ������ ��ġ�� ���.

	//if (Amount == 0)
	//	return;

	//UB2ItemInfo* ItemDataTable = StaticFindItemInfo();
	//if (!ItemDataTable)
	//	return;
	//
	//const float QTEStayTime = 1.0f;
	//float StayTime = InQTE ? QTEStayTime : 0.0f;

	//AB2DropItem* NewDropItem = ItemDataTable->SpawnSingleDropItem(GetWorld(), FItemRefIDHelper::GetGoodsID_Gold(), FinalSpawnTransform, SpawnParam, StayTime);
	//BII_CHECK(NewDropItem && NewDropItem->IsValidObj());

	//if (NewDropItem)
	//{ // ���� Consumables ����� �ƴ����� ConsumingAmount �� ���� ����..
	//	NewDropItem->SignificantData.ConsumingAmount = Amount;
	//}
}

void AB2StageManager::SpawnDropItem(int32 ItemRefID, const FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey)
{
	//FTransform FinalSpawnTransform = SpawnDropDelayHandlingCommon(InitialSpawnLocation, InitialSpawnRotation, DelaySpawnInfoMapKey); // Delay ref actor �� �ִ� ��� �ش� ������ ��ġ�� ���.

	//if (ItemRefID <= 0) // ������ ������ ������ ���� ��Ȳ��. 
	//	return;
	//	
	//UB2ItemInfo* ItemDataTable = StaticFindItemInfo();
	//if (!ItemDataTable)
	//	return;

	//AB2DropItem* NewDropItem = ItemDataTable->SpawnSingleDropItem(GetWorld(), ItemRefID, FinalSpawnTransform, SpawnParam);
	//BII_CHECK(NewDropItem && NewDropItem->IsValidObj());
}

FTransform AB2StageManager::SpawnDropDelayHandlingCommon(FVector InSpawnLocation, FRotator InSpawnRotation, int32 DelaySpawnInfoMapKey)
{
	FTransform RetSpawnTransform(InSpawnRotation, InSpawnLocation);
	//FDelayedSpawnDropItemInfo* ThisInfoForDeferCall = DelayedDropSpawnInfoMap.Find(DelaySpawnInfoMapKey);
	//if (ThisInfoForDeferCall)
	//{ // �̰��� Ÿ�̸Ӹ� ���� �ҷ��� ��� Ÿ�̸� Ŭ����
	//	if (ThisInfoForDeferCall->DelayTH.IsValid())
	//	{
	//		GetWorldTimerManager().ClearTimer(ThisInfoForDeferCall->DelayTH);
	//	}

	//	if (ThisInfoForDeferCall->DelaySpawnLocRefActor)
	//	{
	//		// DelaySpawnLocRefActor �� ��� �� LifeSpan �� Delay ���� ª�ٸ� Valid ���� ���� ���̴�. 
	//		// NotifyForgetACharacter ���� Ȥ�� DelaySpawnLocRefActor �� ���� �ʾҴ��� �˻��ؼ� ������ �ʿ䰡 ����.
	//		checkSlow(ThisInfoForDeferCall->DelaySpawnLocRefActor->IsValidObj());
	//		// DelaySpawnLocRefActor �� �� ��ġ�� ����.
	//		RetSpawnTransform.SetTranslation(ThisInfoForDeferCall->DelaySpawnLocRefActor->GetActorLocation());
	//	}

	//	DelayedDropSpawnInfoMap.Remove(DelaySpawnInfoMapKey);
	//}
	return RetSpawnTransform;
}

void AB2StageManager::CancelOrStopRepeatBattle()
{
	StaticCacheStageData.SetRepeatBattleStateStop();

	/*
	if (CachedGameMode && CachedGameMode->GetMaintainedLocalPlayerAutoState()){
		// ���⼱ �Ͻ������� ���� �� �ƴ϶� �⺻���� auto ���¸� ��.
		TogglePlayerAutoStateClass<>::GetInstance().Signal();
	}
	*/
	SyncDataToUIDoc(); // �� �������� ���� �����Ͱ� ���õǱ� ������..
}
void AB2StageManager::RequestFailStageDefeat()
{
	RequestFailStage(EStageFailReason::Defeat);
}

void AB2StageManager::RequestFailStageGiveUP()
{
	RequestFailStage(EStageFailReason::GiveUp);
}

bool AB2StageManager::HasStageBuff(EStageBuffType InType) const
{
	for (EStageBuffType ThisType : StaticCacheStageData.ServerSelectedStageBuff)
	{
		if (ThisType == InType)
		{
			return true;
		}
	}
	return false;
}

void FB2ScenarioDialogManager::ForceSkip()
{
	//if (DialogInfoInstace && DialogInfoInstace->bNeedPauseGame)
	//{
	//	UGameplayStatics::SetGamePaused(CachedStageManager, false);
	//}

	//UB2UIManager::GetInstance()->CloseUI(UIFName::ScenarioDialog);

	Initialize();
}

FText GetStageClearGradeConditonText(FServerStageID InServerStageId, int32 InWantedGrade)
{
	// ������������ Ư�� Star ����� �ޱ� ���� �ð� ���� �ȳ� �ؽ�Ʈ ����. UI ���� ���.
	// InWantedGrade �� STAGE_CLEAR_GRADE_STAR_1 ~ 3 ����.
	
	//const int32 TimeLimitInSec = BladeIIGameImpl::GetStageDataStore().GetTimeLimitForStageClearGrade(InServerStageId, InWantedGrade);

	int32 FormattedHour = 0;
	int32 FormattedMin = 0;
	int32 FormattedSec = 0;
	//GetTimeFormatFromRawSecond(TimeLimitInSec, FormattedHour, FormattedMin, FormattedSec);
	// �� ������� FormattedMin �� Sec �� ����Ѵ�. �̰� �� �ð� ���� �̷��� ������ �����̰���

	// ��, ��, ��� ���� �̷��� �־���.
	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStage_ClearGradeCondition")),
		FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)), FText::FromString(FString::FromInt(InWantedGrade)));
}


FText GetStageClearGradeInfoText(FServerStageID InServerStageId, int32 InWantedGrade, bool bIsClear)
{
	//const int32 TimeLimitInSec = BladeIIGameImpl::GetStageDataStore().GetTimeLimitForStageClearGrade(InServerStageId, InWantedGrade);

	int32 FormattedHour = 0;
	int32 FormattedMin = 0;
	int32 FormattedSec = 0;
	//GetTimeFormatFromRawSecond(TimeLimitInSec, FormattedHour, FormattedMin, FormattedSec);
	// �� ������� FormattedMin �� Sec �� ����Ѵ�. �̰� �� �ð� ���� �̷��� ������ �����̰���

	// ��, ��, ��� ���� �̷��� �־���.
	if(bIsClear)
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_Grade_Clear_2")),
			FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)));
	else
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_Grade_2")), 
			FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)));
}

void AB2StageManager::OnTalkDialogStart(const int32 GroupID, const bool IsClear)
{
	//// ���� �� ���� BeginPlay Ÿ�̹��̸� ���� TalkDialog ������ �� async �ε� ������ Ư���� ������ �߱����� �ʰ�����
	//// ���ο��� �� ƽ ���Ŀ� TalkDialog �� ���̴� ���ҽ��� �ε��ϴ� �� �־ �ᱹ ū ���ҽ� flush ������ �� ���ƾ� �Ѵ�.
	//if (CachedGameMode)
	//{
	//	CachedGameMode->GameContextAsyncFlush();
	//}

	//UB2UIManager::GetInstance()->OpenUI(UIFName::TalkDialog, true);
	//UB2UIManager::GetInstance()->GetUI<UB2UITalkDialog>(UIFName::TalkDialog)->SetTalkDialog(GroupID, IsClear);
}

void AB2StageManager::OnTalkDialogEnd(const int32 GroupID, const bool IsClear)
{
	//if (IsClear)
	//{
	//	BeginStageClearScene_Phase1();
	//}
	//else
	//{
	//	UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//}
}

bool AB2StageManager::GetIsShowTalkDialog()
{
	//if (GetB2GameModeType(this) == EB2GameMode::Scenario)
	//{
	//	// ���丮 ��� ��ŵ 
	//	UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
	//	if (DocCon)
	//	{
	//		return !DocCon->GetbUserSelectedSkipStory();
	//	}
	//}
	return false;
}

int32 AB2StageManager::IsGoldBonusStage()
{
	//if (FSingleStageInfoData* pStageInfoData = GetCurrentStageInfoData())
	//{
	//	return pStageInfoData->bIsGoldBonusStage;
	//}
	//else
	//{
		return 0;
	//}
}

int32 AB2StageManager::IsBossStage()
{
	//if (FSingleStageInfoData* pStageInfoData = GetCurrentStageInfoData())
	//{
	//	return pStageInfoData->bIsBossStage;
	//}
	//else
	//{
		return 0;
	//}
}

void AB2StageManager::CheckBeginStageClearScene_Phase1()
{
	//bool HasTalkDialog = false;
	//if (GetIsShowTalkDialog() == true)
	//{
	//	AB2MonsterSpawnPool* CheckSpawnPool = CachedGameMode->GetActiveSpawnPool();
	//	if (CheckSpawnPool)
	//	{
	//		int32 GroupID = 0;
	//		if (CheckSpawnPool->TryGetCompleteTalkSetting(GroupID))
	//		{
	//			HasTalkDialog = true;
	//			UB2UIManager::GetInstance()->HideBattleRelatedWidgets(true);
	//			StageEventTalkStartClass<int32, bool>::GetInstance().Signal(GroupID, true);
	//		}
	//	}
	//}

	//if (HasTalkDialog == false)
	//	BeginStageClearScene_Phase1();
}
