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
	//CachedStageManager				= NULL;
}

void AB2TreasureChestBase::BeginDestroy()
{
	Super::BeginDestroy();

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM)
	//{
	//	B2GM->RemoveTreasureChest(this);
	//}
}

//角力 敲饭捞绢俊霸 林绰 瓤苞 - 榜靛 胶迄
void AB2TreasureChestBase::InteractAction()
{
	SetPhase(EInteractivePhaseType::InteractingPhase);
	
	//auto* StageManagerBkup = CachedStageManager;
	//
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM){
	//	B2GM->RemoveTreasureChest(this); //捞 矫痢俊辑 CachedStageManager绰 nullptr捞 等促.
	//}
	//
	//if (StageManagerBkup)
	//{
	//	FDropItemInfo GoldCoinDropInfo;
	//	GoldCoinDropInfo.RewardGoldWeight = RewardGoldWeight;
	//	StageManagerBkup->RequestSpawnDropItem(GoldCoinDropInfo, NULL, GetActorLocation()); // SpawnDelay 啊 鞘夸绝电瘤 捞 TreasureChest 啊 框流捞瘤 臼带瘤 咯窍埃 DelaySpawnRefActor 绰 鞘夸绝澜.
	//}

	EndInteract();
}

