// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square
#include "B2AutoAIController.h"
//#include "BladeII.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameplayTasksComponent.h"
#include "B2MonsterSpawnPool.h"
#include "BladeIIPlayerController.h"

#include "B2AutoWayPoint.h"
#include "B2AutoWayPointMarker.h"
#include "EngineUtils.h"
#include "Event.h"
#include "B2DestructibleLevelObjBase.h"
#include "BladeIIAIController.h"

AB2AutoAIController::AB2AutoAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//if (HasAnyFlags(RF_ClassDefaultObject) == false) //-> Load for the CDO too, to include DefaultBT for cooking
	{
		FString DefaultBTPath;
		GConfig->GetString(TEXT("/Script/BladeII.BladeIIPlayer"), TEXT("DefaultAutoAIBT"), DefaultBTPath, GGameIni);
		DefaultBT = LoadObject<UBehaviorTree>(NULL, *DefaultBTPath);
	}

	CurrentTargetWP			= NULL;
	CurrentTargetMob		= NULL;
	bPassedAllWayPoint		= false;
	bRawPCHadMovementInput	= false;
	bPCHadMovementInput		= false;
	bLastPCHadMovementInput = false;
	fLastPCHadMovementInputFalseTime = 0.0f;
	bPCHadBattleInput		= false;
	CachedRangeAttackedMob	= NULL;

	bAlreadySubscribeEvent  = false;
}

void AB2AutoAIController::BeginPlay()
{
	Super::BeginPlay();

	//// Do not reset variables here, flow can come here after BeginAutoPlay, 
	//// when the Auto state transferred from main map -> extra map. Refer to AB2StageManager::RetrieveDataForExtraMap
	//CAPTURE_UOBJECT(AB2AutoAIController);
	//if (!bAlreadySubscribeEvent)
	//{
	//	StageCompleteTicket = StageCompleteClass<>::GetInstance().Subscribe(
	//		USE_CAPTURE_OBJECT_AND_TICKET(StageComplete)
	//			Capture->FinishAutoPlay();
	//		END_CAPTURE_OBJECT()
	//	);

	//	bAlreadySubscribeEvent = true;
	//}

	//StageCompleted = false;
}

void AB2AutoAIController::BeginDestroy()
{
	UnsubscribeEvents();
	Super::BeginDestroy();
}

void AB2AutoAIController::Destroyed()
{
	UnsubscribeEvents();
	Super::Destroyed();
}

FPathFollowingRequestResult AB2AutoAIController::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath /*= nullptr*/)
{
	ABladeIICharacter* pTargetPawn = Cast<ABladeIICharacter>(MoveRequest.GetGoalActor());

	if (pTargetPawn &&
		(!pTargetPawn->GetMovementComponent() || !pTargetPawn->GetMovementComponent()->IsActive()))
	{
		// 타겟이 위치고정이면 목표를 액터가아닌 위치로 이용한다. 캡슐크기 고려해서
		ABladeIICharacter* pOwnerPawn = Cast<ABladeIICharacter>(GetPawn());
		if (pOwnerPawn)
		{
			FVector DiffVec = pOwnerPawn->GetTargetLocationVectorFromPlayer(pTargetPawn);
			
			FAIMoveRequest LocationMoveRequest;
			LocationMoveRequest.SetGoalLocation(pOwnerPawn->GetActorLocation() + DiffVec);
			LocationMoveRequest.SetAcceptanceRadius(MoveRequest.GetAcceptanceRadius());
			return Super::MoveTo(LocationMoveRequest, OutPath);
		}
	}
	
	return Super::MoveTo(MoveRequest, OutPath);
}

void AB2AutoAIController::UnsubscribeEvents()
{
//	if (bAlreadySubscribeEvent)
//	{
//		StageCompleteClass<>::GetInstance().Unsubscribe(StageCompleteTicket);
//	}
}

