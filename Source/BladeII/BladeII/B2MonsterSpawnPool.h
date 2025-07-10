// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "Animation/SkeletalMeshActor.h"
#include "CommonStruct.h"
#include "BladeIICharacter.h"
#include "B2NPCClassInfo.h"
#include "B2NPCClassInfoBox.h"
#include "B2StageInfo.h"
#include "B2MonsterSpawnPool.generated.h"

typedef TSubclassOf<class ABladeIICharacter> BladeIICharacterClass;

/**
* Provided for specific spawn class designation. Spawn class can be specified by this one or FMonsterSpawnTypeSpecific
*/
USTRUCT(BlueprintType)
struct FMonsterSpawnClassInfo
{
	FMonsterSpawnClassInfo()
	{
		CharacterClassEnum = ENPCClass::ENC_End;
		ClassVariation = ENPCClassVariation::ENCV_Normal;
		CharacterClassBP = NULL;
		MinSpawnObjIndex = -1;
		MaxSpawnObjIndex = -1;
		RewardGoldWeight = 1;
		bCanDropItem = false;
		bNetPlay = false;
		bFixedLocation = false;
	}

	GENERATED_USTRUCT_BODY()

	/**
	 * Character class defined by NPC Class Info.
	 * If you specify this other than ENC_End, this has priority over the CharacterClassBP.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ENPCClass CharacterClassEnum;

	/** Variation within the selected CharacterClassEnum */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ENPCClassVariation ClassVariation;

	/** 
	 * If you want to use a custom class, use this direct Blueprint overriding.
	 * Works only if CharacterClassEnum is ENC_End.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class ABladeIICharacter> CharacterClassBP;

	/** To define the range of object index that will be spawned as this class type within 0 ~ SpawnNum - 1. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 MinSpawnObjIndex;

	/** To define the range of object index that will be spawned as this class type within 0 ~ SpawnNum - 1.
	* Negative value gives all objects to have a chance to use this class. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 MaxSpawnObjIndex;

	/** Weight reward gold of mobs spawned by this Wave */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 RewardGoldWeight;

	/** Can mob drop item? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	uint32 bCanDropItem : 1;

	/** Spawn character for network playing*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	uint32 bNetPlay : 1;

	/** Make this monster non-movable, allows rotations only. It is not only like zero walk speed, also ignores root-motions too.
	 * A flag typically expected for a big-fatty-ass boss mob. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	uint32 bFixedLocation : 1;

	/** Material override for each mesh element. Array index matches to the element index.
	 * Nulled element won't be overridden. 
	 * If this MaterialOverrides element number gets over the base mesh element number, redundant materials are to be set to attached components, in order. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<class UMaterialInterface*> MaterialOverrides;
};

/**
 * Pre-defined information for type-based spawning.
 * An element of UB2MonsterClassTypeMap.
 */
USTRUCT(BlueprintType)
struct FMonsterTypeMapSingleInfo
{
	FMonsterTypeMapSingleInfo()
	{
		CharacterClassEnum = ENPCClass::ENC_End;
		ClassVariation = ENPCClassVariation::ENCV_Normal;
		CharacterType = ECharacterType::ECT_MeleeMob;
	}

	GENERATED_USTRUCT_BODY()

	/** Character class basic identification of NPC Class Info system. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ENPCClass CharacterClassEnum;

	/** Variation within the selected CharacterClassEnum */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ENPCClassVariation ClassVariation;

	/** Rough type to categorize the class definition here.
	 * The monster spawned using this information from MonsterSpawnPool will have its CharacterType to this value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ECharacterType CharacterType;

	/** Material override for each mesh element. Array index matches to the element index.
	 * Nulled element won't be overridden. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<class UMaterialInterface*> MaterialOverrides;
};

/**
 * To allow having a common spawn type map info among multiple maps.
 * You can prepare a derived BP class of this and put any wanted monster type-class mapping to be used in MonsterSpawnPool.
 * Monster spawn pool itself may also has its own FMonsterTypeMapSingleInfo array setting instead of prepared UB2MonsterClassTypeMap asset.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2MonsterClassTypeMap : public UObject
{
	GENERATED_BODY()

public:

	UB2MonsterClassTypeMap(const FObjectInitializer& ObjectInitializer) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FMonsterTypeMapSingleInfo> InfoArray;
};

/**
 * For use of FMonsterTypeMapSingleInfo for wave character spawn
 */
USTRUCT(BlueprintType)
struct FMonsterSpawnTypeSpecific
{
	FMonsterSpawnTypeSpecific()
	{
		CharacterType = ECharacterType::ECT_MeleeMob;
		MinSpawnObjIndex = -1;
		MaxSpawnObjIndex = -1;
		RewardGoldWeight = 1;
		bCanDropItem = false;
	}

	GENERATED_USTRUCT_BODY()

	/** Rough types to categorize the class */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ECharacterType CharacterType;

	/** To define the range of object index that will be spawned as this class type within 0 ~ SpawnNum - 1. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 MinSpawnObjIndex;

	/** To define the range of object index that will be spawned as this class type within 0 ~ SpawnNum - 1.
	* Negative value gives all objects to have a chance to use this type. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 MaxSpawnObjIndex;

	/** Weight reward gold of mobs spawned by this Wave */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 RewardGoldWeight;

	/** Can mob drop item? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	bool bCanDropItem;
};


/**
 * Contains main settings for each wave.
 */
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolWaveSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolWaveSettings()
	{
		SpawnNum = 5;
		SpawnNumRandomSlack = 0;
		SpawnAreaComponentName = NAME_None;
		bRandomSpawnLocation = false;
		WaveExec_FromBeginning = false;
		WaveExec_PostCompleteWaveNum = -1;
		FinalSpawnDelay = 0.0f;
		FinalSpawnDelay_ShortCut = 0.0f;
		WaveExec_TriggerComponent = NAME_None;
		WaveCompl_TerminateAll = false;
		bBossWave = false;
		bCanReuseWave = false;
	}

