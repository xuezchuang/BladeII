// Fill out your copyright notice in the Description page of Project Settings.


#include "B2TreasureChestBase.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "BladeIICharacter.h"

// Sets default values
AB2TreasureChestBase::AB2TreasureChestBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick	= false;
	TriggerType						= EInteractiveTriggeringType::InteractByTriggerVolume;
	
	RewardGoldWeight				= 1;	
	CachedStageManager				= NULL;
}

void AB2TreasureChestBase::BeginDestroy()
{
	Super::BeginDestroy();

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM){
		B2GM->RemoveTreasureChest(this);
	}	
}

//실제 플레이어에게 주는 효과 - 골드 스폰
void AB2TreasureChestBase::InteractAction()
{
	SetPhase(EInteractivePhaseType::InteractingPhase);
	
	auto* StageManagerBkup = CachedStageManager;
	
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM){
		B2GM->RemoveTreasureChest(this); //이 시점에서 CachedStageManager는 nullptr이 된다.
	}
	
	if (StageManagerBkup)
	{
		FDropItemInfo GoldCoinDropInfo;
		GoldCoinDropInfo.RewardGoldWeight = RewardGoldWeight;
		StageManagerBkup->RequestSpawnDropItem(GoldCoinDropInfo, NULL, GetActorLocation()); // SpawnDelay 가 필요없든지 이 TreasureChest 가 움직이지 않던지 여하간 DelaySpawnRefActor 는 필요없음.
	}

	EndInteract();
}

