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

	StagePlayTimeAutoUpdateInterval = 0.2f; // 최종적인 정확도에 영향을 미치지는 않지만 전투 UI 표시된 것과 결과 UI 표시에서의 차이를 가급적 줄일 필요가 있어서 작은 값으로..

	CurrentPlayingDialogSound = NULL;
	
	CustomaizeBGM = false;
}

void AB2StageManager::PreBeginPlay()
{	
//	// 다른 Actor 들의 BeginPlay 가 불리기 전에 처리되어야 할 것들을 여기에 쑤셔넣어 보자.
//#if WITH_EDITOR
//	if (GEditor && GEditor->bIsPlayWorldQueued) // PlayInEditor 라도 로비를 통해 온 경우는 여기를 통과하지 않을 것.
//		StaticCacheStageData.Initialize();
//#endif
//	
//	ScenarioDialogManager.SetStageManager(this);
//
//	GetStageRewardManager().SetStageManager(this);
//	//[@AKI, 180213] 이거는 세팅 안되게 일단 이거는 빠져야 할듯(이게 StageId를바꾸나....)
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!Utilities::FConnectionState::GetInstance().IsConnected()
//		// FConnectionState 만 검사한다면 한번이라도 로비맵 열고 접속한 이후에는 서버 접속한 걸로 처리되어 에디터 설정을 사용할 수가 없음. 
//		// 에디터 및 그냥 Open 을 통한 플레이에서의 편의상 standalone 모드 제공
//		|| DevOnlyIsStandalonePlay()
//		)
//	{
//		StaticCacheStageData.SetStandaloneStageData(this);
//	}
//#endif
//
//	RefreshStagePlayTimeCheck(); // ExtraMap 상관없이 카운트는 시작
//
//	// 기타 다른 데이터들..
//	
//	SyncDataToUIDoc();
//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//	{
//		UB2UIDocHero* UIDocClass = UB2UIDocHelper::GetDocHero(PCI);
//		if (UIDocClass)
//		{ // 스테이지 클리어할 때 (레벨업하면) 세팅되는 데이터. 시작할 때 리셋.
//			UIDocClass->SetLastAcquiredSkillPoint(0);
//		}
//	}

	SubscribeEvents(); // BeginPlay 시점에 불리는 이벤트가 있을 테니 이 시점에 먼저 등록해 놓아야.
}

// Called when the game starts or when spawned
void AB2StageManager::BeginPlay()
{
	Super::BeginPlay();
	
	StageBeginHandling();

	BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = -1; // 이 시점에서 더 이상 필요하지 않을 뿐더러 남아 있으면 에디터 플레이에 방해가 됨
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

	//// Note: 다른 actor 들의 BeginPlay 이전에 처리가 필요한 부분은 ABladeIIGameMode::PreBeginPlay 를 참고.
	//// 이것이 불리는 시점은 단순히 actor 들의 BeginPlay 가 처리되는 시점임. 선후관계는 정의되지 않음.

	//SyncDataToUIDoc(); // 또 한번 해주지 머.

	//if (StaticCacheStageData.bPlayInExtraLevel) // ExtraLevel 로딩 시의 처리. SetReadyPlayingExtraMap 의 반대
	//{
	//	//ABladeIIGameMode::HideHUDCenterMessage(); // Extralevel 로딩 대기 시작하면서 켰던 센터 메시지 끄기. 혹시 그 사이에 다른 센터 메시지가 필요해진다면..?
	//	// ABladeIIGameMode::HideHUDCenterMessage 가 어차피 안먹히는데 레벨 변경 후라 그쪽에서 캐싱해 놓은 레퍼런스가 의미가 없기 때문이다. 어쩔 수 없지.
	//	UB2UIManager::GetInstance()->CloseAllMsgPopups();
	//	RetrieveDataForExtraMap();
	//}

	//PlayStageBGM();

	//UpdateStagePlayTime(false); // UI 표시를 위해 처음 한번 확실히 불러줄 필요가 있음.
	//GetWorldTimerManager().SetTimer(AutoUpdateStagePlayTimeTH, this, &AB2StageManager::AutoUpdateStagePlayTimeCB, StagePlayTimeAutoUpdateInterval, false);

	//StageBeginClass<>::GetInstance().Signal(); // UI 쪽 신호.
}

