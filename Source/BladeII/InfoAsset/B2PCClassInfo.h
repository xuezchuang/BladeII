// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "B2MessageInfo.h"
#include "BladeIIPlayer.h"
#include "CommonStruct.h"
#include "Engine/StreamableManager.h"
#include "BladeII.h"
#include "B2InGameHUDInfo_Player.h"
#include "B2PCClassInfo.generated.h"

/** Defines player characters part, where each part is represented by a single mesh asset and being a "part" to combining the whole character mesh.
 * It can be related to EItemEquipPlace, but not like 1:1 mapping. */
UENUM()
enum class EPCPart : uint8
{
	// Must be sequential, as their casted value can be array index.
	EPCPT_Face = 0,
	EPCPT_Head = 1,
	EPCPT_Body = 2,
	EPCPT_Leg = 3,
	EPCPT_Arm = 4,
	EPCPT_Weapon = 5,
	EPCPT_Robe = 6, // probably not for all.

	EPCPT_Wing = 7, // Some other part (Robe) can be removed if this is to be attached.

	// Check GetItemEquipPlaceForPCPart and GetPCPartForItemEquipPlace for new entry.
	// New entry must be assigned to higher number.

	EPCPT_End // It can be used as array size when declaring sort of PC part info.
};
/** Find out designated part's material section ID of composite mesh. */
int32 GetPCCompositeMtrlID(EPCPart InPartID);

/** It will returns matching ItemEquipPlace "if any". i.e. not like strict 1:1 mapping. */
EItemEquipPlace GetItemEquipPlaceForPCPart(EPCPart InPCPart);
EPCPart GetPCPartForItemEquipPlace(EItemEquipPlace InEquipPlace);
/** Some parts can be like sub or extra to other part. */
EPCPart GetExtraAttachPartEnum(EPCPart InMainPart);

bool IsCostumeEquipPlace(const ECostumeEquipPlace& InCostumeEquipPlace);


/** See if PartToAdd can be used only when OtherPartToTest should be removed.
 * True return means those two parts are mutually exclusive and PartToAdd is to be used instead of OtherPartToTest. */
bool ShouldRemoveOtherPart(EPCClass InPCClass, EPCPart PartToAdd, EPCPart OtherPartToTest);

/** Struct to define mesh assets for player character's parts (equipment) */
USTRUCT(BlueprintType)
struct FPCPartsAssetInfo
{
	GENERATED_USTRUCT_BODY()

	FPCPartsAssetInfo()
	{
		PartEnum = EPCPart::EPCPT_End;
		MeshAsset = NULL;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPCPart PartEnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMesh* MeshAsset;
};

/**
 * Some of combat ability variable to be defined by PCClassInfo, not by other data get from server.
 */
USTRUCT(BlueprintType)
struct FB2PlayerCombatAbilityEssentialSetupInfo
{
	GENERATED_USTRUCT_BODY()

	FB2PlayerCombatAbilityEssentialSetupInfo()
	{
		MaxArmorOverride = 0.0f;
	}

	/** Armor value is just overridden. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxArmorOverride;

	// Apply new members overriding at ABladeIIPlayer::OverrideByPCClassInfo
};

/** A part of UB2PCClassInfo, it is separated for prevent loading in-game only assets in lobby.
 * Not all in-game only info are here, only some stuff that takes significant memory */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2InGameOnlyPCClassInfo : public UObject
{
	GENERATED_BODY()

protected:
	/**
	* The actual Blueprint class which is used for spawn and overridden by properties here.
	* Leave it as NULL if to use the default one. We provide a default base BP class, but any other special class can be used.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABladeIIPlayer> BaseBPClass;
public:
	TSubclassOf<class ABladeIIPlayer> GetBaseBPClass(UObject* WorldContextObject);

	/** The in-game animation Blueprint, one of the most major asset, 
	 * referencing a lot of anim sequences (eventually other resources by anim notifies) and even containing significant anim driven logic. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimBlueprintGeneratedClass* AnimBPOverride;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* BehaviorTreeOverride;

	//
	// I would like to put HUDInfoClassOverride here, then portraits should not be accessed from HUDInfo.
	//

	/** It is added to the default rather than replace, but guess there is nothing at BaseBP anyway? */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TArray<TSubclassOf<class ABladeIIProjectile>> AdditionalProjectileClasses;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<TSubclassOf<class AB2AreaDamageActorBase>> AdditionalAreaDamageClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor FloorRingColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FloorRingColorMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* FloorRingPSOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* ReadyWeaponSkillFXOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultAttackRangeOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoMoveRangeOverride;

	UB2InGameOnlyPCClassInfo(const FObjectInitializer& ObjectInitializer);
};


/**
 * All information necessary to define a player character class, based on ABladeIIPlayer
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2PCClassInfo : public UObject
{
	GENERATED_BODY()
	
	FStreamableManager InfoLoadManager; // To use TSoftClassPtr
	/** I'm not so sure on async load behavior of FStreamableManager so placed somewhat duplicated variables to check async load state. */
	uint32 bInGameOnlyAssetAsyncLoadRequestPending : 1;
	TSharedPtr<FStreamableHandle> InGameOnlyAssetAsyncLoadHandle;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPCClass PCClassEnum;

