// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
// Author : leejswo(SangwooLee)
#pragma once
////#include "BladeII.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "CommonStruct.h"
#include "BladeIIDamageType.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkinnedMeshComponent.h"
#include "UObject/ObjectSaveContext.h"
#include "BladeIIProjectile.h"
#include "B2AreaDamageActorBase.h"
//#include "B2FloatingAbnormalEffect.h"
//#include "UObject/ObjectSaveContext.h"
#include "B2CharacterBuffManager.h"
#include "BladeIICharacter.generated.h"
//
/** To categorize characters. */
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	// List PCs here.

	// For in-game spawn, get those values from B2PCClassInfo. See ABladeIIPlayer::OverrideByPCClassInfo
	ECT_UnknownPC = 0,

	ECT_Assassin = 1,
	ECT_Gladiator = 2,
	ECT_Wizard = 3,
	ECT_Fighter = 4,

	ECT_PCEnd, // for categories
	// Check UB2PCClassInfo::FromPCClassToCharacterType and FromCharacterTypeToPCClass for additional PC entry.

	// List NPCs below

	// For in-game spawn NPCs, CharacterType can come be set by NPCClassInfo (UB2NPCClassInfo::SpawnNPCClassInfoCharacter), or MonsterSpawnPool (AB2MonsterSpawnPool::SpawnWave)
	ECT_MeleeMob,
	ECT_RangeMob,
	ECT_MiddleMob,
	ECT_StoneMob,

	ECT_MobEnd, // for categories

	ECT_BossMob,

	ECT_Mercenary,

	ECT_End
};

/** NPC classes identification except variation information. */
UENUM(BlueprintType)
enum class ENPCClass : uint8
{
	// List all the NPC classes based on ABladeIICharacter here.

	ENC_Mob_DummyBegin = 0, // Dummy 0.. to make adding new TMap entry easier.

	////////////////////////////////////////////////////////////////////////////////
	// Declared symbols below must be known to the server side too.
	// There must be an excel script file (.xlsm) having the list of declared names here at a specific path of perforce depot.
	// A new entry of the same name must be added to that script file with other server side identifications.
	////////////////////////////////////////////////////////////////////////////////

	ENC_Mob_AncientKnight,
	ENC_Mob_AngelKnight01,
	ENC_Mob_AngelKnight02,
	ENC_Mob_Bat,
	ENC_Mob_Butcher,
	ENC_Mob_Butcher_Axe,
	ENC_Mob_Cyclops,
	ENC_Mob_Daniel,
	ENC_Mob_DarkPriest,
	ENC_Mob_DevilNest,
	ENC_Mob_Dog,
	ENC_Mob_EvilDog,
	ENC_Mob_Gargoyle,
	ENC_Mob_GG,
	ENC_Mob_Ghost,
	ENC_Mob_Ghoul,

	ENC_Mob_Goblin_Boss,
	ENC_Mob_Goblin_Bow,
	ENC_Mob_Goblin_Bowling,
	ENC_Mob_Goblin_King,
	ENC_Mob_Goblin_Magician,
	ENC_Mob_Goblin_Sword,
	ENC_Mob_Goblin_Treasure,
	ENC_Mob_Goblin_Wolverine,

	ENC_Mob_Gregor,
	ENC_Mob_MeltSpider,
	ENC_Mob_Mimic,
	ENC_Mob_Mimic_Small,
	ENC_Mob_Minotaur,
	ENC_Mob_Perez,
	ENC_Mob_PoisonEye,
	ENC_Mob_Rachel,
	ENC_Mob_Rat,
	ENC_Mob_Rat_Baby,

	ENC_Mob_Skell_Sword,
	ENC_Mob_Skell_Bow,

	ENC_Mob_Soldier_Crossbow,
	ENC_Mob_Soldier_Elite,
	ENC_Mob_Soldier_Normal,
	ENC_Mob_Soldier_Sword,

	ENC_Mob_Spider,
	ENC_Mob_SpiderBomb,

	ENC_Mob_Werewolf,

	ENC_Mob_Zombie_Fat,
	ENC_Mob_Zombie_Female,
	ENC_Mob_Zombie_Male,

	ENC_Mob_Stone_Red,
	ENC_Mob_Stone_Yellow,
	ENC_Mob_Stone_Purple,
	ENC_Mob_Stone_Green,

	ENC_Mob_Verace_Leg,

	// PCMobs are not in alphabetical order, in order of EPCClass..
	ENC_PCMob_Gladiator,
	ENC_PCMob_Assassin,
	ENC_PCMob_Wizard,
	ENC_PCMob_Fighter,

	// Put a new Boss mob ID to SpawnPoolAsyncBlacklist, as it possibly uses BossMob_Master_AnimBP, which takes more time to load.

	ENC_Boss_Apocalypse01,
	ENC_Boss_Apocalypse02,
	ENC_Boss_Apocalypse03,
	ENC_Boss_Apocalypse04,
	ENC_Boss_Azrael,
	ENC_Boss_Azrael_Horse,
	ENC_Boss_Beelzebub,
	ENC_Boss_Caligula,
	ENC_Boss_Dark,
	ENC_Boss_Harang,
	ENC_Boss_Janne,
	ENC_Boss_Lilith,
	ENC_Boss_Magnus,
	ENC_Boss_Magnus_P2, // Phase 2
	ENC_Boss_Nidhogg,
	ENC_Boss_Sargon,
	ENC_Boss_Sargon_P2, // Phase 2
	ENC_Boss_Daniel,
	ENC_Boss_Remiel,
	ENC_Boss_Verace,
	ENC_Boss_Verace_P2,	// Phase 2
	ENC_Boss_Musca,
	ENC_Boss_Camael,

	//>> Reserve# are specially assigned for emergency use. See UB2NPCClassInfo::PostEditChangeProperty
	ENC_Reserve01,
	ENC_Reserve02,
	ENC_Reserve03,
	//<<

	//GuildNPC
	ENC_Guild_Soldier_Sword,
	ENC_Guild_Soldier_Spear,
	ENC_Guild_Soldier_Bow,
	ENC_Guild_Goat,
	ENC_Guild_Gregor,
	ENC_Guild_Bertha,
	ENC_Guild_Azrael,
	ENC_Guild_Daniel,
	ENC_Guild_Miracle,
	ENC_Guild_Ramiel,

	ENC_End
};

#define ENPCCLASS_ENUM_PACKAGE_NAME TEXT("ENPCClass")
#define ENPCCLASSENUM_COMMON_PREFIX TEXT("ENC_")

UENUM(BlueprintType)
enum class EQTEType : uint8
{
	EQT_Normal = 0,
	EQT_Mount,
	EQT_Break,
	EQT_End
};
//
/** Variations for each NPC Class. Could be functional variation or just appearance? */
UENUM(BlueprintType)
enum class ENPCClassVariation : uint8
{
	ENCV_Normal,
	ENCV_Unique,
	ENCV_Unique_Boss,
	ENCV_Superior,
	ENCV_Superior_Boss,
	ENCV_Hell,
	ENCV_Hell_Boss,
	ENCV_CounterDungeon, //-> It is to be removed. ENC_Mob_Butcher_Axe was once this variaion of ENC_Mob_Butcher.
	ENCV_ControlBattle,
	ENCV_GuildBattle,
};
#define ENPCCLASSVARI_ENUM_PACKAGE_NAME TEXT("ENPCClassVariation")
#define ENPCCLASSVARIENUM_COMMON_PREFIX TEXT("ENCV_")

/** Just combined enum info.. */
struct FCombinedNPCClassID
{
	FCombinedNPCClassID()
	{
		NPCClassEnum = ENPCClass::ENC_End;
		NPCVariationEnum = ENPCClassVariation::ENCV_Normal;
	}
	FCombinedNPCClassID(ENPCClass InClassEnum, ENPCClassVariation InVariEnum)
	{
		NPCClassEnum = InClassEnum;
		NPCVariationEnum = InVariEnum;
	}
	FORCEINLINE bool operator==(const FCombinedNPCClassID& InOther) const
	{
		return (this->NPCClassEnum == InOther.NPCClassEnum && this->NPCVariationEnum == InOther.NPCVariationEnum);
	}
private:
	ENPCClass NPCClassEnum;
	ENPCClassVariation NPCVariationEnum;
public:
	FORCEINLINE ENPCClass GetNPCClassEnum() const
	{
		return NPCClassEnum;
	}
	FORCEINLINE ENPCClassVariation GetNPCVariationEnum() const
	{
		return NPCVariationEnum;
	}
	FORCEINLINE uint32 GetMyTypeHash() const
	{
		return static_cast<uint32>(NPCClassEnum) * 100 + static_cast<uint32>(NPCVariationEnum);
	}
};

/** For use of TMap with FCombinedNPCClassID key */
FORCEINLINE uint32 GetTypeHash(const FCombinedNPCClassID& InId)
{
	return InId.GetMyTypeHash();
}

