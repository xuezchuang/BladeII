#pragma once

#include "BladeIIPlayer.h"
#include "B2Airport.h"
#include "BladeII.h"
#include "BladeIITestDummyPlayer.generated.h"

/**
 * For various testing purpose.. especially performance profiling.
 */
UCLASS()
class ABladeIITestDummyPlayer : public ABladeIIPlayer
{
	GENERATED_BODY()
	
	EAttackState ThisTickFinalAttackState;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // No UPROPERTY within this block..

	// Set attack state to manually play specific animation.
	EAttackState DirectAttackState;
	/** For combo attack state sequential increase. */
	int32 FinalAttackStateMaintainCount;
	const int32 FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT = 1;
	
	/** If true, DirectAttackState will be randomly set for each anim play */
	uint32 bUseRandomAttackState : 1;

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

#endif

public:

	ABladeIITestDummyPlayer(const FObjectInitializer& ObjectInitializer);

	/** "Direct" AttackState means this state is to be "directly" picked up by AnimBP.. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	EAttackState GetDirectAttackState() const;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // No UFUCTION within this block..

	// AActor interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void Tick(float) override;
		
	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;

	virtual void ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled = false) override;
private:
	void AttackStateEndInternal(EAttackState InLastAttackState);
public:
	virtual int32 GetSkillLOD() const { return 1; }

	virtual bool IsFineToStartAnyNewInputHandling() override;
	virtual bool IsFineToStartAnyNewMoveInputHandling() override;

	virtual bool IsHeadPointLightAllowed() const override { return false; }
protected:
	virtual void SubscribeEvents() override;
	virtual void UnsubscribeEvents() override;

	void RandomSpawnFloatingUIs();
	void RandomSpawnDamageFx();

	void BeginNextAnimCommon(EAttackState SpecifiedNextAnim = EAttackState::ECS_End);
public:
	void SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword);
	void SetDirectAttackState(EAttackState InState);
	void SetUseRandomAttackState(bool bInRandomAttackState);
	void SetCooltimeBetweenAnim(float InCooltime);
protected:
	void GoToNextRandomAttackState();
public:
	/**
	 * The main util for using this player class..
	 *   bPossessByAIController : Set it true to make spawned dummy behave least naturally like getting affected by gravity.
	 */
	static ABladeIITestDummyPlayer* SpawnWithStandardEquipment(UObject* WorldContextObject, EPCClass InClass, const FTransform& InSpawnTransform, 
		int32 InEquipmentGrade, float InSpawnFloatingUIInterval = 0.0f, float InSpawnDamageFxInterval = 0.0f, bool bShowWing = false, int32 InWingGrade = MAX_WING_EVOLUTION_GRADE,
		bool bPossessByAIController = true);

#endif
};

FORCEINLINE EAttackState ABladeIITestDummyPlayer::GetDirectAttackState() const
{
	return ThisTickFinalAttackState;
}
