// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIPlayer.h"
#include "B2GuildGameMode.h"
#include "BladeIIMercenaryPlayer.generated.h"
/**
 * 
 */

struct FMercenarySkillData
{
	int32 SkillID;
	int32 SkillAttackIndex;
	float MaxSkillCoolTime;
	float RemainSkillCoolTime;
	bool bUseSkill;
	bool bAvailableSkill;

	bool IsRemainCooltime();
	void ResetSkillCoolTime();
};

UCLASS(Blueprintable)
class ABladeIIMercenaryPlayer : public ABladeIIPlayer, public IGuildPawnInterface
{
	GENERATED_BODY()

public:
	ABladeIIMercenaryPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeCombatStats() override;
	void InitCombateStatsForParam(const FCombatStatParam& CombatStatParam);

	bool CanRotateToTarget();
	bool IsCurrentAttackIndexSkill() const;
	bool IsAnotherNPCUsingSkill();
	bool StartUseSkill();
	void ReserveUseSkill();
	bool IsActiveUseSkill(bool bCheckCharacterState = true);
	void SetPlayerControlSkill(bool bControl);
	void SetMercenaryInfo(const FGuildMercenaryInfo& InMercenaryInfo);
	void SetMercenaryUsingState(TSharedPtr<FNPCUsingSkillState> InUsingSkillState);
	const FGuildMercenaryInfo& GetMercenaryInfo();

	bool IsSkillUsable();
	virtual bool IsSkillAttacking() const override;
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetTeamNum(int32 InTeamNum) override;

	virtual void StopAttack() override;

	virtual void SetAttackState(EAttackState InAttackState) override;

	virtual void OnAttackState() override;
	
	virtual void OnEndAttackState() override;

	virtual void OnAnimMobDeadEnd(bool bInUseDeadRagDoll, float InRemainLifeSpan, bool bInUseDeadSink = false, float InDeadSinkDelayTime = 0.f, float InDeadSinkZOffset = 0.f, float InDeadSinkDuration = 0.f);

	virtual bool GetAreaDamageTargetLocation(float RandMaxDist, FVector& OutTargetLocation, const FVector& LocationOffset, bool bSelfTarget) override;

	virtual void PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak) override;

	virtual bool CalcAIAttackIndex(float TargetDistance, int32& OutAttackIndex, float& OutRadius) override;

	virtual const FGuildPlayerInfo& GetGuildPlayerInfo() override;
	virtual bool PawnIsAlive() override;
	virtual class ABladeIICharacter* GetPawnInstance() override;
	virtual FVector GetLocation() const override;
	virtual AController* GetPawnController() override;
	virtual FString GetPlayerStartName() override;

	void SetFloorRingInfo(class UParticleSystem* InFloorRingPS, FLinearColor InFloorRingColor, float InFloorRingColorMultiplier);
	virtual void ResetSkillCooltime() override;

	TSharedPtr<FMercenarySkillData> GetUseSkillData();

	virtual void FilterTeamBuffAffectMember(ETeamBuffType TeamBuffType, TArray<class ABladeIICharacter*>& InChracters) override;

//player 특성 버림
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EnableInput(APlayerController*) override;
	virtual void DisableInput(APlayerController*) override;
	virtual void Destroyed() override;
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	virtual float GetMoveAcceptableRadius() override;

	virtual float GetShield() const override;
	virtual void SetShield(const float NewShieldAmount) override;

	virtual bool IsComboAttacking() const override;
	virtual bool IsCounterAttacking() const override;

	virtual bool HadHugeDamaged() override;
	virtual bool IsGuarding() override;

	virtual class ABladeIICharacter*  GetNearestEnemyCharacter() override;

	virtual void PostActorCreated() override;

	virtual ENPCClassVariation GetNPCClassVariation() const override;

	virtual bool GetAbleToMove() override;

	virtual FVector GetVelocity() const override;

	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const override;

	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser) override;

	virtual void InitUpdateUIDoc() override;

	virtual float GetBonusBuffDamageDecreaseAmount() const override;
	virtual float GetBuffReflectionDamageRate() const override;
	virtual float GetBonusBuffHealHPByHit() const override;
	virtual float GetBonusBuffHealHP() const override;
	virtual float GetBonusBuffIncMoveSpeed() const override;

	virtual float GetBonusBuffReceiveDamageDecreaseRate() const override;
	virtual float GetBonusBuffAttackDamageIncreaseRate() const override;

	virtual float GetAttackDamageIncreaseRateByClass(EPCClass TargetClass) const override;
	virtual float GetReceiveDamageDecreaseRateByClass(EPCClass AttackerClass) const override;

	virtual float GetDamageRadiusIncreaseRate() const override;

	virtual int32 GetBonusBuffAffectNum() const override;
	virtual float GetBonusBuffDuration() const override;

	virtual bool CanBeTarget() const override;

	virtual float GetStaticOptionAppliedWalkSpeed() const override;

	virtual bool ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage) override;

	virtual class UMaterialInstanceDynamic* GetChainMaterial() override;

	virtual bool IsForceApplyHitAnimation() override;

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;

protected:
	void OnMercenaryStartSkill(int32 InSkillAttackIndex);
	void UpdateSkill(float DeltaTime);
	
	void AddStatMercenaryBySkill(EMercenarySkillOption InOption, float InValue);
	void AddSkillStat(EMercenarySkillOption InOption, float InValue);
	void AddPassiveSkill(EMercenarySkillOption InOption, float InValue);
	void AddMercenaryMastery(EMercenarySkillOption InOption, float InValue);
	
	virtual void InitializeCombatOptionCacheValues() override; 

	virtual const TArray<int32>& GetInvalidAttackIndices() override;

	virtual float GetDamageIncreaseRate()			const override;
	virtual float GetBossDamageIncreaseRate()		const override;
	virtual float GetCriticalRate()					const override;
	virtual float GetCriticalDamageRate()			const override;
	virtual float GetDamageDecreaseRate()			const override;
	virtual float GetMeleeDamageDecreaseRate()		const override;
	virtual float GetRangeDamageDecreaseRate()		const override;
	virtual float GetBossDamageDecreaseRate()		const override;
	virtual float GetResistCriticalRate()			const override;
	virtual float GetHealHPAmountRateOnKill()		const override;
	virtual float GetAbsorbHPAmountRateOnAttack()	const override;
	virtual float GetDurationDecreaseRate()			const override;

	void ChangeFloorRingPS(class UParticleSystem* NewFloorRingPS);

//player 특성 버림
	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType) override;
	virtual void ProcessTakeDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual class UAnimSequenceBase* GetAnimStateOverrideSequence() override;

	virtual void OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride) override;

	virtual void NotifyTargetDamaged(class ABladeIICharacter* DamagedTarget, float DamageAmount);

	virtual void CharacterBuffUpdateUIdoc() override;

private:
	void SetMercenarySkill(const FGuildMercenaryInfo& InMercenaryInfo);

private:
	UPROPERTY(Transient)
		TArray<class UB2PassiveSkill*> PassiveSkills;

private:

	TMap<EUnitedCombatOptions, float> CombatStats;
	TMap<EMercenarySkillOption, float> SkillStats;

	bool IsInAttackState;
	bool bSkillControlledPlayer;
	bool bReservedUseSkill;
	int32 CacheAttackIndex;

	TArray<FAIAttackData> NPCAttackDatas;
	TArray<int32> NPCInvalidIndices;

	FGuildMercenaryInfo MercenaryInfo;
	TArray<TSharedPtr<FMercenarySkillData>> MercenarySkillDatas;

	TSharedPtr<FNPCUsingSkillState> UsingSkillState;

};