/** To categorize characters. */
UENUM(BlueprintType)
enum class ETeamType : uint8
{
	ETT_Red = 0,
	ETT_Blue = 1,
	ETT_Mob = 11,
	ETT_End
};

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	ECS_None = 0,
	ECS_Dash,
	ECS_Combo01,
	ECS_Combo02,
	ECS_Combo03,
	ECS_Combo04,
	ECS_Combo05,
	ECS_Combo06,
	ECS_Skill01 = 10,
	ECS_Skill02,
	ECS_Skill03,
	ECS_Skill04,
	ECS_Skill05,
	ECS_Skill06,
	ECS_TagOtherChar,
	ECS_WeaponSkill,
	ECS_GuardStart = 20,
	ECS_Guard = 21,
	ECS_GuardEnd = 22,
	ECS_Evade = 23,
	ECS_Counter01 = 30,
	ECS_Counter01Dash,
	ECS_Counter02,
	ECS_Counter02Dash,
	ECS_Counter03,
	ECS_Counter03Dash,
	ECS_Counter04,
	ECS_Counter04Dash,
	ECS_Counter05,
	ECS_Counter05Dash,

	// Check Is**AttackState below when you add a new entry here.

	ECS_End
};

FORCEINLINE bool IsOneOfComboAttackState(EAttackState InAttackState)
{
	return (InAttackState >= EAttackState::ECS_Combo01 && InAttackState <= EAttackState::ECS_Combo06);
}
FORCEINLINE bool IsOneOfSkillAttackState(EAttackState InAttackState)
{ // Including Tag and WeaponSkill.
	return (InAttackState >= EAttackState::ECS_Skill01 && InAttackState < EAttackState::ECS_GuardStart);
}
FORCEINLINE bool IsCounterAttackingState(EAttackState InAttackState)
{
	// Including counter dash.
	return (InAttackState >= EAttackState::ECS_Counter01 && InAttackState <= EAttackState::ECS_Counter05Dash);
}
FORCEINLINE bool IsGuardingState(EAttackState InAttackState)
{
	// I just thought that GuardEnd can be counted as guarding, by looking at the state machine of PC AnimBP
	return (InAttackState >= EAttackState::ECS_GuardStart && InAttackState < EAttackState::ECS_GuardEnd);
}

/** For handling stage event animation by animation blueprint state */
UENUM(BlueprintType)
enum class EStageEventShowState : uint8
{
	ESES_None, // Other normal animation states (e.g. by AttackState) are used.
	ESES_Appearing, // Stage start in the case of player character, any kind of spawning moment in other cases.
	ESES_Dead,
	ESES_VictoryMain, // Stage clear in the case of main player character
	ESES_VictorySub, // Stage clear in the case of sub player character
	ESES_Extra // To set the show state, when this actor is not really the main part, to prevent unwanted motion.
};

UENUM(BlueprintType)
enum class EBloodStoneType : uint8
{
	EBST_NULL = 0,
	EBST_UnBeatable = 1, // Target on Others
	EBST_AllKill,
	EBST_OnFire = 100, // Target on My Character
	EBST_OnPoision
};

USTRUCT(BlueprintType)
struct FAIAttackData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = AttackData, EditAnywhere)
	float MinDistance;

	UPROPERTY(Category = AttackData, EditAnywhere)
	float MaxDistance;

	UPROPERTY(Category = AttackData, EditAnywhere)
	float PossibilityWeight;

	UPROPERTY(Category = AttackData, EditAnywhere)
	float MinDelay;

	UPROPERTY(Category = AttackData, EditAnywhere)
	float MaxDelay;

	UPROPERTY(Category = AttackData, EditAnywhere)
	uint32 bUseForRecoverArmor : 1;

	FAIAttackData()
		: MinDistance(0.0f)
		, MaxDistance(270.0f)
		, PossibilityWeight(1.f)
		, MinDelay(0.f)
		, MaxDelay(0.f)
		, bUseForRecoverArmor(false)
	{
	}
};

/** Whether to be affected by each entry in EStateAttackType */
USTRUCT(BlueprintType)
struct FDamageStateConfigData
{
	GENERATED_USTRUCT_BODY()

	/** Each flag here represents if a character will get affected by relevant damage state of EStateAttackType */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAllowStun : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAllowFreezing : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAllowBurning : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAllowPoison : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame) // only animation
		uint32 bAllowKnockBack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame) // only animation
		uint32 bAllowBigBounce : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAllowTaunt : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAllowSeal : 1;

	FDamageStateConfigData()
		: bAllowStun(true)
		, bAllowFreezing(true)
		, bAllowBurning(true)
		, bAllowPoison(true)
		, bAllowKnockBack(true)
		, bAllowBigBounce(true)
		, bAllowTaunt(true)
		, bAllowSeal(true)
	{
	}

	bool IsAllowedState(EStateAttackType StateAttackType) const
	{
		// Allow multiple state change, apply duration decrease gradually
		switch (StateAttackType)
		{
		case EStateAttackType::EDT_Normal:
			return false;
		case EStateAttackType::EDT_Stun:
			return bAllowStun;
		case EStateAttackType::EDT_Freezing:
			return bAllowFreezing;
		case EStateAttackType::EDT_Poison:
			return bAllowPoison;
		case EStateAttackType::EDT_Burning:
			return bAllowBurning;
		case EStateAttackType::EDT_NockBack:
			return bAllowKnockBack;
		case EStateAttackType::EDT_BigBounce:
			return bAllowBigBounce;
		case EStateAttackType::EDT_Seal:
			return bAllowSeal;
		case EStateAttackType::EDT_End:
			return false;
		default:
			return false;
		}
		return false;
	}

	bool IsAllowedBuff(EBuffType InBuffType) const
	{
		switch (InBuffType)
		{
		case EBuffType::Debuff_Burning:
			return bAllowBurning;
		case EBuffType::Debuff_Freezing:
			return bAllowFreezing;
		case EBuffType::Debuff_Poison:
			return bAllowPoison;
		case EBuffType::Debuff_Stun:
			return bAllowStun;
		case EBuffType::Debuff_Taunt:
			return bAllowTaunt;
		case EBuffType::Debuff_Seal:
			return bAllowSeal;
		default:
			return true;
		}
		return true;
	}
};

USTRUCT(BlueprintType)
struct FAIPhaseData
{
	GENERATED_USTRUCT_BODY()

	// Percentage for Max Health
	UPROPERTY(Category = TriggerData, EditAnywhere)
	float HealthTrigger;

	UPROPERTY(Category = TriggerData, EditAnywhere)
	FName TriggerEventName;

	// Spawn as a new Character
	UPROPERTY(Category = PhaseData, EditAnywhere)
	ENPCClass BladeIICharacterClass;

	UPROPERTY(Category = PhaseData, EditAnywhere)
	ENPCClassVariation CharacterClassVariation;

	// New Attack Data...
	UPROPERTY(Category = PhaseData, EditAnywhere)
	TArray<FAIAttackData> AttackDataArray;

	UPROPERTY(Category = PhaseData, EditAnywhere)
	TArray<int32> InvalidAttackIndices;

	UPROPERTY(Category = PhaseData, EditAnywhere)
	float MaxWalkSpeed;

	UPROPERTY(Category = PhaseData, EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(Category = PhaseData, EditAnywhere)
	int32 PhaseAIAttackIndex;

	// Particle Effect
	UPROPERTY(EditAnywhere, Category = ParticleSystem, meta = (ToolTip = "The particle system to spawn for this phase"))
	UParticleSystem* PSTemplate;

	UPROPERTY(EditAnywhere, Category = ParticleSystem, meta = (ToolTip = "The socket or bone to attach the system to"))
	FName SocketName;

	// Offset from the socket / bone location
	UPROPERTY(EditAnywhere, Category = ParticleSystem, meta = (ToolTip = "Offset from the socket or bone to place the particle system"))
	FVector LocationOffset;

	// Offset from the socket / bone rotation
	UPROPERTY(EditAnywhere, Category = ParticleSystem, meta = (ToolTip = "Rotation offset from the socket or bone for the particle system"))
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, Category = ParticleSystem, meta = (DisplayName = "Ignore Custom Time Dilation", ToolTip = "Should Ignore Custom Time Dilation"))
	uint32 bIgnoreCustomTimeDilation : 1;

	// Buff mode change, EBT_End means no change
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialChange)
	EUniqueBuffType BuffModeOverride;

	FAIPhaseData() : BladeIICharacterClass(ENPCClass::ENC_End), CharacterClassVariation(ENPCClassVariation::ENCV_Normal),
		MaxWalkSpeed(0.0f), BehaviorTree(NULL), PhaseAIAttackIndex(INDEX_NONE), PSTemplate(NULL), BuffModeOverride(EUniqueBuffType::EBT_End)
	{
	}

	UPROPERTY(Category = PhaseData, EditAnywhere)
	FName DialogCodeNameOnPhaseChanged;
};

/**
 * It is now somewhat miscellaneous stuff of DropItem. More specific information will be given by server and/or ItemInfo, etc..
 */
USTRUCT(BlueprintType)
struct FDropItemInfo
{
	FDropItemInfo();

	GENERATED_USTRUCT_BODY();

	UPROPERTY(Transient)
	uint32 RewardGoldWeight;

	UPROPERTY(Transient)
	bool bCanDropItem;

	/** Gold spawn delay from death of owner */
	UPROPERTY(EditAnywhere)
	float GoldSpawnDelay;

	/** Reward item spawn delay from death of owner */
	UPROPERTY(EditAnywhere)
	float RewardSpawnDelay;

	/** for Testing */
	UPROPERTY(EditAnywhere)
	int32 TestNumSpawnGold;
	/** for Testing */
	UPROPERTY(EditAnywhere)
	int32 TestNumSpawnItemRefID;
};

/** Essential selected information for NPCClassInfo setup, related to combat. */
USTRUCT(BlueprintType)
struct FB2CharMovementEssentialSetupInfo
{
	GENERATED_USTRUCT_BODY()

