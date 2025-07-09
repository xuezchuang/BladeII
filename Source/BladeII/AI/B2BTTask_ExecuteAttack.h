#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "B2BTTask_ExecuteAttack.generated.h"

UCLASS()
class UB2BTTask_ExecuteAttack : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII AI")
	bool	bThinkTagAsSkill;

private:
	EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};