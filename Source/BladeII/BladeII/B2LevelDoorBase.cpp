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

	// 대체로 이런 종류 애들 shadow 가 modulated shadow 특성하고 맞물려서 그다지 보기 안좋음.
	// Modulated shadow 를 대체하는 괜찮은 수단이 나오기 전까지는 꺼둔다.
	TurnOffMeshComponentsDyamicShadowForModulated(this);
}

void AB2LevelDoorBase::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	// Deprecated 속성 전달.. 이전 거는 뭔가 설정을 했는데 이번 꺼는 설정 안한 경우.
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
				// 이건 한시적으로 값이 다시 세팅되도록 하는 거.. 삽질한 게 있어서.. 대략 2018년 쯤이면 없애도 될 듯.
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
	
	// DoorEnableType 이랑 기타 설정에 따라 해당하는 Event 이면 bDoorEnabled 세팅..
	SetFlagByTriggerEnableTypeCommon(bDoorEnabled, NewDoorEnableType, OptionalEventName, EnableDoorEventName, DisableDoorEventName);

	if (bDoorEnabled && DoorOpenType == ELevelDoorOpenType::EDOT_SomebodyOverlap)
	{
		// 약간 특수 케이스로 플레이어가 붙어 있다면 한번 열어주자.
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
		LastDoorOpenCharacter = NULL; // 애매하군.

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
		PlayDoorOpenSound(); // 이건 닫혔다 열릴 때 한번만 플레이 되도록.
	}
	bIsDoorCurrentlyOpen = true;

	// Send door open event, then the subclass's Blueprint is responsible for implementing the actual door open operation.
	DoorOpen();
}

void AB2LevelDoorBase::NativeDoorClose()
{
	GetWorldTimerManager().ClearTimer(DoorCloseTimerHandle); // 타이머 타고 왔을지 모르니 꺼 놓음.

	if (bPendingDoorClose == true) // Could be set to false while waiting.
	{
		bIsDoorCurrentlyOpen = false;
		PlayDoorCloseSound(); // 추가 조건은 필요없을 듯?

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