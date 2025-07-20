#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "B2BTTask_PlayerMoveTo.generated.h"


UCLASS( config = Game )
class BLADEII_API UB2BTTask_PlayerMoveTo : public UBTTask_MoveTo
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;
	void PerformTeleportToRandomReachablePoint( UBehaviorTreeComponent* OwnerComp );

	UPROPERTY( config, Category = Node, EditAnywhere )
	float RandomPointInNavigableRadius;

	UPROPERTY( config, Category = Node, EditAnywhere )
	float TeleportDelayTime;

	bool bTeleport;
	FTimerHandle TeleportTimerHandle;
};

