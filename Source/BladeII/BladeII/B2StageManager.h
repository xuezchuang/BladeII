// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BladeII.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "BladeIIPlayer.h"
//#include "B2StageInfo.h"
#include "B2StageEventDirector.h"
#include "CommonStruct.h"
#include "B2UIEnum.h"
#include "GameFramework/GameMode.h"
#include "../DataStore/B2ClientDataStore.h"
#include "B2StageManager.generated.h"

USTRUCT()
struct FStageRewardManager
{
	GENERATED_USTRUCT_BODY()

public:
	void Initialize();

	FORCEINLINE void SetStageManager(class AB2StageManager* InStageManager) { StageManager = InStageManager; }
	FORCEINLINE void SetTotalStageRewardGold(uint32 Gold) { TotalStageRewardGold = Gold; }
	FORCEINLINE int32 GetTotalStageRewardGold() { return TotalStageRewardGold; }
	FORCEINLINE void SetDefeatBossRewardItem(const FB2Item& Item){ DefeatBossRewardItems.Add(Item); }
	FORCEINLINE void SetDefeatBossPieceRewardItem(const FB2Item& Item) { DefeatBossPieceRewardItems.Add(Item); }
	
	void RequestSpawnDropItem(const struct FDropItemInfo& DropItemInfo, class ABladeIICharacter* DelaySpawnRefActor, const FVector& SpawnLocation);
	enum SelectItemPhase
	{
		Normal,
		Additional,
		UseTicket
	};

private:
	int32 GetQTEGold() const;
	int32 GetRewardGold(float weight, float TotalMobWeight, bool InQTE = false);
	const FB2Item& GetDefeatBossRewardItem();
	const FB2Item& GetDefeatBossPieceRewardItem();

public:
	class UB2ItemInfo* GetItemInfoTable();

	void AcquireGold(uint32 AcquireGold);
	void AcquireItem(int32 AcquireItemRefID);
	void SelectItem(int32 SelectedItemRefID, SelectItemPhase Phase);

	void PreloadAllRewardItemInfo(); // To load lazy-loaded TAssets in advance. Supposed to be called at BeginPlay stage.

private:
	static const int32 DropCoinCountForQTE = 5;

	int32 TotalStageRewardGold;
	int32 BossItemCurrentIndex;
	int32 BossPieceItemCurrentIndex;
	int32 SumCurrentRewardGold;

	TArray<FB2Item> DefeatBossRewardItems;
	TArray<FB2Item> StageClearRewardItems;
	TArray<FB2Item> DefeatBossPieceRewardItems;

	class AB2StageManager* StageManager;	
};

/*
 * To keep data while level changing
 */

USTRUCT()
struct FCacheStageEssentialDataToKeep
{
	GENERATED_USTRUCT_BODY()

	FCacheStageEssentialDataToKeep();

public:
	void Initialize();
	void SetStandaloneStageData(AB2StageManager* StageManager);
	void SetStageResponseData(FString Token, int32 DropGolds, int32 DropGoldGoldmob, const TArray<FB2Item>& Items, int32 InMaxResurrectionChance, const TArray<FB2Item>& _bossPieceItems);
	void SetStageResponseDataStageBuff(int32 BuffFlag);
	void SetStageResponseDataQuest(int32 QuestItemID, int32 QuestItemCount);
	void SetQTEBonus(int32 StartQTEBonus, int32 FinishQTEBonus);
	
	void RequestStartStage(int32 InClientStageId, EStageDifficulty StageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass, const TArray<EStageBuffType>& StageBuffTypes, FRepeatBattleStateSet RepeatBattle, bool bSkipStoryCinematic, int32 MainPresetId, int32 SubPresetId);
	void RequestStartStage(int32 InClientStageId, EStageDifficulty StageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass, const TArray<EStageBuffType>& StageBuffTypes, FRepeatBattleStateSet RepeatBattle, bool bSkipStoryCinematic, int32 TutorialId, int32 MainPresetId, int32 SubPresetId);
	void RequestStartStageAgainAtPlace(); // For Restart stage, not the repeat battle.
	void RequestStartStageNext();
	void SetStageBasisData(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass, int32* InStageOption = nullptr, FRepeatBattleStateSet* InRepeatBattleState = nullptr);
	void SetStageBasisData_CounterDungeon(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass);
private:
	void SetStageBasisData_CommonInternal(int32 InClientStageId, EStageDifficulty InStageDifficulty, EPCClass MainCharClass, EPCClass SubCharClass);
public:
	void ClearStageBasisData(); // To be used on entering lobby.
	void ResponseStartStage(AGameMode* InCurrentGameMode, const FB2StartStageInfo& Info);

