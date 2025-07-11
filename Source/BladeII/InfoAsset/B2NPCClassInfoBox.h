// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BladeII.h"
#include "UObject/Object.h"
#include "B2NPCClassInfo.h"
#include "B2AssetLoader.h"
#include "B2NPCClassInfoBox.generated.h"


/**
 * The data set of NPCClassInfoBox, to search and load proper NPCClassInfo asset on demand.
 */
USTRUCT(BlueprintType)
struct FNPCClassInfoSearchMap
{
	GENERATED_USTRUCT_BODY()

	FNPCClassInfoSearchMap()
	{
		NPCClassEnum = ENPCClass::ENC_End;
	}

	/** The most basic identification for Info settings contained here.
	 * Must match to the its pointing NPCClassInfo asset's NPCClassEnum. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ENPCClass NPCClassEnum;

	/**
	 * Actual info data array per variation of this NPC class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ENPCClassVariation, TSoftClassPtr<UB2NPCSingleClassInfo>> VariationAssets;
};


/**
 * Simply the collection of UB2NPCClassInfo, but unlike UB2PCClassInfoBox, functionalities applying info data are implemented here.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2NPCClassInfoBox : public UObject, public IB2AsyncInterface
{
	GENERATED_BODY()
		
	/** 
	 * If true, not all loaded NPCClassInfo data will be simply unloaded at level transition, instead some are get to added to RootSet. 
	 * You should always use ShouldUnloadAllOnLevelTransition instead of direct access of this variable. 
	 */
	static bool bManagedNPCClassUnload;
	/**
	 * Total estimated NPCSingleClassInfo asset num added to RootSet by ManagedNPCClassUnload
	 * Do not take it too serious. It is "estimated" value.
	 */
	int32 EstimatedRootedAssetNum;

protected:

	/**
	 * We once had other Info data struct array in a little different format, and is replaced, so its name has "New".
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TMap<ENPCClass, FNPCClassInfoSearchMap>		NewInfoMap;
	
	/** All loaded references to prevent GC.
	 * The key is uniquely defined for each UB2NPCSingleClassInfo entry. */
	UPROPERTY(Transient)
	TMap<uint32, UObject*>						LoadedPtrMap; 
	
	/** Any NPCClass contained here better not be loaded asynchronously after BeginPlay */
	static TMap<ENPCClass, bool> SpawnPoolAsyncBlacklists;

	static void SetupSpawnPoolAsyncBlacklist();

	/* AsyncInterface */
	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList);

public:

	UB2NPCClassInfoBox(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostLoad() override;

	/**
	 * The main NPCSingleClassInfo accessing interface
	 */
	UB2NPCSingleClassInfo* GetNPCSingleClassInfo(ENPCClass InClass, ENPCClassVariation InVariation);
	UB2NPCSingleClassInfo* GetNPCSingleClassInfo(const FCombinedNPCClassID& ClassID)
	{
		return GetNPCSingleClassInfo(ClassID.GetNPCClassEnum(), ClassID.GetNPCVariationEnum());
	}

	UB2NPCSingleClassInfo* FindSingleClassInfo(const FCombinedNPCClassID& ClassID);

	/**
	 * Spawns a character in designated world with a ENPCClass and other parameters..
	 * It simply calls UWorld::SpawnActor inside, so do other things manually if necessary, such as FinishSpawning call for defer construction.
	 * In fact, better set SpawnParameters.bDeferConstruction to true, to let BeginPlay called after applying NPCClassInfo.
	 */
	class ABladeIICharacter* SpawnNPCClassInfoCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);

	ABladeIICharacter* SpawnNPCClassInfoNetCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);

	ABladeIICharacter* SpawnNPCClassInfoRaidNetCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);

	ABladeIICharacter* SpawnNPCClassInfoControlNetCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);

	ABladeIICharacter* SpawnNPCClassInfoGuildCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	class ABladeIITestDummyNPC* SpawnNPCClassInfoDummyCharacter(class UWorld* SpawnWorld, ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters);
#endif

	/** Might need to call this for all necessary class of a stage at beginplay timing. */
	void SyncPreloadClassAssets(const TArray<FCombinedNPCClassID>& InClasses); // It is now "Synchronous" Preload

	virtual void GetAsyncRequestInfoFromNPCIds(const TArray<FCombinedNPCClassID>& NPCIDs, TArray<FB2AsyncRequestInfo>& OutRequestInfoList);
	TSoftClassPtr<UB2NPCSingleClassInfo> GetNPCSingleClassAsset(const FCombinedNPCClassID& ClassID);

	void Unload(const FCombinedNPCClassID& NPCClassID);

	/** Might need to call this whenever a stage is about to be unloaded? */
	void UnloadAll();

	void UnloadExcept(const TMap<FCombinedNPCClassID, bool>& InExceptIDs);
	void UnloadExceptCurrentExplicit(class ABladeIIGameMode* CurrGameMode);

	static bool ShouldUnloadAllOnLevelTransition();
	FORCEINLINE int32 GetEstimatedRootedAssetNum() const { return EstimatedRootedAssetNum; }

	static bool IsInSpawnPoolAsyncBlacklist(ENPCClass InCheckClass);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
private:
	FString DevTestLastAsyncReqTimeName;
	bool bDevTestLastAsyncReqTimeMarked;
	double DevTestLastAsyncReqTime;
public:
	/** To display time taken to complete async request, call it right after TryAsyncLoad */
	void DevTestMarkLastAsyncReqTime(const FString& InReqName);
#endif

#if WITH_EDITOR
	// Editor only struct to put some ref together.
	struct NPCClassPreviewAssetInfo
	{
		NPCClassPreviewAssetInfo() : SKMesh(nullptr) {}
		class USkeletalMesh* SKMesh;
		TArray<class UMaterialInterface*> MaterialOverrides;
	};
	/** Get the skeletal mesh of a class considering base blueprint and overriding condition. */
	NPCClassPreviewAssetInfo GetPreviewAssetInfoFromClass(ENPCClass InClass, ENPCClassVariation InVariation);

	void OnAssetEditorOpen(UObject* InAsset);

	/** For safe use of TSoftClassPtr in editor.
	 * Arguments are totally optional only for editor splash progress. */
	void EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr = nullptr, float InStartProgress = 0.0f, float InEndProgress = 0.0f);
	
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
	
	/** Check if one element's own NPCClassEnum is different from what's being set in the BP class of AssetPtr, and popup message */
	void CheckAllNPCClassEnumSettingValid();

	/** Try find SingleClassInfo that has its SensitiveDataRowKey as designated name. It is just check data integrity in editor. */
	UB2NPCSingleClassInfo* TryGetMatchingClassInfo(FName InSensitiveDataKey);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void FirstTimeNPCSensitiveInfoConstruction();

	/** Returns false if its own NPCClassEnum is different from what's being set in the BP class of AssetPtr */
	bool GatherInvalidSetting(TArray<FCombinedNPCClassID>& OutInvalidTypes);

#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};


int32 GetNPCClassKey(ENPCClass InClassEnum, ENPCClassVariation InClassVariation);
void FromNPCClassKey(int32 InKeyValue, ENPCClass& OutClassEnum, ENPCClassVariation& OutClassVariation);
int32 GetNPCClassKey(const FCombinedNPCClassID& NPCClassID);
void FromNPCClassKey(int32 InKeyValue, FCombinedNPCClassID& OutNPCClassID);