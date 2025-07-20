
#include "BladeIIPlayerAIController.h"
//#include "BladeII.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameplayTasksComponent.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "BladeIIGameMode.h"
#include "BladeIIUtil.h"
#include "BladeIIGameMode.h"
#include "B2TMGameMode.h"
#include "B2GuildGameMode.h"
#include "B2SkillRTManager.h"

ABladeIIPlayerAIController::ABladeIIPlayerAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TotalDashDuration	= -1.f;
	CurrentDashDuration = -1.f;
	MaxDashDistance		= 1000.f;

	TeamNum				= INDEX_NONE;
	
	InjectedGameRule	= nullptr;
}

void ABladeIIPlayerAIController::BeginPlay()
{
	Super::BeginPlay();

	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	BII_CHECK(GameMode);

	InjectedGameRule = GameMode->GetGameRule();

	LoadBehaviorTreeFromAsset();
}

void ABladeIIPlayerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Char == nullptr)
		return;
	
	if (BrainComponent && !BrainComponent->IsRunning())
		return;	

	if (B2Char->IsInQTEState() && !B2Char->IsQTEMounting())
	{
		return;
	}

	// Validate Target
	if (!B2Char->bNeedUpdateTarget)
	{
		if (B2Char->TargetActor == NULL)
		{
			B2Char->bNeedUpdateTarget = true;
		}
		else if (B2Char->TargetActor->bIsDying)
		{
			B2Char->TargetActor = NULL; // Prevent last attack rotation
		}
		else if (B2Char->TargetActor->IsInQTEState())
		{
			// No UpdateTarget for QTE state...
			return;
		}
		else if (B2Char->GetLastMovementInputVector() != FVector::ZeroVector && B2Char->bDisableRotateToInput)
		{
			// Guided target change
			B2Char->bNeedUpdateTarget = true;
		}
		else if (B2Char->TargetingMode == ETargetingMode::ETM_RetargetNearest && TotalDashDuration <= 0.f)
		{
			B2Char->bNeedUpdateTarget = true;
		}
		else if (TotalDashDuration <= 0.f) // Don't update target during dash
		{
			FVector PlayerLoc = B2Char->GetCapsuleComponent() ? B2Char->GetActorLocation() + B2Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : B2Char->GetActorLocation();
			FVector TargetLoc = B2Char->TargetActor->GetBaseMesh() ? B2Char->TargetActor->GetBaseMesh()->GetComponentLocation() : B2Char->TargetActor->GetActorLocation();

			if ((PlayerLoc - TargetLoc).Size() > B2Char->TargetingDistance)
			{
				B2Char->bNeedUpdateTarget = true;
			}
			else
			{
				FCollisionQueryParams Params(ABladeIIPlayer::TargetTraceName, false, this);
				Params.AddIgnoredActor(B2Char->TargetActor);
				FHitResult Result;
				GetWorld()->LineTraceSingleByChannel(Result, PlayerLoc, TargetLoc, ECC_Visibility, Params);

				B2Char->bNeedUpdateTarget = Result.bBlockingHit;
			}
		}
	}

	if (B2Char->bNeedUpdateTarget)
	{
		B2Char->bNeedUpdateTarget = false;
		FindTarget();
	}
	if (B2Char)
	{
		B2Char->RotateToTarget(DeltaTime);
	}
}
//
//void ABladeIIPlayerAIController::Possess(APawn* InPawn)
//{
//	Super::Possess(InPawn);
//
//	auto* B2Player = Cast<ABladeIIPlayer>(GetPawn());
//	BII_CHECK(B2Player);
//	
//	B2Player->Role = ROLE_Authority;
//}
//
//void ABladeIIPlayerAIController::UnPossess()
//{
//	auto* B2Player = Cast<ABladeIIPlayer>(GetPawn());
//	if (B2Player)
//	{
//		B2Player->Role = ROLE_SimulatedProxy;
//		B2Player->StopRepeatingInput();
//	}
//		
//	Super::UnPossess();
//	
//	CurrentTargetMob	= NULL;
//}

void ABladeIIPlayerAIController::BeginAutoPlay()
{
	CurrentTargetMob	= NULL;
	RunBehaviorTree(DefaultBT);
}

void ABladeIIPlayerAIController::StopAutoPlay()
{
	/*
	UnPossess();
	/*/
	if(BrainComponent)
		BrainComponent->StopLogic(TEXT("ForceStop"));

	auto* ControlledPlayerPawn = Cast<ABladeIIPlayer>(GetPawn());
	if (ControlledPlayerPawn)
	{
		ControlledPlayerPawn->StopRepeatingInput();
		ControlledPlayerPawn->SetAttackState(EAttackState::ECS_None);
	}
	//*/
}

bool ABladeIIPlayerAIController::IsRunningAI()
{
	if (BrainComponent)
		return BrainComponent->IsRunning();
	return false;
}