	void RequestStartCounterDungeon(FServerStageID InServerStageId, EPCClass MainCharClass, EPCClass SubCharClass);
	void PrepareForceStartCounterDungeon(FServerStageID InServerStageId);	// For Console Command
	void ResponseStartCounterDungeon(AGameMode* InCurrentGameMode, const FString& Token);

	void RequestStartHeroTower(AGameMode* InCurrentGameMode, int32 nStartFloor, EPCClass MainCharClass, EPCClass SubCharClass, FString szToken = TEXT(""));
	void ResponseStartHeroTower(AGameMode* InCurrentGameMode, const FB2ResponseStartHeroTowerPtr& msgPtr);

	void RequestStartDimension(AGameMode* InCurrentGameMode, int32 nDiff, int32 nMap, EPCClass MainCharClass, EPCClass SubCharClass);
	void ResponseStartDimension(AGameMode* InCurrentGameMode, const FB2ResponseStartDimensionPtr& msgPtr);

	void CachingStageInfoData();

	void PopUpImpossibleToStartStageByFullInven(const TArray<b2network::B2InventoryCountServerInfoPtr>& InvenCountInfo);
	bool EquipNumberCheck(EPCClass& PCClass);
	void SetRepeatBattleStateBoostOn(bool InBoostOn);
	void FullEtherInventory(const FB2StartStageInfo& StageInfo);

	void InitQTECount();
	void AddQTECount(int32 QTEStateNum);
	int32 GetQTECount(int32 QTEStateNum) const;
	int32 GetQTEBonus(int32 QTEStateNum) const;
	FORCEINLINE int32 GetRecentQTE() const { return RecentQTEState; }

	FORCEINLINE FStageRewardManager& GetStageReward() { return StageReward; }
	
	FORCEINLINE int32 GetCurrentClientStageId() { return ClientStageId; }
	FORCEINLINE FServerStageID GetServerStageID() { return ServerStageID; }
	FORCEINLINE bool IsScenarioMode() { return bScenarioMode; }
	FORCEINLINE EStageDifficulty GetStageDifficulty() { return StageDifficultyLevel; }
	// Some repeatedly used condition..
	FORCEINLINE static bool IsScenarioModeAllowedDifficulty(EStageDifficulty InDifficulty) { return (InDifficulty == EStageDifficulty::ESD_Normal); }

	FORCEINLINE int32 GetTotalStageRewardGold() { return StageReward.GetTotalStageRewardGold(); }
	FORCEINLINE float GetStageClearTime() { return PlayTimeInMainMap; }

	FORCEINLINE int32 GetMaxResurrectionChance(EResurrectGameModeType ModeType) { return ResurrectionList[ModeType].MaxResurrectionChance; };
	FORCEINLINE int32 GetRemainingResurrectionChance(EResurrectGameModeType ModeType) { return ResurrectionList[ModeType].RemainingResurrectionChance; };
	FORCEINLINE int32 GetResurrectionCost(EResurrectGameModeType ModeType) { return ResurrectionList[ModeType].ResurrectionCost; };

	//void SetClearResurrection(EResurrectGameModeType ResurrectModeType, int32 InMaxResurrectionChance);
	void DecreaseRemainingResurrectionChance(EResurrectGameModeType ResurrectModeType);
	//void SetResurrectionCostByLookup(EResurrectGameModeType ResurrectModeType, int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance); // Set resurrect cost by looking up client data store's master table.
	//	
	/** Set combat state related cache variables ready before we start load extra level */
	void SetReadyPreExtraCombatVariables(class ABladeIIPlayer* CurrentActivePlayer, class UB2SkillRTManager* SkillRTMgr);
	/** Restore on extra level loading for SkillRTManager InactivePlayerStateCache. */
	void SetPostExtraMapLoadSkillRTManager(class ABladeIIPlayer* CurrentActivePlayer, struct FInactivePlayerStateCache* StructToSet);
	/** Restore on extra level loading for initially active BladeIIPlayer */
	void SetPostExtraMapLoadB2Player(class ABladeIIPlayer* CurrentActivePlayer);
	/** Other data that goes to ExtraMap, requiring sort of manual handling. */
	FORCEINLINE bool IsPlayInExtraLevel() { return bPlayInExtraLevel; }
	FORCEINLINE EPCClass GetPreExtraLastPlayedClass() { return PreExtraLastPlayedCharClass; } // Better refer to this only for extra map transition.