	/** Setting this will constrain the character class which can be created from this wave. 
	 * Multiple classes can be specified for random select and/or for different settings per certain object index range. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnClassInfo> SpawnClasses;

	/** Set the class by rough type (category). This won't work if you have specified SpawnClasses.
	 * You also need to add some elements to SpawnTypeInfo at B2MonsterSpawnPool actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnTypeSpecific> SpawnCharacterTypes;

	/** How many monsters appear in this wave? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 SpawnNum;

	/** If you set this to positive value, the actual spawn number in runtime can be randomly reduced to this amount.
	 * like: (SpawnNum - SpawnNumRandomSlack) ~ SpawnNum */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 SpawnNumRandomSlack;

	/**
	 * If true, each actor's spawn location is randomly set inside of SpawnAreaComponentName.
	 * Otherwise, need to provide each actor's spawn location using the editing actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	uint32 bRandomSpawnLocation : 1;
	
	/** Specify the name of the collision component where this wave location will be based, among the child components of MonsterSpawnPool actor. 
	 * Even when bRandomSpawnLocation is false, newly added entries will use the random locations based on this if not previously set.
	 * NAME_None uses the default collision (root). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName SpawnAreaComponentName;

	/** If true, it will be already populated from the beginning, regardless of other conditions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	uint32 WaveExec_FromBeginning : 1;
	
	/** Setting this to one of wave settings array index number will trigger this wave after completing the specified wave.
	 * Negative value disables this condition. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveExec_PostCompleteWaveNum;
	
	/** Setting this to one of SpawnPool's collision component trigger this wave when player hits the child collision having the same name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName WaveExec_TriggerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName Wave_AppointmentBGMName;


protected:
	/**
	 * For any WaveExec setting, specifies time delay in seconds between the condition meeting and final start of the wave.
	 * 0 or less value results instant starting.
	 * This works for SCENARIO mode (or story mode) where the stage event cut-scene of long version is to be played. Otherwise FinalSpawnDelay_ShortCut is used.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float FinalSpawnDelay;

	/**
	 * This is an ugly HACK HACK HACK HACK HACK HACK HACK HACK!!
	 * We should not set in-game cinematic playing time to FinalSpawnDelay from the first time, but there are too many so we can hardly think about a way to fix it in formal way.
	 * It is a big FUCKING SHAME, I am so sorry about this SHIT.
	 * This is delay \for non-scenario mode (regular adventure mode) where shorter version of stage event cut-scenes are to be played.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float FinalSpawnDelay_ShortCut;
public:

	FORCEINLINE float GetFinalSpawnDelay(bool bIsScenarioMode) const {
		// Intended to return different value by bScenarioMode flag of StageManager.
		return bIsScenarioMode ? FinalSpawnDelay : FinalSpawnDelay_ShortCut;
	}

	/** Terminate all waves if this wave ends. Suggested for a wave containing boss mob, but this setting itself does not guarantee that this wave is for a boss mob.
	 * When other active waves are terminated due to an wave having this option on, the other waves' completion events won't be called. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	uint32 WaveCompl_TerminateAll : 1;


	/** Check this render a mob spawned from this setting being considered as boss mob.
	 * Practically, a special HUD will be invoked to a mob spawned from this setting.
	 * As more than one boss mob can appear in a stage, the end of BossWave does not necessarily means the end of all waves.
	 * In many cases though, you might still want to check WaveCompl_TerminateAll too, another setting which might be suit for boss. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	uint32 bBossWave : 1;

	/** If true, this wave can be started again in the same stage. The spawn pool will be never completed until other specific condition is met. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	uint32 bCanReuseWave : 1;

	/** When bRandomSpawnLocation is false, edited locations of editing actors will be copied to this. Not for direct editing. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FTransform> FixedSpawnTransforms;
};

/** Minor version of WaveSettings, used by mobs which can summon other mobs */
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolSummonSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolSummonSettings()
	{
		SummonIndex = 0;
		SpawnNum = 1;
		MaxObjectByThisSetting = 3;
		TransientMySettingIndex = -1;
	}

	/** A sort of ID of summon setting. Summon notify will specify summon setting by SummonIndex.
	 * SummonIndex can imply monster's level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 SummonIndex;

	/** Setting this will constrain the character class which can be created from this summoning.
	 * Multiple classes can be specified for random select and/or for different settings per certain object index range. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnClassInfo> SpawnClasses;

	/** Set the class by rough type (category). This won't work if you have specified SpawnClasses.
	 * You also need to add some elements to SpawnTypeInfo at B2MonsterSpawnPool actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnTypeSpecific> SpawnCharacterTypes;

	/** How many monsters appear by this summoning? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 SpawnNum;

	/** The limitation of summoned mobs using this setting. 
	 * When this number of summoned mobs exist, summoning by this setting won't work by any summoner. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxObjectByThisSetting;

	/** Not for setting, for some hacky variable transfer.. */
	int32 TransientMySettingIndex;
};

/** Simply wrapped for use of SummonSetting array in TMap, by SummonSettingAsset */
USTRUCT(BlueprintType)
struct FWrappedSummonSettingsArray
{
	GENERATED_USTRUCT_BODY()

	FWrappedSummonSettingsArray(){}

