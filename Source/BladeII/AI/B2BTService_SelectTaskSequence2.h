#pragma once

#include "BehaviorTree/BTService.h"
#include "B2BTService_SelectTaskSequence2.generated.h"

UCLASS()
class UB2BTService_SelectTaskSequence2 : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(Category = BladeII, EditAnywhere, meta = (ClampMin = "0.001"))
		float SearchTargetDistance;
	UPROPERTY(Category = BladeII, EditAnywhere, meta = (ClampMin = "0.001"))
		float AttackTargetDistance;
};