
#include "B2BTService_UpdateKnowledge.h"
//#include "BladeII.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"

UB2BTService_UpdateKnowledge::UB2BTService_UpdateKnowledge(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName		= "Update Knowledge";
	Interval		= 0.25f;
	RandomDeviation = 0.0f;
}

void UB2BTService_UpdateKnowledge::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	MyController = Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	BII_CHECK(MyController);

	MyController->ClearMyMemory();
	
	for (FConstControllerIterator Iterator = MyController->GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		auto*	AIControled = Cast<ABladeIIPlayerAIController>(*Iterator);

		if (!AIControled)
			continue;

		auto*	Pawn		= Cast<ABladeIIPlayer>(AIControled->GetPawn());
		UpdateKnowledge(MyController, Pawn);
	}
}

void UB2BTService_UpdateKnowledge::UpdateKnowledge(class ABladeIIPlayerAIController* MyController, class ABladeIIPlayer* Player)
{
	auto&	Memory	=	MyController->GetMyMemory();
}