	/** Displayed name. Put English name here.
	 * The final localized name for each language will be defined at localization archive file using this string as source. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizedNameKey;
	
	/** 
	 * It will be the main rendering mesh when no other parts are provided.
	 * Otherwise, it provides base skeleton which will be the basis of animation, while this BaseMesh itself is hidden.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMesh* BaseMeshAsset;

	/**
	 * For default - non equipped - player character representation.
	 * Composing them can be just identical to BaseMeshAsset, but it still be needed, as non-equipped part uses this info.
	 */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TArray<FPCPartsAssetInfo> DefaultPartsInfo;
	
protected:
	/**
	 * In-game only assets for saving lobby memory. It is expected to take most memory of here.
	 */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TSoftClassPtr<UB2InGameOnlyPCClassInfo> InGameOnlyAssetPtr;
private:
	UPROPERTY(Transient)
	TSubclassOf<UB2InGameOnlyPCClassInfo> LoadedInGameOnlyAssetPtr; // Keep the loaded Info asset reference to prevent it get GCd.
public:
	/** It will try load InGameOnlyAsset. It will check if not being called from LobbGameMode. */
	UB2InGameOnlyPCClassInfo* GetInGameOnlyInfo(UObject* WorldContextObject);
	bool IsInGameOnlyInfoAtRootSet();

	/** Use this for Pre-load */
	void ReqAsyncLoad(UObject* WorldContextObject, TAsyncLoadPriority InPriority = FStreamableManager::DefaultAsyncLoadPriority);
private:
	void InGameOnlyAssetAsyncLoadCB();
	void ConditionalSetLoadedInGameOnlyInfoRootSet();
public:
	TSubclassOf<class ABladeIIPlayer> GetBaseBPClass(UObject* WorldContextObject);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2InGameHUDInfo_Player> HUDInfoClassOverride;

	/** Uniform scale for now. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshScaleOverride;

	/** For the root capsule collision component. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleHalfHeightOverride;

	/** For the root capsule collision component. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleRadiusOverride;

	/** Additional offset applied when spawning damage num effect. 0 for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageNumZOffsetOverride;
	
	/** Information array to create additional attached static mesh components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCHSKCompAttachStaticMeshInfo> AttachStaticMeshes;

	/** Information array to create additional attached skeletal mesh components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCHSKCompAttachSkeletalMeshInfo> AttachSkeletalMeshes;

	/** Information array to create additional attached particle system components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCHSKCompAttachParticleSystemInfo> AttachParticleSystems;

	/** Selected properties of CharacterMovementComponent */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FB2CharMovementEssentialSetupInfo CharMovementEssentialSettings;
	
	/** Selected properties that will affect character's combat abilities. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FB2PlayerCombatAbilityEssentialSetupInfo CombatAbilityEssentialSettings;
	
	/** 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectDistanceOverride;

	/** 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingDistanceOverride;

	/** Auto AI's attack limit. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistanceToAllowAutoAttackOverride;

	/** Override to BTTask_MoveTo::AcceptableRadius*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveToMobAcceptableRadiusOverride;

	UPROPERTY()
	uint32 bOverride_TargetingMode : 1;

	/** Will be overridden if it's other than ETM_End. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverride_TargetingMode"))
	ETargetingMode TargetingModeOverride;

	//
	// A lot to be added...
	//
	// Apply additional items at ABladeIIPlayer::OverrideByPCClassInfo
	//


public:
	virtual void PostLoad() override;

	UB2PCClassInfo(const FObjectInitializer& ObjectInitializer);
	
	/** Just a simple mapping.. */
	static ECharacterType FromPCClassToCharacterType(EPCClass InPCClassEnum);
	static EPCClass FromCharacterTypeToPCClass(ECharacterType InChTypeEnum);
	

	/** Get class code name with common prefix trimmed from InPCClassEnum. See also ABladeIIPlayer::GetTrimmedNameFromPCClassEnum */
	static FString GetTrimmedNameOfPCClassEnum(EPCClass InPCClassEnum);

	FText GetLocalizedName();

	/**
	 * PCClassInfo utility to set-up a designated skeletal mesh component being set-up by merged mesh (or maybe by child components) representing equipped items,
	 * In common in-game environment, SKCompToSetup is supposed to be player character's main body component, but it could be something else (e.g. for lobby).
	 * Likewise, SKCompOwner can be either BladeIIPlayer or SkeletalMeshActor.
	 */
	static bool SetupSKCompForParts(EPCClass InPCClass, class AActor* SKCompOwner, class USkeletalMeshComponent* SKCompToSetup,
		class USkeletalMesh*& InOutPrebuiltCompositeMesh, // When it is provided, it is used instead of real merging process, or the result mesh is returned through this.
		class USkeletalMesh* InBaseMesh,
		const TArray<FPCPartsAssetInfo>& InDefaultParts,
		const TArray<FB2Item>& InEquippedItems,
		const FB2Wing* InWingData, // NULL if wing is not available
		class UAnimBlueprintGeneratedClass* AnimBPForInGameChar,
		bool bForceUnloadPartResourceAfterMerge, // For memory saving.. Set it true if you build a mesh only for temporary reason.. like collect book.
		bool bInMergeSections,
		bool bBuildAndGetResultMeshOnly = false // If true, it won't set the result SkeletalMesh to the component, you get the result mesh by InOutPrebuiltCompositeMesh
		);
private:
	/** Destroy any attached component, created by FPCPartExtraAttackFxSet (and anything that can be possibly added), if exist by previous setup. */
	static void DestroyExtraAttachPartComponents(class AActor* SKCompOwner, class USkeletalMeshComponent* SKCompToSetup);

public:

