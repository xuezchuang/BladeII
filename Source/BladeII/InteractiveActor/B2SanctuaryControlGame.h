// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "InteractiveActor/B2SanctuaryActorBase.h"
#include "B2SanctuaryControlGame.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API AB2SanctuaryControlGame : public AB2SanctuaryActorBase
{
	GENERATED_UCLASS_BODY()
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII", meta = (DisplayName = "SanctuaryControlGameDeactive"))
	void DeactiveSanctuary();

	void ReceiveInteract(class AActor* InInteractingActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
	int32 NetIndex;
protected:
	virtual void Interact(class AActor* InInteractingActor);
	
};