	FORCEINLINE FString GetPlayToken() { return PlayToken; };
	FORCEINLINE FRepeatBattleStateSet GetRepeatBattleState() { return RepeatBattleState; }
	void SetRepeatBattleStateStop(); // When you want stop repeat battle in-game

	FORCEINLINE void SetHeroTowerFloor(int32 nFloor){ HeroTowerFloor = nFloor; }
	FORCEINLINE int32 GetHeroTowerFloor(){ return HeroTowerFloor; }

private:
	EPCClass			RequestedMainPlayerCharClass;
	EPCClass			RequestedSubPlayerCharClass;
	int32				RequestedClientStageId;
	EStageDifficulty	RequestedStageDifficulty;
	FServerStageID		RequestServerStageId;
	int32				RequestedStageOption;
	int32				RequestedTutorialId;

	int32				RequestMainPCPresetId;
	int32				RequestSubPCPresetId;

	int32				ClientStageId;
	EStageDifficulty	StageDifficultyLevel;
	FServerStageID		ServerStageID;

	TArray<EStageBuffType> ServerSelectedStageBuff;

	/** It decides which In-game matinee cinematic of StageEventDirector will be played, along with EStageEventManageMode setting.
	 * True for first story mode play, and be false either by default or by user selection afterward. */
	uint32 bScenarioMode : 1;
	/** Only used for deciding bScenarioMode internally. It means the user selected SkipStory from BattleStageInfo UI.
	 * Final ScenarioMode flag will depend on other conditions too. */
	uint32 bStageRequestedWithSkipStoryCinematic : 1;

	FString PlayToken; 
	bool bSpawnGoldMob;
	FStageRewardManager StageReward;	

	//////////////////////////////////////////////////
	// For data transition to extra map.

	bool bHasExtraLevel;
	bool bPlayInExtraLevel;
	float PlayTimeInMainMap;

	int32 MainMapStartingGold;
	int32 MainMapLastGold;

	EPCClass PreExtraLastPlayedCharClass; // Either MainPlayerCharClass or SubPlayerCharClass. ExtraMap default player class will be this one.
	float PreExtraMainPlayerLastHP;
	float PreExtraMainPlayerLastMaxHP; // Either one of main/sub character will calculate max HP again at next stage, but inactive character does not, save it for extra map transition.
	float PreExtraMainPlayerLastArmor;
	int32 PreExtraMainPlayerWeaponSkillPoint;
	float PreExtraSubPlayerLastHP;
	float PreExtraSubPlayerLastMaxHP;
	float PreExtraSubPlayerLastArmor;
	int32 PreExtraSubPlayerWeaponSkillPoint;

	//////////////////////////////////////////////////

	/** The exp that the player will get after complete this stage. It won't be per-mob basis.
	* Must be loaded or provided from server for each stage loading. */
	int32 TotalExpOfCurrentStage;

	/*int32 MaxResurrectionChance;
	int32 RemainingResurrectionChance;
	int32 ResurrectionCost;*/

	TMap<EResurrectGameModeType, FResurrectData> ResurrectionList;

	FRepeatBattleStateSet RepeatBattleState;

	int32 HeroTowerFloor;

	int32 RecentQTEState;
	int32 StartQTEAttackCount;
	int32 FinishQTEAttackCount;
	int32 StartQTEBonusValue;  // one click qte
	int32 FinishQTEBonusValue; // double click qte
#if WITH_EDITOR // For use play-in-editor stage settings
	bool bStandalonePIE;
	FORCEINLINE bool IsStandalonePIE() const { return (GIsEditor && bStandalonePIE); }
#endif

	/** This stage manager is valid only for a stage, so cache the StageInfoData at the first time querying it. */
	//FSingleStageInfoData* CachedCurrentStageInfoData;

	friend class AB2StageManager;
};