	FB2CharMovementEssentialSetupInfo()
	{
		MaxWalkSpeedOverride = 0.0f;
		MaxAccelerationOverride = 0.0f;
		RotationRate = FRotator(-1.0f, -1.0f, -1.0f);
	}

	/** 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxWalkSpeedOverride;

	/** 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAccelerationOverride;

	/** negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RotationRate;

	// Any any properties of CharacterMovementComponent as necessary,
	// then apply their overriding at UB2NPCClassInfo::SpawnNPCClassInfoCharacte
};

/** Essential selected information for NPCClassInfo (and.. PC?) setup, regarding combat stats. */
USTRUCT(BlueprintType)
struct FB2CharCombatAbilityEssentialSetupInfo
{
	GENERATED_USTRUCT_BODY()

	FB2CharCombatAbilityEssentialSetupInfo()
	{
		HPScale = 1.0f;
		AttackDefenseLevelScale = 1.0f;
		MaxArmorOverride = 0.0f;
	}

	/** Multiplication to the basic heath value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HPScale;

	/** Multiplication to the basic Attack/DefenseLevel value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDefenseLevelScale;

	/** Armor value is just overridden. 0 or negative value for using default value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxArmorOverride;

	// Apply new members overriding at UB2NPCClassInfo::SpawnNPCClassInfoCharacter and ABladeIIPlayer::OverrideByPCClassInfo
};

/** Information about static mesh asset, to be a component attached to the character's skeletal mesh component */
USTRUCT(BlueprintType)
struct FCHSKCompAttachStaticMeshInfo
{
	GENERATED_USTRUCT_BODY()

	FCHSKCompAttachStaticMeshInfo()
	{
		AttachSocketName = NAME_None;
		AttachScale = 1.0f;
		StaticMeshAsset = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttachScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* StaticMeshAsset;

	// RelativeTransform might be necessary some day.. but we can either just adjust the socket.
};

/** Information about skeletal mesh asset, to be a component attached to the character's skeletal mesh component */
USTRUCT(BlueprintType)
struct FCHSKCompAttachSkeletalMeshInfo
{
	GENERATED_USTRUCT_BODY()

	FCHSKCompAttachSkeletalMeshInfo()
	{
		AttachSocketName = NAME_None;
		AttachScale = 1.0f;
		SkeletalMeshAsset = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttachScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMesh* SkeletalMeshAsset;
};

/** Information about particle system asset, to be a component attached to the character's skeletal mesh component */
USTRUCT(BlueprintType)
struct FCHSKCompAttachParticleSystemInfo
{
	GENERATED_USTRUCT_BODY()

	FCHSKCompAttachParticleSystemInfo()
	{
		AttachSocketName = NAME_None;
		AttachScale3D = FVector(1.f);
		LocationOffset = FVector::ZeroVector;
		RotationOffset = FRotator::ZeroRotator;
		bIgnoreBoneRotation = false;
		ParticleSystemAsset = NULL;
	}

	virtual ~FCHSKCompAttachParticleSystemInfo() {} // This struct has virtual function so need virtual destructor.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AttachScale3D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttachParticle")
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttachParticle")
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttachParticle")
	uint32 bIgnoreBoneRotation : 1;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UParticleSystem> ParticleSystemAsset;
public:
	virtual class UParticleSystem* GetParticleSystemAsset(); // Load (if not loaded yet) and return ParticleSystemAsset

	// It is declared mainly for its subclass use, to be overridden for compositing PC part and its relevant effects.
	virtual FString GetCompCreateName(AActor* InOwner) const
	{
		return FString::Printf(TEXT("CompFromCHSKCompAttachPSInfo_%s_%s"), (InOwner ? *InOwner->GetName() : TEXT("_")), *AttachSocketName.ToString());
	}

	/** A helper function to create particle system component owned by InCompOwner and attach to InParentComp using setup here. */
	class UParticleSystemComponent* CreatePSCHelper(AActor* InCompOwner, USkeletalMeshComponent* InParentComp);

#if WITH_EDITOR
	// It is just for preventing non desired setup
	void EditorClearPSAsset();
	friend class UB2WingInfo; // friend declaration only for editor.
#endif
};

/** All necessary cached information for a BladeIICharacter Mob spawned from MonsterSpawnPool. A sort of stage context data. */
USTRUCT()
struct FMobSpawnedBirthplaceInfo
{
	GENERATED_USTRUCT_BODY()

	FMobSpawnedBirthplaceInfo()
	{
		//Birthplace = NULL;
		WaveNumber = -1;
		WaveObjIndex = -1;
		bSpawnedAsBossMob = false;
		bIsSummoned = false;
		bOwnerDeathReported = false;
	}

	//FMobSpawnedBirthplaceInfo(class AB2MonsterSpawnPool* InBirthplace,
	//	int32 InWaveNum, int32 InWaveObjIndex, bool bInBossMob, bool bInSummoned)
	//{
	//	Birthplace = InBirthplace;
	//	WaveNumber = InWaveNum;
	//	WaveObjIndex = InWaveObjIndex;
	//	bSpawnedAsBossMob = bInBossMob;
	//	bIsSummoned = bInSummoned;
	//	bOwnerDeathReported = false; // Intended to be initially false in any case.
	//}

	/** The spawn pool that its owning character born, if any. Must be null for characters directly placed on the map. */
	//UPROPERTY()
	//class AB2MonsterSpawnPool* Birthplace;

	/** The wave number that its owning character belongs within wave settings of BirthPlace. */
	int32 WaveNumber;

	/** Index number within a wave setting of Birthplace.
	 * It is expected to have the same number that a component of SpawnPoolEditingActor shows above its head.
	 * Unique only among mobs spawned from the same wave setting of a MonsterSpawnPool.
	 * Not a valid value if its owning character is not from spawn pool (e.g. placed on the map or player character)  */
	int32 WaveObjIndex;

	/** BossHUD will be invoked according to this.
	* This might be more apparent mark that this character is a boss mob for current context, than ECharacterType::ECT_BossMob */
	uint32 bSpawnedAsBossMob : 1;

	/** True means summoned by other character (most likely mob).
	* It is set by SpawnPool, and true means its owning character is spawned by SummonSettings of SpawnPool, but WaveObjIndex is not being used. */
	uint32 bIsSummoned : 1;

	/** Whether the owner mob's death reported to Birthplace, then the owner mob will soon be destroyed. */
	uint32 bOwnerDeathReported : 1;
};

USTRUCT()
struct FDamageReservationInfo
{
	GENERATED_USTRUCT_BODY()

	FDamageReservationInfo()
	{
		ApplyWorldTime = -1.0f;
		DamageFx = nullptr;
		Damage = 0.0f;
		DamageInfo = nullptr;
		EventInstigator = nullptr;
		DamageCauser = nullptr;
	}

	float ApplyWorldTime;
	UParticleSystem* DamageFx;
	float Damage;
	struct FDamageInfo* DamageInfo;
	class AController* EventInstigator;
	class AActor* DamageCauser;
};

UCLASS(Blueprintable)
class ABladeIICharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Being set if spawned by NPCClassInfo. Not valid for BladeIIPlayer. */
	UPROPERTY(Transient, VisibleAnywhere)
	ENPCClass NPCClassEnum;
	UPROPERTY(Transient, VisibleAnywhere)
	ENPCClassVariation NPCClassVariation;

	/** This was originally a public BlueprintReadWrite property but later changed as private for its importance in consistency and debugging stuff, etc..
	 * There could be still old blueprint node that accesses it directly, but cannot add new node accessing it directly anymore. */
	UPROPERTY(Transient, VisibleAnywhere, Category = BladeIIGame)
	EAttackState AttackState;

	/** When it's other than ESES_None, it will override other normal animation state by AttackState. */
	UPROPERTY(Transient, VisibleAnywhere)
	EStageEventShowState StageEventShowState;

	/** Signal to AnimBP that the character decided to do attacking either by player input or by AI operation.
	 * This was once bIsAttacking */
	UPROPERTY(Transient, VisibleAnywhere, Category = BladeIIGame)
	uint32 bSignalAttack : 1;

	/** Being set to true for any character that is one of "directed" PlayActor of StageEventDirector. (not maintaining in-game state)
	 * Dead handling need to be delayed when it is set to prevent GCd while playing event scene. */
	UPROPERTY(Transient, VisibleAnywhere)
	uint32 bPlayingDirectedStageEvent : 1;

	bool bForceCancelAttackTriggered;

	// Data for dead sinking
	void StartDeadSink();
	bool bIsDeadSinking;
	float DeadSinkZOffsetPerSecond;
	float DeadSinkLeftTime;

	bool IsAboardMob(ENPCClass eNpcClass);

protected:
	int32 GetAIAttackIndex(const TBitArray<>& InBitIndexArray, const float InTotalWeight, const int32 InPreIDX, const TArray<FAIAttackData>& InDataArray);

	virtual bool CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType);

	/** Take damage, handle death */
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual uint32 GetNetId() const
	{
		return 0;
	}
protected:
	virtual void TakeDamageAbnormalState(const FDamageInfo& DamageInfo, AActor* DamageCauser);

	/** Notified when my attack hit and gave a damage to other character. */
	virtual void NotifyTargetDamaged(class ABladeIICharacter* DamagedTarget, float DamageAmount);

	virtual bool IsDamageAnimationProcessable(const FDamageInfo& DamageInfo);

	virtual void ProcessTakeDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser);

	virtual EAttackType GetSuitableDamageType(const FDamageInfo& DamageInfo, bool IsArmorBreak);
	virtual void PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak);
	virtual void PostDamageAnimation(const FDamageInfo& DamageInfo, AActor* DamageCauser);

