// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "GameFramework/Actor.h"
#include "Camera/PlayerCameraManager.h"
#include "Navigation/PathFollowingComponent.h"
//#include "B2SimpleEventScene.h"
#include "CommonStruct.h"
#include "BladeIICharacter.h"
//#include "B2UIEnum.h"
//#include "B2StageInfo.h"
//#include "BladeII.h"
#include "B2StageEventDirector.generated.h"

/** Overall behavior of stage event director actor, regarding not just camera. */
UENUM(BlueprintType)
enum class EStageEventDirectorOverallMode : uint8
{
	ESEDOM_ExportedCamera,

	ESEDOM_ControlledMatinee,

	ESEDOM_ManualCamera
};


/** Defines event types for a stage. It can be moving to other place with overall stage management implementation. */
UENUM(BlueprintType)
enum class EStageEvent : uint8
{
	ESEV_Begin, // Stage begin typically for the player

	ESEV_MobAppear,
	ESEV_MobDying,
	ESEV_MobPhase, // Invoked when a certain mob's phase is just changed (to MobEventMobPhase).

	ESEV_MobWaveClear,

	// Invoked when the player has kicked the shit out. Notified by StageManager (and possibly originated from MonsterSpawnPool)
	ESEV_Clear,

	// Invoked after Clear only when either one of player character has gone through level-up during last play.
	//ESEV_LevelUp, -> Once implemented, but now just a part of Clear event, LevelUp just invokes LevelUp UI which is also a part of StageResult UI.

	// MatchResult/ESEV_MatchOnlySuccessResult/ESEV_MatchOnlyDefeatResult (originally for PVP result) is specially separated from ESEV_Clear, as losing player also need to see the winning player, as well as the consideration of remote character stuff.
	ESEV_MatchResult,
	ESEV_MatchResultOnlyWinning,
	ESEV_MatchResultOnlyDefeat,

	// Invoked when the player touches a component belongs to this actor, specified by TriggerComponentName
	ESEV_ComponentTrigger,

	// Kill Last Enemy(for PVP, Team Match)
	ESEV_FinishAttack,

	// Should begin by CustomBeginShow call from Blueprint.
	ESEV_Custom

	// Check GetDefaultDesiredPlayActorSESState for additional entry.
};

UENUM()
enum class EStageEventSkipType : uint8
{
	SEST_CompleteIgnore, 
	SEST_LeastSkip, // It means that you can't see event scene, but much process will be done internally to sync PlayActors to their intended post-event location.
	SEST_Play
};

/** EStageEvent::ESEV_Clear is divided according to this.. */
UENUM(BlueprintType)
enum class EStageClearEventPhase : uint8
{
	ESCE_Phase1,
	ESCE_Phase2
};

/** Defines Widget types for a stage event.*/
UENUM(BlueprintType)
enum class EStageEventWidgetType : uint8
{
	None					UMETA(DisplayName = "None"),
	StageBegin				UMETA(DisplayName = "StageBegin"),
	StageClear				UMETA(DisplayName = "StageClear"),
	MobAppearBossStage		UMETA(DisplayName = "MobAppearBossStage"),
	MobAppearBossArea		UMETA(DisplayName = "MobAppearBossArea"),
	PVPBegin				UMETA(DisplayName = "PVPBegin"),
	PVPResult				UMETA(DisplayName = "PVPResult"),
	RaidBegin				UMETA(DisplayName = "RaidBegin"),
	RaidResult				UMETA(DisplayName = "RaidResult"),
	GuildBegin				UMETA(DisplayName = "GuildBegin"),
	GuildFinishBattle		UMETA(DisplayName = "GuildFinishBattle"),
	GuildResult				UMETA(DisplayName = "GuildResult"),
	ControlIntro			UMETA(DisplayName = "ControlIntro"),
	Max,
};

/** To specify the character type of playing actor hosting a show. The real "actor" here. */
UENUM(BlueprintType)
enum class EStageEventPlayActor : uint8
{
	ESEE_CurrentPlayer, // Could be main or sub, just currently playing class.
	ESEE_OtherPlayer, // Could be main or sub, the opposite of currently playing class.

	ESEE_MainPlayer,
	ESEE_SubPlayer,

	ESEE_WinningPlayer_Current, // Originally for PVP mode winner (current active one), no matter if it is local or remote player.
	ESEE_WinningPlayer_Other, // Current inactive (tag) one of winning side.
	ESEE_RemoteOpponent_Current, // PVP mode opponent, the current active one.

	ESEE_TeamAllyPlayer,
	ESEE_TeamEnemyPlayer,
	ESEE_TeamWinningPlayer,

	ESEE_WaveMob,

	ESEE_SetUpOutside,

	ESEE_RaidPlayer,
	ESEE_WinningRaidPlayer,
	ESEE_DefeatedRaidPlayer,

	//////////////////////////////
	// Tutorial setup has fixed player character class, not by user selection.
	ESEE_Tutorial_Gladiator,
	ESEE_Tutorial_Assassin,
	ESEE_Tutorial_Wizard,
	ESEE_Tutorial_Fighter,
	//////////////////////////////


	// Check GetRelevantPlayActor and GetDefaultDesiredPlayActorSESState for additional enum entry.
	// as well as IsPlayActorTypeForPlayerCharacter for PC type.

	ESEE_End
};

/** This number of director cameras are prepared to be utilized. Not only here, but also at other places like BladeIIPlayer. */
const int32 MAX_DIRECTOR_CAMERA = 3;

/** Selection of pre-installed camera set, when you don't use matinee. */
UENUM(BlueprintType)
enum class EManualCameraSelection : uint8
{
	// Use pre-installed camera at StageEventDirector actor
	EDCT_DirectorActorCamera,

	// To be added, like PlayerCamera, and possibly BossMobCamera.. then modify GetManualCameraFromSetting

	EDCT_End
};

/** Spatial relationship between this actor and/or camera component to the play actor. */
UENUM(BlueprintType)
enum class EPlayActorPlacement : uint8
{
	/** Either leave the play actor at the current location, or at the location set by SimpleEventScene */
	EPAP_NoAdjust,

	/** Ignore other settings. Manually bring play actor to specific relative loc/rot to the stage event director actor. */
	EPAP_BringPlayActor,

	/** Place a certain camera component or the director actor based on the play actor.
	 * Almost opposite to the BringPlayActor, but it does not ignore other settings like ExportedEventScene (SimpleEventScene). */
	EPAP_AdjustToPlayActor,

	/** Almost like EPAP_AdjustToPlayActor, but additionally tracks the play actor's movement while show is playing.
	 * It is useful when the play actor is a mob and is movable by bMaintainInGameState option.
	 * Effective only for the ManualCamera mode, and just the same as AdjustToPlayActor for ExportedCamera mode. */
	EPAP_ContinuouslyAdjustToPlayActor,

	/** For controlled matinee only, Same to EPAP_AdjustToPlayActor except that only play actor's location is used */
	EPAP_AdjustToPlayActorLocationOnly
};

/** Settings for a single pre-installed camera, necessary when you use one of EManualCameraSelection, not the matinee or cameraanim. */
USTRUCT(BlueprintType)
struct FManualCameraSingleSetting
{
	GENERATED_USTRUCT_BODY()

