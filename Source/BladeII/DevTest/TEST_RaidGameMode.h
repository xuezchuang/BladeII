// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "B2StageGameMode.h"
#include "BladeIICharacter.h"
#include "TEST_RaidGameMode.generated.h"

class TestRaidGameModeGameRule : public StageGameRule
{
public:
	virtual bool AllowToTag() const override { return false; } // Tend to touch tag button by mistake.. 
	virtual bool IsQTEEnabled() const override { return false; }
	virtual bool AllowPlayerPointLight() const override { return false; } // Performance rule
};

/**
 * For performance test, with no network connection.
 */
UCLASS()
class BLADEII_API ATEST_RaidGameMode : public AB2StageGameMode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "RaidTestSetup")
	ENPCClass DummyTargetMobClass;
	UPROPERTY(EditAnywhere, Category = "RaidTestSetup")
	ENPCClassVariation DummyTargetMobVariation;

	UPROPERTY(Transient)
	class ABladeIITestDummyNPC* DummyRaidTargetMob;

	UPROPERTY(Transient)
	TArray<class ABladeIITestDummyPlayer*> AllDummyPlayers;

public:
	ATEST_RaidGameMode(const FObjectInitializer& ObjectInitializer);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // Put everything inside this except UPROPERTY and UFUNCTIIONs

	virtual void BeginDestroy() override;
protected:
	virtual void PreBeginPlay() override;
	virtual void SubscribeEvents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSecond) override;

	virtual const FString GetOpeningMovieName() override { return FString(TEXT("")); }
	
	void GatherDummyMarkers(class ASkeletalMeshActor*& OutDummyTargetMobMarker, TArray<class ASkeletalMeshActor*>& OutDummyPlayerMarkers);

	void BeginTestSession();
	void BeginTestSessionWithOptions(float InTestSessionTime);
	class ABladeIITestDummyPlayer* SpawnSingleTestPlayerDummy(int32 InMyIndex, const FVector& InDummyCenterLocation);
	class ABladeIITestDummyNPC* SpawnSingleTestMobDummy(const FVector& InLocation, const FRotator& InRotation);
	void EndTestSession();

	virtual EB2SkillLODType GetSkillLODType() override { return EB2SkillLODType::ESLT_ALL_LOD; }

public:

	bool bTestSessionBegun;
	
	float TotalTestSessionTime;
	
	float DummyAnimPlayInterval;
	int32 DummyEquipGrade;
	float DummyFloatingUIInterval;
	float DummyDamageFxInterval;
	float DummySpawnRandRadius;
	float CamDistScale;
	bool bWingOn;
	
	bool bGatherPerfStats;

	FTimerHandle TestSessionTH;

#endif

};
