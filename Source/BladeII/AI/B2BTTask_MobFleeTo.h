#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "B2BTTask_MobFleeTo.generated.h"

UCLASS()
class UB2BTTask_MobFleeTo : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 TestNumRandLocation;

	UPROPERTY(EditAnywhere)
	float SearchRadius;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	EBTNodeResult::Type PerformFleeToTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};