	UPROPERTY(EditAnywhere)
	TArray<FMonsterSpawnPoolSummonSettings> DataArray;
};

/**
* We provide asset class having SummonSettings to allow having a common summon settings across multiple maps.
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2SummonSettingAsset : public UObject
{
	GENERATED_BODY()

public:
	UB2SummonSettingAsset(const FObjectInitializer& ObjectInitializer){}

protected:
	// Old data, when there's no difference per difficulty. To be removed some day..
	UPROPERTY()
	TArray<FMonsterSpawnPoolSummonSettings> InfoArray_DEPRECATED;

	/** Summon settings per difficulty level. i.e. Map key is target difficulty. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<int32, FWrappedSummonSettingsArray> PerDifficultyInfoMap;
public:
	const TArray<FMonsterSpawnPoolSummonSettings>* GetInfoArrayForDifficulty(EStageDifficulty InDifficulty) const;

	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void CheckDataIntegrity();
#endif
};

UENUM()
enum class EWaveCustomEventTiming : uint8
{
	EWCET_WaveStart,
	EWCET_WaveComplete
};

UENUM()
enum class ETalkEventTiming : uint8
{
	ETET_StageBegin,
	ETET_StageFinish
};

/**
* Just to keep the FMonsterSpawnPoolWaveSettings not too big..? Put custom event settings through this.
*/
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolWaveLevelCustomEventSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolWaveLevelCustomEventSettings()
	{
		WaveNum = -1;
		EventTiming = EWaveCustomEventTiming::EWCET_WaveComplete;
		CustomEventName = NAME_None;
	}

	/** The array index of the wave which is associated with the event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveNum;

	/** At which timing of wave the event will be invoked? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	EWaveCustomEventTiming EventTiming;

	/** The custom event name which will be invoked at a certain of wave with WaveNum */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName CustomEventName;
};

/**
 * Somewhat like WaveLevelCustomEvents, but in actor basis.
 */
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolWaveSignalActorSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolWaveSignalActorSettings()
	{
		WaveNum = -1;
		EventTiming = EWaveCustomEventTiming::EWCET_WaveComplete;
		SignalingActor = NULL;
		OptionalEventName = NAME_None;
	}

	/** The array index of the wave which is associated with the event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveNum;

	/** At which timing of wave the event will be invoked? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	EWaveCustomEventTiming EventTiming;

	/** The Actor reference that will call the custom event callback */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	class AActor* SignalingActor;

	/** You may designate event name which you will get by NotifyActorCustomEvent or BP's ActorCustomEvent.
	 * It's optional, so you may just use NAME_None if you don't have to worry about get confused by other events. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName OptionalEventName;
};


/**
 * Now, components enable/disable settings are done by separate struct too.
 */
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolWaveComponentOnOffSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolWaveComponentOnOffSettings()
	{
		WaveNum = -1;
		EventTiming = EWaveCustomEventTiming::EWCET_WaveComplete;
		bCheckForOnOrOff = false;
		ComponentName = NAME_None;
	}

	/** The array index of the wave which is associated with the event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveNum;

	/** At which timing of wave the event will be invoked? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	EWaveCustomEventTiming EventTiming;

	/** Get to be enabled if checked, disabled if unchecked. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	uint32 bCheckForOnOrOff : 1;

	/** The name of the component that will be enabled or disabled at specified timing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName ComponentName;
};

USTRUCT(BlueprintType)
struct FMonsterSpawnPoolTalkDialogSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 GroupID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	ETalkEventTiming EventTiming;
};

/**
* Open Dialog when stopping Wave or starting Wave / overlapping any comp
*/
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolWaveDialogSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolWaveDialogSettings()
	{
		ManageMode = EStageEventManageMode::SEMM_Always;
		WaveNum = -1;
		EventTiming = EWaveCustomEventTiming::EWCET_WaveComplete;
		DialogCodeName = NAME_None;
		bMaintainBattleUIs = false;
	}

	/** One of event activate conditions, especially regarding the GameMode. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStageEventManageMode ManageMode;

	/** The array index of the wave which is associated with the event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveNum;

	/** At which timing of wave the event will be invoked? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	EWaveCustomEventTiming EventTiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName TriggerComponentName;

	/** The Codename of Dialog will be enabled or disabled at specified timing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FName DialogCodeName;

	/** True will maintain in-game battle UIs visible */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bMaintainBattleUIs : 1;
};

/** Sublevel load/unload settings mainly for performane optimization */
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolWaveLevelStreamingSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolWaveLevelStreamingSettings()
	{
		WaveNum = -1;
		EventTiming = EWaveCustomEventTiming::EWCET_WaveComplete;
	}

	/** The array index of the wave which is associated with the event */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveNum;

	/** At which timing of wave the event will be invoked? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	EWaveCustomEventTiming EventTiming;

	/** The main setting here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FB2StageStreamingSettings StreamingSetting;

	FTimerHandle StreamingSettingFlushTH;
};

/** 
 * Not for serialization, just for runtime state tracking
 */
USTRUCT()
struct FMonsterSpawnPoolRuntimeWaveState
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolRuntimeWaveState()
	{
		bWaveStarted = false;
		bWaveCompleted = false;
	}

	/** Are they all spawned? */
	UPROPERTY(Transient)
	bool bWaveStarted;

	// Are all spawned monsters dead?
	UPROPERTY(Transient)
	bool bWaveCompleted;

	//Num of Mobs will spawn
	UPROPERTY(Transient)
	int32 FinalSpawnNum;

	/** Contains spawned actors in current wave. Reset for new wave. */
	UPROPERTY(Transient)
	TArray<class ABladeIICharacter*> FellowsInThisWave;
};

/** Like RuntimeWaveState, but for summoned mobs. */
USTRUCT()
struct FMonsterSpawnPoolSummonedFellowState
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolSummonedFellowState()
	{
		SummonSettingIndex = -1;
		Summoner = NULL;
		Summonee = NULL;
	}

	/** To track by which SummonSettings element the Summonee spawned, then to limit summoning number. */
	int32 SummonSettingIndex;

	/** The owner who casted summoning. */
	UPROPERTY(Transient)
	class ABladeIICharacter* Summoner;

	/** Summoned one by Summoner. */
	UPROPERTY(Transient)
	class ABladeIICharacter* Summonee;
};

/** To enable settings for multiple stages. */
USTRUCT(BlueprintType)
struct FMonsterSpawnPoolPerStageSettings
{
	GENERATED_USTRUCT_BODY()

