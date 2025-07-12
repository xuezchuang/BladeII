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

	//속박
	Debuff_Immobilize = 1,

	//빙결
	Debuff_Freezing,

	//스턴
	Debuff_Stun,

	//화염
	Debuff_Burning,

	//중독
	Debuff_Poison,

	//도발
	Debuff_Taunt,

	//침묵
	DeBuff_Silent,

	//봉인
	Debuff_Seal,

	//광폭화
	Buff_Fury,

	//은신
	Buff_Hide,

	//받는데미지 감소
	Buff_DecreaseReceiveDamage,

	//데미지 반사
	Buff_ReflectDamage,

	//체력흡수(공격할때)
	Buff_HealHPByAttack,

	//쉴드
	Buff_Shield,

	//치명타확률 증가
	Buff_IncreaseCriticalRate,

	//디버프 면역
	Buff_AbnormalStateImmune,

	//치명타데미지 증가
	Buff_IncreaseCriticalDamage,

	//아머(다달면 피격모션 하는거) 증가
	Buff_IncreaseArmor,

	//힐
	Buff_SkillHealHP,

	//무적
	Buff_Unbeatable,

	//데미지 증가
	Buff_IncreaseAttackDamage,

	//이동속도 증가
	Buff_IncreaseMoveSpeed,

	//공격에 빙결판정 추가
	Buff_AllFreezeAttack,

	//공격에 스턴판정 추가
	Buff_AllStunAttack,

	//공격에 화염판정 추가
	Buff_AllBurningAttack,

	//공격에 중독판정 추가
	Buff_AllPoisonAttack,

	//히트 프리즈
	Buff_HitFreeze,

	//캐릭터 강화(Scale UP, A,D UP)
	Buff_Enhance,

	//방어력 증가
	Buff_IncreaseDefense,

	//디스펠
	Buff_Dispell,

	//Heal
	Buff_Heal,

	// 성소 오브 습등시 데미지 증가
	Buff_IncreaseAttackDamage_Orb,

	// 성소 오브 습등시 데미지 감소
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

//뒤에 추가 기존꺼 밀림
UENUM(BlueprintType, meta = (Bitflags))
enum class ETargetSelectType : uint8
{
	ETST_AddAllyTeam,
	ETST_AddEnemyTeam,
	ETST_FilterMercenary,	//용병만
	ETST_FilterPlayer,		//플레이어만
	ETST_ExceptSelf,		//자신 삭제
	ETST_FilterSelf,		//나만 
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