USTRUCT()
struct FB2ScenarioDialogManager
{
	GENERATED_USTRUCT_BODY()

public:

	void SetStageManager(AB2StageManager* StageManager);

	// @param bMaintainBattleUI : Let BattleUIs on, assuming ScenarioDialog hide battle UIs (by bHideBattleRelatedWidgets) by default.
	void SetScenarioDialog(FName ToSetDialogCodeName, bool bMaintainBattleUI = false);
	void SetScenarioDialog(const struct FDialogInfoInstance* ToSetDialogInfoInstance, bool bMaintainBattleUI = false);
	void Initialize();
	void ForceSkip();

	void NextDialogLine(); // It will still check DelayToNextLine, next line will finally show-up by NexDialogLineImmediate
	void NextDialogLineImmediate();
private:
	void SetDialogLine(int32 NextDialogLineIndex);

	class UB2DialogCharacterInfoSet* GetDialogCharacterInfoSet(UObject* WorldContextObject);
	struct FDialogCharacterInfo* GetDialogCharacterInfo(FName ToGetCharacterCodeName, UObject* WorldContextObject);
	USoundCue* GetDialogSound(FName CharacterCodeName, FName SoundCodeName, UObject* WorldContextObject);
	
private:
	const struct FDialogInfoInstance* DialogInfoInstace;

	int32 CurrentDialogLineIndex;

	FTimerHandle GoToNextLineTimerHandle;
	class AB2StageManager* CachedStageManager;
};

USTRUCT()
struct FDelayedSpawnDropItemInfo
{
	GENERATED_USTRUCT_BODY()

	FDelayedSpawnDropItemInfo()
	{
		DelaySpawnLocRefActor = NULL;
	}

	UPROPERTY(Transient)
	class ABladeIICharacter* DelaySpawnLocRefActor;

	FTimerHandle DelayTH;
};

/**
 * Management of overall stage state and event, supporting some (but not all) operation of MonsterSpawnPool, StageEventDirector, etc.
 * Consistent only for one stage, not between multiple stages. Same fate to the GameMode.
 * Created only for B2StageGameMode.
 */
UCLASS(notplaceable)
class BLADEII_API AB2StageManager : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	class AB2StageGameModeBase* CachedGameMode;

private:
	static FCacheStageEssentialDataToKeep StaticCacheStageData;
	FB2ScenarioDialogManager ScenarioDialogManager;
	UPROPERTY(Transient)
	class UAudioComponent* CurrentPlayingDialogSound;

public:
	
	FORCEINLINE static FCacheStageEssentialDataToKeep& GetCacheStageKeepEssentialData() { return StaticCacheStageData; }
	FORCEINLINE static FStageRewardManager& GetStageRewardManager(){ return StaticCacheStageData.GetStageReward(); }

	void NextDialogLineWrapper();
	void NextDialogLineImmediateWrapper();
	void SkipDialogWrapper();
	void PauseDialog(bool pause);

	void PlayDialogSound(USoundCue* InSoundCue);
	void StopCurrentDialogSound();
		
	void SetGameMode(class AB2StageGameModeBase* InGameMode);
	//virtual void Tick(float DeltaSeconds);

	void ResetPlayTime();

	bool IsEndStage(int32 CurrentStageID);
	FServerStageID GetNextServerStageID(int32 CurrentStageID, EStageDifficulty CurStageDifficulty);
protected:	

	/** Set to true simply when all stuff (mainly spawn pools) of this level get cleared. */
	uint32 bCurrentLevelCleared : 1;
	/** All levels (in the case of ExtraMap) need to be cleared to set this true. For single level stage, it is almost same to bCurrentLevelCleared. */
	uint32 bStageCleared : 1;	

	/** All mobs in world stop moving (just idle pose) while this is true, even for newly spawned mobs.
	 * This is almost intended to be used for directed stage event scene. Refer to bStopAllMobsWhilePlaying of StageEventDirector. */
	uint32 bStopAllMobs : 1;

