#include "BladeII.h"
#include "B2BTTask_SelectMoveMethod.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIIGameMode.h"

UB2BTTask_SelectMoveMethod::UB2BTTask_SelectMoveMethod(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Move Method";
}

EBTNodeResult::Type UB2BTTask_SelectMoveMethod::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	auto*	GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Controller));
	BII_CHECK(GameMode);
	auto    IsAIAllowedToEvade = GameMode->GetGameRule()->IsAIAllowedToEvade();

	auto	EmitNoise = FMath::FRand();

	if (EmitNoise <= 0.4f && IsAIAllowedToEvade)
		Player->SetAttackState(EAttackState::ECS_Evade);

	return EBTNodeResult::Succeeded;
}