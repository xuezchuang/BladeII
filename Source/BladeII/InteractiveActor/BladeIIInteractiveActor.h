// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BladeIIInteractiveActor.generated.h"

UENUM()
namespace EInteractiveTriggeringType
{
	enum Type
	{
		InteractByDamaged,
		InteractByHit,
		InteractByTriggerVolume,
		InteractBySpawn
	};
}

UENUM()
namespace EInteractivePhaseType
{
	enum Type
	{
		ReadyPhase,
		InteractingPhase,
		FinishPhase
	};
}

UCLASS(abstract)
class BLADEII_API ABladeIIInteractiveActor : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	virtual void BeginPlay() override;

	/** Requires blocking collision to get this called */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** Requires dynamic collision to get this called */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	/** The maximum HP which will be given at BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractTrigger")
	float MaxHP;

	/** The object will be broken when the CurrentHP goes below 0. */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "InteractTrigger", meta = (EditCondition = false))
	float CurrentHP;

	/** Time to play interacting-anim */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float InteractingTimeOverride;
	
	/** If true, other Mobs or NPCs cannot interact this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	uint32 bOnlyPlayerCanInteract : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InteractTrigger")
	TEnumAsByte<EInteractiveTriggeringType::Type> TriggerType;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII", meta = (DisplayName = "InteractActorBeginInteract"))
	void OnBeginInteract();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII", meta = (DisplayName = "InteractActorEndInteract"))
	void OnEndInteract();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetPhaseForce(TEnumAsByte<EInteractivePhaseType::Type> InPhase);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void RecycleInteractiveActor();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII", meta = (DisplayName = "InteractActorRecycle"))
	void OnRecycleInteractiveActor();
	
protected:	
	virtual void Interact(class AActor* InInteractingActor);

	virtual void BeginInteract(class AActor* InInteractingActor);
	virtual void EndInteract();

	virtual void InteractAction();

	FORCEINLINE TEnumAsByte<EInteractivePhaseType::Type> GetPhase() { return Phase; }
	FORCEINLINE void SetPhase(TEnumAsByte<EInteractivePhaseType::Type> InPhase) { Phase = InPhase; }

	virtual bool IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType);

private:	

	FTimerHandle EndInteractTimerHandle;
	TEnumAsByte<EInteractivePhaseType::Type> Phase;
};

UCLASS(abstract)
class BLADEII_API AContinuousInAreaInteractiveActor : public ABladeIIInteractiveActor
{
	GENERATED_UCLASS_BODY()
public:
	virtual void BeginPlay() override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	float PeriodSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float LifeSpan;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginInteract(class AActor* InInteractingActor) override;
	virtual void InteractAction() override;
	virtual void EndInteract() override;
	
private:
	FTimerHandle InteractPeriodTimerHandle;
	FTimerHandle InteractFinishTimerHandle;

	class AActor* InteractingActor;
};

UCLASS(abstract)
class BLADEII_API ASkeletalMeshInteractiveActor : public ABladeIIInteractiveActor
{
	GENERATED_UCLASS_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeII, meta = (ExposeFunctionCategories = "Mesh,Components|SkeletalMesh,Animation,Physics", AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;
public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() { return SkeletalMeshComponent; }

public:
	/** Animation asset to be used for Interacting to Player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	class UAnimationAsset* InteractingAnim;

	virtual void BeginPlay() override;

protected:
	virtual void BeginInteract(AActor* InInteractingActor) override;
};