private:
	FTimerHandle ExtraMapMessageDelayTimer;
	FTimerHandle ExtraMapLoadingDelayTimer;	
	
	/** To replace GameState's ElapsedTime (for better precision and to consider B2 specific environment), it is supposed to be the stage play time after clearing the stage. 
	 * While playing the stage, it might not have the up-to-dated time depend on update handling. */
	float LastUpdatedPlayTime;

	/** It caches the last moment (get by UWorld::GetTimeSeconds) when LastUpdatedPlayTime is updated.
	 * Then, stage play time at any point afterward is, LastUpdatedPlayTime + UWorld::GetTimeSeconds - PlayTimeLastCheckedCache */
	float PlayTimeLastCheckedCache;

	/** True means LastUpdatedPlayTime is supposed to be updated from PlayTimeLastCheckedCache at any moment.
	 * False means LastUpdatedPlayTime cannot be updated until RefreshStagePlayTimeCheck is called. */
	uint32 bCountingPlayTime : 1;

	/** Stuff for stage clear event scene. They are either deployed object set by Blueprint, or being dynamically created internally. */
	UPROPERTY()
	class ACameraActor* StageClearViewCam;
	UPROPERTY()
	class UParticleSystemComponent* StageClearWingFxComp;
	UPROPERTY()
	class UParticleSystemComponent* StageClearGemFxComp;
	UPROPERTY()
	class UParticleSystemComponent* StageClearBGFxComp;
	/** Those fog visibilities are cached to turn off for clear phase 1 and turn it back on after phase 2. */
	TMap<class AExponentialHeightFog*, bool> CachedExpFogVisMap;
	TMap<class AAtmosphericFog*, bool> CachedAtmosFogVisMap;

	uint32 IsPossiblePlayBossBgm;
	int32 CurrentBossMobCount; // Counted for any mobs spawned with boss flag of spawn pool

	bool bPausedDialog;

	bool CustomaizeBGM;
	FName CustomaizeBGMName;

	TArray<std::shared_ptr<class issue>> Issues;

public:	
	// Sets default values for this actor's properties
	AB2StageManager(const FObjectInitializer& ObjectInitializer);

	/** This is not an AActor interface. Originated from BladeIIGameMode::PreBeginPlay Called before any other actor's BeginPlay is called. */
	void PreBeginPlay();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
	virtual void Destroyed() override;

	/** Simply comes from BeginPlay here.
	* Also look at ABladeIIGameMode::PreBeginPlay for any handling required before other actor's BeginPlay */
	void StageBeginHandling();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bool DevOnlyIsStandalonePlay() const;
#endif

private:
	void SyncDataToUIDoc(); // Call this probably at beginning phase. Not for realtime use.

	void SubscribeEvents();
	void UnsubscribeEvents();

	void OnTalkDialogStart(const int32 GroupID, const bool IsClear);
	void OnTalkDialogEnd(const int32 GroupID, const bool IsClear);

	void CheckBeginStageClearScene_Phase1();
public:
	/** From spawn pool on a single mob spawn, either by wave spawn or summoning. Arguments almost reflects ABladeIICharacter::SetBirthPlace
	 * In the case of wave mob spawn notify, it will be soon followed by NotifySpawnPoolSingleWaveStart */
	void NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo);
	/** Called from spawn pool actor which is just completed (all mobs in all waves are dead). */
	void NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool);
	/** Another spawn pool event notification. */
	void NotifySpawnPoolSingleWaveStart(class AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum);
	/** Notified when all mobs in a wave died. */
	void NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum);
	/** Notified when one mob in a wave died. At this point, DyingMob actor might still be far from final destruction. See NotifyForgetACharacter for that use. */
	void NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex);
	/** Notified when one mob in a wave just changed (advance) phase. DestroyedPrevRef is non-null only when this phase change caused re-spawn, otherwise null. */
	void NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase);
	/** Notified when a summoned mob died. At this point, DyingMob actor might still be far from final destruction. See NotifyForgetACharacter for that use. */
	void NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob);
	/** CharacterAboutToBeDestroyed will be gone after this call. It is like notification of setting NULL for references to CharacterAboutToBeDestroyed if any. */
	virtual void NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed);

	void NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType);
	/** Stage event scene end notification does not necessarily mean that all the show is played to the end. It can be stop at middle, and this notify must be still valid. */
	void NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector);

	/** Notification of UI page opening completion, ideally considering opening widget animation stuff, but in most cases without special modification, it probably notified on simple constructing moment */
	void NotifyUIWidgetOpenComplete(FName InOpenedUI);

	// Other various event callbacks can be added..

	// TODO : Consider change those Notify** to private, and call them by EventSubsystem.
	
	/** Additional To GameMode's PreloadAnyNecessaryInfo */
	void PreloadAdditionalNecessaryInfo();

	bool GetIsShowTalkDialog();

	int32 IsGoldBonusStage();
	int32 IsBossStage();

