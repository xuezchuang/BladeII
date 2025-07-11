// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimInstance.h"
#include "B2AnimInstance.generated.h"

/**
 * 
 */

DEFINE_LOG_CATEGORY_STATIC(LogB2Animation, Log, All);

UCLASS()
class BLADEII_API UB2AnimInstance : public UAnimInstance
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	float CurrentSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	float LastComboEndTime;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboNext;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboEnd;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboEnabled;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboReserved;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboNextEnabled;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboSkill;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bCounterRserved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	bool bAllowedDash;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bDashEnabled;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bWarpEnabled;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bDamaged;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bDamageToggle;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	int32 DamageNum;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bGuarding;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	float LastGuardTime;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bIsRunning;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bGuardEnable;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bIsInputEnable;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bAllowGuardStart;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bStunned;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	EAttackState AttackState;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	ESkillAnimType SkillAnimIndex;
	
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bIsSpawnEnd;
	
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bDead;
	
	//UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	//EPCSpawnMotionState SpawnMotionState;
	
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bSkillCancelled;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	EAttackType DamageAnimIndex;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	EGuardBreakLogicType GuardBreakLogic;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bPreTagEnd;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bInQTEState;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bQTEAttack;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	int32 QTEStateNum;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bSmash;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bComboNextThisFrame;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	EQTEType QTEType;

	/*
		True이면 공격 시작(ECS_Combo1) 로 진입이 가능한 상태
	*/
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bHadRestartAttack;
	
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	ENPCClass QTENPCClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	bool bQTERun;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	uint8 GameModeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	float cfg_DashMinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	float cfg_DashMaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	float cfg_WarpMaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	float cfg_RunningSpeed;

	// Animation Common
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	bool bUseOneCounterAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimState")
	bool bForceRotateInRootMotion;


	// Fighter
	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	int32 AttackDir;

	UPROPERTY(BlueprintReadWrite, Category = "AnimState")
	class ABladeIIPlayer* OwnerPlayer;

	UPROPERTY(BlueprintReadWrite, Category = "CustomAnim")
		bool bStartCustomAnim;

	UPROPERTY(BlueprintReadOnly, Category = "CustomAnim")
		UAnimSequenceBase* CustomAnimSequence;

	virtual void OnAnimStateChanged(const FName& PrveState, const FName& NewState);
	virtual void RegisterStateChangeDelegate(const FName& MachineName);
	virtual void RegisterNativeTransition(const FName& MachineName);

	UFUNCTION()
	void OnStateChangeEntry(const struct FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex);

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeUninitializeAnimation() override;

	virtual bool ProcessDying();
	virtual bool ProcessComboEnd();
	virtual bool ProcessCounterHandling();
	virtual bool ProcessSkill();
	virtual bool ProcessGuard();

	virtual void IncreaseAttackState(int32 IncreaseLevel = 1);

	bool IsNormalAttackState(EAttackState InAttackState);
	bool IsInDamageAnimation();

	UPROPERTY()
	FName CurrentAnimState;

	// Simulated Role Only

	UPROPERTY()
	FName ReplicatedState;

	UFUNCTION()
	bool NativeTransitionFunc(const FName& PrevState, const FName& NextState);

