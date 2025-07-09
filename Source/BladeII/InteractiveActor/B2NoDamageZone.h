// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "B2NoDamageZone.generated.h"

UCLASS()
class BLADEII_API AB2NoDamageZone : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CollisionComponents;

public:	
	// Sets default values for this actor's properties
	AB2NoDamageZone();

	// AActor interfaces...
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void CheckNoDamageFX(class ABladeIIPlayer* TargetPlayer, bool isOverlap);

private:
	void CheckOverlap();

protected:
	// Loop OK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystem* NoDamageFX_PS;

	// No Loop
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DecoFX)
	class UParticleSystem* NoDamageFX_PS_End;

	TArray<class UParticleSystemComponent*> NoDamageFX_Components;
	
};