	FMonsterSpawnPoolPerStageSettings()
	{
		TargetStageNum = 1;
		TargetDifficultyLevel = (int32)EStageDifficulty::ESD_Normal;
	}

	/** Which stage this setting cover? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 TargetStageNum;

	/** For which difficulty level (defined at StageInfo table) this setting for? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 TargetDifficultyLevel;	// 	todo : 조만간 EStageDifficulty로 변경될 예정 -> 바꾸려면 기존에 저장된 것들이 의도된 enum 값으로 잘 Serialize 되어야.. 기존에 enum 을 못 쓴건 난이도가 어떻게 확정될 지 모르는 상황에서 int 가 무난했기 때문.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolWaveSettings> WaveSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolWaveComponentOnOffSettings> WaveCompOnOffSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray <FMonsterSpawnPoolWaveLevelStreamingSettings> WaveLevelStreamingSettings;

	/** Optionally generate level custom events which will be invoked at some specific point of the wave. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolWaveLevelCustomEventSettings> WaveLevelCustomEvents;

	/** Like WaveLevelCustomEvents, optionally generate custom events for other actors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolWaveSignalActorSettings> WaveActorCustomEvents;

	/** Like WaveLevelCustomEvents, optionally open the dialog. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolWaveDialogSettings> WaveDialogEvents;
	

	/** Used by mobs to summon other mobs. This setting can be used repeatedly to dynamically spawn mobs at any moment. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolSummonSettings> SummonSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolTalkDialogSettings> TalkDialogSettings;
};

/**
 * Managing spawning monster, its type, wave, and relevant stage flow...
 */
UCLASS(BlueprintType)
class BLADEII_API AB2MonsterSpawnPool : public AActor
{
	GENERATED_BODY()


	uint32 bCheckedForBeginWave : 1; // Checked for wave of begin condition

	/** Default collision component, being a root component, as well as triggering stuff. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DefaultCollisionComponent;
	
#if WITH_EDITORONLY_DATA
private :
	/** Billboard Component displayed in editor */
	UPROPERTY()
	class UBillboardComponent* EditorSpriteComponent;
#endif

	/** The stage manager that this spawn pool is registered. StageNum will come from here.
	 * Available only for B2StageGameMode. */
	UPROPERTY(Transient)
	class AB2StageManager* CachedStageManager;

	// Cached FTimerHandle map to clear desired timer when timer callback is executed.
	TMap<int32, FTimerHandle> SpawnWaveDelayTHMap;

protected:

	/** Put most necessary settings through here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FMonsterSpawnPoolPerStageSettings> PerStageSettings;

public:

	/** To safely get the per-stage settings for any StageNum.
	 * It will try find setup for lower (easier) difficulty level by default. */
	const FMonsterSpawnPoolPerStageSettings* GetDesiredStageSetting(int32 InClientStageId, EStageDifficulty DifficultyLevel, bool bFallBackToLowerDifficulty = true);

	FORCEINLINE int32 GetStageSettingsNum() { return PerStageSettings.Num(); }

	/** To safely get the wave settings for current stage. */
	int32 GetStageWaveNum(int32 InClientStageId, EStageDifficulty DifficultyLevel);
	const FMonsterSpawnPoolWaveSettings* GetStageWaveSetting(int32 InClientStageId, EStageDifficulty DifficultyLevel, int32 WaveNum);

	/** Get all SummonSettings for ClientStageId, having matching SummonIndex */
	void GetAllStageSummonSetting(TArray<FMonsterSpawnPoolSummonSettings>& OutMatchingSettings, int32 InClientStageId, EStageDifficulty DifficultyLevel, int32 SummonIndex);

	/** Use this if you just want to iterate stage settings and get wave num for each stage settings. Provide this almost for use from outside. */
	int32 GetStageWaveNumBySettingIndex(int32 SettingIndex);
	int32 GetTargetStageIdBySettingIndex(int32 SettingIndex);
	int32 GetTargetDifficultyLevelBySettingIndex(int32 SettingIndex);

	void SetCurrentGameMode(class ABladeIIGameMode* gm)
	{
		CurrentGameMode	= gm;
	}

	/** See if this SpawnPool has PerStageSetting for designated StageNumer and Difficulty. */
	bool HasSetupFor(int32 InTestStageNum, EStageDifficulty InTestDifficulty) const;

protected:

	/** If true, no mobs will appear regardless of wave settings. Provided for test purpose. Not working for final release. 
	 * All the wave will be automatically completed right at the starting moment. */
	UPROPERTY(EditAnywhere, Category = "Wave")
	uint32 bDisableAllSpawn : 1;

	/** Start from 1. This will be given at beginning and not likely to be changed while actively running. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Wave")
	int32 CurrentClientStageId;
	/** Just like CurrentStageNum.. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Wave")
	int32 CurrentDifficultyLevel;
		
	/** Must have the same number of elements to WaveSettings of CurrentStage. */
	TArray<FMonsterSpawnPoolRuntimeWaveState> RuntimeWaveStates;

	/** Mob list spawned by SummonSetings. */
	UPROPERTY(Transient)
	TArray<FMonsterSpawnPoolSummonedFellowState> SummonedFellows;

	/** To prevent some unwanted operation. */
	bool bForceClearing;

	/** Their components are automatically generated for WaveSettings of CurrentEditingStage, to support editing work. 
	 * Even when they get serialized to the map, won't be with any actual data. */
	UPROPERTY(VisibleAnywhere, Category = "Wave")
	TArray<class AB2SpawnPoolEditingActor*> WaveEditingActors;


	/** Show if we have kicked shit out of here.
	 * Just for current stage. We don't provide if every stage is cleared within this class. */
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	bool bAllWaveDoneForCurrentStage;

	/** If true, you can define the OwnMonsterTypeMapInfo array only for this SpawnPool actor itself.
	 * Otherwise, need to specify one of B2MonsterClassTypeMap asset */
	UPROPERTY(EditAnywhere, Category = "Wave")
	bool bUseOwnTypeMapInfo;

