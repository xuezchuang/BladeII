#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "B2BTTask_UpdateDirection.generated.h"

UCLASS()
class UB2BTTask_UpdateDirection : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};