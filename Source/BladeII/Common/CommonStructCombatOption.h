#pragma once

#include "B2MessageInfoCommonStruct.h"
#include "CommonStructCombatOption.generated.h"

// A part of CommonStruct.h. Do not include this by itself. Include "CommonStruct.h"
UENUM(BlueprintType)
enum class ESkillOption : uint8
{
	ESO_NULL = 0,
	// Active Skill Option
	ESO_IncAttackDamage = 1,
	ESO_DecCoolTime,
	ESO_IncCriticalRate,
	ESO_IncCriticalDamage,
	ESO_IncAttackDamageToBoss,					//=5
	ESO_IncMotion,
	ESO_IncArea,
	ESO_DecUnderAttackDamage,
	ESO_IncMoveSpeed,
	ESO_HealHP,									//=10
	ESO_DecCoolTimeOtherSkills,
	// Buff Skill Option
	ESO_IncProtectHPRateOnBuff,
	ESO_BuffTime,
	ESO_IncAttackDamageOnBuff,
	ESO_DecReceiveDamageOnBuff,				//=15
	ESO_IncMaxNumCharacterBuff,
	// Passive Skill Option
	ESO_HealHPByCountAttack,
	ESO_IncNormalDamage,
	ESO_IncSkillDamage,
	ESO_IncAttackDamageToGladiator,			//=20
	ESO_IncAttackDamageToAssassin,
	ESO_IncAttackDamageToWizard,
	ESO_IncAttackDamageToFighter,
	ESO_IncMaxHP,
	ESO_DecUnderAttackMeleeDamage,				//=25
	ESO_DecUnderAttackRangeDamage,
	ESO_IncResistAbnormal,	// Decreasing option in fact.. (DecAbnormalStateTime)
	ESO_DecReceiveDamageByGladiator,
	ESO_DecReceiveDamageByAssassin,
	ESO_DecReceiveDamageByWizard,			//=30
	ESO_DecReceiveDamageByFighter,
	ESO_EnableTagOnKnockBack,
	ESO_EnableTagOnBigBounce,
	// Buff Skill Option
	ESO_IncAttackSpeedOnBuff,
	ESO_HealHPOnHitOnBuff,						//=35
	ESO_IncMoveSpeedDuring3Sec,
	ESO_IncReflectDamageOnBuff,
	ESO_IncCriticalRateOnBuff,
	ESO_IncCriticalDamageOnBuff,
	ESO_DecReceiveDamage,						//=40
	//
	// Check GetSkillOptionApplyType for additional Dec** option type
	//

	ESO_End
};

inline ESkillOption SvrToCliSkillOption(int32 ServerSkillOptionID)
{
	return static_cast<ESkillOption>(ServerSkillOptionID);
}

inline int32 CliToSvrSkillOption(ESkillOption ClientSkillOptionEnum)
{
	return static_cast<int32>(ClientSkillOptionEnum);
}

UENUM(BlueprintType)
enum class EMercenarySkillOption : uint8
{
	EMSO_NULL = 0,
	// Active Skill Option
	EMSO_IncAttackDamage = 1,
	EMSO_DecCoolTime,
	EMSO_IncCriticalRate,
	EMSO_IncCriticalDamage,
	EMSO_IncAttackDamageToBoss,					//=5
	EMSO_IncMotion,
	EMSO_IncArea,
	EMSO_DecUnderAttackDamage,
	EMSO_IncMoveSpeed,
	EMSO_HealHP,									//=10
	EMSO_DecCoolTimeOtherSkills,
	// Buff Skill Option
	EMSO_IncProtectHPRateOnBuff,
	EMSO_BuffTime,
	EMSO_IncAttackDamageOnBuff,
	EMSO_DecReceiveDamageOnBuff,				//=15
	EMSO_IncMaxNumCharacterBuff,
	// Passive Skill Option
	EMSO_HealHPByCountAttack,
	EMSO_IncNormalDamage,
	EMSO_IncSkillDamage,
	EMSO_IncAttackDamageToGladiator,			//=20
	EMSO_IncAttackDamageToAssassin,
	EMSO_IncAttackDamageToWizard,
	EMSO_IncAttackDamageToFighter,
	EMSO_IncMaxHP,
	EMSO_DecUnderAttackMeleeDamage,				//=25
	EMSO_DecUnderAttackRangeDamage,
	EMSO_IncResistAbnormal,	// Decreasing option in fact.. (DecAbnormalStateTime)
	EMSO_DecReceiveDamageByGladiator,
	EMSO_DecReceiveDamageByAssassin,
	EMSO_DecReceiveDamageByWizard,			//=30
	EMSO_DecReceiveDamageByFighter,
	EMSO_EnableTagOnKnockBack,
	EMSO_EnableTagOnBigBounce,
	// Buff Skill Option
	EMSO_IncAttackSpeedOnBuff,
	EMSO_HealHPOnHitOnBuff,						//=35
	EMSO_IncMoveSpeedDuring3Sec,
	EMSO_IncReflectDamageOnBuff,
	EMSO_IncCriticalRateOnBuff,
	EMSO_IncCriticalDamageOnBuff,
	EMSO_DecReceiveDamage,						//=40
	//眠啊且巴烙
	//Passive Skill老 荐档 瓷仿摹 老荐档
	EMSO_LowHealthRecoveryHP,
	EMSO_HealHPByAttack,
	EMSO_ImmunAbnormalState,
	EMSO_Invincible,



	EMSO_End
};


inline EMercenarySkillOption SvrToCliMercenarySkillOption(int32 ServerSkillOptionID)
{
	return static_cast<EMercenarySkillOption>(ServerSkillOptionID);
}

inline int32 CliToSvrMercenarySkillOption(EMercenarySkillOption ClientSkillOptionEnum)
{
	return static_cast<int32>(ClientSkillOptionEnum);
}


/** Item's option(abilities) other than the basic attack/defense point. (also being called as item option) */
UENUM(BlueprintType)
enum class EItemOption : uint8
{
	//////////////////////////////////////////////////
	//
	// Watch out for a new entry and/or number changes!
	// The number must be consistent between server and client!
	//
	// In addition, check related utilities declared at B2inventory.h (GetItemOptionApplyType, IsItemOptionPerPCClass, etc..)
	//
	//////////////////////////////////////////////////

	//
	// Offensive options
	//

