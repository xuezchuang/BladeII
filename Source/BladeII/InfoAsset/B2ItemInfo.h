// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "B2Inventory.h"
#include "BladeIiPlayer.h"
#include "B2AssetLoader.h"
#include "../Common/CommonStruct.h"
#include "../UI/B2UIEnum.h"
#include "UObject/ObjectSaveContext.h"
#include "B2ItemInfo.generated.h"

// Some socket name define for special effect to be attached to upgraded item.
#define ENHANCED_WEAPON_EFFECT_SOCKET_L TEXT("s_wp_level_L")
#define ENHANCED_WEAPON_EFFECT_SOCKET_R TEXT("s_wp_level_R")
#define SIXSTAR_WEAPON_EFFECT_SOCKET_L_01 TEXT("s_wp_aura_L_01")
#define SIXSTAR_WEAPON_EFFECT_SOCKET_R_01 TEXT("s_wp_aura_R_01")
#define SIXSTAR_WEAPON_EFFECT_SOCKET_L_02 TEXT("s_wp_aura_L_02")
#define SIXSTAR_WEAPON_EFFECT_SOCKET_R_02 TEXT("s_wp_aura_R_02")
#define BODYAMMOR_EFFECT_SOCKET TEXT("s_ba_aura")
#define HELMET_EFFECT_SOCKET TEXT("s_helmet_aura")
#define GLOVE_EFFECT_SOCKET_L_01 TEXT("s_glove_aura_L_01")
#define GLOVE_EFFECT_SOCKET_R_01 TEXT("s_glove_aura_R_01")
#define SHOSE_EFFECT_SOCKET_L_01 TEXT("s_shose_aura_L_01")
#define SHOSE_EFFECT_SOCKET_R_01 TEXT("s_shose_aura_R_01")

/** To keep loaded asset pointer of lazy-loaded FSingleItemInfoData member, to prevent it get GCd. */
USTRUCT()
struct FSingleItemInfoLoadedPtrData
{
	GENERATED_USTRUCT_BODY()

	FSingleItemInfoLoadedPtrData()
	{
		LoadedIconMaterial = NULL;
		LoadedPCPartMesh = NULL;
		LoadedPCPartMaterial = NULL;
		LoadedPCPartExtraMesh = NULL;
		LoadedPCPartExtraMaterial = NULL;
		LoadedDropMainBodyPST = NULL;
		LoadedDropSuckingPS = NULL;
		LoadedDropPlaySoundCue = NULL;
		LoadedSixStarWeaponEffect = NULL;
		LoadedSixStarWeaponEffect_R = NULL;
		LoadedSixStarWeaponEffect2 = NULL;
		LoadedSixStarWeaponEffect2_R = NULL;
	}

	// Put hard references of all TSoftObjectPtr/TSoftClassPtr members of FSingleItemInfoData

	UPROPERTY(Transient)
	class UMaterialInterface* LoadedIconMaterial;

	UPROPERTY(Transient)
	class USkeletalMesh* LoadedPCPartMesh;

	UPROPERTY(Transient)
	class UMaterialInterface* LoadedPCPartMaterial;

	UPROPERTY(Transient)
	class USkeletalMesh* LoadedPCPartExtraMesh;

	UPROPERTY(Transient)
	class UMaterialInterface* LoadedPCPartExtraMaterial;

	UPROPERTY(Transient)
	class UParticleSystem* LoadedDropMainBodyPST;

	/** Effect played at the moment of player sucking. */
	UPROPERTY(Transient)
	class UParticleSystem* LoadedDropSuckingPS;

	UPROPERTY(Transient)
	class USoundCue* LoadedDropPlaySoundCue;

	UPROPERTY(Transient)
	class UParticleSystem* LoadedSixStarWeaponEffect;

	UPROPERTY(Transient)
	class UParticleSystem* LoadedSixStarWeaponEffect_R;

	UPROPERTY(Transient)
	class UParticleSystem* LoadedSixStarWeaponEffect2;

	UPROPERTY(Transient)
	class UParticleSystem* LoadedSixStarWeaponEffect2_R;
};


/**
 * An item's client side info struct including resources, having its row name as ItemRefID.
 * Its data is being the base for both FB2Item and ADropItem
 */
