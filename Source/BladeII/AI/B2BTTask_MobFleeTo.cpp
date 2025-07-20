
#include "B2BTTask_MobFleeTo.h"
//#include "BladeII.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "../BladeII/BladeIICharacter.h"

UB2BTTask_MobFleeTo::UB2BTTask_MobFleeTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Flee to";
}

EBTNodeResult::Type UB2BTTask_MobFleeTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformFleeToTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_MobFleeTo::PerformFleeToTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	auto* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	if (ControlledCharacter == nullptr)
		return EBTNodeResult::Failed;

	if (ControlledCharacter->GetAttackState() != EAttackState::ECS_None || !ControlledCharacter->GetAbleToControl())
		return EBTNodeResult::Failed;

	FVector FleeLocation = ControlledCharacter->GetFleeLocation(TestNumRandLocation, SearchRadius);

	//UNavigationSystem::SimpleMoveToLocation(MyController, FleeLocation);

	return EBTNodeResult::Succeeded;
}