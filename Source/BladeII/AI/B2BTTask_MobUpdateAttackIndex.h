#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "B2BTTask_MobUpdateAttackIndex.generated.h"

UCLASS()
class UB2BTTask_MobUpdateAttackIndex : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	EBTNodeResult::Type PerformUpdateAttackIndexTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	float AIAttackDelayTime;
	bool bAIAttackDelaying;
};