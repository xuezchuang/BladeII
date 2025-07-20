#pragma once

#include "B2AIUtil.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "B2BTService_CheckAggroByTaunt.generated.h"

UCLASS()
class UB2BTService_CheckAggroByTaunt : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector TauntListKey;

	UPROPERTY(Transient)
	UB2CharacterContainer* TauntList;

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};