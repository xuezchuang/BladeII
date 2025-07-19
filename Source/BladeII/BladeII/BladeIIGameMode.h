// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "BladeII.h"
#include "GameFramework/GameMode.h"

#include "BladeIICharacter.h"
////#include "B2ClientDataStore.h"
////#include "B2StageDataStore.h"
#include "B2CommonSoundInfo.h"
//#include "B2StageEventDirector.h"
//#include "B2GameLoadingScreen.h"
#include "B2Chatting.h"
////#include "Quest.h"
//#include "BladeIIUtil.h"
////#include "B2PreRenderer.h"
//#include "B2AssetLoader.h"AddAutoWayPoint
#include "../Common/CommonStruct.h"
#include "memory"
#include "../UI/B2UIEnum.h"
#include "B2GameLoadingScreen.h"
#include "../DataStore/CharacterDataStore.h"
#include "BladeIIPlayer.h"
#include "../Misc/B2PreRenderer.h"
#include "../NetPlay/BladeIINetPlayer.h"
#include "../DataStore/B2ClientDataStore.h"
#include "B2StageEventDirector.h"
#include "../AI/B2AutoWayPoint.h"
#include "../InteractiveActor/B2TreasureChestBase.h"
#include "../InteractiveActor/B2DestructibleLevelObjBase.h"
//#include "../InteractiveActor/B2JumpEventZone.h"
#include "BladeIIGameMode.generated.h"

/** 
 * Enum type representing B2LOC_CAT_~~ for Blueprint BladeIIGetLOCText util 
 * Not every B2LOC_CAT_~ has to have matching EB2LOC_CAT entry.
 * Add only when you want to use it at Blueprint.
 */
UENUM(BlueprintType)
enum class EB2LOC_CAT : uint8 
{
	_GENERAL			UMETA(DisplayName = "General Text"),
	_WORLDNAME			UMETA(DisplayName = "World Text"),
	_CHARNAME			UMETA(DisplayName = "Character Name Text"),
	_ITEMNAME			UMETA(DisplayName = "Item Name Text"),
	_DIALOGTEXT			UMETA(DisplayName = "Dialog Text"),
	_SKILLDATA			UMETA(DisplayName = "Skill Text"),
	_MAILCONTENT		UMETA(DisplayName = "Mail Text"),
	_BATTLE_MENT		UMETA(DisplayName = "Battle Text"),
	_QUESTTEXT			UMETA(DisplayName = "Quest Text"),
	_ERROR_CODE			UMETA(DisplayName = "Error Text"),
	_MISSIONTEXT		UMETA(DisplayName = "Mission Text"),
	_WINGTEXT			UMETA(DisplayName = "Wing Text"),
	_STORE				UMETA(DisplayName = "Store Text"),
	_ETHER				UMETA(DisplayName = "Ether Text"),
	_ANCIENTRELIC		UMETA(DisplayName = "Relic Text"),
	_UNITYSKILLMISSION	UMETA(DisplayName = "UnityMission Text"),
	_ITEMPREFIX			UMETA(DisplayName = "ItemPrefix Text"),
	_LOCALPUSH			UMETA(DisplayName = "LocalPush Text"),
	_DUELMODLOSEMENT	UMETA(DisplayName = "DuelModeLoseMent Text"),
	_EVENT				UMETA(DisplayName = "Event Text"),
	_CONFIGURATION		UMETA(DisplayName = "Configuration Text"),
	_BATTLEMESSAGE		UMETA(DisplayName = "BattleMessage Text"),
	_GUILDNPC			UMETA(DisplayName = "GuildNPC Text"),
	_GUILDNPCSKILL		UMETA(DisplayName = "GuildNPCSkill Text"),
};
/** For use of EB2LOC_CAT_ToString */
#define EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(InRawID) \
	case EB2LOC_CAT::_##InRawID : return B2LOC_CAT_##InRawID;
/** Provide mapping between EB2LOC_CAT enum and defined strings above. */
FString EB2LOC_CAT_ToString(EB2LOC_CAT InB2LOCEnum);

/** Make CustomeEvent of this name to level Blueprint of some Mod map which need caching some level objects.
 * The execution path of this CustomEvent is most likely the part of BeginPlay.. */
const FName RECACHE_LEVEL_RESOURCE_CENAME(TEXT("RecacheLevelResource"));

#if BII_STATS
/** Special purpose performance measuring.. during game playing, for statistical stat values */
struct FStatisticalPerfStatCollector
{
	FStatisticalPerfStatCollector();

	static bool bDoStatCollecting;
	static bool bSkipEventCinematics;
	static bool bIsCurrentlyInEventScene;
	static float SkipFrameTimeLimit;
private:
	bool bInitialized;
	bool bDataAvailableForDisplay;
	// They come from FStatUnitData
	FSimpleStatisticRecorder<float> GameThreadTimes;
	FSimpleStatisticRecorder<float> RenderThreadTimes;
	FSimpleStatisticRecorder<float> GPUTimes; // Might not be available.. then just see if FrameTime is a lot bigger than other Thread times..
	FSimpleStatisticRecorder<float> FrameTimes;

	FSimpleStatisticRecorder<int32> DrawCalls;
	FSimpleStatisticRecorder<int32> PolyCounts;

	float Result_GameThreadTime_Avg;
	float Result_RenderThreadTime_Avg;
	float Result_GPUTime_Avg;
	float Result_FrameTime_Avg;
	int32 Result_DrawCall_Avg;
	int32 Result_PolyCount_Avg;

