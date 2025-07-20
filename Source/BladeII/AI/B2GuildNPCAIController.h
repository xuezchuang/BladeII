// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeIIAIController.h"
#include "B2GuildNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API AB2GuildNPCAIController : public ABladeIIAIController
{
	GENERATED_BODY()

public:
	AB2GuildNPCAIController(const FObjectInitializer& ObjectInitializer);

	//virtual void Possess(APawn* InPawn) override;
	//virtual void UnPossess() override;
	virtual bool BeginAutoPlay();

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	virtual class ABladeIICharacter* PickOpponent();
	virtual bool UpdateTarget();

	class ABladeIICharacter* GetCurrentTarget();
	
	virtual void SetTeamNum(int8 InTeamNum);
	int8 GetTeamNum();

protected:
	class ABladeIICharacter* CurrentTarget;

	int8 TeamNum;
};


UCLASS()
class BLADEII_API AB2TestGuildNPCAIController : public AB2GuildNPCAIController
{
	GENERATED_BODY()

public:
	AB2TestGuildNPCAIController(const FObjectInitializer& ObjectInitializer);

	virtual class ABladeIICharacter* PickOpponent() override;
	virtual void SetTeamNum(int8 InTeamNum) override;
};