	FManualCameraSingleSetting()
	{
		CameraIndex = 0;
		BlendTimeFromPrev = 1.0f; 
		BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
		StayTime = 0.0f;
	}

	/** Index within camera array. 0 ~ MAX_DIRECTOR_CAMERA - 1 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	int32 CameraIndex;

	/** Blend time to be taken from previous camera to the camera of this setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	float BlendTimeFromPrev;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction;

	/** Time to stay at this camera view after blending to this camera is completed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	float StayTime;
};

UENUM(BlueprintType)
enum class EPlayActorStateMode : uint8
{
	/** Some sort of prepared animation is supposed to be played. In ControlledMatinee, it means a SkeletalMeshActor replaces the in-game character. */
	EPASM_Directed,

	/** Almost same to Directed, for the Controlled Matinee only however, in this case, SkeletalMeshActor controlled by Matinee track will be visible from the first place. */
	EPASM_DirectedFromBeginning,

	/** Almost same to Directed, for the Controlled Matinee only however, in this case, SkeletalMeshActor controlled by the Matinee track appears to end. */
	EPASM_DirectedAfterFinish,

	/** Almost maintains in-game state, no SkeletalMeshActor, but the character is not moving and not attacking either. */
	EPASM_InGameNonMoving,

	/** Simply maintains in-game state. */
	EPASM_MaintainInGame
};

/** For additional naming of player character's interp group */
UENUM(BlueprintType)
enum class EPCInterpGroupNetType : uint8
{
	PCIGNT_Local, // For single stage mode play.

	PCIGNT_SuperPeer, // The one who has some authority, in the case of PVP, the one who entered first.
	PCIGNT_CommonPeer,

	// Might need CommonPeer2, 3.. some day?

	PCIGNT_End
};

UENUM(BlueprintType)
enum class EEndActionPlayActorAdjustType : uint8
{
	EAPAAT_AdjustToPuppet,
	EAPAAT_BackToOriginal,
	EAPAAT_NoAdjust,
};

// Those name are to be attached at the rear of interp group name to find which track the player character should use
#define INTERPGROUP_SUFFIX_SUPERPEER TEXT("P1")
#define INTERPGROUP_SUFFIX_COMMONPEER TEXT("P2")
#define FOLDERGROUP_PREFIX TEXT("Folder_")
#define INTERPGROUP_SUFFIX_RAND TEXT("Rand%d")

/**
 * Contains main settings for event directing, per a character (call it PlayActor here)
 */
USTRUCT(BlueprintType)
struct FPlayActorShowSettings
{
	GENERATED_USTRUCT_BODY()

	FPlayActorShowSettings()
	{
		bOverallMode_ExportedCamera = false;
		bOverallMode_ControlledMatinee = true;
		bOverallMode_ManualCamera = false;
		bOverallMode_ExportedOrManualCamera = false;
		bPlayActorType_WaveMob = false;
		bUsePlayActorIndex = false;

		PlayActorType = EStageEventPlayActor::ESEE_End;
		PlayActorIndex = 0;
		//ExportedEventScene = NULL;
		PlayActorPlacement = EPlayActorPlacement::EPAP_NoAdjust;
		//StageEventStateOverride = EStageEventShowState::ESES_None;
		PlayActorStateMode = EPlayActorStateMode::EPASM_Directed;
		EndActionPlayActorAdjustType = EEndActionPlayActorAdjustType::EAPAAT_AdjustToPuppet;
		ActorScaleOverride = 0.0f;
		ManualRelativeLocation = FVector(0.0f, 0.0f, 0.0f);
		ManualRelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		StartDelay = 0.0f;
	}
	
	// For editcondition. No need to directly referencing them (in fact, don't do that)
	UPROPERTY()
	uint32 bOverallMode_ExportedCamera : 1;
	UPROPERTY()
	uint32 bOverallMode_ControlledMatinee : 1;
	UPROPERTY()
	uint32 bOverallMode_ManualCamera : 1;
	UPROPERTY()
	uint32 bOverallMode_ExportedOrManualCamera : 1;

	UPROPERTY()
	uint32 bPlayActorType_WaveMob : 1;

	UPROPERTY()
	uint32 bUsePlayActorIndex : 1;

	/** Type of PlayActor hosting this show. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EStageEventPlayActor PlayActorType;

	/** Return Whether PlayActorType is TeamType*/
	FORCEINLINE bool IsTeamPlayActorType(){ return PlayActorType == EStageEventPlayActor::ESEE_TeamAllyPlayer || PlayActorType == EStageEventPlayActor::ESEE_TeamEnemyPlayer || PlayActorType == EStageEventPlayActor::ESEE_TeamWinningPlayer; }

	/** Return Whether PlayActorType is RaidType*/
	FORCEINLINE bool IsRaidPlayActorType(){ return PlayActorType == EStageEventPlayActor::ESEE_RaidPlayer || PlayActorType == EStageEventPlayActor::ESEE_WinningRaidPlayer || PlayActorType == EStageEventPlayActor::ESEE_DefeatedRaidPlayer; }

	/** Index of PlayActor. Start Index is 0 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bUsePlayActorIndex"))
	int32 PlayActorIndex;

	/** When PlayActorType is WaveMob, specifies the wave number and mob index like "WaveNum MobIdx", e.g "0 1". */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bPlayActorType_WaveMob"))
	FString WaveMobSelectKeyword;

	/** Sets movement data exported from matinee. No direct player location/rotation control once using it.
	 * Supposed to be working in Exported CameraAnim mode with the director camera of CameraAnimApplyCamIndex. 
	 * The initial location of active camera applied for CameraAnim will be the base location for this scene. (or in the other way, play actor can be the base) */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedCamera"))
	//class UB2SimpleEventScene* ExportedEventScene;
	
	/** Options to locate play actor or the director actor/camera component. The actual behavior can be different by overall mode selection. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EPlayActorPlacement PlayActorPlacement;

	/** Basically, the StageEventState will be set to hard-coded value set according to the context (GetDefaultDesiredPlayActorSESState).
	 * You can set this to other than ESES_None to override the StageEventState for this show, to play any other wanted animation. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedOrManualCamera"))
	//EStageEventShowState StageEventStateOverride;

	/** For controlled matinee, the interp group name for the play actor of this setting.
	 * It could be just the expected group name, or the common name excluding certain postfix. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	FName MatchingInterpGroupName;
		
	/** Sets whether the PlayActor is acting as in-game character or playing directed animation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EPlayActorStateMode PlayActorStateMode;
	
	FORCEINLINE bool IsDirectedMode() { return (PlayActorStateMode == EPlayActorStateMode::EPASM_Directed || PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning); }

	FORCEINLINE bool IsOneOfAdjustToPlayActorPlacement() {return (
		PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActor || 
		PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor || 
		PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActorLocationOnly);}

	/**
	 * Put the play actor back to its original location after the show is over.
	 * For the Controlled Matinee, it is rather "sync" to the group actor puppet, but the same result anyway.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EEndActionPlayActorAdjustType EndActionPlayActorAdjustType;

	/** For ControlledMatinee bound SkeletalMeshActor. 
	 * If you set this bigger than 0, SkeletalMeshActor scale will be set to this value, instead of getting it from PlayActor. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	float ActorScaleOverride;

	/** PlayActor's local location to StageEventDirector's location, when PlayActorPlacement is EPAP_BringPlayActor and possibly for EPAP_AdjustToPlayActor too */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedOrManualCamera"))
	FVector ManualRelativeLocation;

