// Fill out your copyright notice in the Description page of Project Settings.


#include "B2SanctuaryActorBase.h"
#include "BladeIIPlayer.h"

// Sets default values
AB2SanctuaryActorBase::AB2SanctuaryActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume;
	TotalHealPercentage = 50.f;
	NumFragment = 4;
}

void AB2SanctuaryActorBase::InteractAction()
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	float FirstDropYawAngle = FMath::FRandRange(0.f, 360.f);
	float ModAngle = (360.f / NumFragment);

	float RemainHeal = TotalHealPercentage;

	FActorSpawnParameters SpawnParam;
	//SpawnParam.bDeferPostSpawnInitialize = true;

	for (int32 i = 0; i < NumFragment; ++i)
	{
		float MinAngle = i * ModAngle + FirstDropYawAngle;
		float MaxAngle = MinAngle + ModAngle;

		FRotator SpawnRotation(0.f, FMath::FRandRange(MinAngle, MaxAngle), 0.f);
		
		FTransform SpawnTransform(SpawnRotation, GetActorLocation() + SpawnEnergyBallOffset);

		auto* SpawnedActor = World->SpawnActor<AB2EnergyBall>(FragmentTemplateClass, SpawnTransform, SpawnParam);
		if (SpawnedActor)
		{
			float RandHeal = (TotalHealPercentage / NumFragment) * FMath::RandRange(0.8f, 1.2f);

			if (i != NumFragment - 1)
				RemainHeal -= RandHeal;
			else
				RandHeal = RemainHeal;

			SpawnedActor->SetHealPercentage(RandHeal);

			if (bRandomFragmentStayTime)
			{
				if (i == 0)
					SpawnedActor->StayTime += FragmentStayTimeRandomOffset_Min;
				else if (i == NumFragment - 1)
					SpawnedActor->StayTime += FragmentStayTimeRandomOffset_Max;
				else
				{
					float StepRange = (FragmentStayTimeRandomOffset_Max - FragmentStayTimeRandomOffset_Min) / NumFragment;

					SpawnedActor->StayTime += FMath::RandRange(StepRange * i, StepRange * (i + 1));
				}
			}			

			//World->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, SpawnParam);
		}
			
	}

	Super::InteractAction();
}


void AB2EnergyBall::OnPlayerSuckedMe(class ABladeIIPlayer* InSucker)
{
	InSucker->HealingHPByPercent(HealPercentage);
}