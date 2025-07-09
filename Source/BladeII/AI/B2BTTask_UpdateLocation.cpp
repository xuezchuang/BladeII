#include "BladeII.h"
#include "B2BTTask_UpdateLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIIUtil.h"

UB2BTTask_UpdateLocation::UB2BTTask_UpdateLocation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Location";
}

EBTNodeResult::Type UB2BTTask_UpdateLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto*	Controller	= Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	auto*	Player		= Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard	= OwnerComp.GetBlackboardComponent();
	ABladeIICharacter* pTarget = Controller->PickOpponent();

	BII_CHECK(Controller);
	BII_CHECK(Player);
	BII_CHECK(Blackboard);
	BII_CHECK(pTarget);

	if (Player->IsAttacking())
		return EBTNodeResult::Failed;

	const FString TargetToBeat(TEXT("TargetToBeat"));

	auto	Ret				= Controller->UpdateTarget();
	auto	MyLocation		= Player->GetActorLocation();

	// Not Enough Close To Goal Location So Do nothing...
//	if ((MyLocation - LocationToGo).SizeSquared() > 100.0f)
//		return EBTNodeResult::Succeeded;

//	bool	EmitNoise		= FMath::RandBool();
	auto	EmitNoise		= FMath::FRand();

	Blackboard->SetValueAsObject(FName(*TargetToBeat), pTarget);

	if (EmitNoise <= 0.2f)
	{
		auto NoiseLocation	= GetRandomReachablePointInRadiusWithCurrNavSys(GetWorld(), MyLocation, 250.0f);
		//Blackboard->SetValueAsObject(FName(*TargetToBeat), pTarget);//여기가 원래 LocationToGo 자리
	}

	return EBTNodeResult::Succeeded;
}
