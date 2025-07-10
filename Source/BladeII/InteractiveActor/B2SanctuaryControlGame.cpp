// Fill out your copyright notice in the Description page of Project Settings.


#include "B2SanctuaryControlGame.h"
#include "B2ControlGameMode.h"
#include "BladeIINetPlayer.h"


AB2SanctuaryControlGame::AB2SanctuaryControlGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetIndex = -1;
}

void AB2SanctuaryControlGame::BeginPlay()
{
	Super::BeginPlay();

	// 첨에 꺼놓는다. 구슬 숨기는건BP에서 해줌
	SetPhase(EInteractivePhaseType::FinishPhase);
}

void AB2SanctuaryControlGame::ReceiveInteract(class AActor* InInteractingActor)
{
	////BeginInteract(InInteractingActor);
	//// 부모 기능 안쓴다. AB2SanctuaryActorBase::InteractAction내용 복붙 + 피안채우고 특정타겟 추적하게함.
	//// 원래는 로컬플레이어 추적한다. 힐량관련 내용 제거.

	//// 패킷 받구 꺼줌.
	//DeactiveSanctuary();

	//auto* World = GetWorld();
	//if (World == nullptr)
	//	return;

	//float FirstDropYawAngle = FMath::FRandRange(0.f, 360.f);
	//float ModAngle = (360.f / NumFragment);

	//FActorSpawnParameters SpawnParam;
	//SpawnParam.bDeferPostSpawnInitialize = true;

	//for (int32 i = 0; i < NumFragment; ++i)
	//{
	//	float MinAngle = i * ModAngle + FirstDropYawAngle;
	//	float MaxAngle = MinAngle + ModAngle;

	//	FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f);

	//	FTransform SpawnTransform(SpawnRotation, GetActorLocation() + SpawnEnergyBallOffset);

	//	auto* SpawnedActor = World->SpawnActor<AB2EnergyBall>(FragmentTemplateClass, SpawnTransform, SpawnParam);
	//	if (SpawnedActor)
	//	{
	//		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(InInteractingActor);

	//		if(pPlayer)
	//			SpawnedActor->SetSucker(pPlayer);

	//		SpawnedActor->SetHealPercentage(0);

	//		if (bRandomFragmentStayTime)
	//		{
	//			if (i == 0)
	//				SpawnedActor->StayTime += FragmentStayTimeRandomOffset_Min;
	//			else if (i == NumFragment - 1)
	//				SpawnedActor->StayTime += FragmentStayTimeRandomOffset_Max;
	//			else
	//			{
	//				float StepRange = (FragmentStayTimeRandomOffset_Max - FragmentStayTimeRandomOffset_Min) / NumFragment;

	//				SpawnedActor->StayTime += FMath::RandRange(StepRange * i, StepRange * (i + 1));
	//			}
	//		}

	//		World->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, SpawnParam);
	//	}

	//}

	//SetPhase(EInteractivePhaseType::FinishPhase);
}

void AB2SanctuaryControlGame::Interact(class AActor* InInteractingActor)
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
	{
		if (pGM->GetPeerType() != PEER_SUPER)
			return;
		
		// 플레이어만 고려함.
		ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(InInteractingActor);

		// AI는 먹지마라..
		if (pPlayer && pPlayer->GetNetStatus() != ABladeIINetPlayer::NET_AIMASTER)
		{
			// 일단 끄고 패킷전송
			SetPhase(EInteractivePhaseType::FinishPhase);

			pGM->NetSendDrinkControlSanctuary(pPlayer->GetNetId(), NetIndex);
		}
	}
	
}
