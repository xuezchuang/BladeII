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

	StagePlayTimeAutoUpdateInterval = 0.2f; // 弥辆利牢 沥犬档俊 康氢阑 固摹瘤绰 臼瘤父 傈捧 UI 钎矫等 巴苞 搬苞 UI 钎矫俊辑狼 瞒捞甫 啊鞭利 临老 鞘夸啊 乐绢辑 累篮 蔼栏肺..

	CurrentPlayingDialogSound = NULL;
	
	CustomaizeBGM = false;
}

void AB2StageManager::PreBeginPlay()
{	
//	// 促弗 Actor 甸狼 BeginPlay 啊 阂府扁 傈俊 贸府登绢具 且 巴甸阑 咯扁俊 茎寂持绢 焊磊.
//#if WITH_EDITOR
//	if (GEditor && GEditor->bIsPlayWorldQueued) // PlayInEditor 扼档 肺厚甫 烹秦 柯 版快绰 咯扁甫 烹苞窍瘤 臼阑 巴.
//		StaticCacheStageData.Initialize();
//#endif
//	
//	ScenarioDialogManager.SetStageManager(this);
//
//	GetStageRewardManager().SetStageManager(this);
//	//[@AKI, 180213] 捞芭绰 技泼 救登霸 老窜 捞芭绰 狐廉具 且淀(捞霸 StageId甫官操唱....)
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!Utilities::FConnectionState::GetInstance().IsConnected()
//		// FConnectionState 父 八荤茄促搁 茄锅捞扼档 肺厚甘 凯绊 立加茄 捞饶俊绰 辑滚 立加茄 吧肺 贸府登绢 俊叼磐 汲沥阑 荤侩且 荐啊 绝澜. 
//		// 俊叼磐 棺 弊成 Open 阑 烹茄 敲饭捞俊辑狼 祈狼惑 standalone 葛靛 力傍
//		|| DevOnlyIsStandalonePlay()
//		)
//	{
//		StaticCacheStageData.SetStandaloneStageData(this);
//	}
//#endif
//
//	RefreshStagePlayTimeCheck(); // ExtraMap 惑包绝捞 墨款飘绰 矫累
//
//	// 扁鸥 促弗 单捞磐甸..
//	
//	SyncDataToUIDoc();
//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//	{
//		UB2UIDocHero* UIDocClass = UB2UIDocHelper::GetDocHero(PCI);
//		if (UIDocClass)
//		{ // 胶抛捞瘤 努府绢且 锭 (饭骇诀窍搁) 技泼登绰 单捞磐. 矫累且 锭 府悸.
//			UIDocClass->SetLastAcquiredSkillPoint(0);
//		}
//	}

	SubscribeEvents(); // BeginPlay 矫痢俊 阂府绰 捞亥飘啊 乐阑 抛聪 捞 矫痢俊 刚历 殿废秦 初酒具.
}

// Called when the game starts or when spawned
void AB2StageManager::BeginPlay()
{
	Super::BeginPlay();
	
	StageBeginHandling();

	BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = -1; // 捞 矫痢俊辑 歹 捞惑 鞘夸窍瘤 臼阑 挥歹矾 巢酒 乐栏搁 俊叼磐 敲饭捞俊 规秦啊 凳
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

	//// Note: 促弗 actor 甸狼 BeginPlay 捞傈俊 贸府啊 鞘夸茄 何盒篮 ABladeIIGameMode::PreBeginPlay 甫 曼绊.
	//// 捞巴捞 阂府绰 矫痢篮 窜鉴洒 actor 甸狼 BeginPlay 啊 贸府登绰 矫痢烙. 急饶包拌绰 沥狼登瘤 臼澜.

	//SyncDataToUIDoc(); // 肚 茄锅 秦林瘤 赣.

	//if (StaticCacheStageData.bPlayInExtraLevel) // ExtraLevel 肺爹 矫狼 贸府. SetReadyPlayingExtraMap 狼 馆措
	//{
	//	//ABladeIIGameMode::HideHUDCenterMessage(); // Extralevel 肺爹 措扁 矫累窍搁辑 淖带 季磐 皋矫瘤 掺扁. 趣矫 弊 荤捞俊 促弗 季磐 皋矫瘤啊 鞘夸秦柳促搁..?
	//	// ABladeIIGameMode::HideHUDCenterMessage 啊 绢瞒乔 救冈洒绰单 饭骇 函版 饶扼 弊率俊辑 某教秦 初篮 饭欺繁胶啊 狼固啊 绝扁 锭巩捞促. 绢驴 荐 绝瘤.
	//	UB2UIManager::GetInstance()->CloseAllMsgPopups();
	//	RetrieveDataForExtraMap();
	//}

	//PlayStageBGM();

	//UpdateStagePlayTime(false); // UI 钎矫甫 困秦 贸澜 茄锅 犬角洒 阂矾临 鞘夸啊 乐澜.
	//GetWorldTimerManager().SetTimer(AutoUpdateStagePlayTimeTH, this, &AB2StageManager::AutoUpdateStagePlayTimeCB, StagePlayTimeAutoUpdateInterval, false);

	//StageBeginClass<>::GetInstance().Signal(); // UI 率 脚龋.
}

