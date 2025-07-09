#include "BladeII.h"
#include "B2BTTask_MobSearchPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UB2BTTask_MobSearchPlayer::UB2BTTask_MobSearchPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Search Player";
}

EBTNodeResult::Type UB2BTTask_MobSearchPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformSearchTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_MobSearchPlayer::PerformSearchTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	auto* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	if (ControlledCharacter == nullptr)
		return EBTNodeResult::Failed;
	
	//[@AKI, 170809] [FIXME]몹의 AttackState가 ECS_None이 아니어서 공격 안하는 이슈 발생됨.
	//AIController나 BladeIICharacter쪽에 AttackState 꼬임 현상이 있는 듯
	if (ControlledCharacter->GetAttackState() != EAttackState::ECS_None || !ControlledCharacter->GetAbleToControl())
		return ControlledCharacter->bNeedUpdateTarget ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

	auto* PlayerCharacter = Cast<ABladeIIPlayer>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName));
	if (PlayerCharacter == nullptr)
		return ControlledCharacter->bNeedUpdateTarget ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

	float TargetDistanceSquared = (PlayerCharacter->GetActorLocation() - ControlledCharacter->GetActorLocation()).SizeSquared();
	if (TargetDistanceSquared > FMath::Square(ControlledCharacter->DetectDistance))
		return ControlledCharacter->bNeedUpdateTarget ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	
	ControlledCharacter->DetectDistance = FMath::Max(ControlledCharacter->DetectDistance, ControlledCharacter->GetAIMaxAttackRange());
	ControlledCharacter->bNeedSpawnCheck = false;
	
	if (ControlledCharacter->CustomTimeDilation == 0) 
		//Spawn anim이 실행된다.
		ControlledCharacter->CustomTimeDilation = 1.f;

	ControlledCharacter->UpdateAIAttackIndex(FMath::Sqrt(TargetDistanceSquared), true);

	return EBTNodeResult::Succeeded;
}