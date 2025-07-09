#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "B2BTTask_SelectAttackMethod.generated.h"

UCLASS()
class UB2BTTask_SelectAttackMethod : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII AI")
	bool	bThinkTagAsSkill;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};