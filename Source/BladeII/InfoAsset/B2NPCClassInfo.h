// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "BladeIICharacter.h"
#include "../BladeII/BladeIIDamageType.h"
#include "B2NPCClassInfo.generated.h"

/** UI related settings of FNPCSingleClassInfo struct. */
USTRUCT(BlueprintType)
struct FNPCSingleClassInfo_UI
{
	GENERATED_USTRUCT_BODY()

	FNPCSingleClassInfo_UI()
	{
		PortraitMIC = NULL;
	}

	/** For Boss mob's portrait icon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMaterialInterface* PortraitMIC;
};

/**
 * Actual information element that composes NPC class.
 * Defines a NPC class per ENPCClass and ENPCClassVariation
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2NPCSingleClassInfo : public UObject
{
	GENERATED_BODY()

public:

	UB2NPCSingleClassInfo(const FObjectInitializer& ObjectInitializer);
	
	/** The most basic identification for Info settings contained here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ENPCClass NPCClassEnum;

	/** Identification within a single NPCClass (which comes from its owning UB2NPCClassInfo). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ENPCClassVariation ClassVariation;

	/** Displayed name. Put English name here.
	 * The final localized name for each language will be defined at localization archive file using this string as source. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizedNameKey;
	FText GetLocalizedName();

	/** Almost for area boss mobs. This is also a localized property. Edit in English only. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LocalizedNickNameKey;
	FText GetLocalizedNickName();

	/** It will be overridden anyway.
	 * Even with this setting, the finally considered character type from monster spawn pool can be different by MonsterClassTypeMap,
	 * and will be overridden by that info if spawned through monster spawn pool with MonsterClassTypeMap */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//ECharacterType DefaultCharacterType;

	/** It is automatically suggested when empty (None), based on class enum and variation to access NPCSensitiveInfo data.
	 * You may edit it in any name you want, just ensure NPCSensitiveInfo has matching row entry. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SensitiveDataRowKey;

//protected:
//	/**
//	 * The actual Blueprint class which is used for spawn and overridden by properties here.
//	 * Leave it as NULL if to use the default one. We provide a default base BP class, but any other special class can be used.
//	 */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//	TSubclassOf<class ABladeIICharacter> BaseBPClass;
public:
	TSubclassOf<class ABladeIICharacter> GetBaseBPClass(UObject* WorldContextObject);

	// Put any asset references and/or properties of BladeIICharacter which is expected to have different values among classes.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMesh* MeshAssetOverride;

	/** Mesh offset from the root component (capsule component). Z offset is added to capsule component's half height. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector MeshRelativeOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HPBarSocketName;

	/** Material override for each mesh element. Array index matches to the element index.
	 * Nulled element won't be overridden. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class UMaterialInterface*> MaterialOverrides;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimBlueprintGeneratedClass* AnimBPOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* BehaviorTreeOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIgnoreRotateByDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFixInAir;

	/** All UI-related info. It is almost required for possible Boss mobs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FNPCSingleClassInfo_UI NPCUIInfo;

	/** Uniform scale for now. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MeshScaleOverride;

	/** For the root capsule collsion component. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleHalfHeightOverride;

	/** For the root capsule collision component. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleRadiusOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> CapsuleCollisionObjectTypeOverride;

	/** For center shadow component size. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShadowSizeOverride;

	/** Additional offset applied when spawning damage num effect. 0 for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageNumZOffsetOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bForceReceivdAbsoluteDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bForceReceivdAttackCountDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FullAttactCountSize;

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

	/** If checked, NPC is initially inactive (playing simple staying motion) right after spawn, then start moving once detected player (by DetectDistance)
	 * Most likely for NPCs having spawn stay motion. AI BehaviorTree must have supporting node to make it active. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bUsePreSpawnState : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint32 bUsePhaseChangeAnimation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQTEType QTEType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float QTEMountTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float QTEEnableTime;

	/** At which percent (0.0 ~ 100.0) of full HP this mob can enter QTE mode?
	 * Almost for "boss" mobs. 0 means no QTE for this character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float QTEInvokeHPPercent;

	/** 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectDistanceOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RegenerateHPPercentagePerSecondOverride;

	/** It is added to the default rather than replace, but guess there is nothing at BaseBP anyway? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class ABladeIIProjectile>> AdditionalProjectileClasses;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<TSubclassOf<class AB2AreaDamageActorBase>> AdditionalAreaDamageClasses;

	/** Like AdditionalProjectileClasses, added to the default AIAttackDataArray */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAIAttackData> AdditionalAIAttackDataArray;

	/** Another AI data defining how a character's mind getting fucked up and shit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAIPhaseData> AdditionalPhaseDataArray;

	/** Miscellaneous stuff of DropItem. More specific information will be given by server and/or ItemInfo, etc.. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDropItemInfo DropItemInfo;

	/** Must set this. Will be overridden anyway unless we provide other special flag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageStateConfigData AllowedDamageState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SpawnNotifyMsgString;
	 
	// If you want to visible until special case, use time Amount 0. And you must specify the end of the popup.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnNotifyMsgTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowRecoilAnimation;

	// Initial Influence
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitialInfluence")
	EInfluenceType InfluenceType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitialInfluence")
	float InfluenceAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitialInfluence")
	float InfluenceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitialInfluence")
	float InfluenceStateDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitialInfluence")
	float InitialInfluenceStateTriggerRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	bool bBloodStoneAura;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	//EBloodStoneType BloodStoneType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	float BloodStoneAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	EDamageLogicType  BloodStoneBuffLogicType;

	virtual void PostLoad() override;

	/** Get all possible NPCClasses that can be required by phase setting of this class  */
	void GetAllExpectedPhaseClasses(TArray<FCombinedNPCClassID>& OutClassIDs);

#if !UE_BUILD_SHIPPING
	/** Find matching NPCClass enum entry from a string, expecting the string has no common prefix "ENC_"
	 * Return ENC_End if nothing found. */
	static ENPCClass FindNPCClassEnumFromTrimmedName(FString InTrimmedName);
	/** Find matching NPCClassVariation enum entry from a string, expecting the string has no common prefix "ENCV_"
	 * Return ENCV_Normal if nothing found. */
	static ENPCClassVariation FindNPCClassVariEnumFromTrimmedName(FString InTrimmedName);
#endif

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