public:

	void SimulateAnimation();
	void SetReplicatedState(const FString& State);

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetCurrentSpeed(float Speed) { CurrentSpeed = Speed; }
	FORCEINLINE float GetCurrentSpeed() const { return CurrentSpeed; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetLastComboEndTime(float Time) { LastComboEndTime = Time; }
	FORCEINLINE float GetLastComboEndtime() const { return LastComboEndTime; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetAnimAttackState(EAttackState State, bool bForce = false);
	FORCEINLINE EAttackState GetAnimAttackState() const { return AttackState; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetSmash(bool InSmash) { bSmash = InSmash; }
	FORCEINLINE bool IsSmash() const { return bSmash; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetDamaged(bool bInDamaged) { bDamaged = bInDamaged; }
	FORCEINLINE bool IsDamaged() const { return bDamaged; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetDamageNum(int32 InDamageNum) { DamageNum = InDamageNum; }
	FORCEINLINE int32 GetDamageNum() const { return DamageNum; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboNext(bool bNext) { bComboNext = bNext; }
	FORCEINLINE bool IsComboNext() const { return bComboNext; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboEnd(bool InComboEnd) { bComboEnd = InComboEnd; }
	FORCEINLINE bool IsComboEnd() const { return bComboEnd; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboEnabled(bool bEnable) { bComboEnabled = bEnable; }
	FORCEINLINE bool IsComboEnabled() const { return bComboEnabled; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboReserved(bool bReserved) { bComboReserved = bReserved; }
	FORCEINLINE bool IsComboReserved() const { return bComboReserved; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboNextEnabled(bool bEnable) { bComboNextEnabled = bEnable; }
	FORCEINLINE bool IsComboNextEnabled() const { return bComboNextEnabled; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboSkill(bool bSKill) { bComboSkill = bSKill; }
	FORCEINLINE bool IsComboSkill() const { return bComboSkill; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetCounterReserve(bool bReserve) { bCounterRserved = bReserve; }
	FORCEINLINE bool IsCounterReserved() const { return bCounterRserved; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetDashEnabled(bool bEnable) { bDashEnabled = bEnable; }
	FORCEINLINE bool IsDashEnabled() const { return bDashEnabled; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetWarpEnabled(bool bEnable) { bWarpEnabled = bEnable; }
	FORCEINLINE bool IsWarpEnabled() const { return bWarpEnabled; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetDamageToggle(bool bToggle) { bDamageToggle = bToggle; }
	FORCEINLINE bool IsDamageToggle() const { return bDamageToggle; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetGuarding(bool bGuard) { bGuarding = bGuard; }
	FORCEINLINE bool IsGuarding() const { return bGuarding; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetLastGuardTime(float Time) { LastGuardTime = Time; }
	FORCEINLINE float GetLastGuardTime() const { return LastGuardTime; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetRunning(bool bRunning) { bIsRunning = bRunning; }
	FORCEINLINE bool IsRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetGuardEnable(bool bEnable) { bGuardEnable = bEnable; }
	FORCEINLINE bool IsGuardEnable() const { return bGuardEnable; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetInputEnable(bool bEnable) { bIsInputEnable = bEnable; }
	FORCEINLINE bool IsInputEnable() const { return bIsInputEnable; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetAllowGuardStart(bool bAllow) { bAllowGuardStart = bAllow; }
	FORCEINLINE bool IsAllowGuardStart() const { return bAllowGuardStart; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetStunned(bool bStun) { bStunned = bStun; }
	FORCEINLINE bool IsStunned() const { return bStunned; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetSkillAnimType(ESkillAnimType SkillAnimType) { SkillAnimIndex = SkillAnimType; }
	FORCEINLINE ESkillAnimType GetSkillAnimType() const { return SkillAnimIndex; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetIsSpawnEnd(bool bEnd) { bIsSpawnEnd = bEnd; }
	FORCEINLINE bool IsSpawnEnd() const { return bIsSpawnEnd; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetDead(bool InDead) { bDead = InDead; }
	FORCEINLINE bool IsDead() const { return bDead; }

	//UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	//void SetSpawnMotionState(EPCSpawnMotionState MotionState) { SpawnMotionState = MotionState; }
	//FORCEINLINE EPCSpawnMotionState GetSpawnMotionState() const { return SpawnMotionState; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetSkillCanceled(bool bCanceled) { bSkillCancelled = bCanceled; }
	FORCEINLINE bool IsSkillCanceled() const { return bSkillCancelled; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetDamageAnimIndex(EAttackType DamageAttackType);
	FORCEINLINE EAttackType GetDamageAnimIndex() const { return DamageAnimIndex; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetGuardBreakLogic(EGuardBreakLogicType InGuardBreakLogic) { GuardBreakLogic = InGuardBreakLogic; }
	FORCEINLINE EGuardBreakLogicType GetGuardBreakLogic() const { return GuardBreakLogic; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetPreTagEnd(bool bTagEnd) { bPreTagEnd = bTagEnd; }
	FORCEINLINE bool IsPreTagEnd() const { return bPreTagEnd; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetInQTEState(bool InQTEState) { bInQTEState = InQTEState; }
	FORCEINLINE bool IsInQTEState() const { return bInQTEState; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetQTEAttack(bool InQTEAttack) { bQTEAttack = InQTEAttack; }
	FORCEINLINE bool IsQTEAttack() const { return bQTEAttack; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetQTEStateNum(int32 StateNum) { QTEStateNum = StateNum; }
	FORCEINLINE int32 GetQTEStateNum() const { return QTEStateNum; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetComboNextThisFrame(bool bThisFrame) { bComboNextThisFrame = bThisFrame; }
	FORCEINLINE bool IsComboNextThisFrame() const { return bComboNextThisFrame; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetQTEType(EQTEType InQTEType) { QTEType = InQTEType; }
	FORCEINLINE EQTEType GetQTEType() const { return QTEType; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetHadRestartAttack(bool bRestartAttack) { bHadRestartAttack = bRestartAttack; }
	FORCEINLINE bool HasRestartAttack() const { return bHadRestartAttack; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetQTENPCClass(ENPCClass InQTENPCClass) { QTENPCClass = InQTENPCClass; }
	FORCEINLINE ENPCClass GetQTENPCClass() const { return QTENPCClass; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void SetQTERun(bool bRun) { bQTERun = bRun; }
	FORCEINLINE bool IsQTERun() const { return bQTERun; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstace")
	void SetForceRotateInRootMotion(bool bForce);
	FORCEINLINE bool IsForceRotateInRootMotion() const { return bForceRotateInRootMotion; }

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	bool StartCustomAnim(UAnimSequenceBase* InAnimAsset);

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	bool IsOwnerAuthority();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateAbnormalState();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateQTEState();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateDamageState();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateDamageEnd();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateRunning();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateDashOrWarpEnable();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateComboNext();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void UpdateBeginAttack();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void OnSpawnEnd();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	void OnDeadEnd();

	UFUNCTION(BlueprintCallable, Category = "B2AnimInstance")
	const float GetGameTime() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsCounterDungeon() const;
};
