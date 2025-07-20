
#include "B2BTTask_SelectMoveMethod2.h"
//#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIICharacter.h"
#include "B2ControlModePlayer.h"
#include "BladeIIPlayerAIController.h"
#include "../BladeII/BladeIIUtil.h"

UB2BTTask_SelectMoveMethod2::UB2BTTask_SelectMoveMethod2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Move Method II";
}

EBTNodeResult::Type UB2BTTask_SelectMoveMethod2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto*	Controller	= Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	auto*	Player		= Cast<AB2ControlModePlayer>(Controller->GetPawn());
	auto*	Blackboard	= OwnerComp.GetBlackboardComponent();

	BII_CHECK(Controller);
	BII_CHECK(Player);
	BII_CHECK(Blackboard);

	if (Player->IsAttacking())
		return EBTNodeResult::Failed;

	auto	EmitNoise = FMath::FRand();

	if (EmitNoise <= 0.4f && Player->IsAllowToEvade())
		Player->SetAttackState(EAttackState::ECS_Evade);

	return EBTNodeResult::Succeeded;
}