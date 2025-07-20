// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlReadyToBattle.h"




UB2UIControlReadyToBattle::UB2UIControlReadyToBattle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FightFxOffsetTime = 0.f;
	FightFxCamDistance = 100.f;
	FightFxScale = 1.f;
}

void UB2UIControlReadyToBattle::ShowFightFX()
{
	//if (FightFX == nullptr)
	//	return;

	//APlayerController* LocalPlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
	//ULocalPlayer* const LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
	//if (LP && LP->ViewportClient)
	//{
	//	FVector2D Center;
	//	LP->ViewportClient->GetViewportSize(Center);
	//	Center *= 0.5f;

	//	FVector WorldLoc;
	//	FVector WorldDir;
	//	LocalPlayerController->DeprojectScreenPositionToWorld(Center.X, Center.Y, WorldLoc, WorldDir);

	//	auto* CameraManager = UGameplayStatics::GetLocalPlayerCameraManager(GetWorld());
	//	if (CameraManager)
	//	{
	//		PSCFightFX = NewObject<UParticleSystemComponent>(GetWorld()->GetWorldSettings());
	//		PSCFightFX->bAutoDestroy = true;
	//		PSCFightFX->bAllowAnyoneToDestroyMe = true;
	//		PSCFightFX->SecondsBeforeInactive = 0.0f;
	//		PSCFightFX->bAutoActivate = false;
	//		PSCFightFX->SetTemplate(FightFX);
	//		PSCFightFX->bOverrideLODMethod = false;

	//		PSCFightFX->AttachToComponent(CameraManager->GetTransformComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	//		PSCFightFX->SetAbsolute(false, false, true);
	//		PSCFightFX->SetRelativeTransform(FTransform(FRotator(0.f, 180.f, 0.f), FVector(FightFxCamDistance, 0.f, 0.f), FVector(FightFxScale)));

	//		PSCFightFX->RegisterComponentWithWorld(GetWorld());

	//		PSCFightFX->ActivateSystem(true);
	//	}
	//}
}