void AB2StageManager::SyncDataToUIDoc()
{ // UI 俊辑 镜 单捞磐甸 技泼.
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
{ // 辑滚俊 嘿瘤 臼篮 配呕 standalone 敲饭捞 咯何
#if WITH_EDITOR
	if (IsStandalonePIE()){
		return true;
	}
#endif

	if (FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode()) { // 捞吧 技泼窍绰 霸 俩 加祈且 淀..
		return true;
	}

	if (GEngine)
	{
		FWorldContext* TheWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (TheWorldContext)
		{ // Open 疙飞俊 Standalone 牢磊. 抗甫 甸绢 Open LV01_02?Standalone?StageNum=2 捞繁 侥栏肺
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
	//		SpawnedMob->PreventMovementByStageEvent(); // StopOrRestoreAllWorldMobs 龋免 矫痢 捞饶俊 spawn 等 巴甸档 贸府.
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
	CheckStageClear(); // SpawnPool 辆丰绰 措眉肺 StageClear 肺 捞绢瘤唱 眠啊 炼扒捞 鞘夸且 荐 乐澜.
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
	// DestroyedPrevRef 绰 NULL 捞 酒囱 版快扼档 IsValidObj 窍瘤 臼阑 巴烙.
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
	if (!bCurrentLevelCleared) // 饭骇 辆丰 饶俊 棵 荐 乐栏骨肺 诀单捞飘 窍瘤 臼档废 林狼.
	{
		// 酒贰 UpdateStagePlayTime 俊 true 甫 持绢林搁 楷免 厘搁 犁积登绰 悼救 泪矫 矫埃 墨款泼捞 吝窜瞪 巴.
		// 瘤陛篮 楷免 档吝俊档 矫埃 墨款泼 窍霸 登菌澜.
		UpdateStagePlayTime(false);
	}
}
void AB2StageManager::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{ // 捞 Notify 啊 怖 傈眉 楷免 厘搁捞 敲饭捞登菌澜阑 舵窍瘤绰 臼绰促. 吝埃俊 吝窜沁歹扼档 捞犯霸 坷霸 瞪 巴.
	if (!bCurrentLevelCleared) // 饭骇 辆丰 饶俊 棵 荐 乐栏骨肺 林狼..
	{
		RefreshStagePlayTimeCheck();
	}
}

void AB2StageManager::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{

}

void AB2StageManager::PreloadAdditionalNecessaryInfo()
{
	// TAsset 肺爹窍绰 InfoAsset 甸捞 措惑烙. 扁夯利栏肺 AB2StageGameMode::PreloadAnyNecessaryInfo 俊辑 贸府窍绰 捞寇狼 巴甸

	GetStageRewardManager().PreloadAllRewardItemInfo(); // 榜靛尔.. 咯鸥 焊胶各 棱疽阑 锭狼 焊惑 酒捞袍 殿 肺爹.
}

void AB2StageManager::CheckStageClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::CheckStageClear"));
//
//	bool bSpawnPoolDone = true;
//
//	// 葛电 SpawnPool 甸捞 场车绰瘤 眉农 -> 老窜篮 SpawnPool 窍唱 啊沥捞瘤父 汲摹 啊瓷己篮 乐栏聪 巢败狄.
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
//	// 扁鸥 炼扒甸 积扁绰 版快..
//	//
//
//
//	bCurrentLevelCleared = bSpawnPoolDone; // 促弗 炼扒篮 鞘夸俊 蝶扼 眠啊.
//
//	if (bCurrentLevelCleared) // 饭骇捞 场抄 扒单 Stage 努府绢啊 登扁 困秦辑绰 酒流 粱 歹 眉农且 霸 乐澜.
//	{		
//		// 敲饭捞 磊眉绰 场车栏骨肺 墨款飘 吝窜窍搁辑 PlayTime 盎脚. ExtraMap 捞 乐促搁 瘤陛鳖瘤 墨款飘等 PlayTime 篮 傈崔瞪 巴. 
//		UpdateStagePlayTime(true); // 捞固 楷免 厘搁捞 矫累登搁辑 吝窜等 惑怕老 荐 乐促. (酒付 弊霸 措何盒老淀?)
//
//#if WITH_EDITOR
//		// 俊叼磐俊辑 ExtraMap 窜刀 敲饭捞且 锭俊 磊扁磊脚阑 促矫 肺爹窍瘤 臼绊 胶抛捞瘤 辆丰 贸府甫 窍档废 茄促.
//		bool bEDITOR_IsExtraMap = false;
//		if (GIsEditor && StaticCacheStageData.bHasExtraLevel && StaticCacheStageData.CachedCurrentStageInfoData)
//		{
//			UPackage* OuterMostPackage = GetWorld()->GetOutermost();
//			// 盔夯 FIleName 苞 ExtraMap 捞 鞍篮瘤 八荤. FileName 篮 版肺甫 器窃窍绰单 ExtraMap 篮 弊犯瘤 臼阑 巴捞骨肺 盖 缔何盒父 眉农.
//			// 拱沸 抗惑 寇狼 匙捞怪栏肺 牢秦 钧蹲窍霸 ExtraMap 贸府啊 瞪 荐档 乐摆瘤父 俊叼磐 捞酱捞绊 巩力啊 等促搁 陛规 舅霸 瞪 巴烙
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
//			// ExtraMap 阑 肺爹窃俊 蝶扼 促澜 甘俊 傈崔秦 临 单捞磐甫 技泼.. 秦具 窍绰单 捞 矫痢俊辑 窍搁 ExtraMapDelay 俊 蝶扼 割啊瘤 初摹绰 巴捞 积辨 荐 乐促.
//			// LoadExtraMap 矫痢俊 贸府窃.
//			//SetReadyPlayingExtraMap();
//			
//			// ExtraMap 傈券 皋矫瘤.. ExtraMapDelay 焊促 词柳 臼霸 唱客具 窃.
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
//				PreHandleLoadExtraMap(); // 眠饶 荤沥俊 蝶扼 PreHandleLoadExtraMap 篮 距埃 菊寸败龙 荐档.. 埃窜窍瘤绰 臼摆瘤父
//				LoadExtraMap(); // 官肺 肺爹
//			}
//		}
//		else
//		{
//			// Extra 甘鳖瘤 绊妨秦辑 柳楼 场抄 芭.
//			bStageCleared = true;
//			StageCompleteClass<>::GetInstance().Signal();
//			if (GetB2GameModeType(CachedGameMode) == EB2GameMode::CounterDungeon)
//			{//馆拜带傈 
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
//				RequestStageClear(); // 辑滚俊 胶抛捞瘤 努府绢 舅覆
//			}			
//		}
//
//		// Stage 辆丰 棺 Transition 措扁 矫 涝仿 阂啊瓷窍霸
//		ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//		if (B2PC)
//		{
//			// Stage 辆丰 棺 Transition 措扁 矫 涝仿 阂啊瓷窍霸
//			B2PC->SetStageEndNonCombatState(true);
//		}
//	}
}

void AB2StageManager::RequestStageClear()
{
	StaticCacheStageData.PlayTimeInMainMap = LastUpdatedPlayTime;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// 矫累矫俊 辑滚甫 烹窍瘤 臼疽栏搁 StageClear 眉农档 肺拿俊辑 官肺. IsConnected 眉农甫 窍瘤 臼绰单 楷搬等 惑怕俊辑 俊叼磐俊辑 橇府轰 敲饭捞甫 窍霸 瞪 荐 乐栏骨肺
	// 窜, 捻欧 滚傈俊辑绰 歹 捞惑 捞繁 规侥狼 努府绢 贸府甫 瘤盔窍瘤 臼绰促.
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
		FMath::TruncToInt(LastUpdatedPlayTime), // 努府绢 殿鞭阑 沥窍绰 荐摹牢单 宝洒 局概茄 惑炔俊辑 馆棵覆窍绰 巴焊促 啊瓷茄 亮霸 唱坷档废 磊福绰 霸 炒摆瘤..
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
//		DocBattleStage->SetStageClearGrade(StageClearGrade); // 胶抛捞瘤 辆丰 UI 喊 阂鸥绰 芭 割俺唱 唱坷绰瘤.
//		
//		/* Item */
//		int32 MultipleValue = GET_TUPLE_DATA(FB2ResponseClearStage::multiple_value_index, Info);
//		// 焊惑 酒捞袍.. 扁夯 酒捞袍 格废捞尔.. 焊惑榜靛(靛而榜靛 捞寇狼)档 乐栏搁 焊惑酒捞袍 屈怕肺 眠啊茄促.
//		GetB2ItemArray(FB2ResponseClearStage::items_index, Info);
//		// 焊胶炼阿 焊惑 酒捞袍
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
//		// 俊抛福 焊惑 酒捞袍
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
//		// 犁丰 焊惑 酒捞袍
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
//		// 魄概 厘厚 酒捞袍
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
//		// 魄概 俊抛福 酒捞袍
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
//		// 各棱绊 冻绢瘤绰 Drop Gold
//		int32 StageDropGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_drop_gold_index, Info);
//		// 胶抛捞瘤 努府绢 饶 焊惑栏肺 临荐 乐绰 Reward Gold
//		TArray<int32> StageRewardGolds = GET_TUPLE_DATA(FB2ResponseClearStage::reward_golds_index, Info);
//		// Reward Gold 醚钦 (x3 硅荐 利侩)
//		int32 StageRewardTotalGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_reward_gold_index, Info);
//		// QTE Gold 
//		int32 QTEGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_qte_gold_index, Info);		
//		// 弥辆 Total Gold (厘厚 魄概 Gold 固器窃)
//		int32 TotalGold = GET_TUPLE_DATA(FB2ResponseClearStage::total_added_gold_index, Info);
//		// 厘厚/俊抛福 魄概 Gold
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
//		// 焊惑 酒捞袍 府胶飘
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
//	//单捞磐 历厘. 焊惑 酒捞袍档 咯扁辑 甸绢皑.
//	BladeIIGameImpl::GetStageDataStore().ClearStage(Info, StaticCacheStageData.ServerStageID);
//
//	ABladeIIPlayerController* PlayerController = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	FMyAssetStore* MyAssetStore = PlayerController? &PlayerController->GetMyAssetStore() : NULL;
//	if (MyAssetStore)
//	{ // 努扼 楷免 侩捞瘤父 泅犁绰 角力 蔼阑 辑滚俊辑 罐疽绊 搬苞 拳搁俊 力措肺 谎妨林妨搁 咯扁 教农啊 鞘夸窃.
//		// 辟单 拌加 肋 救登绊 窍搁 FB2MessageInfoResponseClearStage 俊 total_added_drop_gold 尔 total_added_reward_gold 乐栏聪 弊吧肺促啊..
//		MyAssetStore->SyncCurrentTempGoldToAccountInfo(true); // Freeze 矫挪促.
//	}
//	OnStageClear();
}

void AB2StageManager::OnStageClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageManager::OnStageClear"));

	//if (CachedGameMode)
	//{ // 扁夯利牢 Auto 惑怕甫 掺绰 霸 酒聪扼 老矫利栏肺父 馋促. 扁夯 auto 惑怕绰 攫力电 历厘且 鞘夸啊 乐栏骨肺.
	//	CachedGameMode->TemporaryStopLocalPlayerAuto();
	//}

	//UB2UIDocBattleStage* DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//if (DocBattleStage)
	//{
	//	DocBattleStage->SetStagePlayTime(StaticCacheStageData.GetStageClearTime());
	//	if (!Utilities::FConnectionState::GetInstance().IsConnected())
	//	{
	//		DocBattleStage->SetStageClearGrade(STAGE_CLEAR_GRADE_STAR_3); // 楷搬 救等 惑怕扼搁 傲 促 焊捞霸 父顶 盲况霖促.
	//	}
	//}

	//// 胶抛捞瘤 努府绢 饶 焊惑 魂沥 殿阑 困茄 noti.
	//// StageEventDirector 率栏肺 notify 甫 窍扁 傈俊 捞率栏肺 刚历 秦具 EventWidget 檬扁拳 矫 曼绊且 单捞磐甸捞 盲况龙 巴烙.
	//auto* BladeIIPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(BladeIIPlayer ? BladeIIPlayer->GetController() : UGameplayStatics::GetLocalPlayerController(this));
	//BII_CHECK(BladeIIPlayer && B2PC);

	//// 胶抛捞瘤 努府绢 楷免肺 牢秦 皋葛府 惑铰 傈俊 啊瓷茄 巴甸阑 郴妨淋. 
	//// GC 甫 倒扁 锭巩俊 喉肺欧捞 粱 吧副 荐 乐促. 焊胶各 阜鸥 锭 喉肺欧捞 粱 芭浇赴促 酵栏搁 犁绊.
	//ManualUnloadOnStageClear(this); 
	//
	//// 捞 鸥捞怪 令澜 秦辑 StageResult UI 甫 归弊扼款靛 肺爹秦 夯促.. 档吝俊 浇陆 脾变 窃.
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
	// 矫累矫俊 辑滚甫 烹窍瘤 臼疽栏搁 StageClear 眉农档 肺拿俊辑 官肺. IsConnected 眉农甫 窍瘤 臼绰单 楷搬等 惑怕俊辑 俊叼磐俊辑 橇府轰 敲饭捞甫 窍霸 瞪 荐 乐栏骨肺
	// 窜, 捻欧 滚傈俊辑绰 歹 捞惑 捞繁 规侥狼 努府绢 贸府甫 瘤盔窍瘤 臼绰促.
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
	//		// colosseum comment : reward list 甫 曼炼窍咯 府况靛 鸥涝俊 蝶弗 焊惑阑 贸府且 荐 乐档废 累诀捞 鞘夸钦聪促.

	//		// 努府绢搁 楷免 UI绰 楷免窍搁辑 剁况淋
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
	//		// 菩硅搁 扁粮 康旷狼 啪 搬苞 UI啊 酒囱 DefeatUI 甫 剁况淋.
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
	//		// 焊惑贸府		
	//		for (auto Reward : MsgPtr->totems)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().AddTotem(Reward); // 配袍
	//		}

	//		for (auto Reward : MsgPtr->update_items)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE, Reward); // 配袍 炼阿 
	//		}

	//		// 纠贸府
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
	// 咯扁急 DisplayTime 阑 临 荐绰 绝绊 岿靛啊 货肺 肺爹 等 饶俊 流立 波具 窃. 捞霸 粱 巩力牢单, 咯扁辑 凯绢敌 扑诀阑 饭骇 狼粮利牢 霸烙内靛俊 某教秦 初扁档 亮瘤 臼绊 秦辑. 傲 饭骇 坷锹且 锭 扑诀甸篮 葛滴 秦力窍绰 侥捞 登绢具 且 淀.
	//FText TransitionMessage = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("InGameHUD_TrasitToExtra")));
	//ABladeIIGameMode::SetHUDCenterMessage(TransitionMessage, 0.0f, 1); // AnimIndex 1 锅. 捞扒 粱 窍靛内爹烙. 锅龋 撇赴促绊 奴 曼荤啊 老绢朝 巴档 酒聪绊..
}

void AB2StageManager::ExtraMapLoadingDelayTimerCB()
{
	GetWorldTimerManager().ClearTimer(ExtraMapLoadingDelayTimer);
	PreHandleLoadExtraMap(); // 眠饶 荤沥俊 蝶扼 PreHandleLoadExtraMap 篮 距埃 菊寸败龙 荐档.. 埃窜窍瘤绰 臼摆瘤父
	LoadExtraMap();
}

void AB2StageManager::PreHandleLoadExtraMap()
{
	// 盔贰 促弗 霸 乐菌绰单 瘤陛篮 叠尔 SaveDataForExtraMap 父 乐焙..

	SetReadyPlayingExtraMap(); // ExtraMap 栏肺 逞败临 单捞磐 历厘.
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
	//	StaticCacheStageData.PreExtraLastPlayedCharClass = PlayerCharacter->GetCurrentPlayerClass(); // 泅犁 努贰胶父 历厘. 促弗 窍唱绰 捞 蔼俊 蝶扼 DataStore 俊 乐绰 Main 趣篮 Sub 肺.
	//	auto* MyAssetStore = &PlayerController->GetMyAssetStore();
	//	if (MyAssetStore)
	//	{
	//		StaticCacheStageData.MainMapStartingGold = MyAssetStore->GetInitialGoldAmount();
	//		StaticCacheStageData.MainMapLastGold = MyAssetStore->GetCurrentTempGoldAmount();
	//	}

	//	UB2SkillRTManager* SkillRTManager = PlayerCharacter->GetSkillRTManager();
	//	if (SkillRTManager)
	//	{
	//		StaticCacheStageData.SetReadyPreExtraCombatVariables(PlayerCharacter, SkillRTManager); // 唱赣瘤 傈捧 惑怕 包访 函荐甸篮 蝶肺 葛酒辑..
	//	}
	//}	
}

void AB2StageManager::RetrieveDataForExtraMap()
{
	//// SetReadyPlayingExtraMap 俊辑 PreExtraLastPlayedCharClass 甫 技泼窍绰单 弊犯促绊 咯扁辑 GetPreExtraLastPlayedClass 甫 窍瘤 臼绰促. 弊扒 霸烙葛靛俊辑 Player Character spawn 矫 固府 秦 初疽绢具.
	//// Health 甸档 付蛮啊瘤. 弊扒 BladeIIPlayer 尔 SkillRTManager 率俊辑..

	//ABladeIIPlayer* PlayerCharacter = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//BII_CHECK(PlayerCharacter && PlayerCharacter->IsValidObj());

	//if (PlayerCharacter)
	//{
	//	auto* PlayerController = Cast<ABladeIIPlayerController>(PlayerCharacter->GetController());
	//	auto* MyAssetStore = PlayerController ? &PlayerController->GetMyAssetStore() : NULL;
	//	if (MyAssetStore)
	//	{
	//		MyAssetStore->SetStartingGold(StaticCacheStageData.MainMapStartingGold);
	//		MyAssetStore->AddGold(StaticCacheStageData.MainMapLastGold - StaticCacheStageData.MainMapStartingGold); // 泅犁 蔼阑 MainMapLastGold 肺.
	//	}
	//}

	//LastUpdatedPlayTime = StaticCacheStageData.PlayTimeInMainMap;
}

void AB2StageManager::BeginStageClearScene_Phase1()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageClear); // StageClear UI 俊辑 焊郴绰 脚龋俊 狼秦 BeginStageClearScene_Phase2 啊 矫累瞪 巴.

	//// StageClear Phase1 侩 墨皋扼 技泼.
	//UWorld* TheWorld = GetWorld();
	//ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (TheWorld && B2PC)
	//{
	//	if (!StageClearViewCam){
	//		// 辑宏饭骇 BP 甫 烹秦 技泼捞 登瘤 臼疽促搁 郴何利栏肺 流立 积己茄促.
	//		// 啊瓷茄 sky box 殿俊 啊府瘤 臼篮 鳖父 硅版阑 官扼杭 荐 乐霸. 弊犯促绊 呈公 钢搁 弊巴措肺 何悼家荐 沥剐档 巩力甫 稗阑 荐 乐促.
	//		// 荤侩窍绰 捞棋飘啊 岿靛谅钎俊父 align 登绰 芭 鞍酒辑 困唱 酒贰甫 焊瘤绰 给窍绊 荐乞阑 蜡瘤窍搁 fog 锭巩俊 钢府辑档 绢涤瘤啊 臼篮单 傲 酒贰辑 fog 甫 阐
	//		FTransform CamSpawnTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(200000.0f, 0.0f, 0.0f));
	//		FActorSpawnParameters CamSpawnParam;
	//		StageClearViewCam = Cast<ACameraActor>(TheWorld->SpawnActor(ACameraActor::StaticClass(), &CamSpawnTransform, CamSpawnParam));
	//	}
	//	UCameraComponent* StageClearViewCamComp = StageClearViewCam ? StageClearViewCam->GetCameraComponent() : NULL;
	//	if (StageClearViewCam && StageClearViewCamComp)
	//	{
	//		// 客捞靛 拳搁 措览.. 捞扒 弊成 静搁 承篮 拳搁俊辑 楷免 瓤苞甸捞 漏府霸 登绢辑 adaptive FOV 啊 鞘夸窃.
	//		StageClearViewCamComp->SetConstraintAspectRatio(false);
	//		StageClearViewCamComp->SetAspectRatioAdaptiveFOV(true);

	//		EnsureTargetToStageClearCam();

	//		StageClearPhase1_StartVictoryBGFx(); // 墨皋扼啊 积己登搁 VictoryBGFx 绰 官肺 矫累. 唱赣瘤 Wing 捞尔 Gem 篮 UIStageClear 率俊辑 脚龋甫 焊尘 巴.
	//	}
	//	
	//	// StageClearPhase1 篮 StageEventDirector 俊辑 clnematic mode 柳涝阑 窍瘤 臼栏骨肺 咯扁辑 利寸洒 持绢淋. 辆丰 矫痢捞聪 CinematicMode 俊辑 唱坷绰 吧 捌沥且 鞘夸绰 绝摆瘤..?
	//	B2PC->SetCinematicMode(true, false, true, false, true);
	//}

	//TurnOffAllFogs(); // 器弊 锭巩俊 盔窍绰 绢滴款 硅版捞 救唱坷聪 波初磊.
}