	EIO_Offense_IncAttack = 0,
	EIO_Offense_IncDamage,
	EIO_Offense_IncNormalDamage,
	EIO_Offense_IncCriticalDamage,
	EIO_Offense_IncCriticalRate,
	EIO_Offense_IncBossDamage,
	EIO_Offense_IncAttackSpeed,
	EIO_Offense_IncAdditionalDamage,

	//
	// Skill type options
	//

	EIO_Skill_IncSkillDamage,
	EIO_Skill_DecSkillCooltime,
	EIO_Skill_IncSkill1Damage,
	EIO_Skill_IncSkill2Damage,
	EIO_Skill_IncSkill3Damage,
	EIO_Skill_IncSkill5Damage,
	// Not skill 4 and 6 kkk

	//
	// Health type options
	//

	EIO_Health_ProbableHPAbsorption, // HP absorption on target "hit"
	EIO_Health_ProbableRecoverHP, // HP recover on target's "death".
	EIO_Health_IncHealth,
	EIO_Health_PerSecRecoverHP,

	//
	// Defensive options
	//

	EIO_Defense_IncDefense,
	EIO_Defense_DecAbnormalStateTime,
	EIO_Defense_DecDamage,
	EIO_Defense_DecBossDamage,
	EIO_Defense_DecMeleeDamage,
	EIO_Defense_DecRangedDamage,
	EIO_Defense_ResistCritical,
	EIO_Defense_DecAdditionalDamage,

	//
	// Miscellaneous.. they have their own categories like probability type, adventure type though.. well guess that I don't really have to give a shit.
	//

	EIO_Misc_LastNormalAttackKnockbackRate,
	EIO_Misc_AdditionalExp,
	EIO_Misc_AdditionalGold,
	EIO_Misc_IncMoveSpeed,
	EIO_Misc_DecTagCooltime,
	//*
	EIO_Tag_Offense_IncAttack,
	EIO_Tag_Defense_IncDefense,
	EIO_Tag_Health_IncHealth,

	EIO_HeroTower_Offense_IncAttack,
	EIO_HeroTower_Defense_IncDefense,
	EIO_HeroTower_Health_IncHealth,

	EIO_Assault_Offense_IncAttack,
	EIO_Assault_Defense_IncDefense,
	EIO_Assault_Health_IncHealth,

	EIO_Raid_Offense_IncAttack,
	EIO_Raid_Defense_IncDefense,
	EIO_Raid_Health_IncHealth,

	EIO_TeamMatch_Offense_IncAttack,
	EIO_TeamMatch_Defense_IncDefense,
	EIO_TeamMatch_Health_IncHealth,

	EIO_Guild_Offense_IncAttack,
	EIO_Guild_Defense_IncDefense,
	EIO_Guild_Health_IncHealth,
	//*/
	EIO_End // None. It may represent empty slot.
};

/**
 * It lists combat options by more than one separated systems (ItemOption and SkillOption.. but any others possible)
 * It does not list all, and mainly intended to include options applied in passive manner, especially in the case of skill options.
 * A relevant ItemOption and/or SkillOption entry can be found for each identifier.
 * It could be found by both systems or could be found by only one.
 * See GetSkillOptionOfUnitedOption and GetItemOptionOfUnitedOption
 */
enum class EUnitedCombatOptions : uint8
{
	UCO_Offense_IncAttack,	// Increasing one of primary combat stat.

	UCO_Offense_IncDamage,
	UCO_Offense_IncNormalDamage,
	UCO_Offense_IncCriticalDamage,
	UCO_Offense_IncCriticalRate,
	UCO_Offense_IncBossDamage,
	UCO_Offense_IncAttackSpeed,
	UCO_Offense_IncAdditionalDamage,

	// Applied for PC to PC
	UCO_Offense_IncAttackDamageToGladiator,
	UCO_Offense_IncAttackDamageToAssassin,
	UCO_Offense_IncAttackDamageToWizard,
	UCO_Offense_IncAttackDamageToFighter,

	//UCO_Offense_IncDamageArea, Active Skill..

	//
	// Skill type options.. options which can affect skill, not the SkillOption itself.
	//

	UCO_Skill_IncSkillDamage,
	UCO_Skill_DecSkillCooltime_General,
	//UCO_Skill_DecSkillCooltime_Other, // Active Skill
	UCO_Skill_IncSkill1Damage,
	UCO_Skill_IncSkill2Damage,
	UCO_Skill_IncSkill3Damage,
	UCO_Skill_IncSkill5Damage,



	//
	// Health type options
	//

	UCO_Health_IncHealth,	// Increasing one of primary combat stat.
	// Different from IncHealth in applying type.
	UCO_Health_IncMaxHP,
	UCO_Health_ProbableHPAbsorption, // HP absorption on target "hit"
	UCO_Health_ProbableRecoverHP, // HP recover on target's "death".
	UCO_Health_PerSecRecoverHP,
	//UCO_Health_HealHP, // Active Skill.



	//
	// Defensive options
	//

	UCO_Defense_IncDefense,		// Increasing one of primary combat stat.
	UCO_Defense_DecAbnormalStateTime,

	UCO_Defense_DecDamage,
	UCO_Defense_DecBossDamage,
	UCO_Defense_DecMeleeDamage,
	UCO_Defense_DecRangedDamage,

	// Applied for PC to PC
	UCO_Defense_DecDamageByGladiator,
	UCO_Defense_DecDamageByAssassin,
	UCO_Defense_DecDamageByWizard,
	UCO_Defense_DecDamageByFighter,

	UCO_Defense_ResistCritical,
	UCO_Defense_HealHPByCountAttack,
	UCO_Defense_DecAdditionalDamage,



	// Buff Skill Option.. they are probably only for active skills, not include here for now.
	/*UCO_Buff_IncProtectHPRateOnBuff,
	UCO_Buff_BuffTime,
	UCO_Buff_IncAttackDamageOnBuff,
	UCO_Buff_DecReceiveDamageBonusBuff,
	UCO_Buff_DecReceiveDamageOnBuff,
	UCO_Buff_IncMaxNumCharacterBuff,
	UCO_Buff_IncAttackSpeedOnBuff,
	UCO_Buff_HealHPOnHitOnBuff,
	UCO_Buff_IncMoveSpeedDuring3Sec,
	UCO_Buff_IncReflectDamageOnBuff,
	UCO_Buff_IncCriticalRateOnBuff,
	UCO_Buff_IncCriticalDamageOnBuff,
	*/