void AB2StageManager::SyncDataToUIDoc()
{ // UI 에서 쓸 데이터들 세팅.
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
{ // 서버에 붙지 않은 토탈 standalone 플레이 여부
#if WITH_EDITOR
	if (IsStandalonePIE()){
		return true;
	}
#endif

	if (FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode()) { // 이걸 세팅하는 게 젤 속편할 듯..
		return true;
	}

	if (GEngine)
	{
		FWorldContext* TheWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (TheWorldContext)
		{ // Open 명령에 Standalone 인자. 예를 들어 Open LV01_02?Standalone?StageNum=2 이런 식으로
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
	//		SpawnedMob->PreventMovementByStageEvent(); // StopOrRestoreAllWorldMobs 호출 시점 이후에 spawn 된 것들도 처리.
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
	CheckStageClear(); // SpawnPool 종료는 대체로 StageClear 로 이어지나 추가 조건이 필요할 수 있음.
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
	// DestroyedPrevRef 는 NULL 이 아닌 경우라도 IsValidObj 하지 않을 것임.
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
	if (!bCurrentLevelCleared) // 레벨 종료 후에 올 수 있으므로 업데이트 하지 않도록 주의.
	{
		// 아래 UpdateStagePlayTime 에 true 를 넣어주면 연출 장면 재생되는 동안 잠시 시간 카운팅이 중단될 것.
		// 지금은 연출 도중에도 시간 카운팅 하게 되었음.
		UpdateStagePlayTime(false);
	}
}
void AB2StageManager::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{ // 이 Notify 가 꼭 전체 연출 장면이 플레이되었음을 뜻하지는 않는다. 중간에 중단했더라도 이렇게 오게 될 것.
	if (!bCurrentLevelCleared) // 레벨 종료 후에 올 수 있으므로 주의..
	{
		RefreshStagePlayTimeCheck();
	}
}

void AB2StageManager::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{

}

void AB2StageManager::PreloadAdditionalNecessaryInfo()
{
	// TAsset 로딩하는 InfoAsset 들이 대상임. 기본적으로 AB2StageGameMode::PreloadAnyNecessaryInfo 에서 처리하는 이외의 것들

	GetStageRewardManager().PreloadAllRewardItemInfo(); // 골드랑.. 여타 보스몹 잡았을 때의 보상 아이템 등 로딩.
}

void AB2StageManager::CheckStageClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::CheckStageClear"));
//
//	bool bSpawnPoolDone = true;
//
//	// 모든 SpawnPool 들이 끝났는지 체크 -> 일단은 SpawnPool 하나 가정이지만 설치 가능성은 있으니 남겨둠.
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
//	// 기타 조건들 생기는 경우..
//	//
//
//
//	bCurrentLevelCleared = bSpawnPoolDone; // 다른 조건은 필요에 따라 추가.
//
//	if (bCurrentLevelCleared) // 레벨이 끝난 건데 Stage 클리어가 되기 위해서는 아직 좀 더 체크할 게 있음.
//	{		
//		// 플레이 자체는 끝났으므로 카운트 중단하면서 PlayTime 갱신. ExtraMap 이 있다면 지금까지 카운트된 PlayTime 은 전달될 것. 
//		UpdateStagePlayTime(true); // 이미 연출 장면이 시작되면서 중단된 상태일 수 있다. (아마 그게 대부분일듯?)
//
//#if WITH_EDITOR
//		// 에디터에서 ExtraMap 단독 플레이할 때에 자기자신을 다시 로딩하지 않고 스테이지 종료 처리를 하도록 한다.
//		bool bEDITOR_IsExtraMap = false;
//		if (GIsEditor && StaticCacheStageData.bHasExtraLevel && StaticCacheStageData.CachedCurrentStageInfoData)
//		{
//			UPackage* OuterMostPackage = GetWorld()->GetOutermost();
//			// 원본 FIleName 과 ExtraMap 이 같은지 검사. FileName 은 경로를 포함하는데 ExtraMap 은 그렇지 않을 것이므로 맨 뒷부분만 체크.
//			// 물론 예상 외의 네이밍으로 인해 엉뚱하게 ExtraMap 처리가 될 수도 있겠지만 에디터 이슈이고 문제가 된다면 금방 알게 될 것임
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
//			// ExtraMap 을 로딩함에 따라 다음 맵에 전달해 줄 데이터를 세팅.. 해야 하는데 이 시점에서 하면 ExtraMapDelay 에 따라 몇가지 놓치는 것이 생길 수 있다.
//			// LoadExtraMap 시점에 처리함.
//			//SetReadyPlayingExtraMap();
//			
//			// ExtraMap 전환 메시지.. ExtraMapDelay 보다 늦진 않게 나와야 함.
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
//				PreHandleLoadExtraMap(); // 추후 사정에 따라 PreHandleLoadExtraMap 은 약간 앞당겨질 수도.. 간단하지는 않겠지만
//				LoadExtraMap(); // 바로 로딩
//			}
//		}
//		else
//		{
//			// Extra 맵까지 고려해서 진짜 끝난 거.
//			bStageCleared = true;
//			StageCompleteClass<>::GetInstance().Signal();
//			if (GetB2GameModeType(CachedGameMode) == EB2GameMode::CounterDungeon)
//			{//반격던전 
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
//				RequestStageClear(); // 서버에 스테이지 클리어 알림
//			}			
//		}
//
//		// Stage 종료 및 Transition 대기 시 입력 불가능하게
//		ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//		if (B2PC)
//		{
//			// Stage 종료 및 Transition 대기 시 입력 불가능하게
//			B2PC->SetStageEndNonCombatState(true);
//		}
//	}
}

void AB2StageManager::RequestStageClear()
{
	StaticCacheStageData.PlayTimeInMainMap = LastUpdatedPlayTime;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// 시작시에 서버를 통하지 않았으면 StageClear 체크도 로컬에서 바로. IsConnected 체크를 하지 않는데 연결된 상태에서 에디터에서 프리뷰 플레이를 하게 될 수 있으므로
	// 단, 쿠킹 버전에서는 더 이상 이런 방식의 클리어 처리를 지원하지 않는다.
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
		FMath::TruncToInt(LastUpdatedPlayTime), // 클리어 등급을 정하는 수치인데 괜히 애매한 상황에서 반올림하는 것보다 가능한 좋게 나오도록 자르는 게 낫겠지..
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
//		DocBattleStage->SetStageClearGrade(StageClearGrade); // 스테이지 종료 UI 별 불타는 거 몇개나 나오는지.
//		
//		/* Item */
//		int32 MultipleValue = GET_TUPLE_DATA(FB2ResponseClearStage::multiple_value_index, Info);
//		// 보상 아이템.. 기본 아이템 목록이랑.. 보상골드(드랍골드 이외의)도 있으면 보상아이템 형태로 추가한다.
//		GetB2ItemArray(FB2ResponseClearStage::items_index, Info);
//		// 보스조각 보상 아이템
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
//		// 에테르 보상 아이템
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
//		// 재료 보상 아이템
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
//		// 판매 장비 아이템
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
//		// 판매 에테르 아이템
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
//		// 몹잡고 떨어지는 Drop Gold
//		int32 StageDropGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_drop_gold_index, Info);
//		// 스테이지 클리어 후 보상으로 줄수 있는 Reward Gold
//		TArray<int32> StageRewardGolds = GET_TUPLE_DATA(FB2ResponseClearStage::reward_golds_index, Info);
//		// Reward Gold 총합 (x3 배수 적용)
//		int32 StageRewardTotalGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_reward_gold_index, Info);
//		// QTE Gold 
//		int32 QTEGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_qte_gold_index, Info);		
//		// 최종 Total Gold (장비 판매 Gold 미포함)
//		int32 TotalGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_gold_index, Info);
//		// 장비/에테르 판매 Gold
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
//		// 보상 아이템 리스트
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
//	//데이터 저장. 보상 아이템도 여기서 들어감.
//	BladeIIGameImpl::GetStageDataStore().ClearStage(Info, StaticCacheStageData.ServerStageID);
//
//	ABladeIIPlayerController* PlayerController = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	FMyAssetStore* MyAssetStore = PlayerController? &PlayerController->GetMyAssetStore() : NULL;
//	if (MyAssetStore)
//	{ // 클라 연출 용이지만 현재는 실제 값을 서버에서 받았고 결과 화면에 제대로 뿌려주려면 여기 싱크가 필요함.
//		// 근데 계속 잘 안되고 하면 FB2MessageInfoResponseClearStage 에 total_added_drop_gold 랑 total_added_reward_gold 있으니 그걸로다가..
//		MyAssetStore->SyncCurrentTempGoldToAccountInfo(true); // Freeze 시킨다.
//	}
//	OnStageClear();
}

void AB2StageManager::OnStageClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::OnStageClear"));

	//if (CachedGameMode)
	//{ // 기본적인 Auto 상태를 끄는 게 아니라 일시적으로만 끈다. 기본 auto 상태는 언제든 저장할 필요가 있으므로.
	//	CachedGameMode->TemporaryStopLocalPlayerAuto();
	//}

	//UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//if (DocBattleStage)
	//{
	//	DocBattleStage->SetStagePlayTime(StaticCacheStageData.GetStageClearTime());
	//	if (!Utilities::FConnectionState::GetInstance().IsConnected())
	//	{
	//		DocBattleStage->SetStageClearGrade(STAGE_CLEAR_GRADE_STAR_3); // 연결 안된 상태라면 걍 다 보이게 만땅 채워준다.
	//	}
	//}

	//// 스테이지 클리어 후 보상 산정 등을 위한 noti.
	//// StageEventDirector 쪽으로 notify 를 하기 전에 이쪽으로 먼저 해야 EventWidget 초기화 시 참고할 데이터들이 채워질 것임.
	//auto* BladeIIPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(BladeIIPlayer ? BladeIIPlayer->GetController() : UGameplayStatics::GetLocalPlayerController(this));
	//BII_CHECK(BladeIIPlayer && B2PC);

	//// 스테이지 클리어 연출로 인해 메모리 상승 전에 가능한 것들을 내려줌. 
	//// GC 를 돌기 때문에 블로킹이 좀 걸릴 수 있다. 보스몹 막타 때 블로킹이 좀 거슬린다 싶으면 재고.
	//ManualUnloadOnStageClear(this); 
	//
	//// 이 타이밍 즈음 해서 StageResult UI 를 백그라운드 로딩해 본다.. 도중에 슬쩍 튀긴 함.
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
	// 시작시에 서버를 통하지 않았으면 StageClear 체크도 로컬에서 바로. IsConnected 체크를 하지 않는데 연결된 상태에서 에디터에서 프리뷰 플레이를 하게 될 수 있으므로
	// 단, 쿠킹 버전에서는 더 이상 이런 방식의 클리어 처리를 지원하지 않는다.
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
	//		// colosseum comment : reward list 를 참조하여 리워드 타입에 따른 보상을 처리할 수 있도록 작업이 필요합니다.

	//		// 클리어면 연출 UI는 연출하면서 띄워줌
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
	//		// 패배면 기존 영웅의 탑 결과 UI가 아닌 DefeatUI 를 띄워줌.
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
	//		// 보상처리		
	//		for (auto Reward : MsgPtr->totems)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().AddTotem(Reward); // 토템
	//		}

	//		for (auto Reward : MsgPtr->update_items)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE, Reward); // 토템 조각 
	//		}

	//		// 씬처리
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
	// 여기선 DisplayTime 을 줄 수는 없고 월드가 새로 로딩 된 후에 직접 꺼야 함. 이게 좀 문제인데, 여기서 열어둔 팝업을 레벨 의존적인 게임코드에 캐싱해 놓기도 좋지 않고 해서. 걍 레벨 오픈할 때 팝업들은 모두 해제하는 식이 되어야 할 듯.
	//FText TransitionMessage = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("InGameHUD_TrasitToExtra")));
	//ABladeIIGameMode::SetHUDCenterMessage(TransitionMessage, 0.0f, 1); // AnimIndex 1 번. 이건 좀 하드코딩임. 번호 틀린다고 큰 참사가 일어날 것도 아니고..
}

void AB2StageManager::ExtraMapLoadingDelayTimerCB()
{
	GetWorldTimerManager().ClearTimer(ExtraMapLoadingDelayTimer);
	PreHandleLoadExtraMap(); // 추후 사정에 따라 PreHandleLoadExtraMap 은 약간 앞당겨질 수도.. 간단하지는 않겠지만
	LoadExtraMap();
}

void AB2StageManager::PreHandleLoadExtraMap()
{
	// 원래 다른 게 있었는데 지금은 딸랑 SaveDataForExtraMap 만 있군..

	SetReadyPlayingExtraMap(); // ExtraMap 으로 넘겨줄 데이터 저장.
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
	//	StaticCacheStageData.PreExtraLastPlayedCharClass = PlayerCharacter->GetCurrentPlayerClass(); // 현재 클래스만 저장. 다른 하나는 이 값에 따라 DataStore 에 있는 Main 혹은 Sub 로.
	//	auto* MyAssetStore = &PlayerController->GetMyAssetStore();
	//	if (MyAssetStore)
	//	{
	//		StaticCacheStageData.MainMapStartingGold = MyAssetStore->GetInitialGoldAmount();
	//		StaticCacheStageData.MainMapLastGold = MyAssetStore->GetCurrentTempGoldAmount();
	//	}

	//	UB2SkillRTManager* SkillRTManager = PlayerCharacter->GetSkillRTManager();
	//	if (SkillRTManager)
	//	{
	//		StaticCacheStageData.SetReadyPreExtraCombatVariables(PlayerCharacter, SkillRTManager); // 나머지 전투 상태 관련 변수들은 따로 모아서..
	//	}
	//}	
}

void AB2StageManager::RetrieveDataForExtraMap()
{
	//// SetReadyPlayingExtraMap 에서 PreExtraLastPlayedCharClass 를 세팅하는데 그렇다고 여기서 GetPreExtraLastPlayedClass 를 하지 않는다. 그건 게임모드에서 Player Character spawn 시 미리 해 놓았어야.
	//// Health 들도 마찬가지. 그건 BladeIIPlayer 랑 SkillRTManager 쪽에서..

	//ABladeIIPlayer* PlayerCharacter = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//BII_CHECK(PlayerCharacter && PlayerCharacter->IsValidObj());

	//if (PlayerCharacter)
	//{
	//	auto* PlayerController = Cast<ABladeIIPlayerController>(PlayerCharacter->GetController());
	//	auto* MyAssetStore = PlayerController ? &PlayerController->GetMyAssetStore() : NULL;
	//	if (MyAssetStore)
	//	{
	//		MyAssetStore->SetStartingGold(StaticCacheStageData.MainMapStartingGold);
	//		MyAssetStore->AddGold(StaticCacheStageData.MainMapLastGold - StaticCacheStageData.MainMapStartingGold); // 현재 값을 MainMapLastGold 로.
	//	}
	//}

	//LastUpdatedPlayTime = StaticCacheStageData.PlayTimeInMainMap;
}

void AB2StageManager::BeginStageClearScene_Phase1()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageClear); // StageClear UI 에서 보내는 신호에 의해 BeginStageClearScene_Phase2 가 시작될 것.

	//// StageClear Phase1 용 카메라 세팅.
	//UWorld* TheWorld = GetWorld();
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (TheWorld && B2PC)
	//{
	//	if (!StageClearViewCam){
	//		// 서브레벨 BP 를 통해 세팅이 되지 않았다면 내부적으로 직접 생성한다.
	//		// 가능한 sky box 등에 가리지 않은 까만 배경을 바라볼 수 있게. 그렇다고 너무 멀면 그것대로 부동소수 정밀도 문제를 겪을 수 있다.
	//		// 사용하는 이펙트가 월드좌표에만 align 되는 거 같아서 위나 아래를 보지는 못하고 수평을 유지하면 fog 때문에 멀리서도 어둡지가 않은데 걍 아래서 fog 를 끔
	//		FTransform CamSpawnTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(200000.0f, 0.0f, 0.0f));
	//		FActorSpawnParameters CamSpawnParam;
	//		StageClearViewCam = Cast<ACameraActor>(TheWorld->SpawnActor(ACameraActor::StaticClass(), &CamSpawnTransform, CamSpawnParam));
	//	}
	//	UCameraComponent* StageClearViewCamComp = StageClearViewCam ? StageClearViewCam->GetCameraComponent() : NULL;
	//	if (StageClearViewCam && StageClearViewCamComp)
	//	{
	//		// 와이드 화면 대응.. 이건 그냥 쓰면 넓은 화면에서 연출 효과들이 짤리게 되어서 adaptive FOV 가 필요함.
	//		StageClearViewCamComp->SetConstraintAspectRatio(false);
	//		StageClearViewCamComp->SetAspectRatioAdaptiveFOV(true);

	//		EnsureTargetToStageClearCam();

	//		StageClearPhase1_StartVictoryBGFx(); // 카메라가 생성되면 VictoryBGFx 는 바로 시작. 나머지 Wing 이랑 Gem 은 UIStageClear 쪽에서 신호를 보낼 것.
	//	}
	//	
	//	// StageClearPhase1 은 StageEventDirector 에서 clnematic mode 진입을 하지 않으므로 여기서 적당히 넣어줌. 종료 시점이니 CinematicMode 에서 나오는 걸 걱정할 필요는 없겠지..?
	//	B2PC->SetCinematicMode(true, false, true, false, true);
	//}

	//TurnOffAllFogs(); // 포그 때문에 원하는 어두운 배경이 안나오니 꺼놓자.
}

