// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Engine/TriggerBox.h"
#include "CommonStruct.h"
#include "B2TriggerBox.generated.h"

/**
 * Extended to be working with various BladeII stage objects, such as MonsterSpawnPool.
 */
UCLASS()
class BLADEII_API AB2TriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
protected:

	/** Set if this trigger can do its special managed overlap notification by game event context.
	 * This is meant to be set by various event situation, like wave completion of MonsterSpawnPool.
	 * Setting the default property of this flag is also up to its event usage.. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Managed Overlap")
	bool bManagedOverlapEnabled;

	/** By which kind of event the special managed overlap notification can be enabled.. or be enabled at the first moment.. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Managed Overlap")
	ETriggerEnableType ManagedOverlapEnableType;

	/**
	 * When EnableType is ETET_EnableByNamedCustomEvent,
	 * bManagedOverlapEnabled can be set to true only when the ActorCustomEvent comes with matching OptionalEventName.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII Managed Overlap")
	FName ManagedOverlapEnableEventName;

	/**
	 * When EnableType is ETET_EnableByNamedCustomEvent,
	 * bManagedOverlapEnabled can be set to false only when the ActorCustomEvent comes with matching OptionalEventName.
	 * bManagedOverlapEnabled cannot be set to false if EnableType is ETET_EnableByAnyCustomEvent.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII Managed Overlap")
	FName ManagedOverlapDisableEventName;

	/**
	 * Activated when overlapped by some actor while bManagedOverlapEnabled is true.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Collision")
	FActorBeginOverlapSignature OnManagedActorBeginOverlap;
	UPROPERTY(BlueprintAssignable, Category = "Collision")
	FActorBeginOverlapSignature OnManagedActorEndOverlap;

public:
	AB2TriggerBox(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyActorCustomEvent(FName OptionalEventName = NAME_None, UObject* OptionalNotifyingObject = NULL) /*override*/;

	// Overridden to call OnManagedActorBegin/EndOverlap
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
