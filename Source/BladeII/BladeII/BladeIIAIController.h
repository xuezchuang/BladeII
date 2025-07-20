// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "AIController.h"
#include "BladeIICharacter.h"
#include "BladeIIAIController.generated.h"


/**
 * Native base of BladeiICharacter's AIController. 
 * Not only for the AI, but It also serves as consistent state holder of BladeIICharacter which can go through re-spawn (e.g. by phase transition).
 * In that aspect, it could be considered as some sort of character state controller.
 */
UCLASS()
class BLADEII_API ABladeIIAIController : public AAIController
{
	GENERATED_BODY()

	/** Current phase state number starting from 0 and being increased sequentially.
	 * It is the index of phase data array which leads to the "next" phase.
	 * As the "current" phase state, CurrentPhaseNum - 1 is the phase data array index of current phase, except for 0 phase which does not require phase data. */
	int32 CurrentPhaseNum;

public:
	ABladeIIAIController(const FObjectInitializer& ObjectInitializer);

	//virtual void Possess(APawn* InPawn) override;

	/** Changes its possessed character's phase by 1. Calls PhaseChange with relevant phase number and data. */
	void ToNextPhase();

	/** Changes AI phase. It is called by a trigger defined by possessed BladeIICharacter's PhaseDataArray.
	 * This phase num index and its relevant PhaseData element is passed to here. 
	 * It can goes through BladeIICharacter's re-spawn. */
	void PhaseChange(int32 ToPhaseNum, const FAIPhaseData& ToPhaseData);
	

	FORCEINLINE int32 GetCurrentPhaseNum() { return CurrentPhaseNum; }

	void StopAutoPlay();

	void ApplyMountBehaviorTree();
};
