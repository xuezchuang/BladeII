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

	//// ManagedOverlapEnableType 捞尔 扁鸥 汲沥俊 蝶扼 秦寸窍绰 Event 捞搁 bManagedOverlapEnabled 技泼..
	//SetFlagByTriggerEnableTypeCommon(bManagedOverlapEnabled, ManagedOverlapEnableType,
	//	OptionalEventName, ManagedOverlapEnableEventName, ManagedOverlapDisableEventName);

	//// AB2LevelDoorBase 俊辑档 弊矾绰单 漂荐 纳捞胶肺 敲饭捞绢啊 嘿绢 乐促搁 茄锅 碍力 矫弊澄阑 朝副 鞘夸啊 乐摆促.
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

	// 扁夯 BeginOverlap 扁瓷篮 弊措肺 悼累窍绊 眠啊肺..
	if (bManagedOverlapEnabled){
		OnManagedActorBeginOverlap.Broadcast(this, OtherActor);
	}
}
void AB2TriggerBox::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// 扁夯 EndOverlap 扁瓷篮 弊措肺 悼累窍绊 眠啊肺..
	if (bManagedOverlapEnabled){
		OnManagedActorEndOverlap.Broadcast(this, OtherActor);
	}
}
