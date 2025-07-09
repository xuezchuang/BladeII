#include "BladeII.h"
#include "BladeIIUtil.h"
#include "AIController.h"
#include "B2BTTask_PlayerMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UB2BTTask_PlayerMoveTo::UB2BTTask_PlayerMoveTo( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
	, RandomPointInNavigableRadius( 700.0f )
	, TeleportDelayTime( 15.0f )
	, bTeleport( false )
{
	NodeName = "BTTask_PlayerMoveTo";
	bNotifyTick = false;
	bNotifyTaskFinished = true;

	// accept only actors and vectors
	BlackboardKey.AddObjectFilter( this, GET_MEMBER_NAME_CHECKED( UB2BTTask_PlayerMoveTo, BlackboardKey ), AActor::StaticClass() );
	BlackboardKey.AddVectorFilter( this, GET_MEMBER_NAME_CHECKED( UB2BTTask_PlayerMoveTo, BlackboardKey ) );
}

EBTNodeResult::Type UB2BTTask_PlayerMoveTo::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	EBTNodeResult::Type NodeResult = Super::ExecuteTask( OwnerComp, NodeMemory );

	AAIController* MyController = Cast< AAIController >( OwnerComp.GetOwner() );
	if ( !MyController ) return EBTNodeResult::Failed;

	ABladeIIPlayer* MyPawn = Cast< ABladeIIPlayer >( MyController->GetPawn() );
	if ( !MyPawn ) return EBTNodeResult::Failed;

	if ( NodeResult == EBTNodeResult::Failed )
	{
		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		if ( BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() )
		{
			if ( !bTeleport && MyPawn->GetVelocity().IsNearlyZero() )
			{
				bTeleport = true;
				MyPawn->GetWorldTimerManager().SetTimer( TeleportTimerHandle, FTimerDelegate::CreateUObject( this, &UB2BTTask_PlayerMoveTo::PerformTeleportToRandomReachablePoint, &OwnerComp ), TeleportDelayTime, false );
			}
		}
	}
	else
	{
		bTeleport = false;
		MyPawn->GetWorldTimerManager().ClearTimer( TeleportTimerHandle );
	}

	return NodeResult;
}

void UB2BTTask_PlayerMoveTo::PerformTeleportToRandomReachablePoint( UBehaviorTreeComponent* OwnerComp )
{
	EPathFollowingRequestResult::Type RequestResult = EPathFollowingRequestResult::Failed;

	AAIController* MyController = Cast< AAIController >( OwnerComp->GetOwner() );
	if ( !MyController ) return;

	APawn* MyPawn = MyController->GetPawn();
	if ( !MyPawn ) return;

	if ( !bTeleport )
	{
		MyPawn->GetWorldTimerManager().ClearTimer( TeleportTimerHandle );
		return;
	}

	const UBlackboardComponent* MyBlackboard = OwnerComp->GetBlackboardComponent();
	UObject* KeyValue = MyBlackboard->GetValue< UBlackboardKeyType_Object >( BlackboardKey.GetSelectedKeyID() );
	AActor* TargetActor = Cast< AActor >( KeyValue );
	if ( !TargetActor ) return;

	FNavLocation RandomPoint;
	//UNavigationSystem* const NavigationSystem = GetWorld()->GetNavigationSystem();
	//if ( NavigationSystem )
	//{
	//	NavigationSystem->GetRandomPointInNavigableRadius( TargetActor->GetActorLocation()
	//		, RandomPointInNavigableRadius
	//		, RandomPoint );

	//	MyPawn->TeleportTo( RandomPoint.Location, MyPawn->GetActorRotation(), true, false );
	//	SnapBladeIICharacterToGround( Cast< ABladeIIPlayer >( MyPawn ) );
	//}

	//bTeleport = false;
	MyPawn->GetWorldTimerManager().ClearTimer( TeleportTimerHandle );
}
