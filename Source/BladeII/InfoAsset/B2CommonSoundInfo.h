// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Sound/SoundCue.h"
#include "Engine/StreamableManager.h"
//#include "BladeII.h"
#include "B2CommonSoundInfo.generated.h"

UENUM()
enum class ECommonSoundID : uint8
{
	ECSID_None = 0, // Intentionally added 0 element for easier editing.

	// BGM
	ECSID_BGM_MainLobby,

	ECSID_BGM_CombatGoldBonusStageVictory,
	ECSID_BGM_CombatBossStageVictory,
	ECSID_BGM_CombatVictory,
	ECSID_BGM_CombatDefeat,

	ECSID_BGM_Lose,

	ECSID_BGM_PVPStandby,
	ECSID_BGM_PVPCombat,
	ECSID_BGM_PVPVictory,
	
	ECSID_BGM_TMStandby,
	ECSID_BGM_TMCombat,
	ECSID_BGM_TMVictory,

	ECSID_BGM_RaidStandby,
	ECSID_BGM_RaidCombat,
	ECSID_BGM_RaidVictory,
	ECSID_BGM_RaidDefeat,
	ECSID_BGM_RaidSargonCombat,
	ECSID_BGM_RaidSargonPhaseChange,
	ECSID_BGM_RaidSargonDie,

	ECSID_BGM_CounterDGVictory,
	ECSID_BGM_Counter1Phase,
	ECSID_BGM_Counter2Phase,

	ECSID_BGM_HeroTWVictory,

	ECSID_BGM_ControlBattleCombat,
	ECSID_BGM_ControlBattleVictory,

	ECSID_BGM_GuildBattleCombat,
	ECSID_BGM_GuildBattleVictory,
	ECSID_BGM_GuildBattleDefeat,

	// AMB
	ECSID_AMB_LobbyMainScene,
	ECSID_AMB_BattleCountDown,
	ECSID_AMB_BattleCountDown_Last,


	ECSID_End
};
FORCEINLINE int32 CommonSoundIDToInt(ECommonSoundID InID)
{
	return static_cast<int32>(InID);
}
/** For use of TMap with ECommonSoundID key */
FORCEINLINE uint32 GetTypeHash(const ECommonSoundID& InId)
{
	return static_cast<uint32>(InId);
}

/** Struct to contain loaded TAsset references of a single PCClassInfo entry to prevent it get GCd. */
USTRUCT()
struct FSingleCommonSoundInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FSingleCommonSoundInfoLoadedRefHolder()
	{
		LoadedSoundPtr = NULL;
	}

	UPROPERTY(Transient)
	USoundCue* LoadedSoundPtr;
};

USTRUCT(BlueprintType)
struct FSingleCommonSoundInfoMap
{
	GENERATED_USTRUCT_BODY()

	FSingleCommonSoundInfoMap()
	{
		SoundID = ECommonSoundID::ECSID_End;
	}

	/** Will be set automatically, by its holding map key. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECommonSoundID SoundID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundCue> AssetPtr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeInOutDuration;

	/** An utility to get the actual SoundCue asset defined by AssetPtr. Need additional information on loaded ptr map caching. */
	USoundCue* GetSoundCue(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap);

	void Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleCommonSoundInfoLoadedRefHolder>& LoadedPtrMap); // Unload if loaded.

};

enum class EStageCombatSoundContext : uint8
{
	ESCSC_NormalCombat,
	ESCSC_BossCombat

	// Might add AreaBossCombat? or differentiate it by stage?
	
};

/** A little more specialized common sound info map, to handle stage BGM setting. */
USTRUCT(BlueprintType)
struct FSingleCommonSoundInfoMap_Stage : public FSingleCommonSoundInfoMap
{
	GENERATED_USTRUCT_BODY()

	FSingleCommonSoundInfoMap_Stage()
		: FSingleCommonSoundInfoMap()
	{
		SoundID = ECommonSoundID::ECSID_End; // ID does nothing for this setting.

		bForExtraMap = false;
		bForNormalCombat = false;
		bForBossCombat = false;
	}

	/** Put string like "2-10,15" will associate this sound info map to stage from 2 to 10 and 15. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RelevantStageKeyStr;
	
	/** Parsed result of RelevantStageKeyStr. Only int32 key is used. Bool value is a dummy. */
	TMap<int32, bool> CachedSupportedStageList;

	/** Check if this setting is for the extra map of a stage, which is typically for area boss. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bForExtraMap : 1;

	/** Check if this is to be played for normal combat time */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bForNormalCombat : 1;

	/** Check if this is to be played when boss mob appeared. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bForBossCombat : 1;

	/** See if this mappings stage context flag matches to designated EStageCombatSoundContext */
	bool IsSupportedStageCombatContext(EStageCombatSoundContext InCheckContext);
};

USTRUCT(BlueprintType)
struct FSingleCommonSoundInfoMap_UIScene : public FSingleCommonSoundInfoMap
{
	GENERATED_USTRUCT_BODY()

		FSingleCommonSoundInfoMap_UIScene()
		: FSingleCommonSoundInfoMap()
	{}
};


USTRUCT(BlueprintType)
struct FButtonClickDefaultSoundInfoMap
{
	GENERATED_USTRUCT_BODY()

	FButtonClickDefaultSoundInfoMap()
	{
		AssetPtr = NULL;
	}

	// No identification..

	// No TAsset for now. Button click sound is usually tiny and guess better not load at everytime playing it.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* AssetPtr;

//#if WITH_EDITORONLY_DATA -> Doesn't work now..