	/** PlayActor's local rotation to StageEventDirector's rotation, when PlayActorPlacement is EPAP_BringPlayActor and possibly for EPAP_AdjustToPlayActor too */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedOrManualCamera"))
	FRotator ManualRelativeRotation;

	/** Additional delay from the moment of overall show begin. Not for controlled matinee */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedOrManualCamera"))
	float StartDelay;	
};

/** Native runtime state for each FPlayActorShowSettings */
USTRUCT()
struct FPlayActorShowRuntimeState
{
	GENERATED_USTRUCT_BODY()

	FPlayActorShowRuntimeState()
	{
		ThePlayActor = NULL;
		bIsForLocalPlayerCharacter = false;
		CtrlMatineePuppet = NULL;
		bShowStarted = false;
		bShowFinished = false;
		BackupWorldLocation = FVector(0, 0, 0);
		BackupWorldRotation = FRotator(0, 0, 0);
	}
	
protected:
	/** Cached PlayActor found by PlayActorType. It can be NULL.
	 * -> Be careful that in some circumstances, ThePlayActor reference might become invalid after event scene. Still not known well yet.. */
	UPROPERTY(Transient)
	class ABladeIICharacter* ThePlayActor;
	/** If this is for current main player character. Then you just use GetB2LocalPlayerChar instead of ThePlayActor. */
	uint32 bIsForLocalPlayerCharacter : 1;
public:
	/** GetPlayActor can "legally" returns NULL. */
	class ABladeIICharacter* GetPlayActor(class AB2StageEventDirector* OwnerSED);
	void SetPlayActor(class ABladeIICharacter* InPlayActor);
	FORCEINLINE bool IsForLocalPlayerCharacter() { return bIsForLocalPlayerCharacter; }

	/** For controlled matinee only, This is a SkeletalMeshActor deployed on the map as a matinee group actor.
	 * PlayActor might get sync to this Puppet's location after the show, depend on the setting. */
	UPROPERTY(Transient)
	class ASkeletalMeshActor* CtrlMatineePuppet;

	uint32 bShowStarted : 1;
	uint32 bShowFinished : 1;

	/** To restore the location and rotation after show finish 
	 * if PlayActorShowSettings has RelativeLocation/Rotation with bManuallyBringPlayActor and bPutPlayActorBackToOriginalLocation checked */
	FVector BackupWorldLocation;
	FRotator BackupWorldRotation;

	FTimerHandle ShowStartDelayTimerHandle;

};

/** Struct to support placement editing using preview component. Mapping between the preview component and the setting that the component represents. */
USTRUCT()
struct FSEDPreviewCompInfo
{
	GENERATED_USTRUCT_BODY()

	FSEDPreviewCompInfo()
	{
		SKComp = NULL;
		ShowSettingIndex = 0;
	}

	UPROPERTY(Transient)
	class USkeletalMeshComponent* SKComp;

	/** The show setting index that the SKComp represents */
	int32 ShowSettingIndex;
};

/** Data struct to cache temporary character reference (about to be destroyed soon..) to use dying mob in delayed mob dying event. */
USTRUCT()
struct FDelayedDyingMobCache
{
	GENERATED_USTRUCT_BODY()
	
	FDelayedDyingMobCache()
	{
		TheDyingMob = NULL;
		WaveIndex = -1;
		ObjIndex = -1;
	}

protected:
	/** Watch out for GC on this.. */
	UPROPERTY(Transient)
	class ABladeIICharacter* TheDyingMob;

	// Number of wave and obj from its birthplace (spawn pool)
	int32 WaveIndex;
	int32 ObjIndex;

public:
	void SetDyingMobCache(class ABladeIICharacter* InDyingMob, int32 InWaveIndex, int32 InObjIndex);
	void ForgetIt(); // When you done with it, or TheDyingMob has been destroyed for final..
	void ForgetIfMatch(class ABladeIICharacter* InMobToForget);
	class ABladeIICharacter* GetIfMatch(int32 InWaveIndex, int32 InObjIndex);
};

/**
 * To support various settings for a single stage event, such as camera work, animation play, special widget, and possibly matinee play too.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API AB2StageEventDirector : public AActor
{
	GENERATED_BODY()
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRootComponent;

#if WITH_EDITORONLY_DATA
private :
	/** Billboard Component displayed in editor */
	UPROPERTY()
	class UBillboardComponent* EditorSpriteComponent;

#endif
	
	UPROPERTY(Transient)
	class ABladeIIGameMode* CachedGameMode;
	class ABladeIIGameMode* GetB2GameMode();

	/** The stage manager that this actor is registered. StageId will come from here. 
	 * Available only for B2StageGameMode. It can be null in other GameMode. */
	UPROPERTY(Transient)
	class AB2StageManager* CachedStageManager;

	/** Copied from StageManager. Not sure if really needed. */
	int32 CurrentStageId;
	EStageDifficulty CurrentStageDifficulty;

	////////////////////////////////////////////////////////////////////////////////
	// [NOTE] Assumes a single event cut.
	// ...
	////////////////////////////////////////////////////////////////////////////////

