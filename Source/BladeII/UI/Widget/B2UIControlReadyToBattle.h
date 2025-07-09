// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlReadyToBattle.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlReadyToBattle : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UB2UIControlReadyToBattle(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_ReadyToStartGame();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void ShowFightFX();

protected:
	UPROPERTY(EditAnywhere, Category = BladeII)
		class UParticleSystem* FightFX;

	UPROPERTY(EditAnywhere, Category = BladeII)
		float FightFxOffsetTime;

	UPROPERTY(EditAnywhere, Category = BladeII)
		float FightFxCamDistance;

	UPROPERTY(EditAnywhere, Category = BladeII)
		float FightFxScale;
	
private:
	TWeakObjectPtr<class UParticleSystemComponent> PSCFightFX;
};
