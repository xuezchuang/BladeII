// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2DestructibleLevelObjBase.h"
#include "BladeIIPlayer.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "BladeIIGameMode.h"
#include "Engine/DamageEvents.h"
#include "BladeIIUtil.h"

FDestructParam::FDestructParam()
{
	PostDestructLifeSpan = 5.0f;
	DestructForceMultiplier = 1000.0f;

	CollisionForceMultiplier = 2.0f;

	MaxDestructImpulseAcc = 100.0f;
}

AB2DestructibleLevelObjBase::AB2DestructibleLevelObjBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	USkeletalMeshComponent* SKMeshComp = GetSkeletalMeshComponent();
	// Requires dynamic property for taking damage, and requires blocking property for collision.
	// As a default, BlockAllDynamic can satisfy both..
	if (SKMeshComp)
	{
		SKMeshComp->SetCollisionProfileName(TEXT("Destructible"));
		//SKMeshComp->bGenerateOverlapEvents = true;
		SKMeshComp->SetCanEverAffectNavigation(false);
	}

	InteractingTimeOverride = 0.12f;
}

void AB2DestructibleLevelObjBase::BeginPlay()
{
	Super::BeginPlay();	
		
	USkeletalMeshComponent* SKMeshComp = GetSkeletalMeshComponent();
	if (SKMeshComp)
	{			
		if (!DoUsePlayDestructAnim())
		{
			SKMeshComp->SetSimulatePhysics(false);

			// Sleep for now, and let it wake up in the event situation.
			SKMeshComp->PutAllRigidBodiesToSleep();
		}
		//강제 설정.
		SKMeshComp->SetCollisionProfileName(TEXT("Destructible"));
		//SKMeshComp->bGenerateOverlapEvents = true;
	}	

	CurrentHP = MaxHP;
}

bool AB2DestructibleLevelObjBase::IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType)
{
	if (GetPhase() != EInteractivePhaseType::ReadyPhase)
		return false;

	if (bOnlyPlayerCanInteract && !((Other->IsA(ABladeIIProjectile::StaticClass()) && Cast<ABladeIIPlayer>(Other->GetInstigator()) != NULL) || Cast<ABladeIIPlayer>(Other) != NULL))
	{
		return false;
	}

	if (TriggerType != InTriggerType)
	{
		return TriggerType == EInteractiveTriggeringType::InteractByHit ? InTriggerType == EInteractiveTriggeringType::InteractByDamaged : false;
	}

	return true;
}


void AB2DestructibleLevelObjBase::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	DestructImpulse = CalcDestructResult(HitNormal, OtherComp->GetOwner()->GetVelocity().Size(), OtherComp->Bounds.SphereRadius);
	DestructHitLocation = HitLocation;
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

float AB2DestructibleLevelObjBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	//DestructImpulse = DestructImpulse.IsNearlyZero() ? DamageCauser->GetActorForwardVector() * Damage* DestructParameter.DestructForceMultiplier * DamageEvent.ImpulseScale : DestructImpulse;

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AB2DestructibleLevelObjBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	DestructImpulse = CalcDestructResult(OtherActor->GetVelocity().GetSafeNormal(), OtherActor->GetVelocity().Size(), FBoxSphereBounds(OtherActor->GetComponentsBoundingBox()).SphereRadius);

	Super::NotifyActorBeginOverlap(OtherActor);
}

FVector AB2DestructibleLevelObjBase::CalcDestructResult(const FVector& Direction, float Speed, float BoundsRadius)
{
	// Give some hard coded scale and we also give additional configurable parameter.
	float ScaledOtherSpeed = Speed * 0.01f;
	// Use the bound size as mass, because mass can be overridden for other reasons, and not likely to reflect the colliding component's reasonable mass.
	// In addition, Character's SimulatePhysics is false, so GetMass generates warning.
	// It can be changed though.
	float SimpleDerivedMass = BoundsRadius * 0.01f;

	// Let's use momentum, instead of energy. This will be used to derive DestructImpulse below.
	float ImpactMomentum = ScaledOtherSpeed * SimpleDerivedMass * DestructParameter.CollisionForceMultiplier;

	return Direction * ImpactMomentum * DestructParameter.DestructForceMultiplier;
}

void AB2DestructibleLevelObjBase::Interact(class AActor* InInteractingActor)
{
	if (TriggerType == EInteractiveTriggeringType::InteractByHit)
	{
		//음.. BaseClass를 바꿔야 하나..
		float CalcDamageSquared = (DestructImpulse / DestructParameter.DestructForceMultiplier).SizeSquared();
		if (MaxHP * MaxHP > CalcDamageSquared)
			return;
	}

	BeginInteract(InInteractingActor);
}

void AB2DestructibleLevelObjBase::BeginInteract(class AActor* InInteractingActor)
{
	SetPhase(EInteractivePhaseType::InteractingPhase);

	USkeletalMeshComponent* SKMeshComp = GetSkeletalMeshComponent();
	if (SKMeshComp)
	{
		if (!DoUsePlayDestructAnim())
		{
			if (SKMeshComp->IsSimulatingPhysics() == false)
			{
				SKMeshComp->SetSimulatePhysics(true);
			}

			SKMeshComp->PutAllRigidBodiesToSleep(); // Not just yet..
		}		
		else
			SKMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM) {
		B2GM->RemoveDestructibleObject(this);
	}
	
	Super::BeginInteract(InInteractingActor);
}

void AB2DestructibleLevelObjBase::InteractAction()
{
	USkeletalMeshComponent* SKMeshComp = GetSkeletalMeshComponent();
	if (SKMeshComp)
	{
		if (!DoUsePlayDestructAnim())
		{
			SKMeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
			SKMeshComp->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Ignore);
			// Physical destruction.

			// Could be false depend on the configuration.
			if (SKMeshComp->IsSimulatingPhysics() == false)
			{
				SKMeshComp->SetSimulatePhysics(true);
			}

			SKMeshComp->WakeAllRigidBodies();

			FVector ImpulseDir = DestructImpulse;
			ImpulseDir.Normalize();
			float ImpulseSize = DestructImpulse.Size();
			// SKMeshComp is simulating physics at least now, so no problem with getting mass..
			float SKMeshMass = FMath::Max(SKMeshComp->GetMass(), 1.0f);

			// ImpulseSize limit. (divide by mass to get the acceleration)
			ImpulseSize = FMath::Min(ImpulseSize / SKMeshMass, DestructParameter.MaxDestructImpulseAcc) * SKMeshMass;

			SKMeshComp->AddRadialImpulse(DestructHitLocation.IsNearlyZero() ? SKMeshComp->GetComponentLocation() : DestructHitLocation, SKMeshComp->Bounds.SphereRadius, ImpulseSize, ERadialImpulseFalloff::RIF_Linear);
		}		
	}	

	// Additional handling from the blueprint, if any.
	ExtraPostDestruction(DestructImpulse);

	EndInteract();
}

void AB2DestructibleLevelObjBase::EndInteract()
{
	Super::EndInteract();

	SetLifeSpan(DestructParameter.PostDestructLifeSpan);

	// PostDestructLifeSpan 딱 되었을 때 제거하는 게 좋긴 한데..
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//if (B2GM){
	//	B2GM->RemoveDestructibleObject(this);
	//}
}

#if WITH_EDITOR

void AB2DestructibleLevelObjBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_MaxHP = GET_MEMBER_NAME_CHECKED(AB2DestructibleLevelObjBase, MaxHP);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_MaxHP)
	{
		MaxHP = FMath::Max(0.0f, MaxHP);
	}
}

#endif