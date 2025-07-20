
#include "B2BTTask_MobUpdateAttackIndex.h"
//#include "BladeII.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BladeIICharacter.h"

UB2BTTask_MobUpdateAttackIndex::UB2BTTask_MobUpdateAttackIndex(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Attack Index";
	AIAttackDelayTime = 0.f;
	bAIAttackDelaying = false;
}

EBTNodeResult::Type UB2BTTask_MobUpdateAttackIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformUpdateAttackIndexTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_MobUpdateAttackIndex::PerformUpdateAttackIndexTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	auto* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	if (!IsValid(ControlledCharacter))
		return EBTNodeResult::Failed;

	if (ControlledCharacter->GetAttackState() != EAttackState::ECS_None || !ControlledCharacter->GetAbleToControl())
		return EBTNodeResult::Failed;

	if (!BlackboardKey.IsSet())
		return EBTNodeResult::Failed;
	
	auto* PlayerCharacter = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (PlayerCharacter == nullptr)
		return EBTNodeResult::Failed;

	ControlledCharacter->UpdateAIAttackIndex((PlayerCharacter->GetActorLocation() - ControlledCharacter->GetActorLocation()).Size());

	if (ControlledCharacter->AIAttackIndex < 0)
		return EBTNodeResult::Failed;

	if (!bAIAttackDelaying)
	{
		bAIAttackDelaying = true;
		AIAttackDelayTime = ControlledCharacter->GetAIAttackDelay();		
	}	

	if (bAIAttackDelaying && AIAttackDelayTime <= 0.f)
	{
		bAIAttackDelaying = false;
		AIAttackDelayTime = 0.f;
				
		ControlledCharacter->SetSignalAttack(true);

		return EBTNodeResult::Succeeded;
	}		

	return EBTNodeResult::InProgress;
}

void UB2BTTask_MobUpdateAttackIndex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bAIAttackDelaying)
		AIAttackDelayTime -= DeltaSeconds;

	const EBTNodeResult::Type NodeResult = PerformUpdateAttackIndexTask(OwnerComp, NodeMemory);
	if (NodeResult != EBTNodeResult::InProgress)
	{
		FinishLatentTask(OwnerComp, NodeResult);
	}
}