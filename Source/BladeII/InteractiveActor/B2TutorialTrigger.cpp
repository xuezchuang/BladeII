// Fill out your copyright notice in the Description page of Project Settings.


#include "B2TutorialTrigger.h"

AB2TutorialTrigger::AB2TutorialTrigger(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
#if WITH_EDITOR && WITH_EDITORONLY_DATA

	EditorTextComponent = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(TEXT("UITriggerText"));
	if (EditorTextComponent)
	{
		EditorTextComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditorTextComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EditorTextComponent->SetVisibility(true);
		EditorTextComponent->SetHiddenInGame(true);
		EditorTextComponent->SetHorizontalAlignment(EHTA_Center);
		EditorTextComponent->SetWorldSize(35.0f);
		EditorTextComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		EditorTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // 좀 높여서
		EditorTextComponent->SetTextRenderColor(FColor(255, 255, 0));

		EditorRefreshTextComponent(this->GetName());
	}

#endif
}

void AB2TutorialTrigger::InteractAction()
{
	Super::InteractAction();

	AB2TutorialGameMode* B2GameMode = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GameMode)
	{
		B2GameMode->SetStartTutorial(TutorialNumber);
	}
}

bool AB2TutorialTrigger::IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType)
{
	return Super::IsTriggering(Other, InTriggerType);
}

#if WITH_EDITOR

void AB2TutorialTrigger::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	EditorRefreshTextComponent(GetName());
}

void AB2TutorialTrigger::EditorRefreshTextComponent(const FString& NewString)
{
	if (EditorTextComponent)
	{
		EditorTextComponent->SetText(FText::FromString(NewString));
	}
}


#endif