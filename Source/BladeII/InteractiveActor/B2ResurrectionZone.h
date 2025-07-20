// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "GameFramework/Actor.h"
#include "BladeIICharacter.h"
#include "B2ResurrectionZone.generated.h"

class ABladeIINetPlayer;
class AB2NetGameMode;

UCLASS()
class BLADEII_API AB2ResurrectionZone : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AB2ResurrectionZone();

	// AActor interfaces...
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	static AB2ResurrectionZone* SpawnHelpAreaActor(TSubclassOf<AB2ResurrectionZone> Class, ABladeIICharacter* Owner);

public:
	void SetMaxResurrectionTime(float InMaxResurrectionTime = 3.f);

	float GetResurrectionTime();
	float GetResurrectionTimeRate();

	void CheckOverlap(float DeltaTime);
	void StartHelpTime();
	void StopHelpTime();
	void ExecuteResurrection();

	bool							AleadyResurrection;
	bool							IsOverlappingActors;
	float							HelpResurrectionTime;
	float							MaxHelpResurrectionTime;

	AB2NetGameMode*					InjectedGameMode;

	UPROPERTY(Transient)
	TArray<ABladeIINetPlayer*>		ResurrectionHelpers;
	ABladeIINetPlayer*				LastResurrectionHelper;
};
