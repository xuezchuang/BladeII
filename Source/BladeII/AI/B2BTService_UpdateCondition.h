#pragma once

#include "BehaviorTree/BTService.h"
#include "Knowledge.h"
#include "B2BTService_UpdateCondition.generated.h"

UCLASS()
class UB2BTService_UpdateCondition : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};