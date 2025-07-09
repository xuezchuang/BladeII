#include "BladeII.h"
#include "B2BTService_UpdateTargetOrWayPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "B2AutoAIController.h"
#include "B2AutoWayPoint.h"
#include "B2AutoWayPointMarker.h"
#include "BladeIIPlayerController.h"

UB2BTService_UpdateTargetOrWayPoint::UB2BTService_UpdateTargetOrWayPoint(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	NodeName		= "UpdateTargetOrWayPoint";
	Interval		= 0.25f;
	RandomDeviation = 0.1f;
}

void UB2BTService_UpdateTargetOrWayPoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//auto*	Owner = Cast<AB2AutoAIController>(OwnerComp.GetOwner());
	//
	//// 없으면 로컬플레이어꺼 갖다씀
	//if (!Owner)
	//{
	//	ABladeIIPlayerController* pPC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//	Owner = pPC->GetAutoAIController();
	//}

	//auto	RetMob = Owner->UpdateTargetMobInPriority();
	//auto*	BlackBoard = OwnerComp.GetBlackboardComponent();

	//BII_CHECK(Owner);
	//BII_CHECK(BlackBoard);

	//const FString WayPointKey(TEXT("TargetWayPoint"));
	//const FString TargetMobKey(TEXT("CombatTargetMob"));
	//const FString TargetKey(TEXT("TargetInteresting"));

	//float MobDist = FLT_MAX;
	//float objDist = FLT_MAX;

	//if (RetMob)
	//{
	//	if (Owner->CurrentTargetMob)
	//	{
	//		ABladeIICharacter* pOwnerCharacter = Cast<ABladeIICharacter>(Owner->GetPawn());
	//		
	//		if(pOwnerCharacter)
	//			MobDist = pOwnerCharacter->GetTargetLocationVectorFromPlayer(Owner->CurrentTargetMob).Size2D();
	//	}	
	//}

	//if (!RetMob)
	//{
	//	BlackBoard->ClearValue(FName(*TargetMobKey));
	//}

	//BlackBoard->SetValueAsObject(FName(*WayPointKey), Owner->CurrentTargetMK);
	////vs waypoint
	//float WayPointDist = FLT_MAX;

	//if (RetMob)
	//{
	//	if (Owner->CurrentTargetWP)
	//		WayPointDist = Owner->CurrentTargetWP->FindNearestMarker(Owner, Owner->CurrentTargetMob);

	//	if (FMath::Max(WayPointDist, SearchMinMonsterDistance) < MobDist)
	//	{
	//		RetMob = false;
	//		BlackBoard->SetValueAsObject(FName(*TargetKey), Owner->CurrentTargetMK);
	//		BlackBoard->ClearValue(FName(*TargetMobKey));
	//	}
	//	else
	//	{
	//		BlackBoard->SetValueAsObject(FName(*TargetKey), Owner->CurrentTargetMob);
	//		BlackBoard->SetValueAsObject(FName(*TargetMobKey), Owner->CurrentTargetMob);
	//	}
	//}
	//else
	//{
	//	AActor* pTarget = Cast<AActor>(BlackBoard->GetValueAsObject(FName(*TargetKey)));
	//	if (Owner->CurrentTargetWP)
	//	{
	//		if (pTarget != Owner->CurrentTargetMK && pTarget != NULL)
	//			Owner->CurrentTargetWP->FindNearestMarker(Owner, pTarget);
	//		else
	//			Owner->CurrentTargetWP->FindNearestMarker(Owner, Owner->CurrentTargetWP);
	//	}

	//	BlackBoard->SetValueAsObject(FName(*WayPointKey), Owner->CurrentTargetMK);
	//	BlackBoard->SetValueAsObject(FName(*TargetKey), Owner->CurrentTargetMK);
	//	BlackBoard->ClearValue(FName(*TargetMobKey));		
	//}

	//if(!RetMob)
	//{
	//	BlackBoard->SetValueAsObject(FName(*WayPointKey), Owner->CurrentTargetMK);
	//}
}