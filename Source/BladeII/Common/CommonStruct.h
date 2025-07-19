#pragma once


#include "B2MessageInfo.h"
//#include "TypeList.h"

#include "CommonStructCombatOption.h" // A part of CommonStruct.h
#include <variant>

UENUM(BlueprintType)
enum class EPCClass : uint8
{
	// List all the PC classes based on ABladeIIPlayer here.

	// Change the naming convention here can result failure of CheckPCClassEnumNamingConvention. Not really impossible, but requires much caution.

	EPC_Gladiator = 0,
	EPC_Assassin,
	EPC_Wizard,
	EPC_Fighter,

	// Check all places we've put DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS for a new character type, if ever...

	EPC_End
};

/** Use GetMaxPCClassNum for casting EPCClass::EPC_End for class max number */
FORCEINLINE int32 PCClassToInt(EPCClass InPCClassEnum) {
	return static_cast<int32>(InPCClassEnum);
}
FORCEINLINE EPCClass IntToPCClass(int32 InInt) {
	return static_cast<EPCClass>(InInt);
}
/** It is more important to use GetMaxPCClassNum than PCClassToInt or IntToPCClass.. */
FORCEINLINE int32 GetMaxPCClassNum() {
	return static_cast<int32>(EPCClass::EPC_End);
}
/** Supposed to be the same to GetMaxPCClassNum, provided for statically sized array. */
#define PCCLASS_ARRAY_SIZE (static_cast<int32>(EPCClass::EPC_End))

#define EPCCLASS_ENUM_PACKAGE_NAME TEXT("EPCClass")
#define EPCCLASSENUM_COMMON_PREFIX TEXT("EPC_")

/** For putting assert in any place that need manual handling of new player character, if ever.
* I suppose that it is enough to check for editor only. */
inline void DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS()
{
#if WITH_EDITOR
	check(PCClassToInt(EPCClass::EPC_Gladiator) == 0);
	check(PCClassToInt(EPCClass::EPC_Fighter) + 1 == GetMaxPCClassNum());

	check(b2network::B2CharacterType::Gladiator == 1);
	check(b2network::B2CharacterType::Fighter + 1 == b2network::B2CharacterType::None);
#endif
}

inline int32 CliToSvrPCClassType(EPCClass ClientType)
{
	switch (ClientType)
	{
	case EPCClass::EPC_Gladiator:	return b2network::B2CharacterType::Gladiator;
	case EPCClass::EPC_Assassin:	return b2network::B2CharacterType::Assassin;
	case EPCClass::EPC_Wizard:		return b2network::B2CharacterType::Wizard;
	case EPCClass::EPC_Fighter:		return b2network::B2CharacterType::Fighter;
	}
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	return b2network::B2CharacterType::None;
}

inline EPCClass SvrToCliPCClassType(int32 InServerType)
{
	switch (InServerType)
	{
	case b2network::B2CharacterType::Gladiator:		return EPCClass::EPC_Gladiator;		break;
	case b2network::B2CharacterType::Assassin:		return EPCClass::EPC_Assassin;		break;
	case b2network::B2CharacterType::Wizard:		return EPCClass::EPC_Wizard;		break;
	case b2network::B2CharacterType::Fighter:		return EPCClass::EPC_Fighter;		break;
	}
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	return EPCClass::EPC_End;
}

//================================================================================
// BEGIN definitions for ITEM and INVENTORY (except EItemOption)

/**
* The primary classification of an item.
*/
UENUM(BlueprintType)
enum class EItemClass : uint8
{
	EIC_Weapon,

	// Protections..
	EIC_Helmet,
	EIC_BodyArmor,
	EIC_Glove,
	EIC_Shoes,

	// Accessories..
	EIC_Belt,
	EIC_Necklace,
	EIC_Ring,

	// Costumes
	EIC_CostumeWeapon,
	EIC_CostumeHat,
	EIC_CostumeArmor,

	EIC_Wing,

	EIC_SetItem,

	// Others.. (consumables)

	EIC_Ingredient, // We don't use the term "material" here, as the term material is already being recognized in different meaning in client.

	EIC_GOODS, // Probably not seen in inventory

	EIC_UpgradeStone,
	EIC_EnhaceScroll,
	EIC_BlessedEnhanceScroll,
	EIC_ProtectionScroll,

	EIC_BossPiece,

	EIC_WeaponEssence,
	EIC_ProtectionEssence,
	EIC_AccessoryEssence,

	// 초월 재료
	EIC_WeaponSurpassAnvil,
	EIC_ArmorSurpassAnvil,
	EIC_AccessorySurpassAnvil,


	EIC_Gold, // Could be a special type of Goods..

	// To be added ...
	EIC_Ether,

	EIC_End
};

UENUM(BlueprintType)
enum class EEquipCategoryType : uint8
{
	mNULL = 0,
	WEAPON = 1,
	ARMOR = 2,
	ACCESSORY = 3,
	End
};

/** It is essentially set by ItemClass. */
UENUM(BlueprintType)
enum class EItemInvenType : uint8
{
	EIIVT_Weapon,
	EIIVT_Protection,
	EIIVT_Accessory,
	EIIVT_Consumables,
	EIIVT_United,

	EIIVT_CostumeWeapon,
	EIIVT_CostumeArmor,

	EIIVT_End
};

/** Almost resembles EItemClass,  */
UENUM(BlueprintType)
enum class EItemEquipPlace : uint8
{
	// Each has equipping slot in inventory.

	EIEP_Weapon = 0,

	// Protections
	EIEP_Helmet = 1,
	EIEP_BodyArmor = 2,
	EIEP_Glove = 3,
	EIEP_Shoes = 4,

	// Accessories
	EIEP_Belt = 5,
	EIEP_Necklace = 6,
	EIEP_Ring = 7,

	EIEP_End = 8 // Not an equipment, most likely consumables.
};

UENUM(BlueprintType)
enum class ECostumeEquipPlace : uint8
{
	ECEP_None = 0,

	ECEP_Weapon = 201,
	ECEP_Helmet = 202,
	ECEP_Body = 203,

	ECEP_End,
};

/** This is for some items that can be equipped at multiple sides like dual sword or shoes. */
enum class EItemEquipSide : uint8
{
	EIES_Left,
	EIES_Right,

	EIES_End // Probably means no side separation.
};

UENUM(BlueprintType)
enum class EItemPrimaryPointType : uint8
{
	EIPP_Attack,
	EIPP_Defense,
	EIPP_Health,

	// Some special primary point can be added..
	EIPP_End // No primary point available, for accessories..
};

inline EItemOption ItemPrimaryPointTypeToEItemOptionType(EItemPrimaryPointType PrimaryPointType)
{
	switch (PrimaryPointType)
	{
	case EItemPrimaryPointType::EIPP_Attack: return EItemOption::EIO_Offense_IncAttack;
	case EItemPrimaryPointType::EIPP_Defense: return EItemOption::EIO_Defense_IncDefense;
	case EItemPrimaryPointType::EIPP_Health: return EItemOption::EIO_Health_IncHealth;
	}
	return EItemOption::EIO_End;
}

UENUM(BlueprintType)
enum class ECharacterMaterialOverrideType : uint8
{
	ECMO_None = 0,
	ECMO_Ice,
	ECMO_Fire,
	ECMO_Poison,
	ECMO_Hide,
	EDE_End
};

UENUM(BlueprintType)
enum class ECharStatApplyType : uint8
{
	ECSA_NormalPoint,				// 기본
	ECSA_IntrinsicOptions,			// 고유 옵션
	ECSA_RandomOptions,			// 랜덤 옵션
	ECSA_SealOption,				//각성옵션
	ECSA_WingNormalOption,			// 날개 기본
	ECSA_WingAdditionalOption,		// 날개 추가 옵션
	ECSA_RankPromotion,			// 진급
	ECSA_SkillOption,				// 스킬옵션
	ECSA_ItemSetEffect,			// 아이템 세트 효과	(%)
	ECSA_EnhanceSetEffect,			// 강화 세트 효과	(%)
	ECSA_RelicOption,			// 고대유물 옵션
	ECSA_EtherOption,			// 에테르 옵션
	ECSA_GuildOption,			// 길드 옵션(버프) (%)
	ECSA_FairyOption,			// 페어리 옵션
	ECSA_TotemSmeltOptions,		// 제련 보너스
	ECSA_End
};

UENUM(BlueprintType)
enum class EItemOptionApplyType : uint8
{
	EIOAT_Item,
	EIOAT_Skill,
	EIOAT_End,
};

enum class EInventoryPresetMode : uint8
{
	EIPM_None,
	EIPM_Equipment,
	EIPM_Skill,

	EIPM_End
};

UENUM(BlueprintType)
enum class EItemPresetSlotType : uint8
{
	EIPS_First,
	EIPS_Second,
	EIPS_Third,

	EIPS_End
};

enum class EHallOfFameMode : int32
{
	None = -1,
	PVP = 2,
	TEAM_MATCH,
	CONTROL,
	Max,
};

UENUM(BlueprintType)
enum class EWEALTH_TYPE : uint8
{
	GEM,
	DARK_CRYSTAL,
	GOLD,
	SOCIAL,
	BLADE_POINT,
	REST_TIME,
	STAGE_BOOST_TICKET,
	PVP1ON1_TICKET,
	TEAM_MATCH_TICKET,
	HERO_TOWER_TICKET,
	FAME_POINT,
	STAGE_AUTO_CLEAR_TICKET
};

enum class EGuildTeamState : uint8
{
	Waiting = 0,
	Start,
	End,
	Max,
};

enum class ECharacterPowerSpecific : uint8	// = Server : static class Consts
{
	TOTAL_POWER = 0,

	STAT_POWER = 1,
	ITEM_POWER = 2,
	COSTUME_POWER = 3,
	SET_ITEM_POWER = 4,
	RANK_POWER = 5,
	WING_POWER = 6,
	RELIC_POWER = 7,
	SKILL_POWER = 8,
	UNITY_SKILL_POWER = 9,
	AETHER_POWER = 10,
	FAIRY_POWER = 11,
	TOTEM_POWER = 12,
	SET_COUSTUME_POWER = 13,

	Max,
};

namespace AdventureNewChapterNumber
{
	const int32 Act1_5 = 0;
	const int32 Act6 = 6;
	const int32 Act7 = 7;
}

inline int32 CliToSvrPresetID(EItemPresetSlotType PresetID)
{
	return static_cast<int32>(PresetID);
}

inline EItemPresetSlotType SvrToCliPresetID(int32 SvrPresetID)
{
	return static_cast<EItemPresetSlotType>(SvrPresetID);
}


#define CASE_ITEMPRIMETYPE(itemPrime) case b2network::B2ItemPrimeType::BIPT_##itemPrime: \
	return EItemPrimaryPointType::EIPP_##itemPrime

inline EItemPrimaryPointType SvrToCliPrimPointType(int32 ServerPointID)
{
	switch (ServerPointID)
	{
		CASE_ITEMPRIMETYPE(Attack);
		CASE_ITEMPRIMETYPE(Defense);
		CASE_ITEMPRIMETYPE(Health);

	default:
		return EItemPrimaryPointType::EIPP_End;
	}
}

inline EItemClass SvrToCliItemClass(int32 ServerItemType)
{
	switch (ServerItemType)
	{
	case b2network::B2ItemType::Weapon:			return EItemClass::EIC_Weapon;
	case b2network::B2ItemType::Helmet:			return EItemClass::EIC_Helmet;
	case b2network::B2ItemType::BodyArmor:		return EItemClass::EIC_BodyArmor;
	case b2network::B2ItemType::Glove:			return EItemClass::EIC_Glove;
	case b2network::B2ItemType::Shoes:			return EItemClass::EIC_Shoes;
	case b2network::B2ItemType::Belt:			return EItemClass::EIC_Belt;
	case b2network::B2ItemType::Necklace:		return EItemClass::EIC_Necklace;
	case b2network::B2ItemType::Ring:			return EItemClass::EIC_Ring;
	case b2network::B2ItemType::Material:		return EItemClass::EIC_Ingredient; // "Material" is not proper term in client, as it is already being used widely in different meaning.
	case b2network::B2ItemType::GOODS:			return EItemClass::EIC_GOODS;
	case b2network::B2ItemType::UpgradeStone:		return EItemClass::EIC_UpgradeStone;
	case b2network::B2ItemType::EnhanceScroll:		return EItemClass::EIC_EnhaceScroll;
	case b2network::B2ItemType::BlessedEnhanceScroll:	return EItemClass::EIC_BlessedEnhanceScroll;
	case b2network::B2ItemType::ProtectionScroll:		return EItemClass::EIC_ProtectionScroll;
	case b2network::B2ItemType::BossPiece:		return EItemClass::EIC_BossPiece;
	case b2network::B2ItemType::WeaponEssence:	return EItemClass::EIC_WeaponEssence;
	case b2network::B2ItemType::ArmorEssence:	return EItemClass::EIC_ProtectionEssence;
	case b2network::B2ItemType::AccessoryEssence:	return EItemClass::EIC_AccessoryEssence;
	case b2network::B2ItemType::WeaponSurpassAnvil:	return EItemClass::EIC_WeaponSurpassAnvil;
	case b2network::B2ItemType::ArmorSurpassAnvil:	return EItemClass::EIC_ArmorSurpassAnvil;
	case b2network::B2ItemType::AccessorySurpassAnvil:	return EItemClass::EIC_AccessorySurpassAnvil;
	case b2network::B2ItemType::Gold:			return EItemClass::EIC_Gold;
		//case b2network::B2ItemType::Wing:
		//case b2network::B2ItemType::UpgradeStone:
	case b2network::B2CostumeType::CostumeWeapon:	return EItemClass::EIC_CostumeWeapon;
	case b2network::B2CostumeType::CostumeHat:		return EItemClass::EIC_CostumeHat;
	case b2network::B2CostumeType::CostumeArmor:	return EItemClass::EIC_CostumeArmor;
	}

	return EItemClass::EIC_End;
}

inline EEquipCategoryType SvrToCliEquipCategoryType(int32 ServerInvenType)
{
	switch (ServerInvenType)
	{
	case b2network::B2EquipCategory::WEAPON:		return EEquipCategoryType::WEAPON;
	case b2network::B2EquipCategory::ARMOR:			return EEquipCategoryType::ARMOR;
	case b2network::B2EquipCategory::ACCESSORY:		return EEquipCategoryType::ACCESSORY;
	}

	return EEquipCategoryType::End;
}

inline int32 CliToSvrEquipCategoryType(EEquipCategoryType CliInvenType)
{
	switch (CliInvenType)
	{
	case EEquipCategoryType::WEAPON:		return b2network::B2EquipCategory::WEAPON;
	case EEquipCategoryType::ARMOR:			return b2network::B2EquipCategory::ARMOR;
	case EEquipCategoryType::ACCESSORY:		return b2network::B2EquipCategory::ACCESSORY;
	}

	return INDEX_NONE;
}

inline EItemInvenType SvrToCliItemInvenType(int32 ServerInvenType)
{
	switch (ServerInvenType)
	{
	case b2network::B2InventoryType::Weapon:		return EItemInvenType::EIIVT_Weapon;
	case b2network::B2InventoryType::Protection:	return EItemInvenType::EIIVT_Protection;
	case b2network::B2InventoryType::Accessory:		return EItemInvenType::EIIVT_Accessory;
	case b2network::B2InventoryType::Consumables:	return EItemInvenType::EIIVT_Consumables;
		//case b2network::B2InventoryType::Wing:		..
	case b2network::B2InventoryType::CostumeWeapon: return EItemInvenType::EIIVT_CostumeWeapon;
	case b2network::B2InventoryType::CostumeArmor:	return EItemInvenType::EIIVT_CostumeArmor;
	}

	return EItemInvenType::EIIVT_End;
}

inline int32 CliToSvrItemInvenType(EItemInvenType CliInvenType)
{
	switch (CliInvenType)
	{
	case EItemInvenType::EIIVT_Weapon:				return b2network::B2InventoryType::Weapon;
	case EItemInvenType::EIIVT_Protection:			return b2network::B2InventoryType::Protection;
	case EItemInvenType::EIIVT_Accessory:			return b2network::B2InventoryType::Accessory;
	case EItemInvenType::EIIVT_Consumables:			return b2network::B2InventoryType::Consumables;
		//.. b2network::B2InventoryType::Wing:		..
	case EItemInvenType::EIIVT_CostumeWeapon:		return b2network::B2InventoryType::CostumeWeapon;
	case EItemInvenType::EIIVT_CostumeArmor:		return b2network::B2InventoryType::CostumeArmor;
	}

	return INDEX_NONE;
}

inline ECostumeEquipPlace SvrToCliCostumeEquipPlace(int32 ServerEquipPos)
{
	switch (ServerEquipPos)
	{
	case b2network::B2EquipPosition::CostumeWeapon:		return ECostumeEquipPlace::ECEP_Weapon;
	case b2network::B2EquipPosition::CostumeHat:		return ECostumeEquipPlace::ECEP_Helmet;
	case b2network::B2EquipPosition::CostumeArmor:		return ECostumeEquipPlace::ECEP_Body;
	}

	return ECostumeEquipPlace::ECEP_None;
}

inline int32 CliToSvrItemEquipPlace(ECostumeEquipPlace ClientEquipPos)
{
	switch (ClientEquipPos)
	{
	case ECostumeEquipPlace::ECEP_Weapon:	return b2network::B2EquipPosition::CostumeWeapon;
	case ECostumeEquipPlace::ECEP_Helmet:	return b2network::B2EquipPosition::CostumeHat;
	case ECostumeEquipPlace::ECEP_Body:		return b2network::B2EquipPosition::CostumeArmor;
	}

	return b2network::B2EquipPosition::None;
}

inline EItemEquipPlace SvrToCliItemEquipPlace(int32 ServerEquipPos)
{
	switch (ServerEquipPos)
	{
	case b2network::B2EquipPosition::Weapon:		return EItemEquipPlace::EIEP_Weapon;
	case b2network::B2EquipPosition::Helmet:		return EItemEquipPlace::EIEP_Helmet;
	case b2network::B2EquipPosition::BodyArmor:		return EItemEquipPlace::EIEP_BodyArmor;
	case b2network::B2EquipPosition::Glove:			return EItemEquipPlace::EIEP_Glove;
	case b2network::B2EquipPosition::Shoes:			return EItemEquipPlace::EIEP_Shoes;
	case b2network::B2EquipPosition::Belt:			return EItemEquipPlace::EIEP_Belt;
	case b2network::B2EquipPosition::Necklace:		return EItemEquipPlace::EIEP_Necklace;
	case b2network::B2EquipPosition::Ring:			return EItemEquipPlace::EIEP_Ring;
		//case b2network::B2EquipPosition::Wing:
	}

	return EItemEquipPlace::EIEP_End;
}

inline int32 CliToSvrItemEquipPlace(EItemEquipPlace ClientEquipPos)
{
	switch (ClientEquipPos)
	{
	case EItemEquipPlace::EIEP_Weapon:			return b2network::B2EquipPosition::Weapon;
	case EItemEquipPlace::EIEP_Helmet:			return b2network::B2EquipPosition::Helmet;
	case EItemEquipPlace::EIEP_BodyArmor:		return b2network::B2EquipPosition::BodyArmor;
	case EItemEquipPlace::EIEP_Glove:			return b2network::B2EquipPosition::Glove;
	case EItemEquipPlace::EIEP_Shoes:			return b2network::B2EquipPosition::Shoes;
	case EItemEquipPlace::EIEP_Belt:			return b2network::B2EquipPosition::Belt;
	case EItemEquipPlace::EIEP_Necklace:		return b2network::B2EquipPosition::Necklace;
	case EItemEquipPlace::EIEP_Ring:			return b2network::B2EquipPosition::Ring;
		//case b2network::B2EquipPosition::Wing:
	}

	return b2network::B2EquipPosition::Wing;
}

UENUM(BlueprintType)
enum class EFairyType : uint8
{
	FAIRY_NONE,

	FAIRY_NIECE,
	FAIRY_PANG,
	FAIRY_RIO,

	FAIRY_MAX
};

inline EFairyType SvrToCliFairyType(const int32 FairyType)
{
	switch (FairyType)
	{
	case b2network::B2FairyType::NIECE:		return EFairyType::FAIRY_NIECE;
	case b2network::B2FairyType::PANG:		return EFairyType::FAIRY_PANG;
	case b2network::B2FairyType::RIO:		return EFairyType::FAIRY_RIO;
	default:								return EFairyType::FAIRY_NONE;
	}

	return EFairyType::FAIRY_NONE;
}
inline int32 CliToSvrFairyType(const EFairyType FairyType)
{
	switch (FairyType)
	{
	case EFairyType::FAIRY_NIECE:		return b2network::B2FairyType::NIECE;
	case EFairyType::FAIRY_PANG:		return b2network::B2FairyType::PANG;
	case EFairyType::FAIRY_RIO:			return b2network::B2FairyType::RIO;
	case EFairyType::FAIRY_MAX:			return b2network::B2FairyType::RIO + 1;
	default:							return 0;
	}
}

UENUM(BlueprintType)
enum class ETotemGrade : uint8
{
	TG_NORMAL = 0,
	TG_MAGIC,
	TG_RARE,
	TG_HERO,
	TG_LEGEND,
	TG_RELIC,

	TG_MAX
};

UENUM(BlueprintType)
enum class ETotemBasePrefix : uint8
{
	TBP_VALOROUS = 0,
	TBP_PATRON,
	TBP_VIGOROUS,

	TBP_MAX
};

UENUM(BlueprintType)
enum class ETotemRefinePrefix : uint8
{
	TRP_ECLIPSE = 0,
	TRP_SHINING,
	TRP_STRONG,
	TRP_SACRED,

	TRP_MAX
};

UENUM()
enum class ETotemRefineCostType : uint8
{
	TRC_GOLD,
	TRC_DIMENTIONAL_PIECE,

	TRC_MAX,
};

UENUM()
enum class EItemForgeType : uint8
{
	Forge,	// Note: This value has been named as "MAKING" from server table.
	Upgrade
};

UENUM()
enum class EItemForgeLimitType : uint8
{
	NumberOfTry,
	NumberOfSuccess,

	None
};

UENUM()
enum class EItemForgePeriodLimitType : uint8
{
	Once,
	Day,
	Week,
	Month,
	None
};

UENUM()
enum class EItemForgeTabType : uint8
{
	Weapon,
	Armor,
	Accessory,

	//add extra here
	Event
};

enum class EItemForgeListVisibleType : uint8
{
	Enabled,
	Disabled,
	Limited,
};