	virtual void ProcessTakeDamageEffect(const FDamageInfo& DamageInfo, AActor* DamageCauser);

	virtual ABladeIICharacter* GetQTEActor() const
	{
		return nullptr;
	}
	virtual ABladeIICharacter* GetLocalCharacter() const;

	virtual float GetDamageByDamageLogic(const float OriginalDamage, EDamageLogicType DamageLogic, ABladeIICharacter* Attacker, AActor* DamageCauser, const FDamageInfo& DamageInfo, bool& OutIsCritical);
	virtual float GetNormalDamage(const float OriginalDamage, ABladeIICharacter* Attacker, AActor* DamageCauser, const FDamageInfo& DamageInfo, bool& OutIsCritical);

	virtual float GetScaledAttackLevel();
	virtual float GetScaledDefenceLevel();

	// Damage Interface
	virtual float GetBaseDamageIncreaseRate(ABladeIICharacter* Attacker, const FDamageInfo& DamageInfo, AActor* DamageCauser);
	virtual float GetDamageRate(ABladeIICharacter* Attacker, const FDamageInfo& DamageInfo, AActor* DamageCauser);
	virtual float GetAdditionalDamage(ABladeIICharacter* Attacker);

	virtual void UpdateArmorByDamageInfo(const FDamageInfo& DamageInfo);

public:

	virtual bool IsPlayerCharacter() const
	{
		return false;
	}
	virtual bool IsPlayerControlled() const
	{
		return false;
	}
	virtual bool IsMob() const
	{
		return !IsPlayerCharacter();
	}
	virtual bool IsLocalCharacter() const;
	virtual bool IsQTEActor(const ABladeIICharacter* OwnerCharacter) const;

