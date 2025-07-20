// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, ACTION SQUARE

#pragma once
#include "BladeII.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/EngineTypes.h"
#include "Camera/PlayerCameraManager.h"
#include "BladeIIWorldSettings.generated.h"

/**
 * Defines default player top-down camera view. The final camera behavior can be overridden at area defined by ActiveCamera interface.
 */
USTRUCT(BlueprintType)
struct FWorldDefaultPlayCameraSettings
{
	GENERATED_USTRUCT_BODY()

	FWorldDefaultPlayCameraSettings()
	{
		bOverrideCameraBoomPitch = false;
		CameraBoomPitch = -40.0f;
		bOverrideCameraBoomYaw = false;
		CameraBoomYaw = -90.0f;
		bOverrideCameraBoomRoll = false;
		CameraBoomRoll = 0.0f;
		bOverrideCameraBoomArmLength = false;
		CameraBoomArmLength = 800.0f;
		CameraBoomArmLength_Extra = 0.0f;
		bOverrideCameraTargetFocusOffset = false;
		CameraTargetFocusOffset = FVector::ZeroVector;
		bCameraBoomLockedViewTarget = false;
	}
	
	UPROPERTY()
	uint32 bOverrideCameraBoomPitch : 1;
	/** Override default setup of player character's Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera", meta = (EditCondition = "bOverrideCameraBoomPitch"))
	float CameraBoomPitch;

	UPROPERTY()
	uint32 bOverrideCameraBoomYaw : 1;
	/** Override default setup of player character's Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera", meta = (EditCondition = "bOverrideCameraBoomYaw"))
	float CameraBoomYaw;

	UPROPERTY()
	uint32 bOverrideCameraBoomRoll : 1;
	/** Override default setup of player character's Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera", meta = (EditCondition = "bOverrideCameraBoomRoll"))
	float CameraBoomRoll;
	
	UPROPERTY()
	uint32 bOverrideCameraBoomArmLength : 1;
	/** Override default setup of player character's Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera", meta = (EditCondition = "bOverrideCameraBoomArmLength"))
	float CameraBoomArmLength;
	/** Used if it is bigger than CameraBoomArmLength, with further system level adjustment.
	 * Basically, setting it larger than CameraBoomArmLength is just the same as longer CameraBoomArmLength in most circumstances.
	 * However, the amount of adjustment by this property can be varied by a system console variable. ("r.B2CameraDistanceAlpha") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveCamera", meta = (EditCondition = "bOverrideCameraBoomArmLength"))
	float CameraBoomArmLength_Extra;

	UPROPERTY()
	uint32 bOverrideCameraTargetFocusOffset : 1;
	/** Override default setup of player character's Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera", meta = (EditCondition = "bOverrideCameraTargetFocusOffset"))
	FVector CameraTargetFocusOffset;

	/** Using special camera-work like counter DG */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera")
	uint32 bCameraBoomLockedViewTarget : 1;

#if WITH_EDITOR
	/** Load values of single preset. UniquePresetIDString will be the part (prefix) of ini item's key string, most likely reflecting EWorldDefaultPlayCameraPreset enum item name. */
	static void LoadPresetValues(FWorldDefaultPlayCameraSettings& OutResult, FString UniquePresetIDString);
	void ApplyPresetValues(FWorldDefaultPlayCameraSettings& InPreset);
#endif
};

/** For editing convenience */
UENUM()
enum class EWorldDefaultPlayCameraPreset : uint8
{
	EWDCP_StandardTopDown,

	EWDCP_StandardBackView,

	EWDCP_End
};
// Keep the naming of EWorldDefaultPlayCameraPreset
#define EWORLDDEFAULTCAMERAPRESET_COMMON_PREFIX TEXT("EWDCP_")

/** Selected properties of point light to affect the one attached on our main player character. */
USTRUCT(BlueprintType)
struct FWorldPlayerLightSettings
{
	GENERATED_USTRUCT_BODY()

	//
	// Properties and default values here comes from ULightComponent and/or its nearby family.
	//

	FWorldPlayerLightSettings()
	{
		bOverrideLightColor = false;
		LightColor = FColor::White;

		bOverrideIntensity = false;
		Intensity = 35000.0f; // This default value is from base Blueprint setting as of March 2017
	}

	UPROPERTY()
	uint32 bOverrideLightColor : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideLightColor"))
	FColor LightColor;

	UPROPERTY()
	uint32 bOverrideIntensity : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideIntensity"))
	float Intensity;

};

/** Uniquely identify monster spawn pool wave.. including stage and difficulty info.
 * It is a little special.. using FName for easier setup. */
struct FCombinedUniqueSPWaveID
{
	FCombinedUniqueSPWaveID()
	{
		CombinedKeyName = NAME_None;
	}
	FCombinedUniqueSPWaveID(int32 InClientStageId, int32 InTargetDifficulty, int32 InTargetWave)
	{
		CombinedKeyName = WaveInfoToKeyName(InClientStageId, InTargetDifficulty, InTargetWave);
	}
	FORCEINLINE bool operator==(const FCombinedUniqueSPWaveID& InOther) const
	{
		return (this->CombinedKeyName == InOther.CombinedKeyName);
	}
protected:
	/** Supposed to be combined like 00_00_00, where each is ClientStageId, Difficulty, and Wave num. */
	FName CombinedKeyName;
public:
	static FName WaveInfoToKeyName(int32 InClientStageId, int32 InTargetDifficulty, int32 InTargetWave);
	FORCEINLINE FName GetMyKeyName() const { return CombinedKeyName; }

