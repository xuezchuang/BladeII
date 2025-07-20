// Fill out your copyright notice in the Description page of Project Settings.


#include "B2ResurrectionZone.h"

#include "BladeIINetPlayer.h"
#include "B2NetGameMode.h"
//#include "B2UIManager_InGameCombat.h"

// Sets default values
AB2ResurrectionZone::AB2ResurrectionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HelpResurrectionTime = 0.f;
	MaxHelpResurrectionTime = 3.0f;
	IsOverlappingActors = false;
	AleadyResurrection = false;
	ResurrectionHelpers.Empty();

	InjectedGameMode = nullptr;
}

void AB2ResurrectionZone::BeginPlay()
{
	Super::BeginPlay();

	auto* pGM = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(this));
	BII_CHECK(pGM);

	InjectedGameMode = pGM;
}

void AB2ResurrectionZone::BeginDestroy()
{
	if (InjectedGameMode != nullptr)
	{
		if (InjectedGameMode->GetLocalController() != nullptr)
		{
			if (ABladeIINetPlayer* MyPlayer = Cast<ABladeIINetPlayer>(InjectedGameMode->GetLocalController()->GetPawn()))
			{
				for (auto Hepler : ResurrectionHelpers)
				{
					if (Hepler->GetNetId() != MyPlayer->GetNetId())
					{
						LastResurrectionHelper = Hepler;
						break;
					}
				}
			}
		}
	}

	Super::BeginDestroy();
}

// Called every frame
void AB2ResurrectionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckOverlap(DeltaTime);
}

void AB2ResurrectionZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	auto* ZoneOwner = GetOwner();
	if (ZoneOwner != nullptr && ZoneOwner != OtherActor)
	{
		if (ABladeIINetPlayer* NetPlayer = Cast<ABladeIINetPlayer>(OtherActor))
		{
			ResurrectionHelpers.Add(NetPlayer);
			StartHelpTime();
		}
	}
}

void AB2ResurrectionZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (ABladeIINetPlayer* NetPlayer = Cast<ABladeIINetPlayer>(OtherActor))
	{
		if (ABladeIINetPlayer* MyPlayer = Cast<ABladeIINetPlayer>(InjectedGameMode->GetLocalController()->GetPawn()))
		{
			if (NetPlayer->GetNetId() != MyPlayer->GetNetId())
				LastResurrectionHelper = NetPlayer;
		}

		if (ResurrectionHelpers.Contains(NetPlayer))
			ResurrectionHelpers.Remove(NetPlayer);
	}	

	if (ResurrectionHelpers.Num() == 0)
		StopHelpTime();
}

AB2ResurrectionZone* AB2ResurrectionZone::SpawnHelpAreaActor(TSubclassOf<AB2ResurrectionZone> Class, ABladeIICharacter* Owner)
{
	if (Owner == nullptr)
		return nullptr;

	UWorld* World = Owner->GetWorld();
	if (World == nullptr)
		return nullptr;
	
	auto* OwnerMesh = Owner->GetMesh();	
	if (OwnerMesh == nullptr)
		return nullptr;

	// 泅犁 某腐磐 困摹俊 Z蔼篮 顶栏肺 悸泼
	FVector  OwnerLocation (Owner->GetActorLocation());
	OwnerLocation.Z = OwnerMesh->GetBoneLocation(B2PLAYER_ROOTBONE_NAME).Z;
	FTransform SpawnTransform(FRotator::ZeroRotator, OwnerLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.bDeferConstruction = true;
	if (AB2ResurrectionZone* HelpAreaActor = Cast<AB2ResurrectionZone>(UGameplayStatics::BeginDeferredActorSpawnFromClass(Owner, Class, SpawnTransform)))
	{
		HelpAreaActor->SetOwner(Owner);
		HelpAreaActor->SetInstigator(Owner->GetInstigator());
		HelpAreaActor->ReregisterAllComponents();
		HelpAreaActor->FinishSpawning(SpawnTransform);

		return HelpAreaActor;
	}	

	return nullptr;
}

void AB2ResurrectionZone::SetMaxResurrectionTime(float InMaxResurrectionTime /*= 3.f*/)
{
	MaxHelpResurrectionTime = InMaxResurrectionTime;
}

float AB2ResurrectionZone::GetResurrectionTime()
{
	if (IsOverlappingActors == false)
		return 0.0f;

	return HelpResurrectionTime;
}

float AB2ResurrectionZone::GetResurrectionTimeRate()
{
	if (IsOverlappingActors == false || MaxHelpResurrectionTime == 0.f)
		return 0.0f;

	return FMath::Clamp(((float)HelpResurrectionTime / MaxHelpResurrectionTime), 0.0f, 1.0f);
}

void AB2ResurrectionZone::CheckOverlap(float DeltaTime)
{
	if (!IsOverlappingActors)
		return;

	HelpResurrectionTime += DeltaTime;
	if (MaxHelpResurrectionTime > HelpResurrectionTime)
		return;

	ExecuteResurrection();
	StopHelpTime();
}

void AB2ResurrectionZone::StartHelpTime()
{
	if (IsOverlappingActors)
		return;

	IsOverlappingActors = true;
	HelpResurrectionTime = 0.f;
}

void AB2ResurrectionZone::StopHelpTime()
{
	IsOverlappingActors = false;
	HelpResurrectionTime = 0.f;
}

void AB2ResurrectionZone::ExecuteResurrection()
{
	if (AleadyResurrection)
		return;

	AleadyResurrection = true;

	auto* LocalPlayer = Cast<ABladeIIPlayer>(InjectedGameMode->GetLocalController()->GetPawn());

	if (LocalPlayer == GetOwner())
	{
		InjectedGameMode->ForceResurrectLocalPlayer();
	
		if(LastResurrectionHelper != nullptr)	
			InjectedGameMode->SendResurrectHelper(LastResurrectionHelper);
	}
}