	/** To use spawn control by type, instead of specific class define. This is ignored if bUseOwnTypeMapInfo is true. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<UB2MonsterClassTypeMap> TypeMapClassAsset;

	/** Like TypeMapClassAsset, this is used for spawn control by type, when you want to use type define for this actor only.
	 * This is available when bUseOwnTypeMapInfo is true. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave", meta = (EditCondition = "bUseOwnTypeMapInfo"))
	TArray<FMonsterTypeMapSingleInfo> OwnTypeMapInfo;

	/** You may have pre-created summon setting asset for use of this spawn pool. The SummonSettings of PerStageSetting override this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<UB2SummonSettingAsset> SummonSettingAsset;

	UPROPERTY(Transient)
	class ABladeIIGameMode*	CurrentGameMode;

	/** They are reserved at PreloadPoolMonsters. 
	 * NPCClass data of those IDs are not loaded at BeginPlay, and supposed to be loaded asynchronously afterward. */
	TArray<FCombinedNPCClassID> ReservedNPCClassIDsForAsyncLoad;

public:
	  
	// Sets default values for this actor's properties
	AB2MonsterSpawnPool(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;

	virtual void PostLoad() override;
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void BeginPlaySetup();
public:
	void ConditionalBeginWaveCheck();

	/** Check if any one of my PerStageSettings will be used for this loaded world, being decided and cached by BladeIIGameMode.
	 * Available after PreBeginPlay */
	bool AmIActiveForCurrentWorld();

private:
	/** Just simple destroying without any gameplay death handling. i.e not for clearing waves. */
	void DestroyAllActiveWaves();

public:

	/** Returns all npc IDs (class and variation) required by spawn settings. Mainly for preload info assets stuff. */
	void GetAllExpectedNPCIDs(TArray<FCombinedNPCClassID>& OutExpectedIDs);

	void GetWaveNPCIDs(const int32 WaveIndex, TArray<FCombinedNPCClassID>& OutNPCIDs);
	void GetSpawnNPCAssets(const int32 WaveIndex, TArray<FB2AsyncRequestInfo>& OutAssetStrings);

	void GetEssentialPreloadNPCIDs(TArray<FCombinedNPCClassID>& OutNPCIDs);

	void PreloadPoolMonsters();
	void TryAsyncLoadReservedMonsters(); // Async request of all remaining NPC data after PreloadPoolMonsters
	void TryAsyncLoadMonsters(int32 BeginWave, int32 EndWave = INDEX_NONE/* MaxWave */);
	void SetForcedPerStageSettings(int32 InClientStageNum, int32 InTargetDifficulty);

	const int32 GetBasicAsyncLoadBeginWave();

private:
	/** Internal functionality of GetAllExpectedNPCIDs, either for WaveSetting or SummonSetting */
	void GatherAllClassAndTypesFromSetting(TArray<FCombinedNPCClassID>& OutCombinedIDs, TArray<ECharacterType>& OutTypes,
		const FMonsterSpawnPoolWaveSettings* WaveSetting, const FMonsterSpawnPoolSummonSettings* SummonSetting);
public:
	/** Another pre-load stuff.. */
	void GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode);

	/** Use this if you want to start a wave by custom (blueprint) event.. */
	UFUNCTION(BlueprintCallable, Category = BladeII)
	bool CustomSpawnWave(int32 WaveNum);

	/** It allows restart wave from the beginning without re-loading the world */
	void ResetAssumeNoWorldLoading();

	/** Spawn mobs by SummonSettings having SummonIndex, around Summoner's location within MaxDistance. 
	 * It is like SpawnWave for SummonSettings */
	bool SummonMobs(class ABladeIICharacter* Summoner, int32 SummonIndex, float MaxDistance);
private:
	/** Sub functionality of SummonMobs */
	class ABladeIICharacter* SummonSingleMob(class ABladeIICharacter* Summoner, FMonsterSpawnPoolSummonSettings& InSetting, 
		int32 SummonObjIndex, float MaxDistance);

	/** Returns how many summoned mobs exist by SummonSettings of SummonSettingIndex. */
	int CountSummonedMobsBySetting(int32 SummonSettingIndex);

public:
	
	/** Called when any actors born from this spawn pool (including summoned one) is about to die. */
	void OnSpawnedMonsterDead(class ABladeIICharacter* DyingMonster, int32 DeadMobWaveNum, int32 DeadMobObjIndex, bool bIsSummonedMob);

private:
	
	/** Do the spawn operation for a specific wave. It won't do any if a wave of specified WaveNum is already started.
	 * @param bDeferWaveStartCallBack : By default (true) OnSingleWaveStarted is called inside SpawnWave if SpawnWave is successful. 
	 *			In some (or many) cases, it is desired to call OnSingleWaveStarted later due to multiple, simultaneous wave spawn.
	 *			Use this with care, and do not forget to call OnSingleWaveStarted anyway.. */
	bool SpawnWave(int32 WaveNum, bool bDeferWaveStartCallBack = false);

	struct FSpawnClassFromSetting_OutParams
	{
		FSpawnClassFromSetting_OutParams()
		{
			OutClassEnum = ENPCClass::ENC_End;
			OutClassVariation = ENPCClassVariation::ENCV_Normal;
			OutClassBP = NULL;
			OutCharacterType = ECharacterType::ECT_End;
			OutWeightRewardGold = 0;
			OutCanDropItem = false;
			OutNetPlay = false;
			bOutBossMob = false;
			bOutFixedLocation = false;
		}

