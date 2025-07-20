
#include "B2BTService_UpdateTargetPlayer.h"
//#include "BladeII.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BladeIIPlayer.h"
#include "BladeIIGameMode.h"

void UB2BTService_UpdateTargetPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto* OldTarget = Cast<ABladeIIPlayer>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName));
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName, nullptr);

	AAIController* MyController = OwnerComp.GetAIOwner();
	auto* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	if (ControlledCharacter == nullptr)
		return;
	if (!ControlledCharacter->GetAbleToMove())
		return;

	auto* GameMode			= Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(ControlledCharacter));
	auto* PlayerCharacter	= GameMode->GetNearestPlayerCharacter(ControlledCharacter, true);
// 	if (PlayerCharacter == nullptr)
// 		return;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName, PlayerCharacter);

	if (OldTarget != PlayerCharacter)
		OwnerComp.RestartTree();
}