void AB2StageManager::StageClearPhase1_StartVictoryWingFx()
{// StageClear UI 에서의 이벤트를 통해 오게 될 것.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearWingFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearWingFxTemplate() : NULL),
	//	StageClearWingFxComp, TEXT("StageClearWingFxRelativePos"), TEXT("StageClearWingFxRelativeRot"), TEXT("StageClearWingFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryGemFx()
{ // StageClear UI 에서의 이벤트를 통해 오게 될 것.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearGemFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearGemFxTemplate() : NULL),
	//	StageClearGemFxComp, TEXT("StageClearGemFxRelativePos"), TEXT("StageClearGemFxRelativeRot"), TEXT("StageClearGemFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryBGFx()
{// StageClear UI 에서의 이벤트를 통해 오게 될 것.
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
	//	// 와이드 화면 대응.. 이건 그냥 쓰면 넓은 화면에서 연출 효과들이 짤리게 되어서 adaptive FOV 가 필요함.
	//	StageClearViewCamComp->SetConstraintAspectRatio(false);
	//	StageClearViewCamComp->SetAspectRatioAdaptiveFOV(true);
	//	
	//	EnsureTargetToStageClearCam(); // 부끄러운 짓

	//	if (InOutCompForSet) // 카메라처럼 맵에 배치된 것이 세팅되어 있다면 그걸 사용하고
	//	{
	//		InOutCompForSet->Activate();
	//	}
	//	else // 아니면 내부에서 생성
	//	{
	//		if (InPSTemplate)
	//		{
	//			// 카메라로부터의 상대 위치 및 스케일 등은 테스트를 통해 정해서 ini 에 기록.
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
	TurnBackOnAllFogs(); // Phase 1 에서 끈 포그들 다시 켜기.

	//check(CachedGameMode);
	//const TArray<class AB2StageEventDirector*>& EventDirectors = CachedGameMode->GetEventDirectorArray();
	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	// 이건 StageClear UI 에서 오는 신호를 받는다. 최종 결과창과 함께하는 승리 포즈 띄우기
	//	// ESEV_Clear 대신 거기서 쪼개진 다른 enum 으로 검사하게 될 수도 있음.
	//	if (CurrSED->GetDirectingEvent() == EStageEvent::ESEV_Clear){
	//		CurrSED->CustomBeginShow();
	//	}
	//}

	//EnsureTargetToStageClearCam(); // 부끄러운 짓
	//
	//// 승리 BGM 은 결국 Phase2 와 함께..
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//BII_CHECK(B2GM);
	//if (B2GM)
	//{
	//	B2GM->PlayStageVictoryBGM(IsGoldBonusStage(), IsBossStage());
	//}
}

void AB2StageManager::EnsureTargetToStageClearCam()
{
	// 기본적으로는 StageClearScene Phase1 시작 시 StageClearViewCam 으로 ViewTarget 한번만 세팅하면 되는 건데 이런저런 카메라효과가 있는 스킬과 함께 막타를 날릴 시 그게 안 먹히는 일이 있다.
	//// 부끄럽게도 그걸 제대로 고치지 못해서 이 코드를 따로 묶어서 여러번 호출함. 그렇게 해서 크게 손해 볼 것도 아니고 해서 
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

	//	// 그냥 끄지 않고 visibility 를 백업해 놨다가 끔. 나중에 다시 켜야 하니
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
		if (ExpFogElem.Key && ExpFogElem.Value){ // 기존에 visible 했던 경우만 다시 켠다.
			ExpFogElem.Key->SetActorHiddenInGame(false);
		}
	}
	for (auto& AtmosFogElem : CachedAtmosFogVisMap)
	{
		if (AtmosFogElem.Key && AtmosFogElem.Value){ // 기존에 visible 했던 경우만 다시 켠다.
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
	//[@SameLine, 180524] 보스몹 타입 스폰시 무조건 랜덤 bgm 재생 수정
	//x-10(BossStage)가 아닌 경우 첫bgm->보스bgm 변경후 변경X
	//x-10(BossStage)의 경우 첫bgm->보스bgm->보스bgm 변경 가능

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
{ // 여길 통해 세팅된다면 레벨에 배치된 애들일 것. StageClearPhase1_StartVictoryGemFx 등에서 동적 처리를 하지 않게 된다.
	StageClearViewCam = InCamActor;
	StageClearWingFxComp = InPSWingComp;
	StageClearGemFxComp = InPSGemComp;
	StageClearBGFxComp = InPSBGComp;
}

void AB2StageManager::RefreshStagePlayTimeCheck()
{ // UpdateStagePlayTime 이외에 따로 카운트 시작 시점 갱신이 필요한 경우 사용. 일종의 Start 나 Resume 격.
	UWorld* TheWorld = GetWorld();
	BII_CHECK(TheWorld);

	if (TheWorld)
	{
		PlayTimeLastCheckedCache = TheWorld->GetTimeSeconds();

		bCountingPlayTime = true; // Refresh 를 한 이상 카운트 시작.
	}
}
void AB2StageManager::UpdateStagePlayTime(bool bStopCounting)
{
	//UWorld* TheWorld = GetWorld();
	//BII_CHECK(TheWorld);

	//if (TheWorld && bCountingPlayTime)
	//{ // 마지막 PlayTimeLastCheckedCache 시점으로부터 누적 시간 갱신.

	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//	bool bCinematicMode = B2PC && B2PC->IsInCinematicMode();

	//	if(!bCinematicMode)
	//		LastUpdatedPlayTime += TheWorld->GetTimeSeconds() - PlayTimeLastCheckedCache;

	//	PlayTimeLastCheckedCache = TheWorld->GetTimeSeconds();

	//	if (bStopCounting) // 이 경우 Stop 이나 Pause 격으로 사용.
	//	{
	//		bCountingPlayTime = false;
	//	}
	//	
	//	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	//	if (DocBS) // UI Doc 업데이트.
	//	{
	//		DocBS->SetStagePlayTime((int32)LastUpdatedPlayTime);
	//	}
	//}
}

float AB2StageManager::GetStagePlayTime(bool bUpdateNow)
{
	if (bUpdateNow)
	{
		UpdateStagePlayTime(false); // 중간 임의 시점에 업데이트 하는 사용. 중단된 상태에선 어차피 업데이트 안 함.
	}

	return LastUpdatedPlayTime;
}

void AB2StageManager::AutoUpdateStagePlayTimeCB()
{
	GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);

	UpdateStagePlayTime(false); // 업데이트 되면서 UIDoc 업데이트 하면 알아서 표시 갱신 ㅋ 중단 도중에는 알아서 노카운트

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
//	// 현재 선택된 DifficultyLevel 과 Info 세팅 (내지는 서버 마스터 데이터) 에서 비롯되는 실제 적용되는 scale 값 리턴
//	return BladeIIGameImpl::GetStageDataStore().GetStageDifficultyScale(
//		StaticCacheStageData.ClientStageId, 
//		StaticCacheStageData.StageDifficultyLevel,
//		// B2GameModeType 넘겨주는 건 M15 에서의 임시.
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
	//		CurrMob->PreventMovementByStageEvent(); // PreventMovementByStageEvent 로 해야 AnimNotify 에 의해 AllowMovement 될 때 안 돌아감 ㄷㄷ
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
//	// 기본적으로는 에디터 플레이 및 서버 연결이 안된 상태에서의 프리뷰 플레이 정도에 사용.
//	// 최종 게임에서 정상적인 상황이면 사용할 기능은 아니지만 스테이지 진입 직후 네트워크가 끊겼다면 일단 플레이는 가능해야 하므로 어느 정도 세팅은 필요하다.
//
//	check(StageManager);
//
//	TotalExpOfCurrentStage = 1000; // 물론, 정식 세팅은 아님.
//
//	// 일단 WorldSettings 에 세팅한 값을 우선 적용 시도. 로딩 옵션을 주었다면 아래에서 그게 적용될 것. 물론 개발용 세팅
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
//	// 개발 버전용 스테이지 번호 등의 옵션을 블루프린트 OpenLevel 옵션 문자열(Open 명령 인자)을 통해 세팅할 수 있도록. 최종적으로는 서버에서 가져와야 함.
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
//			else if (TheWorldContext->LastURL.HasOption(TEXT("SN"))) // 짤은 버전
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
//			else if (TheWorldContext->LastURL.HasOption(TEXT("DF"))) // 짧은 버전
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
//			else if (TheWorldContext->LastURL.HasOption(TEXT("SM"))) // 짧은 버전
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
//	// 단독 플레이라도 난이도에 따라 시나리오 모드를 보지 못하도록.
//	// 여기서 예외 처리 하지 않으면 상위 난이도에서도 시나리오 연출을 보게 될 수 있는데 괜히 에디터 플레이에서 기능을 엉뚱하게 확인할 소지가 있음.
//	if (!IsScenarioModeAllowedDifficulty(StageDifficultyLevel))
//	{
//		bScenarioMode = false;
//	}
//
//	if (BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId > 0)
//	{
//		ClientStageId = BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId;
//		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = -1; // 한번 쓰고 나서도 남아있으면 여러모로 곤란
//	}
//
//	CachingStageInfoData();
//
//	if (CachedCurrentStageInfoData)
//	{
//		bHasExtraLevel = CachedCurrentStageInfoData->bHasExtraMap;
//
//		//ExtraMap은 World가 다른 개념으로 보상에 대해 추가 설정해줘야 한다.(서버 데이타 확정시까지 임시)
//		StageReward.SetTotalStageRewardGold(bPlayInExtraLevel ? 700 : 500);
//
//		UB2ItemInfo* ItemDataTable = StageReward.GetItemInfoTable();
//		if (ItemDataTable)
//		{
//			// 일단은 스테이지에서는 하나의 아이템만이 드롭된다고 하자. (기획은 최대 2개)
//			// StageReward.SetDefeatBossRewardItem(ItemDataTable->GetRandomItemRefID());
//		}
//	}
//
//#if WITH_EDITOR // For use play-in-editor stage settings
//	if (GIsEditor){
//		bStandalonePIE = true;
//	}
//
//	// 이것도 에디터 프리뷰 차원에서
//	FB2Item DummyReward;
//	DummyReward.ItemRefID = 1000100;
//	StageReward.SetDefeatBossRewardItem(DummyReward);
//#endif
}

void FCacheStageEssentialDataToKeep::Initialize()
{
	StageReward.Initialize();

	//
	// 응답 받고 값 세팅 전 이게 불리므로 Request 상태 캐싱 류 변수들은 여기서 초기화 하면 안됨. 필요하면 생성자에서..
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

	// EResurrectGameModeType::EResurrectType_OnlyClientMode는 리스트에서 제외해준다.
	//ResurrectionList.Empty();
	//for (int32 i = (int32)EResurrectGameModeType::EResurrectType_Stage; i < (int32)EResurrectGameModeType::EResurrectType_Max; ++i)
	//	ResurrectionList.Add((EResurrectGameModeType)i, FResurrectData());

	//ServerSelectedStageBuff.Empty();

	/*
	//UI에서 폴링을 방지 하기 위해 Doc에 데이터싱크
	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		DocBattle->SetMaxResurrectionChance(MaxResurrectionChance);
		DocBattle->SetRemainingResurrectionChance(RemainingResurrectionChance);
		DocBattle->SetResurrectionCost(ResurrectionCost);
	}
	*/
#if WITH_EDITOR // For use play-in-editor stage settings
	bStandalonePIE = GIsEditor; // 에디터에서는 로비를 통해 로딩되어 서버 응답을 받은 경우 다시 false 세팅이 될 것.
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
	//// AllPCClassNeedCleanInven 은 비어있지 않아야 말이 되지만 서버까지 오간 응답이 어떻게 될지에 대해서 check 를 걸고 싶지는 않군.
	//// 인벤토리를 비워낼 필요가 있는 클래스들을 미리 점검해서 인벤토리 창 갔을 때 선택되어 있는 클래스에 반영한다.
	//const bool bIsMainClassNeedToCleanInven = (AllPCClassNeedCleanInven.Find(RequestedMainPlayerCharClass) != INDEX_NONE);
	//const bool bIsSubClassNeedToCleanInven = (AllPCClassNeedCleanInven.Find(RequestedSubPlayerCharClass) != INDEX_NONE);
	//// Main/Sub 둘 다 괜찮더라도 다른 선택 안된 애의 inventory 가 차 있으면 그것도 입장불가임.
	//const EPCClass FallbackInvenCleanCharClass = (AllPCClassNeedCleanInven.Num() > 0) ? AllPCClassNeedCleanInven[0] : EPCClass::EPC_Gladiator;
	//PCClass = bIsMainClassNeedToCleanInven ? RequestedMainPlayerCharClass :
	//	(bIsSubClassNeedToCleanInven ? RequestedSubPlayerCharClass : FallbackInvenCleanCharClass);
	//if (PCClass == EPCClass::EPC_End) { // 이런 일은 없어야겠지만..
	//	PCClass = EPCClass::EPC_Gladiator;
	//}

	//// 그리고 클래스 선택이 되었다면 실제로 차 있는 클래스 탭까지..
	//const bool bIsNeedToClean = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, PCClass);
	//if (!bIsNeedToClean)
	//	return false;
	//*/

	//if (AllPCClassNeedCleanInven.Num() > 0)
	//{
	//	// 이 시점에 PreLoadingScreen 이 열려 있을 것이므로 닫도록.
	//	ClosePreLoadingScreenAllClass<>::GetInstance().Signal();
	//	CancelStartGameStageClass<>::GetInstance().Signal();
	//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // 기타 전투 요청 처리가 불가한 경우 가능한 반복 전투 요청을 취소해야 함.

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
	// 일단은 아무것도 안함 
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
		DocBS->SetSelectedStageBuffs(ServerSelectedStageBuff); // Server Response에서 인정된 Buff만 UIDoc에 갱신
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
	//// [180712_YJ] 난이도 On/OFF기능 추가.
	//// 혹시나 강제로 막힌난이도에 진입했을경우에, 에러팝업 호출 및 로비로 이동.
	//// 튜토리얼 고려 안함.
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

	//bStageRequestedWithSkipStoryCinematic = bSkipStoryCinematic; // 응답이 온 후 bScenarioMode 여부 판별에 쓰임.

	//// 여기서의 RepeatBattleState 는 Response 가 오면 세팅한다.

	////ToDo 서버로 입장요청 패킷을 보낸다. (스테이지Id, 메인/서브 캐릭터 클래스, 기타 아이템(골드업, 아이템추가권등등)
	//
	//UE_LOG(LogBladeII, Log, TEXT("StageRequest"));
	////[@AKI, 180214] RequestServerStageId가 0으로 들어오는 현상 때문에 ensure걸어 놓음. 혹시 여기에 걸릴 경우 안광익에게 연락 바람.
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

	TArray<EStageBuffType> ThisRequestBuff = ServerSelectedStageBuff; // 혹시라도 초기 선택과 마찬가지로 지속해서 넣어야 한다면 AllSelectedStageBuff 로
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

	//// 처음 플레이 시에는 true. 두번째 이상일 때에는 스토리모드에서 사용자가 스토리넘김을 선택했거나, 모헙모드(일반 이상의 난이도)이면 false.
	//bScenarioMode =
	//	// 난이도가 달라졌을 시 ServerStageID 도 달라지게 되므로 첫 플레이 시에는 IsStageClearedOnce 가 다시 false 가 된다. 따라서 IsScenarioModeAllowedDifficulty 는 && 조건이 되어야 함.
	//	IsScenarioModeAllowedDifficulty(InStageDifficulty) && 
	//	(StageDataStore.IsStageClearedOnce(ServerStageID) == false || !bStageRequestedWithSkipStoryCinematic);

	//SetStageBasisData_CommonInternal(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass);

	//if (InRepeatBattleState)
	//	RepeatBattleState = *InRepeatBattleState;
}

void FCacheStageEssentialDataToKeep::SetStageBasisData_CounterDungeon(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass)
{
	Initialize();

	// 반격던전은 ClientId와 ServerId가 같음.
	ServerStageID = FServerStageID(InClientStageId);
	bScenarioMode = false;

	SetStageBasisData_CommonInternal(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass);
}

void FCacheStageEssentialDataToKeep::SetStageBasisData_CommonInternal(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass)
{
	// ServerStageID 는 이거 이전에 세팅되어 있어야 하는데... TutorialGameMode 같은 예외 상황이 있어서 체크는 못하겠군 ㅡㅡ
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
	ServerStageID = FServerStageID(); // 일단 이걸 리셋하는 게 젤 중요.
	bScenarioMode = true; // 얘는 기본값 true 가 더 어울리겠다.
	bStageRequestedWithSkipStoryCinematic = false;
	ServerSelectedStageBuff.Empty();
	bHasExtraLevel = false;
	//[@AKI, 180208] Extra Map 들어갔다오면 초기화를 안시키는 문제(캐쉬 되어 있는 잔재가 남아있음)
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
	//AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(InCurrentGameMode); // 일반적인 경우 여기로 들어오는 건 LobbyGameMode. 로비에서 스테이지 시작 버튼 누르고 서버 응답 시 클라이언트 맵 로딩 전 값 전달하는 부분.
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

	//// FB2StartStageInfo 에 최종 허가된 StageBuff 정보가 들어있다면 RequestedStageOption 대신 그걸로 해서 SetStageResponseData 에서 세팅을
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

	//// 스테이지 시작과 함께 AccountInfo 갱신 필요로 인해 ClientDataStore/StageDataStore 쪽에도.
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

	//// 연출 켰으면 로그전송
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
	//// 반격던전은 GetServerStageID로 ServerStageID를 가져올 수 없으므로, SetStageBasisData_CounterDungeon() 으로 따로 처리
	//SetStageBasisData_CounterDungeon(RequestedClientStageId, RequestedStageDifficulty, RequestedMainPlayerCharClass, RequestedSubPlayerCharClass);

	//TArray<FB2Item> EmptyArray;
	//SetStageResponseData(Token, 0, 0, EmptyArray, 0, EmptyArray);

	//StartBladeIICounterDungeon(InCurrentGameMode, ClientStageId);
}

// colosseum comment : bUseGem 제거
void FCacheStageEssentialDataToKeep::RequestStartHeroTower(AGameMode* InCurrentGameMode, int32 nStartFloor, EPCClass MainCharClass, EPCClass SubCharClass, FString szToken)
{
	//RequestedMainPlayerCharClass = MainCharClass;
	//RequestedSubPlayerCharClass = SubCharClass;

	//// 층갖고 마스터데이터에서 아이디 얻어와랑
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
//		// colosseum comment : daily_left_free_try_count 제거
//		//pDoc->m_ptrGetHeroTower->daily_left_free_try_count = msgPtr->left_free_ticket;
//		pDoc->m_ptrGetHeroTower->daily_try_count = msgPtr->daily_enter_count;
//		// colosseum comment : current_gem 제거
//		//BladeIIGameImpl::GetClientDataStore().SetGemAmount(msgPtr->current_gem);
//		pDoc->m_bIsClearHeroTower = false;
//	}	
//	
//#if !UE_BUILD_SHIPPING
//	extern bool gUseHeroTowerResetNoLoading;
//#endif
//	AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(InCurrentGameMode);
//	if (pGM 
//#if !UE_BUILD_SHIPPING // 영웅의 탑은 다음 층 바로 이동시 로딩 없이 스테이지 리셋을 하는데 이게 개발도중 추가되는 코드 대응이 안되면 문제를 일으키기 쉬우므로 특정 문제 발생시 테스트를 위해 플래그를 제공.
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
	//// 토템드랍이 100% 이니 강제로 스폰할것 챙겨줌
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
//	////UI에서 폴링을 방지 하기 위해 Doc에 데이터싱크
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
//	//// 부활 차수 별로 정해져 있는 비용을 ClientDataStore 에서 룩업.
//
//	//ResurrectionList[ResurrectModeType].ResurrectionCost = BladeIIGameImpl::GetClientDataStore().GetResurrectionCost(ResurrectModeType, NextResurrectionNum);
//	//
//	//// UIDoc에도 셋팅
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
	//{// StructToSet 은 현재 숨어있는 태그 캐릭터 상태 저장용이라 AmIMainCharacter 와 반대의 값을 사용해서 세팅.
	//	StructToSet->MaxHealth = CurrentActivePlayer->AmIMainCharacter() ? PreExtraSubPlayerLastMaxHP : PreExtraMainPlayerLastMaxHP;
	//	// 사실상 ExtraMap 로딩 직전의 CurrentHealth 값일 건데 LastHealth 가 딱히 필요한가?
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
{ // 인게임에서 반복전투 stop 하려면 여기서 리셋 후 StageManager 에서 추가 처리 필요.
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
	//	{//남은 골드 획득 가중치를 모두 더한다.
	//		LeftGoldWeight += CheckSpawnPool->GetLeftGoldWeight(StageManager->GetCurrentClientStageId(), StageManager->GetStageDifficultyLevel());
	//	}

	//	for (auto* TreasureChest : B2GM->GetTreasureChestArray())
	//	{
	//		LeftGoldWeight += TreasureChest->RewardGoldWeight;
	//	}

	//	if (LeftGoldWeight > weight)
	//	{//남은 골드량을 위에서 구한 가중치 총합으로 나누어 기준값을 정한다.
	//		int32 randBarometer = (TotalStageRewardGold - SumCurrentRewardGold) / LeftGoldWeight * weight;

	//		rtnValue = FMath::Max(1, (int32)(randBarometer * (1.f + FMath::FRandRange(-0.2f, 0.2f))));
	//		rtnValue = (rtnValue + SumCurrentRewardGold > TotalStageRewardGold) ? TotalStageRewardGold - SumCurrentRewardGold : rtnValue;

	//		SumCurrentRewardGold += rtnValue;
	//	}
	//	else
	//	{//남은 총 가중치가 현재 구할 가중치보다 적을 경우 남은 골드 모두 반환
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
//	//CommonSpawnParam.bDeferConstruction = true; // bDeferConstruction 과 bDeferPostSpawnInitialize 는 SpawnDropItem 도중에 자동으로 사용될 것.
//	CommonSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;		
//	
//	bool IsQTEDeath = DelaySpawnRefActor ? DelaySpawnRefActor->IsInQTEState() : false;
//
//	uint32 GoldWieght = IsQTEDeath ? DropCoinCountForQTE : DropItemInfo.RewardGoldWeight;
//	uint32 NumGoldCoin = GoldWieght; //별다른 이야기가 없으면 weight 1당 1개씩
//	uint32 NumDropItems = NumGoldCoin + (DropItemInfo.bCanDropItem ? 1 : 0);
//
//#define AVAILABLE_EDITOR_PREVIEW_SPAWNITEM 1
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//	//에디트 프리뷰 용도 - SpawnPool로 스폰된 몹이 아닌 경우.
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
//		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f); // 여기선 Rotation 만 구한다. Location 은 delay 가 있을 경우 SpawnRefActor 의 업데이트 된 위치값을 가져감.
//
//		StageManager->DeferredSpawnDropGold(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? 10 : 
//#endif
//			// SpawnTimeDelay 는 spawn 부하 분산을 위해 추가 조절을 할 수도 있다.
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
//		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f); // 여기선 Rotation 만 구한다. Location 은 delay 가 있을 경우 SpawnRefActor 의 업데이트 된 위치값을 가져감.
//
//		StageManager->DeferredSpawnDropItem(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? DropItemInfo.TestNumSpawnItemRefID :
//#endif
//			// SpawnTimeDelay 는 spawn 부하 분산을 위해 추가 조절을 할 수도 있다.
//			GetDefeatBossRewardItem().ItemRefID, CommonSpawnParam, DelaySpawnRefActor, SpawnLocation, SpawnRotation, DropItemInfo.RewardSpawnDelay);
//		StageManager->DeferredSpawnDropItem(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? DropItemInfo.TestNumSpawnItemRefID :
//#endif
//			// SpawnTimeDelay 는 spawn 부하 분산을 위해 추가 조절을 할 수도 있다.
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

		// 이게 ID 별로 딸려 있는 데이터를 다 로딩하는 구조라 ItemInfo 로딩을 나눈 뒤로는 그리 권장되는 사용은 아닌데
		// Reward 아이템이라면 사실상 대부분의 리소스들이 필요하긴 할 거 같다. 일단 놔둠. 좀 낭비가 된다 싶으면 PreloadClassAssets 를 범주별로 나누는 걸 해볼 수도.
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
	//	if (bMaintainBattleUI) // 기본적으로 이쪽 widget 설정에 bHideBattleRelatedWidgets 가 있을 건데 따로 켜고 싶은 경우를 위해..
	//	{
	//		UIMgr->HideBattleRelatedWidgets(false);
	//	}
	//}

	//UB2UIDocScenarioDialog* DocSDG = UB2UIDocHelper::GetDocScenarioDialog();
	//if (DocSDG)
	//{
	//	// bLastBattleUIHidden 값에 따라 대화창 위치가 좀 바뀔 거.
	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(CachedStageManager));
	//	if (B2PC && B2PC->IsInCinematicMode())
	//	{
	//		DocSDG->SetbLastBattleUIHidden(B2PC->IsHUDHidingCinematicMode()); // CinematicMode 설정은 우선시 됨.
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
		// 종료시 INDEX_NONE
		// 반드시 ForceSkip() 이전에 호출 되어야 함 - DialogInstance가 nullptr 됨
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
	// DialogCharacterInfo 가 Portrait material 도 들고 있지만 그건 DialogCharacterInfo 에서 직접 꺼내오지 않고 InfoSet 테이블에서 별도 인터페이스로 가져옴.
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
	UIDocDialog->SetThisDialogShowingTime(DialogLineInfo.ShowingTime); // Dialog UI 쪽에서 참고만.. 타이머는 여기서 돌린다. SetDialogText 전에 이거부터..
	UIDocDialog->SetDialogText(BladeIIGetLOCText(B2LOC_CAT_DIALOGTEXT, DialogLineInfo.DialogLineCodeName.ToString()));
	
	UIDocDialog->SetShowNextButton(
		// 다음 줄이 있을 때에 NextButton 을 보인다. 어차피 전체 영역버튼이나 조그만 NextButton 이나 역할 자체는 동일할 거..
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
	//		else if (bPausedDialog) // 실제로 Pause상태일 때  - Pause 이후 실행된 Dialog Sound는 Pause와 관계없이 계속 실행되므로 굳이 Resume시킬 필요 없다
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
	//// 실제로 다음 라인이 있는 경우, 바로 넘어가기 전 Delay 를 체크
	//if (CachedStageManager && DialogInfoInstace 
	//	&& DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex)
	//	&& DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex + 1))
	//{
	//	const FDialogLine& CurrDialogSequence = DialogInfoInstace->DialogSequence[CurrentDialogLineIndex];
	//	if (CurrDialogSequence.DelayToNextLine > 0.0f)
	//	{ // GoToNextLineTimerHandle 돌려쓰기
	//		CachedStageManager->GetWorldTimerManager().SetTimer(GoToNextLineTimerHandle, CachedStageManager, &AB2StageManager::NextDialogLineImmediateWrapper, CurrDialogSequence.DelayToNextLine, false);

	//		// ForceSkip 같은 건 하면 안되고 잠시 숨기기만 하자.
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
	//	NextDialogLineImmediate(); // 다음 라인이 없는 경우라면 skip 처리가 될 것.
	//}
}

void FB2ScenarioDialogManager::NextDialogLineImmediate()
{
//	if (CachedStageManager)
//	{
//		CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
//	}
//
//	// NextDialogLine 에서 delay 시켰던 경우. hide 복구를.
//	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//	UB2UIScenarioDialog* DialogUI = UIMgr ? UIMgr->GetUI<UB2UIScenarioDialog>(UIFName::ScenarioDialog) : NULL;
//#if !UE_BUILD_SHIPPING
//	extern bool gbHACKInGameUIHiddenByDevCommand; // UI 숨기는 치트 썼는데 나올 수 있으니 이것도 고려를..
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
	StopCurrentDialogSound(); // 중단부터
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
	// 디바이스 테스트 등을 위해 Gold spawn 비율을 조절할 수 있게 함.

	float FinalDropGoldSpawnRate = FMath::Clamp(GCheat_DropGoldSpawnRate, 0.0f, 1.0f);
	
	float ThisSpawnChanceInv = FMath::FRandRange(0.0f, 1.0f);
	if (ThisSpawnChanceInv > FinalDropGoldSpawnRate){
		return;
	}
#endif

	// 바로 Spawn 이라면 여기로 넘어온 SpawnLocation/Rotation 값을 쓰겠지만 delay 되는 경우는 DelaySpawnLocRefActor 의 위치를 사용하게 될 것.

	if (SpawnTimeDelay > 0.0f)
	{
		FDelayedSpawnDropItemInfo NewDelayInfo;
		GetWorldTimerManager().SetTimer(NewDelayInfo.DelayTH,
			FTimerDelegate::CreateUObject(this, &AB2StageManager::SpawnDropGold, Amount, SpawnParam, SpawnLocation, SpawnRotation, DelayedDropSpawnSerialNumber, false),
			SpawnTimeDelay, false);
		
		NewDelayInfo.DelaySpawnLocRefActor = DelaySpawnLocRefActor;
		DelayedDropSpawnInfoMap.Add(DelayedDropSpawnSerialNumber, NewDelayInfo);
		++DelayedDropSpawnSerialNumber; // 일련번호라 계속 증가
	}
	else
	{ // 바로 spawn 할 때에는 SpawnTimerMapKey 값을 음수로
		SpawnDropGold(Amount, SpawnParam, SpawnLocation, SpawnRotation, -1, InQTE);
	}
}

void AB2StageManager::DeferredSpawnDropItem(int32 ItemRefID, const struct FActorSpawnParameters SpawnParam, ABladeIICharacter* DelaySpawnLocRefActor, const FVector& SpawnLocation, const FRotator SpawnRotation, float SpawnTimeDelay)
{
	// 바로 Spawn 이라면 여기로 넘어온 SpawnLocation/Rotation 값을 쓰겠지만 delay 되는 경우는 DelaySpawnLocRefActor 의 위치를 사용하게 될 것.

	if (SpawnTimeDelay > 0.0f)
	{		
		FDelayedSpawnDropItemInfo NewDelayInfo;
		GetWorldTimerManager().SetTimer(NewDelayInfo.DelayTH,
			FTimerDelegate::CreateUObject(this, &AB2StageManager::SpawnDropItem, ItemRefID, SpawnParam, SpawnLocation, SpawnRotation, DelayedDropSpawnSerialNumber),
			SpawnTimeDelay, false);

		NewDelayInfo.DelaySpawnLocRefActor = DelaySpawnLocRefActor;
		DelayedDropSpawnInfoMap.Add(DelayedDropSpawnSerialNumber, NewDelayInfo);
		++DelayedDropSpawnSerialNumber; // 일련번호라 계속 증가
	}
	else
	{ // 바로 spawn 할 때에는 SpawnTimerMapKey 값을 음수로
		SpawnDropItem(ItemRefID, SpawnParam, SpawnLocation, SpawnRotation, -1);
	}
}

void AB2StageManager::SpawnDropGold(int32 Amount, const FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey, bool InQTE)
{
	//FTransform FinalSpawnTransform = SpawnDropDelayHandlingCommon(InitialSpawnLocation, InitialSpawnRotation, DelaySpawnInfoMapKey); // Delay ref actor 가 있는 경우 해당 액터의 위치를 사용.

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
	//{ // 굳이 Consumables 취급은 아니지만 ConsumingAmount 를 통해 전달..
	//	NewDropItem->SignificantData.ConsumingAmount = Amount;
	//}
}

void AB2StageManager::SpawnDropItem(int32 ItemRefID, const FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey)
{
	//FTransform FinalSpawnTransform = SpawnDropDelayHandlingCommon(InitialSpawnLocation, InitialSpawnRotation, DelaySpawnInfoMapKey); // Delay ref actor 가 있는 경우 해당 액터의 위치를 사용.

	//if (ItemRefID <= 0) // 실제로 정보를 얻어오지 못한 상황임. 
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
	//{ // 이것이 타이머를 통해 불려진 경우 타이머 클리어
	//	if (ThisInfoForDeferCall->DelayTH.IsValid())
	//	{
	//		GetWorldTimerManager().ClearTimer(ThisInfoForDeferCall->DelayTH);
	//	}

	//	if (ThisInfoForDeferCall->DelaySpawnLocRefActor)
	//	{
	//		// DelaySpawnLocRefActor 의 사망 시 LifeSpan 이 Delay 보다 짧다면 Valid 하지 않을 것이다. 
	//		// NotifyForgetACharacter 에서 혹시 DelaySpawnLocRefActor 가 오지 않았는지 검사해서 제거할 필요가 있음.
	//		checkSlow(ThisInfoForDeferCall->DelaySpawnLocRefActor->IsValidObj());
	//		// DelaySpawnLocRefActor 의 현 위치로 갱신.
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
		// 여기선 일시적으로 끄는 게 아니라 기본적인 auto 상태를 끔.
		TogglePlayerAutoStateClass<>::GetInstance().Signal();
	}
	*/
	SyncDataToUIDoc(); // 좀 쓸데없이 많은 데이터가 세팅되긴 하지만..
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
	// 스테이지에서 특정 Star 등급을 받기 위한 시간 기준 안내 텍스트 리턴. UI 에서 사용.
	// InWantedGrade 는 STAGE_CLEAR_GRADE_STAR_1 ~ 3 사이.
	
	//const int32 TimeLimitInSec = BladeIIGameImpl::GetStageDataStore().GetTimeLimitForStageClearGrade(InServerStageId, InWantedGrade);

	int32 FormattedHour = 0;
	int32 FormattedMin = 0;
	int32 FormattedSec = 0;
	//GetTimeFormatFromRawSecond(TimeLimitInSec, FormattedHour, FormattedMin, FormattedSec);
	// 위 결과에서 FormattedMin 과 Sec 만 사용한다. 이게 몇 시간 단위 이렇게 나오면 막장이겠지

	// 분, 초, 등급 정보 이렇게 넣어줌.
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
	// 위 결과에서 FormattedMin 과 Sec 만 사용한다. 이게 몇 시간 단위 이렇게 나오면 막장이겠지

	// 분, 초, 등급 정보 이렇게 넣어줌.
	if(bIsClear)
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_Grade_Clear_2")),
			FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)));
	else
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_Grade_2")), 
			FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)));
}

void AB2StageManager::OnTalkDialogStart(const int32 GroupID, const bool IsClear)
{
	//// 만일 이 때가 BeginPlay 타이밍이면 당장 TalkDialog 열리는 게 async 로딩 관련한 특별한 문제를 야기하지 않겠지만
	//// 내부에서 한 틱 이후에 TalkDialog 에 쓰이는 리소스를 로딩하는 게 있어서 결국 큰 리소스 flush 꺼리는 해 놓아야 한다.
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
	//	// 스토리 모드 스킵 
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
