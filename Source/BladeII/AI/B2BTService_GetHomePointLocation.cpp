// Fill out your copyright notice in the Description page of Project Settings.

#include "B2BTService_GetHomePointLocation.h"
//#include "BladeII.h"
#include "BladeIIGameMode.h"
#include "B2HomePoint.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


void UB2BTService_GetHomePointLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto*	Controller = Cast<AAIController>(OwnerComp.GetOwner());
	BII_CHECK(Controller);

	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Controller));
	BII_CHECK(GameMode);

	//auto* HomePoint = GameMode->GetHomePoint();

	//if(HomePoint)
	//	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.SelectedKeyName, HomePoint->GetActorLocation());
}
