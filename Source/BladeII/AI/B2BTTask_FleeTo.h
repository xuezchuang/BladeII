#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "B2BTTask_FleeTo.generated.h"

UCLASS()
class UB2BTTask_FleeTo : public UBTTask_BlackboardBase
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