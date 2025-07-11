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

//���� �÷��̾�� �ִ� ȿ�� - ��� ����
void AB2TreasureChestBase::InteractAction()
{
	SetPhase(EInteractivePhaseType::InteractingPhase);
	
	auto* StageManagerBkup = CachedStageManager;
	
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM){
		B2GM->RemoveTreasureChest(this); //�� �������� CachedStageManager�� nullptr�� �ȴ�.
	}
	
	if (StageManagerBkup)
	{
		FDropItemInfo GoldCoinDropInfo;
		GoldCoinDropInfo.RewardGoldWeight = RewardGoldWeight;
		StageManagerBkup->RequestSpawnDropItem(GoldCoinDropInfo, NULL, GetActorLocation()); // SpawnDelay �� �ʿ������ �� TreasureChest �� �������� �ʴ��� ���ϰ� DelaySpawnRefActor �� �ʿ����.
	}

	EndInteract();
}