void AB2AutoAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bPCHadMovementInput = false;
	if (bRawPCHadMovementInput == true)
	{
		bPCHadMovementInput = true; // It will last until next tick.
		bRawPCHadMovementInput = false; // Temporary.
	}


	// 이동 입력이 있다가 없어진틱
	if (bLastPCHadMovementInput && !bPCHadMovementInput)
	{
		fLastPCHadMovementInputFalseTime = GetWorld()->GetTimeSeconds();
	}

	// 받아온 이동 인풋상황 저장
	bLastPCHadMovementInput = bPCHadMovementInput;

	// 이동 입력 없어지고도 잠깐 유지
	if (GetWorld()->GetTimeSeconds() - fLastPCHadMovementInputFalseTime <= 0.1f)
	{
		bPCHadMovementInput = true;
	}

	if (!CurrentTargetMob)
		return;

	// CurrentTargetMob 쪽에서 콜백을 부르도록 바꿀.. 아니 근데 AutoAIController 도 보장이 안 되니 그냥 여기서 하는 게 속편하겠다.
	if (CurrentTargetMob->IsAlive() == false)
	{
		CurrentTargetMob = NULL;
	}
}

void AB2AutoAIController::BeginAutoPlay()
{
	CurrentTargetMK		= NULL;
	CurrentTargetWP		= NULL;
	CurrentTargetMob	= NULL;
	bPassedAllWayPoint	= false;

	// Setting CurrentTargetWP to null and call TargetToNextWayPoint will find the first one.
	TargetToNextWayPoint();
}

void AB2AutoAIController::FinishAutoPlay()
{
	CurrentTargetMK		= NULL;
	CurrentTargetWP		= NULL;
	CurrentTargetMob	= NULL;
	bPassedAllWayPoint	= true;

	if (BrainComponent)
		BrainComponent->StopLogic(TEXT("Clear Stage..."));

	StageCompleted = true;
}

void AB2AutoAIController::ResetAutoPlay()
{
	StageCompleted = false;
}

void AB2AutoAIController::TargetToNextWayPoint()
{
	// Supposed to be called when autoplay has just begun or the player just passed current WP.
	//auto* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (!B2GM)
	//	return;

	//TArray<AB2AutoWayPoint*> WorldWP = B2GM->GetWayPointList();

	//bool bFoundTarget = false;

	//// WorldWP should be sorted by PassOrder.
	//for (auto* waypoint : WorldWP)
	//{
	//	if (!waypoint->HasValidPassOrder())
	//		continue;

	//	if (CurrentTargetWP == NULL && !waypoint->IsPassed())
	//	{
	//		// If CurrentTargetWP is NULL (just started), pick up the earliest not-passed one as the current one.
	//		// WPs can be marked as passed in player controlled mode too.
	//		CurrentTargetWP = waypoint;
	//		CurrentTargetMK = AB2AutoWayPointMarker::GetInstance(GetWorld());
	//		bFoundTarget = true;
	//		break;
	//	}
	//	
	//	// Get the first one having larger PassOrder. We might miss one if we got duplicated PassOrder.
	//	// Editor ensures unique PassOrder, but level streaming can break it... (although we are not likely to use the level streaming.)
	//	// Then we might just pick up the earliest non-passed one.
	//	if (CurrentTargetWP && !waypoint->IsPassed() && waypoint->PassOrder > CurrentTargetWP->PassOrder)
	//	{
	//		CurrentTargetWP = waypoint;
	//		CurrentTargetMK = AB2AutoWayPointMarker::GetInstance(GetWorld());
	//		bFoundTarget = true;
	//		break;
	//	}
	//}

	//if (bFoundTarget == false)
	//{
	//	// Means, there's nothing or we have gone through all.
	//	bPassedAllWayPoint = true;

	//	OnPassedAllWayPoint();
	//}
}

