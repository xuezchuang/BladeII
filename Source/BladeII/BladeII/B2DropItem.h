// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Common/CommonStruct.h"
//#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2DropItem.generated.h"

/**
 * Shits left by a dead monster.
 * Have much common with BladeIIProjectile, as it chases player to get sucked.
 */
UCLASS(ABSTRACT)
class BLADEII_API AB2DropBase : public AActor
{
	GENERATED_UCLASS_BODY()

protected:

	/** OffsetZ from its dead character's body */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float SpawnLocationOffsetZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float SpawnMinRandomDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float SpawnMaxRandomDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float SpawningTime;
public:
	/** Time to stay at the spawn location before it starts to fly to the player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float StayTime;
protected:
	/** Time to start play sound (by AudioComp) after spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float PlaySoundDelay;

	/** Time to stay alive after the player sucked this item.
	 * During this time, It is invisible, just let it live for some while to express its glorious fucking moment of getting sucked by wicked player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	float PostSuckLifeSpan;

	/** Effect played at the moment of player sucking. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DropItem)
	class UParticleSystem* SuckingPS;

	/** Cached particle system component spawned by SuckingPS. */
	UPROPERTY(BlueprintReadOnly, Category = DropItem)
	class UParticleSystemComponent* CachedSuckingPSComp;

	/** Cached particle system component spawned by SuckingPS. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DropItem)
	float SpawnHeight;

	/** Cached particle system component spawned by SuckingPS. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DropItem)
	bool bFindFloor;

private:

	/** Use projectile movement component for player chasing. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DropItem, meta = (AllowPrivateAccess = "true"))
	class UB2DropItemMovementComponent* DropItemMoveComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DropItem, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComp;

	/** The main render body */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DropItem, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* MainBodyPS;

	/** Either dropping sound or sucking sound.. The play timing is controlled by PlaySoundDelay. Other details to be controlled by this component properties. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DropItem, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioComp;

	//class ABladeIIPlayer* m_Sucker;

	float QTEStayTime;

	FTimerHandle TimeToGetSuckedTimerHandle;
	FTimerHandle PlaySoundTimerHandle;
	FTimerHandle TimeToFinishSpawningHandle;

	uint32 bSoundPlayedAtLeastOnce : 1;

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SetQTEStayTime(float InStayTime) {
		QTEStayTime = InStayTime;
	}

	//void SetSucker(ABladeIIPlayer* pSucker);

private:
	/** Start move out to the player.. */
	void TimeToGetSuckedCB();

	/** Finish Spawing drop item */
	void TimeToFinishSpawning();

	void PlayDropItemSound();

	/** Final sucking procedure. */
	//void PlayerSucksMe(class ABladeIIPlayer* InSucker);

	void CheckPlayerSucksMe();

public:
	/** To be overridden by subclasses for their own sucking event handling purpose. */
	virtual void OnPlayerSuckedMe(class ABladeIIPlayer* InSucker);

	/** If OnPlayerSuckedMe is not enough for its Blueprint subclass.. */
	//UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	//void OnPlayerSuckedMeBP(class ABladeIIPlayer* InSucker);

protected:
	/** Returns MovementComp subobject **/
	FORCEINLINE class UB2DropItemMovementComponent* GetMovementComp() const {
		return DropItemMoveComp;
	}
	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const {
		return CollisionComp;
	}

	friend class AB2DropItem;
};

UCLASS(Blueprintable)
class BLADEII_API AB2DropItem : public AB2DropBase
{
	GENERATED_BODY()

public:
	/** Any significant item data will be transferred through this struct. Only part of this FB2Item struct is used though.
	* Temporarily public. */
	FB2Item SignificantData;

public:
	/** Spawn operation procedure of ItemInfo. Better be called before BeginPlay */
	void OverrideByItemInfo(UB2ItemInfo* WholeInfoTable, FSingleItemInfoData* InSingleInfoOfID);
	/** It is executed right after being spawned as AB2DropItem, before overridden by ItemInfo. */
	void OverrideByBaseBP(TSubclassOf<AB2DropItem> BPClassToOverride);

	/** To be overridden by subclasses for their own sucking event handling purpose. */
	//virtual void OnPlayerSuckedMe(class ABladeIIPlayer* InSucker) override;
};

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Projectile, ProjectileBounces, ProjectileSimulation, Velocity))
class BLADEII_API UB2DropItemMovementComponent : public UProjectileMovementComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Transient)
	FVector Acceleration;

	UPROPERTY(Transient)
	FVector CachedLocationTarget;

	void FinishSpawning();
	void StartHommingToSucker(class ABladeIIPlayer* Player);

	virtual FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime) const override;

	void SetSpawningLocationInfo(float SpawningTime, const FVector& LocationTarget, float HighestHeight = 0, bool bFindFloorLocation = false);

	void SetSpawningVelocityInfo(const FVector& InitialVelocity, float GravityZ);
private:
	void SetAcceleration(const FVector& InitialVelocity, float SpawningTime, const FVector& LocationTarget);

	FTimerHandle TimeToFinishSpawningTimerHandle;
};