protected:

	/** Some properties and functionalities are restricted by this selection. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EStageEventDirectorOverallMode OverallMode;

	// For editcondition. No need to directly referencing them (in fact, don't do that)
	UPROPERTY()
	uint32 bOverallMode_ExportedCamera : 1;
	UPROPERTY()
	uint32 bOverallMode_ControlledMatinee : 1;
	UPROPERTY()
	uint32 bOverallMode_ManualCamera : 1;
	UPROPERTY()
	uint32 bOverallMode_ExportedOrManualCamera : 1;
	
	/** For which event this StageEventDirector actor is handling? or it might be just a trigger condition. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EStageEvent DirectingEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bCheckPlayOne : 1;

	/** Valid only for Clear event, for now it is transient state flag. */
	EStageClearEventPhase ClearEventPhase;

	/** Similar stuff like bOverallMode_**. True for ESEV_MobAppear and ESEV_MobDying. */
	UPROPERTY()
	uint32 bDirectingEvent_MobEvent : 1;
	UPROPERTY()
	uint32 bDirectingEvent_MobDying : 1;
	UPROPERTY()
	uint32 bDirectingEvent_MobPhase : 1;
	UPROPERTY()
	uint32 bDirectingEvent_ComponentTrigger : 1;

	/** One of event activate conditions, especially regarding the GameMode. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	EStageEventManageMode ManageMode;
	/** If true, even when this event is not played due to ManageMode condition, BeginShow will be executed for PlayActor sync stuff. 
	 * -> For now, no plan for least skip. Only complete ignore.. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	//uint32 bManageModeLeastSkip : 1;

	/** Stage numbers that this event is played. Empty list will be played for any stages. (Possibly for stage-based GameMode) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	TArray<int32> SupportedStages;
	/** Almost like SupportedStages, the numbers here are difficulty level, instead of stage number. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	TArray<int32> SupportedDifficulties;

	/**  
	 * By which mob (object) of MonsterSpawnPool that the MobAppear and Dying (and Phase too) event will be invoked?
	 * In addition, it is required to identify the major (boss) mob to give information to the widget, even when the DirectingEvent is not one of Mob event.
	 * The same syntax as WaveMobSelectKeyword, "WaveNum MobIndex", e.g "0 1"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	FString MobEventMobSelectKeyword;

	/** Valid only for ESSV_MobPhase DirectingEvent, it specifies at which phase advance (1 ~ ) this event will be invoked.
	 * Must be bigger than 0, as 0 phase is just the default phase. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bDirectingEvent_MobPhase"))
	int MobEventMobPhase;

	/** The name of a collision (shape) component to begin show when DirectingEvent is ComponentTrigger */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bDirectingEvent_ComponentTrigger"))
	FName TriggerComponentName;

	/** The main director settings per a play actor casted for this show. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	TArray<FPlayActorShowSettings> ShowSettings;
	
	/** Must have the same number of elements to ShowSettings, even when a show cannot be started due to any condition. */
	UPROPERTY(Transient)
	TArray<FPlayActorShowRuntimeState> ShowStates;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Widget")
	EStageEventWidgetType EventWidgetType;

	/** Start from the first moment with 0 or negative value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Widget")
	float EventWidgetStartTime;
	/** Last until the end with 0 or negative value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Widget")
	float EventWidgetEndTime;

	/** If you set this, the show can begin at least after the show hosted by any one of these has finished. 
	 * DirectingEvent is still valid condition, so must meet both conditions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Time")
	TArray<class AB2StageEventDirector*> DependentDirectorActors;

	/** Time to taken from the supposed event moment to the actual show begin. Might be useful for clearing event. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Time")
	float OverallStartDelay;

	/** Setting it to positive value will constraint the length of the show, 
	 * or the show length will depend on animation and camera work length, etc.
	 * @REMARK : It is counted as game time, i.e. affected by time dilation if any. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Time")
	float SpecifiedOverallLength;

	/** If true, the show will not be finished after prepared animation and camera works are done.
	 * Intended for standby for user input. 
	 * Not work if SpecifiedOverallLength is set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Time")
	uint32 bStandbyAtTheEnd : 1;

	/** If true, StandbyAtTheEnd makes world paused. Only UIs will be animated. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Time", meta = (EditCondition = "bStandbyAtTheEnd"))
	uint32 bStandbyPause : 1;

	/** Optional time dilation to be applied globally, if you don't use time dilation by matinee or animnotify.
	 * This is still an asset setting. i.e. User play rate will be applied additionally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Time")
	float GlobalTimeDilation;

	////////////////////////////////////////////////////////////////////////////////

	/** CameraAnim asset for bUseCameraAnim */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedCamera"))
	//class UCameraAnim* CameraAnim;

	/** The camera index within DirectorCamera set, that the CameraAnim is based on. Negative or out of range value will use player default camera.
	 * The camera of this index is also the base for play actor's relative loc/rot (or in the other way) when SimpleEventScene is used. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedCamera"))
	int32 CameraAnimApplyCamIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ExportedCamera"))
	float CameraAnimInitialBlendingTime;

	//UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = ( EditCondition = "bOverallMode_ControlledMatinee" ) )
	//class AB2AutoWayPoint* ForcedMarkPassedWaypoint;

	/** For controlled matinee mode, the matinee actor created by Add Matinee menu, or deployed from Matinee Data (InterpData) */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	//class ALevelSequenceActor * MatineeActor;

	/** Blending time for final transition to the player camera after the show is over.
	* Not only for the ManualCamera, but also for CameraAnim */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	float FinalCameraRestoreBlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	FName ShowBeginEventName;

	/** Optional custom event name which will be invoked in addition to the default events according to the DirectingEvent. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	FName ShowOverEventName;

	/**
	 * If checked in ControlledMatinee mode, it will move player character to the first movement key frame of skeletal mesh track
	 * representing the current player character, before the matinee play begins.
	 * Nothing will be done regarding this if there's no skeletal mesh track matching to current player character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	uint32 bPCSyncToMatineeStart : 1;

	/** 
	 * If checked in ControlledMatinee mode, in-game active director track will be set according to the current player character.
	 * Matinee must be prepared with multiple director tracks, with their names having certain postfix for each player character,
	 * just like MatchingInterpGroupName
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	uint32 bPCDependentDirectorTrack : 1;

	/**
	 * If being set larger than 1, it assumes the assigned matinee is largely divided by two groups, which can be randomly selected.
	 * You must configure the matinee group and tracks according to specific rule and naming to make it really works.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	int32 RandomMatineeGroupNum;

	/** It would be meaningful only when the player character is not the part of event scene. 
	 * It won't hide the skeletal mesh actor which is suppose to represent the player character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bTryHidePlayer : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bForceNoHidePlayer : 1;

	/** Hide HUD (UI widget) while playing the event show. The widget created by EventWidgetClass will appear anyway. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bHideHud : 1;

	/** Disable player input while playing the event show. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bDisableInput : 1;

	/** Turns off the movable point light attached to the local player character, which is the default dynamic point light of BladeII game stage. 
	 * When this event scene has its own light control, this better be checked.
	 * The total number of movable point lights affecting an object must not exceed r.MobileNumDynamicPointLights */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bHidePlayerPointLight : 1;
	/**
	 * It is effective only when HidePlayerPointLight is false.
	 * By default, point light of player characters other than the current main local player character is not allowed, to limit total movable point lights.
	 * Use it with care..
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent")
	uint32 bAllowOtherPlayerPointLight : 1;

	/**
	 * Transient setup to safely fix lighting setups done under wrong functionality.
	 * Per PCClass light group selection was not working before fixing a specific bug (http://172.168.2.105:8091/browse/B2BUG-712)
	 * However, most lighting setups are done under the buggy functionality, so it takes time to fix them all.
	 * It is meant to be checked once fixing the lighting setup to new environment..
	 * and finally to be removed.
	 */
	UPROPERTY()
	uint32 bHackMyPerPCClassLightingSetupDone_DEPRECATED : 1;

	/** 
	 * CAUTION!
	 * It actually switches local player character when there are two player character show settings.
	 * Use with extreme care!
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	uint32 bLocalPlayerSwitching : 1;
	
	/** If set, the game-play camera defined by this ActiveCameraActor is applied after the show is over,
	 * regardless of the overlap condition */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc")
	//class AB2ActiveCameraActor* ActiveCameraForceOnFinish;

	/** If current player character is not one of the play actors of ShowSettings, 
	 * this will enforce the player character to stop attack motion, which can cause various undesired behavior. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc")
	uint32 bForceStopPlayerAttackState : 1;

	/** If checked, all mobs (event if it is not a part of directed scene) in the world stop moving (and attacking too) 
	 * while the stage event scene is played. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc")
	uint32 bStopAllMobsWhilePlaying : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc", meta = (EditCondition = "bStopAllMobsWhilePlaying"))
	uint32 bForceCancelAttackMotionAllMob : 1;

	///** You may specify level to load/unload at the beginning of this event.
	// * Better not include any level that this actor or referenced matinee is placed. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc")
	//FB2StageStreamingSettings LevelStreamingOnBegin;

	///** You may specify level to load/unload at the end of this event.
	// * Better not include any level that this actor or referenced matinee is placed. */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc")
	//FB2StageStreamingSettings LevelStreamingOnEnd;

	/** Set it to some small value like 1 ~ 2 can reduce some noticeable frame skip when the scene is to be started at level beginning. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_Misc", meta = (EditCondition = "bOverallMode_ControlledMatinee"))
	int32 MatineeFirstSkipFrames;

	////////////////////////////////////////////////////////////////////////////////

	/** Which camera set will be used? */
	//-> Probably just stick to DirectorActorCamera.. not editable until other sets are introduced
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_ManualCamera", meta = (EditCondition = "bOverallMode_ManualCamera"))
	EManualCameraSelection ManualCameraSelection;

	/** For director camera, make elements up to number of MAX_DIRECTOR_CAMERA */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_ManualCamera", meta = (EditCondition = "bOverallMode_ManualCamera"))
	TArray<FManualCameraSingleSetting> ManualCameraSettings;

	// Pre-installed director cameras. Declared individually just to expose it. 
	// In native, refer to them by DirectorCamArrayRef array
	// They are not supposed to be moving in-game. Even with camera anim, it does not actually move the location of the camera component.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_ManualCamera")
	class UCameraComponent* DirectorCamera_0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_ManualCamera")
	class UCameraComponent* DirectorCamera_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageEvent_ManualCamera")
	class UCameraComponent* DirectorCamera_2;

	////////////////////////////////////////////////////////////////////////////////
	
	/** To assign DirectorCameras in array structure for native reference. */
	UPROPERTY(Transient)
	class UCameraComponent* DirectorCamArrayRef[MAX_DIRECTOR_CAMERA];

	/** It could be one of DirectorCameras here, or could be other cameras installed at other character classes by ManualCameraSelection. */
	UPROPERTY(Transient)
	class UCameraComponent* CurrentActiveCamera;