USTRUCT()
struct FSingleItemInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleItemInfoData()
	{			
		CachedMyRefID = -1;

		bHasExtraSlotEffect = false;

		BaseBPClass = NULL;
		
		Offset_SpawnMinDistance = 0.f;
		Offset_SpawnMaxDistance = 0.f;
		Offset_SpawnLocationZ = 0.0f;
		Offset_SpawningTime = 0.f;

		Offset_StayTime = 0.0f;
		PlaySoundDelayOverride = 0.0f;
		PostSuckLifeSpanOverride = 0.0f;
		Offset_CollisionRadius = 0.0f;

		FGoodsType = GoodsTypeFName::None;

		bMIDAtlasParamFromUIMIC = false;
	}

	int32 CachedMyRefID; // Being set by B2ItemInfo when it is retrieved by GetInfoData.
	FORCEINLINE bool IsCachedMyRefIDValid() const { return (CachedMyRefID > 0); }

	/** Localized Name Key. Using RowName in LocalizedTextTable. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LocalizedNameKey;

public:
	/** You may optionally attach certain prefix by ItemQuality. ItemQuality should be in range 1 ~ MAX_ITEM_QUALITY in that case. */
	FText GetLocalizedName(bool bAttachQualityPrefix = false, int32 ItemQuality = 0);

