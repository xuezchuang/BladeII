#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "B2BTTask_DynamicMoveTo.generated.h"

UCLASS()
class UB2BTTask_DynamicMoveTo : public UBTTask_MoveTo
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector AcceptableRadiusKey;

	virtual UAITask_MoveTo* PrepareMoveTask(UBehaviorTreeComponent& OwnerComp, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest) override;
};