	float Result_GameThreadTime_SD;
	float Result_RenderThreadTime_SD;
	float Result_GPUTime_SD;
	float Result_FrameTime_SD;
	float Result_DrawCall_SD;
	float Result_PolyCount_SD;

	float Result_GameThreadTime_Upper10;
	float Result_RenderThreadTime_Upper10;
	float Result_GPUTime_Upper10;
	float Result_FrameTime_Upper10;
	int32 Result_DrawCall_Upper10;
	int32 Result_PolyCount_Upper10;

	int32 SkippedFrameNum;
	double SkippedFrameTimeAvg;

public:
	static void LoadSettings();
	// Each event is supposed to be GameMode event..
	void OnBegin(class ABladeIIGameMode* InB2GM);
	void OnTick(class ABladeIIGameMode* InB2GM, float InDeltaTime);
	void OnEnd(class ABladeIIGameMode* InB2GM);
	FORCEINLINE bool CanDisplayData() const { return bDataAvailableForDisplay; }
private:
	void DisplayResultStats(class ABladeIIGameMode* InB2GM);
};
#endif

enum class EB2SkillLODType : uint8
{
	ESLT_NONE = 0,
	ESLT_ONLY_FOR_SELF = 1,
	ESLT_ALL_LOD = 2,
};

class IGameRule
{
public:
	virtual ~IGameRule() {}

	virtual bool AllowToTag() const = 0;
	virtual bool IsGlobalTimeDilationEnabled() const = 0;
	virtual bool IsQTEEnabled() const = 0;
	virtual FString GetBehaviorTreeAsset() const = 0;
	virtual bool IsAIAllowedToEvade() const = 0;
	virtual bool AllowPlayerPointLight() const = 0; // Performance rule
	virtual bool UseSectionMergedPCModel() const = 0;  // Performance rule, uesd for combat scene.
	virtual bool ConditionalUseSectionMergedPCModel() const = 0; // .. by scalability option.
	virtual bool AllowOtherPlayerDamageNumNotify() const = 0; 
	virtual bool IsEvadeCoolTimeCheck() const = 0;
	virtual bool IsHitFreezeEnabled() const = 0;
	virtual bool ForceApplyHitAnimation() const = 0;
};

class DefaultGameRule : public IGameRule
{
public:
	virtual bool AllowToTag() const override { return true; }
	virtual bool IsGlobalTimeDilationEnabled() const override { return true; }
	virtual bool IsQTEEnabled() const override { return true; }
	virtual FString GetBehaviorTreeAsset() const override { return FString(); }
	virtual bool IsAIAllowedToEvade() const override { return true;	}
	virtual bool AllowPlayerPointLight() const override { return true; } // Performance rule
	virtual bool UseSectionMergedPCModel() const override { return false; } // Performance rule, uesd for combat scene.
	virtual bool ConditionalUseSectionMergedPCModel() const override { return false; } // .. by scalability option.
	virtual bool AllowOtherPlayerDamageNumNotify() const override { return true; }
	virtual bool IsEvadeCoolTimeCheck() const override { return false; }
	virtual bool IsHitFreezeEnabled() const override { return true; }
	virtual bool ForceApplyHitAnimation() const override { return false; }
};

class issue;
using issue_ptr = std::shared_ptr<issue>;

struct FB2DamageLog
{
	FB2DamageLog() : ActualDamage(0.f), AttackerAccountID(0) {}

	float	ActualDamage;
	int64	AttackerAccountID;
};

/**
 * The base of all GameModes for BladeII game.. except the DLCFront
 */
UCLASS(config = game)
class BLADEII_API ABladeIIGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	/** As a substitution of AGameMode.DefaultPawnClass by BladeII PCClassInfo.
	 * DefaultPawnClass is now a fallback for EPC_End. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	EPCClass DefaultPCClassEnum;

	/** The active PC class that will be applied for current spawning.
	 * BladeIIPlayer will refer to this value for its property overriding by PCClassInfo. */
	EPCClass CurrentPCClassEnum;

	int32 LoadingImageIdx; // Loading Image Idx, randomly selected.
	uint32 bLoadingImageUnloadedAndClear : 1;// For some hacky workaround.. safer unload..
	FTimerHandle LoadingImageUnloadTH;	

	EPreRenderPhase PreRenderPhase;
	///** Pre-render manager, must be working only at level startup. */
	UPROPERTY(Transient)
	class UB2PreRenderer* PreRenderer;

	// Cached when request resurrection
	EResurrectBuffType LastRequestedStageBuffType;

	TArray<issue_ptr>	Issues;


	FB2DamageLog	DamageLogInfo;