protected:
	/**
	 * The actual Blueprint class which is used for spawn and overridden by properties here.
	 * Leave it as NULL if to use the default one. We provide a default base BP class, but any other special class can be used.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AB2DropItem> BaseBPClass;
public:
	TSubclassOf<class AB2DropItem> GetBaseBPClass(UObject* WorldContextObject);

protected:
	/** Icon material for the use of UMG. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> IconMaterial; // Also check relevant FSingleItemInfoLoadedPtrData member
public:
	class UMaterialInterface* GetIconMaterial(class UB2ItemInfo* InInfoObj);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint32 bHasExtraSlotEffect : 1;

	////////////////////////////////////////////////////////////////////////////////
	// For PC part mesh composition.

protected:
	/** Mesh resource to be a part of whole in-game mesh who equip it. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class USkeletalMesh> PCPartMesh;
public:
	class USkeletalMesh* GetPCPartMesh(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_PCPartMesh() const // For FB2AsyncRequestInfo AssetKey.
	{ 
		check(IsCachedMyRefIDValid()); // If we get caught here, need to find a way to call it later or set CachedMyRefID prior to this.
		return CachedMyRefID * 100 + 1; // Simply need a unique uint32 identification.
	}

protected:
	/** Material to be applied to the mesh section (or component) created by PCPartMesh. Leave it null if use default applied material. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> PCPartMaterial;
public:
	class UMaterialInterface* GetPCPartMaterial(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_PCPartMaterial() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 2;
	}

	/** Only for some part that can have extra attachment. See GetExtraAttachPart. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class USkeletalMesh> PCPartExtraMesh;
public:
	class USkeletalMesh* GetPCPartExtraMesh(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_PCPartExtraMesh() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 11;
	}

protected:
	/** Material override for PCPartExtraMesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> PCPartExtraMaterial;
public:
	class UMaterialInterface* GetPCPartExtraMaterial(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_PCPartExtraMaterial() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 12;
	}

protected:
	/** Only for weapons of six star grade. It will be used both for Left and Right if _R version is not specified. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> SixStarWeaponEffect;
public:
	class UParticleSystem* GetSixStarWeaponEffect(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_SixStarWeaponEffect() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 21;
	}

protected:
	/** Only for weapons of six star grade. If _R version is not specified, non _R version will be used both for Left and Right. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> SixStarWeaponEffect_R;
public:
	class UParticleSystem* GetSixStarWeaponEffect_R(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_SixStarWeaponEffect_R() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 22;
	}

protected:
	/** Only for weapons of six star grade. It will be used both for Left and Right if _R version is not specified. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> SixStarWeaponEffect2;
public:
	class UParticleSystem* GetSixStarWeaponEffect2(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_SixStarWeaponEffect2() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 23;
	}
protected:
	/** Only for weapons of six star grade. If _R version is not specified, non _R version will be used both for Left and Right. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> SixStarWeaponEffect2_R;
public:
	class UParticleSystem* GetSixStarWeaponEffect2_R(class UB2ItemInfo* InInfoObj);
	FORCEINLINE uint32 GetAsyncReqKey_SixStarWeaponEffect2_R() const // For FB2AsyncRequestInfo AssetKey.
	{
		check(IsCachedMyRefIDValid());
		return CachedMyRefID * 100 + 24;
	}


	////////////////////////////////////////////////////////////////////////////////
	// For drop item manifestation.

protected:
	/** Particle system template for drop item manifestation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> DropMainBodyPST; // Also check relevant FSingleItemInfoLoadedPtrData member
public:
	class UParticleSystem* GetDropMainBodyPST(class UB2ItemInfo* InInfoObj);

protected:
	/** Effect played at the moment of player drop item sucking. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> DropSuckingPS; // Also check relevant FSingleItemInfoLoadedPtrData member
public:
	class UParticleSystem* GetDropSuckingPS(class UB2ItemInfo* InInfoObj);

	/** Offset from its dead character's body */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Offset_SpawnMinDistance;

	/** Offset from its dead character's body */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Offset_SpawnMaxDistance;

	/** Offset from its dead character's body */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Offset_SpawnLocationZ;

	/** Offset from its dead character's body */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Offset_SpawningTime;

	/** Time to stay at the spawn location before it starts to fly to the player. 0 or negative value for using default value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Offset_StayTime;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class USoundCue> DropPlaySoundCue; // Also check relevant FSingleItemInfoLoadedPtrData member
public:
	class USoundCue* GetDropPlaySoundCue(class UB2ItemInfo* InInfoObj);

	/** Time to start play sound (by AudioComp) after spawn. 0 or negative value for using default value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PlaySoundDelayOverride;

	/** Time to stay alive after the player sucked this item.
	 * During this time, It is invisible, just let it live for some while to express its glorious fucking moment of getting sucked by wicked player. 
	 * 0 or negative value for using default value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PostSuckLifeSpanOverride;

	/** Specifies the radius of collision body. 0 or negative value for using default value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Offset_CollisionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName FGoodsType;

	/** Specifies the radius of collision body. 0 or negative value for using default value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bMIDAtlasParamFromUIMIC;
	

	////////////////////////////////////////////////////////////////////////////////

	//
	// Any additional data..
	//

	/** Load all lazy-loaded assets by TSoftObjectPtr. It could block the execution for significant time.
	 * MyRefID and LoadedPtrMap are about preventing GC of loaded assets, coming from UB2ItemInfo container.
	 * Basically, it is not meant to be used for all circumstances, Do not use this when you need to limit loading for only necessary resources. */
	void LoadAllTAssets(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap);

	/** Unload all lazy-loaded assets. It is not only for the Editor, also being formal in-game feature. */
	void UnloadAllTAssets(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap, bool bUnloadEvenRootSet);

	/** Specially for saving memory. Unload all data except necessary resources for mesh part/effect composition. */
	void UnloadAllExceptEquipPart(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap);

	void UnloadOnlyEquipPart(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap, bool bUnloadEvenRootSet);
	void UnloadOnlyEquipPart(class UB2ItemInfo* OwnerItemInfoTable, bool bUnloadEvenRootSet);
	
	/** It is now about saving loading time. */
	void MarkLoadedEquipPartAtRooSet(bool bRootSet);

	void GetEquipPartsAsyncLoadReqInfo(TArray<FB2AsyncRequestInfo>& OutRequestInfoList);
	void CacheOnEquipPartsAsyncLoadComplete(int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap);
};

USTRUCT()
struct FItemSetInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FItemSetInfoData()
	{
		GroupID = 0;
		Grade = 0;
		CharacterClass = EPCClass::EPC_End;
		IconMaterial = nullptr;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 GroupID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Grade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPCClass CharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LocalizedNameKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInterface> IconMaterial;
};

/** Some data binding for unique identification of async load request. */
struct FItemInfoEquipPartsAsyncReqCombinedID
{
	FItemInfoEquipPartsAsyncReqCombinedID()
	{
		MySerialNumber = -1;
	}
private:
	TArray<int32> RefIDs; // The main data need to be uniquely identified, but itself cannot be simply encoded, unless we can afford very very big number.
public:
	int64 MySerialNumber; // Serial number being counted inside ItemInfo.
	FString MyAsyncRequestName;
	
	void SetRefIDs(const TArray<int32>& InRefIDs);
	FORCEINLINE const TArray<int32>& GetRefIDs() const { return RefIDs; }
	FORCEINLINE bool IsValid() const { return (MySerialNumber >= 0 && MyAsyncRequestName.Len() > 0); }

