
#include "B2BTTask_ExecuteDefense.h"
#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"

UB2BTTask_ExecuteDefense::UB2BTTask_ExecuteDefense(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Execute Defense";
}

EBTNodeResult::Type UB2BTTask_ExecuteDefense::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto*	Controller = Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	auto*	Player = Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard = OwnerComp.GetBlackboardComponent();

	BII_CHECK(Controller);
	BII_CHECK(Player);
	BII_CHECK(Blackboard);

	if (Player->IsAttacking())
		return EBTNodeResult::Failed;

	Player->StartGuard();

	return EBTNodeResult::Succeeded;
}