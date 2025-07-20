#pragma once

#include "BehaviorTree/BTService.h"
#include "B2BTService_UpdateTargetOrWayPoint.generated.h"

UCLASS()
class UB2BTService_UpdateTargetOrWayPoint : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(Category = BladeII, EditAnywhere, meta = (ClampMin = "0.001"))
		float SearchMinObjectDistance;
	UPROPERTY(Category = BladeII, EditAnywhere, meta = (ClampMin = "0.001"))
		float SearchMaxObjectDistance;
	UPROPERTY(Category = BladeII, EditAnywhere, meta = (ClampMin = "0.001"))
		float SearchMinMonsterDistance;
	UPROPERTY(Category = BladeII, EditAnywhere, meta = (ClampMin = "0.001"))
		float SearchMaxMonsterDistance;


	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};