	//
	// Miscellaneous..
	//

	UCO_Misc_LastNormalAttackKnockbackRate,
	UCO_Misc_AdditionalExp,		// 救靖
	UCO_Misc_AdditionalGold,	// 救靖
	UCO_Misc_IncMoveSpeed,
	UCO_Misc_DecTagCooltime,

	UCO_Tag_Offense_IncAttack,
	UCO_Tag_Defense_IncDefense,
	UCO_Tag_Health_IncHealth,

	UCO_HeroTower_Offense_IncAttack,
	UCO_HeroTower_Defense_IncDefense,
	UCO_HeroTower_Health_IncHealth,

	UCO_Assault_Offense_IncAttack,
	UCO_Assault_Defense_IncDefense,
	UCO_Assault_Health_IncHealth,

	UCO_Raid_Offense_IncAttack,
	UCO_Raid_Defense_IncDefense,
	UCO_Raid_Health_IncHealth,

	UCO_TeamMatch_Offense_IncAttack,
	UCO_TeamMatch_Defense_IncDefense,
	UCO_TeamMatch_Health_IncHealth,

	UCO_Guild_Offense_IncAttack,
	UCO_Guild_Defense_IncDefense,
	UCO_Guild_Health_IncHealth,

	UCO_End
};

enum class EOptionVariation : uint8
{
	EOV_Incerease = 0,
	EOV_Decrease,

	EOV_End
};

/** Provides mapping between EUnitedCombatOptions to EItemOption and/or ESkillOption */
struct FUCOBoundInfo
{
	FUCOBoundInfo(EUnitedCombatOptions InUCO, EItemOption InMappedItemOption, ESkillOption InMappedSkillOption)
		: UnitedOptionId(InUCO)
		, MappedItemOption(InMappedItemOption)
		, MappedSkillOption(InMappedSkillOption)
	{
	}
private:
	const EUnitedCombatOptions UnitedOptionId; // The key ID of this bound info.
	// Not both of those mapped options are ensured to be valid. One of them might be unused (**_End)
	const EItemOption MappedItemOption;
	const ESkillOption MappedSkillOption;
public:
	FORCEINLINE EUnitedCombatOptions GetUnitedOptionId() const { return UnitedOptionId; }
	FORCEINLINE EItemOption GetMappedItemOption() const { return MappedItemOption; }
	FORCEINLINE ESkillOption GetMappedSkillOption() const { return MappedSkillOption; }
};

