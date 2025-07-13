// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2StageGameMode.h"
#include "B2DimensionGameMode.generated.h"

class DimensionGameRule : public DefaultGameRule
{
public:
	virtual bool AllowToTag() const override { return false; }
	virtual bool IsQTEEnabled() const override { return true; }
	virtual bool AllowPlayerPointLight() const override { return true; }
};

UCLASS()
class AB2DimensionGameMode : public AB2StageGameModeBase
{
	GENERATED_BODY()

public:
	AB2DimensionGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PreBeginPlay() override;
	virtual void BeginPlay() override;
	virtual void SubscribeEvents() override;
	virtual void SetupUIScene() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void PlayVictoryBGM() override;
	virtual void GiveUpGame() override;
	virtual float GetCurrentStageDifficultyScale() override;
	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer) override;
	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::Dimension; }
	virtual bool IsQTEEnabled() { return true; }

	virtual void GoToVillage() final;
	virtual void GoToVillageDefeated() final;
	virtual void GoToMap() final;

	virtual void NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo) override;
	virtual void NotifySpawnPoolSingleWaveStart(class AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum) override;
	virtual void NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum) override;
	virtual void NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex) override;
	virtual void NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser) override;

	virtual float GetTotalAttack(ICharacterDataStore*);
	virtual float GetTotalDefense(ICharacterDataStore*);

	FTimespan GetElapsedTime();
	void CheckAura();
protected:
	virtual void DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty) final;
	bool CheckBuff_Me(EBloodStoneType type);
	bool IsBuffTargetMe(EBloodStoneType type);

private:
	bool CheckBloodStoneAlive();
	bool CheckBloodStone(class ABladeIICharacter* SpawnedMob);

	void SetttingAura_Mob(EBloodStoneType type, float amount);
	void SetttingAura_Me(EBloodStoneType type, EDamageLogicType damagetype, float amount, class ABladeIICharacter* TargetCharacter);

	void ReleaseAura_Mob(EBloodStoneType type);
	void ReleaseAura_Me(EBloodStoneType type);

	void GotoDimensionMain();

protected:
	DimensionGameRule* GameRule;
	TArray<class ABladeIICharacter*> SpawnedMobs;
	class AB2MonsterSpawnPool* CashedSpawnPool;

};