public:

	virtual bool NeedToRecordDamageLog(const float ActualDamage, class ABladeIICharacter* Attacker);
	virtual void RecordDamageLog(const float ActualDamage, class ABladeIICharacter* Victim, class ABladeIICharacter* Attacker);
	virtual void SendDamageLog();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	TArray<FLinearColor> FloorRingColors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	TArray<float> FloorRingColorMultipliers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	bool bOverrideFloorRingColor;

	/** This is mainly intended to be used for loading only necessary PCClassInfo objects, 
	 * assuming that PCClassInfoBox uses TAsset lazy-loading. */
	virtual TArray<EPCClass> GetPCClassesToPreLoad();
	virtual TArray<EPCClass> GetPCClassesToMatineePreLoad();
	/** Returns skill animation identifiers to pre-load. Skill anim data has big potion within whole PCClassInfo. */
	virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad();
	static TArray<FCombinedPCSkillAnimID> GetAllPCSkillAnimsOfClass(EPCClass InCharClass);

	void TryAsyncLoadSkillAnims(ICharacterDataStore* TargetDataStore);
	TArray<FCombinedPCSkillAnimID> GetEquippedSkillAnimIDs(EPCClass PlayerClass, ICharacterDataStore* DataStore);

	virtual void GatherEquippedSkillAnimIDs(ICharacterDataStore* TargetDataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills);

	/** Load any info assets, to be lazy loaded (TSoftObjectPtr stuff) and required by this game mode.
	 * It is expected to be slow and cause much memory increase. 
	 * @param bAboutToPlayLoadingMovie : Designate if loading movie is to be played soon. Being used for calculating loading progress. */
	virtual void PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie);
	void PreloadInGameInfoAsset_SynchronousOnly(bool bAboutToPlayLoadingMovie);
	void PreloadInGameInfoAsset_AsyncAllowed(bool bAboutToPlayLoadingMovie);

	virtual void PreloadLocalSkillAnimation();
	
	virtual void PreloadMatineeRefAssetsDefault() /*override*/;
	int32 B2GamePreloadMatineeRefAssets();

	/** It is meant to be called in a circumstance that any async loading request can be possibly messed up.
	 *  It does FlushAsyncLoading so itself can be messy, but it is meant to be called in advance.. */
	void GameContextAsyncFlush();

	/** Meant to be called at preload timing, to do mesh merge in advance, before playing. 
	 * InOptionalCharEntryID is required for remote player character. See GetCachedCompositeMeshMapKey */
	void PrebuildAndCacheCharMesh(EPCClass InCharClass, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData, bool bMergeSections, int32 InOptionalCharEntryID = -1);
protected:
	void PrebuildLocalTagCharMesh();
public:
	virtual bool IsAllowTag();

	//////////
	// Pre-rendering for OpenGL-ES shader caching at level beginning
	bool ShouldSetupPreRender();
protected:
	/** The main entry function of Pre-render dummy setup */
	virtual void SetupPreRenderObjects();

public:
	virtual bool IsInPreRenderPhase() const { return (PreRenderPhase == EPreRenderPhase::EPRP_Selected || PreRenderPhase == EPreRenderPhase::EPRP_AllPrim); }
	FORCEINLINE EPreRenderPhase GetPreRenderPhase() const { return PreRenderPhase; }
	virtual void UpdateForPreRenderObjects();	
private:
	void OnPreRenderComplete();
	//////

public:
	void ShowMissionDialog();
	void SetGuildBuff(const TArray<b2network::B2GuildSkillPtr>& GuildSkill);

	DECLARE_MULTICAST_DELEGATE(FOnReserveDeath)
	DECLARE_MULTICAST_DELEGATE(FOnReserveTakeDamageText)
	FOnReserveDeath& OnReserveDeath() { return CharacterDeathEvent; };
	FOnReserveTakeDamageText& OnReserveTakeDamageText() { return CharacterTakeDamageEvent; };

	////[@AKI, 170815] 네이밍 참........네이밍 바꿔주실분...ㅠㅠ
	void MakeMobUndead();
	void MakeUndeadMobDeath();
	
private:
	////////////////////////////////////////////////////////////////////////////////
	FOnReserveDeath CharacterDeathEvent;
	FOnReserveTakeDamageText CharacterTakeDamageEvent;

protected:

	//================================================================================
	// List of selected types of actors.

	/** All spawn pools loaded in this stage are registered here.
	 * -> It is now supposed to be just one! See AddSpawnPool. We maintain them in array yet, for later requirement.. */
	UPROPERTY(Transient)
	TArray<class AB2MonsterSpawnPool*> SpawnPools;

	///** All stage event directors loaded in this stage are registered here. */
	UPROPERTY(Transient)
	TArray<class AB2StageEventDirector*> EventDirectors;

	/** All ActiveCameraActors loaded in this stage are registered here. */
	UPROPERTY(Transient)
	TArray<class AB2ActiveCameraActor*> ActiveCameraActors;

	UPROPERTY(Transient)
	TArray<class AB2AutoWayPoint*>	AutoWayPoints;

	/** All TreasureChests loaded in this stage are registered here. */
	UPROPERTY(Transient)
	TArray<class AB2TreasureChestBase*> TreasureChests;

	UPROPERTY(Transient)
	TArray<class AB2DestructibleLevelObjBase*> DestructibleObjects;

	UPROPERTY(Transient)
	TArray<class AB2HomePoint*> HomePoints;

	UPROPERTY(Transient)
	TArray<class AB2DialogTriggerActorBase*> DialogTriggers;

	UPROPERTY(Transient)
	TArray<class AActor*> TaggedActors;


	//
	// Check UpdateManagedActorList and RemoveAllManagedActors for new type of actor list.
	//

	//================================================================================

	// Set to true after at least one stage event scene has been played. A HACK to detect the beginning event.
	uint32 bAnyStageEventPlayedOnce : 1;
	
	/** Outermost(?) flag of local player character's auto state. The actual current auto state can be temporarily different due to stage event, etc. */
	int32 PlayerAutoBattleType;

	/** Defer TryAutoBeginAutoAtBeginning call, for some GameMode that actual match start is deferred from BeginPlay call. */
	uint32 bDeferAutoBeginAuto : 1;

	/** Containing and managing all spawned character's combat HUD including the player. */
	UPROPERTY(Transient)
	class AB2UIManager_InGameCombat* UIManager_InGameCombat;
	