inline ETotemRefineCostType SvrToCliRefineCostType(uint8 ServerTotemRefineCostType)
{
	switch (ServerTotemRefineCostType)
	{
	case b2network::B2TotemRefineCostType::GOLD:				return ETotemRefineCostType::TRC_GOLD;
	case b2network::B2TotemRefineCostType::DIMENSIONAL_PIECE:	return ETotemRefineCostType::TRC_DIMENTIONAL_PIECE;
	}

	return ETotemRefineCostType::TRC_MAX;
}

inline uint8 CliToSvrRefineCostType(ETotemRefineCostType ClientTotemRefineCostType)
{
	switch (ClientTotemRefineCostType)
	{
	case ETotemRefineCostType::TRC_GOLD:						return b2network::B2TotemRefineCostType::GOLD;
	case ETotemRefineCostType::TRC_DIMENTIONAL_PIECE:			return b2network::B2TotemRefineCostType::DIMENSIONAL_PIECE;
	}

	return 0;
}

inline ETotemGrade ItemStarGradeToTotemGrade(uint8 StarGrade)
{
	switch (StarGrade)
	{
	case 1:			return ETotemGrade::TG_NORMAL;
	case 2:			return ETotemGrade::TG_MAGIC;
	case 3:			return ETotemGrade::TG_RARE;
	case 4:			return ETotemGrade::TG_HERO;
	case 5:			return ETotemGrade::TG_LEGEND;
	case 6:			return ETotemGrade::TG_RELIC;
	}

	return ETotemGrade::TG_MAX;
}

inline uint8 TotemGradeToItemStarGrade(ETotemGrade TotemGrade)
{
	switch (TotemGrade)
	{
	case ETotemGrade::TG_NORMAL:		return 1;
	case ETotemGrade::TG_MAGIC:			return 2;
	case ETotemGrade::TG_RARE:			return 3;
	case ETotemGrade::TG_HERO:			return 4;
	case ETotemGrade::TG_LEGEND:		return 5;
	case ETotemGrade::TG_RELIC:			return 6;
	}

	return 0;
}

inline ETotemGrade SvrToCliTotemGrade(int32 ServerTotemGrade)
{
	switch (ServerTotemGrade)
	{
	case b2network::B2TotemGrade::NORMAL:			return ETotemGrade::TG_NORMAL;
	case b2network::B2TotemGrade::MAGIC:			return ETotemGrade::TG_MAGIC;
	case b2network::B2TotemGrade::RARE:				return ETotemGrade::TG_RARE;
	case b2network::B2TotemGrade::HERO:				return ETotemGrade::TG_HERO;
	case b2network::B2TotemGrade::LEGEND:			return ETotemGrade::TG_LEGEND;
	case b2network::B2TotemGrade::RELIC:			return ETotemGrade::TG_RELIC;
	}

	return ETotemGrade::TG_MAX;
}

inline int32 CliToSvrTotemGrade(ETotemGrade ClientTotemGrade)
{
	switch (ClientTotemGrade)
	{
	case ETotemGrade::TG_NORMAL:					return b2network::B2TotemGrade::NORMAL;
	case ETotemGrade::TG_MAGIC:						return b2network::B2TotemGrade::MAGIC;
	case ETotemGrade::TG_RARE:						return b2network::B2TotemGrade::RARE;
	case ETotemGrade::TG_HERO:						return b2network::B2TotemGrade::HERO;
	case ETotemGrade::TG_LEGEND:					return b2network::B2TotemGrade::LEGEND;
	case ETotemGrade::TG_RELIC:						return b2network::B2TotemGrade::RELIC;
	}

	return 0;
}

inline ETotemBasePrefix SvrToCliTotemBasePrefix(int32 ServerTotemBasePrefix)
{
	switch (ServerTotemBasePrefix)
	{
	case b2network::B2TotemBasePrefix::VALOROUS:	return ETotemBasePrefix::TBP_VALOROUS;
	case b2network::B2TotemBasePrefix::PATRON:		return ETotemBasePrefix::TBP_PATRON;
	case b2network::B2TotemBasePrefix::VIGOROUS:	return ETotemBasePrefix::TBP_VIGOROUS;
	}

	return ETotemBasePrefix::TBP_MAX;
}

inline int32 CliToSvrTotemBasePrefix(ETotemBasePrefix ClientTotemBasePrefix)
{
	switch (ClientTotemBasePrefix)
	{
	case ETotemBasePrefix::TBP_VALOROUS:			return b2network::B2TotemBasePrefix::VALOROUS;
	case ETotemBasePrefix::TBP_PATRON:				return b2network::B2TotemBasePrefix::PATRON;
	case ETotemBasePrefix::TBP_VIGOROUS:			return b2network::B2TotemBasePrefix::VIGOROUS;
	}

	return 0;
}

inline ETotemRefinePrefix SvrToCliTotemRefinePrefix(int32 ServerTotemRefinePrefix)
{
	switch (ServerTotemRefinePrefix)
	{
	case b2network::B2TotemRefinePrefix::ECLIPSE:	return ETotemRefinePrefix::TRP_ECLIPSE;
	case b2network::B2TotemRefinePrefix::SHINING:	return ETotemRefinePrefix::TRP_SHINING;
	case b2network::B2TotemRefinePrefix::STRONG:	return ETotemRefinePrefix::TRP_STRONG;
	case b2network::B2TotemRefinePrefix::SACRED:	return ETotemRefinePrefix::TRP_SACRED;
	}

	return ETotemRefinePrefix::TRP_MAX;
}

inline int32 CliToSvrTotemRefinePrefix(ETotemRefinePrefix ClientTotemRefinePrefix)
{
	switch (ClientTotemRefinePrefix)
	{
	case ETotemRefinePrefix::TRP_ECLIPSE:			return b2network::B2TotemRefinePrefix::ECLIPSE;
	case ETotemRefinePrefix::TRP_SHINING:			return b2network::B2TotemRefinePrefix::SHINING;
	case ETotemRefinePrefix::TRP_STRONG:			return b2network::B2TotemRefinePrefix::STRONG;
	case ETotemRefinePrefix::TRP_SACRED:			return b2network::B2TotemRefinePrefix::SACRED;
	}

	return 0;
}


//#define MINIMUM_SURPASS_STAR_GRADE 6
//#define MAX_NORMAL_ITEM_STAR_GRADE 8
#define MAX_ENHANCE_ITEM_LEVEL 70
#define SKILL_INVALID_ID 0
#define UNDEFINED_SKILL_LEVEL -1
#define MIN_COUNT_ENHANCE_ITEM_SET_EFFECT 6
#define MAX_DECOMPOSITION_COUNT 3
#define MAX_TOTEM_INVENTORY_SLOT 200
#define MAX_ENHANCE_COSTUME_LEVEL 10
/**
* Native representation of a single item, containing the data that affects the player who equip it.
* It is mostly server based. Client replicates this data for UI visualization and various control stuff.
*/
struct FB2Item
{
	FB2Item()
	{
		Empty();
	}

	FB2Item(struct FMD_ItemSingleElem const* ItemElem);
	//에테르를 item으로 변환 (스테이지 획득관련에서만 사용해야 한다!!!)
	FB2Item(const b2network::B2AetherServerInfoPtr Ether);
	FB2Item(const b2network::B2CostumeServerInfo& CostumeItemInfo);

	//
	// No UPROPERTY is expected here (no asset reference either)
	// Most properties are to be replicated from server.
	//

	/** The unique instance UID, which is ensured to be different among all item instances. */
	int64 InstanceUID;

	/** Identification of all item types. Not like InstanceUID, multiple instances can exist having the same ItemRefID (probably with different ability points).
	* Each digit can have defined information, check DevOnlyGet**FromRefID functions below. Client does not need to know that.. Just for a convenience.. */
	int32 ItemRefID;

	/** Multiple items of different class/place having the same causes synergy. 0 for none? */
	int32 SetID;

	/** Overall or inborn rate of an item. Normally 1 ~ FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE. */
	uint8 StarGrade;

	uint8 MaxStarGrade;

	/** Extra grade specification within same StarGrade */
	uint8 Quality;

	int32 ConsumingAmount; // Probably for consumables only. How many times it can be used until get totally consumed.

	EItemClass ItemClass;

	EItemInvenType InventoryType; // Possible places for inventory container
	EItemEquipPlace EquipPlace; // Possible places for equipping. It does not designate current equip state. Check bCurrentlyEquipped for that.

	ECostumeEquipPlace CostumeEquipPlace; //코스튬 착용 위치

	/** Not meaningful for some item type, which uses shared slot. It is supposed to be matching to EPCClass. See ItemAllowedPCClassEnum */
	int32 AllowedPCClass;

	// Refer to IsItem(Levelup/Composition/Upgrade/Surpass)Allowed regarding the enhance level and allowed item modification.
	int32 MaxLevel; // Max levelup for current StarGrade range.
	int32 Level; // LevelupItem level gets increased and reset per each StarGrade range. (Can over MaxLevel when it surpass)
	float Exp; // 0 ~ 100.
	int32 SurpassCount; // Surpass is possible once reached max enhance level of max star grade.
	int32 EnhanceLevel;	// Enhance Level 0~65
	float ItemFactorLevel; // ItemFactorLevel gets continuously increased along all StarGrade and Item FactorLevel.

	/** Most basic item ability. */
	EItemPrimaryPointType PrimaryPointType;

	bool bMaterialFromAtlas;

private:
	float PrimaryPoint;
public:
	FORCEINLINE void SetPrimaryPoint(float InPoint) {
		PrimaryPoint = InPoint;
	}

	/** Returns PrimaryPoint, either just the PrimaryPoint,
	* or including one of PrimaryPoint Increasing option matching its PrimaryPointType (if presents). */
	float GetPrimaryPoint(bool bCountRelevantOption = false) const;
	void SetNewItemState() {
		IsNew = false;
	}

	//When provided only ItemRefID use Previewupdate
	//Bcause ItemMasterData have not primary, power...
	void UpdatePreviewItemPrimaryPoint();
	void UpdatePreviewItemPower();

	static const int32 MinimumSurpassStarGrade = 6;
	static const int32 MaxSurpassStarGrade = 7;
	static const int32 ItemNormalMaxLevel = 30;

	/** Maximum number of intrinsic options is fixed by MAX_ITEM_INTRINSIC_OPTION. Ignored if have more than that. */
	TArray<FItemOption> IntrinsicOptions;

	/** Additional random Options. The maximum number is fixed by MAX_ITEM_RANDOM_OPTION. */
	TArray<FItemOption> RandomOptions;

	int32 OpenSealSlotCount;
	TArray<FSealOption> SealOptions;

	int32 BuyingPrice;
	int32 SellingPrice;

	/** Consider this as server's equipping signal to let client know that this item must go to equip list.
	* Client will try set it to be consistent with the LobbyInventory's equipped list anyway. */
	uint32 bCurrentlyEquipped : 1;

	/** To be excluded from some of automatic inventory manipulations. */
	uint32 bIsLocked : 1;

	/** Inventory menu only transient flag designating that it is selected for other item's Enhancement/Composition/Surpassing ingredient. */
	uint32 bSelectedAsIngred : 1;

	/** True only for a short while, after newly get from shop or combat reward, etc..
	* Its main purpose is to check newly acquired items for equipping suggestion,
	* It must not be set to true after being checked once. */
	uint32 bNewlyAddedForEquipSuggest : 1;
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	// For data transformation between server and client.

	// Item Total Combatpower
	float Power;

	bool IsNew;

	FB2Item& operator=(const b2network::B2ItemServerInfoPtr rhs);

	void UpdateItemMasterDataOnMD();
private:
	void UpdateByItemMasterData(const FMD_ItemSingleElem* InMasterData);
public:
	void Empty()
	{
		InstanceUID = 0;
		ItemRefID = 0;
		SetID = 0;
		StarGrade = 1;
		MaxStarGrade = 6;
		Quality = 0; // 0 quality by default to remove prefix with null item data.
		ConsumingAmount = 0;
		ItemClass = EItemClass::EIC_End;
		InventoryType = EItemInvenType::EIIVT_End;
		EquipPlace = EItemEquipPlace::EIEP_End;
		CostumeEquipPlace = ECostumeEquipPlace::ECEP_None;
		AllowedPCClass = -1;
		MaxLevel = 1;
		Level = 1;
		Exp = 0.0f;
		SurpassCount = 0;
		ItemFactorLevel = 0.0f;
		EnhanceLevel = 0;
		PrimaryPointType = EItemPrimaryPointType::EIPP_End;
		PrimaryPoint = 0.0f;
		BuyingPrice = 0;
		SellingPrice = 0;
		bCurrentlyEquipped = false;
		bIsLocked = false;
		bSelectedAsIngred = false;
		bNewlyAddedForEquipSuggest = false;
		Power = 0;
		IsNew = false;
		IntrinsicOptions.Empty();
		RandomOptions.Empty();
		bMaterialFromAtlas = false;
		OpenSealSlotCount = 0;
	}

	bool IsEmpty()
	{
		return (InstanceUID == 0 && ItemRefID == 0 && ItemClass == EItemClass::EIC_End);
	}
};

enum class EWingIngredAcqElemType // Initially intended for UI.. would be better if its name designed for general use.
{
	WAET_EnhanceIngred,
	WAET_EvolveIngred,
	WAET_PropOptionIngred
};

struct FB2RewardItemPreviewInfo // For BattleStageInfo. To show reward item information in brief
{
	FB2RewardItemPreviewInfo()
	{
		PrimPointMinValue = 0.0f;
		PrimPointMaxValue = 0.0f;
		RandomOptionCount = 0;

		MinGrade = 0;
		MaxGrade = 0;
	}

	FB2Item SomeCommonData; // Just a few data field will be used.
	float PrimPointMinValue;
	float PrimPointMaxValue;
	int32 RandomOptionCount;

	int				MinGrade;
	int				MaxGrade;
};

struct FB2AetherItemPreviewInfo
{
	FB2AetherItemPreviewInfo()
	{
		AetherType = 0;
		AetherTiers.Empty();
		AetherGrades.Empty();

		MinGrade = 0;
		MaxGrade = 0;
	}

	int32			AetherType;
	TArray<int32>	AetherTiers;
	TArray<int32>	AetherGrades;

	int				MinGrade;
	int				MaxGrade;
};

struct FB2MaterialItemPreviewInfo
{
	FB2MaterialItemPreviewInfo()
	{
		ItemRefID = 0;
		ItemDropCount = 0;
	}

	int32			ItemRefID;
	int32			ItemDropCount;
};

/** Sub page tabs of hero management mode */
UENUM(BlueprintType)
enum class EHeroMgmtSubMode : uint8
{
	EHMSM_Inventory,
	EHMSM_Brevet,
	EHMSM_Skill,
	EHMSM_Wing,
	EHMSM_Relic,
	EHMSM_Costume,
	EHMSM_Totem,

	// Added sub mode goes here, prior to EHMSM_RelicManagement_EXTRAPAGE

	EHMSM_RelicManagement_EXTRAPAGE, // Must NOT be a part of EHeroMgmtSubMode iteration!!! must end number.

	EHMSM_End
};

/** Return value excludes EHMSM_RelicManagement_EXTRAPAGE */
FORCEINLINE int32 GetHeroMgmtSubModeMaxNum() {
	return static_cast<int32>(EHeroMgmtSubMode::EHMSM_End) - 1;
}

/** Enhance, upgrade, composition, surpass stuff */
UENUM(BlueprintType)
enum class ELobbyInvenItemOpMode : uint8
{
	EIVIOP_None, // Just normal situation
	EIVIOP_Levelup,
	EIVIOP_Upgrade,
	EIVIOP_Composition,
	EIVIOP_Surpass,
	EIVIOP_Enhancement,
	EIVIOP_EnhancementChange,
	EIVIOP_HeroChange,
	EIVIOP_EnhancementChangeMode,
	EIVIOP_TotemSmelting,
	EIVIOP_EnhancementCostume,
	EIVIOP_Hammer,
};

#define GET_TUPLE_DATA(Index, TUPLE)				\
	std::get<Index>(TUPLE)

struct FB2ItemApperanceInfo
{
	int32	dropItemTemplateID;
	int32	quality;

	FB2ItemApperanceInfo& operator = (const b2network::B2ItemServerInfoPtr rhs)
	{
		this->dropItemTemplateID = rhs->template_id;
		this->quality = rhs->quality;

		return *this;
	}
};

struct FB2ItemUID
{
	int64	UID;

	FB2ItemUID& operator = (const b2network::B2ItemServerInfoPtr rhs)
	{
		this->UID = rhs->id;

		return *this;
	}
};

UENUM(BlueprintType)
enum class EStoreItemType : uint8
{
	BladePoint,
	Gem,
	Gold,
	Item,
	GeneralLottery,
	SelectiveLottery,
	Package,
	DarkCrystal,
	GoldBuffTicket,
	ExpBuffTicket,
	StageBoostTicket,
	ShopTenLotteryTicket,
	Selective_Aether_Lottery,
	Prefix_Seletive_Lottery,
	StageAutoClearTicket,
	NicknameChangeTicket
};

// END definitions for ITEM and INVENTORY
//================================================================================

//================================================================================
// BEGIN definitions for WING system

// How many prop options one wing can have?
const int32 MAX_WING_PROP_OPTION_NUM = 6;

struct FWingPropOption // Refer to FItemOption struct, which has similar role for FB2Item
{
	FWingPropOption()
	{
		InitVariables();
	}
	//FWingPropOption(EItemOption InOptionType, bool bInIsOpen, int32 InOptionLevel, float InRawAmount)
	//{
	//	MyOptionType = InOptionType;
	//	bInIsOpen = bIsOpen;
	//	OptionLevel = InOptionLevel;
	//	RawOptionAmount = InRawAmount;
	//}
	void InitVariables()
	{
		MyOptionType = EItemOption::EIO_End;
		RequiredWingEvolveGrade = 1;
		bIsOpen = false;
		MaxOptionLevel = 30; // Approximately this value, but need to get exact value from server.
		OptionLevel = 1;
		RawOptionAmount = 0.0f;
	}
	// It just use the same identification to ItemOption
	EItemOption MyOptionType;

	int32 RequiredWingEvolveGrade; // Supposed to be open if owing wing's evolve grade is at least at this level.
	bool bIsOpen;

	int32 MaxOptionLevel;
	int32 OptionLevel;

	/** How much effective the option will be. As its name implies, this could be the nominal value for display,
	* then the final value to be applied could be a little different, especially in the case of decreasing option.
	* See GetEffectivePropOptionValue for getting the final effective value of these. */
	float RawOptionAmount;
};

// Client wing prop option index starts from 0, while server starts from 1.
inline int32 CliToSvrWingPropOptionIndex(int32 CliOptionIndex) {
	return CliOptionIndex + 1;
}
inline int32 SvrToCliWingPropOptionIndex(int32 SvrOptionIndex) {
	return SvrOptionIndex - 1;
}

/**
* Native representation of a single wing, containing the data that affects the player who has it.
* Like FB2Item, it is mostly server based. Client replicates this data for UI visualization and various control stuff.
*/
struct FB2Wing
{
	FB2Wing()
	{
		EssetialConstruct();

		OwnerPCClass = 0;
		EvolutionGrade = 1;
		EvolutionFailedCount = 0;
		EnhanceLevel = 1;
		EnhancePoint = 0;

		bShouldBeVisible = true;

		AttackPoint = 0.0f;
		DefensePoint = 0.0f;
		HealthPoint = 0.0f;
	}

	FB2Wing(const b2network::FB2MessageInfoResponseGetWingPtr& ServerResponsePtr)
	{
		EssetialConstruct(); // Need to generate PropOptions array.

		*this = ServerResponsePtr;
	}
private:
	void EssetialConstruct()
	{
		PropOptions.Empty();
		// Assuming that we got fixed number of options, but option type is not really fixed now.. just get if from server data.
		for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
		{
			PropOptions.Add(FWingPropOption());
		}
	}
public:

	/**
	* Casted EPCClass. The PCClass who can own (equip) this wing. Another id that requires different mesh resource.
	*/
	int32 OwnerPCClass;

	/**
	* The most prominent grade of a wing. Different mesh resource will be applied for different Evolution grade.
	*/
	int32 EvolutionGrade;

	/**
	* It automatically evolve when it fails more than certain times.
	*/
	int32 EvolutionFailedCount;

	/**
	* It increases and reset for each Evolution. Just like enhance level of an item.
	*/
	int32 EnhanceLevel;

	/**
	* EnhanceLevel is increased when EnhancePoint reaches at certain amount, like experience point.
	*/
	int32 EnhancePoint;

	/**
	* Whether to draw this wing on client display.
	*/
	uint32 bShouldBeVisible : 1;

private:
	// Here begins wing's basic ability to increase player's primary combat points.
	float AttackPoint;
	float DefensePoint;
	float HealthPoint;
public:
	FORCEINLINE void SetAttackPoint(float InPoint) {
		AttackPoint = InPoint;
	}
	FORCEINLINE void SetDefensePoint(float InPoint) {
		DefensePoint = InPoint;
	}
	FORCEINLINE void SetHealthPoint(float InPoint) {
		HealthPoint = InPoint;
	}

	/** Returns AttackPoint, either just the AttackPoint, or considering option which can influences AttackPoint */
	float GetAttackPoint(bool bCountRelevantOption = true) const;
	float GetDefensePoint(bool bCountRelevantOption = true) const;
	float GetHealthPoint(bool bCountRelevantOption = true) const;
private:
	float GetExtraPrimPointByRelevantOption(EItemPrimaryPointType InPrimPointType) const;

private:
	/** Not really meant to capsulize this one, just to provide indexing interface..
	* This array size is virtually static. */
	TArray<FWingPropOption> PropOptions;
public:

	const FWingPropOption* GetPropOptionPtr(EItemOption InOptionType) const;
	const FWingPropOption* GetPropOptionPtr(int32 InIndex) const;
	const TArray<FWingPropOption>& GetPropOptionArr() const;
	/** Get final scaled value to be effectively applied to the character who has this wing, of designated wing prop option type.
	* Do not use this if you just gather the raw option point along with other item option points. */
	float GetEffectivePropOptionValue(EItemOption InOptionType) const;

	/** Update by server struct containing most necessary data. Almost intended for local character player. */
	void UpdateByServerGetWingResponse(const b2network::FB2MessageInfoResponseGetWingPtr& InResponseGetWingPtr);

	/** This wing instance data will be available for combat features after calling this
	* Almost intended for remote player characters. */
	void UpdateByPartialServerInfo(const b2network::B2CharacterServerInfoPtr& InCharServerInfo, const TArray<b2network::B2WingOptionInfoPtr>& InWingOptionInfo);

	/** If bUpdateAllImmediate is true, it updates all data by mater data lookup, by calling UpdateStats */
	void UpdateSinglePropOptionByServerInfo(const b2network::B2WingOptionInfoPtr& InServerInfoPtr, bool bUpdateAllImmediate = false);

