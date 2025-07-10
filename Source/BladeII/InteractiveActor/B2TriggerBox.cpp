// Fill out your copyright notice in the Description page of Project Settings.


#include "B2TriggerBox.h"
#include "BladeIIPlayer.h"

AB2TriggerBox::AB2TriggerBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bManagedOverlapEnabled = true;
	ManagedOverlapEnableType = ETriggerEnableType::ETET_DoNotChange;
	ManagedOverlapEnableEventName = NAME_None;
	ManagedOverlapDisableEventName = NAME_None;
}

void AB2TriggerBox::NotifyActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	//Super::NotifyActorCustomEvent(OptionalEventName, OptionalNotifyingObject);

	//bool bPrevManagedOverlapEnabled = bManagedOverlapEnabled;

	//// ManagedOverlapEnableType 이랑 기타 설정에 따라 해당하는 Event 이면 bManagedOverlapEnabled 세팅..
	//SetFlagByTriggerEnableTypeCommon(bManagedOverlapEnabled, ManagedOverlapEnableType,
	//	OptionalEventName, ManagedOverlapEnableEventName, ManagedOverlapDisableEventName);

	//// AB2LevelDoorBase 에서도 그러는데 특수 케이스로 플레이어가 붙어 있다면 한번 강제 시그널을 날릴 필요가 있겠다.
	//if (!bPrevManagedOverlapEnabled && bManagedOverlapEnabled)
	//{
	//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (B2Player && IsOverlappingActor(B2Player))
	//	{
	//		OnManagedActorBeginOverlap.Broadcast(this, B2Player);
	//	}
	//}
}

void AB2TriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// 기본 BeginOverlap 기능은 그대로 동작하고 추가로..
	if (bManagedOverlapEnabled){
		OnManagedActorBeginOverlap.Broadcast(this, OtherActor);
	}
}
void AB2TriggerBox::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// 기본 EndOverlap 기능은 그대로 동작하고 추가로..
	if (bManagedOverlapEnabled){
		OnManagedActorEndOverlap.Broadcast(this, OtherActor);
	}
}