bool AB2AutoAIController::UpdateTargetObjectInPriority(float minDistance, float maxDistance)
{
	auto* Destructible = PickOneInterestedDestructible();


	if (!Destructible)
	{
		CurrentTargetObject = NULL;
		return false;
	}

	auto* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());

	if (Destructible && PossessedPlayer )
	{
		const FVector PlayerLocation = PossessedPlayer->GetActorLocation();
		const float DistSQTargetObject = (Destructible->GetActorLocation() - PlayerLocation).SizeSquared();
		if (DistSQTargetObject < maxDistance*maxDistance)
		{
			CurrentTargetObject = Destructible;
			return true;
		}
		/*
		const float DistSQTargetMob = CurrentTargetMob? (CurrentTargetMob->GetActorLocation() - PlayerLocation).SizeSquared() : FLT_MAX;
		const float DistSQTargetObject = (Destructible->GetActorLocation() - PlayerLocation).SizeSquared();
		if (maxDistance*maxDistance > DistSQTargetObject && DistSQTargetMob > FMath::Max(DistSQTargetObject, minDistance * minDistance))
		{
			if (CurrentTargetWP)
			{
				const float DistSQTargetWP = CurrentTargetWP->FindNearestMarker(this, Destructible);
				if (DistSQTargetWP > minDistance && DistSQTargetWP * DistSQTargetWP < DistSQTargetObject)
				{
					CurrentTargetObject = NULL;
					return false;
				}
			}
			CurrentTargetObject = Destructible;
			return true;
		}
		*/
	}

	CurrentTargetObject = NULL;
	return false;
}

bool AB2AutoAIController::UpdateTargetMobInPriority()
{
	auto* GeneralTargetMob	= PickGeneralTargetMob();
	auto* InterestedWaveMob = PickOneInterestedWaveMob();

	if (!GeneralTargetMob && !InterestedWaveMob)
	{
		CurrentTargetMob = NULL;
		return false;
	}

	// If we got both, pick up the nearest one,
	// but I guess we might consider GeneralTargetMob as higher priority, as that is probably be the closest one already.
	auto* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());

	if ( GeneralTargetMob && InterestedWaveMob && PossessedPlayer )
	{
		const FVector PlayerLocation = PossessedPlayer->GetActorLocation();

		const float DistSQGeneralTargetMob = (GeneralTargetMob->GetActorLocation() - PlayerLocation).SizeSquared();
		const float DistSQInterestedWaveMob = (InterestedWaveMob->GetActorLocation() - PlayerLocation).SizeSquared();

		if (DistSQInterestedWaveMob < DistSQGeneralTargetMob)
		{
			CurrentTargetMob = InterestedWaveMob;
		}
		else
		{
			CurrentTargetMob = GeneralTargetMob;
		}

		return true;
	}

	CurrentTargetMob = GeneralTargetMob ? GeneralTargetMob : InterestedWaveMob;
	return true;
}

ABladeIICharacter* AB2AutoAIController::PickGeneralTargetMob()
{
	ABladeIICharacter*	FoundTarget		= NULL;
	ABladeIIPlayer*		PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());

	if (!PossessedPlayer)
		return FoundTarget;

	//BII_CHECK(PossessedPlayer && PossessedPlayer->IsValidObj());
	
	// Now, we just use B2PC's TargetActor kkkkk. It already got some logic to find a target.
	FoundTarget = PossessedPlayer->TargetActor;

	if (FoundTarget == CachedRangeAttackedMob)
	{
		CachedRangeAttackedMob = NULL; // Forget it as we already targeted it.
	}

	// Even PC does not have a TargetActor, player can be under ranged attack. Better not ignore it..
	if (FoundTarget == NULL && CachedRangeAttackedMob != NULL)
	{
		if (CachedRangeAttackedMob->IsAlive() &&
			(PossessedPlayer->GetActorLocation() - CachedRangeAttackedMob->GetActorLocation()).Size() < CachedRangeAttackedMob->GetAIMaxAttackRange() * 1.1f)
		{
			// Check the distance again. If it is located out of its own target range, forget it.
			FoundTarget = CachedRangeAttackedMob;
		}

		CachedRangeAttackedMob = NULL; // Forget it until it attacks me again.
	}

	return FoundTarget;
}

