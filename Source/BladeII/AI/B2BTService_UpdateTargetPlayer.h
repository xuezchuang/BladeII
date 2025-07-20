#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "B2BTService_UpdateTargetPlayer.generated.h"

UCLASS()
class UB2BTService_UpdateTargetPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};