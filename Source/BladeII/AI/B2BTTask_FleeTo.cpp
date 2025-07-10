
#include "B2BTTask_FleeTo.h"
//#include "BladeII.h"
#include "AIController.h"
#include "B2AIUtil.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UB2BTTask_FleeTo::UB2BTTask_FleeTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Flee to";
}

EBTNodeResult::Type UB2BTTask_FleeTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformFleeToTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_FleeTo::PerformFleeToTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AAIController* MyController = OwnerComp.GetAIOwner();
	//auto* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	//if (ControlledCharacter == nullptr)
	//	return EBTNodeResult::Failed;

	//if (!ControlledCharacter->GetAbleToControl())
	//	return EBTNodeResult::Failed;

	//UB2CharacterContainer* Container = Cast<UB2CharacterContainer>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName));
	//if (Container == nullptr)
	//	return EBTNodeResult::Failed;

	//FVector FleeLocation = B2AIUtil::GetFleeLocation(ControlledCharacter, Container->Chracters, TestNumRandLocation, SearchRadius);
	//FNavigationSystem::SimpleMoveToLocation(MyController, FleeLocation);

	return EBTNodeResult::Succeeded;
	
	
}