	/**
	* It assumes that basic properties from server response are filled and fill-up other data (combat options) based on those properties.
	* Returns false if it could not find required master data.
	*/
	bool UpdateStats();

	friend class UB2Airport;
};

// END definitions for WING system
//================================================================================

/** StageEventManageMode defines in which case the EventDirector should play the show, mainly according to the game mode context. */
UENUM(BlueprintType)
enum class EStageEventManageMode : uint8
{
	/** Played only for story mode, when the stage is not cleared yet or user does not selected skip option. */
	SEMM_ScenarioOnly,

	/** Played when the user selected skip story once after cleared the stage, or when playing adventure mode (of harder difficulty)
	* A stage event having this setting is intended to be shorter than scenario mode setting. */
	SEMM_RegularStageOnly,

	/** Always played for main adventure, regardless of scenario mode case (the first play).
	* It can be set instead of SEMM_Always when a map is re-used for other special challenge mode, and you don't want a EventDirector being played for them. */
	SEMM_AlwaysForAdventure,

	/** Played for whatever case. */
	SEMM_Always
};

UENUM()
enum class EPreLoadingScreenType : uint8
{
	PLST_StageBegin,
	PLST_GoToLobby,

	PLST_PreRender_First, // For first time execution, pre-render gamemode.
	PLST_PreRender_Default, // Any other pre-render after first time pre-render.

	// To be added..

	PLST_End
};
FORCEINLINE bool IsOneOfPreRenderScreenType(EPreLoadingScreenType InType)
{
	return (InType == EPreLoadingScreenType::PLST_PreRender_Default) || (InType == EPreLoadingScreenType::PLST_PreRender_First);
}

enum class EHeroTowerMapType : int32
{
	// Each has equipping slot in inventory.
	EHTMT_NONE = 0,
	EHTMT_UNDER = 3001,
	EHTMT_MIDDLE = 3002,
	EHTMT_UPPER = 3003,
	EHTMT_BOSS = 3101,
};

inline EHeroTowerMapType SvrToCliHeroTowerMapType(int32 MapType)
{
	switch (MapType)
	{
	case b2network::B2HeroTowerMapType::UNDER: return EHeroTowerMapType::EHTMT_UNDER;
	case b2network::B2HeroTowerMapType::MIDDLE: return EHeroTowerMapType::EHTMT_MIDDLE;
	case b2network::B2HeroTowerMapType::UPPER: return EHeroTowerMapType::EHTMT_UPPER;
	case b2network::B2HeroTowerMapType::BOSS: return EHeroTowerMapType::EHTMT_BOSS;
	default: return EHeroTowerMapType::EHTMT_NONE;
	}
}

using FB2ResponseStartStage = b2network::FB2MessageInfoResponseStartStage;
using FB2StartStageInfo = TU::MakeTuple<FB2ResponseStartStage::type_list>::Result;

using FB2OriginAccountInfo = b2network::B2AccountInfo;
using FB2OriginAccountInfoPtr = b2network::B2AccountInfoPtr;
using FB2AccountInfo = TU::MakeTuple<FB2OriginAccountInfo::type_list>::Result;

using FB2OriginCharacterInfo = b2network::B2CharacterServerInfo;
using FB2OriginCharacterInfoPtr = b2network::B2CharacterServerInfoPtr;
using FB2CharacterInfo = TU::MakeTuple<FB2OriginCharacterInfo::type_list>::Result;

using FB2ResponseWholeInfo = b2network::FB2MessageInfoResponseGetAccount;
using FB2WholeInfo = TU::MakeTuple<FB2ResponseWholeInfo::type_list>::Result;
using FB2ItemPresetInfo = b2network::B2ItemPresetPtr;

using FB2ResponseAccountItems = b2network::FB2MessageInfoResponseGetAccountItem;
using FB2AccountItems = TU::MakeTuple<FB2ResponseAccountItems::type_list>::Result;

using FB2ResponseMasterDatas = b2network::FB2MessageInfoResponseGetMasterData;
using FB2MasterDatas = TU::MakeTuple<FB2ResponseMasterDatas::type_list>::Result;

using FB2ResponseGetLatestEventDataPtr = b2network::FB2MessageInfoResponseGetLatestEventDataPtr;

struct FB2AddedItems
{
	TArray<FB2Item>				AddedItems;
};

using FB2ResponseDismantleItems = b2network::FB2MessageInfoResponseDismantleItems;
using FB2DismantleItems = TU::MakeTuple<FB2ResponseDismantleItems::type_list>::Result;

using FB2ResponseSoldItem = b2network::FB2MessageInfoResponseSellItem;
using FB2SoldItems = TU::MakeTuple<FB2ResponseSoldItem::type_list>::Result;

using FB2ResponseSellConsumableItem = b2network::FB2MessageInfoResponseSellConsumableItem;
using FB2SellConsumableItem = TU::MakeTuple<FB2ResponseSellConsumableItem::type_list>::Result;

using FB2ResponseConvertData = b2network::FB2MessageInfoResponseConvertData;
using FB2ConvertData = TU::MakeTuple<FB2ResponseConvertData::type_list>::Result;

using FB2ResponseConvertItem = b2network::FB2MessageInfoResponseConvertItem;
using FB2ConvertItem = TU::MakeTuple<FB2ResponseConvertItem::type_list>::Result;

using FB2ResponseEquippedItem = b2network::FB2MessageInfoResponseEquipItem;
using FB2EquippedItems = TU::MakeTuple<FB2ResponseEquippedItem::type_list>::Result;

using FB2ResponseUnequippedItem = b2network::FB2MessageInfoResponseUnequipItem;
using FB2UnequippedItems = TU::MakeTuple<FB2ResponseUnequippedItem::type_list>::Result;

using FB2ResponseLockItem = b2network::FB2MessageInfoResponseLockItem;
using FB2LockedItems = TU::MakeTuple<FB2ResponseLockItem::type_list>::Result;

using FB2ResponseUnlockItem = b2network::FB2MessageInfoResponseUnlockItem;
using FB2UnlockedItems = TU::MakeTuple<FB2ResponseUnlockItem::type_list>::Result;

using FB2ResponseLevelupItem = b2network::FB2MessageInfoResponseLevelupItem;
using FB2LevelupItem = TU::MakeTuple<FB2ResponseLevelupItem::type_list>::Result;

using FB2ResponseEnhanceItem = b2network::FB2MessageInfoResponseEnhanceItem;
using FB2EnhanceItem = TU::MakeTuple<FB2ResponseEnhanceItem::type_list>::Result;

using FB2ResponseDismantleItems = b2network::FB2MessageInfoResponseDismantleItems;
using FB2DismantleItems = TU::MakeTuple<FB2ResponseDismantleItems::type_list>::Result;

using FB2ResponseExchangeItemEnhanceLevel = b2network::FB2MessageInfoResponseExchangeItemEnhanceLevel;
using FB2ItemEnhanceLevel = TU::MakeTuple<FB2ResponseExchangeItemEnhanceLevel::type_list>::Result;

using FB2ResponseOpenSealSlotItem = b2network::FB2MessageInfoResponseOpenSealSlotItem;
using FB2OpenSealSlotItem = TU::MakeTuple<FB2ResponseOpenSealSlotItem::type_list>::Result;
using FB2ResponseOpenSealSlotItemPtr = b2network::FB2MessageInfoResponseOpenSealSlotItemPtr;

using FB2ResponseCarveSealItem = b2network::FB2MessageInfoResponseCarveSealItem;
using FB2CarveSealItem = TU::MakeTuple<FB2ResponseCarveSealItem::type_list>::Result;
using FB2ResponseCarveSealItemPtr = b2network::FB2MessageInfoResponseCarveSealItemPtr;

using FB2ResponseSurpassItem = b2network::FB2MessageInfoResponseSurpassItem;
using FB2SurpassItem = TU::MakeTuple<FB2ResponseSurpassItem::type_list>::Result;

using FB2ResponseUpgradeItem = b2network::FB2MessageInfoResponseUpgradeItem;
using FB2UpgradeItem = TU::MakeTuple<FB2ResponseUpgradeItem::type_list>::Result;

using FB2ResponseComposeItem = b2network::FB2MessageInfoResponseComposeItem;
using FB2ComposeItem = TU::MakeTuple<FB2ResponseComposeItem::type_list>::Result;

using FB2ResponseExtendInventory = b2network::FB2MessageInfoResponseExtendInventory;
using FB2ExtendInventory = TU::MakeTuple<FB2ResponseExtendInventory::type_list>::Result;

using FB2ResponseClearStage = b2network::FB2MessageInfoResponseClearStage;
using FB2ClearStageInfo = TU::MakeTuple<FB2ResponseClearStage::type_list>::Result;

using FB2ResponseResurrection = b2network::FB2MessageInfoResponseResurrection;
using FB2Resurrection = TU::MakeTuple<FB2ResponseResurrection::type_list>::Result;

using FB2ResponseSweepStage = b2network::FB2MessageInfoResponseSweepStage;
using FB2SweepStageInfo = TU::MakeTuple<FB2ResponseSweepStage::type_list>::Result;

using FB2ResponseRaidResurrection = b2network::FB2MessageInfoResponseRaidResurrection;
using FB2RaidResurrection = TU::MakeTuple<FB2ResponseRaidResurrection::type_list>::Result;

using FB2ResponseBladePointCharge = b2network::FB2MessageInfoResponseCheckBladePointCharge;
using FB2BladePointCharge = TU::MakeTuple<FB2ResponseBladePointCharge::type_list>::Result;

using FB2CheckChargePointsPtr = b2network::FB2MessageInfoResponseCheckChargePointsPtr;

using FB2ResponsePvpMatchPointCharge = b2network::FB2MessageInfoResponseCheckPvPMatchPointCharge;
using FB2PvpMatchPointCharge = TU::MakeTuple<FB2ResponsePvpMatchPointCharge::type_list>::Result;

using FB2ResponseTeamMatchPointCharge = b2network::FB2MessageInfoResponseCheckTeamMatchPointCharge;
using FB2TeamMatchPointCharge = TU::MakeTuple<FB2ResponseTeamMatchPointCharge::type_list>::Result;

using FB2ResponseItemSurpassCost = b2network::FB2MessageInfoResponseGetItemSurpassCost;
using FB2GetItemSurpassCost = TU::MakeTuple<FB2ResponseItemSurpassCost::type_list>::Result;

using FB2ResponseItemUpgradeCost = b2network::FB2MessageInfoResponseGetItemUpgradeCost;
using FB2GetItemUpgradeCost = TU::MakeTuple<FB2ResponseItemUpgradeCost::type_list>::Result;

using FB2ResponseItemComposeCost = b2network::FB2MessageInfoResponseGetItemComposeCost;
using FB2GetItemComposeCost = TU::MakeTuple<FB2ResponseItemComposeCost::type_list>::Result;

using FB2ActRepeatClearRewardInfoPtr = b2network::B2ActRepeatClearRewardInfoPtr;

using FB2ResponseReceiveActClearReward = b2network::FB2MessageInfoResponseReceiveActClearReward;
using FB2GetReceiveActClearReward = TU::MakeTuple<FB2ResponseReceiveActClearReward::type_list>::Result;

using FB2ResponseReceiveActRepeatClearReward = b2network::FB2MessageInfoResponseReceiveActRepeatClearReward;
using FB2GetReceiveActRepeatClearReward = TU::MakeTuple<FB2ResponseReceiveActRepeatClearReward::type_list>::Result;

using FB2ResponseGetActInfo = b2network::FB2MessageInfoResponseGetActInfo;
using FB2OriginGetActInfoPtr = b2network::FB2MessageInfoResponseGetActInfoPtr;
using FB2GetActInfo = TU::MakeTuple<FB2ResponseGetActInfo::type_list>::Result;

using FB2ResponseGetStageInfo = b2network::FB2MessageInfoResponseGetStageInfo;
using FB2OriginGetStageInfoPtr = b2network::FB2MessageInfoResponseGetStageInfoPtr;
using FB2GetStageInfo = TU::MakeTuple<FB2ResponseGetStageInfo::type_list>::Result;

using FB2ConfigDataPtr = b2network::B2ConfigDataPtr;

using FB2SkillInfo = b2network::B2SkillServerInfo;

using FB2ResponseLevelUpSkill = b2network::FB2MessageInfoResponseLevelUpSkill;
using FB2LevelUpSkillInfoPtr = b2network::FB2MessageInfoResponseLevelUpSkillPtr;
using FB2LevelUpSkillInfo = TU::MakeTuple<FB2ResponseLevelUpSkill::type_list>::Result;

using FB2ResponseResetSkill = b2network::FB2MessageInfoResponseResetSkill;
using FB2ResetSkillInfoPtr = b2network::FB2MessageInfoResponseResetSkillPtr;
using FB2ResetSkillInfo = TU::MakeTuple<FB2ResponseResetSkill::type_list>::Result;

using FB2ResponseBuySkillPoint = b2network::FB2MessageInfoResponseBuySkillPoint;
using FB2BuySkillPointInfoPtr = b2network::FB2MessageInfoResponseBuySkillPointPtr;
using FB2BuySkillPointInfo = TU::MakeTuple<FB2ResponseBuySkillPoint::type_list>::Result;

using FB2ResponseSetUsingSkill = b2network::FB2MessageInfoResponseSetUsingSkill;
using FB2SetUsingSkillInfoPtr = b2network::FB2MessageInfoResponseSetUsingSkillPtr;
using FB2SetUsingSkillInfo = TU::MakeTuple<FB2ResponseSetUsingSkill::type_list>::Result;

using FB2RoomJoinResponse = b2network::FB2MessageInfoRoomJoinResponse;
using FB2RoomJoin = TU::MakeTuple<FB2RoomJoinResponse::type_list>::Result;

using FB2RoomParticipantPtr = b2network::B2RoomParticipantPtr;

using FB2FindAccountInfoPtr = b2network::B2FindAccountInfoPtr;

using FB2ResponseTeamBattleStatus = b2network::FB2MessageInfoResponseGetTeamBattleStatus;
using FB2TeamBattleStatusPtr = b2network::FB2MessageInfoResponseGetTeamBattleStatusPtr;
using FB2TeamBattleStatus = TU::MakeTuple<FB2ResponseTeamBattleStatus::type_list>::Result;

using FB2TeamBattleEntryPtr = b2network::B2TeamBattleEntryPtr;

using FB2ResponseTeamMatchMaking = b2network::FB2MessageInfoResponseTeamMatchmaking;
using FB2TeamMatchMakingPtr = b2network::FB2MessageInfoResponseTeamMatchmakingPtr;
using FB2TeamMatchMaking = TU::MakeTuple<FB2ResponseTeamMatchMaking::type_list>::Result;

using FB2ResponseTagMatchMaking = b2network::FB2MessageInfoMatchmakingResponse;
using FB2TagMatchMakingPtr = b2network::FB2MessageInfoMatchmakingResponsePtr;
using FB2TagMatchMaking = TU::MakeTuple<FB2ResponseTagMatchMaking::type_list>::Result;

using FB2OpponentTeamMatchInfoDataPtr = b2network::B2TeamPtr;
using FB2OpponentMatchInfoDataPtr = b2network::B2MatcherPtr;

using FB2ResponseTeamMatchResult = b2network::FB2MessageInfoResponseTeamMatchResult;
using FB2TeamMatchResultPtr = b2network::FB2MessageInfoResponseTeamMatchResultPtr;
using FB2TeamMatchResult = TU::MakeTuple<FB2ResponseTeamMatchResult::type_list>::Result;

using FB2ResponseEnhanceTeamFormation = b2network::FB2MessageInfoResponseEnhanceTeamBattleFormation;
using FB2EnhanceTeamFormationPtr = b2network::FB2MessageInfoResponseEnhanceTeamBattleFormationPtr;
using FB2EnhanceTeamFormation = TU::MakeTuple<FB2ResponseEnhanceTeamFormation::type_list>::Result;

using FB2ResponseEnhanceRankNode = b2network::FB2MessageInfoResponseEnhanceRankNode;
using FB2EnhanceRankNode = TU::MakeTuple<FB2ResponseEnhanceRankNode::type_list>::Result;

using FB2ResponseGetMatchStatus = b2network::FB2MessageInfoResponseGetMatchStatus;
using FB2TagMatchStatusPtr = b2network::FB2MessageInfoResponseGetMatchStatusPtr;
using FB2TagMatchStatus = TU::MakeTuple<FB2ResponseGetMatchStatus::type_list>::Result;

using FB2MatchRanker = b2network::B2MatchRanker;
using FB2ChracterExpInfoPtr = b2network::B2IncreaseCharacterExpInfoPtr;
using FB2LightCharacterPtr = b2network::B2LightCharacterPtr;
using FB2ResponseTagMatchResult = b2network::FB2MessageInfoMatchResultResponse;
using FB2TagMatchResultPtr = b2network::FB2MessageInfoMatchResultResponsePtr;
using FB2TagMatchResult = TU::MakeTuple<FB2ResponseTagMatchResult::type_list>::Result;

using FB2ResponseGetChatChannel = b2network::FB2MessageInfoGetChatChannelResponse;
using FB2GetChatChannelResultPtr = b2network::FB2MessageInfoGetChatChannelResponsePtr;
using FB2GetChatChannelResult = TU::MakeTuple<FB2ResponseGetChatChannel::type_list>::Result;

const int32 MatchResult_Win = b2network::B2MatchResultType::BMRT_WIN;
const int32 MatchResult_Lose = b2network::B2MatchResultType::BMRT_LOSE;
const int32 MatchResult_Draw = b2network::B2MatchResultType::BMRT_DRAW;

using FB2ResponseGetMailList = b2network::FB2MessageInfoResponseGetMailList;
using FB2MailListPtr = b2network::FB2MessageInfoResponseGetMailListPtr;
using FB2MailList = TU::MakeTuple<FB2ResponseGetMailList::type_list>::Result;

using FB2MailContentPtr = b2network::B2MailInfoPtr;
using FB2MailLotteryItemInfoPtr = b2network::B2MailItemInfoPtr;
using FB2ResponseOpenMailPtr = b2network::FB2MessageInfoResponseOpenMailPtr;

using FB2ResponseOpenAllMail = b2network::FB2MessageInfoResponseOpenAllMail;
using FB2OpenAllMailPtr = b2network::FB2MessageInfoResponseOpenAllMailPtr;
using FB2OpenAllMail = TU::MakeTuple<FB2ResponseOpenAllMail::type_list>::Result;

using FB2FriendPtr = b2network::B2FriendPtr;
using FB2UserPtr = b2network::B2UserPtr;
using FB2CharacterSummaryPtr = b2network::B2CharacterSummaryPtr;

using FB2ResponseGetFriendList = b2network::FB2MessageInfoResponseGetFriendList;
using FB2FriendListPtr = b2network::FB2MessageInfoResponseGetFriendListPtr;
using FB2FriendList = TU::MakeTuple<FB2ResponseGetFriendList::type_list>::Result;

using FB2ResponseDeleteFriend = b2network::FB2MessageInfoResponseDeleteFriend;
using FB2DeleteFriendPtr = b2network::FB2MessageInfoResponseDeleteFriendPtr;
using FB2DeleteFriend = TU::MakeTuple<FB2ResponseDeleteFriend::type_list>::Result;

using FB2ResponseGetAllSendAskFriend = b2network::FB2MessageInfoResponseGetAllSendAskFriend;
using FB2AllSendAskFriendListPtr = b2network::FB2MessageInfoResponseGetAllSendAskFriendPtr;
using FBAllSendAskFriendList = TU::MakeTuple<FB2ResponseGetAllSendAskFriend::type_list>::Result;

using FB2ResponseGetAllReceiveAskFriend = b2network::FB2MessageInfoResponseGetAllReceiveAskFriend;
using FB2AllReceiveAskFriendListPtr = b2network::FB2MessageInfoResponseGetAllReceiveAskFriendPtr;
using FB2AllReceiveAskFriendList = TU::MakeTuple<FB2ResponseGetAllReceiveAskFriend::type_list>::Result;

using FB2ResponseSendSocialPoint = b2network::FB2MessageInfoResponseSendSocialPoint;
using FB2SendSocialPointPtr = b2network::FB2MessageInfoResponseSendSocialPointPtr;
using FB2SendSocialPoint = TU::MakeTuple<FB2ResponseSendSocialPoint::type_list>::Result;

using FB2ResponseGetRecommendFriend = b2network::FB2MessageInfoResponseGetRecommendFriend;
using FB2RecommendFriendListPtr = b2network::FB2MessageInfoResponseGetRecommendFriendPtr;
using FB2RecommendFriendList = TU::MakeTuple<FB2ResponseGetRecommendFriend::type_list>::Result;

using FB2ResponseFindUser = b2network::FB2MessageInfoResponseFindUser;
using FB2FindUserPtr = b2network::FB2MessageInfoResponseFindUserPtr;
using FB2FindUser = TU::MakeTuple<FB2ResponseFindUser::type_list>::Result;

using FB2ResponseGetWing = b2network::FB2MessageInfoResponseGetWing;
using FB2GetWingPtr = b2network::FB2MessageInfoResponseGetWingPtr;
using FB2GetWing = TU::MakeTuple<FB2ResponseGetWing::type_list>::Result;

using FB2ResponseEnhanceWing = b2network::FB2MessageInfoResponseEnhanceWing;
using FB2EnhanceWingPtr = b2network::FB2MessageInfoResponseEnhanceWingPtr;
using FB2EnhanceWing = TU::MakeTuple<FB2ResponseEnhanceWing::type_list>::Result;

using FB2ResponseUpgradeWing = b2network::FB2MessageInfoResponseUpgradeWing;
using FB2UpgradeWingPtr = b2network::FB2MessageInfoResponseUpgradeWingPtr;
using FB2UpgradeWing = TU::MakeTuple<FB2ResponseUpgradeWing::type_list>::Result;

using FB2ResponseEnhanceWingOption = b2network::FB2MessageInfoResponseEnhanceWingOption;
using FB2EnhanceWingOptionPtr = b2network::FB2MessageInfoResponseEnhanceWingOptionPtr;
using FB2EnhanceWingOption = TU::MakeTuple<FB2ResponseEnhanceWingOption::type_list>::Result;

using FB2ResponseSetWingVisible = b2network::FB2MessageInfoResponseSetWingVisible;
using FB2SetWingVisiblePtr = b2network::FB2MessageInfoResponseSetWingVisiblePtr;
using FB2SetWingVisible = TU::MakeTuple<FB2ResponseSetWingVisible::type_list>::Result;

using FB2ResponseSetHelmetVisible = b2network::FB2MessageInfoResponseSetHelmetVisible;
using FB2SetHelmetVisiblePtr = b2network::FB2MessageInfoResponseSetHelmetVisiblePtr;
using FB2SetHelmetVisible = TU::MakeTuple<FB2ResponseSetHelmetVisible::type_list>::Result;

