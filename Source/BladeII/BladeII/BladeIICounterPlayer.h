#pragma once
////#include "BladeII.h"
//#include "BladeIIPlayer.h"
//#include "BladeIICounterPlayer.generated.h"
//
//UCLASS()
//class ABladeIICounterPlayer : public ABladeIIPlayer
//{
//	GENERATED_UCLASS_BODY()
//
//public:
//	// override AActor interfaces...
//	virtual void BeginPlay() override;
////	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
//	
//	// override ABladeIIPlayer interfaces...
//	virtual void StartSkill01() override;
//	virtual void OnCounterStart() override;
//	virtual void SetAttackState(EAttackState) override;
//
//	//virtual void SetReservedSkill(EAttackState InSkillState) override {}
//
//	virtual void PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak) override;
//
//	virtual void UpdateUIDocOnTargetDamaged() override;
//
//	virtual float GetAdditionalCriticalRateByComboNum() const override;
//	virtual float GetDamageIncreaseRate() const override;
//
//	// My interfaces...
//
//	void OverrideSkillUIAsset(EPCClass PCClass);
//
//	void EnableNormalAttack();
//	void DisableNormalAttack();
//	void ModifyInputCommand(bool);
//
//	int32 GetCounterAttackPoint() const 
//	{ 
//		return CounterAttackPoint; 
//	}
//
//	void AddCounterAttackPoint(int32);
//	void SetCounterAttackPoint(int32);
//
//private:
//	virtual void InitUpdateUIDoc() override;
//	virtual void InitializeCombatStats() override;
//
//	virtual void ResetComboNumTimerCallback() override;
//
//private:	
//
//	/** Stack point in CounterDG */
//	int32 CounterAttackPoint;
//};