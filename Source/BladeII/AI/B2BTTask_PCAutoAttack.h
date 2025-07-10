// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
//#include "BladeII.h"
#include "B2BTTask_PCAutoAttack.generated.h"

UENUM()
namespace EPCAutoAttackType
{
	enum Type
	{
		EPCAAT_Combo = 0, // Simple attack. Successive call do the combo.
		
		EPCAAT_Skill = 1,
		// TODO: Need skill number..

		EPCAAT_Counter = 2,
		
		// Counter seems like a difficult one..

		EPCAAT_End
	};
}

/**
 * Attack action by auto player. Generated as a native class considering much possibilities in choosing attack action.
 */
UCLASS()
class BLADEII_API UB2BTTask_PCAutoAttack : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
public:

	/** The mobs within this distance will be counted to decide if it's worth to do the skill attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	float SkillWorthyMobDistance;

	/** When the sum of nearby mobs' combat ability (relative to the player) is larger than this, 
	 * it will be considered as worth do the skill attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	float SkillWorthyMonsterLevelScale;

	/** In 0.0 ~ 1.0 scale */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	float CounterProbability;

	/** The mobs within this distance will be counted to decide if it's worth try the counter attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	float CounterWorthyMobDistance;

	/** When we have mobs attacking more that this number near the player within CounterWorthyMobDistance,
	* it will be considered as worth try the counter attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	int32 CounterWorthyAttackingMonsterCount;

	/** When the sum of nearby mobs' combat ability (relative to the player) is larger that this,
	* it will be considered as worth try the counter attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	float CounterWorthyMonsterLevelScale;

	/** Shorter interval will result more frequent attack command by AI. 
	 * It cannot still make more attack animation and notifies, but too short interval can make a little abnormal behavior. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	float MinAttackInterval;

private:

	/** Keep guard action and don't do other attack or skill while this is true. */
	bool bTryingCounter;

	FTimerHandle GuardStartTimerHandle;
	FTimerHandle GuardFinishTimerHandle;

	bool bInAttackCooling; // Attack disabled state by MinAttackInterval.
	FTimerHandle AttackIntervalTimer;

private:
	//>>> UBTTaskNode interfaces
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	EBTNodeResult::Type PerformAttackTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	EBTNodeResult::Type PerformAttackObjectTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	EPCAutoAttackType::Type ChooseAttackType(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	/** Considering cooltime and some other conditions.. */
	bool CanDoSkillAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	/** Aside from "if it can do", does the player surrounded by monsters powerful enough? */
	bool IsWorthDoSkillAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	/** To measure and compare characters' combat ability, while not so precise scale..
	 * It reflects current state. (e.g. dying character does not returns a high score) */
	float GetCharacterRoughCombatGauge(class ABladeIICharacter* InCharacter);

	bool CanDoCounterAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	bool IsWorthDoCounterAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	void TryingCounterStartTimerCallback(class ABladeIIPlayer* InPlayer);
	void TryingCounterFinishTimerCallback(class ABladeIIPlayer* InPlayer);
	
	void GetWorldMobList(TArray<ABladeIICharacter*>& OutList); // Simple wrapper for some game mode 
	
	/** Not sure exactly which place we need to call this.. */
	void OnQuitTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	/** To set attack cooling after any attack command. */
	void PostAttackSetAttackCooling(class ABladeIIPlayer* InPlayer);
	void AttackIntervalTimerCB(class ABladeIIPlayer* InPlayer);

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
public:
	/** Auto play is useful for performance test, but then need to control player attack pattern. */
	static bool bDevAllowComboAttackOnly;
#endif
};