using FB2ResponseGetDonationEvent = b2network::FB2MessageInfoResponseGetDonationEvent;
using FB2GetDonationEventPtr = b2network::FB2MessageInfoResponseGetDonationEventPtr;

using FB2ResponseMakeDonation = b2network::FB2MessageInfoResponseMakeDonation;
using FB2MakeDonationPtr = b2network::FB2MessageInfoResponseMakeDonationPtr;

using FB2ResponseDonationPointReward = b2network::FB2MessageInfoResponseDonationPointReward;
using FB2DonationPointRewardPtr = b2network::FB2MessageInfoResponseDonationPointRewardPtr;

using FB2ResponseGetFairy = b2network::FB2MessageInfoResponseGetFairy;
using FB2GetFairyPtr = b2network::FB2MessageInfoResponseGetFairyPtr;
using FB2GetFairy = TU::MakeTuple<FB2ResponseGetFairy::type_list>::Result;

using FB2ResponseLevelupFairy = b2network::FB2MessageInfoResponseLevelupFairy;
using FB2LevelupFairyPtr = b2network::FB2MessageInfoResponseLevelupFairyPtr;
using FB2LevelupFairy = TU::MakeTuple<FB2ResponseLevelupFairy::type_list>::Result;

using FB2ResponseBlessFairy = b2network::FB2MessageInfoResponseBlessFairy;
using FB2BlessFairyPtr = b2network::FB2MessageInfoResponseBlessFairyPtr;
using FB2BlessFairy = TU::MakeTuple<FB2ResponseBlessFairy::type_list>::Result;

using FB2ResponseReceiveFairyGift = b2network::FB2MessageInfoResponseReceiveFairyGift;
using FB2ReceiveFairyGiftPtr = b2network::FB2MessageInfoResponseReceiveFairyGiftPtr;

using FB2ResponseGetAccountCostume = b2network::FB2MessageInfoResponseGetAccountCostume;
using FB2GetAccountCostumePtr = b2network::FB2MessageInfoResponseGetAccountCostumePtr;

using FB2ResponseEquipCostume = b2network::FB2MessageInfoResponseEquipCostume;
using FB2EquipCostumePtr = b2network::FB2MessageInfoResponseEquipCostumePtr;

using FB2ResponseUnequipCostume = b2network::FB2MessageInfoResponseUnequipCostume;
using FB2UnequipCostumePtr = b2network::FB2MessageInfoResponseUnequipCostumePtr;

using FB2ResponseEnhanceCostume = b2network::FB2MessageInfoResponseEnhanceCostume;
using FB2EnhanceCostumePtr = b2network::FB2MessageInfoResponseEnhanceCostumePtr;

using FB2ResponseDismantleCostumes = b2network::FB2MessageInfoResponseDismantleCostumes;
using FB2DismantleCostumesPtr = b2network::FB2MessageInfoResponseDismantleCostumesPtr;

using FB2ResponseLockCostumes = b2network::FB2MessageInfoResponseLockCostumes;
using FB2LockCostumesPtr = b2network::FB2MessageInfoResponseLockCostumesPtr;

using FB2ResponseUnlockCostumes = b2network::FB2MessageInfoResponseUnlockCostumes;
using FB2UnlockCostumesPtr = b2network::FB2MessageInfoResponseUnlockCostumesPtr;

using FB2ResponseGetCostumeShop = b2network::FB2MessageInfoResponseGetCostumeShop;
using FB2GetCostumeShopPtr = b2network::FB2MessageInfoResponseGetCostumeShopPtr;

using FB2ResponsePurchaseCostume = b2network::FB2MessageInfoResponsePurchaseCostume;
using FB2PurchaseCostumePtr = b2network::FB2MessageInfoResponsePurchaseCostumePtr;

using FB2ResponseSetCostumeVisible = b2network::FB2MessageInfoResponseSetCostumeVisible;
using FB2SetCostumeVisiblePtr = b2network::FB2MessageInfoResponseSetCostumeVisiblePtr;

using FB2ResponseAddCostumeToInventory = b2network::FB2MessageInfoResponseAddCostumeToInventory;
using FB2AddCostumeToInventoryPtr = b2network::FB2MessageInfoResponseAddCostumeToInventoryPtr;

using FB2CollectionItemPtr = b2network::FB2MessageInfoResponseGetCollectionItemPtr;
using FB2RewardCollectionItemPtr = b2network::FB2MessageInfoResponseRewardCollectionItemPtr;
using FB2RewardCollectionSetItemPtr = b2network::FB2MessageInfoResponseRewardCollectionSetItemPtr;
using FB2FindAccountPtr = b2network::FB2MessageInfoResponseFindAccountPtr;
using FB2OpenDuelRewardBoxPtr = b2network::FB2MessageInfoResponseOpenDuelRewardBoxPtr;
using FB2GetDuelRewardBoxInfoPtr = b2network::FB2MessageInfoResponseGetDuelRewardBoxInfoPtr;
using FB2GetDuelRewardBoxGainableListPtr = b2network::FB2MessageInfoResponseGetDuelRewardBoxGainableListPtr;
using FB2AssaultMvpRewardPtr = b2network::FB2MessageInfoResponseReceiveAssaultMvpRewardPtr;

using FB2LevelUpPackageMissionPtr = b2network::B2LevelUpPackageMissionPtr;
using FB2ReachLevelPackageMissionPtr = b2network::B2ReachLevelPackageMissionPtr;
using FB2FlatrateBonusStatePtr = b2network::B2FlatRatePackageBonusStatePtr;
using FB2ResponseMagicShopPtr = b2network::B2MagicShopPtr;

using FB2UpdateNicknamePtr = b2network::FB2MessageInfoResponseUpdateNicknamePtr;

// totem
using FB2ResponseGetAccountTotem = b2network::FB2MessageInfoResponseGetAccountTotem;
using FB2GetAccountTotemPtr = b2network::FB2MessageInfoResponseGetAccountTotemPtr;
using FB2ResponseEquipTotems = b2network::FB2MessageInfoResponseEquipTotems;
using FB2EquipTotemsPtr = b2network::FB2MessageInfoResponseEquipTotemsPtr;
using FB2ResponseUnequipTotems = b2network::FB2MessageInfoResponseUnequipTotems;
using FB2UnequipTotemsPtr = b2network::FB2MessageInfoResponseUnequipTotemsPtr;
using FB2ResponseLockTotems = b2network::FB2MessageInfoResponseLockTotems;
using FB2LockTotemsPtr = b2network::FB2MessageInfoResponseLockTotemsPtr;
using FB2ResponseUnlockTotems = b2network::FB2MessageInfoResponseUnlockTotems;
using FB2UnlockTotemsPtr = b2network::FB2MessageInfoResponseUnlockTotemsPtr;
using FB2ResponseSellTotems = b2network::FB2MessageInfoResponseSellTotems;
using FB2SellTotemsPtr = b2network::FB2MessageInfoResponseSellTotemsPtr;
using FB2ResponseDismantleTotems = b2network::FB2MessageInfoResponseDismantleTotems;
using FB2DismantleTotemsPtr = b2network::FB2MessageInfoResponseDismantleTotemsPtr;
using FB2ResponseRefineTotem = b2network::FB2MessageInfoResponseRefineTotem;
using FB2RefineTotemPtr = b2network::FB2MessageInfoResponseRefineTotemPtr;
using FB2ResponseModifyTotemNewFlag = b2network::FB2MessageInfoResponseModifyTotemNewFlag;
using FB2ModifyTotemNewFlagPtr = b2network::FB2MessageInfoResponseModifyTotemNewFlagPtr;
using FB2ResponseAddTotemToInventory = b2network::FB2MessageInfoResponseAddTotemToInventory;
using FB2AddTotemToInventoryPtr = b2network::FB2MessageInfoResponseAddTotemToInventoryPtr;

//ItemForge
using FB2ResponseGetItemForgeListPtr = b2network::FB2MessageInfoResponseGetFactoryPtr;
using FB2ResponseTryItemForgePtr = b2network::FB2MessageInfoResponseMakeFactoryItemPtr;

//SealBox
using FB2ResponseUnsealBoxPtr = b2network::FB2MessageInfoResponseUnsealBoxPtr;

// ServerCmd
using FB2ServerCmdPtr = b2network::FB2MessageInfoResponseServerCmdPtr;

enum class EItemCollectionState : uint8
{
	None,
	Collected,
	Enhanced,
	Rewarded,
	Lottery,
	Max
};

inline EItemCollectionState SvrToClientItemCollectionState(int32 SvrState)
{
	switch (SvrState)
	{
	case b2network::B2CollectionItemState::COLLECTED: return EItemCollectionState::Collected;
	case b2network::B2CollectionItemState::ENHANCED: return EItemCollectionState::Enhanced;
	case b2network::B2CollectionItemState::REWARDED: return EItemCollectionState::Rewarded;
	}
	return EItemCollectionState::None;
}

enum class ECollectionType : uint8
{
	None,
	SetItem,
	Weapon,
	Armor,
	Accessory,
	Wing,

	Max
};

inline ECollectionType SvrToCliCollectionCategory(int32 ServerCategory)
{
	switch (ServerCategory)
	{
	case b2network::B2EquipCategory::WEAPON: return ECollectionType::Weapon;
	case b2network::B2EquipCategory::ARMOR: return ECollectionType::Armor;
	case b2network::B2EquipCategory::ACCESSORY: return ECollectionType::Accessory;
	}

	return ECollectionType::None;
}

inline int32 CliToSvrCollectionCategory(ECollectionType CliCollectionType)
{
	switch (CliCollectionType)
	{
	case ECollectionType::Weapon: return b2network::B2EquipCategory::WEAPON;
	case ECollectionType::Armor: return b2network::B2EquipCategory::ARMOR;
	case ECollectionType::Accessory: return b2network::B2EquipCategory::ACCESSORY;
	}

	return 0;
}

inline TArray<EItemClass> GetItemClassListByCollectionType(ECollectionType CollectionType)
{
	TArray<EItemClass> ItemClassList;
	switch (CollectionType)
	{
	case ECollectionType::SetItem: ItemClassList.Add(EItemClass::EIC_SetItem); break;
	case ECollectionType::Weapon: ItemClassList.Add(EItemClass::EIC_Weapon); break;
	case ECollectionType::Wing: ItemClassList.Add(EItemClass::EIC_Wing); break;
	case ECollectionType::Armor:
	{
		ItemClassList.Add(EItemClass::EIC_Helmet);
		ItemClassList.Add(EItemClass::EIC_BodyArmor);
		ItemClassList.Add(EItemClass::EIC_Glove);
		ItemClassList.Add(EItemClass::EIC_Shoes);
		break;
	}
	case ECollectionType::Accessory:
	{
		ItemClassList.Add(EItemClass::EIC_Belt);
		ItemClassList.Add(EItemClass::EIC_Necklace);
		ItemClassList.Add(EItemClass::EIC_Ring);
		break;
	}
	}

	return ItemClassList;
}

enum class MailTabIndex : uint8
{
	EMailTab_Gift = b2network::B2MailCategory::PRESENT,
	EMailTab_BladePoint = b2network::B2MailCategory::BLADE_POINT,
	EMailTab_Friendship = b2network::B2MailCategory::SOCIAL_POINT,
	EMailTab_Ticket = b2network::B2MailCategory::LOTTERY,
	EMailTab_Max
};

enum class MailWhatCause : uint8
{
	Attendance = b2network::B2MailSourceType::ATTENDANCE,
	PlayTime = b2network::B2MailSourceType::PLAY_TIME,
	EventAttendance = b2network::B2MailSourceType::EVENT_ATTENDANCE,
	DailyMission = b2network::B2MailSourceType::DAILY_MISSION,
	WeeklyMission = b2network::B2MailSourceType::WEEKLY_MISSION,
	MonthlyMission = b2network::B2MailSourceType::MONTHLY_MISSION,
	SerialMission = b2network::B2MailSourceType::SERIAL_MISSION,
	MissionCompletePointReward = b2network::B2MailSourceType::MISSION_COMPLETE_POINT_REWARD,
	DailyQuest = b2network::B2MailSourceType::DAILY_QUEST,
	MainQuest = b2network::B2MailSourceType::MAIN_QUEST,
	SubQuest = b2network::B2MailSourceType::SUB_QUEST,
	PvpWin = b2network::B2MailSourceType::PVP_RESULT,
	PvpRewardBox = b2network::B2MailSourceType::PVP_REWARD_BOX,
	PvpRanking = b2network::B2MailSourceType::PVP_RANKING,
	CounterDungeonWin = b2network::B2MailSourceType::COUNTER_DUNGEON_WIN,
	CounterDungeonWeekly = b2network::B2MailSourceType::COUNTER_DUNGEON_WEEKLY,
	TeamMatchWin = b2network::B2MailSourceType::TEAM_MATCH_RESULT,
	TeamMatchRewardBox = b2network::B2MailSourceType::TEAM_MATCH_REWARD_BOX,
	TeamMatchRanking = b2network::B2MailSourceType::TEAM_MATCH_RANKING,
	HeroTower = b2network::B2MailSourceType::HERO_TOWER,
	SourceRaid = b2network::B2MailSourceType::SOURCE_RAID,
	Tutorial = b2network::B2MailSourceType::TUTORIAL,
	AssaultWin = b2network::B2MailSourceType::ASSAULT_RESULT,
	AssaultRewardBox = b2network::B2MailSourceType::ASSAULT_REWARD_BOX,
	AssaultRanking = b2network::B2MailSourceType::ASSAULT_RANKING,
	AssaultMvp = b2network::B2MailSourceType::ASSAULT_MVP,
	GuildBattle = b2network::B2MailSourceType::GUILD_BATTLE,
	GuildRewardBox = b2network::B2MailSourceType::GUILD_REWARD_BOX,
	GuildSupport = b2network::B2MailSourceType::GUILD_SUPPORT,
	Friend = b2network::B2MailSourceType::FRIEND,
	Collect = b2network::B2MailSourceType::COLLECT,
	ActClear = b2network::B2MailSourceType::ACT_CLEAR,
	BuyShop = b2network::B2MailSourceType::BUY_SHOP,
	ShopDrawMileage = b2network::B2MailSourceType::SHOP_DRAW_MILEAGE,
	ShopBuyMileage = b2network::B2MailSourceType::SHOP_BUY_MILEAGE,
	Push = b2network::B2MailSourceType::PUSH,
	MainTenance = b2network::B2MailSourceType::MAINTENANCE,
	Event = b2network::B2MailSourceType::EVENT,
	Admin = b2network::B2MailSourceType::ADMIN,
	UnitySkill = b2network::B2MailSourceType::UNITY_SKILL_MISSION,
	HeroTowerSweep = b2network::B2MailSourceType::HERO_TOWER_SWEEP,
	Rest = b2network::B2MailSourceType::REST,
	Coupon = b2network::B2MailSourceType::COUPON,
	Promotion = b2network::B2MailSourceType::PROMOTION,
	KakaoFriendInvite = b2network::B2MailSourceType::KAKAO_FRIEND_INVITE,
	ShopFlatratePackageBonus = b2network::B2MailSourceType::SHOP_FLAT_RATE_PACKAGE_BONUS,
	BuyMagicShop = b2network::B2MailSourceType::BUY_MAGIC_SHOP,
	FamePraiseReward = b2network::B2MailSourceType::FAME_PRAISE_REWARD,
	FamePraiseRankingReward = b2network::B2MailSourceType::FAME_RAISED_RANKING_REWARD,
	FameRankerReward = b2network::B2MailSourceType::FAME_RANKER_REWARD,
	BeginnerAttendanceReward = b2network::B2MailSourceType::BEGINNER_ATTENDANCE,
	MakeDonationReward = b2network::B2MailSourceType::MAKE_DONATION,
	DonationPointReward = b2network::B2MailSourceType::DONATION_POINT_REWARD,
	DonationPointRankingReward = b2network::B2MailSourceType::DONATION_POINT_RANKING,
	DimensionRankingReward = b2network::B2MailSourceType::DIMENSION_RANKING,
	FairyGift = b2network::B2MailSourceType::FAIRY_GIFT,
	NewAccountReward = b2network::B2MailSourceType::WELCOME,
	GuildOut = b2network::B2MailSourceType::GUILD_REFUND,
};

UENUM()
enum class MailSenderType : uint8
{
	Arin = 1,	//b2network::B2MailSenderType::ARIN,
	Hunt = 2,	//b2network::B2MailSenderType::HUNT,
	Gaius = 3,	//b2network::B2MailSenderType::GAIUS,
	Bella = 4,	//b2network::B2MailSenderType::BELLA,
	Luchi = 5,	//b2network::B2MailSenderType::LUCHI,
	Ramiel = 6,	//b2network::B2MailSenderType::RAMIEL,
	Admin = 7,	//b2network::B2MailSenderType::ADMIN,
	Gratia = 8,	//b2network::B2MailSenderType::GRATIA,
	Brokk = 9, //b2network::B2MailSenderType::BROKK;
};

UENUM()
enum class MailGiftType : uint8
{
	Gold = 1,						//b2network::B2MailAttachType::GOLD,
	Gem = 2,						//b2network::B2MailAttachType::GEM,
	Item = 3,						//b2network::B2MailAttachType::ITEM,
	BladePoint = 4,					//b2network::B2MailAttachType::BLADE_POINT,
	SocialPoint = 5,				//b2network::B2MailAttachType::SOCIAL_POINT,
	LotteryTicket = 6,				//b2network::B2MailAttachType::GENERAL_LOTTERY,
	Selective_Lottery = 7,			//b2network::B2MailAttachType::SELECTIVE_LOTTERY
	Selective_Item_Lottery = 8,		//b2network::B2MailAttachType::SELECTIVE_ITEM_LOTTERY
	Hero_Piece = 9,					//b2network::B2MailAttachType::ATTACH_HERO_PIECE,
	Rank_Promotion_Point = 10,		//b2network::B2MailAttachType::ATTACH_RANK_PROMOTION_POINT
	Dark_Crystal = 11,				//b2network::B2MailAttachType::ATTACH_DARK_CRYSTAL
	Fixed_Grade_Lottey = 12,		//b2network::B2MailAttachType::ATTACH_FIXED_GRADE_LOTTERY
	Stage_Boost_Ticket = 13,		//b2network::B2MailAttachType::ATTACH_STAGE_BOOST_TICKET
	Hot_Time_Gold_Buff_Ticket = 14,	//b2network::B2MailAttachType::ATTACH_HOT_TIME_GOLD_BUFF_TICKET
	Hot_Time_Exp_Buff_Ticket = 15,	//b2network::B2MailAttachType::ATTACH_HOT_TIME_EXP_BUFF_TICKET
	Shop_Ten_Lottery_Ticket = 16,	//b2network::B2MailAttachType::ATTACH_SHOP_TEN_LOTTERY_TICKET
	Fame_Point = 17,				//b2network::B2MailAttachType::ATTACH_FAME_POINT
	Selective_Ether_Lottery = 18,   //b2network::B2MailAttachType::ATTACH
	Prefix_Selective_Lottery = 19,  //b2network::B2MailAttachType::PREFIX_SELECTIVE_LOTTERY
	Stage_Auto_Clear_Ticket = 20,	//b2network::B2MailAttachType::STAGE_AUTO_CLEAR_TICKET
	Prefix_Totem_Lottery = 21,	//b2network::B2MailAttachType::TOTEM_PREFIX_LOTTERY
	Advanced_Rank_Promotion_Point = 22,	//b2network::B2MailAttachType::ADVANCED_RANK_PROMOTION_POINT
};

UENUM()
enum class EGeneralLotteryGrade : uint8
{
	None = 0,
	Normal = 1, //b2network::B2LotteryGrade::NORMAL,
	Special = 2, //b2network::B2LotteryGrade::SPECIAL,
	Premium = 3, //b2network::B2LotteryGrade::PREMIUM,
	Signature = 4, //b2network::B2LotteryGrade::SIGNATURE
	PLATINUM = 5, //b2network::B2LotteryGrade::PLATINUM
};

UENUM()
enum class ELotteryItemCategory : uint8
{
	None = 0,
	Weapon = 1,	//b2network::B2EquipCategory::WEAPON
	Armor = 2,	//b2network::B2EquipCategory::ARMOR
	Accessory = 3,	//b2network::B2EquipCategory::ACCESSORY
};

enum class EDailyAttendanceType
{
	NONE,
	GENERAL = b2network::B2DailyAttendanceType::GENERAL,
	RETURNED = b2network::B2DailyAttendanceType::RETURNED,
	BEGINNER = b2network::B2DailyAttendanceType::BEGINNER,
};

enum class ERewardType : uint8
{
	None = 0,
	Gold = b2network::B2RewardType::GOLD,
	Gem = b2network::B2RewardType::GEM,
	BladePoint = b2network::B2RewardType::BLADE_POINT,
	Item = b2network::B2RewardType::ITEM,
	General_Lottery = b2network::B2RewardType::GENERAL_LOTTERY,
	Selective_Lottery = b2network::B2RewardType::SELECTIVE_LOTTERY,
	Buff_Ticket = b2network::B2RewardType::BUFF_TICKET,
	Hero_Piece = b2network::B2RewardType::HERO_PIECE,
	Selective_Item_Lottery = b2network::B2RewardType::SELECTIVE_ITEM_LOTTERY,
	Rank_Promotion_Point = b2network::B2RewardType::RANK_PROMOTION_POINT,
	Advanced_Rank_Promotion_Point = b2network::B2RewardType::ADVANCED_RANK_PROMOTION_POINT,
	ExperiencePoint = b2network::B2RewardType::EXP,
	Social_Point = b2network::B2RewardType::SOCIAL_POINT,
	Dark_Crystal = b2network::B2RewardType::DARK_CRYSTAL,
	Fixed_Grade_Lottey = b2network::B2RewardType::FIXED_GRADE_LOTTERY,
	Stage_Boost_Ticket = b2network::B2RewardType::STAGE_BOOST_TICKET,
	Hot_Time_Gold_Buff_Ticket = b2network::B2RewardType::HOT_TIME_GOLD_BUFF_TICKET,
	Hot_Time_Exp_Buff_Ticket = b2network::B2RewardType::HOT_TIME_EXP_BUFF_TICKET,
	Shop_Ten_Lottery_Ticket = b2network::B2RewardType::SHOP_TEN_LOTTERY_TICKET,
	Fame_Point = b2network::B2RewardType::FAME_POINT,
	Prefix_Selective_Lottery = b2network::B2RewardType::PREFIX_SELECTIVE_LOTTERY,
	Stage_Auto_Clear_Ticket = b2network::B2RewardType::STAGE_AUTO_CLEAR_TICKET,
};

