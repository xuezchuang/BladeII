
#include "B2BTService_SelectTaskSequence.h"
#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "B2AI.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "B2Lottery.h"

UB2BTService_SelectTaskSequence::UB2BTService_SelectTaskSequence(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Task Sequence";
	Interval = 0.25f;
	RandomDeviation = 0.25f;
}

void UB2BTService_SelectTaskSequence::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	Controller	= Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	auto*	Player		= Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard	= OwnerComp.GetBlackboardComponent();

	//auto	Ret			= Controller->UpdateTargetAndLocation(TEXT("LocationToGo"));
	auto	Ret = Controller->UpdateTarget();

	BII_CHECK(Controller);
	BII_CHECK(Player);
	BII_CHECK(Blackboard);

	const FString EnumKey(TEXT("SelectedTaskMode"));

	Lottery	Lotto;

	// to make probability range...
	if (Ret)
	{
		Lotto.PushBall(static_cast<uint8>(BTTASK_MODE::EBM_MOVE), 0.3f);
		Lotto.PushBall(static_cast<uint8>(BTTASK_MODE::EBM_ATTACK), 0.7f);
	}
	else
	{
		Lotto.PushBall(static_cast<uint8>(BTTASK_MODE::EBM_MOVE), 0.9f);
		Lotto.PushBall(static_cast<uint8>(BTTASK_MODE::EBM_ATTACK), 0.1f);
	}

	auto	ball	= static_cast<BTTASK_MODE>(Lotto.PopBall());
	uint8	Task	= 0;

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