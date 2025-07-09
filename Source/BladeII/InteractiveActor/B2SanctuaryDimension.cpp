// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2SanctuaryDimension.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2DimensionGameMode.h"

AB2SanctuaryDimension::AB2SanctuaryDimension(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume;
}

void AB2SanctuaryDimension::InteractAction()
{
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	float FirstDropYawAngle = FMath::FRandRange(0.f, 360.f);

	FActorSpawnParameters SpawnParam;
	//SpawnParam.bDeferPostSpawnInitialize = true;

	FRotator SpawnRotation(0.f, FirstDropYawAngle, 0.f);

	FTransform SpawnTransform(SpawnRotation, GetActorLocation() + SpawnEnergyBallOffset);

	auto* SpawnedActor = World->SpawnActor<AB2SantuaryDimensionBall>(FragmentTemplateClass, SpawnTransform, SpawnParam);
	if (SpawnedActor)
	{
		//World->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, SpawnParam);
	}

	Super::InteractAction();
}

void AB2SantuaryDimensionBall::OnPlayerSuckedMe(class ABladeIIPlayer* InSucker)
{
	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		switch (SanctuaryType)
		{
		case ESanctuaryType::EST_Heal:
			InSucker->HealingHPByPercent(Amount);
			break;
		case ESanctuaryType::EST_AttUp:
			DocBattle->SetCurDimensionBuffAttackCount(1);
			InSucker->AddBuff(EBuffType::Buff_IncreaseAttackDamage_Orb, TimeDuration, Amount);
			break;
		case ESanctuaryType::EST_DefUp:
			DocBattle->SetCurDimensionBuffDefenseCount(1);
			InSucker->AddBuff(EBuffType::Buff_DecreaseReceiveDamage_Orb, TimeDuration, Amount);
			break;
		}
	}
}