inline ERewardType MailGiftToRewardType(MailGiftType MailType)
{
	switch (MailType)
	{
	case MailGiftType::Gold: return ERewardType::Gold;
	case MailGiftType::Gem: return ERewardType::Gem;
	case MailGiftType::Item: return ERewardType::Item;
	case MailGiftType::BladePoint: return ERewardType::BladePoint;
	case MailGiftType::SocialPoint: return ERewardType::Social_Point;
	case MailGiftType::LotteryTicket: return ERewardType::General_Lottery;
	case MailGiftType::Selective_Lottery: return ERewardType::Selective_Lottery;
	case MailGiftType::Selective_Item_Lottery: return ERewardType::Selective_Item_Lottery;
	case MailGiftType::Hero_Piece: return ERewardType::Hero_Piece;
	case MailGiftType::Rank_Promotion_Point: return ERewardType::Rank_Promotion_Point;
	case MailGiftType::Dark_Crystal: return ERewardType::Dark_Crystal;
	case MailGiftType::Fixed_Grade_Lottey: return ERewardType::Fixed_Grade_Lottey;
	case MailGiftType::Stage_Boost_Ticket: return ERewardType::Stage_Boost_Ticket;
	case MailGiftType::Hot_Time_Gold_Buff_Ticket: return ERewardType::Hot_Time_Gold_Buff_Ticket;
	case MailGiftType::Hot_Time_Exp_Buff_Ticket: return ERewardType::Hot_Time_Exp_Buff_Ticket;
	case MailGiftType::Shop_Ten_Lottery_Ticket: return ERewardType::Shop_Ten_Lottery_Ticket;
	case MailGiftType::Fame_Point: return ERewardType::Fame_Point;
	case MailGiftType::Prefix_Selective_Lottery: return ERewardType::Prefix_Selective_Lottery;
	case MailGiftType::Advanced_Rank_Promotion_Point: return ERewardType::Advanced_Rank_Promotion_Point;
	}

	return ERewardType::Gold;
}

enum class EStorePageWhere : uint8
{
	Store = 0,
	EquipStore = 1,
	PackageStore = 2,
	MagicStore = 3,
};

enum class EDocUserDataType : uint8
{
	None = 0,
	Gold = 1,
	Gem = 2,
	DarkGem = 3,
	HeroPiece = 4,
	BladePoint = 5,
	SocialPoint = 6,
	RankPromotionPoint = 7,
	StageBoostTicket = 8,
	FamePoint = 9,
	StageAutoClearTicket = 10,
	DimensionPiece = 11,
	AdvancedRankPromotionPoint = 12,

	AllRefresh,
};

enum class EAttendanceTab : int32
{
	None = -1,
	Attendance = 0,
	EventNewUser,
	ConnectReward,
	EventAttendance,
	Event_2,
	Max,
};

struct FResurrectData
{
	int32 MaxResurrectionChance;
	int32 RemainingResurrectionChance;
	int32 ResurrectionCost;

	void Clear()
	{
		MaxResurrectionChance = 0;
		RemainingResurrectionChance = 0;
		ResurrectionCost = 0;
	}

	void ClearResurrectionChange(int32 MaxChance)
	{
		MaxResurrectionChance = MaxChance;
		RemainingResurrectionChance = MaxChance;
	}

	void DecreaseResurrectionChance()
	{
		if (RemainingResurrectionChance <= 0)
			return;

		--RemainingResurrectionChance;
	}
};

/** Common state variables set on RepeatBattle both for lobby and stage. */
struct FRepeatBattleStateSet
{
	FRepeatBattleStateSet()
	{
		bIsOn = false;
		bRepeatOne = false;
		bRepeatAll = false;
		bBoostOn = false;
		CurrentRepeatCount = 0;
		RemainingBoostCount = 0;
	}
	bool bIsOn;
	bool bRepeatOne;
	bool bRepeatAll;
	bool bBoostOn;
	int32 CurrentRepeatCount;
	int32 RemainingBoostCount;
};
const int32 B2_REPEAT_BATTLE_BOOST_FACTOR = 3;

/** Player's weak part for current selected stage combat, to get proper Arin consulting. */
UENUM()
enum class EArinConsultPreCombat_MainSubject : uint8
{
	ARCPCMS_None, // Nothing to improve, no Arin consulting.
	ARCPCMS_Attack,
	ARCPCMS_Defense,
	ARCPCMS_Skill
};

UENUM()
enum class EResurrectBuffType : uint8
{
	RBT_None = 0,
	RBT_Attack = 1,
	RBT_Defense = 2,
	RBT_Health = 3,
	RBT_End
};

inline EResurrectBuffType SvrToCliResurrectBuffType(int32 ServerRBT)
{
	switch (ServerRBT)
	{
	case b2network::B2ResurrectBuffType::BRBT_Attack: return EResurrectBuffType::RBT_Attack;
	case b2network::B2ResurrectBuffType::BRBT_Defense: return EResurrectBuffType::RBT_Defense;
	case b2network::B2ResurrectBuffType::BRBT_Health: return EResurrectBuffType::RBT_Health;
	}
	return EResurrectBuffType::RBT_None;
}

inline int32 CliToSvrResurrectBuffType(EResurrectBuffType CliRBT)
{
	switch (CliRBT)
	{
	case EResurrectBuffType::RBT_Attack: return b2network::B2ResurrectBuffType::BRBT_Attack;
	case EResurrectBuffType::RBT_Defense: return b2network::B2ResurrectBuffType::BRBT_Defense;
	case EResurrectBuffType::RBT_Health: return b2network::B2ResurrectBuffType::BRBT_Health;
	}
	return b2network::B2ResurrectBuffType::BRBT_None;
}

UENUM()
enum class EStageBuffType : uint8
{
	SBT_IncreaseExp = 0,
	SBT_IncreaseGold = 1,
	SBT_AutoSkill = 2,
	SBT_BoostGameSpeed = 3,
	SBT_End = 4
};

inline EStageBuffType SvrToCliStageBuffType(int32 ServerSBT)
{
	switch (ServerSBT)
	{
	case b2network::B2StageBuffFlag::INCREASE_EXP: return EStageBuffType::SBT_IncreaseExp;
	case b2network::B2StageBuffFlag::INCREASE_GOLD: return EStageBuffType::SBT_IncreaseGold;
	case b2network::B2StageBuffFlag::AUTO_SKILL: return EStageBuffType::SBT_AutoSkill;
	case b2network::B2StageBuffFlag::BOOST_GAME_SPEED: return EStageBuffType::SBT_BoostGameSpeed;
	}
	return EStageBuffType::SBT_End;
}
/** Server's stage buff type can be expressed as a single number for multiple selection. */
inline void SvrToCliStageBuffTypeComposit(int32 InServerSBT, TArray<EStageBuffType>& OutAllCliSBT)
{
	if (InServerSBT & b2network::B2StageBuffFlag::INCREASE_EXP)
	{
		OutAllCliSBT.Add(EStageBuffType::SBT_IncreaseExp);
	}
	if (InServerSBT & b2network::B2StageBuffFlag::INCREASE_GOLD)
	{
		OutAllCliSBT.Add(EStageBuffType::SBT_IncreaseGold);
	}
	if (InServerSBT & b2network::B2StageBuffFlag::AUTO_SKILL)
	{
		OutAllCliSBT.Add(EStageBuffType::SBT_AutoSkill);
	}
	if (InServerSBT & b2network::B2StageBuffFlag::BOOST_GAME_SPEED)
	{
		OutAllCliSBT.Add(EStageBuffType::SBT_BoostGameSpeed);
	}
}

inline int32 CliToSvrStageBuffType(EStageBuffType CliSBT)
{
	switch (CliSBT)
	{
	case EStageBuffType::SBT_IncreaseExp: return b2network::B2StageBuffFlag::INCREASE_EXP;
	case EStageBuffType::SBT_IncreaseGold: return b2network::B2StageBuffFlag::INCREASE_GOLD;
	case EStageBuffType::SBT_AutoSkill: return b2network::B2StageBuffFlag::AUTO_SKILL;
	case EStageBuffType::SBT_BoostGameSpeed: return b2network::B2StageBuffFlag::BOOST_GAME_SPEED;
	}
	return 0;
}
int32 CliToSvrStageBuffTypeComposit(const TArray<EStageBuffType>& AllCliSBT);

UENUM(BlueprintType)
enum class ETMFormation : uint8
{
	ETM_Normal = 0,
	ETM_Attack,
	ETM_Defense,
	ETM_End
};

enum class EFormationNUM : uint8
{
	None = 0,
	Front = 1,
	Center = 2,
	Back = 3,
	Left = 4,
	Right = 5,
};

inline ETMFormation SvrToCliFormationType(int32 ServerType)
{
	switch (ServerType)
	{
	case b2network::B2BattleFormationType::BTBFT_Normal: return ETMFormation::ETM_Normal;
	case b2network::B2BattleFormationType::BTBFT_Offensive: return ETMFormation::ETM_Attack;
	case b2network::B2BattleFormationType::BTBFT_Defensive: return ETMFormation::ETM_Defense;
	}
	return ETMFormation::ETM_End;
}

inline int32 CliToSvrFormationType(ETMFormation ClientType)
{
	switch (ClientType)
	{
	case ETMFormation::ETM_Normal:	return b2network::B2BattleFormationType::BTBFT_Normal;
	case ETMFormation::ETM_Attack:	return b2network::B2BattleFormationType::BTBFT_Offensive;
	case ETMFormation::ETM_Defense:	return b2network::B2BattleFormationType::BTBFT_Defensive;
	}
	return 0;
}

const int32 B2_CHAR_NICKNAME_MIN_LENGTH = 2;
const int32 B2_CHAR_NICKNAME_MAX_LENGTH = 8;

UENUM(BlueprintType)
enum class EBrevetStatusType : uint8
{
	mNULL = 0,
	EBST_Atk = 1,
	EBST_Def = 2,
	EBST_Hp = 3,
	EBST_End
};

UENUM(BlueprintType)
enum class EB2GameMode : uint8
{
	Unknown,
	Lobby,
	Scenario,
	CounterDungeon,
	HeroTower,
	PVP_Tag,
	PVP_Team,
	Raid,
	Control,
	Tutorial,
	Guild,
	PVPReplay,
	Dimension,

	PreRender, // Not for playing, but still one of game-mode

	End
};

using FB2ResponseGetQuest = b2network::FB2MessageInfoResponseGetQuest;
using FB2ResponseGetQuestPtr = b2network::FB2MessageInfoResponseGetQuestPtr;
using FB2GetQuest = TU::MakeTuple<FB2ResponseGetQuest::type_list>::Result;

using FB2ResponseAcceptQuest = b2network::FB2MessageInfoResponseAcceptQuest;
using FB2ResponseAcceptQuestPtr = b2network::FB2MessageInfoResponseAcceptQuestPtr;
using FB2AcceptQuest = TU::MakeTuple<FB2ResponseAcceptQuest::type_list>::Result;

using FB2ResponseReceiveQuestReward = b2network::FB2MessageInfoResponseReceiveQuestReward;
using FB2ResponseReceiveQuestRewardPtr = b2network::FB2MessageInfoResponseReceiveQuestRewardPtr;
using FB2ReceiveQuestReward = TU::MakeTuple<FB2ResponseReceiveQuestReward::type_list>::Result;

using FB2GiveUpQuestPtr = b2network::FB2MessageInfoResponseGiveUpQuestPtr;

using FB2ResponseFinishCounterDungeon = b2network::FB2MessageInfoResponseFinishCounterDungeon;
using FB2ResponseFinishCounterDungeonPtr = b2network::FB2MessageInfoResponseFinishCounterDungeonPtr;
using FB2FinishCounterDungeonInfo = TU::MakeTuple<FB2ResponseFinishCounterDungeon::type_list>::Result;

using FB2ResponseSweepCounterDungeon = b2network::FB2MessageInfoResponseSweepCounterDungeon;
using FB2ResponseSweepCounterDungeonPtr = b2network::FB2MessageInfoResponseSweepCounterDungeonPtr;
using FB2SweepCounterDungeonInfo = TU::MakeTuple<FB2ResponseSweepCounterDungeon::type_list>::Result;

using FB2ResponseStartCounterDungeon = b2network::FB2MessageInfoResponseStartCounterDungeonPtr;
using FB2ResponseForceStartCounterDungeonPtr = b2network::FB2MessageInfoResponseForceStartCounterDungeonPtr;

using FB2ResponseCounterDungeonWeeklyReward = b2network::FB2MessageInfoResponseCounterDungeonWeeklyReward;
using FB2ResponseCounterDungeonWeeklyRewardPtr = b2network::FB2MessageInfoResponseCounterDungeonWeeklyRewardPtr;
using FB2CounterDungeonWeeklyReward = TU::MakeTuple<FB2ResponseCounterDungeonWeeklyReward::type_list>::Result;

using FB2ResponseCounterDungeonBuyTicket = b2network::FB2MessageInfoResponseBuyCounterDungeonTicket;
using FB2ResponseCounterDungeonBuyTicketPtr = b2network::FB2MessageInfoResponseBuyCounterDungeonTicketPtr;
using FB2CounterDungeonBuyTicket = TU::MakeTuple<FB2ResponseCounterDungeonBuyTicket::type_list>::Result;

using FB2ResponseGetDimensionPtr = b2network::FB2MessageInfoResponseGetDimensionPtr;
using FB2ResponseStartDimensionPtr = b2network::FB2MessageInfoResponseStartDimensionPtr;
using FB2ResponseFinishDimensionPtr = b2network::FB2MessageInfoResponseFinishDimensionPtr;

struct FB2QuestReward
{
	FB2QuestReward()
		: receive_gold(0)
		, receive_gem(0)
		, receive_blade_point(0)
		, receive_rank_promotion_point(0)
	{
		receive_item_ids.Empty();
		receive_selective_lottery_ids.Empty();
		receive_general_lottery_ids.Empty();
	}

	int32			receive_gold;
	int32			receive_gem;
	int32			receive_blade_point;
	int32			receive_rank_promotion_point;
	TArray<int32>	receive_item_ids;
	TArray<int32>	receive_selective_lottery_ids;
	TArray<int32>	receive_general_lottery_ids;

	int32			rewardid;
};
using FB2QuestRewardPtr = std::shared_ptr<FB2QuestReward>;

using FB2ResponseGetCounterDungeonStatus = b2network::FB2MessageInfoResponseGetCounterDungeon;
using FB2ResponseGetCounterDungeonStatusPtr = b2network::FB2MessageInfoResponseGetCounterDungeonPtr;
using FB2GetCounterDungeon = TU::MakeTuple<FB2ResponseGetCounterDungeonStatus::type_list>::Result;

using FB2ResponseGetMission = b2network::FB2MessageInfoResponseGetMission;
using FB2ResponseGetMissionPtr = b2network::FB2MessageInfoResponseGetMissionPtr;
using FB2GetMission = TU::MakeTuple<FB2ResponseGetMission::type_list>::Result;

using FB2ResponseReceiveMissionReward = b2network::FB2MessageInfoResponseReceiveMissionReward;
using FB2ResponseReceiveMissionRewardPtr = b2network::FB2MessageInfoResponseReceiveMissionRewardPtr;
using FB2ReceiveMissionReward = TU::MakeTuple<FB2ResponseReceiveMissionReward::type_list>::Result;

using FB2ResponseReceiveCompleteMissionReward = b2network::FB2MessageInfoResponseReceiveCompleteMissionReward;
using FB2ResponseReceiveCompleteMissionRewardPtr = b2network::FB2MessageInfoResponseReceiveCompleteMissionRewardPtr;
using FB2ReceiveCompleteMissionReward = TU::MakeTuple<FB2ResponseReceiveCompleteMissionReward::type_list>::Result;

const int B2_GAME_FRIEND_MAX_COUNT = 50;
const int B2_FRIEND_ASK_RECEIVE_MAX_COUNT = 50;
const int B2_FRIEND_ASK_SEND_MAX_COUNT = 50;
const int B2_RECOMMEND_FRIEND_REFRESH_COOLTIME_SEC = 3;

UENUM(BlueprintType)
enum class EStageDifficulty : uint8
{
	ESD_None = 0,
	ESD_Normal = 1,	// Story Mode
	ESD_Hard = 2,
	ESD_VeryHard = 3,
	ESD_Hell = 4,
	ESD_End,

	// Check for spots using ESD_Hell as the last difficulty identifier, if we've got to add additional difficulty level, ever.
	// I would suggest ESD_Shitty for harder one.
};
FORCEINLINE EStageDifficulty GetOneEasierStageDifficulty(EStageDifficulty InTestDifficulty)
{
	return (InTestDifficulty > EStageDifficulty::ESD_Normal) ? static_cast<EStageDifficulty>(static_cast<int32>(InTestDifficulty) - 1) : EStageDifficulty::ESD_Normal;
}
FORCEINLINE EStageDifficulty GetOneHarderStageDifficulty(EStageDifficulty InTestDifficulty)
{
	return (InTestDifficulty < EStageDifficulty::ESD_Hell) ? static_cast<EStageDifficulty>(static_cast<int32>(InTestDifficulty) + 1) : EStageDifficulty::ESD_Hell;
}

inline int32 CliToSvrStageDifficulty(EStageDifficulty difficulty)
{
	switch (difficulty)
	{
	case EStageDifficulty::ESD_Normal:
		return b2network::B2StageDifficulty::NORMAL;
	case EStageDifficulty::ESD_Hard:
		return b2network::B2StageDifficulty::HARD;
	case EStageDifficulty::ESD_VeryHard:
		return b2network::B2StageDifficulty::VERY_HARD;
	case EStageDifficulty::ESD_Hell:
		return b2network::B2StageDifficulty::HELL;
	}
	return b2network::B2StageDifficulty::NORMAL;
}

inline EStageDifficulty SvrToCliStageDifficulty(int32 difficulty)
{
	switch (difficulty)
	{
	case b2network::B2StageDifficulty::NORMAL:
		return EStageDifficulty::ESD_Normal;
	case b2network::B2StageDifficulty::HARD:
		return EStageDifficulty::ESD_Hard;
	case b2network::B2StageDifficulty::VERY_HARD:
		return EStageDifficulty::ESD_VeryHard;
	case b2network::B2StageDifficulty::HELL:
		return EStageDifficulty::ESD_Hell;
	}
	return EStageDifficulty::ESD_Normal;
}

//inline EB2GameMode SvrDuelModeToCliGameMode(int32 InDuelMode)
//{
//	// Not exactly one to one mapping. b2network::B2DuelMode is not for all kind of GameMode type..
//	switch (InDuelMode)
//	{
//	case b2network::B2DuelMode::DUEL_MODE_PVP: return EB2GameMode::PVP_Tag;
//	case b2network::B2DuelMode::DUEL_MODE_TEAM: return EB2GameMode::PVP_Team;
//	case b2network::B2DuelMode::DUEL_MODE_ASSAULT: return EB2GameMode::Control;
//	}
//	return  EB2GameMode::Unknown;
//}
//
//inline int32 CliGameModeToSvrDuelMode(EB2GameMode InGameMode)
//{
//	// Not exactly one to one mapping. b2network::B2DuelMode is not for all kind of GameMode type..
//	switch (InGameMode)
//	{
//	case EB2GameMode::PVP_Tag: return b2network::B2DuelMode::DUEL_MODE_PVP;
//	case EB2GameMode::PVP_Team: return b2network::B2DuelMode::DUEL_MODE_TEAM;
//	case EB2GameMode::Control: return b2network::B2DuelMode::DUEL_MODE_ASSAULT;
//	}
//	return b2network::B2DuelMode::DUEL_MODE_NONE;
//}

using FB2ResponseReciveCreateGuild = b2network::FB2MessageInfoResponseCreateGuild;
using FB2ResponseReciveCreateGuildPtr = b2network::FB2MessageInfoResponseCreateGuildPtr;
using FB2ReceiveCreateGuild = TU::MakeTuple<FB2ResponseReciveCreateGuild::type_list>::Result;

using FB2ResponseGuildInfoPtr = b2network::B2GuildPtr;

using FB2ResponseGuildDetailInfo = b2network::FB2MessageInfoResponseGetGuildDetailInfo;
using FB2ResponseGuildDetailInfoPtr = b2network::FB2MessageInfoResponseGetGuildDetailInfoPtr;
using FB2ReceiveGuildInfo = TU::MakeTuple<FB2ResponseGuildDetailInfo::type_list>::Result;

using FB2ResponseeRecommenGuild = b2network::FB2MessageInfoResponseGetRecommendGuild;
using FB2ResponseeRecommenGuildPtr = b2network::FB2MessageInfoResponseGetRecommendGuildPtr;
using FB2ReciveRecommenGuild = TU::MakeTuple<FB2ResponseeRecommenGuild::type_list>::Result;

using FB2ResponseeAppointSubGuildMaster = b2network::FB2MessageInfoResponseAppointSubGuildMaster;
using FB2ResponseeAppointSubGuildMasterPtr = b2network::FB2MessageInfoResponseAppointSubGuildMasterPtr;
using FB2ReciveAppointSubGuildMasterInfo = TU::MakeTuple<FB2ResponseeAppointSubGuildMaster::type_list>::Result;

using FB2ResponseeDismissSubGuildMaster = b2network::FB2MessageInfoResponseDismissSubGuildMaster;
using FB2ResponseeDismissSubGuildMasterPtr = b2network::FB2MessageInfoResponseDismissSubGuildMasterPtr;
using FB2ReciveDismissSubGuildMasterInfo = TU::MakeTuple<FB2ResponseeDismissSubGuildMaster::type_list>::Result;

using FB2ResponseGetGuildBattle = b2network::FB2MessageInfoResponseGetGuildBattle;
using FB2ResponseGetGuildBattlePtr = b2network::FB2MessageInfoResponseGetGuildBattlePtr;
using FB2ReciveGetGuildBattleInfo = TU::MakeTuple<FB2ResponseGetGuildBattle::type_list>::Result;

using FB2ResponseDailyAttendance = b2network::FB2MessageInfoResponseDailyAttendance;
using FB2ResponseDailyAttendancePtr = b2network::FB2MessageInfoResponseDailyAttendancePtr;
using FB2ReceiveDailyAttendance = TU::MakeTuple<FB2ResponseDailyAttendance::type_list>::Result;

using FB2ResponseStartGuildBattlePtr = b2network::FB2MessageInfoResponseStartGuildBattlePtr;

using FB2ResponseSetGuildBattlePtr = b2network::FB2MessageInfoResponseSetGuildBattleEntryPtr;
using FB2ResponseFinishGuildBattlePtr = b2network::FB2MessageInfoResponseFinishGuildBattlePtr;