private: 
		
	FTimerHandle OverallStartDelayTimerHandle;
	FTimerHandle OverallLengthTimerHandle;
	FTimerHandle ManualCameraBlendTimerHandle;
	FTimerHandle StartEventWidgetTimerHandle;
	FTimerHandle FinishEventWidgetTimerHandle;
	FTimerHandle PCSyncToMatineeStartFallbackTimerHandle;
	FTimerHandle LevelStreamingOnBeginFlushTH;
	FTimerHandle LevelStreamingOnEndFlushTH;

	/** Condition caching variables to support deferred begin show by DependentDirectorActor */
	uint32 bDirectingEventConditionMet : 1;
	uint32 bDependentDirectorActorMet : 1;

	/** One of flags designating a part of show has been finished, when OverallMode is ManualCamera. */
	uint32 bAllManualCameraEnded : 1;
	/** One of flags designating a part of show has been finished, when OverallMode is ExportedCamera. */
	uint32 bCameraAnimFinished : 1;
	uint32 bControlledMatineeFinished : 1;

	/** Another flag designating a part of show (UI) has been finished */
	uint32 bRelevantUIOpenComplete : 1;
	/** It is set only when my event is started and UI is open. */
	FName MyRelevantUIName;

	/** To be set in valid range at beginning moment by RandomMatineeGroupNum, if RandomMatineeGroupNum is larger than 1. */
	int32 RandomlySetMatineeGroupNum;
	/** See if RandomMatineeGroupNum is set for actual random group selection and final random number has been selected in valid range. */
	FORCEINLINE bool IsValidRandomMatineeGroupNumSet() const { return (RandomMatineeGroupNum > 1 && RandomlySetMatineeGroupNum >= 1 && RandomlySetMatineeGroupNum <= RandomMatineeGroupNum); }

	/** To use either main/sub character different from CachedPlayerCharacter. It must be destroyed after everything is done here. 
	 * In normal settings, no more than 1 element is expected, but just made it as an array for any unexpected case.. */
	UPROPERTY(Transient)
	TArray<class ABladeIIPlayer*> TempSpawnedPuppets;
	
	uint32 bCurrentlyPlaying : 1;

//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//	/** Cached flag at local config file, per-stage basis. It could be used for some purpose like preventing play it more than twice depend on user setting. */
//	bool bPlayedAtLeastOnce;
//	/** Requires to be in different package file name, to be uniquely identified. */
//	FORCEINLINE FString GetPlayedAtLeastOnceSectionString() { return FString::Printf(TEXT("/Script/BladeII.%s"), *GetFullName()); }
//	FORCEINLINE FString GetPlayedAtLeastOnceKeyString(int32 PlayStageNum) { return FString::Printf(TEXT("PlayedAtLeastOnce_%d"), PlayStageNum); }
//#endif
	/** A little bit different from bPlayedAtLeastOnce. To prevent re-entrance once it is played through for this stage. Gotta be loaded once more.. */
	uint32 bPlayedOnceForThisStage : 1;

	/** Being cached to be the reference when SimpleEventScene is used 
	 * -> Might not need, as director camera components are not supposed to be moving. */
	//FVector CachedInitialActiveCamLocation;
	//FRotator CachedInitialActiveCamRotation;

	/** They are like SpawnPoolEditingActor, basically PlayActor preview, and additionally support placement edit for some play actor placement option.
	 * Editing native components in dynamic array is possible by modification in FComponentEditorUtils::CanEditNativeComponent. */
	UPROPERTY(Transient, EditAnywhere)
	TArray<class USkeletalMeshComponent*> PreviewSKComps;
	/** Mapping between PreviewSKComps and show setting. */
	UPROPERTY(Transient)
	TArray<FSEDPreviewCompInfo> PreviewMapping;
	/** Info Text displayed on PreviewSKComps */
	UPROPERTY(Transient)
	TArray<class UTextRenderComponent*> PreviewSKCompsInfoText;

	/** Another info text for director camera */
	UPROPERTY(Transient)
	TArray<class UTextRenderComponent*> DirectorCameraNumberText;

	UPROPERTY(Transient)
	class UTextRenderComponent* ActorNameDisplayText;

	UPROPERTY(Transient)
	TArray<class UTextRenderComponent*> AnyShapeCompNameText;

	/** Transient flag for bPCSyncToMatineeStart operation in ControlledMatinee mode. */
	uint32 bCurrentPCSyncToMatineeStart : 1;

	/** True only when it is match result event and I am the winner (i.e. local player has won, instead of remote player) */
	uint32 bMatchResultEventAsWinner : 1;

	/** To cached the attacking state and restore it back. */
	uint32 bWasPlayerAttacking : 1;

	UPROPERTY(Transient)
	ABladeIICharacter* SetUpOutSideCharacter;

	/** A cache data struct to barely support MobDying event with delay. */
	UPROPERTY(Transient)
	FDelayedDyingMobCache DelayedDyingMobCache;
	FORCEINLINE bool NeedDelayedDyingMobCache() const { return (DirectingEvent == EStageEvent::ESEV_MobDying && OverallStartDelay > 0.0f); }

	/** For disable/enable handling for pre-render.. a special case. */
	//UPROPERTY()
	//TArray<class UInterpTrack*> TracksToRestoreForPreRender;

	bool bEnterCinematicMode;

