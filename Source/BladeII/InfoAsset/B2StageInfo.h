// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
//#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "CommonStruct.h"
#include "TimerManager.h"
#include "B2StageInfo.generated.h"

USTRUCT(BlueprintType)
struct FMovieSubtitle
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndTime;
};

/**
 * The actual data composing UB2StageInfo, representing information per single stage.
 */
USTRUCT(BlueprintType)
struct FSingleStageInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleStageInfoData() : MovieName(TEXT(""))
	{
		ChapterNum = 0;
		SubChapterNum = 0;
		InvokedGameMode = EB2GameMode::Lobby;
		DifficultyLevel = 1.0f;
		DifficultyLevel_2 = 1.0f;
		DifficultyLevel_3 = 1.0f;
		bIsGoldBonusStage = false;
		bIsBossStage = false;
		bHasExtraMap = false;
		ExtraMapDelay = 0.0f;
		ExtraMapDelay_ShortCut = 0.0f;
		ExtraMapMessageDelay = 0.0f;
		ExtraMapMessageDelay_ShortCut = 0.0f;
	}

	/** It associates this StageInfoData to a specific ChapterInfo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ChapterNum;

	/** The combination of ChapterNum and SubChapterNum is unique for each stage. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SubChapterNum;

	/** The combination of ChapterNum and SubChapterNum is unique for each stage. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EB2GameMode InvokedGameMode;

protected:
	/** StageStory also uses this key, but from different table (category) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizationStageNameKey;
public:
	FText GetLocalizedStageName();
	FText GetLocalizedStageStory();

protected:
	/** To affect combat variables of mobs.
	 * It was initially the unique DifficultyLevel, but now the first difficulty level. (@search tag DifficultyLevel_1)
	 * DifficultyLevel is in fact, MISNOMER. Difficulty"Scale" is more proper. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DisplayName = "Difficulty Scale 1"))
	float DifficultyLevel;

	/** DifficultyLevel is in fact, MISNOMER. Difficulty"Scale" is more proper. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DisplayName = "Difficulty Scale 2"))
	float DifficultyLevel_2;

	/** DifficultyLevel is in fact, MISNOMER. Difficulty"Scale" is more proper. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DisplayName = "Difficulty Scale 3"))
	float DifficultyLevel_3;

	/** DifficultyLevel is in fact, MISNOMER. Difficulty"Scale" is more proper. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (DisplayName = "Difficulty Scale 4"))
	float DifficultyLevel_4;

public:
	float GetDifficultyLevel_ClientData(EStageDifficulty StageDifficulty);

public:
	//
	// Difficulty level can be added. See STAGE_DIFFICULTY_# const and AB2StageManager::GetStageDifficultyScale
	// The higher, the harder.
	//

	/** Gold amount itself comes from server, client does not do much stuff regarding this, just a setting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bIsGoldBonusStage : 1;

	/** Means "Area" Boss, stage having the big one, and ExtraMap in most cases. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bIsBossStage : 1;

	/** Specify it to one of PlayerStart actor's PlayerStartTag, deployed at MainMap.
	 * No need if the level has only one PlayerStart. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString PlayerStartTag;

	/** The basic map to load for this stage. A map also has its own setting data in its WorldSettings.
	 * The stage can end on clearing the main map, or it can have extra map. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MainMap;

	//
	// For ExtraMap feature.. check FExtraMapTransitionData for any data here to transfer from main map to successive map. (or send through server)
	//
	/** True means this stage has extra map to complete (usually Boss map) after clearing the main map. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bHasExtraMap : 1;

	/** Extra map to clear after the main map. Most likely to be a Boss map. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bHasExtraMap"))
	FName ExtraMap;

protected:
	/** Post-clear main map delay.. for scenario mode where full event cut scene is to be played. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bHasExtraMap"))
	float ExtraMapDelay;
	/** The same HACKY variable just like FinalSpawnDelay_ShortCut of FMonsterSpawnPoolWaveSettings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bHasExtraMap"))
	float ExtraMapDelay_ShortCut;
public:
	FORCEINLINE float GetFinalExtraMapDelay(bool bIsScenarioMode) const {
		// Intended to return different value by bScenarioMode flag of StageManager.
		return bIsScenarioMode ? ExtraMapDelay : ExtraMapDelay_ShortCut;
	}
protected:
	/** Must be less than ExtraMapDelay.. Like ExtraMapDelay this is also for scenario mode only. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bHasExtraMap"))
	float ExtraMapMessageDelay;
	/** The same HACKY variable just like FinalSpawnDelay_ShortCut of FMonsterSpawnPoolWaveSettings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bHasExtraMap"))
	float ExtraMapMessageDelay_ShortCut;
public:
	FORCEINLINE float GetFinalExtraMapMessageDelay(bool bIsScenarioMode) const {
		// Intended to return different value by bScenarioMode flag of StageManager. In addition, ensure it to be same or shorter than ExtraMapDelay.
		return bIsScenarioMode ? FMath::Min(ExtraMapMessageDelay, ExtraMapDelay) : FMath::Min(ExtraMapMessageDelay_ShortCut, ExtraMapDelay_ShortCut);
	}

protected:
	/** This is like common raw naming. Localized movie file name is supposed to have certain suffix. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MovieName;
public:
	const FString GetLocalizedMovieName() const;
	bool DoesCurrLocTypeMovieFileExist() const;
	bool HasScenarioMovie() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<struct FMovieSubtitle> MovieSubtitles;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D>  StageTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D>  NPCTexture;
public:
	class UTexture2D* GetStageTexture();
	class UTexture2D* GetNPCTexture();
};

bool DoesLocalizedMovieFileExist(const FString& InRawMovieName, int32 CheckSoundLocType);
const FString ConvertToSafeLocalizedMovieName(const FString& InRawMovieName);

USTRUCT()
struct FB2StageInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FB2StageInfoLoadedRefHolder()
	{
		LoadedStageTexture = NULL;
		LoadedNPCTexture = NULL;
	}

	UPROPERTY(Transient)
	class UTexture2D* LoadedStageTexture;
	class UTexture2D* LoadedNPCTexture;
};

/**
 * Wrapper to a (unique) DataTable asset containing information of all stages represented in number.
 * It could be similar to the WorldSettings, the biggest difference is that StageInfo is per-stage basis, where multiple stages can share one level (map)
 */