ABladeIICharacter* AB2AutoAIController::PickOneInterestedWaveMob()
{
	//auto* PossessedPlayer	= Cast<ABladeIIPlayer>(GetPawn());
	//auto* B2SGM				= Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//auto* StageManager		= B2SGM ? B2SGM->GetStageManager() : NULL;
	//
	//if (!PossessedPlayer)
	//	return NULL;

	//const FVector PlayerLocation = PossessedPlayer->GetActorLocation();

	//TArray<ABladeIICharacter*> LiveEnemys;
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//BII_CHECK(B2GM);
	//if (B2GM){
	//	B2GM->GetAllWorldEnemyList(LiveEnemys);
	//}

	// Could be various conditions, but let's just pick up the closest one.
	float ClosestDistanceSQ = BIG_NUMBER;
	ABladeIICharacter* SelectedOne = NULL;

	//for (auto* mob : LiveEnemys)
	//{
	//	if (!mob->IsAlive() || mob == PossessedPlayer)
	//		continue;

	//	const float CurrDistSQ = (mob->GetActorLocation() - PlayerLocation).SizeSquared();

	//	if (CurrDistSQ < ClosestDistanceSQ)
	//	{
	//		ClosestDistanceSQ = CurrDistSQ;
	//		SelectedOne = mob;
	//	}
	//}

	return SelectedOne;
}

class AActor* AB2AutoAIController::PickOneInterestedDestructible()
{
	//auto* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());
	//auto* B2SGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//auto* StageManager = B2SGM ? B2SGM->GetStageManager() : NULL;

	//if (!PossessedPlayer)
	//	return NULL;

	//const FVector PlayerLocation = PossessedPlayer->GetActorLocation();

	//TArray<AB2DestructibleLevelObjBase*> Objects;
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//BII_CHECK(B2GM);
	//if (B2GM) {
	//	Objects = B2GM->GetDestructibleList();
	//}

	//// Could be various conditions, but let's just pick up the closest one.
	//float ClosestDistanceSQ = BIG_NUMBER;
	AActor* SelectedOne = NULL;

	//for (auto* mob : Objects)
	//{
	//	if (!mob->IsValidObj())
	//		continue;

	//	//const float CurrDistSQ = (mob->GetActorLocation() - PlayerLocation).SizeSquared();
	//	float CurrDistSQ = FLT_MAX;
	//	FHitResult Result;
	//	//GetWorld()->LineTraceSingleByChannel(Result, PlayerLocation, mob->GetActorLocation(), ECC_WorldStatic, FCollisionQueryParams(NAME_None, false, UGameplayStatics::GetLocalPlayerCharacter(this)));
	//	if (Result.bBlockingHit && Result.Actor == mob)
	//	{
	//		if (GetWorld()->GetNavigationSystem()->GetPathLength(PlayerLocation, mob->GetActorLocation(), CurrDistSQ) == ENavigationQueryResult::Success)
	//		{
	//			if (CurrDistSQ < ClosestDistanceSQ)
	//			{
	//				ClosestDistanceSQ = CurrDistSQ;
	//				SelectedOne = mob;
	//			}
	//		}
	//	}
	//}
	return SelectedOne;
}

bool AB2AutoAIController::HadMovementInput()
{
	return bPCHadMovementInput;
}

bool AB2AutoAIController::HadBattleInput()
{
	ABladeIIPlayer* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());
	if (PossessedPlayer)
	{
		return PossessedPlayer->IsAttackKeyPressed() || PossessedPlayer->IsGuardKeyPressed();
	}

	return false;
}

bool AB2AutoAIController::ShouldStopAutoDueToCinematicMode()
{
	//auto* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());
	//if (!PossessedPlayer)
		return false;

	//auto* B2PC = Cast<ABladeIIPlayerController>(PossessedPlayer->GetController());
	//if (!B2PC)
	//	return false;

	//// 모든 CinematicMode 보다는 입력이 안 들어오는 경우만. 입력이 먹힌다는 얘기는 게임 플레이가 유지될 것이란 얘기이므로.
	//return B2PC->IsInputDisabledCinematicMode();
}

