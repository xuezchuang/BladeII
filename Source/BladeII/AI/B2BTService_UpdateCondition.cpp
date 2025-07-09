#include "BladeII.h"
#include "B2BTService_UpdateCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayerAIController.h"

UB2BTService_UpdateCondition::UB2BTService_UpdateCondition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Condition";
	Interval = 1.00f;
	RandomDeviation = 0.0f;
}

void UB2BTService_UpdateCondition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}