#include "BladeII.h"
#include "B2BTService_SelectTaskSequence2.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "B2AI.h"
#include "BladeIIGameMode.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "B2HomePoint.h"
#include "B2Lottery.h"

UB2BTService_SelectTaskSequence2::UB2BTService_SelectTaskSequence2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Task Sequence II";
	Interval = 0.25f;
	RandomDeviation = 0.25f;
}

void UB2BTService_SelectTaskSequence2::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	Controller	= Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	BII_CHECK(Controller);

	auto*	Player		= Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard	= OwnerComp.GetBlackboardComponent();
	auto*	GameMode	= Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Controller));
	BII_CHECK(GameMode);

	auto*	HomePoint	= GameMode->GetHomePoint();

	//Controller->CurrentTargetMob 보다 위에 있어야 함. 여기서 CurrentTargetMob을 설정 함
	Controller->UpdateTarget();
	ABladeIICharacter* pTarget = Controller->CurrentTargetMob;

	BII_CHECK(Player);
	BII_CHECK(Blackboard);

	const FString EnumKey(TEXT("SelectedTaskMode"));
	const FString TargetToBeat(TEXT("TargetToBeat"));
	
	// 타겟 없으면 백홈
	if (!pTarget)
	{
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_RETURNHOME));
		return;
	}

	// 타겟몹 설정
	Blackboard->SetValueAsObject(FName(*TargetToBeat), pTarget);

	// 타겟과의 거리
	float fTargetDistance = (pTarget->GetActorLocation() - Player->GetActorLocation()).Size();
		
	if (fTargetDistance < AttackTargetDistance)
	{
		// 공격 범위 안이면 공격
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_ATTACK));
	}
	else if (fTargetDistance < SearchTargetDistance)
	{
		// 인식 범위 안이면 이동
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_MOVE));
	}
	else
	{
		// 그외엔 백홈
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_RETURNHOME));
		Blackboard->SetValueAsObject(FName(*TargetToBeat), HomePoint);
	}

}