		FSpawnClassFromSetting_OutParams(ENPCClass& InClassEnum,
										 ENPCClassVariation& InClassVariation,
										 TSubclassOf<class ABladeIICharacter>& InClassBP,
										 ECharacterType& InCharacterType)
		{
			OutClassEnum = InClassEnum;
			OutClassVariation = InClassVariation;
			OutClassBP = InClassBP;
			OutCharacterType = InCharacterType;
			OutWeightRewardGold = 0;
			OutCanDropItem = false;
			OutNetPlay = false;
			bOutBossMob = false;
			bOutFixedLocation = false;
		}

		ENPCClass OutClassEnum;
		ENPCClassVariation OutClassVariation;
		TSubclassOf<class ABladeIICharacter> OutClassBP;
		ECharacterType OutCharacterType;
		int32 OutWeightRewardGold;
		bool OutCanDropItem;
		bool OutNetPlay;
		TArray<class UMaterialInterface*> MaterialOverrides;
		bool bOutBossMob; // Almost for passing some parameter.
		bool bOutFixedLocation;
	};

	/** Provide a common interface both for wave spawn and summoning */
	class ABladeIICharacter* SpawnSingleMonsterCommon(FSpawnClassFromSetting_OutParams& InClassParam, FTransform& InSpawnTransform);

public:
	static void OverrideMaterialForNewSpawned(class ABladeIICharacter* NewSpawned, const TArray<class UMaterialInterface*>& MaterialOverrides
#if WITH_EDITOR
		, class USkeletalMeshComponent* InEditorMeshComp = nullptr // If specified as valid value, NewSpawned will be ignored.
#endif
	);
private:

	/** Returns a suggested monster class by reference parameter from given wave setting or summon setting, either in ENPCClass enum or BladeIICharacter BP class. 
	 * Either InWaveSetting or InSummonSetting is required. InWaveSetting is higher priority.
	 * OutCharacterType is optional result information available only when you use SpawnCharacterTypes, instead of specific class.
	 * Returns true if any class has been found in either way. */
	bool GetSpawnClassFromSetting(FSpawnClassFromSetting_OutParams& OutParams, const FMonsterSpawnPoolWaveSettings* InWaveSetting, 
		const FMonsterSpawnPoolSummonSettings* InSummonSetting, int32 SpawnObjIndex);

	/**  
	 * Main method to get the spawn transform based on the wave setting.
	 * SpawnObjIndex is used only when bRandomSpawnLocation is false.
	 */
	FTransform GetSpawnTransform(const FMonsterSpawnPoolWaveSettings* InWaveSetting, int32 SpawnObjIndex);

	/** Called right after monsters in a specific wave are spawned.. */
	void OnSingleWaveStarted(int32 WaveNum);
	void ExecuteWaveEventsCommonInternal(int32 WaveNum, EWaveCustomEventTiming InTiming);

	/** Check complete state of each wave, and also see if all have gone. */
	void CheckWaveCompletion();

	void EnsureCleanFellowList();

	/** Called when all monsters in a wave with specific number have died. */
	void OnSingleWaveComplete(int32 WaveNum);

	/** Component enabling routine for components being set as one of WaveCompOnOffSettings having bOnOrOff as true */
	void WaveEnableSingleComponent(FName ComponentName);
	/** Component disabling routine for components being set as one of WaveCompOnOffSettings having bOnOrOff as false */
	void WaveDisableSingleComponent(FName ComponentName);

	/** Called when you kicked shit out of this hell for current stage. */
	void OnAllWaveComplete();

	void WaveLevelStreamingFlushCB(FTimerHandle InUsedTimerHandle);

public:
	/** Called right after monsters in a specific wave are spawned.. */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void ExtraOnSingleWaveStarted_BP(int32 WaveNum);

	/** Called when all monsters in a wave with specific number have died. */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void ExtraOnSingleWaveComplete_BP(int32 WaveNum);

	/** Called when you kicked shit out of this hell for current stage. */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void ExtraOnAllWaveComplete_BP();

private:

	/** Kill spawned monsters and mark waves completed up to DesiredFinishWaveNum.
	 * If DesiredFinishWaveNum is negative, it will clear all waves. 
	 * It does not call OnSingleWaveComplete inside.. Call it manually if needed. 
	 * If bMarkCompleted is false, set bWaveCompleted manually or by CheckWaveCompletion afterward.  */
	void ForceClearWaves(int32 DesiredFinishWaveNum = -1, bool bMarkCompleted = true);

	void GetWaveLevelStreamingSettings(TArray<FMonsterSpawnPoolWaveLevelStreamingSettings>& OutSettings, EWaveCustomEventTiming EventTiming, int32 WaveNum);

	/** Get wave component on/off settings having matching conditions.
	 * Multiple settings can be found. */
	void GetWaveCompOnOffSettings(TArray<FMonsterSpawnPoolWaveComponentOnOffSettings>& OutSelected, EWaveCustomEventTiming EventTiming, int32 WaveNum);

	/** Simple querying custom event name with matching conditions. */
	void GetWaveLevelCustomEventName(TArray<FName>& OutEvents, EWaveCustomEventTiming EventTiming, int32 WaveNum);

	/** Get wave signaling actor settings having matching conditions.
	 * Multiple settings can be found. */
	void GetWaveSignalActorSettings(TArray<FMonsterSpawnPoolWaveSignalActorSettings>& OutSelected, EWaveCustomEventTiming EventTiming, int32 WaveNum);

	struct WaveDialogSettings_OutParam // Selected parameter only necessary for dialog invoking.
	{
		FName DialogCode;
		bool bMaintainBattleUIs;

		WaveDialogSettings_OutParam(){
			DialogCode = NAME_None;
			bMaintainBattleUIs = false;
		}
	};
	/** To Open Dialog by DialogCodeName, get the DialogCodeNames of matching condition. */
	void GetWaveDialogCodeName(TArray<WaveDialogSettings_OutParam>& OutDialogParams, EWaveCustomEventTiming EventTiming, int32 WaveNum);

public:
	/** Will be registered to all component which serve as a trigger to get the overlap notify */
	UFUNCTION()
	void OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** Will be registered to all component which serve as a trigger to get the overlap notify */
	UFUNCTION()
	void OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnComponentForDialogBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	void RegisterComponentOverlapCallback();
	void RemoveComponentOverlapCallback();