	virtual ABladeIICharacter* GetAttacker(AActor* DamageCauser);
	virtual void AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo);
	virtual void AffectToAttackerAfterVictimDie(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo);
	virtual bool RequestDamage(const float Damage, const FDamageInfo* DamageInfo, ABladeIICharacter* DamageCauser, bool NetBroadcast = false);

	FORCEINLINE class UB2CharacterBuffManager* GetBuffManager() const
	{
		return BuffManager;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsBuffActive(EBuffType BuffType) const;
	class UB2Buff_Base* GetBuff(EBuffType  BuffType) const;

	virtual bool ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage);

	virtual void AddDamageReservation(const FDamageReservationInfo& ReservDmgInfo);

	bool IsInTinyDamage() const;
	bool IsInHugeDamage() const;

	virtual void ApplyDamageAnimation(EAttackType DamagedType);
	void ApplyDefaultDamageAnimation();

	/** check if pawn is still alive. It also checks if this is valid actor object. */
	bool IsAlive() const;

	/** switch to ragdoll */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void SetRagdollPhysics(float LifeSpan = 10.f, bool bNoRagdoll = false);

	/** get current health */
	float GetHealth() const;

	/** get max health */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetMaxHealth() const;

	/** get current armor */
	float GetArmor() const;

	/** get max armor */
	float GetMaxArmor() const;

	virtual float GetShield() const
	{
		return 0.0f;
	} // Possibly by buff effect.
	virtual void SetShield(const float NewShieldAmount) {}
	virtual void MakeSlow(float _Duration);
	virtual void SetMinDrawDistance(float _distance);

	/** Heal.. by percent (%), not 0.01 scaled value. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void HealingHPByPercent(float Percentage, bool bUseEffect = false, bool bUseTextEffect = false);
	/** The same healing function by absolute amount instead of percent scale. */
	virtual void HealingHPByAmount(float InHealAmount, bool bUseEffect = false, bool bUseTextEffect = false);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void RecoverArmor();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void AllowMovement();

	/** Make movement impossible (sets movement mode to MOVE_None). */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void PreventMovement();

	/** The effect is the same as Allow/PreventMovement, but is provided for stage event directed scene.
	 * If movement/attack is prevented by this, AllowMovement by AnimNotify won't work */
	void AllowMovementByStageEvent();
	void PreventMovementByStageEvent();

	FORCEINLINE void ForceCancelAttackMotion()
	{
		bForceCancelAttackTriggered = true;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool ConsumeForceCancelAttackMotion();

	/**
	 * It is suggested to use this instead of GetMesh(), when you want to access the main SkeletalMeshComponent which is the base parent of other attached mesh components.
	 * due to possible multi-component setup of BladeIIPlayer (or BladeIICharacter too)
	 * For multi-component setup of BladeIIPlayer, the component get by GetBaseMesh must be the one having all necessary bones and sockets.
	 */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	class USkeletalMeshComponent* GetBaseMesh() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void UpdateDamageInfo(const FDamageInfo& InDamageInfo);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void ShootProjectile(float Damage, FName BoneName, const FDamageInfo& InDamageInfo, bool bOverrideDamageInfo = false, AActor* TargetActor = NULL, bool bUseSocketName = false, int32 ProjIndex = 0, FRotator OffsetRotation = FRotator::ZeroRotator, float ShootSpeed = 0.f, float MaxLifeTime = 0.f, bool bPenetrate = false, bool bIgnoreBoneRotation = false, float FreezeDuration = 0.f);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void CastAreaDamage_DamageZoneSkillIndex(float Damage, const FDamageInfo& InDamageInfo, bool bOverrideDamageInfo, const FVector& TargetLocation, int32 ClassIndex, float Radius, float CastingTime, float Interval, float LifeTime);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void CastAreaDamage_SpecifyFx(float Damage, const FDamageInfo& InDamageInfo, bool bOverrideDamageInfo, const FVector& TargetLocation, const FLinearColor& WarningFxColor, float Radius, float CastingTime, float Interval, float LifeTime, float FirstImpactTime, class UParticleSystem* WarningFX, class UParticleSystem* ActivatingFX, FVector WarningFxScale = FVector(1.f, 1.f, 1.f));

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual bool GetAreaDamageTargetLocation(float RandMaxDist, FVector& TargetLocation, const FVector& LocationOffset, bool bSelfTarget);

	void InnerGetAreaDamageTargetLocation(ABladeIICharacter* TargetCharacter, float RandMaxDist, FVector& OutTargetLocation, const FVector& LocationOffset);

	UPROPERTY(Transient)
	FDamageInfo CurrentDamageInfo;


	FORCEINLINE bool GetIsUndead()
	{
		return m_bIsUndead;
	};
	FORCEINLINE void SetIsUndead(bool _bIsUndead)
	{
		m_bIsUndead = _bIsUndead;
	};

	virtual void SetSummonOwner(ABladeIICharacter* OwnerCharacter)
	{
		SummonOwner = OwnerCharacter;
	}
	virtual ABladeIICharacter* GetSummonOwner() const
	{
		return SummonOwner;
	}

	virtual void AddMinion(ABladeIICharacter* SingleMinion);
	virtual void RemoveMinion(ABladeIICharacter* SingleMinion);
	virtual const TArray<ABladeIICharacter*>& GetMinions()
	{
		return Minions;
	}

private:
	TArray<uint32> EventCancelCastAreaDamageSubscribeKeys;

protected:
	/** Mainly intended to adjust projectile shooting pitch */
	virtual FVector GetDirectionToTargetCharacter();
	virtual FVector GetDirectionToTargetCharacter(FVector& BoneLocation);
	virtual bool IsTargetableCharacter(ABladeIICharacter* TargetCharacter);

	virtual void CheckQTEEnable(ETeamType HitTeam = ETeamType::ETT_End);

	void LocationAdjustCharacterOverlap(float DeltaSeconds);

public:

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetTargetDistanceInTarget(class AActor* InTargetActor) const;

	virtual bool CalcAIAttackIndex(float TargetDistance, int32& OutAttackIndex, float& OutRadius);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void UpdateAIAttackIndex(float TargetDistance, bool bOnlyUpdateMoveDist = false);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetAIAttackDelay();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	float GetAIMaxAttackRange();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetAttackState(EAttackState InAttackState);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetForceAttackState(EAttackState InAttackState);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	EAttackState GetAttackState() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void OnAttackState();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void OnEndAttackState();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetSignalAttack(bool bInSignal);
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool GetSignalAttack() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual ENPCClass GetNPCClass() const; // Only for BladeIICharacter, not for BladeIIPlayer.

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual ENPCClassVariation GetNPCClassVariation() const; // Only for BladeIICharacter, not for BladeIIPlayer.

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual EPCClass GetCurrentPlayerClass() const {
		return EPCClass::EPC_End;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	FVector GetFleeLocation(int32 TestNumLocation, float Radius);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	class AActor* GetTeleportDestination(FName ActorTagName);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	FVector GetTopLocation();

	/** Valid StageEventShowState setting requires InDirectorActor not to be NULL,
	 * Otherwise, null out DirectorActor and setting to ESES_None state can be accepted. */
	void SetStageEventShowState(EStageEventShowState InNewState, class AB2StageEventDirector* InDirectorActor);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	EStageEventShowState GetStageEventShowState();

	FORCEINLINE void SetPlayingDirectedStageEvent(bool bInPlaying) {
		bPlayingDirectedStageEvent = bInPlaying;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsPlayingDirectedStageEvent() const;

	/** To be called by SESEndNotify which will be installed at playing animation montage. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void NotifyStageEventShowEnd();

	virtual void ApplyQTEEnable(ETeamType HitTeam = ETeamType::ETT_End);

	FVector GetTargetLocationVectorFromPlayer(AActor* pTargetActor, bool bIncludeCapsule = true);

	UPROPERTY(BlueprintReadOnly, Category = BladeIIGame)
	FText LocalizedUserNickName;

	/** Expected for boss mobs.. */
	UPROPERTY(BlueprintReadOnly, Category = BladeIIGame)
	FText DecoratingNickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	ECharacterType CharacterType; // This is very initial type, intended both for PC and NPC. See NPCClassEnum (ABladeIICharacter::GetNPCClass) and ABladeIIPlayer::GetCurrentPlayerClass

	// Base property to derive other data 
	// -> Mobs will get this from StageInfo. Player character will get this from B2PlayerExpManager (essentially from server)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	int32 CharacterLevel;

	// % of Damage Multiplier for this character, PC should be 100%
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	int32 CharacterDamageRate;

	// Attack level value, Initialized on start play, should be const during gameplay
	UPROPERTY(Transient, BlueprintReadOnly, Category = Damage)
	float TotalAttack;

	// Defense level value, Initialized on start play, should be const during gameplay
	UPROPERTY(Transient, BlueprintReadOnly, Category = Damage)
	float TotalDefense;

	/** Not only movement, but also attacking is get disabled when it is false. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bAbleToMove : 1;

protected:
	/** If true, bAbleToMove won't work. */
	uint32 bMovementPreventByStageEvent : 1;

private:
	/** Being set if this mob was in attacking anim state without official dead handling at DeadEnd (anim notify)
	 * It possibly means this mob has done suicide attack (in that case, it is expected)
	 * but there could be some other ugly cases which is not really expected.
	 * For ensuring game flow handling (e.g. SpawnPool wave) Suicide will be called along with setting this flag.
	 * This flag itself is set to picked up by AnimBP to prevent any more dead anim handling. */
	UPROPERTY(Transient, VisibleAnywhere, Category = BladeIIGame)
	uint32 bCommittedSuicideAtDeadEnd : 1;

	/** Flag for QTE timing. */
	UPROPERTY(Transient, VisibleAnywhere, Category = BladeIIGame)
	uint32 bQTEEnabled : 1;

	UPROPERTY(Transient, VisibleAnywhere, Category = BladeIIGame)
	uint32 bInQTEState : 1;
public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bQTERun : 1;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	int32 QTEStateNum; // Non-zero for mounted QTE

	ETeamType QTEEnableTeam;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bNeedUpdateTarget : 1;

	/** Intended to be true while in normal combo attacking. A notify state attached at combo attack animation can set this.
	 * Next combo is reserved If player make attack input while this is true. */
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bComboEnabled : 1;


	/** Need to set it true if a character wants to supports initial inactive pre-spawn state. In that case, the character gets active after it is turned off by AI.
	 * AnimBP also need to support this feature (check this variable and change its anim state) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bNeedSpawnCheck : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bUsePhaseChangeAnimation : 1;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bInPhaseAnimation : 1;

	// Immune for Abnormal State
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bImmune : 1;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bHadRecoverArmor : 1;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bHadChangedPhase : 1;

	UPROPERTY(Transient, BlueprintReadOnly, Category = BladeIIGame)
	uint32 bValueForTextEffect : 1;

	UPROPERTY(Transient, BlueprintReadOnly, Category = BladeIIGame)
	bool bForceReceivdAbsoluteDamage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = BladeIIGame)
	bool bForceReceivdAttackCountDamage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = BladeIIGame)
	int32 FullAttactCountSize;

	// Increase/Decrease Receive Damage for certain situation
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	float DamageMultiplier;

	// Increase/Decrease Attack Damage for certain situation
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	float BaseDamageRate;

	// Reflect Damage Amount
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	float ReflectDamageAmount;

	// HealHPByAttack Amount in percent
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	float HealHPByAttackRate;

	// State Attack type
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	EStateAttackType StateAttackType;

	// State Attack duration
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	float StateAttackDuration;

	// State Attack duration
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	float StateAttackAmount;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	EAttackType LastDamageType;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	uint32 bForceSpawAnim : 1;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	EStateAttackType LastStateDamageType;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	bool bForceComboEnd;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	EGuardBreakLogicType LastGuardBreakType;

	// For QTE animation
	UPROPERTY(Transient, BlueprintReadOnly, Category = BladeIIGame)
	EPCClass QTEPCClassEnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	bool bAllowRecoilAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	bool bBloodStoneAura;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	EBloodStoneType BloodStoneType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	float BloodStoneAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseBloodStone")
	EDamageLogicType  BloodStoneBuffLogicType;

	// Reserve data for Initial influence
	EInfluenceType InitialInfluence;
	float InitialInfluenceAmount;
	float InitialInfluenceDuration;
	float InitialInfluenceStateDuration;
	float InitialInfluenceStateTriggerRate;

	ECharacterMaterialOverrideType CurrentOverridenMatType;
	TArray<ECharacterMaterialOverrideType> OverridenMatTypeList;

	/** LastStateDamageType will affect the character according to the relevant flags of DamageStateConfigData */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	FDamageStateConfigData AllowedDamageStateData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame, meta = (ClampMin = "0.0"))
	float DetectDistance;

	/** Player Camera Transform when this character is CameraTargetActor of BladeIIPlayerController,
	 * and the camera is targeting this character by camera component transform. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraTarget)
	FTransform CameraTargetTransform;

	/** Additional rotation when this character is CameraTargetActor of BladeIIPlayerController
	 * and the camera is targeting this character by camera boom rotation, instead of CameraTargetTransform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraTarget)
	FRotator CameraTargetBoomRotOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	TArray<TSubclassOf<class ABladeIIProjectile>> ProjectileClasses;

protected:

	/** Contains selected components from its own components,
	 * which is considered to be better not shown at event scene. */
	UPROPERTY(Transient)
	TArray<class UActorComponent*> EventPuppetSetupBlacklist;

	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	int32 DamagedNum;

	// No damage but damage effect
	bool bInvincible;
	bool m_bIsUndead;

	virtual void SetDamagedNum(int32 NewDamagedNum);
	FORCEINLINE void IncreaseDamageNum()
	{
		SetDamagedNum(DamagedNum + 1);
	}

public:
	class UB2DamageEffectInfo* GetDamageEffectInfo();

	virtual class UB2Buff_Base* AddBuff(EBuffType BuffType, float fDuration, float Amount = 0.0f, AActor* BuffCauser = nullptr, bool bUseEffect = true, bool bUseTextEffect = true, TArray<float>* PtrAdditionalParams = nullptr);
	void AddBuffEnemy(EBuffType BuffType, float fDistance, float fDuration, float Amount, bool bUseEffect = true, bool bUseTextEffect = true);
	void ClearBuff(EBuffType BuffType);
	void ClearAllBuffs();
	void ClearDebuffs();

	virtual void UpdateDamageNum(const FDamageInfo& DamageInfo);
	virtual void ResetDamageNum();

	virtual void OnDamageEnd();

	FORCEINLINE int32 GetDamagedNum() const
	{
		return DamagedNum;
	}
	FORCEINLINE void SetForceComboEnd()
	{
		bForceComboEnd = true;
	}
	FORCEINLINE void ConsumeComboEnd()
	{
		bForceComboEnd = false;
	}
	FORCEINLINE bool IsForceComboEnd()
	{
		return bForceComboEnd;
	}

	virtual void ClearAttachedParticles();

	bool IsAllowState(EStateAttackType StateAttack);

	bool IsAllowBuff(EBuffType InBuffType);

	// FanAngle Range 0 ~ 180
	void GetEnemiesInRadius(const float Radius, TArray<ABladeIICharacter*>& OutEnemies, float FanAngle = 180.0f);

	class UB2BuffModeEffectInfo* GetBuffModeEffectInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	TArray<TSubclassOf<class AB2AreaDamageActorBase>> AreaDamageClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	FName DamageSocketName;

	/** The default pawn class used by players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TArray<FAIAttackData> AIAttackDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TArray<int32> InvalidAttackIndices;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	int32 AIAttackIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	float AIAttackMoveAcceptableRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	uint32 bUseAIIndexLog : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TArray<FAIPhaseData> PhaseDataArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UParticleSystemComponent* PhasePSComp;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CenterShadow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light)
	class UMaterialInterface* ShadowMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light, meta = (UIMin = 10, ClampMin = 10))
	float ShadowSizeCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light, meta = (UIMin = 1, ClampMin = 1))
	float ShadowMaxDistance;

	/** Additional offset applied when spawning damage num effect. To come from PC/NPCClassInfo as well. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	float DamageNumZOffset;

	// Use this when you actually need to apply DamageNumZOffset
	FORCEINLINE float GetScaledDamageNumZOffset() const
	{
		return DamageNumZOffset * GetMesh()->GetComponentTransform().GetScale3D().Z;
	}

	/** Setup decal component to be attached to the character as simplified shadow, stuff like scales and attach socket names, not like everything. */
	virtual void SetupDecalCompForCenterShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent);
	virtual void SetupDecalCompForLeftShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent) {}; // Probably for BladeIIPlayer
	virtual void SetupDecalCompForRightShadowCommon(class UDecalComponent* InDecalCompToUse, class USceneComponent* AttachParent) {}; // Probably for BladeIIPlayer

	virtual void ClearEffectInAbnormalState();

	bool GetAbleToControl();
	virtual bool GetAbleToMove() /*override*/;
	virtual float GetMoveAcceptableRadius() /*override*/;
	virtual void PauseForDuration(float Duration) /*override*/;
	virtual bool ShouldIgnoreTimeDilation() const /*override*/;
	virtual const void* GetCustomDamageInfo() /*override*/;

	virtual void FlashForDuration(FName FlashColorName, FLinearColor FlashColor, FName FlashIntensityName, float FlashIntensity, float Duration);

	/** Updates Pawn's rotation to the given rotation, assumed to be the Controller's ControlRotation. Respects the bUseControllerRotation* settings. */
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;

	// Character stats
	virtual float GetDamageIncreaseRate()			const;
	virtual float GetBossDamageIncreaseRate()		const
	{
		return 0.f;
	}
	virtual float GetCriticalRate()					const;
	virtual float GetCriticalDamageRate()			const;
	virtual float GetDamageDecreaseRate()			const;
	virtual float GetMeleeDamageDecreaseRate()		const
	{
		return 1.f;
	}
	virtual float GetRangeDamageDecreaseRate()		const
	{
		return 1.f;
	}
	virtual float GetBossDamageDecreaseRate()		const
	{
		return 1.f;
	}
	virtual float GetResistCriticalRate()			const
	{
		return 1.f;
	}
	virtual float GetHealHPAmountRateOnKill()		const
	{
		return 0.f;
	}
	virtual float GetAbsorbHPAmountRateOnAttack()	const
	{
		return 0.f;
	}
	virtual float GetDurationDecreaseRate()			const
	{
		return 1.f;
	}
	virtual float GetExtraTakeDamageRate()			const;

	virtual float GetIncAdditionalDamage()			const
	{
		return 0.f;
	}
	virtual float GetDecAdditionalDamage()			const
	{
		return 0.f;
	}

	// Buff
	virtual float GetBonusBuffDuration() const
	{
		return 0.f;
	}

	virtual float GetBonusBuffDamageDecreaseAmount() const
	{
		return 0.f;
	}
	virtual float GetBuffReflectionDamageRate() const
	{
		return 1.f;
	}
	virtual float GetBonusBuffHealHPByHit() const
	{
		return 0.f;
	}
	virtual float GetBonusBuffHealHP() const
	{
		return 0.f;
	}
	virtual float GetBonusBuffIncMoveSpeed() const
	{
		return 0.f;
	}

	virtual float GetBonusBuffReceiveDamageDecreaseRate() const
	{
		return 1.f;
	}
	virtual float GetBonusBuffAttackDamageIncreaseRate() const
	{
		return 0.f;
	}

	virtual float GetAttackDamageIncreaseRateByClass(EPCClass TargetClass) const
	{
		return 0.f;
	}
	virtual float GetReceiveDamageDecreaseRateByClass(EPCClass AttackerClass) const
	{
		return 1.f;
	}


	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual float GetDamageRadiusIncreaseRate() const
	{
		return 1.f;
	}

	virtual int32 GetBonusBuffAffectNum() const
	{
		return 0;
	}

	// Initialized on start play, should be const during gameplay
	float AttackLevel;
	float DefenseLevel;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void SetInvincible(bool bNewInvincible);

	virtual float GetStaticOptionAppliedWalkSpeed() const
	{
		return CachedBaseMaxWalkSpeed;
	};

