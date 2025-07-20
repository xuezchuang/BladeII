#pragma once

#include "BehaviorTree/BTService.h"
#include "Knowledge.h"
#include "B2BTService_UpdateKnowledge.generated.h"

UCLASS()
class UB2BTService_UpdateKnowledge : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	void UpdateKnowledge(class ABladeIIPlayerAIController*, class ABladeIIPlayer*);
};