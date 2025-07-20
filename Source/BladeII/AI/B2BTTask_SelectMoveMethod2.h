#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "B2BTTask_SelectMoveMethod2.generated.h"

UCLASS()
class UB2BTTask_SelectMoveMethod2 : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};