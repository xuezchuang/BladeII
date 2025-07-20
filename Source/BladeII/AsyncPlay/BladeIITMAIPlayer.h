#pragma once

#include "BladeIIPlayer.h"
#include "../BladeII/B2SkillRTManager.h"
#include "BladeIITMAIPlayer.generated.h"

UCLASS()
class ABladeIITMAIPlayer : public ABladeIIPlayer
{
	GENERATED_UCLASS_BODY()

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) final;
	virtual void BeginPlay() final;

	virtual void PossessedBy(AController* NewController) override;

	virtual bool CanPlayerHaveMoveMotion() const { return true; }
	virtual bool IsFineToStartAnyNewInputHandling() final;
	virtual bool IsFineToStartAnyNewMoveInputHandling() final;
	virtual UB2SkillRTManager* GetSkillRTManager() final;

	virtual void InitializeCombatStats() override;
	virtual void StartDash(float InDashDuration, float InMaxDistance) final;
	virtual void StopDash() final;

	virtual int64 GetAccountId() override;
	virtual bool IsPlayerControlled() const { return false; }

	virtual EPCClass GetTagPlayerClass() final;

	virtual class ICharacterDataStore* GetCharacterDataStore();
	virtual int32 GetTeamNum() const final;

	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual bool GetIsLocalPlayerTeam() override;

	void SetTeamCharClasses(EPCClass, EPCClass, EPCClass);

	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void NotifyPlayerDeadEnd()override;

	virtual int32 GetSkillLOD() const final { return 1; } //[@AKI, 170815][M20]6성 스킬 개선 NetMode에서는 무조건 LOD 하게 변경
	//virtual int32 GetSkillLOD() const final { return (GetTeamNum() != 0) ? 1 : 0; }


	virtual float UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled = false) override;

	/** Number to uniquely identify player character (including pending tag character) in in-game level */
	virtual int32 GetGameEntryID() const override;
	virtual bool HasValidGameEntryID() const override;

	virtual bool IsHeadPointLightAllowed() const override { return false; }

	virtual int32 GetHighPrioritySkillIndex() override;
	virtual EAttackState StartSkillInner(int32 SkillInputIndex, bool IsPlayerInput) override;
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser) override;
protected:
	virtual bool IsVisibleFloorRing() final;

	EPCClass TeamOtherCharClasses[2];
	int32	MyTeamNumber;
};