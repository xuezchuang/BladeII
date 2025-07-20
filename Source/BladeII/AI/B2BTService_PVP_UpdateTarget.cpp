
#include "B2BTService_PVP_UpdateTarget.h"
#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayerAIController.h"
#include "B2AutoWayPoint.h"

UB2BTService_PVP_UpdateTarget::UB2BTService_PVP_UpdateTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "PVPUpdateTarget";
	Interval = 0.25f;
	RandomDeviation = 0.1f;
}

void UB2BTService_PVP_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	Owner = Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	BII_CHECK(Owner);

	auto	Ret = Owner->UpdateTarget();
	auto*	BlackBoard = OwnerComp.GetBlackboardComponent();
	BII_CHECK(BlackBoard);

	const FString OpponentKey(TEXT("Opponent"));

	if (Ret)
	{
		BlackBoard->SetValueAsObject(FName(*OpponentKey), Owner->CurrentTargetMob);
	}
	else
	{
		BlackBoard->ClearValue(FName(*OpponentKey));
	}
}