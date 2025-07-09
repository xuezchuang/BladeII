#include "BladeII.h"
#include "B2BTService_SelectTaskForPVP.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "B2AI.h"
#include "BladeIIGameMode.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "B2HomePoint.h"
#include "B2Lottery.h"

UB2BTService_SelectTaskForPVP::UB2BTService_SelectTaskForPVP(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Task Sequence";
	Interval = 0.25f;
	RandomDeviation = 0.25f;
}

void UB2BTService_SelectTaskForPVP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	Controller = Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	BII_CHECK(Controller);
	//Controller->CurrentTargetMob ���� ���� �־�� ��. ���⼭ CurrentTargetMob�� ���� ��
	auto	Ret = Controller->UpdateTarget();

	auto*	Player = Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard = OwnerComp.GetBlackboardComponent();
	auto*	GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Controller));
	auto*	HomePoint = GameMode->GetHomePoint();
	ABladeIICharacter* pTarget = Controller->CurrentTargetMob;
	
	BII_CHECK(Player);
	BII_CHECK(Blackboard);
	BII_CHECK(GameMode);

	const FString EnumKey(TEXT("SelectedTaskMode"));
	const FString TargetToBeat(TEXT("TargetToBeat"));
	//const FString LocationToGo(TEXT("LocationToGo"));

	// Ÿ�� ������ ��Ȩ
	if (!Ret)
	{
		//Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_RETURNHOME));
		return;
	}

	// Ÿ�ٸ� ����
	Blackboard->SetValueAsObject(FName(*TargetToBeat), pTarget);

	// Ÿ�ٰ��� �Ÿ�
	float fTargetDistance = (pTarget->GetActorLocation() - Player->GetActorLocation()).Size();

	if (fTargetDistance < AttackTargetDistance)
	{
		// ���� ���� ���̸� ����
		Lottery	Lotto;

		Lotto.PushBall(static_cast<uint8>(BTTASK_MODE::EBM_MOVE), 0.3f);
		Lotto.PushBall(static_cast<uint8>(BTTASK_MODE::EBM_ATTACK), 0.7f);
		
		auto	ball = static_cast<BTTASK_MODE>(Lotto.PopBall());
		uint8	Task = 0;

		switch (ball)
		{
		case BTTASK_MODE::EBM_MOVE:
			Task = 0;
			break;
		case BTTASK_MODE::EBM_ATTACK:
			Task = 1;
			break;
		}

		Blackboard->SetValueAsEnum(FName(*EnumKey), Task);
	}
	else if (fTargetDistance < SearchTargetDistance)
	{
		// �ν� ���� ���̸� �̵�
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_MOVE));
	}
	else
	{
		// �׿ܿ� ��Ȩ
// 		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_RETURNHOME));
// 		Blackboard->SetValueAsObject(FName(*TargetToBeat), HomePoint);
	}
	//*/
}
