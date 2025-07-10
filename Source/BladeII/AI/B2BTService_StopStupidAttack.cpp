
#include "B2BTService_StopStupidAttack.h"
//#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BladeIIPlayer.h"

UB2BTService_StopStupidAttack::UB2BTService_StopStupidAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName		= "StopStupidAttack";
	Interval		= 0.0f;
	RandomDeviation = 0.0f;
}

void UB2BTService_StopStupidAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	Owner		= Cast<AController>(OwnerComp.GetOwner());
	auto*	Avatar		= Cast<ABladeIIPlayer>(Owner->GetPawn());
	auto*	BlackBoard	= OwnerComp.GetBlackboardComponent();

	if (!Avatar)
	{
		//Avatar = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
		//if (Avatar)
		//{
		//	Avatar->StopAttack();
		//	return;
		//}
	}

	BII_CHECK(Owner);
	BII_CHECK(Avatar);
	BII_CHECK(BlackBoard)
		
	if (!(Avatar && Avatar->IsSkillAttacking() == false))
		return;

	Avatar->StopAttack();
}