	const FItemInfoEquipPartsAsyncReqCombinedID& operator=(const FItemInfoEquipPartsAsyncReqCombinedID& InOther);
	bool operator==(const FItemInfoEquipPartsAsyncReqCombinedID& InOther) const;

	bool IsEqualByRefIDs(const FItemInfoEquipPartsAsyncReqCombinedID& InOther) const;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FItemInfoEquipPartsAyncLoadCompleteEvent, const FString&);

/**
 * Wrapper to a DataTable asset containing all FSingleItemInfo
 */
UCLASS()
class BLADEII_API UB2ItemInfo : public UObject, public IB2AsyncInterface
{
	GENERATED_BODY()
	
protected:
	
	/** Data table asset, containing the actual data, providing CSV import/export feature */
	UPROPERTY()
	class UDataTable* TheData;

	UPROPERTY()
	class UDataTable* SetInfoData;

	FStreamableManager InfoLoadManager; // To use TSoftObjectPtr

	UPROPERTY()
	TMap<int32, FSingleItemInfoLoadedPtrData> LoadedPtrMap; // Contains references of loaded assets, to prevent them get GCd.

	TMap<int32, FName> SetUniqueKeyMap;

	/** List of info, that currently being async loaded.. an element must be removed on load completion. */
	TArray<FItemInfoEquipPartsAsyncReqCombinedID> PendingEquipPartsAsyncLoadReqList;
	/** Simple sequential counter, being MySerialNumber of a new FItemInfoEquipPartsAsyncLoadReqInfo */
	int64 EquipPartsAsyncLoadReqCounter;

public:
	UB2ItemInfo(const FObjectInitializer& ObjectInitializer);

	/** Get single info data by ItemRefID, NULL if not found. 
	 * For Editor or bFullyLoad true, it loads all lazyloaded TSoftObjectPtr. Otherwise, TAsset refs are still not valid. */
	FSingleItemInfoData* GetInfoData(int32 ItemRefID, bool bFullyLoad = false);
	FItemSetInfoData* GetSetInfoData(int32 SetUniqueKey);
	FItemSetInfoData* GetSetInfoData(int32 GroupID, EPCClass PCClass, int32 Grade);

	void GenerateSetKeyList();

	/** Need to call this for all necessary (or at least expected) item info of a stage at beginplay timing, to prevent hitching while gameplay. */
	void PreloadClassAssets(const TArray<int32>& InRefIDList);

	/** To be called whenever a stage is about to be unloaded. */
	void UnloadAll();

	/** Provided for special memory saving functionality.
	 * It unload all resources here except mesh/material resource data necessary for compositing mesh resources for local character.
	 * Only for limited places. */
	void UnloadAllExceptCurrentLocalEquipPartData();

private:
	/** To be used by FSingleItemInfoData, for loading each TAsset resources */
	FSingleItemInfoLoadedPtrData* GetLoadedPtrForID(int32 InItemID);

	FORCEINLINE FStreamableManager& GetInfoLoadManager() { return InfoLoadManager; }
	friend struct FSingleItemInfoData;
public:

	/**
	 * Spawns one drop item of specified ItemRefID.
	 * It will set some of SpawnParameters (bDefer** stuff), to true internally to divide spawning procedure.
	 */
	class AB2DropItem* SpawnSingleDropItem(class UWorld* SpawnWorld, int32 ItemRefID, const FTransform& SpawnTransform, const FActorSpawnParameters& SpawnParameters, float StayTime = 0.0f);