	/** Do NOT use this for game code. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EditorOnlyComment;

//#endif
};

USTRUCT()
struct FSingleSoundWaveInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FSingleSoundWaveInfoLoadedRefHolder()
	{
		LoadedSoundPtr = NULL;
	}

	UPROPERTY(Transient)
	class USoundWave* LoadedSoundPtr;
};

USTRUCT(BlueprintType)
struct FSingleStageMiddleBossBGM
{
	GENERATED_USTRUCT_BODY()

	FSingleStageMiddleBossBGM()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USoundWave> AssetPtr;

	USoundWave* GetSoundWave(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSingleSoundWaveInfoLoadedRefHolder>& LoadedPtrMap);
	void Unload(FStreamableManager& InLoadManager);
};

USTRUCT(BlueprintType)
struct BLADEII_API FStageMiddleBossBGM
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray <FSingleStageMiddleBossBGM> IntroBGMArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray <FSingleStageMiddleBossBGM> LoopBGMArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class USoundCue> SoundCuePtr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeInOutDuration;

	UPROPERTY(Transient)
	FSingleCommonSoundInfoLoadedRefHolder LoadedSoundHolder;

	USoundCue* GetSoundCue(FStreamableManager& InLoadManager);
	void UnloadAll(FStreamableManager& InLoadManager, TMap<int32, FSingleSoundWaveInfoLoadedRefHolder>& LoadedPtrMap);
};


USTRUCT()
struct FStageCustomizeBossBGMInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

		FStageCustomizeBossBGMInfoLoadedRefHolder()
	{
		LoadedSoundPtr = NULL;
	}

	UPROPERTY(Transient)
		class USoundCue* LoadedSoundPtr;
};


USTRUCT(BlueprintType)
struct BLADEII_API FStageCustomizeBossBGM
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray <FSingleStageMiddleBossBGM> IntroBGMArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<class USoundCue> SoundCuePtr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float FadeInOutDuration;


	USoundCue* GetSoundCue(FStreamableManager& InLoadManager, TMap<FName, FStageCustomizeBossBGMInfoLoadedRefHolder>& InLoadedStageBGMPtrMap, const FName& InName);
	//void UnloadAll(FStreamableManager& InLoadManager, TMap<int32, FSingleSoundWaveInfoLoadedRefHolder>& LoadedPtrMap);
};
/**
 * Sound asset container for sound assets need to be played by native event.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2CommonSoundInfo : public UObject
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<ECommonSoundID, FSingleCommonSoundInfoMap> MainInfoMap;
	
	/** All loaded references of MainInfoArray to prevent GC. */
	UPROPERTY(Transient)
	TMap<int32, FSingleCommonSoundInfoLoadedRefHolder> LoadedMainInfoPtrMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FSingleCommonSoundInfoMap_Stage> InfoArray_StageBGM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FStageMiddleBossBGM StageMiddleBGM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<FName, FStageCustomizeBossBGM> StageCustomizeBossBGM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<EUIScene, FSingleCommonSoundInfoMap_UIScene> UISceneMap;

	/** All loaded references of InfoArray_StageBGM to prevent GC. */
	UPROPERTY(Transient)
	TMap<int32, FSingleSoundWaveInfoLoadedRefHolder> LoadedMiddleStageBGMPtrMap;

	UPROPERTY(Transient)
	TMap<int32, FSingleCommonSoundInfoLoadedRefHolder> LoadedStageBGMPtrMap;

	UPROPERTY(Transient)
	TMap<int32, FSingleCommonSoundInfoLoadedRefHolder> LoadedUISceneBGMPtrMap;

	UPROPERTY(Transient)
	TMap<FName,FStageCustomizeBossBGMInfoLoadedRefHolder> LoadedCoutomizeMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FButtonClickDefaultSoundInfoMap> ButtonClickDefSoundArray;

	
	FStreamableManager InfoLoadManager; // To use TSoftObjectPtr

public:

	UB2CommonSoundInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	/** You can get sound asset of InfoMapArray which can get by ID only, not for StageCombat here. */
	USoundCue* GetSoundByID(ECommonSoundID InID);
	float GetFadeInOutDuractionByID(ECommonSoundID InID);

	bool GetSoundByUIScene(EUIScene InUIScene, USoundCue** OutSoundCue, float & OutFadeInOutDuration);

	/** Get sound asset by stage number and additional stage combat context. No SoundID is required. */
	USoundCue* GetBGMSoundOfStageCombat(int32 InStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, float& OutFadeInOutDuration);
	USoundCue* GetButtonClickDefSound(int32 InIndex);
	USoundCue* GetCustomizeBossBGM(const FName& InCoustomName);

private:
	/** To get sound play stage numbers from RelevantStageKeyStr */
	void ParseStageKeyStr(TMap<int32, bool>& OutStages, const FString& InKeyStr);

	USoundCue* GetMissleBossStageBGM(float& OutFadeInOutDuration);
	USoundCue* GetNormalOrBossStageBGM(int32 InStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, float& OutFadeInOutDuration, int32 InWaveNumberbool = 0 , const FName& InName = FName() );

public:

	FORCEINLINE FStreamableManager& GetInfoLoadManager() { return InfoLoadManager; }

	/** Might need to call this for all necessary class of a stage at beginplay timing. */
	void PreloadClassAssets(const TArray<ECommonSoundID>& InIDs);

	/** Might need to call this whenever a stage is about to be unloaded? */
	void UnloadAll();

#if WITH_EDITOR
	
	/** For safe use of TSoftClassPtr in editor. */
	void EditorLoadAll();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void EnsureDataIntegrity();
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP // Clean-up loaded TAsset references. They are temporary code until formally fix wrong 
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};
