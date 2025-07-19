// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square
#include "B2LevelDoorBase.h"
#include "BladeII.h"

#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "BladeIIUtil.h"
#include "BladeIIInteractiveActor.h"

// Sets default values
AB2LevelDoorBase::AB2LevelDoorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultRootComponent);

	bDoorEnabled = true;

	DoorEnableType_DEPRECATED = ELevelDoorEnableType::EDET_DoNotChange;
	NewDoorEnableType = ETriggerEnableType::ETET_DoNotChange;
	EnableDoorEventName = NAME_None;
	DisableDoorEventName = NAME_None;

	DoorOpenType = ELevelDoorOpenType::EDOT_SomebodyOverlap;
	bOnlyPlayerCanOpen = true;
	
	EndOverlapCloseTimeDelay = 1.0f;

	LastDoorOpenCharacter = NULL;
	bPendingDoorClose = false;

	bIsDoorCurrentlyOpen = false;

	DoorOpenSoundComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DoorOpenSoundComp"));
	DoorOpenSoundComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DoorOpenSoundComp->bAutoActivate = false;

	DoorCloseSoundComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DoorCloseSoundComp"));
	DoorCloseSoundComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DoorCloseSoundComp->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AB2LevelDoorBase::BeginPlay()
{
	Super::BeginPlay();

	// ��ü�� �̷� ���� �ֵ� shadow �� modulated shadow Ư���ϰ� �¹����� �״��� ���� ������.
	// Modulated shadow �� ��ü�ϴ� ������ ������ ������ �������� ���д�.
	TurnOffMeshComponentsDyamicShadowForModulated(this);
}

void AB2LevelDoorBase::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	// Deprecated �Ӽ� ����.. ���� �Ŵ� ���� ������ �ߴµ� �̹� ���� ���� ���� ���.
	if (DoorEnableType_DEPRECATED != ELevelDoorEnableType::EDET_DoNotChange && NewDoorEnableType == ETriggerEnableType::ETET_DoNotChange)
	{
		if (DoorEnableType_DEPRECATED == ELevelDoorEnableType::EDET_EnableByAnyCustomEvent){
			NewDoorEnableType = ETriggerEnableType::ETET_EnableByAnyCustomEvent;
		}
		else if (DoorEnableType_DEPRECATED == ELevelDoorEnableType::EDET_EnableByNamedCustomEvent){
			NewDoorEnableType = ETriggerEnableType::ETET_EnableByNamedCustomEvent;
		}
	}

	if (GIsEditor)
	{
		TArray<UActorComponent*> AllMeshComps;
		GetComponents(AllMeshComps);
		for (UActorComponent* ThisActorComp : AllMeshComps)
		{
			UMeshComponent* ThisMeshComp = Cast<UMeshComponent>(ThisActorComp);
			if (ThisMeshComp)
			{
				// �̰� �ѽ������� ���� �ٽ� ���õǵ��� �ϴ� ��.. ������ �� �־.. �뷫 2018�� ���̸� ���ֵ� �� ��.
				ThisMeshComp->bCastDynamicShadow = true;
			}
		}
	}
#endif
}

// Called every frame
void AB2LevelDoorBase::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

}

void AB2LevelDoorBase::NotifyActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	//Super::NotifyActorCustomEvent(OptionalEventName, OptionalNotifyingObject);
	
	// DoorEnableType �̶� ��Ÿ ������ ���� �ش��ϴ� Event �̸� bDoorEnabled ����..
	SetFlagByTriggerEnableTypeCommon(bDoorEnabled, NewDoorEnableType, OptionalEventName, EnableDoorEventName, DisableDoorEventName);

	if (bDoorEnabled && DoorOpenType == ELevelDoorOpenType::EDOT_SomebodyOverlap)
	{
		// �ణ Ư�� ���̽��� �÷��̾ �پ� �ִٸ� �ѹ� ��������.
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		ABladeIIPlayer* B2Player = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;
		if (B2Player && IsOverlappingActor(B2Player))
		{
			NativeDoorOpen(B2Player);
		}
	}
}

void AB2LevelDoorBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ABladeIICharacter* CastedCharacter = Cast<ABladeIICharacter>(OtherActor);
	ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(OtherActor);

	if (bDoorEnabled &&
		DoorOpenType == ELevelDoorOpenType::EDOT_SomebodyOverlap &&
		IsValid(CastedCharacter) && // Allow only for approach by a character, not other object (e.g. projectiles).
		(bOnlyPlayerCanOpen == false || CastedPlayer != NULL))
	{
		NativeDoorOpen(CastedCharacter);
	}
}

void AB2LevelDoorBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	ABladeIICharacter* CastedCharacter = Cast<ABladeIICharacter>(OtherActor);
	ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(OtherActor);
		
	if (
		// No check for EDOT_SomebodyOverlap. I guess even if a door can be open by other conditions, it can be closed by EndOverlap.
		// Only somebody who can open this door, can close this door.
		CastedCharacter && (bOnlyPlayerCanOpen == false || CastedPlayer != NULL) &&
		// Additionally, check it to be closed only by somebody opened the door most recently. (unless it is not available anymore)
		(LastDoorOpenCharacter == NULL || LastDoorOpenCharacter->IsAlive() == false || CastedCharacter == LastDoorOpenCharacter)
		)
	{
		// Close the door regardless of bDoorEnabled condition.
		// This time, though, we set a timer to handle delayed door close event.
		
		bPendingDoorClose = true; // If set to false before the DoorClose is called, DoorClose will be canceled.
		LastDoorOpenCharacter = NULL; // �ָ��ϱ�.

		if (EndOverlapCloseTimeDelay > 0.0f)
		{
			GetWorldTimerManager().SetTimer(DoorCloseTimerHandle, this, &AB2LevelDoorBase::NativeDoorClose, EndOverlapCloseTimeDelay, false);
		}
		else
		{
			NativeDoorClose();
		}		
	}
}

void AB2LevelDoorBase::NativeDoorOpen(ABladeIICharacter* OpeningCharacter)
{
	LastDoorOpenCharacter = OpeningCharacter;

	bPendingDoorClose = false; // If waiting for close event, cancel it.

	if (bIsDoorCurrentlyOpen == false)
	{
		PlayDoorOpenSound(); // �̰� ������ ���� �� �ѹ��� �÷��� �ǵ���.
	}
	bIsDoorCurrentlyOpen = true;

	// Send door open event, then the subclass's Blueprint is responsible for implementing the actual door open operation.
	DoorOpen();
}

void AB2LevelDoorBase::NativeDoorClose()
{
	GetWorldTimerManager().ClearTimer(DoorCloseTimerHandle); // Ÿ�̸� Ÿ�� ������ �𸣴� �� ����.

	if (bPendingDoorClose == true) // Could be set to false while waiting.
	{
		bIsDoorCurrentlyOpen = false;
		PlayDoorCloseSound(); // �߰� ������ �ʿ���� ��?

		// Probably don't have to see any condition here..?
		DoorClose();

		bPendingDoorClose = false;
	}
}

void AB2LevelDoorBase::PlayDoorOpenSound()
{
	if (DoorOpenSoundComp && DoorOpenSoundComp->Sound)
	{
		DoorOpenSoundComp->Play();
	}
}

void AB2LevelDoorBase::PlayDoorCloseSound()
{
	if (DoorCloseSoundComp && DoorCloseSoundComp->Sound)
	{
		DoorCloseSoundComp->Play();
	}
}