// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "GameFramework/DamageType.h"
#include "BladeIIDamageType.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Default = 0,
	EAT_LeftSwing = 1, // Used for random damage anim for PC
	EAT_RightSwing = 2, // Used for random damage anim for PC
	EAT_AirUp,
	EAT_AirDown,
	EAT_BigBounce,
	EAT_KnockBack,
	EAT_End
};

inline bool IsTinyDamage(EAttackType AttackType)
{
	return AttackType == EAttackType::EAT_LeftSwing || AttackType == EAttackType::EAT_RightSwing;
}

inline bool IsHugeDamage(EAttackType AttackType)
{
	return AttackType == EAttackType::EAT_AirUp || AttackType == EAttackType::EAT_AirDown || AttackType == EAttackType::EAT_BigBounce || AttackType == EAttackType::EAT_KnockBack;
}

UENUM(BlueprintType)
enum class EDamageWeapon : uint8
{
	EDW_Blade = 0,
	EDW_Hammer,
	EDW_Magic,
	EDW_End
};

// May have connection between EStateAttackType?
UENUM(BlueprintType)
enum class EDamageElement : uint8
{
	EDE_None = 0,
	EDE_Ice,
	EDE_Fire,
	EDE_Poison,
	EDE_Elec,
	EDE_End
};

// Buff state
UENUM(BlueprintType)
enum class EBuffType : uint8
{
	Buff_None = 0,

	//加冠
	Debuff_Immobilize = 1,

	//葫搬
	Debuff_Freezing,

	//胶畔
	Debuff_Stun,

	//拳堪
	Debuff_Burning,

	//吝刀
	Debuff_Poison,

	//档惯
	Debuff_Taunt,

	//魔宫
	DeBuff_Silent,

	//豪牢
	Debuff_Seal,

	//堡气拳
	Buff_Fury,

	//篮脚
	Buff_Hide,

	//罐绰单固瘤 皑家
	Buff_DecreaseReceiveDamage,

	//单固瘤 馆荤
	Buff_ReflectDamage,

	//眉仿软荐(傍拜且锭)
	Buff_HealHPByAttack,

	//蒋靛
	Buff_Shield,

	//摹疙鸥犬伏 刘啊
	Buff_IncreaseCriticalRate,

	//叼滚橇 搁开
	Buff_AbnormalStateImmune,

	//摹疙鸥单固瘤 刘啊
	Buff_IncreaseCriticalDamage,

	//酒赣(促崔搁 乔拜葛记 窍绰芭) 刘啊
	Buff_IncreaseArmor,

	//鳃
	Buff_SkillHealHP,

	//公利
	Buff_Unbeatable,

	//单固瘤 刘啊
	Buff_IncreaseAttackDamage,

	//捞悼加档 刘啊
	Buff_IncreaseMoveSpeed,

	//傍拜俊 葫搬魄沥 眠啊
	Buff_AllFreezeAttack,

	//傍拜俊 胶畔魄沥 眠啊
	Buff_AllStunAttack,

	//傍拜俊 拳堪魄沥 眠啊
	Buff_AllBurningAttack,

	//傍拜俊 吝刀魄沥 眠啊
	Buff_AllPoisonAttack,

	//洒飘 橇府令
	Buff_HitFreeze,

	//某腐磐 碍拳(Scale UP, A,D UP)
	Buff_Enhance,

	//规绢仿 刘啊
	Buff_IncreaseDefense,

	//叼胶歧
	Buff_Dispell,

	//Heal
	Buff_Heal,

	// 己家 坷宏 嚼殿矫 单固瘤 刘啊
	Buff_IncreaseAttackDamage_Orb,

	// 己家 坷宏 嚼殿矫 单固瘤 皑家
	Buff_DecreaseReceiveDamage_Orb,

	Buff_End
};

UENUM(BlueprintType)
enum class EUniqueBuffType : uint8
{
	EBT_Normal = 0,
	EBT_Fury = 1, // Monster only for now
	// 4th Skill buff, cannot be shared
	EBT_Hide = 2,
	EBT_DecreaseDamage = 3,
	EBT_ReflectDamage = 4,
	EBT_HealHPByAttack = 5,
	EBT_End
};

UENUM(BlueprintType)
enum class ETeamBuffType : uint8
{
	ETBT_Normal = 0,
	// 6th Skill buff, can be shared
	ETBT_Shield = 1,
	ETBT_IncreaseCriticalRate = 2,
	ETBT_AbnormalStateImmune = 4,
	ETBT_IncreaseCriticalDamage = 8,
	ETBT_Enhance = 16,
	ETBT_Taunt = 32,
	ETBT_Dispell = 64,
	ETBT_End = 128
};

//第俊 眠啊 扁粮波 剐覆
UENUM(BlueprintType, meta = (Bitflags))
enum class ETargetSelectType : uint8
{
	ETST_AddAllyTeam,
	ETST_AddEnemyTeam,
	ETST_FilterMercenary,	//侩捍父
	ETST_FilterPlayer,		//敲饭捞绢父
	ETST_ExceptSelf,		//磊脚 昏力
	ETST_FilterSelf,		//唱父 
	/////////
};
ENUM_CLASS_FLAGS(ETargetSelectType);