protected:

	/** To handle transition between ActiveCameraActor located nearby. ActiveActive kkk
	 * It could be NULL at any time */
	UPROPERTY(Transient)
	class AB2ActiveCameraActor* CurrentActiveActiveCameraActor;
	//
	///** Only one active spawn pool for currently loaded level among SpawnPools array here.
	// * It is supposed to be consistent along a single level. (not like CurrentActiveActiveCameraActor) */
	UPROPERTY(Transient)
	class AB2MonsterSpawnPool* TheActiveSpawnPool;

	//////////////////////////////////////////////////
	// GlobalSceneBlur, mainly for WorldBackgroundBlur of BladeIIUserWidget.

	/** Transient flag being set to true while GlobalSceneBlur is on. */
	uint32 bGlobalSceneBlurOn : 1;
	/** GlobalSceneBlur assumes active camera component is consistent during it is on.
	 * Adjust and restore post process settings are done to this camera component. */
	UPROPERTY(Transient)
	class UCameraComponent* CachedCameraAtBlurBegin;

	/** Cached from CachedCameraAtBlurBegin to restore settings when quitting from GlobalSceneBlur. */
	FPostProcessSettings CachedPPBeforeBlur;
	float CachedPPWeightBeforeBlur;

	//////////////////////////////////////////////////
	UPROPERTY(Transient)
	class UB2BGMManager* BGMManager;

	UPROPERTY(Transient)
	class UAudioComponent* CurrentPlayingAMB;
	
	uint32 bBuffTick : 1;

#if BII_STATS
	/** Performance profiling, for some period and getting statistical result. */
	FStatisticalPerfStatCollector PerfStatCollector;
public:
	virtual void DrawAdditionalPerfStatInfo(float DrawXSoFar, float DrawYSoFar) {}
#endif

private:
	/** If true, it ignores default provided loading screen wait (finish) routine by BladeIIGameMode and/or DefaultGameMoviePlay.
	 * Once set, you need to finish the loading screen properly. */
	uint32 bUseCustomWaitForLoadingScreen : 1;

protected:
	/** Widget class to be visible before some sort of loading movie. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2PreLoadingScreen> PreLoadingScreenWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2PreLoadingScreen> PreLoadingScreenWidgetClass_PreRender;
	/** Not being managed by some sort of UIManager. */
	UPROPERTY(Transient)
	class UB2PreLoadingScreen* CreatedPreLoadingScreen;
	UPROPERTY(Transient)
	class UB2PreLoadingScreen* CreatedPreLoadingScreen_PreRender;
public:
	ABladeIIGameMode(const FObjectInitializer& ObjectInitializer);

	void OnPreLoadMap(const FString& InString);
protected:
	virtual void ExtraOnPreLoadMap() {}; // For extended class overriding..
	virtual void SetupNextGameLoadingScreen();
public:
	void SetupLoadingScreenImageCommon(bool bSetImagePersistent, bool bInDisplayProgress, bool bInDisplayIndicator, 
		const TCHAR* InOptionalImageName = nullptr, // Only for a special circumstance. By default, it will pick up an image by GameMode context.
		EB2LoadingMessageType InLoadingMessageType = EB2LoadingMessageType::ELMT_GameLoading
	);
#if WITH_EDITOR
	void OnEditorEndPlayMap();
#endif
	virtual const FString GetOpeningMovieName() { return FString(TEXT("")); }
	/** To be played when next game-mode is loaded */
	virtual const FString GetClosingMovieName() { return FString(TEXT("")); }
	const FString GetLoadingImageName(bool bPersistent);
	class UTexture2D* GetLoadingImageTexture(EB2GameMode InGameMode, bool bPersistent);
	void OnLoadingImageUnloadTimerCB();
	void UnloadAndClearLoadingImageState();
	virtual void GetMovieSubtitles(TArray<B2MovieSubtitle>* Subtitles)
	{
		return;
	}

	/** For some game-mode or circumstances that need to use different loading screen finish (wait) timing (from default handling) */
	void SetUseCustomWaitForLoadingScreen(bool bSet);
	FORCEINLINE bool UseCustomWaitForLoadingScreen() const { return bUseCustomWaitForLoadingScreen; }
	/** Call it at proper place when bUseCustomWaitForLoadingScreen is true. */
	void WaitForLoadingScreen();
	void ConditionalWaitForLoadingScreen(); // It might be safer..?
	virtual void OnLoadingScreenFinished();

	void OpenPreLoadingScreen(EPreLoadingScreenType InPreLoadType, EB2GameMode GameMode = EB2GameMode::Unknown); // Pre-loading screen before the opening movie, or could be on at any other timing.
	void ClosePreLoadingScreen(EPreLoadingScreenType InPreLoadType);
	void ClosePreLoadingScreenAll();
	class UB2PreLoadingScreen* GetCurrCreatedPreLoadingScreen(EPreLoadingScreenType InType);

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;

	virtual void Tick(float dt) override;

	/** Overridden to handle DefaultPCClassEnum */
	//UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void StartPlay() override;

	virtual void HandleMatchHasStarted() override;

	UFUNCTION(BlueprintCallable, Category = "Game")
	AB2StageEventDirector* GetCurrentPlayingStageEventDirector();

	FORCEINLINE bool HasAnyStageEventScenePlayed() const { return bAnyStageEventPlayedOnce; }
	FORCEINLINE void MarkAnyStageEventPlayedOnce() { bAnyStageEventPlayedOnce = true; }
	int32 GetPlayerAutoBattleType() { return PlayerAutoBattleType; }


	FORCEINLINE bool IsAbleToBuffTick() { return bBuffTick;	}
	void SetBuffTick(bool InBuffTick) { bBuffTick = InBuffTick; }

protected:
	/** ini key name of locally saved auto state per game-mode type (not for all types of game-mode) */
	virtual FString GetLocalSavedAutoStateKeyName() const { return TEXT(""); } // Need to provide unique key name for GameMode that need to save auto state.
	int32 GetLocalSavedAutoType() const;
	void SaveLocalSavedAutoType() const;