/* Define it just to place it at this header file.*/
FORCEINLINE void INITIALIZE_UNITED_OPTION_MAPPING_BLOCK(TMap<EUnitedCombatOptions, FUCOBoundInfo>& InMappingTable, TMap<EItemOption, EUnitedCombatOptions>& InItemOptionMappingTable,
	TMap<ESkillOption, EUnitedCombatOptions>& InSkillMappingTable, TMap<EMercenarySkillOption, EUnitedCombatOptions>& InMercenarySkillMappingTable)
{
	InMappingTable.Empty(); // Could be called more than once for mistake..
	InItemOptionMappingTable.Empty();
	InSkillMappingTable.Empty();

#define ADD_UNITED_OPTION_MAPPING(UnitedOption, ItemOption, SkillOption, MercenaryOption) \
	InMappingTable.Add(UnitedOption, FUCOBoundInfo(UnitedOption, ItemOption, SkillOption)); \
	InItemOptionMappingTable.Add(ItemOption, UnitedOption); \
	InSkillMappingTable.Add(SkillOption, UnitedOption); \
	InMercenarySkillMappingTable.Add(MercenaryOption, UnitedOption);

	// Any **_End member of BoundInfo means it is not mapped for that option. Implemented for only one option system.
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAttack, EItemOption::EIO_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncDamage, EItemOption::EIO_Offense_IncDamage, ESkillOption::ESO_IncAttackDamage, EMercenarySkillOption::EMSO_IncAttackDamage); // ESO_IncAttackDamage can be also applied as active.
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncNormalDamage, EItemOption::EIO_Offense_IncNormalDamage, ESkillOption::ESO_IncNormalDamage, EMercenarySkillOption::EMSO_IncNormalDamage);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, EItemOption::EIO_Offense_IncCriticalDamage, ESkillOption::ESO_IncCriticalDamage, EMercenarySkillOption::EMSO_IncCriticalDamage); // ESO_IncCriticalDamage can be also applied as active.
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncCriticalRate, EItemOption::EIO_Offense_IncCriticalRate, ESkillOption::ESO_IncCriticalRate, EMercenarySkillOption::EMSO_IncCriticalRate); // ESO_IncCriticalRate can be also applied as active.
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncBossDamage, EItemOption::EIO_Offense_IncBossDamage, ESkillOption::ESO_IncAttackDamageToBoss, EMercenarySkillOption::EMSO_IncAttackDamageToBoss); // ESO_IncAttackDamageToBoss can be also applied as active.
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAttackSpeed, EItemOption::EIO_Offense_IncAttackSpeed, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAdditionalDamage, EItemOption::EIO_Offense_IncAdditionalDamage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToGladiator, EItemOption::EIO_End, ESkillOption::ESO_IncAttackDamageToGladiator, EMercenarySkillOption::EMSO_IncAttackDamageToGladiator);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToAssassin, EItemOption::EIO_End, ESkillOption::ESO_IncAttackDamageToAssassin, EMercenarySkillOption::EMSO_IncAttackDamageToAssassin);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToWizard, EItemOption::EIO_End, ESkillOption::ESO_IncAttackDamageToWizard, EMercenarySkillOption::EMSO_IncAttackDamageToWizard);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToFighter, EItemOption::EIO_End, ESkillOption::ESO_IncAttackDamageToFighter, EMercenarySkillOption::EMSO_IncAttackDamageToFighter);
	//ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Offense_IncDamageArea, EItemOption::EIO_End, ESkillOption::ESO_IncArea);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_IncSkillDamage, EItemOption::EIO_Skill_IncSkillDamage, ESkillOption::ESO_IncSkillDamage, EMercenarySkillOption::EMSO_IncSkillDamage);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_DecSkillCooltime_General, EItemOption::EIO_Skill_DecSkillCooltime, ESkillOption::ESO_DecCoolTime, EMercenarySkillOption::EMSO_DecCoolTime); // ESO_DecCoolTime can be also applied as active.
	//ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_DecSkillCooltime_Other, EItemOption::EIO_End, ESkillOption::ESO_DecCoolTimeOtherSkills); ESO_DecCoolTimeOtherSkills is purely active I guess..
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_IncSkill1Damage, EItemOption::EIO_Skill_IncSkill1Damage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_IncSkill2Damage, EItemOption::EIO_Skill_IncSkill2Damage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_IncSkill3Damage, EItemOption::EIO_Skill_IncSkill3Damage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Skill_IncSkill5Damage, EItemOption::EIO_Skill_IncSkill5Damage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Health_IncHealth, EItemOption::EIO_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Health_IncMaxHP, EItemOption::EIO_End, ESkillOption::ESO_IncMaxHP, EMercenarySkillOption::EMSO_IncMaxHP);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, EItemOption::EIO_Health_ProbableHPAbsorption, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, EItemOption::EIO_Health_ProbableRecoverHP, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Health_PerSecRecoverHP, EItemOption::EIO_Health_PerSecRecoverHP, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	// HealHP could be ProbableRecoverHP or PerSecRecoverHP. Not implemented at this point.
	//ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Health_HealHP, EItemOption::EIO_End, ESkillOption::ESO_HealHP);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_IncDefense, EItemOption::EIO_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	// Here is an odd case. ItemOption DecAbnormalStateTime and SkillOption IncResistAbnormal is the same. While the naming is somewhat different.																		
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime, EItemOption::EIO_Defense_DecAbnormalStateTime, ESkillOption::ESO_IncResistAbnormal, EMercenarySkillOption::EMSO_IncResistAbnormal);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecDamage, EItemOption::EIO_Defense_DecDamage, ESkillOption::ESO_DecUnderAttackDamage, EMercenarySkillOption::EMSO_DecUnderAttackDamage);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecBossDamage, EItemOption::EIO_Defense_DecBossDamage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecMeleeDamage, EItemOption::EIO_Defense_DecMeleeDamage, ESkillOption::ESO_DecUnderAttackMeleeDamage, EMercenarySkillOption::EMSO_DecUnderAttackMeleeDamage);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecRangedDamage, EItemOption::EIO_Defense_DecRangedDamage, ESkillOption::ESO_DecUnderAttackRangeDamage, EMercenarySkillOption::EMSO_DecUnderAttackRangeDamage);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecDamageByGladiator, EItemOption::EIO_End, ESkillOption::ESO_DecReceiveDamageByGladiator, EMercenarySkillOption::EMSO_DecReceiveDamageByGladiator);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecDamageByAssassin, EItemOption::EIO_End, ESkillOption::ESO_DecReceiveDamageByAssassin, EMercenarySkillOption::EMSO_DecReceiveDamageByAssassin);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecDamageByWizard, EItemOption::EIO_End, ESkillOption::ESO_DecReceiveDamageByWizard, EMercenarySkillOption::EMSO_DecReceiveDamageByWizard);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecDamageByFighter, EItemOption::EIO_End, ESkillOption::ESO_DecReceiveDamageByFighter, EMercenarySkillOption::EMSO_DecReceiveDamageByFighter);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_ResistCritical, EItemOption::EIO_Defense_ResistCritical, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_HealHPByCountAttack, EItemOption::EIO_End, ESkillOption::ESO_HealHPByCountAttack, EMercenarySkillOption::EMSO_HealHPByCountAttack);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Defense_DecAdditionalDamage, EItemOption::EIO_Defense_DecAdditionalDamage, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);


	/*
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncProtectHPRateOnBuff, EItemOption::EIO_End, ESkillOption::ESO_IncProtectHPRateOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_BuffTime, EItemOption::EIO_End, ESkillOption::ESO_BuffTime);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncAttackDamageOnBuff, EItemOption::EIO_End, ESkillOption::ESO_IncAttackDamageOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_DecReceiveDamageBonusBuff, EItemOption::EIO_End, ESkillOption::ESO_DecReceiveDamage);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_DecReceiveDamageOnBuff, EItemOption::EIO_End, ESkillOption::ESO_DecReceiveDamageOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncMaxNumCharacterBuff, EItemOption::EIO_End, ESkillOption::ESO_IncMaxNumCharacterBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncAttackSpeedOnBuff, EItemOption::EIO_End, ESkillOption::ESO_IncAttackSpeedOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_HealHPOnHitOnBuff, EItemOption::EIO_End, ESkillOption::ESO_HealHPOnHitOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncMoveSpeedDuring3Sec, EItemOption::EIO_End, ESkillOption::ESO_IncMoveSpeedDuring3Sec);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncReflectDamageOnBuff, EItemOption::EIO_End, ESkillOption::ESO_IncReflectDamageOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncCriticalRateOnBuff, EItemOption::EIO_End, ESkillOption::ESO_IncCriticalRateOnBuff);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Buff_IncCriticalDamageOnBuff, EItemOption::EIO_End, ESkillOption::ESO_IncCriticalDamageOnBuff);
	*/

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Misc_LastNormalAttackKnockbackRate, EItemOption::EIO_Misc_LastNormalAttackKnockbackRate, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Misc_AdditionalExp, EItemOption::EIO_Misc_AdditionalExp, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Misc_AdditionalGold, EItemOption::EIO_Misc_AdditionalGold, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Misc_IncMoveSpeed, EItemOption::EIO_Misc_IncMoveSpeed, ESkillOption::ESO_IncMoveSpeed, EMercenarySkillOption::EMSO_IncMoveSpeed);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Misc_DecTagCooltime, EItemOption::EIO_Misc_DecTagCooltime, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	//*
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Tag_Offense_IncAttack, EItemOption::EIO_Tag_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Tag_Defense_IncDefense, EItemOption::EIO_Tag_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Tag_Health_IncHealth, EItemOption::EIO_Tag_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_HeroTower_Offense_IncAttack, EItemOption::EIO_HeroTower_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_HeroTower_Defense_IncDefense, EItemOption::EIO_HeroTower_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_HeroTower_Health_IncHealth, EItemOption::EIO_HeroTower_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Assault_Offense_IncAttack, EItemOption::EIO_Assault_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Assault_Defense_IncDefense, EItemOption::EIO_Assault_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Assault_Health_IncHealth, EItemOption::EIO_Assault_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Raid_Offense_IncAttack, EItemOption::EIO_Raid_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Raid_Defense_IncDefense, EItemOption::EIO_Raid_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Raid_Health_IncHealth, EItemOption::EIO_Raid_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_TeamMatch_Offense_IncAttack, EItemOption::EIO_TeamMatch_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_TeamMatch_Defense_IncDefense, EItemOption::EIO_TeamMatch_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_TeamMatch_Health_IncHealth, EItemOption::EIO_TeamMatch_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);

	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Guild_Offense_IncAttack, EItemOption::EIO_Guild_Offense_IncAttack, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Guild_Defense_IncDefense, EItemOption::EIO_Guild_Defense_IncDefense, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	ADD_UNITED_OPTION_MAPPING(EUnitedCombatOptions::UCO_Guild_Health_IncHealth, EItemOption::EIO_Guild_Health_IncHealth, ESkillOption::ESO_End, EMercenarySkillOption::EMSO_End);
	//*/
}

