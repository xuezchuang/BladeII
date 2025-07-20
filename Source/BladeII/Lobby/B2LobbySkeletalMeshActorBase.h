#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "BoneControllers/AnimNode_LookAt.h"
#include "B2LobbySceneManager.h"
#include "B2LobbySkeletalMeshActorBase.generated.h"

/**
 * Introduced to bind lobby SkeletalMeshActor types in a single parent. 
 * Some are implemented after AB2LobbySkeletalMeshActorBase..
 */
UCLASS(abstract)
class AB2UnitedLobbySKMeshActorBase : public ASkeletalMeshActor
{
	GENERATED_BODY()

public:
	AB2UnitedLobbySKMeshActorBase(const FObjectInitializer& ObjectInitializer);
};

/**
 * Not all SkeletalMeshActors in lobby are in this type.. 
 * It is implicitly a mesh actor representing the "player character".
 */
UCLASS(abstract)
class AB2LobbySkeletalMeshActorBase : public AB2UnitedLobbySKMeshActorBase
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	class TSubclassOf<UAnimInstance> AnimClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TSoftObjectPtr<class UAnimSequenceBase> LeftRotateAnimAsset;
public:
	FORCEINLINE class UAnimSequenceBase* GetLeftRotateAnimAsset() { return LeftRotateAnimAsset.LoadSynchronous(); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TSoftObjectPtr<class UAnimSequenceBase> RightRotateAnimAsset;
public:
	FORCEINLINE class UAnimSequenceBase* GetRightRotateAnimAsset() { return RightRotateAnimAsset.LoadSynchronous(); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	class UAnimSequenceBase* SelectedAnimAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LookAt)
	FVector LookAtOffset;
		
public:
	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	virtual bool UseLookAt() { return false; };

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void SetActionAnim(bool bToSet) { bActionAnimTriggered = bToSet; }

	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	bool GetActionAnim(){ return bActionAnimTriggered; }

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void OnEndedActionAnim();

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void SetRotateAnim(bool bToSet) { bRotateAnimTriggered = bToSet; }

	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	bool GetRotateAnim(){ return bRotateAnimTriggered; }

	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	bool IsLeftRotation(){ return bLeftRotation; }

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void OnStartedRotatationAnim();

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void OnEndedRotatationAnim();

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void EnableRotateInput() { bEnableInputRotate = true; }

	UFUNCTION(BlueprintCallable, Category = "LobbyCharacter")
	void IgnoreLookAtBlend(bool bIgnore) { bIgnoreLookAtState = bIgnore; }

	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	float GetLookUpBlend();
	
	FVector GetLobbyAnimAdjustOffset();
	
	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	FRotator GetActorToCameraRotationActorSpace();

	UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	FVector GetLookAtTargetLocation();

	//UFUNCTION(BlueprintPure, Category = "LobbyCharacter")
	//FWrapperVectorPtr GetPrevLookAtTargetLocation() { return FWrapperVectorPtr(&PrevLookAtTargetLocation); }

	void UpdateLookAtParameters();
	
	void SetPauseLookAt(float PauseTime);

protected:
	bool bActionAnimTriggered;
	bool bPlayingActionAnim;

	bool bRotateAnimTriggered;
	bool bLeftRotation;
	bool bPlayingRotateAnim;
	bool bEnableInputRotate;

	bool bCameraOutLookAtArea;
	bool bIgnoreLookAtState;

	float LookUpAlphaBlend;
	float RestoreFromPauseBlend;
	float RemainPauseLookUpTime;

	FRotator ActorToCameraRotationActorSpace;
	FVector LookAtTargetLocation;

	FVector PrevLookAtTargetLocation;

	TWeakObjectPtr<class UAnimSequenceBase> CurrentAnimSequence;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	void PlayTurnAnimation(bool bLeftTurn, bool bInSkipAnimNotifiers = false, bool bInSkipSoundNotifiersOnly = false);
	virtual void PlaySelectAnimation(ELobbyScene CurrentLobbyScene = ELobbyScene::ELobbyScene_CharacterIntro, bool bInSkipAnimNotifiers = false, bool bInSkipSoundNotifiersOnly = false);

	UFUNCTION(BlueprintPure, Category="LobbyCharacter")
	class UAnimSequenceBase* GetCurrentAnimSequence();

	virtual bool IsPlayingSelectedAnim();
	
protected:
	void PlayAnimation(UAnimSequenceBase* Anim, bool bInSkipAnimNotifiers = false, bool bInSkipSoundNotifiersOnly = false);
	bool CheckLookAtEnable();


	friend struct FAnimNode_B2RandomPlayer;
};