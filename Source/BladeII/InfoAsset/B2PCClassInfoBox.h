// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.generated.h"

/** Struct to contain loaded TAsset references of a single PCClassInfo entry to prevent it get GCd. */
USTRUCT()
struct FSinglePCClassInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FSinglePCClassInfoLoadedRefHolder()
	{
		LoadedInfoPtr = NULL;
	}

	UPROPERTY(Transient)
	TSubclassOf<UB2PCClassInfo> LoadedInfoPtr;
};

/**
 * The data set of PCClassInfoBox, to search and load proper PCClassInfo asset on demand.
 */
USTRUCT(BlueprintType)
struct FPCClassInfoSearchMap
{
	GENERATED_USTRUCT_BODY()

	FPCClassInfoSearchMap()
	{
		PCClassEnum = EPCClass::EPC_End;
	}

	/** The most basic identification for Info settings contained here.
	 * Must match to the its pointing PCClassInfo asset's PCClassEnum. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPCClass PCClassEnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UB2PCClassInfo> AssetPtr;
	
	/** An utility to get the actual NPCClassInfo asset defined by AssetPtr. Need additional information on loaded ptr map caching. */
	class UB2PCClassInfo* GetPCClassInfoObject(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap);

	void Unload(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap); // Unload if loaded.

	bool IsClassDataAtRootSet(bool bCheckForInGameOnlyInfo);

#if WITH_EDITOR
	/** Returns false if its own PCClassEnum is different from what's being set in the BP class of AssetPtr */
	bool CheckPCClassEnumSettingValid(FStreamableManager& InLoadManager, int32 MyArrayIndex, TMap<int32, FSinglePCClassInfoLoadedRefHolder>& LoadedPtrMap);
#endif
};

/**
 * Simply the collection of UB2PCClassInfo
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2PCClassInfoBox : public UObject
{
	GENERATED_BODY()
		
	/** 
	 * If true, loaded PCClassInfo data won't be simply unloaded at level transition time and be cached for reduce next level loading. 
	 * You should always use ShouldUnloadAllPCClassInfoOnLevelTransition instead of direct access of this variable. 
	 */
	static bool bManagedPCClassUnload;


protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2SkillInfo> AllPCSkillInfos;

	/** For extra performance required by special mode that has many PC characters displayed, InfoAsset having necessary information for mesh section merge. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<UB2PCMeshSectionMergeInfo> MeshSectionMergeInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FPCClassInfoSearchMap> InfoMapArray;

	/** All loaded references to prevent GC. */
	UPROPERTY(Transient)
	TMap<int32, FSinglePCClassInfoLoadedRefHolder> LoadedPtrMap;

	FStreamableManager InfoLoadManager; // To use TSoftClassPtr

public:

	UB2PCClassInfoBox(const FObjectInitializer& ObjectInitializer);

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
		
	static bool ShouldUnloadAllPCClassInfoOnLevelTransition();
#if !UE_BUILD_SHIPPING
	// Setting bManagedPCClassUnload at runtime is only for Development!
	static void DevSetManagedPCClassUnload(bool bSet) { bManagedPCClassUnload = bSet; }
#endif
	/**
	 * Simple search function.
	 */
	class UB2PCClassInfo* GetSingleClassInfo(EPCClass InClass);

	bool IsClassDataAtRootSet(EPCClass InClass, bool bCheckForInGameOnlyInfo);

	/** To prevent sync-load blocking during game-play, call this for all necessary classes of a stage at beginplay timing. */
	void PreloadClassAssets(const TArray<EPCClass>& InClasses);

	///////////////////////////////////////////
	// Prevent the use of basic Unload interfaces of PCClassInfoBox, 
	// to make it clear that a PCClassInfo data won't be unloaded with ManagedPCClassUnload.
private:
	/** Might need to call this whenever a stage is about to be unloaded? */
	void UnloadAll();
	/** For ManagedPCClassUnload */
	void UnloadExcept(const TArray<EPCClass>& InExceptClasses);
	void UnloadAllOrExceptSelected();
public:
	void UnloadAllOrExceptSelectedIfAllowed(); // Use this one as the master function.
	///////////////////////////////////////////

	class UB2SkillInfo* GetAllSkillInfo();

	UB2PCMeshSectionMergeInfo* GetMeshSectionMergeInfo();

#if WITH_EDITOR
	/** Get the base skeletal mesh (base dummy skeleton or could be a fallback where no equipments are attached) 
	 * of a class considering base blueprint and overriding condition. */
	class USkeletalMesh* GetBaseSkeletalMeshFromClass(EPCClass InClass);
	/** For all PC classes. Guess not necessary other than the editor? */
	void GetSkeletalMeshesForAllClasses(TArray<USkeletalMesh*>& OutMeshes);
	/** Find a PCClassInfo that using the same SkeletalMesh asset to InTestSKMesh */
	class UB2PCClassInfo* GetPCClassInfoForMatchingSKMesh(class USkeletalMesh* InTestSKMesh);

	/** For safe use of TSoftClassPtr in editor. 
	 * Arguments are totally optional only for editor splash progress. */
	void EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr = nullptr, float InStartProgress = 0.0f, float InEndProgress = 0.0f);

	/** Returns false if one element's own PCClassEnum is different from what's being set in the BP class of AssetPtr */
	bool CheckAllPCClassEnumSettingValid();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
