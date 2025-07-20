#include "B2BTDecorator_IfPlayerIsAlive.h"
//#include "BladeII.h"


#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIIUtil.h"

UB2BTDecorator_IfPlayerIsAlive::UB2BTDecorator_IfPlayerIsAlive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Run while player is alive";
}

bool UB2BTDecorator_IfPlayerIsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Controller	= Cast<AController>(OwnerComp.GetOwner());
	const auto* Player		= Cast<ABladeIICharacter>(Controller->GetPawn());

	BII_CHECK(Controller && Player);

	return Player->IsAlive();
}