// Probability of HP recover/absorption options below are fixed. The option values are recover/absorption amount.
FORCEINLINE float GetProbableRecoverHPProbability() { return 0.1f; }
FORCEINLINE float GetProbableHPAbsorptionProbability() { return 0.05f; }

/**
 * Defines single Option of an item. An item has multiple abilities.
 */
struct FItemOption
{
	FItemOption()
	{
		OptionType = EItemOption::EIO_End;
		OptionVariationType = EOptionVariation::EOV_Incerease;
		RawOptionAmount = 0.0f;
	}

	FItemOption(EItemOption InType, float InAmount)
	{
		OptionType = InType;
		OptionVariationType = EOptionVariation::EOV_Incerease;
		RawOptionAmount = InAmount;
	}

	FItemOption(EItemOption InType, EOptionVariation InVariationType, float InAmount)
	{
		OptionType = InType;
		OptionVariationType = InVariationType;
		RawOptionAmount = InAmount;
	}

	EItemOption OptionType;

	/** Default 蔼篮  EOptionVariation::EOV_Incerease 捞哥, 瓷仿摹啊 皑家窍绰 漂喊茄 惑炔俊辑父 立辟茄促. */
	EOptionVariation OptionVariationType;

	/** How much effective the option will be. As its name implies, this could be the nominal value for display,
	 * then the final value to be applied could be a little different, especially in the case of decreasing option.
	 * See GetEffectiveItemOptionValue for getting the final effective value of these. */
	float RawOptionAmount;
};

struct FSealOption
{
	FSealOption()
	{
		SealSlotNumber = 0;
		IsOpened = false;
	}
	FSealOption(int32 IntSlotNumber, bool InIsOpened, EItemOption InOptionType, float InOptionAmount)
	{
		SealSlotNumber = IntSlotNumber;
		IsOpened = InIsOpened;
		OptionInfo.OptionType = InOptionType;
		OptionInfo.RawOptionAmount = InOptionAmount;
	}

	int32 SealSlotNumber;
	bool IsOpened;
	FItemOption OptionInfo;
};

/**
* 捞 可记篮 皑家 瓷仿摹 利侩阑 困秦 眠啊登菌栏哥, 扁粮俊 瓷仿摹甸篮 钦魂阑 TArray(float) 甫 啊瘤绊 鉴瞒利栏肺 钦魂秦啊哥 拌魂沁栏唱
* 皑家秦具 且 版快俊绰 馆措肺 拌魂秦林扁 拌魂侩栏肺 眠啊
*/
struct FOptionValue
{
	FOptionValue()
	{
		OptionVariationType = EOptionVariation::EOV_Incerease;
		RawOptionAmount = 0.0f;
	}

	FOptionValue(float InAmount)
	{
		OptionVariationType = EOptionVariation::EOV_Incerease;
		RawOptionAmount = InAmount;
	}

	FOptionValue(EOptionVariation InVariationType, float InAmount)
	{
		OptionVariationType = InVariationType;
		RawOptionAmount = InAmount;
	}

	EOptionVariation OptionVariationType;
	float RawOptionAmount;
};

#define CASE_ITEMOPTION(itemOption) \
	case b2network::B2ItemOption::BIO_##itemOption: \
		return EItemOption::EIO_##itemOption;

#define CASE_ITEMOPTION_REVERSE(itemOption) \
	case EItemOption::EIO_##itemOption: \
		return b2network::B2ItemOption::BIO_##itemOption;