	/** In-game and lobby common assets are always loaded when a PCClassInfo asset is loaded from box.
	 * In-game only assets are loaded only when it is requested. */
	void SyncLoadInGameOnlyAssets();
	void UnloadInGameOnlyAssets();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Check the enum entry name to not being out of certain rule.. */
	static bool CheckPCClassEnumNamingConvention();
#endif
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};


/**
 * ======================================================================================================
 * For dynamically merge player character mesh sections
 * ======================================================================================================
 */


USTRUCT(BlueprintType)
struct FMaterialParamterReplace
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName OriginalMtrlParam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ReplaceMtrlParam;
};



USTRUCT(BlueprintType)
struct FMergeSectionParameter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPCPart TargetPart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D UVOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMaterialParamterReplace> ScalarParams;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMaterialParamterReplace> VectorParams;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMaterialParamterReplace> TextureParams;
};

/** Data info struct of parts group that still being a single section after section merge. */
USTRUCT(BlueprintType)
struct FPCMeshSectionMergeGroupData
{
	GENERATED_USTRUCT_BODY()

	FPCMeshSectionMergeGroupData()
	{
		SectionMaterial = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SectionID;

	// I would like to make it as TSoftObjectPtr but somethings wrong..?
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMaterialInterface* SectionMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMergeSectionParameter> MergeMtrlParameters;
};


USTRUCT(BlueprintType)
struct FCostumeReplaceData
{
	GENERATED_USTRUCT_BODY()

	FCostumeReplaceData() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ECostumeEquipPlace CostumeEquipPlace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<EPCPart> CombineCostumePCPart;

	//1개 이상이면... 코스튬 한 부위가 다른 파트 여러개를 잡는거임
	FORCEINLINE bool IsCombineCostumeEquipPlace() { return CombineCostumePCPart.Num() > 1;	}
};


/**
 * Not only containing editable information for mesh section merge, but also being a namespace for related operations
 */
UCLASS(BlueprintType, Blueprintable)
class UB2PCMeshSectionMergeInfo : public UObject
{
	GENERATED_BODY()

public:
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	/** It will be its usage flag until formally added to our project. */
	static int32 SectionMergeTestFlag;
	static bool ShouldForceUseSectionMerge();
	static bool ShouldForceUnuseSectionMerge();
#endif
	static bool ShouldUseSectionMergeByGameContext(UObject* WorldContextObject);
	static bool ShouldUseSectionMergeForStageEvent(UObject* WorldContextObject);
	

protected:

	/** There will be separated section even after section merge, just reduced.
	 * This data is per section that might still exist in multiple number even after the merging operation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPCMeshSectionMergeGroupData> MergedSectionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPCMeshSectionMergeGroupData> CombineCostumeMergedSectionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCostumeReplaceData> CostumeReplaceData;

public:

	UB2PCMeshSectionMergeInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	/** Get section data that will be applied after merge. not before merge. */


	const FPCMeshSectionMergeGroupData* GetMergeSectionData(bool InHaveCombineCostume, EPCPart InPartEnum) const;
	const FMergeSectionParameter* GetParamterReplaceInfo(bool InHaveCombineCostume, EPCPart InPartEnum) const;

	int32 GetMergedSectionID(bool InHaveCombineCostume, EPCPart InPartEnum) const;

	class UMaterialInterface* GetMergedSectionMaterial(bool InHaveCombineCostume, EPCPart InPartEnum) const;
	FVector2D GetPartUVOffset(bool InHaveCombineCostume, EPCPart InPartEnum) const;

	/** Based on original material (of non merged mesh), set up material parameters for merged sections, 
	 * assuming that InSectionMergedMesh has materials specially prepared for merged section. */
	void SetupMergedSectionMaterialParams(bool InHaveCombineCostume, class USkeletalMesh* InSectionMergedMesh, TMap<EPCPart, class UMaterialInterface*>& OriginalNonMergedMaterials);

	//아바타
	const FCostumeReplaceData* GetCostumeReplaceData(ECostumeEquipPlace InCostumeEquipPlace) const;
	void GetReplaceCostumeEquipPlace(ECostumeEquipPlace InCostumeEquipPlace, TArray<EPCPart>& OutPCPartReplace);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void CheckDataIntegrity();
#endif
};