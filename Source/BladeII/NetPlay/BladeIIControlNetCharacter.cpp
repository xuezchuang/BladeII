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

	// ������ �ʴ´�.
	if (Health <= 0)
		Health = 1;
}

void ABladeIIControlNetCharacter::ReserveQTEEnableEndCB()
{
	// ���� ������ ȣ��Ʈ���Ѵ�.
	if (GetNetStatus() == NET_MASTER)
		Super::ReserveQTEEnableEndCB();
}

void ABladeIIControlNetCharacter::OnNetReceiveQTEEnableEnd()
{
	// �θ� ȣ�����ְ� ���� ����.
	Super::OnQTEEnableEndCB();

	SetRemoteDyingSignal();
	SetActorEnableCollision(false);
	GetMovementComponent()->StopMovementImmediately();
	//Die(0, FDamageEvent(UDamageType::StaticClass()), GetController(), NULL);
}

void ABladeIIControlNetCharacter::OnQTEEnableEndCB()
{
	// ȣ��Ʈ�� ����(Ż�� �����������) üũ�ؼ� ���. ���� �ް�ó��.
	if (!bAllowedMount)
	{
		AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

		if (pGM)
		{
			pGM->NetSendQTEEnableEnd();
		}
	}	
}