protected:
	// Scale to the final computed AttackLevel and DefenseLevel. It will get from NPCClassInfo, not probably for PCs
	float FinalADScale;
	float FinalHPScale;

	// Regenerate HP can be either by percent or absolute amount.
	float RegenerateHPPercentagePerSecond;
	float RegenerateHPAmountPerSecond;
	uint32 bRegenerateHPByPercent : 1; // True will use RegenerateHPPercentagePerSecond, or RegenerateHPAmountPerSecond

	// Cached from PCClassInfo definition. It is the base walk speed when player has no other equipped options.
	float CachedBaseMaxWalkSpeed;

protected:

	struct FStateDamageState // Almost for UI display. LastStateDamageType cannot take much information.
	{
		FStateDamageState(){
			bIsOn = false;
			LastStartedTime = 0.0f;
			RemainingTime = 0.0f;
		}
		uint32 bIsOn : 1;
		float LastStartedTime;
		float RemainingTime;
	};
	TArray<FStateDamageState> CachedStateDamageStates;

	FStateDamageState CachedInvincibleBuffState; // Not really one of StateDamage, just use the same struct for convenience..

public:
	FORCEINLINE void SetFinalADScale(float InScale)
	{
		FinalADScale = InScale;
	} // Provide only Setter..
	FORCEINLINE void SetFinalHPScale(float InScale)
	{
		FinalHPScale = InScale;
	}

	/** You may set it to regenerate by percent value or absolute amount. Set 0 or less turns off regenerating timer. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void SetRegenerateHPRate(float PercentageOrAmount, bool bInRegenByPercent = true);
	static float GetRegenerateHPPeriod()
	{
		return 1.0f;
	} // Fixed value for now.

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void SetCanBeDamagedForB2(bool bNewCanBeDamaged);

protected:
	/** Kill this pawn */
	virtual void KilledBy(class APawn* EventInstigator);

	virtual void DropItem();

	virtual void TriggerKillMonster();

	/** notification when killed, for both the server and client. */
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser);

	void ReportMyDeathToBirthplace(); // Part of OnDeath, for mob spawned from SpawnPool. Sometimes need this to be called manually.

	bool HasRootMotion() const;
public:
	void ConditionalReportMyDeathToBirthplace();
	void OnMountStartComplete();

	float GetScaledCapsuleRadius();
	float GetScaledCapsuleHalfHeight();

	void SetMoveCollision(bool bBlockPawn);

protected:
	/** play effects on hit */
	void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	/** Apply momentum caused by damage. */
	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
public:
	/** Called from Mob AnimBP, on DeadEnd notify */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void OnAnimMobDeadEnd(bool bInUseDeadRagDoll, float InRemainLifeSpan, bool bInUseDeadSink = false, float InDeadSinkDelayTime = 0.f, float InDeadSinkZOffset = 0.f, float InDeadSinkDuration = 0.f);
	/** Called from Mob AnimBP, on DeadEffect notify. It returns created DeadBodyMID reference for AnimBP handling later. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	class UMaterialInstanceDynamic* OnAnimMobDeadEffectSpawn(class UMaterialInterface* InDeadBodyMI, FName InDeadBodyMIParamName,
		class UParticleSystem* InDeadParticleTemplate, FName InDeadParticleAttachBoneName, FVector InScale = FVector(1.f, 1.f, 1.f));

	virtual void LifeSpanExpired() override; // Dying mob's last crying

	virtual int32 GetTeamNum() const
	{
		return TeamNum;
	}
	virtual void SetTeamNum(int32 InTeamNum)
	{
		TeamNum = InTeamNum;
	}

	/** Most likely the last dead handling. This character will be invalid after InLifeSpan.
	 * It calls SetLifeSpan, take QTEState (and possibly other stuff..) into account. */
	void WrappedSetLifeSpan(float InLifespan);

	void SetPvPTutorialHandicap(const ABladeIICharacter* InPlayer, float InStatsPercent);
public:
	void SetCustomTimeDilation(float NewDilation);

protected:
	void ResetFlash();

	friend class UAnimNotifyState_B2CustomTimeDilation;

	// Cached for Slowed Time Dilation
	float CachedTimeDilation;
	FDamageInfo CachedDamageInfo;
	FDamageInfo ReflectionDamageInfo;

	virtual const TArray<FAIAttackData>& GetAIAttackArray();
	virtual const TArray<int32>& GetInvalidAttackIndices();

	/** Name of member properties that will be excluded when its instance is overridden by BaseBP class object of ClassInfo. */
	static TArray<FName> BaseBPCopyExcludeProperties;

public:

	ABladeIICharacter(const FObjectInitializer& ObjectInitializer);

	/** Identifies if pawn is in its dying state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	uint32 bIsDying : 1;

	// Current health of the Pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float MaxHealth;

	// Current health of the Pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Armor)
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Armor)
	float MaxArmor;

	/** Setting this enables float HP bar widget, only for common mobs.
	 * Not for player and boss mob who will have its own UI widget for various stuff including HP bar.
	 * Other Floating Widget classes are now AB2UIManager_InGameCombat */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	//TSubclassOf<class UB2FloatingHPBar> FloatingHPBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FName AttachHPBarSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FString SpawnNotifyMsgString;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	float SpawnNotifyMsgTime;

	// Damage Anim Index for EAT_Default case
	UPROPERTY(Transient, BlueprintReadWrite, Category = BladeIIGame)
	EAttackType RandDamageAnimType;

	UPROPERTY(Transient)
	ABladeIICharacter* SummonOwner;

	UPROPERTY(Transient)
	TArray<ABladeIICharacter*> Minions;


private:
	USkeletalMeshSocket const* HPBarSocket;
	bool	bHPBarAttachedToSocket;

public:
	FVector GetFloatingBarPosition();
	FORCEINLINE bool IsHPBarAttachedToSocket()
	{
		return bHPBarAttachedToSocket;
	}

protected:

	/** For post-dead leaving shits of monsters. This information will come from NPCClassInfo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeIIGame)
	FDropItemInfo DropItemInfo;
private:
	/** At which percent (0.0 ~ 100.0) of full HP this mob can enter QTE mode?
	 * Settings coming from NPCClassInfo.
	 * <0 means no QTE for this character. */
	float QTEInvokeHPPercent;

	int32 nCanBeDamagedFalseCount;