	/** Temporary function. until get stage reward from server*/
	int32 GetRandomItemRefID();
#if WITH_EDITOR
	/** For safe use of TSoftObjectPtr in editor.
	 * Arguments are totally optional only for editor splash progress. */
	void EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr = nullptr, float InStartProgress = 0.0f, float InEndProgress = 0.0f);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	class UMaterialInterface* GetItemIcon(int32 ItemRefId);
	class UMaterialInterface* GetSetItemIcon(int32 SetUniqueKey);

	/** You may optionally attach certain prefix by ItemQuality. ItemQuality should be in range 1 ~ MAX_ITEM_QUALITY in that case. */
	FText GetItemName(int32 ItemRefId, bool bAttachQualityPrefix = false, int32 ItemQuality = 0);
	//FText GetSetItemName(int32 SetUniqueKey);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	static void GetItemIconAndName(int32 ItemRefId, class UMaterialInstance*& ItemIcon, FText& ItemName);

	// =========================================================== 
	// From here, Async load interfaces, partly overriding IB2LoadInterface interfaces, 
	// partly new interfaces for its own need.

	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList) override;

	/**
	 * It will be the main function, that you use when you request async loading of part assets that needed for a character mesh composition.
	 * Similar to PreloadClassAssets, but it does not block to load, and loading will be completed later. In addition, not all assets are loaded, only for equipment parts.
	 */
	bool TryAsyncLoadOfEquipParts(const TArray<int32>& InRefIDList, FString& OutRequestedName, bool bShowBlockingUI);

	/** You get a data struct that needed for async load interface use, with a unique identification of designated RefID list. */
	bool GetAsyncReqCombinedIDofPartsList(const TArray<int32>& InRefIDList, 
		FItemInfoEquipPartsAsyncReqCombinedID& OutNewOrGeneratedID,
		bool bGenerateNewOnMiss = false /* False might return invalid data, True might increase internal serial number and array. */);

	void ResetAsyncLoadReqInfo();

	FItemInfoEquipPartsAyncLoadCompleteEvent OnEquipPartsAyncLoadComplete;
	// =========================================================== 
};

USTRUCT()
struct FB2ItemMiscInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FB2ItemMiscInfoLoadedRefHolder()
	{
		ClearAll();
	}

	// All loaded references for TAsset members of UB2ItemMiscInfo

	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedItemIconBGMtrlPerGrade;
	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedItemIconBGMtrlPerGrade_Simple;

	
	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedEquipCategoryIcon;	

	/** Equipped item icon background material. Element index is StarGrade - 1 */
	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedEquipIconBGMtrlPerGrade;

	UPROPERTY(Transient)
	class UMaterialInterface* LoadedItemStarGradeImage_Normal;
	UPROPERTY(Transient)
	class UMaterialInterface* LoadedItemStarGradeImage_Surpassed;

	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedItemQualityBGMtrl;

	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedEtherSetBGMtrl;

	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedEtherPosBGMtrl;

	UPROPERTY(Transient)
		TMap<int32, class UMaterialInterface*> LoadedEtherTierBGMtrl;

	UPROPERTY(Transient)
		TMap<int32, class UMaterialInterface*> LoadedStarNumberMtrl;

	UPROPERTY(Transient)
		TMap<int32, class UMaterialInterface*> LoadedEtherTierNameBGMtrl;

	/** costume item icon background material. Element index is StarGrade - 1 */
	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedCostumeIconBGMtrlPerGrade;

	/** Totem item icon material. */
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* LoadedTotemIcon_NotSmelted;

	UPROPERTY(Transient)
	TMap<int32, class UMaterialInterface*> LoadedTotemOptionIndexIcon;

	void ClearAll()
	{
		LoadedItemIconBGMtrlPerGrade.Empty();
		LoadedItemIconBGMtrlPerGrade_Simple.Empty();
		
		LoadedEquipCategoryIcon.Empty();

		LoadedItemQualityBGMtrl.Empty();
		LoadedEquipIconBGMtrlPerGrade.Empty();
		LoadedItemStarGradeImage_Normal = NULL;
		LoadedItemStarGradeImage_Surpassed = NULL;
		LoadedItemQualityBGMtrl.Empty();
		LoadedEtherSetBGMtrl.Empty();
		LoadedEtherPosBGMtrl.Empty();
		LoadedEtherTierBGMtrl.Empty();
		LoadedEtherTierNameBGMtrl.Empty();
		LoadedCostumeIconBGMtrlPerGrade.Empty();
		LoadedTotemIcon_NotSmelted = NULL;
		LoadedTotemOptionIndexIcon.Empty();
		LoadedStarNumberMtrl.Empty();
	}
};

/**
 * Resource table for items, but they are not bound to specific RefID.
 * It could be a part of SomeInfo, but it has its own domain to be separated.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2ItemMiscInfo : public UObject
{
	GENERATED_BODY()

	FStreamableManager InfoLoadManager;
	
public:
	UB2ItemMiscInfo(const FObjectInitializer& ObjectInitializer);

protected:
	/** For general (Inventory stored, reward, etc) item icon background material. Element index is StarGrade - 1 */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> ItemIconBGMtrlPerGrade;
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> ItemIconBGMtrlPerGrade_Simple;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> ConsumablesIconBGMtrl;

