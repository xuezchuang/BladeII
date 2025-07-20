//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
////#include "BladeII.h"
//#include "B2StageGameMode.h"
//#include "TEST_ControlGameMode.generated.h"
//
//class TestControlGameModeGameRule : public StageGameRule
//{
//public:
//	virtual bool AllowToTag() const override { return false; } // Tend to touch tag button by mistake.. ControlGameMode has no tag anyhow.
//	virtual bool IsQTEEnabled() const override { return true; }
//	virtual bool AllowPlayerPointLight() const override { return false; } // Performance rule
//};
//
///**
// * For performance test.
// */
//UCLASS()
//class BLADEII_API ATEST_ControlGameMode : public AB2StageGameMode
//{
//	GENERATED_BODY()
//
//protected:
//
//	/** Set by level Blueprint */
//	UPROPERTY(Transient)
//	class ASkeletalMeshActor* CachedMinotaur;
//
//	UPROPERTY(Transient)
//	TArray<class ABladeIITestDummyPlayer*> AllSpawnedDummy;
//
//public:
//	ATEST_ControlGameMode(const FObjectInitializer& ObjectInitializer);
//
//	UFUNCTION(BlueprintCallable)
//	void SetResourceCacheFromBP(class ASkeletalMeshActor* InMinotaur);
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // Put everything inside this except UPROPERTY and UFUNCTIIONs
//
//	virtual void BeginDestroy() override;
//protected:
//	virtual void PreBeginPlay() override;
//	virtual void SubscribeEvents() override;
//	virtual void BeginPlay() override;
//	virtual void Tick(float DeltaSecond) override;
//
//	virtual const FString GetOpeningMovieName() override { return FString(TEXT("")); }
//	
//	void BeginTestSession();
//	void BeginTestSessionWithOptions(float InTestSessionTime, int32 InComplexityLevel);
//	class ABladeIITestDummyPlayer* SpawnSingleTestPlayerDummy(int32 InComplexityLevel, int32 InMyIndex, const FVector& InDummyCenterLocation);
//	void EndTestSession();
//
//	virtual EB2SkillLODType GetSkillLODType() override { return EB2SkillLODType::ESLT_ALL_LOD; }
//
//#if BII_STATS
//	virtual void DrawAdditionalPerfStatInfo(float DrawXSoFar, float DrawYSoFar) override;
//#endif
//
//public:
//
//	bool bTestSessionBegun;
//	
//	static float TotalTestSessionTime;
//	static int32 TestComplexityLevel;
//	
//	static float DummyAnimPlayInterval;
//	static int32 DummyEquipGrade;
//	static float DummyFloatingUIInterval;
//	static float DummyDamageFxInterval;
//	static float DummySpawnRadius;
//	static bool bDummyMobOn;
//	static bool bWingOn;
//	
//	FTimerHandle TestSessionTH;
//
//#endif
//
//};
