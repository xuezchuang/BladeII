#pragma once

#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "B2BTDecorator_CheckLOStoTarget.generated.h"

UCLASS(HideCategories = (Condition))
class UB2BTDecorator_CheckLOStoTarget : public UBTDecorator_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};