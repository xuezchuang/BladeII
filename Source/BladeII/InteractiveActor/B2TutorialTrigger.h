// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "GameFramework/Actor.h"
#include "B2TutorialGameMode.h"
#include "BladeIIInteractiveActor.h"
#include "B2TutorialTrigger.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API AB2TutorialTrigger : public ABladeIIInteractiveActor
{
	GENERATED_BODY()

public:
	AB2TutorialTrigger(const FObjectInitializer& ObjectInitializer);

	virtual void InteractAction() override;
	virtual bool IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType) override;

#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void EditorRefreshTextComponent(const FString& NewString);
#endif

#if WITH_EDITORONLY_DATA

private:

	UPROPERTY()
	class UTextRenderComponent* EditorTextComponent;

#endif

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	//ETutorialNumber TutorialNumber;

};