public:
	class UMaterialInterface* GetItemIconBGMtrl(int32 InStarGrade, bool bSimpleBG, bool bForConsumables);
	void UnloadItemIconBGMtrl(int32 InStarGrade, bool bSimpleBG);

protected:
	/** Element index is StarGrade - 1 */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> EquipCategoryIcon;
	
public:
	class UMaterialInterface* GetEquipCategoryIcon(EEquipCategoryType InEquipCategory);
	void UnloadEquipCategoryIcon(EEquipCategoryType InEquipCategory);
	FText GetEquipCategoryName(EEquipCategoryType InEquipCategory);

protected:
	/** More simplified equipped item icon background material. Element index is StarGrade - 1 */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> EquipIconBGMtrlPerGrade;
public:
	class UMaterialInterface* GetEquipIconBGMtrl(int32 InStarGrade);
	void UnloadEquipIconBGMtrl(int32 InStarGrade);


protected:
	UPROPERTY(EditAnywhere)
	TArray<FSlateColor> ItemNameTextColorPerGrade;
public:
	FSlateColor GetItemNameTextColor(int32 InStarGrade);

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> ItemStarGradeImage_Normal;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> ItemStarGradeImage_Surpassed;
public:
	class UMaterialInterface* GetItemStarGradeImage(bool bSurpassed);
	void UnloadItemStarGradeImage(bool bSurpassed);

protected:
	/** Tiny background under quality text. Element index is quality - 1. */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> ItemQualityBGMtrl;
public:
	class UMaterialInterface* GetItemQualityBGMtrl(int32 InQuality);
	void UnloadItemQualityBGMtrl(int32 InQuality);

protected:
	UPROPERTY(EditAnywhere)
		TMap<int32, TSoftObjectPtr<class UMaterialInterface>> EtherPosBGMtrl;
	UPROPERTY(EditAnywhere)
		TMap<int32, TSoftObjectPtr<class UMaterialInterface>> EtherTierBGMtrl;
	UPROPERTY(EditAnywhere)
		TMap<int32, TSoftObjectPtr<class UMaterialInterface>> EtherTierNameBGMtrl;
	UPROPERTY(EditAnywhere)
		TMap<int32, FSlateColor> EtherTierTextColor;

public:
	class UMaterialInterface* GetEtherPosBGMtrl(int32 ElemIndex);
	void UnloadEtherPosBGMtrl(int32 ElemIndex);

	class UMaterialInterface* GetEtherTierBGMtrl(int32 EtherTier);
	void UnloadEtherTierBGMtrl(int32 EtherTier);

	class UMaterialInterface* GetEtherTierNameBGMtrl(int32 EtherTier);
	void UnloadEtherTierNameBGMtrl(int32 EtherTier);

	FSlateColor GetEtherTierTextColor(int32 EtherTier);

protected:
	UPROPERTY(EditAnywhere)
		TMap<int32, TSoftObjectPtr<class UMaterialInterface>> StarNumberMtrl;
public:
	class UMaterialInterface* GetStarNumberMtrl(int32 ElemIndex);
	void UnloadStarNumberMtrl(int32 ElemIndex);

protected:
	/** More simplified costume item icon background material. Element index is StarGrade - 1 */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> CostumeIconBGMtrlPerGrade;
public:
	class UMaterialInterface* GetCostumeIconBGMtrl(int32 InStarGrade);
	void UnloadCostumeIconBGMtrl(int32 InStarGrade);

protected:
	/** Totem item icon material. */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> TotemIcon_NotSmelted;
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class UMaterialInterface>> TotemOptionIndexIcon;
public:
	class UMaterialInterface* GetTotemIconNotSmelted();
	void UnloadTotemIconNotSmelted();

	class UMaterialInterface* GetTotemOptionIndexIcon(int32 InIndex);
	void UnloadTotemOptionIndexIcon(int32 InIndex);

private:
	UPROPERTY(Transient)
	FB2ItemMiscInfoLoadedRefHolder AllLoadedRefHolder; // Loaded references preventing GC.
	// Common util for material cache maps of AllLoadedRefHolder
	void AddOrUpdateMtrlRefHolderCache(int32 EntryIndex, UMaterialInterface* MtrlToCache, TMap<int32, class UMaterialInterface*>& HolderMap);
public:
	
	void UnloadAll();
	
#if WITH_EDITOR
	//void EditorLoadAll();
#endif
	
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};