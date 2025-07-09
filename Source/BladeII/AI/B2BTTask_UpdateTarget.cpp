#include "BladeII.h"
#include "B2BTTask_UpdateTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayerAIController.h"

UB2BTTask_UpdateTarget::UB2BTTask_UpdateTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Target";
}

EBTNodeResult::Type UB2BTTask_UpdateTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type	NodeResult	= EBTNodeResult::Succeeded;

	auto*	Controller		= Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	auto	Ret				= Controller->UpdateTarget();
	auto*	Blackboard		= OwnerComp.GetBlackboardComponent();

	BII_CHECK(Controller);
	BII_CHECK(Blackboard);

	const FString OpponentKey(TEXT("TargetToBeat"));

	if (Ret)
	{
		Blackboard->SetValueAsObject(FName(*OpponentKey), Controller->CurrentTargetMob);
	}
	else
	{
		Blackboard->ClearValue(FName(*OpponentKey));
		NodeResult = EBTNodeResult::Failed;
	}

	return NodeResult;
}