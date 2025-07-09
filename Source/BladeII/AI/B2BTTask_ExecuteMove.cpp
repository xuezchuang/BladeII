#include "BladeII.h"
#include "B2BTTask_ExecuteMove.h"

UB2BTTask_ExecuteMove::UB2BTTask_ExecuteMove(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Execute Move";
}

EBTNodeResult::Type UB2BTTask_ExecuteMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}