UENUM(BlueprintType)
enum class EInfluenceType : uint8
{
	EIT_IncreaseArmor = 0,
	EIT_SkillHealHP,
	EIT_Invincible,
	EIT_DecreaseReceiveDamage,
	EIT_IncreaseAttackDamage,
	EIT_IncreaseMoveSpeed,
	EIT_ReflectDamage, // EBT_ReflectDamage Buff always override
	EIT_HealHPByAttack, // EBT_HealHPByAttack Buff always override
	EIT_AllFreezeAttack, // Should match with EStateAttackType
	EIT_AllStunAttack, // Should match with EStateAttackType
	EIT_AllBurningAttack, // Should match with EStateAttackType
	EIT_AllPoisonAttack, // Should match with EStateAttackType
	EIT_End
};

// Abnormal state
UENUM(BlueprintType)
enum class EStateAttackType : uint8
{
	EDT_Normal = 0,
	EDT_Freezing,
	EDT_Stun,
	EDT_Burning, // independent, Damage over time
	EDT_Poison, // independent, Slow
	EDT_Immobilize,
	EDT_BigBounce,
	EDT_NockBack,
	EDT_Seal,
	EDT_End
};

inline int32 GetStateAttackTypeArrayNum() {
	return (static_cast<int32>(EStateAttackType::EDT_End) - 1);
}
inline int32 GetStateAttackTypeArrayIndex(EStateAttackType InType) {
	return (static_cast<int32>(InType) - 1);
}

EBuffType GetBuffTypeByStateAttack(EStateAttackType InType);

UENUM(BlueprintType)
enum class EDamageLogicType : uint8
{
	ELT_Normal = 0, // Default
	ELT_Absolute,
	ELT_AttactCount,
	ELT_PctByTargetTotalHP,
	ELT_PctByTargetCurrentHP,
	ELT_PctByTargetLostHP,
	ELT_PctByDealerTotalHP, // Not available for State Damage
	ELT_PctByDealerCurrentHP,
	ELT_PctByDealerLostHP,
	ELT_End
};

UENUM(BlueprintType)
enum class EArmorBreakLogicType : uint8
{
	EALT_Normal = 0, // Default
	EALT_AlwaysBreak,
	EALT_End
};

UENUM(BlueprintType)
enum class EGuardBreakLogicType : uint8
{
	EGLT_AbleToGuard = 0, // Default
	EGLT_GuardBreak,
	EGLT_UnableToGuard,
	EGLT_End
};

UENUM(BlueprintType)
namespace EDamageNumberType
{
	enum Type
	{
		EDNT_EnemyDamage = 0,
		EDNT_FriendlyDamage,
		EDNT_EnemyDamageCritical,
		EDNT_FriendlyDamageCritical,
		EDNT_EnemyDamageOverTime,
		EDNT_FriendlyDamageOverTime,
		EDNT_Heal,
		EDNT_End
	};
}

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	EHD_None = 0,
	EHD_PosX, // same as Hit Actor Rotation
	EHD_NegX,
	EHD_PosY,
	EHD_NegY,
	EHD_HitActor, // with no random rotation
	EHD_End
};

USTRUCT(BlueprintType)
struct FDamageInfo //: public FDamageInfoBase
{
	GENERATED_USTRUCT_BODY()

	FDamageInfo();
	void SetDamageInfo(const struct FDamageInfo* DamageInfo, const class ABladeIICharacter* B2Char);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArmorBreak, Meta = (DisplayName = "Armor Break Response"))
	EAttackType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	EDamageWeapon WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Element)
	EDamageElement ElementType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EStateAttackType StateType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	EDamageLogicType DamageLogicType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArmorBreak)
	EArmorBreakLogicType ArmorBreakLogicType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArmorBreak)
	EGuardBreakLogicType GuardBreakLogicType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArmorBreak)
	float ArmorBreakAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float StateDamageTriggerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float StateDamageDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float StateDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EDamageLogicType StateDamageLogicType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float CriticalRateOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float CriticalDamageRateOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float DamageIncreaseRateOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float BossDamageIncreaseRateOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	FVector Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	FVector Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bApplyToNPC : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bCountable : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bStunCountable : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bUseHitSound : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bIgnoreNotifyToAttacker : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bOffenseEtherActivatable : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bDefenseEtherActivatable : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bForceApplyStateDamage : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint32 bIgnoreIncreaseDamageNum : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	EHitDirection HitDirectionOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float HitRandomAngleOverride;


	FORCEINLINE uint32 GetDamageHash() const
	{
		return DamageHash;
	}
	uint32 CalculateDamageHash(UObject* Object) const;

	static const FDamageInfo* GetDefaultDamagePtr();
	static const FDamageInfo& GetDefaultDamage();

private:

	UPROPERTY()
	mutable uint32 DamageHash;
};


USTRUCT()
struct FDamageInfoContainer
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TMap<uint32, FDamageInfo> DamageInfoMap;

	bool RegisterDamageInfo(uint32 HashValue, const FDamageInfo& DamageInfo);

	static const FDamageInfo* GetDamageInfo(uint32 HashValue);
	static FDamageInfoContainer& GetContainer();
};