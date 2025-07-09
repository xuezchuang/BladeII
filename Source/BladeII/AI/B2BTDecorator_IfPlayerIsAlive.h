#pragma once

#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BTDecorator.h"
#include "B2BTDecorator_IfPlayerIsAlive.generated.h"

UCLASS(HideCategories = (Condition))
class UB2BTDecorator_IfPlayerIsAlive : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};