void AB2StageManager::StageClearPhase1_StartVictoryWingFx()
{// StageClear UI 俊辑狼 捞亥飘甫 烹秦 坷霸 瞪 巴.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearWingFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearWingFxTemplate() : NULL),
	//	StageClearWingFxComp, TEXT("StageClearWingFxRelativePos"), TEXT("StageClearWingFxRelativeRot"), TEXT("StageClearWingFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryGemFx()
{ // StageClear UI 俊辑狼 捞亥飘甫 烹秦 坷霸 瞪 巴.
	//UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//StageClearPhase1_StartVictoryFxCommon(
	//	StageClearGemFxComp ? NULL : (SomeInfo ? SomeInfo->GetStageClearGemFxTemplate() : NULL),
	//	StageClearGemFxComp, TEXT("StageClearGemFxRelativePos"), TEXT("StageClearGemFxRelativeRot"), TEXT("StageClearGemFxScale")
	//	);
}

void AB2StageManager::StageClearPhase1_StartVictoryBGFx()
{// StageClear UI 俊辑狼 捞亥飘甫 烹秦 坷霸 瞪 巴.
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
	//	// 客捞靛 拳搁 措览.. 捞扒 弊成 静搁 承篮 拳搁俊辑 楷免 瓤苞甸捞 漏府霸 登绢辑 adaptive FOV 啊 鞘夸窃.
	//	StageClearViewCamComp->SetConstraintAspectRatio(false);
	//	StageClearViewCamComp->SetAspectRatioAdaptiveFOV(true);
	//	
	//	EnsureTargetToStageClearCam(); // 何掺矾款 窿

	//	if (InOutCompForSet) // 墨皋扼贸烦 甘俊 硅摹等 巴捞 技泼登绢 乐促搁 弊吧 荤侩窍绊
	//	{
	//		InOutCompForSet->Activate();
	//	}
	//	else // 酒聪搁 郴何俊辑 积己
	//	{
	//		if (InPSTemplate)
	//		{
	//			// 墨皋扼肺何磐狼 惑措 困摹 棺 胶纳老 殿篮 抛胶飘甫 烹秦 沥秦辑 ini 俊 扁废.
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
	TurnBackOnAllFogs(); // Phase 1 俊辑 馋 器弊甸 促矫 难扁.

	//check(CachedGameMode);
	//const TArray<class AB2StageEventDirector*>& EventDirectors = CachedGameMode->GetEventDirectorArray();
	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	// 捞扒 StageClear UI 俊辑 坷绰 脚龋甫 罐绰促. 弥辆 搬苞芒苞 窃膊窍绰 铰府 器令 剁快扁
	//	// ESEV_Clear 措脚 芭扁辑 律俺柳 促弗 enum 栏肺 八荤窍霸 瞪 荐档 乐澜.
	//	if (CurrSED->GetDirectingEvent() == EStageEvent::ESEV_Clear){
	//		CurrSED->CustomBeginShow();
	//	}
	//}

	//EnsureTargetToStageClearCam(); // 何掺矾款 窿
	//
	//// 铰府 BGM 篮 搬惫 Phase2 客 窃膊..
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//BII_CHECK(B2GM);
	//if (B2GM)
	//{
	//	B2GM->PlayStageVictoryBGM(IsGoldBonusStage(), IsBossStage());
	//}
}

void AB2StageManager::EnsureTargetToStageClearCam()
{
	// 扁夯利栏肺绰 StageClearScene Phase1 矫累 矫 StageClearViewCam 栏肺 ViewTarget 茄锅父 技泼窍搁 登绰 扒单 捞繁历繁 墨皋扼瓤苞啊 乐绰 胶懦苞 窃膊 阜鸥甫 朝副 矫 弊霸 救 冈洒绰 老捞 乐促.
	//// 何掺反霸档 弊吧 力措肺 绊摹瘤 给秦辑 捞 内靛甫 蝶肺 弓绢辑 咯矾锅 龋免窃. 弊犯霸 秦辑 农霸 颊秦 杭 巴档 酒聪绊 秦辑 
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

	//	// 弊成 掺瘤 臼绊 visibility 甫 归诀秦 躇促啊 阐. 唱吝俊 促矫 难具 窍聪
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
		if (ExpFogElem.Key && ExpFogElem.Value){ // 扁粮俊 visible 沁带 版快父 促矫 囊促.
			ExpFogElem.Key->SetActorHiddenInGame(false);
		}
	}
	for (auto& AtmosFogElem : CachedAtmosFogVisMap)
	{
		if (AtmosFogElem.Key && AtmosFogElem.Value){ // 扁粮俊 visible 沁带 版快父 促矫 囊促.
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
	//[@SameLine, 180524] 焊胶各 鸥涝 胶迄矫 公炼扒 罚待 bgm 犁积 荐沥
	//x-10(BossStage)啊 酒囱 版快 霉bgm->焊胶bgm 函版饶 函版X
	//x-10(BossStage)狼 版快 霉bgm->焊胶bgm->焊胶bgm 函版 啊瓷

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
{ // 咯辨 烹秦 技泼等促搁 饭骇俊 硅摹等 局甸老 巴. StageClearPhase1_StartVictoryGemFx 殿俊辑 悼利 贸府甫 窍瘤 臼霸 等促.
	StageClearViewCam = InCamActor;
	StageClearWingFxComp = InPSWingComp;
	StageClearGemFxComp = InPSGemComp;
	StageClearBGFxComp = InPSBGComp;
}

void AB2StageManager::RefreshStagePlayTimeCheck()
{ // UpdateStagePlayTime 捞寇俊 蝶肺 墨款飘 矫累 矫痢 盎脚捞 鞘夸茄 版快 荤侩. 老辆狼 Start 唱 Resume 拜.
	UWorld* TheWorld = GetWorld();
	BII_CHECK(TheWorld);

	if (TheWorld)
	{
		PlayTimeLastCheckedCache = TheWorld->GetTimeSeconds();

		bCountingPlayTime = true; // Refresh 甫 茄 捞惑 墨款飘 矫累.
	}
}
void AB2StageManager::UpdateStagePlayTime(bool bStopCounting)
{
	//UWorld* TheWorld = GetWorld();
	//BII_CHECK(TheWorld);

	//if (TheWorld && bCountingPlayTime)
	//{ // 付瘤阜 PlayTimeLastCheckedCache 矫痢栏肺何磐 穿利 矫埃 盎脚.

	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//	bool bCinematicMode = B2PC && B2PC->IsInCinematicMode();

	//	if(!bCinematicMode)
	//		LastUpdatedPlayTime += TheWorld->GetTimeSeconds() - PlayTimeLastCheckedCache;

	//	PlayTimeLastCheckedCache = TheWorld->GetTimeSeconds();

	//	if (bStopCounting) // 捞 版快 Stop 捞唱 Pause 拜栏肺 荤侩.
	//	{
	//		bCountingPlayTime = false;
	//	}
	//	
	//	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	//	if (DocBS) // UI Doc 诀单捞飘.
	//	{
	//		DocBS->SetStagePlayTime((int32)LastUpdatedPlayTime);
	//	}
	//}
}

float AB2StageManager::GetStagePlayTime(bool bUpdateNow)
{
	if (bUpdateNow)
	{
		UpdateStagePlayTime(false); // 吝埃 烙狼 矫痢俊 诀单捞飘 窍绰 荤侩. 吝窜等 惑怕俊急 绢瞒乔 诀单捞飘 救 窃.
	}

	return LastUpdatedPlayTime;
}

void AB2StageManager::AutoUpdateStagePlayTimeCB()
{
	GetWorldTimerManager().ClearTimer(AutoUpdateStagePlayTimeTH);

	UpdateStagePlayTime(false); // 诀单捞飘 登搁辑 UIDoc 诀单捞飘 窍搁 舅酒辑 钎矫 盎脚 せ 吝窜 档吝俊绰 舅酒辑 畴墨款飘

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
//	// 泅犁 急琶等 DifficultyLevel 苞 Info 技泼 (郴瘤绰 辑滚 付胶磐 单捞磐) 俊辑 厚吩登绰 角力 利侩登绰 scale 蔼 府畔
//	return BladeIIGameImpl::GetStageDataStore().GetStageDifficultyScale(
//		StaticCacheStageData.ClientStageId, 
//		StaticCacheStageData.StageDifficultyLevel,
//		// B2GameModeType 逞败林绰 扒 M15 俊辑狼 烙矫.
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
	//		CurrMob->PreventMovementByStageEvent(); // PreventMovementByStageEvent 肺 秦具 AnimNotify 俊 狼秦 AllowMovement 瞪 锭 救 倒酒皑 ぇぇ
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
//	// 扁夯利栏肺绰 俊叼磐 敲饭捞 棺 辑滚 楷搬捞 救等 惑怕俊辑狼 橇府轰 敲饭捞 沥档俊 荤侩.
//	// 弥辆 霸烙俊辑 沥惑利牢 惑炔捞搁 荤侩且 扁瓷篮 酒聪瘤父 胶抛捞瘤 柳涝 流饶 匙飘况农啊 谗板促搁 老窜 敲饭捞绰 啊瓷秦具 窍骨肺 绢蠢 沥档 技泼篮 鞘夸窍促.
//
//	check(StageManager);
//
//	TotalExpOfCurrentStage = 1000; // 拱沸, 沥侥 技泼篮 酒丛.
//
//	// 老窜 WorldSettings 俊 技泼茄 蔼阑 快急 利侩 矫档. 肺爹 可记阑 林菌促搁 酒贰俊辑 弊霸 利侩瞪 巴. 拱沸 俺惯侩 技泼
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
//	// 俺惯 滚傈侩 胶抛捞瘤 锅龋 殿狼 可记阑 喉风橇赴飘 OpenLevel 可记 巩磊凯(Open 疙飞 牢磊)阑 烹秦 技泼且 荐 乐档废. 弥辆利栏肺绰 辑滚俊辑 啊廉客具 窃.
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
//			else if (TheWorldContext->LastURL.HasOption(TEXT("SN"))) // 漏篮 滚傈
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
//			else if (TheWorldContext->LastURL.HasOption(TEXT("DF"))) // 陋篮 滚傈
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
//			else if (TheWorldContext->LastURL.HasOption(TEXT("SM"))) // 陋篮 滚傈
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
//	// 窜刀 敲饭捞扼档 抄捞档俊 蝶扼 矫唱府坷 葛靛甫 焊瘤 给窍档废.
//	// 咯扁辑 抗寇 贸府 窍瘤 臼栏搁 惑困 抄捞档俊辑档 矫唱府坷 楷免阑 焊霸 瞪 荐 乐绰单 宝洒 俊叼磐 敲饭捞俊辑 扁瓷阑 钧蹲窍霸 犬牢且 家瘤啊 乐澜.
//	if (!IsScenarioModeAllowedDifficulty(StageDifficultyLevel))
//	{
//		bScenarioMode = false;
//	}
//
//	if (BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId > 0)
//	{
//		ClientStageId = BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId;
//		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = -1; // 茄锅 静绊 唱辑档 巢酒乐栏搁 咯矾葛肺 帮鄂
//	}
//
//	CachingStageInfoData();
//
//	if (CachedCurrentStageInfoData)
//	{
//		bHasExtraLevel = CachedCurrentStageInfoData->bHasExtraMap;
//
//		//ExtraMap篮 World啊 促弗 俺充栏肺 焊惑俊 措秦 眠啊 汲沥秦拎具 茄促.(辑滚 单捞鸥 犬沥矫鳖瘤 烙矫)
//		StageReward.SetTotalStageRewardGold(bPlayInExtraLevel ? 700 : 500);
//
//		UB2ItemInfo* ItemDataTable = StageReward.GetItemInfoTable();
//		if (ItemDataTable)
//		{
//			// 老窜篮 胶抛捞瘤俊辑绰 窍唱狼 酒捞袍父捞 靛酚等促绊 窍磊. (扁裙篮 弥措 2俺)
//			// StageReward.SetDefeatBossRewardItem(ItemDataTable->GetRandomItemRefID());
//		}
//	}
//
//#if WITH_EDITOR // For use play-in-editor stage settings
//	if (GIsEditor){
//		bStandalonePIE = true;
//	}
//
//	// 捞巴档 俊叼磐 橇府轰 瞒盔俊辑
//	FB2Item DummyReward;
//	DummyReward.ItemRefID = 1000100;
//	StageReward.SetDefeatBossRewardItem(DummyReward);
//#endif
}

void FCacheStageEssentialDataToKeep::Initialize()
{
	StageReward.Initialize();

	//
	// 览翠 罐绊 蔼 技泼 傈 捞霸 阂府骨肺 Request 惑怕 某教 幅 函荐甸篮 咯扁辑 檬扁拳 窍搁 救凳. 鞘夸窍搁 积己磊俊辑..
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

	// EResurrectGameModeType::EResurrectType_OnlyClientMode绰 府胶飘俊辑 力寇秦霖促.
	//ResurrectionList.Empty();
	//for (int32 i = (int32)EResurrectGameModeType::EResurrectType_Stage; i < (int32)EResurrectGameModeType::EResurrectType_Max; ++i)
	//	ResurrectionList.Add((EResurrectGameModeType)i, FResurrectData());

	//ServerSelectedStageBuff.Empty();

	/*
	//UI俊辑 弃傅阑 规瘤 窍扁 困秦 Doc俊 单捞磐教农
	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		DocBattle->SetMaxResurrectionChance(MaxResurrectionChance);
		DocBattle->SetRemainingResurrectionChance(RemainingResurrectionChance);
		DocBattle->SetResurrectionCost(ResurrectionCost);
	}
	*/
#if WITH_EDITOR // For use play-in-editor stage settings
	bStandalonePIE = GIsEditor; // 俊叼磐俊辑绰 肺厚甫 烹秦 肺爹登绢 辑滚 览翠阑 罐篮 版快 促矫 false 技泼捞 瞪 巴.
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
	//// AllPCClassNeedCleanInven 篮 厚绢乐瘤 臼酒具 富捞 登瘤父 辑滚鳖瘤 坷埃 览翠捞 绢痘霸 瞪瘤俊 措秦辑 check 甫 吧绊 酵瘤绰 臼焙.
	//// 牢亥配府甫 厚况尘 鞘夸啊 乐绰 努贰胶甸阑 固府 痢八秦辑 牢亥配府 芒 艾阑 锭 急琶登绢 乐绰 努贰胶俊 馆康茄促.
	//const bool bIsMainClassNeedToCleanInven = (AllPCClassNeedCleanInven.Find(RequestedMainPlayerCharClass) != INDEX_NONE);
	//const bool bIsSubClassNeedToCleanInven = (AllPCClassNeedCleanInven.Find(RequestedSubPlayerCharClass) != INDEX_NONE);
	//// Main/Sub 笛 促 宝满歹扼档 促弗 急琶 救等 局狼 inventory 啊 瞒 乐栏搁 弊巴档 涝厘阂啊烙.
	//const EPCClass FallbackInvenCleanCharClass = (AllPCClassNeedCleanInven.Num() > 0) ? AllPCClassNeedCleanInven[0] : EPCClass::EPC_Gladiator;
	//PCClass = bIsMainClassNeedToCleanInven ? RequestedMainPlayerCharClass :
	//	(bIsSubClassNeedToCleanInven ? RequestedSubPlayerCharClass : FallbackInvenCleanCharClass);
	//if (PCClass == EPCClass::EPC_End) { // 捞繁 老篮 绝绢具摆瘤父..
	//	PCClass = EPCClass::EPC_Gladiator;
	//}

	//// 弊府绊 努贰胶 急琶捞 登菌促搁 角力肺 瞒 乐绰 努贰胶 徘鳖瘤..
	//const bool bIsNeedToClean = UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, PCClass);
	//if (!bIsNeedToClean)
	//	return false;
	//*/

	//if (AllPCClassNeedCleanInven.Num() > 0)
	//{
	//	// 捞 矫痢俊 PreLoadingScreen 捞 凯妨 乐阑 巴捞骨肺 摧档废.
	//	ClosePreLoadingScreenAllClass<>::GetInstance().Signal();
	//	CancelStartGameStageClass<>::GetInstance().Signal();
	//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal(); // 扁鸥 傈捧 夸没 贸府啊 阂啊茄 版快 啊瓷茄 馆汗 傈捧 夸没阑 秒家秦具 窃.

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
	// 老窜篮 酒公巴档 救窃 
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
		DocBS->SetSelectedStageBuffs(ServerSelectedStageBuff); // Server Response俊辑 牢沥等 Buff父 UIDoc俊 盎脚
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
	//// [180712_YJ] 抄捞档 On/OFF扁瓷 眠啊.
	//// 趣矫唱 碍力肺 阜腮抄捞档俊 柳涝沁阑版快俊, 俊矾扑诀 龋免 棺 肺厚肺 捞悼.
	//// 譬配府倔 绊妨 救窃.
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

	//bStageRequestedWithSkipStoryCinematic = bSkipStoryCinematic; // 览翠捞 柯 饶 bScenarioMode 咯何 魄喊俊 静烙.

	//// 咯扁辑狼 RepeatBattleState 绰 Response 啊 坷搁 技泼茄促.

	////ToDo 辑滚肺 涝厘夸没 菩哦阑 焊辰促. (胶抛捞瘤Id, 皋牢/辑宏 某腐磐 努贰胶, 扁鸥 酒捞袍(榜靛诀, 酒捞袍眠啊鼻殿殿)
	//
	//UE_LOG(LogBladeII, Log, TEXT("StageRequest"));
	////[@AKI, 180214] RequestServerStageId啊 0栏肺 甸绢坷绰 泅惑 锭巩俊 ensure吧绢 初澜. 趣矫 咯扁俊 吧副 版快 救堡劳俊霸 楷遏 官恩.
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

	TArray<EStageBuffType> ThisRequestBuff = ServerSelectedStageBuff; // 趣矫扼档 檬扁 急琶苞 付蛮啊瘤肺 瘤加秦辑 持绢具 茄促搁 AllSelectedStageBuff 肺
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

	//// 贸澜 敲饭捞 矫俊绰 true. 滴锅掳 捞惑老 锭俊绰 胶配府葛靛俊辑 荤侩磊啊 胶配府逞辫阑 急琶沁芭唱, 葛情葛靛(老馆 捞惑狼 抄捞档)捞搁 false.
	//bScenarioMode =
	//	// 抄捞档啊 崔扼脸阑 矫 ServerStageID 档 崔扼瘤霸 登骨肺 霉 敲饭捞 矫俊绰 IsStageClearedOnce 啊 促矫 false 啊 等促. 蝶扼辑 IsScenarioModeAllowedDifficulty 绰 && 炼扒捞 登绢具 窃.
	//	IsScenarioModeAllowedDifficulty(InStageDifficulty) && 
	//	(StageDataStore.IsStageClearedOnce(ServerStageID) == false || !bStageRequestedWithSkipStoryCinematic);

	//SetStageBasisData_CommonInternal(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass);

	//if (InRepeatBattleState)
	//	RepeatBattleState = *InRepeatBattleState;
}

void FCacheStageEssentialDataToKeep::SetStageBasisData_CounterDungeon(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass)
{
	Initialize();

	// 馆拜带傈篮 ClientId客 ServerId啊 鞍澜.
	ServerStageID = FServerStageID(InClientStageId);
	bScenarioMode = false;

	SetStageBasisData_CommonInternal(InClientStageId, InStageDifficulty, MainCharClass, SubCharClass);
}

void FCacheStageEssentialDataToKeep::SetStageBasisData_CommonInternal(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass)
{
	// ServerStageID 绰 捞芭 捞傈俊 技泼登绢 乐绢具 窍绰单... TutorialGameMode 鞍篮 抗寇 惑炔捞 乐绢辑 眉农绰 给窍摆焙 ぱぱ
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
	ServerStageID = FServerStageID(); // 老窜 捞吧 府悸窍绰 霸 俩 吝夸.
	bScenarioMode = true; // 娟绰 扁夯蔼 true 啊 歹 绢匡府摆促.
	bStageRequestedWithSkipStoryCinematic = false;
	ServerSelectedStageBuff.Empty();
	bHasExtraLevel = false;
	//[@AKI, 180208] Extra Map 甸绢艾促坷搁 檬扁拳甫 救矫虐绰 巩力(某浆 登绢 乐绰 儡犁啊 巢酒乐澜)
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
	//AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(InCurrentGameMode); // 老馆利牢 版快 咯扁肺 甸绢坷绰 扒 LobbyGameMode. 肺厚俊辑 胶抛捞瘤 矫累 滚瓢 穿福绊 辑滚 览翠 矫 努扼捞攫飘 甘 肺爹 傈 蔼 傈崔窍绰 何盒.
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

	//// FB2StartStageInfo 俊 弥辆 倾啊等 StageBuff 沥焊啊 甸绢乐促搁 RequestedStageOption 措脚 弊吧肺 秦辑 SetStageResponseData 俊辑 技泼阑
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

	//// 胶抛捞瘤 矫累苞 窃膊 AccountInfo 盎脚 鞘夸肺 牢秦 ClientDataStore/StageDataStore 率俊档.
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

	//// 楷免 淖栏搁 肺弊傈价
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
	//// 馆拜带傈篮 GetServerStageID肺 ServerStageID甫 啊廉棵 荐 绝栏骨肺, SetStageBasisData_CounterDungeon() 栏肺 蝶肺 贸府
	//SetStageBasisData_CounterDungeon(RequestedClientStageId, RequestedStageDifficulty, RequestedMainPlayerCharClass, RequestedSubPlayerCharClass);

	//TArray<FB2Item> EmptyArray;
	//SetStageResponseData(Token, 0, 0, EmptyArray, 0, EmptyArray);

	//StartBladeIICounterDungeon(InCurrentGameMode, ClientStageId);
}

// colosseum comment : bUseGem 力芭
void FCacheStageEssentialDataToKeep::RequestStartHeroTower(AGameMode* InCurrentGameMode, int32 nStartFloor, EPCClass MainCharClass, EPCClass SubCharClass, FString szToken)
{
	//RequestedMainPlayerCharClass = MainCharClass;
	//RequestedSubPlayerCharClass = SubCharClass;

	//// 摸爱绊 付胶磐单捞磐俊辑 酒捞叼 掘绢客尔
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
//		// colosseum comment : daily_left_free_try_count 力芭
//		//pDoc->m_ptrGetHeroTower->daily_left_free_try_count = msgPtr->left_free_ticket;
//		pDoc->m_ptrGetHeroTower->daily_try_count = msgPtr->daily_enter_count;
//		// colosseum comment : current_gem 力芭
//		//BladeIIGameImpl::GetClientDataStore().SetGemAmount(msgPtr->current_gem);
//		pDoc->m_bIsClearHeroTower = false;
//	}	
//	
//#if !UE_BUILD_SHIPPING
//	extern bool gUseHeroTowerResetNoLoading;
//#endif
//	AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(InCurrentGameMode);
//	if (pGM 
//#if !UE_BUILD_SHIPPING // 康旷狼 啪篮 促澜 摸 官肺 捞悼矫 肺爹 绝捞 胶抛捞瘤 府悸阑 窍绰单 捞霸 俺惯档吝 眠啊登绰 内靛 措览捞 救登搁 巩力甫 老栏虐扁 浆快骨肺 漂沥 巩力 惯积矫 抛胶飘甫 困秦 敲贰弊甫 力傍.
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
	//// 配袍靛而捞 100% 捞聪 碍力肺 胶迄且巴 矛败淋
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
//	////UI俊辑 弃傅阑 规瘤 窍扁 困秦 Doc俊 单捞磐教农
//	//if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
//	//{
//	//	DocBattle->SetResurrectGameModeType(ResurrectModeType);
//	//	DocBattle->SetMaxResurrectionChance(ResurrectionList[ResurrectModeType].MaxResurrectionChance);
//	//	DocBattle->SetRemainingResurrectionChance(ResurrectionList[ResurrectModeType].RemainingResurrectionChance);
//	//	DocBattle->SetResurrectionCost(ResurrectionList[ResurrectModeType].ResurrectionCost);
//	//}
//}

void FCacheStageEssentialDataToKeep::DecreaseRemainingResurrectionChance(EResurrectGameModeType ResurrectModeType)
{ 
	if (ResurrectModeType == EResurrectGameModeType::EResurrectType_OnlyClientMode)
		return;

	//BII_CHECK(ResurrectionList.Contains(ResurrectModeType));

	//ResurrectionList[ResurrectModeType].DecreaseResurrectionChance();
	//SetResurrectionCostByLookup(ResurrectModeType, ResurrectionList[ResurrectModeType].MaxResurrectionChance, ResurrectionList[ResurrectModeType].RemainingResurrectionChance);

	//auto* BattleDoc = UB2UIDocHelper::GetDocBattle();
	//if (BattleDoc)
	//	BattleDoc->SetRemainingResurrectionChance(ResurrectionList[ResurrectModeType].RemainingResurrectionChance);
}

//void FCacheStageEssentialDataToKeep::SetResurrectionCostByLookup(EResurrectGameModeType ResurrectModeType, int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance)
//{
//	//const int32 NextResurrectionNum = InMaxResurrectionChance - InRemainingResurrectionChance + 1;
//	//// 何劝 瞒荐 喊肺 沥秦廉 乐绰 厚侩阑 ClientDataStore 俊辑 疯诀.
//
//	//ResurrectionList[ResurrectModeType].ResurrectionCost = BladeIIGameImpl::GetClientDataStore().GetResurrectionCost(ResurrectModeType, NextResurrectionNum);
//	//
//	//// UIDoc俊档 悸泼
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
	//{// StructToSet 篮 泅犁 见绢乐绰 怕弊 某腐磐 惑怕 历厘侩捞扼 AmIMainCharacter 客 馆措狼 蔼阑 荤侩秦辑 技泼.
	//	StructToSet->MaxHealth = CurrentActivePlayer->AmIMainCharacter() ? PreExtraSubPlayerLastMaxHP : PreExtraMainPlayerLastMaxHP;
	//	// 荤角惑 ExtraMap 肺爹 流傈狼 CurrentHealth 蔼老 扒单 LastHealth 啊 迭洒 鞘夸茄啊?
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
{ // 牢霸烙俊辑 馆汗傈捧 stop 窍妨搁 咯扁辑 府悸 饶 StageManager 俊辑 眠啊 贸府 鞘夸.
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
	//	{//巢篮 榜靛 裙垫 啊吝摹甫 葛滴 歹茄促.
	//		LeftGoldWeight += CheckSpawnPool->GetLeftGoldWeight(StageManager->GetCurrentClientStageId(), StageManager->GetStageDifficultyLevel());
	//	}

	//	for (auto* TreasureChest : B2GM->GetTreasureChestArray())
	//	{
	//		LeftGoldWeight += TreasureChest->RewardGoldWeight;
	//	}

	//	if (LeftGoldWeight > weight)
	//	{//巢篮 榜靛樊阑 困俊辑 备茄 啊吝摹 醚钦栏肺 唱穿绢 扁霖蔼阑 沥茄促.
	//		int32 randBarometer = (TotalStageRewardGold - SumCurrentRewardGold) / LeftGoldWeight * weight;

	//		rtnValue = FMath::Max(1, (int32)(randBarometer * (1.f + FMath::FRandRange(-0.2f, 0.2f))));
	//		rtnValue = (rtnValue + SumCurrentRewardGold > TotalStageRewardGold) ? TotalStageRewardGold - SumCurrentRewardGold : rtnValue;

	//		SumCurrentRewardGold += rtnValue;
	//	}
	//	else
	//	{//巢篮 醚 啊吝摹啊 泅犁 备且 啊吝摹焊促 利阑 版快 巢篮 榜靛 葛滴 馆券
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
//	//CommonSpawnParam.bDeferConstruction = true; // bDeferConstruction 苞 bDeferPostSpawnInitialize 绰 SpawnDropItem 档吝俊 磊悼栏肺 荤侩瞪 巴.
//	CommonSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;		
//	
//	bool IsQTEDeath = DelaySpawnRefActor ? DelaySpawnRefActor->IsInQTEState() : false;
//
//	uint32 GoldWieght = IsQTEDeath ? DropCoinCountForQTE : DropItemInfo.RewardGoldWeight;
//	uint32 NumGoldCoin = GoldWieght; //喊促弗 捞具扁啊 绝栏搁 weight 1寸 1俺究
//	uint32 NumDropItems = NumGoldCoin + (DropItemInfo.bCanDropItem ? 1 : 0);
//
//#define AVAILABLE_EDITOR_PREVIEW_SPAWNITEM 1
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//	//俊叼飘 橇府轰 侩档 - SpawnPool肺 胶迄等 各捞 酒囱 版快.
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
//		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f); // 咯扁急 Rotation 父 备茄促. Location 篮 delay 啊 乐阑 版快 SpawnRefActor 狼 诀单捞飘 等 困摹蔼阑 啊廉皑.
//
//		StageManager->DeferredSpawnDropGold(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? 10 : 
//#endif
//			// SpawnTimeDelay 绰 spawn 何窍 盒魂阑 困秦 眠啊 炼例阑 且 荐档 乐促.
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
//		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f); // 咯扁急 Rotation 父 备茄促. Location 篮 delay 啊 乐阑 版快 SpawnRefActor 狼 诀单捞飘 等 困摹蔼阑 啊廉皑.
//
//		StageManager->DeferredSpawnDropItem(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? DropItemInfo.TestNumSpawnItemRefID :
//#endif
//			// SpawnTimeDelay 绰 spawn 何窍 盒魂阑 困秦 眠啊 炼例阑 且 荐档 乐促.
//			GetDefeatBossRewardItem().ItemRefID, CommonSpawnParam, DelaySpawnRefActor, SpawnLocation, SpawnRotation, DropItemInfo.RewardSpawnDelay);
//		StageManager->DeferredSpawnDropItem(
//#if AVAILABLE_EDITOR_PREVIEW_SPAWNITEM
//			bEditorPreview ? DropItemInfo.TestNumSpawnItemRefID :
//#endif
//			// SpawnTimeDelay 绰 spawn 何窍 盒魂阑 困秦 眠啊 炼例阑 且 荐档 乐促.
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

		// 捞霸 ID 喊肺 叠妨 乐绰 单捞磐甫 促 肺爹窍绰 备炼扼 ItemInfo 肺爹阑 唱传 第肺绰 弊府 鼻厘登绰 荤侩篮 酒囱单
		// Reward 酒捞袍捞扼搁 荤角惑 措何盒狼 府家胶甸捞 鞘夸窍变 且 芭 鞍促. 老窜 出狄. 粱 扯厚啊 等促 酵栏搁 PreloadClassAssets 甫 裹林喊肺 唱穿绰 吧 秦杭 荐档.
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
	//	if (bMaintainBattleUI) // 扁夯利栏肺 捞率 widget 汲沥俊 bHideBattleRelatedWidgets 啊 乐阑 扒单 蝶肺 难绊 酵篮 版快甫 困秦..
	//	{
	//		UIMgr->HideBattleRelatedWidgets(false);
	//	}
	//}

	//UB2UIDocScenarioDialog* DocSDG = UB2UIDocHelper::GetDocScenarioDialog();
	//if (DocSDG)
	//{
	//	// bLastBattleUIHidden 蔼俊 蝶扼 措拳芒 困摹啊 粱 官拆 芭.
	//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(CachedStageManager));
	//	if (B2PC && B2PC->IsInCinematicMode())
	//	{
	//		DocSDG->SetbLastBattleUIHidden(B2PC->IsHUDHidingCinematicMode()); // CinematicMode 汲沥篮 快急矫 凳.
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
		// 辆丰矫 INDEX_NONE
		// 馆靛矫 ForceSkip() 捞傈俊 龋免 登绢具 窃 - DialogInstance啊 nullptr 凳
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
	// DialogCharacterInfo 啊 Portrait material 档 甸绊 乐瘤父 弊扒 DialogCharacterInfo 俊辑 流立 波郴坷瘤 臼绊 InfoSet 抛捞喉俊辑 喊档 牢磐其捞胶肺 啊廉咳.
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
	UIDocDialog->SetThisDialogShowingTime(DialogLineInfo.ShowingTime); // Dialog UI 率俊辑 曼绊父.. 鸥捞赣绰 咯扁辑 倒赴促. SetDialogText 傈俊 捞芭何磐..
	UIDocDialog->SetDialogText(BladeIIGetLOCText(B2LOC_CAT_DIALOGTEXT, DialogLineInfo.DialogLineCodeName.ToString()));
	
	UIDocDialog->SetShowNextButton(
		// 促澜 临捞 乐阑 锭俊 NextButton 阑 焊牢促. 绢瞒乔 傈眉 康开滚瓢捞唱 炼弊父 NextButton 捞唱 开且 磊眉绰 悼老且 芭..
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
	//		else if (bPausedDialog) // 角力肺 Pause惑怕老 锭  - Pause 捞饶 角青等 Dialog Sound绰 Pause客 包拌绝捞 拌加 角青登骨肺 被捞 Resume矫懦 鞘夸 绝促
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
	//// 角力肺 促澜 扼牢捞 乐绰 版快, 官肺 逞绢啊扁 傈 Delay 甫 眉农
	//if (CachedStageManager && DialogInfoInstace 
	//	&& DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex)
	//	&& DialogInfoInstace->DialogSequence.IsValidIndex(CurrentDialogLineIndex + 1))
	//{
	//	const FDialogLine& CurrDialogSequence = DialogInfoInstace->DialogSequence[CurrentDialogLineIndex];
	//	if (CurrDialogSequence.DelayToNextLine > 0.0f)
	//	{ // GoToNextLineTimerHandle 倒妨静扁
	//		CachedStageManager->GetWorldTimerManager().SetTimer(GoToNextLineTimerHandle, CachedStageManager, &AB2StageManager::NextDialogLineImmediateWrapper, CurrDialogSequence.DelayToNextLine, false);

	//		// ForceSkip 鞍篮 扒 窍搁 救登绊 泪矫 见扁扁父 窍磊.
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
	//	NextDialogLineImmediate(); // 促澜 扼牢捞 绝绰 版快扼搁 skip 贸府啊 瞪 巴.
	//}
}

void FB2ScenarioDialogManager::NextDialogLineImmediate()
{
//	if (CachedStageManager)
//	{
//		CachedStageManager->GetWorldTimerManager().ClearTimer(GoToNextLineTimerHandle);
//	}
//
//	// NextDialogLine 俊辑 delay 矫淖带 版快. hide 汗备甫.
//	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//	UB2UIScenarioDialog* DialogUI = UIMgr ? UIMgr->GetUI<UB2UIScenarioDialog>(UIFName::ScenarioDialog) : NULL;
//#if !UE_BUILD_SHIPPING
//	extern bool gbHACKInGameUIHiddenByDevCommand; // UI 见扁绰 摹飘 借绰单 唱棵 荐 乐栏聪 捞巴档 绊妨甫..
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
	StopCurrentDialogSound(); // 吝窜何磐
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
	// 叼官捞胶 抛胶飘 殿阑 困秦 Gold spawn 厚啦阑 炼例且 荐 乐霸 窃.

	float FinalDropGoldSpawnRate = FMath::Clamp(GCheat_DropGoldSpawnRate, 0.0f, 1.0f);
	
	float ThisSpawnChanceInv = FMath::FRandRange(0.0f, 1.0f);
	if (ThisSpawnChanceInv > FinalDropGoldSpawnRate){
		return;
	}
#endif

	// 官肺 Spawn 捞扼搁 咯扁肺 逞绢柯 SpawnLocation/Rotation 蔼阑 静摆瘤父 delay 登绰 版快绰 DelaySpawnLocRefActor 狼 困摹甫 荤侩窍霸 瞪 巴.

	if (SpawnTimeDelay > 0.0f)
	{
		FDelayedSpawnDropItemInfo NewDelayInfo;
		GetWorldTimerManager().SetTimer(NewDelayInfo.DelayTH,
			FTimerDelegate::CreateUObject(this, &AB2StageManager::SpawnDropGold, Amount, SpawnParam, SpawnLocation, SpawnRotation, DelayedDropSpawnSerialNumber, false),
			SpawnTimeDelay, false);
		
		NewDelayInfo.DelaySpawnLocRefActor = DelaySpawnLocRefActor;
		DelayedDropSpawnInfoMap.Add(DelayedDropSpawnSerialNumber, NewDelayInfo);
		++DelayedDropSpawnSerialNumber; // 老访锅龋扼 拌加 刘啊
	}
	else
	{ // 官肺 spawn 且 锭俊绰 SpawnTimerMapKey 蔼阑 澜荐肺
		SpawnDropGold(Amount, SpawnParam, SpawnLocation, SpawnRotation, -1, InQTE);
	}
}

void AB2StageManager::DeferredSpawnDropItem(int32 ItemRefID, const struct FActorSpawnParameters SpawnParam, ABladeIICharacter* DelaySpawnLocRefActor, const FVector& SpawnLocation, const FRotator SpawnRotation, float SpawnTimeDelay)
{
	// 官肺 Spawn 捞扼搁 咯扁肺 逞绢柯 SpawnLocation/Rotation 蔼阑 静摆瘤父 delay 登绰 版快绰 DelaySpawnLocRefActor 狼 困摹甫 荤侩窍霸 瞪 巴.

	if (SpawnTimeDelay > 0.0f)
	{		
		FDelayedSpawnDropItemInfo NewDelayInfo;
		GetWorldTimerManager().SetTimer(NewDelayInfo.DelayTH,
			FTimerDelegate::CreateUObject(this, &AB2StageManager::SpawnDropItem, ItemRefID, SpawnParam, SpawnLocation, SpawnRotation, DelayedDropSpawnSerialNumber),
			SpawnTimeDelay, false);

		NewDelayInfo.DelaySpawnLocRefActor = DelaySpawnLocRefActor;
		DelayedDropSpawnInfoMap.Add(DelayedDropSpawnSerialNumber, NewDelayInfo);
		++DelayedDropSpawnSerialNumber; // 老访锅龋扼 拌加 刘啊
	}
	else
	{ // 官肺 spawn 且 锭俊绰 SpawnTimerMapKey 蔼阑 澜荐肺
		SpawnDropItem(ItemRefID, SpawnParam, SpawnLocation, SpawnRotation, -1);
	}
}

void AB2StageManager::SpawnDropGold(int32 Amount, const FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey, bool InQTE)
{
	//FTransform FinalSpawnTransform = SpawnDropDelayHandlingCommon(InitialSpawnLocation, InitialSpawnRotation, DelaySpawnInfoMapKey); // Delay ref actor 啊 乐绰 版快 秦寸 咀磐狼 困摹甫 荤侩.

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
	//{ // 被捞 Consumables 秒鞭篮 酒聪瘤父 ConsumingAmount 甫 烹秦 傈崔..
	//	NewDropItem->SignificantData.ConsumingAmount = Amount;
	//}
}

void AB2StageManager::SpawnDropItem(int32 ItemRefID, const FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey)
{
	//FTransform FinalSpawnTransform = SpawnDropDelayHandlingCommon(InitialSpawnLocation, InitialSpawnRotation, DelaySpawnInfoMapKey); // Delay ref actor 啊 乐绰 版快 秦寸 咀磐狼 困摹甫 荤侩.

	//if (ItemRefID <= 0) // 角力肺 沥焊甫 掘绢坷瘤 给茄 惑炔烙. 
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
	//{ // 捞巴捞 鸥捞赣甫 烹秦 阂妨柳 版快 鸥捞赣 努府绢
	//	if (ThisInfoForDeferCall->DelayTH.IsValid())
	//	{
	//		GetWorldTimerManager().ClearTimer(ThisInfoForDeferCall->DelayTH);
	//	}

	//	if (ThisInfoForDeferCall->DelaySpawnLocRefActor)
	//	{
	//		// DelaySpawnLocRefActor 狼 荤噶 矫 LifeSpan 捞 Delay 焊促 陋促搁 Valid 窍瘤 臼阑 巴捞促. 
	//		// NotifyForgetACharacter 俊辑 趣矫 DelaySpawnLocRefActor 啊 坷瘤 臼疽绰瘤 八荤秦辑 力芭且 鞘夸啊 乐澜.
	//		checkSlow(ThisInfoForDeferCall->DelaySpawnLocRefActor->IsValidObj());
	//		// DelaySpawnLocRefActor 狼 泅 困摹肺 盎脚.
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
		// 咯扁急 老矫利栏肺 掺绰 霸 酒聪扼 扁夯利牢 auto 惑怕甫 阐.
		TogglePlayerAutoStateClass<>::GetInstance().Signal();
	}
	*/
	SyncDataToUIDoc(); // 粱 镜单绝捞 腹篮 单捞磐啊 技泼登变 窍瘤父..
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
	// 胶抛捞瘤俊辑 漂沥 Star 殿鞭阑 罐扁 困茄 矫埃 扁霖 救郴 咆胶飘 府畔. UI 俊辑 荤侩.
	// InWantedGrade 绰 STAGE_CLEAR_GRADE_STAR_1 ~ 3 荤捞.
	
	//const int32 TimeLimitInSec = BladeIIGameImpl::GetStageDataStore().GetTimeLimitForStageClearGrade(InServerStageId, InWantedGrade);

	int32 FormattedHour = 0;
	int32 FormattedMin = 0;
	int32 FormattedSec = 0;
	//GetTimeFormatFromRawSecond(TimeLimitInSec, FormattedHour, FormattedMin, FormattedSec);
	// 困 搬苞俊辑 FormattedMin 苞 Sec 父 荤侩茄促. 捞霸 割 矫埃 窜困 捞犯霸 唱坷搁 阜厘捞摆瘤

	// 盒, 檬, 殿鞭 沥焊 捞犯霸 持绢淋.
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
	// 困 搬苞俊辑 FormattedMin 苞 Sec 父 荤侩茄促. 捞霸 割 矫埃 窜困 捞犯霸 唱坷搁 阜厘捞摆瘤

	// 盒, 檬, 殿鞭 沥焊 捞犯霸 持绢淋.
	if(bIsClear)
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_Grade_Clear_2")),
			FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)));
	else
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_Grade_2")), 
			FText::FromString(FString::FromInt(FormattedMin)), FText::FromString(FString::FromInt(FormattedSec)));
}

void AB2StageManager::OnTalkDialogStart(const int32 GroupID, const bool IsClear)
{
	//// 父老 捞 锭啊 BeginPlay 鸥捞怪捞搁 寸厘 TalkDialog 凯府绰 霸 async 肺爹 包访茄 漂喊茄 巩力甫 具扁窍瘤 臼摆瘤父
	//// 郴何俊辑 茄 平 捞饶俊 TalkDialog 俊 静捞绰 府家胶甫 肺爹窍绰 霸 乐绢辑 搬惫 奴 府家胶 flush 波府绰 秦 初酒具 茄促.
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
	//	// 胶配府 葛靛 胶诺 
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