public:	
	// Sets default values for this actor's properties
	AB2StageEventDirector(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;
	virtual void PostRegisterAllComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EssentialBeginHandlingCommon();

	int32 GetAllowedMatineeTrackPointLightNum() const;

	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	virtual void BeginDestroy() override;

	// Called every frame. 
	virtual void Tick( float DeltaSeconds ) override;
	
	void NotifyGameStarted(); // From GameMode's HandleMatchHasStarted

	/** Notified for any kind of game clear/victory for current game mode. -> However, some game type might not use this. */
	void NotifyGameClear();
	/** Separated from NotifyGameClear, due to a bit special result scene of PVP mode. */
	void NotifyMatchEnd(bool bAmIWinnder);
	void NotifyFinishAttack(ABladeIICharacter* Inflictor);
	/** Notified from GameMode for some spawn pool event. */
	void NotifySpawnPoolSingleWaveStart(int32 WaveNum);
	void NotifySpawnPoolSingleWaveComplete(int32 WaveNum);
	void NotifySpawnPoolSingleMobDead(class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobObjIndex);
	void NotifySpawnPoolSingleMobPhaseChange(class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase);

	void NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed);

	/** Notification of UI page opening completion, related to one of stage event. */
	void NotifyStageEventUIOpenComplete(FName InOpenedUI);

	/** To handle child component touch event for ComponentTrigger. */
	UFUNCTION()
	void SelectedComponentTriggerBeginOverlapCB(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** To begin show at any custom moment (at least after BeginPlay). */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void CustomBeginShow();

	/** To finish show at any custom moment. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void CustomFinishShow();

	/** Check if cannot begin the show by wrong setting or any kind of context.
	 * You cannot use this for checking user setting of turning off the event scene. It might be impossible to play through the stage if stage event scene is completely turned off. 
	 * Skipping stage event scene by user preference still requires BeginShow */
	bool CanPlayShow();

	/** See BeginShow and FinishShow for this condition check. */
	FORCEINLINE bool IsPlayingShow() { return bCurrentlyPlaying; }

	/** It allows play event again without re-loading the world */
	void ResetAssumeNoWorldLoading();
	
	/** Returns proper skip type according to its ManageMode flag and current game context.
	 * It is static for use in other stage event related object. e.g. DialogTriggerActor. 
	 * @param bLeastSkip : Probably for StageEventDirector only. */
	static EStageEventSkipType CheckSkipForManageMode(UObject* WorldContextObject, EStageEventManageMode InManageMode, bool bLeastSkip);

	static bool IsAdventureModeGame(UObject* WorldContextObject); // See if current gamemode is for adventure mode.

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetVisibilityMatineePuppetPointLight(bool bVislble);

	bool IsExpectedForGameMode(class ABladeIIGameMode* InB2GM);
	bool IsStageSupportedForGameMode(class ABladeIIGameMode* InB2GM) const;
private:
	bool IsStageSupported(int32 InStageId, EStageDifficulty InDifficulty) const;
		/** Subsets of IsStageSupported */
		bool IsStageNumSupported(int32 InStageId) const;
		bool IsStageDifficultySupported(EStageDifficulty InDifficulty) const;
	/** It's so stupid that I have no freaking idea why there's mixed use of EStageDifficulty and int everywhere. */
	FORCEINLINE bool IsStageSupported(int32 InStageId, int32 InDifficulty) const { return IsStageSupported(InStageId, static_cast<EStageDifficulty>(InDifficulty)); }

	bool IsPossibleAdventureModeBeginEvent(); // A little hack for B2 event scene setting.

	/** Main entry point to kick off the show. Use BeginShowOrBeginCountDown for normal event call */
	void BeginShow();

	/** Part of BeginShow handling, which need to be done after the actual show is started.
	 * In the case of PCSyncToMatineeStart, BeginShow does not start the actual show right at the moment, so this need to be called later. */
	void DeferredBeginShowHandlingForMatineePCSync();

	void ApplyStageEventTimeDilation();
public:// For stable even scene time dilation apply.
	FORCEINLINE void ApplyStageEventTimeDilationIfPlaying() { if (IsPlayingShow()) { ApplyStageEventTimeDilation(); } }
private:

	/** Wrapped BeginShow to handle OverallStartDelay and DependentDirectorActor */
	void BeginShowOrBeginCountDown();
	bool HasValidDependentDirectorActorSetting();

	/** Timer callback to handle StartDelay property of FPlayActorShowSettings */
	void DelayedShowStartTimerCB(int32 PlayActorIdx);

	/** To handle AdjustToPlayActor/ContinuouslyAdjustToPlayActor placement option for ManualCamera mode.
	 * @param bInPositionOnly : True does not adjust rotation. */
	void AdjustSelfPlacementToPlayActor(class ABladeIICharacter* InPlayActor, FVector& InPlayActorRelativeLocation, FRotator& InPlayActorRelativeRotation, bool bInPositionOnly = false);

	/** Kick off a single show in ShowSettings hosted by a PlayActor of index */
	bool BeginSingleShow(int32 PlayActorIdx);

	/** Subroutine of BeginSingleShow, for controlled matinee. */
	bool SetupSingleShowMatineeGroup(FPlayActorShowSettings& InSettingForThis, FPlayActorShowRuntimeState& InStateForThis);

	/** Need to do some more than just calling visibility on/off function, so we go some util here. */
	void SetVisibilityOfMatineePuppet(ASkeletalMeshActor* InPuppet, class ABladeIICharacter* InRelevantPlayActor, bool bVisible);
	/** Some dirty special handling of particle system components, which seems not being activated in simple manner.. */
	void ActivateMatineePuppetPSCs(ASkeletalMeshActor* InPuppet, class ABladeIICharacter* InRelevantPlayActor);

	AActor* GetGroupActorOfGroupName(const FName& InGroupName);

	/** Select one proper director track from MatineeActor for bPCDependentDirectorTrack or RandomMatineeGroupNum. */
	void SelectFinalDirectorTrack();

	/** Remove GroupActor from InterpGroup which is not desired by current game context, 
	 * probably for the player class selection, and some more.. */
	void RuntimeRemoveNonDesiredGroup();

	/** Sub-functionality of RuntimeRemoveNonDesiredGroup. */
	void RuntimeDisableInterpGroup(class UInterpGroup* GroupToDisable, class ALevelSequenceActor * OwnerMatinee);
	
	/**
	* [poikl2212] copy from Matinee.h
	*
	* Locates the specified group's parent group folder, if it has one
	*
	* @param ChildGroup The group who's parent we should search for
	*
	* @return Returns the parent group pointer or NULL if one wasn't found	
	*/
	//UInterpGroup* FindParentGroupFolder(UInterpGroup* ChildGroup, class ALevelSequenceActor * OwnerMatinee) const;

	/** Returns final matinee interp group name to search. Could be the same as RawInterpGroupName or might have some postfix attached. */
	FString GetFinalMatchingInterpGroupName(class ABladeIICharacter* InPlayActor, FString RawInterpGroupName, bool bForFolderGroup = false);
	FString GetFinalMatchingInterpGroupNamePC(FString RawInterpGroupName, FString TrimmedPCClassEnumName, EPCInterpGroupNetType NetType = EPCInterpGroupNetType::PCIGNT_Local, bool bForFolderGroup = false);
	FString GetFinalMatchingInterpGroupNameRandom(FString RawInterpGroupName, int32 RandNum, bool bForFolderGroup = false); // The RandNum here supposed to start from 1.
#if WITH_EDITOR
	bool IsPossibleInterpGroupNameForPC(const FString& InCheckName) const;
#endif

	void SetAllPointLightGroupAffectWorld(bool bAffectWorld);
	void HackSpareLeastPointLight();

	/** To check all relevant Matinee groups. PC settings will have more than one OutGroupNames elements. */
	void GetAllSupposedInterpGroupNamesForSetting(TArray<FName>& OutGroupNames, FPlayActorShowSettings& InSetting);

	/** Bundling a couple of steps.. Call it when things are ready. */
	void FinalBeginMatineePlay();

	/** Switching the local player to the final rendering character in Matinee **/
	void SwitchingLastRenderingPlayer();
	/** Sub functionality of LocalPlayerSwitching */
	void GetShowStatesOfPlayerChar(TArray<FPlayActorShowRuntimeState*>& OutFoundStates);
	
	void EnsureHidePlayActorsForControlledMatinee();
	void EnterCinematicModeForControlledMatinee();
	void SetAllPlayerHiddenInGame(bool bForceHide);

	/** Check condition for matinee start by bPCSyncToMatineeStart option. It does some initial settings to start synching procedure too.
	 * If returns true, sync before start play, if false, start right at the moment. */
	bool CheckForPCSyncToMatineeStart();

	/** Begin matinee when PC sync by bPCSyncToMatineeStart is done, usually by callback */
	void BeginMatineeFromPCSyncToMatineeStart();

	/** Callback from PathFollowingComponent */
	void OnPCSyncToMatineeStartCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	/** In the case of path fining not work properly for PC sync, we additionally set a timer to force begin matinee. */
	void PCSyncToMatineeStartFallbackTimerCB();

	/** Find the wanted play actor defined by EStageEventPlayActor, could spawn one in the case of main/sub player character. 
	 * WaveMobSelectKeyword is used only when InType is WaveMob. */
	class ABladeIICharacter* GetRelevantPlayActor(EStageEventPlayActor InType, FString& WaveMobSelectKeyword, int32 PlayerIndex = 0);

	class ABladeIIPlayer* GetTutorialPlayActor(EStageEventPlayActor InType);

	/** Returns sub character if currently playing main character, and returns main character if currently playing sub character.
	 * In any way, it returns a transient puppet, not being possessed by player controller. */
	class ABladeIIPlayer* GetOtherTagClassPuppet();
	class ABladeIIPlayer* GetRemoteOtherTagClassPuppet();

	class UCameraComponent* GetManualCameraFromSetting(FManualCameraSingleSetting& InSetting);

	/** Check all conditions if show is about to end, then set the final finish timer. 
	 * Gate to finish the show in most normal conditions. Some settings directly use CustomFinishShow though. */
	void CheckShowEndCountDown();

	/** Now, controlled matinee also required a bit complicated finish check.. */
	void CheckShowEndCountDown_ControlledMatinee(bool bForStandByNoti);

	/** Returns if an individual show with ShowIndex requires finish check to finish the whole show, 
	 * when the show length is not bound by SpecifiedOverallLength */
	bool DoesShowRequireFinishCheck(int32 ShowIndex);

	/** Finish the show and go back to normal play. It does not do all the necessary stuff to finish the show at any moment.
	 * Use CustomFinishShow for that use. */
	void FinishShow();
	void PostFinishShow();

	/** Finish handling of each play actor for controlled matinee mode, restore the visibility, and possibly some sort of position sync stuff..? */
	void RestorePlayActorForCtrlMatineeEnd(FPlayActorShowSettings& InSettingForThis, FPlayActorShowRuntimeState& InStateForThis);
	
	//////////////////////////////////////////////////
	// End notifications from each mode and/or settings to trigger the overall show end check.

	/** Called when the animation of one play actor driven by StageEventShowState is finished. */
	void OnSingleStageEventShowOver(int32 ActorShowIndex);

	/** Called by matinee's custom stop notification, for ControlledMatinee 
	 * bStandByNoti comes when bStandbyAtTheEnd is used. */
	void OnControlledMatineeStop(bool bStandByNoti);

	/** Called by CameraAnim's custom stop notification, for ExportedCamera */
	void OnExportedCameraAnimStop();

	void OnAllManualCameraOver();
	
	//////////////////////////////////////////////////

	/** One of show over handling. Ensure return to game play camera, either the player top-down camera or camera of ActiveCamera..
	 * bDeactivateOtherCams : Set it to true only for special occasion. You might not want to set it to true for correct blending out from this actor to player. 
	 * If you set it to false in that case, camera transition will start from this actor's root, not the last active camera component. */
	void ReturnToGamePlayCamera(bool bDeactivateOtherCams = false);

	/** Deactivate all camera components, not only those installed at this actor, but also those in other actor working for this one. */
	void DeactivateAllManualCams();

	/** Begin transition to the director camera setting of CamSettingIndex, within ManualCameraSettings array.
	* It will also do the camera work finish check. */
	void BeginTargetToManualCamera(int32 CamSettingIndex);

	void OverallStartDelayTimerCB();
	void OverallLengthTimerCB();

	/** Called when camera transition to current one is completed, with the setting index within ManualCameraSettings array. */
	void ManualCameraBlendCB(int32 CamSettingIndex);
	/** Called when camera stayed for specified StayTime after transited to the current one, with the setting index within ManualCameraSettings array. */
	void ManualCameraBlendStayCB(int32 CamSettingIndex);

	///** Set the InPlayActor's location and rotation based on SimpleEventScene's key data and camera anim applied camera's location and rotation. 
	// * InMoveKey is relative loc/rot data from play actor to the camera. */
	//void SetSESPlayActorMovementFromCamera(FSimpleEventSceneMoveKeyData& InMoveKey, class ABladeIICharacter* InPlayActor);
	///** Set the camera anim applied camera's location and rotation based on SimpleEventScene's key data and designated InPlayActor's location and rotation.
	// * InInvMoveKey is relative loc/rot data from camera to play actor. */
	//void SetSESCameraMovementFromPlayActor(FSimpleEventSceneMoveKeyData& InInvMoveKey, class ABladeIICharacter* InPlayActor);

	bool IsExportedEventSceneValid(class UB2SimpleEventScene* InEventScene);

	/** Get EStageEventShowState enum value used for animation play state handling, relevant to the stage event and play actor type. 
	 * StageEventStateOverride of ShowSetting can override the result value from here. */
	EStageEventShowState GetDefaultDesiredPlayActorSESState(EStageEvent InEvent, EStageEventPlayActor InPlayActorType);

	void SyncDataToUIDoc(ABladeIICharacter* InEventTriggeringMob); // Call this probably at beginning phase. Not for realtime use.
public:
	static bool IsStageEventRelatedUI(FName InUI);
private:
	/** Overall StartEventWidget handling. */
	void StartEventWidgetOrStartCountdown();

	void StartEventWidget();
	void FinishEventWidget();
	
	void StartEventWidgetTimerCB();
	void FinishEventWidgetTimerCB();

	FORCEINLINE bool IsOverallLengthConstrainted() { return (SpecifiedOverallLength > 0.0f); }

	bool IsPlayActorTypeForPlayerCharacter(EStageEventPlayActor InType) const;
	bool IsPlayActorTypeTutorialFixedClass(EStageEventPlayActor InType) const;
	EPCClass TutorialPlayActorTypeToPCClass(EStageEventPlayActor InType) const;
	
	/** See if a show setting uses ManualRelativeLocation/Rotation. OverallMode also affect the result. */
	bool DoesThisSettingRequireManualPlacement(FPlayActorShowSettings& InSetting);

	/** Returns if the setting use current gameplay location of play actor. 
	 * It could be intentional or just mistake. OverallMode also affect the result. */
	bool DoesThisSettingHaveNoFixedPlacement(FPlayActorShowSettings& InSetting);

public:
	
	/** Called by a play actor when its event show animation triggers SESEndNotify
	 * Might not be available for controlled matinee mode, or if SESEndNotify is not used for any reason. */
	void NotifyStageEventShowOver(class ABladeIICharacter* PlayActorJustOver);

	/** To get end notification from (Controlled) Matinee.. */
	virtual void NotifyActorCustomEvent(FName OptionalEventName = NAME_None, UObject* OptionalNotifyingObject = NULL);
		
	/** To handle begin condition by DependentDirectorActor */
	void NotifyShowOverFromDependentDirectorActor();

	bool IsOneOfDependentDirectorActor(class AB2StageEventDirector* CheckSED);

private:
	void LevelStreamingOnBeginFlushCB();
	void LevelStreamingOnEndFlushCB();

	/** Notification of show end when StandbyAtTheEnd is used. 
	 * In this case, it still does not return to the game. */
	void OnStartStandbyAtTheEnd();
	
	bool IsPlayerCharacterOneOfPlayActor();
	bool ShouldMaintainPlayerCharacterState();

public:
	bool ShouldStopAutoDuringShow() const;
	bool ShouldApplyCinematicScalabilityDuringShow() const;

private:
	bool ShouldBeInvincibleDuringShow() const;
	bool HasDirectedFromBeginningTrack(); // Has any show setting having PlayActorStateMode as EPASM_DirectedFromBeginning
public:
	void SetupPreRenderSKMeshActor(class ASkeletalMeshActor* InOutDefaultSKActorToSetup, TArray<ASkeletalMeshActor*>& OutNewCreatedSKActors);
	void TemporaryDisableTracksForPreRender();
	void RestoreTracksAfterPreRender();
	/** Set between 0.0 ~ 1.0, regardless of the length */
	void ForceSetMatineePlayingPosScale(float InPositionScale);

	void LoadLazyLoadedMatineeRefAssets(const TArray<FString>& ExcludeGroupNameFilters);

	/** The opposite of EditorGetMatineeGroupActorsFromShowSetting.. This is necessary for PreRender Setup. */
	void GetMatineeGroupActorsNotBoundtoShowSetting(TArray<AActor*>& OutGroupActors);

	void GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode);

	/** Override to get selected camera component in the editor, not just the first one. */
	virtual UActorComponent* FindComponentByClass(const TSubclassOf<UActorComponent> ComponentClass) const;

	UActorComponent* FindComponentByName(FName& InComponentName) const;

	FORCEINLINE void SetStageManager(class AB2StageManager* InManager) { CachedStageManager = InManager; }
	FORCEINLINE EStageEvent GetDirectingEvent() { return DirectingEvent; }
	FORCEINLINE EStageClearEventPhase GetClearEventPhase() { return ClearEventPhase; }
	FORCEINLINE FName GetMyRelevantUIName() { return MyRelevantUIName; }

	class ABladeIIPlayer* GetB2LocalPlayerChar();
	class ABladeIIPlayerController* GetB2LocalPlayerCon();
	class ABladeIINetPlayer* GetB2MatchOpponent();
	class ABladeIIPlayer* GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly = false, bool bSpawnPuppetForDead = false);
	/** It will return the player character which is likely to be highlighted depend on setting. It could return NULL. */
	class ABladeIIPlayer* GetShowHeroPlayerIfAny();
	class ABladeIIPlayer* GetRaidPlayer(int32 nPlayerIndex, bool bLiveOnly = false);

