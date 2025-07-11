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

	//// ManagedOverlapEnableType �̶� ��Ÿ ������ ���� �ش��ϴ� Event �̸� bManagedOverlapEnabled ����..
	//SetFlagByTriggerEnableTypeCommon(bManagedOverlapEnabled, ManagedOverlapEnableType,
	//	OptionalEventName, ManagedOverlapEnableEventName, ManagedOverlapDisableEventName);

	//// AB2LevelDoorBase ������ �׷��µ� Ư�� ���̽��� �÷��̾ �پ� �ִٸ� �ѹ� ���� �ñ׳��� ���� �ʿ䰡 �ְڴ�.
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

	// �⺻ BeginOverlap ����� �״�� �����ϰ� �߰���..
	if (bManagedOverlapEnabled){
		OnManagedActorBeginOverlap.Broadcast(this, OtherActor);
	}
}
void AB2TriggerBox::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// �⺻ EndOverlap ����� �״�� �����ϰ� �߰���..
	if (bManagedOverlapEnabled){
		OnManagedActorEndOverlap.Broadcast(this, OtherActor);
	}
}