bool ABladeIIPlayerAIController::UpdateTargetAndLocation(FString const& KeyName)
{
	BII_CHECK(KeyName.Len() > 0);

	auto* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());
	BII_CHECK(PossessedPlayer);

	auto* PickedOpponent = PickOpponent();

	if (!PickedOpponent)
	{
		CurrentTargetMob = NULL;
		return false;
	}

	auto KeyId = Blackboard->GetKeyID(FName(*KeyName));
	if (KeyId != FBlackboard::InvalidKey)
	{
		auto NearLocation = GetRandomReachablePointInRadiusWithCurrNavSys(GetWorld(), PickedOpponent->GetActorLocation(), PickedOpponent->GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.0f);
		Blackboard->SetValueAsVector(FName(*KeyName), NearLocation);
	}

	CurrentTargetMob = PickedOpponent;

	//捞霸 绝栏搁 某腐磐啊 鸥百阑 酒 促聪搁辑 傍拜阑 救窍霸 凳.
	//快府绰 Rotate to face BB entry甫 荤侩且荐 绝澜(捞蜡绰 咯矾啊瘤 烙.)
	PossessedPlayer->TargetActor = CurrentTargetMob;

	return true;
}

bool ABladeIIPlayerAIController::UpdateTarget()
{
	CurrentTargetMob = PickOpponent();

	if (!CurrentTargetMob)
	{
		return false;
	}

	//捞霸 绝栏搁 某腐磐啊 鸥百阑 酒 促聪搁辑 傍拜阑 救窍霸 凳.
	//快府绰 Rotate to face BB entry甫 荤侩且荐 绝澜(捞蜡绰 咯矾啊瘤 烙.)
	ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());
	if (B2Char)
	{
		B2Char->TargetActor = CurrentTargetMob;
		if (CurrentTargetMob->bIsDying)
			B2Char->TargetActor = nullptr;
	}

	return true;
}

void ABladeIIPlayerAIController::FindTarget()
{
	// Find Target...
	ABladeIIPlayer* B2Char = Cast<ABladeIIPlayer>(GetPawn());

	if (!B2Char)
	{
		return;
	}

	auto* PrevActor		= B2Char->TargetActor;
	float TargetDist	= B2Char->UpdateTarget(false);

	if (B2Char->TargetActor == NULL || PrevActor != B2Char->TargetActor || TargetDist > MaxDashDistance)
	{
		B2Char->StopDash();
	}
}

ABladeIICharacter* ABladeIIPlayerAIController::PickOpponent()
{
	auto* PossessedPlayer = Cast<ABladeIIPlayer>(GetPawn());

	//
	BII_CHECK(PossessedPlayer);

	const FVector PlayerLocation = PossessedPlayer->GetActorLocation();

	TArray<ABladeIICharacter*> LiveMobs;
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	BII_CHECK(B2GM);

	ABladeIICharacter* SelectedOne = nullptr;

	B2GM->GetAllWorldEnemyList(LiveMobs, GetTeamNum());
	// Could be various conditions, but let's just pick up the closest one.
	float ClosestDistanceSQ = BIG_NUMBER;

	for (auto* mob : LiveMobs)
	{
		if (mob == PossessedPlayer || !mob->IsAlive())
			continue;

		const float CurrDistSQ = (mob->GetActorLocation() - PlayerLocation).SizeSquared();

		if (CurrDistSQ < ClosestDistanceSQ)
		{
			ClosestDistanceSQ = CurrDistSQ;
			SelectedOne = mob;
		}
	}


	return SelectedOne;
}

void ABladeIIPlayerAIController::LoadBehaviorTreeFromAsset()
{
	if (DefaultBT)
		return;

	auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	BII_CHECK(GameMode);

	InjectedGameRule = GameMode->GetGameRule();

	auto  Asset = InjectedGameRule->GetBehaviorTreeAsset();

	if (!Asset.IsEmpty())
	{
		DefaultBT = LoadObject<UBehaviorTree>(NULL, *Asset);
	}
}

class ABladeIICharacter* AB2TeamMatchPlayerAIController::PickOpponent()
{
	ABladeIICharacter* SelectedOne = nullptr;
	if (AB2TMGameMode* TeamMatchGame = Cast<AB2TMGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		SelectedOne = TeamMatchGame->GetHighAggroOpponentCharacter(GetTeamNum());
	}

	return SelectedOne;
}


class ABladeIICharacter* AB2GuildPlayerAIController::PickOpponent()
{
	ABladeIICharacter* SelectedOne = nullptr;
	if (AB2GuildGameMode* GuildGameMode = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		SelectedOne = GuildGameMode->GetHighAggroOpponentCharacter(Cast<ABladeIICharacter>(GetPawn()), GetTeamNum(), GetPawn() ? GetPawn()->GetActorLocation() : FVector());
	}

	return SelectedOne;
}
