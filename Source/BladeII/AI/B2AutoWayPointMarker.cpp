// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#include "BladeII.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/ShapeComponent.h"
#include "BladeIIPlayer.h"
#include "B2AutoWayPoint.h"
#include "B2AutoWayPointMarker.h"
#include "B2AutoAIController.h"
#include "B2MonsterSpawnPool.h"
#include "B2StageManager.h"
#include "BladeIIGameMode.h"

AB2AutoWayPointMarker* AB2AutoWayPointMarker::Instance = nullptr;
AB2AutoWayPointMarker* AB2AutoWayPointMarker::GetInstance(class UWorld* World)
{
	if (Instance == nullptr || Instance->GetClass() == nullptr)
	{
		FActorSpawnParameters SpawnInfo;
		Instance = Cast<AB2AutoWayPointMarker>(World->SpawnActor(AB2AutoWayPointMarker::StaticClass(), &FTransform::Identity, SpawnInfo));
	}
	BII_CHECK(Instance);
	return Instance;
}


// Sets default values
AB2AutoWayPointMarker::AB2AutoWayPointMarker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// This can check player overlap for EAWPPC_Overlap, as well as being the root component.
	DefaultCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DefaultCollisionComponent"));
	SetRootComponent(DefaultCollisionComponent);
	DefaultCollisionComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	DefaultCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DefaultCollisionComponent->SetCollisionProfileName(CollisionProfileName_OverlapMostDynamic);
	//DefaultCollisionComponent->bGenerateOverlapEvents = true;
	Instance = nullptr;
}

void AB2AutoWayPointMarker::MoveMarker(const FTransform& trm)
{
	DefaultCollisionComponent->SetWorldTransform(trm);
	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AB2AutoWayPointMarker::DelegateMoveMarker);
	//DrawDebugBox(GetWorld(), GetActorLocation(), DefaultCollisionComponent->GetUnscaledBoxExtent(), FColor::Green, true, 3);
}

void AB2AutoWayPointMarker::DelegateMoveMarker()
{
	//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	//DefaultCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//DefaultCollisionComponent->bGenerateOverlapEvents = true;
}

void AB2AutoWayPointMarker::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);

	if (!B2Player)
		return;

	//DefaultCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//DefaultCollisionComponent->bGenerateOverlapEvents = false;
	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AB2AutoWayPointMarker::DelegateBeginOverlap);
}

void AB2AutoWayPointMarker::DelegateBeginOverlap()
{
	//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


void AB2AutoWayPointMarker::BeginPlay()
{
	Super::BeginPlay();
}

void AB2AutoWayPointMarker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Instance)
	{
		Instance->ConditionalBeginDestroy();
		Instance = nullptr;
	}
}
