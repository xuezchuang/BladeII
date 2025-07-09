// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "B2BTService_GetHomePointLocation.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2BTService_GetHomePointLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
		
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
