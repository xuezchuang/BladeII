// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NetPlay/BladeIINetCharacter.h"
#include "BladeIIControlNetCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API ABladeIIControlNetCharacter : public ABladeIINetCharacter
{
	GENERATED_BODY()
	
public:
	ABladeIIControlNetCharacter(const FObjectInitializer& ObjectInitializer);
	
	// BladeIICharacter Interface
	virtual void SetHealth(float NewHealth, bool bReceivedFromHost = false);

	virtual void ReserveQTEEnableEndCB() final;

	void OnNetReceiveQTEEnableEnd();

protected:
	virtual void OnQTEEnableEndCB() final;

public:
	bool bAllowedMount;
};
