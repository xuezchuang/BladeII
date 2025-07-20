#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "B2BTTask_MobSearchPlayer.generated.h"

UCLASS()
class UB2BTTask_MobSearchPlayer : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	EBTNodeResult::Type PerformSearchTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};