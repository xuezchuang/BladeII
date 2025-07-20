#pragma once

#include "BehaviorTree/BTService.h"
#include "B2BTService_StopStupidAttack.generated.h"

UCLASS()
class UB2BTService_StopStupidAttack : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};