public:
	FORCEINLINE float GetQTEInvokeHPPercent() const
	{
		return QTEInvokeHPPercent;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void OnQTENext();

	virtual void ReserveQTEEnableEndCB();

	virtual void OnRecoverQTEBreakPoint() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QTE)
	EQTEType QTEType;

	float QTEMountTime; // Max allowed mount time when this character is mount target.
	float QTEEnableTime;
	bool QTELimitEnable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QTE)
	float CurrentQTEBreakPoint;

	FORCEINLINE bool IsControlledByQTEPlayer() const
	{
		return bInQTEState && QTEType == EQTEType::EQT_Mount;
	};

	FORCEINLINE bool IsTimingForQTEPossession() const
	{ // Can player take me for QTE target now? not for successive QTE kick.. and Break is not that QTE.
		return !bInQTEState && bQTEEnabled && (QTEType != EQTEType::EQT_Mount || QTEStateNum == 1) && (QTEType != EQTEType::EQT_Break);
	};

	FORCEINLINE bool IsInQTEBreakState() const
	{
		return (QTEType == EQTEType::EQT_Break) && bQTEEnabled;
	}
	FORCEINLINE bool IsQTEBreakable() const
	{
		return (QTEType == EQTEType::EQT_Break) && QTEInvokeHPPercent > 0.0f;
	}
	FORCEINLINE float GetMaxQTEBreakPoint() const
	{
		return QTEInvokeHPPercent * 0.01f * MaxHealth;
	}
	FORCEINLINE float GetCurrentQTEBreakPoint() const
	{
		return CurrentQTEBreakPoint;
	}

	FORCEINLINE void SetQTEStateNum(int32 nQTEStateNum)
	{
		QTEStateNum = nQTEStateNum;
	}
	FORCEINLINE int32 GetQTEStateNum() const
	{
		return QTEStateNum;
	}

	void SetQTELimitEnabled(bool bQTELimitEnable);
	bool GetQTELimitEnabled();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void SetQTEEnabled(bool bInEnable);
	/** Simply returns QTEEnabled flag. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool GetQTEEnabled() const;

	/** Total QTE state setup method. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetQTEState();
	/** Simply sets bInQTEState flag */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void SetQTEStateFlag(bool bInState);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsInQTEState() const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsControlledByLocalPlayer();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void RecoverQTEBreakPoint();

	/** Pawn suicide. It can be used to kill a character at any time. It will do all necessary game flow handling by dead */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void Suicide();

	/** Not for all case that called Suicide.. AnimBP need to pick it up and prevent any other dead anim state. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool DidCommitSuicideAtDeadEnd() const;

	/** Current phase number is also the phase data array index for next phase.
	 * Refer to the comment of BladeIIAIController's CurrentPhaseNum */
	int32 GetCurrentPhaseNum();

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void GoToNextPhase();

	void SetBuffMode(EUniqueBuffType BuffMode, bool bForced = false, float Amount = 0.f, float Duration = 0.f, bool bUseEffect = false, bool bUseTextEffect = false);

	void PreApplyInfluenceParam(EInfluenceType InfluenceType, float& ApplyAmount, float& ApplyDuration) const;
	void ApplyInfluence(EInfluenceType InfluenceType, float InAmount, float InDuration, float InStateDuration, float InStateTriggerRate, bool bUseEffect, bool bUseTextEffect);

	void IncreaseArmor(float Amount, bool bUseEffect = false, bool bUseTextEffect = false);

	/** Returns True if the pawn can die in the current state */
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;
	bool CanDieFromCounterDungeon();
	/**
	* Kills pawn.  Server/authority only.
	* @param KillingDamage - Damage amount of the killing blow
	* @param DamageEvent - Damage event of the killing blow
	* @param Killer - Who killed this pawn
	* @param DamageCauser - the Actor that directly caused the damage (i.e. the Projectile that exploded, the Weapon that fired, etc)
	* @returns true if allowed
	*/
	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser);

	/** Not like killing, rather OnDeath handling for QTE caught mob. */
	void DieForQTE();

	// Die when we fall out of the world.
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	virtual void BeginPlay() override;

	/** Initialize combat variables such as attack, defense, health, etc, based on character's level. */
	virtual void InitializeCombatStats();

	virtual void BeginDestroy() override;

	virtual void Destroyed() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

	/** Set the information comes from Birthplace - SpawnPool - as one of most essential stage context data, e.g. wave number and if it is a boss mob, etc. */
	void SetBirthPlace(const FMobSpawnedBirthplaceInfo& InBirthplaceInfo);

	/** Set up matinee group skeletal mesh actor for stage event director in controlled matinee mode.
	 * Basically setting up the skeletal mesh resource with the same one for this character,
	 * but additional code work is required according to the character customization and/or equipment stuff. */
	virtual void SetupControlledMatineePuppet(class ASkeletalMeshActor* InMatineePuppet, bool bSetupLightComponent, class USkeletalMesh* OptionalSKMeshToSet = nullptr);
	virtual void ClearControlledMatineePuppet(class ASkeletalMeshActor* InMatineePuppet);
	virtual void SetVisibilityMatineePuppetPointLight(class ASkeletalMeshActor* InMatineePuppet, bool bOn);
	bool IsCompInEventSceneBlacklist(UActorComponent* InCheckComp) const;

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void PullEnemyByTick(float fDeltaTime, float fForceStrength, float fRadius, FVector LocationOffset);

	virtual bool IsPullableActor(AActor* TargetActor);
private:
	/** For each type of actor components, see if the same one needs to be created and attached to the MatineePuppoet.
	 * These checks are required to prevent duplicated component attaching when a single SkeletalMeshActor is used for more than one matinee track.
	 * InCompToCheck is one of components attached to this character. */
	bool DoesMatineePuppetSetupForStaticMesh(class ASkeletalMeshActor* InMatineePuppet, class UStaticMeshComponent* InCompToCheck);
	bool DoesMatineePuppetSetupForSkeletalMesh(class ASkeletalMeshActor* InMatineePuppet, class USkeletalMeshComponent* InCompToCheck);
	bool DoesMatineePuppetSetupForParticleSystem(class ASkeletalMeshActor* InMatineePuppet, class UParticleSystemComponent* InCompToCheck);
	bool DoesMatineePuppetSetupForDecal(class ASkeletalMeshActor* InMatineePuppet, class UDecalComponent* InCompToCheck);
	bool DoesMatineePuppetSetupForLight(class ASkeletalMeshActor* InMatineePuppet, class UPointLightComponent* InCompToCheck);

	FName HitFlashIntensityName;

protected:
	/** Handle to reset hit flash timer */
	FTimerHandle SetHitFlashTimerHandle;

	/** Handle to regenerate HP */
	FTimerHandle SetRegenerateHPHandle;

	/** Handle for Set Dead sink */
	FTimerHandle SetDeadSinkHandle;

	FTimerHandle OnQTEEnableEndTH;
	virtual void OnQTEEnableEndCB(); // To stop OnDeathTimeDilation

	virtual void CreateUIManager();

	TArray<FDamageReservationInfo> DamageReservationInfos;
	UPROPERTY(Transient)
	class UB2CharacterBuffManager* BuffManager;

public:
	void HideFloatingHPBar(bool bInHide);
	void CancleQTEEnableEndCB();

	ABladeIICharacter* FindQteOwnerPlayer() const;

	void SetDieForce();

	void SetHideFromEventDirector(bool bHide);

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsHideFromEventDirector()
	{
		return bHideFromEventDirector;
	}

	void ResetEmissive();

	bool bEmissiveActiveByBuff;

protected:
	void DestroyUIManager();
	//// A timer callback for delayed DestroyUIManager call
	FTimerHandle DelayedDestroyUIManagerTH;
	void DelayedDestroyUIManagerCB();

	/** Sync some UI doc data which can be missing for initial value. */
	virtual void InitUpdateUIDoc();
	/** To update some of relevant UIDoc data on every tick basis. Be careful to put variables to be updated here. */
	virtual void CharacterBuffUpdateUIdoc() {} // For BladeIIPlayer

	void RegenerateHPByTimer();

	void RemoveCPURenderDataOfAllSkComps();

	virtual void SetHealth(float NewHealth, bool bReceivedFromHost = false);
	/**
	 * Some of stage context info are here, e.g. if this character should be regarded as a boss mob.
	 * Data here won't be valid if this character is not from SpawnPool (placed BP, etc, but not really expected in a formal way except for a Player Character.)
	 */
	FMobSpawnedBirthplaceInfo CachedBirthplaceInfo;

	/** Being set for mobs just being spawned (currently under spawning process) from SpawnPool.
	 * It is to protect monsters from death not even registered to SpawnPool's runtime state. */
	uint32 bSpawningNoDamageGuard : 1;

	/** It will be set to some (non-nulled) value while playing stage event animation */
	//UPROPERTY(Transient)
	//class AB2StageEventDirector* CachedStageEventDirector;

	FVector OscillAmplitude;
	FVector OscillFrequency;
	FVector OscillOffset;
	bool bNeedOscillation;
	void SetOscillation(const FVector& Amplitude, const FVector& Frequency);
	virtual void UpdateOscillation(float DeltaSeconds);

	// Damage data for freeze
	FVector DeferredImpulse;
	bool bDeferredMassIndependent;

	// Cached Material for change (original)
	UPROPERTY(Transient)
	TArray<class UMaterialInterface*> CachedMaterials;

	// Cached Material for change, only for abnormal cases, use one material for every elements
	UPROPERTY(Transient)
	TArray<class UMaterialInterface*> CachedStateMaterials;

	/** Back-up MeshComponentUpdateFlag when certain Anim notify state (or by any other stuff..) changes its mesh component's MeshComponentUpdateFlag */
	//TEnumAsByte<EMeshComponentUpdateFlag::Type> TemporaryMeshCompUpdateFlagBackUp;
	uint32 bTemporarilySetMeshCompUpdateFlag : 1;

	int32				TeamNum;

	bool bHideFromEventDirector;

	bool bIgnoreRotateByDamage;