inline EItemOption SvrToCliOptionType(int32 ServerOptionID)
{
	switch (ServerOptionID)
	{
		CASE_ITEMOPTION(Offense_IncAttack)
			CASE_ITEMOPTION(Offense_IncDamage)
			CASE_ITEMOPTION(Offense_IncNormalDamage)
			CASE_ITEMOPTION(Offense_IncCriticalDamage)
			CASE_ITEMOPTION(Offense_IncCriticalRate)
			CASE_ITEMOPTION(Offense_IncBossDamage)
			CASE_ITEMOPTION(Offense_IncAttackSpeed)
			CASE_ITEMOPTION(Offense_IncAdditionalDamage)

			CASE_ITEMOPTION(Skill_IncSkillDamage)
			CASE_ITEMOPTION(Skill_DecSkillCooltime)
			CASE_ITEMOPTION(Skill_IncSkill1Damage)
			CASE_ITEMOPTION(Skill_IncSkill2Damage)
			CASE_ITEMOPTION(Skill_IncSkill3Damage)
			CASE_ITEMOPTION(Skill_IncSkill5Damage)

			CASE_ITEMOPTION(Health_ProbableHPAbsorption)
			CASE_ITEMOPTION(Health_ProbableRecoverHP)
			CASE_ITEMOPTION(Health_IncHealth)
			CASE_ITEMOPTION(Health_PerSecRecoverHP)

			CASE_ITEMOPTION(Defense_IncDefense)
			CASE_ITEMOPTION(Defense_DecAbnormalStateTime)
			CASE_ITEMOPTION(Defense_DecDamage)
			CASE_ITEMOPTION(Defense_DecBossDamage)
			CASE_ITEMOPTION(Defense_DecMeleeDamage)
			CASE_ITEMOPTION(Defense_DecRangedDamage)
			CASE_ITEMOPTION(Defense_ResistCritical)
			CASE_ITEMOPTION(Defense_DecAdditionalDamage)

			CASE_ITEMOPTION(Misc_LastNormalAttackKnockbackRate)
			CASE_ITEMOPTION(Misc_AdditionalExp)
			CASE_ITEMOPTION(Misc_AdditionalGold)
			CASE_ITEMOPTION(Misc_IncMoveSpeed)
			CASE_ITEMOPTION(Misc_DecTagCooltime)

			CASE_ITEMOPTION(Tag_Offense_IncAttack)
			CASE_ITEMOPTION(Tag_Defense_IncDefense)
			CASE_ITEMOPTION(Tag_Health_IncHealth)

			CASE_ITEMOPTION(HeroTower_Offense_IncAttack)
			CASE_ITEMOPTION(HeroTower_Defense_IncDefense)
			CASE_ITEMOPTION(HeroTower_Health_IncHealth)

			CASE_ITEMOPTION(Assault_Offense_IncAttack)
			CASE_ITEMOPTION(Assault_Defense_IncDefense)
			CASE_ITEMOPTION(Assault_Health_IncHealth)

			CASE_ITEMOPTION(Raid_Offense_IncAttack)
			CASE_ITEMOPTION(Raid_Defense_IncDefense)
			CASE_ITEMOPTION(Raid_Health_IncHealth)

			CASE_ITEMOPTION(TeamMatch_Offense_IncAttack)
			CASE_ITEMOPTION(TeamMatch_Defense_IncDefense)
			CASE_ITEMOPTION(TeamMatch_Health_IncHealth)

			CASE_ITEMOPTION(Guild_Offense_IncAttack)
			CASE_ITEMOPTION(Guild_Defense_IncDefense)
			CASE_ITEMOPTION(Guild_Health_IncHealth)

	default:
		return EItemOption::EIO_End;
	}
}

