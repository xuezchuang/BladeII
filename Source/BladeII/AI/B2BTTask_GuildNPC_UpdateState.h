#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "B2BTTask_GuildNPC_UpdateState.generated.h"

UCLASS()
class UB2BTTask_GuildNPC_UpdateState : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector AcceptableRadiusKey;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(Category = BladeII, EditAnywhere)
	float AddAttackTargetDistance;
};