// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "InteractiveActor/BladeIIInteractiveActor.h"
#include "B2DropItem.h"
#include "B2SanctuaryDimension.generated.h"


UENUM(BlueprintType)
enum class ESanctuaryType : uint8
{
	EST_None,
	EST_Heal,
	EST_AttUp,
	EST_DefUp,
	EST_SpdUp,
	EST_UnBeat
};

/**
 * 
 */
UCLASS()
class BLADEII_API AB2SanctuaryDimension : public ABladeIIInteractiveActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	TSubclassOf<class AB2SantuaryDimensionBall> FragmentTemplateClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	FVector SpawnEnergyBallOffset;

protected:
	virtual void InteractAction() override;	
};

UCLASS(Blueprintable)
class BLADEII_API AB2SantuaryDimensionBall : public AB2DropBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	ESanctuaryType SanctuaryType = ESanctuaryType::EST_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float Amount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	float TimeDuration = -1.f;

	virtual void OnPlayerSuckedMe(class ABladeIIPlayer* InSucker) override;
};