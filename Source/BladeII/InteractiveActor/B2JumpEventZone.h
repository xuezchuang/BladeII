// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "InteractiveActor/BladeIIInteractiveActor.h"
#include "../BladeII/B2StageEventDirector.h"
#include "B2JumpEventZone.generated.h"

/**
 * 
 */


//enum class EStageEvent : uint8;


UCLASS()
class BLADEII_API AB2JumpEventZone : public ABladeIIInteractiveActor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void BeginInteract(class AActor* InInteractingActor) override;
	virtual void EndInteract() override;

	virtual void InteractAction() override;

	virtual void NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType);

	void SubscribeStageEventDirector();
	void UnSubscribeStageEventDirector();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class AB2AutoWayPoint* TriggerShareWayPoint;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<class AB2StageEventDirector*> OwnerEventDirectors;

protected:

	UPROPERTY()
	AActor* CurrentInteractActor;

	uint32 StageEventSceneBeginTicket;
};
