#pragma once

#include "B2LobbySkeletalMeshActorBase.h"
#include "B2LobbySkeletalMeshActor.generated.h"

USTRUCT(BlueprintType)
struct FB2LobbyCameraProperty
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamTargetFarFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamTargetNearFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamFarLimitDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamNearLimitDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamFarUnderCamLimitRotPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamNearUnderCamLimitRotPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float AddtionalZOffset;
};

UCLASS(BlueprintType)
class AB2LobbySkeletalMeshActor : public AB2LobbySkeletalMeshActorBase
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	float ZoomCamTargetHeightOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	FB2LobbyCameraProperty HorizontalCameraProperty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	FB2LobbyCameraProperty VerticalCameraProperty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LobbyCharacter)
	EPCClass PCClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LobbyCharacter)
	TSoftObjectPtr<class USoundBase> CloseUPSound;
public: 
	FORCEINLINE class USoundBase* GetCloseUPSound() { return CloseUPSound.LoadSynchronous(); }

	UFUNCTION(BlueprintImplementableEvent)
	void OnCloseUP();

	//UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	virtual bool UseLookAt() override { return bCloseUpState && !bPlayingActionAnim && !bPlayingRotateAnim; }

private:
	bool bCloseUpState;
	bool bPendingCloseUp;

public:

	void PlayTouchAnimation(UAnimSequenceBase* TouchAnim);
	void SetCloseUpState(bool bSet, bool bPlaySelectionAnim = true, bool bForceSetState = false);

	virtual void Tick(float DeltaSeconds) override;
	/** Event when this actor is clicked by the mouse when using the clickable interface. */
	virtual void NotifyActorOnReleased(FKey ButtonReleased = EKeys::LeftMouseButton) override;
	/** Event when this actor is under the finger when untouched when click events are enabled. */
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex) override;
	
private:
	void OnTouchedActor();	
};