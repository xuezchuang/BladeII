#pragma once

#include "BladeIICharacter.h"
#include "BladeII.h"
#include "BladeIITestDummyNPC.generated.h"

/**
 * For various testing purpose.. especially performance profiling.
 * It might share duplicated functionalities with ABladeIITestDummyPlayer.
 * A NPC spawned as this class won't be possessed by AIController. Just do dummy animation.
 */
UCLASS()
class ABladeIITestDummyNPC : public ABladeIICharacter
{
	GENERATED_BODY()
	
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // No UPROPERTY within this block..

	// Set attack state to manually play specific animation.
	EAttackState DirectAttackState;
	
	/** If true, DirectAttackState will be randomly set for each anim play */
	uint32 bUseRandomAttackState : 1;

	/** Normally true for dummy, but there could be exceptions.. */
	uint32 bMaintainLocation : 1;
	uint32 bMaintainRotation : 1;

	float CooltimeBetweenAnim;
	FTimerHandle CooltimeBetweenAnimTH;

	/** Floating UIs (damage num, abnormal state, etc) spawn rate per sec. 0 or less will spawn none. */
	float SpawnFloatingUIInterval;
	float NextTimeSpawnFloatingUIInterval;
	float TimeSinceLastSpawnFloatingUITime;
	/** DamageEffect (by DamageEffectInfo) spawn rate per sec. 0 or less will spawn none. */
	float SpawnDamageFxInterval;
	float NextTimeSpawnDamageFxInterval;
	float TimeSinceLastSpawnDamageFxTime;
public:
	// Given at spawn moment.
	FVector InitialLocation;
	FRotator InitialRotation;

#endif

public:

	ABladeIITestDummyNPC(const FObjectInitializer& ObjectInitializer);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void SetAttackState(EAttackState InAttackState) override;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // No UFUCTION within this block..

	// AActor interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void Tick(float) override;
	
	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;

	virtual void InitializeCombatStats() override;

	//virtual void ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled = false) override;
		
protected:
	void OnComboEnd(EAttackState LastState);

	void RandomSpawnFloatingUIs();
	void RandomSpawnDamageFx();

	void BeginNextAnimCommon(EAttackState SpecifiedNextAnim = EAttackState::ECS_End);
public:
	void SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword);
	void SetDirectAttackState(EAttackState InState);
	void SetUseRandomAttackState(bool bInRandomAttackState);
	void SetCooltimeBetweenAnim(float InCooltime);
	void SetMaintainLocation(bool bMaintain);
	void SetMaintainRotation(bool bMaintain);
protected:
	void GoToNextRandomAttackState();
public:
	EAttackState GetDirectAttackState() const;

	/**
	 * The main util for using this NPC class..
	 *   bPossessByAIController : Set it true to make spawned dummy behave least naturally like getting affected by gravity.
	 */
	static ABladeIITestDummyNPC* SpawnUtil(UObject* WorldContextObject,
		ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& InSpawnTransform, 
		float InSpawnFloatingUIInterval = 0.0f, float InSpawnDamageFxInterval = 0.0f);
#endif
};

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void TestRandomSpawnFloatingUIs(ABladeIICharacter* InOwnerDummy, float HPProbabilityAlpha = 0.5f /* 0.0 ~ 1.0. The rate between HP floating UI and other Buff mode floating UI. */);
void TestRandomSpawnDamageFx(ABladeIICharacter* InOwnerDummy);
FORCEINLINE EAttackState ABladeIITestDummyNPC::GetDirectAttackState() const
{
	return DirectAttackState;
}
#endif