	/** Check if we got any wave that starts from the beginning. */
	void CheckNewWaveSpawnBeginning();

	/** Check the condition for a new wave when a wave is jsut done. */
	void CheckNewWaveSpawnOtherWaveComplete(int32 JustCompletedWave);

	/** Timer callback for SpawnDelayTimerHandle */
	void DelayedNewWaveSpawn(int32 WaveNum);

	/** Some common code block to remove duplicated code. */
	bool InternalSpawnWaveOrDelayCommon(int32 ThisWaveIndex, const FMonsterSpawnPoolWaveSettings* ThisWaveSetting);

	/** Check collision and start any new wave if condition mets. */
	void CheckNewWaveSpawnCollisionTrigger(AActor* BeginOverlapActor);

	/** Find a primitive component with a specified name */
	class UPrimitiveComponent* FindComponentByName(const FName& InName);

public:
	/** See if a component is specified for wave spawn triggering for current stage. 
	 * Valid only for components contained in this actor */
	bool IsComponentForTriggerPurpose(UActorComponent* InComp);

	bool IsComponentForTriggerToOpenDialogPurpose(UActorComponent* InComp);

	WaveDialogSettings_OutParam GetDialogCodeParamByTriggeringComponent(UPrimitiveComponent* InComp);

	/** See if a component is specified for enabled after completing some wave for current stage.
	 * Valid only for components contained in this actor */
	bool IsComponentEnabledLater(UActorComponent* InComp);
	
	void SetStageManager(class AB2StageManager* InManager);
	
	FORCEINLINE bool AreAllDoneForCurrentStage() { return bAllWaveDoneForCurrentStage; }

	int32 GetCurrentClientStageId();
	int32 GetCurrentStageDifficulty();

	class UBoxComponent* GetDefaultCollisionComponent() const;

	/** Get class info asset containing necessary mob spawn information from GameMode */
	class UB2NPCClassInfoBox* GetMobClassInfoBox();

	FMonsterSpawnPoolRuntimeWaveState* GetRuntimeWaveState(int32 WaveNum);

	/** Returns all mobs (optionally including dying mobs too) in the specified wave.
	 * However, even if bOnlyLiveMobs is false, dying mob is maintained in the list only for a very short moment (See OnSpawnedMonsterDead). */
	void GetWaveMobList(int32 WaveNum, TArray<class ABladeIICharacter*>& OutLiveMobs, bool bOnlyLiveMobs = true);

	/** Get all mobs currently spawned from here. Refer to GetWaveMobList for how bOnlyLiveMobs works. */
	void GetAllWaveMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true);
	void GetAllSummonedMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs = true);

	/** For a certain circumstance. The OldOne must to be destroyed soon. See ABladeIICharacter::PhaseChange */
	void SwapMonsterForPhaseSpawn(class ABladeIICharacter* OldOne, class ABladeIICharacter* NewOne);

	/** Returns left Mobs count of specific stage*/
	int32 GetNumOfMobsStageLeftWave(int32 StageNum, EStageDifficulty DifficultyLevel, bool bIncludeSummoned);

	/** Returns left weight-reward-gold*/
	int32 GetLeftGoldWeight(int32 StageNum, EStageDifficulty DifficultyLevel);

	bool TryGetStartTalkSetting(int32 &OutGroupID);
	bool TryGetCompleteTalkSetting(int32 &OutGroupID);

#if !UE_BUILD_SHIPPING
	/** Start and immediately finish all waves up to the index of DesiredFinishWaveNum within current stage's wave settings array. */
	void Cheat_FinishUpToCertainWave(int32 DesiredFinishWaveNum);

	void ProceedOnEventIgnoringCheat(); // For real special purpose test
#endif

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	
	/** Generate transient components for editing purpose for current editing stage. */
	void RegenerateEditingActors();

	void DestroyAllEditingActors();

	/** Create a EditingActor to represent spawn setting of a wave. */
	class AB2SpawnPoolEditingActor* CreateSingleWaveEditingActor(int32 StageNum, EStageDifficulty DifficultyLevel, int32 WaveNum);

	/** Ensure to have proper number of FixedSpawnTransforms array element when you don't use RandomSpawnLocation. 
	 * It does not ensure to have exact values. Just the number of array element. */
	void EnsureHaveFixedSpawnTransforms();

	/** To provide a way to set the spawn location/rotation from editing actor */
	void ExplictSetSpawnTransform(int32 StageNum, EStageDifficulty DifficultyLevel, int32 WaveSettingNum, int32 ObjNum, FVector InLocation, FRotator InRotation);

	/** Generate transient components to display the names of primitive component composing this actor. */
	void RegenerateComponentNameDisplay();

	/** Wrapped GetSpawnClassFromSetting to be used from outside for some editing purpose. */
	bool EditorGetSpawnClassOfWaveMob(ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation, TSubclassOf<class ABladeIICharacter>& OutClassBP, int32 InWaveNum, int32 InObjIndex);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditUndo() override;
	virtual void PostEditMove(bool bFinished) override;
#endif
};

/** Utility to parse WaveMobSelectKeyword string property (expected in "1 3" format) in numeric format. */
void ParseForWaveMobSelectKeyword(FString InKeyword, int32& OutWaveNum, int32& OutMobIndex);

/** Some common functionality to find spawn pool having desired stage number and difficulty setting among the list. */
AB2MonsterSpawnPool* GetDesiredSpawnPoolOfSettingFromList(TArray<AB2MonsterSpawnPool*>& InSPs, int32 InTargetStageNum, EStageDifficulty InTargetDifficulty, bool bFallBackToLowerDifficulty = true);
void SetForcedPerStageSettings(TArray<AB2MonsterSpawnPool*>& InSPs, int32 InClientStageNum, int32 InTargetDifficulty);