using FB2ResponseGuildBattleTurnResultPtr = b2network::FB2MessageInfoResponseGuildBattleTurnResultPtr;
using FB2ResponseGuildBattleTurnRewardPtr = b2network::FB2MessageInfoResponseGuildBattleTurnRewardPtr;

using FB2ResponseGetFriendRankingsPtr = b2network::FB2MessageInfoResponseGetFriendRankingsPtr;

using FB2ResponseGetGuildInviteListPtr = b2network::FB2MessageInfoResponseGetGuildInviteListPtr;

using FB2ResponseLeaveGuildPtr = b2network::FB2MessageInfoResponseLeaveGuildPtr;

using FB2ResponseGetGuildMemberCandidatesPtr = b2network::FB2MessageInfoResponseGetGuildMemberCandidatesPtr;

using FB2ResponseGuildBattleHistoryPtr = b2network::FB2MessageInfoResponseGuildBattleHistoryPtr;

using FB2ResponseGuildBattleTurnRewardPtr = b2network::FB2MessageInfoResponseGuildBattleTurnRewardPtr;

using FB2ResponseUpdateGuildPtr = b2network::FB2MessageInfoResponseUpdateGuildPtr;

using FB2ResponseUpdateGuildNoticePtr = b2network::FB2MessageInfoResponseUpdateGuildNoticePtr;

using FB2ResponseFindGuildPtr = b2network::FB2MessageInfoResponseFindGuildPtr;

using FB2ResponseGetGuildRankingListPtr = b2network::FB2MessageInfoResponseGetGuildRankingListPtr;

using FB2ResponseGuildBattleSeasonRewardPtr = b2network::FB2MessageInfoResponseGuildBattleSeasonRewardPtr;

using FB2ResponseLevelUpGuildSkillPtr = b2network::FB2MessageInfoResponseLevelUpGuildSkillPtr;

using FB2ResponseBuyGuildSkillPtr = b2network::FB2MessageInfoResponseBuyGuildSkillPtr;
using FB2ResponseObtainGuildMasterPtr = b2network::FB2MessageInfoResponseObtainGuildMasterPtr;
using FB2ResponseHandOverGuildMasterPtr = b2network::FB2MessageInfoResponseHandOverGuildMasterPtr;

using B2UnitySkillMissionPtr = b2network::B2UnitySkillMissionPtr;
using FB2ResponseGetUnitySkillMissionPtr = b2network::FB2MessageInfoResponseGetUnitySkillMissionPtr;
using FB2ResponseUnitySkillRewardPtr = b2network::FB2MessageInfoResponseReceiveUnitySkillRewardPtr;
using FB2ResponseAwakenUnitySkillMissionPtr = b2network::FB2MessageInfoResponseAwakenUnitySkillMissionPtr;
using FB2ResponseAwakenUnitySkillPtr = b2network::FB2MessageInfoResponseAwakenUnitySkillPtr;

using FB2ResponseGetContentsOpenStatusPtr = b2network::FB2MessageInfoResponseGetContentsOpenStatusPtr;

using FB2ResponseCheckModeOpenPtr = b2network::FB2MessageInfoResponseCheckModeOpenPtr;
using FB2ResponseRestRewardPtr = b2network::FB2MessageInfoResponseReceiveRestRewardPtr;

using FB2GuildMemberPtr = b2network::B2GuildMemberPtr;
using FB2ResponseGuildMemberPtr = b2network::FB2MessageInfoResponseGetGuildMemberPtr;

struct FB2NetMatchPeerInfo
{
	uint32		NetId;
	uint8		Class;
	uint8		PlayerStartIndex;
	uint8 PlayerTeamType;
};

using FB2ResponseRaidResultPtr = b2network::FB2MessageInfoResponseRaidResultPtr;
using FB2ResponseGetHeroTowerPtr = b2network::FB2MessageInfoResponseGetHeroTowerPtr;
using FB2ResponseStartHeroTowerPtr = b2network::FB2MessageInfoResponseStartHeroTowerPtr;
using FB2ResponseFinishHeroTowerPtr = b2network::FB2MessageInfoResponseFinishHeroTowerPtr;
using FB2ResponseAbandonHeroTowerPtr = b2network::FB2MessageInfoResponseAbandonHeroTowerPtr;
using FB2ResponseSweepHeroTowerPtr = b2network::FB2MessageInfoResponseSweepHeroTowerPtr;
using FB2ResponseBuyHeroTowerTicketPtr = b2network::FB2MessageInfoResponseBuyHeroTowerTicketPtr;
using FB2ResponseGetAssaultBattleStatusPtr = b2network::FB2MessageInfoResponseGetAssaultBattleStatusPtr;
using FB2ResponseAssaultResultPtr = b2network::FB2MessageInfoResponseAssaultResultPtr;
using B2RelicPtr = b2network::B2RelicPtr;
using B2RollingBannerPtr = b2network::B2RollingBannerPtr;
using FB2ResponseEnhanceRelicPtr = b2network::FB2MessageInfoResponseEnhanceRelicPtr;
using FB2ResponsePromotionRelicPtr = b2network::FB2MessageInfoResponsePromotionRelicPtr;
using FB2ResponseGetAccountRelicPtr = b2network::FB2MessageInfoResponseGetAccountRelicPtr;
using FB2ResponseGetLotteryShopPtr = b2network::FB2MessageInfoResponseGetLotteryShopPtr;
using FB2ResponseDrawShopLotteryPtr = b2network::FB2MessageInfoResponseDrawShopLotteryPtr;
using FB2ResponseGuildDetailInfoPtr = b2network::FB2MessageInfoResponseGetGuildDetailInfoPtr;
using FB2ResponseGetRaidPtr = b2network::FB2MessageInfoResponseGetRaidPtr;
using FB2ResponseRaidMatchmakingPtr = b2network::FB2MessageInfoRaidMatchmakingResponsePtr;
using FB2ResponseRoomJoinPtr = b2network::FB2MessageInfoRoomJoinResponsePtr;
using FB2RoomPropertyPtr = b2network::B2RoomPropertyPtr;

using FB2ResponseGetChangeItemCharacterTypeCostPtr = b2network::FB2MessageInfoResponseGetChangeItemCharacterTypeCostPtr;
using FB2ResponseChangeItemCharacterTypePtr = b2network::FB2MessageInfoResponseChangeItemCharacterTypePtr;

using FB2ResponseEventAttendancePtr = b2network::FB2MessageInfoResponseEventAttendancePtr;
using FB2ResponseBeginnerAttendancePtr = b2network::FB2MessageInfoResponseBeginnerAttendancePtr;
using FB2ResponseUpdateDailyPlayTimePtr = b2network::FB2MessageInfoResponseUpdateDailyPlayTimePtr;
using FB2ResponseReceiveDailyPlayTimeRewardPtr = b2network::FB2MessageInfoResponseReceiveDailyPlayTimeRewardPtr;

using FB2ResponseKakaoMessageProcessingPtr = b2network::FB2MessageInfoResponseKakaoMessageProcessingPtr;


using FB2ResponseKakaoAccountConversionPrepare = b2network::FB2MessageInfoResponseKakaoAccountConversionPrepare;
using FB2ResponseKakaoAccountConversionPreparePtr = b2network::FB2MessageInfoResponseKakaoAccountConversionPreparePtr;
using FB2ReciveKakaoAccountConversionPrepare = TU::MakeTuple<FB2ResponseKakaoAccountConversionPrepare::type_list>::Result;

using FB2ResponseKakaoAccountConversionComplete = b2network::FB2MessageInfoResponseKakaoAccountConversionComplete;
using FB2ResponseKakaoAccountConversionCompletePtr = b2network::FB2MessageInfoResponseKakaoAccountConversionCompletePtr;
using FB2ReciveKakaoAccountConversionComplete = TU::MakeTuple<FB2ResponseKakaoAccountConversionComplete::type_list>::Result;

using FB2ResponseGetFeaturedMission = b2network::FB2MessageInfoResponseGetFeaturedMission;
using FB2ResponseGetFeaturedMissionPtr = b2network::FB2MessageInfoResponseGetFeaturedMissionPtr;
using FB2ReciveGetFeaturedMission = TU::MakeTuple<FB2ResponseGetFeaturedMission::type_list>::Result;

//event
using FB2ResponseGetEventPtr = b2network::FB2MessageInfoResponseGetEventPtr;
using FB2ResponseReceiveLevelUpEventRewardPtr = b2network::FB2MessageInfoResponseReceiveLevelUpEventRewardPtr;
using FB2ResponseReceiveStageClearEventRewardPtr = b2network::FB2MessageInfoResponseReceiveStageClearEventRewardPtr;
using FB2ResponseRewardPointShopEventdPtr = b2network::FB2MessageInfoResponseRewardPointShopEventPtr;
using FB2ResponseReceivePointEventRewardPtr = b2network::FB2MessageInfoResponseReceivePointEventRewardPtr;
using FB2ResponseRewardTenLotteryEventPtr = b2network::FB2MessageInfoResponseRewardTenLotteryEventPtr;
using FB2ResponseRewardSpendGemEventPtr = b2network::FB2MessageInfoResponseRewardSpendGemEventPtr;
using FB2ResponseReceiveGemPurchaseEventRewardPtr = b2network::FB2MessageInfoResponseReceiveGemPurchaseEventRewardPtr;
using FB2ResponseRewardTenLotterySavingsEventPtr = b2network::FB2MessageInfoResponseRewardTenLotterySavingsEventPtr;
using FB2ResponseRollDiceEventPtr = b2network::FB2MessageInfoResponseRollDiceEventPtr;
using FB2ResponseBuyDiceEventPointPtr = b2network::FB2MessageInfoResponseBuyDiceEventPointPtr;
using FB2ResponseRewardDiceEventMileagePtr = b2network::FB2MessageInfoResponseRewardDiceEventMileagePtr;


using FB2ResponseFriendInviteRewardInfoPtr = b2network::FB2MessageInfoResponsePlatformFriendInviteInfoPtr;
using FB2ResponseFriendInviteCountUpPtr = b2network::FB2MessageInfoResponsePlatformFriendInviteCountUpPtr;
using FB2ResponseFriendJoinerCountUpPtr = b2network::FB2MessageInfoResponsePlatformFriendJoinerCountUpPtr;
using FB2ResponseFriendInviteRewardPtr = b2network::FB2MessageInfoResponsePlatformFriendInviteRewardPtr;
using FB2FriendInviteRewardInfoPtr = b2network::B2FriendInviteRewardInfoPtr;

using FB2ResponseIAPGetMarketProductPtr = b2network::FB2MessageInfoResponseGetMarketProductPtr;
using FB2ResponseIAPPreparePurchasePtr = b2network::FB2MessageInfoResponsePreparePurchasePtr;
using FB2ResponseIAPVerifyPurchasePtr = b2network::FB2MessageInfoResponseVerifyPurchasePtr;
using FB2ResponseIAPCancelPurchasePtr = b2network::FB2MessageInfoResponseCancelPurchasePtr;
using FB2ResponseIAPFinishPurchasePtr = b2network::FB2MessageInfoResponseFinishPurchasePtr;

using FB2ResponseGetSeasonMissionPtr = b2network::FB2MessageInfoResponseGetSeasonMissionPtr;
using FB2ResponseRewardSeasonMissionPtr = b2network::FB2MessageInfoResponseRewardSeasonMissionPtr;
using FB2ResponseAttendSeasonMissionPtr = b2network::FB2MessageInfoResponseAttendSeasonMissionPtr;

using FB2ResponseBuyHotTimeBuffPtr = b2network::FB2MessageInfoResponseBuyHotTimeBuffPtr;

using FB2ResponseBuyShopResultInfoPtr = b2network::B2BuyShopResultInfoPtr;

UENUM(BlueprintType)
enum class EStoreItemCost : uint8	// = EStoreCostType	// = B2ShopBuyPriceType
{
	Gem,
	FriendshipPoint,
	DarkCrystal,
	Cash,
	Gold,
	FamePoint,

	TenLotteryTicket,	// Client Only

	End
};

UENUM(BlueprintType)
enum class ESealCostType : uint8	// = B2SealPriceType	// = B2SealSlotOpenPriceType
{
	mNULL = 0,
	Gem = 1,
	Gold = 2,
	End
};

UENUM(BlueprintType)
enum class ESummonItemDifficulty : uint8
{
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class ESummonItemCost : uint8
{
	Gem,
	Friendship,
	TenLotteryTicket
};

FORCEINLINE EStoreItemCost SummonItemCostToStoreItemCost(ESummonItemCost InSummonItemCost)
{
	switch (InSummonItemCost)
	{
	case ESummonItemCost::Gem: return EStoreItemCost::Gem;
	case ESummonItemCost::Friendship: return EStoreItemCost::FriendshipPoint;
	}
	return EStoreItemCost::End;
}

UENUM(BlueprintType)
enum class ESummonItemDescription : uint8
{
	FreeSummonEnable,
	FreeSummonAfter,
	AtLeastOneEqualsOrGreaterGrade,
};

/** Shows special summon item result FX.. and our cutey pretty sexy shop girl will do crazy strip show.
* I just assumed the possibility of additional jackpot grade... that make shop girl do more crazy show, so it is LV1 */
const int32 ITEM_SUMMON_JACKPOT_GRADE_LV1 = 6;

using FB2ResponseGetGeneralShopPtr = b2network::FB2MessageInfoResponseGetGeneralShopPtr;
using FB2ResponseBuyShopProductPtr = b2network::FB2MessageInfoResponseBuyShopProductPtr;

using FB2ResponseGetMagicShopPtr = b2network::FB2MessageInfoResponseGetMagicShopPtr;
using FB2ResponseRenewalMagicShopPtr = b2network::FB2MessageInfoResponseRenewalMagicShopPtr;
using FB2ResponsePurchaseMagicShopPtr = b2network::FB2MessageInfoResponsePurchaseMagicShopPtr;

using FB2ResponseRewardShopMileagePtr = b2network::FB2MessageInfoResponseRewardShopMileagePtr;

using FB2ResponseGetPackageShopPtr = b2network::FB2MessageInfoResponseGetPackageShopPtr;
using FB2ResponseGetPeriodPackageStatePtr = b2network::FB2MessageInfoResponseGetPeriodPackageStatePtr;
using FB2ResponseRewardFlatRatePackageBonusPtr = b2network::FB2MessageInfoResponseRewardFlatRatePackageBonusPtr;
using FB2ResponseRewardLevelUpPackagePtr = b2network::FB2MessageInfoResponseRewardLevelUpPackagePtr;
using FB2ResponseRewardReachLevelPackagePtr = b2network::FB2MessageInfoResponseRewardReachLevelPackagePtr;

using FB2ResponseGetContentsModeStatePtr = b2network::FB2MessageInfoResponseGetContentsModeStatePtr;
using FB2ResponseGetDuelModeInfoPtr = b2network::FB2MessageInfoResponseGetDuelModeInfoPtr;
using FB2ResponseGetChallengeModeInfoPtr = b2network::FB2MessageInfoResponseGetChallengeModeInfoPtr;
using FB2ResponseGetBlockChatListPtr = b2network::FB2MessageInfoResponseGetBlockChatListPtr;
using FB2ResponseBlockChatPtr = b2network::FB2MessageInfoResponseBlockChatPtr;

using FB2ResponseClientStateReportPtr = b2network::FB2MessageInfoResponseClientStateReportPtr;

using FB2ResponseGetHallOfFamePtr = b2network::FB2MessageInfoResponseGetHallOfFamePtr;
using FB2ResponsePraiseTargetRankerPtr = b2network::FB2MessageInfoResponsePraiseTargetRankerPtr;

using FB2ResponseGuildDonationPtr = b2network::FB2MessageInfoResponseGuildDonationPtr;

using FB2ResponseCheckReachPowerPtr = b2network::FB2MessageInfoResponseCheckReachPowerPtr;
using FB2ResponseTrackingPowerDetailPtr = b2network::FB2MessageInfoResponseTrackingPowerDetailPtr;

using FB2ResponseDismantleCostumesPtr = b2network::FB2MessageInfoResponseDismantleCostumesPtr;
using FB2ResponseLockCostumesPtr = b2network::FB2MessageInfoResponseLockCostumesPtr;
using FB2ResponseUnlockCostumesPtr = b2network::FB2MessageInfoResponseUnlockCostumesPtr;
using FB2ResponseEnhanceCostumePtr = b2network::FB2MessageInfoResponseEnhanceCostumePtr;

/** To control trigger by game context, for example lock/unlock door after specific wave clear. */
UENUM(BlueprintType)
enum class ETriggerEnableType : uint8
{
	// Don't get affected by any ActorCustomEvent
	ETET_DoNotChange,

	// Get to be enabled by any ActorCustomEvent. Cannot be disabled afterward.
	ETET_EnableByAnyCustomEvent,

	// Get to be enabled by a ActorCustomEvent with matching name.
	ETET_EnableByNamedCustomEvent,

	ETET_End
};
// Common function to use ETriggerEnableType as its basic intended usage.
FORCEINLINE void SetFlagByTriggerEnableTypeCommon(bool& bOutEnableFlag,
	ETriggerEnableType InEnableType,
	const FName& InReceivedEventName, // It is expected come as an argument of NotifyActorCustomEvent
	const FName& InEnableEventName, const FName& InDisableEventName
)
{
	if (InEnableType == ETriggerEnableType::ETET_EnableByAnyCustomEvent)
	{// If we don't care about the event name, just set it true in any case..
		bOutEnableFlag = true;
	}
	else if (InEnableType == ETriggerEnableType::ETET_EnableByNamedCustomEvent)
	{ // We care about the event name for ETET_EnableByNamedCustomEvent
		if (InReceivedEventName == InEnableEventName)
		{
			bOutEnableFlag = true;
		}
		else if (InReceivedEventName == InDisableEventName)
		{
			bOutEnableFlag = false;
		}
	}
}

struct FAttendanceRewardInfo
{
	bool bIsAttendance;
	int32 nAttendanceDay;
	int32 nAttendance_Id;
	int32 nAttendance_type;
	int32 nAttendance_count;
	bool bReceiveDone;

	b2network::B2RewardPtr DailyAttendanceRewardItem;
};

enum class EUICategoryMarkForNew : uint8
{
	NoNeed,
	LobbyMain,
	Item,
	SkillPoint,
	Wing
};

struct FCommonRewardInfo
{
	FCommonRewardInfo() : RewardID(0), RewardAmount(0), bTemplateId(false) {}
	FCommonRewardInfo(int32 InRewardID, int32 InRewardAmount) : RewardID(InRewardID), RewardAmount(InRewardAmount), bTemplateId(false) {}
	FCommonRewardInfo(int32 InRewardID, int32 InRewardAmount, bool bTemplateId_) : RewardID(InRewardID), RewardAmount(InRewardAmount), bTemplateId(bTemplateId_) {}
	FCommonRewardInfo(b2network::B2RewardPtr InRewardPtr)
	{
		if (InRewardPtr != nullptr)
		{
			RewardID = InRewardPtr->raw_reward->id;
			RewardAmount = InRewardPtr->raw_reward->count;
			bTemplateId = false;
		}
	}

	FCommonRewardInfo& operator=(b2network::B2RewardPtr InRewardPtr)
	{
		if (InRewardPtr != nullptr)
		{
			RewardID = InRewardPtr->raw_reward->id;
			RewardAmount = InRewardPtr->raw_reward->count;
			bTemplateId = false;
		}

		return *this;
	}

	int32		RewardID;
	int32		RewardAmount;

	bool		bTemplateId;
};

enum class ERoomCloseReasonType : uint8
{
	NONE = 0,
	START_PVP = 1,
	START_RAID = 2,
	BREAK_RAID = 3,
	START_ASSAULT = 4,
};

inline int32 CliToSvrRoomCloseReasonType(ERoomCloseReasonType RoomCloseType)
{
	switch (RoomCloseType)
	{
	case ERoomCloseReasonType::START_PVP:
		return b2network::B2RoomCloseReason::START_PVP;
	case ERoomCloseReasonType::START_RAID:
		return b2network::B2RoomCloseReason::START_RAID;
	case ERoomCloseReasonType::BREAK_RAID:
		return b2network::B2RoomCloseReason::BREAK_RAID;
	case ERoomCloseReasonType::START_ASSAULT:
		return b2network::B2RoomCloseReason::START_ASSAULT;
	}

	return 0;
}

inline ERoomCloseReasonType SvrToCliRoomCloseReasonType(int RoomCloseType)
{
	switch (RoomCloseType)
	{
	case b2network::B2RoomCloseReason::START_PVP:
		return ERoomCloseReasonType::START_PVP;
	case b2network::B2RoomCloseReason::START_RAID:
		return ERoomCloseReasonType::START_RAID;
	case b2network::B2RoomCloseReason::BREAK_RAID:
		return ERoomCloseReasonType::BREAK_RAID;
	case b2network::B2RoomCloseReason::START_ASSAULT:
		return ERoomCloseReasonType::START_ASSAULT;
	}

	return ERoomCloseReasonType::NONE;
}

UENUM(BlueprintType)
enum class ENetMatchResult : uint8
{ // In the aspect of local player..
	LocalWin,
	LocalLose,
	Draw
};
inline ENetMatchResult SvrToCliNetMatchResult(int32 SvrMatchResType)
{
	switch (SvrMatchResType)
	{
	case b2network::B2MatchResultType::BMRT_WIN: return ENetMatchResult::LocalWin;
	case b2network::B2MatchResultType::BMRT_LOSE: return ENetMatchResult::LocalLose;
	}
	return ENetMatchResult::Draw;
}

//================================================================================================================================================
// Note :	FServerStageID
//			ClientStageId(int32)와 타입을 명확하게 하기위해서 새로운 타입을 정의 (둘다 int32로 되어있으면 코드상 실수가 많을수 있다)
//			타입을 명확히 구분하기위해 explicit 사용(빼지말것..) / 연산은 편하게 사용하기위해 연산자 재정의
//================================================================================================================================================
//			ClientStageId : 스테이지 아이디를 나타냄 (10 : 스테이지 10)
//			ServerStageId : ClientStageID + (난이도 * 1000) (2010 : 난이도 2의 스테이지 10)
//================================================================================================================================================
//	주의(spivy) :  ServerStageId 계산법은 서버에서 하는 값으로 추후에 상황에 따라 변경될 수도 있으니 클라에선 따로 계산하지말고, 서버에서 주는값으로 판단할것
//				   explicit을 하기위한 용도의 클래스이므로 키워드 빼지말것!!
//================================================================================================================================================
struct FServerStageID
{
	int32 Id;	// ServerStageId : 2010 (=> 2(난이도)*1000 + 10(클라이언트 스테이지 넘버)) (모든 계산법은 서버에서~)

	FServerStageID()
		: Id(0)
	{}