protected:
	/** True for some game-mode that supports auto play and when saved auto play state is true. */
	virtual bool ShouldStartAutoFromBeginning();
	/** "Auto"matically "begin" "Auto" at "begin"ning if condition mets */
	void TryAutoBeginAutoAtBeginning();
public:
	/** General auto play on/off method. Other Start/StopAutoPlay methods in BladeIIPlayer(Controller) are not for direct use by user input. */
	virtual void SetPlayerAutoType(int32 InAuto);
	void ToggleLocalPlayerAutoState();
	bool IsLocalPlayerAutoBattle();
	/** It does not affect maintained auto state, and implicitly means player will go back to auto at some time */
	void TemporaryStopLocalPlayerAuto();
	/** Restore to auto play if maintained state is true and temporarily turned off */
	void RestoreLocalPlayerAuto();

	virtual bool IsGlobalTimeDilationEnabled(const AActor* RelativePawn = NULL);

protected:
	/** Called before any other actor's BeginPlay is called. */
	virtual void PreBeginPlay();
	
public:
	/** GameMode interface to specially tweak engine scalability setting by game mode 
	 * If InSelectedGraphicsLevel is GraphicsLevel_End, it will read saved value from ini. */
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End);
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const;
	void UpdateManagedActorList(); // Add some type of B2 level deployed actors for our management.
protected:
	void RemoveAllManagedActors();

	virtual void SetDesiredPCClassEnum();

	uint32 bEventsSubscribed : 1; // To un-subscribe only in subscribed GameMode object.
	virtual void SubscribeEvents(); // Events of EventSubsystem

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//DLCGameMode를 제외한 모든 GameMode는 RAII로 구현 할 것
	//UnsubscribeEvents()를 개별로 호출해야 될 일이 있으면 안광익, 김규열팀장님 혹은 권영TD님에게 문의 할 것
	virtual void UnsubscribeEvents() final; //!!DO NOT Change & Use THIS METHOD!!
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** To be executed at begin timing. meant to find relevant one for current stage number and difficulty. */
	virtual void DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty = true);

	virtual AActor* FindPlayerStartForCurrStage(AController* InController);
public:
	void GoToTitle();

	virtual void GoToTutorial(bool bIsSkip);

	/** Overridden to handle editor routine. It re-load the current loaded level. */
	virtual void RestartGame();

	/** Giving up. */
	virtual void GoToVillage();
	virtual void GoToVillageDefeated(); // GoToVillage from defeated menu

	virtual void GoToMap();

	virtual void GoToDeferredStage(int32 InNextClientStageId);
	virtual void GoToNextArea();

	virtual void EnterShop(int32 nWhereShop);

	void RecacheLevelResources();

	/** Difficulty "Scale" derived from DifficultyLevel and Info settings (or server master data). */
	virtual float GetCurrentStageDifficultyScale() { return 0.f; }
	virtual float GetDamageRandomRate() { return 1.f; }

	virtual float GetPVEProportionalConstant();
	virtual float GetModifiedBossmobMaxHealth() {
		return 1.f;
	}

	virtual float GetDamageRateByGameModeType() {
		return 1.f;
	}
	virtual float GetHealthRateByGameModeType();

	virtual bool IsNetGameMode() const {
		return false;
	}
	virtual bool IsPVPGameMode() const {
		return false;
	}

	virtual float GetPVPMaxDamageRatioByGameModeType();
	virtual float GetPVPDamageScaleByGameModeType();

	float GetAttackCoefficient();
	float GetDefenseCoefficient();

	virtual EB2SkillLODType GetSkillLODType() {
		return EB2SkillLODType::ESLT_NONE;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	EPCClass GetCurrentPCClassEnum();

	/** Get PCClassInfo object for CurrentPCClassEnum */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	class UB2PCClassInfo* GetCurrentPCClassInfo();

	/**
	 * Spawns a player character in designated world with a EPCClass and other parameters..
	 * It simply calls UWorld::SpawnActor inside, so do other things manually if necessary, such as FinishSpawning call for defer construction.
	 * Provided for manual PC spawn in Tag, etc.
	 */
	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame") -> Cannot make this BlueprintCallable due to FActorSpawnParameters. Need one more wrapping.
	virtual class ABladeIIPlayer* SpawnPCClassInfoPlayer(class UWorld* SpawnWorld, EPCClass InClass, 
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters, bool bSpawnAsDummyPuppet = false, UClass* OverridePawnClass = nullptr);

	/**
	 * Do some similar stuff like blueprint spawnactor utility for our PCClassInfo player spawn.
	 * @REMARK : In most common cases, better set bDeferConstruction to true and call FinishSpawning manually to let player controller available at BeginPlay timing.
	 */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "Spawn PC Class Info Player", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static class ABladeIIPlayer* StaticSpawnPCClassInfoPlayer(UObject* WorldContextObject, EPCClass InClass, const FTransform& SpawnTransform, 
		ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, bool bDeferConstruction = true, AActor* InOwner = nullptr);
protected:
	/** It is called right after spawning default player pawn for game world, before even register its components. */
	virtual void HookOnSpawnDefaultPawn(APawn* InSpawned) /*override*/;
public:

	/**
	 * It spawns player character of InClass, without possessing it by player controller.
	 * Most likely for cinematic purpose.
	 * Calling side need to cache the required reference and destroy manually if not necessary anymore.
	 *
	 * bPossessByAIController : The base BP is likely to have its AutoPossessAI as PlacedInWorld, so it won't get possessed by any controller if dynamically spawned.
	 *				In that case, newly spawned puppet would not be affected by gravity or other natural stuff, etc.. Set it as true if you don't want to see that.
	 */
	static class ABladeIIPlayer* SpawnPlayerCharAsPuppet(UObject* WorldContextObject, EPCClass InClass, 
															const FTransform& InSpawnTransform, bool bPossessByAIController = true, UClass* OverridePawnClass = nullptr);
	
	void FinishSpawnByPCClassInfo(); // To reset CurrentPCClassEnum after PC spawn.

	/** Wrapper to SkillRTManager's main operation to be called from Blueprint TagNotify. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual bool TagPlayerCharacter(AActor* Player);

	/** Request player character (perhaps local) resurrect to server */
	virtual bool RequestResurrect(EResurrectBuffType InResurrectBuffType);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void ForceFeedback();
	
	virtual bool IsAllowCameraWorking() { return true; }

	//================================================================================
	// Methods to handle/use actor lists.
protected:
	/** From spawn pool on a single mob spawn, either by wave spawn or summoning. Arguments almost reflects ABladeIICharacter::SetBirthPlace
	 * In the case of wave mob spawn notify, it will be soon followed by NotifySpawnPoolSingleWaveStart */
	virtual void NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo);
	/** Called from spawn pool actor which is just completed (all mobs in all waves are dead). */
	virtual void NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool);
	/** Another spawn pool event notification. */
	virtual void NotifySpawnPoolSingleWaveStart(class AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum);
	/** Notified when all mobs in a wave died. */
	virtual void NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum);
	/** Notified when one mob in a wave died. At this point, DyingMob actor might still be far from final destruction. See NotifyForgetACharacter for that use. */
	virtual void NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex);
	/** Notified when one mob in a wave just changed (advance) phase. DestroyedPrevRef is non-null only when this phase change caused re-spawn, otherwise null. */
	virtual void NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase);
	/** Notified when a summoned mob died. At this point, DyingMob actor might still be far from final destruction. See NotifyForgetACharacter for that use. */
	virtual void NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob);
	/** CharacterAboutToBeDestroyed will be gone after this call. It is like notification of setting NULL for references to CharacterAboutToBeDestroyed if any. */
	virtual void NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed);
	
