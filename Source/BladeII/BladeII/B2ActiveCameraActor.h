//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "GameFramework/Actor.h"
//#include "B2ActiveCameraActor.generated.h"
//
///** About how to move the camera itself. */
//UENUM(BlueprintType)
//enum class EActiveCameraMovementMode : uint8
//{
//	// Like default top-down camera
//	EACMM_FollowPlayer,
//
//	// Non moving, target still can be moving.
//	EACMM_Fixed,
//
//	// Somewhere between FollowPlayer and Fixed. Based on some fixed position and also follows player movement with certain scale. The scale parameter should be provided with this selection.
//	EACMM_ScaledFollowPlayer
//};
//
///** About how to focus the camera, involving rotation. */
//UENUM(BlueprintType)
//enum class EActiveCameraTargetingMode : uint8
//{
//	// Target to the player character
//	EACTM_Player,
//
//	// Target to a specified mob. The mob information should be provided with this selection.
//	EACTM_Mob,
//
//	EACTM_Fixed
//};
//
///** About how to activate (or trigger) the active camera */
//UENUM(BlueprintType)
//enum class EActiveCameraTriggerMode : uint8
//{
//	/** Activate in actor's area defined by all child components
//	 * This is most suggested activate mode. */
//	EACTRG_ActorArea,
//
//	/** Activate and deactivate each time touches selected child collision components.
//	 * Need some care to setup for this mode. Not really recommended, but I can't find other way to meet the requirement.. */
//	EACTRG_ToggleByComponent,
//
//	EACTRG_BeginStage,
//
//	EACTRG_ManualActive,
//};
//
//
///**
// * It almost resembles FWorldDefaultPlayCameraSettings, but could get to be different.
// */
//USTRUCT(BlueprintType)
//struct FActiveCameraPlayerTopDownCameraSettings
//{
//	GENERATED_USTRUCT_BODY()
//
//	FActiveCameraPlayerTopDownCameraSettings()
//	{
//		bOverrideCameraBoomPitch = false;
//		CameraBoomPitch = -50.0f;
//		bOverrideCameraBoomYaw = false;
//		CameraBoomYaw = -130.0f;
//		bOverrideCameraBoomRoll = false;
//		CameraBoomRoll = 0.0f;
//		bOverrideCameraBoomArmLength = false;
//		CameraBoomArmLength = 700.0f;
//		CameraBoomArmLength_Extra = 0.0f;
//		bOverrideCameraTargetFocusOffset = false;
//		CameraTargetFocusOffset = FVector::ZeroVector;
//		bCameraBoomLockedViewTarget = false;
//		bOverrideCameraPitch = false;
//		CameraPitch = 0.0f;
//		bOverrideCameraYaw = false;
//		CameraYaw = 0.0f;
//	}
//
//	UPROPERTY()
//	uint32 bOverrideCameraBoomPitch : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomPitch"))
//	float CameraBoomPitch;
//
//	UPROPERTY()
//	uint32 bOverrideCameraBoomYaw : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomYaw"))
//	float CameraBoomYaw;
//
//	UPROPERTY()
//	uint32 bOverrideCameraBoomRoll : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomRoll"))
//	float CameraBoomRoll;
//
//	UPROPERTY()
//	uint32 bOverrideCameraBoomArmLength : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomArmLength"))
//	float CameraBoomArmLength;
//	/** Extra range of CameraBoomArmLength, applied with further system level adjustment. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomArmLength"))
//	float CameraBoomArmLength_Extra;
//
//	UPROPERTY()
//	uint32 bOverrideCameraTargetFocusOffset : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraTargetFocusOffset"))
//	FVector CameraTargetFocusOffset;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera")
//	uint32 bCameraBoomLockedViewTarget : 1;
//
//	UPROPERTY()
//	uint32 bOverrideCameraPitch : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraPitch"))
//	float CameraPitch;
//
//	UPROPERTY()
//	uint32 bOverrideCameraYaw : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraYaw"))
//	float CameraYaw;
//
//	// Other properties for directly controlling camera component's transform might be added?
//	// Check SetupForConsistentPlayerTopDownCamera for added properties.
//};
//
///**
// * To adjust mob targeting with player top-down camera a little bit
// */
//USTRUCT(BlueprintType)
//struct FActiveCameraMobTargetAdjustSettings
//{
//	GENERATED_USTRUCT_BODY()
//
//	FActiveCameraMobTargetAdjustSettings()
//	{
//		BoomRotOffsetPitch = 0.0f;
//		BoomRotOffsetYaw = 0.0f;
//		CamRotOffsetPitch = 0.0f;
//		CamRotOffsetYaw = 0.0f;
//		bOverrideCameraBoomArmLength = false;
//		CameraBoomArmLength = 700.0f;
//		CameraBoomArmLength_Extra = 0.0f;
//		TargetLocationOffset = FVector(0.0f, 0.0f, 0.0f);
//	}
//
//	/** Additional pitch value to be applied to the camera boom when targeting mob */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	float BoomRotOffsetPitch;
//
//	/** Additional yaw value to be applied to the camera boom when targeting mob */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	float BoomRotOffsetYaw;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	float CamRotOffsetPitch;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	float CamRotOffsetYaw;
//
//	UPROPERTY()
//	uint32 bOverrideCameraBoomArmLength : 1;
//	/** Override default setup of player character's Blueprint. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomArmLength"))
//	float CameraBoomArmLength;
//	/** Extra range of CameraBoomArmLength, applied with further system level adjustment. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomArmLength"))
//	float CameraBoomArmLength_Extra;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	FVector TargetLocationOffset;
//};
//
///**
// * Selected settings of SpringArmComponent. To give unified setting interface regardless of ActiveCameraComp or player's TopDown camera.
// */
//USTRUCT(BlueprintType)
//struct FActiveCameraSpringArmSelectedSettings
//{
//	GENERATED_USTRUCT_BODY()
//
//	FActiveCameraSpringArmSelectedSettings()
//	{
//		bEnableCameraLag = false;
//		bEnableCameraRotationLag = false;
//		CameraLagSpeed = 10.f;
//		CameraRotationLagSpeed = 10.f;
//		CameraLagMaxDistance = 0.f;
//	}
//
//	//
//	// Member properties here are from SpringArmComponent.
//	// Check CopyFromSelectedSettingsToSpringArmComponent and CopyFromSpringArmComponentToSelectedSettings for additional members.
//	//
//
//	/**
//	 * If true, camera lags behind target position to smooth its movement.
//	 * @see CameraLagSpeed
//	 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
//	uint32 bEnableCameraLag : 1;
//
//	/**
//	 * If true, camera lags behind target rotation to smooth its movement.
//	 * @see CameraRotationLagSpeed
//	 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
//	uint32 bEnableCameraRotationLag : 1;
//
//	/** If bEnableCameraLag is true, controls how quickly camera reaches target position. Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag). */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, meta = (editcondition = "bEnableCameraLag", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
//	float CameraLagSpeed;
//
//	/** If bEnableCameraRotationLag is true, controls how quickly camera reaches target position. Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag). */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, meta = (editcondition = "bEnableCameraRotationLag", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
//	float CameraRotationLagSpeed;
//
//	/** Max distance the camera target may lag behind the current location. If set to zero, no max distance is enforced. */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, meta = (editcondition = "bEnableCameraLag", ClampMin = "0.0", UIMin = "0.0"))
//	float CameraLagMaxDistance;
//};
//
//UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "B2 Active Camera"))
//class BLADEII_API AB2ActiveCameraActor : public AActor
//{
//	GENERATED_BODY()
//	
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
//	class USceneComponent* DefaultRootComponent;
//
//#if WITH_EDITORONLY_DATA
//private :
//	/** Billboard Component displayed in editor */
//	UPROPERTY()
//	class UBillboardComponent* EditorSpriteComponent;
//
//#endif
//
//protected:
//
//	/** About how to move the camera */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	EActiveCameraMovementMode CameraMovementMode;
//
//	/** About how to focus the camera */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bDualCameraBlending_Inverse"))
//	EActiveCameraTargetingMode CameraTargetingMode;
//	
//	//////////////////////////////////////////////////////////////////////
//	// Properties for EditCondition, automatically set. Use them only for editcondition, not referring in code.
//	UPROPERTY()
//	uint32 bDualCameraBlending_Inverse : 1;
//	UPROPERTY()
//	uint32 bCameraMovementMode_ScaledFollowingPlayer : 1;
//	UPROPERTY()
//	uint32 bCameraTargetingMode_Mob : 1;
//	UPROPERTY()
//	uint32 bCameraTargetingMode_Mob_with_PlayerCameraBoom : 1;
//	UPROPERTY()
//	uint32 bCameraTargetingMode_Fixed : 1;
//	UPROPERTY()
//	uint32 bCameraMovementTarget_BothPlayer : 1;
//	UPROPERTY()
//	uint32 bUseActiveCameraComponentMode : 1;
//	UPROPERTY()
//	uint32 bTriggerMode_ToggleByComponent : 1;
//	UPROPERTY()
//	uint32 bUseDCBExtraPlayerTopDownCameraSettings : 1;
//	UPROPERTY()
//	uint32 bUseDCBExtraFOV_Override : 1;
//	//////////////////////////////////////////////////////////////////////
//
//	/** Show if current play camera is under this actor's influence. */
//	uint32 bIsActivated : 1;
//
//	/** The lower, the more ActiveCamera component will stick to its placed origin. 1.0 makes ActiveCamera component sticks to the player character.
//	 * Necessary when CameraMovementMode is ScaledFollowPlayer */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bCameraMovementMode_ScaledFollowingPlayer"))
//	float CameraPlayerFollowingScale;
//
//	/** To select camera target mob from WaveSetting of currently active spawnpool. Specifies the wave number and mob index like "WaveNum MobIdx", e.g "0 1". */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bCameraTargetingMode_Mob"))
//	FString TargetMobSelectKeyword;
//
//	/** When the CameraTarget is Mob and player's camera boom is used (FollowPlayer) */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bCameraTargetingMode_Mob_with_PlayerCameraBoom"))
//	FActiveCameraMobTargetAdjustSettings TargetMobCameraAdjustSettings;
//
//	/** Used when both CameraMovementMode and CameraTargetingMode are for player.
//	* It can be set for each element basis. Player's current setting is used for non-overridden values. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bCameraMovementTarget_BothPlayer"))
//	FActiveCameraPlayerTopDownCameraSettings PlayerTopDownCameraSettings;
//
//	UPROPERTY()
//	uint32 bOverrideFOVAngle : 1;
//	/** Override FOV angle no matter which camera component is applied. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bOverrideFOVAngle"))
//	float FOV_Override;
//	
//	/** When this is checked, CameraMovementMode and CameraTargetingMode are ignored.
//	 * It assumes this actor has additional CameraComponent installed (other than ActiveCameraComp),
//	 * and the result is blending between those two cameras by player's location. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	uint32 bDualCameraBlending : 1;
//	
//	/** You need to add additional camera component and set its name here for bDualCameraBlending */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bDualCameraBlending"))
//	FName DCBExtraCamName;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bDualCameraBlending"))
//	FName DCBFlagName;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bDualCameraBlending"))
//	FName DCBExtraFlagName;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bUseDCBExtraPlayerTopDownCameraSettings"))
//	FActiveCameraPlayerTopDownCameraSettings DCBExtraPlayerTopDownCameraSettings;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bUseDCBExtraFOV_Override"))
//	float DCBExtraFOV_Override;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera")
//	bool bRestoreCamera;
//
//	UPROPERTY()
//	uint32 bOverrideSpringArmSettings : 1;
//	/** You can control selected properties of SpringArm component, either the ActiveCameraSpringArm, or player character's CameraBoom. 
//	 * If the overall setting is using ActiveCameraComp (instead of player top-down camera), spring arm's leg setting can be just directly adjusted. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bOverrideSpringArmSettings"))
//	FActiveCameraSpringArmSelectedSettings SpringArmSettings;
//
//	UPROPERTY()
//	uint32 bUseCameraBoomRotationClamp : 1;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bUseCameraBoomRotationClamp"))
//	float MinCameraBoomRotationClamp;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (editcondition = "bUseCameraBoomRotationClamp"))
//	float MaxCameraBoomRotationClamp;
//
//	/** Blend time for begin activate from previous camera */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Time")
//	float BlendInTime;
//	/** Blend time for end activate to player top-down camera */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Time")
//	float BlendOutTime;
//
//	/** A hack to prevent confusing at the moment of transition between top-down camera and active camera here.
//	 * But looks like it is also necessary when top-down camera is consistently used (just except when the target is also the player). */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Time")
//	float TransitionInputDisableTime;
//
//	/** Moves player character a little bit in the current direction while input is disabled by TransitionInputDisableTime.
//	 * This scale value is multiplied to the player root component height, and the player will move in this distance. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Time")
//	float TransitionAutoMoveScale;
//
//	/** Defines the way to activate/deactivate. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Trigger")
//	EActiveCameraTriggerMode TriggerMode;
//
//	/** Add all collision (shape) components to trigger beginning for ToggleByComponent mode. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Trigger", meta = (editcondition = "bTriggerMode_ToggleByComponent"))
//	TArray<FName> ToggleMode_BeginComponents;
//
//	/** Add all collision (shape) components to trigger ending for ToggleByComponent mode. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Trigger", meta = (editcondition = "bTriggerMode_ToggleByComponent"))
//	TArray<FName> ToggleMode_EndComponents;
//
//	/** The main worker here. Either player top-down camera can be used depend on the configuration. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Misc")
//	class UCameraComponent* ActiveCameraComp;
//	/** ActiveCameraComp is attached to the spring arm, like player top-down camera. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Misc")
//	class USpringArmComponent* ActiveCameraSpringArm;
//	
//	/** Player movement direction directly depends on the camera yaw, 
//	 * but in this active camera environment, it can cause problem when the camera abruptly changed the rotation yaw at right above the player's head.
//	 * Check this will fix the player character's movement direction based on the ActiveCameraComponent's placed yaw value. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Misc", meta = (editcondition = "bUseActiveCameraComponentMode"))
//	uint32 bUsePlacedActiveCamYawAsInputBase : 1;
//
//	/** For fixed camera target mode, relative to this actor.
//	 * This is where TargetPositionEditingComp is located, and this value is get to sync from TargetPositionEditingComp. 
//	 * You can still edit it manually. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Misc", meta = (editcondition = "bCameraTargetingMode_Fixed"))
//	FVector FixedCameraTargetPosition;
//	
//	/** For kinda smooth input handling when camera is abruptly changing. 
//	 * Higher value (1.0, not using this feature) can lead to unstable input handling especially when the player is almost right below the camera, 
//	 * while lower value can lead to undesired movement direction.
//	 * However, it also causes problem where angle sign changes while angle itself is not really changed (e.g. -180 -> 180) 
//	 * Not really simple to use.. setting the default value to not using it (1.0) */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera_Misc")
//	float PlayerMoveInputAdjustBoomSyncAlpha;
//
//private:
//	/** It must be valid while this camera actor is actively working (from Begin to End) 
//	 * It is for camera system use, interacting player's controller can be different, or the same for local mode game (e.g. stage mode) */
//	UPROPERTY(Transient)
//	class ABladeIIPlayerController* CachedLocalPC;
//	/** Interacting player character's owning controller. It is supposed to be the same to CachedLocalPC in stage game mode. */
//	UPROPERTY(Transient)
//	class AController* CachedInteractingController;
//
//	/** Cached reference at last begin timing. Never assume that it is valid! It is used to detect whether new player character began to interact. */
//	UPROPERTY(Transient)
//	class ABladeIIPlayer* LastInteractBeginPlayer;
//public:
//	class ABladeIIPlayer* GetInteractPlayerCharacter();
//
//	EActiveCameraTriggerMode GetTriggerMode() { return TriggerMode; }
//
//private:
//	
//	/** Would be valid only when CameraTargetingMode is Mob, by TargetMobSpawnPool and TargetMobSelectKeyword */
//	UPROPERTY(Transient)
//	class ABladeIICharacter* CachedTargetMob;
//
//	/** To provide placed base camera component transform. Cached at BeginPlay. */
//	FTransform InitialActiveCameraSpringArmTransform;
//
//	/** For input disabling while transiting. */
//	FTimerHandle TransitInTimeTimerHandle;
//	uint32 bTransitInInputDisabled : 1;
//	FTimerHandle TransitOutTimeTimerHandle;
//	uint32 bTransitOutInputDisabled : 1;
//
//	/** Transient editor-only static mesh component to preview and edit the location in Fixed CameraTargetingMode 
//	 * Just a single element is necessary. Put it in array structure to enable editing native component in editor viewport using modification of FComponentEditorUtils::CanEditNativeComponent */
//	UPROPERTY(Transient, EditAnywhere)
//	TArray<class UStaticMeshComponent*> TargetPositionEditingComp;
//	
//	UPROPERTY(Transient)
//	TArray<class UTextRenderComponent*> ShapeCompInfoText;
//
//	/** To restore camera boom lag settings when quit from here, if bOverrideSpringArmSettings is true. */
//	FActiveCameraSpringArmSelectedSettings PlayerCameraBoomSettingBackup;
//
//	/** For setting player input handling camera yaw for DualCameraBlending mode. */
//	float CachedDualCameraBlendedYaw;
//
//	bool bActivedByAnimation;
//
//public:	
//	// Sets default values for this actor's properties
//	AB2ActiveCameraActor(const FObjectInitializer& ObjectInitializer);
//
//	virtual void PostLoad() override;
//	virtual void PostRegisterAllComponents() override;
//
//	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
//	virtual void BeginDestroy() override;
//
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//	
//	// Called every frame
//	virtual void Tick( float DeltaSeconds ) override;
//
//	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
//
//	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
//
//	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
//
//	/** Will be registered to all child shape (collision) components */
//	UFUNCTION()
//	void OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//	/** Will be registered to all child shape (collision) components */
//	UFUNCTION()
//	void OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
//private:
//	void RegisterComponentOverlapCallback();
//	void RemoveComponentOverlapCallback();
//public:
//
//	/** Test if currently need to apply the ActiveCamera at any moment, then BeginActiveCamera if condition met, 
//	 * It is necessary in some special occasion, where Begin/EndOverlap test is not enough for some reason. 
//	 * You can optionally override active camera's blend time by give positive BlendInTimeOverride.
//	 * Returns true if BeginActiveCamera is executed.  */
//	bool ManualTestBeginActiveCamera(float BlendInTimeOverride = -1.0f);
//
//	/** Apply the active camera regardless of other conditions like overlapping. Provided for some special usage. 
//	 * It is different from ManualTestBeginActiveCamera which does the overlapping test at least. 
//	 * You can optionally override active camera's blend time by give positive BlendInTimeOverride.
//	 * In addition, you can optionally provide interacting controller, possibly for net game mode.  */
//	UFUNCTION(BlueprintCallable, Category = "ActiveCameraActor")
//	void ForceBeginActiveCamera(float BlendInTimeOverride = -1.0f, class AController* InOptionalInteractingController = NULL, bool bUseConditionalBeginTransitionInputDisable = true);
//
//	/** To end at any moment from outside */
//	UFUNCTION(BlueprintCallable, Category = "ActiveCameraActor")
//	void CustomEndActiveCamera();
//	
//	bool IsBaseActiveCamera();
//
//	float GetBlendInTime() { return BlendInTime; }
//	float GetBlendOutTime() { return BlendOutTime; }
//
//	FORCEINLINE void SetActivedByAnimation(bool bAcrive) { bActivedByAnimation = bAcrive; }
//	FORCEINLINE bool IsActivedByAnimation() { return bActivedByAnimation; }
//
//private:
//
//	/** Main entry point of this camera actor's functionality.
//	 * You can optionally override active camera's blend time by give positive BlendInTimeOverride. */
//	void BeginActiveCamera(class ABladeIIPlayerController* InLocalPC, float BlendInTimeOverride = -1.0f, class AController* InOptionalInteractingController = NULL, bool bUseConditionalBeginTransitionInputDisable = true);
//	/** Part of BeginActiveCamera, which can be used solely for new player character interaction without active state change. */
//	void BeginForNewInteractionCommon(class ABladeIIPlayerController* InLocalPC, float FinalBlendInTime, class AController* InOptionalInteractingController = NULL);
//	/** Main exit point of this camera actor's functionality. */
//	void EndActiveCamera();
//
//	void OverrideSpringArmSettings();
//	void RestoreSpringArmSettings();
//	
//	void UpdateActiveCamera(float DeltaSeconds);
//	void CheckForOutdatedInteraction();
//
//	/** For CameraMovementMode and CameraTargetingMode are both for player. */
//	void SetupForConsistentPlayerTopDownCamera();
//
//	/** False means it will stick to player top-down camera */
//	bool IsUsingActiveCameraComponent();
//
//	/** Set ActiveCamera's rotation (in fact, the spring arm) to make it target to specific location. Assumes world coordinate. */
//	void SetActiveCameraRotationToTarget(FVector InViewTargetLocation);
//	/** Almost like SetActiveCameraRotationToTarget, when current mode uses player top down camera.
//	 * Rotation offsets and ArmLengthOverride are possible, and ArmLengthOverride works only for values larger than 0. */
//	void SetPlayerCameraBoomRotationToTarget(FVector InViewTargetLocation, float RotOffsetPitch = 0.0f, float RotOffsetYaw = 0.0f, float ArmLengthOverride = 0.0f);
//
//	void SetPlayerCameraRotationToTarget(FVector InViewTargetLocation, float RotOffsetPitch = 0.0f, float RotOffsetYaw = 0.0f);
//
//	/** Get the wave mob specified by TargetMobSpawnPool and TargetMobSelectKeyword */
//	class ABladeIICharacter* GetDesiredCameraTargetWaveMob();
//
//	/** For begin or end transition input disabling. */
//	void ConditionalBeginTransitionInputDisable(bool bTransitIn);
//	void TransitInTimerCB();
//	void TransitOutTimerCB();
//
//	/** Supposedly only one of ActiveCameraActors in the world should return true at one time. */
//	FORCEINLINE bool IsCurrentlyActive() { return bIsActivated; }
//
//	// For EACTRG_ToggleByComponent
//	bool IsOneOfBeginToggleComp(class UShapeComponent* InTestComp);
//	bool IsOneOfEndToggleComp(class UShapeComponent* InTestComp);
//
//	class UCameraComponent* FindExtraCamCompForDualCameraBlending();
//	class USceneComponent* FindSceneComponentForDualCameraBlending(FName CompName);
//	float GetDualCameraBlendingWeight();
//	
//public:
//	UFUNCTION(BlueprintCallable, Category = "ActiveCameraActor")
//	void ChangeCameraOwnerPlayer(ABladeIIPlayer* targetPlayer, float OverrideBlendingTime = -1.0f);
//
//#if WITH_EDITOR
//	/** Refresh editing component either by regenerate necessary components or just sync their location. */
//	void RefreshEditingComp(bool bRegenerateStaticComp = true);
//	void SyncFromEditingCompPlacement();
//	void DestroyEditingComponents(bool bDestroyStaticCompToo);
//public:
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//	virtual void PostEditMove(bool bFinished) override;	
//private:
//	/** Ensure setting editcondition properties properly */
//	void SetEditConditionProperties();
//public:
//#endif
//};
