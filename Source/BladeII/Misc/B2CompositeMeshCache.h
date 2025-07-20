// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "CommonStruct.h"
#include "../BladeII.h"
#include "B2CompositeMeshCache.generated.h"

/** Data struct checking if cached composite mesh result is the same. */
struct FB2GMPCCompositeMeshHash
{
	FB2GMPCCompositeMeshHash()
	{
		CharacterClass = EPCClass::EPC_End;
		bWingComposited = false;
	}
	FB2GMPCCompositeMeshHash(EPCClass InPCClass, const TArray<int32>& InEquipmentId, bool bInWingComposited, const FB2Wing& InWingData);
	FB2GMPCCompositeMeshHash(EPCClass InPCClass, const TArray<FB2Item>& InEquipment, bool bInWingComposited, const FB2Wing& InWingData);

private:

	/** EquipmentIDs can reflect PCClass but if nothing is equipped? */
	EPCClass CharacterClass;

	/** The major part.. the mesh outfits are different only by RefID.. right? */
	TArray<int32> EquipmentRefIds;

	/** FB2Wing has bShouldBeVisible flag, so it can be redundant.. but declared here due to some code struct.. */
	uint32 bWingComposited : 1;
	/** Not all of Wing data is needed.. but there's no RefID stuff for Wing. */
	FB2Wing WingData;
public:

	bool IsEqual(const FB2GMPCCompositeMeshHash& InOther) const;
	/** Equality operator is the "reason" of this struct. */
	FORCEINLINE bool operator==(const FB2GMPCCompositeMeshHash& InOther) const { return IsEqual(InOther); }
	FORCEINLINE bool operator!=(const FB2GMPCCompositeMeshHash& InOther) const { return !IsEqual(InOther); }
};

/** A mapping for caching dynamically created skeletal mesh for player character. */
USTRUCT()
struct FB2GMPCCompositeMeshCacher
{
	GENERATED_USTRUCT_BODY()

	FB2GMPCCompositeMeshCacher()
	{
		CreatedCompositeMesh = nullptr;
		CreatedCompositeMesh_SectionMerged = nullptr;
	}

protected:
	/** It will check to see if CreatedCompositeMesh is outdated and need to be refreshed. */
	FB2GMPCCompositeMeshHash CompositeMeshHash;
	UPROPERTY(Transient)
	class USkeletalMesh* CreatedCompositeMesh;

	/** Section merged version for faster rendering */
	FB2GMPCCompositeMeshHash CompositeMeshHash_SectionMerged;
	UPROPERTY(Transient)
	class USkeletalMesh* CreatedCompositeMesh_SectionMerged;
public:

	bool IsOutdated(EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData, bool bForSectionMerged) const;
	bool IsOutdated(EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing& InWingData, bool bForSectionMerged) const;

	void Set(class USkeletalMesh* InNewMesh, EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData, bool bForSectionMerged);
	void Set(class USkeletalMesh* InNewMesh, EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing& InWingData, bool bForSectionMerged);

	/** Call it before you throw out this cache from any holder.. */
	void DiscardMe();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	void DevSetMeshesRooSet();
#endif

	FORCEINLINE class USkeletalMesh* Get(bool bForSectionMerged) const { return bForSectionMerged ? CreatedCompositeMesh_SectionMerged : CreatedCompositeMesh; }
};


/**
 * An object class manages composited BladeII player character meshes, preventing unnecessary creation/destruction.
 */
UCLASS()
class BLADEII_API UB2CompositeMeshCache : public UObject
{
	GENERATED_BODY()

public:
	UB2CompositeMeshCache(const FObjectInitializer& ObjectInitializer);


protected:
	
	enum class ECompositeMeshCacheType : uint8
	{
		ECMCT_Local,
		ECMCT_Remote
	};

	/** Cached for local player character mesh. Meshes contained here are not meant to be destroyed and recreated for new map loading. i.e. RootSet.
	 * Key is int casted PCClass */
	UPROPERTY(Transient)
	TMap<int32, FB2GMPCCompositeMeshCacher> LocalPCCache;

	/** For any other character other than the local player.. 
	 * In the case of network game-mode, key is player character's GameEntryID */
	UPROPERTY(Transient)
	TMap<int32, FB2GMPCCompositeMeshCacher> RemotePCCache;

public:

	/** Sets dynamically built skeletal mesh object for InPCClass and/or InPlayerCharacter, regarding PC part system.
	 * Cached composite mesh will be used for next time, to make re-building unnecessary. */
	void SetCachedCompositeMesh(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter, class USkeletalMesh* InMeshToSet,
		const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData,
		bool bForSectionMerged);

	/** It returns built and cached composite mesh for a specified player character, if any (set by SetCachedCompositeMesh).
	 * It is about PCClassInfo mesh composition considering equipment. A new mesh will be built if it returns NULL. */
	class USkeletalMesh* GetCachedCompositeMesh(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter,
		const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData,
		bool bForSectionMerged);

	bool ShouldSearchForLocalPCCache(class ABladeIIPlayer* InPlayerCharacter) const;

	/** Either LocalPCCache or RemotePCCache */
	TMap<int32, FB2GMPCCompositeMeshCacher>& GetRelevantCache(class ABladeIIPlayer* InPlayerCharacter);
	TMap<int32, FB2GMPCCompositeMeshCacher>& GetRelevantCache(ECompositeMeshCacheType InType);

	int32 GetCachedCompositeMeshMapKey(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter) const;

	int32 GetCachedCompositeMeshMapKey(EPCClass InPCClass, int32 InGameEntryID = -1) const;

	bool IsOneOfCachedCompositeMesh(class USkeletalMesh* InTestMesh) const;

	void OnPreLoadMap(class ABladeIIGameMode* InGameModeAboutToShutdown);

private:
	void DiscardCaches(ECompositeMeshCacheType DiscardCacheType);
public:

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	void DevResetCompositMeshCache();

private:
	// To test the actual gain.. compared to the old way
	bool bAllowLocalPCMeshRootSet;
public:
	void SetAllowLocalPCMeshRootSet(bool bSet);
#endif
	bool IsLocalPCMeshRootSetAllowed() const;
};

/** The one that you use for global mesh cache manager access. */
UB2CompositeMeshCache* StaticFindCompositeMeshCacher();