#if WITH_EDITOR

	//
	// Preview component support for editing purpose
	//

	/** Regenerate preview components (optionally just relocate them without regeneration), 
	 * set their placement and other info, etc. */
	void RefreshPreviewSKComps(bool bRegenerateSKComp = true);

private:
	/** Adjust SkeletalMeshActors connected to the MatineeActor for some convenience, and else..? */
	void SetupPreviewForControlledMatinee();

	/** Get the array of actors connected to matinee group relevant to the InSetting.
	 * Works only for ControlledMatinee mode. */
	void EditorGetMatineeGroupActorsFromShowSetting(FPlayActorShowSettings& InSetting, TArray<AActor*>& OutGroupActors);

	/** Set ManualRelativeLocation/Rotation from preview components depend on the placement option. */
	void SyncFromPreviewSKCompPlacement();
	/** For component's unique name representing ShowSetting. */
	FString GetPreviewSKCompNameForSetting(int32 ShowSettingIndex);
	FString GetPreviewInfoTextCompNameForSetting(int32 ShowSettingIndex);
	/** Return desired PCClassInfo asset if InType is for player character. */
	class UB2PCClassInfo* GetPCClassInfoForPlayActorType(EStageEventPlayActor InType);

	/** Try getting matching skeletal mesh asset and suggested transform.
	 * Returns true if the setting is valid for editor preview. 
	 * The result OutTransform does not include z offset originated from pivot difference between character and skeletal mesh. 
	 * In the case of ShowSetting requiring manual placement, better to apply the z offset. 
	 * OptionalSpecificPCClassMesh is used when you want to get information for Matinee's specific Skeletal group actor. Other than that, you may pass NULL for that.
	 */
	bool GetEssentialPreviewInfoForSetting(FPlayActorShowSettings& InSetting, class USkeletalMesh* OptionalSpecificPCClassMesh,
		class USkeletalMesh*& OutSKMesh, FTransform& OutTransform);
	
	/** Destroy preview purpose skeletalMeshComponents and TextComponents. 
	 * If bDestroyAll is false, it does not destroy PreviewSKComps (for use in the case of just moving them while recreating others). */
	void DestroyPreviewComponents(bool bDestroyAll);

	/** Ensure matinee connected skeletal mesh actors to be invisible in game, and having no collision. */
	void SafetyProcessMatineeSkActors();

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
private:
	/** Ensure setting editcondition properties properly */
	void SetEditConditionProperties();
public:
	virtual void PostEditUndo() override;
	virtual void PostEditMove(bool bFinished) override;
	
	void CheckAndWarnForLightSetup();
	//UInterpGroup* GetInterpGroupOfObjectName(FName InFindGroupObjectName);

	static void CheckAndWarnForComponentTriggerSetup(UWorld* InWorld);
	static bool HasAnyDirectedFromBeginningTrackInCurrentWorld(UWorld* InWorld);

#endif
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
public:
	static bool bDisableStageEventLights;
#endif
};
