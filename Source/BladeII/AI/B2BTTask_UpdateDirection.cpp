
#include "B2BTTask_UpdateDirection.h"
//#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIIPlayer.h"

UB2BTTask_UpdateDirection::UB2BTTask_UpdateDirection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Direction";
}

EBTNodeResult::Type UB2BTTask_UpdateDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto*	Controller = Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	auto*	Player = Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard = OwnerComp.GetBlackboardComponent();

	BII_CHECK(Controller);
	BII_CHECK(Player);
	BII_CHECK(Blackboard);

	const FString OpponentKey(TEXT("TargetToBeat"));

	auto*	Opponent = Cast<ABladeIIPlayer>(Blackboard->GetValueAsObject(FName(*OpponentKey)));

	if (!Opponent || Player->IsAttacking())
		return EBTNodeResult::Failed;

	auto	DesiredLookAt	= Opponent->GetActorLocation() - Player->GetActorLocation(); DesiredLookAt.Normalize();
	auto	CurrentLookAt	= Player->GetActorRotation().Vector(); CurrentLookAt.Normalize();
	auto	NearlyEqual		= FVector::DotProduct(DesiredLookAt, CurrentLookAt);

	if ( NearlyEqual < 0.85f )
	{
		auto	DesiredDirection = FVector(DesiredLookAt.X, DesiredLookAt.Y, 0.0f);
		Player->SetActorRotation(DesiredDirection.Rotation());
	}

	return EBTNodeResult::Succeeded;
}