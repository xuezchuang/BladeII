
#include "B2BTTask_GuildNPC_UpdateState.h"
#include "BladeII.h"
#include "B2GuildNPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "B2AI.h"
#include "BladeIICharacter.h"

UB2BTTask_GuildNPC_UpdateState::UB2BTTask_GuildNPC_UpdateState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2BTTask_GuildNPC_UpdateState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		AcceptableRadiusKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UB2BTTask_GuildNPC_UpdateState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto*	Controller = Cast<AB2GuildNPCAIController>(OwnerComp.GetOwner());
	BII_CHECK(Controller);

	ABladeIICharacter*		ControlledCharacter = Cast<ABladeIICharacter>(Controller->GetPawn());
	UBlackboardComponent*	Blackboard = OwnerComp.GetBlackboardComponent();

	ABladeIICharacter* CurrentTarget = Controller->GetCurrentTarget();

	BII_CHECK(ControlledCharacter);
	BII_CHECK(Blackboard);

	// 鸥百 绝栏搁... 
	if (!CurrentTarget || CurrentTarget == ControlledCharacter)
	{
		Blackboard->SetValue<UBlackboardKeyType_Enum>(BlackboardKey.SelectedKeyName, static_cast<uint8>(BTTASK_MODE::EBM_DEFENSE));
		return EBTNodeResult::Failed;
	}

	if (ControlledCharacter->bInPhaseAnimation)
	{
		Blackboard->SetValue<UBlackboardKeyType_Enum>(BlackboardKey.SelectedKeyName, static_cast<uint8>(BTTASK_MODE::EBM_DEFENSE));
		return EBTNodeResult::Failed;
	}

	float TargetDistance = (CurrentTarget->GetActorLocation() - ControlledCharacter->GetActorLocation()).Size();
	int32 OutAttackIndex = INDEX_NONE;
	float OutRadius = 0.0f;

	bool bValidAttackIndex = ControlledCharacter->CalcAIAttackIndex(TargetDistance, OutAttackIndex, OutRadius);
	float FinalAcceptableRadius = AddAttackTargetDistance + OutRadius;

	Blackboard->SetValue<UBlackboardKeyType_Float>(AcceptableRadiusKey.SelectedKeyName, FinalAcceptableRadius);

	BTTASK_MODE DeterminTaskMode = TargetDistance < FinalAcceptableRadius ? BTTASK_MODE::EBM_ATTACK : BTTASK_MODE::EBM_MOVE;

	if (bValidAttackIndex)
		DeterminTaskMode = BTTASK_MODE::EBM_ATTACK;

	if (DeterminTaskMode == BTTASK_MODE::EBM_MOVE)
		ControlledCharacter->SetSignalAttack(false);

	Blackboard->SetValue<UBlackboardKeyType_Enum>(BlackboardKey.SelectedKeyName, static_cast<uint8>(DeterminTaskMode));

	return EBTNodeResult::Succeeded;
}