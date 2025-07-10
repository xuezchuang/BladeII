
#include "B2BTTask_NPCSearchTarget.h"
//#include "BladeII.h"
#include "B2GuildNPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UB2BTTask_NPCSearchTarget::UB2BTTask_NPCSearchTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EBTNodeResult::Type UB2BTTask_NPCSearchTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformSearchTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_NPCSearchTarget::PerformSearchTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AB2GuildNPCAIController* MyController = Cast<AB2GuildNPCAIController>(OwnerComp.GetAIOwner());
	ABladeIICharacter* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	if (ControlledCharacter == nullptr)
		return EBTNodeResult::Failed;
	
	ABladeIICharacter* TargetCharacter = MyController->GetCurrentTarget();

	bool bValidTarget = TargetCharacter && TargetCharacter->IsAlive();

	if (bValidTarget && ControlledCharacter->GetAttackState() != EAttackState::ECS_None || !ControlledCharacter->GetAbleToControl())
		return EBTNodeResult::Succeeded;

	MyController->UpdateTarget();
	TargetCharacter = MyController->GetCurrentTarget();

	if (TargetCharacter == nullptr)
		return EBTNodeResult::Failed;
	
	//거리 업데이트
	float TargetDistance = (TargetCharacter->GetActorLocation() - ControlledCharacter->GetActorLocation()).Size();
	ControlledCharacter->UpdateAIAttackIndex(TargetDistance, true);

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName, TargetCharacter);

	return EBTNodeResult::Succeeded;
}