	//
	// No interfaces for extracting each specific info like StageNum and WaveNum.. That requires string parsing.
	//

	FORCEINLINE uint32 GetMyTypeHash() const { return static_cast<uint32>(CombinedKeyName.GetNumber()); }
};

/** For use of TMap with FCombinedUniqueSPWaveID key */
FORCEINLINE uint32 GetTypeHash(const FCombinedUniqueSPWaveID& InId)
{
	return InId.GetMyTypeHash();
}

/**
 * An Actor containing world (per-level) properties for BladeII
 * It can be regarded as a sort of InfoAsset, although it is an unreal world object.
 */
UCLASS(BlueprintType)
class BLADEII_API ABladeIIWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
protected:

	/** A key to search the localized area name from text table. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizedAreaNameKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizedAreaSubNameKey;

	/** Non-localized area sub name. -> Now replaced by LocalizedAreaSubNameKey */
	UPROPERTY()
	FString EnglishAreaSubName_DEPRECATED;
	
	/** Set the stage number for various editor preview and play-in-editor (for other dev test too).
	 * MonsterSpawnPool and StageEventDirector are affected by this setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 EditorStageNumber;

	/** Set the stage difficulty level for various editor preview and play-in-editor (for other dev test too).
	 * MonsterSpawnPool is affected by this setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 EditorStageDifficulty;	// todo : 炼父埃 EStageDifficulty肺 函版瞪 抗沥 -> 捞扒 SpawnPool 捞唱 StageEventDirector 客绰 崔府 EStageDifficulty 肺 官操绰 单俊 蝶弗 何淬篮 绝促. 盔茄促搁.

	/** Set the scenario mode (the first try) flag for editor and development fallback.
	 * StageEventDirector is affected by this setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Editor Scenario Mode"), Category = "BladeII")
	uint32 bDevTestScenarioMode : 1;

	/** Defines world default play camera, 
	 * where each overridden element will be applied as default if no overriding by ActiveCamera system presents. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera")
	FWorldDefaultPlayCameraSettings DefaultPlayerCameraSettings;

	/** To set some preset values to DefaultPlayerCameraSettings.
	 * This is editor-only transient variable, not really used in runtime. */
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly, Category = "BladeII_Camera")
	EWorldDefaultPlayCameraPreset TransientDefaultCameraPreset;

	/** It affects the light attached on player character head. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FWorldPlayerLightSettings PlayerLightSettings;

	/** A special setting as a workaround to the limitation of modulated shadow.
	 * Put any wave info that their mobs better not have dynamic shadows.. due to geometric or level design characteristics..
	 * It is TMap for searching, bool key field is not so significant.
	 * Key is supposed to have the rule of CombinedKeyName of FCombinedUniqueSPWaveID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<FName, bool> DynamicShadowExcludeWaveNameKeys;

private:

#if WITH_EDITOR // No UPROPERTY here
	// Preset values for EWorldDefaultPlayCameraPreset. 
	FWorldDefaultPlayCameraSettings DefaultCameraPreset_StandardTopDown;
	FWorldDefaultPlayCameraSettings DefaultCameraPreset_StandardBackView;

	TSubclassOf<class AGameModeBase> PreviousDefaultGameMode;
#endif

public:

	// Sets default values for this actor's properties
	ABladeIIWorldSettings(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostLoad() override;

	/** Invokes other actor's BeginPlay at level start. */
	virtual void NotifyBeginPlay() override;

	virtual void BeginPlay() override;

	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	void InitDefaultPlayerCameraSetting();

	///** To be back to player top-down camera with world default camera setting at any goddamn moment.
	// * It will use default object value if DefaultPlayerCameraSettings override none. */
	void ApplyDefaultPlayerCameraSetting(class ABladeIIPlayerController* InB2PC, float BlendTime = 0.0f, EViewTargetBlendFunction BlendFunc = VTBlend_Cubic, bool bNoSetViewTarget = false);

	void ApplyDefaultPlayerCameraSettingForPlayerPawn(class ABladeIIPlayer* targetPlayer);

	void ApplyPlayerLightSettingForPlayerPawn(class ABladeIIPlayer* targetPlayer);

	FText GetLocalizedAreaNameText();
	FText GetLocalizedAreaSubNameText();

	bool IsDynamicShadowDisallowedWave(int32 InCurrClientStage, int32 InCurrDifficulty, int32 InCheckWaveNum) const;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	/** Just use the same one for the Editor. Need both DevTest** and Editor** naming for clearness. */
	FORCEINLINE int32 GetDevTestStageNumber() { return EditorStageNumber; }
	FORCEINLINE int32 GetDevTestStageDifficulty() { return EditorStageDifficulty; }
	FORCEINLINE bool GetDevTestIsScenarioMode() { return bDevTestScenarioMode; }
#endif

#if WITH_EDITOR
	FORCEINLINE int32 GetEditorStageNumber() { return EditorStageNumber; }
	FORCEINLINE int32 GetEditorStageDifficulty() { return EditorStageDifficulty; }
	/** For FWorldDefaultPlayCameraSettings::LoadPresetValues */
	FString GetIniKeyStringPrefixOfDefaultPresetEnum(EWorldDefaultPlayCameraPreset InPresetEnum);
	virtual void PreEditChange(FProperty* PropertyThatWillChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif // WITH_EDITOR
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// Better not do this kind of crap unless for a development purpose feature.
	friend class ATEST_RaidGameMode;
#endif
};