//void AB2AutoAIController::Possess(APawn* InPawn)
//{
//	// already cleared stage...
//	// so we are not going to need to play auto...
//	if (StageCompleted)
//		return;
//
//	// Cannot use Super's Possess. It unpossesses current pawn.
//	// We need to maintain current pawn(player)'s possess state. This AutoAIController is additional.
//	//Super::Possess(InPawn);
//
//	// don't even try possessing pending-kill pawns
//	if (InPawn != nullptr && InPawn->IsPendingKill())
//	{
//		return;
//	}
//
//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(InPawn);
//	BII_CHECK(B2Player);
//
//	if (B2Player != NULL)
//	{
//		//B2Player->PossessedBy(this); -> Don't do this kind of stuff here. Player still needs to be possessed by player controller.
//		
//		SetPawn(B2Player);
//
//		// update rotation to match possessed pawn's rotation
//		SetControlRotation(B2Player->GetActorRotation());
//		
//		// no point in doing navigation setup if player has no movement component
//		const UPawnMovementComponent* MovementComp = B2Player->GetMovementComponent();
//		if (MovementComp != NULL)
//		{
//			UpdateNavigationComponents();
//		}
//
//		if (GetPathFollowingComponent())
//		{
//			GetPathFollowingComponent()->Initialize();
//		}
//
//		if (bWantsPlayerState)
//		{
//			ChangeState(NAME_Playing);
//		}
//
//		// a Pawn controlled by AI _requires_ a GameplayTasksComponent, so if Pawn 
//		// doesn't have one we need to create it
//		if (CachedGameplayTasksComponent == nullptr)
//		{
//			UGameplayTasksComponent* GTComp = B2Player->FindComponentByClass<UGameplayTasksComponent>();
//			if (GTComp == nullptr)
//			{
//				GTComp = NewObject<UGameplayTasksComponent>(InPawn, TEXT("GameplayTasksComponent"));
//				GTComp->RegisterComponent();
//			}
//			CachedGameplayTasksComponent = GTComp;
//		}
//
//		if (CachedGameplayTasksComponent && !CachedGameplayTasksComponent->OnClaimedResourcesChange.Contains(this, GET_FUNCTION_NAME_CHECKED(AAIController, OnGameplayTaskResourcesClaimed)))
//		{
//			CachedGameplayTasksComponent->OnClaimedResourcesChange.AddDynamic(this, &AAIController::OnGameplayTaskResourcesClaimed);
//
//			//REDIRECT_OBJECT_TO_VLOG(CachedGameplayTasksComponent, this);
//		}
//
//		if (BrainComponent)
//		{
//			BrainComponent->Cleanup();
//		}
//
//		// Set the behavior tree, the real shit here.
//		if (B2Player->BehaviorTree)
//		{
//			RunBehaviorTree(B2Player->BehaviorTree);
//		}
//		else if (DefaultBT)
//		{
//			RunBehaviorTree(DefaultBT);
//		}
//
//		BeginAutoPlay();
//	}
//
//	OnPossess(InPawn);
//}
//
//void AB2AutoAIController::UnPossess()
//{
//	//Super::UnPossess();
//
//	APawn* OldPawnBackup = GetPawn();
//
//	// Like Possess, don't use Super's here. Player pawn still get possessed by the player controller after this.
//	// Don't do something like Pawn->UnPossessed() here.
//	SetPawn(NULL);
//
//	if (GetPathFollowingComponent())
//	{
//		GetPathFollowingComponent()->Cleanup();
//	}
//
//	if (bStopAILogicOnUnposses)
//	{
//		if (BrainComponent)
//		{
//			BrainComponent->Cleanup();
//		}
//	}
//
//	if (CachedGameplayTasksComponent && (CachedGameplayTasksComponent->GetOwner() == OldPawnBackup))
//	{
//		CachedGameplayTasksComponent->OnClaimedResourcesChange.RemoveDynamic(this, &AAIController::OnGameplayTaskResourcesClaimed);
//		CachedGameplayTasksComponent = nullptr;
//	}
//
//	OnUnpossess(OldPawnBackup);
//
//	CurrentTargetWP = NULL;
//	CurrentTargetMob = NULL;
//	CurrentTargetMK = NULL;
//
//	// AIController still alive after UnPossess. ABladeIIGameMode will destroy this AutoAIController after UnPossess.
//}

void AB2AutoAIController::OnPassedAllWayPoint()
{
	CurrentTargetWP = NULL;
	CurrentTargetMK = NULL;

	// When all the waypoints are cleared, we can either stop the auto play like below,
	// but let's just leave it, then possessed player can still fight if an enemy is spotted at nearby location.
	//if(ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetPawn()))
	//{
	//	B2Player->StopAutoPlay(); // It will eventually destroy this AutoAIController..
	//}
}