inline int32 CliToSvrOptionType(EItemOption ClientOptionID)
{
	switch (ClientOptionID)
	{
		CASE_ITEMOPTION_REVERSE(Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(Offense_IncDamage)
			CASE_ITEMOPTION_REVERSE(Offense_IncNormalDamage)
			CASE_ITEMOPTION_REVERSE(Offense_IncCriticalDamage)
			CASE_ITEMOPTION_REVERSE(Offense_IncCriticalRate)
			CASE_ITEMOPTION_REVERSE(Offense_IncBossDamage)
			CASE_ITEMOPTION_REVERSE(Offense_IncAttackSpeed)
			CASE_ITEMOPTION_REVERSE(Offense_IncAdditionalDamage)

			CASE_ITEMOPTION_REVERSE(Skill_IncSkillDamage)
			CASE_ITEMOPTION_REVERSE(Skill_DecSkillCooltime)
			CASE_ITEMOPTION_REVERSE(Skill_IncSkill1Damage)
			CASE_ITEMOPTION_REVERSE(Skill_IncSkill2Damage)
			CASE_ITEMOPTION_REVERSE(Skill_IncSkill3Damage)
			CASE_ITEMOPTION_REVERSE(Skill_IncSkill5Damage)

			CASE_ITEMOPTION_REVERSE(Health_ProbableHPAbsorption)
			CASE_ITEMOPTION_REVERSE(Health_ProbableRecoverHP)
			CASE_ITEMOPTION_REVERSE(Health_IncHealth)
			CASE_ITEMOPTION_REVERSE(Health_PerSecRecoverHP)

			CASE_ITEMOPTION_REVERSE(Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(Defense_DecAbnormalStateTime)
			CASE_ITEMOPTION_REVERSE(Defense_DecDamage)
			CASE_ITEMOPTION_REVERSE(Defense_DecBossDamage)
			CASE_ITEMOPTION_REVERSE(Defense_DecMeleeDamage)
			CASE_ITEMOPTION_REVERSE(Defense_DecRangedDamage)
			CASE_ITEMOPTION_REVERSE(Defense_ResistCritical)
			CASE_ITEMOPTION_REVERSE(Defense_DecAdditionalDamage)

			CASE_ITEMOPTION_REVERSE(Misc_LastNormalAttackKnockbackRate)
			CASE_ITEMOPTION_REVERSE(Misc_AdditionalExp)
			CASE_ITEMOPTION_REVERSE(Misc_AdditionalGold)
			CASE_ITEMOPTION_REVERSE(Misc_IncMoveSpeed)
			CASE_ITEMOPTION_REVERSE(Misc_DecTagCooltime)

			CASE_ITEMOPTION_REVERSE(Tag_Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(Tag_Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(Tag_Health_IncHealth)

			CASE_ITEMOPTION_REVERSE(HeroTower_Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(HeroTower_Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(HeroTower_Health_IncHealth)

			CASE_ITEMOPTION_REVERSE(Assault_Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(Assault_Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(Assault_Health_IncHealth)

			CASE_ITEMOPTION_REVERSE(Raid_Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(Raid_Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(Raid_Health_IncHealth)

			CASE_ITEMOPTION_REVERSE(TeamMatch_Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(TeamMatch_Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(TeamMatch_Health_IncHealth)

			CASE_ITEMOPTION_REVERSE(Guild_Offense_IncAttack)
			CASE_ITEMOPTION_REVERSE(Guild_Defense_IncDefense)
			CASE_ITEMOPTION_REVERSE(Guild_Health_IncHealth)

	default:
		return 0;
	}
}

//================================================================================
// BEGIN combat option common functionalities for item, skill, wing, and such

// Commonly used for applying option values across various combat system, equipment, skill, and wing, etc.
enum class ECombatOptionApplyType : uint8
{
	// See GetCombatOptionApplyType for each option type enum

	ECOAT_Add_Value, // For increasing options, mainly for primary Attack,Defense,Health options
	ECOAT_Add_Percent, // Most general increasing options
	ECOAT_Subtract_Value, // For simple decreasing. Better be careful to not make negative value as the result of subtracting.
	ECOAT_OneMinusMultiply // For general decreasing options. (1 - RawAmountRatio) * (1 - RawAmountRatio) * ... 
};

/** Actual CombatOptionApplyType classification */
FORCEINLINE ECombatOptionApplyType GetCombatOptionApplyType(EItemOption InOptionType)
{
	switch (InOptionType)
	{
	case EItemOption::EIO_Defense_DecAbnormalStateTime:
	case EItemOption::EIO_Defense_DecDamage:
	case EItemOption::EIO_Defense_DecBossDamage:
	case EItemOption::EIO_Defense_DecMeleeDamage:
	case EItemOption::EIO_Defense_DecRangedDamage:
	case EItemOption::EIO_Defense_ResistCritical:
		return ECombatOptionApplyType::ECOAT_OneMinusMultiply;
	case EItemOption::EIO_Skill_DecSkillCooltime: // It might be OneMinusMultiply..
	case EItemOption::EIO_Misc_DecTagCooltime: // It might be OneMinusMultiply..
		return ECombatOptionApplyType::ECOAT_Subtract_Value;
	case EItemOption::EIO_Offense_IncAttack:
	case EItemOption::EIO_Defense_IncDefense:
	case EItemOption::EIO_Health_IncHealth:
	case EItemOption::EIO_Health_PerSecRecoverHP:
	case EItemOption::EIO_Offense_IncAdditionalDamage:
	case EItemOption::EIO_Defense_DecAdditionalDamage:
		return ECombatOptionApplyType::ECOAT_Add_Value;
	}
	return ECombatOptionApplyType::ECOAT_Add_Percent;
}
FORCEINLINE ECombatOptionApplyType GetCombatOptionApplyType(ESkillOption InOptionType)
{
	switch (InOptionType)
	{
	case ESkillOption::ESO_DecUnderAttackDamage:
	case ESkillOption::ESO_DecReceiveDamageOnBuff:
	case ESkillOption::ESO_DecUnderAttackMeleeDamage:
	case ESkillOption::ESO_DecUnderAttackRangeDamage:
	case ESkillOption::ESO_DecReceiveDamageByGladiator:
	case ESkillOption::ESO_DecReceiveDamageByAssassin:
	case ESkillOption::ESO_DecReceiveDamageByWizard:
	case ESkillOption::ESO_DecReceiveDamageByFighter:
	case ESkillOption::ESO_DecReceiveDamage:
	case ESkillOption::ESO_IncResistAbnormal: // An odd case. Not really "increasing" some value, but "decreasing" abnormal state time.
		return ECombatOptionApplyType::ECOAT_OneMinusMultiply;
	case ESkillOption::ESO_DecCoolTime: // It might be OneMinusMultiply..
	case ESkillOption::ESO_DecCoolTimeOtherSkills: // It might be OneMinusMultiply..
		return ECombatOptionApplyType::ECOAT_Subtract_Value;
		// Currently no Attack/Defense/Health simple Add options with ESkillOption, if so, put them as Add_Value.
	}
	return ECombatOptionApplyType::ECOAT_Add_Percent;
}

FORCEINLINE ECombatOptionApplyType GetCombatOptionApplyType(EMercenarySkillOption InOptionType)
{
	switch (InOptionType)
	{
	case EMercenarySkillOption::EMSO_DecUnderAttackDamage:
	case EMercenarySkillOption::EMSO_DecReceiveDamageOnBuff:
	case EMercenarySkillOption::EMSO_DecUnderAttackMeleeDamage:
	case EMercenarySkillOption::EMSO_DecUnderAttackRangeDamage:
	case EMercenarySkillOption::EMSO_DecReceiveDamageByGladiator:
	case EMercenarySkillOption::EMSO_DecReceiveDamageByAssassin:
	case EMercenarySkillOption::EMSO_DecReceiveDamageByWizard:
	case EMercenarySkillOption::EMSO_DecReceiveDamageByFighter:
	case EMercenarySkillOption::EMSO_DecReceiveDamage:
	case EMercenarySkillOption::EMSO_IncResistAbnormal: // An odd case. Not really "increasing" some value, but "decreasing" abnormal state time.
		return ECombatOptionApplyType::ECOAT_OneMinusMultiply;
	case EMercenarySkillOption::EMSO_DecCoolTime: // It might be OneMinusMultiply..
	case EMercenarySkillOption::EMSO_DecCoolTimeOtherSkills: // It might be OneMinusMultiply..
		return ECombatOptionApplyType::ECOAT_Subtract_Value;
		// Currently no Attack/Defense/Health simple Add options with ESkillOption, if so, put them as Add_Value.
	}
	return ECombatOptionApplyType::ECOAT_Add_Percent;
}


/** InRawValue is supposed to be in percent range (0 ~ 100), like displayed value. */
FORCEINLINE float GetEffectiveValueForOptionApplyType(ECombatOptionApplyType InOptionApplyType, FOptionValue InRawValue)
{
	if (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply)
	{
		// In the case of OneMinusMultiply, the return value should be applied by multiplication
		if (InRawValue.OptionVariationType == EOptionVariation::EOV_Incerease)
		{
			return (1.0f - FMath::Clamp(InRawValue.RawOptionAmount * 0.01f, 0.0f, 1.0f));
		}
		else if (InRawValue.OptionVariationType == EOptionVariation::EOV_Decrease)
		{
			return (1.0f + FMath::Clamp(InRawValue.RawOptionAmount * 0.01f, 0.0f, 1.0f));
		}
	}
	else if (InOptionApplyType == ECombatOptionApplyType::ECOAT_Add_Percent)
	{
		// In the case of Add, the return value should be applied by addition.
		return (InRawValue.RawOptionAmount * 0.01f);
	}

	return InRawValue.RawOptionAmount;

	// For OneMinusMultiply and Add_Percent type, RawValue is scaled by 0.01,
	// but if the final code consider the value in % scale, it needs to be multiplied by 100. It depends on the final implementation, case by case..

	// Might consider scaling -1.0 for effective value of Subtract_Value type.. but current implementation finally subtracting the result value, so let it be positive at this stage.
}

FORCEINLINE float GetConvertNotRawValueForOptionApplyType(ECombatOptionApplyType InOptionApplyType, float InRawValue)
{
	if (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply)
	{
		return ((100.0f - FMath::Clamp(InRawValue * 100.0f, 0.0f, 100.0f)));
	}
	else if (InOptionApplyType == ECombatOptionApplyType::ECOAT_Add_Percent)
	{
		return (InRawValue * 100.0f);
	}
	else
	{
		return InRawValue;
	}
}

FORCEINLINE float GetInitialValueOfCombatOptionApplyType(ECombatOptionApplyType InOptionApplyType)
{// Initial value should be different depend on whether it is add or (1 minus) multiplication.
	return (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply) ? 1.0f : 0.0f;
}

/** Core of GetEffectiveOptionValueTempl - The final core
 * The purpose of this function is to provides proper calculation by ECombatOptionApplyType */
float GetEffectiveOptionValueInternal(ECombatOptionApplyType InOptionApplyType,
	TArray<FOptionValue>& InSourceValues, // Raw (displayed) values if bRawValueSource is true, effective scaled values if false
	bool bRawValueSource = true,// Designate whether InSourceValues are raw values or effective scaled values.
	bool bClampValue = false // 蔼俊 弥措 弥家蔼 焊沥捞 甸绢埃促.
);


/** Get scaled value appropriate to apply for each OptionType
 * from gathered values (expected in range of 0 ~ 100 if they are specified as raw) of InOptionType */
template<typename TOptionTypeEnum>
float GetEffectiveOptionValueTempl(TOptionTypeEnum InOptionType,
	TArray<FOptionValue>& InSourceValues, // Raw (displayed) values if bRawValueSource is true, effective scaled values if false
	bool bRawValueSource = true) // Designate whether InSourceValues are raw values or effective scaled values.
{
	return GetEffectiveOptionValueInternal(GetCombatOptionApplyType(InOptionType), InSourceValues, bRawValueSource);
}
/** Take enhance set effect into account, which is not better be a part of GetEffectiveOptionValueTempl or GetEffectiveOptionValueInternal */
template<typename TOptionTypeEnum>
float GetEffectiveOptionValueWithEnhanceSetEffectTempl(TOptionTypeEnum InOptionType,
	TArray<FOptionValue>& InSourceValues, // Raw (displayed) values if bRawValueSource is true, effective scaled values if false
	double InEnhanceSetEffectFactor,
	bool bRawValueSource = true) // Designate whether InSourceValues are raw values or effective scaled values.
{
	float TotalEffectiveValue = GetEffectiveOptionValueTempl<TOptionTypeEnum>(InOptionType, InSourceValues, bRawValueSource);
	// Note : Enhance item set effect is applied by multiplication "at the last".
	if (TotalEffectiveValue != 0.0f && InEnhanceSetEffectFactor != 0.0)
		TotalEffectiveValue += (TotalEffectiveValue * GetEffectiveValueForOptionApplyType(ECombatOptionApplyType::ECOAT_Add_Percent, InEnhanceSetEffectFactor));

	return TotalEffectiveValue;
}

/** A little wrapping on GetEffectiveOptionValueTempl for ESkillOption */
FORCEINLINE float GetEffectivePassiveSkillOptionValue(ESkillOption InOptionType, TArray<FOptionValue>& InRawValues)
{ // Consider source values here are "raw" values not scaled..
	return GetEffectiveOptionValueTempl<ESkillOption>(InOptionType, InRawValues, true);
}
/** A little wrapping on GetEffectiveOptionValueTempl for ESkillOption */
FORCEINLINE float GetEffectiveItemOptionValue(EItemOption InOptionType, TArray<FOptionValue>& InRawValues)
{ // Consider source values here are "raw" values not scaled..
	return GetEffectiveOptionValueTempl<EItemOption>(InOptionType, InRawValues, true);
}
FORCEINLINE float GetEffectiveItemOptionValueWithEnhanceSetEffect(EItemOption InOptionType, TArray<FOptionValue>& InRawValues, const double InEnhanceEffectFactor)
{ // Consider source values here are "raw" values not scaled..
	return GetEffectiveOptionValueWithEnhanceSetEffectTempl<EItemOption>(InOptionType, InRawValues, InEnhanceEffectFactor, true);
}

// END combat option common functionalities 
//================================================================================







inline FString ERelicOptionToText(int32 option)
{
	//
	// No freaking idea what this SHIT is.. A different option system?
	//

	const int32 TagIncAttack = 101;
	const int32 TagIncDefense = 102;
	const int32 TagIncHP = 103;
	const int32 HeroTowerIncAttack = 201;
	const int32 HeroTowerIncDefense = 202;
	const int32 HeroTowerIncHP = 203;
	const int32 AssaultIncAttack = 301;
	const int32 AssaultIncDefense = 302;
	const int32 AssaultIncHP = 303;
	const int32 RaidIncAttack = 401;
	const int32 RaidIncDefense = 402;
	const int32 RaidIncHP = 403;
	const int32 TeamMatchIncAttack = 501;
	const int32 TeamMatchIncDefense = 502;
	const int32 TeamMatchIncHP = 503;
	const int32 GuildIncAttack = 601;
	const int32 GuildIncDefense = 602;
	const int32 GuildIncHP = 603;

	switch (option)
	{
	case TagIncAttack:
		return TEXT("TagIncAttack");
		break;
	case TagIncDefense:
		return TEXT("TagIncDefense");
		break;
	case TagIncHP:
		return TEXT("TagIncHP");
		break;
	case HeroTowerIncAttack:
		return TEXT("HeroTowerIncAttack");
		break;
	case HeroTowerIncDefense:
		return TEXT("HeroTowerIncDefense");
		break;
	case HeroTowerIncHP:
		return TEXT("HeroTowerIncHP");
		break;
	case AssaultIncAttack:
		return TEXT("AssaultIncAttack");
		break;
	case AssaultIncDefense:
		return TEXT("AssaultIncDefense");
		break;
	case AssaultIncHP:
		return TEXT("AssaultIncHP");
		break;
	case RaidIncAttack:
		return TEXT("RaidIncAttack");
		break;
	case RaidIncDefense:
		return TEXT("RaidIncDefense");
		break;
	case RaidIncHP:
		return TEXT("RaidIncHP");
		break;
	case TeamMatchIncAttack:
		return TEXT("TeamMatchIncAttack");
		break;
	case TeamMatchIncDefense:
		return TEXT("TeamMatchIncDefense");
		break;
	case TeamMatchIncHP:
		return TEXT("TeamMatchIncHP");
		break;
	case GuildIncAttack:
		return TEXT("GuildIncAttack");
		break;
	case GuildIncDefense:
		return TEXT("GuildIncDefense");
		break;
	case GuildIncHP:
		return TEXT("GuildIncHP");
		break;
	}
	return TEXT("");
}
