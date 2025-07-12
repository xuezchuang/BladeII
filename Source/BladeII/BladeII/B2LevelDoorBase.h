// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "GameFramework/Actor.h"
#include "CommonStruct.h"
#include "B2LevelDoorBase.generated.h"

////////////////////////////////////////////////////////////////////////////////
// !!!!! This is deprecated and replaced by ETriggerEnableType !!!!!
UENUM(BlueprintType)
enum class ELevelDoorEnableType : uint8
{
	// Don't get affected by any ActorCustomEvent
	EDET_DoNotChange,

	// Get to be enabled by any ActorCustomEvent. Cannot be disabled afterward.
	EDET_EnableByAnyCustomEvent,

	// Get to be enabled by a ActorCustomEvent with matching name.
	EDET_EnableByNamedCustomEvent,

	EDET_End
};
// !!!!! This is deprecated and replaced by ETriggerEnableType !!!!!
////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ELevelDoorOpenType : uint8
{
	// When some character gets nearby the door, like automatic door.
	EDOT_SomebodyOverlap,

	// Might add some more..? Guess it could be a little tricky to add other DoorOpenType.
	// I wish SomebodyOverlap is all we need to handle.

	EDOT_End
};

UCLASS(Blueprintable)
class BLADEII_API AB2LevelDoorBase : public AActor
{
	GENERATED_BODY()

protected:
	
	/** Set if this door can do its basic functionality - Open and Close.
	 * This is meant to be set by various event situation, like wave completion of MonsterSpawnPool.
	 * Setting the default property of this flag is also up to its event usage.. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	bool bDoorEnabled;
		
	UPROPERTY()
	ELevelDoorEnableType DoorEnableType_DEPRECATED;
	/** By which kind of event the door can be enabled.. or be enabled at the first moment.. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	ETriggerEnableType NewDoorEnableType;

	/** 
	 * When DoorEnableType is ETET_EnableByNamedCustomEvent,
	 * bDoorEnabled can be set to true only when the ActorCustomEvent comes with matching OptionalEventName.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	FName EnableDoorEventName;

	/**
	 * When DoorEnableType is ETET_EnableByNamedCustomEvent,
	 * bDoorEnabled can be set to false only when the ActorCustomEvent comes with matching OptionalEventName.
	 * bDoorEnabled cannot be set to false if DoorEnableType is ETET_EnableByAnyCustomEvent.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	FName DisableDoorEventName;

	/** When the door is enabled, how to open it? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	ELevelDoorOpenType DoorOpenType;

	/** Set to true if you don't want this door gets open by all the mobs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	bool bOnlyPlayerCanOpen;

	/** After how long time this door can be close after EndOverlap event? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float EndOverlapCloseTimeDelay;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BladeII, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* DoorOpenSoundComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = BladeII, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* DoorCloseSoundComp;

private:

	/** To check a bit more detailed (and possibly stable) door close condition. */
	UPROPERTY(Transient)
	class ABladeIICharacter* LastDoorOpenCharacter;

	bool bPendingDoorClose; // To prevent mixed open/close mess.. Basically true during EndOverlapCloseTimeDelay

	uint32 bIsDoorCurrentlyOpen : 1;

	FTimerHandle DoorCloseTimerHandle;

public:	
	// Sets default values for this actor's properties
	AB2LevelDoorBase(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostLoad() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyActorCustomEvent(FName OptionalEventName = NAME_None, UObject* OptionalNotifyingObject = NULL) /*override*/;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	virtual void NotifyActorEndOverlap(AActor* OtherActor);

	/** They don't actually do opening/closing work. Could be just unified interfaces to send signal to the blueprint. */
	void NativeDoorOpen(class ABladeIICharacter* OpeningCharacter);
	void NativeDoorClose();

	/** This is the event invoked when the door is about to open. Connect BP logic to actually open the door. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "DoorOpen"), Category = BladeII)
	void DoorOpen();

	/** This is the event invoked when the door is about to open. Connect BP logic to actually open the door. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "DoorOpenReverse"), Category = BladeII)
	void DoorOpenReverse();

	/** This is the event invoked when the door is about to open. Connect BP logic to actually open the door. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "DoorStop"), Category = BladeII)
	void DoorStop();

	/** This is the event invoked when the door is about to close. Connect BP logic to actually close the door. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "DoorClose"), Category = BladeII)
	void DoorClose();

	/** Close to the DoorOpen event, but not likely to be overlapped.. */
	void PlayDoorOpenSound();

	/** Close to the DoorClose event, but not likely to be overlapped.. */
	void PlayDoorCloseSound();

	UFUNCTION(BlueprintCallable, Category = BladeII)
	bool IsDoorCurrentlyOpen();
};

FORCEINLINE bool AB2LevelDoorBase::IsDoorCurrentlyOpen() { return bIsDoorCurrentlyOpen; }