// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2StageGameModeBase.h"
#include "B2CounterAttackGameMode.generated.h"

class CounterAttackGameRule : public DefaultGameRule
{
public:
	virtual bool AllowToTag() const override 
	{ 
		return false; 
	}
};

/**
* GameMode for the most basic stage or scenario based mode game. Combat through stages.
*/
UCLASS()
class BLADEII_API AB2CounterAttackGameMode: public AB2StageGameModeBase
{
	GENERATED_BODY()

public:
	AB2CounterAttackGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PreBeginPlay() override;
	virtual void SetupUIScene() override;
	virtual void RestartPlayer(class AController* NewPlayer) override;
	void ModifyInputCommand(class ABladeIIPlayer* Player);
	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::CounterDungeon; }
	//virtual EResurrectGameModeType GetResurrectGameModeType() override { return EResurrectGameModeType::EResurrectType_Stage; }

	virtual void GoToVillage() override;
	virtual void GoToVillageDefeated() override;
	virtual void GoToMap() override;

	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;
	
	static void GetFullStackSkillId(EPCClass InCharacterClass, TArray<int32>& OutSkillArray);	

	UFUNCTION(BlueprintCallable, Category="CounterAttackDG")
	void MobAttackStarted(class ABladeIICharacter* Mob);

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void MobAttackFinished(class ABladeIICharacter* Mob);

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void OnBeginMobStun();

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void OnFinishMobStun();

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void OnIntoDownStay();

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void OnOutofDownStay();

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void SetDifficultyFactor(EUIBattleDifficulty Difficulty, float MobAttackRateScale);

	bool SetFinishState(class ABladeIICharacter* Mob);

	FORCEINLINE bool IsEnablePlayerAttack() { return bEnableNormalAttack; }
	
	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void MoveCharacters(const FVector& DiffMove, bool bPauseCameraLag, float PauseTime = 0.f, float TimeCameraMove = 1.f);
	
	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void SetLimitMoveCharacters(const FBox& LimitSpace);

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void SetDecideAccuracyTime(float Perfect, float Great, float Good);

	UFUNCTION(BlueprintCallable, Category = "CounterAttackDG")
	void OnCounterGuideNotify();

	virtual void PlayVictoryBGM() override;

	virtual void OnQuestCompleteNotify(int32 QuestSlot) {}

	virtual float GetCurrentStageDifficultyScale() override;

	virtual float GetTotalAttack(ICharacterDataStore*) final;
	virtual float GetTotalDefense(ICharacterDataStore*) final;

	virtual void OverrideDamageInfo(FDamageInfo&, const ABladeIICharacter*) final;
	virtual void OverridePauseForDuration(float pausetime, const ABladeIICharacter*) final;

	virtual void PlayBGMByStageContext(int32 InClientStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, bool bForceStopCurrent = false) final;
protected:
	virtual void SubscribeEvents() override;

	/** Notified when one mob in a wave just changed (advance) phase. DestroyedPrevRef is non-null only when this phase change caused re-spawn, otherwise null. */
	virtual void NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase) override;

	virtual TArray<EPCClass> GetPCClassesToPreLoad() override;
	/** Returns skill animation identifiers to pre-load. Skill anim data has big potion within whole PCClassInfo. */
	//virtual TArray<FCombinedPCSkillAnimID> GetPCSkillAnimsToPreLoad() override;
	virtual void PreBuildTagCharMesh() override {/* Do nothing here, as CounterAttackGM has no Tag character. */};

	void FinishAttack();

	void CameraLagStart();
	
	void ReserveShowBadAccuracy(float time);
	void ShowBadAccuracy();

	virtual void NotifyUIWidgetOpenComplete(FName InOpenedUI) override;

private:
	void EnablePlayerNormalAttack();
	void DisablePlayerNormalAttack();

	float		GuardStartedTime;

	FTimerHandle BadAccuracyHandle;
	FTimerHandle CameraLagStartTimeHandle;

	float PerfectCheckTime;
	float GreatCheckTime;
	float GoodCheckTime;

	UPROPERTY(Transient)
	bool bEnableNormalAttack;
	float DefaultMobAnimPlayRate;

	bool bTriggeredFinishAttack;
	bool bEndFinishAttack;

	float CameraLagSpeed;

	FBox LimitMoveCharacter;
	FVector MovedDelta;

	TMap<EUIBattleDifficulty, float> DifficultyMobAttackRateMap;

	TWeakObjectPtr<class UAnimSequenceBase> MobAttackAnim;
	TWeakObjectPtr<class ABladeIICharacter> BossMob;
};