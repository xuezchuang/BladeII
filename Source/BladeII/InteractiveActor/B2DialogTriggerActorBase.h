#pragma once

#include "BladeIIInteractiveActor.h"
#include "CommonStruct.h"
#include "B2DialogTriggerActorBase.generated.h"

UCLASS()
class BLADEII_API AB2DialogTriggerActorBase : public ABladeIIInteractiveActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRootComponent;

#if WITH_EDITORONLY_DATA
private :
	/** Billboard Component displayed in editor */
	UPROPERTY()
	class UBillboardComponent* EditorSpriteComponent;
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName	DialogCodeName;

	/** One of event activate conditions, especially regarding the GameMode. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStageEventManageMode ManageMode;

	/** Stage numbers that this event is played. Empty list will be played for any stages. (Possibly for stage-based GameMode) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> SupportedStages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<EStageDifficulty> SupportedDifficulty;

	/** True will maintain in-game battle UIs visible */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bMaintainBattleUIs : 1;

protected:
	virtual void InteractAction();

	virtual bool IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType) override;

	bool IsStageSupported(int32 InClientStageId) const;
	bool IsDifficultySupported(EStageDifficulty InStageDifficulty) const;
public:
	bool IsExpectedForGameMode(class ABladeIIGameMode* InB2GM) const;

#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};