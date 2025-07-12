// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIGameMode.h"
#include "B2StageEventDirector.h"
#include "../AI/B2AutoWayPoint.h"
#include "B2StageGameModeBase.generated.h"

/**
* GameMode for the most basic stage or scenario based mode game. Combat through stages.
*/
UCLASS(Abstract)
class BLADEII_API AB2StageGameModeBase : public ABladeIIGameMode
{
	GENERATED_BODY()

protected:
	/** The main worker here. This StageManager was in fact, introduced earlier than StageGameMode.
	 * Just the legacy of old code when we had no idea on extending GameMode for stage game. */
	UPROPERTY(Transient)
	class AB2StageManager* StageManager;

public:
	AB2StageGameModeBase(const FObjectInitializer& ObjectInitializer);

	/** Return existing one, or spawn a new one. */
	class AB2StageManager* GetStageManager();
	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true) override;
	virtual float GetCurrentStageDifficultyScale() override;
	float GetDamageRandomRate() override;
	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer) override;
	virtual void NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser) override;
	virtual void SetResurrectPlayerPosition(ABladeIIPlayer* pPlayer = NULL) override;

	virtual bool IsDefeated(ABladeIIPlayer* Player) override;

	//================================================================================
	// Methods to handle/use actor lists.
protected:
	virtual void NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo) override;
	virtual void NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool) override;
	virtual void NotifySpawnPoolSingleWaveStart(class AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum) override;
	virtual void NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum) override;
	virtual void NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex) override;
	virtual void NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase) override;
	virtual void NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob) override;
	virtual void NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed) override;
	virtual void NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType) override;
	virtual void NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector) override;
	virtual void NotifyUIWidgetOpenComplete(FName InOpenedUI) override;
public:
	virtual void AddSpawnPool(class AB2MonsterSpawnPool* InNewSpawnPool) override;
	virtual void AddStageEventDirector(class AB2StageEventDirector* InNewEventDirector) override;
	virtual void AddActiveCameraActor(class AB2ActiveCameraActor* InNewActiveCameraActor) override;
	virtual void AddAutoWayPoint(AB2AutoWayPoint* InNewWayPoint) override;
	virtual void AddTreasureChest(class AB2TreasureChestBase* InNewTreasureChest) override;
	virtual void RemoveTreasureChest(class AB2TreasureChestBase* InNewTreasureChest) override;
	//================================================================================

protected:
	/** Called before any other actor's BeginPlay is called. */
	virtual void PreBeginPlay() override;
	virtual void BeginPlay() override;
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;
	virtual bool RequestResurrect(EResurrectBuffType InResurrectBuffType) override;

	virtual void DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty = true) override;

	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	/** Returns skill animation identifiers to pre-load. Skill anim data has big potion within whole PCClassInfo. */
	virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad() override;
	/** Load any info assets, to be lazy loaded (TSoftObjectPtr stuff) and required by this game mode.
	 * It is expected to be slow and cause much memory increase. */
	virtual void PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie) override;
	virtual void PreBuildTagCharMesh();
	
	void GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode);
	void PreloadAllExpectedDialogAssets();

	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets);

	AActor* FindPlayerStartForCurrStage(AController* InController); // Find specified one by StageInfo


public:
	/** Interface to reset stage without world re-loading. Be careful when you use it. */
	virtual void  ResetAssumeNoWorldLoading();

protected:
	bool bFlagDeferredGoToMap;
	bool bFlagDeferredGoToNextArea;
};

