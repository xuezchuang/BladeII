// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "InteractiveActor/BladeIIInteractiveActor.h"
#include "B2DropItem.h"
#include "B2SanctuaryActorBase.generated.h"

/**
 * 
 */
UCLASS(hidecategories = InteractTrigger)
class BLADEII_API AB2SanctuaryActorBase : public ABladeIIInteractiveActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	int32 NumFragment;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	bool bRandomFragmentStayTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII, meta = (EditCondition = "bRandomFragmentStayTime"))
	float FragmentStayTimeRandomOffset_Min;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII, meta = (EditCondition = "bRandomFragmentStayTime"))
	float FragmentStayTimeRandomOffset_Max;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float TotalHealPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	TSubclassOf<class AB2EnergyBall> FragmentTemplateClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	FVector SpawnEnergyBallOffset;

protected:
	virtual void InteractAction() override;
};

UCLASS(Blueprintable)
class BLADEII_API AB2EnergyBall : public AB2DropBase
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetHealPercentage(float ToSetValue) {HealPercentage = ToSetValue;}

public:
	virtual void OnPlayerSuckedMe(class ABladeIIPlayer* InSucker) /*override*/;

protected:
	float HealPercentage;
};