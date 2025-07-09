#include "BladeII.h"
#include "B2BTDecorator_CheckLOStoTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIIPlayer.h"

UB2BTDecorator_CheckLOStoTarget::UB2BTDecorator_CheckLOStoTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Check Line of Sight to Target";
	bNotifyTick = true;
}

void UB2BTDecorator_CheckLOStoTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
		OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
}

bool UB2BTDecorator_CheckLOStoTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Controller = Cast<AAIController>(OwnerComp.GetOwner());
	const auto* Player = Cast<ABladeIICharacter>(Controller->GetPawn());
	BII_CHECK(Controller && Player);

	const auto*	BlackBoard = OwnerComp.GetBlackboardComponent();
	const auto* Target = Cast<ABladeIICharacter>(BlackBoard->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));

	if (Target)
	{
		FCollisionShape CollisionShape = Player->GetCapsuleComponent()->GetCollisionShape();
		CollisionShape.Capsule.HalfHeight *= 0.25f;
		CollisionShape.Capsule.Radius *= 0.25f;

		FCollisionQueryParams Params(ABladeIIPlayer::TargetTraceName, false, Player);
		Params.AddIgnoredActor(Target);
		FHitResult Result;

		GetWorld()->SweepSingleByChannel(Result, Player->GetActorLocation(), Target->GetActorLocation(), FQuat::Identity, ECC_Visibility, CollisionShape, Params);

		if (Result.bBlockingHit)
		{
			return true;
		}
	}

	return false;
}