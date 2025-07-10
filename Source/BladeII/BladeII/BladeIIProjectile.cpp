// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeIIProjectile.h"

#include "BladeIICharacter.h"
#include "BladeIIGameMode.h"

ABladeIIProjectile::ABladeIIProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/*CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bCheckAsyncSceneOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);
	RootComponent = CollisionComp;

	MovementComp = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;

	bIsPenetrating = false;

	ParticleComp = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleComp"));
	ParticleComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;

	DamageInfo.WeaponType = EDamageWeapon::EDW_Hammer;
	DamageTypeClass = UDamageType::StaticClass();*/
}

void ABladeIIProjectile::PostInitProperties()
{
	Super::PostInitProperties();

	const uint32 DamageInfoHash = DamageInfo.CalculateDamageHash(this);
	FDamageInfoContainer::GetContainer().RegisterDamageInfo(DamageInfoHash, DamageInfo);
}

void ABladeIIProjectile::InitProjectile(const FVector& ShootDirection, float Damage, AActor* Target, float ShootSpeed, float MaxLifeTime, bool bPenetrate, const FDamageInfo* InDamageInfo, float InFreezeDuration)
{
	BII_CHECK(MovementComp && CollisionComp);

	//if (ShootSpeed > 0.f)
	//{
	//	MovementComp->InitialSpeed = FMath::Min(MovementComp->MaxSpeed, ShootSpeed);
	//}

	//MovementComp->bInitialVelocityInLocalSpace = false;
	//MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	//MovementComp->OnProjectileStop.AddDynamic(this, &ABladeIIProjectile::OnImpact);
	//CollisionComp->MoveIgnoreActors.Add(Instigator);

	//bIsPenetrating = bPenetrate;

	//CollisionComp->bGenerateOverlapEvents = true;
	////CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	//// TODO: Need to organize Physics channel
	//CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//CollisionComp->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	//CollisionComp->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	////CollisionComp->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	////CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABladeIIProjectile::OnTouch);

	//ShootDir = ShootDirection;
	//ImpactDamage = Damage;
	//FreezeDuration = InFreezeDuration;

	//if (MaxLifeTime > 0.f)
	//{
	//	MaxLifeTime = FMath::Min(5.f, MaxLifeTime);
	//	SetLifeSpan(MaxLifeTime);
	//}
	//else if (InitialLifeSpan <= 0.f) // No default max time, just use const 5 sec 
	//{
	//	SetLifeSpan(5.f);
	//}

	//MyController = GetInstigatorController();	

	////ParticleComp->ActivateSystem();
	//
	//if (Target)
	//{
	//	MovementComp->bIsHomingProjectile = true;
	//	MovementComp->HomingTargetComponent = Target->GetRootComponent();
	//}

	//DamageInfo.SetDamageInfo(InDamageInfo, Cast<ABladeIICharacter>(GetOwner()));
}

void ABladeIIProjectile::OnImpact(FHitResult const& HitResult)
{
	//UClass* ValidClass = (DamageTypeClass != NULL) ? (UClass*)DamageTypeClass : (UClass*)UDamageType::StaticClass();

	//// 적아니면 리턴
	//ABladeIICharacter* pChar = Cast<ABladeIICharacter>(HitResult.Actor.Get());
	//if (pChar && !pChar->IsEnemy(this))
	//{
	//	return;
	//}

	//if (HitResult.Actor.Get() && HitResult.Actor.Get()->bCanBeDamaged)
	//{
	//	UGameplayStatics::ApplyPointDamage(HitResult.Actor.Get(), ImpactDamage, ShootDir, HitResult, NULL, this, ValidClass, FreezeDuration);
	//}

	//ShowImpactEffects(HitResult);
	//DisableAndDestroy();
}

void ABladeIIProjectile::OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (Other == Instigator)
	//{
	//	return;
	//}

	//// 적아니면 리턴
	//ABladeIICharacter* pChar = Cast<ABladeIICharacter>(Other);
	//if (pChar && !pChar->IsEnemy(this))
	//{
	//	return;
	//}

	//UClass* ValidClass = (DamageTypeClass != NULL) ? (UClass*)DamageTypeClass : (UClass*)UDamageType::StaticClass();

	//if (Other && Other->bCanBeDamaged)
	//{
	//	UGameplayStatics::ApplyPointDamage(Other, ImpactDamage, ShootDir, SweepResult, NULL, this, ValidClass, FreezeDuration);
	//	ShowImpactEffects(SweepResult);
	//}

	//if (!bIsPenetrating || Cast<ABladeIICharacter>(Other) == NULL)
	//{
	//	DisableAndDestroy();
	//}
}

void ABladeIIProjectile::ShowImpactEffects(FHitResult const& HitResult)
{
	//if (ImpactFX)
	//{
	//	FVector TargetLocation = GetActorLocation();
	//	ACharacter* Char = Cast<ACharacter>(HitResult.Actor.Get());
	//	if (Char && Char->GetCharacterMovement())
	//	{
	//		TargetLocation = Char->GetCharacterMovement()->GetActorFeetLocation() + CharCollisionOffset;
	//		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, TargetLocation, FRotator(0.f, 0.f, 0.f));
	//	}
	//	else
	//	{
	//		if (bUseImpactNormal)
	//		{
	//			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, TargetLocation, HitResult.ImpactNormal.Rotation() + FRotator(-90.f, 0.f, 0.f));
	//		}
	//		else
	//		{
	//			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, TargetLocation, GetActorRotation());
	//		}
	//	}
	//}
}

void ABladeIIProjectile::DisableAndDestroy()
{
	//SetActorHiddenInGame(!bEnableRenderAfterCollision);
	//SetActorEnableCollision(false);
	//MovementComp->StopMovementImmediately();

	//ParticleComp->DeactivateSystem();

	//// stop replication and give clients some time to show explosion
	//bTearOff = true;
	//SetLifeSpan(1.0f);
}

void ABladeIIProjectile::TornOff()
{
	Super::TornOff();

	//FVector ProjDirection = GetActorRotation().Vector();
	//const FVector StartTrace = GetActorLocation() - ProjDirection * 10.f;
	//const FVector EndTrace = GetActorLocation() + ProjDirection * 150.f;

	//FHitResult Impact;
	//if (!GetWorld()->LineTraceSingleByChannel(Impact, StartTrace, EndTrace, COLLISION_PROJECTILE, FCollisionQueryParams(TEXT("ProjClient"), true, this)))
	//{
	//	// failsafe
	//	Impact.ImpactPoint = GetActorLocation();
	//	Impact.ImpactNormal = -ProjDirection;
	//	Impact.Normal = -ProjDirection;
	//}

	//ShowImpactEffects(Impact);
	//Destroy();
}

void ABladeIIProjectile::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	if (MovementComp)
	{
		MovementComp->Velocity = NewVelocity;
	}
}

const void* ABladeIIProjectile::GetCustomDamageInfo()
{
	return &DamageInfo;
}