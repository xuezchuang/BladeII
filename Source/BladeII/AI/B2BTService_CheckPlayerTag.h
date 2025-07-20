#pragma once

#include "BehaviorTree/BTService.h"
#include "Knowledge.h"
#include "B2BTService_CheckPlayerTag.generated.h"

UCLASS()
class UB2BTService_CheckPlayerTag : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};