#if WITH_EDITOR
/** It is for various editor viewport work connected to spawn pool, returns active one considering editor stage number and difficulty. 
 * It provides bFallBackToLowerDifficulty option like GetDesiredStageSetting */
AB2MonsterSpawnPool* EditorGetCurrentActiveSpawnPool(bool bFallBackToLowerDifficulty = true);
FORCEINLINE bool AmICurrentEditorActive(AB2MonsterSpawnPool* CheckSP) { return (CheckSP && (CheckSP == EditorGetCurrentActiveSpawnPool())); }
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// For SpawnPoolEditingActor below..
////////////////////////////////////////////////////////////////////////////////////////////////////

/** Just to track some simple information like how many wave we really cover. */
struct FSpawnPoolEditingWaveObjectSimpleInfo
{
	FSpawnPoolEditingWaveObjectSimpleInfo()
	{
		WaveNum = -1;
		SpawnObjNum = -1;
		bRandomLocation = false;
	}

	int32 WaveNum;
	int32 SpawnObjNum;
	bool bRandomLocation;
};


UCLASS(notplaceable)
class BLADEII_API UB2SpawnPoolEditingSKComponent : public USkeletalMeshComponent
{
	GENERATED_UCLASS_BODY()

public:
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

/**
* Somewhat like Landscape Gizmo, an editing purpose actor for B2MonsterSpawnPool
* Additionally generate edit supporting components.
*/
UCLASS(notplaceable)
class BLADEII_API AB2SpawnPoolEditingActor : public AActor
{
	GENERATED_UCLASS_BODY()

	/**
	 * Connections to the variable that this actor is representing.
	 */
	class AB2MonsterSpawnPool* OwnerPool; // Should be the same as the Owner anyway..

	/** Simple info to track some wave and spawn obj indices, not the real shit. */
	TArray<FSpawnPoolEditingWaveObjectSimpleInfo> EditingSimpleInfo;

	/** Too big meshes might make editing work harder.. Limit by z height. */
	static float MeshSizeLimit;

	int32 CurrEditingClientStageId;
	EStageDifficulty CurrEditingDifficultyLevel;

	/** Just to have a chance to make them editable in editor viewport, Put them in array..
	 * We've also modified FComponentEditorUtils::CanEditNativeComponent a little bit to support this to be editable in editor viewport. */
	UPROPERTY(Transient, EditAnywhere)
	TArray<class USkeletalMeshComponent*> FakeSKCompArray;

public:
	AB2SpawnPoolEditingActor();

	virtual void BeginDestroy() override;

	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

#if WITH_EDITOR
	/** To find the component relevant to the wave number and object number */
	FString GetEditingSKMeshComponentName(int32 WaveNum, int32 SpawnObjNum);
	FString GetExtraInfoTextComponentName(int32 WaveNum, int32 SpawnObjNum);
	/** Watch out for confusing name.. Getting text render component's name that renders spawn pool's component name. */
	FString GetComponentNameDisplayComponentName(FString ComponentToDisplayName);

	/** Provide look-up from RootComp functionality, to get the wanted component with specific WaveNum and SpawnObjNum */
	class USkeletalMeshComponent* FindRelevantEditingSKMeshComp(int32 WaveNum, int32 SpawnObjNum);

	class UTextRenderComponent* FindRelevantExtraInfoTextRenderComp(int32 WaveNum, int32 SpawnObjNum);

	class UTextRenderComponent* FindRelevantComponentNameDisplayComp(FString ComponentToDisplayName);

	FSpawnPoolEditingWaveObjectSimpleInfo* FindEditingWaveObjectSimpleInfo(int32 WaveNum, int32 SpawnObjNum);

	void SetOwnerPool(class AB2MonsterSpawnPool* InOwnerPool);

	/**
	 * Setting information for a component representing a specific object in a wave.
	 * This will add a new component whenever necessary.
	 * Character class can be specified either by ENPCClass enum or BladeIICharacter BP class. ClassBP is counted if ClassEnum is ENC_End.
	 * Correct ObjWorldTransform is necessary regardless of bInRandomLocation, bInRandomLocation is just for setting up a text.
	 */
	void SetRepresentingInfo(int32 InStageNum, EStageDifficulty InDifficultyLevel, int32 InWaveSettingNum, int32 InSpawnObjNum, 
		ENPCClass CharacterClassEnum, ENPCClassVariation ClassVariation, TSubclassOf<class ABladeIICharacter> CharacterClassBP,
		bool bInRandomLocation, FTransform ObjWorldTransform, const TArray<class UMaterialInterface*>& InOptionalWaveMtrlOverride);
private:
	void SetRepresentativeMesh(int32 InWaveSettingNum, int32 InSpawnObjNum, ENPCClass CharacterClassEnum, ENPCClassVariation ClassVariation, TSubclassOf<class ABladeIICharacter> CharacterClassBP);
	FVector GetEditingCompToSpawnLocOffset(class USkeletalMeshComponent* EditingComp);
public:

	/** Methods to remove removed entry.. We might just clear everything before call SetRepresentingInfo for every spawn information. */
	void RemoveEditingInfo_UnnecessaryWave(int32 AllWaveNum);
	void RemoveEditingInfo_AllButOneWave(int32 RemainWaveNum);
	void RemoveEditingInfo_UnnecessaryObj(int32 WaveNum, int32 AllObjNumInWave);

	/** To make component name display */
	void SetComponentToDisplayInfo(class UPrimitiveComponent* InComp);

	/** Remove all text render components for component display purpose. Not like remove only unnecessary.. */
	void RemoveAllComponentDisplay();
		
	virtual void PostEditMove(bool bFinished) override;

	/** As a result of this actor's job, sync edited component's locations here to spawn pool */
	void SyncEditingActorLocationToSpawnPool();
#endif // WITH_EDITOR
};
