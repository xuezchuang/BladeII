// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
//#include "BladeII.h"
#include "GameFramework/PlayerController.h"
////#include "Matinee/MatineeActor.h"
#include "BladeIIPlayer.h"
//#include "Navigation/GridPathFollowingComponent.h"
//#include "MyAssetStore.h"
#include "BladeIIPlayerController.generated.h"

UCLASS()
class ABladeIIPlayerController : public APlayerController
{
	GENERATED_BODY()

////public:
////	ABladeIIPlayerController(const FObjectInitializer& ObjectInitializer);
////
////	//////////////////////////////////////////////////
////	// Camera target or posture setting.
////	//
////
////	/** 
////	 * Set camera target actor that the player top-down camera will be targeting.
////	 * It can be initially set by either camera component transform, or camera boom rotation.
////	 * Setting this will invalidate explicit camera boom setting by SetCameraBoomPosture and SetCameraBoomYaw. Pitch and ArmLength can be maintained
////	 * @param bSetByCameraBoom : Depend on the selection, BladeIICharacter's CameraTargetTransform or CameraTargetBoomRotOffset is used inside.
////	 * @param bTargetPitchToo : False only looks at target by yaw, true looks at target by both yaw and pitch. Valid only when bSetByCameraBoom is true. CameraTargetBoomRotOffset is applied anyway.
////	 */
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraTarget(ABladeIICharacter* CameraTarget, bool bSetByCameraBoom = true, bool bTargetPitchToo = false);
////
////	/** 
////	 * Adjust the camera boom's rotation and distance where the player top-down camera is attached.
////	 * Affected only once when this is called, but consistent for possessed pawn change.
////	 * Setting this will invalidate explicit CameraTargetActor by SetCameraTarget
////	 */
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraBoomPosture(float InRotPitch, float InRotYaw, float InArmLength);
////	/**
////	 * Adjust the camera boom's rotation pitch where the player top-down camera is attached.
////	 * Affected only once when this is called, but consistent for possessed pawn change.
////	 */
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraBoomPitch(float InRotPitch);
////	/**
////	 * Adjust the camera boom's rotation yaw where the player top-down camera is attached.
////	 * Affected only once when this is called, but consistent for possessed pawn change.
////	 * Setting this will invalidate CameraTargetActor by SetCameraTarget
////	 */
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraBoomYaw(float InRotYaw);
////
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraBoomRoll(float InRotRoll);
////	/**
////	 * Adjust the camera boom's distance where the player top-down camera is attached.
////	 * Affected only once when this is called, but consistent for possessed pawn change.
////	 */
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraBoomArmLength(float InArmLength);
////
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraTargetFocusOffset(const FVector& Offset);
////	
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void SetCameraBoomLockedViewTarget(bool bLockedViewTarget);
////
////	void SetDefaultCameraRotator();
////
////private:
////	void ApplyExplicitCameraBoomParams();
////public:
////
////	/** Apply player top-down camera with parameters defined at world setting. You can optionally give blend parameters.
////	 * Depend on the settings, previous settings done by SetCameraTarget, SetCameraBoom** here are ignored. */
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void ApplyWorldDefaultCamera(float BlendTime = 0.0f, enum EViewTargetBlendFunction BlendFunc = VTBlend_Cubic);
////
////	/** Apply light settings of WorldSetting actor to the point light of possessed player character. */
////	void ApplyWorldPlayerLightSetting();
////
////	//
////	//////////////////////////////////////////////////
////
////	void SetPawn(APawn* InPawn) override;
////
////	virtual bool IsSkippedByCustomLogic() const /*override*/;
////
////	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	//ALevelSequenceActor * GetMatineeActor();
////
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	void KillAllEnemies();
////
////	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BladeIIGame)
////	//ALevelSequenceActor * MatineeActor;
////
////	virtual void SetCinematicMode(bool bInCinematicMode, bool bHidePlayer, bool bAffectsHUD, bool bAffectsMovement, bool bAffectsTurning) override;
////
////	void DeactivateVitualJoystick();
////
////	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)override;
////
////private:
////	/** True if CinematicMode is set with bAffectsHUD. It is supposed to be referred by BladeIIUserWidget and/or UIManager to manage the widget's visibility. 
////	 * The default behavior hides AHUD instance which we don't use. */
////	uint32 bHUDHidingCinematicMode : 1;
////	/** For a little clearer cinematic mode input setting. I'm might be doing something stupid.. */
////	uint32 bInputDisabledCinematicMode : 1;
////	/** It disables input to player character (like bInputDisabledCinematicMode) and make player character invincible.
////	 * Once it is set, consider the world is to be destroyed soon. */
////	uint32 bStageEndNonCombatState : 1;
////
////	//FMyAssetStore	MyAssetStore;
////
////	int32 nCinematicModeCount;
////
////	UPROPERTY(Transient)
////	UPathFollowingComponent* CachePathFollowingComp;
////public:
////	void SetStageEndNonCombatState(bool bInValue);
////	
////	/** Introduced to reduce confusing by move input axis switching at the boundary of active camera actor. */
////	uint32 bDisableMoveInputForActiveCameraTransition : 1;
////
////protected:
////	/** True if the controlled character should navigate to the mouse cursor. */
////	uint32 bMoveToMouseCursor : 1;
////
////	//////////////////////////////////////////////////
////	// Setting one of camera control variables below will be maintained through changing possessed pawn. (by tagging, resurrection stuff)
////	//
////
////	/** If set, camera will continuously look at this actor. ExplicitCameraBoomYaw is ignored when this is set. */
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	class ABladeIICharacter* CameraTargetActor;
////	/** Whether initially targeting using camera boom or direct camera transform. */
////	uint32 bCameraTargetSetByCameraBoom : 1;
////	/** Whether to looks at CameraTargetActor by pitch too. If false, only yaw will be used to look at the camera target. */
////	uint32 bCameraTargetFollowPitchToo : 1;
////
////	/** 
////	 * Set the Player's camera boom's rotation pitch to this value when SetCameraBoomPosture is called, maintained for possession change.
////	 */
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	float ExplicitCameraBoomPitch;
////	uint32 bExplicitCameraBoomPitchSet : 1; // Being set to true at first SetCameraBoomPitch call
////
////	/**
////	 * Set the Player's camera boom's rotation yaw to this value when SetCameraBoomPosture is called, maintained for possession change.
////	 * Cannot be with CameraTargetActor.
////	 */
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	float ExplicitCameraBoomYaw;
////	uint32 bExplicitCameraBoomYawSet : 1; // Being set to true at first SetCameraBoomYaw call
////
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	float ExplicitCameraBoomRoll;
////	uint32 bExplicitCameraBoomRollSet : 1;
////
////	/**
////	 * Set the Player's camera boom's arm length to this value when SetCameraBoomPosture is called, maintained for possession change.
////	 */
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	float ExplicitCameraBoomArmLength;
////	uint32 bExplicitCameraBoomArmLengthSet : 1; // Being set to true at first SetCameraBoomArmLength call
////
////	/**
////	* Set the Player's camera boom's location(as ViewTarget) to this value when SetCameraBoomPosture is called, maintained for possession change.
////	*/
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	FVector ExplicitCameraTargetFocusOffset;
////	uint32 bExplicitCameraTargetFocusOffsetSet : 1;
////
////	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BladeIIGame)
////	uint32 bExplicitCameraBoomLockedViewTarget : 1;
////	uint32 bExplicitCameraBoomLockedViewTargetSet : 1;
////
////	//
////	//////////////////////////////////////////////////
////
////	/** Spawn state to play appropriate spawn motion.
////	 * This is placed at here, instead of BladeIIPlayer, 
////	 * to have proper state before player character's initial animation tick has processed by AnimBP overriding of B2PCClassInfo. */
////	UPROPERTY(Transient)
////	EPCSpawnMotionState PlayerSpawnMotionState;
////
////	/** AI controller for auto play. In auto play mode, player is possessed by both player controller and this auto AI controller.
////	* We assume player is in auto play mode if AutoAIController reference is valid. */
////	//UPROPERTY()
////	//class AB2AutoAIController* AutoAIController;
////
////	/** Total skill management. Normal skill, resurrection, tag, etc.. */
////	UPROPERTY(Transient)
////	class UB2SkillRTManager* SkillRTManager;
////
////	virtual void BeginPlay() override;
////	virtual void BeginDestroy() override;
////	/** Called once this actor has been deleted */
////	virtual void Destroyed() override;
////
////	// Begin PlayerController interface
////	virtual void PlayerTick(float DeltaTime) override;
////	virtual void SetupInputComponent() override;
////	// End PlayerController interface
////
////	void HideViewBlockingObjects();
////
////	void FindTarget(bool bInputGuided);
////
////	/** Navigate player to the current mouse cursor location. */
////	void MoveToMouseCursor();
////
////	/** Navigate player to the current touch location. */
////	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
////	
////	/** Navigate player to the given world location. */
////	void SetNewMoveDestination(const FVector DestLocation);
////
////	/** Input handlers for SetDestination action. */
////	void OnSetDestinationPressed();
////	void OnSetDestinationReleased();
////
////	/** Spawns an AI controller used for auto play. Placed it here mimicking SpawnPlayerController. */
////	//class AB2AutoAIController* SpawnAutoAIController(FVector const& InSpawnLocation, FRotator const& InSpawnRotation);
////
////public:
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	bool IsInCinematicMode() const;
////	
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	bool IsHUDHidingCinematicMode() const;
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	bool IsInputDisabledCinematicMode() const;
////
////	FORCEINLINE void SetPCSpawnMotionState(EPCSpawnMotionState InState) { PlayerSpawnMotionState = InState; }
////	FORCEINLINE EPCSpawnMotionState GetPCSpawnMotionState() { return PlayerSpawnMotionState; }
////
////
////	bool StartAutoPlay(class ABladeIIPlayer*);
////	void StopAutoPlay();
////	void ResetAutoAiController();
////
////	bool MoveToActor(AActor* Goal, float AcceptanceRadius);
////
////	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
////	//AB2AutoAIController* GetAutoAIController() const { return AutoAIController; }
////	bool IsFineToStartAnyNewInputHandling() const {
////		return bStageEndNonCombatState == false && (IsInCinematicMode() == false || IsInputDisabledCinematicMode() == false);
////	}
////	bool IsFineToStartAnyNewMoveInputHandling() const {
////		return bDisableMoveInputForActiveCameraTransition == false && bStageEndNonCombatState == false &&	(IsInCinematicMode() == false || IsInputDisabledCinematicMode() == false);
////	}
////	bool CanPlayerHaveMoveMotion() const;
////	FORCEINLINE bool IsStageEndNonCombatState() const { return bStageEndNonCombatState; }
////
////	/** Spawns SkillRTManger for this controller. */
////	class UB2SkillRTManager* GetPlayerSkillRTManager();
////	//		
////	//FMyAssetStore&			GetMyAssetStore()
////	//{
////	//	return MyAssetStore;
////	//}
};
////
////FORCEINLINE bool ABladeIIPlayerController::IsInCinematicMode() const
////{
////	return bCinematicMode;
////}
////
////FORCEINLINE bool ABladeIIPlayerController::IsHUDHidingCinematicMode() const 
////{ 
////	return bHUDHidingCinematicMode; 
////}
////
////FORCEINLINE bool ABladeIIPlayerController::IsInputDisabledCinematicMode() const
////{ 
////	return bInputDisabledCinematicMode; 
////}