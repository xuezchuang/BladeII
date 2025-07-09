// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "BladeIIControlNetCharacter.h"
#include "B2ControlGameMode.h"



ABladeIIControlNetCharacter::ABladeIIControlNetCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowedMount = false;
}

void ABladeIIControlNetCharacter::SetHealth(float NewHealth, bool bReceivedFromHost /*= false*/)
{
	Super::SetHealth(NewHealth, bReceivedFromHost);

	// 죽이지 않는다.
	if (Health <= 0)
		Health = 1;
}

void ABladeIIControlNetCharacter::ReserveQTEEnableEndCB()
{
	// 종료 예약은 호스트만한다.
	if (GetNetStatus() == NET_MASTER)
		Super::ReserveQTEEnableEndCB();
}

void ABladeIIControlNetCharacter::OnNetReceiveQTEEnableEnd()
{
	// 부모꺼 호출해주고 나는 죽음.
	Super::OnQTEEnableEndCB();

	SetRemoteDyingSignal();
	SetActorEnableCollision(false);
	GetMovementComponent()->StopMovementImmediately();
	//Die(0, FDamageEvent(UDamageType::StaticClass()), GetController(), NULL);
}

void ABladeIIControlNetCharacter::OnQTEEnableEndCB()
{
	// 호스트가 조건(탈것 허락안했을때) 체크해서 방송. 나도 받고처리.
	if (!bAllowedMount)
	{
		AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

		if (pGM)
		{
			pGM->NetSendQTEEnableEnd();
		}
	}	
}
