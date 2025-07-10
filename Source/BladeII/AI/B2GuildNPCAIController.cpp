// Fill out your copyright notice in the Description page of Project Settings.


#include "B2GuildNPCAIController.h"
//#include "BladeII.h"
#include "BrainComponent.h"
#include "BladeIIMercenaryPlayer.h"

AB2GuildNPCAIController::AB2GuildNPCAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentTarget = nullptr;
	TeamNum = INDEX_NONE;
}

//void AB2GuildNPCAIController::Possess(APawn * InPawn)
//{
//	AAIController::Possess(InPawn);
//	SetTeamNum(TeamNum);
//}
//
//void AB2GuildNPCAIController::UnPossess()
//{
//	CurrentTarget = nullptr;
//	Super::UnPossess();
//}

bool AB2GuildNPCAIController::BeginAutoPlay()
{
	CurrentTarget = nullptr;

	ABladeIICharacter* B2Char = Cast<ABladeIICharacter>(GetPawn());
	if (B2Char && B2Char->BehaviorTree)
	{
		if (BrainComponent)
			BrainComponent->RestartLogic();
		else
			RunBehaviorTree(B2Char->BehaviorTree);
		return true;
	}
		
	return false;
}

void AB2GuildNPCAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	ABladeIIMercenaryPlayer* B2Char = Cast<ABladeIIMercenaryPlayer>(GetPawn());
	if (B2Char && B2Char->CanRotateToTarget())
		Super::UpdateControlRotation(DeltaTime, bUpdatePawn);
}

ABladeIICharacter * AB2GuildNPCAIController::PickOpponent()
{
	ABladeIICharacter* SelectedOne = nullptr;
	if (AB2GuildGameMode* GuildGameMode = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		SelectedOne = GuildGameMode->GetHighAggroOpponentCharacter(Cast<ABladeIICharacter>(GetPawn()), GetTeamNum(), GetPawn() ? GetPawn()->GetActorLocation(): FVector());
	}
	return SelectedOne;
}

bool AB2GuildNPCAIController::UpdateTarget()
{
	CurrentTarget = PickOpponent();

	return CurrentTarget != nullptr;
}

ABladeIICharacter * AB2GuildNPCAIController::GetCurrentTarget()
{
	return CurrentTarget;
}

void AB2GuildNPCAIController::SetTeamNum(int8 InTeamNum)
{
	if (AB2GuildGameMode* GuildGameMode = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		ABladeIIMercenaryPlayer* ControlledPawn = Cast<ABladeIIMercenaryPlayer>(GetPawn());
		if (ControlledPawn)
		{
			ControlledPawn->SetTeamNum(InTeamNum);
			ControlledPawn->SetPlayerControlSkill(InTeamNum == GuildGameMode->GetMyTeamNum());
		}
	}
	TeamNum = InTeamNum;
}

int8 AB2GuildNPCAIController::GetTeamNum()
{
	return TeamNum;
}

AB2TestGuildNPCAIController::AB2TestGuildNPCAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ABladeIICharacter * AB2TestGuildNPCAIController::PickOpponent()
{
	//return Cast<ABladeIICharacter>(UGameplayStatics::GetLocalPlayerCharacter(this));
	return NULL;
}

void AB2TestGuildNPCAIController::SetTeamNum(int8 InTeamNum)
{
	TeamNum = InTeamNum;
	ABladeIICharacter* B2Character = Cast<ABladeIICharacter>(GetPawn());
	if (B2Character)
		B2Character->SetTeamNum(InTeamNum);
}