private:

	/** Check for stage clear condition and do some stage clear processing if so. */
	void CheckStageClear();

	void RequestStageClear();
	void ResponseStageClear(const FB2ClearStageInfo& Info);
	/** Do some stuff for final confirmed stage clear event. */
	void OnStageClear();

	void RequestFailStage(EStageFailReason reason);

public:
	void RequestCounterDungeonClear();
private:
	void ResponseFinishCounterDungeon(const FB2FinishCounterDungeonInfo& Info);
	void OnCounterDungeonClear();

public:
	void RequestHeroTowerClear(bool bClear);
	void RequestAbandonHeroTower();
	void RequestDimensionClear(EDimensionResult bClear);

private:
	void ResponseHeroTowerClear(const FB2ResponseFinishHeroTowerPtr& MsgPtr);
	void ResponseDimensionClear(const FB2ResponseFinishDimensionPtr& MsgPtr);

	void ExtraMapMessageDelayTimerCB();
	void ShowExtraMapMessage();
	void ExtraMapLoadingDelayTimerCB();

	/** Some stuff that have to be done right before the LoadExtraMap */
	void PreHandleLoadExtraMap();
	/** Load the extra map defined at StageInfoTable. This StageManager will be recreated for the extra map. */
	void LoadExtraMap();

	
	/** Save some data to transfer for extra map if necessary for this stage. */
	void SetReadyPlayingExtraMap();
	/** Retrieve data set by SetReadyPlayingExtraMap */
	void RetrieveDataForExtraMap();
	
	/** For handling divided stage clear (victory) event scene */
	void BeginStageClearScene_Phase1();
	void StageClearPhase1_StartVictoryWingFx();
	void StageClearPhase1_StartVictoryGemFx();
	void StageClearPhase1_StartVictoryBGFx();
	void StageClearPhase1_StartVictoryFxCommon(class UParticleSystem* InPSTemplate, class UParticleSystemComponent*& InOutCompForSet, const TCHAR* PosConfigKey, const TCHAR* RotConfigKey, const TCHAR* ScaleConfigKey);
	void BeginStageClearScene_Phase2();
	void EnsureTargetToStageClearCam();
	// No EndStageClearScene stuff, because they are invoked at last.

	void TurnOffAllFogs(); // Cache fogs visible state and turn them off (for stage clear scene).
	void TurnBackOnAllFogs();

	void PlayStageBGM(); // It will play relevant BGM for current stage context.
	void PlayBossStageBGM();


public:	
	/** They are supposed to be set by level deployed objects. If not set through here, they are dynamically created. */
	void SetStageClearViewObjects(class ACameraActor* InCamActor, class UParticleSystemComponent* InPSWingComp, class UParticleSystemComponent* InPSGemComp, class UParticleSystemComponent* InPSBGComp);

	/** Update PlayTimeLastCheckedCache to the time of calling moment, then next UpdateStagePlayTime call will get the PlayTime counted from this moment. 
	 * UpdateStagePlayTime does this too, but you call this when you just need refresh PlayTimeLastCheckedCache. 
	 * It also resume counting when bCountingPlayTime is false. */
	void RefreshStagePlayTimeCheck();
	/** Update the LastUpdatedPlayTime to reflect actual played time so far. PlayTimeLastCheckedCache will be updated too. 
	 * If bStopCounting is false, you just update the PlayTime and keep counting, but if bStopCounting is true, PlayTime won't be updated until resumed by RefreshStagePlayTimeCheck. */
	void UpdateStagePlayTime(bool bStopCounting);
	/** Get PlayTime so far in second unit. If bUpdateNow is false, the return value is valid only after some moment like stage clear. */
	float GetStagePlayTime(bool bUpdateNow = true);
private:
	void AutoUpdateStagePlayTimeCB(); // We now need to automatically update play time for battle UI display..
	FTimerHandle AutoUpdateStagePlayTimeTH;
	float StagePlayTimeAutoUpdateInterval; // It does not affect the final precision, just for in-game battle UI display.

