// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "BladeIIInteractiveActor.h"
#include "B2DestructibleLevelObjBase.generated.h"

USTRUCT(BlueprintType)
struct BLADEII_API FDestructParam
{
	GENERATED_USTRUCT_BODY()

public:
	/** How long will it take to disappear after broken? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PostDestructLifeSpan;

	/** Adjust the force size which will be given when it is destroyed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DestructForceMultiplier;

	/** Adjust the colliding force additionally. Basically derived from mass and speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionForceMultiplier;

	/** The maximum limit on the acceleration cause by the destruction impulse, considering the mass. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDestructImpulseAcc;

	FDestructParam();
};
/**
 * The native base for destructible level object, assuming skeletalmesh with physics asset.
 */
UCLASS(hidecategories = (Object, Replication, Input))
class BLADEII_API AB2DestructibleLevelObjBase : public ASkeletalMeshInteractiveActor
{
	GENERATED_BODY()	

public:

	AB2DestructibleLevelObjBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	FDestructParam DestructParameter;

private:
		
	FVector DestructImpulse;
	FVector DestructHitLocation;
public:

	virtual void BeginPlay() override;
	
	/** Requires blocking collision to get this called */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** Requires dynamic collision to get this called */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	/** To implement additional zero HP event handling from the Blueprint. */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void ExtraPostDestruction(FVector InDestructImpulse);

	inline bool DestructibleBySingleDamageHit() { return (TriggerType == EInteractiveTriggeringType::InteractByDamaged && MaxHP <= 0.0f); }
	inline bool DestructibleBySingleCollisionHit() { return (TriggerType == EInteractiveTriggeringType::InteractByHit && MaxHP <= 0.0f); }

protected:
	virtual void Interact(class AActor* InInteractingActor) override;
	virtual void BeginInteract(class AActor* InInteractingActor) override;
	virtual void EndInteract() override;
	virtual void InteractAction() override;

	virtual bool IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType) override;

private:
	FVector CalcDestructResult(const FVector& Direction, float Speed, float BoundsRadius);
	inline bool DoUsePlayDestructAnim(){ return InteractingAnim != nullptr; }

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