public:
	void StopAllStageEventDirector(class AB2StageEventDirector* skipDirector);

protected:
	virtual void NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType);
	/** Stage event scene end notification does not necessarily mean that all the show is played to the end. It can be stop at middle, and this notify must be still valid. */
	virtual void NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector);

	/** Notification of UI page opening completion, ideally considering opening widget animation stuff, but in most cases without special modification, it probably notified on simple constructing moment */
	virtual void NotifyUIWidgetOpenComplete(FName InOpenedUI);

	virtual void StartEventScene_FinishAttack(ABladeIICharacter* Inflictor) final;

	void EnsureStageEventTimeDilation();

	virtual void RestoreWholeGameTimeScale(); // Restore to normally expected value from possible temporary setting.

public:
	virtual void AddSpawnPool(class AB2MonsterSpawnPool* InNewSpawnPool);
	virtual void AddStageEventDirector(class AB2StageEventDirector* InNewEventDirector);
	virtual void AddActiveCameraActor(class AB2ActiveCameraActor* InNewActiveCameraActor);
	class AB2ActiveCameraActor* FindActiveCameraActorByTag(FName ACATagName);
	virtual void AddAutoWayPoint(AB2AutoWayPoint* InNewWayPoint);
	const TArray<AB2AutoWayPoint*>& GetWayPointList() const { return AutoWayPoints; }
	virtual void AddTreasureChest(class AB2TreasureChestBase* InNewTreasureChest);
	virtual void RemoveTreasureChest(class AB2TreasureChestBase* InRemoveTreasureChest);
	virtual void AddDestructibleObject(class AB2DestructibleLevelObjBase* InNewDestructibleObject);
	virtual void RemoveDestructibleObject(class AB2DestructibleLevelObjBase* InRemoveDestructibleObject);
	const TArray<AB2DestructibleLevelObjBase*>& GetDestructibleList() const { return DestructibleObjects; }

	virtual void AddHomePoint(class AB2HomePoint*);
	class AB2HomePoint* GetHomePoint() const;
	
	virtual void AddDialogTriggerActor(class AB2DialogTriggerActorBase* InDlgTriggerActor);

	virtual void AddTaggedActor(class AActor* InTaggedActor);
	class AActor* GetTaggedActor(FName TagName);

	/** Get the list of mobs spawned from a specific wave of MosterSpawnPool here.
	* @param bOnlyLiveMobs : False will include dying mobs waiting for final destruction. */
	void GetSingleWaveMobList(int32 WaveNum, TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true);
private:
	/** Get the list of mobs spawned by MonsterSpawnPool wave
	* -> Make it public if specifically need wave mob list.. */
	void GetAllWaveMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true);
	/** Get the list of mobs spawned by summoning using MonsterSpawnPool settings.
	* -> Make it public if specifically need wave mob list.. */
	void GetAllSummonedMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true);

public:
	void GetAllCharacterList(TArray<class ABladeIICharacter*>& OutList);

	/** Get all mobs managed by this stage manager. This includes GetAllWaveMobList. Others like summoned mobs can be added.
	* @param bOnlyLiveMobs : False will include dying mobs waiting for final destruction. */
	virtual void GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true);

	/** Apply ActiveCamera if the player is currently under one of ActiveCameraActor's influence.
	 * Returns true if applied one.
	 * You can optionally override active camera's blend time by give positive BlendInTimeOverride. */
	bool ApplyActiveCameraIfAny(float BlendInTimeOverride = -1.0f);