	// 주의 : 절대 explicit 빼지말것
	explicit FServerStageID(int32 InStageId)
		: Id(InStageId)
	{}

	bool IsEmpty()
	{
		return (Id == 0);
	}

	bool IsEnable()
	{
		return (Id > 1000);	// 이건 임시방편용
	}

	// operator
	bool operator== (const int32& Other) const {
		return (Id == Other);
	}
	bool operator!= (const int32& Other) const {
		return (Id != Other);
	}
	bool operator== (const FServerStageID& OtherStageId) const {
		return Id == OtherStageId.Id;
	}
	bool operator!= (const FServerStageID& OtherStageId) const {
		return Id != OtherStageId.Id;
	}
	bool operator< (const FServerStageID& OtherStageId) const {
		return Id < OtherStageId.Id;
	}
	bool operator> (const FServerStageID& OtherStageId) const {
		return Id > OtherStageId.Id;
	}

	// Use Map Container
	friend uint32 GetTypeHash(const FServerStageID& ServerStageId)
	{
		return static_cast<uint8>(ServerStageId.Id);
	}
};

// Identifiers for stage clear grade, being set by time taken to clear a stage.
const int32 STAGE_CLEAR_GRADE_STAR_1 = 1; // Low
const int32 STAGE_CLEAR_GRADE_STAR_2 = 2;
const int32 STAGE_CLEAR_GRADE_STAR_3 = 3; // High

UENUM()
enum class EHeroTowerReadyUIState : uint8
{
	None,
	Intro,
	MoveTower,
	MoveTowerSweep,
	Ready,
};

//If you add a value, you must change the last value.
enum class EStageClearCondition : int32
{
	//Clear = b2::protocol::commondata::StageClearProperty::CLEAR_DEFAULT,
	//InTime = b2::protocol::commondata::StageClearProperty::CLEAR_IN_TIME,
	//AllSurvival = b2::protocol::commondata::StageClearProperty::CLEAR_ALL_SURVIVAL,
	//MaxCondition = b2::protocol::commondata::StageClearProperty::CLEAR_ALL_SURVIVAL + 1 // end
	Clear = 0,
	InTime,
	AllSurvival ,
	MaxCondition,
};

UENUM()
enum class EDimensionResult : int32
{
	CLEAR = 0,
	DEFEAT = 1,
	GIVEUP = 2
};

using FB2ResponseGetItemRandomOptionPtr = b2network::FB2MessageInfoResponseGetItemRandomOptionPtr;
using FB2ResponseGenerateItemRandomOptionPtr = b2network::FB2MessageInfoResponseGenerateItemRandomOptionPtr;
using FB2ResponseApplyItemRandomOptionPtr = b2network::FB2MessageInfoResponseApplyItemRandomOptionPtr;
using FB2ResponseDiscardItemRandomOptionPtr = b2network::FB2MessageInfoResponseDiscardItemRandomOptionPtr;

using FB2ResponseGetItemRandomQualityPtr = b2network::FB2MessageInfoResponseGetItemRandomQualityPtr;
using FB2ResponseGenerateItemRandomQualityPtr = b2network::FB2MessageInfoResponseGenerateItemRandomQualityPtr;
using FB2ResponseApplyItemRandomQualityPtr = b2network::FB2MessageInfoResponseApplyItemRandomQualityPtr;
using FB2ResponseDiscardItemRandomQualityPtr = b2network::FB2MessageInfoResponseDiscardItemRandomQualityPtr;

using FB2ResponseCheckRedDot = b2network::FB2MessageInfoResponseCheckRedDot;
using FB2MessageInfoResponseCheckRedDotPtr = std::shared_ptr<FB2ResponseCheckRedDot>;

//using FB2ResponseCheckRedDotPtr = b2network::FB2MessageInfoResponseCheckRedDotPtr;
//using FB2CheckRedDotMark = TU::MakeTuple<FB2ResponseCheckRedDot::type_list>::Result;

using FB2ResponseGetTutorialProgressPtr = b2network::FB2MessageInfoResponseGetTutorialProgressPtr;
using FB2ResponseSetTutorialProgressPtr = b2network::FB2MessageInfoResponseSetTutorialProgressPtr;
using FB2ResponseUpdateTutorialProgressPtr = b2network::FB2MessageInfoResponseUpdateTutorialProgressPtr;
using FB2ResponseSkipTutorialProgressPtr = b2network::FB2MessageInfoResponseSkipTutorialProgressPtr;
using FB2ResponseDrawTutorialItemPtr = b2network::FB2MessageInfoResponseDrawTutorialItemPtr;
using FB2ResponsePrepareTutorialPtr = b2network::FB2MessageInfoResponsePrepareTutorialPtr;
using FB2ResponseGiveTutorialRewardPtr = b2network::FB2MessageInfoResponseGiveTutorialRewardPtr;

using FB2ResponseAskGuildSupportPtr = b2network::FB2MessageInfoResponseAskGuildSupportPtr;
using FB2ResponseGuildSupportPtr = b2network::FB2MessageInfoResponseGuildSupportPtr;
using FB2ResponseGetGuildChatPtr = b2network::FB2MessageInfoResponseGetGuildChatPtr;

using FB2GetAccountEtherPtr = b2network::FB2MessageInfoResponseGetAccountAetherPtr;
using FB2MountEtherPtr = b2network::FB2MessageInfoResponseMountAethersPtr;
using FB2UnmountEtherPtr = b2network::FB2MessageInfoResponseUnmountAethersPtr;
using FB2SellEtherPtr = b2network::FB2MessageInfoResponseSellAethersPtr;
using FB2DecompositionEtherPtr = b2network::FB2MessageInfoResponseDismantleAethersPtr;
using FB2EnhanceEtherPtr = b2network::FB2MessageInfoResponseEnhanceAetherPtr;
using FB2LockEtherPtr = b2network::FB2MessageInfoResponseLockAethersPtr;
using FB2UnlockEtherPtr = b2network::FB2MessageInfoResponseUnlockAethersPtr;
using FB2SelectEtherDefenseOptionPtr = b2network::FB2MessageInfoResponseSelectAetherDefenseOptionPtr;
using FB2SelectEtherUnitySkillOptionPtr = b2network::FB2MessageInfoResponseSelectAetherUnitySkillOptionPtr;
using FB2ModifyEtherNewFlagPtr = b2network::FB2MessageInfoResponseModifyAetherNewFlagPtr;
using FB2ExtendEtherInventoryPtr = b2network::FB2MessageInfoResponseExtendAetherInventoryPtr;

using FB2ShopCashBonusPtr = b2network::B2mdShopCashBonusPtr;
using FB2ShopFundsSalePtr = b2network::B2mdShopFundsSalePtr;
using FB2ShopLotterySalePtr = b2network::B2mdShopLotterySalePtr;
using FB2ShopPackageTabPtr = b2network::B2mdShopPackageTabPtr;
using FB2ShopLevelUpPackagePtr = b2network::B2mdShopLevelUpPackagePtr;
using FB2ShopReachLevelPackagePtr = b2network::B2mdShopReachLevelPackagePtr;
using FB2ShopFlatRatePackageBonusPtr = b2network::B2mdShopFlatRatePackageBonusPtr;

using FB2UnitySkillOptionPtr = b2network::B2mdUnitySkillOptionPtr;
using FB2UnitySkillAwakenMissionPtr = b2network::B2mdUnitySkillAwakenMissionPtr;

using FB2ExtendMessagePtr = b2network::B2mdExtendMessagePtr;

UENUM(BlueprintType)
enum class EControlChatType : uint8
{
	None,
	Message,
	Emoticon,
};

/** Graphics level allowed for user interface by BladeII game.
* Overall engine scalability level will be adjusted by this level. Refer to the interfaces at B2Scalability */
UENUM(BlueprintType)
enum class EB2GraphicsLevel : uint8
{
	GraphicsLevel_LOW = 0,
	GraphicsLevel_MID = 1,
	GraphicsLevel_HIGH = 2,

	GraphicsLevel_End
};
/** Frame rate limit level mapped to the user interface.
* Refer to GetMappedCVarOfFrameLimitLevel for the mapping to internal variable. */
enum class EB2FrameLimitLevel : uint8
{
	Low = 0,
	Mid = 1,
	High = 2,

	End
};
/** Game screen resolution level mapped to the user interface,
* Scalability setup by EB2GraphicsLevel can also adjust the resolution. It provides additional independent control.
* Refer to GetMappedRQTypeOfResolutionLevel for the mapping to internal variable. */
enum class EB2ResolutionLevel : uint8
{
	Low = 0,
	Mid = 1,
	High = 2,

	End
};

UENUM()
enum class EEtherSetType : uint8
{
	None,
	Offense,
	Defense
};

UENUM()
enum class ERaidGotoWhere : uint8
{
	RaidMain = 0,
	Lobby,
	Rematch,
	PartyRematch
};

struct FRaidInviteInfo
{
	FRaidInviteInfo(FString roomid, EPCClass pcclass, int32 raidtype, int32 step, int32 rtype, FString invitename)
		: InvitedRoomId(roomid)
		, ChangeMainPCClass(pcclass)
		, RaidType(raidtype)
		, RaidStep(step)
		, RoomType(rtype)
		, InviterName(invitename)
	{
	}

	explicit FRaidInviteInfo()
		: InvitedRoomId(FString(""))
		, ChangeMainPCClass(EPCClass::EPC_End)
		, RaidType(0)
		, RaidStep(0)
		, RoomType(0)
		, InviterName(FString(TEXT("")))
	{
	}

	FString InvitedRoomId;
	EPCClass ChangeMainPCClass;
	int32 RaidType;
	int32 RaidStep;
	int32 RoomType;
	FString InviterName;

	FDateTime InviteTime; // UTC
};

UENUM()
enum class EInvitationState : uint8
{
	None,
	WaitingAccept,	// 초대 발신자 ( 상대 수락 대기 )
	WaitingDecision,// 초대 수신자 ( 수락/거절 버튼 대기 )
	Success
};

struct FB2InvitationInfo
{
	int32		InvitationType;
	FString		Inviter;
	FString		Invitee;
	FString		InvitationInfo;
	b2network::B2RoomLocationPtr RoomInfoPtr;

	bool		bMyInvitation;

	EInvitationState State;

	FB2InvitationInfo()
		: InvitationType(0), Inviter(TEXT("")), Invitee(TEXT("")), InvitationInfo(TEXT("")), RoomInfoPtr(nullptr), bMyInvitation(false), State(EInvitationState::None) {}

	void Clear()
	{
		InvitationInfo = 0;
		Inviter = TEXT("");
		Invitee = TEXT("");
		InvitationInfo = TEXT("");
		RoomInfoPtr = nullptr;
		bMyInvitation = false;
		State = EInvitationState::None;
	}

	bool IsValid()
	{
		return RoomInfoPtr != nullptr && InvitationInfo.Len() > 0;
	}
};

FORCEINLINE EB2GameMode InvitationTypeToGameMode(int32 InvitationType)
{
	switch (InvitationType)
	{
	case 1: return EB2GameMode::PVP_Tag;	// 아직 b2network 타입 정해지지 않음 - 숫자 1 바인딩
	}

	return EB2GameMode::Unknown;
}

struct FRankerInfo
{
	FRankerInfo()

	{
		Ranking = 0;
		PraiseCount = 0;
		MatchGrade = 0;
		MatchStarCount = 0;
		MvpCount = 0;
		AttackPower = 0;
		AccountID = 0;
		Mode = EHallOfFameMode::None;
		CharacterClass = EPCClass::EPC_End;
		UserName = "";
		Wing_Grade = -1;
		Wing_Level = -1;
		WingVisible = true;
		HelmetVisible = true;
		CostumeWeaponVisible = true;
		CostumeHelmetVisible = true;
		CostumeBodyVisible = true;
	}

	int32 Ranking;
	int32 PraiseCount;
	int32 MatchGrade;
	int32 MatchStarCount;
	int32 MvpCount;
	int32 AttackPower;
	int64 AccountID;
	int32 Wing_Grade;
	int32 Wing_Level;
	bool WingVisible;
	bool HelmetVisible;
	bool CostumeWeaponVisible;
	bool CostumeHelmetVisible;
	bool CostumeBodyVisible;

	EHallOfFameMode Mode;
	EPCClass CharacterClass;
	FString UserName;
	TArray<b2network::B2ItemServerInfoPtr> EquipItems;
	TArray<b2network::B2CostumeServerInfoPtr> EquipICostumeItems;
};

struct FCombatStatParam
{
	class ICharacterDataStore* CharacterDataStore;
	EPCClass SpawnPCClass; //EPC_End == AI

	TArray<EPCClass> AddStatClasses;

	float ReduceDamageBonusByFormation;
	int32 AttackBonusByFormation;
	float ModeDamageRate;
	float ModeHealthRate;
};


struct CombatStatInfoRawDatas
{
	// ECharStatApplyType에 따른 PrimaryPoint값을 가지고있는다.
	// 실제로 적용되는 최종값은 GetEffectiveOptionValueTempl(...)으로 결정
	TMap<ECharStatApplyType, TArray<FOptionValue>> PrimaryPointRawValues;

	CombatStatInfoRawDatas()
	{
		PrimaryPointRawValues.Empty();
	}

	// 기존 시스템을 최대한 유지한다.
	// 만약 토템 외에 장비에 감소 옵션을 넣고 싶다면 해당 부분의 옵션 삽입부를 찾아가
	// AddPrimaryPointValue(ECharStatApplyType InApplyType, float InValue) -> AddPrimaryPointValue(ECharStatApplyType InApplyType, FOptionValue InValue) 로 바꿔준다.
	void AddPrimaryPointValue(ECharStatApplyType InApplyType, float InValue)
	{
		if (InValue == 0.f)
			return;

		if (PrimaryPointRawValues.Contains(InApplyType) == false)
			PrimaryPointRawValues.Add(InApplyType, TArray<FOptionValue>());

		FOptionValue OptionValue = FOptionValue(InValue);
		PrimaryPointRawValues[InApplyType].Add(OptionValue);
	}

	void AddPrimaryPointValue(ECharStatApplyType InApplyType, FOptionValue InValue)
	{
		if (InValue.RawOptionAmount == 0.f)
			return;

		if (PrimaryPointRawValues.Contains(InApplyType) == false)
			PrimaryPointRawValues.Add(InApplyType, TArray<FOptionValue>());

		PrimaryPointRawValues[InApplyType].Add(InValue);
	}

	bool GetRawValues(ECharStatApplyType InApplyType, TArray<FOptionValue>& OutRawValues)
	{
		OutRawValues.Empty();

		if (PrimaryPointRawValues.Contains(InApplyType) == false)
			return false;

		OutRawValues = PrimaryPointRawValues[InApplyType];
		return true;
	}

	bool GetAllRawValues(TArray<FOptionValue>& OutRawValues)
	{
		OutRawValues.Empty();

		if (IsEmpty())
			return false;

		for (auto RawData : PrimaryPointRawValues)
			OutRawValues += RawData.Value;

		return true;
	}

	template<typename TOptionTypeEnum>
	bool PreCalculateCharApplyType(TOptionTypeEnum InOptionType, ECharStatApplyType InApplyType)
	{
		if (PrimaryPointRawValues.Contains(InApplyType) == false)
		{
			return false;
		}

		// GetEffectiveOptionValueTempl 를 통해 미리 계산되어 나온 값은 Source 로 들어간 값들인 RawValue 형식이 아니기 때문에
		// 다시 계산 식으로 들어가기 위해 RawValue 형식으로 바꿔주는 절차를 가집니다.
		float ItemOptionValue = GetEffectiveOptionValueTempl<TOptionTypeEnum>(InOptionType, PrimaryPointRawValues[InApplyType]);
		ItemOptionValue = GetConvertNotRawValueForOptionApplyType(GetCombatOptionApplyType(InOptionType), ItemOptionValue);

		PrimaryPointRawValues[InApplyType].Empty();
		PrimaryPointRawValues[InApplyType].Add(FOptionValue(ItemOptionValue));

		return true;
	}

	bool IsEmpty()
	{
		return (PrimaryPointRawValues.Num() == 0);
	}

	void Empty(ECharStatApplyType InApplyType = ECharStatApplyType::ECSA_End)
	{
		if (InApplyType == ECharStatApplyType::ECSA_End)
			PrimaryPointRawValues.Empty();
		else
			PrimaryPointRawValues.Remove(InApplyType);
	}
};

enum class EUpgradeded : uint8 // 어떤 승급석을 사용해야되는지 옵션용
{
	UPGRADE_BEFORE = 0,		// 승급전에 체크할 경우
	UPGRADE_AFTER,			// 승급후에 체크할 경우
	UPGRADE_NONE,			// 옵션 필요 없을 경우
};
/**
* Provide RefID rule handling, special type ID query, etc..
*/
class FItemRefIDHelper
{
private:
	/**
	Some special const definitions for common digit or RefIDs of special types.
	Not for direct access.
	**/
	const static int32 ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT = 1;
	const static int32 ITEM_REF_ID_LARGEST_DIGIT_ESSENCE = 2;
	const static int32 ITEM_REF_ID_LARGEST_DIGIT_COSTUME = 3;
	const static int32 ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL = 6;
	const static int32 ITEM_REF_ID_LARGEST_DIGIT_Consumable = 8;
	const static int32 ITEM_REF_ID_LARGEST_DIGIT_REWARDS = 9;

	// RefID of Goods.. They are not really seen in inventory view like other items, but provide RefID for some case.
	const static int32 ITEM_REF_ID_GOLD = 9000000;
	const static int32 ITEM_REF_ID_GEM = 8000003;
	const static int32 ITEM_REF_ID_BLADEPOINT = 8000004;
	const static int32 ITEM_REF_ID_FRIENDSHIP_POINT = 9000003;
	const static int32 ITEM_REF_ID_DARK_CRYSTAL = 9000004;
	const static int32 ITEM_REF_ID_EXP = 9000001;
	const static int32 ITEM_REF_ID_MISSION_POINT = 9000002;

	const static int32 ITEM_REF_ID_GOLD_BUFF_TICKET = 9000008;
	const static int32 ITEM_REF_ID_EXP_BUFF_TICKET = 9000009;
	const static int32 ITEM_REF_ID_SHOPTENLOTTERY_TICKET = 9000013;

	const static int32 ITEM_REF_ID_HONER_POINT = 9000014;

	const static int32 ITEM_REF_ID_STAGE_AUTO_CLEAR_TICKET = 9000015;

	const static int32 ITEM_REF_ID_GENERAL_LOTTERY_BEGIN = 7001000;
	const static int32 ITEM_REF_ID_SELECTIVE_LOTTERY_BEGIN = 7002000;
	const static int32 ITEM_REF_ID_SELECTIVE_ITEM_LOTTERY_BEGIN = 7003000;

	const static int32 ITEM_REF_ID_FIXED_GRADE_LOTTERY = 7004000;

	const static int32 ITEM_REF_ID_SELECTIVE_ETHER_LOTTERY_BEGIN = 7005000;

	const static int32 ITEM_REF_ID_PREFIX_SELECTIVE_LOTTERY_BEGIN = 7006000;

	const static int32 TOTEM_REF_ID_PREFIXED_GRADE_LOTTERY_BEGIN = 7007000;

	const static int32 ITEM_REF_ID_UPGRADE_STONE_WEAPON = 8000000;
	const static int32 ITEM_REF_ID_UPGRADE_STONE_PROTECTION = 8000001;
	const static int32 ITEM_REF_ID_UPGRADE_STONE_ACCESSORY = 8000002;

	const static int32 ITEM_REF_ID_ADVANCED_UPGRADE_STONE_WEAPON = 8000100;
	const static int32 ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ARMOR = 8000101;
	const static int32 ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ACCESSORY = 8000102;

	const static int32 ITEM_REF_ID_ENHANCE_SCROLL_WEAPON = 8000010;
	const static int32 ITEM_REF_ID_ENHANCE_SCROLL_PROTECTION = 8000011;
	const static int32 ITEM_REF_ID_ENHANCE_SCROLL_ACCESSORY = 8000012;

	const static int32 ITEM_REF_ID_BLESS_ENHANCE_SCROLL_WEAPON = 8000013;
	const static int32 ITEM_REF_ID_BLESS_ENHANCE_SCROLL_PROTECTION = 8000014;
	const static int32 ITEM_REF_ID_BLESS_ENHANCE_SCROLL_ACCESSORY = 8000015;

	const static int32 ITEM_REF_ID_ENHANCE_PROTECTION_ITEM = 8000016;

	const static int32 ITEM_REF_ID_BOSSPIECE_BEGIN = 8001000;

	const static int32 ITEM_REF_ID_WING_ENHANCE_INGREDIENT = 8000005;
	const static int32 ITEM_REF_ID_WING_EVOLVE_INGREDIENT = 8000006;
	const static int32 ITEM_REF_ID_WING_PROP_OPTION_ENHANCE_INGREDIENT = 8000007;

	const static int32 ITEM_REF_ID_NICKNAMECHANGE_TICKET = 8003000;

	const static int32 ITEM_REF_ID_CONTROL_MVPPOINT = 9000006;

	const static int32 ITEM_REF_ID_STEEL_HAMMER = 8004001;
	const static int32 ITEM_REF_ID_ADVANCED_STEEL_HAMMER = 8004011;
	const static int32 ITEM_REF_ID_PLATINUM_HAMMER = 8004021;
	const static int32 ITEM_REF_ID_ADVANCED_PLATINUM_HAMMER = 8004031;

public:

	// Simply put some unique IDs.. They are not even Goods..
	const static int32 ITEM_REF_ID_BREVET_STONE = 8000008;
	const static int32 ITEM_REF_ID_HERO_PIECE = 8000009;
	const static int32 ITEM_REF_ID_ETHER_PIECE = 8000017;
	const static int32 ITEM_REF_ID_DIMENSION_PIECE = 8000018;
	const static int32 ITEM_REF_ID_DISMANTLE_ITEM = 8002000;
	const static int32 ITEM_REF_ID_STAGE_BOOST = 9000007;

	const static int32 ITEM_REF_ID_RANDOM_ACCESSORY = 9000010;
	const static int32 ITEM_REF_ID_RANDOM_BONUS_ACCESSORY = 9000011;
	const static int32 ITEM_REF_ID_RANDOM_ESSECNE = 9000012;
	const static int32 ITEM_REF_ID_PREVIEW_TOTEM = 9005001;

	const static int32 ITEM_REF_ID_ENHANCE_COSTUMEWEAPON_ITEM = 8000019;
	const static int32 ITEM_REF_ID_ENHANCE_COSTUMEARMOR_ITEM = 8000020;
	const static int32 ITEM_REF_ID_ADVANCED_BREVET_STONE = 8000108;

public:

	static int32 GetGoodsID_Gold() {
		return ITEM_REF_ID_GOLD;
	}
	static int32 GetGoodsID_Gem() {
		return ITEM_REF_ID_GEM;
	}
	static int32 GetGoodsID_BladePoint() {
		return ITEM_REF_ID_BLADEPOINT;
	}
	static int32 GetGoodsID_FriendshipPoint() {
		return ITEM_REF_ID_FRIENDSHIP_POINT;
	}
	static int32 GetGoodsID_DarkCrystal() {
		return ITEM_REF_ID_DARK_CRYSTAL;
	}
	static int32 GetGoodsID_EXP() {
		return ITEM_REF_ID_EXP;
	}
	static int32 GetGoodsID_MissionPoint() {
		return ITEM_REF_ID_MISSION_POINT;
	}
	static int32 GetGoodsID_ControlMVPPoint() {
		return ITEM_REF_ID_CONTROL_MVPPOINT;
	}
	static int32 GetGoodsID_GoldBuffTicket() {
		return ITEM_REF_ID_GOLD_BUFF_TICKET;
	}
	static int32 GetGoodsID_ExpBuffTicket() {
		return ITEM_REF_ID_EXP_BUFF_TICKET;
	}
	static int32 GetGoodsID_StageBoostTicket() {
		return ITEM_REF_ID_STAGE_BOOST;
	}
	static int32 GetGoodsID_ShopTenLotteryTicket() {
		return ITEM_REF_ID_SHOPTENLOTTERY_TICKET;
	}
	static int32 GetGoodsID_FamePoint() {
		return ITEM_REF_ID_HONER_POINT;
	}
	static int32 GetGoodsID_Dismantle() {
		return ITEM_REF_ID_DISMANTLE_ITEM;
	} // 페어리 레벨업 재료
	static int32 GetGoodsID_StageAutoClearTicket() {
		return ITEM_REF_ID_STAGE_AUTO_CLEAR_TICKET;
	}
	static int32 GetGoodsID_NicknameChangeTicket() {
		return ITEM_REF_ID_NICKNAMECHANGE_TICKET;
	}

	static bool IsGoodsRefID(int32 InID);

	static int32 GetUpgradeStoneIDForUpgradeTarget(const FB2Item& InUpgradeTarget, const EUpgradeded EUpgraded = EUpgradeded::UPGRADE_NONE);
	static int32 GetEnhanceScrollRefId(const FB2Item& InTargetItem);
	static int32 GetBlessEnhanceScrollRefId(const FB2Item& InTargetItem);
	static int32 GetEnhanceProtectionItemRefId(); // Parameter can be added later.

	static int32 GetGeneralLotteryRefID(int32 InRewardID);
	static int32 GetSelectiveLotteryRefID(int32 InRewardID);
	static int32 GetSelectiveItemLotteryRefID(int32 InRewardID);
	static int32 GetFixedGradeLotteryRefID(int32 InRewardID);
	static int32 GetPrefixTotemLotteryRefID(int32 InRewardID);

	static int32 GetPrefixSelectiveLotteryRefID(int32 InRewardID);

	static int32 GetSelectiveEtherLotteryRefID(int32 InRewardID);

	static int32 GetRewardItemId(ERewardType InRewardType, int32 InRewardID);
	static int32 GetProductDataItemId(EStoreItemType InStoreItemType, int32 InProductId);
	static ERewardType GetRewardTypeByRefID(int32 InRefID);

	/** Returns Relic ID from its relevant BossPiece RefID (which can enhance the Relic) */
	static int32 GetRelicIDFromBossPieceRefID(int32 InRefID);
	/** Reverse of GetRelicIDFromBossPieceRefID */
	static int32 GetBossPieceRefIDFromRelicID(int32 InRelicID);

	static int32 GetWingEnhanceResourceRefID() {
		return ITEM_REF_ID_WING_ENHANCE_INGREDIENT;
	}
	static int32 GetWingEvolveResourceRefID() {
		return ITEM_REF_ID_WING_EVOLVE_INGREDIENT;
	}
	static int32 GetWingPropOptionEnhanceResourceRefID() {
		return ITEM_REF_ID_WING_PROP_OPTION_ENHANCE_INGREDIENT;
	}
	static int32 GetWingResourceRefIDByAcqElemType(EWingIngredAcqElemType InAcqElemType);

	/**
	Here are utilities to extract essential item information based on RefID
	Extracting item information from RefID is now official, but better not use them to much.
	**/
	static EItemClass ExtractItemClassFromRefID(int32 InItemRefID);
	static EItemClass ExtractItemClassFromRefIDForItemForge(int32 InItemRefID);

	static EPCClass ExtractItemAllowedPCClassFromRefID(int32 InItemRefID);
	static int32 ExtractItemStarGradeFromRefID(int32 InItemRefID);
	static int32 ExtractItemVariationFromRefID(int32 InItemRefID);
	static TArray<int32> GetAllGradeItemRefIDs(int32 InItemRefID);

	// By largest digit.. For formal code you need to check "ItemClass" !!!
	static bool IsEquipmentItemByRefID(int32 InRefID);
	static bool IsEssenceItemByRefID(int32 InRefID);
	static bool IsCostumeItemByRefID(int32 InRefID);
	static bool IsAnvilItemByRefID(int32 InRefID);

	static int32 GenerateRewardRepresentativeItemID(int32 InRewardGroupID, int32 InType);

	static int32 GetRareWeaponUpgradeStoneID() {
		return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_WEAPON;
	}
	static int32 GetRareArmorUpgradeStoneID() {
		return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ARMOR;
	}
	static int32 GetRareAccessoryUpgradeStoneID() {
		return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ACCESSORY;
	}

	static int32 GetSteelHammerID() {
		return ITEM_REF_ID_STEEL_HAMMER;
	}
	static int32 GetAdvancedSteelHammerID() {
		return ITEM_REF_ID_ADVANCED_STEEL_HAMMER;
	}
	static int32 GetPlatinumHammerID() {
		return ITEM_REF_ID_PLATINUM_HAMMER;
	}
	static int32 GetAdvancedPlatinumHammerID() {
		return ITEM_REF_ID_ADVANCED_PLATINUM_HAMMER;
	}

	static int32 GetEquipmentDigit() {
		return ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT;
	}
	static int32 GetEssenceDigit() {
		return ITEM_REF_ID_LARGEST_DIGIT_ESSENCE;
	}
	static int32 GetCostumeDigit() {
		return ITEM_REF_ID_LARGEST_DIGIT_COSTUME;
	}
	static int32 GetSurpassAnvilDigit() {
		return ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL;
	}
};

/**
* Defines possible return values of "Async loading request with specifying loading complete callback"
*/
enum class EAsyncLoadWithReservedCBRet : uint8
{
	EALRC_RequestedNormal, // Normally expected operation..
	EALRC_AlreadyLoaded, // Happiest case, do the callback right now..
	EALRC_OtherPending, // Unhappy case. Cannot do any..
	EALRC_UnknownFailure // Better not happen..
};

enum class EStageFailReason : uint8
{
	Defeat = 1,	//b2network::B2StageFailReason::DEFEAT
	GiveUp = 2, //b2network::B2StageFailReason::GIVEUP
};

enum class ERequestUpdateDailyPlayStep : uint8
{
	NotRequest = 0,
	RequestUpdateDailyTime,
	ResponseUpdateDailyTime,
};

inline int32 CliToSvrStageFailReason(EStageFailReason FailReason)
{
	switch (FailReason)
	{
	case EStageFailReason::Defeat:
		return b2network::B2StageFailReason::DEFEAT;
	case EStageFailReason::GiveUp:
		return b2network::B2StageFailReason::GIVEUP;
	}
	return b2network::B2StageFailReason::GIVEUP;
}

// kakao friend
struct FB2KakaoFriendInfo
{
	FString sKakaoUserID;
	FString sKakaoNickName;
	FString sThumbnailImageUrl;
	bool bInvited;
};

// kakao events
struct FB2KakaoFriendEventInfo
{
	bool IsEventCardEnabled;
	int32 EventID;
	int32 TotalJoinerCount;
	int32 MaxHostRewardCount;
	int64 StartTime;
	int64 EndTime;
	FString HostRewardCode;
	FString JoinerRewardCode;
	FString InvitationUrl;
};

enum class EKakaoInviteResultCode : uint32
{
	Success = 200,	// 성공
	ExceedMonthlyUsage = 7001, // 월간 허용 사용량 초과
	ExceedDailyUsage = 7002, // 일간 허용 사용량 초과
	SettingDisabled = 7101, // 수신 차단 상태
	NotKakaoTalkUser = 7202, // 카카오톡 유저 아님
	Error = 9999,	// 모든 에러
};

UENUM(BlueprintType)
enum class EHotTimeBuffType :uint8
{
	mNULL = 0,
	Gold = 1,
	Exp = 2,
	End = 3
};

UENUM(BlueprintType)
enum class ERaidBossIndex : uint8
{
	BOSS_DEFAULT = 0,
	BOSS_HARANG,
	BOSS_SARGON,
	BOSS_MAX
};

inline EHotTimeBuffType SvrToCliHotTimeBuffType(int32 InServerType)
{
	//switch (InServerType)
	//{
	//case b2network::B2HotTimeBuffType::GOLD:	return EHotTimeBuffType::Gold;
	//case b2network::B2HotTimeBuffType::EXP:		return EHotTimeBuffType::Exp;
	//}
	return EHotTimeBuffType::End;
}

inline int32 CliToSvrHotTimeBuffType(EHotTimeBuffType InClientType)
{
	switch (InClientType)
	{
	case EHotTimeBuffType::Gold:		return b2network::B2HotTimeBuffType::GOLD;
	case EHotTimeBuffType::Exp:			return b2network::B2HotTimeBuffType::EXP;
	}
	return -1;
}

inline EDayOfWeek SvrToEngineDay(int32 InSeverDay)
{
	EDayOfWeek Day = EDayOfWeek::Monday;

	switch (InSeverDay)
	{
	case b2network::B2DayOfWeek::SUNDAY:
		Day = EDayOfWeek::Sunday;
		break;
	case b2network::B2DayOfWeek::MONDAY:
		Day = EDayOfWeek::Monday;
		break;
	case b2network::B2DayOfWeek::TUESDAY:
		Day = EDayOfWeek::Tuesday;
		break;
	case b2network::B2DayOfWeek::WEDNESDAY:
		Day = EDayOfWeek::Wednesday;
		break;
	case b2network::B2DayOfWeek::THURSDAY:
		Day = EDayOfWeek::Thursday;
		break;
	case b2network::B2DayOfWeek::FRIDAY:
		Day = EDayOfWeek::Friday;
		break;
	case b2network::B2DayOfWeek::SATURDAY:
		Day = EDayOfWeek::Saturday;
		break;
	default:
		break;
	}

	return Day;
}

inline FString SvrToCliDayString(int32 Inday)
{
	FString DayString = FString();
	switch (Inday)
	{
	case b2network::B2DayOfWeek::SUNDAY:
		DayString = TEXT("CounterAttack_Sunday");
		break;
	case b2network::B2DayOfWeek::MONDAY:
		DayString = TEXT("CounterAttack_Monday");
		break;
	case b2network::B2DayOfWeek::TUESDAY:
		DayString = TEXT("CounterAttack_Tuesday");
		break;
	case b2network::B2DayOfWeek::WEDNESDAY:
		DayString = TEXT("CounterAttack_Wedneday");
		break;
	case b2network::B2DayOfWeek::THURSDAY:
		DayString = TEXT("CounterAttack_Thursday");
		break;
	case b2network::B2DayOfWeek::FRIDAY:
		DayString = TEXT("CounterAttack_Friday");
		break;
	case b2network::B2DayOfWeek::SATURDAY:
		DayString = TEXT("CounterAttack_Saturday");
		break;
	default:
		break;
	}

	return DayString;
}

//Copy InternationalizationSettingsModel, ETimezoneSetting
UENUM()
enum class ETimeZone : uint8
{
	InternationalDateLineWest UMETA(DisplayName = "(UTC-12:00) International Date Line West"),
	CoordinatedUniversalTimeNeg11 UMETA(DisplayName = "(UTC-11:00) Coordinated Universal Time -11"),
	Samoa UMETA(DisplayName = "(UTC-11:00) Samoa"),
	Hawaii UMETA(DisplayName = "(UTC-10:00) Hawaii"),
	Alaska UMETA(DisplayName = "(UTC-09:00) Alaska"),
	PacificTime_USCAN UMETA(DisplayName = "(UTC-08:00) Pacific Time (US and Canada)"),
	BajaCalifornia UMETA(DisplayName = "(UTC-08:00) Baja California"),
	MountainTime_USCAN UMETA(DisplayName = "(UTC-07:00) Mountain Time (US and Canada)"),
	Chihuahua_LaPaz_Mazatlan UMETA(DisplayName = "(UTC-07:00) Chihuahua, La Paz, Mazatlan"),
	Arizona UMETA(DisplayName = "(UTC-07:00) Arizona"),
	Saskatchewan UMETA(DisplayName = "(UTC-06:00) Saskatchewan"),
	CentralAmerica UMETA(DisplayName = "(UTC-06:00) Central America"),
	CentralTime_USCAN UMETA(DisplayName = "(UTC-06:00) Central Time (US and Canada)"),
	Guadalajara_MexicoCity_Monterrey UMETA(DisplayName = "(UTC-06:00) Guadalajara, Mexico City, Monterrey"),
	EasternTime_USCAN UMETA(DisplayName = "(UTC-05:00) Eastern Time (US and Canada)"),
	Bogota_Lima_Quito UMETA(DisplayName = "(UTC-05:00) Bogota, Lima, Quito"),
	Indiana_US UMETA(DisplayName = "(UTC-05:00) Indiana (East)"),
	Caracas UMETA(DisplayName = "(UTC-04:30) Caracas"),
	AtlanticTime_Canada UMETA(DisplayName = "(UTC-04:00) Atlantic Time (Canada)"),
	Cuiaba UMETA(DisplayName = "(UTC-04:00) Cuiaba"),
	Santiago UMETA(DisplayName = "(UTC-04:00) Santiago"),
	Georgetown_LaPaz_Manaus_SanJuan UMETA(DisplayName = "(UTC-04:00) Georgetown, La Paz, Manaus, San Juan"),
	Asuncion UMETA(DisplayName = "(UTC-04:00) Asuncion"),
	Newfoundland UMETA(DisplayName = "(UTC-03:30) Newfoundland"),
	Brasilia UMETA(DisplayName = "(UTC-03:00) Brasilia"),
	Greenland UMETA(DisplayName = "(UTC-03:00) Greenland"),
	Montevideo UMETA(DisplayName = "(UTC-03:00) Montevideo"),
	Cayenne_Fortaleza UMETA(DisplayName = "(UTC-03:00) Cayenne, Fortaleza"),
	BuenosAires UMETA(DisplayName = "(UTC-03:00) Buenos Aires"),
	MidAtlantic UMETA(DisplayName = "(UTC-02:00) Mid-Atlantic"),
	CoordinatedUniversalTimeNeg02 UMETA(DisplayName = "(UTC-02:00) Coordinated Universal Time -02"),
	Azores UMETA(DisplayName = "(UTC-01:00) Azores"),
	CaboVerdeIs UMETA(DisplayName = "(UTC-01:00) Cabo Verde Is."),
	Dublin_Edinburgh_Lisbon_London UMETA(DisplayName = "(UTC) Dublin, Edinburgh, Lisbon, London"),
	Monrovia_Reykjavik UMETA(DisplayName = "(UTC) Monrovia, Reykjavik"),
	Casablanca UMETA(DisplayName = "(UTC) Casablanca"),
	UTC UMETA(DisplayName = "(UTC) Coordinated Universal Time"),
	Belgrade_Bratislava_Budapest_Ljubljana_Prague UMETA(DisplayName = "(UTC+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague"),
	Sarajevo_Skopje_Warsaw_Zagreb UMETA(DisplayName = "(UTC+01:00) Sarajevo, Skopje, Warsaw, Zagreb"),
	Brussels_Copenhagen_Madrid_Paris UMETA(DisplayName = "(UTC+01:00) Brussels, Copenhagen, Madrid, Paris"),
	WestCentralAfrica UMETA(DisplayName = "(UTC+01:00) West Central Africa"),
	Amsterdam_Berlin_Bern_Rome_Stockholm_Vienna UMETA(DisplayName = "(UTC+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna"),
	Windhoek UMETA(DisplayName = "(UTC+01:00) Windhoek"),
	Minsk UMETA(DisplayName = "(UTC+02:00) Minsk"),
	Cairo UMETA(DisplayName = "(UTC+02:00) Cairo"),
	Helsinki_Kyiv_Riga_Sofia_Tallinn_Vilnius UMETA(DisplayName = "(UTC+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius"),
	Athens_Bucharest UMETA(DisplayName = "(UTC+02:00) Athens, Bucharest"),
	Jerusalem UMETA(DisplayName = "(UTC+02:00) Jerusalem"),
	Amman UMETA(DisplayName = "(UTC+02:00) Amman"),
	Beirut UMETA(DisplayName = "(UTC+02:00) Beirut"),
	Harare_Pretoria UMETA(DisplayName = "(UTC+02:00) Harare, Pretoria"),
	Damascus UMETA(DisplayName = "(UTC+02:00) Damascus"),
	Istanbul UMETA(DisplayName = "(UTC+02:00) Istanbul"),
	Kuwait_Riyadh UMETA(DisplayName = "(UTC+03:00) Kuwait, Riyadh"),
	Baghdad UMETA(DisplayName = "(UTC+03:00) Baghdad"),
	Nairobi UMETA(DisplayName = "(UTC+03:00) Nairobi"),
	Kaliningrad UMETA(DisplayName = "(UTC+03:00) Kaliningrad"),
	Tehran UMETA(DisplayName = "(UTC+03:30) Tehran"),
	Moscow_StPetersburg_Volgograd UMETA(DisplayName = "(UTC+04:00) Moscow, St. Petersburg, Volgograd"),
	AbuDhabi_Muscat UMETA(DisplayName = "(UTC+04:00) Abu Dhabi, Muscat"),
	Baku UMETA(DisplayName = "(UTC+04:00) Baku"),
	Yerevan UMETA(DisplayName = "(UTC+04:00) Yerevan"),
	Tbilisi UMETA(DisplayName = "(UTC+04:00) Tbilisi"),
	PortLouis UMETA(DisplayName = "(UTC+04:00) Port Louis"),
	Kabul UMETA(DisplayName = "(UTC+04:30) Kabul"),
	Tashkent UMETA(DisplayName = "(UTC+05:00) Tashkent"),
	Islamabad_Karachi UMETA(DisplayName = "(UTC+05:00) Islamabad, Karachi"),
	Chennai_Kolkata_Mumbai_NewDelhi UMETA(DisplayName = "(UTC+05:30) Chennai, Kolkata, Mumbai, New Delhi"),
	SriJayawardenepura UMETA(DisplayName = "(UTC+05:30) Sri Jayawardenepura"),
	Kathmandu UMETA(DisplayName = "(UTC+05:45) Kathmandu"),
	Ekaterinburg UMETA(DisplayName = "(UTC+06:00) Ekaterinburg"),
	Astana UMETA(DisplayName = "(UTC+06:00) Astana"),
	Dhaka UMETA(DisplayName = "(UTC+06:00) Dhaka"),
	Yangon_Rangoon UMETA(DisplayName = "(UTC+06:30) Yangon (Rangoon)"),
	Novosibirsk UMETA(DisplayName = "(UTC+07:00) Novosibirsk"),
	Bangkok_Hanoi_Jakarta UMETA(DisplayName = "(UTC+07:00) Bangkok, Hanoi, Jakarta"),
	Krasnoyarsk UMETA(DisplayName = "(UTC+08:00) Krasnoyarsk"),
	Beijing_Chongqing_HongKong_Urumqi UMETA(DisplayName = "(UTC+08:00) Beijing, Chongqing, Hong Kong, Urumqi"),
	KualaLumpur_Singapore UMETA(DisplayName = "(UTC+08:00) Kuala Lumpur, Singapore"),
	Taipei UMETA(DisplayName = "(UTC+08:00) Taipei"),
	Perth UMETA(DisplayName = "(UTC+08:00) Perth"),
	Ulaanbaatar UMETA(DisplayName = "(UTC+08:00) Ulaanbaatar"),
	Irkutsk UMETA(DisplayName = "(UTC+09:00) Irkutsk"),
	Seoul UMETA(DisplayName = "(UTC+09:00) Seoul"),
	Osaka_Sapporo_Tokyo UMETA(DisplayName = "(UTC+09:00) Osaka, Sapporo, Tokyo"),
	Darwin UMETA(DisplayName = "(UTC+09:30) Darwin"),
	Adelaide UMETA(DisplayName = "(UTC+09:30) Adelaide"),
	Yakutsk UMETA(DisplayName = "(UTC+10:00) Yakutsk"),
	Canberra_Melbourne_Sydney UMETA(DisplayName = "(UTC+10:00) Canberra, Melbourne, Sydney"),
	Brisbane UMETA(DisplayName = "(UTC+10:00) Brisbane"),
	Hobart UMETA(DisplayName = "(UTC+10:00) Hobart"),
	Guam_PortMoresby UMETA(DisplayName = "(UTC+10:00) Guam, Port Moresby"),
	Vladivostok UMETA(DisplayName = "(UTC+11:00) Vladivostok"),
	SolomonIs_NewCaledonia UMETA(DisplayName = "(UTC+11:00) Solomon Is., New Caledonia"),
	Magadan UMETA(DisplayName = "(UTC+12:00) Magadan"),
	Fiji UMETA(DisplayName = "(UTC+12:00) Fiji"),
	Auckland_Wellington UMETA(DisplayName = "(UTC+12:00) Auckland, Wellington"),
	CoordinatedUniversalTime12 UMETA(DisplayName = "(UTC+12:00) Coordinated Universal Time +12"),
	Nukualofa UMETA(DisplayName = "(UTC+13:00) Nuku'alofa"),

	LocalTime
};

struct FNetSyncAnimInfo
{
	FNetSyncAnimInfo()
	{
		nSyncedAnimStateIndex = -1;
	}

	uint8 TargetPCClass;

	int32 nSyncedAnimStateIndex;
	bool bSmash;
	bool bDamaged;
	bool bInQTEState;

	// EPCSpawnMotionState
	uint8 PCSpawnMotionState;

	// EAttackType 
	uint8 DamageAnimIndex;

	// EGuardBreakLogicType
	uint8 GuardBreakLogic;

	// EQTEType
	uint8 QTEType;

	// ENPCClass 
	uint8 QTENPCClass;

	// EAttackState 
	uint8 AttackState;

	// ESkillAnimType
	uint8 SkillAnimType;

	// QTEStateNum
	uint8 QTEStateNum;
};