public:
	void OverrideMaterial(class UMaterialInterface* NewMaterial, ECharacterMaterialOverrideType MatType);
	void RestoreOverrideMaterial(ECharacterMaterialOverrideType MatType);

	class UMaterialInterface* GetOriginalMaterial(int32 MaterialIndex);

	virtual void SetupLODInfoAsNPC();

	/** Set and run. */
	void SetBehaviorTree(class UBehaviorTree* InBT);

	/** Apply NPCClassInfo, as a main part of NPCClassInfo spawn procedure. */
	void OverrideByNPCClassInfo(ENPCClass InClass, class UB2NPCSingleClassInfo* InClassInfo);

	void SetIgnoreRotateByDamage()
	{
		bIgnoreRotateByDamage = true;
	}
protected:
	/** It is executed right after being spawned as ABladeIICharacter, before overridden by NPCClassInfo. */
	virtual void OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride);

	void PreCreateAndSetMaterialInstanceDynamic();

public:

	virtual void NotifyActorCustomEvent(FName EventName = NAME_None, UObject* OptionalNotifyingObject = NULL) /*override*/;

	/** See if this character is attacking in any kind. */
	bool IsAttacking() const;
	virtual bool IsComboAttacking() const;
	virtual bool IsSkillAttacking() const;
	virtual bool IsCounterAttacking() const;

	virtual bool HadHugeDamaged(); // Knockback, Big bounce

	bool IsApplyGuarding();
	virtual bool IsGuarding();

	bool HasSkillAttackSignal() const;

	bool IsImmune(const FDamageInfo* DamageInfoPtr = nullptr);
	bool IsGuardSuccess(const FDamageInfo* DamageInfoPtr = nullptr);
	bool IsForceStateAttack(const FDamageInfo* DamageInfoPtr);

	bool IsApplyDamageNotifyUI(const ABladeIICharacter* Attacker) const;

	virtual bool IsForceApplyHitAnimation()
	{
		return false;
	}

	//>> Check damage type runtime state 
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsStunned() const;
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	bool IsFrozen() const;

	FORCEINLINE bool IsBossMob()
	{
		return CharacterType > ECharacterType::ECT_MobEnd ? true : false;
	}

	/** To be used by some Anim notify state, which temporarily changes MeshComponentUpdateFlag and restore it back soon. */
	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	//void TemporarySetMeshComponentUpdateFlag(TEnumAsByte<EMeshComponentUpdateFlag::Type> InNewFlag);
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void RestoreTemporaryMeshComponentUpdateFlag();

	/** It calls SkeletalMeshComponent's SetAnimInstanceClass in its hidden state, preventing some multi-threading related crash in some mobile platform */
	void SafeSetAnimBPClass(class UAnimBlueprintGeneratedClass* InAnimBPClass);

	class AB2MonsterSpawnPool* GetBirthPlace();
	FORCEINLINE int32 GetSpawnPoolWaveNum()
	{
		return CachedBirthplaceInfo.WaveNumber;
	}
	FORCEINLINE int32 GetSpawnPoolWaveObjIndex()
	{
		return CachedBirthplaceInfo.WaveObjIndex;
	}
	FORCEINLINE bool IsSpawnedAsBossMob()
	{
		return CachedBirthplaceInfo.bSpawnedAsBossMob;
	}
	FORCEINLINE bool IsSummonedMob()
	{
		return CachedBirthplaceInfo.bIsSummoned;
	}
	bool AmIInterestedBossMob() const;

	// To be used by SpawnPool during spawning.
	FORCEINLINE void BeginSpawningNoDamageGuard()
	{
		bSpawningNoDamageGuard = true;
	}
	FORCEINLINE void EndSpawningNoDamageGuard()
	{
		bSpawningNoDamageGuard = false;
	}

	void SetDropItemInfo(FDropItemInfo& InDropItemInfo);

	FORCEINLINE void SetWeightRewardGold(int32 weight)
	{
		DropItemInfo.RewardGoldWeight = weight;
	}
	FORCEINLINE int32 GetWeightRewardGold()
	{
		return DropItemInfo.RewardGoldWeight;
	}

	FORCEINLINE void SetCanDropItem(bool CanDropItem)
	{
		DropItemInfo.bCanDropItem = CanDropItem;
	}

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual class ABladeIICharacter* GetNearestEnemyCharacter();

	virtual void GetNearestFriendCharacters(TArray<class ABladeIICharacter*>& Friends, float Range, int32 MaxNum);

	bool IsEnemy(class AActor* DamageCauser);
	bool IsEnemy(int32 TargetTeamNum);

	class IGameRule* GetInjectedGameRule() const
	{
		return InjectedGameRule;
	}

	void CheckReservationDamage();

	/** Spawn drop items based on DropItemInfo, as one of on-dead operation */
	//void SpawnDropItem();

	class UB2NPCSingleClassInfo* GetMyNPCClassInfo(); // Only for Mobs.

	IGameRule* InjectedGameRule;

	bool CanTakeEtherEffect(class AActor* EffectCauser);

public:
	static float FreezeTimeDilation;
	static float MinTimeDilation;

	// Per mesh instanced AnimNotifyState data
	// For B2CharEmissive
	FLinearColor B2CharEmissiveTargetColor;
	float B2CharEmissiveRemainTime;
	uint8 ChainBitField;

	// For B2CustomTimeDilation
	float B2TimeDilationForAnimNS;
	float B2CachedDilationForAnimNS;
	virtual class UMaterialInstanceDynamic* GetChainMaterial()
	{
		return NULL;
	}

	virtual bool IsImmortalMode() const {
		return false;
	}
	virtual bool IsHeroMode() const {
		return false;
	}

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
public:
	static bool bHideAllNPCs;
#endif
};
//
//////////////////////////////////////////////////////////////////////////
// Inlines

FORCEINLINE float ABladeIICharacter::GetHealth() const
{
	return Health;
}

FORCEINLINE float ABladeIICharacter::GetArmor() const
{
	return Armor;
}

FORCEINLINE float ABladeIICharacter::GetMaxArmor() const
{
	return MaxArmor;
}

FORCEINLINE bool ABladeIICharacter::IsAlive() const
{
	//return IsValidObj() && !bIsDying;
	return false;
}

FORCEINLINE EAttackState ABladeIICharacter::GetAttackState() const
{
	return AttackState;
}

FORCEINLINE bool ABladeIICharacter::GetSignalAttack() const
{
	return bSignalAttack;
}

FORCEINLINE ENPCClass ABladeIICharacter::GetNPCClass() const
{
	return NPCClassEnum;
}

FORCEINLINE ENPCClassVariation ABladeIICharacter::GetNPCClassVariation() const
{
	return NPCClassVariation;
}

FORCEINLINE EStageEventShowState ABladeIICharacter::GetStageEventShowState()
{
	return StageEventShowState;
}

FORCEINLINE bool ABladeIICharacter::IsPlayingDirectedStageEvent() const
{
	return bPlayingDirectedStageEvent;
}

FORCEINLINE bool ABladeIICharacter::GetQTEEnabled() const
{
	return bQTEEnabled;
}

FORCEINLINE bool ABladeIICharacter::IsInQTEState() const
{
	return bInQTEState;
}

FORCEINLINE bool ABladeIICharacter::DidCommitSuicideAtDeadEnd() const
{
	return bCommittedSuicideAtDeadEnd;
}

namespace
{
	// Shared utility function
	void EmissiveNotifyEnd(class USkeletalMeshComponent* MeshComp, uint8& ChainBitField, UMaterialInstanceDynamic* ChainMat, bool bForced = false)
	{
		//if (MeshComp == NULL)
		//{
		//	return;
		//}

		//ABladeIICharacter* pPawn = Cast<ABladeIICharacter>(MeshComp->GetOwner());
		//if (!pPawn)
		//	return;

		//for (int32 i = 0; i < MeshComp->GetNumMaterials(); ++i)
		//{
		//	if (bForced || (MeshComp->B2BitFields & (1 << i)))
		//	{
		//		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(pPawn->GetOriginalMaterial(i));
		//		if (DynamicMaterial)
		//		{
		//			// Assume Dynamic MI always have a parent MI, because SetMaterial cause problem on device
		//			UMaterialInstance* OriginalMI = Cast<UMaterialInstance>(DynamicMaterial->Parent);
		//			if (OriginalMI)
		//			{
		//				// SetMaterial cause problem on device, but this works fine
		//				DynamicMaterial->CopyParameterOverrides(OriginalMI);
		//			}
		//		}
		//	}
		//}

		//MeshComp->B2BitFields = 0;

		//if (ChainMat && (bForced || ChainBitField != 0))
		//{
		//	// Assume Dynamic MI always have a parent MI, because SetMaterial cause problem on device
		//	UMaterialInstance* OriginalMI = Cast<UMaterialInstance>(ChainMat->Parent);
		//	if (OriginalMI)
		//	{
		//		// SetMaterial cause problem on device, but this works fine
		//		ChainMat->CopyParameterOverrides(OriginalMI);
		//	}
		//}

		//ChainBitField = 0;
	}
};
