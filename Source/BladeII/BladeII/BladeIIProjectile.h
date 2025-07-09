// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BladeIIDamageType.h"
#include "Components/SphereComponent.h"
#include "BladeIIProjectile.generated.h"

UCLASS(Abstract, Blueprintable)
class BLADEII_API ABladeIIProjectile : public AActor
{
	GENERATED_UCLASS_BODY()


	virtual void PostInitProperties() override;

	/** initialize projectile */
	void InitProjectile(const FVector& ShootDirection, float Damage, AActor* Target = NULL, float ShootSpeed = 0.f, float MaxLifeTime = 0.f, bool bPenetrate = false, const FDamageInfo* DamageInfo = NULL, float InFreezeDuration = 0.f);

	/** handle hit */
	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

	UFUNCTION()
	void OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	FVector CharCollisionOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bEnableRenderAfterCollision : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bUseImpactNormal : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bUseCharFootLocation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bIsPenetrating : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	FDamageInfo DamageInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	TSubclassOf<class UDamageType> DamageTypeClass;

protected:
	/** impact effect for projectile */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	class UParticleSystem *ImpactFX;

private:
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleComp;
protected:

	/** controller that fired me (cache for damage calculations) */
	TWeakObjectPtr<AController> MyController;

	/** damage data passed from weapon */
	int32 ImpactDamage;

	/** cached direction of shot */
	FVector ShootDir;

	/** shutdown projectile and prepare for destruction */
	void DisableAndDestroy();

	/** show effects on hit */
	void ShowImpactEffects(FHitResult const& HitResult);

	/** replication finished, show impacts */
	virtual void TornOff() override;

	/** update velocity on client */
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

	float FreezeDuration;

public:
	/** Returns MovementComp subobject **/
	FORCEINLINE UProjectileMovementComponent* GetMovementComp() const { return MovementComp; }
	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }

	virtual const void* GetCustomDamageInfo() /*override*/;
};