UCLASS()
class BLADEII_API UB2StageInfo : public UObject
{
	GENERATED_BODY()

protected:

	/** Data table asset, containing the actual data, providing CSV import/export feature */
	UPROPERTY()
	class UDataTable* TheData;

	int32 FinalClientStageId;
	int32 StartAdventureModeChapterNum;
	int32 EndAdventureModeChapterNum;

	// Just put stuff for editor safety
	UPROPERTY(Transient)
	TArray<FB2StageInfoLoadedRefHolder> EditorAllLoadedTAssetHolder;

public:
	UB2StageInfo(const FObjectInitializer& ObjectInitializer);

	void SetDataByCBTVersionContent();

	/** Get single info data of a ClientStageId, NULL if not found.
	 * StageNum is supposed to be InfoArray index + 1 (start from 1, not 0) */
	FSingleStageInfoData* GetInfoData(int32 InClientStageId);
	FSingleStageInfoData* GetInfoData(int32 InClientStageId, EStageDifficulty InDifficulty);

	/** Provide mapping between chapter/sub-chapter and stage number. You can get unique stage number by chapter and sub-chapter number.
	 * Returns 0 if no such stage number is found. */
	int32 GetClientStageIdOfChapterNums(int32 InChapterNum, int32 InSubChapterNum);
	int32 GetChapterNumByClientStageId(int32 InClientStageId);
	int32 GetSubChapterNumByClientStageId(int32 InClientStageId);

	bool IsInvokedGameMode(int32 InClientStageId, EB2GameMode InCheckGameMode);

	/** Get single info data by chapter/sub-chapter number combination, instead of stage number. */
	FSingleStageInfoData* GetInfoDataByChapterNums(int32 InChapterNum, int32 InSubChapterNum);

	/** Get all FSingleStageInfoData elements having their ChapterNumber same as InChapterToSearch, to OutFoundData array. */
	void GetAllInfoDataOfChapter(TArray<FSingleStageInfoData*>& OutFoundData, int32 InChapterToSearch);

	/** Returns how many stage info exist here. */
	int32 GetMaxClientStageId();
	/** Different from GetMaxClientStageId */
	FORCEINLINE int32 GetFinalClientStageId() const {
		return FinalClientStageId;
	}
	FORCEINLINE int32 GetStartAdventureModeChapterNum() const {
		return StartAdventureModeChapterNum;
	}
	FORCEINLINE int32 GetEndAdventureModeChapterNum() const {
		return EndAdventureModeChapterNum;
	}


	int32 ChangeDiffcultyStageID(int32 InClientSTageID, EStageDifficulty InDifficulty);

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
	void EditorLoadAll(); // Now it has TAsset and it is DataTable..
#endif
};

/** Simple level streaming settings for shared use of some stage event properties. */
USTRUCT(BlueprintType)
struct FB2StageStreamingSettings
{
	GENERATED_USTRUCT_BODY()

	FB2StageStreamingSettings()
	{
		LoadLimitTime = -1.0f;
	}

	// We expect just single level is specified in most cases, but make it array for possible extended use.

	/** Put level names that you see from ContentBrowser.
	 * Whole path is not required unless other map having the same name exist at different path. */
	UPROPERTY(EditAnywhere)
	TArray<FName> LevelToLoad;

	/** Put level names that you see from ContentBrowser.
	 * Whole path is not required unless other map having the same name exist at different path. */
	UPROPERTY(EditAnywhere)
	TArray<FName> LevelToUnload;

	/** It will try flush streaming after specified seconds, if set as positive value.
	 * Leave it zero or negative value will flush streaming right at the moment. */
	UPROPERTY(EditAnywhere)
	float LoadLimitTime;

	// Load/Unload levels specified by this setting, and flush depend on LoadLimitTime setting.
	void ExecuteStreaming(UWorld* InWorld,
		// You need to provide some stuff to call flush streaming, coming from world context object.
		// Not significant if LoadLimitTime is zero or negative though.
		FTimerHandle& InTimerHandle,
		FTimerDelegate const& InFlushStreamingTimerDelegate);

#if WITH_EDITOR
	// Meant to be called on edit property.
	void EditorCheckForProperSetting(class UWorld* InWorld);
	// Returns true if a level having NameToCheck exist in InWorld.
	static bool EditorCheckLevelNameInWorld(class UWorld* InWorld, const FName& NameToCheck);
	static bool EditorCheckLevelStreamingMethodInWorld(class UWorld* InWorld, const FName& NameToCheck);
#endif
};