public:
	/** Getting the actual data within the unique StageInfoTable considering CurrentStageNumber.
	 * Provided for the most common use. */
	//FSingleStageInfoData* GetCurrentStageInfoData();

	FORCEINLINE EStageDifficulty GetStageDifficultyLevel() const { return StaticCacheStageData.StageDifficultyLevel; }
	float GetStageDifficultyScale(); // This is the more in final form, to be actually applied.

	/** Apply or cancel bStopAllMobs option. */
	void StopOrRestoreAllWorldMobs(bool bStop);

	void ForceCancelAttackAllWorldMobs();

	/** Spawns client side drop item manifestation with wanted time delay. They are just same to SpawnDropGold/Item if SpawnTimeDelay is 0 or negative.
	 * but now the delay requires DelaySpawnLocRefActor, because spawn location should considers actor's location after delay. */
	void DeferredSpawnDropGold(int32 Amount, const struct FActorSpawnParameters SpawnParam, class ABladeIICharacter* DelaySpawnLocRefActor, const FVector& SpawnLocation, const FRotator SpawnRotation, float SpawnTimeDelay, bool InQTE = false);
	void DeferredSpawnDropItem(int32 ItemRefID, const struct FActorSpawnParameters SpawnParam, class ABladeIICharacter* DelaySpawnLocRefActor, const FVector& SpawnLocation, const FRotator SpawnRotation, float SpawnTimeDelay);

	void PlayBGM();
	void PlayCustomBGM(const FName& InName);
private:
	TMap<int32, FDelayedSpawnDropItemInfo> DelayedDropSpawnInfoMap; // Cached data for handling DeferredSpawnDrop** calls.
	int32 DelayedDropSpawnSerialNumber; // Simply increased for each DeferredSpawnDrop** call
	/** Spawns client side drop item manifestation. Put negative value to SpawnTimerMapKey if you just calling without DeferredSpawnDrop stuff. */
	void SpawnDropGold(int32 Amount, const struct FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey, bool InQTE = false);
	void SpawnDropItem(int32 ItemRefID, const struct FActorSpawnParameters SpawnParam, FVector InitialSpawnLocation, FRotator InitialSpawnRotation, int32 DelaySpawnInfoMapKey);
	FTransform SpawnDropDelayHandlingCommon(FVector InSpawnLocation, FRotator InSpawnRotation, int32 DelaySpawnInfoMapKey);
public:

	void CancelOrStopRepeatBattle();
	void RequestFailStageDefeat();
	void RequestFailStageGiveUP();

	bool HasStageBuff(EStageBuffType InType) const;

	// Either DelaySpawnRefActor or SpawnLocation, you might need DelaySpawnRefActor when DelaySpawnRefActor is moving and spawn is to be delayed
	FORCEINLINE void RequestSpawnDropItem(const struct FDropItemInfo& DropItemInfo, class ABladeIICharacter* DelaySpawnRefActor, const FVector& SpawnLocation) { GetStageRewardManager().RequestSpawnDropItem(DropItemInfo, DelaySpawnRefActor, SpawnLocation); }
	FORCEINLINE void RepeatBattleStop() { StaticCacheStageData.SetRepeatBattleStateStop(); }
	FORCEINLINE bool IsStageCleared() { return bStageCleared; }
	FORCEINLINE int32 GetCurrentClientStageId() const { return StaticCacheStageData.GetCurrentClientStageId(); }
	FORCEINLINE FServerStageID GetServerStageID() const { return StaticCacheStageData.GetServerStageID(); }
	FORCEINLINE bool IsScenarioMode() const{ return StaticCacheStageData.IsScenarioMode(); } // Should play full story in-game cinematic?
	FORCEINLINE bool IsPlayingInExtraLevel() const { return StaticCacheStageData.IsPlayInExtraLevel(); }
#if WITH_EDITOR
	FORCEINLINE bool IsStandalonePIE() const { return StaticCacheStageData.IsStandalonePIE(); }
#endif

	FORCEINLINE FRepeatBattleStateSet GetRepeatBattleState() { return StaticCacheStageData.GetRepeatBattleState(); }
};

// For use of UI text display, being displayed at multiple places.
FText GetStageClearGradeConditonText(FServerStageID InServerStageId, int32 InWantedGrade);
FText GetStageClearGradeInfoText(FServerStageID InServerStageId, int32 InWantedGrade, bool bIsClear = false);