//
	virtual void SetBaseActiveCamera();

	FORCEINLINE const TArray<class AB2ActiveCameraActor*>& GetActiveCameraArray()
	{
		return ActiveCameraActors;
	}
	FORCEINLINE const TArray<class AB2StageEventDirector*>& GetEventDirectorArray()
	{
		return EventDirectors;
	}
	FORCEINLINE const TArray<class AB2MonsterSpawnPool*>& GetSpawnPoolArray()
	{
		return SpawnPools;
	}
	FORCEINLINE const TArray<class AB2TreasureChestBase*>& GetTreasureChestArray()
	{
		return TreasureChests;
	}
	FORCEINLINE const TArray<class AB2DestructibleLevelObjBase*>& GetDestructibleObjectArray()
	{
		return DestructibleObjects;
	}

	FORCEINLINE EResurrectBuffType GetLastRequestedStageBuffType() const { return LastRequestedStageBuffType; }
	//================================================================================

	/** Summon monsters using SummonSettings having matching SummonIndex,
	 * from the active spawn pool for currently loaded world. 
	 * MaxRadius specifies possible area of summoned mobs around the Summoner. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual bool SummonFromSpawnPool(class ABladeIICharacter* Summoner, int32 SummonIndex, float MaxRadius);
		
	FORCEINLINE class AB2UIManager_InGameCombat* GetUIManager_InGameCombat() { return UIManager_InGameCombat; }

	/** Show a simple message text displayed at player HUD center, with optionally specifying DisplayTime.
	 * Need to manually turn it off if DisplayTime is less or equal to zero. 
	 * AnimIndex : Index of UMG animations prepared for the center message at player HUD widget. Default value for default open animation. */
	static void SetHUDCenterMessage(const FText& InMessageText, float DisplayTime, int32 AnimIndex = 0);
	/** Manually turn off HUD center message, if DisplayTime was 0 or less when you called SetHUDCenterMessage last time, but it works only in same level. */
	static void HideHUDCenterMessage();
	
	static void GetObjectInLevel(ULevel* _level /* GetWorld()->GetCurrentLevel()*/,UClass* _classType/* ASkeletalMeshActor::StaticClass()*/, TArray<AActor*>& _OutWasVisibleSkeletalActorsPool /*Out*/);
	//B2CameraAnim에서 위에 메소드를 사용해도 되지만 O(2n)의 속도를 들이기 때문에 따로 메소드를 만듬(알고리즘상 2n == n이지만 측청 시간은 다를 테니....)
	static void GetObjectHiddenInLevel(ULevel* _level /* GetWorld()->GetCurrentLevel()*/, UClass* _classType/* ASkeletalMeshActor::StaticClass()*/, TArray<AActor*>& _OutWasVisibleSkeletalActorsPool /*Out*/);
	static void GetObjectVisibleInLevel(const TArray<AActor*>& _OutWasVisibleSkeletalActorsPool /*In*/);

	/**
	 * Overridden to handle pause menu UI
	 */
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	/** Begin simple global blurring effect, actually using Depth-of-Field, but like blur virtually everything.
	 * @REMARK : BlurScale might not really do some in mobile. */
	bool BeginGlobalSceneBlur(float BlurScale);
	void EndGlobalSceneBlur();

	virtual void NotifyCharacterBeginPlay(class ABladeIICharacter* TargetCharacter){};

	/** Notified when the player character is dead and dead animation is played through. */
	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer);
	virtual void NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser);
	virtual void NotifyAnimMobDeadEnd(class ABladeIICharacter* TargetCharacter) {};
	virtual void NotifyTargetDamagedFromDamagedTarget(class ABladeIICharacter* DamageCauser, class ABladeIICharacter* DamagedTarget, float fDamageAmount) {};
	virtual void NotifyQTEMountBeginToIdle(class ABladeIIPlayer* pPlayer) {};
	virtual void NotifyPlayerStartSkill(class ABladeIIPlayer* pPlayer, EAttackState SkillAttackState) {};
	
	void DeactivateWorldMovablePointLight(); // Safety process.. to meet mobile dynamic point light limitation.

	// Direct notification to StageEventDirectors in world for some specific game events.
	void StartEventScene_GameBegin();
	void StartEventScene_GameClear();
	virtual void StartEventScene_MatchResult(bool HasLocalPlayerWon);

	/** Play a designated CommonSoundInfo BGM by specific ID. Previous BGM will be stop.
	 * @param bForceStopCurrent : If false, current BGM will stop only when designated sound is found. */
	void PlayBGMByCommonSoundID(ECommonSoundID InID, bool bForceStopCurrent = false);
	/** Play a designated CommonSoundInfo BGM by stage combat context. Previous BGM will be stop. */
	virtual void PlayBGMByStageContext(int32 InClientStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, bool bForceStopCurrent = false);

	void PlayBGMByUIScene(EUIScene InUIScene);


	virtual void PlayVictoryBGM();
	void PlayStageVictoryBGM(int32 IsGoldBonusStage, int32 IsBosStage);

	virtual void KillAllEnemy();


private:

	bool CheckConditionBroadcastMessage();
