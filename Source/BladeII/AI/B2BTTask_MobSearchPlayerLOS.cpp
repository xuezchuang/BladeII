#include "BladeII.h"
#include "B2BTTask_MobSearchPlayerLOS.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UB2BTTask_MobSearchPlayerLOS::UB2BTTask_MobSearchPlayerLOS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Search Player Line Of Sight";
}

EBTNodeResult::Type UB2BTTask_MobSearchPlayerLOS::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return PerformSearchTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_MobSearchPlayerLOS::PerformSearchTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	auto* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	if (ControlledCharacter == nullptr)
		return EBTNodeResult::Failed;

	if (ControlledCharacter->GetAttackState() != EAttackState::ECS_None || !ControlledCharacter->GetAbleToControl())
		return EBTNodeResult::Failed;

	FVector ControlledCharLocation = ControlledCharacter->GetActorLocation();

	auto* PlayerCharacter = Cast<ABladeIIPlayer>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName));
	if (PlayerCharacter == nullptr)
		return EBTNodeResult::Failed;

	FVector PlayerCharLocation = PlayerCharacter->GetActorLocation();

	//�þ߰Ÿ��������� �ָ� �ּ��� ����Ȯ���� Ŀ������. �þ߰Ÿ��� 1/2 ���� �ȿ����� ������ ����
	float HalfCheckDistanceSquared = FMath::Square(CheckDistance * 0.5f);
	float TestFailToSearch = FMath::Max(0.f, ((PlayerCharLocation - ControlledCharLocation).SizeSquared() - HalfCheckDistanceSquared) / HalfCheckDistanceSquared);
	float RandomizeFailureRate = FMath::RandRange(0.f, 1.f);
	if (RandomizeFailureRate < TestFailToSearch)
		return EBTNodeResult::Failed;

	//LOS�� �ɸ��� �� �Ǵ�
	FHitResult Hit;
	FCollisionQueryParams Params(NAME_None, false, ControlledCharacter);
	UWorld* World = GEngine->GetWorldFromContextObject(ControlledCharacter, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->LineTraceSingleByChannel(Hit, ControlledCharLocation, PlayerCharLocation, ECollisionChannel::ECC_Visibility, Params) : false;

	if (!bHit)
	{
		ControlledCharacter->bNeedSpawnCheck = false;

		if (ControlledCharacter->CustomTimeDilation == 0)
			//Spawn anim�� ����ȴ�.
			ControlledCharacter->CustomTimeDilation = 1.f;
	}

	return bHit ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
}