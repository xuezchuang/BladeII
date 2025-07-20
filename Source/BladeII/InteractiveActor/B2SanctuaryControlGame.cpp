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

	// 梅俊 波初绰促. 备浇 见扁绰扒BP俊辑 秦淋
	SetPhase(EInteractivePhaseType::FinishPhase);
}

void AB2SanctuaryControlGame::ReceiveInteract(class AActor* InInteractingActor)
{
	////BeginInteract(InInteractingActor);
	//// 何葛 扁瓷 救敬促. AB2SanctuaryActorBase::InteractAction郴侩 汗嘿 + 乔救盲快绊 漂沥鸥百 眠利窍霸窃.
	//// 盔贰绰 肺拿敲饭捞绢 眠利茄促. 鳃樊包访 郴侩 力芭.

	//// 菩哦 罐备 波淋.
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
		
		// 敲饭捞绢父 绊妨窃.
		ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(InInteractingActor);

		// AI绰 冈瘤付扼..
		if (pPlayer && pPlayer->GetNetStatus() != ABladeIINetPlayer::NET_AIMASTER)
		{
			// 老窜 掺绊 菩哦傈价
			SetPhase(EInteractivePhaseType::FinishPhase);

			pGM->NetSendDrinkControlSanctuary(pPlayer->GetNetId(), NetIndex);
		}
	}
	
}