public:
	/** Stop BGM played by above functions. It will destroy and null-out CurrentPlayingBGM reference. */
	void StopBGM();
	/** Temporarily pause currently playing one, without destroying it. Can resume as well. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "SetPauseBGM")
	void SetPauseBGM(bool bPause);
	/** AMB can be additively played to BGM */
	void PlayAMBByCommonSoundID(ECommonSoundID InID, bool bForceStopCurrentAMB = false, bool bStopCurrentBGM = false);
	void StopAMB();
	/** Temporarily pause currently playing one, without destroying it. Can resume as well. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "SetPauseAMB")
	void SetPauseAMB(bool bPause);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "GetMatineePositionByEventName")
	static float GetMatineePositionByEventName(class ALevelSequenceActor* MatineeActor, FName EventName);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "SetAllowCullDistance")
	void B2GM_SetAllowCullDistance(bool bAllow, TArray<UPrimitiveComponent*> Components);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void PlayBGMCustomize(const FName& InName);

	/** Use the designated KeyTiem of the first found camera track group of MatineeActor as current camera */
	static void SetCameraMatineeCam(class ALevelSequenceActor * MatineeActor, float KeyTime);

	FORCEINLINE void SetCurrentActiveActiveCameraActor(class AB2ActiveCameraActor* InActiveCam) { CurrentActiveActiveCameraActor = InActiveCam; }
	FORCEINLINE class AB2ActiveCameraActor* GetCurrentActiveActiveCameraActor() { return CurrentActiveActiveCameraActor; }
	void CustomEndCurrentActiveActiveCameraActor();

	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true);
	virtual void GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLive = true){};
	virtual void SetupUIScene();

	virtual void OnStageBegin();

	// B2ContentsMode, B2ContentsModeState, B2ContentsModeState
	virtual void OnReceiveContentsModeStateNotify(int32 ContentsMode, int32 PrevModeState, int32 CurrentModeState);

	void CheckOpenModeStateNoticeUI(int32 ContentsMode, int32 CurrentModeState);

	virtual void CancelOrStopRepeatBattle();
	virtual void GiveUpGame();

	virtual bool IsDefeated(ABladeIIPlayer* Player) { return false; }

	void UpdateAdminMessage(const FString &InMessage, const int32 InDisplayTime);
	virtual void UpdateChatUI(bool bIsScrollEnd);
	void TryOpenChatSinglePopup(const FB2ChatMessage& InNewMessage);

	virtual class ICharacterDataStore* GetRemotePlayerDataStore(uint32 RemotePlayerIndex){ return nullptr; }

	virtual EB2GameMode GetB2GameModeType() { return EB2GameMode::Unknown; }
	virtual ABladeIICharacter* SpawnCharacter(class UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);
	virtual ACharacter* GetNearestPlayerCharacter(FVector, bool = false);
	virtual ACharacter* GetNearestPlayerCharacter(class ACharacter*, bool = false);

	virtual void SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer = NULL);
	virtual EResurrectGameModeType GetResurrectGameModeType() { return EResurrectGameModeType::EResurrectType_Stage; }
	virtual void SetPlayerCollisionType(ABladeIINetPlayer* pPlayer) {};

	/** The active spawn pool consistent for this loaded level, considering stage number and difficulty. */
	class AB2MonsterSpawnPool* GetActiveSpawnPool() { return TheActiveSpawnPool; }

	virtual void ShowCachedCompleteQuestNotify();
	virtual void OnQuestCompleteNotify(int32 QuestSlot);

	virtual EPCSpawnMotionState GetBornAgainSpawnMotionState() { return EPCSpawnMotionState::EPST_BornAgain; }
	virtual void RealtimeUpdateUIDoc();

	 
	virtual float GetTotalAttack(ICharacterDataStore*);
	virtual float GetTotalDefense(ICharacterDataStore*);

	virtual void OverrideDamageInfo(FDamageInfo&, const ABladeIICharacter*);
	virtual void OverridePauseForDuration(float pausetime, const ABladeIICharacter*);
#if WITH_EDITOR
	bool IsDefaultPawnTestBP() const;
#endif

#if WITH_EDITOR
	static bool bDevCheckBladeIIGameModeEnteredOnce;
#endif

	IGameRule* GetGameRule() const
	{
		//BII_CHECK(MyGameRule);
		return MyGameRule;
	}

	void SetGameRule(IGameRule* GameRule)
	{
		BII_CHECK(GameRule);
		if(MyGameRule)
			return;

		MyGameRule = GameRule;
	}

	void DeleteGameRule()
	{
		if (!MyGameRule)
			return;

		delete MyGameRule;
		MyGameRule = nullptr;
	}

	virtual void OnPlatformPause();

protected:
	virtual void RealtimeUpdateUIDocSkill(ABladeIIPlayer* const B2Player);
	virtual void RealtimeUpdateUIDocHealth(ABladeIIPlayer* const B2Player);

private:
	uint32 DeliveryStartStageTicket;
	uint32 DeliveryStartCounterDungeonTicket;
	uint32 DeliveryForceStartCounterTicket;
	uint32 DeliveryQuestRewardTicket;
	int32 DeliveryStartHeroTowerTicket;
	uint32 UIWidgetOpenCompleteTicket;
	uint32 TogglePlayerAutoStateTicket;
	uint32 UpdateAccmulatedPlayTimeTicket;
	uint32 ShowMissionDialogTicket;
	uint32 GuildBuffSetttingTicket;
	uint32 StageBeginTicket;

	EB2GameMode CachedPreLoadingGameMode;
	// To extend the loading screen.. in some circumstance.
	static FString LastUsedLoadingImageName;

	IGameRule*	MyGameRule;
};

FORCEINLINE EPCClass ABladeIIGameMode::GetCurrentPCClassEnum()
{
	return CurrentPCClassEnum;
}

USTRUCT(BlueprintType)
struct FMatchingAnims
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* Ready_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* Ready_R;
};
