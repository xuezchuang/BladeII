#include "B2MessageInfoConverter.h"
#include "B2NetworkPlugin.h"
//#include "ProtobufCodec.h"
//#include "ProtobufFactory.h"

#include "LogManager.h"
//namespace b2network 
//{
//	//////////////////////////////////////////////////////////////////////
//	 convert b2 enum to pb enum
//
//	b2::protocol::commondata::Character::TYPE FB2MessageInfoConverter::B2CharacterType2PbCharacter(int32 B2Character)
//	{
//		b2::protocol::commondata::Character::TYPE CharacterType = b2::protocol::commondata::Character::GLADIATOR;
//		switch (B2Character)
//		{
//		case B2CharacterType::Gladiator:
//			CharacterType = b2::protocol::commondata::Character::GLADIATOR;
//			break;
//		case B2CharacterType::Assassin:
//			CharacterType = b2::protocol::commondata::Character::ASSASSIN;
//			break;
//		case B2CharacterType::Wizard:
//			CharacterType = b2::protocol::commondata::Character::WIZARD;
//			break;
//		case B2CharacterType::Fighter:
//			CharacterType = b2::protocol::commondata::Character::FIGHTER;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//	
//	b2::protocol::commondata::Item::TYPE FB2MessageInfoConverter::B2ItemType2PbItem(int32 B2Item)
//	{
//		b2::protocol::commondata::Item::TYPE ItemType = b2::protocol::commondata::Item::WEAPON;
//		switch (B2Item)
//		{
//		case B2ItemType::Weapon:
//			ItemType = b2::protocol::commondata::Item::WEAPON;
//			break;
//		case B2ItemType::Helmet:
//			ItemType = b2::protocol::commondata::Item::HAT;
//			break;
//		case B2ItemType::BodyArmor:
//			ItemType = b2::protocol::commondata::Item::ARMOR;
//			break;
//		case B2ItemType::Glove:
//			ItemType = b2::protocol::commondata::Item::GLOVE;
//			break;
//		case B2ItemType::Shoes:
//			ItemType = b2::protocol::commondata::Item::SHOES;
//			break;
//		case B2ItemType::Belt:
//			ItemType = b2::protocol::commondata::Item::BELT;
//			break;
//		case B2ItemType::Necklace:
//			ItemType = b2::protocol::commondata::Item::NECK;
//			break;
//		case B2ItemType::Ring:
//			ItemType = b2::protocol::commondata::Item::RING;
//			break;
//		case B2ItemType::Material:
//			ItemType = b2::protocol::commondata::Item::MATERIAL;
//			break;
//		case B2ItemType::GOODS:
//			ItemType = b2::protocol::commondata::Item::GOODS;
//			break;
//		case B2ItemType::UpgradeStone:
//			ItemType = b2::protocol::commondata::Item::UPGRADE_STONE;
//			break;
//		case B2ItemType::EnhanceScroll:
//			ItemType = b2::protocol::commondata::Item::ENHANCE_SCROLL;
//			break;
//		case B2ItemType::BlessedEnhanceScroll:
//			ItemType = b2::protocol::commondata::Item::BLESSED_ENHANCE_SCROLL;
//			break;
//		case B2ItemType::ProtectionScroll:
//			ItemType = b2::protocol::commondata::Item::PROTECTION_SCROLL;
//			break;
//		case B2ItemType::BossPiece:
//			ItemType = b2::protocol::commondata::Item::BOSS_PIECE;
//			break;
//		case B2ItemType::WeaponEssence:
//			ItemType = b2::protocol::commondata::Item::WEAPON_ESSENCE;
//			break;
//		case B2ItemType::ArmorEssence:
//			ItemType = b2::protocol::commondata::Item::ARMOR_ESSENCE;
//			break;
//		case B2ItemType::AccessoryEssence:
//			ItemType = b2::protocol::commondata::Item::ACCESSORY_ESSENCE;
//			break;
//		case B2ItemType::RankPromotionPoint:
//			ItemType = b2::protocol::commondata::Item::RANK_PROMOTION_POINT;
//			break;
//		case B2ItemType::HeroPiece:
//			ItemType = b2::protocol::commondata::Item::HERO_PIECE;
//			break;
//		case B2ItemType::AetherPiece:
//			ItemType = b2::protocol::commondata::Item::AETHER_PIECE;
//			break;
//		case B2ItemType::Dismantle_Material:
//			ItemType = b2::protocol::commondata::Item::DISMANTLE_MATERIAL;
//			break;
//		case B2ItemType::DimensionalPiece:
//			ItemType = b2::protocol::commondata::Item::DIMENSIONAL_PIECE;
//			break;
//		case B2ItemType::CostumeEnhanceMaterial:
//			ItemType = b2::protocol::commondata::Item::COSTUME_ENHANCE_MATERIAL;
//			break;
//		case B2ItemType::WeaponSurpassAnvil:
//			ItemType = b2::protocol::commondata::Item::WEAPON_SURPASS_ANVIL;
//			break;
//		case B2ItemType::ArmorSurpassAnvil:
//			ItemType = b2::protocol::commondata::Item::ARMOR_SURPASS_ANVIL;
//			break;
//		case B2ItemType::AccessorySurpassAnvil:
//			ItemType = b2::protocol::commondata::Item::ACCESSORY_SURPASS_ANVIL;
//			break;
//		case B2ItemType::Gold:
//			ItemType = b2::protocol::commondata::Item::GOLD;
//			break;
//		case B2ItemType::AdvancedRankPromotionPoint:
//			ItemType = b2::protocol::commondata::Item::ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//		return ItemType;
//	}
//
//	b2::protocol::commondata::Costume::TYPE FB2MessageInfoConverter::B2CostumeType2PbCostumeType(int32 B2Costume)
//	{
//		b2::protocol::commondata::Costume::TYPE CostumeType = b2::protocol::commondata::Costume::COSTUME_WEAPON;
//		switch (B2Costume)
//		{
//		case B2CostumeType::CostumeWeapon:
//			CostumeType = b2::protocol::commondata::Costume::COSTUME_WEAPON;
//			break;
//		case B2CostumeType::CostumeHat:
//			CostumeType = b2::protocol::commondata::Costume::COSTUME_HAT;
//			break;
//		case B2CostumeType::CostumeArmor:
//			CostumeType = b2::protocol::commondata::Costume::COSTUME_ARMOR;
//			break;
//		}
//		return CostumeType;
//	}
//
//	b2::protocol::commondata::Inventory::TYPE FB2MessageInfoConverter::B2InventoryType2PbInventory(int32 B2Inventory)
//	{
//		b2::protocol::commondata::Inventory::TYPE InventoryType = b2::protocol::commondata::Inventory::WEAPON;
//		switch (B2Inventory)
//		{
//		case b2network::B2InventoryType::Weapon:
//			InventoryType = b2::protocol::commondata::Inventory::WEAPON;
//			break;
//		case b2network::B2InventoryType::Protection:
//			InventoryType = b2::protocol::commondata::Inventory::ARMOR;
//			break;
//		case b2network::B2InventoryType::Accessory:
//			InventoryType = b2::protocol::commondata::Inventory::ACCESSORY;
//			break;
//		case b2network::B2InventoryType::Wing:
//			InventoryType = b2::protocol::commondata::Inventory::WING;
//			break;
//		case b2network::B2InventoryType::CostumeWeapon:
//			InventoryType = b2::protocol::commondata::Inventory::COSTUME_WEAPON;
//			break;
//		case b2network::B2InventoryType::CostumeArmor:
//			InventoryType = b2::protocol::commondata::Inventory::COSTUME_ARMOR;
//			break;
//		case b2network::B2InventoryType::Consumables:
//			InventoryType = b2::protocol::commondata::Inventory::CONSUMABLE;
//			break;
//		default:
//			break;
//		}
//		return InventoryType;
//	}
//
//	b2::protocol::commondata::EquipPosition FB2MessageInfoConverter::B2EquipPositionType2PbEquipPosition(int32 B2EquipPos)
//	{
//		b2::protocol::commondata::EquipPosition EquipPosition = b2::protocol::commondata::WEAPON;
//		switch (B2EquipPos)
//		{
//		case b2network::B2EquipPosition::Weapon:
//			EquipPosition = b2::protocol::commondata::WEAPON;
//			break;
//		case b2network::B2EquipPosition::Helmet:
//			EquipPosition = b2::protocol::commondata::HAT;
//			break;
//		case b2network::B2EquipPosition::BodyArmor:
//			EquipPosition = b2::protocol::commondata::ARMOR;
//			break;
//		case b2network::B2EquipPosition::Glove:
//			EquipPosition = b2::protocol::commondata::GLOVE;
//			break;
//		case b2network::B2EquipPosition::Shoes:
//			EquipPosition = b2::protocol::commondata::SHOES;
//			break;
//		case b2network::B2EquipPosition::Belt:
//			EquipPosition = b2::protocol::commondata::BELT;
//			break;
//		case b2network::B2EquipPosition::Necklace:
//			EquipPosition = b2::protocol::commondata::NECK;
//			break;
//		case b2network::B2EquipPosition::Ring:
//			EquipPosition = b2::protocol::commondata::RING;
//			break;
//		case b2network::B2EquipPosition::None:
//			EquipPosition = b2::protocol::commondata::NONE;
//			break;
//		case b2network::B2EquipPosition::CostumeWeapon:
//			EquipPosition = b2::protocol::commondata::COSTUME_WEAPON;
//			break;
//		case b2network::B2EquipPosition::CostumeHat:
//			EquipPosition = b2::protocol::commondata::COSTUME_HAT;
//			break;
//		case b2network::B2EquipPosition::CostumeArmor:
//			EquipPosition = b2::protocol::commondata::COSTUME_ARMOR;
//			break;
//		default:
//			break;
//		}
//		return EquipPosition;
//	}
//
//	b2::protocol::commondata::BattleFormation::TYPE FB2MessageInfoConverter::B2BattleFormationType2PbBattleFormationType(int32 B2Formation)
//	{
//		b2::protocol::commondata::BattleFormation::TYPE formationType = b2::protocol::commondata::BattleFormation::NORMAL;
//		switch (B2Formation)
//		{
//		case b2network::B2BattleFormationType::BTBFT_Normal:
//			formationType = b2::protocol::commondata::BattleFormation::NORMAL;
//			break;
//		case b2network::B2BattleFormationType::BTBFT_Offensive:
//			formationType = b2::protocol::commondata::BattleFormation::OFFENSIVE;
//			break;
//		case b2network::B2BattleFormationType::BTBFT_Defensive:
//			formationType = b2::protocol::commondata::BattleFormation::DEFENSIVE;
//			break;
//		default:
//			break;
//		}
//		return formationType;
//	}
//
//	b2::protocol::commondata::MatchResult::TYPE FB2MessageInfoConverter::B2MatchResultType2PbMatchResultType(int32 matchResult)
//	{
//		b2::protocol::commondata::MatchResult::TYPE b2MatchResult = b2::protocol::commondata::MatchResult::DRAW;
//		switch (matchResult)
//		{
//		case b2network::B2MatchResultType::BMRT_WIN:
//			b2MatchResult = b2::protocol::commondata::MatchResult::WIN;
//			break;
//		case b2network::B2MatchResultType::BMRT_LOSE:
//			b2MatchResult = b2::protocol::commondata::MatchResult::LOSE;
//			break;
//		case b2network::B2MatchResultType::BMRT_DRAW:
//			b2MatchResult = b2::protocol::commondata::MatchResult::DRAW;
//			break;
//		default:
//			break;
//		}
//		return b2MatchResult;
//	}
//
//	b2::protocol::commondata::MatchMaking::TYPE FB2MessageInfoConverter::B2MatchMakingType2PbMatchMakingType(int32 B2MatchInfo)
//	{
//		b2::protocol::commondata::MatchMaking::TYPE b2MatchMakingType = b2::protocol::commondata::MatchMaking::PVP_RANK;
//		switch (B2MatchInfo)
//		{
//		case b2network::B2MatchMakingType::BMMT_PVP_RANK:
//			b2MatchMakingType = b2::protocol::commondata::MatchMaking::PVP_RANK;
//			break;
//		case b2network::B2MatchMakingType::BMMT_PVP_FRIENDLY:
//			b2MatchMakingType = b2::protocol::commondata::MatchMaking::PVP_FRIENDLY;
//			break;
//        case b2network::B2MatchMakingType::BMMT_RAID_RANK:
//            b2MatchMakingType = b2::protocol::commondata::MatchMaking::RAID_RANK;
//            break;
//        case b2network::B2MatchMakingType::BMMT_ASSAULT_RANK:
//            b2MatchMakingType = b2::protocol::commondata::MatchMaking::ASSAULT_RANK;
//            break;
//		default:
//			break;
//		}
//		return b2MatchMakingType;
//	}
//
//	b2::protocol::commondata::MailCategory FB2MessageInfoConverter::B2MailCategory2PbMailCategory(int32 B2MailCategory)
//	{
//		b2::protocol::commondata::MailCategory b2MailCategory = b2::protocol::commondata::MailCategory::PRESENT;
//
//		switch(B2MailCategory)
//		{
//		case b2network::B2MailCategory::PRESENT:
//			b2MailCategory = b2::protocol::commondata::MailCategory::PRESENT;
//			break;
//
//		case b2network::B2MailCategory::BLADE_POINT:
//			b2MailCategory = b2::protocol::commondata::MailCategory::BLADE_POINT;
//			break;
//
//		case b2network::B2MailCategory::SOCIAL_POINT:
//			b2MailCategory = b2::protocol::commondata::MailCategory::SOCIAL_POINT;
//			break;
//
//		case b2network::B2MailCategory::LOTTERY:
//			b2MailCategory = b2::protocol::commondata::MailCategory::LOTTERY;
//			break;
//		}
//		return b2MailCategory;
//	}
//
//	b2::protocol::commondata::MailAttachType FB2MessageInfoConverter::B2MailAttachType2PbMailAttachType(int32 B2MailAttachType)
//	{
//		b2::protocol::commondata::MailAttachType b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_GOLD;
//
//		switch (B2MailAttachType)
//		{
//			case b2network::B2MailAttachType::GOLD:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_GOLD;
//				break;
//
//			case b2network::B2MailAttachType::GEM:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_GEM;
//				break;
//
//			case b2network::B2MailAttachType::ITEM:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_ITEM;
//				break;
//
//			case b2network::B2MailAttachType::BLADE_POINT:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_BLADE_POINT;
//				break;
//
//			case b2network::B2MailAttachType::SOCIAL_POINT:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_SOCIAL_POINT;
//				break;
//
//			case b2network::B2MailAttachType::GENERAL_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_GENERAL_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::SELECTIVE_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_SELECTIVE_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::SELECTIVE_ITEM_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_SELECTIVE_ITEM_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::HERO_PIECE:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_HERO_PIECE;
//				break;
//
//			case b2network::B2MailAttachType::RANK_PROMOTION_POINT:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_RANK_PROMOTION_POINT;
//				break;
//
//			case b2network::B2MailAttachType::DARK_CRYSTAL:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_DARK_CRYSTAL;
//				break;
//
//			case b2network::B2MailAttachType::FIXED_GRADE_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_FIXED_GRADE_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::STAGE_BOOST_TICKET:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_STAGE_BOOST_TICKET;
//				break;
//
//			case b2network::B2MailAttachType::HOT_TIME_GOLD_BUFF_TICKET:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_HOT_TIME_GOLD_BUFF_TICKET;
//				break;
//
//			case b2network::B2MailAttachType::HOT_TIME_EXP_BUFF_TICKET:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_HOT_TIME_EXP_BUFF_TICKET;
//				break;
//
//			case b2network::B2MailAttachType::SHOP_TEN_LOTTERY_TICKET:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_SHOP_TEN_LOTTERY_TICKET;
//				break;
//
//			case b2network::B2MailAttachType::FAME_POINT:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_FAME_POINT;
//				break;
//
//			case b2network::B2MailAttachType::SELECTIVE_AETHER_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_SELECTIVE_AETHER_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::PREFIX_SELECTIVE_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_PREFIX_SELECTIVE_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::STAGE_AUTO_CLEAR_TICKET:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_STAGE_AUTO_CLEAR_TICKET;
//				break;
//
//			case b2network::B2MailAttachType::TOTEM_PREFIX_LOTTERY:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_TOTEM_PREFIX_LOTTERY;
//				break;
//
//			case b2network::B2MailAttachType::ADVANCED_RANK_PROMOTION_POINT:
//				b2MailAttachType = b2::protocol::commondata::MailAttachType::ATTACH_ADVANCED_RANK_PROMOTION_POINT;
//				break;
//		}
//		return b2MailAttachType;
//	}
//
//	b2::protocol::commondata::MailPresentAttachType FB2MessageInfoConverter::B2MailPresentAttachType2PbMailPresentAttachType(int32 B2MailPresentAttachType)
//	{
//		b2::protocol::commondata::MailPresentAttachType b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::MONEY;
//
//		switch (B2MailPresentAttachType)
//		{
//		case b2network::B2MailPresentAttachType::MONEY:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::MONEY;
//			break;
//		case b2network::B2MailPresentAttachType::GEM:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::GEM;
//			break;
//		case b2network::B2MailPresentAttachType::ITEM:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::ITEM;
//			break;
//		case b2network::B2MailPresentAttachType::HERO_PIECE:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::HERO_PIECE;
//			break;
//		case b2network::B2MailPresentAttachType::RANK_PROMOTION_POINT:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::RANK_PROMOTION_POINT;
//			break;
//		case b2network::B2MailPresentAttachType::DARK_CRYSTAL:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::DARK_CRYSTAL;
//			break;
//		case b2network::B2MailPresentAttachType::STAGE_BOOST_TICKET:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::STAGE_BOOST_TICKET;
//			break;
//		case b2network::B2MailPresentAttachType::HOT_TIME_GOLD_BUFF_TICKET:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::HOT_TIME_GOLD_BUFF_TICKET;
//			break;
//		case b2network::B2MailPresentAttachType::HOT_TIME_EXP_BUFF_TICKET:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::HOT_TIME_EXP_BUFF_TICKET;
//			break;
//		case b2network::B2MailPresentAttachType::SHOP_TEN_LOTTERY_TICKET:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::SHOP_TEN_LOTTERY_TICKET;
//			break;
//		case b2network::B2MailPresentAttachType::FAME_POINT:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::FAME_POINT;
//			break;
//		case b2network::B2MailPresentAttachType::STAGE_AUTO_CLEAR_TICKET:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::STAGE_AUTO_CLEAR_TICKET;
//			break;
//		case b2network::B2MailPresentAttachType::ADVANCED_RANK_PROMOTION_POINT:
//			b2MailPresentAttachType = b2::protocol::commondata::MailPresentAttachType::ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//		return b2MailPresentAttachType;
//	}
//
//
//	b2::protocol::commondata::MatchInfo::TYPE FB2MessageInfoConverter::B2MatchInfoType2PbMatchInfoType(int32 B2MatchInfoType)
//	{
//		b2::protocol::commondata::MatchInfo::TYPE b2MatchInfoType = b2::protocol::commondata::MatchInfo::PLAYER;
//
//		switch (B2MatchInfoType)
//		{
//		case b2network::B2MatchInfoType::BMIT_AI:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::AI;
//			break;
//
//		case b2network::B2MatchInfoType::BMIT_PLAYER:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::PLAYER;
//			break;
//
//		case b2network::B2MatchInfoType::BMIT_TEAM:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::TEAM;
//			break;
//		case b2network::B2MatchInfoType::BMIT_RAID:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::RAID;
//			break;
//		case b2network::B2MatchInfoType::BMIT_ASSAULT:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::ASSAULT;
//			break;
//		case b2network::B2MatchInfoType::BMIT_HEROTOWER:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::HEROTOWER;
//			break;
//		case b2network::B2MatchInfoType::BMIT_DONATION:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::DONATION;
//			break;
//		case b2network::B2MatchInfoType::BMIT_DIMENSION:
//			b2MatchInfoType = b2::protocol::commondata::MatchInfo::DIMENSION;
//			break;
//		}
//		return b2MatchInfoType;
//	}
//
//	b2::protocol::commondata::Guild::JoinOption FB2MessageInfoConverter::ToGuildJoinOption(int32 b2Value)
//	{
//		b2::protocol::commondata::Guild::JoinOption pbType = b2::protocol::commondata::Guild::PUBLIC;
//
//		switch (b2Value)
//		{
//		case b2network::B2GuildJoinOption::PUBLIC:
//			pbType = b2::protocol::commondata::Guild::PUBLIC;
//			break;
//
//		case b2network::B2GuildJoinOption::PRIVATE:
//			pbType = b2::protocol::commondata::Guild::PRIVATE;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::MissionInfo::MissionType FB2MessageInfoConverter::B2MissionTypeToPbMissionType(int32 B2MissionType)
//	{
//		b2::protocol::commondata::MissionInfo::MissionType b2MissionType = b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_DAILY;
//
//		switch (B2MissionType)
//		{
//			case b2network::B2MissionType::DAILY:
//				b2MissionType = b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_DAILY;
//			break;
//
//			case b2network::B2MissionType::WEEKLY:
//				b2MissionType = b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_WEEKLY;
//			break;
//
//			case b2network::B2MissionType::MONTHLY:
//				b2MissionType = b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_MONTHLY;
//			break;
//
//			case b2network::B2MissionType::SERIAL:
//				b2MissionType = b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_SERIAL;
//			break;
//
//			case b2network::B2MissionType::FEATURED:
//				b2MissionType = b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_FEATURED;
//			break;
//		}
//		return b2MissionType;
//	}
//
//	b2::protocol::commondata::StageDifficulty FB2MessageInfoConverter::B2StageDifficultyToPbStageDifficulty(int32 b2Value)
//	{
//		b2::protocol::commondata::StageDifficulty pbType = b2::protocol::commondata::StageDifficulty::NORMAL;
//
//		switch (b2Value)
//		{
//		case B2StageDifficulty::NORMAL:
//			pbType = b2::protocol::commondata::StageDifficulty::NORMAL;
//			break;
//
//		case B2StageDifficulty::HARD:
//			pbType = b2::protocol::commondata::StageDifficulty::HARD;
//			break;
//
//		case B2StageDifficulty::VERY_HARD:
//			pbType = b2::protocol::commondata::StageDifficulty::VERY_HARD;
//			break;
//
//		case B2StageDifficulty::HELL:
//			pbType = b2::protocol::commondata::StageDifficulty::HELL;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::session::FailStageRequest::REASON FB2MessageInfoConverter::B2StageFailReasonToPbStageFailReason(int32 b2Value)
//	{
//		b2::protocol::session::FailStageRequest::REASON pbType = b2::protocol::session::FailStageRequest::DEFEAT;
//
//		switch (b2Value)
//		{
//		case B2StageFailReason::DEFEAT:
//			pbType = b2::protocol::session::FailStageRequest::DEFEAT;
//			break;
//
//		case B2StageFailReason::GIVEUP:
//			pbType = b2::protocol::session::FailStageRequest::GIVEUP;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::StageClearProperty FB2MessageInfoConverter::B2StageClearPropertyToPbStageClearProperty(int32 b2Value)
//	{
//		b2::protocol::commondata::StageClearProperty pbType = b2::protocol::commondata::StageClearProperty::CLEAR_DEFAULT;
//
//		switch (b2Value)
//		{
//		case B2StageClearProperty::CLEAR_DEFAULT:
//			pbType = b2::protocol::commondata::StageClearProperty::CLEAR_DEFAULT;
//			break;
//
//		case B2StageClearProperty::CLEAR_IN_TIME:
//			pbType = b2::protocol::commondata::StageClearProperty::CLEAR_IN_TIME;
//			break;
//
//		case B2StageClearProperty::CLEAR_ALL_SURVIVAL:
//			pbType = b2::protocol::commondata::StageClearProperty::CLEAR_ALL_SURVIVAL;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::session::RaidReqType FB2MessageInfoConverter::ToRaidReqType(int32 B2Value)
//	{
//		switch (B2Value)
//		{
//		case B2RaidRoomType::QUICK_JOIN:
//			return b2::protocol::session::RaidReqType::QUICK_JOIN;
//		case B2RaidRoomType::CREATE_PUBLIC_ROOM:
//			return b2::protocol::session::RaidReqType::CREATE_PUBLIC_ROOM;
//		case B2RaidRoomType::CREATE_PRIVATE_ROOM:
//			return b2::protocol::session::RaidReqType::CREATE_PRIVATE_ROOM;
//		case B2RaidRoomType::REJOIN:
//			return b2::protocol::session::RaidReqType::REJOIN;
//		}
//
//		return b2::protocol::session::RaidReqType::QUICK_JOIN;
//	}
//
//	b2::protocol::session::FinishDimensionRequest::RESULT FB2MessageInfoConverter::ToPBDimensionResult(int32 b2Value)
//	{
//		b2::protocol::session::FinishDimensionRequest::RESULT pbType = b2::protocol::session::FinishDimensionRequest::CLEAR;
//
//		switch (b2Value)
//		{
//		case B2DimensionResult::CLEAR:
//			pbType = b2::protocol::session::FinishDimensionRequest::CLEAR;
//			break;
//
//		case B2DimensionResult::DEFEAT:
//			pbType = b2::protocol::session::FinishDimensionRequest::DEFEAT;
//			break;
//
//		case B2DimensionResult::GIVEUP:
//			pbType = b2::protocol::session::FinishDimensionRequest::GIVEUP;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::EquipCategory FB2MessageInfoConverter::B2EquipCategoryToPbEquipCategory(int32 b2Value)
//	{
//		b2::protocol::commondata::EquipCategory pbType = b2::protocol::commondata::EquipCategory::EQUIP_WEAPON;
//
//		switch (b2Value)
//		{
//		case B2EquipCategory::NONE:
//			pbType = b2::protocol::commondata::EquipCategory::EQUIP_NONE;
//			break;
//
//		case B2EquipCategory::WEAPON:
//			pbType = b2::protocol::commondata::EquipCategory::EQUIP_WEAPON;
//			break;
//
//		case B2EquipCategory::ARMOR:
//			pbType = b2::protocol::commondata::EquipCategory::EQUIP_ARMOR;
//			break;
//
//		case B2EquipCategory::ACCESSORY:
//			pbType = b2::protocol::commondata::EquipCategory::EQUIP_ACCESSORY;
//			break;
//
//		case B2EquipCategory::WEAPON_ESSENCE:
//			pbType = b2::protocol::commondata::EquipCategory::ESSENCE_WEAPON;
//			break;
//
//		case B2EquipCategory::ARMOR_ESSENCE:
//			pbType = b2::protocol::commondata::EquipCategory::ESSENCE_ARMOR;
//			break;
//
//		case B2EquipCategory::ACCESSORY_ESSENCE:
//			pbType = b2::protocol::commondata::EquipCategory::ESSENCE_ACCESSORY;
//			break;
//
//		case B2EquipCategory::WEAPON_SURPASS_ANVIL:
//			pbType = b2::protocol::commondata::EquipCategory::WEAPON_SURPASS_ANVIL;
//			break;
//
//		case B2EquipCategory::ARMOR_SURPASS_ANVIL:
//			pbType = b2::protocol::commondata::EquipCategory::ARMOR_SURPASS_ANVIL;
//			break;
//		case B2EquipCategory::ACCESSORY_SURPASS_ANVIL:
//			pbType = b2::protocol::commondata::EquipCategory::ACCESSORY_SURPASS_ANVIL;
//			break;
//
//		case B2EquipCategory::COSTUME_WEAPON:
//			pbType = b2::protocol::commondata::EquipCategory::EC_COSTUME_WEAPON;
//			break;
//
//		case B2EquipCategory::COSTUME_HAT:
//			pbType = b2::protocol::commondata::EquipCategory::EC_COSTUME_HAT;
//			break;
//
//		case B2EquipCategory::COSTUME_ARMOR:
//			pbType = b2::protocol::commondata::EquipCategory::EC_COSTUME_ARMOR;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::Shop::DrawPriceType FB2MessageInfoConverter::B2ShopDrawPriceTypeToPbShopDrawPriceType(int32 b2Value)
//	{
//		b2::protocol::commondata::Shop::DrawPriceType pbType = b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_DRAW_GEM;
//
//		switch (b2Value)
//		{
//		case B2ShopDrawPriceType::GEM:
//			pbType = b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_DRAW_GEM;
//			break;
//
//		case B2ShopDrawPriceType::SOCIAL_POINT:
//			pbType = b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_DRAW_SOCIAL_POINT;
//			break;
//
//		case B2ShopDrawPriceType::SHOP_TEN_LOTTERY_TICKET:
//			pbType = b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_SHOP_TEN_LOTTERY_TICKET;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::Shop::MileageType FB2MessageInfoConverter::B2ShopMileageTypeToPbShopMileageType(int32 b2Value)
//	{
//		b2::protocol::commondata::Shop::MileageType pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_LOTTERY;
//
//		switch (b2Value)
//		{
//		case B2ShopMileageType::LOTTERY:
//			pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_LOTTERY;
//			break;
//
//		case B2ShopMileageType::BLADE_POINT:
//			pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_BLADE_POINT;
//			break;
//
//		case B2ShopMileageType::GEM:
//			pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_GEM;
//			break;
//
//		case B2ShopMileageType::GOLD:
//			pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_GOLD;
//			break;
//
//		case B2ShopMileageType::DARK_CRYSTAL:
//			pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_DARK_CRYSTAL;
//			break;
//
//		case B2ShopMileageType::PACKAGE:
//			pbType = b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_PACKAGE;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::GuildBattleResult::TYPE FB2MessageInfoConverter::ToGuildBattleResultType(int32 b2Value)
//	{
//		b2::protocol::commondata::GuildBattleResult::TYPE pbType = b2::protocol::commondata::GuildBattleResult::LOSE;
//
//		switch (b2Value)
//		{
//		case B2GuildBattleResultType::BGBRT_LOSE:
//			pbType = b2::protocol::commondata::GuildBattleResult::LOSE;
//			break;
//
//		case B2GuildBattleResultType::BGBRT_DRAW:
//			pbType = b2::protocol::commondata::GuildBattleResult::DRAW;
//			break;
//
//		case B2GuildBattleResultType::BGBRT_WIN:
//			pbType = b2::protocol::commondata::GuildBattleResult::WIN;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::GuildBattleState::TYPE FB2MessageInfoConverter::ToGuildBattleStateType(int32 b2Value)
//	{
//		b2::protocol::commondata::GuildBattleState::TYPE pbType = b2::protocol::commondata::GuildBattleState::NONE;
//
//		switch (b2Value)
//		{
//		case B2GuildBattleStateType::NONE:
//			pbType = b2::protocol::commondata::GuildBattleState::NONE;
//			break;
//
//		case B2GuildBattleStateType::INIT:
//			pbType = b2::protocol::commondata::GuildBattleState::INIT;
//			break;
//
//		case B2GuildBattleStateType::INITDONE:
//			pbType = b2::protocol::commondata::GuildBattleState::INITDONE;
//			break;
//
//		case B2GuildBattleStateType::READY:
//			pbType = b2::protocol::commondata::GuildBattleState::READY;
//			break;
//
//		case B2GuildBattleStateType::MATCHING:
//			pbType = b2::protocol::commondata::GuildBattleState::MATCHING;
//			break;
//
//		case B2GuildBattleStateType::MATCHINGDONE:
//			pbType = b2::protocol::commondata::GuildBattleState::MATCHINGDONE;
//			break;
//
//		case B2GuildBattleStateType::BATTLE:
//			pbType = b2::protocol::commondata::GuildBattleState::BATTLE;
//			break;
//
//		case B2GuildBattleStateType::SETTLEMENT:
//			pbType = b2::protocol::commondata::GuildBattleState::SETTLEMENT;
//			break;
//
//		case B2GuildBattleStateType::SETTLEMENTDONE:
//			pbType = b2::protocol::commondata::GuildBattleState::SETTLEMENTDONE;
//			break;
//
//		case B2GuildBattleStateType::RESULT:
//			pbType = b2::protocol::commondata::GuildBattleState::RESULT;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::channel::RoomCloseReason FB2MessageInfoConverter::B2RoomCloseReasonToPbRoomCloseReason(int32 b2Value)
//	{
//		b2::protocol::channel::RoomCloseReason pbType = b2::protocol::channel::RoomCloseReason::START_PVP;
//
//		switch (b2Value)
//		{
//		case B2RoomCloseReason::START_PVP:
//			pbType = b2::protocol::channel::RoomCloseReason::START_PVP;
//			break;
//		case B2RoomCloseReason::START_RAID:
//			pbType = b2::protocol::channel::RoomCloseReason::START_RAID;
//			break;
//		case B2RoomCloseReason::BREAK_RAID:
//			pbType = b2::protocol::channel::RoomCloseReason::BREAK_RAID;
//			break;
//		case B2RoomCloseReason::START_ASSAULT:
//			pbType = b2::protocol::channel::RoomCloseReason::START_ASSAULT;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::GroupPrefix FB2MessageInfoConverter::ToGroupPrefix(int32 b2Value)
//	{
//		b2::protocol::commondata::GroupPrefix pbType = b2::protocol::commondata::GroupPrefix::NON_EQUIP;
//
//		switch (b2Value)
//		{
//		case B2GroupPrefix::SACRED:
//			pbType = b2::protocol::commondata::GroupPrefix::SACRED;
//			break;
//		
//		case B2GroupPrefix::MYSTIC:
//			pbType = b2::protocol::commondata::GroupPrefix::MYSTIC;
//			break;
//
//		case B2GroupPrefix::STEEL:
//			pbType = b2::protocol::commondata::GroupPrefix::STEEL;
//			break;
//
//		case B2GroupPrefix::SILENT:
//			pbType = b2::protocol::commondata::GroupPrefix::SILENT;
//			break;
//
//		case B2GroupPrefix::GROWTH:
//			pbType = b2::protocol::commondata::GroupPrefix::GROWTH;
//			break;
//
//		case B2GroupPrefix::DUEL:
//			pbType = b2::protocol::commondata::GroupPrefix::DUEL;
//			break;
//
//		case B2GroupPrefix::HEAVEN:
//			pbType = b2::protocol::commondata::GroupPrefix::HEAVEN;
//			break;
//
//		case B2GroupPrefix::ETERNAL:
//			pbType = b2::protocol::commondata::GroupPrefix::ETERNAL;
//			break;
//
//		case B2GroupPrefix::COLD:
//			pbType = b2::protocol::commondata::GroupPrefix::COLD;
//			break;
//
//		case B2GroupPrefix::CORRUPT:
//			pbType = b2::protocol::commondata::GroupPrefix::CORRUPT;
//			break;
//
//		case B2GroupPrefix::SUN:
//			pbType = b2::protocol::commondata::GroupPrefix::SUN;
//			break;
//
//		case B2GroupPrefix::BEACH:
//			pbType = b2::protocol::commondata::GroupPrefix::BEACH;
//			break;
//
//		case B2GroupPrefix::STUDY:
//			pbType = b2::protocol::commondata::GroupPrefix::STUDY;
//			break;
//
//		case B2GroupPrefix::CHRISTMAS:
//			pbType = b2::protocol::commondata::GroupPrefix::CHRISTMAS;
//			break;
//
//		case B2GroupPrefix::NEW_YEAR:
//			pbType = b2::protocol::commondata::GroupPrefix::NEW_YEAR;
//			break;
//
//		case B2GroupPrefix::HALLOWEEN:
//			pbType = b2::protocol::commondata::GroupPrefix::HALLOWEEN;
//			break;
//
//		case B2GroupPrefix::NON_EQUIP:
//			pbType = b2::protocol::commondata::GroupPrefix::NON_EQUIP;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::ItemQuality FB2MessageInfoConverter::ToItemQuality(int32 b2Value)
//	{
//		b2::protocol::commondata::ItemQuality pbType = b2::protocol::commondata::ItemQuality::COMMON;
//
//		switch (b2Value)
//		{
//		case B2ItemQuality::COMMON:
//			pbType = b2::protocol::commondata::ItemQuality::COMMON;
//			break;
//
//		case B2ItemQuality::UNCOMMON:
//			pbType = b2::protocol::commondata::ItemQuality::UNCOMMON;
//			break;
//
//		case B2ItemQuality::RARE:
//			pbType = b2::protocol::commondata::ItemQuality::RARE;
//			break;
//
//		case B2ItemQuality::EPIC:
//			pbType = b2::protocol::commondata::ItemQuality::EPIC;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::DuelMode FB2MessageInfoConverter::ToDuelMode(int32 b2Value)
//	{
//		b2::protocol::commondata::DuelMode pbType = b2::protocol::commondata::DuelMode::DUEL_MODE_NONE;
//
//		switch (b2Value)
//		{
//		case B2DuelMode::DUEL_MODE_NONE:
//			pbType = b2::protocol::commondata::DuelMode::DUEL_MODE_NONE;
//			break;
//
//		case B2DuelMode::DUEL_MODE_PVP:
//			pbType = b2::protocol::commondata::DuelMode::DUEL_MODE_PVP;
//			break;
//
//		case B2DuelMode::DUEL_MODE_TEAM:
//			pbType = b2::protocol::commondata::DuelMode::DUEL_MODE_TEAM;
//			break;
//
//		case B2DuelMode::DUEL_MODE_ASSAULT:
//			pbType = b2::protocol::commondata::DuelMode::DUEL_MODE_ASSAULT;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::RewardBoxGrade FB2MessageInfoConverter::ToRewardBoxGrade(int32 b2Value)
//	{
//		b2::protocol::commondata::RewardBoxGrade pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_NONE;
//
//		switch (b2Value)
//		{
//		case B2RewardBoxGrade::REWARD_BOX_NONE:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_NONE;
//			break;
//
//		case B2RewardBoxGrade::REWARD_BOX_BRONZE:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_BRONZE;
//			break;
//
//		case B2RewardBoxGrade::REWARD_BOX_SILVER:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_SILVER;
//			break;
//
//		case B2RewardBoxGrade::REWARD_BOX_GOLD:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_GOLD;
//			break;
//
//		case B2RewardBoxGrade::REWARD_BOX_PLATINUM:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_PLATINUM;
//			break;
//
//		case B2RewardBoxGrade::REWARD_BOX_DIAMOND:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_DIAMOND;
//			break;
//
//		case B2RewardBoxGrade::REWARD_BOX_MASTER:
//			pbType = b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_MASTER;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::session::ChatType FB2MessageInfoConverter::ToChatType(int32 b2Value)
//	{
//		b2::protocol::session::ChatType pbType = b2::protocol::session::ChatType::CHANNEL;
//
//		switch (b2Value)
//		{
//		case B2ChatType::NOTICE:
//			pbType = b2::protocol::session::ChatType::NOTICE;
//			break;
//
//		case B2ChatType::CHANNEL:
//			pbType = b2::protocol::session::ChatType::CHANNEL;
//			break;
//
//		case B2ChatType::GUILD_NOTICE:
//			pbType = b2::protocol::session::ChatType::GUILD_NOTICE;
//			break;
//
//		case B2ChatType::GUILD:
//			pbType = b2::protocol::session::ChatType::GUILD;
//			break;
//
//		case B2ChatType::ONE_TO_ONE:
//			pbType = b2::protocol::session::ChatType::ONE_TO_ONE;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::Aether::TYPE FB2MessageInfoConverter::ToAetherType(int32 b2Value)
//	{
//		b2::protocol::commondata::Aether::TYPE pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_ALL;
//
//		switch (b2Value)
//		{
//		case B2AetherType::THUNDERSTORM:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_THUNDERSTORM;
//			break;
//		case B2AetherType::DESTROY:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_DESTROY;
//			break;
//		case B2AetherType::INSIGHT:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_INSIGHT;
//			break;
//		case B2AetherType::RAGE:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_RAGE;
//			break;
//		case B2AetherType::DESPAIR:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_DESPAIR;
//			break;
//		case B2AetherType::RESTRAINT:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_RESTRAINT;
//			break;
//		case B2AetherType::RESIST:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_RESIST;
//			break;
//		case B2AetherType::REVENGE:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_REVENGE;
//			break;
//		case B2AetherType::PROTECT:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_PROTECT;
//			break;
//		case B2AetherType::PASSION:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_PASSION;
//			break;
//		case B2AetherType::OBLIVION:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_OBLIVION;
//			break;
//		case B2AetherType::FREEZING:
//			pbType = b2::protocol::commondata::Aether::TYPE::Aether_TYPE_FREEZING;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::Aether::TIER FB2MessageInfoConverter::ToAetherTier(int32 b2Value)
//	{
//		b2::protocol::commondata::Aether::TIER pbType = b2::protocol::commondata::Aether::TIER::Aether_TIER_SEAL;
//
//		switch (b2Value)
//		{
//		case B2AetherTierType::SEAL:
//			pbType = b2::protocol::commondata::Aether::TIER::Aether_TIER_SEAL;
//			break;
//		case B2AetherTierType::ANTI_DEVIL:
//			pbType = b2::protocol::commondata::Aether::TIER::Aether_TIER_ANTI_DEVIL;
//			break;
//		case B2AetherTierType::EXTERMINATE_DEVIL:
//			pbType = b2::protocol::commondata::Aether::TIER::Aether_TIER_EXTERMINATE_DEVIL;
//			break;
//		case B2AetherTierType::ETERNAL_LIFE:
//			pbType = b2::protocol::commondata::Aether::TIER::Aether_TIER_ETERNAL_LIFE;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::Aether::SetOptionType FB2MessageInfoConverter::ToAetherSetOptionType(int32 b2Value)
//	{
//		b2::protocol::commondata::Aether::SetOptionType pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_THUNDER_DAMAGE;
//
//		switch (b2Value)
//		{
//		case B2AetherSetOptionType::THUNDER_DAMAGE:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_THUNDER_DAMAGE;
//			break;
//		case B2AetherSetOptionType::IMMOBILIZE:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_IMMOBILIZE;
//			break;
//		case B2AetherSetOptionType::SHIELD:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SHIELD;
//			break;
//		case B2AetherSetOptionType::FIRE_RANGE:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_FIRE_RANGE;
//			break;
//		case B2AetherSetOptionType::METEOR_DAMAGE:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_METEOR_DAMAGE;
//			break;
//		case B2AetherSetOptionType::SKILL_COOLTIME:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SKILL_COOLTIME;
//			break;
//		case B2AetherSetOptionType::SKILL_GAGE:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SKILL_GAGE;
//			break;
//		case B2AetherSetOptionType::STUN:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_STUN;
//			break;
//		case B2AetherSetOptionType::SUPER_ARMOR:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SUPER_ARMOR;
//			break;
//		case B2AetherSetOptionType::DAMAGE_REFLECT:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_DAMAGE_REFLECT;
//			break;
//		case B2AetherSetOptionType::PROHIBIT_BEHAVIOR:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_PROHIBIT_BEHAVIOR;
//			break;
//		case B2AetherSetOptionType::FREEZE_RANGE:
//			pbType = b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_FREEZE_RANGE;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::masterdata::mdGuildMercenaryDonation::DonationType FB2MessageInfoConverter::ToDonationType(int32 b2Value)
//	{
//		b2::protocol::masterdata::mdGuildMercenaryDonation::DonationType pbType = b2::protocol::masterdata::mdGuildMercenaryDonation::GOLD;
//
//		switch (b2Value)
//		{
//		case B2GuildMercenaryDonationType::GOLD:
//			pbType = b2::protocol::masterdata::mdGuildMercenaryDonation::GOLD;
//			break;
//
//		case B2GuildMercenaryDonationType::GEM:
//			pbType = b2::protocol::masterdata::mdGuildMercenaryDonation::GEM;
//			break;
//		}
//		return pbType;
//	}
//
//	b2::protocol::commondata::MatchInfo::TYPE FB2MessageInfoConverter::ToMatchType(int32 b2Value)
//	{
//		b2::protocol::commondata::MatchInfo::TYPE pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_AI;
//
//		switch (b2Value)
//		{
//		case B2MatchType::AI:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_AI;
//			break;
//
//		case B2MatchType::PLAYER:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_PLAYER;
//			break;
//
//		case B2MatchType::TEAM:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_TEAM;
//			break;
//
//		case B2MatchType::RAID:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_RAID;
//			break;
//
//		case B2MatchType::ASSAULT:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_ASSAULT;
//			break;
//
//		case B2MatchType::HEROTOWER:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_HEROTOWER;
//			break;
//
//		case B2MatchType::DONATION:
//			pbType = b2::protocol::commondata::MatchInfo::TYPE::MatchInfo_TYPE_DONATION;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::FriendInviteRewardType FB2MessageInfoConverter::ToFriendInviteRewardType(int32 b2Value)
//	{
//		b2::protocol::commondata::FriendInviteRewardType pbType = b2::protocol::commondata::FriendInviteRewardType::INVITE_COUNT;
//		switch (b2Value)
//		{
//		case B2FriendInviteRewardType::INVITE_COUNT:
//			pbType = b2::protocol::commondata::FriendInviteRewardType::INVITE_COUNT;
//			break;
//		case B2FriendInviteRewardType::JOINER_COUNT:
//			pbType = b2::protocol::commondata::FriendInviteRewardType::JOINER_COUNT;
//			break;
//		}
//
//		return pbType;
//	}
//	
//	b2::protocol::session::CancelPurchaseRequest::REASON  FB2MessageInfoConverter::ToMarketPurchaseCancel(int32 b2Value)
//	{
//		b2::protocol::session::CancelPurchaseRequest::REASON pbType = b2::protocol::session::CancelPurchaseRequest::CANCEL_CURRENT;
//		switch (b2Value)
//		{
//		case B2MarketPurchaseCancel::CANCEL_CURRENT:
//			pbType = b2::protocol::session::CancelPurchaseRequest::CANCEL_CURRENT;
//			break;
//		case B2MarketPurchaseCancel::CANCEL_PAST:
//			pbType = b2::protocol::session::CancelPurchaseRequest::CANCEL_PAST;
//			break;
//		case B2MarketPurchaseCancel::ORDER_FAILURE:
//			pbType = b2::protocol::session::CancelPurchaseRequest::ORDER_FAILURE;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::ContentsMode FB2MessageInfoConverter::ToContentsMode(int32 b2Value)
//	{
//		b2::protocol::commondata::ContentsMode pbType = b2::protocol::commondata::ContentsMode::CM_STAGE;
//		switch (b2Value)
//		{
//		case B2ContentsMode::STAGE:
//			pbType = b2::protocol::commondata::ContentsMode::CM_STAGE;
//			break;
//
//		case B2ContentsMode::PVP_DUEL:
//			pbType = b2::protocol::commondata::ContentsMode::CM_PVP_DUEL;
//			break;
//
//		case B2ContentsMode::PVP_TEAM:
//			pbType = b2::protocol::commondata::ContentsMode::CM_PVP_TEAM;
//			break;
//
//		case B2ContentsMode::ASSAULT:
//			pbType = b2::protocol::commondata::ContentsMode::CM_ASSAULT;
//			break;
//
//		case B2ContentsMode::GUILD:
//			pbType = b2::protocol::commondata::ContentsMode::CM_GUILD;
//			break;
//
//		case B2ContentsMode::HERO_TOWER:
//			pbType = b2::protocol::commondata::ContentsMode::CM_HERO_TOWER;
//			break;
//
//		case B2ContentsMode::RAID:
//			pbType = b2::protocol::commondata::ContentsMode::CM_RAID;
//			break;
//
//		case B2ContentsMode::PROMOTION:
//			pbType = b2::protocol::commondata::ContentsMode::CM_PROMOTION;
//			break;
//
//		case B2ContentsMode::COUNTER:
//			pbType = b2::protocol::commondata::ContentsMode::CM_COUNTER;
//			break;
//
//		case B2ContentsMode::FRIEND:
//			pbType = b2::protocol::commondata::ContentsMode::CM_FRIEND;
//			break;
//
//		case B2ContentsMode::INVENTORY:
//			pbType = b2::protocol::commondata::ContentsMode::CM_INVENTORY;
//			break;
//
//		case B2ContentsMode::MISSION:
//			pbType = b2::protocol::commondata::ContentsMode::CM_MISSION;
//			break;
//
//		case B2ContentsMode::SHOP:
//			pbType = b2::protocol::commondata::ContentsMode::CM_SHOP;
//			break;
//
//		case B2ContentsMode::DONATION:
//			pbType = b2::protocol::commondata::ContentsMode::CM_DONATION;
//			break;
//
//		case B2ContentsMode::DIMENSION:
//			pbType = b2::protocol::commondata::ContentsMode::CM_DIMENSION;
//			break;
//
//		case B2ContentsMode::COSTUME:
//			pbType = b2::protocol::commondata::ContentsMode::CM_COSTUME;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::StageRepeatMode FB2MessageInfoConverter::B2StageRepeatModeToPbStageRepeatMode(int32 b2Value)
//	{
//		b2::protocol::commondata::StageRepeatMode pbType = b2::protocol::commondata::StageRepeatMode::SRM_NORMAL;
//		switch (b2Value)
//		{
//		case B2StageRepeatMode::NORMAL:
//			pbType = b2::protocol::commondata::StageRepeatMode::SRM_NORMAL;
//			break;
//		case B2StageRepeatMode::REPEATED:
//			pbType = b2::protocol::commondata::StageRepeatMode::SRM_REPEATED;
//			break;
//		case B2StageRepeatMode::CONTINUOUS:
//			pbType = b2::protocol::commondata::StageRepeatMode::SRM_CONTINUOUS;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::Shop::BuyPriceType FB2MessageInfoConverter::B2ShopBuyPriceTypeToPbShopBuyPriceType(int32 b2Value)
//	{
//		b2::protocol::commondata::Shop::BuyPriceType pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_GEM;
//		switch (b2Value)
//		{
//		case B2ShopBuyPriceType::GEM:
//			pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_GEM;
//			break;
//		case B2ShopBuyPriceType::SOCIAL_POINT:
//			pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_SOCIAL_POINT;
//			break;
//		case B2ShopBuyPriceType::DARK_CRYSTAL:
//			pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_DARK_CRYSTAL;
//			break;
//		case B2ShopBuyPriceType::CASH:
//			pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_CASH;
//			break;
//		case B2ShopBuyPriceType::GOLD:
//			pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_GOLD;
//			break;
//		case B2ShopBuyPriceType::FAME_POINT:
//			pbType = b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_FAME_POINT;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::ItemGetRouteType FB2MessageInfoConverter::B2ItemGetRouteType2PbItemGetRouteType(int32 b2Value)
//	{
//		b2::protocol::commondata::ItemGetRouteType pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_ETC;
//		switch (b2Value)
//		{
//		case B2ItemGetRouteType::BUY_SHOP:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_BUY_SHOP;
//			break;
//		case B2ItemGetRouteType::LOTTERY:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_LOTTERY;
//			break;
//		case B2ItemGetRouteType::UPGRADE_ITEM:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_UPGRADE_ITEM;
//			break;
//		case B2ItemGetRouteType::COMPOSE_ITEM:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_COMPOSE_ITEM;
//			break;
//		case B2ItemGetRouteType::DISMANTLE_AETHER:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_AETHER;
//			break;
//		case B2ItemGetRouteType::DISMANTLE_ITEM:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_ITEM;
//			break;
//		case B2ItemGetRouteType::MAKE_DONATION:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_MAKE_DONATION;
//			break;
//		case B2ItemGetRouteType::DISMANTLE_TOTEM:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_TOTEM;
//			break;
//		case B2ItemGetRouteType::DISMANTLE_COSTUME:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_COSTUME;
//			break;
//		case B2ItemGetRouteType::FACTORY_MAKE:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_FACTORY_MAKE;
//			break;
//		case B2ItemGetRouteType::FACTORY_UPGRADE:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_FACTORY_UPGRADE;
//			break;
//		case B2ItemGetRouteType::UNSEAL_BOX:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_UNSEAL_BOX;
//			break;
//		case B2ItemGetRouteType::ETC:
//			pbType = b2::protocol::commondata::ItemGetRouteType::ROUTE_ETC;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::FairyType FB2MessageInfoConverter::B2FairyType2PbFairyType(int32 b2Value)
//	{
//		b2::protocol::commondata::FairyType pbType = b2::protocol::commondata::FairyType::NIECE;
//		switch (b2Value)
//		{
//		case B2FairyType::NIECE:
//			pbType = b2::protocol::commondata::FairyType::NIECE;
//			break;
//		case B2FairyType::PANG:
//			pbType = b2::protocol::commondata::FairyType::PANG;
//			break;
//		case B2FairyType::RIO:
//			pbType = b2::protocol::commondata::FairyType::RIO;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::TotemEquipSlot FB2MessageInfoConverter::B2TotemEquipSlot2PbTotemEquipSlot(int32 b2Value)
//	{
//		b2::protocol::commondata::TotemEquipSlot pbType = b2::protocol::commondata::TotemEquipSlot::TOTEM_NONE;
//		switch (b2Value)
//		{
//		case B2TotemEquipSlot::NONE:
//			pbType = b2::protocol::commondata::TotemEquipSlot::TOTEM_NONE;
//			break;
//		case B2TotemEquipSlot::TOP:
//			pbType = b2::protocol::commondata::TotemEquipSlot::TOTEM_TOP;
//			break;
//		case B2TotemEquipSlot::MIDDLE:
//			pbType = b2::protocol::commondata::TotemEquipSlot::TOTEM_MIDDLE;
//			break;
//		case B2TotemEquipSlot::BOTTOM:
//			pbType = b2::protocol::commondata::TotemEquipSlot::TOTEM_BOTTOM;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::TotemGrade FB2MessageInfoConverter::B2TotemGrade2PbTotemGrade(int32 b2Value)
//	{
//		b2::protocol::commondata::TotemGrade pbType = b2::protocol::commondata::TotemGrade::TG_NORMAL;
//		switch (b2Value)
//		{
//		case B2TotemGrade::NORMAL:
//			pbType = b2::protocol::commondata::TotemGrade::TG_NORMAL;
//			break;
//		case B2TotemGrade::MAGIC:
//			pbType = b2::protocol::commondata::TotemGrade::TG_MAGIC;
//			break;
//		case B2TotemGrade::RARE:
//			pbType = b2::protocol::commondata::TotemGrade::TG_RARE;
//			break;
//		case B2TotemGrade::HERO:
//			pbType = b2::protocol::commondata::TotemGrade::TG_HERO;
//			break;
//		case B2TotemGrade::LEGEND:
//			pbType = b2::protocol::commondata::TotemGrade::TG_LEGEND;
//			break;
//		case B2TotemGrade::RELIC:
//			pbType = b2::protocol::commondata::TotemGrade::TG_RELIC;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::TotemBasePrefix FB2MessageInfoConverter::B2TotemBasePrefix2PbTotemBasePrefix(int32 b2Value)
//	{
//		b2::protocol::commondata::TotemBasePrefix pbType = b2::protocol::commondata::TotemBasePrefix::TBP_VALOROUS;
//		switch (b2Value)
//		{
//		case B2TotemBasePrefix::VALOROUS:
//			pbType = b2::protocol::commondata::TotemBasePrefix::TBP_VALOROUS;
//			break;
//		case B2TotemBasePrefix::PATRON:
//			pbType = b2::protocol::commondata::TotemBasePrefix::TBP_PATRON;
//			break;
//		case B2TotemBasePrefix::VIGOROUS:
//			pbType = b2::protocol::commondata::TotemBasePrefix::TBP_VIGOROUS;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::TotemRefinePrefix FB2MessageInfoConverter::B2TotemRefinePrefix2PbTotemRefinePrefix(int32 b2Value)
//	{
//		b2::protocol::commondata::TotemRefinePrefix pbType = b2::protocol::commondata::TotemRefinePrefix::TRP_ECLIPSE;
//		switch (b2Value)
//		{
//		case B2TotemRefinePrefix::ECLIPSE:
//			pbType = b2::protocol::commondata::TotemRefinePrefix::TRP_ECLIPSE;
//			break;
//		case B2TotemRefinePrefix::SHINING:
//			pbType = b2::protocol::commondata::TotemRefinePrefix::TRP_SHINING;
//			break;
//		case B2TotemRefinePrefix::STRONG:
//			pbType = b2::protocol::commondata::TotemRefinePrefix::TRP_STRONG;
//			break;
//		case B2TotemRefinePrefix::SACRED:
//			pbType = b2::protocol::commondata::TotemRefinePrefix::TRP_SACRED;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::TotemRefineCostType FB2MessageInfoConverter::B2TotemRefineCostType2PbTotemRefineCostType(int32 b2Value)
//	{
//		b2::protocol::commondata::TotemRefineCostType pbType = b2::protocol::commondata::TotemRefineCostType::TRC_GOLD;
//		switch (b2Value)
//		{
//		case B2TotemRefineCostType::GOLD:
//			pbType = b2::protocol::commondata::TotemRefineCostType::TRC_GOLD;
//			break;
//		case B2TotemRefineCostType::DIMENSIONAL_PIECE:
//			pbType = b2::protocol::commondata::TotemRefineCostType::TRC_DIMENSIONAL_PIECE;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::FactoryLimitType FB2MessageInfoConverter::B2FactoryLimitType2PbFactoryLimitType(int32 b2Value)
//	{
//		b2::protocol::commondata::FactoryLimitType pbType = b2::protocol::commondata::FactoryLimitType::FL_TRY;
//		switch (b2Value)
//		{
//		case B2FactoryLimitType::TRY:
//			pbType = b2::protocol::commondata::FactoryLimitType::FL_TRY;
//			break;
//		case B2FactoryLimitType::SUCCESS:
//			pbType = b2::protocol::commondata::FactoryLimitType::FL_SUCCESS;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::FactoryLimitTimeType FB2MessageInfoConverter::B2FactoryLimitTimeType2PbFactoryLimitTimeType(int32 b2Value)
//	{
//		b2::protocol::commondata::FactoryLimitTimeType pbType = b2::protocol::commondata::FactoryLimitTimeType::FLT_ONCE;
//		switch (b2Value)
//		{
//		case B2FactoryLimitTimeType::ONCE:
//			pbType = b2::protocol::commondata::FactoryLimitTimeType::FLT_ONCE;
//			break;
//		case B2FactoryLimitTimeType::DAY:
//			pbType = b2::protocol::commondata::FactoryLimitTimeType::FLT_DAY;
//			break;
//		case B2FactoryLimitTimeType::WEEK:
//			pbType = b2::protocol::commondata::FactoryLimitTimeType::FLT_WEEK;
//			break;
//		case B2FactoryLimitTimeType::MONTH:
//			pbType = b2::protocol::commondata::FactoryLimitTimeType::FLT_MONTH;
//			break;
//		case B2FactoryLimitTimeType::NONE:
//			pbType = b2::protocol::commondata::FactoryLimitTimeType::FLT_NONE;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::FactoryVisibleType FB2MessageInfoConverter::B2FactoryVisibleType2PbFactoryVisibleType(int32 b2Value)
//	{
//		b2::protocol::commondata::FactoryVisibleType pbType = b2::protocol::commondata::FactoryVisibleType::FV_CLOSE;
//		switch (b2Value)
//		{
//		case B2FactoryVisibleType::CLOSE:
//			pbType = b2::protocol::commondata::FactoryVisibleType::FV_CLOSE;
//			break;
//		case B2FactoryVisibleType::OPEN:
//			pbType = b2::protocol::commondata::FactoryVisibleType::FV_OPEN;
//			break;
//		case B2FactoryVisibleType::PERIOD:
//			pbType = b2::protocol::commondata::FactoryVisibleType::FV_PERIOD;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::DonationConsumableType FB2MessageInfoConverter::B2DonationConsumableType2PbDonationConsumableType(int32 b2Value)
//	{
//		b2::protocol::commondata::DonationConsumableType pbType = b2::protocol::commondata::DonationConsumableType::CT_UNDEFINED;
//		switch (b2Value)
//		{
//		case B2DonationConsumableType::GEM:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_GEM;
//			break;
//		case B2DonationConsumableType::BLADE_POINT:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_BLADE_POINT;
//			break;
//		case B2DonationConsumableType::RANK_PROMOTION_POINT:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_RANK_PROMOTION_POINT;
//			break;
//		case B2DonationConsumableType::HERO_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_HERO_PIECE;
//			break;
//		case B2DonationConsumableType::MAGIC_CRYSTAL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_MAGIC_CRYSTAL;
//			break;
//		case B2DonationConsumableType::TEARS_WIND:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_TEARS_WIND;
//			break;
//		case B2DonationConsumableType::FEATHER_ARCHANGEL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_FEATHER_ARCHANGEL;
//			break;
//		case B2DonationConsumableType::WEAPON_UPGRADE_STONE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_WEAPON_UPGRADE_STONE;
//			break;
//		case B2DonationConsumableType::ARMOR_UPGRADE_STONE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_ARMOR_UPGRADE_STONE;
//			break;
//		case B2DonationConsumableType::ACCESSORY_UPGRADE_STONE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_ACCESSORY_UPGRADE_STONE;
//			break;
//		case B2DonationConsumableType::WEAPON_ENHANCE_SCROLL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_WEAPON_ENHANCE_SCROLL;
//			break;
//		case B2DonationConsumableType::ARMOR_ENHANCE_SCROLL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_ARMOR_ENHANCE_SCROLL;
//			break;
//		case B2DonationConsumableType::ACCESSORY_ENHANCE_SCROLL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_ACCESSORY_ENHANCE_SCROLL;
//			break;
//		case B2DonationConsumableType::BLESSED_WEAPON_ENHANCE_SCROLL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_BLESSED_WEAPON_ENHANCE_SCROLL;
//			break;
//		case B2DonationConsumableType::BLESSED_ARMOR_ENHANCE_SCROLL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_BLESSED_ARMOR_ENHANCE_SCROLL;
//			break;
//		case B2DonationConsumableType::BLESSED_ACCESSORY_ENHANCE_SCROLL:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_BLESSED_ACCESSORY_ENHANCE_SCROLL;
//			break;
//		case B2DonationConsumableType::RAMIEL_PROTECTION:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_RAMIEL_PROTECTION;
//			break;
//		case B2DonationConsumableType::AETHER_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_AETHER_PIECE;
//			break;
//		case B2DonationConsumableType::CALIGULA_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_CALIGULA_PIECE;
//			break;
//		case B2DonationConsumableType::LILLIS_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_LILLIS_PIECE;
//			break;
//		case B2DonationConsumableType::BEELZEBUB_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_BEELZEBUB_PIECE;
//			break;
//		case B2DonationConsumableType::AZRAEL_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_AZRAEL_PIECE;
//			break;
//		case B2DonationConsumableType::NIDHOGG_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_NIDHOGG_PIECE;
//			break;
//		case B2DonationConsumableType::APOCALYPSE_PIECE:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_APOCALYPSE_PIECE;
//			break;
//		case B2DonationConsumableType::LIGHT_LAMP:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_LIGHT_LAMP;
//			break;
//		case B2DonationConsumableType::ADVANCED_RANK_PROMOTION_POINT:
//			pbType = b2::protocol::commondata::DonationConsumableType::CT_ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//
//		return pbType;
//	}
//
//	b2::protocol::commondata::DonationCostType FB2MessageInfoConverter::B2DonationCostType2PbDonationCostType(int32 b2Value)
//	{
//		b2::protocol::commondata::DonationCostType pbType = b2::protocol::commondata::DonationCostType::DC_GEM;
//		switch (b2Value)
//		{
//		case B2DonationCostType::GEM:
//			pbType = b2::protocol::commondata::DonationCostType::DC_GEM;
//			break;
//		case B2DonationCostType::GOLD:
//			pbType = b2::protocol::commondata::DonationCostType::DC_GOLD;
//		}
//
//		return pbType;
//	}
//
//	////////////////////////////////////////////////////////////////////////
//	 convert pb enum to b2 enum
//
//	int32 FB2MessageInfoConverter::PbCharacter2B2CharacterType(b2::protocol::commondata::Character::TYPE PbCharacter)
//	{
//		int32 CharacterType = B2CharacterType::None;
//		switch (PbCharacter)
//		{
//		case b2::protocol::commondata::Character::GLADIATOR:
//			CharacterType = B2CharacterType::Gladiator;
//			break;
//		case b2::protocol::commondata::Character::ASSASSIN:
//			CharacterType = B2CharacterType::Assassin;
//			break;
//		case b2::protocol::commondata::Character::WIZARD:
//			CharacterType = B2CharacterType::Wizard;
//			break;
//		case b2::protocol::commondata::Character::FIGHTER:
//			CharacterType = B2CharacterType::Fighter;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//
//	int32 FB2MessageInfoConverter::PbLightCharacter2B2CharacterType(b2::protocol::commondata::LightCharacter::TYPE PbLightCharacter)
//	{
//		int32 CharacterType = B2CharacterType::None;
//		switch (PbLightCharacter)
//		{
//		case b2::protocol::commondata::LightCharacter::GLADIATOR:
//			CharacterType = B2CharacterType::Gladiator;
//			break;
//		case b2::protocol::commondata::LightCharacter::ASSASSIN:
//			CharacterType = B2CharacterType::Assassin;
//			break;
//		case b2::protocol::commondata::LightCharacter::WIZARD:
//			CharacterType = B2CharacterType::Wizard;
//			break;
//		case b2::protocol::commondata::LightCharacter::FIGHTER:
//			CharacterType = B2CharacterType::Fighter;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//	
//	int32 FB2MessageInfoConverter::PbItemCharacter2B2CharacterType(b2::protocol::commondata::Item::CharacterType PbCharacter)
//	{
//		int32 CharacterType = B2CharacterType::None;
//		switch (PbCharacter)
//		{
//		case b2::protocol::commondata::Item::COMMON:
//			CharacterType = B2CharacterType::Common;
//			break;
//		case b2::protocol::commondata::Item::GLADIATOR:
//			CharacterType = B2CharacterType::Gladiator;
//			break;
//		case b2::protocol::commondata::Item::ASSASSIN:
//			CharacterType = B2CharacterType::Assassin;
//			break;
//		case b2::protocol::commondata::Item::WIZARD:
//			CharacterType = B2CharacterType::Wizard;
//			break;
//		case b2::protocol::commondata::Item::FIGHTER:
//			CharacterType = B2CharacterType::Fighter;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//
//	int32 FB2MessageInfoConverter::PbCostumeCharacter2B2CharacterType(b2::protocol::commondata::Costume::CharacterType PbCharacter)
//	{
//		int32 CharacterType = B2CharacterType::Gladiator;
//		switch (PbCharacter)
//		{
//		case b2::protocol::commondata::Costume::GLADIATOR:
//			CharacterType = B2CharacterType::Gladiator;
//			break;
//		case b2::protocol::commondata::Costume::ASSASSIN:
//			CharacterType = B2CharacterType::Assassin;
//			break;
//		case b2::protocol::commondata::Costume::WIZARD:
//			CharacterType = B2CharacterType::Wizard;
//			break;
//		case b2::protocol::commondata::Costume::FIGHTER:
//			CharacterType = B2CharacterType::Fighter;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//
//	int32 FB2MessageInfoConverter::PbTotemCharacter2B2CharacterType(b2::protocol::commondata::Totem::CharacterType PbCharacter)
//	{
//		int32 CharacterType = B2CharacterType::None;
//		switch (PbCharacter)
//		{
//		case b2::protocol::commondata::Totem::NONE:
//			CharacterType = B2CharacterType::None;
//			break;
//		case b2::protocol::commondata::Totem::GLADIATOR:
//			CharacterType = B2CharacterType::Gladiator;
//			break;
//		case b2::protocol::commondata::Totem::ASSASSIN:
//			CharacterType = B2CharacterType::Assassin;
//			break;
//		case b2::protocol::commondata::Totem::WIZARD:
//			CharacterType = B2CharacterType::Wizard;
//			break;
//		case b2::protocol::commondata::Totem::FIGHTER:
//			CharacterType = B2CharacterType::Fighter;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//
//	int32 FB2MessageInfoConverter::PbAetherCharacter2B2CharacterType(b2::protocol::commondata::Aether::CharacterType PbCharacter)
//	{
//		int32 CharacterType = B2CharacterType::Not_Used;
//		switch (PbCharacter)
//		{
//		case b2::protocol::commondata::Aether::NOT_USED:
//			CharacterType = B2CharacterType::Not_Used;
//			break;
//		case b2::protocol::commondata::Aether::GLADIATOR:
//			CharacterType = B2CharacterType::Gladiator;
//			break;
//		case b2::protocol::commondata::Aether::ASSASSIN:
//			CharacterType = B2CharacterType::Assassin;
//			break;
//		case b2::protocol::commondata::Aether::WIZARD:
//			CharacterType = B2CharacterType::Wizard;
//			break;
//		case b2::protocol::commondata::Aether::FIGHTER:
//			CharacterType = B2CharacterType::Fighter;
//			break;
//		default:
//			break;
//		}
//		return CharacterType;
//	}
//
//	int32 FB2MessageInfoConverter::PbItem2B2ItemType(b2::protocol::commondata::Item::TYPE PbItem)
//	{
//		int32 ItemType = B2ItemType::None;
//		switch (PbItem)
//		{
//		case b2::protocol::commondata::Item::WEAPON:
//			ItemType = B2ItemType::Weapon;
//			break;
//		case b2::protocol::commondata::Item::HAT:
//			ItemType = B2ItemType::Helmet;
//			break;
//		case b2::protocol::commondata::Item::ARMOR:
//			ItemType = B2ItemType::BodyArmor;
//			break;
//		case b2::protocol::commondata::Item::GLOVE:
//			ItemType = B2ItemType::Glove;
//			break;
//		case b2::protocol::commondata::Item::SHOES:
//			ItemType = B2ItemType::Shoes;
//			break;
//		case b2::protocol::commondata::Item::BELT:
//			ItemType = B2ItemType::Belt;
//			break;
//		case b2::protocol::commondata::Item::NECK:
//			ItemType = B2ItemType::Necklace;
//			break;
//		case b2::protocol::commondata::Item::RING:
//			ItemType = B2ItemType::Ring;
//			break;
//		case b2::protocol::commondata::Item::MATERIAL:
//			ItemType = B2ItemType::Material;
//			break;
//		case b2::protocol::commondata::Item::GOODS:
//			ItemType = B2ItemType::GOODS;
//			break;
//		case b2::protocol::commondata::Item::UPGRADE_STONE:
//			ItemType = B2ItemType::UpgradeStone;
//			break;
//		case b2::protocol::commondata::Item::ENHANCE_SCROLL:
//			ItemType = B2ItemType::EnhanceScroll;
//			break;
//		case b2::protocol::commondata::Item::BLESSED_ENHANCE_SCROLL:
//			ItemType = B2ItemType::BlessedEnhanceScroll;
//			break;
//		case b2::protocol::commondata::Item::PROTECTION_SCROLL:
//			ItemType = B2ItemType::ProtectionScroll;
//			break;
//		case b2::protocol::commondata::Item::BOSS_PIECE:
//			ItemType = B2ItemType::BossPiece;
//			break;
//		case b2::protocol::commondata::Item::WEAPON_ESSENCE:
//			ItemType = B2ItemType::WeaponEssence;
//			break;
//		case b2::protocol::commondata::Item::ARMOR_ESSENCE:
//			ItemType = B2ItemType::ArmorEssence;
//			break;
//		case b2::protocol::commondata::Item::ACCESSORY_ESSENCE:
//			ItemType = B2ItemType::AccessoryEssence;
//			break;
//		case b2::protocol::commondata::Item::RANK_PROMOTION_POINT:
//			ItemType = B2ItemType::RankPromotionPoint;
//			break;
//		case b2::protocol::commondata::Item::HERO_PIECE:
//			ItemType = B2ItemType::HeroPiece;
//			break;
//		case b2::protocol::commondata::Item::AETHER_PIECE:
//			ItemType = B2ItemType::AetherPiece;
//			break;
//		case b2::protocol::commondata::Item::DISMANTLE_MATERIAL:
//			ItemType = B2ItemType::Dismantle_Material;
//			break;
//		case b2::protocol::commondata::Item::DIMENSIONAL_PIECE:
//			ItemType = B2ItemType::DimensionalPiece;
//			break;
//		case b2::protocol::commondata::Item::COSTUME_ENHANCE_MATERIAL:
//			ItemType = B2ItemType::CostumeEnhanceMaterial;
//			break;
//		case b2::protocol::commondata::Item::WEAPON_SURPASS_ANVIL:
//			ItemType = B2ItemType::WeaponSurpassAnvil;
//			break;
//		case b2::protocol::commondata::Item::ARMOR_SURPASS_ANVIL:
//			ItemType = B2ItemType::ArmorSurpassAnvil;
//			break;
//		case b2::protocol::commondata::Item::ACCESSORY_SURPASS_ANVIL:
//			ItemType = B2ItemType::AccessorySurpassAnvil;
//			break;
//		case b2::protocol::commondata::Item::SEAL_BOX:
//			ItemType = B2ItemType::SealBox;
//			break;
//		case b2::protocol::commondata::Item::GOLD:
//			ItemType = B2ItemType::Gold;
//			break;
//		case b2::protocol::commondata::Item::ADVANCED_RANK_PROMOTION_POINT:
//			ItemType = B2ItemType::AdvancedRankPromotionPoint;
//			break;
//		default:
//			break;
//		}
//		return ItemType;
//	}
//
//	int32 FB2MessageInfoConverter::PbCostume2B2CostumeType(b2::protocol::commondata::Costume::TYPE PbCostume)
//	{
//		int32 CostumeType = B2CostumeType::CostumeWeapon;
//		switch (PbCostume)
//		{
//		case b2::protocol::commondata::Costume::COSTUME_WEAPON:
//			CostumeType = B2CostumeType::CostumeWeapon;
//			break;
//		case b2::protocol::commondata::Costume::COSTUME_HAT:
//			CostumeType = B2CostumeType::CostumeHat;
//			break;
//		case b2::protocol::commondata::Costume::COSTUME_ARMOR:
//			CostumeType = B2CostumeType::CostumeArmor;
//			break;
//		default:
//			break;
//		}
//		return CostumeType;
//	}
//
//	int32 FB2MessageInfoConverter::PbInventory2B2InventoryType(b2::protocol::commondata::Inventory::TYPE PbInventory)
//	{
//		int32 InventoryType = B2InventoryType::Weapon;
//		switch (PbInventory)
//		{
//		case b2::protocol::commondata::Inventory::WEAPON:
//			InventoryType = B2InventoryType::Weapon;
//			break;
//		case b2::protocol::commondata::Inventory::ARMOR:
//			InventoryType = B2InventoryType::Protection;
//			break;
//		case b2::protocol::commondata::Inventory::ACCESSORY:
//			InventoryType = B2InventoryType::Accessory;
//			break;
//		case b2::protocol::commondata::Inventory::WING:
//			InventoryType = B2InventoryType::Wing;
//			break;
//		case b2::protocol::commondata::Inventory::COSTUME_WEAPON:
//			InventoryType = B2InventoryType::CostumeWeapon;
//			break;
//		case b2::protocol::commondata::Inventory::COSTUME_ARMOR:
//			InventoryType = B2InventoryType::CostumeArmor;
//			break;
//		case b2::protocol::commondata::Inventory::CONSUMABLE:
//			InventoryType = B2InventoryType::Consumables;
//			break;
//		default:
//			break;
//		}
//		return InventoryType;
//	}
//
//	int32 FB2MessageInfoConverter::PbEquipPosition2B2EquipPositionType(b2::protocol::commondata::EquipPosition PbEquipPosition)
//	{
//		int32 EquipPosition = B2EquipPosition::None;
//		switch (PbEquipPosition)
//		{
//		case b2::protocol::commondata::NONE:
//			EquipPosition = B2EquipPosition::None;
//			break;
//		case b2::protocol::commondata::WEAPON:
//			EquipPosition = B2EquipPosition::Weapon;
//			break;
//		case b2::protocol::commondata::HAT:
//			EquipPosition = B2EquipPosition::Helmet;
//			break;
//		case b2::protocol::commondata::ARMOR:
//			EquipPosition = B2EquipPosition::BodyArmor;
//			break;
//		case b2::protocol::commondata::GLOVE:
//			EquipPosition = B2EquipPosition::Glove;
//			break;
//		case b2::protocol::commondata::SHOES:
//			EquipPosition = B2EquipPosition::Shoes;
//			break;
//		case b2::protocol::commondata::BELT:
//			EquipPosition = B2EquipPosition::Belt;
//			break;
//		case b2::protocol::commondata::NECK:
//			EquipPosition = B2EquipPosition::Necklace;
//			break;
//		case b2::protocol::commondata::RING:
//			EquipPosition = B2EquipPosition::Ring;
//			break;
//		case b2::protocol::commondata::WING:
//			EquipPosition = B2EquipPosition::Wing;
//			break;
//		case b2::protocol::commondata::COSTUME_WEAPON:
//			EquipPosition = B2EquipPosition::CostumeWeapon;
//			break;
//		case b2::protocol::commondata::COSTUME_HAT:
//			EquipPosition = B2EquipPosition::CostumeHat;
//			break;
//		case b2::protocol::commondata::COSTUME_ARMOR:
//			EquipPosition = B2EquipPosition::CostumeArmor;
//			break;
//		default:
//			break;
//		}
//		return EquipPosition;
//	}
//	
//	int32 FB2MessageInfoConverter::PbRoomType2B2RoomType(b2::protocol::channel::RoomType PbRoomType)
//	{
//		int32 RoomType = B2RoomType::BRT_Town;
//		switch (PbRoomType)
//		{
//        case b2::protocol::channel::PVP:
//            RoomType = B2RoomType::BRT_Town;
//            break;
//
//        case b2::protocol::channel::RAID:
//            RoomType = B2RoomType::BRT_Raid;
//            break;
//
//        case b2::protocol::channel::ASSAULT:
//            RoomType = B2RoomType::BRT_Assault;
//            break;
//		}
//		return RoomType;
//	}
//
//	int32 FB2MessageInfoConverter::PbBattleFormationType2B2BattleFormation(b2::protocol::commondata::BattleFormation::TYPE PbFormation)
//	{
//		int32 FormationType = B2BattleFormationType::BTBFT_Normal;
//		switch (PbFormation)
//		{
//		case b2::protocol::commondata::BattleFormation::NORMAL:
//			FormationType = B2BattleFormationType::BTBFT_Normal;
//			break;
//		case b2::protocol::commondata::BattleFormation::OFFENSIVE:
//			FormationType = B2BattleFormationType::BTBFT_Offensive;
//			break;
//		case b2::protocol::commondata::BattleFormation::DEFENSIVE:
//			FormationType = B2BattleFormationType::BTBFT_Defensive;
//			break;
//		default:
//			break;
//		}
//		return FormationType;
//	}
//
//	int32 FB2MessageInfoConverter::PbMatchResultType2B2MatchResultType(b2::protocol::commondata::MatchResult::TYPE matchResult)
//	{
//		int32 b2MatchResult = B2MatchResultType::BMRT_DRAW;
//		switch (matchResult)
//		{
//		case b2::protocol::commondata::MatchResult::WIN:
//			b2MatchResult = B2MatchResultType::BMRT_WIN;
//			break;
//		case b2::protocol::commondata::MatchResult::LOSE:
//			b2MatchResult = B2MatchResultType::BMRT_LOSE;
//			break;
//		case b2::protocol::commondata::MatchResult::DRAW:
//			b2MatchResult = B2MatchResultType::BMRT_DRAW;
//			break;
//		default:
//			break;
//		}
//		return b2MatchResult;
//	}
//
//	int32 FB2MessageInfoConverter::PbMatchMakingType2B2MatchMakingType(b2::protocol::commondata::MatchMaking::TYPE PbMatchMakingType)
//	{
//		int32 b2MatchMakingType = B2MatchMakingType::BMMT_PVP_RANK;
//		switch (PbMatchMakingType)
//		{
//		case b2::protocol::commondata::MatchMaking::PVP_RANK:
//			b2MatchMakingType = B2MatchMakingType::BMMT_PVP_RANK;
//			break;
//		case b2::protocol::commondata::MatchMaking::PVP_FRIENDLY:
//			b2MatchMakingType = B2MatchMakingType::BMMT_PVP_FRIENDLY;
//			break;
//        case b2::protocol::commondata::MatchMaking::RAID_RANK:
//            b2MatchMakingType = B2MatchMakingType::BMMT_RAID_RANK;
//            break;
//        case b2::protocol::commondata::MatchMaking::ASSAULT_RANK:
//            b2MatchMakingType = B2MatchMakingType::BMMT_ASSAULT_RANK;
//            break;
//		default:
//			break;
//		}
//		return b2MatchMakingType;
//	}
//
//	int32 FB2MessageInfoConverter::PbMatchInfoType2B2MatchInfoType(b2::protocol::commondata::MatchInfo::TYPE matchInfoType)
//	{
//		int32 b2MatchInfoType = B2MatchInfoType::BMIT_PLAYER;
//		switch (matchInfoType)
//		{
//			case b2::protocol::commondata::MatchInfo::AI:
//				b2MatchInfoType = B2MatchInfoType::BMIT_AI;
//				break;
//			case b2::protocol::commondata::MatchInfo::PLAYER:
//				b2MatchInfoType = B2MatchInfoType::BMIT_PLAYER;
//				break;
//			case b2::protocol::commondata::MatchInfo::TEAM:
//				b2MatchInfoType = B2MatchInfoType::BMIT_TEAM;
//				break;
//			case b2::protocol::commondata::MatchInfo::RAID:
//				b2MatchInfoType = B2MatchInfoType::BMIT_RAID;
//				break;
//			case b2::protocol::commondata::MatchInfo::ASSAULT:
//				b2MatchInfoType = B2MatchInfoType::BMIT_ASSAULT;
//				break;
//			case b2::protocol::commondata::MatchInfo::HEROTOWER:
//				b2MatchInfoType = B2MatchInfoType::BMIT_HEROTOWER;
//				break;
//			case b2::protocol::commondata::MatchInfo::DONATION:
//				b2MatchInfoType = B2MatchInfoType::BMIT_DONATION;
//				break;
//			case b2::protocol::commondata::MatchInfo::DIMENSION:
//				b2MatchInfoType = B2MatchInfoType::BMIT_DIMENSION;
//				break;
//			default:
//				break;
//		}
//		return b2MatchInfoType;
//	}
//
//	int32 FB2MessageInfoConverter::PbMailCategory2B2MailCategory(b2::protocol::commondata::MailCategory mailCategory)
//	{
//		int32 b2MailCategory = B2MailCategory::PRESENT;
//		switch (mailCategory)
//		{
//		case b2::protocol::commondata::MailCategory::PRESENT:
//			b2MailCategory = B2MailCategory::PRESENT;
//			break;
//		case b2::protocol::commondata::MailCategory::BLADE_POINT:
//			b2MailCategory = B2MailCategory::BLADE_POINT;
//			break;
//		case b2::protocol::commondata::MailCategory::SOCIAL_POINT:
//			b2MailCategory = B2MailCategory::SOCIAL_POINT;
//			break;
//		case b2::protocol::commondata::MailCategory::LOTTERY:
//			b2MailCategory = B2MailCategory::LOTTERY;
//			break;
//		}
//		return b2MailCategory;
//	}
//
//	int32 FB2MessageInfoConverter::PbMailAttachType2B2MailAttachType(b2::protocol::commondata::MailAttachType mailAttachType)
//	{
//		int32 b2MailAttachType = B2MailAttachType::GOLD;
//		switch (mailAttachType)
//		{
//		case b2::protocol::commondata::MailAttachType::ATTACH_GOLD:
//			b2MailAttachType = B2MailAttachType::GOLD;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_GEM:
//			b2MailAttachType = B2MailAttachType::GEM;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_ITEM:
//			b2MailAttachType = B2MailAttachType::ITEM;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_BLADE_POINT:
//			b2MailAttachType = B2MailAttachType::BLADE_POINT;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_SOCIAL_POINT:
//			b2MailAttachType = B2MailAttachType::SOCIAL_POINT;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_GENERAL_LOTTERY:
//			b2MailAttachType = B2MailAttachType::GENERAL_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_SELECTIVE_LOTTERY:
//			b2MailAttachType = B2MailAttachType::SELECTIVE_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_SELECTIVE_ITEM_LOTTERY:
//			b2MailAttachType = B2MailAttachType::SELECTIVE_ITEM_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_HERO_PIECE:
//			b2MailAttachType = B2MailAttachType::HERO_PIECE;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_RANK_PROMOTION_POINT:
//			b2MailAttachType = B2MailAttachType::RANK_PROMOTION_POINT;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_DARK_CRYSTAL:
//			b2MailAttachType = B2MailAttachType::DARK_CRYSTAL;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_FIXED_GRADE_LOTTERY:
//			b2MailAttachType = B2MailAttachType::FIXED_GRADE_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_STAGE_BOOST_TICKET:
//			b2MailAttachType = B2MailAttachType::STAGE_BOOST_TICKET;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_HOT_TIME_GOLD_BUFF_TICKET:
//			b2MailAttachType = B2MailAttachType::HOT_TIME_GOLD_BUFF_TICKET;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_HOT_TIME_EXP_BUFF_TICKET:
//			b2MailAttachType = B2MailAttachType::HOT_TIME_EXP_BUFF_TICKET;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_SHOP_TEN_LOTTERY_TICKET:
//			b2MailAttachType = B2MailAttachType::SHOP_TEN_LOTTERY_TICKET;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_FAME_POINT:
//			b2MailAttachType = B2MailAttachType::FAME_POINT;
//			break;
//
//		case b2::protocol::commondata::MailAttachType::ATTACH_SELECTIVE_AETHER_LOTTERY:
//			b2MailAttachType = B2MailAttachType::SELECTIVE_AETHER_LOTTERY;
//			break;
//		case b2::protocol::commondata::MailAttachType::ATTACH_PREFIX_SELECTIVE_LOTTERY:
//			b2MailAttachType = B2MailAttachType::PREFIX_SELECTIVE_LOTTERY;
//			break;
//		case b2::protocol::commondata::MailAttachType::ATTACH_STAGE_AUTO_CLEAR_TICKET:
//			b2MailAttachType = B2MailAttachType::STAGE_AUTO_CLEAR_TICKET;
//			break;
//		case b2::protocol::commondata::MailAttachType::ATTACH_TOTEM_PREFIX_LOTTERY:
//			b2MailAttachType = B2MailAttachType::TOTEM_PREFIX_LOTTERY;
//			break;
//		case b2::protocol::commondata::MailAttachType::ATTACH_ADVANCED_RANK_PROMOTION_POINT:
//			b2MailAttachType = B2MailAttachType::ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//		return b2MailAttachType;
//	}
//
//	int32 FB2MessageInfoConverter::PbMailPresentAttachType2B2MailPresentAttachType(b2::protocol::commondata::MailPresentAttachType attachType)
//	{
//		int32 b2MailPresentAttachType = B2MailPresentAttachType::MONEY;
//		switch (attachType)
//		{
//		case b2::protocol::commondata::MailPresentAttachType::MONEY:
//			b2MailPresentAttachType = B2MailPresentAttachType::MONEY;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::GEM:
//			b2MailPresentAttachType = B2MailPresentAttachType::GEM;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::ITEM:
//			b2MailPresentAttachType = B2MailPresentAttachType::ITEM;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::HERO_PIECE:
//			b2MailPresentAttachType = B2MailPresentAttachType::HERO_PIECE;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::RANK_PROMOTION_POINT:
//			b2MailPresentAttachType = B2MailPresentAttachType::RANK_PROMOTION_POINT;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::DARK_CRYSTAL:
//			b2MailPresentAttachType = B2MailPresentAttachType::DARK_CRYSTAL;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::STAGE_BOOST_TICKET:
//			b2MailPresentAttachType = B2MailPresentAttachType::STAGE_BOOST_TICKET;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::HOT_TIME_GOLD_BUFF_TICKET:
//			b2MailPresentAttachType = B2MailPresentAttachType::HOT_TIME_GOLD_BUFF_TICKET;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::HOT_TIME_EXP_BUFF_TICKET:
//			b2MailPresentAttachType = B2MailPresentAttachType::HOT_TIME_EXP_BUFF_TICKET;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::SHOP_TEN_LOTTERY_TICKET:
//			b2MailPresentAttachType = B2MailPresentAttachType::SHOP_TEN_LOTTERY_TICKET;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::FAME_POINT:
//			b2MailPresentAttachType = B2MailPresentAttachType::FAME_POINT;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::STAGE_AUTO_CLEAR_TICKET:
//			b2MailPresentAttachType = B2MailPresentAttachType::STAGE_AUTO_CLEAR_TICKET;
//			break;
//		case b2::protocol::commondata::MailPresentAttachType::ADVANCED_RANK_PROMOTION_POINT:
//			b2MailPresentAttachType = B2MailPresentAttachType::ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//		return b2MailPresentAttachType;;
//	}
//
//	int32 FB2MessageInfoConverter::PbMailSenderType2B2MailSenderType(b2::protocol::commondata::MailSenderType senderType)
//	{
//		int32 b2MailSenderType = B2MailSenderType::GAIUS;
//		switch (senderType)
//		{
//		case b2::protocol::commondata::MailSenderType::SENDER_FRIEND:
//			b2MailSenderType = B2MailSenderType::FRIEND;
//			break;
//		case b2::protocol::commondata::MailSenderType::GAIUS:
//			b2MailSenderType = B2MailSenderType::GAIUS;
//			break;
//		case b2::protocol::commondata::MailSenderType::RAMIEL:
//			b2MailSenderType = B2MailSenderType::RAMIEL;
//			break;		
//		case b2::protocol::commondata::MailSenderType::ADMIN:
//			b2MailSenderType = B2MailSenderType::ADMIN;
//			break;
//		case b2::protocol::commondata::MailSenderType::GUILD_MEMBER:
//			b2MailSenderType = B2MailSenderType::GUILD_MEMBER;
//			break;
//		}
//		return b2MailSenderType;
//	}
//
//	int32 FB2MessageInfoConverter::PbMailSourceType2B2MailSourceType(b2::protocol::commondata::MailSourceType sourceType)
//	{
//		int32 b2MailPlaceType = B2MailSourceType::ATTENDANCE;
//		switch (sourceType)
//		{
//		case b2::protocol::commondata::MailSourceType::SOURCE_FRIEND:
//			b2MailPlaceType = B2MailSourceType::FRIEND;
//			break;
//		case b2::protocol::commondata::MailSourceType::MATCH_MAKING:
//			b2MailPlaceType = B2MailSourceType::MATCH_MAKING;
//			break;
//		case b2::protocol::commondata::MailSourceType::TEAM_BATTLE:
//			b2MailPlaceType = B2MailSourceType::TEAM_BATTLE;
//			break;
//		case b2::protocol::commondata::MailSourceType::ATTENDANCE:
//			b2MailPlaceType = B2MailSourceType::ATTENDANCE;
//			break;
//		case b2::protocol::commondata::MailSourceType::EVENT:
//			b2MailPlaceType = B2MailSourceType::EVENT;
//			break;
//		case b2::protocol::commondata::MailSourceType::MAINTENANCE:
//			b2MailPlaceType = B2MailSourceType::MAINTENANCE;
//			break;
//		case b2::protocol::commondata::MailSourceType::QUEST_REWARD:
//			b2MailPlaceType = B2MailSourceType::QUEST_REWARD;
//			break;
//		case b2::protocol::commondata::MailSourceType::SHOP:
//			b2MailPlaceType = B2MailSourceType::SHOP;
//			break;
//		case b2::protocol::commondata::MailSourceType::MISSION_REWARD:
//			b2MailPlaceType = B2MailSourceType::MISSION_REWARD;
//			break;
//		case b2::protocol::commondata::MailSourceType::GUILD_SUPPORT:
//			b2MailPlaceType = B2MailSourceType::GUILD_SUPPORT;
//			break;
//		}
//		return b2MailPlaceType;
//	}
//
//	int32 FB2MessageInfoConverter::PbDayOfWeek2B2DayOfWeek(b2::protocol::commondata::DayOfWeek dayOfWeek)
//	{
//		int32 b2DayOfWeek = B2DayOfWeek::MONDAY;
//		switch (dayOfWeek)
//		{
//		case b2::protocol::commondata::DayOfWeek::SUNDAY:
//			b2DayOfWeek = B2DayOfWeek::SUNDAY;
//			break;
//		case b2::protocol::commondata::DayOfWeek::MONDAY:
//			b2DayOfWeek = B2DayOfWeek::MONDAY;
//			break;
//		case b2::protocol::commondata::DayOfWeek::TUESDAY:
//			b2DayOfWeek = B2DayOfWeek::TUESDAY;
//			break;
//		case b2::protocol::commondata::DayOfWeek::WEDNESDAY:
//			b2DayOfWeek = B2DayOfWeek::WEDNESDAY;
//			break;
//		case b2::protocol::commondata::DayOfWeek::THURSDAY:
//			b2DayOfWeek = B2DayOfWeek::THURSDAY;
//			break;
//		case b2::protocol::commondata::DayOfWeek::FRIDAY:
//			b2DayOfWeek = B2DayOfWeek::FRIDAY;
//			break;
//		case b2::protocol::commondata::DayOfWeek::SATURDAY:
//			b2DayOfWeek = B2DayOfWeek::SATURDAY;
//			break;
//		}
//		return b2DayOfWeek;
//	}
//
//	int32 FB2MessageInfoConverter::PbCounterDungeonType2B2CounterDungeonType(b2::protocol::masterdata::mdCounterDungeon::TYPE type)
//	{
//		int32 b2DungeonType = B2CounterDungeon::TYPE::WEAPON;
//		switch (type)
//		{
//		case b2::protocol::masterdata::mdCounterDungeon::ARMOR:
//			b2DungeonType = B2CounterDungeon::TYPE::ARMOR;
//			break;
//		case b2::protocol::masterdata::mdCounterDungeon::WEAPON:
//			b2DungeonType = B2CounterDungeon::TYPE::WEAPON;
//			break;
//		case b2::protocol::masterdata::mdCounterDungeon::ACCESSORY:
//			b2DungeonType = B2CounterDungeon::TYPE::ACCESSORY;
//			break;
//		case b2::protocol::masterdata::mdCounterDungeon::GOLD:
//			b2DungeonType = B2CounterDungeon::TYPE::GOLD;
//			break;
//		}
//		return b2DungeonType;
//	}
//
//	int32 FB2MessageInfoConverter::PbCounterDungeonDifficulty2B2CounterDungeonDifficulty(b2::protocol::masterdata::mdCounterDungeon::DIFFICULTY difficulty)
//	{
//		int32 b2DungeonDifficulty = B2CounterDungeon::DIFFICULTY::EASY;
//		switch (difficulty)
//		{
//		case b2::protocol::masterdata::mdCounterDungeon::EASY:
//			b2DungeonDifficulty = B2CounterDungeon::DIFFICULTY::EASY;
//			break;
//		case b2::protocol::masterdata::mdCounterDungeon::NORMAL:
//			b2DungeonDifficulty = B2CounterDungeon::DIFFICULTY::NORMAL;
//			break;
//		case b2::protocol::masterdata::mdCounterDungeon::HARD:
//			b2DungeonDifficulty = B2CounterDungeon::DIFFICULTY::HARD;
//			break;
//		}
//		return b2DungeonDifficulty;
//	}
//
//	int32 FB2MessageInfoConverter::PbRewardTypeToB2RewardType(b2::protocol::commondata::RewardType type)
//	{
//		int32 b2RewardType = B2RewardType::GOLD;
//		switch (type)
//		{
//		case b2::protocol::commondata::RewardType::REWARD_GOLD:
//			b2RewardType = B2RewardType::GOLD;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_GEM:
//			b2RewardType = B2RewardType::GEM;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_BLADE_POINT:
//			b2RewardType = B2RewardType::BLADE_POINT;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_ITEM:
//			b2RewardType = B2RewardType::ITEM;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_GENERAL_LOTTERY:
//			b2RewardType = B2RewardType::GENERAL_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_SELECTIVE_LOTTERY:
//			b2RewardType = B2RewardType::SELECTIVE_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_BUFF_TICKET:
//			b2RewardType = B2RewardType::BUFF_TICKET;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_HERO_PIECE:
//			b2RewardType = B2RewardType::HERO_PIECE;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_SELECTIVE_ITEM_LOTTERY:
//			b2RewardType = B2RewardType::SELECTIVE_ITEM_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_RANK_PROMOTION_POINT:
//			b2RewardType = B2RewardType::RANK_PROMOTION_POINT;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_EXP:
//			b2RewardType = B2RewardType::EXP;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_DARK_CRYSTAL:
//			b2RewardType = B2RewardType::DARK_CRYSTAL;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_SOCIAL_POINT:
//			b2RewardType = B2RewardType::SOCIAL_POINT;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_FIXED_GRADE_LOTTERY:
//			b2RewardType = B2RewardType::FIXED_GRADE_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_STAGE_BOOST_TICKET:
//			b2RewardType = B2RewardType::STAGE_BOOST_TICKET;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_HOT_TIME_GOLD_BUFF_TICKET:
//			b2RewardType = B2RewardType::HOT_TIME_GOLD_BUFF_TICKET;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_HOT_TIME_EXP_BUFF_TICKET:
//			b2RewardType = B2RewardType::HOT_TIME_EXP_BUFF_TICKET;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_SHOP_TEN_LOTTERY_TICKET:
//			b2RewardType = B2RewardType::SHOP_TEN_LOTTERY_TICKET;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_FAME_POINT:
//			b2RewardType = B2RewardType::FAME_POINT;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_SELECTIVE_AETHER_LOTTERY:
//			b2RewardType = B2RewardType::SELECTIVE_AETHER_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_PREFIX_SELECTIVE_LOTTERY:
//			b2RewardType = B2RewardType::PREFIX_SELECTIVE_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_STAGE_AUTO_CLEAR_TICKET:
//			b2RewardType = B2RewardType::STAGE_AUTO_CLEAR_TICKET;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_COSTUME:
//			b2RewardType = B2RewardType::COSTUME;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_TOTEM:
//			b2RewardType = B2RewardType::TOTEM;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_TOTEM_PREFIX_LOTTERY:
//			b2RewardType = B2RewardType::TOTEM_PREFIX_LOTTERY;
//			break;
//		case b2::protocol::commondata::RewardType::REWARD_ADVANCED_RANK_PROMOTION_POINT:
//			b2RewardType = B2RewardType::ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//		return b2RewardType;
//	}
//
//	int32 FB2MessageInfoConverter::PbQuestSlotTypeToB2QuestSlotType(b2::protocol::commondata::QuestInfo::QuestSlotType type)
//	{
//		int32 b2QuestSlotType = B2QuestSlotType::MAIN1;
//		switch (type)
//		{
//			case b2::protocol::commondata::QuestInfo::QuestSlotType::QuestInfo_QuestSlotType_QUEST_MAIN1:
//				b2QuestSlotType = B2QuestSlotType::MAIN1;
//			break;
//
//			case b2::protocol::commondata::QuestInfo::QuestSlotType::QuestInfo_QuestSlotType_QUEST_MAIN2:
//				b2QuestSlotType = B2QuestSlotType::MAIN2;
//			break;
//
//			case b2::protocol::commondata::QuestInfo::QuestSlotType::QuestInfo_QuestSlotType_QUEST_SUB1:
//				b2QuestSlotType = B2QuestSlotType::SUB1;
//			break;
//
//			case b2::protocol::commondata::QuestInfo::QuestSlotType::QuestInfo_QuestSlotType_QUEST_SUB2:
//				b2QuestSlotType = B2QuestSlotType::SUB2;
//			break;
//		}
//		return b2QuestSlotType;
//	}
//
//	b2::protocol::commondata::QuestInfo::QuestSlotType FB2MessageInfoConverter::B2QuestSlotTypeToPbQuestSlotType(int32 B2QuestSlotType)
//	{
//		b2::protocol::commondata::QuestInfo::QuestSlotType b2QuestSlotType = b2::protocol::commondata::QuestInfo::QUEST_MAIN1;
//
//		switch (B2QuestSlotType)
//		{
//		case b2network::B2QuestSlotType::MAIN1:
//			b2QuestSlotType = b2::protocol::commondata::QuestInfo::QUEST_MAIN1;
//			break;
//
//		case b2network::B2QuestSlotType::MAIN2:
//			b2QuestSlotType = b2::protocol::commondata::QuestInfo::QUEST_MAIN2;
//			break;
//
//		case b2network::B2QuestSlotType::SUB1:
//			b2QuestSlotType = b2::protocol::commondata::QuestInfo::QUEST_SUB1;
//			break;
//
//		case b2network::B2QuestSlotType::SUB2:
//			b2QuestSlotType = b2::protocol::commondata::QuestInfo::QUEST_SUB2;
//			break;
//		}
//		return b2QuestSlotType;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildJoinOption(b2::protocol::commondata::Guild::JoinOption type)
//	{
//		int32 b2Type = B2GuildJoinOption::PUBLIC;
//		switch (type)
//		{
//		case b2::protocol::commondata::Guild::PUBLIC:
//			b2Type = B2GuildJoinOption::PUBLIC;
//			break;
//
//		case b2::protocol::commondata::Guild::PRIVATE:
//			b2Type = B2GuildJoinOption::PRIVATE;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildMemberRank(b2::protocol::commondata::GuildMember::Rank type)
//	{
//		int32 b2Type = B2GuildMemberRank::NORMAL;
//		switch (type)
//		{
//		case b2::protocol::commondata::GuildMember::MASTER:
//			b2Type = B2GuildMemberRank::MASTER;
//			break;
//
//		case b2::protocol::commondata::GuildMember::SUB_MASTER:
//			b2Type = B2GuildMemberRank::SUB_MASTER;
//			break;
//
//		case b2::protocol::commondata::GuildMember::NORMAL:
//			b2Type = B2GuildMemberRank::NORMAL;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbMissionTypeToB2MissionType(b2::protocol::commondata::MissionInfo::MissionType type)
//	{
//		int32 b2Type = B2MissionType::DAILY;
//
//		switch (type) {
//			case b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_DAILY:
//				b2Type = B2MissionType::DAILY;
//			break;
//
//			case b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_WEEKLY:
//				b2Type = B2MissionType::WEEKLY;
//			break;
//
//			case b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_MONTHLY:
//				b2Type = B2MissionType::MONTHLY;
//			break;
//
//			case b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_SERIAL:
//				b2Type = B2MissionType::SERIAL;
//			break;
//
//			case b2::protocol::commondata::MissionInfo::MissionType::MissionInfo_MissionType_FEATURED:
//				b2Type = B2MissionType::FEATURED;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbMissionStateTypeToMissionStateType(b2::protocol::commondata::MissionInfo::MissionStateType type)
//	{
//		int32 b2Type = B2MissionStateType::PROGRESS;
//
//		switch (type) {
//			case b2::protocol::commondata::MissionInfo::MissionStateType::MissionInfo_MissionStateType_PROGRESS:
//				b2Type = B2MissionStateType::PROGRESS;
//			break;
//
//			case b2::protocol::commondata::MissionInfo::MissionStateType::MissionInfo_MissionStateType_COMPLETED:
//				b2Type = B2MissionStateType::COMPLETED;
//			break;
//
//			case b2::protocol::commondata::MissionInfo::MissionStateType::MissionInfo_MissionStateType_REWARDED:
//				b2Type = B2MissionStateType::REWARDED;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbStageDifficultyToB2StageDifficulty(b2::protocol::commondata::StageDifficulty type)
//	{
//		int32 b2Type = B2StageDifficulty::NORMAL;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::StageDifficulty::NORMAL:
//			b2Type = B2StageDifficulty::NORMAL;
//			break;
//
//		case b2::protocol::commondata::StageDifficulty::HARD:
//			b2Type = B2StageDifficulty::HARD;
//			break;
//
//		case b2::protocol::commondata::StageDifficulty::VERY_HARD:
//			b2Type = B2StageDifficulty::VERY_HARD;
//			break;
//
//		case b2::protocol::commondata::StageDifficulty::HELL:
//			b2Type = B2StageDifficulty::HELL;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbStageClearPropertyToB2StageClearProperty(b2::protocol::commondata::StageClearProperty type)
//	{
//		int32 b2Type = B2StageClearProperty::CLEAR_DEFAULT;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::StageClearProperty::CLEAR_DEFAULT:
//			b2Type = B2StageClearProperty::CLEAR_DEFAULT;
//			break;
//
//		case b2::protocol::commondata::StageClearProperty::CLEAR_IN_TIME:
//			b2Type = B2StageClearProperty::CLEAR_IN_TIME;
//			break;
//
//		case b2::protocol::commondata::StageClearProperty::CLEAR_ALL_SURVIVAL:
//			b2Type = B2StageClearProperty::CLEAR_ALL_SURVIVAL;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2RaidReqType(b2::protocol::session::RaidReqType type)
//	{
//		switch (type)
//		{
//		case b2::protocol::session::RaidReqType::QUICK_JOIN:
//			return B2RaidRoomType::QUICK_JOIN;
//		case b2::protocol::session::RaidReqType::CREATE_PUBLIC_ROOM:
//			return B2RaidRoomType::CREATE_PUBLIC_ROOM;
//		case b2::protocol::session::RaidReqType::CREATE_PRIVATE_ROOM:
//			return B2RaidRoomType::CREATE_PRIVATE_ROOM;
//		case b2::protocol::session::RaidReqType::REJOIN:
//			return B2RaidRoomType::REJOIN;
//		}
//
//		return B2RaidRoomType::QUICK_JOIN;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2DimensionResult(b2::protocol::session::FinishDimensionRequest::RESULT result)
//	{
//		switch (result)
//		{
//		case b2::protocol::session::FinishDimensionRequest::CLEAR:
//			return B2DimensionResult::CLEAR;
//
//		case b2::protocol::session::FinishDimensionRequest::DEFEAT:
//			return B2DimensionResult::DEFEAT;
//
//		case b2::protocol::session::FinishDimensionRequest::GIVEUP:
//			return B2DimensionResult::GIVEUP;
//		}
//
//		return B2DimensionResult::CLEAR;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopMileageTypeToB2ShopMileageType(b2::protocol::commondata::Shop::MileageType type)
//	{
//		int32 b2Type = B2ShopMileageType::LOTTERY;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_LOTTERY:
//			b2Type = B2ShopMileageType::LOTTERY;
//			break;
//
//		case b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_BLADE_POINT:
//			b2Type = B2ShopMileageType::BLADE_POINT;
//			break;
//
//		case b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_GEM:
//			b2Type = B2ShopMileageType::GEM;
//			break;
//
//		case b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_GOLD:
//			b2Type = B2ShopMileageType::GOLD;
//			break;
//
//		case b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_DARK_CRYSTAL:
//			b2Type = B2ShopMileageType::DARK_CRYSTAL;
//			break;
//
//		case b2::protocol::commondata::Shop::MileageType::Shop_MileageType_MILEAGE_PACKAGE:
//			b2Type = B2ShopMileageType::PACKAGE;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbEquipCategoryToB2EquipCategory(b2::protocol::commondata::EquipCategory type)
//	{
//		int32 b2Type = B2EquipCategory::WEAPON;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::EquipCategory::EQUIP_NONE:
//			b2Type = B2EquipCategory::NONE;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::EQUIP_WEAPON:
//			b2Type = B2EquipCategory::WEAPON;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::EQUIP_ARMOR:
//			b2Type = B2EquipCategory::ARMOR;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::EQUIP_ACCESSORY:
//			b2Type = B2EquipCategory::ACCESSORY;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::ESSENCE_WEAPON:
//			b2Type = B2EquipCategory::WEAPON_ESSENCE;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::ESSENCE_ARMOR:
//			b2Type = B2EquipCategory::ARMOR_ESSENCE;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::ESSENCE_ACCESSORY:
//			b2Type = B2EquipCategory::ACCESSORY_ESSENCE;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::WEAPON_SURPASS_ANVIL:
//			b2Type = B2EquipCategory::WEAPON_SURPASS_ANVIL;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::ARMOR_SURPASS_ANVIL:
//			b2Type = B2EquipCategory::ARMOR_SURPASS_ANVIL;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::ACCESSORY_SURPASS_ANVIL:
//			b2Type = B2EquipCategory::ACCESSORY_SURPASS_ANVIL;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::EC_COSTUME_WEAPON:
//			b2Type = B2EquipCategory::COSTUME_WEAPON;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::EC_COSTUME_HAT:
//			b2Type = B2EquipCategory::COSTUME_HAT;
//			break;
//
//		case b2::protocol::commondata::EquipCategory::EC_COSTUME_ARMOR:
//			b2Type = B2EquipCategory::COSTUME_ARMOR;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopDrawPriceTypeToB2ShopDrawPriceType(b2::protocol::commondata::Shop::DrawPriceType type)
//	{
//		int32 b2Type = B2ShopDrawPriceType::GEM;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_DRAW_GEM:
//			b2Type = B2ShopDrawPriceType::GEM;
//			break;
//
//		case b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_DRAW_SOCIAL_POINT:
//			b2Type = B2ShopDrawPriceType::SOCIAL_POINT;
//			break;
//
//		case b2::protocol::commondata::Shop::DrawPriceType::Shop_DrawPriceType_SHOP_TEN_LOTTERY_TICKET:
//			b2Type = B2ShopDrawPriceType::SHOP_TEN_LOTTERY_TICKET;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopBuyPriceTypeToB2ShopBuyPriceType(b2::protocol::commondata::Shop::BuyPriceType type)
//	{
//		int32 b2Type = B2ShopBuyPriceType::GEM;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_GEM:
//			b2Type = B2ShopBuyPriceType::GEM;
//			break;
//
//		case b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_SOCIAL_POINT:
//			b2Type = B2ShopBuyPriceType::SOCIAL_POINT;
//			break;
//
//		case b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_DARK_CRYSTAL:
//			b2Type = B2ShopBuyPriceType::DARK_CRYSTAL;
//			break;
//		case b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_CASH:
//			b2Type = B2ShopBuyPriceType::CASH;
//			break;
//		case b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_GOLD:
//			b2Type = B2ShopBuyPriceType::GOLD;
//			break;
//		case b2::protocol::commondata::Shop::BuyPriceType::Shop_BuyPriceType_FAME_POINT:
//			b2Type = B2ShopBuyPriceType::FAME_POINT;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopProductTypeToB2ShopProductType(b2::protocol::commondata::Shop::ProductType type)
//	{
//		int32 b2Type = B2ShopProductType::BLADE_POINT;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_BLADE_POINT:
//			b2Type = B2ShopProductType::BLADE_POINT;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_GEM:
//			b2Type = B2ShopProductType::GEM;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_GOLD:
//			b2Type = B2ShopProductType::GOLD;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_ITEM:
//			b2Type = B2ShopProductType::ITEM;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_GENERAL_LOTTERY:
//			b2Type = B2ShopProductType::GENERAL_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_SELECTIVE_LOTTERY:
//			b2Type = B2ShopProductType::SELECTIVE_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_PACKAGE:
//			b2Type = B2ShopProductType::PACKAGE;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_DARK_CRYSTAL:
//			b2Type = B2ShopProductType::DARK_CRYSTAL;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_GOLD_BUFF_TICKET:
//			b2Type = B2ShopProductType::GOLD_BUFF_TICKET;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_EXP_BUFF_TICKET:
//			b2Type = B2ShopProductType::EXP_BUFF_TICKET;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_STAGE_BOOST_TICKET:
//			b2Type = B2ShopProductType::STAGE_BOOST_TICKET;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_SHOP_TEN_LOTTERY_TICKET:
//			b2Type = B2ShopProductType::SHOP_TEN_LOTTERY_TICKET;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_SELECTIVE_AETHER_LOTTERY:
//			b2Type = B2ShopProductType::SELECTIVE_AETHER_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_PREFIX_SELECTIVE_LOTTERY:
//			b2Type = B2ShopProductType::PREFIX_SELECTIVE_LOTTERY;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_STAGE_AUTO_CLEAR_TICKET:
//			b2Type = B2ShopProductType::STAGE_AUTO_CLEAR_TICKET;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_COSTUME:
//			b2Type = B2ShopProductType::COSTUME;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_TOTEM:
//			b2Type = B2ShopProductType::TOTEM;
//			break;
//
//		case b2::protocol::commondata::Shop::ProductType::Shop_ProductType_PRODUCT_TOTEM_PREFIX_LOTTERY:
//			b2Type = B2ShopProductType::TOTEM_PREFIX_LOTTERY;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopMarkTypeToB2ShopMarkType(b2::protocol::commondata::Shop::MarkType type)
//	{
//		int32 b2Type = B2ShopMarkType::MARK_NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::MarkType::Shop_MarkType_MARK_NONE:
//			b2Type = B2ShopMarkType::MARK_NONE;
//			break;
//
//		case b2::protocol::commondata::Shop::MarkType::Shop_MarkType_MARK_NEW:
//			b2Type = B2ShopMarkType::MARK_NEW;
//			break;
//
//		case b2::protocol::commondata::Shop::MarkType::Shop_MarkType_MARK_HOT:
//			b2Type = B2ShopMarkType::MARK_HOT;
//			break;
//
//		case b2::protocol::commondata::Shop::MarkType::Shop_MarkType_MARK_HIT:
//			b2Type = B2ShopMarkType::MARK_HIT;
//			break;
//
//		case b2::protocol::commondata::Shop::MarkType::Shop_MarkType_MARK_LIMITED:
//			b2Type = B2ShopMarkType::MARK_LIMITED;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopTypeToB2ShopType(b2::protocol::commondata::Shop::ShopType type)
//	{
//		int32 b2Type = B2ShopType::SHOP_PACKAGE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::ShopType::Shop_ShopType_SHOP_PACKAGE:
//			b2Type = B2ShopType::SHOP_PACKAGE;
//			break;
//
//		case b2::protocol::commondata::Shop::ShopType::Shop_ShopType_SHOP_DARK_CRYSTAL:
//			b2Type = B2ShopType::SHOP_DARK_CRYSTAL;
//			break;
//
//		case b2::protocol::commondata::Shop::ShopType::Shop_ShopType_SHOP_GOODS:
//			b2Type = B2ShopType::SHOP_GOODS;
//			break;
//
//		case b2::protocol::commondata::Shop::ShopType::Shop_ShopType_SHOP_FAME_POINT:
//			b2Type = B2ShopType::SHOP_FAME_POINT;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbShopRestrictTypeToB2ShopRestrictType(b2::protocol::commondata::Shop::RestrictType type)
//	{
//		int32 b2Type = B2ShopRestrictType::SHOP_NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Shop::RestrictType::Shop_RestrictType_SHOP_NONE:
//			b2Type = B2ShopRestrictType::SHOP_NONE;
//			break;
//
//		case b2::protocol::commondata::Shop::RestrictType::Shop_RestrictType_SHOP_DAY:
//			b2Type = B2ShopRestrictType::SHOP_DAY;
//			break;
//
//		case b2::protocol::commondata::Shop::RestrictType::Shop_RestrictType_SHOP_WEEK:
//			b2Type = B2ShopRestrictType::SHOP_WEEK;
//			break;
//
//		case b2::protocol::commondata::Shop::RestrictType::Shop_RestrictType_SHOP_MONTH:
//			b2Type = B2ShopRestrictType::SHOP_MONTH;
//			break;
//
//		case b2::protocol::commondata::Shop::RestrictType::Shop_RestrictType_SHOP_NUMBER:
//			b2Type = B2ShopRestrictType::SHOP_NUMBER;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbCollectionItemStateToB2CollectionItemState(b2::protocol::commondata::CollectionItemState type)
//	{
//		int32 b2Type = B2CollectionItemState::COLLECTED;
//
//		switch (type) 
//		{
//		case b2::protocol::commondata::CollectionItemState::COLLECTED:
//			b2Type = B2CollectionItemState::COLLECTED;
//			break;
//
//		case b2::protocol::commondata::CollectionItemState::ENHANCED:
//			b2Type = B2CollectionItemState::ENHANCED;
//			break;
//
//		case b2::protocol::commondata::CollectionItemState::REWARDED:
//			b2Type = B2CollectionItemState::REWARDED;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbCollectionCategoryToB2CollectionCategory(b2::protocol::commondata::CollectionCategory category)
//	{
//		int32 b2Type = B2CollectionCategory::SET;
//
//		switch (category)
//		{
//		case b2::protocol::commondata::CollectionCategory::COLLECTION_SET:
//			b2Type = B2CollectionCategory::SET;
//			break;
//
//		case b2::protocol::commondata::CollectionCategory::COLLECTION_WEAPON:
//			b2Type = B2CollectionCategory::WEAPON;
//			break;
//
//		case b2::protocol::commondata::CollectionCategory::COLLECTION_ARMOR:
//			b2Type = B2CollectionCategory::ARMOR;
//			break;
//
//		case b2::protocol::commondata::CollectionCategory::COLLECTION_ACCESSORY:
//			b2Type = B2CollectionCategory::ACCESSORY;
//			break;
//
//		case b2::protocol::commondata::CollectionCategory::COLLECTION_WING:
//			b2Type = B2CollectionCategory::WING;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbDailyAttendanceTypeToB2DailyAttendanceType(b2::protocol::commondata::DailyAttendanceType category)
//	{
//		int32 b2Type = B2DailyAttendanceType::GENERAL;
//
//		switch (category)
//		{
//		case b2::protocol::commondata::DailyAttendanceType::ATTENDANCE_GENERAL:
//			b2Type = B2DailyAttendanceType::GENERAL;
//			break;
//
//		case b2::protocol::commondata::DailyAttendanceType::ATTENDANCE_RETURNED:
//			b2Type = B2DailyAttendanceType::RETURNED;
//			break;
//
//		case b2::protocol::commondata::DailyAttendanceType::ATTENDANCE_BEGINNER:
//			b2Type = B2DailyAttendanceType::BEGINNER;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildBattleResultType(b2::protocol::commondata::GuildBattleResult::TYPE type)
//	{
//		int32 b2Type = B2GuildBattleResultType::BGBRT_LOSE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::GuildBattleResult::LOSE:
//			b2Type = B2GuildBattleResultType::BGBRT_LOSE;
//			break;
//
//		case b2::protocol::commondata::GuildBattleResult::DRAW:
//			b2Type = B2GuildBattleResultType::BGBRT_DRAW;
//			break;
//
//		case b2::protocol::commondata::GuildBattleResult::WIN:
//			b2Type = B2GuildBattleResultType::BGBRT_WIN;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildBattleStateType(b2::protocol::commondata::GuildBattleState::TYPE type)
//	{
//		int32 b2Type = B2GuildBattleStateType::NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::GuildBattleState::NONE:
//			b2Type = B2GuildBattleStateType::NONE;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::INIT:
//			b2Type = B2GuildBattleStateType::INIT;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::INITDONE:
//			b2Type = B2GuildBattleStateType::INITDONE;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::READY:
//			b2Type = B2GuildBattleStateType::READY;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::MATCHING:
//			b2Type = B2GuildBattleStateType::MATCHING;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::MATCHINGDONE:
//			b2Type = B2GuildBattleStateType::MATCHINGDONE;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::BATTLE:
//			b2Type = B2GuildBattleStateType::BATTLE;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::SETTLEMENT:
//			b2Type = B2GuildBattleStateType::SETTLEMENT;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::SETTLEMENTDONE:
//			b2Type = B2GuildBattleStateType::SETTLEMENTDONE;
//			break;
//
//		case b2::protocol::commondata::GuildBattleState::RESULT:
//			b2Type = B2GuildBattleStateType::RESULT;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildInviteState(b2::protocol::commondata::Guild::InviteState type)
//	{
//		int32 b2Type = B2GuildInviteState::NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Guild::NONE:
//			b2Type = B2GuildInviteState::NONE;
//			break;
//
//		case b2::protocol::commondata::Guild::JOINED_GUILD:
//			b2Type = B2GuildInviteState::JOINED_GUILD;
//			break;
//
//		case b2::protocol::commondata::Guild::INVITED:
//			b2Type = B2GuildInviteState::INVITED;
//			break;
//
//		case b2::protocol::commondata::Guild::NON_INVITING:
//			b2Type = B2GuildInviteState::NON_INVITING;
//			break;
//
//		case b2::protocol::commondata::Guild::INVITABLE:
//			b2Type = B2GuildInviteState::INVITABLE;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2AttributeChange(b2::protocol::commondata::AttributeChange type)
//	{
//		int32 b2Type = B2AttributeChange::INC_ATTACK;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::AttributeChange::INC_ATTACK:
//			b2Type = B2AttributeChange::INC_ATTACK;
//			break;
//
//		case b2::protocol::commondata::AttributeChange::INC_DEFENCE:
//			b2Type = B2AttributeChange::INC_DEFENCE;
//			break;
//
//		case b2::protocol::commondata::AttributeChange::INC_HEALTH:
//			b2Type = B2AttributeChange::INC_HEALTH;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2LotteryGrade(b2::protocol::commondata::LotteryGrade type)
//	{
//		int32 b2Type = B2LotteryGrade::NORMAL;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::LotteryGrade::LOTTERY_NORMAL:
//			b2Type = B2LotteryGrade::NORMAL;
//			break;
//
//		case b2::protocol::commondata::LotteryGrade::LOTTERY_SPECIAL:
//			b2Type = B2LotteryGrade::SPECIAL;
//			break;
//
//		case b2::protocol::commondata::LotteryGrade::LOTTERY_PREMIUM:
//			b2Type = B2LotteryGrade::PREMIUM;
//			break;
//
//		case b2::protocol::commondata::LotteryGrade::LOTTERY_SIGNATURE:
//			b2Type = B2LotteryGrade::SIGNATURE;
//			break;
//
//		case b2::protocol::commondata::LotteryGrade::LOTTERY_PLATINUM:
//			b2Type = B2LotteryGrade::PLATINUM;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GroupPrefix(b2::protocol::commondata::GroupPrefix type)
//	{
//		int32 b2Type = B2GroupPrefix::NON_EQUIP;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::GroupPrefix::SACRED:
//			b2Type = B2GroupPrefix::SACRED;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::MYSTIC:
//			b2Type = B2GroupPrefix::MYSTIC;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::STEEL:
//			b2Type = B2GroupPrefix::STEEL;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::SILENT:
//			b2Type = B2GroupPrefix::SILENT;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::GROWTH:
//			b2Type = B2GroupPrefix::GROWTH;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::DUEL:
//			b2Type = B2GroupPrefix::DUEL;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::HEAVEN:
//			b2Type = B2GroupPrefix::HEAVEN;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::ETERNAL:
//			b2Type = B2GroupPrefix::ETERNAL;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::COLD:
//			b2Type = B2GroupPrefix::COLD;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::CORRUPT:
//			b2Type = B2GroupPrefix::CORRUPT;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::SUN:
//			b2Type = B2GroupPrefix::SUN;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::BEACH:
//			b2Type = B2GroupPrefix::BEACH;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::STUDY:
//			b2Type = B2GroupPrefix::STUDY;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::CHRISTMAS:
//			b2Type = B2GroupPrefix::CHRISTMAS;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::NEW_YEAR:
//			b2Type = B2GroupPrefix::NEW_YEAR;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::HALLOWEEN:
//			b2Type = B2GroupPrefix::HALLOWEEN;
//			break;
//
//		case b2::protocol::commondata::GroupPrefix::NON_EQUIP:
//			b2Type = B2GroupPrefix::NON_EQUIP;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2DuelMode(b2::protocol::commondata::DuelMode type)
//	{
//		int32 b2Type = B2DuelMode::DUEL_MODE_NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::DuelMode::DUEL_MODE_NONE:
//			b2Type = B2DuelMode::DUEL_MODE_NONE;
//			break;
//
//		case b2::protocol::commondata::DuelMode::DUEL_MODE_PVP:
//			b2Type = B2DuelMode::DUEL_MODE_PVP;
//			break;
//
//		case b2::protocol::commondata::DuelMode::DUEL_MODE_TEAM:
//			b2Type = B2DuelMode::DUEL_MODE_TEAM;
//			break;
//
//		case b2::protocol::commondata::DuelMode::DUEL_MODE_ASSAULT:
//			b2Type = B2DuelMode::DUEL_MODE_ASSAULT;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2RewardBoxGrade(b2::protocol::commondata::RewardBoxGrade type)
//	{
//		int32 b2Type = B2RewardBoxGrade::REWARD_BOX_NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_NONE:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_NONE;
//			break;
//
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_BRONZE:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_BRONZE;
//			break;
//
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_SILVER:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_SILVER;
//			break;
//
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_GOLD:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_GOLD;
//			break;
//
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_PLATINUM:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_PLATINUM;
//			break;
//
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_DIAMOND:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_DIAMOND;
//			break;
//
//		case b2::protocol::commondata::RewardBoxGrade::REWARD_BOX_MASTER:
//			b2Type = B2RewardBoxGrade::REWARD_BOX_MASTER;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2ChatType(b2::protocol::session::ChatType type)
//	{
//		int b2Type = B2ChatType::CHANNEL;
//
//		switch (type)
//		{
//		case b2::protocol::session::ChatType::NOTICE:
//			b2Type = B2ChatType::NOTICE;
//			break;
//
//		case b2::protocol::session::ChatType::CHANNEL:
//			b2Type = B2ChatType::CHANNEL;
//			break;
//
//		case b2::protocol::session::ChatType::GUILD_NOTICE:
//			b2Type = B2ChatType::GUILD_NOTICE;
//			break;
//
//		case b2::protocol::session::ChatType::GUILD:
//			b2Type = B2ChatType::GUILD;
//			break;
//
//		case b2::protocol::session::ChatType::ONE_TO_ONE:
//			b2Type = B2ChatType::ONE_TO_ONE;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2ChatNotifyType(b2::protocol::session::ChatNoticeType type)
//	{
//		int b2Type = B2ChatNotifyType::GUILD_JOIN;
//
//		switch (type)
//		{
//		case b2::protocol::session::ChatNoticeType::GUILD_JOIN:
//			b2Type = B2ChatNotifyType::GUILD_JOIN;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_LEAVE:
//			b2Type = B2ChatNotifyType::GUILD_LEAVE;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_KICK:
//			b2Type = B2ChatNotifyType::GUILD_KICK;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_START_BATTLE:
//			b2Type = B2ChatNotifyType::GUILD_START_BATTLE;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_END_BATTLE:
//			b2Type = B2ChatNotifyType::GUILD_END_BATTLE;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_CHANGE_NOTICE:
//			b2Type = B2ChatNotifyType::GUILD_CHANGE_NOTICE;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::ITEM_GET:
//			b2Type = B2ChatNotifyType::GET_ITEM;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::RAID_START:
//			b2Type = B2ChatNotifyType::RAID_START;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::RAID_END:
//			b2Type = B2ChatNotifyType::RAID_END;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::ASSAULT_START:
//			b2Type = B2ChatNotifyType::ASSAULT_START;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::ASSAULT_END:
//			b2Type = B2ChatNotifyType::ASSAULT_END;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_SUPPORT:
//			b2Type = B2ChatNotifyType::GUILD_SUPPORT;
//			break;
//
//		case b2::protocol::session::ChatNoticeType::GUILD_SUPPORT_COMPLETE:
//			b2Type = B2ChatNotifyType::GUILD_SUPPORT_COMPLETE;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2AetherType(b2::protocol::commondata::Aether::TYPE type)
//	{
//		int b2Type = B2AetherType::ALL;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_THUNDERSTORM:
//			b2Type = B2AetherType::THUNDERSTORM;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_DESTROY:
//			b2Type = B2AetherType::DESTROY;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_INSIGHT:
//			b2Type = B2AetherType::INSIGHT;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_RAGE:
//			b2Type = B2AetherType::RAGE;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_DESPAIR:
//			b2Type = B2AetherType::DESPAIR;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_RESTRAINT:
//			b2Type = B2AetherType::RESTRAINT;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_RESIST:
//			b2Type = B2AetherType::RESIST;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_REVENGE:
//			b2Type = B2AetherType::REVENGE;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_PROTECT:
//			b2Type = B2AetherType::PROTECT;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_PASSION:
//			b2Type = B2AetherType::PASSION;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_OBLIVION:
//			b2Type = B2AetherType::OBLIVION;
//			break;
//		case b2::protocol::commondata::Aether::TYPE::Aether_TYPE_FREEZING:
//			b2Type = B2AetherType::FREEZING;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2AetherTier(b2::protocol::commondata::Aether::TIER type)
//	{
//		int b2Type = B2AetherTierType::SEAL;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Aether::TIER::Aether_TIER_SEAL:
//			b2Type = B2AetherTierType::SEAL;
//			break;
//		case b2::protocol::commondata::Aether::TIER::Aether_TIER_ANTI_DEVIL:
//			b2Type = B2AetherTierType::ANTI_DEVIL;
//			break;
//		case b2::protocol::commondata::Aether::TIER::Aether_TIER_EXTERMINATE_DEVIL:
//			b2Type = B2AetherTierType::EXTERMINATE_DEVIL;
//			break;
//		case b2::protocol::commondata::Aether::TIER::Aether_TIER_ETERNAL_LIFE:
//			b2Type = B2AetherTierType::ETERNAL_LIFE;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2AetherSetOption(b2::protocol::commondata::Aether::SetOptionType type)
//	{
//		int b2Type = B2AetherSetOptionType::THUNDER_DAMAGE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_THUNDER_DAMAGE:
//			b2Type = B2AetherSetOptionType::THUNDER_DAMAGE;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_IMMOBILIZE:
//			b2Type = B2AetherSetOptionType::IMMOBILIZE;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SHIELD:
//			b2Type = B2AetherSetOptionType::SHIELD;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_FIRE_RANGE:
//			b2Type = B2AetherSetOptionType::FIRE_RANGE;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_METEOR_DAMAGE:
//			b2Type = B2AetherSetOptionType::METEOR_DAMAGE;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SKILL_COOLTIME:
//			b2Type = B2AetherSetOptionType::SKILL_COOLTIME;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SKILL_GAGE:
//			b2Type = B2AetherSetOptionType::SKILL_GAGE;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_STUN:
//			b2Type = B2AetherSetOptionType::STUN;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_SUPER_ARMOR:
//			b2Type = B2AetherSetOptionType::SUPER_ARMOR;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_DAMAGE_REFLECT:
//			b2Type = B2AetherSetOptionType::DAMAGE_REFLECT;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_PROHIBIT_BEHAVIOR:
//			b2Type = B2AetherSetOptionType::PROHIBIT_BEHAVIOR;
//			break;
//		case b2::protocol::commondata::Aether::SetOptionType::Aether_SetOptionType_FREEZE_RANGE:
//			b2Type = B2AetherSetOptionType::FREEZE_RANGE;
//			break;
//		}
//		return b2Type;
//	}
//
//	b2::protocol::session::RedDotHint FB2MessageInfoConverter::B2RedDotHintType2PbRedDotHint(int32 b2Value)
//	{
//		switch (b2Value)
//		{
//		default:
//		case B2RedDotHintType::RED_DOT_ALL:			return b2::protocol::session::RedDotHint::RED_DOT_ALL;			break;
//		case B2RedDotHintType::RED_DOT_EQUIP:		return b2::protocol::session::RedDotHint::RED_DOT_EQUIP;		break;
//		case B2RedDotHintType::RED_DOT_COLLECTION:	return b2::protocol::session::RedDotHint::RED_DOT_COLLECTION;	break;
//		case B2RedDotHintType::RED_DOT_MISSION:		return b2::protocol::session::RedDotHint::RED_DOT_MISSION;		break;
//		case B2RedDotHintType::RED_DOT_FRIEND:		return b2::protocol::session::RedDotHint::RED_DOT_FRIEND;		break;
//		case B2RedDotHintType::RED_DOT_ADVENTURE:	return b2::protocol::session::RedDotHint::RED_DOT_ADVENTURE;	break;
//		case B2RedDotHintType::RED_DOT_DUNGEON:		return b2::protocol::session::RedDotHint::RED_DOT_DUNGEON;		break;
//		case B2RedDotHintType::RED_DOT_PVP:			return b2::protocol::session::RedDotHint::RED_DOT_PVP;			break;
//		case B2RedDotHintType::RED_DOT_GUILD:		return b2::protocol::session::RedDotHint::RED_DOT_GUILD;		break;
//		case B2RedDotHintType::RED_DOT_SHOP:		return b2::protocol::session::RedDotHint::RED_DOT_SHOP;			break;
//		case B2RedDotHintType::RED_DOT_MAIL:		return b2::protocol::session::RedDotHint::RED_DOT_MAIL;			break;
//		case B2RedDotHintType::RED_DOT_OPTION:		return b2::protocol::session::RedDotHint::RED_DOT_OPTION;		break;
//		case B2RedDotHintType::RED_DOT_EVENT:		return b2::protocol::session::RedDotHint::RED_DOT_EVENT;		break;
//		case B2RedDotHintType::RED_DOT_SHOP_PACKAGE:return b2::protocol::session::RedDotHint::RED_DOT_SHOP_PACKAGE;	break;
//		case B2RedDotHintType::RED_DOT_MAGIC_SHOP:	return b2::protocol::session::RedDotHint::RED_DOT_MAGIC_SHOP;	break;
//		case B2RedDotHintType::RED_DOT_HALL_OF_FAME:return b2::protocol::session::RedDotHint::RED_DOT_HALL_OF_FAME;	break;
//		case B2RedDotHintType::RED_DOT_FAIRY:		return b2::protocol::session::RedDotHint::RED_DOT_FAIRY;		break;
//		case B2RedDotHintType::RED_DOT_DONATION:	return b2::protocol::session::RedDotHint::RED_DOT_DONATION;		break;
//		case B2RedDotHintType::RED_DOT_ETC:			return b2::protocol::session::RedDotHint::RED_DOT_ETC;			break;
//		}
//	}
//
//	b2::protocol::commondata::EventCategory FB2MessageInfoConverter::B2EventCategory2PbEventCategory(int32 b2Value)
//	{
//		switch (b2Value)
//		{
//		default:
//		case B2EventCategory::POINT_REWARD:			return b2::protocol::commondata::EventCategory::POINT_REWARD;		break;
//		case B2EventCategory::HOT_TIME_BUFF:		return b2::protocol::commondata::EventCategory::HOT_TIME_BUFF;		break;
//		case B2EventCategory::LEVEL_UP:				return b2::protocol::commondata::EventCategory::LEVEL_UP;			break;
//		case B2EventCategory::SEASON_MISSION:		return b2::protocol::commondata::EventCategory::SEASON_MISSION;		break;
//		case B2EventCategory::STAGE_CLEAR:			return b2::protocol::commondata::EventCategory::STAGE_CLEAR;		break;
//		}
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildMercenaryDonationType(b2::protocol::masterdata::mdGuildMercenaryDonation::DonationType type)
//	{
//		int b2Type = B2GuildMercenaryDonationType::GOLD;
//
//		switch (type)
//		{
//		case b2::protocol::masterdata::mdGuildMercenaryDonation::GOLD:
//			b2Type = B2GuildMercenaryDonationType::GOLD;
//			break;
//
//		case b2::protocol::masterdata::mdGuildMercenaryDonation::GEM:
//			b2Type = B2GuildMercenaryDonationType::GEM;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2MarketPurchaseState(b2::protocol::commondata::MarketPurchaseInfo::STATE state)
//	{
//		int b2Type = B2MarketPurchaseState::REGISTERED;
//
//		switch (state)
//		{
//		case b2::protocol::commondata::MarketPurchaseInfo::REGISTERED:
//			b2Type = B2MarketPurchaseState::REGISTERED;
//			break;
//
//		case b2::protocol::commondata::MarketPurchaseInfo::VERIFIED:
//			b2Type = B2MarketPurchaseState::VERIFIED;
//			break;
//
//		case b2::protocol::commondata::MarketPurchaseInfo::COMPLETED:
//			b2Type = B2MarketPurchaseState::COMPLETED;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2ContentsMode(b2::protocol::commondata::ContentsMode type)
//	{
//		int b2Type = B2ContentsMode::NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::ContentsMode::CM_STAGE:
//			b2Type = B2ContentsMode::STAGE;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_PVP_DUEL:
//			b2Type = B2ContentsMode::PVP_DUEL;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_PVP_TEAM:
//			b2Type = B2ContentsMode::PVP_TEAM;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_ASSAULT:
//			b2Type = B2ContentsMode::ASSAULT;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_GUILD:
//			b2Type = B2ContentsMode::GUILD;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_HERO_TOWER:
//			b2Type = B2ContentsMode::HERO_TOWER;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_RAID:
//			b2Type = B2ContentsMode::RAID;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_PROMOTION:
//			b2Type = B2ContentsMode::PROMOTION;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_COUNTER:
//			b2Type = B2ContentsMode::COUNTER;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_FRIEND:
//			b2Type = B2ContentsMode::FRIEND;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_INVENTORY:
//			b2Type = B2ContentsMode::INVENTORY;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_MISSION:
//			b2Type = B2ContentsMode::MISSION;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_SHOP:
//			b2Type = B2ContentsMode::SHOP;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_DONATION:
//			b2Type = B2ContentsMode::DONATION;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_DIMENSION:
//			b2Type = B2ContentsMode::DIMENSION;
//			break;
//
//		case b2::protocol::commondata::ContentsMode::CM_COSTUME:
//			b2Type = B2ContentsMode::COSTUME;
//			break;
//		}
//
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2EventAttanceBonusType(b2::protocol::masterdata::mdEventAttendance::BonusType type)
//	{
//		int b2Type = B2EventAttanceBonusType::NONE;
//
//		switch (type)
//		{
//		case b2::protocol::masterdata::mdEventAttendance::NONE:
//			b2Type = B2EventAttanceBonusType::NONE;
//			break;
//
//		case b2::protocol::masterdata::mdEventAttendance::PERIODIC_PACKAGE:
//			b2Type = B2EventAttanceBonusType::FLAT_RATE_ALL;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2GuildBattleEntryType(b2::protocol::commondata::GuildBattleEntry::TYPE type)
//	{
//		int b2Type = B2GuildBattleEntryType::CHARACTER;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::GuildBattleEntry::CHARACTER:
//			b2Type = B2GuildBattleEntryType::CHARACTER;
//			break;
//
//		case b2::protocol::commondata::GuildBattleEntry::MERCENARY:
//			b2Type = B2GuildBattleEntryType::MERCENARY;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::PbItemGetRouteType2B2ItemGetRouteType(b2::protocol::commondata::ItemGetRouteType type)
//	{
//		int b2Type = B2ItemGetRouteType::ETC;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_BUY_SHOP:
//			b2Type = B2ItemGetRouteType::BUY_SHOP;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_LOTTERY:
//			b2Type = B2ItemGetRouteType::LOTTERY;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_UPGRADE_ITEM:
//			b2Type = B2ItemGetRouteType::UPGRADE_ITEM;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_COMPOSE_ITEM:
//			b2Type = B2ItemGetRouteType::COMPOSE_ITEM;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_AETHER:
//			b2Type = B2ItemGetRouteType::DISMANTLE_AETHER;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_ITEM:
//			b2Type = B2ItemGetRouteType::DISMANTLE_ITEM;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_MAKE_DONATION:
//			b2Type = B2ItemGetRouteType::MAKE_DONATION;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_TOTEM:
//			b2Type = B2ItemGetRouteType::DISMANTLE_TOTEM;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_DISMANTLE_COSTUME:
//			b2Type = B2ItemGetRouteType::DISMANTLE_COSTUME;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_FACTORY_MAKE:
//			b2Type = B2ItemGetRouteType::FACTORY_MAKE;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_FACTORY_UPGRADE:
//			b2Type = B2ItemGetRouteType::FACTORY_UPGRADE;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_UNSEAL_BOX:
//			b2Type = B2ItemGetRouteType::UNSEAL_BOX;
//			break;
//		case b2::protocol::commondata::ItemGetRouteType::ROUTE_ETC:
//			b2Type = B2ItemGetRouteType::ETC;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2FairyType(b2::protocol::commondata::FairyType type)
//	{
//		int b2Type = B2FairyType::NIECE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::FairyType::NIECE:
//			b2Type = B2FairyType::NIECE;
//			break;
//		case b2::protocol::commondata::FairyType::PANG:
//			b2Type = B2FairyType::PANG;
//			break;
//		case b2::protocol::commondata::FairyType::RIO:
//			b2Type = B2FairyType::RIO;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2TotemEquipSlot(b2::protocol::commondata::TotemEquipSlot type)
//	{
//		int b2Type = B2TotemEquipSlot::NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::TotemEquipSlot::TOTEM_NONE:
//			b2Type = B2TotemEquipSlot::NONE;
//			break;
//		case b2::protocol::commondata::TotemEquipSlot::TOTEM_TOP:
//			b2Type = B2TotemEquipSlot::TOP;
//			break;
//		case b2::protocol::commondata::TotemEquipSlot::TOTEM_MIDDLE:
//			b2Type = B2TotemEquipSlot::MIDDLE;
//			break;
//		case b2::protocol::commondata::TotemEquipSlot::TOTEM_BOTTOM:
//			b2Type = B2TotemEquipSlot::BOTTOM;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2TotemGrade(b2::protocol::commondata::TotemGrade type)
//	{
//		int b2Type = B2TotemGrade::NORMAL;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::TotemGrade::TG_NORMAL:
//			b2Type = B2TotemGrade::NORMAL;
//			break;
//		case b2::protocol::commondata::TotemGrade::TG_MAGIC:
//			b2Type = B2TotemGrade::MAGIC;
//			break;
//		case b2::protocol::commondata::TotemGrade::TG_RARE:
//			b2Type = B2TotemGrade::RARE;
//			break;
//		case b2::protocol::commondata::TotemGrade::TG_HERO:
//			b2Type = B2TotemGrade::HERO;
//			break;
//		case b2::protocol::commondata::TotemGrade::TG_LEGEND:
//			b2Type = B2TotemGrade::LEGEND;
//			break;
//		case b2::protocol::commondata::TotemGrade::TG_RELIC:
//			b2Type = B2TotemGrade::RELIC;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2TotemBasePrefix(b2::protocol::commondata::TotemBasePrefix type)
//	{
//		int b2Type = B2TotemBasePrefix::VALOROUS;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::TotemBasePrefix::TBP_VALOROUS:
//			b2Type = B2TotemBasePrefix::VALOROUS;
//			break;
//		case b2::protocol::commondata::TotemBasePrefix::TBP_PATRON:
//			b2Type = B2TotemBasePrefix::PATRON;
//			break;
//		case b2::protocol::commondata::TotemBasePrefix::TBP_VIGOROUS:
//			b2Type = B2TotemBasePrefix::VIGOROUS;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2TotemRefinePrefix(b2::protocol::commondata::TotemRefinePrefix type)
//	{
//		int b2Type = B2TotemRefinePrefix::ECLIPSE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::TotemRefinePrefix::TRP_ECLIPSE:
//			b2Type = B2TotemRefinePrefix::ECLIPSE;
//			break;
//		case b2::protocol::commondata::TotemRefinePrefix::TRP_SHINING:
//			b2Type = B2TotemRefinePrefix::SHINING;
//			break;
//		case b2::protocol::commondata::TotemRefinePrefix::TRP_STRONG:
//			b2Type = B2TotemRefinePrefix::STRONG;
//			break;
//		case b2::protocol::commondata::TotemRefinePrefix::TRP_SACRED:
//			b2Type = B2TotemRefinePrefix::SACRED;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2Refinestatus(b2::protocol::commondata::TotemRefineState::Refinestatus type)
//	{
//		int b2Type = B2Refinestatus::NONE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::TotemRefineState::Refinestatus::TotemRefineState_Refinestatus_NONE:
//			b2Type = B2Refinestatus::NONE;
//			break;
//		case b2::protocol::commondata::TotemRefineState::Refinestatus::TotemRefineState_Refinestatus_SUCCESS:
//			b2Type = B2Refinestatus::SUCCESS;
//			break;
//		case b2::protocol::commondata::TotemRefineState::Refinestatus::TotemRefineState_Refinestatus_FAIL:
//			b2Type = B2Refinestatus::FAIL;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2TotemRefineCostType(b2::protocol::commondata::TotemRefineCostType type)
//	{
//		int b2Type = B2TotemRefineCostType::GOLD;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::TotemRefineCostType::TRC_GOLD:
//			b2Type = B2TotemRefineCostType::GOLD;
//			break;
//		case b2::protocol::commondata::TotemRefineCostType::TRC_DIMENSIONAL_PIECE:
//			b2Type = B2TotemRefineCostType::DIMENSIONAL_PIECE;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2FactoryResultType(b2::protocol::commondata::FactoryResultType type)
//	{
//		int b2Type = B2FactoryResultType::FAIL;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::FactoryResultType::FR_FAIL:
//			b2Type = B2FactoryResultType::FAIL;
//			break;
//		case b2::protocol::commondata::FactoryResultType::FR_SUCCESS:
//			b2Type = B2FactoryResultType::SUCCESS;
//			break;
//		case b2::protocol::commondata::FactoryResultType::FR_HUGE_SUCCESS:
//			b2Type = B2FactoryResultType::HUGE_SUCCESS;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2FactoryLimitType(b2::protocol::commondata::FactoryLimitType type)
//	{
//		int b2Type = B2FactoryLimitType::TRY;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::FactoryLimitType::FL_TRY:
//			b2Type = B2FactoryLimitType::TRY;
//			break;
//		case b2::protocol::commondata::FactoryLimitType::FL_SUCCESS:
//			b2Type = B2FactoryLimitType::SUCCESS;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2FactoryLimitTimeType(b2::protocol::commondata::FactoryLimitTimeType type)
//	{
//		int b2Type = B2FactoryLimitTimeType::ONCE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::FactoryLimitTimeType::FLT_ONCE:
//			b2Type = B2FactoryLimitTimeType::ONCE;
//			break;
//		case b2::protocol::commondata::FactoryLimitTimeType::FLT_DAY:
//			b2Type = B2FactoryLimitTimeType::DAY;
//			break;
//		case b2::protocol::commondata::FactoryLimitTimeType::FLT_WEEK:
//			b2Type = B2FactoryLimitTimeType::WEEK;
//			break;
//		case b2::protocol::commondata::FactoryLimitTimeType::FLT_MONTH:
//			b2Type = B2FactoryLimitTimeType::MONTH;
//			break;
//		case b2::protocol::commondata::FactoryLimitTimeType::FLT_NONE:
//			b2Type = B2FactoryLimitTimeType::NONE;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2FactoryVisibleType(b2::protocol::commondata::FactoryVisibleType type)
//	{
//		int b2Type = B2FactoryVisibleType::CLOSE;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::FactoryVisibleType::FV_CLOSE:
//			b2Type = B2FactoryVisibleType::CLOSE;
//			break;
//		case b2::protocol::commondata::FactoryVisibleType::FV_OPEN:
//			b2Type = B2FactoryVisibleType::OPEN;
//			break;
//		case b2::protocol::commondata::FactoryVisibleType::FV_PERIOD:
//			b2Type = B2FactoryVisibleType::PERIOD;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2DonationConsumableType(b2::protocol::commondata::DonationConsumableType type)
//	{
//		int b2Type = B2DonationConsumableType::UNDEFINED;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::DonationConsumableType::CT_GEM:
//			b2Type = B2DonationConsumableType::GEM;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_BLADE_POINT:
//			b2Type = B2DonationConsumableType::BLADE_POINT;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_RANK_PROMOTION_POINT:
//			b2Type = B2DonationConsumableType::RANK_PROMOTION_POINT;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_HERO_PIECE:
//			b2Type = B2DonationConsumableType::HERO_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_MAGIC_CRYSTAL:
//			b2Type = B2DonationConsumableType::MAGIC_CRYSTAL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_TEARS_WIND:
//			b2Type = B2DonationConsumableType::TEARS_WIND;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_FEATHER_ARCHANGEL:
//			b2Type = B2DonationConsumableType::FEATHER_ARCHANGEL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_WEAPON_UPGRADE_STONE:
//			b2Type = B2DonationConsumableType::WEAPON_UPGRADE_STONE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_ARMOR_UPGRADE_STONE:
//			b2Type = B2DonationConsumableType::ARMOR_UPGRADE_STONE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_ACCESSORY_UPGRADE_STONE:
//			b2Type = B2DonationConsumableType::ACCESSORY_UPGRADE_STONE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_WEAPON_ENHANCE_SCROLL:
//			b2Type = B2DonationConsumableType::WEAPON_ENHANCE_SCROLL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_ARMOR_ENHANCE_SCROLL:
//			b2Type = B2DonationConsumableType::ARMOR_ENHANCE_SCROLL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_ACCESSORY_ENHANCE_SCROLL:
//			b2Type = B2DonationConsumableType::ACCESSORY_ENHANCE_SCROLL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_BLESSED_WEAPON_ENHANCE_SCROLL:
//			b2Type = B2DonationConsumableType::BLESSED_WEAPON_ENHANCE_SCROLL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_BLESSED_ARMOR_ENHANCE_SCROLL:
//			b2Type = B2DonationConsumableType::BLESSED_ARMOR_ENHANCE_SCROLL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_BLESSED_ACCESSORY_ENHANCE_SCROLL:
//			b2Type = B2DonationConsumableType::BLESSED_ACCESSORY_ENHANCE_SCROLL;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_RAMIEL_PROTECTION:
//			b2Type = B2DonationConsumableType::RAMIEL_PROTECTION;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_AETHER_PIECE:
//			b2Type = B2DonationConsumableType::AETHER_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_CALIGULA_PIECE:
//			b2Type = B2DonationConsumableType::CALIGULA_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_LILLIS_PIECE:
//			b2Type = B2DonationConsumableType::LILLIS_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_BEELZEBUB_PIECE:
//			b2Type = B2DonationConsumableType::BEELZEBUB_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_AZRAEL_PIECE:
//			b2Type = B2DonationConsumableType::AZRAEL_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_NIDHOGG_PIECE:
//			b2Type = B2DonationConsumableType::NIDHOGG_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_APOCALYPSE_PIECE:
//			b2Type = B2DonationConsumableType::APOCALYPSE_PIECE;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_LIGHT_LAMP:
//			b2Type = B2DonationConsumableType::LIGHT_LAMP;
//			break;
//		case b2::protocol::commondata::DonationConsumableType::CT_ADVANCED_RANK_PROMOTION_POINT:
//			b2Type = B2DonationConsumableType::ADVANCED_RANK_PROMOTION_POINT;
//			break;
//		}
//		return b2Type;
//	}
//
//	int32 FB2MessageInfoConverter::ToB2DonationCostType(b2::protocol::commondata::DonationCostType type)
//	{
//		int b2Type = B2DonationCostType::GEM;
//
//		switch (type)
//		{
//		case b2::protocol::commondata::DonationCostType::DC_GEM:
//			b2Type = B2DonationCostType::GEM;
//			break;
//		case b2::protocol::commondata::DonationCostType::DC_GOLD:
//			b2Type = B2DonationCostType::GOLD;
//		}
//		return b2Type;
//	}
//
//	b2::protocol::commondata::Endpoint FB2MessageInfoConverter::B2Endpoint2PbEndpoint(const B2EndpointPtr b2Endpoint)
//    {
//        b2::protocol::commondata::Endpoint endpoint;
//        endpoint.set_host(TCHAR_TO_ANSI(*b2Endpoint->host));
//        endpoint.set_port(b2Endpoint->port);
//        return endpoint;
//    }
//
//    b2::protocol::commondata::RoomLocation FB2MessageInfoConverter::B2RoomLocation2PbRoomLocation(const B2RoomLocationPtr b2RoomLocation)
//    {
//        b2::protocol::commondata::RoomLocation roomLocation;
//        roomLocation.mutable_channel_endpoint()->CopyFrom(B2Endpoint2PbEndpoint(b2RoomLocation->channel_endpoint));
//        roomLocation.set_room_id(TCHAR_TO_ANSI(*b2RoomLocation->room_id));
//        return roomLocation;
//    }
//
//    b2::protocol::channel::RoomProperty FB2MessageInfoConverter::B2RoomProperty2PbRoomProperty(const B2RoomPropertyPtr b2RoomProperty)
//    {
//        b2::protocol::channel::RoomProperty roomProperty;
//        roomProperty.set_key(TCHAR_TO_ANSI(*b2RoomProperty->key));
//        roomProperty.set_value(TCHAR_TO_ANSI(*b2RoomProperty->value));
//        return roomProperty;
//    }
//
//	b2::protocol::session::FindAccountRequest::QueryParameter FB2MessageInfoConverter::B2FindAccountQuery2PbQueryParameter(const B2FindAccountQueryPtr b2FindAccountQuery)
//	{
//		b2::protocol::session::FindAccountRequest::QueryParameter queryParameter;
//		if (!b2FindAccountQuery->nickname.IsEmpty())
//		{
//			queryParameter.set_nickname(TCHAR_TO_UTF8(*b2FindAccountQuery->nickname));
//		}
//		else if (b2FindAccountQuery->account_id != 0)
//		{
//			queryParameter.set_account_id(b2FindAccountQuery->account_id);
//		}
//
//		for (auto characterType : b2FindAccountQuery->character_types)
//		{
//			queryParameter.add_character_types(B2CharacterType2PbCharacter(characterType));
//		}
//
//		if (b2FindAccountQuery->item_preset_id >= 0)
//		{
//			queryParameter.set_item_preset_id(b2FindAccountQuery->item_preset_id);
//		}
//		if (b2FindAccountQuery->skill_slot >= 0)
//		{
//			queryParameter.set_skill_slot(b2FindAccountQuery->skill_slot);
//		}
//		return queryParameter;
//	}
//
//	b2::protocol::commondata::BattleFormation FB2MessageInfoConverter::B2BattleFormation2PbBattleFormation(const B2BattleFormationPtr b2Formation)
//	{
//		b2::protocol::commondata::BattleFormation formation;
//
//		formation.set_formation_type(B2BattleFormationType2PbBattleFormationType(b2Formation->formation_type));
//		formation.set_formation_level(b2Formation->formation_level);
//		return formation;
//	}
//
//	b2::protocol::commondata::TeamBattleEntry FB2MessageInfoConverter::B2TeamBattleEntry2PbTeamBattleEntry(const B2TeamBattleEntryPtr entry)
//	{
//		b2::protocol::commondata::TeamBattleEntry entryPb;
//		entryPb.set_formation_type(B2BattleFormationType2PbBattleFormationType(entry->formation_type));
//		entryPb.set_character_slot_1(B2CharacterType2PbCharacter(entry->character_slot_1));
//		entryPb.set_character_slot_2(B2CharacterType2PbCharacter(entry->character_slot_2));
//		entryPb.set_character_slot_3(B2CharacterType2PbCharacter(entry->character_slot_3));
//		return entryPb;
//	}
//
//	b2::protocol::commondata::MatchResult FB2MessageInfoConverter::B2MatchResult2PbMatchResult(const B2MatchResultPtr result)
//	{
//		b2::protocol::commondata::MatchResult matchResultPb;
//		matchResultPb.set_account_id(result->account_id);
//		matchResultPb.set_match_token(TCHAR_TO_ANSI(*result->match_token));
//		matchResultPb.set_result(B2MatchResultType2PbMatchResultType(result->result));
//		return matchResultPb;
//	}
//
//	b2::protocol::commondata::GuildBattleEntry::SlotData FB2MessageInfoConverter::B2Slot2PbGuildBattleEntrySlot(const int32 slotNum, const int32 entryCode)
//	{
//		b2::protocol::commondata::GuildBattleEntry::SlotData pb;
//		pb.set_slot_num(slotNum);
//		pb.set_entry_code(entryCode);
//		return pb;
//	}
//
//	b2::protocol::commondata::RoomCharacter FB2MessageInfoConverter::ToRoomCharacter(const B2RoomCharacterPtr b2RoomCharacter)
//	{
//		b2::protocol::commondata::RoomCharacter pb;
//		pb.set_character_type(B2CharacterType2PbCharacter(b2RoomCharacter->character_type));
//		pb.set_character_level(b2RoomCharacter->character_level);
//		pb.set_character_power(b2RoomCharacter->character_power);
//		return pb;
//	}
//
//	b2::protocol::commondata::AssaultBattleInfo FB2MessageInfoConverter::ToAssaultBattleInfo(const B2AssaultBattleInfoPtr b2AssaultBattleInfo)
//	{
//		b2::protocol::commondata::AssaultBattleInfo pb;
//		pb.set_kill_count(b2AssaultBattleInfo->kill_count);
//		pb.set_death_count(b2AssaultBattleInfo->death_count);
//		pb.set_damage(b2AssaultBattleInfo->damage);
//		return pb;
//	}
//
//	b2::protocol::commondata::PresetNum FB2MessageInfoConverter::ToPresetNum(const B2PresetNumPtr b2PresetNum)
//	{
//		b2::protocol::commondata::PresetNum pb;
//		pb.set_character_type(B2CharacterType2PbCharacter(b2PresetNum->character_type));
//		pb.set_item_preset_num(b2PresetNum->item_preset_num);
//		pb.set_skill_preset_num(b2PresetNum->skill_preset_num);
//		return pb;
//	}
//
//	b2::protocol::session::ClearStageRequest::ItemSpotSaleOption FB2MessageInfoConverter::ToItemSpotSaleOption(const B2ItemSpotSaleOptionPtr b2ItemSpotSaleOption)
//	{
//		b2::protocol::session::ClearStageRequest::ItemSpotSaleOption pb;
//		for (int32 inventoryType : b2ItemSpotSaleOption->inventory_types) {
//			pb.add_inventory_types(B2InventoryType2PbInventory(inventoryType));
//		}
//		pb.set_item_grade(b2ItemSpotSaleOption->item_grade);
//		return pb;
//	}
//
//	b2::protocol::session::ClearStageRequest::AetherSpotSaleOption FB2MessageInfoConverter::ToAetherSpotSaleOption(const B2AetherSpotSaleOptionPtr b2AetherSpotSaleOption)
//	{
//		b2::protocol::session::ClearStageRequest::AetherSpotSaleOption pb;
//		pb.set_aether_grade(b2AetherSpotSaleOption->aether_grade);
//		return pb;
//	}
//
//	b2::protocol::session::TrackingPowerDetailRequest::CharacterPowerSpecific FB2MessageInfoConverter::ToCharacterPowerSpecific(const B2CharacterPowerSpecificPtr b2CharacterPowerSpecific)
//	{
//		b2::protocol::session::TrackingPowerDetailRequest::CharacterPowerSpecific pb;
//		pb.set_category(b2CharacterPowerSpecific->category);
//		pb.set_power(b2CharacterPowerSpecific->power);
//		return pb;
//	}
//
//	////////////////////////////////////////////////////////////////////////
//	 common pb to b2 struct
//
//	B2AccountInfoPtr FB2MessageInfoConverter::AccountPb2B2AccountInfo(const b2::protocol::commondata::WorldAccount& account)
//	{
//		auto b2Account = std::make_shared<B2AccountInfo>();
//		b2Account->name = UTF8_TO_TCHAR(account.name().c_str());
//		if (account.has_account_id()) b2Account->account_id = account.account_id();
//		if (account.has_cash()) b2Account->cash = account.cash();
//		if (account.has_money()) b2Account->money = account.money();
//		if (account.has_blade_point()) b2Account->blade_point = account.blade_point();
//		if (account.has_blade_point_max()) b2Account->blade_point_max = account.blade_point_max();
//		if (account.has_blade_point_charge_lefttime_second()) b2Account->blade_point_charge_lefttime_second = account.blade_point_charge_lefttime_second();
//		if (account.has_inventory_aether_buy_count()) b2Account->inventory_aether_buy_count = account.inventory_aether_buy_count();
//		if (account.has_inventory_consumable_slot_count()) b2Account->inventory_consumable_slot_count = account.inventory_consumable_slot_count();
//		if (account.has_dark_crystal()) b2Account->dark_crystal = account.dark_crystal();
//		if (account.has_stage_boost_ticket()) b2Account->stage_boost_ticket = account.stage_boost_ticket();
//		if (account.has_shop_ten_lottery_ticket()) b2Account->shop_ten_lottery_ticket = account.shop_ten_lottery_ticket();
//		if (account.has_guild_id()) b2Account->guild_id = account.guild_id();
//		if (account.has_fame_point()) b2Account->fame_point = account.fame_point();
//		if (account.has_social_point()) b2Account->social_point = account.social_point();
//		if (account.has_last_play_stage_id()) b2Account->last_play_stage_id = account.last_play_stage_id();
//		if (account.has_last_clear_stage_id_normal()) b2Account->last_clear_stage_id_normal = account.last_clear_stage_id_normal();
//		if (account.has_last_clear_stage_id_hard()) b2Account->last_clear_stage_id_hard = account.last_clear_stage_id_hard();
//		if (account.has_last_clear_stage_id_very_hard()) b2Account->last_clear_stage_id_very_hard = account.last_clear_stage_id_very_hard();
//		if (account.has_last_clear_stage_id_hell()) b2Account->last_clear_stage_id_hell = account.last_clear_stage_id_hell();
//		if (account.has_attendance_type()) b2Account->attendance_type = PbDailyAttendanceTypeToB2DailyAttendanceType(account.attendance_type());
//		if (account.has_attendance_day()) b2Account->attendance_day = account.attendance_day();
//		if (account.has_nextattendance_time()) b2Account->nextAttendance_time = account.nextattendance_time();
//		if (account.has_daily_play_time_in_sec()) b2Account->daily_play_time_in_sec = account.daily_play_time_in_sec();
//		if (account.has_daily_play_time_reward_index()) b2Account->daily_play_time_reward_index = account.daily_play_time_reward_index();
//		if (account.has_daily_play_time_update_time()) b2Account->daily_play_time_update_time = account.daily_play_time_update_time();
//		if (account.has_rest_time()) b2Account->rest_time = account.rest_time();
//		if (account.has_leave_guild_time()) b2Account->leave_guild_time = account.leave_guild_time();
//		if (account.has_kicked_guild_id()) b2Account->kicked_guild_id = account.kicked_guild_id();
//		if (account.has_gold_buff_end_time()) b2Account->gold_buff_end_time = account.gold_buff_end_time();
//		if (account.has_exp_buff_end_time()) b2Account->exp_buff_end_time = account.exp_buff_end_time();
//		if (account.has_gold_buff_ticket()) b2Account->gold_buff_ticket = account.gold_buff_ticket();
//		if (account.has_exp_buff_ticket()) b2Account->exp_buff_ticket = account.exp_buff_ticket();
//		if (account.has_stage_auto_clear_ticket()) b2Account->stage_auto_clear_ticket = account.stage_auto_clear_ticket();
//		if (account.has_next_fairy_gift_time()) b2Account->next_fairy_gift_time = account.next_fairy_gift_time();
//
//		return b2Account;
//	}
//
//	B2LightAccountPtr FB2MessageInfoConverter::LightAccountPb2B2LightAccount(const b2::protocol::commondata::LightAccount& lightAccount)
//	{
//		auto b2LihgtAccount = std::make_shared<B2LightAccount>();
//
//		if (lightAccount.has_account_id()) b2LihgtAccount->account_id = lightAccount.account_id();
//		if (lightAccount.has_cash()) b2LihgtAccount->cash = lightAccount.cash();
//		if (lightAccount.has_money()) b2LihgtAccount->money = lightAccount.money();
//		if (lightAccount.has_blade_point()) b2LihgtAccount->blade_point = lightAccount.blade_point();
//		if (lightAccount.has_dark_crystal()) b2LihgtAccount->dark_crystal = lightAccount.dark_crystal();
//		if (lightAccount.has_social_point()) b2LihgtAccount->social_point = lightAccount.social_point();
//		if (lightAccount.has_stage_boost_ticket()) b2LihgtAccount->stage_boost_ticket = lightAccount.stage_boost_ticket();
//		if (lightAccount.has_shop_ten_lottery_ticket()) b2LihgtAccount->shop_ten_lottery_ticket = lightAccount.shop_ten_lottery_ticket();
//		if (lightAccount.has_fame_point()) b2LihgtAccount->fame_point = lightAccount.fame_point();
//		if (lightAccount.has_stage_auto_clear_ticket()) b2LihgtAccount->stage_auto_clear_ticket = lightAccount.stage_auto_clear_ticket();
//		
//		return b2LihgtAccount;
//	}
//
//	b2network::B2FindAccountInfoPtr FB2MessageInfoConverter::AccountInfoPb2B2FindAccount(const b2::protocol::commondata::AccountInfo& accountInfo)
//	{
//		auto b2FindAccount = std::make_shared<B2FindAccountInfo>();
//
//		b2FindAccount->nickname = UTF8_TO_TCHAR(accountInfo.nickname().c_str());
//		b2FindAccount->account_id = accountInfo.account_id();
//		b2FindAccount->guild_id = accountInfo.guild_id();
//		b2FindAccount->assault_match_grade = accountInfo.assault_match_grade();
//		b2FindAccount->assault_match_star_count = accountInfo.assault_match_star_count();
//
//		for (auto character : accountInfo.characters())
//		{
//			b2FindAccount->characters.Emplace(CharacterPb2B2CharacterServerInfo(character));
//		}
//
//		for (auto item : accountInfo.equip_items())
//		{
//			b2FindAccount->equip_items.Emplace(ItemPb2B2ItemServerInfo(item));
//		}
//
//		for (auto costume : accountInfo.equip_costumes())
//		{
//			b2FindAccount->equip_costumes.Emplace(CostumePb2B2CostumeServerInfo(costume));
//		}
//
//		for (auto totem : accountInfo.equip_totems())
//		{
//			b2FindAccount->equip_totems.Emplace(TotemPb2B2TotemServerInfo(totem));
//		}
//
//		for (auto aether : accountInfo.mount_aethers())
//		{
//			b2FindAccount->mount_aethers.Emplace(AetherPb2B2AetherServerInfo(aether));
//		}
//
//		for (auto relic : accountInfo.relics()) 
//		{
//			b2FindAccount->relics.Emplace(ToB2Relic(relic));
//		}
//
//		for (auto wingOption : accountInfo.wing_options())
//		{
//			b2FindAccount->wing_options.Emplace(ToB2WingOptionInfo(wingOption));
//		}
//
//		for (auto guildSkill : accountInfo.guild_skills())
//		{
//			b2FindAccount->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//		}
//
//		for (auto itemPreset : accountInfo.item_presets())
//		{
//			b2FindAccount->item_presets.Emplace(ItemPreset2B2ItemPreset(itemPreset));
//		}
//
//		for (auto presetNum : accountInfo.character_presets())
//		{
//			b2FindAccount->character_presets.Emplace(PresetNumPb2B2PresetNum(presetNum));
//		}
//
//		for (auto unitySkill : accountInfo.unity_skills())
//		{
//			b2FindAccount->unity_skills.Emplace(ToB2UnitySkill(unitySkill));
//		}
//
//		for (auto fairy : accountInfo.fairies())
//		{
//			b2FindAccount->fairies.Emplace(ToB2FairyStatus(fairy));
//		}
//
//		return b2FindAccount;
//	}
//
//	b2network::B2PresetNumPtr FB2MessageInfoConverter::PresetNumPb2B2PresetNum(const b2::protocol::commondata::PresetNum& presetNum)
//	{
//		auto b2PresetNum = std::make_shared<B2PresetNum>();
//
//		b2PresetNum->character_type = PbCharacter2B2CharacterType(presetNum.character_type());
//		b2PresetNum->item_preset_num = presetNum.item_preset_num();
//		b2PresetNum->skill_preset_num = presetNum.skill_preset_num();
//
//		return b2PresetNum;
//	}
//
//	B2ItemServerInfoPtr FB2MessageInfoConverter::ItemPb2B2ItemServerInfo(const b2::protocol::commondata::Item& item)
//	{
//		auto b2Item = std::make_shared<B2ItemServerInfo>();
//
//		b2Item->id = item.id();
//		b2Item->template_id = item.template_id();
//		b2Item->character_type = PbItemCharacter2B2CharacterType(item.character_type());
//		b2Item->inventory_type = PbInventory2B2InventoryType(item.inventory_type());
//		b2Item->equip_position = PbEquipPosition2B2EquipPositionType(item.equip_position());
//		b2Item->amount = item.amount();
//		b2Item->grade = item.grade();
//		b2Item->quality = item.quality();
//		b2Item->random_option_id1 = item.random_option_id1();
//		b2Item->random_option_id2 = item.random_option_id2();
//		b2Item->random_option_id3 = item.random_option_id3();
//		b2Item->random_option_value1 = item.random_option_value1();
//		b2Item->random_option_value2 = item.random_option_value2();
//		b2Item->random_option_value3 = item.random_option_value3();
//		b2Item->level = item.level();
//		b2Item->exp = item.exp();
//		b2Item->enhance_level = item.enhance_level();
//		b2Item->surpass_count = item.surpass_count();
//		b2Item->locked = item.locked();
//		b2Item->power = item.power();
//		b2Item->is_new = item.is_new();
//		b2Item->md_main_attribute_value = item.md_main_attribute_value();
//		b2Item->open_seal_slot_count = item.open_seal_slot_count();
//
//		for (auto state : item.seal_slot_states())
//		{
//			b2Item->seal_slot_states.Emplace(SealSlotState2B2SealSlotState(state));
//		}
//		return b2Item;
//	}
//
//	b2network::B2SealSlotStatePtr FB2MessageInfoConverter::SealSlotState2B2SealSlotState(const b2::protocol::commondata::SealSlotState& sealSlotState)
//	{
//		auto b2SealSlotState = std::make_shared<B2SealSlotState>();
//
//		b2SealSlotState->seal_slot = sealSlotState.seal_slot();
//		b2SealSlotState->opened = sealSlotState.opened();
//		b2SealSlotState->option_id = sealSlotState.option_id();
//		b2SealSlotState->option_value = sealSlotState.option_value();
//
//		return b2SealSlotState;
//	}
//
//	b2network::B2ItemPresetPtr FB2MessageInfoConverter::ItemPreset2B2ItemPreset(const b2::protocol::commondata::ItemPreset& itemPreset)
//	{
//		auto b2ItemPreset = std::make_shared<B2ItemPreset>();
//
//		b2ItemPreset->account_id = itemPreset.account_id();
//		b2ItemPreset->character_type = PbCharacter2B2CharacterType(itemPreset.character_type());
//		b2ItemPreset->preset_id = itemPreset.preset_id();
//		b2ItemPreset->equip_position = PbEquipPosition2B2EquipPositionType(itemPreset.equip_position());
//		b2ItemPreset->item_id = itemPreset.item_id();
//
//		return b2ItemPreset;
//	}
//
//	B2CostumeServerInfoPtr FB2MessageInfoConverter::CostumePb2B2CostumeServerInfo(const b2::protocol::commondata::Costume& costume)
//	{
//		auto b2Costume = std::make_shared<B2CostumeServerInfo>();
//
//		b2Costume->id = costume.id();
//		b2Costume->template_id = costume.template_id();
//		b2Costume->character_type = PbCostumeCharacter2B2CharacterType(costume.character_type());
//		b2Costume->inventory_type = PbInventory2B2InventoryType(costume.inventory_type());
//		b2Costume->equip_position = PbEquipPosition2B2EquipPositionType(costume.equip_position());
//		b2Costume->amount = costume.amount();
//		b2Costume->grade = costume.grade();
//		b2Costume->quality = costume.quality();
//		b2Costume->random_option_id1 = costume.random_option_id1();
//		b2Costume->random_option_id2 = costume.random_option_id2();
//		b2Costume->random_option_id3 = costume.random_option_id3();
//		b2Costume->random_option_value1 = costume.random_option_value1();
//		b2Costume->random_option_value2 = costume.random_option_value2();
//		b2Costume->random_option_value3 = costume.random_option_value3();
//		b2Costume->level = costume.level();
//		b2Costume->exp = costume.exp();
//		b2Costume->enhance_level = costume.enhance_level();
//		b2Costume->surpass_count = costume.surpass_count();
//		b2Costume->locked = costume.locked();
//		b2Costume->power = costume.power();
//		b2Costume->is_new = costume.is_new();
//		b2Costume->md_main_attribute_value = costume.md_main_attribute_value();
//		return b2Costume;
//	}
//
//	B2AetherServerInfoPtr FB2MessageInfoConverter::AetherPb2B2AetherServerInfo(const b2::protocol::commondata::Aether& aether)
//	{
//		auto b2Aether = std::make_shared<B2AetherServerInfo>();
//
//		b2Aether->id = aether.id();
//		b2Aether->type = aether.type();
//		b2Aether->slot = aether.slot();
//		b2Aether->character_type = PbAetherCharacter2B2CharacterType(aether.character_type());
//		b2Aether->level = aether.level();
//		b2Aether->grade = aether.grade();
//		b2Aether->tier = aether.tier();
//		b2Aether->main_option_id = aether.main_option_id();
//		b2Aether->main_option_value = aether.main_option_value();
//		b2Aether->sub_option_id1 = aether.sub_option_id1();
//		b2Aether->sub_option_id2 = aether.sub_option_id2();
//		b2Aether->sub_option_id3 = aether.sub_option_id3();
//		b2Aether->sub_option_value1 = aether.sub_option_value1();
//		b2Aether->sub_option_value2 = aether.sub_option_value2();
//		b2Aether->sub_option_value3 = aether.sub_option_value3();
//		b2Aether->sub_option_enhance_count1 = aether.sub_option_enhance_count1();
//		b2Aether->sub_option_enhance_count2 = aether.sub_option_enhance_count2();
//		b2Aether->sub_option_enhance_count3 = aether.sub_option_enhance_count3();
//		b2Aether->locked = aether.locked();
//		b2Aether->is_new = aether.is_new();
//		b2Aether->calc_main_option_value = aether.calc_main_option_value();
//		b2Aether->calc_sub_option_value1 = aether.calc_sub_option_value1();
//		b2Aether->calc_sub_option_value2 = aether.calc_sub_option_value2();
//		b2Aether->calc_sub_option_value3 = aether.calc_sub_option_value3();
//		b2Aether->calc_tier = aether.calc_tier();
//
//		return b2Aether;
//	}
//
//	B2TotemServerInfoPtr FB2MessageInfoConverter::TotemPb2B2TotemServerInfo(const b2::protocol::commondata::Totem& totem)
//	{
//		auto b2Totem = std::make_shared<B2TotemServerInfo>();
//
//		b2Totem->id = totem.id();
//		b2Totem->template_id = totem.template_id();
//		b2Totem->character_type = PbTotemCharacter2B2CharacterType(totem.character_type());
//		b2Totem->equip_slot = B2TotemEquipSlot2PbTotemEquipSlot(totem.equip_slot());
//		b2Totem->grade = totem.grade();
//		b2Totem->main_option_id = totem.main_option_id();
//		b2Totem->main_option_value = totem.main_option_value();
//
//		for (auto subOptionDetail : totem.sub_option_details())
//		{
//			b2Totem->sub_option_details.Emplace(ToB2TotemSubOptionDetail(subOptionDetail));
//		}
//
//		b2Totem->refine_level = totem.refine_level();
//		b2Totem->refine_try_count = totem.refine_try_count();
//		b2Totem->gold_success_rate = totem.gold_success_rate();
//		b2Totem->piece_success_rate = totem.piece_success_rate();
//		b2Totem->locked = totem.locked();
//		b2Totem->power = totem.power();
//		b2Totem->is_new = totem.is_new();
//		
//		return b2Totem;
//	}
//
//	B2CharacterSetOptionInfoPtr FB2MessageInfoConverter::CharacterSetOptionInfoPb2B2CharacterSetOptionInfo(const b2::protocol::commondata::CharacterSetOptionInfo& characterSetOptionInfo)
//	{
//		auto b2CharacterSetOption = std::make_shared<B2CharacterSetOptionInfo>();
//
//		b2CharacterSetOption->selected_offense_set_option_id = characterSetOptionInfo.selected_offense_set_option_id();
//		b2CharacterSetOption->selected_defense_set_option_id = characterSetOptionInfo.selected_defense_set_option_id();
//
//		for (auto defenseSetOptionid : characterSetOptionInfo.defense_set_option_ids())
//		{
//			b2CharacterSetOption->defense_set_option_ids.Emplace(defenseSetOptionid);
//		}
//
//		return b2CharacterSetOption;
//	}	
//
//	B2SkillServerInfoPtr FB2MessageInfoConverter::SkillPb2B2SkillServerInfo(const b2::protocol::commondata::Skill& skill)
//	{
//		auto b2Skill = std::make_shared<B2SkillServerInfo>();
//
//		b2Skill->skill_id = skill.skill_id();
//		b2Skill->skill_level = skill.skill_level();
//
//		return b2Skill;
//	}
//
//	b2network::B2SkillQuickSlotPtr FB2MessageInfoConverter::SkillQuickSlotPb2B2SkillQuickSlot(const b2::protocol::commondata::SkillQuickSlot& skillPreset)
//	{
//		auto b2SkillQuickSlot = std::make_shared<B2SkillQuickSlot>();
//
//		b2SkillQuickSlot->slot = skillPreset.slot();
//		b2SkillQuickSlot->left_skill_point = skillPreset.left_skill_point();
//		b2SkillQuickSlot->skill_total_power = skillPreset.skill_total_power();
//		b2SkillQuickSlot->using_skill_id_1 = skillPreset.using_skill_id_1();
//		b2SkillQuickSlot->using_skill_id_2 = skillPreset.using_skill_id_2();
//		b2SkillQuickSlot->using_skill_id_3 = skillPreset.using_skill_id_3();
//
//		return b2SkillQuickSlot;
//	}
//
//	b2network::B2mdSkillInfoPtr FB2MessageInfoConverter::mdSkillInfoPb2B2mdSkillInfo(const b2::protocol::masterdata::mdSkillInfo& mdSkillInfo)
//	{
//		auto b2mdSkillInfo = std::make_shared<B2mdSkillInfo>();
//
//		b2mdSkillInfo->skill_id = mdSkillInfo.skill_id();
//		b2mdSkillInfo->character_type = PbCharacter2B2CharacterType(mdSkillInfo.character_type());
//		b2mdSkillInfo->learn_level = mdSkillInfo.learn_level();
//		b2mdSkillInfo->max_skill_level = mdSkillInfo.max_skill_level();
//		b2mdSkillInfo->usable = mdSkillInfo.usable();
//		b2mdSkillInfo->level_up_cost_factor = mdSkillInfo.level_up_cost_factor();
//
//		return b2mdSkillInfo;
//	}
//
//	b2network::B2mdSkillOptionPtr FB2MessageInfoConverter::mdSkillOptionPb2B2mdSkillOption(const b2::protocol::masterdata::mdSkillOption& mdSkillOption)
//	{
//		auto b2mdSkillOption = std::make_shared<B2mdSkillOption>();
//
//		b2mdSkillOption->skill_id = mdSkillOption.skill_id();
//		b2mdSkillOption->skill_level = mdSkillOption.skill_level();
//		b2mdSkillOption->option_id = mdSkillOption.option_id();
//		b2mdSkillOption->option_value = mdSkillOption.option_value();
//		b2mdSkillOption->power = mdSkillOption.power();
//
//		return b2mdSkillOption;
//	}
//
//	b2network::B2mdItemLevelupExpFactorPtr FB2MessageInfoConverter::mdItemLevelupExpFactorPb2B2mdItemLevelupExpFactor(const b2::protocol::masterdata::mdItemLevelupExpFactor& mdItemLevelupExpFactor)
//	{
//		auto b2mdItemLevelupExpFactor = std::make_shared<B2mdItemLevelupExpFactor>();
//
//		b2mdItemLevelupExpFactor->type_name = ANSI_TO_TCHAR(mdItemLevelupExpFactor.type_name().c_str());
//		b2mdItemLevelupExpFactor->key_code = mdItemLevelupExpFactor.key_code();
//		b2mdItemLevelupExpFactor->factor = mdItemLevelupExpFactor.factor();
//
//		return b2mdItemLevelupExpFactor;
//	}
//
//	b2network::B2mdItemOptionAddValuePtr FB2MessageInfoConverter::mdItemOptionAddValuePb2B2mdItemOptionAddValue(const b2::protocol::masterdata::mdItemOptionAddValue& mdItemOptionAddValue)
//	{
//		auto b2mdItemOptionAddValue = std::make_shared<B2mdItemOptionAddValue>();
//
//		b2mdItemOptionAddValue->option_id = mdItemOptionAddValue.option_id();
//		b2mdItemOptionAddValue->add_value = mdItemOptionAddValue.add_value();
//		b2mdItemOptionAddValue->power_factor = mdItemOptionAddValue.power_factor();
//
//		return b2mdItemOptionAddValue;
//	}
//
//
//	b2network::B2mdItemLevelupCostPtr FB2MessageInfoConverter::mdItemLevelupCostPb2B2mdItemLevelupCost(const b2::protocol::masterdata::mdItemLevelupCost& mdItemLevelupCost)
//	{
//		auto b2mdItemLevelupCost = std::make_shared<B2mdItemLevelupCost>();
//
//		b2mdItemLevelupCost->inventory_type = PbInventory2B2InventoryType(mdItemLevelupCost.inventory_type());
//		b2mdItemLevelupCost->level = mdItemLevelupCost.level();
//		b2mdItemLevelupCost->price_grade1 = mdItemLevelupCost.price_grade1();
//		b2mdItemLevelupCost->price_grade2 = mdItemLevelupCost.price_grade2();
//		b2mdItemLevelupCost->price_grade3 = mdItemLevelupCost.price_grade3();
//		b2mdItemLevelupCost->price_grade4 = mdItemLevelupCost.price_grade4();
//		b2mdItemLevelupCost->price_grade5 = mdItemLevelupCost.price_grade5();
//		b2mdItemLevelupCost->price_grade6 = mdItemLevelupCost.price_grade6();
//		b2mdItemLevelupCost->price_grade7 = mdItemLevelupCost.price_grade7();
//		b2mdItemLevelupCost->price_grade8 = mdItemLevelupCost.price_grade8();
//		b2mdItemLevelupCost->price_grade9 = mdItemLevelupCost.price_grade9();
//		b2mdItemLevelupCost->price_grade10 = mdItemLevelupCost.price_grade10();
//		b2mdItemLevelupCost->price_grade11 = mdItemLevelupCost.price_grade11();
//		b2mdItemLevelupCost->price_grade12 = mdItemLevelupCost.price_grade12();
//
//		return b2mdItemLevelupCost;
//	}
//
//	b2network::B2mdItemEnhanceCostPtr FB2MessageInfoConverter::mdItemEnhanceCostPb2B2mdItemEnhanceCost(const b2::protocol::masterdata::mdItemEnhanceCost& mdItemEnhanceCost)
//	{
//		auto b2mdItemEnhanceCost = std::make_shared<B2mdItemEnhanceCost>();
//
//		b2mdItemEnhanceCost->enhance_level = mdItemEnhanceCost.enhance_level();
//		b2mdItemEnhanceCost->enhance_rate = mdItemEnhanceCost.enhance_rate();
//		b2mdItemEnhanceCost->enhance_scroll_cost = mdItemEnhanceCost.enhance_scroll_cost();
//		b2mdItemEnhanceCost->protection_scroll_cost = mdItemEnhanceCost.protection_scroll_cost();
//		b2mdItemEnhanceCost->money_cost = mdItemEnhanceCost.money_cost();
//		b2mdItemEnhanceCost->exchange_cost_dark_crystal = mdItemEnhanceCost.exchange_cost_dary_crystal();
//		b2mdItemEnhanceCost->exchange_cost_gem = mdItemEnhanceCost.exchange_cost_gem();
//
//		return b2mdItemEnhanceCost;
//	}
//
//	b2network::B2FactoryResultPtr FB2MessageInfoConverter::FactoryResultPb2B2FactoryResult(const b2::protocol::commondata::FactoryResult& factoryResult)
//	{
//		auto b2FactoryResult = std::make_shared<B2FactoryResult>();
//
//		b2FactoryResult->factory_result_type = factoryResult.factory_result_type();
//		b2FactoryResult->make_item = ItemPb2B2ItemServerInfo(factoryResult.make_item());
//		b2FactoryResult->bonus_item = ItemPb2B2ItemServerInfo(factoryResult.bonus_item());
//
//		for (auto item : factoryResult.refund_item())
//		{
//			b2FactoryResult->refund_item.Emplace(ItemPb2B2ItemServerInfo(item));
//		}
//
//		return b2FactoryResult;
//	}
//
//	b2network::B2UnsealBoxRewardPtr FB2MessageInfoConverter::UnsealBoxRewardPb2B2UnsealBoxReward(const b2::protocol::commondata::UnsealBoxReward& unsealBoxReward)
//	{
//		auto b2UnsealBoxReward = std::make_shared<B2UnsealBoxReward>();
//
//		b2UnsealBoxReward->reward_type = PbRewardTypeToB2RewardType(unsealBoxReward.reward_type());
//		b2UnsealBoxReward->amount = unsealBoxReward.amount();
//		b2UnsealBoxReward->template_id = unsealBoxReward.template_id();
//		b2UnsealBoxReward->effect_grade = unsealBoxReward.effect_grade();
//		b2UnsealBoxReward->item = ItemPb2B2ItemServerInfo(unsealBoxReward.item());
//		b2UnsealBoxReward->aether = AetherPb2B2AetherServerInfo(unsealBoxReward.aether());
//		b2UnsealBoxReward->totem = TotemPb2B2TotemServerInfo(unsealBoxReward.totem());
//
//		return b2UnsealBoxReward;
//	}
//
//	b2network::B2FactoryLimitStatusPtr FB2MessageInfoConverter::FactoryLimitStatusPb2B2FactoryLimitStatus(const b2::protocol::commondata::FactoryLimitStatus& factoryLimitStatus)
//	{
//		auto b2FactoryLimitStatus = std::make_shared<B2FactoryLimitStatus>();
//
//		b2FactoryLimitStatus->limit_id = factoryLimitStatus.limit_id();
//		b2FactoryLimitStatus->limit_type = factoryLimitStatus.limit_type();
//		b2FactoryLimitStatus->limit_time_type = factoryLimitStatus.limit_time_type();
//		b2FactoryLimitStatus->left_count = factoryLimitStatus.left_count();
//
//		return b2FactoryLimitStatus;
//	}
//
//	b2network::B2FactoryPeriodicStatusPtr FB2MessageInfoConverter::FactoryPeriodicStatusPb2B2FactoryPeriodicStatus(const b2::protocol::commondata::FactoryPeriodicStatus& factoryPeriodicStatus)
//	{
//		auto b2FactoryPeriodicStatus = std::make_shared<B2FactoryPeriodicStatus>();
//
//		b2FactoryPeriodicStatus->factory_id = factoryPeriodicStatus.factory_id();
//		b2FactoryPeriodicStatus->preview_time = factoryPeriodicStatus.preview_time();
//		b2FactoryPeriodicStatus->start_time = factoryPeriodicStatus.start_time();
//		b2FactoryPeriodicStatus->end_time = factoryPeriodicStatus.end_time();
//
//		return b2FactoryPeriodicStatus;
//	}
//
//	b2network::B2mdCostumeEnhanceCostPtr FB2MessageInfoConverter::ToB2mdCostumeEnhanceCost(const b2::protocol::masterdata::mdCostumeEnhanceCost& mdCostumeEnhanceCost)
//	{
//		auto b2mdCostumeEnhanceCost = std::make_shared<B2mdCostumeEnhanceCost>();
//
//		b2mdCostumeEnhanceCost->grade = mdCostumeEnhanceCost.grade();
//		b2mdCostumeEnhanceCost->enhance_level = mdCostumeEnhanceCost.enhance_level();
//		b2mdCostumeEnhanceCost->money_cost = mdCostumeEnhanceCost.money_cost();
//		b2mdCostumeEnhanceCost->weapon_material_id = mdCostumeEnhanceCost.weapon_material_id();
//		b2mdCostumeEnhanceCost->weapon_amount = mdCostumeEnhanceCost.weapon_amount();
//		b2mdCostumeEnhanceCost->weapon_restore_amount = mdCostumeEnhanceCost.weapon_restore_amount();
//		b2mdCostumeEnhanceCost->armor_material_id = mdCostumeEnhanceCost.armor_material_id();
//		b2mdCostumeEnhanceCost->armor_amount = mdCostumeEnhanceCost.armor_amount();
//		b2mdCostumeEnhanceCost->armor_restore_amount = mdCostumeEnhanceCost.armor_restore_amount();
//
//		return b2mdCostumeEnhanceCost;
//	}
//
//	b2network::B2mdItemEnhanceEffectInfoPtr FB2MessageInfoConverter::mdItemEnhanceEffectInfoPb2B2mdItemEnhanceEffectInfo(const b2::protocol::masterdata::mdItemEnhanceEffectInfo& mdItemEnhanceEffectInfo)
//	{
//		auto b2mdItemEnhanceEffectInfo = std::make_shared<B2mdItemEnhanceEffectInfo>();
//
//		b2mdItemEnhanceEffectInfo->enhance_level = mdItemEnhanceEffectInfo.enhance_level();
//		b2mdItemEnhanceEffectInfo->set_effect = mdItemEnhanceEffectInfo.set_effect();
//		b2mdItemEnhanceEffectInfo->item_aura = mdItemEnhanceEffectInfo.item_aura();
//		b2mdItemEnhanceEffectInfo->channel_alpha = mdItemEnhanceEffectInfo.channel_alpha();
//		b2mdItemEnhanceEffectInfo->channel_red = mdItemEnhanceEffectInfo.channel_red();
//		b2mdItemEnhanceEffectInfo->channel_green = mdItemEnhanceEffectInfo.channel_green();
//		b2mdItemEnhanceEffectInfo->channel_blue = mdItemEnhanceEffectInfo.channel_blue();
//
//		return b2mdItemEnhanceEffectInfo;
//	}
//
//	b2network::B2mdItemSurpassLevelPtr FB2MessageInfoConverter::ToB2mdItemSurpassLevel(const b2::protocol::masterdata::mdItemSurpassLevel& pb)
//	{
//		auto b2 = std::make_shared<B2mdItemSurpassLevel>();
//
//		b2->grade = pb.grade();
//		b2->increase_level = pb.increase_level();
//		b2->max_surpass_level = pb.max_surpass_level();
//
//		return b2;
//	}
//
//	b2network::B2mdItemDismantleInfoPtr FB2MessageInfoConverter::ToB2mdItemDismantleInfo(const b2::protocol::masterdata::mdItemDismantleInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdItemDismantleInfo>();
//
//		b2->inventory_type = PbInventory2B2InventoryType(pb.inventory_type());
//		b2->grade = pb.grade();
//		b2->material1 = pb.material1();
//		b2->material_amount1 = pb.material_amount1();
//		b2->material2 = pb.material2();
//		b2->material_amount2 = pb.material_amount2();
//		b2->material3 = pb.material3();
//		b2->material_amount3 = pb.material_amount3();
//
//		return b2;
//	}
//
//	b2network::B2mdResurrectionInfoPtr FB2MessageInfoConverter::mdResurrectionInfoPb2B2ResurrectionInfo(const b2::protocol::masterdata::mdResurrectionInfo& mdResurrectionInfo)
//	{
//		auto b2mdResurrectionInfo = std::make_shared<B2mdResurrectionInfo>();
//
//		b2mdResurrectionInfo->mode_type = mdResurrectionInfo.mode_type();
//		b2mdResurrectionInfo->count = mdResurrectionInfo.count();
//		b2mdResurrectionInfo->price = mdResurrectionInfo.price();
//		b2mdResurrectionInfo->attribute_change1 = mdResurrectionInfo.attribute_change1();
//		b2mdResurrectionInfo->change_value1 = mdResurrectionInfo.change_value1();
//		b2mdResurrectionInfo->attribute_change2 = mdResurrectionInfo.attribute_change2();
//		b2mdResurrectionInfo->change_value2 = mdResurrectionInfo.change_value2();
//		b2mdResurrectionInfo->attribute_change3 = mdResurrectionInfo.attribute_change3();
//		b2mdResurrectionInfo->change_value3 = mdResurrectionInfo.change_value3();
//
//		return b2mdResurrectionInfo;
//	}
//
//	B2CharacterServerInfoPtr FB2MessageInfoConverter::CharacterPb2B2CharacterServerInfo(const b2::protocol::commondata::Character& character)
//	{
//		auto b2Character = std::make_shared<B2CharacterServerInfo>();
//
//		 character stat
//		b2Character->character_type = PbCharacter2B2CharacterType(character.character_type());
//		b2Character->name = UTF8_TO_TCHAR(character.name().c_str());
//		if (character.has_style_face()) b2Character->style_face = character.style_face();
//		if (character.has_style_hair()) b2Character->style_hair = character.style_hair();
//		if (character.has_style_skin()) b2Character->style_skin = character.style_skin();
//		if (character.has_level()) b2Character->level = character.level();
//		if (character.has_exp()) b2Character->exp = character.exp();
//		if (character.has_exp_max()) b2Character->exp_max = character.exp_max();
//		if (character.has_skill_point_buy_count()) b2Character->skill_point_buy_count = character.skill_point_buy_count();
//		if (character.has_inventory_item_buy_count()) b2Character->inventory_item_buy_count = character.inventory_item_buy_count();
//		if (character.has_inventory_item_slot_count()) b2Character->inventory_item_slot_count = character.inventory_item_slot_count();
//		if (character.has_rank()) b2Character->rank = character.rank();
//		if (character.has_rank_node_index()) b2Character->rank_node_index = character.rank_node_index();
//		if (character.has_rank_node_state()) b2Character->rank_node_state = character.rank_node_state();
//		if (character.has_wing_grade()) b2Character->wing_grade = character.wing_grade();
//		if (character.has_wing_level()) b2Character->wing_level = character.wing_level();
//		if (character.has_wing_visible()) b2Character->wing_visible = character.wing_visible();
//		if (character.has_helmet_visible()) b2Character->helmet_visible = character.helmet_visible();
//		if (character.has_aether_offense_set_option_id()) b2Character->aether_offense_set_option_id = character.aether_offense_set_option_id();
//		if (character.has_aether_defense_set_option_id()) b2Character->aether_defense_set_option_id = character.aether_defense_set_option_id();
//		if (character.has_remain_skill_point()) b2Character->remain_skill_point = character.remain_skill_point();
//		if (character.has_unity_skill()) b2Character->unity_skill = character.unity_skill();
//		if (character.has_total_power()) b2Character->total_power = character.total_power();
//		if (character.has_costume_weapon_visible()) b2Character->costume_weapon_visible = character.costume_weapon_visible();
//		if (character.has_costume_hat_visible()) b2Character->costume_hat_visible = character.costume_hat_visible();
//		if (character.has_costume_armor_visible()) b2Character->costume_armor_visible = character.costume_armor_visible();
//		if (character.has_unity_skill_awaken()) b2Character->unity_skill_awaken = character.unity_skill_awaken();
//		
//		for (auto skillPreset : character.skill_quick_slots())
//		{
//			b2Character->skill_quick_slots.Emplace(SkillQuickSlotPb2B2SkillQuickSlot(skillPreset));
//		}
//
//		 skill
//		for (auto skill : character.skills())
//		{
//			b2Character->skills.Emplace(SkillPb2B2SkillServerInfo(skill));
//		}
//		return b2Character;
//	}
//
//	B2CharacterPowerPtr FB2MessageInfoConverter::CharacterPowerPb2B2CharacterPower(const b2::protocol::commondata::CharacterPower& characterPower)
//	{
//		auto b2CharacterPower = std::make_shared<B2CharacterPower>();
//
//		 character stat
//		b2CharacterPower->character_type = PbCharacter2B2CharacterType(characterPower.character_type());
//		b2CharacterPower->total_power = characterPower.total_power();
//		
//		return b2CharacterPower;
//	}
//
//	B2LightCharacterPtr FB2MessageInfoConverter::LightCharacterPb2B2LightCharacter(const b2::protocol::commondata::LightCharacter& lightCharacter)
//	{
//		auto b2LightCharacter = std::make_shared<B2LightCharacter>();
//
//		b2LightCharacter->character_type = PbLightCharacter2B2CharacterType(lightCharacter.character_type());
//		if (lightCharacter.has_level()) b2LightCharacter->level = lightCharacter.level();
//		if (lightCharacter.has_exp()) b2LightCharacter->exp = lightCharacter.exp();
//		if (lightCharacter.has_exp_max()) b2LightCharacter->exp_max = lightCharacter.exp_max();
//		if (lightCharacter.has_changed_level()) b2LightCharacter->changed_level = lightCharacter.changed_level();
//		if (lightCharacter.has_remain_skill_point()) b2LightCharacter->remain_skill_point = lightCharacter.remain_skill_point();
//
//		return b2LightCharacter;
//	}
//
//	B2InventoryCountServerInfoPtr FB2MessageInfoConverter::InventoryCountInfoPb2B2InventoryCountServerInfo(const b2::protocol::commondata::InventoryCountInfo& inventoryInfo)
//	{
//		auto b2InventoryCount = std::make_shared<B2InventoryCountServerInfo>();
//
//		b2InventoryCount->character_type = PbCharacter2B2CharacterType(inventoryInfo.character_type());
//		b2InventoryCount->count = inventoryInfo.count();
//		b2InventoryCount->max_count = inventoryInfo.max_count();
//
//		return b2InventoryCount;
//	}
//	
//	b2network::B2StageClearInfoPtr FB2MessageInfoConverter::StageClearInfoPb2B2StageClearInfo(const b2::protocol::session::GetActInfoResponse::StageClearInfo& clearInfo)
//	{
//		auto b2StageClearInfo = std::make_shared<B2StageClearInfo>();
//
//		b2StageClearInfo->stage_id = clearInfo.stage_id();
//		b2StageClearInfo->clear_grade = clearInfo.clear_grade();
//
//		return b2StageClearInfo;
//	}
//
//	b2network::B2ActRewardInfoPtr FB2MessageInfoConverter::ActRewardInfoPb2B2ActRewardInfo(const b2::protocol::session::GetActInfoResponse::ActRewardInfo& actRewardInfo)
//	{
//		auto b2ActRewardInfo = std::make_shared<B2ActRewardInfo>();
//
//		b2ActRewardInfo->difficulty = PbStageDifficultyToB2StageDifficulty(actRewardInfo.difficulty());
//		b2ActRewardInfo->act_id = actRewardInfo.act_id();
//		b2ActRewardInfo->reward_index = actRewardInfo.reward_index();
//		b2ActRewardInfo->require_star_count = actRewardInfo.require_star_count();
//		for (auto reward : actRewardInfo.rewards())
//		{
//			b2ActRewardInfo->rewards.Emplace(ToB2Reward(reward));
//		}
//		b2ActRewardInfo->received = actRewardInfo.received();
//
//		return b2ActRewardInfo;
//	}
//
//	b2network::B2ActIdentifierPtr FB2MessageInfoConverter::ActIdentifierPb2B2ActIdentifier(const b2::protocol::session::GetActInfoResponse::ActIdentifier& actIdentifier)
//	{
//		auto b2ActIdentifier = std::make_shared<B2ActIdentifier>();
//
//		b2ActIdentifier->difficulty = PbStageDifficultyToB2StageDifficulty(actIdentifier.difficulty());
//		b2ActIdentifier->act_id = actIdentifier.act_id();
//
//		return b2ActIdentifier;
//	}
//
//	b2network::B2StageClearRewardItemInfoPtr FB2MessageInfoConverter::StageClearRewardItemInfoPb2B2StageClearRewardItemInfo(const b2::protocol::session::GetStageInfoResponse::StageClearRewardItemInfo& rewardInfo)
//	{
//		auto b2StageClearRewardInfo = std::make_shared<B2StageClearRewardItemInfo>();
//
//		b2StageClearRewardInfo->group_id = rewardInfo.group_id();
//		b2StageClearRewardInfo->grade = rewardInfo.grade();
//		b2StageClearRewardInfo->type = rewardInfo.type();
//		b2StageClearRewardInfo->main_attribute = rewardInfo.main_attribute();
//		b2StageClearRewardInfo->main_attribute_value_min = rewardInfo.main_attribute_value_min();
//		b2StageClearRewardInfo->main_attribute_value_max = rewardInfo.main_attribute_value_max();
//		b2StageClearRewardInfo->random_option_count = rewardInfo.random_option_count();
//
//		return b2StageClearRewardInfo;
//	}
//
//	b2network::B2StageClearRewardAetherInfoPtr FB2MessageInfoConverter::StageClearRewardAetherInfoPb2B2StageClearRewardAetherInfo(const b2::protocol::session::GetStageInfoResponse::StageClearRewardAetherInfo& rewardInfo)
//	{
//		auto b2StageClearRewardInfo = std::make_shared<B2StageClearRewardAetherInfo>();
//
//		b2StageClearRewardInfo->type = rewardInfo.type();
//		for (auto tier : rewardInfo.tiers())
//		{
//			b2StageClearRewardInfo->tiers.Emplace(tier);
//		}
//
//		for (auto grade : rewardInfo.grades())
//		{
//			b2StageClearRewardInfo->grades.Emplace(grade);
//		}
//
//		return b2StageClearRewardInfo;
//	}
//
//    b2network::B2EndpointPtr FB2MessageInfoConverter::EndpointPb2B2Endpoint(const b2::protocol::commondata::Endpoint& endpoint)
//    {
//        auto b2Endpoint = std::make_shared<B2Endpoint>();
//
//        b2Endpoint->host = ANSI_TO_TCHAR(endpoint.host().c_str());
//        b2Endpoint->port = endpoint.port();
//        return b2Endpoint;
//    }
//
//    b2network::B2MatchInfoPtr FB2MessageInfoConverter::MatchInfoPb2B2MatchInfo(const b2::protocol::commondata::MatchInfo& matchInfo)
//    {
//        auto b2MatchInfo = std::make_shared<B2MatchInfo>();
//
//        b2MatchInfo->match_token = ANSI_TO_TCHAR(matchInfo.match_token().c_str());
//		b2MatchInfo->opponent_type = PbMatchInfoType2B2MatchInfoType(matchInfo.opponent_type());
//        return b2MatchInfo;
//    }
//
//    b2network::B2RoomLocationPtr FB2MessageInfoConverter::RoomLocationPb2B2RoomLocation(const b2::protocol::commondata::RoomLocation& roomLocation)
//    {
//        auto b2RoomLocation = std::make_shared<B2RoomLocation>();
//
//        b2RoomLocation->channel_endpoint = EndpointPb2B2Endpoint(roomLocation.channel_endpoint());
//        b2RoomLocation->room_id = ANSI_TO_TCHAR(roomLocation.room_id().c_str());
//        return b2RoomLocation;
//    }
//
//    b2network::B2RoomParticipantPtr FB2MessageInfoConverter::RoomParticipantPb2B2RoomParticipant(const b2::protocol::channel::RoomParticipant& roomParticipant)
//    {
//        auto b2RoomParticipant = std::make_shared<B2RoomParticipant>();
//
//        b2RoomParticipant->id = roomParticipant.id();
//		if (roomParticipant.has_account_info())
//		{
//			b2RoomParticipant->account_info = AccountInfoPb2B2FindAccount(roomParticipant.account_info());
//		}
//
//		if (roomParticipant.has_account_id())
//		{
//			b2RoomParticipant->account_id = roomParticipant.account_id();
//		}
//
//        return b2RoomParticipant;
//    }
//
//    b2network::B2RoomPropertyPtr FB2MessageInfoConverter::RoomPropertyPb2B2RoomProperty(const b2::protocol::channel::RoomProperty& roomProperty)
//    {
//        auto b2RoomProperty = std::make_shared<B2RoomProperty>();
//        b2RoomProperty->key = ANSI_TO_TCHAR(roomProperty.key().c_str());
//        b2RoomProperty->value = ANSI_TO_TCHAR(roomProperty.value().c_str());
//
//        return b2RoomProperty;
//    }
//
//    b2network::B2ConfigDataPtr FB2MessageInfoConverter::ConfigDataPb2B2ConfigData(const b2::protocol::commondata::ConfigData& configData)
//	{
//		auto b2ConfigData = std::make_shared<B2ConfigData>();
//
//		b2ConfigData->skill_point_buy_price_gem = configData.skill_point_buy_price_gem();
//		b2ConfigData->max_skill_point_buy_count = configData.max_skill_point_buy_count();
//		b2ConfigData->skill_point_reset_price_gem = configData.skill_point_reset_price_gem();
//		b2ConfigData->need_cash_for_extend_inventory = configData.need_cash_for_extend_inventory();
//		b2ConfigData->increase_inventory_slot_per_buy_count = configData.increase_inventory_slot_per_buy_count();
//		b2ConfigData->max_inventory_slot_count = configData.max_inventory_slot_count();
//		b2ConfigData->max_friend_count = configData.max_friend_count();
//		b2ConfigData->max_send_ask_friend = configData.max_send_ask_friend();
//		b2ConfigData->max_receive_ask_friend = configData.max_receive_ask_friend();
//		b2ConfigData->cooltime_for_send_social_point = configData.cooltime_for_send_social_point();
//		b2ConfigData->max_delete_friend_count_for_day = configData.max_delete_friend_count_for_day();
//		b2ConfigData->max_social_point_value = configData.max_social_point_value();
//		b2ConfigData->add_social_point_for_send_to_friend = configData.add_social_point_for_send_to_friend();
//		b2ConfigData->add_social_point_for_receive_from_friend = configData.add_social_point_for_receive_from_friend();
//		b2ConfigData->stage_buff_rate_for_increase_exp = configData.stage_buff_rate_for_increase_exp();
//		b2ConfigData->stage_buff_rate_for_increase_gold = configData.stage_buff_rate_for_increase_gold();
//		b2ConfigData->stage_buff_rate_for_boost_game_speed = configData.stage_buff_rate_for_boost_game_speed();
//		b2ConfigData->guild_set_up_gold_cost = configData.guild_set_up_gold_cost();
//		b2ConfigData->default_max_guild_member_count = configData.default_max_guild_member_count();
//		b2ConfigData->max_invite_guild_count_for_account = configData.max_invite_guild_count_for_account();
//		b2ConfigData->invite_guild_expire_period_by_day = configData.invite_guild_expire_period_by_day();
//		b2ConfigData->max_raid_count_per_day = configData.max_raid_count_per_day();
//		b2ConfigData->update_guild_gold_cost = configData.update_guild_gold_cost();
//		b2ConfigData->guild_master_logout_limit_period_day = configData.guild_master_logout_limit_period_day();
//		b2ConfigData->guild_master_change_period_day = configData.guild_master_change_period_day();
//		b2ConfigData->guild_battle_time_limit_sec_client = configData.guild_battle_time_limit_sec_client();
//		b2ConfigData->hero_tower_sweep_reward_count_per_floor = configData.hero_tower_sweep_reward_count_per_floor();
//		b2ConfigData->assault_resurrection_max_count = configData.assault_resurrection_max_count();
//		b2ConfigData->pvp_match_point_max = configData.pvp_match_point_max();
//		b2ConfigData->pvp_match_point_charge_period_second = configData.pvp_match_point_charge_period_second();
//		b2ConfigData->team_match_point_max = configData.team_match_point_max();
//		b2ConfigData->team_match_point_charge_period_second = configData.team_match_point_charge_period_second();
//		b2ConfigData->item_random_option_change_cost_dark_crystal = configData.item_random_option_change_cost_dark_crystal();
//		b2ConfigData->item_preset_max_count = configData.item_preset_max_count();
//		b2ConfigData->skill_quick_slot_max_count = configData.skill_quick_slot_max_count();
//		b2ConfigData->assault_mvp_point_to_open_reward = configData.assault_mvp_point_to_open_reward();
//		b2ConfigData->duel_reward_challenge_max_count = configData.duel_reward_challenge_max_count();
//		b2ConfigData->guild_support_cool_time_hour = configData.guild_support_cool_time_hour();
//		b2ConfigData->guild_support_max_count_for_day = configData.guild_support_max_count_for_day();
//		b2ConfigData->rest_max_time_second = configData.rest_max_time_second();
//		b2ConfigData->rest_time_second_per_one_point = configData.rest_time_second_per_one_point();
//		b2ConfigData->guild_mercenary_donation_max_count_for_day = configData.guild_mercenary_donation_max_count_for_day();
//		b2ConfigData->guild_rejoin_wait_time_min = configData.guild_rejoin_wait_time_min();
//		b2ConfigData->character_level_max = configData.character_level_max();
//		b2ConfigData->match_continuous_win_count_for_bonus = configData.match_continuous_win_count_for_bonus();
//		b2ConfigData->hero_tower_free_ticket_count_max = configData.hero_tower_free_ticket_count_max();
//		b2ConfigData->counter_dungeon_daily_enter_count_max = configData.counter_dungeon_daily_enter_count_max();
//		b2ConfigData->max_chat_block_count = configData.max_chat_block_count();
//		b2ConfigData->min_floor_for_hero_tower_ranking = configData.min_floor_for_hero_tower_ranking();
//		b2ConfigData->show_review_inducement_popup = configData.show_review_inducement_popup();
//		b2ConfigData->raid_guild_bonus_count_member_2 = configData.raid_guild_bonus_count_member_2();
//		b2ConfigData->raid_guild_bonus_count_member_3 = configData.raid_guild_bonus_count_member_3();
//		b2ConfigData->raid_guild_bonus_count_member_4 = configData.raid_guild_bonus_count_member_4();
//		b2ConfigData->max_open_stage_difficulty = configData.max_open_stage_difficulty();
//		b2ConfigData->item_random_option_change_cost_cash = configData.item_random_option_change_cost_cash();
//		b2ConfigData->event_dice_free_count = configData.event_dice_free_count();
//		b2ConfigData->event_dice_point_per_one_dice = configData.event_dice_point_per_one_dice();
//		b2ConfigData->max_costume_inventory_slot_count = configData.max_costume_inventory_slot_count();
//		b2ConfigData->exchange_count_gold = configData.exchange_item_count_gold();
//		b2ConfigData->exchange_count_gem = configData.exchange_item_count_gem();
//		b2ConfigData->lobby_style = configData.lobby_style();
//		b2ConfigData->advanced_rank_level = configData.advanced_rank_level();
//
//		return b2ConfigData;
//	}
//
//
//	b2network::B2mdStageInfoPtr FB2MessageInfoConverter::mdStageInfoPb2B2mdStageInfo(const b2::protocol::masterdata::mdStageInfo& stageInfo)
//	{
//		auto b2StageInfo = std::make_shared<B2mdStageInfo>();
//
//		b2StageInfo->stage_id = stageInfo.stage_id();
//		b2StageInfo->difficulty = PbStageDifficultyToB2StageDifficulty(stageInfo.difficulty());
//		b2StageInfo->difficulty_scale = stageInfo.difficulty_scale();
//		b2StageInfo->map_id = stageInfo.map_id();
//		b2StageInfo->clear_time_grade_high = stageInfo.clear_time_grade_high();
//		b2StageInfo->clear_time_grade_medium = stageInfo.clear_time_grade_medium();
//		b2StageInfo->recommend_power = stageInfo.recommend_power();
//		b2StageInfo->recommend_attack = stageInfo.recommend_attack();
//		b2StageInfo->recommend_defense = stageInfo.recommend_defense();
//		b2StageInfo->gold_cost_exp_buff = stageInfo.gold_cost_exp_buff();
//		b2StageInfo->gold_cost_gold_buff = stageInfo.gold_cost_gold_buff();
//		b2StageInfo->gold_cost_auto_skill_buff = stageInfo.gold_cost_auto_skill_buff();
//		b2StageInfo->gold_cost_boost_buff = stageInfo.gold_cost_boost_buff();
//		b2StageInfo->aether_type = stageInfo.aether_type();
//		return b2StageInfo;
//	}
//
//	b2network::B2BattleFormationPtr FB2MessageInfoConverter::BattleFormationPb2B2BattleFormation(const b2::protocol::commondata::BattleFormation& formation)
//	{
//		auto b2BattleFormation = std::make_shared<B2BattleFormation>();
//
//		b2BattleFormation->formation_type = PbBattleFormationType2B2BattleFormation(formation.formation_type());
//		b2BattleFormation->formation_level = formation.formation_level();
//
//		return b2BattleFormation;
//	}
//
//	b2network::B2BattleFormationBuffPtr FB2MessageInfoConverter::ToB2BattleFormationBuff(const b2::protocol::commondata::BattleFormationBuff& buff)
//	{
//		auto b2FormationBuff = std::make_shared<B2BattleFormationBuff>();
//
//		b2FormationBuff->id = buff.id();
//		b2FormationBuff->buff_type1 = buff.buff_type1();
//		b2FormationBuff->buff_value1 = buff.buff_value1();
//		b2FormationBuff->buff_type2 = buff.buff_type2();
//		b2FormationBuff->buff_value2 = buff.buff_value2();
//
//		return b2FormationBuff;
//	}
//
//	b2network::B2TeamBattleEntryPtr FB2MessageInfoConverter::TeamBattleEntryPb2B2TeamBattleEntry(const b2::protocol::commondata::TeamBattleEntry& entry)
//	{
//		auto b2TeamBattleEntry = std::make_shared<B2TeamBattleEntry>();
//
//		b2TeamBattleEntry->formation_type = PbBattleFormationType2B2BattleFormation(entry.formation_type());
//		b2TeamBattleEntry->character_slot_1 = PbCharacter2B2CharacterType(entry.character_slot_1());
//		b2TeamBattleEntry->character_slot_2 = PbCharacter2B2CharacterType(entry.character_slot_2());
//		b2TeamBattleEntry->character_slot_3 = PbCharacter2B2CharacterType(entry.character_slot_3());
//
//		return b2TeamBattleEntry;
//	}
//
//	b2network::B2mdTeamBattleFormationInfoPtr FB2MessageInfoConverter::mdTeamBattleFormationInfoPb2B2mdTeamBattleFormationInfo(const b2::protocol::masterdata::mdTeamBattleFormationInfo& formationInfo)
//	{
//		auto b2FormationInfo = std::make_shared<B2mdTeamBattleFormationInfo>();
//
//		b2FormationInfo->formation_type = PbBattleFormationType2B2BattleFormation(formationInfo.formation_type());
//		b2FormationInfo->formation_level = formationInfo.formation_level();
//		b2FormationInfo->enhance_gold_cost = formationInfo.enhance_gold_cost();
//
//		for (auto buff : formationInfo.buffs()) 
//		{
//			b2FormationInfo->buffs.Emplace(ToB2BattleFormationBuff(buff));
//		}
//
//		return b2FormationInfo;
//	}
//
//	b2network::B2TeamPtr FB2MessageInfoConverter::TeamPb2B2Team(const b2::protocol::session::Team& team)
//	{
//		auto b2Team = std::make_shared<B2Team>();
//
//		b2Team->debuff_value = team.debuff_value();
//		b2Team->account = AccountInfoPb2B2FindAccount(team.account());
//		b2Team->formation = BattleFormationPb2B2BattleFormation(team.formation());
//		b2Team->entry = TeamBattleEntryPb2B2TeamBattleEntry(team.entry());
//		b2Team->match_status = MatchStatusPb2B2MatchStatus(team.match_status());
//
//		if (team.has_guild_name()) 
//		{
//			b2Team->guild_name = UTF8_TO_TCHAR(team.guild_name().c_str());
//			b2Team->mark_index = team.mark_index();
//			b2Team->mark_color = team.mark_color();
//			b2Team->mark_bg_index = team.mark_bg_color();
//			b2Team->mark_bg_color = team.mark_bg_color();
//		}
//		else 
//		{
//			b2Team->guild_name = "";
//			b2Team->mark_index = 0;
//			b2Team->mark_color = 0;
//			b2Team->mark_bg_index = 0;
//			b2Team->mark_bg_color = 0;
//		}
//
//		return b2Team;
//	}
//
//	b2network::B2MatcherPtr FB2MessageInfoConverter::MatcherPb2B2Matcher(const b2::protocol::session::Matcher& matcher)
//	{
//		auto b2Matcher = std::make_shared<B2Matcher>();
//
//		b2Matcher->debuff_value = matcher.debuff_value();
//		b2Matcher->account = AccountInfoPb2B2FindAccount(matcher.account());
//		b2Matcher->match_status = MatchStatusPb2B2MatchStatus(matcher.match_status());
//
//		if (matcher.has_guild_name())
//		{
//			b2Matcher->guild_name = UTF8_TO_TCHAR(matcher.guild_name().c_str());
//			b2Matcher->mark_index = matcher.mark_index();
//			b2Matcher->mark_color = matcher.mark_color();
//			b2Matcher->mark_bg_index = matcher.mark_bg_color();
//			b2Matcher->mark_bg_color = matcher.mark_bg_color();
//		}
//		else
//		{
//			b2Matcher->guild_name = "";
//			b2Matcher->mark_index = 0;
//			b2Matcher->mark_color = 0;
//			b2Matcher->mark_bg_index = 0;
//			b2Matcher->mark_bg_color = 0;
//		}
//
//		return b2Matcher;
//	}
//
//	b2network::B2MatchStatusPtr FB2MessageInfoConverter::MatchStatusPb2B2MatchStatus(const b2::protocol::commondata::MatchStatus& status)
//	{
//		auto b2MatchStatus = std::make_shared<B2MatchStatus>();
//
//		b2MatchStatus->battle_grade = status.battle_grade();
//		b2MatchStatus->battle_star_count = status.battle_star_count();
//		b2MatchStatus->continue_win = status.continue_win();
//		b2MatchStatus->continue_win_max = status.continue_win_max();
//
//		return b2MatchStatus;
//	}
//
//	b2network::B2mdRankInfoPtr FB2MessageInfoConverter::mdRankInfoPb2B2mdRankInfo(const b2::protocol::masterdata::mdRankInfo& mdRankInfo)
//	{
//		auto b2RankInfo = std::make_shared<B2mdRankInfo>();
//
//		b2RankInfo->rank = mdRankInfo.rank();
//		b2RankInfo->node_index = mdRankInfo.node_index();
//		b2RankInfo->open_level = mdRankInfo.open_level();
//		b2RankInfo->attribute_change = mdRankInfo.attribute_change();
//		b2RankInfo->change_value = mdRankInfo.change_value();
//		b2RankInfo->need_rank_promotion_point = mdRankInfo.need_rank_promotion_point();
//		b2RankInfo->need_activate_gold = mdRankInfo.need_activate_gold();
//		b2RankInfo->rate = mdRankInfo.rate();
//
//		return b2RankInfo;
//	}
//
//	b2network::B2RankingPtr FB2MessageInfoConverter::RankingPb2B2Ranking(const b2::protocol::commondata::Ranking& ranking)
//	{
//		auto b2Ranking = std::make_shared<B2Ranking>();
//
//		b2Ranking->account_id = ranking.account_id();
//		b2Ranking->ranking = ranking.ranking();
//		b2Ranking->total_ranker_count = ranking.total_ranker_count();
//
//		return b2Ranking;
//	}
//
//	b2network::B2MatchRankerPtr FB2MessageInfoConverter::MatchRankerPb2B2MatchRanker(const b2::protocol::commondata::MatchRanker& ranker)
//	{
//		auto b2MatchRanker = std::make_shared<B2MatchRanker>();
//
//		b2MatchRanker->account_id = ranker.account_id();
//		b2MatchRanker->nickname = UTF8_TO_TCHAR(ranker.nickname().c_str());
//		if (ranker.has_battle_grade())
//		{
//			b2MatchRanker->battle_grade = ranker.battle_grade();
//		}
//
//		if (ranker.has_battle_star_count())
//		{
//			b2MatchRanker->battle_star_count = ranker.battle_star_count();
//		}
//
//		if (ranker.has_best_floor())
//		{
//			b2MatchRanker->best_floor = ranker.best_floor();
//		}
//
//		if (ranker.has_clear_time())
//		{
//			b2MatchRanker->clear_time = ranker.clear_time();
//		}
//
//		b2MatchRanker->total_power = ranker.total_power();
//
//		if (ranker.has_point())
//		{
//			b2MatchRanker->point = ranker.point();
//		}
//
//		for (auto entry : ranker.entries()) 
//		{
//			B2MatchRankerEntry b2Entry;
//			b2Entry.character_type = PbCharacter2B2CharacterType(entry.character_type());
//			b2Entry.level = entry.level();
//			b2MatchRanker->entries.Emplace(b2Entry);
//		}
//
//        if (ranker.has_pvp_replay_url())
//        {
//            b2MatchRanker->pvp_replay_url = UTF8_TO_TCHAR(ranker.pvp_replay_url().c_str());
//
//            if (ranker.has_replay_size_compressed())
//            {
//                b2MatchRanker->replay_size_compressed = ranker.replay_size_compressed();
//            }
//            else
//            {
//                b2MatchRanker->replay_size_compressed = 0;
//            }
//
//            if (ranker.has_replay_size_uncompressed())
//            {
//                b2MatchRanker->replay_size_uncompressed = ranker.replay_size_uncompressed();
//            }
//            else
//            {
//                b2MatchRanker->replay_size_uncompressed = 0;
//            }
//        }
//
//		return b2MatchRanker;
//	}
//
//	b2network::B2RankingRewardPtr FB2MessageInfoConverter::RankingRewardPb2B2RankingReward(const b2::protocol::commondata::RankingReward& reward)
//	{
//		auto b2RankingReward = std::make_shared<B2RankingReward>();
//
//		b2RankingReward->grade = reward.grade();
//		b2RankingReward->star_count = reward.star_count();
//		b2RankingReward->ranking = reward.ranking();
//		b2RankingReward->ranker_count = reward.ranker_count();
//		b2RankingReward->initial_grade = reward.initial_grade();
//
//		for (b2::protocol::commondata::Reward pbReward : reward.rewards())
//		{
//			b2RankingReward->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2RankingReward;
//	}
//
//	b2network::B2DimensionRankingRewardPtr FB2MessageInfoConverter::ToPb2B2DimensionRankingReward(const b2::protocol::commondata::DimensionRankingReward& reward)
//	{
//		auto b2RankingReward = std::make_shared<B2DimensionRankingReward>();
//
//		b2RankingReward->best_difficulty_level = reward.best_difficulty_level();
//		b2RankingReward->best_clear_time_millis = reward.best_clear_time_millis();
//		b2RankingReward->ranking = reward.ranking();
//		b2RankingReward->ranker_count = reward.ranker_count();
//
//		for (b2::protocol::commondata::Reward pbReward : reward.rewards())
//		{
//			b2RankingReward->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2RankingReward;
//	}
//
//	b2network::B2mdDailyMatchRewardPtr FB2MessageInfoConverter::mdDailyMatchRewardPb2B2mdDailyMatchReward(const b2::protocol::masterdata::mdDailyMatchReward& reward)
//	{
//		auto b2mdDailyMatchReward = std::make_shared<B2mdDailyMatchReward>();
//
//		b2mdDailyMatchReward->match_type = PbMatchInfoType2B2MatchInfoType(reward.match_type());
//		b2mdDailyMatchReward->require_count = reward.require_count();
//		b2mdDailyMatchReward->reward_gold = reward.reward_gold();
//		b2mdDailyMatchReward->reward_bladepoint = reward.reward_bladepoint();
//		b2mdDailyMatchReward->reward_gem = reward.reward_gem();
//		b2mdDailyMatchReward->reward_item_template_id = reward.reward_item_template_id();
//
//		return b2mdDailyMatchReward;
//	}
//
//	b2network::B2mdMatchRankingRewardPtr FB2MessageInfoConverter::mdMatchRankingRewardPb2B2mdMatchRankingReward(const b2::protocol::masterdata::mdMatchRankingReward& reward)
//	{
//		auto b2mdMatchRankingReward = std::make_shared<B2mdMatchRankingReward>();
//
//		b2mdMatchRankingReward->grade = reward.grade();
//		b2mdMatchRankingReward->ranking_min = reward.ranking_min();
//		b2mdMatchRankingReward->ranking_max = reward.ranking_max();
//
//		for (auto pbReward : reward.rewards())
//		{
//			b2mdMatchRankingReward->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2mdMatchRankingReward;
//	}
//
//	b2network::B2HallOfFamePtr FB2MessageInfoConverter::ToB2HallOfFame(const b2::protocol::commondata::HallOfFame& pb)
//	{
//		auto b2 = std::make_shared<B2HallOfFame>();
//
//		b2->mode = ToB2ContentsMode(pb.mode());
//		b2->ranking = pb.ranking();
//		b2->account_id = pb.account_id();
//		b2->praised_count = pb.praised_count();
//		b2->match_grade = pb.match_grade();
//		b2->match_start_count = pb.match_start_count();
//		b2->mvp_count = pb.mvp_count();
//
//		return b2;
//	}
//
//	b2network::B2HallOfFameUserPtr FB2MessageInfoConverter::ToB2HallOfFameUser(const b2::protocol::commondata::HallOfFameUser& pb)
//	{
//		auto b2 = std::make_shared<B2HallOfFameUser>();
//
//		b2->user = UserPb2B2User(pb.user());
//
//		for (auto item : pb.equip_items())
//		{
//			b2->equip_items.Emplace(ItemPb2B2ItemServerInfo(item));
//		}
//
//		for (auto costume : pb.equip_costumes())
//		{
//			b2->equip_costumes.Emplace(CostumePb2B2CostumeServerInfo(costume));
//		}
//
//		for (auto totem : pb.equip_totems())
//		{
//			b2->equip_totems.Emplace(TotemPb2B2TotemServerInfo(totem));
//		}
//
//		b2->wing_grade = pb.has_wing_grade() ? pb.wing_grade() : 1;
//		b2->wing_level = pb.has_wing_level() ? pb.wing_level() : 0;
//		b2->wing_visible = pb.has_wing_visible() ? pb.wing_visible() : false;
//		b2->helmet_visible = pb.has_helmet_visible() ? pb.helmet_visible() : false;
//		b2->costume_weapon_visible = pb.has_costume_weapon_visible() ? pb.costume_weapon_visible() : false;
//		b2->costume_hat_visible = pb.has_costume_hat_visible() ? pb.costume_hat_visible() : false;
//		b2->costume_armor_visible = pb.has_costume_armor_visible() ? pb.costume_armor_visible() : false;
//
//		return b2;
//	}
//
//	b2network::B2HallOfFameRewardPtr FB2MessageInfoConverter::ToB2HallOfFameReward(const b2::protocol::commondata::HallOfFameReward& pb)
//	{
//		auto b2 = std::make_shared<B2HallOfFameReward>();
//
//		b2->mode = ToB2ContentsMode(pb.mode());
//		b2->ranking = pb.ranking();
//
//		for (auto reward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(reward));
//		}
//
//		return b2;
//	}
//
//	b2network::B2HallOfFamePraisePtr FB2MessageInfoConverter::ToB2HallOfFamePraise(const b2::protocol::commondata::HallOfFamePraise& pb)
//	{
//		auto b2 = std::make_shared<B2HallOfFamePraise>();
//
//		b2->mode = ToB2ContentsMode(pb.mode());
//		b2->praise_count = pb.praise_count();
//		b2->max_count = pb.max_count();
//		b2->target_account_id = pb.target_account_id();
//
//		return b2;
//	}
//
//	b2network::B2mdTutorialInfoPtr FB2MessageInfoConverter::mdTutorialInfoPb2B2mdTutorialInfo(const b2::protocol::masterdata::mdTutorialInfo& info)
//	{
//		auto b2mdTutorialInfo = std::make_shared<B2mdTutorialInfo>();
//
//		b2mdTutorialInfo->tutorial_id = info.tutorial_id();
//		b2mdTutorialInfo->prev_tutorial_id = info.prev_tutorial_id();
//		b2mdTutorialInfo->character_level = info.character_level();
//		b2mdTutorialInfo->act_cleared = info.act_cleared();
//		b2mdTutorialInfo->cash = info.cash();
//		b2mdTutorialInfo->gold = info.gold();
//		b2mdTutorialInfo->reward_id1 = info.reward_id1();
//		b2mdTutorialInfo->reward_count1 = info.reward_count1();
//		b2mdTutorialInfo->reward_id2 = info.reward_id2();
//		b2mdTutorialInfo->reward_count2 = info.reward_count2();
//		b2mdTutorialInfo->reward_id3 = info.reward_id3();
//		b2mdTutorialInfo->reward_count3 = info.reward_count3();
//
//		return b2mdTutorialInfo;
//	}
//
//	b2network::B2CharacterSummaryPtr FB2MessageInfoConverter::CharacterSummaryPb2B2CharacterSummary(const b2::protocol::commondata::CharacterSummary& character)
//	{
//		auto b2CharacterSummary = std::make_shared<B2CharacterSummary>();
//
//		b2CharacterSummary->account_id = character.account_id();
//		b2CharacterSummary->account_name = UTF8_TO_TCHAR(character.account_name().c_str());
//		b2CharacterSummary->character_type = PbCharacter2B2CharacterType(character.character_type());
//		b2CharacterSummary->character_level = character.character_level();
//		b2CharacterSummary->power = character.power();
//		b2CharacterSummary->is_online = character.is_online();
//
//		return b2CharacterSummary;
//	}
//
//	b2network::B2UserPtr FB2MessageInfoConverter::UserPb2B2User(const b2::protocol::commondata::User& user)
//	{
//		auto b2User = std::make_shared<B2User>();
//
//		b2User->character_summary = CharacterSummaryPb2B2CharacterSummary(user.character_summary());
//		b2User->last_logout_time = user.last_logout_time();
//		
//		if (user.has_guild_name() && !user.guild_name().empty())
//		{
//			b2User->guild_name = UTF8_TO_TCHAR(user.guild_name().c_str());
//			b2User->mark_index = user.mark_index();
//			b2User->mark_color = user.mark_color();
//			b2User->mark_bg_index = user.mark_bg_index();
//			b2User->mark_bg_color = user.mark_bg_color();
//		}
//		else
//		{
//			b2User->guild_name.Empty();
//			b2User->mark_index = 0;
//			b2User->mark_color = 0;
//			b2User->mark_bg_index = 0;
//			b2User->mark_bg_color = 0;
//		}
//
//		return b2User;
//	}
//
//	b2network::B2FriendPtr FB2MessageInfoConverter::FriendPb2B2Friend(const b2::protocol::commondata::Friend& friendPb)
//	{
//		auto b2Friend = std::make_shared<B2Friend>();
//
//		b2Friend->user = UserPb2B2User(friendPb.user());
//		b2Friend->last_social_point_send_time = friendPb.last_social_point_send_time();
//		if (friendPb.has_is_new()) {
//			b2Friend->is_new = friendPb.is_new();
//		}
//		else
//		{
//			b2Friend->is_new = false;
//		}
//		if (friendPb.has_raid_penalty_remove_time())
//		{
//			b2Friend->raid_penalty_remove_time = friendPb.raid_penalty_remove_time();
//		}
//		else
//		{
//			b2Friend->raid_penalty_remove_time = 0;
//		}
//
//		return b2Friend;
//	}
//
//	B2QuestInfoPtr FB2MessageInfoConverter::QuestInfoPb2B2QuestInfo(const b2::protocol::commondata::QuestInfo& questInfo)
//	{
//		auto b2QuestInfo = std::make_shared<B2QuestInfo>();
//
//		b2QuestInfo->slot = PbQuestSlotTypeToB2QuestSlotType(questInfo.slot());		
//		b2QuestInfo->type = questInfo.type();
//		b2QuestInfo->quest_id = questInfo.quest_id();
//		b2QuestInfo->progress_count = questInfo.progress_count();
//		b2QuestInfo->state = questInfo.state();
//
//		return b2QuestInfo;
//	}
//
//	B2mdMainQuestInfoPtr FB2MessageInfoConverter::mdMainQuestInfoPb2B2mdMainQuestInfo(const b2::protocol::masterdata::mdMainQuestInfo& mainQuestInfo)
//	{
//		auto b2QuestInfo = std::make_shared<B2mdMainQuestInfo>();
//
//		b2QuestInfo->id = mainQuestInfo.id();
//		b2QuestInfo->main_quest_type = mainQuestInfo.main_quest_type();
//		b2QuestInfo->act_id = mainQuestInfo.act_id();
//		b2QuestInfo->difficulty = mainQuestInfo.difficulty();
//		b2QuestInfo->event_type = mainQuestInfo.event_type();
//		b2QuestInfo->main_view_condition = mainQuestInfo.main_view_condition();
//		b2QuestInfo->condition_1 = mainQuestInfo.condition_1();
//		b2QuestInfo->condition_2 = mainQuestInfo.condition_2();
//		b2QuestInfo->condition_3 = mainQuestInfo.condition_3();
//		b2QuestInfo->condition_4 = mainQuestInfo.condition_4();
//		b2QuestInfo->goal_count = mainQuestInfo.goal_count();
//
//		for (auto pbReward : mainQuestInfo.rewards())
//		{
//			b2QuestInfo->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2QuestInfo;
//	}
//
//	B2mdDailyQuestInfoPtr FB2MessageInfoConverter::mdDailyQuestInfoPb2B2mdDailyQuestInfo(const b2::protocol::masterdata::mdDailyQuestInfo& dailyQuestInfo)
//	{
//		auto b2QuestInfo = std::make_shared<B2mdDailyQuestInfo>();
//
//		b2QuestInfo->id = dailyQuestInfo.id();
//		b2QuestInfo->event_type = dailyQuestInfo.event_type();
//		b2QuestInfo->condition_1 = dailyQuestInfo.condition_1();
//		b2QuestInfo->condition_2 = dailyQuestInfo.condition_2();
//		b2QuestInfo->condition_3 = dailyQuestInfo.condition_3();
//		b2QuestInfo->condition_4 = dailyQuestInfo.condition_4();
//		b2QuestInfo->goal_count = dailyQuestInfo.goal_count();
//
//		for (auto pbReward : dailyQuestInfo.rewards())
//		{
//			b2QuestInfo->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2QuestInfo;
//	}
//
//	B2mdRewardInfoPtr FB2MessageInfoConverter::mdRewardInfoPb2B2mdRewardnfo(const b2::protocol::masterdata::mdRewardInfo& rewardInfo)
//	{
//		auto b2mdRewardInfo = std::make_shared<B2mdRewardInfo>();
//
//		b2mdRewardInfo->id = rewardInfo.id();
//		b2mdRewardInfo->reward_type = PbRewardTypeToB2RewardType(rewardInfo.reward_type());
//		b2mdRewardInfo->reward_id = rewardInfo.reward_id();
//		b2mdRewardInfo->reward_push_type = rewardInfo.reward_push_type();
//
//		return b2mdRewardInfo;
//	}
//
//	b2network::B2mdCounterDungeonPtr FB2MessageInfoConverter::ToB2mdCounterDungeon(const b2::protocol::masterdata::mdCounterDungeon& pb)
//	{
//		auto b2 = std::make_shared<B2mdCounterDungeon>();
//
//		b2->dungeon_id = pb.dungeon_id();
//		b2->dungeon_type = PbCounterDungeonType2B2CounterDungeonType(pb.dungeon_type());
//		b2->difficulty = PbCounterDungeonDifficulty2B2CounterDungeonDifficulty(pb.difficulty());
//		b2->difficulty_level = pb.difficulty_level();
//		b2->difficulty_scale = pb.difficulty_scale();
//		b2->recommend_power = pb.recommend_power();
//		b2->recommend_attack = pb.recommend_attack();
//		b2->recommend_defense = pb.recommend_defense();
//		b2->drop_gold_min = pb.drop_gold_min();
//		b2->drop_gold_max = pb.drop_gold_max();
//
//		for (auto pbReward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2RewardRandomCount(pbReward));
//		}
//		return b2;
//	}
//
//	b2network::B2mdCounterDungeonPeriodPtr FB2MessageInfoConverter::ToB2mdCounterDungeonPeriod(const b2::protocol::masterdata::mdCounterDungeonPeriod& pb)
//	{
//		auto b2 = std::make_shared<B2mdCounterDungeonPeriod>();
//
//		b2->dungeon_type = PbCounterDungeonType2B2CounterDungeonType(pb.dungeon_type());
//		b2->day_of_week = PbDayOfWeek2B2DayOfWeek(pb.day_of_week());
//		b2->begin_hour = pb.begin_hour();
//		b2->begin_min = pb.begin_min();
//		b2->end_hour = pb.end_hour();
//		b2->end_min = pb.end_min();
//
//		return b2;
//	}
//
//	b2network::B2mdCounterDungeonWeekRewardPtr FB2MessageInfoConverter::ToB2mdCounterDungeonWeekReward(const b2::protocol::masterdata::mdCounterDungeonWeekReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdCounterDungeonWeekReward>();
//
//		b2->index = pb.index();
//		b2->require_count = pb.require_count();
//
//		for (auto pbReward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdDimensionInfoPtr FB2MessageInfoConverter::ToB2mdDimensionInfo(const b2::protocol::masterdata::mdDimensionInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdDimensionInfo>();
//
//		b2->difficulty_level = pb.difficulty_level();
//		b2->difficulty_scale = pb.difficulty_scale();
//		b2->fixed_drop_item_id1 = pb.fixed_drop_item_id1();
//		b2->fixed_drop_count1 = pb.fixed_drop_count1();
//		b2->drop_totem_count_min = pb.drop_totem_count_min();
//		b2->drop_totem_count_max = pb.drop_totem_count_max();
//		b2->recommend_power = pb.recommend_power();
//		b2->recommend_attack = pb.recommend_attack();
//		b2->recommend_defense = pb.recommend_defense();
//
//		return b2;
//	}
//
//	b2network::B2mdDimensionWeekRewardPtr FB2MessageInfoConverter::ToB2mdDimensionWeekReward(const b2::protocol::masterdata::mdDimensionWeekReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdDimensionWeekReward>();
//
//		b2->index = pb.index();
//		b2->ranking_min = pb.ranking_min();
//		b2->ranking_max = pb.ranking_max();
//
//		for (auto pbReward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2RawReward(pbReward));
//		}
//
//		return b2;
//	}
//
//	b2network::B2GuildPtr FB2MessageInfoConverter::ToB2Guild(const b2::protocol::commondata::Guild& guild)
//	{
//		auto b2 = std::make_shared<B2Guild>();
//
//		b2->guild_id = guild.guild_id();
//		b2->guild_name = UTF8_TO_TCHAR(guild.guild_name().c_str());
//		b2->master_account_id = guild.master_account_id();
//		b2->master_name = UTF8_TO_TCHAR(guild.master_name().c_str());
//		b2->max_member_count = guild.max_member_count();
//		b2->notice = UTF8_TO_TCHAR(guild.notice().c_str());
//		b2->level = guild.level();
//		b2->exp = guild.exp();
//		b2->join_option = ToB2GuildJoinOption(guild.join_option());
//		b2->mark_index = guild.mark_index();
//		b2->mark_color = guild.mark_color();
//		b2->mark_bg_index = guild.mark_bg_index();
//		b2->mark_bg_color = guild.mark_bg_color();
//		b2->cur_member_count = guild.cur_member_count();
//		b2->create_time = guild.create_time();
//		b2->season_obtain_medal = guild.season_obtain_medal();
//		b2->season_ranking = guild.season_ranking();
//		b2->season_ranking_total = guild.season_ranking_total();
//		b2->members_avg_power = guild.members_avg_power();
//		b2->total_result = guild.total_result();
//		b2->master_elected_time = (guild.has_master_elected_time())? guild.master_elected_time() : 0;
//
//		if (guild.has_join_guild_battle()) {
//			b2->join_guild_battle = guild.join_guild_battle();
//		}
//		else {
//			b2->join_guild_battle = false;
//		}
//
//		if (guild.has_turn_obtain_medal()) {
//			b2->turn_obtain_medal = guild.turn_obtain_medal();
//		}
//		else {
//			b2->turn_obtain_medal = 0;
//		}
//
//		return b2;
//	}
//
//	b2network::B2GuildMemberPtr FB2MessageInfoConverter::ToB2GuildMember(const b2::protocol::commondata::GuildMember& guildMember)
//	{
//		auto b2 = std::make_shared<B2GuildMember>();
//
//		b2->guild_id = guildMember.guild_id();
//		b2->contribution = guildMember.contribution();
//		b2->rank = ToB2GuildMemberRank(guildMember.rank());
//		b2->character_summary = CharacterSummaryPb2B2CharacterSummary(guildMember.character_summary());
//		b2->last_logout_time = guildMember.last_logout_time();
//		b2->total_support_count = guildMember.total_support_count();
//		b2->today_support_count = guildMember.today_support_count();
//		b2->total_donation_count = guildMember.total_donation_count();
//		b2->today_donation_count = guildMember.today_donation_count();
//		b2->raid_penalty_remove_time = guildMember.raid_penalty_remove_time();
//
//		return b2;
//	}
//
//	b2network::B2GuildSkillPtr FB2MessageInfoConverter::ToB2GuildSkill(const b2::protocol::commondata::GuildSkill& guildSkill)
//	{
//		auto b2 = std::make_shared<B2GuildSkill>();
//
//		b2->id = guildSkill.id();
//		b2->level = guildSkill.level();
//		b2->enabled = guildSkill.enabled();
//		b2->buff_end_time = guildSkill.buff_end_time();
//
//		return b2;
//	}
//
//	b2network::B2MissionInfoPtr FB2MessageInfoConverter::ToB2MissionInfo(const b2::protocol::commondata::MissionInfo& missionInfo)
//	{
//		auto b2 = std::make_shared<B2MissionInfo>();
//
//		b2->mission_type = PbMissionTypeToB2MissionType(missionInfo.mission_type());
//		b2->mission_state_type = PbMissionStateTypeToMissionStateType(missionInfo.mission_state_type());
//		b2->mission_id = missionInfo.mission_id();
//		b2->progress_count = missionInfo.progress_count();
//
//		if (missionInfo.has_completed_step())
//		{
//			b2->completed_step = missionInfo.completed_step();
//		}
//
//		if (missionInfo.has_rewarded_step())
//		{
//			b2->rewarded_step = missionInfo.rewarded_step();
//		}
//
//		return b2;
//	}
//
//	b2network::B2MissionCompletePointInfoPtr FB2MessageInfoConverter::ToB2MissionCompletePointInfo(const b2::protocol::commondata::MissionCompletePointInfo& missionCompletePointInfo)
//	{
//		auto b2 = std::make_shared<B2MissionCompletePointInfo>();
//
//		b2->mission_type = PbMissionTypeToB2MissionType(missionCompletePointInfo.mission_type());
//		b2->completePoint = missionCompletePointInfo.completepoint();
//
//		for (auto rewardIndex : missionCompletePointInfo.reward_indexs())
//		{
//			b2->reward_indexs.Add(rewardIndex);
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdMissionInfoPtr FB2MessageInfoConverter::ToB2mdMissionInfo(const b2::protocol::masterdata::mdMissionInfo& missionInfo)
//	{
//		auto b2 = std::make_shared<B2mdMissionInfo>();
//
//		b2->mission_id = missionInfo.mission_id();
//		b2->mission_type = PbMissionTypeToB2MissionType(missionInfo.mission_type());
//		b2->event_type = missionInfo.event_type();
//		b2->condition1 = missionInfo.condition1();
//		b2->condition2 = missionInfo.condition2();
//		b2->condition3 = missionInfo.condition3();
//		b2->condition4 = missionInfo.condition4();
//		b2->goal_count = missionInfo.goal_count();
//		b2->complete_point = missionInfo.complete_point();
//		b2->reward_id = missionInfo.reward_id();
//		b2->reward_count = missionInfo.reward_count();
//
//		return b2;
//	}
//
//	b2network::B2mdSerialMissionInfoPtr FB2MessageInfoConverter::ToB2mdSerialMissionInfo(const b2::protocol::masterdata::mdSerialMissionInfo& serialMissionInfo)
//	{
//		auto b2 = std::make_shared<B2mdSerialMissionInfo>();
//
//		b2->mission_id = serialMissionInfo.mission_id();
//		b2->mission_step = serialMissionInfo.mission_step();
//		b2->event_type = serialMissionInfo.event_type();
//		b2->condition1 = serialMissionInfo.condition1();
//		b2->condition2 = serialMissionInfo.condition2();
//		b2->condition3 = serialMissionInfo.condition3();
//		b2->condition4 = serialMissionInfo.condition4();
//		b2->goal_count = serialMissionInfo.goal_count();		
//		b2->reward_id = serialMissionInfo.reward_id();
//		b2->reward_count = serialMissionInfo.reward_count();
//
//		return b2;
//	}
//
//	b2network::B2mdMissionCompletePointInfoPtr FB2MessageInfoConverter::ToB2mdMissionCompletePointInfo(const b2::protocol::masterdata::mdMissionCompletePointInfo& completePointInfo)
//	{
//		auto b2 = std::make_shared<B2mdMissionCompletePointInfo>();
//
//		b2->mission_type = PbMissionTypeToB2MissionType(completePointInfo.mission_type());
//		b2->index = completePointInfo.index();
//		b2->require_point = completePointInfo.require_point();
//
//		for (auto pbReward : completePointInfo.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2;
//	}
//
//	B2WingOptionInfoPtr FB2MessageInfoConverter::ToB2WingOptionInfo(const b2::protocol::commondata::WingOption& wingOptionInfo)
//	{
//		auto b2 = std::make_shared<B2WingOptionInfo>();
//
//		b2->character_type = wingOptionInfo.character_type();
//		b2->option_index = wingOptionInfo.option_index();
//		b2->level = wingOptionInfo.level();
//
//		return b2;
//	}
//
//	b2network::B2mdWingEnhancePtr FB2MessageInfoConverter::ToB2mdWingEnhance(const b2::protocol::masterdata::mdWingEnhance& mdWingEnhance)
//	{
//		auto b2 = std::make_shared<B2mdWingEnhance>();
//
//		b2->grade = mdWingEnhance.grade();
//		b2->level = mdWingEnhance.level();
//		b2->need_item_template_id = mdWingEnhance.need_item_template_id();
//		b2->need_item_count = mdWingEnhance.need_item_count();
//		b2->enhance_point = mdWingEnhance.enhance_point();
//		b2->need_enhance_point = mdWingEnhance.need_enhance_point();
//		b2->gold_cost = mdWingEnhance.gold_cost();
//		b2->option_type1 = mdWingEnhance.option_type1();
//		b2->option_value1 = mdWingEnhance.option_value1();
//		b2->option_type2 = mdWingEnhance.option_type2();
//		b2->option_value2 = mdWingEnhance.option_value2();
//		b2->option_type3 = mdWingEnhance.option_type3();
//		b2->option_value3 = mdWingEnhance.option_value3();
//
//		return b2;
//	}
//
//	b2network::B2mdWingUpgradePtr FB2MessageInfoConverter::ToB2mdWingUpgrade(const b2::protocol::masterdata::mdWingUpgrade& mdWingUpgrade)
//	{
//		auto b2 = std::make_shared<B2mdWingUpgrade>();
//
//		b2->grade = mdWingUpgrade.grade();
//		b2->need_item_template_id = mdWingUpgrade.need_item_template_id();
//		b2->need_item_count = mdWingUpgrade.need_item_count();
//		b2->rate = mdWingUpgrade.rate();
//		b2->gold_cost = mdWingUpgrade.gold_cost();
//		b2->fail_point_max = mdWingUpgrade.fail_point_max();
//		b2->option_type1 = mdWingUpgrade.option_type1();
//		b2->option_value1 = mdWingUpgrade.option_value1();
//		b2->option_type2 = mdWingUpgrade.option_type2();
//		b2->option_value2 = mdWingUpgrade.option_value2();
//		b2->option_type3 = mdWingUpgrade.option_type3();
//		b2->option_value3 = mdWingUpgrade.option_value3();
//
//		return b2;
//	}
//
//	B2mdWingOptionValuePtr FB2MessageInfoConverter::ToB2mdWingOptionValue(const b2::protocol::masterdata::mdWingOptionValue& mdWingOptionValue)
//	{
//		auto b2 = std::make_shared<B2mdWingOptionValue>();
//
//		b2->character_type = mdWingOptionValue.character_type();
//		b2->option_index = mdWingOptionValue.option_index();
//		b2->level = mdWingOptionValue.level();
//		b2->option_type = mdWingOptionValue.option_type();
//		b2->option_value = mdWingOptionValue.option_value();
//		b2->need_item_template_id = mdWingOptionValue.need_item_template_id();
//		b2->need_item_count = mdWingOptionValue.need_item_count();
//
//		return b2;
//	}
//
//	B2mdWingOptionOpenPtr FB2MessageInfoConverter::ToB2mdWingOptionOpen(const b2::protocol::masterdata::mdWingOptionOpen& mdWingOptionOpen)
//	{
//		auto b2 = std::make_shared<B2mdWingOptionOpen>();
//
//		b2->character_type = mdWingOptionOpen.character_type();
//		b2->option_index = mdWingOptionOpen.option_index();
//		b2->need_grade = mdWingOptionOpen.need_grade();
//
//		return b2;
//	}
//
//	B2mdWingOptionMaxLevelPtr FB2MessageInfoConverter::ToB2mdWingOptionMaxLevel(const b2::protocol::masterdata::mdWingOptionMaxLevel& mdWingOptionMaxLevel)
//	{
//		auto b2 = std::make_shared<B2mdWingOptionMaxLevel>();
//
//		b2->character_type = mdWingOptionMaxLevel.character_type();
//		b2->option_index = mdWingOptionMaxLevel.option_index();
//		b2->wing_grade = mdWingOptionMaxLevel.wing_grade();
//		b2->max_level = mdWingOptionMaxLevel.max_level();
//
//		return b2;
//	}
//
//	B2FairyStatusPtr FB2MessageInfoConverter::ToB2FairyStatus(const b2::protocol::commondata::FairyStatus& fairyStatus)
//	{
//		auto b2 = std::make_shared<B2FairyStatus>();
//
//		b2->fairy_type = ToB2FairyType(fairyStatus.fairy_type());
//		b2->level = fairyStatus.level();
//		b2->enhance_point = fairyStatus.enhance_point();
//		b2->activated_bless_option_id = fairyStatus.activated_bless_option_id();
//
//		return b2;
//	}
//
//	b2network::B2mdHeroTowerFloorPtr FB2MessageInfoConverter::ToB2mdHeroTowerFloor(const b2::protocol::masterdata::mdHeroTowerFloor& pb)
//	{
//		auto b2 = std::make_shared<B2mdHeroTowerFloor>();
//
//		b2->floor = pb.floor();
//		b2->recommend_power = pb.recommend_power();
//		b2->recommend_attack = pb.recommend_attack();
//		b2->recommend_defense = pb.recommend_defense();
//		b2->clear_limit_time = pb.clear_limit_time();
//		b2->map_id = pb.map_id();
//		b2->difficulty_scale = pb.difficulty_scale();
//		b2->monster_level = pb.monster_level();
//
//		for (auto pbReward : pb.first_rewards())
//		{
//			b2->first_rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		for (auto pbReward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(pbReward));
//		}
//		return b2;
//	}
//
//	b2network::B2mdRelicInfoPtr FB2MessageInfoConverter::ToB2mdRelicInfo(const b2::protocol::masterdata::mdRelicInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdRelicInfo>();
//
//		b2->relic_id = pb.relic_id();
//		b2->open_stage_id = pb.open_stage_id();
//
//		return b2;
//	}
//
//	b2network::B2mdRelicGradeInfoPtr FB2MessageInfoConverter::ToB2mdRelicGradeInfo(const b2::protocol::masterdata::mdRelicGradeInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdRelicGradeInfo>();
//
//		b2->relic_id = pb.relic_id();
//		b2->grade = pb.grade();
//		b2->option_id_1 = pb.option_id_1();
//		b2->option_value_1 = pb.option_value_1();
//		b2->option_id_2 = pb.option_id_2();
//		b2->option_value_2 = pb.option_value_2();
//		b2->option_id_3 = pb.option_id_3();
//		b2->option_value_3 = pb.option_value_3();
//		b2->promotion_rate = pb.promotion_rate();
//		b2->promotion_gold = pb.promotion_gold();
//		b2->promotion_boss_piece = pb.promotion_boss_piece();
//		b2->boss_piece_id = pb.boss_piece_id();
//
//		return b2;
//	}
//
//	b2network::B2mdRelicLevelInfoPtr FB2MessageInfoConverter::ToB2mdRelicLevelInfo(const b2::protocol::masterdata::mdRelicLevelInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdRelicLevelInfo>();
//
//		b2->relic_id = pb.relic_id();
//		b2->enhance_level = pb.enhance_level();
//		b2->mode_option_id_1 = pb.mode_option_id_1();
//		b2->mode_option_value_1 = pb.mode_option_value_1();
//		b2->mode_option_id_2 = pb.mode_option_id_2();
//		b2->mode_option_value_2 = pb.mode_option_value_2();
//		b2->mode_option_id_3 = pb.mode_option_id_3();
//		b2->mode_option_value_3 = pb.mode_option_value_3();
//		b2->enhance_rate = pb.enhance_rate();
//		b2->need_gold = pb.need_gold();
//		b2->need_hero_piece = pb.need_hero_piece();
//
//		return b2;
//	}
//
//	b2network::B2RelicPtr FB2MessageInfoConverter::ToB2Relic(const b2::protocol::commondata::Relic& relic)
//	{
//		auto b2 = std::make_shared<B2Relic>();
//
//		b2->character_type = relic.character_type();
//		b2->relic_id = relic.relic_id();
//		b2->grade = relic.grade();
//		b2->enhance_level = relic.enhance_level();
//
//		return b2;
//	}
//
//	b2network::B2mdShopLotteryPricePtr FB2MessageInfoConverter::ToB2mdShopLotteryPrice(const b2::protocol::masterdata::mdShopLotteryPrice& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopLotteryPrice>();
//
//		b2->is_continuous = pb.continuous();
//		b2->is_again = pb.is_again();
//		b2->gem = pb.gem();
//		b2->calculated_gem = pb.calculated_gem();
//		b2->social_point = pb.social_point();
//
//		return b2;
//	}
//
//	b2network::B2mdShopLotteryPossibleGradePtr FB2MessageInfoConverter::ToB2mdShopLotteryPossibleGrade(const b2::protocol::masterdata::mdShopLotteryPossibleGrade& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopLotteryPossibleGrade>();
//
//		b2->draw_price_type = PbShopDrawPriceTypeToB2ShopDrawPriceType(pb.draw_price_type());
//		b2->is_continuous = pb.is_continuous();
//		b2->min_grade = pb.min_grade();
//		b2->max_grade = pb.max_grade();
//
//		return b2;
//	}
//
//	B2mdShopCashBonusPtr FB2MessageInfoConverter::ToB2mdShopCashBonus(const b2::protocol::masterdata::mdShopCashBonus& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopCashBonus>();
//
//		b2->id = pb.id();
//		b2->increase_rate = pb.increase_rate();
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//
//		return b2;
//	}
//
//	B2mdShopFundsSalePtr FB2MessageInfoConverter::ToB2mdShopFundsSale(const b2::protocol::masterdata::mdShopFundsSale& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopFundsSale>();
//
//		b2->id = pb.id();
//		b2->discount_rate = pb.discount_rate();
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//
//		return b2;
//	}
//
//	B2MagicShopPtr FB2MessageInfoConverter::ToB2MagicShop(const b2::protocol::commondata::MagicShop& pb)
//	{
//		auto b2 = std::make_shared<B2MagicShop>();
//
//		b2->id = pb.id();
//		b2->is_purchased = pb.is_purchased();
//
//		return b2;
//	}
//
//	B2ShopLotteryPtr FB2MessageInfoConverter::ToB2ShopLottery(const b2::protocol::commondata::ShopLottery& pb)
//	{
//		auto b2 = std::make_shared<B2ShopLottery>();
//
//		b2->draw_price_type = PbShopDrawPriceTypeToB2ShopDrawPriceType(pb.draw_price_type());
//		b2->is_continuous = pb.is_continuous();
//		b2->next_free_time = pb.next_free_time();
//		b2->lottery_count = pb.lottery_count();
//
//		return b2;
//	}
//
//	b2network::B2mdItemInfoPtr FB2MessageInfoConverter::ToB2mdItemInfo(const b2::protocol::masterdata::mdItemInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdItemInfo>();
//
//		b2->md_template_id = pb.md_template_id();
//		b2->md_group_id = ToB2GroupPrefix(pb.md_group_id());
//		b2->md_set_id = pb.md_set_id();
//		b2->md_type = PbItem2B2ItemType(pb.md_type()); 
//		b2->md_grade = pb.md_grade();
//		b2->md_inventory_type = PbInventory2B2InventoryType(pb.md_inventory_type());
//		b2->md_equip_position = PbEquipPosition2B2EquipPositionType(pb.md_equip_position());
//		b2->md_max_levelup = pb.md_max_levelup();
//		b2->md_main_attribute = pb.md_main_attribute();
//		b2->md_level_factor = pb.md_level_factor();
//		b2->md_unique_option_id1 = pb.md_unique_option_id1();
//		b2->md_unique_option_value1 = pb.md_unique_option_value1();
//		b2->md_unique_option_id2 = pb.md_unique_option_id2();
//		b2->md_unique_option_value2 = pb.md_unique_option_value2();
//		b2->md_unique_option_id3 = pb.md_unique_option_id3();
//		b2->md_unique_option_value3 = pb.md_unique_option_value3();
//		b2->md_unique_option_id4 = pb.md_unique_option_id4();
//		b2->md_unique_option_value4 = pb.md_unique_option_value4();
//		b2->md_unique_skill_id = pb.md_unique_skill_id();
//		b2->seal_slot = pb.seal_slot();
//		b2->seal_slot_open = pb.seal_slot_open();
//		b2->md_max_grade = pb.md_max_grade();
//		b2->md_random_option_count = pb.md_random_option_count();
//		b2->md_next_grade_template_id = pb.md_next_grade_template_id();
//		b2->md_buy_price = pb.md_buy_price();
//		b2->md_sell_price = pb.md_sell_price();
//		b2->is_stackable = pb.is_stackable();
//		b2->is_saleable = pb.is_saleable();
//		b2->is_equipable = pb.is_equipable();
//		b2->is_levelupable = pb.is_levelupable();
//		b2->is_enhanceable = pb.is_enhanceable();
//		b2->is_upgradable = pb.is_upgradable();
//		b2->is_composable = pb.is_composable();
//		b2->is_surpassable = pb.is_surpassable();
//		b2->is_sealable = pb.is_sealable();
//
//		return b2;
//	}
//
//	b2network::B2mdCostumeInfoPtr FB2MessageInfoConverter::ToB2mdCostumeInfo(const b2::protocol::masterdata::mdCostumeInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdCostumeInfo>();
//
//		b2->md_template_id = pb.md_template_id();
//		b2->md_group_id = ToB2GroupPrefix(pb.md_group_id());
//		b2->md_set_id = pb.md_set_id();
//		b2->md_type = PbCostume2B2CostumeType(pb.md_type());
//		b2->md_grade = pb.md_grade();
//		b2->md_inventory_type = PbInventory2B2InventoryType(pb.md_inventory_type());
//		b2->md_equip_position = PbEquipPosition2B2EquipPositionType(pb.md_equip_position());
//		b2->md_max_levelup = pb.md_max_levelup();
//		b2->md_main_attribute = pb.md_main_attribute();
//		b2->md_level_factor = pb.md_level_factor();
//		b2->md_unique_option_id1 = pb.md_unique_option_id1();
//		b2->md_unique_option_value1 = pb.md_unique_option_value1();
//		b2->md_unique_option_id2 = pb.md_unique_option_id2();
//		b2->md_unique_option_value2 = pb.md_unique_option_value2();
//		b2->md_unique_option_id3 = pb.md_unique_option_id3();
//		b2->md_unique_option_value3 = pb.md_unique_option_value3();
//		b2->md_unique_option_id4 = pb.md_unique_option_id4();
//		b2->md_unique_option_value4 = pb.md_unique_option_value4();
//		b2->md_unique_skill_id = pb.md_unique_skill_id();
//		b2->md_max_grade = pb.md_max_grade();
//		b2->md_random_option_count = pb.md_random_option_count();
//		b2->md_next_grade_template_id = pb.md_next_grade_template_id();
//		b2->md_buy_price = pb.md_buy_price();
//		b2->md_sell_price = pb.md_sell_price();
//		b2->is_stackable = pb.is_stackable();
//		b2->is_saleable = pb.is_saleable();
//		b2->is_equipable = pb.is_equipable();
//		b2->is_levelupable = pb.is_levelupable();
//		b2->is_enhanceable = pb.is_enhanceable();
//		b2->is_upgradable = pb.is_upgradable();
//		b2->is_composable = pb.is_composable();
//		b2->is_surpassable = pb.is_surpassable();
//
//		return b2;
//	}
//
//	b2network::B2ShopProductPtr FB2MessageInfoConverter::ToB2ShopProduct(const b2::protocol::commondata::ShopProduct& pb)
//	{
//		auto b2 = std::make_shared<B2ShopProduct>();
//
//		b2->product_type = PbShopProductTypeToB2ShopProductType(pb.product_type());
//		b2->product_id = pb.product_id();
//		b2->product_count = pb.product_count();
//
//		return b2;
//	}
//
//	B2ShopMileagePtr FB2MessageInfoConverter::ToB2ShopMileage(const b2::protocol::commondata::ShopMileage& pb)
//	{
//		auto b2 = std::make_shared<B2ShopMileage>();
//
//		b2->shop_mileage_type = PbShopMileageTypeToB2ShopMileageType(pb.shop_mileage_type());
//		b2->mileage = pb.mileage();
//		b2->reward_index = pb.reward_index();
//
//		return b2;
//	}
//
//	B2ShopBuyManagePtr FB2MessageInfoConverter::ToB2ShopBuyManage(const b2::protocol::commondata::ShopBuyManage& pb)
//	{
//		auto b2 = std::make_shared<B2ShopBuyManage>();
//
//		b2->product_id = pb.product_id();
//		b2->buy_count = pb.buy_count();
//		b2->updated_time = pb.updated_time();
//
//		return b2;
//	}
//
//	B2ShopProductMarkPtr FB2MessageInfoConverter::ToB2ShopProductMark(const b2::protocol::commondata::ShopProductMark& pb)
//	{
//		auto b2 = std::make_shared<B2ShopProductMark>();
//
//		b2->product_id = pb.product_id();
//		b2->mark_type = PbShopMarkTypeToB2ShopMarkType(pb.mark_type());
//		b2->sales_start_time = pb.has_sales_start_time()? pb.sales_start_time() : 0;
//		b2->sales_end_time = pb.has_sales_end_time() ? pb.sales_end_time() : 0;
//
//		return b2;
//	}
//
//	B2mdShopLotterySpecialGradePtr FB2MessageInfoConverter::ToB2mdShopLotterySpecialGrade(const b2::protocol::masterdata::mdShopLotterySpecialGrade& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopLotterySpecialGrade>();
//
//		b2->min_grade = pb.min_grade();
//		b2->max_grade = pb.max_grade();
//
//		return b2;
//	}
//
//	B2mdShopProductPtr FB2MessageInfoConverter::ToB2mdShopProduct(const b2::protocol::masterdata::mdShopProduct& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopProduct>();
//
//		b2->id = pb.id();
//		b2->shop_type = PbShopTypeToB2ShopType(pb.shop_type());
//		b2->shop_order = pb.shop_order();
//		b2->product_type = PbShopProductTypeToB2ShopProductType(pb.product_type());
//		b2->product_order = pb.product_order();
//		b2->product_id = pb.product_id();
//		b2->product_count = pb.product_count();
//		b2->bonus_product_type = PbShopProductTypeToB2ShopProductType(pb.bonus_product_type());
//		b2->bonus_product_id = pb.bonus_product_id();
//		b2->bonus_product_count = pb.bonus_product_count();
//		b2->calculated_bonus_count = pb.calculated_bonus_count();
//		b2->buy_price_type = pb.buy_price_type();
//		b2->price = pb.price();
//		b2->calculated_price = pb.calculated_price();
//		b2->restric_type = PbShopRestrictTypeToB2ShopRestrictType(pb.restric_type());
//		b2->buy_limit_count = pb.buy_limit_count();
//
//		return b2;
//	}
//
//	B2mdShopPackagePtr FB2MessageInfoConverter::ToB2mdShopPackage(const b2::protocol::masterdata::mdShopPackage& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopPackage>();
//
//		b2->id = pb.id();
//		b2->tab_type = pb.tab_type();
//		b2->category = pb.category();
//		b2->name = UTF8_TO_TCHAR(pb.name().c_str()); 
//		b2->pay_period = pb.pay_period();
//		b2->period_bonus_reward_id = pb.period_bonus_reward_id();
//		b2->period_bonus_reward_count = pb.period_bonus_reward_count();
//
//		return b2;
//	}
//
//	B2mdShopPackageProductPtr FB2MessageInfoConverter::ToB2mdShopPackageProduct(const b2::protocol::masterdata::mdShopPackageProduct& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopPackageProduct>();
//
//		b2->package_id = pb.package_id();
//		b2->index = pb.index();
//		b2->product = ToB2ShopProduct(pb.product());
//
//		return b2;
//	}
//
//	B2mdShopMileageRewardPtr FB2MessageInfoConverter::ToB2mdShopMileageReward(const b2::protocol::masterdata::mdShopMileageReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopMileageReward>();
//
//		b2->mileage_type = PbShopMileageTypeToB2ShopMileageType(pb.mileage_type());
//		b2->index = pb.index();
//		b2->mileage = pb.mileage();
//		
//		for (auto bpReward : pb.rewards()) 
//		{
//			b2->rewards.Emplace(ToB2Reward(bpReward));
//		}
//
//		return b2;
//	}
//
//	B2RawRewardPtr FB2MessageInfoConverter::ToB2RawReward(const b2::protocol::commondata::RawReward& pb)
//	{
//		auto rawReward = std::make_shared<B2RawReward>();
//
//		rawReward->id = pb.id();
//		rawReward->count = pb.count();
//
//		return rawReward;
//	}
//
//	B2RawRewardPtr FB2MessageInfoConverter::ToB2RawReward(const int32 rewardId, const int32 rewardCount)
//	{
//		auto rawReward = std::make_shared<B2RawReward>();
//
//		rawReward->id = rewardId;
//		rawReward->count = rewardCount;
//
//		return rawReward;
//	}
//
//	b2network::B2RewardRandomCountPtr FB2MessageInfoConverter::ToB2RewardRandomCount(const b2::protocol::commondata::RewardRandomCount& pb)
//	{
//		auto reward = std::make_shared<B2RewardRandomCount>();
//
//		reward->id = pb.id();
//		reward->min_count = pb.min_count();
//		reward->max_count = pb.max_count();
//
//		return reward;
//	}
//
//	B2RewardPtr FB2MessageInfoConverter::ToB2Reward(const b2::protocol::commondata::Reward& reward)
//	{
//		auto b2Reward = std::make_shared<B2Reward>();
//
//		b2Reward->raw_reward = ToB2RawReward(reward.raw_reward());
//
//		if (reward.has_item()) b2Reward->item = ItemPb2B2ItemServerInfo(reward.item());
//		if (reward.has_costume()) b2Reward->costume = CostumePb2B2CostumeServerInfo(reward.costume());
//		if (reward.has_aether()) b2Reward->aether = AetherPb2B2AetherServerInfo(reward.aether());
//		if (reward.has_light_account()) b2Reward->light_account = LightAccountPb2B2LightAccount(reward.light_account());
//
//		for (auto lightCharacter : reward.light_characters())
//		{
//			b2Reward->light_characters.Emplace(LightCharacterPb2B2LightCharacter(lightCharacter));
//		}
//
//		return b2Reward;
//	}
//
//	b2network::B2CollectionItemPtr FB2MessageInfoConverter::ToB2CollectionItem(const b2::protocol::commondata::CollectionItem& pb)
//	{
//		auto item = std::make_shared<B2CollectionItem>();
//
//		item->item_template_id = pb.item_template_id();
//		item->collect_state = PbCollectionItemStateToB2CollectionItemState(pb.collect_state());
//		item->is_new_item = pb.is_new_item();
//
//		return item;
//	}
//
//	B2CollectionSetItemPtr FB2MessageInfoConverter::ToB2CollectionSetItem(const b2::protocol::commondata::CollectionSetItem& pb)
//	{
//		auto item = std::make_shared<B2CollectionSetItem>();
//
//		item->grade = pb.grade();
//		item->group_id = pb.group_id();
//		item->character_type = PbCharacter2B2CharacterType(pb.character_type());
//
//		return item;
//	}
//
//	B2mdCollectionRewardPtr FB2MessageInfoConverter::ToB2CollectionReward(const b2::protocol::masterdata::mdCollectionReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdCollectionReward>();
//
//		b2->collection_category = PbCollectionCategoryToB2CollectionCategory(pb.collection_category());
//		b2->grade = pb.grade();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2mdItemSetPtr FB2MessageInfoConverter::ToB2mdItemSet(const b2::protocol::masterdata::mdItemSet& pb)
//	{
//		auto b2 = std::make_shared<B2mdItemSet>();
//
//		b2->set_id = pb.set_id();
//		b2->name = UTF8_TO_TCHAR(pb.name().c_str()); 
//		b2->set_option_id_set2 = pb.set_option_id_set2();
//		b2->set_value_set2 = pb.set_value_set2(); 
//		b2->set_option_id_set3 = pb.set_option_id_set3();
//		b2->set_value_set3 = pb.set_value_set3();
//		b2->set_option_id_set4 = pb.set_option_id_set4();
//		b2->set_value_set4 = pb.set_value_set4();
//
//		return b2;
//	}
//
//	B2mdSelectItemSealOptionPtr FB2MessageInfoConverter::ToB2mdSelectItemSealOption(const b2::protocol::masterdata::mdSelectItemSealOption& pb)
//	{
//		auto b2 = std::make_shared<B2mdSelectItemSealOption>();
//
//		b2->option_id = pb.option_id();
//		b2->slot = pb.slot();
//		b2->prob = pb.prob();
//		b2->decimal_point = pb.decimal_point();
//		b2->min = pb.min();
//		b2->max = pb.max();
//
//		return b2;
//	}
//
//	B2mdItemSealCostPtr FB2MessageInfoConverter::ToB2mdItemSealCost(const b2::protocol::masterdata::mdItemSealCost& pb)
//	{
//		auto b2 = std::make_shared<B2mdItemSealCost>();
//
//		b2->slot = pb.slot();
//		b2->open_price_type = pb.open_price_type();
//		b2->open_price = pb.open_price();
//		b2->template_id1 = pb.template_id1();
//		b2->amount1 = pb.amount1();
//		b2->template_id2 = pb.template_id2();
//		b2->amount2 = pb.amount2();
//		b2->template_id3 = pb.template_id3();
//		b2->amount3 = pb.amount3();
//		b2->template_id4 = pb.template_id4();
//		b2->amount4 = pb.amount4();
//		b2->seal_price_type = pb.seal_price_type();
//		b2->seal_price = pb.seal_price();
//
//		return b2;
//	}
//
//	B2mdSelectItemOptionPtr FB2MessageInfoConverter::ToB2mdSelectItemOption(const b2::protocol::masterdata::mdSelectItemOption& pb)
//	{
//		auto b2 = std::make_shared<B2mdSelectItemOption>();
//
//		b2->option_id = pb.option_id();
//		b2->grade = pb.grade();
//		b2->equip_position = pb.equip_position();
//		b2->prob = pb.prob();
//		b2->decimal_point = pb.decimal_point();
//		b2->min = pb.min();
//		b2->max = pb.max();
//
//		return b2;
//	}
//
//	B2RaidRankingRewardPtr FB2MessageInfoConverter::ToB2RaidRankingReward(const b2::protocol::commondata::RaidRankingReward& pb)
//	{
//		auto b2 = std::make_shared<B2RaidRankingReward>();
//
//		b2->ranking = pb.ranking();
//		b2->item_template_id = pb.item_template_id();
//		b2->item_count = pb.item_count();
//		b2->is_bonus = pb.is_bonus();
//
//		return b2;
//	}
//
//	B2DimensionStatusPtr FB2MessageInfoConverter::ToB2DimensionStatus(const b2::protocol::commondata::DimensionStatus& pb)
//	{
//		auto b2 = std::make_shared<B2DimensionStatus>();
//
//		b2->best_difficulty_level = pb.best_difficulty_level();
//		if (b2->best_difficulty_level > 0) {
//			b2->best_clear_time_millis = pb.best_clear_time_millis();
//			b2->best_character_type = PbCharacter2B2CharacterType(pb.best_character_type());
//			b2->best_character_level = pb.best_character_level();
//			b2->best_total_power = pb.best_total_power();
//			b2->best_record_time = pb.best_record_time();
//		}
//
//		b2->top_difficulty_level = pb.top_difficulty_level();
//
//		b2->last_difficulty_level = pb.has_last_character_type()? pb.last_difficulty_level() : 0;
//		if (pb.has_last_character_type()) {
//			b2->last_character_type = PbCharacter2B2CharacterType(pb.last_character_type());;
//		}
//
//		b2->daily_clear_count = pb.daily_clear_count();
//		b2->daily_clear_count_max = pb.daily_clear_count_max();
//		b2->daily_count_init_time = pb.daily_count_init_time();
//
//		return b2;
//	}
//
//	b2network::B2mdDailyAttendanceInfoPtr FB2MessageInfoConverter::ToB2mdDailyAttendanceInfo(const b2::protocol::masterdata::mdDailyAttendanceInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdDailyAttendanceInfo>();
//
//		b2->attendance_type = PbDailyAttendanceTypeToB2DailyAttendanceType(pb.attendance_type());
//		b2->day = pb.day();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleCharacterSlotPtr FB2MessageInfoConverter::ToB2GuildBattleCharacterSlot(const b2::protocol::commondata::GuildBattleEntry::SlotData& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleCharacterSlot>();
//
//		b2->slot_num = pb.slot_num();
//		b2->entry_code = pb.entry_code();
//		b2->entry_type = ToB2GuildBattleEntryType(pb.entry_type());
//
//		if (pb.has_level()) {
//			b2->level = pb.level();
//		}
//		else {
//			b2->level = 0;
//		}
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleEntryPtr FB2MessageInfoConverter::ToB2GuildBattleEntry(const b2::protocol::commondata::GuildBattleEntry& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleEntry>();
//
//		for (auto slotData : pb.slots())
//		{
//			b2->slots.Emplace(ToB2GuildBattleCharacterSlot(slotData));
//		}
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleMemberPtr FB2MessageInfoConverter::ToB2GuildBattleMember(const b2::protocol::session::GetGuildBattleResponse::Member& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleMember>();
//
//		b2->account_id = pb.account_id();
//		b2->nickname = UTF8_TO_TCHAR(pb.nickname().c_str());
//		b2->power_index = pb.power_index();
//		b2->left_attack = pb.left_attack();
//		b2->left_medal = pb.left_medal();
//		b2->season_obtain_medal = pb.season_obtain_medal();
//		b2->total_power = pb.total_power();
//		b2->entry = ToB2GuildBattleEntry(pb.entry());
//		b2->is_attacked = pb.is_attacked();
//
//		for (auto character_power : pb.character_powers()) 
//		{
//			b2->character_powers.Emplace(CharacterPowerPb2B2CharacterPower(character_power));
//		}
//		return b2;
//	}
//
//	b2network::B2GuildBattleAttackPtr FB2MessageInfoConverter::ToB2GuildBattleAttack(const b2::protocol::session::GetGuildBattleResponse::Attack& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleAttack>();
//
//		b2->guild_id = pb.guild_id();
//		b2->account_id = pb.account_id();
//		b2->attack_count = pb.attack_count();
//		b2->power_index = pb.power_index();
//		b2->nickname = UTF8_TO_TCHAR(pb.nickname().c_str());
//		b2->target_nickname = UTF8_TO_TCHAR(pb.target_nickname().c_str());
//		b2->target_power_index = pb.target_power_index();
//		b2->obtain_medal = pb.obtain_medal();
//		b2->battle_end_time = pb.battle_end_time();
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleGuildPtr FB2MessageInfoConverter::ToB2GuildBattleGuild(const b2::protocol::session::GetGuildBattleResponse::Guild& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleGuild>();
//
//		b2->guild_id = pb.guild_id();
//		b2->guild_name = UTF8_TO_TCHAR(pb.guild_name().c_str());
//		b2->mark_index = pb.mark_index();
//		b2->mark_color = pb.mark_color();
//		b2->mark_bg_index = pb.mark_bg_index();
//		b2->mark_bg_color = pb.mark_bg_color();
//		b2->obtain_medal = pb.obtain_medal();
//		b2->attack_count = pb.attack_count();
//		b2->guild_level = pb.guild_level();
//		b2->guild_exp = pb.guild_exp();
//
//		for (auto member : pb.members())
//		{
//			b2->members.Emplace(ToB2GuildBattleMember(member));
//		}
//
//		for (auto attack : pb.attacks())
//		{
//			b2->attacks.Emplace(ToB2GuildBattleAttack(attack));
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdGuildBattleScheduleInfoPtr FB2MessageInfoConverter::ToB2mdGuildBattleScheduleInfo(const b2::protocol::masterdata::mdGuildBattleScheduleInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildBattleScheduleInfo>();
//
//		b2->turn = pb.turn();
//		b2->state = pb.state();
//		b2->start_day = pb.start_day();
//		b2->start_hour = pb.start_hour();
//		b2->start_min = pb.start_min();
//		b2->end_day = pb.end_day();
//		b2->end_hour = pb.end_hour();
//		b2->end_min = pb.end_min();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildBattleResultRewardPtr FB2MessageInfoConverter::ToB2mdGuildBattleResultReward(const b2::protocol::masterdata::mdGuildBattleResultReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildBattleResultReward>();
//
//		b2->reward_grade = pb.reward_grade();
//		b2->req_count_min = pb.req_count_min();
//		b2->req_count_max = pb.req_count_max();
//
//		for (auto pbReward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdGuildBattleSeasonRewardPtr FB2MessageInfoConverter::ToB2mdGuildBattleSeasonReward(const b2::protocol::masterdata::mdGuildBattleSeasonReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildBattleSeasonReward>();
//
//		b2->order = pb.order();
//		b2->ranking_min = pb.ranking_min();
//		b2->ranking_max = pb.ranking_max();
//		b2->ranking_rate_min = pb.ranking_rate_min();
//		b2->ranking_rate_max = pb.ranking_rate_max();
//
//		for (auto pbReward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(pbReward));
//		}
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleStatePtr FB2MessageInfoConverter::ToB2GuildBattleState(const b2::protocol::commondata::GuildBattleState& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleState>();
//
//		b2->season_id = pb.season_id();
//		b2->turn = pb.turn();
//		b2->current_state = ToB2GuildBattleStateType(pb.current_state());
//		b2->begin_time = pb.begin_time();
//		b2->end_time = pb.end_time();
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleStatePtr FB2MessageInfoConverter::ToB2GuildBattleStateNone()
//	{
//		auto b2 = std::make_shared<B2GuildBattleState>();
//
//		b2->season_id = 0;
//		b2->turn = 0;
//		b2->current_state = B2GuildBattleStateType::NONE;
//		b2->begin_time = 0;
//		b2->end_time = 0;
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleHistoryPtr FB2MessageInfoConverter::ToB2GuildBattleHistory(const b2::protocol::commondata::GuildBattleHistory& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleHistory>();
//
//		b2->obtain_medal = pb.obtain_medal();
//		b2->medal_total_count = pb.medal_total_count();
//		b2->result = ToB2GuildBattleResultType(pb.result());
//		b2->battle_end_time = pb.battle_end_time();
//		b2->target_guild_id = pb.target_guild_id();
//		b2->target_guild_name = UTF8_TO_TCHAR(pb.target_guild_name().c_str());
//		b2->target_obtain_medal = pb.target_obtain_medal();
//		b2->target_mark = pb.target_mark();
//		b2->target_mark_color = pb.target_mark_color();
//		b2->target_mark_bg = pb.target_mark_bg();
//		b2->target_mark_bg_color = pb.target_mark_bg_color();
//		b2->target_medal_total_count = pb.target_medal_total_count();
//		b2->member_count = pb.member_count();
//		b2->attack_count = pb.attack_count();
//		b2->target_member_count = pb.target_member_count();
//		b2->target_attack_count = pb.target_attack_count();
//
//		return b2;
//	}
//
//	b2network::B2GuildBattleRecordPtr FB2MessageInfoConverter::ToB2GuildBattleRecord(const b2::protocol::commondata::GuildBattleRecord& pb)
//	{
//		auto b2 = std::make_shared<B2GuildBattleRecord>();
//
//		b2->win_count = pb.win_count();
//		b2->draw_count = pb.draw_count();
//		b2->lose_count = pb.lose_count();
//		b2->total_obtain_medal = pb.total_obtain_medal();
//		b2->ranking = pb.ranking();
//		b2->ranking_total_count = pb.ranking_total_count();
//		b2->season_id = pb.season_id();
//
//		return b2;
//	}
//
//	B2mdRaidInfoPtr FB2MessageInfoConverter::ToB2mdRaidInfo(const b2::protocol::masterdata::mdRaidInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdRaidInfo>();
//
//		b2->raid_type = pb.raid_type();
//		b2->step = pb.step();
//		b2->difficulty_scale = pb.difficulty_scale();
//		b2->recommend_power = pb.recommend_power();
//		b2->recommend_attack = pb.recommend_attack();
//		b2->recommend_defense = pb.recommend_defense();
//		b2->reward_id = pb.reward_id();
//		b2->reward_min_count = pb.reward_min_count();
//		b2->reward_max_count = pb.reward_max_count();
//		b2->main_reward_id = pb.main_reward_id();
//		b2->main_reward_min_count = pb.main_reward_min_count();
//		b2->main_reward_max_count = pb.main_reward_max_count();
//		b2->reward_item_min_grade = pb.reward_item_min_grade();
//		b2->reward_item_max_grade = pb.reward_item_max_grade();
//		return b2;
//	}
//
//	B2mdMonsterInfoPtr FB2MessageInfoConverter::ToB2mdMonsterInfo(const b2::protocol::masterdata::mdMonsterInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdMonsterInfo>();
//
//		b2->id = pb.id();
//		b2->name = ANSI_TO_TCHAR(pb.name().c_str());
//		b2->symbol = ANSI_TO_TCHAR(pb.symbol().c_str());
//		b2->grade = ANSI_TO_TCHAR(pb.grade().c_str());
//		b2->type = ANSI_TO_TCHAR(pb.type().c_str());
//		b2->desc = ANSI_TO_TCHAR(pb.desc().c_str());
//		b2->hp_scale = pb.hp_scale();
//		b2->attack_defense_level_scale = pb.attack_defense_level_scale();
//		b2->max_armor_override = pb.max_armor_override();
//
//		return b2;
//	}
//
//	b2network::B2GuildMemberCandidatePtr FB2MessageInfoConverter::ToB2GuildMemberCandidate(const b2::protocol::commondata::GuildMemberCandidate& pb)
//	{
//		auto b2 = std::make_shared<B2GuildMemberCandidate>();
//
//		b2->account_id = pb.account_id();
//		b2->invite_state = ToB2GuildInviteState(pb.invite_state());
//
//		return b2;
//	}
//
//	b2network::B2GuildInvitePtr FB2MessageInfoConverter::ToB2GuildInvite(const b2::protocol::session::GetGuildInviteListResponse::Invite& pb)
//	{
//		auto b2 = std::make_shared<B2GuildInvite>();
//
//		b2->guild = ToB2Guild(pb.guild());
//		b2->expire_time = pb.expire_time();
//
//		return b2;
//	}
//
//	B2MailItemInfoPtr FB2MessageInfoConverter::ToB2MailItemInfo(const b2::protocol::commondata::MailItemInfo &pb)
//	{
//		auto b2 = std::make_shared<B2MailItemInfo>();
//
//		b2->item_template_id = pb.item_template_id();
//		b2->item_amount = pb.item_amount();
//		b2->character_type = PbItemCharacter2B2CharacterType(pb.character_type());
//		b2->item = ItemPb2B2ItemServerInfo(pb.item());
//		b2->costume = CostumePb2B2CostumeServerInfo(pb.costume());
//		b2->totem = TotemPb2B2TotemServerInfo(pb.totem());
//				
//		return b2;
//	}
//
//	b2network::B2mdGeneralLotteryPtr FB2MessageInfoConverter::ToB2mdGeneralLottery(const b2::protocol::masterdata::mdGeneralLottery &pb)
//	{
//		auto b2 = std::make_shared<B2mdGeneralLottery>();
//
//		b2->id = pb.id();
//		b2->grade = ToB2LotteryGrade(pb.grade());
//		b2->character_type = PbItemCharacter2B2CharacterType(pb.character_type());
//		b2->equip_category = PbEquipCategoryToB2EquipCategory(pb.equip_category());
//
//		return b2;
//	}
//
//	b2network::B2mdSelectiveLotteryPtr FB2MessageInfoConverter::ToB2mdSelectiveLottery(const b2::protocol::masterdata::mdSelectiveLottery &pb)
//	{
//		auto b2 = std::make_shared<B2mdSelectiveLottery>();
//
//		b2->id = pb.id();
//		b2->character_type = PbItemCharacter2B2CharacterType(pb.character_type());
//		b2->equip_category = PbEquipCategoryToB2EquipCategory(pb.equip_category());
//		b2->item_grade = pb.item_grade();
//
//		return b2;
//	}
//
//	b2network::B2mdFixedGradeLotteryPtr FB2MessageInfoConverter::ToB2mdFixedGradeLottery(const b2::protocol::masterdata::mdFixedGradeLottery &pb)
//	{
//		auto b2 = std::make_shared<B2mdFixedGradeLottery>();
//
//		b2->id = pb.id();
//		b2->character_type = PbItemCharacter2B2CharacterType(pb.character_type());
//		b2->equip_category = PbEquipCategoryToB2EquipCategory(pb.equip_category());
//		b2->item_grade = pb.item_grade();
//
//		return b2;
//	}
//
//	b2network::B2mdPrefixSelectiveLotteryPtr FB2MessageInfoConverter::ToB2mdPrefixSelectiveLottery(const b2::protocol::masterdata::mdPrefixSelectiveLottery &pb)
//	{
//		auto b2 = std::make_shared<B2mdPrefixSelectiveLottery>();
//
//		b2->id = pb.id();
//		b2->character_type = PbItemCharacter2B2CharacterType(pb.character_type());
//		b2->group_id = ToGroupPrefix(pb.group_id());
//		b2->equip_category = PbEquipCategoryToB2EquipCategory(pb.equip_category());
//		b2->item_grade = pb.item_grade();
//		b2->surpass_level = pb.surpass_level();
//		b2->quality = pb.quality();
//
//		return b2;
//	}
//
//	b2network::B2GuildRankingPtr FB2MessageInfoConverter::ToB2GuildRanking(const b2::protocol::commondata::GuildRanking &pb)
//	{
//		auto b2 = std::make_shared<B2GuildRanking>();
//
//		b2->ranking = pb.ranking();
//		b2->guild_id = pb.guild_id();
//		b2->guild_name = UTF8_TO_TCHAR(pb.guild_name().c_str());
//		b2->total_obtain_medal = pb.total_obtain_medal();
//		b2->total_result = pb.total_result();
//		b2->members_avg_power = pb.members_avg_power();
//		b2->create_time = pb.create_time();
//		b2->mark = pb.mark();
//		b2->mark_color = pb.mark_color();
//		b2->mark_bg = pb.mark_bg();
//		b2->mark_bg_color = pb.mark_bg_color();
//		b2->gm_nickname = UTF8_TO_TCHAR(pb.gm_nickname().c_str());
//		b2->max_member_count = pb.max_member_count();
//		b2->cur_member_count = pb.cur_member_count();
//		b2->level = pb.level();
//		b2->exp = pb.exp();
//
//		return b2;
//
//	}
//
//	B2mdAdditionalMatchCostPtr FB2MessageInfoConverter::ToB2mdAdditionalMatchCost(const b2::protocol::masterdata::mdAdditionalMatchCost &pb)
//	{
//		auto b2 = std::make_shared<B2mdAdditionalMatchCost>();
//
//		b2->count = pb.count();
//		b2->cost = pb.cost();
//
//		return b2;
//	}
//
//	B2MatchOpenPeriodInfoPtr FB2MessageInfoConverter::ToB2MatchPeriodInfo(b2::protocol::commondata::MatchOpenPeriodInfo &pb)
//	{
//		auto b2 = std::make_shared<B2MatchOpenPeriodInfo>();
//
//		b2->day_of_week = pb.day_of_week();
//		b2->begin_hour = pb.begin_hour();
//		b2->begin_min= pb.begin_min();
//		b2->end_hour = pb.end_hour();
//		b2->end_min = pb.end_min();
//
//		if (pb.has_raid_type()) 
//		{
//			b2->raid_type = pb.raid_type();
//		}
//		else
//		{
//			b2->raid_type = 0;
//		}
//
//		if (pb.has_assault_type())
//		{
//			b2->assault_type = pb.assault_type();
//		}
//		else
//		{
//			b2->assault_type = 0;
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdShopLotteryDailyLimitPtr FB2MessageInfoConverter::ToB2mdShopLotteryDailyLimit(b2::protocol::masterdata::mdShopLotteryDailyLimit &pb)
//	{
//
//		auto b2 = std::make_shared<B2mdShopLotteryDailyLimit>();
//
//		b2->draw_price_type = PbShopDrawPriceTypeToB2ShopDrawPriceType(pb.draw_price_type());
//		b2->is_continuous = pb.is_continuous();
//		b2->limit_count = pb.limit_count();
//
//		return b2;
//	}
//
//	b2network::B2mdShopLotterySalePtr FB2MessageInfoConverter::ToB2mdShopLotterySale(b2::protocol::masterdata::mdShopLotterySale &pb)
//	{
//
//		auto b2 = std::make_shared<B2mdShopLotterySale>();
//
//		b2->draw_price_type = PbShopDrawPriceTypeToB2ShopDrawPriceType(pb.draw_price_type());
//		b2->is_continuous = pb.is_continuous();
//		b2->discount_rate = pb.discount_rate();
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildSkillPtr FB2MessageInfoConverter::ToB2mdGuildSkill(const b2::protocol::masterdata::mdGuildSkill& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildSkill>();
//
//		b2->id = pb.id();
//		b2->level = pb.level();
//		b2->upgrade_cost_gold = pb.upgrade_cost_gold();
//		b2->upgrade_req_guild_level = pb.upgrade_req_guild_level();
//		b2->buy_cost_gold = pb.buy_cost_gold();
//		b2->buff_option_type = pb.buff_option_type();
//		b2->buff_option_value = pb.buff_option_value();
//		b2->buff_time_sec = pb.buff_time_sec();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildExpPtr FB2MessageInfoConverter::ToB2mdGuildExp(const b2::protocol::masterdata::mdGuildExp& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildExp>();
//
//		b2->level = pb.level();
//		b2->exp_max = pb.exp_max();
//
//		return b2;
//	}
//
//	b2network::B2mdStageFixedDropPtr FB2MessageInfoConverter::ToB2mdStageFixedDrop(const b2::protocol::masterdata::mdStageFixedDrop& pb)
//	{
//		auto b2 = std::make_shared<B2mdStageFixedDrop>();
//
//		b2->stage_id = pb.stage_id();
//		b2->idx = pb.idx();
//		b2->template_id = pb.template_id();
//		b2->min = pb.min();
//		b2->max = pb.max();
//		b2->rate = pb.rate();
//
//		return b2;
//	}
//
//	b2network::B2DropDataPtr FB2MessageInfoConverter::ToB2DropData(const b2::protocol::commondata::DropData& pb)
//	{
//		auto b2 = std::make_shared<B2DropData>();
//
//		b2->template_id = pb.template_id();
//		b2->count = pb.count();
//		
//		return b2;
//	}
//
//	b2network::B2DuelRewardBoxStatusPtr FB2MessageInfoConverter::ToB2DuelRewardBoxStatus(const b2::protocol::commondata::DuelRewardBoxStatus& pb)
//	{
//		auto b2 = std::make_shared<B2DuelRewardBoxStatus>();
//
//		b2->is_reward_state = pb.is_reward_state();
//		b2->box_grade = ToB2RewardBoxGrade(pb.box_grade());
//		b2->win_count = pb.win_count();
//		b2->upgrade_win_count = pb.upgrade_win_count();
//		b2->defeat_count = pb.defeat_count();
//
//		return b2;
//	}
//
//	b2network::B2ContentsOpenStatusPtr FB2MessageInfoConverter::ToB2ContentsOpenStatus(const b2::protocol::commondata::ContentsOpen& pb)
//	{
//		auto b2 = std::make_shared<B2ContentsOpenStatus>();
//
//		b2->mode_id = pb.mode_id();
//		b2->opened = pb.opened();
//		b2->stage_id = pb.stage_id();
//		b2->tutorial_id = pb.tutorial_id();
//
//		return b2;
//	}
//
//	b2network::B2GuildMercenaryPtr FB2MessageInfoConverter::ToB2GuildMercenary(const b2::protocol::commondata::GuildMercenary& pb)
//	{
//		auto b2 = std::make_shared<B2GuildMercenary>();
//
//		b2->id = pb.id();
//		b2->level = pb.level();
//		b2->exp = pb.exp();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildMercenaryPtr FB2MessageInfoConverter::ToB2mdGuildMercenary(const b2::protocol::masterdata::mdGuildMercenary& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildMercenary>();
//
//		b2->id = pb.id();
//		b2->mastery_type = pb.mastery_type();
//		b2->max_armor_override = pb.max_armor_override();
//		b2->skill_id_1 = pb.skill_id_1();
//		b2->skill_id_2 = pb.skill_id_2();
//		b2->skill_id_3 = pb.skill_id_3();
//		b2->max_use_count = pb.max_use_count();
//		b2->is_usable = pb.is_usable();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildMercenarySkillPtr FB2MessageInfoConverter::ToB2mdGuildMercenarySkill(const b2::protocol::masterdata::mdGuildMercenarySkill& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildMercenarySkill>();
//
//		b2->skill_id = pb.skill_id();
//		b2->require_mercenary_level = pb.require_mercenary_level();
//		b2->cool_time_sec = pb.cool_time_sec();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildMercenaryMasteryPtr FB2MessageInfoConverter::ToB2mdGuildMercenaryMastery(const b2::protocol::masterdata::mdGuildMercenaryMastery& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildMercenaryMastery>();
//
//		b2->id = pb.id();
//		b2->level = pb.level();
//		b2->max_exp = pb.max_exp();
//		b2->option_id_1 = pb.option_id_1();
//		b2->option_value_1 = pb.option_value_1();
//		b2->option_id_2 = pb.option_id_2();
//		b2->option_value_2 = pb.option_value_2();
//		b2->option_id_3 = pb.option_id_3();
//		b2->option_value_3 = pb.option_value_3();
//
//		return b2;
//	}
//
//	b2network::B2mdGuildMercenaryDonationPtr FB2MessageInfoConverter::ToB2mdGuildMercenaryDonation(const b2::protocol::masterdata::mdGuildMercenaryDonation& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildMercenaryDonation>();
//
//		b2->donation_type = ToB2GuildMercenaryDonationType(pb.donation_type());
//		b2->item_template_id = pb.item_template_id();
//		b2->donation_amount = pb.donation_amount();
//		b2->exp_increment = pb.exp_increment();
//		
//		return b2;
//	}
//
//	B2AdminMessagePtr FB2MessageInfoConverter::ToB2AdminMessage(const b2::protocol::commondata::AdminMessage& pb)
//	{
//		auto b2 = std::make_shared<B2AdminMessage>();
//
//		b2->message = UTF8_TO_TCHAR(pb.message().c_str());
//		b2->display_time = pb.display_time();
//		
//		return b2;
//	}
//
//	B2PointEventStatusPtr FB2MessageInfoConverter::ToB2PointEventStatus(const b2::protocol::commondata::PointEventStatus& pb)
//	{
//		auto b2 = std::make_shared<B2PointEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->point = pb.point();
//		b2->reward_state_value = pb.reward_state_value();
//		
//		return b2;
//	}
//
//	B2mdEventInfoPtr FB2MessageInfoConverter::ToB2mdEventInfo(const b2::protocol::masterdata::mdEventInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdEventInfo>();
//
//		b2->event_id = pb.event_id();
//		b2->event_category = pb.event_category();
//		b2->event_tab_name = UTF8_TO_TCHAR(pb.event_tab_name().c_str());
//		b2->title = UTF8_TO_TCHAR(pb.title().c_str());
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//		b2->explain = UTF8_TO_TCHAR(pb.explain().c_str());
//		b2->banner_url = pb.banner_url().c_str();
//		b2->link_text = UTF8_TO_TCHAR(pb.link_text().c_str());
//		b2->link_url = pb.link_url().c_str();
//		b2->tab_order = pb.tab_order();
//		b2->event_sub_category = pb.event_sub_category();
//		
//		return b2;
//	}
//
//	B2mdPointEventPtr FB2MessageInfoConverter::ToB2mdPointEvent(const b2::protocol::masterdata::mdPointEvent& pb)
//	{
//		auto b2 = std::make_shared<B2mdPointEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->event_type = pb.event_type();
//		b2->condition1 = pb.condition1();
//		b2->condition2 = pb.condition2();
//		b2->condition3 = pb.condition3();
//		b2->condition4 = pb.condition4();
//		b2->point = pb.point();
//		
//		return b2;
//	}
//
//	B2mdPointEventRewardPtr FB2MessageInfoConverter::ToB2mdPointEventReward(const b2::protocol::masterdata::mdPointEventReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdPointEventReward>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//		b2->need_point = pb.need_point();
//		b2->tab_group_id = pb.tab_group_id();
//		b2->buy_limit_count = pb.buy_limit_count();
//		
//		return b2;
//	}
//
//	B2mdLevelUpEventPtr FB2MessageInfoConverter::ToB2mdLevelUpEvent(const b2::protocol::masterdata::mdLevelUpEvent& pb)
//	{
//		auto b2 = std::make_shared<B2mdLevelUpEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->level = pb.level();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//		
//		return b2;
//	}
//
//	B2LevelUpEventInfoPtr FB2MessageInfoConverter::ToB2LevelUpEventInfo(const b2::protocol::commondata::LevelUpEventInfo& pb)
//	{
//		auto b2 = std::make_shared<B2LevelUpEventInfo>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->state = pb.state();
//		
//		return b2;
//	}
//
//	B2mdHotTimeEventPtr FB2MessageInfoConverter::ToB2mdHotTimeEvent(const b2::protocol::masterdata::mdHotTimeEvent& pb)
//	{
//		auto b2 = std::make_shared<B2mdHotTimeEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->day_of_week = pb.day_of_week();
//		b2->index = pb.index();
//		b2->buff_type = pb.buff_type();
//		b2->begin_hour = pb.begin_hour();
//		b2->begin_min = pb.begin_min();
//		b2->end_hour = pb.end_hour();
//		b2->end_min = pb.end_min();
//		b2->increase_rate = pb.increase_rate();
//		
//		return b2;
//	}
//
//	B2mdSeasonMissionEventPtr FB2MessageInfoConverter::ToB2mdSeasonMissionEvent(const b2::protocol::masterdata::mdSeasonMissionEvent& pb)
//	{
//		auto b2 = std::make_shared<B2mdSeasonMissionEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->day = pb.day();
//		b2->index = pb.index();
//		b2->event_type = pb.event_type();
//		b2->condition1 = pb.condition1();
//		b2->condition2 = pb.condition2();
//		b2->condition3 = pb.condition3();
//		b2->condition4 = pb.condition4();
//		b2->goal_count = pb.goalcount();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//		b2->tutorial_id = pb.tutorial_id();
//		
//		return b2;
//	}
//
//	B2SeasonMissionInfoPtr FB2MessageInfoConverter::ToB2SeasonMissionInfo(const b2::protocol::commondata::SeasonMissionInfo& pb)
//	{
//		auto b2 = std::make_shared<B2SeasonMissionInfo>();
//
//		b2->event_id = pb.event_id();
//		b2->day = pb.day();
//		b2->index = pb.index();
//		b2->progress_count = pb.progress_count();
//		b2->state = pb.state();
//		
//		return b2;
//	}
//
//	B2SeasonMissionEventStatusPtr FB2MessageInfoConverter::ToB2SeasonMissionEventStatus(const b2::protocol::commondata::SeasonMissionEventStatus& pb)
//	{
//		auto b2 = std::make_shared<B2SeasonMissionEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->attendance_day = pb.attendance_day();
//		b2->next_attendance_time = pb.next_attendance_time();
//		
//		return b2;
//	}
//
//	B2StageClearEventStatusPtr FB2MessageInfoConverter::ToB2StageClearEventStatus(const b2::protocol::commondata::StageClearEventStatus& pb)
//	{
//		auto b2 = std::make_shared<B2StageClearEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->state = pb.state();
//
//		return b2;
//	}
//
//	B2PointShopEventStatusPtr FB2MessageInfoConverter::ToB2PointShopEventStatus(const b2::protocol::commondata::PointShopEventStatus& pb)
//	{
//		auto b2 = std::make_shared<B2PointShopEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->point = pb.point();
//
//		return b2;
//	}
//
//	B2PointShopEventRewardHistoryPtr FB2MessageInfoConverter::ToB2PointShopEventRewardHistory(const b2::protocol::commondata::PointShopEventRewardHistory& pb)
//	{
//		auto b2 = std::make_shared<B2PointShopEventRewardHistory>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->total_reward_count = pb.total_reward_count();
//
//		return b2;
//	}
//
//	B2mdStageClearEventPtr FB2MessageInfoConverter::ToB2mdStageClearEvent(const b2::protocol::masterdata::mdStageClearEvent& pb)
//	{
//		auto b2 = std::make_shared<B2mdStageClearEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->stage = pb.stage();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2FriendInviteRewardInfoPtr FB2MessageInfoConverter::ToB2FriendInviteRewardInfo(const b2::protocol::commondata::FriendInviteRewardInfo& pb)
//	{
//		auto b2 = std::make_shared<B2FriendInviteRewardInfo>();
//
//		b2->platform_type = pb.platform_type();
//		b2->invite_count = pb.invite_count();
//		b2->joiner_count = pb.joiner_count();
//		b2->invite_reward_mask = pb.invite_reward_mask();
//		b2->joiner_reward_mask = pb.joiner_reward_mask();
//
//		return b2;
//	}
//
//	B2mdKakaoFriendInviteRewardPtr FB2MessageInfoConverter::ToB2mdKakaoFriendInviteReward(const b2::protocol::masterdata::mdKakaoFriendInviteReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdKakaoFriendInviteReward>();
//
//		b2->invite_reward_type = pb.invite_reward_type();
//		b2->reward_index = pb.reward_index();
//		b2->goal_count = pb.goal_count();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2mdHotTimeProductPtr FB2MessageInfoConverter::ToB2mdHotTimeProduct(const b2::protocol::masterdata::mdHotTimeProduct& pb)
//	{
//		auto b2 = std::make_shared<B2mdHotTimeProduct>();
//
//		b2->buff_type = pb.buff_type();
//		b2->period_hour = pb.period_hour();
//		b2->price = pb.price();
//		b2->increaseRate = pb.increase_rate();
//
//		return b2;
//	}
//
//	B2mdMagicShopProductPtr FB2MessageInfoConverter::ToB2mdMagicShopProduct(const b2::protocol::masterdata::mdMagicShopProduct& pb)
//	{
//		auto b2 = std::make_shared<B2mdMagicShopProduct>();
//
//		b2->id = pb.id();
//		b2->magic_shop_group_type = pb.magic_shop_group_type();
//		b2->product_type = pb.product_type();
//		b2->product_detail = pb.product_detail();
//		b2->product_count = pb.product_count();
//		b2->price_type = pb.price_type();
//		b2->price = pb.price();
//
//		return b2;
//	}
//
//	B2mdMagicShopRenewalCostPtr FB2MessageInfoConverter::ToB2mdMagicShopRenewalCost(const b2::protocol::masterdata::mdMagicShopRenewalCost& pb)
//	{
//		auto b2 = std::make_shared<B2mdMagicShopRenewalCost>();
//
//		b2->renewal_count = pb.renewal_count();
//		b2->gem_cost = pb.gem_cost();
//
//		return b2;
//	}
//
//	B2RollingBannerPtr FB2MessageInfoConverter::ToB2RollingBanner(const b2::protocol::commondata::RollingBanner& pb)
//	{
//		auto b2 = std::make_shared<B2RollingBanner>();
//
//		b2->id = pb.id();
//
//		switch (pb.type()) {
//		case 1 :
//			b2->type = b2network::B2RollingBannerType::PACKAGE;
//			break;
//		case 2 :
//			b2->type = b2network::B2RollingBannerType::EVENT;
//			break;
//		case 3 :
//			b2->type = b2network::B2RollingBannerType::ATTENDANCE_EVENT;
//			break;
//		default:
//			b2->type = 0;
//		}
//
//		b2->sub_type = pb.sub_type();
//		b2->banner_img_url = UTF8_TO_TCHAR(pb.banner_img_url().c_str());
//		b2->web_url = UTF8_TO_TCHAR(pb.web_url().c_str());
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//		b2->banner_duration = pb.banner_duration();
//
//		return b2;
//	}
//
//	B2MarketPurchaseInfoPtr FB2MessageInfoConverter::ToB2MarketPurchaseInfo(const b2::protocol::commondata::MarketPurchaseInfo& pb)
//	{
//		auto b2 = std::make_shared<B2MarketPurchaseInfo>();
//
//		b2->product_id = pb.product_id();
//		b2->market_product_id = UTF8_TO_TCHAR(pb.market_product_id().c_str());
//		b2->developer_payload = UTF8_TO_TCHAR(pb.developer_payload().c_str());
//		b2->purchase_state = ToB2MarketPurchaseState(pb.purchase_state());
//		b2->bonus_increase_rate = pb.bonus_increase_rate();
//
//		return b2;
//	}
//
//	B2mdMarketProductPtr FB2MessageInfoConverter::ToB2mdMarketProduct(const b2::protocol::masterdata::mdMarketProduct& pb)
//	{
//		auto b2 = std::make_shared<B2mdMarketProduct>();
//
//		b2->shop_product_id = pb.shop_product_id();
//		b2->market_product_id = UTF8_TO_TCHAR(pb.market_product_id().c_str());
//		b2->currency_code = UTF8_TO_TCHAR(pb.currency_code().c_str());
//		b2->price = pb.price();
//
//		return b2;
//	}
//
//	B2FlatRatePackageBonusStatePtr FB2MessageInfoConverter::ToB2FlatRatePackageBonusState(const b2::protocol::commondata::Shop::FlatRatePackageBonusState& pb)
//	{
//		auto b2 = std::make_shared<B2FlatRatePackageBonusState>();
//
//		b2->current_index = pb.current_index();
//		b2->weekly_state = pb.weekly_state();
//		b2->monthly_state = pb.monthly_state();
//		b2->reward_state = pb.reward_state();
//
//		return b2;
//	}
//
//	b2network::B2mdDuelModeSettleSchedulePtr FB2MessageInfoConverter::ToB2mdDuelModeSettleSchedule(const b2::protocol::masterdata::mdDuelModeSettleSchedule& pb)
//	{
//		auto b2 = std::make_shared<B2mdDuelModeSettleSchedule>();
//
//		b2->mode_type = pb.mode_type();
//		b2->ranking_settle_state = pb.ranking_settle_state();
//		b2->day = pb.day();
//		b2->end_hour = pb.end_hour();
//		b2->end_min = pb.end_min();
//
//		return b2;
//	}
//
//	B2PeriodicPackageStatePtr FB2MessageInfoConverter::ToB2PeriodicPackageState(const b2::protocol::commondata::Shop::PeriodicPackageState& pb)
//	{
//		auto b2 = std::make_shared<B2PeriodicPackageState>();
//
//		b2->package_id = pb.package_id();
//		b2->end_pay_time = pb.end_pay_time();
//
//		return b2;
//	}
//
//	B2LevelUpPackageMissionPtr FB2MessageInfoConverter::ToB2LevelUpPackageMission(const b2::protocol::commondata::Shop::LevelUpPackageMission& pb)
//	{
//		auto b2 = std::make_shared<B2LevelUpPackageMission>();
//
//		b2->package_id = pb.package_id();
//		b2->character_type = pb.character_type();
//		b2->level = pb.level();
//		b2->state = pb.state();
//
//		return b2;
//	}
//
//	B2ReachLevelPackageMissionPtr FB2MessageInfoConverter::ToB2ReachLevelPackageMission(const b2::protocol::commondata::Shop::ReachLevelPackageMission& pb)
//	{
//		auto b2 = std::make_shared<B2ReachLevelPackageMission>();
//
//		b2->package_id = pb.package_id();
//		b2->character_type = pb.character_type();
//		b2->level = pb.level();
//		b2->state = pb.state();
//		b2->expire_time = pb.expire_time();
//
//		return b2;
//	}
//
//	B2mdShopLevelUpPackagePtr FB2MessageInfoConverter::ToB2mdShopLevelUpPackage(b2::protocol::masterdata::mdShopLevelUpPackage& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopLevelUpPackage>();
//
//		b2->package_id = pb.package_id();
//		b2->character_type = pb.character_type();
//		b2->level = pb.level();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2mdShopReachLevelPackagePtr FB2MessageInfoConverter::ToB2mdShopReachLevelPackage(b2::protocol::masterdata::mdShopReachLevelPackage& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopReachLevelPackage>();
//
//		b2->package_id = pb.package_id();
//		b2->character_type = pb.character_type();
//		b2->reach_level = pb.reach_level();
//		b2->limit_level = pb.limit_level();
//		b2->buy_reward_id = pb.buy_reward_id();
//		b2->buy_reward_count = pb.buy_reward_count();
//		b2->complete_reward_id = pb.complete_reward_id();
//		b2->complete_reward_count = pb.complete_reward_count();
//		b2->expire_day = pb.expire_day();
//
//		return b2;
//	}
//
//	B2mdShopFlatRatePackageBonusPtr FB2MessageInfoConverter::ToB2mdShopFlatRatePackageBonus(b2::protocol::masterdata::mdShopFlatRatePackageBonus& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopFlatRatePackageBonus>();
//
//		b2->index = pb.index();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2mdShopPackageTabPtr FB2MessageInfoConverter::ToB2mdShopPackageTab(b2::protocol::masterdata::mdShopPackageTab& pb)
//	{
//		auto b2 = std::make_shared<B2mdShopPackageTab>();
//
//		b2->tab_type = pb.tab_type();
//		b2->sort_order = pb.sort_order();
//		b2->is_open = pb.is_open();
//
//		return b2;
//	}
//
//	B2UnitySkillPtr FB2MessageInfoConverter::ToB2UnitySkill(b2::protocol::commondata::UnitySkill& pb)
//	{
//		auto b2 = std::make_shared<B2UnitySkill>();
//
//		b2->main_character_type = pb.main_character_type();
//		b2->unity_character_type = pb.unity_character_type();
//		b2->step = pb.step();
//
//		return b2;
//	}
//
//	b2network::B2mdUnitySkillOptionPtr FB2MessageInfoConverter::ToB2mdUnitySkillOption(b2::protocol::masterdata::mdUnitySkillOption& pb)
//	{
//		auto b2 = std::make_shared<B2mdUnitySkillOption>();
//
//		b2->main_character_type = pb.main_character_type();
//		b2->unity_character_type = pb.unity_character_type();
//		b2->step = pb.step();
//		b2->option_id = pb.option_id();
//		b2->option_value = pb.option_value();
//		b2->power = pb.power();
//
//		return b2;
//	}
//
//	b2network::B2BuyShopResultInfoPtr FB2MessageInfoConverter::ToB2BuyShopResultInfo(const b2::protocol::commondata::Shop::BuyResultInfo& pb)
//	{
//		auto b2 = std::make_shared<B2BuyShopResultInfo>();
//
//		for (auto addProduct : pb.add_products())
//		{
//			b2->add_products.Emplace(ToB2ShopProduct(addProduct));
//		}
//
//		for (auto buyHistory : pb.shop_buy_manages())
//		{
//			b2->shop_buy_manages.Emplace(ToB2ShopBuyManage(buyHistory));
//		}
//
//		b2->buy_product_type = pb.buy_product_type();
//
//		b2->periodic_package_state = ToB2PeriodicPackageState(pb.periodic_package_state());
//		b2->flat_rate_package_bonus_state = ToB2FlatRatePackageBonusState(pb.flat_rate_package_bonus_state());
//		for (auto mission : pb.level_up_package_missions())
//		{
//			b2->level_up_package_missions.Emplace(ToB2LevelUpPackageMission(mission));
//		}
//		b2->reach_level_package_mission = ToB2ReachLevelPackageMission(pb.reach_level_package_mission());
//
//		b2->mileage_type = PbShopMileageTypeToB2ShopMileageType(pb.mileage_type());
//		b2->current_mileage = pb.current_mileage();
//
//		for (auto costume : pb.costumes())
//		{
//			b2->costumes.Emplace(CostumePb2B2CostumeServerInfo(costume));
//		}
//
//		for (auto totem : pb.totems())
//		{
//			b2->totems.Emplace(TotemPb2B2TotemServerInfo(totem));
//		}
//
//		return b2;
//	}
//
//	b2network::B2ContentsModeInfoPtr FB2MessageInfoConverter::ToB2ContentsModeInfo(b2::protocol::commondata::ContentsModeInfo& pb)
//	{
//		auto b2 = std::make_shared<B2ContentsModeInfo>();
//
//		b2->mode = pb.mode();
//		b2->state = pb.state();
//
//		return b2;
//	}
//
//	b2network::B2mdWebUrlPtr FB2MessageInfoConverter::ToB2B2mdWebUrl(b2::protocol::masterdata::mdWebUrl& pb)
//	{
//		auto b2 = std::make_shared<B2mdWebUrl>();
//
//		b2->key = UTF8_TO_TCHAR(pb.key().c_str());
//		b2->url = UTF8_TO_TCHAR(pb.url().c_str());
//
//		return b2;
//	}
//
//	B2BlockChatUserPtr FB2MessageInfoConverter::ToB2BlockChatUser(b2::protocol::commondata::BlockChatUser pb)
//	{
//		auto b2 = std::make_shared<B2BlockChatUser>();
//
//		b2->account_id = pb.account_id();
//		b2->nickname = UTF8_TO_TCHAR(pb.nickname().c_str());
//
//		return b2;
//	}
//
//	B2TenLotteryEventStatusPtr FB2MessageInfoConverter::ToB2TenLotteryEventStatus(b2::protocol::commondata::TenLotteryEventStatus pb)
//	{
//		auto b2 = std::make_shared<B2TenLotteryEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->lottery_count = pb.lottery_count();
//
//		for (auto index : pb.rewarded_indexs()) 
//		{
//			b2->rewarded_indexs.Emplace(index);
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdTenLotteryEventPtr FB2MessageInfoConverter::ToB2mdTenLotteryEvent(b2::protocol::masterdata::mdTenLotteryEvent pb)
//	{
//		auto b2 = std::make_shared<B2mdTenLotteryEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->goal_lottery_count = pb.goal_lottery_count();
//
//		for (auto reward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2RawReward(reward));
//		}
//
//		return b2;
//	}
//
//	B2SpendGemEventStatusPtr FB2MessageInfoConverter::ToB2SpendGemEvemtStatus(b2::protocol::commondata::SpendGemEventStatus pb)
//	{
//		auto b2 = std::make_shared<B2SpendGemEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->spend_gem_count = pb.spend_gem_count();
//		b2->reward_state_value = pb.reward_state_value();
//
//		return b2;
//	}
//
//	B2mdSpendGemEventPtr FB2MessageInfoConverter::ToB2mdSpendGemEvent(b2::protocol::masterdata::mdSpendGemEvent pb)
//	{
//		auto b2 = std::make_shared<B2mdSpendGemEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->goal_spend_count = pb.goal_spend_count();
//
//		for (auto reward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2RawReward(reward));
//		}
//
//		return b2;
//	}
//
//	B2GemPurchaseEventStatusPtr FB2MessageInfoConverter::ToB2GemPurchaseEventStatus(b2::protocol::commondata::GemPurchaseEventStatus pb)
//	{
//		auto b2 = std::make_shared<B2GemPurchaseEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->gem_count = pb.gem_count();
//		b2->reward_state_value = pb.reward_state_value();
//
//		return b2;
//	}
//
//	B2mdGemPurchaseEventRewardPtr FB2MessageInfoConverter::ToB2mdGemPurchaseEventReward(b2::protocol::masterdata::mdGemPurchaseEventReward pb)
//	{
//		auto b2 = std::make_shared<B2mdGemPurchaseEventReward>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->goal_count = pb.goal_count();
//
//		b2->rewards.Emplace(ToB2RawReward(pb.reward_id1(), pb.reward_count1()));
//		b2->rewards.Emplace(ToB2RawReward(pb.reward_id2(), pb.reward_count2()));
//		b2->rewards.Emplace(ToB2RawReward(pb.reward_id3(), pb.reward_count3()));
//
//		return b2;
//	}
//
//	B2TenLotterySavingsStatusPtr FB2MessageInfoConverter::ToB2TenLotterySavingsStatus(b2::protocol::commondata::TenLotterySavingsStatus pb)
//	{
//		auto b2 = std::make_shared<B2TenLotterySavingsStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->current_point = pb.current_point();
//
//		return b2;
//	}
//
//	B2mdTenLotterySavingsEventPtr FB2MessageInfoConverter::ToB2mdTenLotterySavingsEvent(b2::protocol::masterdata::mdTenLotterySavingsEvent pb)
//	{
//		auto b2 = std::make_shared<B2mdTenLotterySavingsEvent>();
//
//		b2->event_id = pb.event_id();
//		b2->need_point = pb.need_point();
//		b2->gain_point = pb.gain_point();
//
//		for (auto reward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2RawReward(reward));
//		}
//
//		return b2;
//	}
//
//	b2network::B2mdItemLevelupFactorPtr FB2MessageInfoConverter::ToB2mdItemLevelupFactor(b2::protocol::masterdata::mdItemLevelupFactor pb)
//	{
//		auto b2 = std::make_shared<B2mdItemLevelupFactor>();
//
//		b2->level = pb.level();
//		b2->factor = pb.factor();
//
//		return b2;
//	}
//
//	B2DiceEventStatusPtr FB2MessageInfoConverter::ToB2DiceEventStatus(b2::protocol::commondata::DiceEventStatus pb)
//	{
//		auto b2 = std::make_shared<B2DiceEventStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->point = pb.point();
//		b2->free_dice = pb.free_dice();
//		b2->position = pb.position();
//		b2->finish_count = pb.finish_count();
//		b2->buy_point_count = pb.buy_point_count();
//		b2->roll_dice_count = pb.roll_dice_count();
//		b2->mileage_reward_value = pb.mileage_reward_value();
//
//		for (auto mission : pb.missions())
//		{
//			auto b2Mission = std::make_shared<B2DiceEventMission>();
//
//			b2Mission->event_type = mission.event_type();
//			b2Mission->progress_count = mission.progress_count();
//			b2Mission->state = mission.state();
//
//			b2->missions.Emplace(b2Mission);
//		}
//
//		return b2;
//	}
//
//	B2mdDiceEventMissionPtr FB2MessageInfoConverter::ToB2mdDiceEventMission(b2::protocol::masterdata::mdDiceEventMission pb)
//	{
//		auto b2 = std::make_shared<B2mdDiceEventMission>();
//
//		b2->event_id = pb.event_id();
//		b2->event_type = pb.event_type();
//		b2->goal_count = pb.goal_count();
//		b2->point = pb.point();
//
//		return b2;
//	}
//
//	B2mdDiceEventCellPtr FB2MessageInfoConverter::ToB2mdDiceEventCell(b2::protocol::masterdata::mdDiceEventCell pb)
//	{
//		auto b2 = std::make_shared<B2mdDiceEventCell>();
//
//		b2->event_id = pb.event_id();
//		b2->cell_number = pb.cell_number();
//		b2->cell_type = pb.cell_type();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//		b2->effect_value = pb.effect_value();
//
//		return b2;
//	}
//
//	B2mdDiceEventFinishRewardPtr FB2MessageInfoConverter::ToB2mdDiceEventFinishReward(b2::protocol::masterdata::mdDiceEventFinishReward pb)
//	{
//		auto b2 = std::make_shared<B2mdDiceEventFinishReward>();
//
//		b2->event_id = pb.event_id();
//		b2->number = pb.number();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2SoldItemInfoPtr FB2MessageInfoConverter::ToB2SoldItemInfo(b2::protocol::session::ClearStageResponse::SoldItemInfo pb)
//	{
//		auto b2 = std::make_shared<B2SoldItemInfo>();
//
//		b2->template_id = pb.template_id();
//		b2->quality = pb.quality();
//
//		return b2;
//	}
//
//	B2SoldAetherInfoPtr FB2MessageInfoConverter::ToB2SoldAetherInfo(b2::protocol::session::ClearStageResponse::SoldAetherInfo pb)
//	{
//		auto b2 = std::make_shared<B2SoldAetherInfo>();
//
//		b2->aether_grade = pb.aether_grade();
//		b2->tier = ToB2AetherTier(pb.tier());
//
//		return b2;
//	}
//
//	B2mdDiceEventBuyPointPtr FB2MessageInfoConverter::ToB2mdDiceEventBuyPoint(b2::protocol::masterdata::mdDiceEventBuyPoint pb)
//	{
//		auto b2 = std::make_shared<B2mdDiceEventBuyPoint>();
//
//		b2->event_id = pb.event_id();
//		b2->buy_count = pb.buy_count();
//		b2->gem = pb.gem();
//		b2->point = pb.point();
//
//		return b2;
//	}
//
//	B2mdDiceEventMileagePtr FB2MessageInfoConverter::ToB2mdDiceEventMileage(b2::protocol::masterdata::mdDiceEventMileage pb)
//	{
//		auto b2 = std::make_shared<B2mdDiceEventMileage>();
//
//		b2->event_id = pb.event_id();
//		b2->index = pb.index();
//		b2->dice_count = pb.dice_count();
//
//		b2->rewards.Emplace(ToB2RawReward(pb.reward_id1(), pb.reward_count1()));
//		b2->rewards.Emplace(ToB2RawReward(pb.reward_id2(), pb.reward_count2()));
//		b2->rewards.Emplace(ToB2RawReward(pb.reward_id3(), pb.reward_count3()));
//
//		return b2;
//	}
//
//	B2DonationPointRewardStatePtr FB2MessageInfoConverter::ToB2DonationPointRewardState(b2::protocol::commondata::DonationPointRewardState pb)
//	{
//		auto b2 = std::make_shared<B2DonationPointRewardState>();
//
//		b2->index = pb.index();
//		b2->rewarded = pb.rewarded();
//
//		return b2;
//	}
//
//	B2DonationPresentPtr FB2MessageInfoConverter::ToB2DonationPresent(b2::protocol::commondata::DonationPresent pb)
//	{
//		auto b2 = std::make_shared<B2DonationPresent>();
//
//		b2->item_template_id = pb.item_template_id();
//		b2->amount = pb.amount();
//		b2->donation_consumable_type = ToB2DonationConsumableType(pb.donation_consumable_type());
//
//		return b2;
//	}
//
//	B2mdDonationEventInfoPtr FB2MessageInfoConverter::ToB2mdDonationEventInfo(b2::protocol::masterdata::mdDonationEventInfo pb)
//	{
//		auto b2 = std::make_shared<B2mdDonationEventInfo>();
//
//		b2->donation_id = pb.donation_open_id();
//		b2->donation_template_id = pb.donation_template_id();
//		b2->cost_type = pb.cost_type();
//		b2->cost = pb.cost();
//		b2->extra_cost = pb.extra_cost();
//		b2->cut_off_point = pb.cut_off_point();
//		b2->obtain_point = pb.obtain_point();
//		b2->bundle_cost = pb.bundle_cost();
//		b2->extra_bundle_cost = pb.extra_bundle_cost();
//		b2->bundle_obtain_point = pb.bundle_obtain_point();
//		b2->serial_donation_count = pb.serial_donation_count();
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//		b2->expire_time = pb.expire_time();
//		b2->title = UTF8_TO_TCHAR(pb.title().c_str());
//		b2->description = UTF8_TO_TCHAR(pb.description().c_str());
//		b2->banner_url = pb.banner_url().c_str();
//		b2->link_text = UTF8_TO_TCHAR(pb.link_text().c_str());
//		b2->link_url = pb.link_url().c_str();
//
//		return b2;
//	}
//
//	B2mdDonationPointRankingRewardInfoPtr FB2MessageInfoConverter::ToB2mdDonationPointRankingRewardInfo(b2::protocol::masterdata::mdDonationPointRankingRewardInfo pb)
//	{
//		auto b2 = std::make_shared<B2mdDonationPointRankingRewardInfo>();
//
//		b2->donation_id = pb.donation_id();
//		b2->rank = pb.rank();
//		b2->reward_id1 = pb.reward_id1();
//		b2->reward_count1 = pb.reward_count1();
//		b2->reward_id2 = pb.reward_id2();
//		b2->reward_count2 = pb.reward_count2();
//		b2->reward_id3 = pb.reward_id3();
//		b2->reward_count3 = pb.reward_count3();
//		b2->reward_id4 = pb.reward_id4();
//		b2->reward_count4 = pb.reward_count4();
//		b2->reward_id5 = pb.reward_id5();
//		b2->reward_count5 = pb.reward_count5();
//
//		return b2;
//	}
//
//	B2mdDonationBundleFixedRewardInfoPtr FB2MessageInfoConverter::ToB2mdDonationBundleFixedRewardInfo(b2::protocol::masterdata::mdDonationBundleFixedRewardInfo pb)
//	{
//		auto b2 = std::make_shared<B2mdDonationBundleFixedRewardInfo>();
//
//		b2->donation_id = pb.donation_id();
//		b2->reward_id = pb.reward_id();
//		b2->min_amount = pb.min_amount();
//		b2->max_amount = pb.max_amount();
//
//		return b2;
//	}
//
//	B2mdDonationPointStepRewardInfoPtr FB2MessageInfoConverter::ToB2mdDonationPointStepRewardInfo(b2::protocol::masterdata::mdDonationPointStepRewardInfo pb)
//	{
//		auto b2 = std::make_shared<B2mdDonationPointStepRewardInfo>();
//
//		b2->donation_id = pb.donation_id();
//		b2->step = pb.step();
//		b2->point = pb.point();
//		b2->reward_id1 = pb.reward_id1();
//		b2->reward_count1 = pb.reward_count1();
//		b2->reward_id2 = pb.reward_id2();
//		b2->reward_count2 = pb.reward_count2();
//		b2->reward_id3 = pb.reward_id3();
//		b2->reward_count3 = pb.reward_count3();
//
//		return b2;
//	}
//
//	B2mdFairyInfoPtr FB2MessageInfoConverter::ToB2mdFairyInfo(b2::protocol::masterdata::mdFairyInfo pb)
//	{
//		auto b2 = std::make_shared<B2mdFairyInfo>();
//
//		b2->fairy_type = ToB2FairyType(pb.fairy_type());
//		b2->option_id = pb.option_id();
//		b2->bless_level = pb.bless_level();
//		b2->option_value = pb.option_value();
//		b2->level = pb.level();
//
//		return b2;
//	}
//
//	B2mdFairyLevelupCostPtr FB2MessageInfoConverter::ToB2mdFairyLevelupCost(b2::protocol::masterdata::mdFairyLevelupCost pb)
//	{
//		auto b2 = std::make_shared<B2mdFairyLevelupCost>();
//
//		b2->level = pb.level();
//		b2->material_cost = pb.material_cost();
//
//		return b2;
//	}
//
//	B2mdFairyOpenInfoPtr FB2MessageInfoConverter::ToB2mdFairyOpenInfo(b2::protocol::masterdata::mdFairyOpenInfo pb)
//	{
//		auto b2 = std::make_shared<B2mdFairyOpenInfo>();
//
//		b2->fairy_type = ToB2FairyType(pb.fairy_type());
//		b2->stage_id = pb.stage_id();
//
//		return b2;
//	}
//
//	b2network::B2mdSkillLevelUpPtr FB2MessageInfoConverter::ToB2mdSkillLevelUp(const b2::protocol::masterdata::mdSkillLevelUp& pb)
//	{
//		auto b2 = std::make_shared<B2mdSkillLevelUp>();
//
//		b2->skill_id = pb.skill_id();
//		b2->skill_level = pb.skill_level();
//		b2->req_skill_point = pb.req_skill_point();
//
//		return b2;
//	}
//
//	b2network::B2DuelRewardBoxInfoResponseRewardBoxPtr FB2MessageInfoConverter::ToB2DuelRewardBoxInfoResponseRewardBox(const b2::protocol::session::GetDuelRewardBoxInfoResponse::RewardBox& pb)
//	{
//		auto b2 = std::make_shared<B2DuelRewardBoxInfoResponseRewardBox>();
//
//		b2->grade = ToB2RewardBoxGrade(pb.grade());
//		b2->win_count = pb.win_count();
//
//		return b2;
//	}
//
//	b2network::B2DuelRewardBoxGainableListResponseRewardItemPtr FB2MessageInfoConverter::ToB2DuelRewardBoxGainableListResponseRewardItem(const b2::protocol::session::GetDuelRewardBoxGainableListResponse::RewardItem& pb)
//	{
//		auto b2 = std::make_shared<B2DuelRewardBoxGainableListResponseRewardItem>();
//
//		b2->reward_id = pb.reward_id();
//		b2->min_count = pb.min_count();
//		b2->max_count = pb.max_count();
//
//		return b2;
//	}
//
//	b2network::B2CharacterPowerSpecificPtr FB2MessageInfoConverter::ToB2CharacterPowerSpecific(const b2::protocol::session::TrackingPowerDetailRequest::CharacterPowerSpecific& pb)
//	{
//		auto b2 = std::make_shared<B2CharacterPowerSpecific>();
//
//		b2->category = pb.category();
//		b2->power = pb.power();
//
//		return b2;
//	}
//
//	b2network::B2mdAssaultVictoryRewardInfoPtr FB2MessageInfoConverter::ToB2mdAssaultVictoryReward(const b2::protocol::masterdata::mdAssaultVictoryRewardInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAssaultVictoryRewardInfo>();
//
//		b2->match_result = pb.match_result();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2mdAssaultMvpRewardInfoPtr FB2MessageInfoConverter::ToB2mdAssaultMvpReward(const b2::protocol::masterdata::mdAssaultMvpRewardInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAssaultMvpRewardInfo>();
//
//		b2->reward_id = pb.reward_id();
//		b2->reward_min_count = pb.reward_min_count();
//		b2->reward_max_count = pb.reward_max_count();
//
//		return b2;
//	}
//
//	B2mdEventAttendancePtr FB2MessageInfoConverter::ToB2mdEventAttendance(const b2::protocol::masterdata::mdEventAttendance& pb)
//	{
//		auto b2 = std::make_shared<B2mdEventAttendance>();
//		b2->event_id = pb.event_id();
//		b2->title = UTF8_TO_TCHAR(pb.title().c_str());
//		b2->total_attendance_days = pb.total_attendance_days();
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//		b2->explain = UTF8_TO_TCHAR(pb.explain().c_str());
//		b2->event_url = UTF8_TO_TCHAR(pb.event_url().c_str());
//		b2->bonus_type = ToB2EventAttanceBonusType(pb.bonus_type());
//
//		return b2;
//	}
//
//	B2mdEventAttendanceRewardPtr FB2MessageInfoConverter::ToB2mdEventAttendanceReward(const b2::protocol::masterdata::mdEventAttendanceReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdEventAttendanceReward>();
//
//		b2->event_id = pb.event_id();
//		b2->day = pb.day();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2mdDailyPlayTimeRewardPtr FB2MessageInfoConverter::ToB2mdDailyPlayTimeReward(const b2::protocol::masterdata::mdDailyPlayTimeReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdDailyPlayTimeReward>();
//
//		b2->reward_id = pb.reward_id();
//		b2->play_time_inmin = pb.play_time_inmin();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	B2EventAttendanceStatusPtr FB2MessageInfoConverter::ToB2EventAttendanceStatus(const b2::protocol::commondata::EventAttendanceStatus& pb)
//	{
//		auto b2 = std::make_shared<B2EventAttendanceStatus>();
//
//		b2->event_id = pb.event_id();
//		b2->attendance_day = pb.attendance_day();
//		b2->last_attendance_time = pb.last_attendance_time();
//		b2->next_attendance_time = pb.next_attendance_time();
//
//		return b2;
//	}
//
//	B2BeginnerAttendanceStatusPtr FB2MessageInfoConverter::ToB2BeginnerAttendanceStatus(const b2::protocol::commondata::BeginnerAttendanceStatus& pb)
//	{
//		auto b2 = std::make_shared<B2BeginnerAttendanceStatus>();
//
//		b2->total_attendance_days = pb.total_attendance_days();
//		b2->attendance_day = pb.attendance_day();
//		b2->last_attendance_time = pb.last_attendance_time();
//		b2->next_attendance_time = pb.next_attendance_time();
//		b2->finish_time = pb.finish_time();
//
//		return b2;
//	}
//
//	B2GuildInfoPtr FB2MessageInfoConverter::ToB2ChatGuild(const b2::protocol::commondata::GuildInfo& pb)
//	{
//		auto b2 = std::make_shared<B2GuildInfo>();
//
//		b2->guild_id = pb.guild_id();
//		b2->guild_name = UTF8_TO_TCHAR(pb.guild_name().c_str());
//		b2->mark_index = pb.mark_index();
//		b2->mark_color = pb.mark_color();
//		b2->mark_bg_index = pb.mark_bg_index();
//		b2->mark_bg_color = pb.mark_bg_color();
//
//		return b2;
//	}
//
//	B2mdTutorialInfoPtr FB2MessageInfoConverter::ToB2mdTutorialInfo(const b2::protocol::masterdata::mdTutorialInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTutorialInfo>();
//
//		b2->tutorial_id = pb.tutorial_id();
//		b2->prev_tutorial_id = pb.prev_tutorial_id();
//		b2->character_level = pb.character_level();
//		b2->act_cleared = pb.act_cleared();
//		b2->cash = pb.cash();
//		b2->gold = pb.gold();
//		b2->reward_id1 = pb.reward_id1();
//		b2->reward_count1 = pb.reward_count1();
//		b2->reward_id2 = pb.reward_id2();
//		b2->reward_count2 = pb.reward_count2();
//		b2->reward_id3 = pb.reward_id3();
//		b2->reward_count3 = pb.reward_count3();
//
//		return b2;
//	}
//
//	B2mdDuelModeRewardPtr FB2MessageInfoConverter::ToB2mdDuelModeReward(const b2::protocol::masterdata::mdDuelModeReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdDuelModeReward>();
//
//		b2->duel_mode = pb.duel_mode();
//		b2->match_result = pb.match_result();
//		for (auto reward : pb.rewards())
//		{
//			b2->rewards.Emplace(ToB2Reward(reward));
//		}
//
//		return b2;
//	}
//
//
//	B2mdRaidExpPtr FB2MessageInfoConverter::ToB2mdRaidExp(const b2::protocol::masterdata::mdRaidExp& pb)
//	{
//		auto b2 = std::make_shared<B2mdRaidExp>();
//
//		b2->raid_type = pb.raid_type();
//		b2->step = pb.step();
//		b2->exp = pb.exp();
//
//		return b2;
//	}
//
//	B2mdAetherOptionInfoPtr FB2MessageInfoConverter::ToB2mdAetherOptionInfo(const b2::protocol::masterdata::mdAetherOptionInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAetherOptionInfo>();
//
//		b2->option_id = pb.option_id();
//		b2->option_group = pb.option_group();
//		b2->grade = pb.grade();
//		b2->default_value = pb.default_value();
//		b2->increase_value_per_level = pb.increase_value_per_level();
//
//		return b2;
//	}
//
//	B2mdAetherSlotInfoPtr FB2MessageInfoConverter::ToB2mdAetherSlotInfo(const b2::protocol::masterdata::mdAetherSlotInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAetherSlotInfo>();
//
//		b2->slot_id = pb.slot_id();
//		b2->option_id = pb.option_id();
//
//		return b2;
//	}
//
//	B2mdAetherEnhanceInfoPtr FB2MessageInfoConverter::ToB2mdAetherEnhanceInfo(const b2::protocol::masterdata::mdAetherEnhanceInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAetherEnhanceInfo>();
//
//		b2->enhance_level = pb.enhance_level();
//		b2->grade = pb.grade();
//		b2->aether_piece_cost = pb.aether_piece_cost();
//		b2->enhance_rate = pb.enhance_rate();
//		b2->dismantle_amount = pb.dismantle_amount();
//		b2->sell_price = pb.sell_price();
//
//		return b2;
//	}
//
//	B2mdAetherEnhanceOptionInfoPtr FB2MessageInfoConverter::ToB2mdAetherEnhanceOptionInfo(const b2::protocol::masterdata::mdAetherEnhanceOptionInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAetherEnhanceOptionInfo>();
//
//		b2->enhance_level = pb.enhance_level();
//		b2->tier = pb.tier();
//		b2->state_main_option = pb.state_main_option();
//		b2->state_sub_option = pb.state_sub_option();
//
//		return b2;
//	}
//
//	B2mdAetherSetOptionInfoPtr FB2MessageInfoConverter::ToB2mdAetherSetOptionInfo(const b2::protocol::masterdata::mdAetherSetOptionInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdAetherSetOptionInfo>();
//
//		b2->type = pb.type();
//		b2->tier = pb.tier();
//		b2->set_option_id = pb.set_option_id();
//		b2->min_apply_count = pb.min_apply_count();
//		b2->trigger_rate = pb.trigger_rate();
//		b2->cooldown_second = pb.cooldown_second();
//		b2->damage_rate = pb.damage_rate();
//
//		return b2;
//	}
//
//	b2network::B2GuildChatDataPtr FB2MessageInfoConverter::ToB2GuildChatData(const b2::protocol::session::GuildChatData& pb)
//	{
//		auto b2 = std::make_shared<B2GuildChatData>();
//
//		b2->id = pb.id();
//		b2->guild_id = pb.guild_id();
//		b2->account_id = pb.has_account_id() ? pb.account_id() : 0;
//		b2->nickname = pb.has_nickname() ? UTF8_TO_TCHAR(pb.nickname().c_str()) : UTF8_TO_TCHAR("");
//		b2->create_time = pb.create_time();
//		b2->chat_type = ToB2ChatType(pb.chat_type());
//		b2->message = pb.has_message() ? UTF8_TO_TCHAR(pb.message().c_str()) : UTF8_TO_TCHAR("");
//		b2->chat_notice_type = ToB2ChatNotifyType(pb.chat_notice_type());
//		b2->character_level_max = pb.has_character_level_max() ? pb.character_level_max() : 0;
//
//		for (auto notice_arg : pb.notice_args())
//		{
//			b2->notice_args.Emplace(UTF8_TO_TCHAR(notice_arg.c_str()));
//		}
//
//		b2->support_item_template_id = pb.has_support_item_template_id() ? pb.support_item_template_id() : 0;
//		b2->support_count = pb.has_support_count() ? pb.support_count() : 0;
//		b2->support_max_count = pb.has_support_max_count() ? pb.support_max_count() : 0;
//		b2->update_time = pb.has_update_time() ? pb.update_time() : 0;
//		b2->expire_time = pb.has_expire_time() ? pb.expire_time() : 0;
//
//		return b2;
//	}
//
//	b2network::B2mdGuildSupportItemPtr FB2MessageInfoConverter::ToB2mdGuildSupportItem(const b2::protocol::masterdata::mdGuildSupportItem& pb)
//	{
//		auto b2 = std::make_shared<B2mdGuildSupportItem>();
//
//		b2->item_template_id = pb.item_template_id();
//		b2->support_max_count = pb.support_max_count();
//
//		return b2;
//	}
//
//	b2network::B2mdUnitySkillMissionPtr FB2MessageInfoConverter::ToB2mdUnitySkillMission(const b2::protocol::masterdata::mdUnitySkillMission& pb)
//	{
//		auto b2 = std::make_shared<B2mdUnitySkillMission>();
//
//		b2->main_character_type = pb.main_character_type();
//		b2->unity_character_type = pb.unity_character_type();
//		b2->step = pb.step();
//		b2->index = pb.index();
//		b2->event_type = pb.event_type();
//		b2->condition1 = pb.condition1();
//		b2->condition2 = pb.condition2();
//		b2->condition3 = pb.condition3();
//		b2->condition4 = pb.condition4();
//		b2->goal_count = pb.goal_count();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count = pb.reward_count();
//
//		return b2;
//	}
//
//	b2network::B2mdUnitySkillAwakenMissionPtr FB2MessageInfoConverter::ToB2mdUnitySkillAwakenMission(const b2::protocol::masterdata::mdUnitySkillAwakenMission& pb)
//	{
//		auto b2 = std::make_shared<B2mdUnitySkillAwakenMission>();
//
//		b2->character_type = pb.character_type();
//		b2->mission_id = pb.mission_id();
//		b2->req_item_id_1 = pb.req_item_id_1();
//		b2->req_item_count_1 = pb.req_item_count_1();
//		b2->req_item_id_2 = pb.req_item_id_2();
//		b2->req_item_count_2 = pb.req_item_count_2();
//		b2->req_gold = pb.req_gold();
//		b2->success_rate = pb.success_rate();
//		b2->success_rate_add = pb.success_rate_add();
//
//		return b2;
//	}
//
//	B2UnitySkillMissionPtr FB2MessageInfoConverter::ToB2UnitySkillMission(const b2::protocol::commondata::UnitySkillMission& pb)
//	{
//		auto b2 = std::make_shared<B2UnitySkillMission>();
//		
//		b2->main_character_type = pb.main_character_type();
//		b2->unity_character_type = pb.unity_character_type();
//		b2->step = pb.step();
//		b2->state = pb.state();
//
//		return b2;
//	}
//
//	b2network::B2UnitySkillAwakenMissionPtr FB2MessageInfoConverter::ToB2UnitySkillAwakenMission(const b2::protocol::commondata::UnitySkillAwakenMission& pb)
//	{
//		auto b2 = std::make_shared<B2UnitySkillAwakenMission>();
//
//		b2->character_type = pb.character_type();
//		b2->mission_id = pb.mission_id();
//		b2->try_count = pb.try_count();
//		b2->completed = pb.completed();
//
//		return b2;
//	}
//
//	B2mdRestRewardPtr FB2MessageInfoConverter::ToB2mdRestReward(const b2::protocol::masterdata::mdRestReward& pb)
//	{
//		auto b2 = std::make_shared<B2mdRestReward>();
//		
//		b2->id = pb.id();
//		b2->reward_id = pb.reward_id();
//		b2->reward_count_per_point = pb.reward_count_per_point();
//
//		return b2;
//	}
//
//	B2mdTotemInfoPtr FB2MessageInfoConverter::ToB2mdTotemInfo(const b2::protocol::masterdata::mdTotemInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTotemInfo>();
//
//		b2->template_id = pb.template_id();
//		b2->group_id = pb.group_id();
//		b2->bonus_id = pb.bonus_id();
//		b2->grade = pb.grade();
//		b2->sell_price = pb.sell_price();
//		b2->equip_slot = B2TotemEquipSlot2PbTotemEquipSlot(pb.equip_slot());
//		b2->refine_limit_count1 = pb.refine_limit_count1();
//		b2->refine_limit_count2 = pb.refine_limit_count2();
//		b2->refine_limit_count3 = pb.refine_limit_count3();
//
//		return b2;
//	}
//
//	B2mdSelectTotemOptionPtr FB2MessageInfoConverter::ToB2mdSelectTotemOption(const b2::protocol::masterdata::mdSelectTotemOption& pb)
//	{
//		auto b2 = std::make_shared<B2mdSelectTotemOption>();
//
//		b2->option_id = pb.option_id();
//		b2->grade = pb.grade();
//		b2->min_main_value = pb.min_main_value();
//		b2->max_main_value = pb.max_main_value();
//		b2->min_sub_value = pb.min_sub_value();
//		b2->max_sub_value = pb.max_sub_value();
//		b2->decimal_point = pb.decimal_point();
//
//		return b2;
//	}
//
//	B2mdTotemRefineInfoPtr FB2MessageInfoConverter::ToB2mdTotemRefineInfo(const b2::protocol::masterdata::mdTotemRefineInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTotemRefineInfo>();
//
//		b2->grade = pb.grade();
//		b2->refine_chance = pb.refine_chance();
//		b2->gold_cost = pb.gold_cost();
//		b2->dimensional_piece_cost = pb.dimensional_piece_cost();
//
//		return b2;
//	}
//
//	B2mdTotemRefineRateInfoPtr FB2MessageInfoConverter::ToB2mdTotemRefineRateInfo(const b2::protocol::masterdata::mdTotemRefineRateInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTotemRefineRateInfo>();
//
//		b2->grade = pb.grade();
//		b2->base_rate = pb.base_rate();
//		b2->gold_decrease_rate = pb.gold_decrease_rate();
//		b2->gold_increase_rate = pb.gold_increase_rate();
//		b2->piece_decrease_rate = pb.piece_decrease_rate();
//		b2->piece_increase_rate = pb.piece_increase_rate();
//		b2->min_rate = pb.min_rate();
//		b2->max_rate = pb.max_rate();
//
//		return b2;
//	}
//
//	B2mdTotemDismantleInfoPtr FB2MessageInfoConverter::ToB2mdTotemDismantleInfo(const b2::protocol::masterdata::mdTotemDismantleInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTotemDismantleInfo>();
//
//		b2->refine_level = pb.refine_level();
//		b2->grade = pb.grade();
//		b2->amount = pb.amount();
//
//		return b2;
//	}
//
//	B2mdTotemBonusOptionInfoPtr FB2MessageInfoConverter::ToB2mdTotemBonusOptionInfo(const b2::protocol::masterdata::mdTotemBonusOptionInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTotemBonusOptionInfo>();
//
//		b2->bonus_id = pb.bonus_id();
//		b2->condition1 = pb.condition1();
//		b2->bonus_option_id1 = pb.bonus_option_id1();
//		b2->bonus_value1 = pb.bonus_value1();
//		b2->condition2 = pb.condition2();
//		b2->bonus_option_id2 = pb.bonus_option_id2();
//		b2->bonus_value2 = pb.bonus_value2();
//		b2->condition3 = pb.condition3();
//		b2->bonus_option_id3 = pb.bonus_option_id3();
//		b2->bonus_value3 = pb.bonus_value3();
//
//		return b2;
//	}
//
//	B2mdTotemPrefixLotteryInfoPtr FB2MessageInfoConverter::ToB2mdTotemPrefixLotteryInfo(const b2::protocol::masterdata::mdTotemPrefixLotteryInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdTotemPrefixLotteryInfo>();
//
//		b2->id = pb.id();
//		b2->base_prefix = pb.base_prefix();
//		b2->grade = pb.grade();
//
//		return b2;
//	}
//
//	B2TotemRefineStatePtr FB2MessageInfoConverter::ToB2TotemRefineState(const b2::protocol::commondata::TotemRefineState& pb)
//	{
//		auto b2 = std::make_shared<B2TotemRefineState>();
//
//		b2->index = pb.index();
//		b2->status = ToB2Refinestatus(pb.status());
//
//		return b2;
//	}
//
//	B2TotemSubOptionDetailPtr FB2MessageInfoConverter::ToB2TotemSubOptionDetail(const b2::protocol::commondata::TotemSubOptionDetail& pb)
//	{
//		auto b2 = std::make_shared<B2TotemSubOptionDetail>();
//
//		b2->option_id = pb.option_id();
//		b2->option_value = pb.option_value();
//		b2->refine_count = pb.refine_count();
//
//		for (auto refineState : pb.refine_states())
//		{
//			b2->refine_states.Emplace(ToB2TotemRefineState(refineState));
//		}
//
//		return b2;
//	}
//	
//	B2mdExchangeSystemInfoPtr FB2MessageInfoConverter::ToB2mdExchangeSystemInfo(const b2::protocol::masterdata::mdExchangeSystemInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdExchangeSystemInfo>();
//
//		b2->id = pb.id();
//		b2->template_id = pb.template_id();
//		b2->request_count = pb.request_count();
//		b2->exchange_template_id = pb.exchange_template_id();
//		b2->result_count = pb.result_count();
//
//		return b2;
//	}
//
//	B2mdExchangeSystemCostPtr FB2MessageInfoConverter::ToB2mdExchangeSystemCost(const b2::protocol::masterdata::mdExchangeSystemCost& pb)
//	{
//
//		auto b2 = std::make_shared<B2mdExchangeSystemCost>();
//		b2->exchange_id = pb.exchange_id();
//		b2->count = pb.count();
//		b2->gold_cost = pb.gold_cost();
//		b2->gem_cost = pb.gem_cost();
//
//		return b2;
//	}
//
//	B2mdQualityTransRateInfoPtr FB2MessageInfoConverter::ToB2mdQualityTransRateInfo(const b2::protocol::masterdata::mdQualityTransRateInfo& pb)
//	{
//
//		auto b2 = std::make_shared<B2mdQualityTransRateInfo>();
//		b2->template_id = pb.template_id();
//		b2->equip_category= pb.equip_category();
//		b2->quality= pb.quality();
//		b2->trans_rate = pb.trans_rate();
//		b2->trans_cond_quality= pb.trans_cond_quality();
//
//		return b2;
//	}
//
//	B2mdLotteryShopRateQualityInfoPtr FB2MessageInfoConverter::ToB2mdLotteryShopRateQualityInfo(const b2::protocol::masterdata::mdLotteryShopRateQualityInfo& pb)
//	{
//
//		auto b2 = std::make_shared<B2mdLotteryShopRateQualityInfo>();
//		b2->equip_category = pb.equip_category();
//		b2->quality = pb.quality();
//		b2->rate = pb.rate();
//		b2->quality_inc_level = pb.quality_inc_level();
//
//		return b2;
//	}
//
//	B2mdFactoryInfoPtr FB2MessageInfoConverter::ToB2mdFactoryInfo(const b2::protocol::masterdata::mdFactoryInfo& pb)
//	{
//
//		auto b2 = std::make_shared<B2mdFactoryInfo>();
//		b2->id = pb.id();
//		b2->factory_type = pb.factory_type();
//		b2->factory_tab = pb.factory_tab();
//		b2->group_id = pb.group_id();
//		b2->group_sort = pb.group_sort();
//		b2->item_sort = pb.item_sort();
//		b2->limit_id = pb.limit_id();
//		b2->character_type = pb.character_type();
//		b2->gold_cost = pb.gold_cost();
//		b2->gem_cost = pb.gem_cost();
//		b2->dark_crystal_cost = pb.dark_crystal_cost();
//		b2->success_rate = pb.success_rate();
//		b2->huge_success_rate = pb.huge_success_rate();
//		b2->fail_rate = pb.fail_rate();
//		b2->huge_success_add_item_template_id = pb.huge_success_add_item_template_id();
//		b2->huge_success_add_item_count = pb.huge_success_add_item_count();
//		b2->result_item_template_id = pb.result_item_template_id();
//		b2->result_item_count = pb.result_item_count();
//		b2->main_material_template_id = pb.main_material_template_id();
//		b2->main_material_cond_reinforce = pb.main_material_cond_reinforce();
//		b2->main_material_cond_level = pb.main_material_cond_level();
//		b2->main_material_count = pb.main_material_count();
//		b2->main_material_fail_refund_rate = pb.main_material_fail_refund_rate();
//		b2->material_template_id1 = pb.material_template_id1();
//		b2->material_count1 = pb.material_count1();
//		b2->material_fail_refund_rate1 = pb.material_fail_refund_rate1();
//		b2->material_template_id2 = pb.material_template_id2();
//		b2->material_count2 = pb.material_count2();
//		b2->material_fail_refund_rate2 = pb.material_fail_refund_rate2();
//		b2->material_template_id3 = pb.material_template_id3();
//		b2->material_count3 = pb.material_count3();
//		b2->material_fail_refund_rate3 = pb.material_fail_refund_rate3();
//		b2->material_template_id4 = pb.material_template_id4();
//		b2->material_count4 = pb.material_count4();
//		b2->material_fail_refund_rate4 = pb.material_fail_refund_rate4();
//		b2->visible_type = pb.visible_type();
//
//		return b2;
//	}
//
//	B2mdFactoryMaterialTransInfoPtr FB2MessageInfoConverter::ToB2mdFactoryMaterialTransInfo(const b2::protocol::masterdata::mdFactoryMaterialTransInfo& pb)
//	{
//
//		auto b2 = std::make_shared<B2mdFactoryMaterialTransInfo>();
//		b2->factory_id = pb.factory_id();
//		b2->trans_main_material_template_id = pb.trans_main_material_template_id();
//		b2->trans_main_material_count = pb.trans_main_material_count();
//		b2->trans_material_template_id1 = pb.trans_material_template_id1();
//		b2->trans_material_count1 = pb.trans_material_count1();
//		b2->trans_material_template_id2 = pb.trans_material_template_id2();
//		b2->trans_material_count2 = pb.trans_material_count2();
//		b2->trans_material_template_id3 = pb.trans_material_template_id3();
//		b2->trans_material_count3 = pb.trans_material_count3();
//		b2->trans_material_template_id4 = pb.trans_material_template_id4();
//		b2->trans_material_count4 = pb.trans_material_count4();
//
//		return b2;
//	}
//
//	B2mdFactoryLimitInfoPtr FB2MessageInfoConverter::ToB2mdFactoryLimitInfo(const b2::protocol::masterdata::mdFactoryLimitInfo& pb)
//	{
//
//		auto b2 = std::make_shared<B2mdFactoryLimitInfo>();
//		b2->id = pb.id();
//		b2->limit_type = pb.limit_type();
//		b2->limit_count = pb.limit_count();
//		b2->limit_time_type = pb.limit_time_type();
//
//		return b2;
//	}
//
//	B2FactoryLimitStatusPtr FB2MessageInfoConverter::ToB2FactoryLimitStatus(const b2::protocol::commondata::FactoryLimitStatus& pb)
//	{
//
//		auto b2 = std::make_shared<B2FactoryLimitStatus>();
//		b2->limit_id = pb.limit_id();
//		b2->limit_type = pb.limit_type();
//		b2->limit_time_type = pb.limit_time_type();
//		b2->left_count = pb.left_count();
//
//		return b2;
//	}
//
//	B2FactoryResultPtr FB2MessageInfoConverter::ToB2FactoryResult(const b2::protocol::commondata::FactoryResult& pb)
//	{
//
//		auto b2 = std::make_shared<B2FactoryResult>();
//		b2->factory_result_type = pb.factory_result_type();
//		b2->make_item = ItemPb2B2ItemServerInfo(pb.make_item());
//		b2->bonus_item = ItemPb2B2ItemServerInfo(pb.bonus_item());
//
//		for (auto item : pb.refund_item()) {
//			b2->refund_item.Emplace(ItemPb2B2ItemServerInfo(item));
//		}
//
//		return b2;
//	}
//
//	B2UnsealBoxRewardPtr FB2MessageInfoConverter::ToB2UnsealBoxReward(const b2::protocol::commondata::UnsealBoxReward& pb)
//	{
//
//		auto b2 = std::make_shared<B2UnsealBoxReward>();
//		b2->reward_type = PbRewardTypeToB2RewardType(pb.reward_type());
//		b2->amount = pb.amount();
//		b2->template_id = pb.template_id();
//		b2->item = ItemPb2B2ItemServerInfo(pb.item());
//		b2->aether = AetherPb2B2AetherServerInfo(pb.aether());
//		b2->totem = TotemPb2B2TotemServerInfo(pb.totem());
//
//		return b2;
//	}
//
//	B2FactoryPeriodicStatusPtr FB2MessageInfoConverter::ToB2FactoryPeriodicStatus(const b2::protocol::commondata::FactoryPeriodicStatus& pb)
//	{
//
//		auto b2 = std::make_shared<B2FactoryPeriodicStatus>();
//
//		b2->factory_id = pb.factory_id();
//		b2->preview_time = pb.preview_time();
//		b2->start_time = pb.start_time();
//		b2->end_time = pb.end_time();
//
//		return b2;
//	}
//
//	B2mdSealBoxInfoPtr FB2MessageInfoConverter::ToB2mdSealBoxInfo(const b2::protocol::masterdata::mdSealBoxInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdSealBoxInfo>();
//
//		b2->seal_box_template_id = pb.seal_box_template_id();
//		b2->reward_slot1 = pb.reward_slot1();
//		b2->reward_slot2 = pb.reward_slot2();
//		b2->reward_slot3 = pb.reward_slot3();
//		b2->reward_slot4 = pb.reward_slot4();
//		b2->reward_slot5 = pb.reward_slot5();
//		b2->is_rate_visible= pb.is_rate_visible();
//
//		return b2;
//	}
//
//	B2mdSealBoxRewardInfoPtr FB2MessageInfoConverter::ToB2mdSealBoxRewardInfo(const b2::protocol::masterdata::mdSealBoxRewardInfo& pb) 
//	{
//		auto b2 = std::make_shared<B2mdSealBoxRewardInfo>();
//
//		b2->seal_box_reward_id = pb.seal_box_reward_id();
//		b2->min_fixed_count= pb.min_fixed_count();
//		b2->max_fixed_count= pb.max_fixed_count();
//		b2->is_recycle= pb.is_recycle();
//
//		return b2;
//	}
//
//	B2mdSealBoxRewardRateInfoPtr FB2MessageInfoConverter::ToB2mdSealBoxRewardRateInfo(const b2::protocol::masterdata::mdSealBoxRewardRateInfo& pb)
//	{
//		auto b2 = std::make_shared<B2mdSealBoxRewardRateInfo>();
//
//		b2->seal_box_reward_id = pb.seal_box_reward_id();
//		b2->reward_type= PbRewardTypeToB2RewardType(pb.reward_type());
//		b2->template_id = pb.template_id();
//		b2->min_amount = pb.min_amount();
//		b2->max_amount= pb.max_amount();
//		b2->grade = pb.grade();
//		b2->group_id= ToB2GroupPrefix(pb.group_id());
//		b2->equip_category = PbEquipCategoryToB2EquipCategory(pb.equip_category());
//		b2->equip_position = PbEquipPosition2B2EquipPositionType(pb.equip_position());
//		b2->quality = pb.quality();
//		b2->enhance_level = pb.enhance_level();
//		b2->surpass_count= pb.surpass_count();
//		b2->reward_rate= pb.reward_rate();
//		b2->is_fixed_reward = pb.is_fixed_reward();
//		b2->fixed_reward_rate = pb.fixed_reward_rate();
//		b2->effect_grade = pb.effect_grade();
//
//		return b2;
//	}
//
//	
//	//////////////////////////////////////////////////////////////////////
//	 FB2MessageInfoRequest to request protocol-buffers
//MessagePtr  request = 0;
//	MessagePtr FB2MessageInfoConverter::ToGetAccountRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAccount>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAccountRequest>();
//		return request;
//		return nullptr;
//	}
//		
//	b2network::MessagePtr FB2MessageInfoConverter::ToFindAccountRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFindAccount>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FindAccountRequest>();
//		for (B2FindAccountQueryPtr query : B2MessageInfo->query_parameters)
//		{
//			request->add_query_parameters()->CopyFrom(B2FindAccountQuery2PbQueryParameter(query));
//		}
//		return request;
//		return nullptr;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAccountItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAccountItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAccountItemRequest>();
//		if (B2MessageInfo->character_type != 0) {
//			request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		}
//		if (B2MessageInfo->inventory_type != 0) {
//			request->set_inventory_type(B2InventoryType2PbInventory(B2MessageInfo->inventory_type));
//		}
//		return request;
//		return nullptr;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAccountCostumeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAccountCostume>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAccountCostumeRequest>();
//		if (B2MessageInfo->character_type != 0) {
//			request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		}
//		if (B2MessageInfo->inventory_type != 0) {
//			request->set_inventory_type(B2InventoryType2PbInventory(B2MessageInfo->inventory_type));
//		}
//		return request;
//		return nullptr;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetMasterDataRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetMasterData>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetMasterDataRequest>();
//		return request;
//		return nullptr;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetLatestEventDataRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetLatestEventData>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetLatestEventDataRequest>();
//		request->set_last_update_time(B2MessageInfo->last_update_time);
//		return request;
//		return nullptr;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToSellItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSellItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SellItemsRequest>();
//		for (int64 itemUID : B2MessageInfo->item_ids) {
//			request->add_item_ids(itemUID);
//		}
//		return request;
//		return nullptr;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToSellConsumableItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSellConsumableItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SellConsumableItemRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_sell_count(B2MessageInfo->sell_count);
//		return request;
//		return nullptr;
//	}
//
//
//	MessagePtr FB2MessageInfoConverter::ToConvertDataRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestConvertData>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckExChangeCountRequest>();
//		request->set_template_id(B2MessageInfo->item_id);
//		return request;
//		return nullptr;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToConvertItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestConvertItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ExChangeSystemRequest>();
//		request->set_id(B2MessageInfo->item_id);
//
//		b2::protocol::commondata::ExchangePriceType convertType = b2::protocol::commondata::ExchangePriceType::EC_GEM;
//		if (B2MessageInfo->convert_type == 1)
//		{
//			convertType = b2::protocol::commondata::ExchangePriceType::EC_GEM;
//		}
//		else if (B2MessageInfo->convert_type == 2)
//		{
//			convertType = b2::protocol::commondata::ExchangePriceType::EC_GOLD;
//		}
//		request->set_exchange_type(convertType);
//
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToEquipItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEquipItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EquipItemsRequest>();
//		request->set_preset_id(B2MessageInfo->preset_id);
//		for (int64 itemUID : B2MessageInfo->item_ids) {
//			request->add_item_ids(itemUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToUnequipItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnequipItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnequipItemsRequest>();
//		request->set_preset_id(B2MessageInfo->preset_id);
//		for (int64 itemUID : B2MessageInfo->item_ids) {
//			request->add_item_ids(itemUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToEquipCostumeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEquipCostume>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EquipCostumesRequest>();
//		for (int64 costumeUID : B2MessageInfo->costume_ids) {
//			request->add_costume_ids(costumeUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToUnequipCostumeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnequipCostume>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnequipCostumesRequest>();
//		for (int64 costumeUID : B2MessageInfo->costume_ids) {
//			request->add_costume_ids(costumeUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToEnhanceCostumeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceCostume>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceCostumeRequest>();
//		request->set_costume_id(B2MessageInfo->costume_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToDismantleCostumesRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDismantleCostumes>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DismantleCostumesRequest>();
//		for (int64 costumeUID : B2MessageInfo->costume_ids) {
//			request->add_costume_ids(costumeUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToLockCostumesRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLockCostumes>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LockCostumesRequest>();
//		for (int64 costumeUID : B2MessageInfo->costume_ids) {
//			request->add_costume_ids(costumeUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToUnlockCostumesRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnlockCostumes>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnlockCostumesRequest>();
//		for (int64 costumeUID : B2MessageInfo->costume_ids) {
//			request->add_costume_ids(costumeUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToLockItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLockItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LockItemsRequest>();
//		for (int64 itemUID : B2MessageInfo->item_ids) {
//			request->add_item_ids(itemUID);
//		}
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToUnlockItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnlockItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnlockItemsRequest>();
//		for (int64 itemUID : B2MessageInfo->item_ids) {
//			request->add_item_ids(itemUID);
//		}
//		return request;
//	}
//	
//	MessagePtr FB2MessageInfoConverter::ToExtendInventoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestExtendInventory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ExtendInventoryRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_extend_count(B2MessageInfo->extend_count);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToEnhanceItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceItemRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_scroll_item_id(B2MessageInfo->scroll_item_id);
//		request->set_use_protection(B2MessageInfo->use_protection);
//		request->set_protection_item_id(B2MessageInfo->protection_item_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToDismantleItemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDismantleItems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DismantleItemsRequest>();
//		for (int64 itemUID : B2MessageInfo->item_ids) {
//			request->add_item_ids(itemUID);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToExchangeItemEnhanceLevelRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestExchangeItemEnhanceLevel>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ExchangeItemEnhanceLevelRequest>();
//		request->set_item_id_1(B2MessageInfo->item_id_1);
//		request->set_item_id_2(B2MessageInfo->item_id_2);
//		request->set_price_type(B2MessageInfo->price_type);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToOpenSealSlotItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestOpenSealSlotItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::OpenSealSlotItemRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_seal_slot(B2MessageInfo->seal_slot);*/
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCarveSealItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCarveSealItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CarveSealItemRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_seal_slot(B2MessageInfo->seal_slot);*/
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToLevelupItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLevelupItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LevelupItemRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		for (int64 itemUID : B2MessageInfo->material_item_ids) {
//			request->add_material_item_ids(itemUID);
//		}
//		request->set_auto_levelup(B2MessageInfo->auto_levelup);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToSurpassItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSurpassItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SurpassItemRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		request->set_material_item_id(B2MessageInfo->material_item_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToUpgradeItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpgradeItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpgradeItemRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToComposeItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestComposeItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ComposeItemRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		request->set_sub_item_id(B2MessageInfo->sub_item_id);
//		request->set_current_preset_id(B2MessageInfo->current_preset_id);
//		return request;
//	}
//	
//	b2network::MessagePtr FB2MessageInfoConverter::ToChangeItemCharacterTypeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestChangeItemCharacterType>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ChangeItemCharacterTypeRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetChangeItemCharacterTypeCostRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetChangeItemCharacterTypeCost>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetChangeItemCharacterTypeCostRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToStartStageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestStartStage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::StartStageRequest>();
//		request->set_stage_id(B2MessageInfo->stage_id);
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_tag_character_type(B2CharacterType2PbCharacter(B2MessageInfo->tag_character_type));
//		request->set_selected_option_flags(B2MessageInfo->selected_option_flags);
//		request->set_stage_repeat_mode(B2StageRepeatModeToPbStageRepeatMode(B2MessageInfo->stage_repeat_mode));
//		request->set_boost_mode(B2MessageInfo->boost_mode);
//		request->set_tutorial_id(B2MessageInfo->tutorial_id);
//		request->set_main_character_preset_id(B2MessageInfo->main_character_preset_id);
//		request->set_tag_character_preset_id(B2MessageInfo->tag_character_preset_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToClearStageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestClearStage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ClearStageRequest>();
//		request->set_stage_id(B2MessageInfo->stage_id);
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_tag_character_type(B2CharacterType2PbCharacter(B2MessageInfo->tag_character_type));
//		request->set_clear_time_second(B2MessageInfo->clear_time_second);
//
//		for (B2KillMonsterInfoPtr info : B2MessageInfo->kill_monster_infos)
//		{
//			auto pb = request->add_kill_monster_infos();
//			pb->set_monster_id(info->monster_id);
//			pb->set_monster_grade(info->monster_grade);
//			pb->set_kill_count(info->kill_count);
//
//		}
//		request->mutable_item_sale_option()->CopyFrom(ToItemSpotSaleOption(B2MessageInfo->item_sale_option));
//		request->mutable_aether_sale_option()->CopyFrom(ToAetherSpotSaleOption(B2MessageInfo->aether_sale_option));
//		request->set_total_monster_kill_count(B2MessageInfo->total_monster_kill_count);
//		request->set_is_all_survival(B2MessageInfo->is_all_survival);
//		request->set_qte_start_count(B2MessageInfo->qte_start_count);
//		request->set_qte_finish_count(B2MessageInfo->qte_finish_count);*/
//
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToFailStageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFailStage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FailStageRequest>();
//		request->set_stage_id(B2MessageInfo->stage_id);
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//		request->set_reason(B2StageFailReasonToPbStageFailReason(B2MessageInfo->reason));
//		request->set_play_time_second(B2MessageInfo->play_time_second);*/
//
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToSweepStageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSweepStage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SweepStageRequest>();
//		request->set_stage_id(B2MessageInfo->stage_id);
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_tag_character_type(B2CharacterType2PbCharacter(B2MessageInfo->tag_character_type));
//		request->set_main_character_preset_id(B2MessageInfo->main_character_preset_id);
//		request->set_tag_character_preset_id(B2MessageInfo->tag_character_preset_id);
//		request->set_repeat_count(B2MessageInfo->repeat_count);*/
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToResurrectionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestResurrection>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ResurrectionRequest>();
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//		request->set_selected_buff_index(B2MessageInfo->selected_buff_index);*/
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToCheckBladePointChargeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckBladePointCharge>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckBladePointChargeRequest>();*/
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToGetItemSurpassCostRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetItemSurpassCost>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetItemSurpassCostRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToGetItemUpgradeCostRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetItemUpgradeCost>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetItemUpgradeCostRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);*/
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToGetItemComposeCostRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetItemComposeCost>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetItemComposeCostRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		request->set_current_preset_id(B2MessageInfo->current_preset_id);
//		return request;
//	}
//	
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetActInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetActInfo>(messageInfo);
//
//		auto request = std::make_shared < b2::protocol::session::GetActInfoRequest>();
//		request->set_difficulty(B2StageDifficultyToPbStageDifficulty(B2MessageInfo->difficulty));
//		request->set_act_id(B2MessageInfo->act_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetStageInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetStageInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetStageInfoRequest>();
//		request->set_stage_id(B2MessageInfo->stage_id);
//		return request;
//	}
//
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetMatchStatusRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetMatchStatus>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetMatchStatusRequest>();
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToMatchmakingRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoMatchmakingRequest>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::MatchmakingRequest>();
//		request->set_match_type(B2MatchMakingType2PbMatchMakingType(B2MessageInfo->match_type));
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_tag_character_type(B2CharacterType2PbCharacter(B2MessageInfo->tag_character_type));
//		request->set_use_additional_count(B2MessageInfo->use_additional_count);
//        request->set_room_seed(TCHAR_TO_ANSI(*B2MessageInfo->room_seed));
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToRaidMatchmakingRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRaidMatchmakingRequest>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::RaidMatchmakingRequest>();
//		request->set_req_type(ToRaidReqType(B2MessageInfo->raid_req_type));
//		request->set_battle_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_raid_type((b2::protocol::commondata::RaidType)B2MessageInfo->raid_type);
//		request->set_raid_step(B2MessageInfo->raid_step);
//
//		const auto reqType = ToRaidReqType(B2MessageInfo->raid_req_type);
//		if (reqType == b2::protocol::session::RaidReqType::REJOIN)
//		{
//			const auto channelEndpoint = B2Endpoint2PbEndpoint(B2MessageInfo->channel_endpoint);
//			request->mutable_channel_endpoint()->set_host(channelEndpoint.host());
//			request->mutable_channel_endpoint()->set_port(channelEndpoint.port());
//		}
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToCancelMatchmakingRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/*       auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoCancelMatchmakingRequest>(messageInfo);
//
//			   auto request = std::make_shared<b2::protocol::session::CancelMatchmakingRequest>();*/
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToInvitationRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoInvitationRequest>(messageInfo);
//
//        auto request = std::make_shared <b2::protocol::session::InvitationRequest>();
//        request->set_invitation_type(b2::protocol::commondata::Invitation::PVP_INVITE);
//		request->set_invitee(TCHAR_TO_UTF8(*B2MessageInfo->invitee));
//        request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToAcceptInvitationRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoAcceptInvitationRequest>(messageInfo);
//
//        auto request = std::make_shared <b2::protocol::session::AcceptInvitationRequest>();
//        request->set_invitation_type(b2::protocol::commondata::Invitation::PVP_INVITE);
//		request->set_inviter(TCHAR_TO_UTF8(*B2MessageInfo->inviter));
//        request->set_invitation_ref(TCHAR_TO_ANSI(*B2MessageInfo->invitation_ref));
//		request->set_invitee(TCHAR_TO_UTF8(*B2MessageInfo->invitee));
//        request->mutable_room_location()->CopyFrom(B2RoomLocation2PbRoomLocation(B2MessageInfo->room_location));
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToDeclineInvitationRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/* auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoDeclineInvitationRequest>(messageInfo);
//
//		 auto request = std::make_shared <b2::protocol::session::DeclineInvitationRequest>();
//		 request->set_invitation_type(b2::protocol::commondata::Invitation::PVP_INVITE);
//		 request->set_inviter(TCHAR_TO_UTF8(*B2MessageInfo->inviter));
//		 request->set_invitation_ref(TCHAR_TO_ANSI(*B2MessageInfo->invitation_ref));
//		 request->set_invitee(TCHAR_TO_UTF8(*B2MessageInfo->invitee));*/
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToRoomJoinRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/* auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomJoinRequest>(messageInfo);
//
//		 auto request = std::make_shared<b2::protocol::channel::RoomJoinRequest>();
//		 request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//		 request->set_room_type((b2::protocol::channel::RoomType) B2MessageInfo->room_type);
//		 request->set_room_access((b2::protocol::commondata::Room::ACCESS) B2MessageInfo->room_access);
//		 for (auto property : B2MessageInfo->properties)
//		 {
//			 request->add_properties()->CopyFrom(B2RoomProperty2PbRoomProperty(property));
//		 }
//
//		 for (auto character : B2MessageInfo->room_characters)
//		 {
//			 request->add_room_characters()->CopyFrom(ToRoomCharacter(character));
//		 }
//
//		 for (auto presetNum : B2MessageInfo->character_presets)
//		 {
//			 request->add_character_presets()->CopyFrom(ToPresetNum(presetNum));
//		 }*/
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToRoomLeaveRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomLeaveRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::RoomLeaveRequest>();
//		request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));*/
//        return request;
//    }
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRoomKickRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomKickRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::RoomKickRequest>();
//		request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//		request->set_kick_account_id(B2MessageInfo->kick_account_id);*/
//        return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToRoomMessageCommand(FB2MessageInfoPtr messageInfo)
//    {
//		/* auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomMessageCommand>(messageInfo);
//
//		 auto command = std::make_shared<b2::protocol::channel::RoomMessageCommand>();
//		 command->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//		 if (B2MessageInfo->receiver_id != 0)
//		 {
//			 command->set_receiver_id(B2MessageInfo->receiver_id);
//		 }
//		 command->set_message(TCHAR_TO_UTF8(*B2MessageInfo->message));*/
//        return command;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToRoomSetPropertyRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/* auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomSetPropertyRequest>(messageInfo);
//
//		 auto request = std::make_shared<b2::protocol::channel::RoomSetPropertyRequest>();
//		 request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//		 for (auto property : B2MessageInfo->properties)
//		 {
//			 request->add_properties()->CopyFrom(B2RoomProperty2PbRoomProperty(property));
//		 }*/
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToRoomInviteRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomInviteRequest>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::channel::RoomInviteRequest>();
//        request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//        request->set_invitee(TCHAR_TO_UTF8(*B2MessageInfo->invitee));
//        request->set_invitation_number(B2MessageInfo->invitation_number);
//
//		for raid
//		if (B2MessageInfo->map_id != 0)
//		{
//			request->set_map_id(B2MessageInfo->map_id);
//		}
//
//		if (B2MessageInfo->difficulty != 0)
//		{
//			request->set_difficulty(B2MessageInfo->difficulty);
//		}
//
//        return request;
//    }
//
//    MessagePtr FB2MessageInfoConverter::ToRoomCloseRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomCloseRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::RoomCloseRequest>();
//		request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//
//		if (B2MessageInfo->map_id != 0)
//		{
//			request->set_map_id(B2MessageInfo->map_id);
//		}
//
//		request->set_reason(B2RoomCloseReasonToPbRoomCloseReason(B2MessageInfo->reason));*/
//
//        return request;
//    }
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRoomLeaveResultRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomLeaveResultRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::RoomLeaveResultRequest>();
//		request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//
//		if (B2MessageInfo->leaved_account_id != 0)
//		{
//			request->set_leaved_account_id(B2MessageInfo->leaved_account_id);
//		}
//
//		request->set_match_token(TCHAR_TO_ANSI(*B2MessageInfo->match_token));*/
//
//        return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToRoomAccessControlRequest(FB2MessageInfoPtr messageInfo)
//    {
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRoomAccessControlRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::RoomAccessControlRequest>();
//		request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//		request->set_access(b2::protocol::commondata::Room_ACCESS::Room_ACCESS_PUBLIC);*/
//        return request;
//    }
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReportUserViaChannelRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoReportUserViaChannelRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::ReportUserViaChannelRequest>();
//		request->set_target_account_id(B2MessageInfo->target_account_id);
//		request->set_mode_type((b2::protocol::commondata::ReportModeType)B2MessageInfo->mode_type);
//		request->set_reason_type(B2MessageInfo->reason_type);
//		request->set_explain(TCHAR_TO_UTF8(*B2MessageInfo->explain));*/
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToWorldJoinRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*	auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoWorldJoinRequest>(messageInfo);
//
//			auto request = std::make_shared<b2::protocol::channel::WorldJoinRequest>();
//			if (!B2MessageInfo->world_id.IsEmpty())
//			{
//				request->set_world_id(TCHAR_TO_ANSI(*B2MessageInfo->world_id));
//			}*/
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToWorldLeaveRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoWorldLeaveRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::WorldLeaveRequest>();
//		if (!B2MessageInfo->world_id.IsEmpty())
//		{
//			request->set_world_id(TCHAR_TO_ANSI(*B2MessageInfo->world_id));
//		}*/
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToLevelUpSkillRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLevelUpSkill>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LevelUpSkillRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_skill_id(B2MessageInfo->skill_id);*/
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToResetSkillRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestResetSkill>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ResetSkillRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));*/
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetUsingSkillRequest(FB2MessageInfoPtr messageInfo)
//	{
//		/*auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetUsingSkill>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetUsingSkillRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_slot(B2MessageInfo->slot);
//		request->set_using_skill_id_1(B2MessageInfo->using_skill_id_1);
//		request->set_using_skill_id_2(B2MessageInfo->using_skill_id_2);
//		request->set_using_skill_id_3(B2MessageInfo->using_skill_id_3);*/
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuySkillPointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuySkillPoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuySkillPointRequest>();
//		for (B2BuySkillPointRequestInfoPtr info : B2MessageInfo->buy_infos)
//		{
//			auto buyInfo = request->add_buy_infos();
//			buyInfo->set_character_type(B2CharacterType2PbCharacter(info->character_type));
//			buyInfo->set_skill_point_buy_count(info->skill_point_buy_count);
//		}
//		return request;
//	}
//
//    MessagePtr FB2MessageInfoConverter::ToGiveNicknameRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGiveNickname>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::GiveNicknameRequest>();
//        request->set_nickname(TCHAR_TO_UTF8(*B2MessageInfo->nickname));
//        return request;
//    }
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpdateNicknameRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpdateNickname>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpdateNicknameRequest>();
//		request->set_nickname(TCHAR_TO_UTF8(*B2MessageInfo->nickname));
//		request->set_item_template_id(B2MessageInfo->item_template_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetServerVersionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetServerVersion>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetServerVersionRequest>();
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToGiveCharacterNameRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGiveCharacterName>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::GiveCharacterNameRequest>();
//        request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//        request->set_name(TCHAR_TO_UTF8(*B2MessageInfo->name));
//        return request;
//    }
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveActClearRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveActClearReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ActClearRewardRequest>();	
//		request->set_difficulty(B2StageDifficultyToPbStageDifficulty(B2MessageInfo->difficulty));
//		request->set_act_id(B2MessageInfo->act_id);
//		request->set_reward_index(B2MessageInfo->reward_index);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetTeamBattleStatusRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetTeamBattleStatus>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetTeamBattleStatusRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpdateTeamBattleEntryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpdateTeamBattleEntry>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpdateTeamBattleEntryRequest>();
//		request->mutable_entry()->CopyFrom(B2TeamBattleEntry2PbTeamBattleEntry(B2MessageInfo->entry));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEnhanceTeamBattleFormationRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceTeamBattleFormation>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceTeamBattleFormationRequest>();
//		request->set_formation_type(B2BattleFormationType2PbBattleFormationType(B2MessageInfo->formation_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToTeamMatchmakingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestTeamMatchmaking>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::TeamMatchmakingRequest>();
//		request->set_use_additional_count(B2MessageInfo->use_additional_count);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToTeamMatchResultRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestTeamMatchResult>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::TeamMatchResultRequest>();
//		request->set_match_token(TCHAR_TO_ANSI(*B2MessageInfo->match_token));
//		request->set_match_result(B2MatchResultType2PbMatchResultType(B2MessageInfo->match_result));
//		request->set_is_give_up(B2MessageInfo->is_give_up);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToMatchResultRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoMatchResultRequest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::MatchResultRequest>();
//		request->mutable_result()->CopyFrom(B2MatchResult2PbMatchResult(B2MessageInfo->result));
//		request->set_match_type(ToMatchType(B2MessageInfo->match_type));
//		request->set_room_id(TCHAR_TO_ANSI(*B2MessageInfo->room_id));
//		request->mutable_my_main_character()->CopyFrom(ToRoomCharacter(B2MessageInfo->my_main_character));
//		request->mutable_my_tag_character()->CopyFrom(ToRoomCharacter(B2MessageInfo->my_tag_character));
//		request->mutable_opponent_main_character()->CopyFrom(ToRoomCharacter(B2MessageInfo->opponent_main_character));
//		request->mutable_opponent_tag_character()->CopyFrom(ToRoomCharacter(B2MessageInfo->opponent_tag_character));
//		request->set_is_give_up(B2MessageInfo->is_give_up);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetRankRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetRank>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceRankNodeRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEnhanceRankNodeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceRankNode>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceRankNodeRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetMailListRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetMailList>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetMailListRequest>();
//		if (B2MessageInfo->mail_category != 0) {
//			request->set_mail_category(B2MailCategory2PbMailCategory(B2MessageInfo->mail_category));
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToOpenMailRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestOpenMail>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::OpenMailRequest>();
//		request->set_mail_id(B2MessageInfo->mail_id);
//
//		if (B2MessageInfo->lottery_character_type != 0) {
//			request->set_lottery_character_type(B2CharacterType2PbCharacter(B2MessageInfo->lottery_character_type));
//		}
//
//		if (B2MessageInfo->lottery_item_template_id != 0) {
//			request->set_lottery_item_template_id(B2MessageInfo->lottery_item_template_id);
//		}
//
//		if (B2MessageInfo->lottery_aether_type != 0) {
//			request->set_lottery_aether_type(B2MessageInfo->lottery_aether_type);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToOpenAllMailRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestOpenAllMail>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::OpenAllMailRequest>();
//		request->set_mail_category(B2MailCategory2PbMailCategory(B2MessageInfo->mail_category));
//		request->set_start_id(B2MessageInfo->start_id);
//		request->set_end_id(B2MessageInfo->end_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetNewMailCountRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetNewMailCount>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetNewMailCountRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAskFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAskFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AskFriendRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAcceptAskFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAcceptAskFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AcceptAskFriendRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRejectAskFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRejectAskFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RejectAskFriendRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetFriendListRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetFriendList>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetFriendListRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCancelAskFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCancelAskFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CancelAskFriendRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDeleteFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDeleteFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DeleteFriendRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAllSendAskFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAllSendAskFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAllSendAskFriendRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAllReceiveAskFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAllReceiveAskFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAllReceiveAskFriendRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSendSocialPointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSendSocialPoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SendSocialPointRequest>();
//		for (auto account_id : B2MessageInfo->account_ids)
//		{
//			request->add_account_ids(account_id);			
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetRecommendFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetRecommendFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetRecommendFriendRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFindUserRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFindUser>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FindUserRequest>();
//		request->set_nickname(TCHAR_TO_UTF8(*B2MessageInfo->nickname));
//		request->set_guild_invite_state(B2MessageInfo->guild_invite_state);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToConfirmNewFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestConfirmNewFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ConfirmNewFriendRequest>();
//		request->set_is_all_confirm(B2MessageInfo->is_all_confirm);
//		for (auto account_id : B2MessageInfo->friend_ids)
//		{
//			request->add_friend_ids(account_id);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPlatformFriendInviteInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPlatformFriendInviteInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PlatformFriendInviteInfoRequest>();
//		request->set_platform_type(B2MessageInfo->platform_type);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPlatformFriendInviteCountUpRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPlatformFriendInviteCountUp>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PlatformFriendInviteCountUpRequest>();
//		request->set_platform_type(B2MessageInfo->platform_type);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPlatformFriendJoinerCountUpRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPlatformFriendJoinerCountUp>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PlatformFriendJoinerCountUpRequest>();
//		request->set_platform_type(B2MessageInfo->platform_type);
//		request->set_joiner_count(B2MessageInfo->joiner_count);
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPlatformFriendInviteRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPlatformFriendInviteReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PlatformFriendInviteRewardRequest>();
//		request->set_platform_type(B2MessageInfo->platform_type);
//		request->set_reward_type(ToFriendInviteRewardType(B2MessageInfo->reward_type));
//		request->set_reward_index(B2MessageInfo->reward_index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetQuestRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetQuest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetQuestRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAcceptQuestRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAcceptQuest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AcceptQuestRequest>();
//		request->set_slot(B2QuestSlotTypeToPbQuestSlotType(B2MessageInfo->slot));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveQuestRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveQuestReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveQuestRewardRequest>();
//		request->set_slot(B2QuestSlotTypeToPbQuestSlotType(B2MessageInfo->slot));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetCounterDungeonRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetCounterDungeon>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetCounterDungeonRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToStartCounterDungeonRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestStartCounterDungeon>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::StartCounterDungeonRequest>();
//		request->set_dungeon_id(B2MessageInfo->dungeon_id);
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFinishCounterDungeonRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFinishCounterDungeon>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FinishCounterDungeonRequest>();
//		request->set_dungeon_id(B2MessageInfo->dungeon_id);
//		request->set_play_token(TCHAR_TO_ANSI(*B2MessageInfo->play_token));
//		request->set_is_success(B2MessageInfo->is_success);
//		request->set_is_give_up(B2MessageInfo->is_give_up);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSweepCounterDungeonRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSweepCounterDungeon>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SweepCounterDungeonRequest>();
//		request->set_dungeon_id(B2MessageInfo->dungeon_id);
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuyCounterDungeonTicketRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuyCounterDungeonTicket>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuyCounterDungeonTicketRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCounterDungeonWeeklyRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCounterDungeonWeeklyReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CounterDungeonWeeklyRewardRequest>();
//		request->set_index(B2MessageInfo->index);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetDimensionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetDimension>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetDimensionRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToStartDimensionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestStartDimension>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::StartDimensionRequest>();
//		request->set_difficulty_level(B2MessageInfo->difficulty_level);
//		request->set_dungeon_id(B2MessageInfo->dungeon_id);
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_total_power(B2MessageInfo->total_power);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFinishDimensionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFinishDimension>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FinishDimensionRequest>();
//		request->set_difficulty_level(B2MessageInfo->difficulty_level);
//		request->set_play_token(TCHAR_TO_ANSI(*B2MessageInfo->play_token));
//		request->set_play_result(ToPBDimensionResult(B2MessageInfo->play_result));
//		request->set_play_time_millis(B2MessageInfo->play_time_millis);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCreateGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCreateGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CreateGuildRequest>();
//		request->set_guild_name(TCHAR_TO_UTF8(*B2MessageInfo->guild_name));
//		request->set_join_option(ToGuildJoinOption(B2MessageInfo->join_option));
//		request->set_mark_index(B2MessageInfo->mark_index);
//		request->set_mark_color(B2MessageInfo->mark_color);
//		request->set_mark_bg_index(B2MessageInfo->mark_bg_index);
//		request->set_mark_bg_color(B2MessageInfo->mark_bg_color);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAskJoinGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAskJoinGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AskJoinGuildRequest>();
//		request->set_guild_id(B2MessageInfo->guild_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToLeaveGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLeaveGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LeaveGuildRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildDetailInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildDetailInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildDetailInfoRequest>();
//		request->set_guild_id(B2MessageInfo->guild_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildMemberRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildMember>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildMemberRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDeleteGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDeleteGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DeleteGuildRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetRecommendGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetRecommendGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetRecommendGuildRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAppointSubGuildMasterRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAppointSubGuildMaster>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AppointSubGuildMasterRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDismissSubGuildMasterRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDismissSubGuildMaster>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DismissSubGuildMasterRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToKickGuildMemberRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestKickGuildMember>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::KickGuildMemberRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpdateGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpdateGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpdateGuildRequest>();
//		request->set_join_option(ToGuildJoinOption(B2MessageInfo->join_option));
//		request->set_mark_index(B2MessageInfo->mark_index);
//		request->set_mark_color(B2MessageInfo->mark_color);
//		request->set_mark_bg_index(B2MessageInfo->mark_bg_index);
//		request->set_mark_bg_color(B2MessageInfo->mark_bg_color);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpdateGuildNoticeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpdateGuildNotice>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpdateGuildNoticeRequest>();
//		request->set_notice(TCHAR_TO_UTF8(*B2MessageInfo->notice));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildMemberCandidatesRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildMemberCandidates>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildMemberCandidatesRequest>();
//		request->set_need_friend_list(B2MessageInfo->need_friend_list);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToInviteGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestInviteGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::InviteGuildRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildInviteListRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildInviteList>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildInviteListRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRejectInviteGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRejectInviteGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RejectInviteGuildRequest>();
//		request->set_guild_id(B2MessageInfo->guild_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAcceptInviteGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAcceptInviteGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AcceptInviteGuildRequest>();
//		request->set_guild_id(B2MessageInfo->guild_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFindGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFindGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FindGuildRequest>();
//		request->set_guild_name(TCHAR_TO_UTF8(*B2MessageInfo->guild_name));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToObtainGuildMasterRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestObtainGuildMaster>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ObtainGuildMasterRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToHandOverGuildMasterRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestHandOverGuildMaster>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::HandOverGuildMasterRequest>();
//		request->set_account_id(B2MessageInfo->account_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildRankingListRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildRankingList>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildRankingListRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToLevelUpGuildSkillRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLevelUpGuildSkill>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LevelUpGuildSkillRequest>();
//		request->set_skill_id(B2MessageInfo->skill_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuyGuildSkillRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuyGuildSkill>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuyGuildSkillRequest>();
//		request->set_skill_id(B2MessageInfo->skill_id);
//		request->set_buy_count(B2MessageInfo->buy_count);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetMissionRequest>();
//		if (B2MessageInfo->mission_type > 0) {
//			request->set_mission_type(B2MissionTypeToPbMissionType(B2MessageInfo->mission_type));
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetFeaturedMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetFeaturedMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetFeaturedMissionRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveMissionRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveMissionReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveMissionRewardRequest>();
//		
//		request->set_mission_id(B2MessageInfo->mission_id);
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveCompleteMissionRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveCompleteMissionReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveCompleteMissionRewardRequest>();
//		request->set_mission_type(B2MissionTypeToPbMissionType(B2MessageInfo->mission_type));
//		request->set_reward_index(B2MessageInfo->reward_index);
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetWingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetWing>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetWingRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEnhanceWingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceWing>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceWingRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpgradeWingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpgradeWing>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpgradeWingRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEnhanceWingOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceWingOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceWingOptionRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_option_index(B2MessageInfo->option_index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetDonationEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetDonationEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetDonationEventRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToMakeDonationRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestMakeDonation>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::MakeDonationRequest>();
//		request->set_donation_id(B2MessageInfo->donation_id);
//		request->set_is_bundle(B2MessageInfo->is_bundle);
//		request->set_is_free(B2MessageInfo->is_free);
//		request->set_is_again(B2MessageInfo->is_again);
//		request->set_last_donation_key(TCHAR_TO_ANSI(*B2MessageInfo->last_donation_key));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDonationPointRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDonationPointReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DonationPointRewardRequest>();
//		request->set_donation_id(B2MessageInfo->donation_id);
//		request->set_step(B2MessageInfo->step);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetFairyRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetFairy>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetFairyRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToLevelupFairyRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLevelupFairy>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LevelupFairyRequest>();
//		request->set_fairy_type(B2FairyType2PbFairyType(B2MessageInfo->fairy_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBlessFairyRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBlessFairy>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BlessFairyRequest>();
//		request->set_fairy_type(B2FairyType2PbFairyType(B2MessageInfo->fairy_type));
//		request->set_bless_option_id(B2MessageInfo->bless_option_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveFairyGiftRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveFairyGift>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveFairyGiftRequest>();
//		request->set_fairy_type(B2FairyType2PbFairyType(B2MessageInfo->fairy_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetHeroTowerRequest>();
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToStartHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestStartHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::StartHeroTowerRequest>();
//		request->set_floor(B2MessageInfo->floor);
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_tag_character_type(B2CharacterType2PbCharacter(B2MessageInfo->tag_character_type));
//
//		if (!B2MessageInfo->play_token.IsEmpty()) {
//			request->set_play_token(TCHAR_TO_ANSI(*B2MessageInfo->play_token));
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFinishHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFinishHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FinishHeroTowerRequest>();
//		request->set_floor(B2MessageInfo->floor);
//		request->set_play_token(TCHAR_TO_ANSI(*B2MessageInfo->play_token));
//		request->set_clear_time_sec(B2MessageInfo->clear_time_sec);
//		request->set_is_success(B2MessageInfo->is_success);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAbandonHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAbandonHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AbandonHeroTowerRequest>();
//		request->set_floor(B2MessageInfo->floor);
//		request->set_play_token(TCHAR_TO_ANSI(*B2MessageInfo->play_token));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEnhanceRelicRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceRelic>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceRelicRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_relic_id(B2MessageInfo->relic_id);
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPromotionRelicRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPromotionRelic>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PromotionRelicRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_relic_id(B2MessageInfo->relic_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAccountRelicRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAccountRelic>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAccountRelicRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSweepHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{	
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSweepHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SweepHeroTowerRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuyHeroTowerTicketRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuyHeroTowerTicket>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuyHeroTowerTicketRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDrawShopLotteryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDrawShopLottery>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DrawShopLotteryRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_draw_price_type(B2ShopDrawPriceTypeToPbShopDrawPriceType(B2MessageInfo->draw_price_type));
//		request->set_continuous_draw(B2MessageInfo->continuous_draw);
//		request->set_is_draw_again(B2MessageInfo->is_draw_again);
//		request->set_last_lottery_key(TCHAR_TO_ANSI(*B2MessageInfo->last_lottery_key));
//		request->set_is_free_lottery(B2MessageInfo->is_free_lottery);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardShopMileageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardShopMileage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardShopMileageRequest>();
//		request->set_shop_mileage_type(B2ShopMileageTypeToPbShopMileageType(B2MessageInfo->shop_mileage_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetLotteryShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetLotteryShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetLotteryShopRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuyShopProductRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuyShopProduct>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuyShopProductRequest>();
//		request->set_product_id(B2MessageInfo->product_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPurchaseCostumeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPurchaseCostume>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PurchaseCostumeRequest>();
//		request->set_product_id(B2MessageInfo->product_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGeneralShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGeneralShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGeneralShopRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetCostumeShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetCostumeShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetCostumeShopRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetMagicShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetMagicShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetMagicShopRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRenewalMagicShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRenewalMagicShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RenewalMagicShopRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPurchaseMagicShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPurchaseMagicShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PurchaseMagicShopRequest>();
//
//		request->set_id(B2MessageInfo->id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetCollectionItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetCollectionItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetCollectionItemRequest>();
//
//		if (B2MessageInfo->character_type != 0) {
//			request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		}
//
//		if (B2MessageInfo->equip_category != 0) {
//			request->set_equip_category(B2EquipCategoryToPbEquipCategory(B2MessageInfo->equip_category));
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardCollectionItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardCollectionItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardCollectionItemRequest>();
//
//		request->set_item_template_id(B2MessageInfo->item_template_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardCollectionSetItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardCollectionSetItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardCollectionSetItemRequest>();
//
//		request->set_grade(B2MessageInfo->grade);
//		request->set_group_id(ToGroupPrefix(B2MessageInfo->group_id));
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetMarketProductRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetMarketProduct>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetMarketProductRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPreparePurchaseRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPreparePurchase>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PreparePurchaseRequest>();
//
//		request->set_product_id(B2MessageInfo->product_id);
//		request->set_market_product_id(TCHAR_TO_ANSI(*B2MessageInfo->market_product_id));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToVerifyPurchaseRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestVerifyPurchase>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::VerifyPurchaseRequest>();
//
//		request->set_market_product_id(TCHAR_TO_ANSI(*B2MessageInfo->market_product_id));
//		request->set_developer_payload(TCHAR_TO_ANSI(*B2MessageInfo->developer_payload));
//		request->set_purchase_data(TCHAR_TO_ANSI(*B2MessageInfo->purchase_data));
//		request->set_purchase_signature(TCHAR_TO_ANSI(*B2MessageInfo->purchase_signature));
//		request->set_is_restore(B2MessageInfo->is_restore);
//		request->set_currency_code(TCHAR_TO_ANSI(*B2MessageInfo->currency_code));
//		request->set_price(B2MessageInfo->price);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCancelPurchaseRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCancelPurchase>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CancelPurchaseRequest>();
//
//		request->set_market_product_id(TCHAR_TO_ANSI(*B2MessageInfo->market_product_id));
//		request->set_developer_payload(TCHAR_TO_ANSI(*B2MessageInfo->developer_payload));
//		request->set_cancel_reason(ToMarketPurchaseCancel(B2MessageInfo->cancel_reason));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFinishPurchaseRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFinishPurchase>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FinishPurchaseRequest>();
//
//		request->set_developer_payload(TCHAR_TO_ANSI(*B2MessageInfo->developer_payload));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRaidResultRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRaidResult>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RaidResultRequest>();
//
//		request->set_raid_token(TCHAR_TO_ANSI(*B2MessageInfo->raid_token));
//		request->set_ranking(B2MessageInfo->ranking);
//		request->set_is_bonus_player(B2MessageInfo->is_bonus_player);
//		request->set_is_reward(B2MessageInfo->is_reward);
//		request->set_is_give_up(B2MessageInfo->is_give_up);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDailyAttendanceRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDailyAttendance>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DailyAttendanceRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToJoinGuildBattleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestJoinGuildBattle>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::JoinGuildBattleRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildBattleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildBattle>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildBattleRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToStartGuildBattleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestStartGuildBattle>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::StartGuildBattleRequest>();
//		request->set_target_account_id(B2MessageInfo->target_account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToFinishGuildBattleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestFinishGuildBattle>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::FinishGuildBattleRequest>();
//		request->set_target_account_id(B2MessageInfo->target_account_id);
//		request->set_battle_token(TCHAR_TO_ANSI(*B2MessageInfo->battle_token));
//		request->set_result(B2MatchResultType2PbMatchResultType(B2MessageInfo->result));
//		request->set_round_win_count(B2MessageInfo->round_win_count);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetGuildBattleEntryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetGuildBattleEntry>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetGuildBattleEntryRequest>();
//
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(1, B2MessageInfo->slot_1));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(2, B2MessageInfo->slot_2));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(3, B2MessageInfo->slot_3));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(4, B2MessageInfo->slot_4));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(5, B2MessageInfo->slot_5));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(6, B2MessageInfo->slot_6));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(7, B2MessageInfo->slot_7));
//		request->mutable_entry()->add_slots()->CopyFrom(B2Slot2PbGuildBattleEntrySlot(8, B2MessageInfo->slot_8));
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildBattleTurnResultRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildBattleTurnResult>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildBattleTurnResultRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildBattleTurnRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildBattleTurnReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildBattleTurnRewardRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildBattleSeasonRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildBattleSeasonReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildBattleSeasonRewardRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildBattleHistoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildBattleHistory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildBattleHistoryRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetWingVisibleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetWingVisible>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetWingVisibleRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_is_visible(B2MessageInfo->is_visible);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetHelmetVisibleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetHelmetVisible>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetHelmetVisibleRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_is_visible(B2MessageInfo->is_visible);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetCostumeVisibleRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetCostumeVisible>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetCostumeVisibleRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_costume_type(B2CostumeType2PbCostumeType(B2MessageInfo->costume_type));
//		request->set_is_visible(B2MessageInfo->is_visible);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetRaidRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetRaid>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetRaidRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRaidResurrectionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRaidResurrection>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RaidResurrectionRequest>();
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//		request->set_selected_buff_index(B2MessageInfo->selected_buff_index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAssaultMatchmakingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAssaultMatchmaking>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AssaultMatchmakingRequest>();
//		request->set_battle_character_type(B2CharacterType2PbCharacter(B2MessageInfo->battle_character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCancelAssaultMatchmakingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCancelAssaultMatchmaking>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CancelAssaultMatchmakingRequest>();
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAssaultResultRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAssaultResult>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AssaultResultRequest>();
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//		request->set_match_result(B2MatchResultType2PbMatchResultType(B2MessageInfo->match_result));
//		request->set_is_mvp_player(B2MessageInfo->is_mvp_player);
//		request->set_is_ghost_user(B2MessageInfo->is_ghost_user);
//		request->mutable_battle_info()->CopyFrom(ToAssaultBattleInfo(B2MessageInfo->battle_info));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAssaultResurrectionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAssaultResurrection>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AssaultResurrectionRequest>();
//		request->set_token(TCHAR_TO_ANSI(*B2MessageInfo->token));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCheckPvPMatchPointChargeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckPvPMatchPointCharge>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckPvPMatchPointChargeRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCheckTeamMatchPointChargeRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckTeamMatchPointCharge>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckTeamMatchPointChargeRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAssaultBattleStatusRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAssaultBattleStatus>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAssaultBattleStatusRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetFriendRankingsRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetFriendRankings>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetFriendRankingsRequest>();
//		request->set_opponent_type(B2MatchInfoType2PbMatchInfoType(B2MessageInfo->opponent_type));
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetItemRandomOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetItemRandomOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetItemRandomOptionRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGenerateItemRandomOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGenerateItemRandomOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GenerateItemRandomOptionRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_price_type(B2ShopBuyPriceTypeToPbShopBuyPriceType(B2MessageInfo->price_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToApplyItemRandomOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestApplyItemRandomOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ApplyItemRandomOptionRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDiscardItemRandomOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDiscardItemRandomOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DiscardItemRandomOptionRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetItemRandomQualityRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetItemRandomQuality>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetItemRandomQualityRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGenerateItemRandomQualityRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGenerateItemRandomQuality>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GenerateItemRandomQualityRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		request->set_material_item_id(B2MessageInfo->material_item_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToApplyItemRandomQualityRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestApplyItemRandomQuality>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ApplyItemRandomQualityRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDiscardItemRandomQualityRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDiscardItemRandomQuality>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DiscardItemRandomQualityRequest>();
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetFactoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetFactory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetFactoryRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToMakeFactoryItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestMakeFactoryItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::MakeFactoryItemRequest>();
//		
//		request->set_factory_id(B2MessageInfo->factory_id);
//		request->set_make_count(B2MessageInfo->make_count);
//		request->set_main_item_id(B2MessageInfo->main_item_id);
//		request->set_material_item_id1(B2MessageInfo->material_item_id1);
//		request->set_material_item_id2(B2MessageInfo->material_item_id2);
//		request->set_material_item_id3(B2MessageInfo->material_item_id3);
//		request->set_material_item_id4(B2MessageInfo->material_item_id4);
//		request->set_main_item_template_id(B2MessageInfo->main_item_template_id);
//		request->set_material_item_template_id1(B2MessageInfo->material_item_template_id1);
//		request->set_material_item_template_id2(B2MessageInfo->material_item_template_id2);
//		request->set_material_item_template_id3(B2MessageInfo->material_item_template_id3);
//		request->set_material_item_template_id4(B2MessageInfo->material_item_template_id4);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUnsealBoxRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnsealBox>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnsealBoxRequest>();
//
//		request->set_seal_box_id(B2MessageInfo->seal_box_id);
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_unseal_count(B2MessageInfo->unseal_count);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCheckRedDotRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckRedDot>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckRedDotRequest>();
//		for (int32 hint : B2MessageInfo->red_dot_hint) {
//			request->add_red_dot_hint(b2::protocol::session::RedDotHint(B2RedDotHintType2PbRedDotHint(hint)));
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCheckChargePointsRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckChargePoints>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckChargePointsRequest>();
//		request->set_check_blade_point(B2MessageInfo->check_blade_point);
//		request->set_check_pvp_match_point(B2MessageInfo->check_pvp_match_point);
//		request->set_check_team_match_point(B2MessageInfo->check_team_match_point);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToIncreaseDarkCrystalPaidRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestIncreaseDarkCrystalPaid>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::IncreaseDarkCrystalPaidRequest>();
//		request->set_dark_crystal_paid(B2MessageInfo->dark_crystal_paid);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToModifyItemRandomOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestModifyItemRandomOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ModifyItemRandomOptionRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_slot_num(B2MessageInfo->slot_num);
//		request->set_random_option_id(B2MessageInfo->random_option_id);
//		request->set_random_option_value(B2MessageInfo->random_option_value);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToModifyItemNewFlagRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestModifyItemNewFlag>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ModifyItemNewFlagRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_inventory_type(B2InventoryType2PbInventory(B2MessageInfo->inventory_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToModifyCostumeNewFlagRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestModifyCostumeNewFlag>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ModifyCostumeNewFlagRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_inventory_type(B2InventoryType2PbInventory(B2MessageInfo->inventory_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToStartRaidRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestStartRaid>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::StartRaidRequest>();
//		request->set_battle_character_type(B2CharacterType2PbCharacter(B2MessageInfo->battle_character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetContentsOpenStatusRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetContentsOpenStatus>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetContentsOpenStatusRequest>();
//		request->set_link_key(B2MessageInfo->link_key);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetDuelRewardBoxInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetDuelRewardBoxInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetDuelRewardBoxInfoRequest>();
//		request->set_duel_mode(ToDuelMode(B2MessageInfo->duel_mode));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetDuelRewardBoxGainableListRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetDuelRewardBoxGainableList>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetDuelRewardBoxGainableListRequest>();
//		request->set_duel_mode(ToDuelMode(B2MessageInfo->duel_mode));
//		request->set_grade(ToRewardBoxGrade(B2MessageInfo->grade));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToOpenDuelRewardBoxRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestOpenDuelRewardBox>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::OpenDuelRewardBoxRequest>();
//		request->set_duel_mode(ToDuelMode(B2MessageInfo->duel_mode));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveAssaultMvpRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveAssaultMvpReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveAssaultMvpRewardRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEventAttendanceRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEventAttendance>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EventAttendanceRequest>();
//		request->set_eventid(B2MessageInfo->eventId);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBeginnerAttendanceRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBeginnerAttendance>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BeginnerAttendanceRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpdateDailyPlayTimeRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpdateDailyPlayTime>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpdateDailyPlayTimeRequest>();
//		request->set_play_time_in_sec(B2MessageInfo->play_time_in_sec);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveDailyPlayTimeRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveDailyPlayTimeReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveDailyPlayTimeRewardRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetSeasonMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetSeasonMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetSeasonMissionRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardSeasonMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardSeasonMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardSeasonMissionRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_day(B2MessageInfo->day);
//		request->set_index(B2MessageInfo->index);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAttendSeasonMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAttendSeasonMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AttendSeasonMissionRequest>();
//		return request;
//	}
//
//	 aether
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAccountAetherRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAccountAether>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAccountAetherRequest>();
//		request->set_aether_type(ToAetherType(B2MessageInfo->aether_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDismantleAethersRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDismantleAethers>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DismantleAethersRequest>();
//		for (int64 aetherId : B2MessageInfo->aether_ids) {
//			request->add_aether_ids(aetherId);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToMountAethersRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestMountAethers>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::MountAethersRequest>();
//		for (int64 aetherId : B2MessageInfo->aether_ids) {
//			request->add_aether_ids(aetherId);
//		}
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUnmountAethersRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnmountAethers>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnmountAethersRequest>();
//		for (int64 aetherId : B2MessageInfo->aether_ids) {
//			request->add_aether_ids(aetherId);
//		}
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToLockAethersRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLockAethers>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LockAethersRequest>();
//		for (int64 aetherId : B2MessageInfo->aether_ids) {
//			request->add_aether_ids(aetherId);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUnlockAethersRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnlockAethers>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnlockAethersRequest>();
//		for (int64 aetherId : B2MessageInfo->aether_ids) {
//			request->add_aether_ids(aetherId);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToExtendAetherInventoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestExtendAetherInventory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ExtendAetherInventoryRequest>();
//		request->set_buy_count(B2MessageInfo->buy_count);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEnhanceAetherRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEnhanceAether>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EnhanceAetherRequest>();
//		request->set_aether_id(B2MessageInfo->aether_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSellAethersRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSellAethers>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SellAethersRequest>();
//		for (int64 aetherId : B2MessageInfo->aether_ids) {
//			request->add_aether_ids(aetherId);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToModifyAetherNewFlagRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestModifyAetherNewFlag>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ModifyAetherNewFlagRequest>();
//		request->set_aether_type(ToAetherType(B2MessageInfo->aether_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSelectAetherDefenseOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSelectAetherDefenseOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SelectAetherDefenseOptionRequest>();
//		request->set_set_option_id(B2MessageInfo->set_option_id);
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSelectAetherUnitySkillOptionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSelectAetherUnitySkillOption>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SelectAetherUnitySkillOptionRequest>();
//		request->set_set_option_id(B2MessageInfo->set_option_id);
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetUnitySkillMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetUnitySkillMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetUnitySkillMissionRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveUnitySkillRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveUnitySkillReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveUnitySkillRewardRequest>();
//		request->set_main_character_type(B2CharacterType2PbCharacter(B2MessageInfo->main_character_type));
//		request->set_unity_character_type(B2CharacterType2PbCharacter(B2MessageInfo->unity_character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAwakenUnitySkillMissionRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAwakenUnitySkillMission>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AwakenUnitySkillMissionRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_mission_id(B2MessageInfo->mission_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAwakenUnitySkillRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAwakenUnitySkill>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AwakenUnitySkillRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetDuelModeInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetDuelModeInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetDuelModeInfoRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetChallengeModeInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetChallengeModeInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetChallengeModeInfoRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveRestRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveRestReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveRestRewardRequest>();
//		request->set_reward_id(B2MessageInfo->reward_id);
//		request->set_is_double(B2MessageInfo->isDouble);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCheckModeOpenRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckModeOpen>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckModeOpenRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetHallOfFameRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto b2 = std::static_pointer_cast<FB2MessageInfoRequestGetHallOfFame>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetHallOfFameRequest>();
//		if (b2->mode > 0) {
//			request->set_mode(ToContentsMode(b2->mode));
//		}
//		request->set_need_user_info(b2->need_user_info);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPraiseTargetRankerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto b2 = std::static_pointer_cast<FB2MessageInfoRequestPraiseTargetRanker>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PraiseTargetRankerRequest>();
//		request->set_mode(ToContentsMode(b2->mode));
//		request->set_account_id(b2->account_id);
//		request->set_ranking(b2->ranking);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetTutorialProgressRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetTutorialProgress>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetTutorialProgressRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetTutorialProgressRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetTutorialProgress>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetTutorialProgressRequest>();
//		request->set_tutorial_id(B2MessageInfo->tutorial_id);
//		request->set_is_item_prepared(B2MessageInfo->is_item_prepared);
//		request->set_is_lottery_drawn(B2MessageInfo->is_lottery_drawn);
//		request->set_is_finished(B2MessageInfo->is_finished);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUpdateTutorialProgressRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUpdateTutorialProgress>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UpdateTutorialProgressRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSkipTutorialProgressRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSkipTutorialProgress>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SkipTutorialProgressRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDrawTutorialItemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDrawTutorialItem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DrawTutorialItemRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPrepareTutorialRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPrepareTutorial>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PrepareTutorialRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGiveTutorialRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGiveTutorialReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GiveTutorialRewardRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildChatRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildChat>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildChatRequest>();
//		request->set_format((b2::protocol::session::ChatFormat) B2MessageInfo->format);
//		request->set_body(TCHAR_TO_UTF8(*B2MessageInfo->body));
//
//		if (B2MessageInfo->character_level_max > 0) 
//		{
//			request->set_character_level_max(B2MessageInfo->character_level_max);
//		}
//		else
//		{
//			request->set_character_level_max(0);
//		}
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetGuildChatRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetGuildChat>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetGuildChatRequest>();
//		request->set_chat_id(B2MessageInfo->chat_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAskGuildSupportRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAskGuildSupport>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AskGuildSupportRequest>();
//		request->set_item_template_id(B2MessageInfo->item_template_id);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildSupportRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildSupport>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildSupportRequest>();
//		request->set_target_account_id(B2MessageInfo->target_account_id);
//		request->set_chat_id(B2MessageInfo->chat_id);
//		request->set_count(B2MessageInfo->count);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGuildDonationRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGuildDonation>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GuildDonationRequest>();
//		request->set_mercenary_id(B2MessageInfo->mercenary_id);
//		request->set_donation_type(ToDonationType(B2MessageInfo->donation_type));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToCheckReachPowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestCheckReachPower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::CheckReachPowerRequest>();
//		for (B2CharacterPowerPtr characterPower : B2MessageInfo->character_power) {
//			auto pb = request->add_character_power();
//			pb->set_character_type(B2CharacterType2PbCharacter(characterPower->character_type));
//			pb->set_total_power(characterPower->total_power);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToTrackingPowerDetailRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestTrackingPowerDetail>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::TrackingPowerDetailRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_item_preset_id(B2MessageInfo->item_preset_id);
//		request->set_skill_slot(B2MessageInfo->skill_slot);
//		for (B2CharacterPowerSpecificPtr powerSpecific : B2MessageInfo->specific_power_values) {
//			auto pb = request->add_specific_power_values();
//			pb->set_category(powerSpecific->category);
//			pb->set_power(powerSpecific->power);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToKakaoMessageProcessingRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestKakaoMessageProcessing>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::KakaoMessageProcessingRequest>();
//		request->set_message_box_id(TCHAR_TO_UTF8(*B2MessageInfo->message_box_id));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToKakaoCouponUseRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestKakaoCouponUse>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::KakaoCouponUseRequest>();
//		request->set_coupon_code(TCHAR_TO_UTF8(*B2MessageInfo->coupon_code));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToKakaoAccountConversionPrepareRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestKakaoAccountConversionPrepare>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::KakaoAccountConversionPrepareRequest>();
//		request->set_idp_code(TCHAR_TO_UTF8(*B2MessageInfo->idp_code));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToKakaoAccountConversionCompleteRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestKakaoAccountConversionComplete>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::KakaoAccountConversionCompleteRequest>();
//		request->set_idp_code(TCHAR_TO_UTF8(*B2MessageInfo->idp_code));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetEventRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceivePointEventRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceivePointEventReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceivePointEventRewardRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_reward_index(B2MessageInfo->reward_index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveLevelUpEventRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveLevelUpEventReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveLevelUpEventRewardRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_index(B2MessageInfo->index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveStageClearEventRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveStageClearEventReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveStageClearEventRewardRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_index(B2MessageInfo->index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuyHotTimeBuffRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuyHotTimeBuff>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuyHotTimeBuffRequest>();
//		request->set_buff_type((b2::protocol::commondata::HotTimeBuffType)B2MessageInfo->buff_type);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardFlatRatePackageBonusRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardFlatRatePackageBonus>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardFlatRatePackageBonusRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetPackageShopRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetPackageShop>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetPackageShopRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetPeriodPackageStateRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetPeriodPackageState>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetPeriodPackageStateRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardLevelUpPackageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardLevelUpPackage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardLevelUpPackageRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_package_id(B2MessageInfo->package_id);
//		request->set_level(B2MessageInfo->level);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardReachLevelPackageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardReachLevelPackage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardReachLevelPackageRequest>();
//		request->set_package_id(B2MessageInfo->package_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGiveUpQuestRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGiveUpQuest>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GiveUpQuestRequest>();
//		request->set_slot(B2QuestSlotTypeToPbQuestSlotType(B2MessageInfo->slot));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetContentsModeStateRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetContentsModeState>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetContentsModeStateRequest>();
//		if (B2MessageInfo->mode != B2ContentsMode::NONE) {
//			request->set_mode(ToContentsMode(B2MessageInfo->mode));
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardPointShopEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardPointShopEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardPointShopEventRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_index(B2MessageInfo->index);
//		if (B2MessageInfo->buy_count > 0)
//		{
//			request->set_buy_count(B2MessageInfo->buy_count);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReportUserRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReportUser>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReportUserRequest>();
//		request->set_target_account_id(B2MessageInfo->target_account_id);
//		request->set_mode_type((b2::protocol::commondata::ReportModeType)B2MessageInfo->mode_type);
//		request->set_reason_type(B2MessageInfo->reason_type);
//		request->set_explain(TCHAR_TO_UTF8(*B2MessageInfo->explain));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBlockChatRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBlockChat>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BlockChatRequest>();
//		request->set_block_account_id(B2MessageInfo->block_account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUnblockChatRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnblockChat>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnblockChatRequest>();
//		request->set_unblock_account_id(B2MessageInfo->unblock_account_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetBlockChatListRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetBlockChatList>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetBlockChatListRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToClientStateReportRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestClientStateReport>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ClientStateReportRequest>();
//		request->set_category(TCHAR_TO_UTF8(*B2MessageInfo->category));
//		request->set_event(TCHAR_TO_UTF8(*B2MessageInfo->event));
//		request->set_label(TCHAR_TO_UTF8(*B2MessageInfo->label));
//		request->set_detail_info(TCHAR_TO_UTF8(*B2MessageInfo->detail_info));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardTenLotteryEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardTenLotteryEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardTenLotteryEventRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_index(B2MessageInfo->index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToReceiveGemPurchaseEventRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestReceiveGemPurchaseEventReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ReceiveGemPurchaseEventRewardRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_reward_index(B2MessageInfo->index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardSpendGemEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardSpendGemEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardSpendGemEventRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_index(B2MessageInfo->index);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardTenLotterySavingsEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardTenLotterySavingsEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardTenLotterySavingsEventRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRollDiceEventRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRollDiceEvent>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RollDiceEventRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_is_free_dice(B2MessageInfo->is_free_dice);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToBuyDiceEventPointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestBuyDiceEventPoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::BuyDiceEventPointRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRewardDiceEventMileageRequest(FB2MessageInfoPtr messageInfo)
//	{
//
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRewardDiceEventMileage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RewardDiceEventMileageRequest>();
//		request->set_event_id(B2MessageInfo->event_id);
//		request->set_reward_index(B2MessageInfo->reward_index);
//		return request;
//	}
//
//	 REPLACEMENT POINT: CONVERTER CPP REQUEST
//	 NEVER DELETE LINE ABOVE
//
//	MessagePtr FB2MessageInfoConverter::ToChatCommand(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoChatCommand>(messageInfo);
//
//        auto command = std::make_shared<b2::protocol::session::ChatCommand>();
//        command->set_type(ToChatType(B2MessageInfo->type));
//		command->set_to(TCHAR_TO_UTF8(*B2MessageInfo->to));
//        command->set_format((b2::protocol::session::ChatFormat) B2MessageInfo->format);
//		command->set_body(TCHAR_TO_UTF8(*B2MessageInfo->body));
//
//		if (B2MessageInfo->character_level_max > 0)
//		{
//			command->set_character_level_max(B2MessageInfo->character_level_max);
//		}
//		else
//		{
//			command->set_character_level_max(0);
//		}
//        return command;
//    }
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDeleteAccountRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDeleteAccount>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DeleteAccountRequest>();
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToAddItemToInventoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAddItemToInventory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AddItemToInventoryRequest>();
//		request->set_item_template_id(B2MessageInfo->item_template_id);
//		request->set_item_quality(ToItemQuality(B2MessageInfo->item_quality));
//		request->set_item_amount(B2MessageInfo->item_amount);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToAddCostumeToInventoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAddCostumeToInventory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AddCostumeToInventoryRequest>();
//		request->set_template_id(B2MessageInfo->template_id);
//		return request;
//	}
//
//	MessagePtr FB2MessageInfoConverter::ToIncreaseCashPaidRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestIncreaseCashPaid>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::IncreaseCashPaidRequest>();
//		request->set_cash_paid(B2MessageInfo->cash_paid);
//		return request;
//	}
//	
//	b2network::MessagePtr FB2MessageInfoConverter::ToIncreaseMoneyRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestIncreaseMoney>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::IncreaseMoneyRequest>();
//		request->set_money(B2MessageInfo->money);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetItemLevelupRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetItemLevelup>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetItemLevelupRequest>();
//		request->set_item_id(B2MessageInfo->item_id);
//		request->set_level(B2MessageInfo->level);
//		request->set_exp(B2MessageInfo->exp);
//		request->set_enhance_level(B2MessageInfo->enhance_level);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetCharacterLevelExpRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetCharacterLevelExp>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetCharacterLevelExpRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_level(B2MessageInfo->level);
//		request->set_exp(B2MessageInfo->exp);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetCharacterRankInfoRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetCharacterRankInfo>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetCharacterRankInfoRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_rank(B2MessageInfo->rank);
//		request->set_node_index(B2MessageInfo->nodeIndex);
//		request->set_node_state(B2MessageInfo->nodeState);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetCharacterRelicRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetCharacterRelic>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetCharacterRelicRequest>();
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//		request->set_relic_id(B2MessageInfo->relic_id);
//		request->set_grade(B2MessageInfo->grade);
//		request->set_enhance_level(B2MessageInfo->enhance_level);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetBladePointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetBladePoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetBladePointRequest>();
//		request->set_blade_point(B2MessageInfo->blade_point);
//		return request;
//	}
//
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToIncreaseRankPromotionPointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestIncreaseRankPromotionPoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::IncreaseRankPromotionPointRequest>();
//		request->set_rank_promotion_point(B2MessageInfo->rank_promotion_point);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAddMailRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAddMail>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AddMailRequest>();
//		request->set_mail_category(B2MailCategory2PbMailCategory(B2MessageInfo->mail_category));
//		request->set_attach_type(B2MailPresentAttachType2PbMailPresentAttachType(B2MessageInfo->attach_type));
//		request->set_source_type((b2::protocol::commondata::MailSourceType)B2MessageInfo->source_type);
//		request->set_sender_type((b2::protocol::commondata::MailSenderType)B2MessageInfo->sender_type);
//		request->set_amount(B2MessageInfo->amount);
//		if (B2MessageInfo->item_template_id != 0)
//		{
//			request->set_item_template_id(B2MessageInfo->item_template_id);
//		}
//
//		if (B2MessageInfo->general_lottery_id != 0)
//		{
//			request->set_general_lottery_id(B2MessageInfo->general_lottery_id);
//		}
//
//		if (B2MessageInfo->fixed_grade_lottery_id != 0)
//		{
//			request->set_fixed_grade_lottery_id(B2MessageInfo->fixed_grade_lottery_id);
//		}
//
//		if (B2MessageInfo->selective_lottery_id != 0)
//		{
//			request->set_selective_lottery_id(B2MessageInfo->selective_lottery_id);
//		}
//
//		if (B2MessageInfo->selective_item_lottery_id != 0)
//		{
//			request->set_selective_item_lottery_id(B2MessageInfo->selective_item_lottery_id);
//		}
//
//		if (B2MessageInfo->prefix_selective_lottery_id != 0)
//		{
//			request->set_prefix_selective_lottery_id(B2MessageInfo->prefix_selective_lottery_id);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToMakeRankingRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestMakeRanking>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::MakeRankingRequest>();
//		request->set_match_type(B2MatchInfoType2PbMatchInfoType(B2MessageInfo->match_type));
//		request->set_ranking(B2MessageInfo->ranking);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToForceClearStageRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestForceClearStage>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ForceClearStageRequest>();
//		request->set_stage_id(B2MessageInfo->stage_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetQuestStateRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetQuestState>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetQuestStateRequest>();
//		request->set_quest_id(B2MessageInfo->quest_id);
//		request->set_quest_state(B2MessageInfo->quest_state);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToIncreaseHeroPieceRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestIncreaseHeroPiece>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::IncreaseHeroPieceRequest>();
//		request->set_hero_piece(B2MessageInfo->hero_piece);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToForceClearHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestForceClearHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ForceClearHeroTowerRequest>();
//		request->set_floor(B2MessageInfo->floor);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToForceStartCounterDungeonRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestForceStartCounterDungeon>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ForceStartCounterDungeonRequest>();
//		request->set_dungeon_id(B2MessageInfo->dungeon_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToIncreaseSocialPointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestIncreaseSocialPoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::IncreaseSocialPointRequest>();
//		request->set_social_point(B2MessageInfo->social_point);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPrepareSweepHeroTowerRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPrepareSweepHeroTower>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PrepareSweepHeroTowerRequest>();
//		
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPrepareGuildRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPrepareGuild>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PrepareGuildRequest>();
//		request->set_member_count(B2MessageInfo->member_count);
//		request->set_add_guild_count(B2MessageInfo->add_guild_count);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetGuildBattleStateRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetGuildBattleState>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetGuildBattleStateRequest>();
//		request->set_is_new_season(B2MessageInfo->is_new_season);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToResetAttendanceTimeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestResetAttendanceTime>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ResetAttendanceTimeRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToResetEventAttendanceRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestResetEventAttendance>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ResetEventAttendanceRequest>();
//		request->set_is_only_attendance_time(B2MessageInfo->is_only_attendance_time);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToResetDailyPlayTimeRecordRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestResetDailyPlayTimeRecord>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ResetDailyPlayTimeRecordRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToResetTeamMatchPointRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestResetTeamMatchPoint>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ResetTeamMatchPointRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToChangeQuestIdRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestChangeQuestId>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ChangeQuestIdRequest>();
//
//		request->set_slot_type(B2QuestSlotTypeToPbQuestSlotType(B2MessageInfo->slot_type));
//		request->set_quest_id(B2MessageInfo->quest_id);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToChangeQuestCountRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestChangeQuestCount>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ChangeQuestCountRequest>();
//
//		request->set_slot_type(B2QuestSlotTypeToPbQuestSlotType(B2MessageInfo->slot_type));
//		request->set_count(B2MessageInfo->count);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToPrepareFriendRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestPrepareFriend>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::PrepareFriendRequest>();
//
//		request->set_friend_count(B2MessageInfo->friend_count);
//		request->set_job_type((b2::protocol::session::PrepareFriendRequest_JOB)B2MessageInfo->job_type);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetGuildLevelExpRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetGuildLevelExp>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetGuildLevelExpRequest>();
//
//		request->set_level(B2MessageInfo->level);
//		request->set_exp(B2MessageInfo->exp);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAddAetherToInventoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAddAetherToInventory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AddAetherToInventoryRequest>();
//
//		request->set_aether_type(ToAetherType(B2MessageInfo->aether_type));
//		request->set_slot(B2MessageInfo->slot);
//		request->set_grade(B2MessageInfo->grade);
//		request->set_tier_type(ToAetherTier(B2MessageInfo->tier_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToForceUpdateAetherRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestForceUpdateAether>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ForceUpdateAetherRequest>();
//
//		request->set_aether_id(B2MessageInfo->aether_id);
//		request->set_type(ToAetherType(B2MessageInfo->type));
//		request->set_level(B2MessageInfo->level);
//		request->set_grade(B2MessageInfo->grade);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetMatchSeasonRewardRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetMatchSeasonReward>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetMatchSeasonRewardRequest>();
//
//		request->set_match_type(B2MatchInfoType2PbMatchInfoType(B2MessageInfo->match_type));
//		request->set_ranking(B2MessageInfo->ranking);
//		request->set_point(B2MessageInfo->point);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetStageBoostCountRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetStageBoostCount>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetStageBoostCountRequest>();
//
//		request->set_stage_boost_ticket(B2MessageInfo->stage_boost_ticket);
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSkipMatchTutorialRequest(FB2MessageInfoPtr messageInfo)
//	{
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSkipMatchTutorial>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::SkipMatchTutorialRequest>();
//		request->set_match_type(B2MatchInfoType2PbMatchInfoType(B2MessageInfo->match_type));
//        return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToServerCmdRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestServerCmd>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ServerCmdRequest>();
//		request->set_command(TCHAR_TO_UTF8(*B2MessageInfo->command));
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetDebugModeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetDebugMode>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SetDebugModeRequest>();
//		request->set_skip_keep_alive_check(B2MessageInfo->skip_keep_alive_check);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSetRoomDebugModeRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSetRoomDebugMode>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::channel::SetRoomDebugModeRequest>();
//		request->set_skip_room_heart_beat_check(B2MessageInfo->skip_room_heart_beat_check);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToTraceRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestTrace>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::TraceRequest>();
//		request->set_tag(TCHAR_TO_ANSI(*B2MessageInfo->tag));
//
//		auto now = std::chrono::steady_clock::now().time_since_epoch();
//		auto ts = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
//		request->set_client_send_time(ts);
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetChatChannelRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoGetChatChannelRequest>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::GetChatChannelRequest>();
//        return request;
//    }
//
//    b2network::MessagePtr FB2MessageInfoConverter::ToChangeChatChannelRequest(FB2MessageInfoPtr messageInfo)
//    {
//        auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoChangeChatChannelRequest>(messageInfo);
//
//        auto request = std::make_shared<b2::protocol::session::ChangeChatChannelRequest>();
//        request->set_channel_number(B2MessageInfo->channel_number);
//        return request;
//    }
//
//	 totem
//	b2network::MessagePtr FB2MessageInfoConverter::ToGetAccountTotemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestGetAccountTotem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::GetAccountTotemRequest>();
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToEquipTotemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestEquipTotems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::EquipTotemsRequest>();
//
//		for (int64 totemId : B2MessageInfo->totem_ids) {
//			request->add_totem_ids(totemId);
//		}
//		request->set_character_type(B2CharacterType2PbCharacter(B2MessageInfo->character_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUnequipTotemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnequipTotems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnequipTotemsRequest>();
//
//		for (int64 totemId : B2MessageInfo->totem_ids) {
//			request->add_totem_ids(totemId);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToLockTotemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestLockTotems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::LockTotemsRequest>();
//
//		for (int64 totemId : B2MessageInfo->totem_ids) {
//			request->add_totem_ids(totemId);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToUnlockTotemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestUnlockTotems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::UnlockTotemsRequest>();
//
//		for (int64 totemId : B2MessageInfo->totem_ids) {
//			request->add_totem_ids(totemId);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToSellTotemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestSellTotems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::SellTotemsRequest>();
//
//		for (int64 totemId : B2MessageInfo->totem_ids) {
//			request->add_totem_ids(totemId);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToDismantleTotemsRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestDismantleTotems>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::DismantleTotemsRequest>();
//
//		for (int64 totemId : B2MessageInfo->totem_ids) {
//			request->add_totem_ids(totemId);
//		}
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToRefineTotemRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestRefineTotem>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::RefineTotemRequest>();
//
//		request->set_totem_id(B2MessageInfo->totem_id);
//		request->set_option_id(B2MessageInfo->option_id);
//		request->set_cost_type(B2TotemRefineCostType2PbTotemRefineCostType(B2MessageInfo->cost_type));
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToModifyTotemNewFlagRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestModifyTotemNewFlag>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::ModifyTotemNewFlagRequest>();
//
//		return request;
//	}
//
//	b2network::MessagePtr FB2MessageInfoConverter::ToAddTotemToInventoryRequest(FB2MessageInfoPtr messageInfo)
//	{
//		auto B2MessageInfo = std::static_pointer_cast<FB2MessageInfoRequestAddTotemToInventory>(messageInfo);
//
//		auto request = std::make_shared<b2::protocol::session::AddTotemToInventoryRequest>();
//		request->set_template_id(B2MessageInfo->template_id);
//
//		return request;
//	}
//
//
//	////////////////////////////////////////////////////////////////////////
//	 response protocol-buffers to FB2MessageInfoResponse
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromSessionClosedNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::session::SessionClosedNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoSessionClosedNotify>();
//        B2MessageInfoNotify->reason = notify->reason();
//        return B2MessageInfoNotify;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAccountResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAccountResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAccount>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->account = AccountPb2B2AccountInfo(response->account());
//
//			for (auto character : response->characters()) 
//			{
//				B2MessageInfoResponse->characters.Emplace(CharacterPb2B2CharacterServerInfo(character));
//			}
//			
//			 equip items
//			for (auto item : response->equips()) 
//			{
//				B2MessageInfoResponse->equips.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//			
//			 item preset
//			for (auto itemPreset : response->item_presets()) 
//			{
//				B2MessageInfoResponse->item_presets.Emplace(ItemPreset2B2ItemPreset(itemPreset));
//			}
//
//			 guild infos
//			if (response->has_guild_name())
//			{
//				B2MessageInfoResponse->guild_name = UTF8_TO_TCHAR(response->guild_name().c_str());
//			}
//			else 
//			{
//				B2MessageInfoResponse->guild_name = "";
//			}
//
//			if (response->has_mark_index())
//			{
//				B2MessageInfoResponse->mark_index = response->mark_index();
//			}
//			else
//			{
//				B2MessageInfoResponse->mark_index = 0;
//			}
//
//			if (response->has_mark_color())
//			{
//				B2MessageInfoResponse->mark_color = response->mark_color();
//			}
//			else
//			{
//				B2MessageInfoResponse->mark_color = 0;
//			}
//
//			if (response->has_mark_bg_index())
//			{
//				B2MessageInfoResponse->mark_bg_index = response->mark_bg_index();
//			}
//			else
//			{
//				B2MessageInfoResponse->mark_bg_index = 0;
//			}
//
//			if (response->has_mark_bg_color())
//			{
//				B2MessageInfoResponse->mark_bg_color = response->mark_bg_color();
//			}
//			else
//			{
//				B2MessageInfoResponse->mark_bg_color = 0;
//			}
//
//			if (response->has_my_rank())
//			{
//				B2MessageInfoResponse->my_rank = ToB2GuildMemberRank(response->my_rank());
//			}
//			else
//			{
//				B2MessageInfoResponse->my_rank = 0;
//			}
//
//			 relic infos
//			for (auto relic : response->relics()) 
//			{
//				B2MessageInfoResponse->relics.Emplace(ToB2Relic(relic));
//			}
//
//			 wing options
//			for (auto wingOption : response->wing_options())
//			{
//				B2MessageInfoResponse->wing_options.Emplace(ToB2WingOptionInfo(wingOption));
//			}
//
//			 guild skill
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//
//			 event attendance status
//			for (auto event_attendance : response->event_attendance_statuses())
//			{
//				B2MessageInfoResponse->event_attendance_statuses.Emplace(ToB2EventAttendanceStatus(event_attendance));
//			}
//
//			 aether inventory count
//			if (response->has_aether_inventory_slot_count())
//			{
//				B2MessageInfoResponse->aether_inventory_slot_count = response->aether_inventory_slot_count();
//			}
//			else
//			{
//				B2MessageInfoResponse->aether_inventory_slot_count = 0;
//			}
//
//			 mount aether
//			for (auto aether : response->mount_aethers())
//			{
//				B2MessageInfoResponse->mount_aethers.Emplace(AetherPb2B2AetherServerInfo(aether));
//			}
//
//			 fairy status
//			for (auto fairy : response->fairies())
//			{
//				B2MessageInfoResponse->fairies.Emplace(ToB2FairyStatus(fairy));
//			}
//
//			if (response->has_is_all_tutorial_finished()) 
//			{
//				B2MessageInfoResponse->is_all_tutorial_finished = response->is_all_tutorial_finished();
//			}
//			else 
//			{
//				B2MessageInfoResponse->is_all_tutorial_finished = false;
//			}
//
//			 unity skills
//			for (auto skill : response->unity_skills())
//			{
//				B2MessageInfoResponse->unitySkills.Emplace(ToB2UnitySkillMission(skill));
//			}
//
//			 unity skill awaken
//			for (auto awaken : response->unity_awaken_missions()) 
//			{
//				B2MessageInfoResponse->unity_awaken_missions.Emplace(ToB2UnitySkillAwakenMission(awaken));
//			}
//
//			 rolling banner
//			for (auto rollingBanner : response->rolling_banners())
//			{
//				B2MessageInfoResponse->rolling_banners.Emplace(ToB2RollingBanner(rollingBanner));
//			}
//
//			if (response->has_beginner_attendance_status())
//			{
//				B2MessageInfoResponse->beginner_attendance_status = ToB2BeginnerAttendanceStatus(response->beginner_attendance_status());
//			}
//			else
//			{
//				B2MessageInfoResponse->beginner_attendance_status = 0;
//			}
//
//			costume equip
//			for (auto costume  : response->equip_costumes())
//			{
//				B2MessageInfoResponse->equip_costumes.Emplace(CostumePb2B2CostumeServerInfo(costume));
//			}
//
//			totem equip
//			for (auto totem : response->equip_totems())
//			{
//				B2MessageInfoResponse->equip_totems.Emplace(TotemPb2B2TotemServerInfo(totem));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFindAccountResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FindAccountResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseFindAccount>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int accountCount = response->account_infos_size();
//			for (int i = 0; i < accountCount; ++i)
//			{
//				const b2::protocol::commondata::AccountInfo& accountInfo = response->account_infos(i);
//
//				auto b2FindAccountInfo = AccountInfoPb2B2FindAccount(accountInfo);
//				if (b2FindAccountInfo)
//				{
//					B2MessageInfoResponse->account_infos.Emplace(b2FindAccountInfo);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAccountItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAccountItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAccountItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 items
//			int itemCount = response->items_size();
//			for (int i = 0; i < itemCount; ++i)
//			{
//				const b2::protocol::commondata::Item& item = response->items(i);
//
//				auto b2Item = ItemPb2B2ItemServerInfo(item);
//				if (b2Item)
//				{
//					B2MessageInfoResponse->items.Emplace(b2Item);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAccountCostumeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAccountCostumeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAccountCostume>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 costumes
//			int costumeCount = response->costumes_size();
//			for (int i = 0; i < costumeCount; ++i)
//			{
//				const b2::protocol::commondata::Costume& costume = response->costumes(i);
//
//				auto b2Costume = CostumePb2B2CostumeServerInfo(costume);
//				if (b2Costume)
//				{
//					B2MessageInfoResponse->costumes.Emplace(b2Costume);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetMasterDataResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetMasterDataResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetMasterData>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 config Data
//			if (response->has_config_data())
//			{
//				B2MessageInfoResponse->config_data = ConfigDataPb2B2ConfigData(response->config_data());
//			}
//
//			 mdSkillInfos
//			int mdSkillInfoCount = response->md_skill_infos_size();
//			for (int i = 0; i < mdSkillInfoCount; ++i)
//			{
//				const b2::protocol::masterdata::mdSkillInfo& skillInfo = response->md_skill_infos(i);
//
//				auto b2SkillInfo = mdSkillInfoPb2B2mdSkillInfo(skillInfo);
//				if (b2SkillInfo)
//				{
//					B2MessageInfoResponse->md_skill_infos.Emplace(b2SkillInfo);
//				}
//			}
//
//			 mdSkillOptions
//			int mdSkillOptionCount = response->md_skill_options_size();
//			for (int i = 0; i < mdSkillOptionCount; ++i)
//			{
//				const b2::protocol::masterdata::mdSkillOption& skillOption = response->md_skill_options(i);
//
//				auto b2SkillOption = mdSkillOptionPb2B2mdSkillOption(skillOption);
//				if (b2SkillOption)
//				{
//					B2MessageInfoResponse->md_skill_options.Emplace(b2SkillOption);
//				}
//			}
//
//			 mdItemLevelupExpFactor
//			int mdItemLevelupExpFactorCount = response->md_item_levelup_exp_factors_size();
//			for (int i = 0; i < mdItemLevelupExpFactorCount; ++i)
//			{
//				const b2::protocol::masterdata::mdItemLevelupExpFactor& itemExpFactor = response->md_item_levelup_exp_factors(i);
//
//				auto b2ItemExpFactor = mdItemLevelupExpFactorPb2B2mdItemLevelupExpFactor(itemExpFactor);
//				if (b2ItemExpFactor)
//				{
//					B2MessageInfoResponse->md_item_levelup_exp_factors.Emplace(b2ItemExpFactor);
//				}
//			}
//
//			 mdItemOptionAddValue
//			int mdItemOptionAddValueCount = response->md_item_option_add_values_size();
//			for (int i = 0; i < mdItemOptionAddValueCount; ++i)
//			{
//				const b2::protocol::masterdata::mdItemOptionAddValue& itemOptionValue = response->md_item_option_add_values(i);
//
//				auto b2ItemOptionValue = mdItemOptionAddValuePb2B2mdItemOptionAddValue(itemOptionValue);
//				if (b2ItemOptionValue)
//				{
//					B2MessageInfoResponse->md_item_option_add_values.Emplace(b2ItemOptionValue);
//				}
//			}
//
//			 mdItemLevelupCosts
//			int mdItemLevelupCostCount = response->md_item_levelup_costs_size();
//			for (int i = 0; i < mdItemLevelupCostCount; ++i) 
//			{
//				const b2::protocol::masterdata::mdItemLevelupCost& itemLevelupCost = response->md_item_levelup_costs(i);
//
//				auto b2ItemLevelupCost = mdItemLevelupCostPb2B2mdItemLevelupCost(itemLevelupCost);
//				if (b2ItemLevelupCost)
//				{
//					B2MessageInfoResponse->md_item_levelup_costs.Emplace(b2ItemLevelupCost);
//				}
//			}
//
//			 mdStageInfos
//			int mdStageInfoCount = response->md_stage_infos_size();
//			for (int i = 0; i < mdStageInfoCount; ++i)
//			{
//				const b2::protocol::masterdata::mdStageInfo& stageInfo = response->md_stage_infos(i);
//
//				auto b2StageInfo = mdStageInfoPb2B2mdStageInfo(stageInfo);
//				if (b2StageInfo)
//				{
//					B2MessageInfoResponse->md_stage_infos.Emplace(b2StageInfo);
//				}
//			}
//
//			 mdResurrectionInfo
//			int mdResurrectionCount = response->md_resurrection_infos_size();
//			for (int i = 0; i < mdResurrectionCount; ++i) {
//				const b2::protocol::masterdata::mdResurrectionInfo& resurrectionInfo = response->md_resurrection_infos(i);
//
//				auto b2ResurrectionInfo = mdResurrectionInfoPb2B2ResurrectionInfo(resurrectionInfo);
//				if (b2ResurrectionInfo)
//				{
//					B2MessageInfoResponse->md_resurrection_infos.Emplace(b2ResurrectionInfo);
//				}
//			}
//
//			 mdTeamBattleFormationInfo
//			int mdTeamBattleFormationCount = response->md_formation_infos_size();
//			for (int i = 0; i < mdTeamBattleFormationCount; ++i) {
//				const b2::protocol::masterdata::mdTeamBattleFormationInfo& formationInfo = response->md_formation_infos(i);
//
//				auto b2FormationInfo = mdTeamBattleFormationInfoPb2B2mdTeamBattleFormationInfo(formationInfo);
//				if (b2FormationInfo)
//				{
//					B2MessageInfoResponse->md_formation_infos.Emplace(b2FormationInfo);
//				}
//			}
//
//			 mdRankInfo
//			int mdRankCount = response->md_rank_infos_size();
//			for (int i = 0; i < mdRankCount; ++i) {
//				const b2::protocol::masterdata::mdRankInfo& rankInfo = response->md_rank_infos(i);
//
//				auto b2RankInfo = mdRankInfoPb2B2mdRankInfo(rankInfo);
//				if (b2RankInfo)
//				{
//					B2MessageInfoResponse->md_rank_infos.Emplace(b2RankInfo);
//				}
//			}
//
//			 mdDailyMatchReward
//			for (auto reward : response->md_daily_match_rewards()) 
//			{
//				auto b2DailyMatchReward = mdDailyMatchRewardPb2B2mdDailyMatchReward(reward);
//				if (b2DailyMatchReward) 
//				{
//					B2MessageInfoResponse->md_daily_match_rewards.Emplace(b2DailyMatchReward);
//				}
//			}
//
//			 pvp mdMatchRankingReward
//			for (auto reward : response->md_pvp_ranking_rewards())
//			{
//				auto b2PvPRankingReward = mdMatchRankingRewardPb2B2mdMatchRankingReward(reward);
//				if (b2PvPRankingReward)
//				{
//					B2MessageInfoResponse->md_pvp_ranking_rewards.Emplace(b2PvPRankingReward);
//				}
//			}
//
//			 team mdMatchRankingReward
//			for (auto reward : response->md_team_ranking_rewards())
//			{
//				auto b2TeamRankingReward = mdMatchRankingRewardPb2B2mdMatchRankingReward(reward);
//				if (b2TeamRankingReward)
//				{
//					B2MessageInfoResponse->md_team_ranking_rewards.Emplace(b2TeamRankingReward);
//				}
//			}
//
//			 pvp mdMatchRankingReward
//			for (auto reward : response->md_assault_ranking_rewards())
//			{
//				auto b2PvPRankingReward = mdMatchRankingRewardPb2B2mdMatchRankingReward(reward);
//				if (b2PvPRankingReward)
//				{
//					B2MessageInfoResponse->md_assault_ranking_rewards.Emplace(b2PvPRankingReward);
//				}
//			}
//
//			 counter dungeon
//			for (auto counterDungeon : response->md_counter_dungeons())
//			{
//				auto b2CounterDungeon = ToB2mdCounterDungeon(counterDungeon);
//				if (b2CounterDungeon)
//				{
//					B2MessageInfoResponse->md_counter_dungeons.Emplace(b2CounterDungeon);
//				}
//			}
//
//			 counter dungeon period
//			for (auto period : response->md_counter_dungeon_periods())
//			{
//				auto b2Period = ToB2mdCounterDungeonPeriod(period);
//				if (b2Period)
//				{
//					B2MessageInfoResponse->md_counter_dungeon_periods.Emplace(b2Period);
//				}
//			}
//
//			 counter dungeon week reward
//			for (auto pbReward : response->md_counter_dungeon_week_rewards())
//			{
//				auto b2Reward = ToB2mdCounterDungeonWeekReward(pbReward);
//				if (b2Reward)
//				{
//					B2MessageInfoResponse->md_counter_dungeon_week_rewards.Emplace(b2Reward);
//				}
//			}
//
//			 main quest info
//			for (auto quest : response->md_main_quest_infos())
//			{
//				auto b2MainQuestInfo = mdMainQuestInfoPb2B2mdMainQuestInfo(quest);
//				if (b2MainQuestInfo)
//				{
//					B2MessageInfoResponse->md_main_quest_infos.Emplace(b2MainQuestInfo);
//				}
//			}
//
//			 daily quest info
//			for (auto quest : response->md_daily_quest_infos())
//			{
//				auto b2DailyQuestInfo = mdDailyQuestInfoPb2B2mdDailyQuestInfo(quest);
//				if (b2DailyQuestInfo)
//				{
//					B2MessageInfoResponse->md_daily_quest_infos.Emplace(b2DailyQuestInfo);
//				}
//			}
//
//			 reward info
//			for (auto reward : response->md_reward_infos())
//			{
//				auto b2RewardInfo = mdRewardInfoPb2B2mdRewardnfo(reward);
//				if (b2RewardInfo)
//				{
//					B2MessageInfoResponse->md_reward_infos.Emplace(b2RewardInfo);
//				}
//			}
//
//			 mission info
//			for (auto mission : response->md_mission_infos())
//			{
//				auto b2MissionInfo = ToB2mdMissionInfo(mission);
//				if (b2MissionInfo)
//				{
//					B2MessageInfoResponse->md_mission_infos.Emplace(b2MissionInfo);
//				}
//			}
//
//			 serial mission info
//			for (auto mission : response->md_serial_mission_infos())
//			{
//				auto b2MissionInfo = ToB2mdSerialMissionInfo(mission);
//				if (b2MissionInfo)
//				{
//					B2MessageInfoResponse->md_serial_mission_infos.Emplace(b2MissionInfo);
//				}
//			}
//
//			 mission complete point info
//			for (auto pointInfo : response->md_mission_complete_point_infos())
//			{
//				auto b2PointInfo = ToB2mdMissionCompletePointInfo(pointInfo);
//				if (b2PointInfo)
//				{
//					B2MessageInfoResponse->md_mission_complete_point_infos.Emplace(b2PointInfo);
//				}
//			}
//
//			 wing enhance info
//			for (auto enhanceInfo : response->md_wing_enhance_infos())
//			{
//				auto b2Info = ToB2mdWingEnhance(enhanceInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_wing_enhance_infos.Emplace(b2Info);
//				}
//			}
//
//			 wing upgrade info
//			for (auto upgradeInfo : response->md_wing_upgrade_infos())
//			{
//				auto b2Info = ToB2mdWingUpgrade(upgradeInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_wing_upgrade_infos.Emplace(b2Info);
//				}
//			}
//
//			 wing option value info
//			for (auto optionValueInfo : response->md_wing_option_value_infos())
//			{
//				auto b2Info = ToB2mdWingOptionValue(optionValueInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_wing_option_value_infos.Emplace(b2Info);
//				}
//			}
//
//			 wing option open info
//			for (auto optionOpenInfo : response->md_wing_option_open_infos())
//			{
//				auto b2Info = ToB2mdWingOptionOpen(optionOpenInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_wing_option_open_infos.Emplace(b2Info);
//				}
//			}
//
//			 wing option max level info
//			for (auto optionMaxLevelInfo : response->md_wing_option_max_level_infos())
//			{
//				auto b2Info = ToB2mdWingOptionMaxLevel(optionMaxLevelInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_wing_option_max_level_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_hero_tower_floors
//			for (auto heroTowerFloor : response->md_hero_tower_floors())
//			{
//				auto b2Info = ToB2mdHeroTowerFloor(heroTowerFloor);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_hero_tower_floors.Emplace(b2Info);
//				}
//			}
//
//			 md_relic_infos
//			for (auto relicInfo : response->md_relic_infos())
//			{
//				auto b2Info = ToB2mdRelicInfo(relicInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_relic_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_relic_grade_infos
//			for (auto relicGradeInfo : response->md_relic_grade_infos())
//			{
//				auto b2Info = ToB2mdRelicGradeInfo(relicGradeInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_relic_grade_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_relic_level_infos
//			for (auto relicLevelInfo : response->md_relic_level_infos())
//			{
//				auto b2Info = ToB2mdRelicLevelInfo(relicLevelInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_relic_level_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_shop_lottery_prices
//			for (auto shopPriceInfo : response->md_shop_lottery_prices())
//			{
//				auto b2Info = ToB2mdShopLotteryPrice(shopPriceInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_lottery_prices.Emplace(b2Info);
//				}
//			}
//
//			 md_shop_lottery_possible_grades
//			for (auto shopPriceInfo : response->md_shop_lottery_possible_grades())
//			{
//				auto b2Info = ToB2mdShopLotteryPossibleGrade(shopPriceInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_lottery_possible_grades.Emplace(b2Info);
//				}
//			}
//
//			md_shop_special_lottery_grades
//			for (auto gradeInfo : response->md_shop_lottery_special_grades())
//			{
//				auto b2Info = ToB2mdShopLotterySpecialGrade(gradeInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_lottery_special_grades.Emplace(b2Info);
//				}
//			}
//
//			md_shop_products
//			for (auto productInfo : response->md_shop_products())
//			{
//				auto b2Info = ToB2mdShopProduct(productInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_products.Emplace(b2Info);
//				}
//			}
//
//			md_shop_packages
//			for (auto packageInfo : response->md_shop_packages())
//			{
//				auto b2Info = ToB2mdShopPackage(packageInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_packages.Emplace(b2Info);
//				}
//			}
//
//			md_shop_mileage_rewards
//			for (auto packageInfo : response->md_shop_mileage_rewards())
//			{
//				auto b2Info = ToB2mdShopMileageReward(packageInfo);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_mileage_rewards.Emplace(b2Info);
//				}
//			}
//
//			md_collection_reward
//			for (auto reward : response->md_collection_rewards())
//			{
//				auto b2Info = ToB2CollectionReward(reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_collection_rewards.Emplace(b2Info);
//				}
//			}
//
//			md_daily_attemdance
//			for (auto info : response->md_daily_atttendances())
//			{
//				auto b2Info = ToB2mdDailyAttendanceInfo(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_daily_atttendances.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_battle_result_rewards
//			for (auto info : response->md_guild_battle_result_rewards())
//			{
//				auto b2Info = ToB2mdGuildBattleResultReward(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_battle_result_rewards.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_battle_season_rewards
//			for (auto info : response->md_guild_battle_season_rewards())
//			{
//				auto b2Info = ToB2mdGuildBattleSeasonReward(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_battle_season_rewards.Emplace(b2Info);
//				}
//			}
//
//			 md_raid_info
//			for (auto info : response->md_raid_infos())
//			{
//				auto b2Info = ToB2mdRaidInfo(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_raid_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_monster_info
//			for (auto info : response->md_monster_infos())
//			{
//				auto b2Info = ToB2mdMonsterInfo(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_monster_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_general_lottery_infos
//			for (auto info : response->md_general_lotteries())
//			{
//				auto b2Info = ToB2mdGeneralLottery(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_general_lotteries.Emplace(b2Info);
//				}
//			}
//
//			 md_selective_lotteries
//			for (auto info : response->md_selective_lotteries())
//			{
//				auto b2Info = ToB2mdSelectiveLottery(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_selective_lotteries.Emplace(b2Info);
//				}
//			}
//
//			 md_fixed_grade_lotteries
//			for (auto info : response->md_fixed_grade_lotteries())
//			{
//				auto b2Info = ToB2mdFixedGradeLottery(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_fixed_grade_lotteries.Emplace(b2Info);
//				}
//			}
//
//			 md_prefix_selective_lotteries
//			for (auto info : response->md_prefix_selective_lotteries())
//			{
//				auto b2Info = ToB2mdPrefixSelectiveLottery(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_prefix_selective_lotteries.Emplace(b2Info);
//				}
//			}
//
//			 mdItemEnhanceCosts
//			int mdItemEnhanceCostCount = response->md_item_enhance_costs_size();
//			for (int i = 0; i < mdItemEnhanceCostCount; ++i)
//			{
//				const b2::protocol::masterdata::mdItemEnhanceCost& itemEnhanceCost = response->md_item_enhance_costs(i);
//
//				auto b2ItemEnhanceCost = mdItemEnhanceCostPb2B2mdItemEnhanceCost(itemEnhanceCost);
//				if (b2ItemEnhanceCost)
//				{
//					B2MessageInfoResponse->md_item_enhance_costs.Emplace(b2ItemEnhanceCost);
//				}
//			}
//
//			 mdItemEnhanceEffectInfos
//			int mdItemEnhanceEffectInfoCount = response->md_item_enhance_effect_infos_size();
//			for (int i = 0; i < mdItemEnhanceEffectInfoCount; ++i)
//			{
//				const b2::protocol::masterdata::mdItemEnhanceEffectInfo& itemEnhanceEffectInfo = response->md_item_enhance_effect_infos(i);
//
//				auto b2ItemEnhanceEffectInfo = mdItemEnhanceEffectInfoPb2B2mdItemEnhanceEffectInfo(itemEnhanceEffectInfo);
//				if (b2ItemEnhanceEffectInfo)
//				{
//					B2MessageInfoResponse->md_item_enhance_effect_infos.Emplace(b2ItemEnhanceEffectInfo);
//				}
//			}
//
//			 md_general_lottery_infos
//			for (auto info : response->md_additional_match_cost_infos())
//			{
//				auto b2Info = ToB2mdAdditionalMatchCost(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_additional_match_cost_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_shop_lottery_daily_limits
//			for (auto info : response->md_shop_lottery_daily_limits())
//			{
//				auto b2Info = ToB2mdShopLotteryDailyLimit(info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_shop_lottery_daily_limits.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_skills
//			for (auto guildSkill : response->md_guild_skills())
//			{
//				auto b2Info = ToB2mdGuildSkill(guildSkill);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_skills.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_exps
//			for (auto guildExp : response->md_guild_exps())
//			{
//				auto b2Info = ToB2mdGuildExp(guildExp);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_exps.Emplace(b2Info);
//				}
//			}
//
//			 md_stage_fixed_drops
//			for (auto stageFixedDrop : response->md_stage_fixed_drops())
//			{
//				auto b2Info = ToB2mdStageFixedDrop(stageFixedDrop);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_stage_fixed_drops.Emplace(b2Info);
//				}
//			}
//
//			 md_assault_victory_reward
//			for (auto victory_reward : response->md_assault_victory_reward_infos())
//			{
//				auto b2Info = ToB2mdAssaultVictoryReward(victory_reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_assault_victory_reward_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_assault_mvp_reward
//			for (auto mvp_reward : response->md_assault_mvp_reward_infos())
//			{
//				auto b2Info = ToB2mdAssaultMvpReward(mvp_reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_assault_mvp_reward_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_event_attendances
//			/*for (auto event_attendance : response->md_event_attendances())
//			{
//				auto b2Info = ToB2mdEventAttendance(event_attendance);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_event_attendances.Emplace(b2Info);
//				}
//			}*/
//
//			 md_event_attendance_rewards
//			for (auto reward : response->md_event_attendance_rewards())
//			{
//				auto b2Info = ToB2mdEventAttendanceReward(reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_event_attendance_rewards.Emplace(b2Info);
//				}
//			}
//
//			 md_daily_play_time_rewards
//			for (auto play_time_reward : response->md_daily_play_time_rewards())
//			{
//				auto b2Info = ToB2mdDailyPlayTimeReward(play_time_reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_daily_play_time_rewards.Emplace(b2Info);
//				}
//			}
//
//			 md_tutorial_infos
//			for (auto tutorial_info : response->md_tutorial_infos())
//			{
//				auto b2Info = ToB2mdTutorialInfo(tutorial_info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_tutorial_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_duel_mode_exp
//			for (auto duel_mode_reward : response->md_duel_mode_reward())
//			{
//				auto b2Info = ToB2mdDuelModeReward(duel_mode_reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_duel_mode_reward.Emplace(b2Info);
//				}
//			}
//
//			 md_raid_exp
//			for (auto raid_exp : response->md_raid_exp())
//			{
//				auto b2Info = ToB2mdRaidExp(raid_exp);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_raid_exp.Emplace(b2Info);
//				}
//			}
//
//			 md_aether_option_infos
//			for (auto aether_option_info : response->md_aether_option_infos())
//			{
//				auto b2Info = ToB2mdAetherOptionInfo(aether_option_info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_aether_option_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_aether_slot_infos
//			for (auto aether_slot_info : response->md_aether_slot_infos())
//			{
//				auto b2Info = ToB2mdAetherSlotInfo(aether_slot_info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_aether_slot_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_aether_enhance_infos
//			for (auto aether_enhance_info : response->md_aether_enhance_infos())
//			{
//				auto b2Info = ToB2mdAetherEnhanceInfo(aether_enhance_info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_aether_enhance_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_aether_enhance_option_infos
//			for (auto aether_enhance_option_info : response->md_aether_enhance_option_infos())
//			{
//				auto b2Info = ToB2mdAetherEnhanceOptionInfo(aether_enhance_option_info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_aether_enhance_option_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_aether_set_option_infos
//			for (auto aether_set_option_info : response->md_aether_set_option_infos())
//			{
//				auto b2Info = ToB2mdAetherSetOptionInfo(aether_set_option_info);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_aether_set_option_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_support_items
//			for (auto guild_support_item : response->md_guild_support_items())
//			{
//				auto b2Info = ToB2mdGuildSupportItem(guild_support_item);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_support_items.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_battle_schedule_infos
//			for (auto schedule : response->md_guild_battle_schedule_infos())
//			{
//				auto b2Info = ToB2mdGuildBattleScheduleInfo(schedule);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_battle_schedule_infos.Emplace(b2Info);
//				}
//			}
//
//			 md_unity_skill_missions
//			for (auto mission : response->md_unity_skill_missions())
//			{
//				auto b2Info = ToB2mdUnitySkillMission(mission);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_unity_skill_missions.Emplace(b2Info);
//				}
//			}
//
//			 md_rest_reward
//			for (auto reward : response->md_rest_reward())
//			{
//				auto b2Info = ToB2mdRestReward(reward);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_rest_rewards.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_mercenaries
//			for (auto mercenary : response->md_guild_mercenaries())
//			{
//				auto b2Info = ToB2mdGuildMercenary(mercenary);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_mercenaries.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_mercenary_skills
//			for (auto skill : response->md_guild_mercenary_skills())
//			{
//				auto b2Info = ToB2mdGuildMercenarySkill(skill);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_mercenary_skills.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_mercenary_masteries
//			for (auto mastery : response->md_guild_mercenary_masteries())
//			{
//				auto b2Info = ToB2mdGuildMercenaryMastery(mastery);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_mercenary_masteries.Emplace(b2Info);
//				}
//			}
//
//			 md_guild_mercenary_donations
//			for (auto donation : response->md_guild_mercenary_donations())
//			{
//				auto b2Info = ToB2mdGuildMercenaryDonation(donation);
//				if (b2Info)
//				{
//					B2MessageInfoResponse->md_guild_mercenary_donations.Emplace(b2Info);
//				}
//			}
//
//			 md_item_infos
//			for (auto itemInfo : response->md_item_infos())
//			{
//				B2MessageInfoResponse->md_item_infos.Emplace(ToB2mdItemInfo(itemInfo));
//			}
//
//			 md_item_set_infos
//			for (auto itemInfo : response->md_item_set_infos())
//			{
//				B2MessageInfoResponse->md_item_set_infos.Emplace(ToB2mdItemSet(itemInfo));
//			}
//
//			 md_event_info
//			/*for (auto eventInfo : response->md_event_infos())
//			{
//				B2MessageInfoResponse->md_event_infos.Emplace(ToB2mdEventInfo(eventInfo));
//			}*/
//
//			 md_point_events
//			for (auto eventInfo : response->md_point_events())
//			{
//				B2MessageInfoResponse->md_point_events.Emplace(ToB2mdPointEvent(eventInfo));
//			}
//
//			 md_point_event_rewards
//			for (auto rewardInfo : response->md_point_event_rewards())
//			{
//				B2MessageInfoResponse->md_point_event_rewards.Emplace(ToB2mdPointEventReward(rewardInfo));
//			}
//
//			 md_point_event_rewards
//			for (auto eventInfo : response->md_level_events())
//			{
//				B2MessageInfoResponse->md_level_events.Emplace(ToB2mdLevelUpEvent(eventInfo));
//			}
//
//			 md_hot_time_events
//			/*for (auto eventInfo : response->md_hot_time_events())
//			{
//				B2MessageInfoResponse->md_hot_time_events.Emplace(ToB2mdHotTimeEvent(eventInfo));
//			}*/
//
//			 md_kakao_friend_invite_rewards
//			for (auto rewardInfo : response->md_kakao_friend_invite_rewards())
//			{
//				B2MessageInfoResponse->md_kakao_friend_invite_rewards.Emplace(ToB2mdKakaoFriendInviteReward(rewardInfo));
//			}
//
//			 md_hot_time_product
//			for (auto product : response->md_hot_time_products())
//			{
//				B2MessageInfoResponse->md_hot_time_products.Emplace(ToB2mdHotTimeProduct(product));
//			}
//
//			 md_shop_cash_bonus_infos
//			for (auto cashBonusInfo : response->md_shop_cash_bonus_infos())
//			{
//				B2MessageInfoResponse->md_shop_cash_bonus_infos.Emplace(ToB2mdShopCashBonus(cashBonusInfo));
//			}
//
//			 md_shop_funds_sale_infos
//			for (auto fundsSaleInfo : response->md_shop_funds_sale_infos())
//			{
//				B2MessageInfoResponse->md_shop_funds_sale_infos.Emplace(ToB2mdShopFundsSale(fundsSaleInfo));
//			}
//
//			 md_shop_lottery_sale_infos
//			for (auto lotterySaleInfo : response->md_shop_lottery_sale_infos())
//			{
//				B2MessageInfoResponse->md_shop_lottery_sale_infos.Emplace(ToB2mdShopLotterySale(lotterySaleInfo));
//			}
//
//			 md_shop_package_product
//			for (auto product : response->md_shop_package_products())
//			{
//				B2MessageInfoResponse->md_shop_package_products.Emplace(ToB2mdShopPackageProduct(product));
//			}
//
//			 md_season_mission_events
//			for (auto product : response->md_season_mission_events())
//			{
//				B2MessageInfoResponse->md_season_mission_events.Emplace(ToB2mdSeasonMissionEvent(product));
//			}
//
//			 md_skill_levelups
//			for (auto levelup : response->md_skill_levelups())
//			{
//				B2MessageInfoResponse->md_skill_levelups.Emplace(ToB2mdSkillLevelUp(levelup));
//			}
//
//			 md_duel_mode_settle_schedules
//			for (auto schedule : response->md_duel_mode_settle_schedules())
//			{
//				B2MessageInfoResponse->md_duel_mode_settle_schedules.Emplace(ToB2mdDuelModeSettleSchedule(schedule));
//			}
//
//			 md_shop_level_up_packages
//			for (auto package : response->md_shop_level_up_packages())
//			{
//				B2MessageInfoResponse->md_shop_level_up_packages.Emplace(ToB2mdShopLevelUpPackage(package));
//			}
//
//			 md_shop_reach_level_packages
//			for (auto package : response->md_shop_reach_level_packages())
//			{
//				B2MessageInfoResponse->md_shop_reach_level_packages.Emplace(ToB2mdShopReachLevelPackage(package));
//			}
//
//			 md_shop_flat_rate_package_bonus
//			for (auto package : response->md_shop_flat_rate_package_bonus())
//			{
//				B2MessageInfoResponse->md_shop_flat_rate_package_bonus.Emplace(ToB2mdShopFlatRatePackageBonus(package));
//			}
//
//			 md_shop_package_tab_infos
//			for (auto tab : response->md_shop_package_tab_infos())
//			{
//				B2MessageInfoResponse->md_shop_package_tab_infos.Emplace(ToB2mdShopPackageTab(tab));
//			}
//
//			 md_unity_skill_options
//			for (auto option : response->md_unity_skill_options())
//			{
//				B2MessageInfoResponse->md_unity_skill_options.Emplace(ToB2mdUnitySkillOption(option));
//			}
//
//			 md_stage_clear_events
//			for (auto option : response->md_stage_clear_events())
//			{
//				B2MessageInfoResponse->md_stage_clear_events.Emplace(ToB2mdStageClearEvent(option));
//			}
//
//			 md_web_urls
//			for (auto url : response->md_web_urls())
//			{
//				B2MessageInfoResponse->md_web_urls.Emplace(ToB2B2mdWebUrl(url));
//			}
//
//			 md_ten_lottery_events
//			for (auto eventInfo : response->md_ten_lottery_events())
//			{
//				B2MessageInfoResponse->md_ten_lottery_events.Emplace(ToB2mdTenLotteryEvent(eventInfo));
//			}
//
//			 md_magic_shop_product
//			for (auto eventInfo : response->md_magic_shop_product())
//			{
//				B2MessageInfoResponse->md_magic_shop_product.Emplace(ToB2mdMagicShopProduct(eventInfo));
//			}
//
//			 md_magic_shop_renewal_cost
//			for (auto eventInfo : response->md_magic_shop_renewal_cost())
//			{
//				B2MessageInfoResponse->md_magic_shop_renewal_cost.Emplace(ToB2mdMagicShopRenewalCost(eventInfo));
//			}
//
//			 md_spend_gem_events
//			for (auto eventInfo : response->md_spend_gem_events())
//			{
//				B2MessageInfoResponse->md_spend_gem_event_rewards.Emplace(ToB2mdSpendGemEvent(eventInfo));
//			}
//
//			 md_gem_purchase_event_rewards
//			for (auto eventInfo : response->md_gem_purchase_event_rewards())
//			{
//				B2MessageInfoResponse->md_gem_purchase_event_rewards.Emplace(ToB2mdGemPurchaseEventReward(eventInfo));
//			}
//
//			 md_ten_lottery_savings_events
//			for (auto eventInfo : response->md_ten_lottery_savings_events())
//			{
//				B2MessageInfoResponse->md_ten_lottery_savings_events.Emplace(ToB2mdTenLotterySavingsEvent(eventInfo));
//			}
//
//			 md_item_levelup_factors
//			for (auto factor : response->md_item_levelup_factors())
//			{
//				B2MessageInfoResponse->md_item_levelup_factors.Emplace(ToB2mdItemLevelupFactor(factor));
//			}
//
//			 md_select_item_option
//			for (auto selectItemOption : response->md_select_item_option())
//			{
//				B2MessageInfoResponse->md_select_item_option.Emplace(ToB2mdSelectItemOption(selectItemOption));
//			}
//
//			 md_dice_event_missions
//			for (auto mission : response->md_dice_event_missions())
//			{
//				B2MessageInfoResponse->md_dice_event_missions.Emplace(ToB2mdDiceEventMission(mission));
//			}
//
//			 md_dice_event_cells
//			for (auto cell : response->md_dice_event_cells())
//			{
//				B2MessageInfoResponse->md_dice_event_cells.Emplace(ToB2mdDiceEventCell(cell));
//			}
//
//			 md_dice_event_finish_reward
//			for (auto reward : response->md_dice_event_finish_rewards())
//			{
//				B2MessageInfoResponse->md_dice_event_finish_rewards.Emplace(ToB2mdDiceEventFinishReward(reward));
//			}
//
//			 md_item_surpass_levels
//			for (auto surpass : response->md_item_surpass_levels())
//			{
//				B2MessageInfoResponse->md_item_surpass_levels.Emplace(ToB2mdItemSurpassLevel(surpass));
//			}
//
//			 md_dice_event_buy_points
//			for (auto point : response->md_dice_event_buy_points())
//			{
//				B2MessageInfoResponse->md_dice_event_buy_points.Emplace(ToB2mdDiceEventBuyPoint(point));
//			}
//
//			 md_dice_event_mileages
//			for (auto point : response->md_dice_event_mileages())
//			{
//				B2MessageInfoResponse->md_dice_event_mileages.Emplace(ToB2mdDiceEventMileage(point));
//			}
//
//			 md_item_dismantle_infos
//			for (auto point : response->md_item_dismantle_infos())
//			{
//				B2MessageInfoResponse->md_item_dismantle_infos.Emplace(ToB2mdItemDismantleInfo(point));
//			}
//
//			 md_donation_point_ranking_reward_infos
//			for (auto info : response->md_donation_point_ranking_reward_infos())
//			{
//				B2MessageInfoResponse->md_donation_point_ranking_reward_infos.Emplace(ToB2mdDonationPointRankingRewardInfo(info));
//			}
//
//			 md_donation_bundle_fixed_reward_infos
//			for (auto info : response->md_donation_bundle_fixed_reward_infos())
//			{
//				B2MessageInfoResponse->md_donation_bundle_fixed_reward_infos.Emplace(ToB2mdDonationBundleFixedRewardInfo(info));
//			}
//
//			 md_donation_point_step_reward_infos
//			for (auto info : response->md_donation_point_step_reward_infos())
//			{
//				B2MessageInfoResponse->md_donation_point_step_reward_infos.Emplace(ToB2mdDonationPointStepRewardInfo(info));
//			}
//
//			 md_fairy_infos
//			for (auto eventInfo : response->md_fairy_infos())
//			{
//				B2MessageInfoResponse->md_fairy_infos.Emplace(ToB2mdFairyInfo(eventInfo));
//			}
//
//			 md_fairy_levelup_costs
//			for (auto eventInfo : response->md_fairy_levelup_costs())
//			{
//				B2MessageInfoResponse->md_fairy_levelup_costs.Emplace(ToB2mdFairyLevelupCost(eventInfo));
//			}
//
//			 md_fairy_open_infos
//			for (auto eventInfo : response->md_fairy_open_infos())
//			{
//				B2MessageInfoResponse->md_fairy_open_infos.Emplace(ToB2mdFairyOpenInfo(eventInfo));
//			}
//
//			 md_costume_infos
//			for (auto eventInfo : response->md_costume_infos())
//			{
//				B2MessageInfoResponse->md_costume_infos.Emplace(ToB2mdCostumeInfo(eventInfo));
//			}
//
//			 md_unity_skill_awaken_missions
//			for (auto awakenMission : response->md_unity_skill_awaken_missions())
//			{
//				B2MessageInfoResponse->md_unity_skill_awaken_missions.Emplace(ToB2mdUnitySkillAwakenMission(awakenMission));
//			}
//
//			 md_totem_infos
//			auto B2ExtendMessageInfo = std::make_shared<B2mdExtendMessage>();
//			for (auto info : response->md_totem_infos())
//			{	
//				B2ExtendMessageInfo->md_totem_infos.Emplace(ToB2mdTotemInfo(info));
//			}
//
//			 md_select_totem_options
//			for (auto info : response->md_select_totem_options())
//			{
//				B2ExtendMessageInfo->md_select_totem_options.Emplace(ToB2mdSelectTotemOption(info));
//			}
//
//			 md_totem_refine_infos
//			for (auto info : response->md_totem_refine_infos())
//			{
//				B2ExtendMessageInfo->md_totem_refine_infos.Emplace(ToB2mdTotemRefineInfo(info));
//			}
//
//			 md_totem_refine_rate_infos
//			for (auto info : response->md_totem_refine_rate_infos())
//			{
//				B2ExtendMessageInfo->md_totem_refine_rate_infos.Emplace(ToB2mdTotemRefineRateInfo(info));
//			}
//
//			 md_totem_dismantle_infos
//			for (auto info : response->md_totem_dismantle_infos())
//			{
//				B2ExtendMessageInfo->md_totem_dismantle_infos.Emplace(ToB2mdTotemDismantleInfo(info));
//			}
//
//			 md_totem_bonus_option_infos
//			for (auto info : response->md_totem_bonus_option_infos())
//			{
//				B2ExtendMessageInfo->md_totem_bonus_option_infos.Emplace(ToB2mdTotemBonusOptionInfo(info));
//			}
//
//			 md_dimension_infos
//			for (auto info : response->md_dimension_infos())
//			{
//				B2ExtendMessageInfo->md_dimension_infos.Emplace(ToB2mdDimensionInfo(info));
//			}
//
//			 md_dimension_infos
//			for (auto info : response->md_dimension_week_rewards())
//			{
//				B2ExtendMessageInfo->md_dimension_week_rewards.Emplace(ToB2mdDimensionWeekReward(info));
//			}
//
//			 md_totem_prefix_lottery_infos
//			for (auto info : response->md_totem_prefix_lottery_infos())
//			{
//				B2ExtendMessageInfo->md_totem_prefix_lottery_infos.Emplace(ToB2mdTotemPrefixLotteryInfo(info));
//			}
//
//			 md_item_seal_cost
//			for (auto info : response->md_item_seal_cost())
//			{
//				B2ExtendMessageInfo->md_item_seal_cost.Emplace(ToB2mdItemSealCost(info));
//			}
//
//			 md_select_item_seal_option
//			for (auto info : response->md_select_item_seal_option())
//			{
//				B2ExtendMessageInfo->md_select_item_seal_option.Emplace(ToB2mdSelectItemSealOption(info));
//			}
//
//			 md_costume_enhance_cost
//			for (auto info : response->md_costume_enhance_cost())
//			{
//				B2ExtendMessageInfo->md_costume_enhance_cost.Emplace(ToB2mdCostumeEnhanceCost(info));
//			}
//			
//			 md_exchange_system_info
//			for (auto exchangeInfo : response->md_exchange_system_info())
//			{
//				auto b2Info = ToB2mdExchangeSystemInfo(exchangeInfo);
//				if (b2Info)
//				{
//					B2ExtendMessageInfo->md_exchange_system_info.Emplace(b2Info);
//				}
//			}
//			
//			for (auto exchangesystemcost : response->md_exchange_system_cost())
//			{
//				auto b2Info = ToB2mdExchangeSystemCost(exchangesystemcost);
//				if (b2Info)
//				{
//					B2ExtendMessageInfo->md_exchange_system_cost.Emplace(b2Info);
//				}
//			}
//
//			 md_quality_trans_rate_info
//			for (auto info : response->md_quality_trans_rate_info())
//			{
//				auto b2Info = ToB2mdQualityTransRateInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_quality_trans_rate_info.Emplace(b2Info);
//				}
//			}
//
//			 md_lottery_shop_rate_quality_info
//			for (auto info : response->md_lottery_shop_rate_quality_info())
//			{
//				auto b2Info = ToB2mdLotteryShopRateQualityInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_lottery_shop_rate_quality_info.Emplace(b2Info);
//				}
//			}
//
//			 md_factory_info
//			for (auto info : response->md_factory_info())
//			{
//				auto b2Info = ToB2mdFactoryInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_factory_info.Emplace(b2Info);
//				}
//			}
//
//			 md_factory_limit_info
//			for (auto info : response->md_factory_limit_info())
//			{
//				auto b2Info = ToB2mdFactoryLimitInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_factory_limit_info.Emplace(b2Info);
//				}
//			}
//
//			 md_factory_material_trans_info
//			for (auto info : response->md_factory_material_trans_info())
//			{
//				auto b2Info = ToB2mdFactoryMaterialTransInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_factory_material_trans_info.Emplace(b2Info);
//				}
//			}
//
//			 md_seal_box_info
//			for (auto info : response->md_seal_box_info())
//			{
//				auto b2Info = ToB2mdSealBoxInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_seal_box_info.Emplace(b2Info);
//				}
//			}
//
//			 md_seal_box_reward_info
//			for (auto info : response->md_seal_box_reward_info())
//			{
//				auto b2Info = ToB2mdSealBoxRewardInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_seal_box_reward_info.Emplace(b2Info);
//				}
//			}
//
//			 md_seal_box_reward_rate_info
//			for (auto info : response->md_seal_box_reward_rate_info())
//			{
//				auto b2Info = ToB2mdSealBoxRewardRateInfo(info);
//				if (b2Info) {
//					B2ExtendMessageInfo->md_seal_box_reward_rate_info.Emplace(b2Info);
//				}
//			}
//		
//			B2MessageInfoResponse->md_extend_massages = B2ExtendMessageInfo;
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetLatestEventDataResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetLatestEventDataResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetLatestEventData>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->last_update_time = response->last_update_time();
//
//			if (B2MessageInfoResponse->last_update_time > 0)
//			{
//				 md_event_info
//				for (auto eventInfo : response->md_event_infos())
//				{
//					B2MessageInfoResponse->md_event_infos.Emplace(ToB2mdEventInfo(eventInfo));
//				}
//
//				 md_event_attendances
//				for (auto event_attendance : response->md_event_attendances())
//				{
//					auto b2Info = ToB2mdEventAttendance(event_attendance);
//					if (b2Info)
//					{
//						B2MessageInfoResponse->md_event_attendances.Emplace(b2Info);
//					}
//				}
//
//				 md_hot_time_events
//				for (auto eventInfo : response->md_hot_time_events())
//				{
//					B2MessageInfoResponse->md_hot_time_events.Emplace(ToB2mdHotTimeEvent(eventInfo));
//				}
//
//				 md_donation_event_infos
//				for (auto eventInfo : response->md_donation_event_infos())
//				{
//					B2MessageInfoResponse->md_donation_event_infos.Emplace(ToB2mdDonationEventInfo(eventInfo));
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSellItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SellItemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSellItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->money_total = response->money_total();
//			int ItemCount = response->sell_item_ids_size();
//			for (int i = 0; i < ItemCount; ++i)
//			{
//				B2MessageInfoResponse->sell_item_ids.Emplace(response->sell_item_ids(i));
//			}
//			B2MessageInfoResponse->sell_price = response->sell_price();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSellConsumableItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SellConsumableItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSellConsumableItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->updated_item = ItemPb2B2ItemServerInfo(response->updated_item());
//			B2MessageInfoResponse->deleted_item_id = response->deleted_item_id();
//			B2MessageInfoResponse->sell_price = response->sell_price();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromConvertDataResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckExChangeCountResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseConvertData>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->templateId = response->template_id();
//			B2MessageInfoResponse->exchange_remain_count_gold = response->exchange_remain_count_gold();
//			B2MessageInfoResponse->exchange_remain_count_gem = response->exchange_remain_count_gem();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromConvertItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ExChangeSystemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseConvertItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->exchangetype = response->exchange_type();
//			B2MessageInfoResponse->currentGold = response->current_gold();
//			B2MessageInfoResponse->currentCash = response->current_cash();
//			B2MessageInfoResponse->exchangeItem = ItemPb2B2ItemServerInfo(response->exchange_item());
//			B2MessageInfoResponse->exchangeItemAmount = response->exchange_item_amount();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEquipItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EquipItemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEquipItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->preset_id = response->preset_id();
//
//			for (auto itemId : response->equiped_item_ids()) {
//				B2MessageInfoResponse->equiped_item_ids.Emplace(itemId);
//			}
//			
//			for (auto itemId : response->unequiped_item_ids()) {
//				B2MessageInfoResponse->unequiped_item_ids.Emplace(itemId);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnequipItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnequipItemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnequipItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->preset_id = response->preset_id();
//
//			for (auto itemId : response->unequiped_item_ids()) {
//				B2MessageInfoResponse->unequiped_item_ids.Emplace(itemId);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEquipCostumeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EquipCostumesResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEquipCostume>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto costumeId : response->equiped_costume_ids()) {
//				B2MessageInfoResponse->equiped_costume_ids.Emplace(costumeId);
//			}
//
//			for (auto costumeId : response->unequiped_costume_ids()) {
//				B2MessageInfoResponse->unequiped_costume_ids.Emplace(costumeId);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnequipCostumeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnequipCostumesResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnequipCostume>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto costumeId : response->unequiped_costume_ids()) {
//				B2MessageInfoResponse->unequiped_costume_ids.Emplace(costumeId);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceCostumeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceCostumeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceCostume>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->enhanced_costume = CostumePb2B2CostumeServerInfo(response->enhanced_costume());
//			
//			for (auto materialItem : response->changed_material_items()) {
//				B2MessageInfoResponse->changed_material_items.Emplace(ItemPb2B2ItemServerInfo(materialItem));
//			}
//
//			for (auto itemId : response->deleted_material_item_ids()) {
//				B2MessageInfoResponse->deleted_material_item_ids.Emplace(itemId);
//			}
//
//			B2MessageInfoResponse->current_enhance_level = response->current_enhance_level();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDismantleCostumesResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DismantleCostumesResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDismantleCostumes>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto costumeId : response->dismantled_costume_ids()) {
//				B2MessageInfoResponse->dismantled_costume_ids.Emplace(costumeId);
//			}
//
//			for (auto extractItem : response->extracted_items()) {
//				B2MessageInfoResponse->extracted_items.Emplace(ItemPb2B2ItemServerInfo(extractItem));
//			}
//
//			for (auto amount : response->extracted_item_change_amount()) {
//				B2MessageInfoResponse->extracted_item_change_amount.Emplace(amount);
//			}
//
//			B2MessageInfoResponse->restore_item = ItemPb2B2ItemServerInfo(response->restore_item());
//			B2MessageInfoResponse->restore_items_change_amount = response->restore_items_change_amount();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLockCostumesResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LockCostumesResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLockCostumes>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto costume : response->locked_costumes()) {
//				B2MessageInfoResponse->locked_costumes.Emplace(CostumePb2B2CostumeServerInfo(costume));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnlockCostumesResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnlockCostumesResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnlockCostumes>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto costume : response->unlocked_costumes()) {
//				B2MessageInfoResponse->unlocked_costumes.Emplace(CostumePb2B2CostumeServerInfo(costume));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLockItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LockItemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLockItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int LockedItemCount = response->locked_items_size();
//			for (int i = 0; i < LockedItemCount; ++i)
//			{
//				auto LockedItemInfo = ItemPb2B2ItemServerInfo(response->locked_items(i));
//				if (LockedItemInfo)
//				{
//					B2MessageInfoResponse->locked_items.Emplace(LockedItemInfo);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnlockItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnlockItemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnlockItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int UnlockedItemCount = response->unlocked_items_size();
//			for (int i = 0; i < UnlockedItemCount; ++i)
//			{
//				auto UnlockedItemInfo = ItemPb2B2ItemServerInfo(response->unlocked_items(i));
//				if (UnlockedItemInfo)
//				{
//					B2MessageInfoResponse->unlocked_items.Emplace(UnlockedItemInfo);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromExtendInventoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ExtendInventoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseExtendInventory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			auto CharacterInfo = CharacterPb2B2CharacterServerInfo(response->character());
//			if (CharacterInfo) 
//			{
//				B2MessageInfoResponse->character = CharacterInfo;
//			}
//			B2MessageInfoResponse->cash = response->cash();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->enhanced_item = ItemPb2B2ItemServerInfo(response->enhanced_item());
//
//			int ChangedEnhanceScrollItemCount = response->changed_enhance_scroll_items_size();
//			for (int i = 0; i < ChangedEnhanceScrollItemCount; ++i)
//			{
//				auto EnhanceScrollInfo = ItemPb2B2ItemServerInfo(response->changed_enhance_scroll_items(i));
//				if (EnhanceScrollInfo)
//				{
//					B2MessageInfoResponse->changed_enhance_scroll_items.Emplace(EnhanceScrollInfo);
//				}
//			}
//
//			int DeletedEnhanceScrollItemIdCount = response->deleted_enhance_scroll_item_ids_size();
//			for (int i = 0; i < DeletedEnhanceScrollItemIdCount; ++i)
//			{
//				int64 DeletedEnhanceScrollItemId = response->deleted_enhance_scroll_item_ids(i);
//				B2MessageInfoResponse->deleted_enhance_scroll_item_ids.Emplace(DeletedEnhanceScrollItemId);
//			}
//
//			int ChangedProtectionScrollItemCount = response->changed_protection_scroll_items_size();
//			for (int i = 0; i < ChangedProtectionScrollItemCount; ++i)
//			{
//				auto ProtectionScrollInfo = ItemPb2B2ItemServerInfo(response->changed_protection_scroll_items(i));
//				if (ProtectionScrollInfo)
//				{
//					B2MessageInfoResponse->changed_protection_scroll_items.Emplace(ProtectionScrollInfo);
//				}
//			}
//
//			int DeletedProtectionScrollItemIdCount = response->deleted_protection_scroll_item_ids_size();
//			for (int i = 0; i < DeletedProtectionScrollItemIdCount; ++i)
//			{
//				int64 DeletedProtectionScrollItemId = response->deleted_protection_scroll_item_ids(i);
//				B2MessageInfoResponse->deleted_protection_scroll_item_ids.Emplace(DeletedProtectionScrollItemId);
//			}
//
//			B2MessageInfoResponse->changed_enhance_level = response->changed_enhance_level();
//			B2MessageInfoResponse->is_success = response->is_success();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDismantleItemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DismantleItemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDismantleItems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			
//			int DismantleItemIdsCount = response->dismantled_item_ids_size();
//			for (int i = 0; i < DismantleItemIdsCount; ++i)
//			{
//				int64 DismantleItemId = response->dismantled_item_ids(i);
//				B2MessageInfoResponse->dismantled_item_ids.Emplace(DismantleItemId);
//			}
//
//			int ExtractedItemCount = response->extracted_items_size();
//			for (int i = 0; i < ExtractedItemCount; ++i)
//			{
//				auto ExtractedItemInfo = ItemPb2B2ItemServerInfo(response->extracted_items(i));
//				if (ExtractedItemInfo)
//				{
//					B2MessageInfoResponse->extracted_items.Emplace(ExtractedItemInfo);
//				}
//			}
//
//			int ExtractedItemChangeAmountSize = response->extracted_item_change_amount_size();
//			for (int i = 0; i < ExtractedItemChangeAmountSize; ++i)
//			{
//				int64 ExtractedItemChangeAmount = response->extracted_item_change_amount(i);
//				B2MessageInfoResponse->extracted_item_change_amount.Emplace(ExtractedItemChangeAmount);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//	
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromExchangeItemEnhanceLevelResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ExchangeItemEnhanceLevelResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseExchangeItemEnhanceLevel>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_dark_crystal = response->current_dark_crystal();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->item_data_1 = ItemPb2B2ItemServerInfo(response->item_data_1());
//			B2MessageInfoResponse->item_data_2 = ItemPb2B2ItemServerInfo(response->item_data_2());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromOpenSealSlotItemResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::OpenSealSlotItemResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseOpenSealSlotItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->slot_opened_item = ItemPb2B2ItemServerInfo(response->slot_opened_item());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCarveSealItemResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CarveSealItemResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCarveSealItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->sealed_item = ItemPb2B2ItemServerInfo(response->sealed_item());
//
//			for (auto updatedMaterialItem : response->updated_material_items())
//			{
//				B2MessageInfoResponse->updated_material_items.Emplace(ItemPb2B2ItemServerInfo(updatedMaterialItem));
//			}
//
//			for (auto deletedItemId : response->deleted_material_item_ids())
//			{
//				B2MessageInfoResponse->deleted_material_item_ids.Emplace(deletedItemId);
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLevelupItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LevelupItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLevelupItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->money_cost = response->money_cost();
//			B2MessageInfoResponse->main_item = ItemPb2B2ItemServerInfo(response->main_item());
//
//			int MaterialItemUIDCount = response->material_item_ids_size();
//			for (int i = 0; i < MaterialItemUIDCount; ++i)
//			{
//				int64 MaterialItemUID = response->material_item_ids(i);
//				B2MessageInfoResponse->item_ids.Emplace(MaterialItemUID);
//			}
//			B2MessageInfoResponse->jackpoted = response->jackpoted();
//			B2MessageInfoResponse->great_jackpoted = response->great_jackpoted();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSurpassItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SurpassItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSurpassItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->money_cost = response->money_cost();
//			B2MessageInfoResponse->main_item = ItemPb2B2ItemServerInfo(response->main_item());
//			B2MessageInfoResponse->material_item_id = response->material_item_id();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpgradeItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpgradeItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUpgradeItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->money_cost = response->money_cost();
//			B2MessageInfoResponse->main_item = ItemPb2B2ItemServerInfo(response->main_item());
//			int UpgradeStoneCount = response->upgrade_stone_items_size();
//			for (int i = 0; i < UpgradeStoneCount; ++i)
//			{
//				auto UpgradeStoneInfo = ItemPb2B2ItemServerInfo(response->upgrade_stone_items(i));
//				if (UpgradeStoneInfo)
//				{
//					B2MessageInfoResponse->upgrade_stone_items.Emplace(UpgradeStoneInfo);
//				}
//			}
//			B2MessageInfoResponse->use_upgrade_stone_count = response->use_upgrade_stone_count();
//
//			int DeletedItemCount = response->deleted_item_ids_size();
//			for (int i = 0; i < DeletedItemCount; ++i)
//			{
//				B2MessageInfoResponse->deleted_item_ids.Emplace(response->deleted_item_ids(i));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromComposeItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ComposeItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseComposeItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->money_cost = response->money_cost();
//			B2MessageInfoResponse->new_composed_item = ItemPb2B2ItemServerInfo(response->new_composed_item());
//			B2MessageInfoResponse->deleted_main_item_id = response->deleted_main_item_id();
//			B2MessageInfoResponse->deleted_sub_item_id = response->deleted_sub_item_id();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromChangeItemCharacterTypeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ChangeItemCharacterTypeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseChangeItemCharacterType>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->updated_item = ItemPb2B2ItemServerInfo(response->updated_item());
//			B2MessageInfoResponse->current_dark_crystal = response->current_dark_crystal();
//			B2MessageInfoResponse->dark_crystal_cost = response->dark_crystal_cost();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetChangeItemCharacterTypeCostResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetChangeItemCharacterTypeCostResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetChangeItemCharacterTypeCost>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->dark_crystal_cost = response->dark_crystal_cost();
//			
//			for (auto item : response->change_items())
//			{
//				B2MessageInfoResponse->change_items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromStartStageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::StartStageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseStartStage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->token = ANSI_TO_TCHAR(response->token().c_str());
//			B2MessageInfoResponse->drop_gold = response->drop_gold();
//			for (auto dropItem : response->drop_items())
//			{
//				B2MessageInfoResponse->drop_items.Emplace(ItemPb2B2ItemServerInfo(dropItem));
//			}
//
//			B2MessageInfoResponse->drop_gold_by_gold_monster = response->drop_gold_by_gold_monster();
//			B2MessageInfoResponse->max_resurrection_count = response->max_resurrection_count();
//			B2MessageInfoResponse->need_cash_for_resurrection = response->need_cash_for_resurrection();
//
//			auto AccountInfo = AccountPb2B2AccountInfo(response->account());
//			if (AccountInfo)
//			{
//				B2MessageInfoResponse->account = AccountInfo;
//			}
//			B2MessageInfoResponse->selected_option_flags = response->selected_option_flags();
//
//			for (auto dropData : response->drop_datas())
//			{
//				B2MessageInfoResponse->drop_datas.Emplace(ToB2DropData(dropData));
//			}
//
//			B2MessageInfoResponse->quest_item_id = response->quest_item_id();
//			B2MessageInfoResponse->quest_item_count = response->quest_item_count();
//			B2MessageInfoResponse->qte_bonus_attack = response->qte_bonus_attack();
//			B2MessageInfoResponse->qte_bonus_finish = response->qte_bonus_finish();
//			B2MessageInfoResponse->stage_boost_ticket = response->has_stage_boost_ticket()? response->stage_boost_ticket() : 0;
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//
//			B2MessageInfoResponse->gold_buff_rate = response->gold_buff_rate();
//			B2MessageInfoResponse->exp_buff_rate = response->exp_buff_rate();
//			B2MessageInfoResponse->equip_item_gold_rate = response->equip_item_gold_rate();
//			B2MessageInfoResponse->equip_item_exp_rate = response->equip_item_exp_rate();
//			B2MessageInfoResponse->item_set_gold_rate = response->item_set_gold_rate();
//			B2MessageInfoResponse->item_set_exp_rate = response->item_set_exp_rate();
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_INVENTORY_COUNT)
//		{
//			int InventoryCountInfoSize = response->inventory_count_infos_size();
//			for (int i = 0; i < InventoryCountInfoSize; ++i)
//			{
//				auto InventoryCountInfo = InventoryCountInfoPb2B2InventoryCountServerInfo(response->inventory_count_infos(i));
//				if (InventoryCountInfo)
//				{
//					B2MessageInfoResponse->inventory_count_infos.Emplace(InventoryCountInfo);
//				}
//			}
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT)
//		{
//			B2MessageInfoResponse->current_aether_count = response->current_aether_count();
//			B2MessageInfoResponse->current_aether_max_count = response->current_aether_max_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromClearStageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ClearStageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseClearStage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			auto AccountInfo = AccountPb2B2AccountInfo(response->account());
//			if (AccountInfo)
//			{
//				B2MessageInfoResponse->account = AccountInfo;
//			}
//
//			int CharacterCount = response->characters_size();
//			for (int i = 0; i < CharacterCount; ++i)
//			{
//				auto CharacterInfo = CharacterPb2B2CharacterServerInfo(response->characters(i));
//				if (CharacterInfo)
//				{
//					B2MessageInfoResponse->characters.Emplace(CharacterInfo);
//				}
//			}
//
//			int ItemCount = response->items_size();
//			for (int i = 0; i < ItemCount; ++i)
//			{
//				auto ItemInfo = ItemPb2B2ItemServerInfo(response->items(i));
//				if (ItemInfo)
//				{
//					B2MessageInfoResponse->items.Emplace(ItemInfo);
//				}
//			}
//
//			for (auto item : response->update_items())
//			{
//				B2MessageInfoResponse->update_items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//
//			for (auto aether : response->aethers())
//			{
//				B2MessageInfoResponse->aethers.Emplace(AetherPb2B2AetherServerInfo(aether));
//			}
//
//			for (auto aether : response->update_aethers())
//			{
//				B2MessageInfoResponse->update_aethers.Emplace(AetherPb2B2AetherServerInfo(aether));
//			}
//
//			for (auto rewardGold : response->reward_golds())
//			{
//				B2MessageInfoResponse->reward_golds.Emplace(rewardGold);
//			}
//
//			for (auto fixedItem : response->fixed_items())
//			{
//				B2MessageInfoResponse->fixed_items.Emplace(ToB2DropData(fixedItem));
//			}
//
//			for (auto createdRelic : response->created_relics())
//			{
//				B2MessageInfoResponse->created_relics.Emplace(ToB2Relic(createdRelic));
//			}
//
//			for (auto materialItem : response->material_items())
//			{
//				B2MessageInfoResponse->material_items.Emplace(ToB2DropData(materialItem));
//			}
//
//			for (auto soldItem : response->sold_items())
//			{
//				B2MessageInfoResponse->sold_items.Emplace(ToB2SoldItemInfo(soldItem));
//			}
//
//			for (auto soldAether : response->sold_aethers())
//			{
//				B2MessageInfoResponse->sold_aethers.Emplace(ToB2SoldAetherInfo(soldAether));
//			}
//
//			B2MessageInfoResponse->clear_grade = response->clear_grade();
//			B2MessageInfoResponse->total_added_drop_gold = response->total_added_drop_gold();
//			B2MessageInfoResponse->total_added_reward_gold = response->total_added_reward_gold();
//			B2MessageInfoResponse->total_added_qte_gold = response->total_added_qte_gold();
//			B2MessageInfoResponse->reward_exp = response->reward_exp();
//			B2MessageInfoResponse->total_added_sales_gold = response->total_added_sales_gold();
//
//			B2MessageInfoResponse->quest_item_id = response->quest_item_id();
//			B2MessageInfoResponse->quest_item_count = response->quest_item_count();
//			B2MessageInfoResponse->is_level_up_main_character = response->is_level_up_main_character();
//			B2MessageInfoResponse->is_level_up_tag_character = response->is_level_up_tag_character();
//			B2MessageInfoResponse->clear_mask = response->clear_mask();
//			B2MessageInfoResponse->multiple_value = response->multiple_value();
//			 for event
//			B2MessageInfoResponse->total_added_gold = response->total_added_gold();
//			B2MessageInfoResponse->gold_buff_rate = response->gold_buff_rate();
//			B2MessageInfoResponse->exp_buff_rate = response->exp_buff_rate();
//			B2MessageInfoResponse->equip_item_gold_rate = response->equip_item_gold_rate();
//			B2MessageInfoResponse->equip_item_exp_rate = response->equip_item_exp_rate();
//			B2MessageInfoResponse->item_set_gold_rate = response->item_set_gold_rate();
//			B2MessageInfoResponse->item_set_exp_rate = response->item_set_exp_rate();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFailStageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FailStageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseFailStage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSweepStageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SweepStageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSweepStage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->stage_id = response->stage_id();
//			B2MessageInfoResponse->repeat_count = response->repeat_count();
//			B2MessageInfoResponse->stage_sweep_ticket = response->stage_sweep_ticket();
//
//			auto AccountInfo = AccountPb2B2AccountInfo(response->account());
//			if (AccountInfo)
//			{
//				B2MessageInfoResponse->account = AccountInfo;
//			}
//
//			int CharacterCount = response->characters_size();
//			for (int i = 0; i < CharacterCount; ++i)
//			{
//				auto CharacterInfo = CharacterPb2B2CharacterServerInfo(response->characters(i));
//				if (CharacterInfo)
//				{
//					B2MessageInfoResponse->characters.Emplace(CharacterInfo);
//				}
//			}
//
//			int ItemCount = response->items_size();
//			for (int i = 0; i < ItemCount; ++i)
//			{
//				auto ItemInfo = ItemPb2B2ItemServerInfo(response->items(i));
//				if (ItemInfo)
//				{
//					B2MessageInfoResponse->items.Emplace(ItemInfo);
//				}
//			}
//
//			for (auto aether : response->aethers())
//			{
//				B2MessageInfoResponse->aethers.Emplace(AetherPb2B2AetherServerInfo(aether));
//			}
//
//			for (auto rewardGold : response->reward_golds())
//			{
//				B2MessageInfoResponse->reward_golds.Emplace(rewardGold);
//			}
//
//			for (auto materialItem : response->material_items())
//			{
//				B2MessageInfoResponse->material_items.Emplace(ToB2DropData(materialItem));
//			}
//
//			for (auto fixedItem : response->fixed_items())
//			{
//				B2MessageInfoResponse->fixed_items.Emplace(ToB2DropData(fixedItem));
//			}
//
//			for (auto createdRelic : response->created_relics())
//			{
//				B2MessageInfoResponse->created_relics.Emplace(ToB2Relic(createdRelic));
//			}
//			
//			for (auto item : response->update_items())
//			{
//				B2MessageInfoResponse->update_items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//
//			B2MessageInfoResponse->is_level_up_main_character = response->is_level_up_main_character();
//			B2MessageInfoResponse->is_level_up_tag_character = response->is_level_up_tag_character();
//
//			B2MessageInfoResponse->reward_exp = response->reward_exp();
//			B2MessageInfoResponse->total_added_drop_gold = response->total_added_drop_gold();
//			B2MessageInfoResponse->total_added_reward_gold = response->total_added_reward_gold();
//
//			B2MessageInfoResponse->quest_item_id = response->quest_item_id();
//			B2MessageInfoResponse->quest_item_count = response->quest_item_count();
//
//			 for event
//			B2MessageInfoResponse->total_added_gold = response->total_added_gold();
//			B2MessageInfoResponse->gold_buff_rate = response->gold_buff_rate();
//			B2MessageInfoResponse->exp_buff_rate = response->exp_buff_rate();
//			B2MessageInfoResponse->equip_item_gold_rate = response->equip_item_gold_rate();
//			B2MessageInfoResponse->equip_item_exp_rate = response->equip_item_exp_rate();
//			B2MessageInfoResponse->item_set_gold_rate = response->item_set_gold_rate();
//			B2MessageInfoResponse->item_set_exp_rate = response->item_set_exp_rate();
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_INVENTORY_COUNT)
//		{
//			int InventoryCountInfoSize = response->not_enough_inventories_size();
//			for (int i = 0; i < InventoryCountInfoSize; ++i)
//			{
//				auto InventoryCountInfo = InventoryCountInfoPb2B2InventoryCountServerInfo(response->not_enough_inventories(i));
//				if (InventoryCountInfo)
//				{
//					B2MessageInfoResponse->not_enough_inventories.Emplace(InventoryCountInfo);
//				}
//			}
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT)
//		{
//			B2MessageInfoResponse->current_aether_count = response->current_aether_count();
//			B2MessageInfoResponse->current_aether_max_count = response->current_aether_max_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromResurrectionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ResurrectionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseResurrection>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->max_resurrection_count = response->max_resurrection_count();
//			B2MessageInfoResponse->resurrection_count = response->resurrection_count();
//			B2MessageInfoResponse->used_cash = response->used_cash();
//			B2MessageInfoResponse->total_cash = response->total_cash();
//			B2MessageInfoResponse->next_resurrection_gem_cost = response->next_resurrection_gem_cost();
//			B2MessageInfoResponse->selected_count_resurrection_buff1 = response->selected_count_resurrection_buff1();
//			B2MessageInfoResponse->selected_count_resurrection_buff2 = response->selected_count_resurrection_buff2();
//			B2MessageInfoResponse->selected_count_resurrection_buff3 = response->selected_count_resurrection_buff3();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckBladePointChargeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckBladePointChargeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckBladePointCharge>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->blade_point = response->blade_point();
//			B2MessageInfoResponse->blade_point_max = response->blade_point_max();
//			B2MessageInfoResponse->blade_point_charge_lefttime_second = response->blade_point_charge_lefttime_second();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetItemSurpassCostResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetItemSurpassCostResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetItemSurpassCost>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->need_gold = response->need_gold();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetItemUpgradeCostResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetItemUpgradeCostResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetItemUpgradeCost>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->need_gold = response->need_gold();
//			B2MessageInfoResponse->need_upgrade_stone_template_id = response->need_upgrade_stone_template_id();
//			B2MessageInfoResponse->need_upgrade_stone_count = response->need_upgrade_stone_count();
//
//			if (response->has_next_item()) 
//			{
//				B2MessageInfoResponse->next_item = ItemPb2B2ItemServerInfo(response->next_item());
//			}
//			else 
//			{
//				B2MessageInfoResponse->next_item = nullptr;
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetItemComposeCostResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetItemComposeCostResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetItemComposeCost>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->need_gold = response->need_gold();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetActInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetActInfoResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetActInfo>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int ClearInfoCount = response->stage_clear_infos_size();
//			for (int i = 0; i < ClearInfoCount; ++i)
//			{
//				auto ClearInfo = StageClearInfoPb2B2StageClearInfo(response->stage_clear_infos(i));
//				if (ClearInfo)
//				{
//					B2MessageInfoResponse->stage_clear_infos.Emplace(ClearInfo);
//				}
//			}
//
//			int RewardInfoCount = response->reward_infos_size();
//			for (int i = 0; i < RewardInfoCount; ++i)
//			{
//				auto RewardInfo = ActRewardInfoPb2B2ActRewardInfo(response->reward_infos(i));
//				if (RewardInfo)
//				{
//					B2MessageInfoResponse->reward_infos.Emplace(RewardInfo);
//				}
//			}
//
//			for (auto identifier : response->has_reward_acts())
//			{
//				B2MessageInfoResponse->has_reward_acts.Emplace(ActIdentifierPb2B2ActIdentifier(identifier));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetStageInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetStageInfoResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetStageInfo>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int RewardInfoCount = response->reward_item_infos_size();
//			for (int i = 0; i < RewardInfoCount; ++i)
//			{
//				auto RewardInfo = StageClearRewardItemInfoPb2B2StageClearRewardItemInfo(response->reward_item_infos(i));
//				if (RewardInfo)
//				{
//					B2MessageInfoResponse->reward_item_infos.Emplace(RewardInfo);
//				}
//			}
//
//			int AetherRewardCount = response->reward_aether_infos_size();
//			for (int i = 0; i < AetherRewardCount; ++i)
//			{
//				auto RewardInfo = StageClearRewardAetherInfoPb2B2StageClearRewardAetherInfo(response->reward_aether_infos(i));
//				if (RewardInfo)
//				{
//					B2MessageInfoResponse->reward_aether_infos.Emplace(RewardInfo);
//				}
//			}
//
//			for (auto materialItem : response->material_items())
//			{
//				B2MessageInfoResponse->material_items.Emplace(ToB2DropData(materialItem));
//			}
//
//			B2MessageInfoResponse->auto_repeat_battle_cost = response->auto_repeat_battle_cost();
//			B2MessageInfoResponse->need_blade_point = response->need_blade_point();
//
//			B2MessageInfoResponse->enable_auto_repeat_battle = response->enable_auto_repeat_battle();
//			B2MessageInfoResponse->left_buff_ticket_exp = response->left_buff_ticket_exp();
//			B2MessageInfoResponse->left_buff_ticket_gold = response->left_buff_ticket_gold();
//			B2MessageInfoResponse->left_buff_ticket_autoskill = response->left_buff_ticket_autoskill();
//			B2MessageInfoResponse->left_buff_ticket_boost = response->left_buff_ticket_boost();
//			B2MessageInfoResponse->stage_boost_ticket = response->stage_boost_ticket();
//			B2MessageInfoResponse->best_clear_mask = response->best_clear_mask();
//		}
//		return B2MessageInfoResponse;
//	}
//
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromContentsModeStateNotify(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ContentsModeStateNotify>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoContentsModeStateNotify>();
//		b2->mode = response->mode();
//		b2->prev_state = response->prev_state();
//		b2->current_state = response->current_state();
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetMatchStatusResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetMatchStatusResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetMatchStatus>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//			B2MessageInfoResponse->from_ranking = response->from_ranking();
//			B2MessageInfoResponse->total_ranker_count = response->total_ranker_count();
//
//			B2MessageInfoResponse->match_status = MatchStatusPb2B2MatchStatus(response->match_status());
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//
//			B2MessageInfoResponse->main_character_type = PbCharacter2B2CharacterType(response->main_character_type());
//			B2MessageInfoResponse->tag_character_type = PbCharacter2B2CharacterType(response->tag_character_type());
//			B2MessageInfoResponse->total_power = response->total_power();
//
//			if (response->has_ranking_reward())
//			{
//				B2MessageInfoResponse->ranking_reward = RankingRewardPb2B2RankingReward(response->ranking_reward());
//			}
//			else 
//			{
//				B2MessageInfoResponse->ranking_reward = nullptr;
//			}
//
//			B2MessageInfoResponse->settlement_time = response->settlement_time();
//			B2MessageInfoResponse->settlement_period_hour = response->settlement_period_hour();
//
//			 daily reward
//			if (response->has_today_reward()) {
//				B2MessageInfoResponse->today_reward = response->today_reward();
//			}
//			else {
//				B2MessageInfoResponse->today_reward = false;
//			}
//			if (response->has_last_reward()) {
//				B2MessageInfoResponse->last_reward = response->last_reward();
//			}
//			else {
//				B2MessageInfoResponse->last_reward = false;
//			}
//			B2MessageInfoResponse->today_participation_count = response->today_participation_count();
//			if (response->has_current_gold()) {
//				B2MessageInfoResponse->current_gold = response->current_gold();
//			}
//			else {
//				B2MessageInfoResponse->current_gold = 0;
//			}
//			if (response->has_current_bladepoint()) {
//				B2MessageInfoResponse->current_bladepoint = response->current_bladepoint();
//			}
//			else {
//				B2MessageInfoResponse->current_bladepoint = 0;
//			}
//			if (response->has_current_gem()) {
//				B2MessageInfoResponse->current_gem = response->current_gem();
//			}
//			else {
//				B2MessageInfoResponse->current_gem = 0;
//			}
//			if (response->has_match_point()) {
//				B2MessageInfoResponse->match_point = response->match_point();
//			}
//			else {
//				B2MessageInfoResponse->match_point = 0;
//			}
//			if (response->has_additional_match_count()) {
//				B2MessageInfoResponse->additional_match_count = response->additional_match_count();
//			}
//			else {
//				B2MessageInfoResponse->additional_match_count = 0;
//			}	
//			if (response->has_match_point_next_charge_second()) {
//				B2MessageInfoResponse->match_point_next_charge_second = response->match_point_next_charge_second();
//			}
//			else {
//				B2MessageInfoResponse->match_point_next_charge_second = 0;
//			}
//
//			 reward box
//			if (response->has_reward_box_status()) {
//				B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//			}
//			else {
//				B2MessageInfoResponse->reward_box_status = nullptr;	// std::make_shared<B2DuelRewardBoxStatus>();
//			}
//
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMatchmakingResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::MatchmakingResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoMatchmakingResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//            auto MatchInfo = MatchInfoPb2B2MatchInfo(response->match_info());
//            B2MessageInfoResponse->match_info = MatchInfo;
//
//            B2MessageInfoResponse->room_type = response->room_type();
//
//            if (response->has_room_location())
//            {
//                auto RoomLocation = RoomLocationPb2B2RoomLocation(response->room_location());
//                B2MessageInfoResponse->room_location = RoomLocation;
//            }
//
//			if (response->has_ai_opponent())
//			{
//				B2MessageInfoResponse->ai_opponent = MatcherPb2B2Matcher(response->ai_opponent());
//			}
//
//			if (response->has_additional_match_count())
//			{
//				B2MessageInfoResponse->addadditional_match_count = response->additional_match_count();
//			}
//
//			if (response->has_current_gem())
//			{
//				B2MessageInfoResponse->current_gem = response->current_gem();
//			}
//
//			if (response->has_match_point())
//			{
//				B2MessageInfoResponse->match_point = response->match_point();
//			}
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRaidMatchmakingResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::RaidMatchmakingResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRaidMatchmakingResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//            const auto reqType = response->req_type();
//			B2MessageInfoResponse->req_type = response->req_type();
//			B2MessageInfoResponse->raid_token = ANSI_TO_TCHAR(response->raid_token().c_str());
//			B2MessageInfoResponse->room_type = response->room_type();
//            B2MessageInfoResponse->room_access = response->room_access();
//
//            if (reqType == b2::protocol::session::RaidReqType::QUICK_JOIN)
//            {
//                if (response->has_channel_endpoint())
//                {
//                    auto Endpoint = EndpointPb2B2Endpoint(response->channel_endpoint());
//                    B2MessageInfoResponse->channel_endpoint = Endpoint;
//                }
//            }
//            else if (reqType == b2::protocol::session::RaidReqType::CREATE_PRIVATE_ROOM)
//            {
//                if (response->has_room_location())
//                {
//                    B2MessageInfoResponse->room_location = RoomLocationPb2B2RoomLocation(response->room_location());
//                }
//                else
//                {
//                     TODO this should never happen!
//                }
//            }
//            else
//            {
//                 NOT IMPLEMENTED
//            }
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//        }
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_INVENTORY_COUNT)
//		{
//			int InventoryCountInfoSize = response->inventory_count_infos_size();
//			for (int i = 0; i < InventoryCountInfoSize; ++i)
//			{
//				auto InventoryCountInfo = InventoryCountInfoPb2B2InventoryCountServerInfo(response->inventory_count_infos(i));
//				if (InventoryCountInfo)
//				{
//					B2MessageInfoResponse->inventory_count_infos.Emplace(InventoryCountInfo);
//				}
//			}
//		}
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromCancelMatchmakingResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::CancelMatchmakingResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoCancelMatchmakingResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromInvitationResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::InvitationResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoInvitationResponse>();
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//            if (response->has_invitee())
//            {
//				B2MessageInfoResponse->invitee = UTF8_TO_TCHAR(response->invitee().c_str());
//            }
//
//            if (response->has_invitation_ref())
//            {
//                B2MessageInfoResponse->invitation_ref = ANSI_TO_TCHAR(response->invitation_ref().c_str());
//            }
//
//            if (response->has_room_location())
//            {
//                B2MessageInfoResponse->room_location = RoomLocationPb2B2RoomLocation(response->room_location());
//            }
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromAcceptInvitationResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::AcceptInvitationResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoAcceptInvitationResponse>();
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//        else
//        {
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromDeclineInvitationResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::DeclineInvitationResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoDeclineInvitationResponse>();
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//        else
//        {
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomJoinResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomJoinResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomJoinResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//            B2MessageInfoResponse->room_id = ANSI_TO_TCHAR(response->room_id().c_str());
//			B2MessageInfoResponse->room_type = PbRoomType2B2RoomType(response->room_type());
//            B2MessageInfoResponse->host = RoomParticipantPb2B2RoomParticipant(response->host());
//            for (auto participant : response->participants())
//            {
//                B2MessageInfoResponse->participants.Emplace(RoomParticipantPb2B2RoomParticipant(participant));
//            }
//            for (auto property : response->properties())
//            {
//                B2MessageInfoResponse->properties.Emplace(RoomPropertyPb2B2RoomProperty(property));
//            }
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomLeaveResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomLeaveResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomLeaveResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//            for (auto roomId : response->room_id_list())
//            {
//                B2MessageInfoResponse->room_id_list.Add(ANSI_TO_TCHAR(roomId.c_str()));
//            }
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomKickResponse(MessagePtr message)
//	{
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomKickResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomKickResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//			B2MessageInfoResponse->kicked_account_id = response->kicked_account_id();
//        }
//
//        return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomSetPropertyResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomSetPropertyResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomSetPropertyResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomInviteResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomInviteResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomInviteResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomCloseResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomCloseResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomCloseResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			if (response->has_token())
//			{
//				B2MessageInfoResponse->token = ANSI_TO_TCHAR(response->token().c_str());
//			}
//
//			if (response->has_room_type())
//			{
//				B2MessageInfoResponse->room_type = PbRoomType2B2RoomType(response->room_type());
//			}
//
//			if (response->has_room_id())
//			{
//				B2MessageInfoResponse->room_id = ANSI_TO_TCHAR(response->room_id().c_str());
//			}
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomLeaveResultResponse(MessagePtr message)
//	{
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomLeaveResultResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomLeaveResultResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//
//        return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomAccessControlResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomAccessControlResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomInviteResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//
//        return B2MessageInfoResponse;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomHeartbeatNotify(MessagePtr message)
//	{
//        auto response = std::static_pointer_cast<b2::protocol::channel::RoomHeartbeatNotify>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoRoomHeartbeatNotify>();
//
//        return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReportUserViaChannelResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::channel::ReportUserViaChannelResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoReportUserViaChannelResponse>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromWorldJoinResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::channel::WorldJoinResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoWorldJoinResponse>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->world_id = ANSI_TO_TCHAR(response->world_id().c_str());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromWorldLeaveResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::channel::WorldLeaveResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoWorldLeaveResponse>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto worldId : response->world_id_list())
//			{
//				B2MessageInfoResponse->world_id_list.Add(ANSI_TO_TCHAR(worldId.c_str()));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomParticipantJoinNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::channel::RoomParticipantJoinNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoRoomParticipantJoinNotify>();
//        B2MessageInfoNotify->room_id = ANSI_TO_TCHAR(notify->room_id().c_str());
//		B2MessageInfoNotify->room_type = PbRoomType2B2RoomType(notify->room_type());
//        B2MessageInfoNotify->joiner = RoomParticipantPb2B2RoomParticipant(notify->joiner());
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomParticipantLeaveNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::channel::RoomParticipantLeaveNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoRoomParticipantLeaveNotify>();
//        B2MessageInfoNotify->room_id = ANSI_TO_TCHAR(notify->room_id().c_str());
//		B2MessageInfoNotify->room_type = PbRoomType2B2RoomType(notify->room_type());
//        B2MessageInfoNotify->leaver = RoomParticipantPb2B2RoomParticipant(notify->leaver());
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomHostChangedNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::channel::RoomHostChangedNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoRoomHostChangedNotify>();
//        B2MessageInfoNotify->room_id = ANSI_TO_TCHAR(notify->room_id().c_str());
//        B2MessageInfoNotify->room_type = PbRoomType2B2RoomType(notify->room_type());
//        B2MessageInfoNotify->host_id = notify->host_id();
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromInvitationNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::session::InvitationNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoInvitationNotify>();
//        B2MessageInfoNotify->invitation_type = notify->invitation_type();
//		B2MessageInfoNotify->inviter = UTF8_TO_TCHAR(notify->inviter().c_str());
//        B2MessageInfoNotify->invitation_ref = ANSI_TO_TCHAR(notify->invitation_ref().c_str());
//        B2MessageInfoNotify->room_location = RoomLocationPb2B2RoomLocation(notify->room_location());
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRaidInvitationNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::session::RaidInvitationNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoRaidInvitationNotify>();
//        B2MessageInfoNotify->inviter = UTF8_TO_TCHAR(notify->inviter().c_str());
//        B2MessageInfoNotify->invitation_key = ANSI_TO_TCHAR(notify->invitation_key().c_str());
//        B2MessageInfoNotify->room_location = RoomLocationPb2B2RoomLocation(notify->room_location());
//		B2MessageInfoNotify->map_id = notify->map_id();
//		B2MessageInfoNotify->difficulty = notify->difficulty();
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromAcceptInvitationNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::session::AcceptInvitationNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoAcceptInvitationNotify>();
//        B2MessageInfoNotify->invitation_type = notify->invitation_type();
//		B2MessageInfoNotify->invitee = UTF8_TO_TCHAR(notify->invitee().c_str());
//        B2MessageInfoNotify->invitation_ref = ANSI_TO_TCHAR(notify->invitation_ref().c_str());
//        B2MessageInfoNotify->room_location = RoomLocationPb2B2RoomLocation(notify->room_location());
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromDeclineInvitationNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::session::DeclineInvitationNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoDeclineInvitationNotify>();
//        B2MessageInfoNotify->invitation_type = notify->invitation_type();
//		B2MessageInfoNotify->invitee = UTF8_TO_TCHAR(notify->invitee().c_str());
//        B2MessageInfoNotify->invitation_ref = ANSI_TO_TCHAR(notify->invitation_ref().c_str());
//        return B2MessageInfoNotify;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomMessageNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::channel::RoomMessageNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoRoomMessageNotify>();
//        B2MessageInfoNotify->room_id = ANSI_TO_TCHAR(notify->room_id().c_str());
//		B2MessageInfoNotify->room_type = PbRoomType2B2RoomType(notify->room_type());
//        B2MessageInfoNotify->sender = RoomParticipantPb2B2RoomParticipant(notify->sender());
//		B2MessageInfoNotify->message = UTF8_TO_TCHAR(notify->message().c_str());
//        return B2MessageInfoNotify;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomMessageNotifyBinary(const void* data, int size)
//	{
//		ProtobufCodec codec;
//		uint32_t decoded = 0;
//		uint32_t demanded = 0;
//		auto result = codec.Decode(reinterpret_cast<const char*>(data), size, decoded, demanded);
//		if (result.first == ProtobufCodec::Ok)
//		{
//			auto message = result.second;
//			
//			 TODO
//			if (ProtobufFactory::IsTypeOf(message, b2::protocol::channel::RoomParticipantJoinNotify::default_instance()))
//			{
//				return FromRoomParticipantJoinNotify(message);
//			}
//			else if (ProtobufFactory::IsTypeOf(message, b2::protocol::channel::RoomParticipantLeaveNotify::default_instance()))
//			{
//				return FromRoomParticipantLeaveNotify(message);
//			}
//			else if (ProtobufFactory::IsTypeOf(message, b2::protocol::channel::RoomMessageNotify::default_instance()))
//			{
//				return FromRoomMessageNotify(message);
//			}
//			else if (ProtobufFactory::IsTypeOf(message, b2::protocol::channel::RoomHostChangedNotify::default_instance()))
//			{
//				return FromRoomHostChangedNotify(message);
//			}
//			else if (ProtobufFactory::IsTypeOf(message, b2::protocol::channel::RoomPropertyUpdatedNotify::default_instance()))
//			{
//				return FromRoomPropertyUpdatedNotify(message);
//			}
//			return nullptr;
//		}
//		return nullptr;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRoomPropertyUpdatedNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::channel::RoomPropertyUpdatedNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoRoomPropertyUpdatedNotify>();
//        B2MessageInfoNotify->room_id = ANSI_TO_TCHAR(notify->room_id().c_str());
//        for (const auto& property : notify->properties())
//        {
//            B2MessageInfoNotify->properties.Emplace(RoomPropertyPb2B2RoomProperty(property));
//        }
//        return B2MessageInfoNotify;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLevelUpSkillResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LevelUpSkillResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLevelUpSkill>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character_type = PbCharacter2B2CharacterType(response->character_type());
//			B2MessageInfoResponse->left_skill_point = response->left_skill_point();
//			B2MessageInfoResponse->skill = SkillPb2B2SkillServerInfo(response->skill());
//			B2MessageInfoResponse->current_gold = response->current_gold();
//
//			for (auto quickSlot : response->skill_quick_slots())
//			{
//				B2MessageInfoResponse->skill_quick_slots.Emplace(SkillQuickSlotPb2B2SkillQuickSlot(quickSlot));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromResetSkillResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ResetSkillResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseResetSkill>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->character_type = PbCharacter2B2CharacterType(response->character_type());
//			B2MessageInfoResponse->left_skill_point = response->left_skill_point();
//
//			for (auto skill : response->skills())
//			{
//				B2MessageInfoResponse->skills.Emplace(SkillPb2B2SkillServerInfo(skill));
//			}
//
//			for (auto quickSlot : response->skill_quick_slots())
//			{
//				B2MessageInfoResponse->skill_quick_slots.Emplace(SkillQuickSlotPb2B2SkillQuickSlot(quickSlot));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetUsingSkillResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetUsingSkillResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetUsingSkill>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character_type = PbCharacter2B2CharacterType(response->character_type());
//			B2MessageInfoResponse->skill_quick_slot = SkillQuickSlotPb2B2SkillQuickSlot(response->skill_quick_slot());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuySkillPointResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuySkillPointResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBuySkillPoint>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->left_gem = response->left_gem();
//
//			int dataCount = response->buy_results_size();
//			for (int i = 0; i < dataCount; ++i)
//			{
//				const b2::protocol::session::BuySkillPointResponse::BuyResult& buyResult = response->buy_results(i);
//
//				auto b2BuySkillResponse = std::make_shared<B2BuySkillPointResponseInfo>();
//				b2BuySkillResponse->character_type = PbCharacter2B2CharacterType(buyResult.character_type());
//				b2BuySkillResponse->left_skill_point = buyResult.left_skill_point();
//				b2BuySkillResponse->total_skill_point_buy_count = buyResult.total_skill_point_buy_count();
//
//				B2MessageInfoResponse->buy_results.Emplace(b2BuySkillResponse);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDeleteAccountResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DeleteAccountResponse>(message);
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDeleteAccount>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAddItemToInventoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AddItemToInventoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAddItemToInventory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int itemCount = response->items_size();
//			for (int i = 0; i < itemCount; ++i)
//			{
//				const b2::protocol::commondata::Item& item = response->items(i);
//
//				auto b2Item = ItemPb2B2ItemServerInfo(item);
//				if (b2Item)
//				{
//					B2MessageInfoResponse->items.Emplace(b2Item);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAddCostumeToInventoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AddCostumeToInventoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAddCostumeToInventory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->costume = CostumePb2B2CostumeServerInfo(response->costume());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromIncreaseCashPaidResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::IncreaseCashPaidResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseIncreaseCashPaid>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->cash_paid = response->cash_paid();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromIncreaseMoneyResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::IncreaseMoneyResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseIncreaseMoney>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->money = response->money();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetItemLevelupResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetItemLevelupResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetItemLevelup>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->item = ItemPb2B2ItemServerInfo(response->item());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetCharacterLevelExpResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetCharacterLevelExpResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetCharacterLevelExp>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character = CharacterPb2B2CharacterServerInfo(response->character());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetCharacterRankInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetCharacterRankInfoResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetCharacterRankInfo>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character = CharacterPb2B2CharacterServerInfo(response->character());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetCharacterRelicResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetCharacterRelicResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetCharacterRelic>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->relic = ToB2Relic(response->relic());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetBladePointResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetBladePointResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetBladePoint>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->blade_point = response->blade_point();
//		}
//		return B2MessageInfoResponse;
//	}
//
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromIncreaseRankPromotionPointResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::IncreaseRankPromotionPointResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseIncreaseRankPromotionPoint>();		
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveActClearRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ActClearRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveActClearReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward_index = response->reward_index();
//			B2MessageInfoResponse->reward_gold = 0;	// @NOTE ���� ����
//			B2MessageInfoResponse->reward_gem = 0;	// @NOTE ���� ����
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetTeamBattleStatusResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetTeamBattleStatusResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetTeamBattleStatus>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//			B2MessageInfoResponse->from_ranking = response->from_ranking();
//			B2MessageInfoResponse->total_ranker_count = response->total_ranker_count();
//
//			B2MessageInfoResponse->match_status = MatchStatusPb2B2MatchStatus(response->match_status());
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//
//			int formationCount = response->formations_size();
//			for (int i = 0; i < formationCount; ++i)
//			{
//				const b2::protocol::commondata::BattleFormation& formation = response->formations(i);
//
//				auto b2Formation = BattleFormationPb2B2BattleFormation(formation);
//				if (b2Formation)
//				{
//					B2MessageInfoResponse->formations.Emplace(b2Formation);
//				}
//			}
//
//			B2MessageInfoResponse->entry = TeamBattleEntryPb2B2TeamBattleEntry(response->entry());
//
//			if (response->has_ranking_reward())
//			{
//				B2MessageInfoResponse->ranking_reward = RankingRewardPb2B2RankingReward(response->ranking_reward());
//			}
//			else
//			{
//				B2MessageInfoResponse->ranking_reward = nullptr;
//			}
//
//			B2MessageInfoResponse->settlement_time = response->settlement_time();
//			B2MessageInfoResponse->settlement_period_hour = response->settlement_period_hour();
//
//			 daily reward
//			if (response->has_today_reward()) {
//				B2MessageInfoResponse->today_reward = response->today_reward();
//			}
//			else {
//				B2MessageInfoResponse->today_reward = false;
//			}
//			if (response->has_last_reward()) {
//				B2MessageInfoResponse->last_reward = response->last_reward();
//			}
//			else {
//				B2MessageInfoResponse->last_reward = false;
//			}
//			B2MessageInfoResponse->today_participation_count = response->today_participation_count();
//			if (response->has_current_gold()) {
//				B2MessageInfoResponse->current_gold = response->current_gold();
//			}
//			else {
//				B2MessageInfoResponse->current_gold = 0;
//			}
//			if (response->has_current_bladepoint()) {
//				B2MessageInfoResponse->current_bladepoint = response->current_bladepoint();
//			}
//			else {
//				B2MessageInfoResponse->current_bladepoint = 0;
//			}
//			if (response->has_current_gem()) {
//				B2MessageInfoResponse->current_gem = response->current_gem();
//			}
//			else {
//				B2MessageInfoResponse->current_gem = 0;
//			}
//			if (response->has_match_point()) {
//				B2MessageInfoResponse->match_point = response->match_point();
//			}
//			else {
//				B2MessageInfoResponse->match_point = 0;
//			}
//			if (response->has_additional_match_count()) {
//				B2MessageInfoResponse->additional_match_count = response->additional_match_count();
//			}
//			else {
//				B2MessageInfoResponse->additional_match_count = 0;
//			}
//			if (response->has_match_point_next_charge_second()) {
//				B2MessageInfoResponse->match_point_next_charge_second = response->match_point_next_charge_second();
//			}
//			else {
//				B2MessageInfoResponse->match_point_next_charge_second = 0;
//			}
//
//			 reward box
//			if (response->has_reward_box_status()) {
//				B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//			}
//			else {
//				B2MessageInfoResponse->reward_box_status = nullptr;
//			}
//	
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpdateTeamBattleEntryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpdateTeamBattleEntryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUpdateTeamBattleEntry>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->entry = TeamBattleEntryPb2B2TeamBattleEntry(response->entry());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceTeamBattleFormationResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceTeamBattleFormationResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceTeamBattleFormation>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->formation = BattleFormationPb2B2BattleFormation(response->formation());
//			B2MessageInfoResponse->current_gold = response->current_gold();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromTeamMatchmakingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::TeamMatchmakingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseTeamMatchmaking>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->match_token = ANSI_TO_TCHAR(response->match_token().c_str());
//			B2MessageInfoResponse->opponent_team = TeamPb2B2Team(response->opponent_team());
//
//			if (response->has_additional_match_count())
//			{
//				B2MessageInfoResponse->addadditional_match_count = response->additional_match_count();
//			}
//
//			if (response->has_current_gem())
//			{
//				B2MessageInfoResponse->current_gem = response->current_gem();
//			}
//
//			if (response->has_match_point())
//			{
//				B2MessageInfoResponse->match_point = response->match_point();
//			}
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromTeamMatchResultResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::TeamMatchResultResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseTeamMatchResult>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->match_status = MatchStatusPb2B2MatchStatus(response->match_status());
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//			
//			if (response->has_prev_ranker()) {
//				B2MessageInfoResponse->prev_ranker = MatchRankerPb2B2MatchRanker(response->prev_ranker());
//			}
//
//			 reward box
//			if (response->has_reward_box_status()) {
//				B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//			}
//			else {
//				B2MessageInfoResponse->reward_box_status = nullptr;
//			}
//
//			for (auto reward : response->rewards()) {
//				auto b2Reward = ToB2Reward(reward);
//				B2MessageInfoResponse->rewards.Emplace(b2Reward);
//			}
//
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMatchResultResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::MatchResultResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoMatchResultResponse>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->status = MatchStatusPb2B2MatchStatus(response->status());
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//
//			if (response->has_prev_ranker()) {
//				B2MessageInfoResponse->prev_ranker = MatchRankerPb2B2MatchRanker(response->prev_ranker());
//			}
//			else {
//				B2MessageInfoResponse->prev_ranker = nullptr;
//			}
//
//			 reward box
//			if (response->has_reward_box_status()) {
//				B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//			}
//			else {
//				B2MessageInfoResponse->reward_box_status = nullptr;
//			}
//
//			for (auto reward : response->rewards()) {
//				auto b2Reward = ToB2Reward(reward);
//				B2MessageInfoResponse->rewards.Emplace(b2Reward);
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetRankResponse(MessagePtr message)
//	{
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetRank>();
//		
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceRankNodeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceRankNodeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceRankNode>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character_type = PbCharacter2B2CharacterType(response->character_type());
//			B2MessageInfoResponse->rank = response->rank();
//			B2MessageInfoResponse->rank_node_index = response->rank_node_index();
//			B2MessageInfoResponse->rank_node_state = response->rank_node_state();
//
//			int ChangedRankPromotionItemCount = response->changed_rank_promotion_points_size();
//			for (int i = 0; i < ChangedRankPromotionItemCount; ++i)
//			{
//				auto rankPromotionItem = ItemPb2B2ItemServerInfo(response->changed_rank_promotion_points(i));
//				if (rankPromotionItem)
//				{
//					B2MessageInfoResponse->changed_rank_promotion_points.Emplace(rankPromotionItem);
//				}
//			}
//
//			int DeletedrankPromotionItemIdCount = response->deleted_rank_promotion_point_ids_size();
//			for (int i = 0; i < DeletedrankPromotionItemIdCount; ++i)
//			{
//				int64 DeletedRankPromotionItemId = response->deleted_rank_promotion_point_ids(i);
//				B2MessageInfoResponse->deleted_rank_promotion_point_ids.Emplace(DeletedRankPromotionItemId);
//			}
//			
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->is_success = response->is_success();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetMailListResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetMailListResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetMailList>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int mailCount = response->mail_infos_size();
//			
//			for (int i = 0; i < mailCount; ++i)
//			{
//				auto b2MailInfo = std::make_shared<B2MailInfo>();
//				const b2::protocol::session::GetMailListResponse::MailInfo& mailInfo = response->mail_infos(i);
//
//				b2MailInfo->id = mailInfo.id();
//				b2MailInfo->mail_category = PbMailCategory2B2MailCategory(mailInfo.mail_category());
//				b2MailInfo->mail_attachType = PbMailAttachType2B2MailAttachType(mailInfo.mail_attachtype());
//				if (mailInfo.has_mail_item_info())
//				{
//					b2MailInfo->mail_item_info = ToB2MailItemInfo(mailInfo.mail_item_info());
//				}
//
//				b2MailInfo->sender_type = mailInfo.sender_type();
//				b2MailInfo->source_type = mailInfo.source_type();
//
//				b2MailInfo->sender_name = UTF8_TO_TCHAR(mailInfo.sender_name().c_str());
//				b2MailInfo->expire_time = mailInfo.expire_time();
//				b2MailInfo->gold = mailInfo.gold();
//				b2MailInfo->gem = mailInfo.gem();
//				b2MailInfo->blade_point = mailInfo.blade_point();
//				b2MailInfo->social_point = mailInfo.social_point();				
//				b2MailInfo->general_lottery_id = mailInfo.general_lottery_id();
//				b2MailInfo->fixed_grade_lottery_id = mailInfo.fixed_grade_lottery_id();
//				b2MailInfo->selective_lottery_id = mailInfo.selective_lottery_id();
//				b2MailInfo->selective_item_lottery_id = mailInfo.selective_item_lottery_id();
//				b2MailInfo->hero_piece = mailInfo.hero_piece();
//				b2MailInfo->rank_promotion_point = mailInfo.rank_promotion_point();
//				b2MailInfo->dark_crystal = mailInfo.dark_crystal();
//				b2MailInfo->stage_boost_ticket = mailInfo.stage_boost_ticket();
//				b2MailInfo->hot_time_gold_buff_ticket = mailInfo.hot_time_gold_buff_ticket();
//				b2MailInfo->hot_time_exp_buff_ticket = mailInfo.hot_time_exp_buff_ticket();
//				b2MailInfo->shop_ten_lottery_ticket = mailInfo.shop_ten_lottery_ticket();
//				b2MailInfo->message = UTF8_TO_TCHAR(mailInfo.message().c_str());
//				b2MailInfo->package_id = mailInfo.package_id();
//				b2MailInfo->fame_point = mailInfo.fame_point();
//				b2MailInfo->selective_aether_lottery_id = mailInfo.selective_aether_lottery_id();
//				b2MailInfo->prefix_selective_lottery_id = mailInfo.prefix_selective_lottery_id();
//				b2MailInfo->stage_auto_clear_ticket = mailInfo.stage_auto_clear_ticket();
//				b2MailInfo->totem_prefix_lottery_id = mailInfo.totem_prefix_lottery_id();
//
//				for (auto selectiveItemInfo : mailInfo.selective_item_infos())
//				{
//					b2MailInfo->selective_item_infos.Emplace(ToB2MailItemInfo(selectiveItemInfo));
//				}
//				B2MessageInfoResponse->mail_infos.Emplace(b2MailInfo);
//			}
//
//			B2MessageInfoResponse->new_present_mail_count = response->new_present_mail_count();
//			B2MessageInfoResponse->new_bladepoint_mail_count = response->new_bladepoint_mail_count();
//			B2MessageInfoResponse->new_socialpoint_mail_count = response->new_socialpoint_mail_count();
//			B2MessageInfoResponse->new_lottery_mail_count = response->new_lottery_mail_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromOpenMailResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::OpenMailResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseOpenMail>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->mail_id = response->mail_id();
//			B2MessageInfoResponse->mail_item_info = ToB2MailItemInfo(response->mail_item_info());			
//			B2MessageInfoResponse->gold = response->gold();
//			B2MessageInfoResponse->gem = response->gem();
//			B2MessageInfoResponse->blade_point = response->blade_point();
//			B2MessageInfoResponse->social_point = response->social_point();
//			B2MessageInfoResponse->dark_crystal = response->dark_crystal();
//			B2MessageInfoResponse->stage_boost_ticket = response->stage_boost_ticket();
//			B2MessageInfoResponse->stage_auto_clear_ticket = response->stage_auto_clear_ticket();
//			B2MessageInfoResponse->hot_time_gold_buff_ticket = response->hot_time_gold_buff_ticket();
//			B2MessageInfoResponse->hot_time_exp_buff_ticket = response->hot_time_exp_buff_ticket();
//			B2MessageInfoResponse->shop_ten_lottery_ticket = response->shop_ten_lottery_ticket();
//			B2MessageInfoResponse->fame_point = response->fame_point();
//			B2MessageInfoResponse->aether = AetherPb2B2AetherServerInfo(response->aether());
//			B2MessageInfoResponse->totem = TotemPb2B2TotemServerInfo(response->totem());
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_INVENTORY_COUNT) {
//			B2MessageInfoResponse->exceed_inven_character_type = PbCharacter2B2CharacterType(response->exceed_inven_character_type());
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT) {
//			B2MessageInfoResponse->current_aether_count = response->current_aether_count();
//			B2MessageInfoResponse->aether_inventory_max_count = response->aether_inventory_max_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromOpenAllMailResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::OpenAllMailResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseOpenAllMail>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int mailIdCount = response->opened_mail_ids_size();
//			for (int i = 0; i < mailIdCount; ++i)
//			{
//				B2MessageInfoResponse->mail_ids.Emplace(response->opened_mail_ids(i));
//			}			
//
//			int itemInfoCount = response->item_infos_size();
//			for (int i = 0; i < itemInfoCount; ++i)
//			{
//				auto b2MailInfo = std::make_shared<B2MailItemInfo>();
//
//				b2MailInfo = ToB2MailItemInfo(response->item_infos(i));
//				b2MailInfo->item_template_id = response->item_infos(i).item_template_id();
//				b2MailInfo->item_amount = response->item_infos(i).item_amount();
//				B2MessageInfoResponse->item_infos.Emplace(b2MailInfo);
//			}
//
//			B2MessageInfoResponse->gold = response->gold();
//			B2MessageInfoResponse->gem = response->gem();
//			B2MessageInfoResponse->blade_point = response->blade_point();
//			B2MessageInfoResponse->social_point = response->social_point();
//			B2MessageInfoResponse->dark_crystal = response->dark_crystal();
//			B2MessageInfoResponse->stage_boost_ticket = response->stage_boost_ticket();
//			B2MessageInfoResponse->hot_time_gold_buff_ticket = response->hot_time_gold_buff_ticket();
//			B2MessageInfoResponse->hot_time_exp_buff_ticket = response->hot_time_exp_buff_ticket();
//			B2MessageInfoResponse->shop_ten_lottery_ticket = response->shop_ten_lottery_ticket();
//			B2MessageInfoResponse->fame_point = response->fame_point();
//			B2MessageInfoResponse->stage_auto_clear_ticket = response->stage_auto_clear_ticket();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetNewMailCountResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetNewMailCountResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetNewMailCount>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->new_mail_all = response->new_mail_all();
//			B2MessageInfoResponse->new_present_mail_count = response->new_present_mail_count();
//			B2MessageInfoResponse->new_bladepoint_mail_count = response->new_bladepoint_mail_count();
//			B2MessageInfoResponse->new_socialpoint_mail_count = response->new_socialpoint_mail_count();
//			B2MessageInfoResponse->new_lottery_mail_count = response->new_lottery_mail_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAskFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AskFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAskFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAcceptAskFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AcceptAskFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAcceptAskFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRejectAskFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RejectAskFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRejectAskFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetFriendListResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetFriendListResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetFriendList>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto friendPb : response->friends())
//			{
//				B2MessageInfoResponse->friends.Emplace(FriendPb2B2Friend(friendPb));
//			}
//
//			B2MessageInfoResponse->left_delete_friend_count = response->left_delete_friend_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCancelAskFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CancelAskFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCancelAskFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDeleteFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DeleteFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDeleteFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->left_delete_friend_count = response->left_delete_friend_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAllSendAskFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAllSendAskFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAllSendAskFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto user : response->acceptors())
//			{
//				B2MessageInfoResponse->acceptors.Emplace(UserPb2B2User(user));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAllReceiveAskFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAllReceiveAskFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAllReceiveAskFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto user : response->requestors())
//			{
//				B2MessageInfoResponse->requestors.Emplace(UserPb2B2User(user));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSendSocialPointResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SendSocialPointResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSendSocialPoint>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_social_point = response->current_social_point();
//			for (auto accountId : response->success_account_ids())
//			{
//				B2MessageInfoResponse->success_account_ids.Emplace(accountId);
//			}
//
//			if (response->has_fail_account_id()) 
//			{
//				B2MessageInfoResponse->fail_account_id = response->fail_account_id();
//			}
//			else 
//			{
//				B2MessageInfoResponse->fail_account_id = 0;
//			}
//
//			if (response->has_fail_error_code())
//			{
//				B2MessageInfoResponse->fail_error_code = response->fail_error_code();
//			}
//			else
//			{
//				B2MessageInfoResponse->fail_error_code = 0;
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetRecommendFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetRecommendFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetRecommendFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto user : response->recommended_users())
//			{
//				B2MessageInfoResponse->recommended_users.Emplace(UserPb2B2User(user));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFindUserResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FindUserResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseFindUser>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->user = UserPb2B2User(response->user());
//
//			if (response->has_guild_invite_state()) 
//			{
//				B2MessageInfoResponse->guild_invite_state = ToB2GuildInviteState(response->guild_invite_state());
//			}
//			else
//			{
//				B2MessageInfoResponse->guild_invite_state = B2GuildInviteState::NONE;
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromConfirmNewFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ConfirmNewFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseConfirmNewFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		for (auto accountId : response->applied_friend_ids())
//		{
//			B2MessageInfoResponse->applied_friend_ids.Emplace(accountId);
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPlatformFriendInviteInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PlatformFriendInviteInfoResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePlatformFriendInviteInfo>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->invite_reward_info = ToB2FriendInviteRewardInfo(response->invite_reward_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPlatformFriendInviteCountUpResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PlatformFriendInviteCountUpResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePlatformFriendInviteCountUp>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->invite_reward_info = ToB2FriendInviteRewardInfo(response->invite_reward_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPlatformFriendJoinerCountUpResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PlatformFriendJoinerCountUpResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePlatformFriendJoinerCountUp>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->invite_reward_info = ToB2FriendInviteRewardInfo(response->invite_reward_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPlatformFriendInviteRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PlatformFriendInviteRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePlatformFriendInviteReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->invite_reward_info = ToB2FriendInviteRewardInfo(response->invite_reward_info());
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetQuestResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetQuestResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetQuest>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto questInfo : response->quest_infos())
//			{
//				B2MessageInfoResponse->quest_infos.Emplace(QuestInfoPb2B2QuestInfo(questInfo));
//			}
//
//			B2MessageInfoResponse->daily_quest_initialized = response->daily_quest_initialized();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAcceptQuestResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AcceptQuestResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAcceptQuest>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{			
//			B2MessageInfoResponse->quest_info = QuestInfoPb2B2QuestInfo(response->quest_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveQuestRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveQuestRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveQuestReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->received_reward_slot = PbQuestSlotTypeToB2QuestSlotType(response->received_reward_slot());
//			B2MessageInfoResponse->next_quest_info = QuestInfoPb2B2QuestInfo(response->next_quest_info());
//			for (auto pbReward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetCounterDungeonResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetCounterDungeonResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetCounterDungeon>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int DungeonTypeCount = response->opened_dungeon_types_size();
//			for (int i = 0; i < DungeonTypeCount; ++i)
//			{
//				auto dungeonType = response->opened_dungeon_types(i);
//				B2MessageInfoResponse->opened_dungeon_types.Emplace(PbCounterDungeonType2B2CounterDungeonType(dungeonType));
//			}
//
//			B2MessageInfoResponse->left_ticket = response->left_ticket();
//			B2MessageInfoResponse->weekly_clear_count = response->weekly_clear_count();
//			B2MessageInfoResponse->received_weekly_reward = response->received_weekly_reward();
//			B2MessageInfoResponse->next_buy_ticket_gem_cost = response->next_buy_ticket_gem_cost();
//			B2MessageInfoResponse->weekly_reset_day_of_week = PbDayOfWeek2B2DayOfWeek(response->weekly_reset_day_of_week());
//			B2MessageInfoResponse->main_character_type = PbCharacter2B2CharacterType(response->main_character_type());
//			B2MessageInfoResponse->buy_ticket_count = response->buy_ticket_count();
//			B2MessageInfoResponse->buy_ticket_count_max = response->buy_ticket_count_max();
//			B2MessageInfoResponse->highest_difficulty_level = response->highest_difficulty_level();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromStartCounterDungeonResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::StartCounterDungeonResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseStartCounterDungeon>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->play_token = ANSI_TO_TCHAR(response->play_token().c_str());
//			B2MessageInfoResponse->drop_gold = response->drop_gold();
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFinishCounterDungeonResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FinishCounterDungeonResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseFinishCounterDungeon>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//
//			for (auto pbReward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//
//			B2MessageInfoResponse->left_ticket = response->left_ticket();
//			B2MessageInfoResponse->weekly_clear_count = response->weekly_clear_count();
//			B2MessageInfoResponse->next_buy_ticket_gem_cost = response->next_buy_ticket_gem_cost();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSweepCounterDungeonResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SweepCounterDungeonResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSweepCounterDungeon>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->dungeon_id = response->dungeon_id();
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//
//			for (auto pbReward : response->rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//
//			B2MessageInfoResponse->left_ticket = response->left_ticket();
//			B2MessageInfoResponse->weekly_clear_count = response->weekly_clear_count();
//			B2MessageInfoResponse->next_buy_ticket_gem_cost = response->next_buy_ticket_gem_cost();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuyCounterDungeonTicketResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuyCounterDungeonTicketResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBuyCounterDungeonTicket>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->left_ticket = response->left_ticket();
//			B2MessageInfoResponse->buy_ticket_count = response->buy_ticket_count();
//			B2MessageInfoResponse->buy_ticket_count_max = response->buy_ticket_count_max();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCounterDungeonWeeklyRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CounterDungeonWeeklyRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCounterDungeonWeeklyReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//
//			for (auto pbReward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//
//			B2MessageInfoResponse->received_weekly_reward = response->received_weekly_reward();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetDimensionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetDimensionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetDimension>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->dimension_status = ToB2DimensionStatus(response->dimension_status());
//
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//			B2MessageInfoResponse->from_ranking = response->has_from_ranking() ? response->from_ranking() : 0;
//			B2MessageInfoResponse->total_ranker_count = response->has_total_ranker_count() ? response->total_ranker_count() : 0;
//
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//
//			if (response->has_ranking_reward())
//			{
//				B2MessageInfoResponse->ranking_reward = ToPb2B2DimensionRankingReward(response->ranking_reward());
//			}
//			else
//			{
//				B2MessageInfoResponse->ranking_reward = nullptr;
//			}
//
//			B2MessageInfoResponse->settlement_time = response->settlement_time();
//			B2MessageInfoResponse->settlement_period_hour = response->settlement_period_hour();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromStartDimensionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::StartDimensionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseStartDimension>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->play_token = ANSI_TO_TCHAR(response->play_token().c_str());
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFinishDimensionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FinishDimensionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseFinishDimension>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->play_result = ToPBDimensionResult(response->play_result());
//			B2MessageInfoResponse->dimension_status = ToB2DimensionStatus(response->dimension_status());
//
//			B2MessageInfoResponse->is_new_record = response->is_new_record();
//			for (auto fixedItem : response->fixed_items())
//			{
//				B2MessageInfoResponse->fixed_items.Emplace(ToB2DropData(fixedItem));
//			}
//
//			for (auto pbTotem : response->totems())
//			{
//				B2MessageInfoResponse->totems.Emplace(TotemPb2B2TotemServerInfo(pbTotem));
//			}
//
//			for (auto item : response->update_items())
//			{
//				B2MessageInfoResponse->update_items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCreateGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CreateGuildResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCreateGuild>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->guild_info = ToB2Guild(response->guild_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAskJoinGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AskJoinGuildResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAskJoinGuild>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLeaveGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LeaveGuildResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLeaveGuild>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->leave_guild_time = response->leave_guild_time();
//			B2MessageInfoResponse->guild_id = response->guild_id();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildDetailInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildDetailInfoResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetGuildDetailInfo>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->guild = ToB2Guild(response->guild());
//
//			for (auto guild_member : response->guild_members())
//			{
//				B2MessageInfoResponse->guild_members.Emplace(ToB2GuildMember(guild_member));
//			}
//
//			if (response->has_guild_battle_state())
//			{
//				B2MessageInfoResponse->guild_battle_state = ToB2GuildBattleState(response->guild_battle_state());
//			}
//			else 
//			{
//				B2MessageInfoResponse->guild_battle_state = ToB2GuildBattleStateNone();
//			}
//
//			if (response->has_entry())
//			{
//				B2MessageInfoResponse->entry = ToB2GuildBattleEntry(response->entry());
//			}
//			else
//			{
//				B2MessageInfoResponse->entry = nullptr;
//			}
//
//			if (response->has_has_turn_reward()) 
//			{
//				B2MessageInfoResponse->has_turn_reward = response->has_turn_reward();
//			}
//			else
//			{
//				B2MessageInfoResponse->has_turn_reward = false;
//			}
//
//			for (auto mercenary : response->mercenaries())
//			{
//				B2MessageInfoResponse->mercenaries.Emplace(ToB2GuildMercenary(mercenary));
//			}
//
//			 guild skill
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//
//			if (response->has_joined_guild_battle_member())
//			{
//				B2MessageInfoResponse->joined_guild_battle_member = response->joined_guild_battle_member();
//			}
//			else
//			{
//				B2MessageInfoResponse->joined_guild_battle_member = false;
//			}
//
//			 character powers
//			for (auto characterPower : response->character_powers())
//			{
//				B2MessageInfoResponse->character_powers.Emplace(CharacterPowerPb2B2CharacterPower(characterPower));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildMemberResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildMemberResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetGuildMember>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto guild_member : response->guild_members())
//			{
//				B2MessageInfoResponse->guild_members.Emplace(ToB2GuildMember(guild_member));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDeleteGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DeleteGuildResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDeleteGuild>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetRecommendGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetRecommendGuildResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetRecommendGuild>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto guild : response->guilds())
//			{
//				b2->guilds.Emplace(ToB2Guild(guild));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAppointSubGuildMasterResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AppointSubGuildMasterResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAppointSubGuildMaster>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->changed_member = ToB2GuildMember(response->changed_member());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDismissSubGuildMasterResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DismissSubGuildMasterResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDismissSubGuildMaster>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->changed_member = ToB2GuildMember(response->changed_member());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromKickGuildMemberResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::KickGuildMemberResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseKickGuildMember>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpdateGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpdateGuildResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseUpdateGuild>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->join_option = ToB2GuildJoinOption(response->join_option());
//			b2->mark_index = response->mark_index();
//			b2->mark_color = response->mark_color();
//			b2->mark_bg_index = response->mark_bg_index();
//			b2->mark_bg_color = response->mark_bg_color();
//			b2->current_gold = response->current_gold();
//		}
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpdateGuildNoticeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpdateGuildNoticeResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseUpdateGuildNotice>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->notice = UTF8_TO_TCHAR(response->notice().c_str());
//		}
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildMemberCandidatesResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildMemberCandidatesResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetGuildMemberCandidates>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto candidate : response->candidates())
//			{
//				b2->candidates.Emplace(ToB2GuildMemberCandidate(candidate));
//			}
//
//			if (response->has_need_friend_list()) 
//			{
//				b2->need_friend_list = response->need_friend_list();
//			}
//			else
//			{
//				b2->need_friend_list = false;
//			}
//
//			for (auto friendPb : response->friends())
//			{
//				b2->friends.Emplace(FriendPb2B2Friend(friendPb));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromInviteGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::InviteGuildResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseInviteGuild>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildInviteListResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildInviteListResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetGuildInviteList>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto invite : response->invites())
//			{
//				b2->invites.Emplace(ToB2GuildInvite(invite));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRejectInviteGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RejectInviteGuildResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseRejectInviteGuild>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAcceptInviteGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AcceptInviteGuildResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseAcceptInviteGuild>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFindGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FindGuildResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseFindGuild>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->guild = ToB2Guild(response->guild());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromObtainGuildMasterResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ObtainGuildMasterResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseObtainGuildMaster>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->guild = ToB2Guild(response->guild());
//
//			for (auto guild_member : response->changed_member())
//			{
//				b2->changed_members.Emplace(ToB2GuildMember(guild_member));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromHandOverGuildMasterResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::HandOverGuildMasterResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseHandOverGuildMaster>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->guild = ToB2Guild(response->guild());
//
//			for (auto guild_member : response->changed_member())
//			{
//				b2->changed_members.Emplace(ToB2GuildMember(guild_member));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildRankingListResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildRankingListResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetGuildRankingList>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto guildRanking : response->guild_rankings())
//			{
//				b2->guild_rankings.Emplace(ToB2GuildRanking(guildRanking));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLevelUpGuildSkillResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LevelUpGuildSkillResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseLevelUpGuildSkill>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->skill = ToB2GuildSkill(response->skill());
//			b2->current_gold = response->current_gold();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuyGuildSkillResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuyGuildSkillResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseBuyGuildSkill>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->skill = ToB2GuildSkill(response->skill());
//			b2->current_gold = response->current_gold();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetWingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetWingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetWing>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess()) 
//		{
//			B2MessageInfoResponse->character_type = response->character_type();
//			B2MessageInfoResponse->grade = response->grade();
//			B2MessageInfoResponse->level = response->level();
//			B2MessageInfoResponse->enhance_point = response->enhance_point();
//			B2MessageInfoResponse->upgrade_fail_point = response->upgrade_fail_point();
//
//			for (auto wingInfo : response->wing_option_infos())
//			{
//				B2MessageInfoResponse->wing_option_infos.Emplace(ToB2WingOptionInfo(wingInfo));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceWingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceWingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceWing>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character_type = response->character_type();
//			B2MessageInfoResponse->level = response->level();
//			B2MessageInfoResponse->enhance_point = response->enhance_point();
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->material_item_count = response->material_item_count();
//
//			for (auto itemInfo : response->changed_material_items())
//			{
//				auto UpgradeStoneInfo = ItemPb2B2ItemServerInfo(itemInfo);
//				if (UpgradeStoneInfo)
//				{
//					B2MessageInfoResponse->changed_material_items.Emplace(UpgradeStoneInfo);
//				}
//			}
//
//			int DeletedItemCount = response->deleted_item_ids_size();
//			for (int i = 0; i < DeletedItemCount; ++i)
//			{
//				B2MessageInfoResponse->deleted_item_ids.Emplace(response->deleted_item_ids(i));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpgradeWingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpgradeWingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUpgradeWing>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character_type = response->character_type();
//			B2MessageInfoResponse->isUpgradeSuccess = response->isupgradesuccess();
//			B2MessageInfoResponse->level = response->level();
//			B2MessageInfoResponse->grade = response->grade();
//			B2MessageInfoResponse->current_money = response->current_money();
//			B2MessageInfoResponse->fail_point = response->fail_point();
//			B2MessageInfoResponse->material_item_count = response->material_item_count();
//
//			for (auto itemInfo : response->changed_material_items())
//			{
//				auto UpgradeStoneInfo = ItemPb2B2ItemServerInfo(itemInfo);
//				if (UpgradeStoneInfo)
//				{
//					B2MessageInfoResponse->changed_material_items.Emplace(UpgradeStoneInfo);
//				}
//			}
//
//			int DeletedItemCount = response->deleted_item_ids_size();
//			for (int i = 0; i < DeletedItemCount; ++i)
//			{
//				B2MessageInfoResponse->deleted_item_ids.Emplace(response->deleted_item_ids(i));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceWingOptionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceWingOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceWingOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->character_type = response->character_type();
//			B2MessageInfoResponse->enhanced_wing_option = ToB2WingOptionInfo(response->enhanced_wing_option());
//			B2MessageInfoResponse->material_item_count = response->material_item_count();
//
//			for (auto itemInfo : response->changed_material_items())
//			{
//				auto UpgradeStoneInfo = ItemPb2B2ItemServerInfo(itemInfo);
//				if (UpgradeStoneInfo)
//				{
//					B2MessageInfoResponse->changed_material_items.Emplace(UpgradeStoneInfo);
//				}
//			}
//
//			int DeletedItemCount = response->deleted_item_ids_size();
//			for (int i = 0; i < DeletedItemCount; ++i)
//			{
//				B2MessageInfoResponse->deleted_item_ids.Emplace(response->deleted_item_ids(i));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetDonationEventResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetDonationEventResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetDonationEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		B2MessageInfoResponse->donation_id = response->donation_id();
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->free_donation_count = response->free_donation_count();
//			B2MessageInfoResponse->point = response->point();
//			B2MessageInfoResponse->whole_step = response->whole_step();
//
//			for (auto rewardState : response->point_reward_state())
//			{
//				B2MessageInfoResponse->point_reward_state.Emplace(ToB2DonationPointRewardState(rewardState));
//			}
//
//			B2MessageInfoResponse->settled = response->settled();
//
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//
//			B2MessageInfoResponse->from_ranking = response->from_ranking();
//			B2MessageInfoResponse->total_ranker_count = response->total_ranker_count();
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMakeDonationResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::MakeDonationResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseMakeDonation>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->is_free_donation = response->is_free_donation();
//			B2MessageInfoResponse->free_donation_count = response->free_donation_count();
//
//			for (auto present : response->donation_presents())
//			{
//				B2MessageInfoResponse->donation_presents.Emplace(ToB2DonationPresent(present));
//			}
//
//			B2MessageInfoResponse->decrease_gem = response->decrease_gem();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->decrease_gold = response->decrease_gold();
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->obtain_point = response->obtain_point();
//			B2MessageInfoResponse->current_point = response->current_point();
//			B2MessageInfoResponse->last_donation_key = ANSI_TO_TCHAR(response->last_donation_key().c_str());
//			B2MessageInfoResponse->bundled_fixed_reward = ToB2RawReward(response->bundled_fixed_reward());
//
//			for (auto item : response->items())
//			{
//				B2MessageInfoResponse->items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//
//			B2MessageInfoResponse->present_gem = response->present_gem();
//			B2MessageInfoResponse->present_blade_point = response->present_blade_point();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDonationPointRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DonationPointRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDonationPointReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2RawReward(reward));
//			}
//
//			for (auto rewardState : response->point_reward_state())
//			{
//				B2MessageInfoResponse->point_reward_state.Emplace(ToB2DonationPointRewardState(rewardState));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetFairyResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetFairyResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetFairy>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto fairy : response->fairies())
//			{
//				B2MessageInfoResponse->fairies.Emplace(ToB2FairyStatus(fairy));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLevelupFairyResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LevelupFairyResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLevelupFairy>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->updated_fairy = ToB2FairyStatus(response->updated_fairy());
//			B2MessageInfoResponse->changed_material_item = ItemPb2B2ItemServerInfo(response->changed_material_item());
//			B2MessageInfoResponse->deleted_item_id = response->deleted_item_id();
//			B2MessageInfoResponse->is_levelup = response->is_levelup();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBlessFairyResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BlessFairyResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBlessFairy>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->updated_fairy = ToB2FairyStatus(response->updated_fairy());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveFairyGiftResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveFairyGiftResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveFairyGift>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->jackpoted = response->jackpoted();
//			B2MessageInfoResponse->multiple_value = response->multiple_value();
//			B2MessageInfoResponse->fairy_type = ToB2FairyType(response->fairy_type());
//			B2MessageInfoResponse->next_fairy_gift_time = response->next_fairy_gift_time();
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetHeroTowerResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetHeroTower>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->start_floor = response->start_floor();
//			b2->max_clear_floor = response->max_clear_floor();
//			b2->main_character_type = PbCharacter2B2CharacterType(response->main_character_type());
//			b2->tag_character_type = PbCharacter2B2CharacterType(response->tag_character_type());
//			b2->daily_try_count = response->daily_try_count();
//			b2->current_free_ticket = response->current_free_ticket();
//			b2->current_buy_ticket = response->current_buy_ticket();
//			b2->possible_try_count = response->possible_try_count();
//			b2->current_hero_piece = ItemPb2B2ItemServerInfo(response->current_hero_piece());
//
//			for (auto relic : response->relics())
//			{
//				b2->relics.Emplace(ToB2Relic(relic));
//			}
//
//			b2->is_sweep = response->is_sweep();
//
//			for (auto ranker : response->rankers())
//			{
//				b2->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//
//			b2->from_ranking = response->from_ranking();
//			b2->total_ranker_count = response->total_ranker_count();
//			b2->ranking = RankingPb2B2Ranking(response->ranking());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromStartHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::StartHeroTowerResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseStartHeroTower>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->play_token = ANSI_TO_TCHAR(response->play_token().c_str());
//			b2->left_free_ticket = response->left_free_ticket();
//			b2->daily_enter_count = response->daily_enter_count();
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				b2->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFinishHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FinishHeroTowerResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseFinishHeroTower>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				b2->rewards.Emplace(ToB2Reward(reward));
//			}
//
//			b2->next_floor = response->next_floor();
//			b2->play_token = ANSI_TO_TCHAR(response->play_token().c_str());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAbandonHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AbandonHeroTowerResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseAbandonHeroTower>();
//		b2->SetErrorCode(response->result().error_code());
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceRelicResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceRelicResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseEnhanceRelic>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->relic = ToB2Relic(response->relic());
//			
//			if (b2->IsSuccess())
//			{
//				b2->relic = ToB2Relic(response->relic());
//
//				int ChangedHeroPieceItemCount = response->changed_hero_piece_items_size();
//				for (int i = 0; i < ChangedHeroPieceItemCount; ++i)
//				{
//					auto HeroPieceInfo = ItemPb2B2ItemServerInfo(response->changed_hero_piece_items(i));
//					if (HeroPieceInfo)
//					{
//						b2->changed_hero_piece_items.Emplace(HeroPieceInfo);
//					}
//				}
//
//				int DeletedHeroPieceItemIdCount = response->deleted_hero_piece_item_ids_size();
//				for (int i = 0; i < DeletedHeroPieceItemIdCount; ++i)
//				{
//					int64 DeletedHeroPieceItemId = response->deleted_hero_piece_item_ids(i);
//					b2->deleted_hero_piece_item_ids.Emplace(DeletedHeroPieceItemId);
//				}
//
//				b2->current_money = response->current_money();
//			}
//
//			b2->current_money = response->current_money();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPromotionRelicResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PromotionRelicResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponsePromotionRelic>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->relic = ToB2Relic(response->relic());
//
//			int ChangedBossPieceItemCount = response->changed_boss_piece_items_size();
//			for (int i = 0; i < ChangedBossPieceItemCount; ++i)
//			{
//				auto BossPieceInfo = ItemPb2B2ItemServerInfo(response->changed_boss_piece_items(i));
//				if (BossPieceInfo)
//				{
//					b2->changed_boss_piece_items.Emplace(BossPieceInfo);
//				}
//			}
//
//			int DeletedBossPieceItemIdCount = response->deleted_boss_piece_item_ids_size();
//			for (int i = 0; i < DeletedBossPieceItemIdCount; ++i)
//			{
//				int64 DeletedBossPieceItemId = response->deleted_boss_piece_item_ids(i);
//				b2->deleted_boss_piece_item_ids.Emplace(DeletedBossPieceItemId);
//			}
//
//			b2->current_money = response->current_money();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAccountRelicResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAccountRelicResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetAccountRelic>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			int relicCount = response->relics_size();
//			for (int i = 0; i < relicCount; ++i)
//			{
//				auto relic = ToB2Relic(response->relics(i));
//				if (relic)
//				{
//					b2->relics.Emplace(relic);
//				}
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSweepHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SweepHeroTowerResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseSweepHeroTower>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				b2->rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuyHeroTowerTicketResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuyHeroTowerTicketResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseBuyHeroTowerTicket>();
//		b2->SetErrorCode(response->result().error_code());
//
//		if (b2->IsSuccess())
//		{
//			b2->current_cash = response->current_cash();
//			b2->current_free_ticket = response->current_free_ticket();
//			b2->current_buy_ticket = response->current_buy_ticket();
//			b2->possible_try_count = response->possible_try_count();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mission_info : response->mission_infos())
//			{
//				B2MessageInfoResponse->mission_infos.Emplace(ToB2MissionInfo(mission_info));
//			}
//
//			for (auto mission_complete_point_info : response->mission_complete_point_infos())
//			{
//				B2MessageInfoResponse->mission_complete_point_infos.Emplace(ToB2MissionCompletePointInfo(mission_complete_point_info));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetFeaturedMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetFeaturedMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetFeaturedMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mission_info : response->mission_infos())
//			{
//				B2MessageInfoResponse->mission_infos.Emplace(ToB2MissionInfo(mission_info));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveMissionRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveMissionRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveMissionReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->received_mission_info = ToB2MissionInfo(response->received_mission_info());
//
//			for (b2::protocol::commondata::Reward pbReward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveCompleteMissionRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveCompleteMissionRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveCompleteMissionReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->received_mission_type = PbMissionTypeToB2MissionType(response->received_mission_type());
//			B2MessageInfoResponse->received_reward_index = response->received_reward_index();
//
//			for (b2::protocol::commondata::Reward pbReward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetSeasonMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetSeasonMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetSeasonMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mission : response->missions()) 
//			{
//				B2MessageInfoResponse->missions.Emplace(ToB2SeasonMissionInfo(mission));
//			}
//
//			for (auto status : response->event_statuses()) 
//			{
//				B2MessageInfoResponse->event_statuses.Emplace(ToB2SeasonMissionEventStatus(status));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardSeasonMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardSeasonMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardSeasonMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			if (response->has_reward_mission())
//			{
//				B2MessageInfoResponse->reward_mission = ToB2SeasonMissionInfo(response->reward_mission());
//			}
//
//			if (response->has_reward()) 
//			{
//				B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAttendSeasonMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AttendSeasonMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAttendSeasonMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto status : response->event_statuses()) 
//			{
//				B2MessageInfoResponse->event_statuses.Emplace(ToB2SeasonMissionEventStatus(status));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDrawShopLotteryResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::DrawShopLotteryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDrawShopLottery>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->free_lottery = response->free_lottery();
//
//			for (b2::protocol::commondata::Item item : response->items())
//			{
//				auto itemServerInfo = ItemPb2B2ItemServerInfo(item);
//				B2MessageInfoResponse->items.Emplace(itemServerInfo);
//			}
//
//			B2MessageInfoResponse->shop_ten_lottery_ticket = response->shop_ten_lottery_ticket();
//			B2MessageInfoResponse->gem = response->gem();
//			B2MessageInfoResponse->social_point = response->social_point();
//			B2MessageInfoResponse->mileage = response->mileage();
//			B2MessageInfoResponse->next_free_time = response->next_free_time();
//			B2MessageInfoResponse->daily_lottery_count = response->daily_lottery_count();
//			B2MessageInfoResponse->last_lottery_key = ANSI_TO_TCHAR(response->last_lottery_key().c_str());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardShopMileageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardShopMileageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardShopMileage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->shop_mileage_type = PbShopMileageTypeToB2ShopMileageType(response->shop_mileage_type());
//
//			for (auto pbReward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//			}
//
//			B2MessageInfoResponse->received_reward_index = response->received_reward_index();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetLotteryShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetLotteryShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetLotteryShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->mileage = response->mileage();
//			B2MessageInfoResponse->reward_index = response->reward_index();
//			B2MessageInfoResponse->shop_ten_lottery_ticket = response->shop_ten_lottery_ticket();
//
//			for (auto lottery : response->shop_lotteries())
//			{
//				B2MessageInfoResponse->shop_lotteries.Emplace(ToB2ShopLottery(lottery));
//			}			
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuyShopProductResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuyShopProductResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBuyShopProduct>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->buy_price_type = PbShopBuyPriceTypeToB2ShopBuyPriceType(response->buy_price_type());
//			B2MessageInfoResponse->price = response->price();
//			B2MessageInfoResponse->buy_result_info = ToB2BuyShopResultInfo(response->buy_result_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPurchaseCostumeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PurchaseCostumeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePurchaseCostume>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->buy_price_type = PbShopBuyPriceTypeToB2ShopBuyPriceType(response->buy_price_type());
//			B2MessageInfoResponse->price = response->price();
//			
//			int costumeCount = response->costumes_size();
//			for (int i = 0; i < costumeCount; ++i)
//			{
//				const b2::protocol::commondata::Costume& costume = response->costumes(i);
//
//				auto b2Costume = CostumePb2B2CostumeServerInfo(costume);
//				if (b2Costume)
//				{
//					B2MessageInfoResponse->costumes.Emplace(b2Costume);
//				}
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGeneralShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGeneralShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetGeneralShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mileage : response->shop_mileages())
//			{
//				B2MessageInfoResponse->shop_mileages.Emplace(ToB2ShopMileage(mileage));
//			}
//
//			for (auto buyHistory : response->shop_buy_manages())
//			{
//				B2MessageInfoResponse->shop_buy_manages.Emplace(ToB2ShopBuyManage(buyHistory));
//			}
//
//			for (auto mark : response->shop_product_marks())
//			{
//				B2MessageInfoResponse->shop_product_marks.Emplace(ToB2ShopProductMark(mark));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetCostumeShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetCostumeShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetCostumeShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto buyHistory : response->shop_buy_manages())
//			{
//				B2MessageInfoResponse->shop_buy_manages.Emplace(ToB2ShopBuyManage(buyHistory));
//			}
//
//			for (auto mark : response->shop_product_marks())
//			{
//				B2MessageInfoResponse->shop_product_marks.Emplace(ToB2ShopProductMark(mark));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetMagicShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetMagicShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetMagicShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->renewal_count = response->renewal_count();
//			B2MessageInfoResponse->next_free_renewal_time = response->next_free_renewal_time();
//			B2MessageInfoResponse->is_free_renewal = response->is_free_renewal();
//
//			for (auto magicShop : response->magic_shops())
//			{
//				B2MessageInfoResponse->magic_shops.Emplace(ToB2MagicShop(magicShop));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRenewalMagicShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RenewalMagicShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRenewalMagicShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		
//		if (B2MessageInfoResponse->IsSuccess())
//		{	
//			B2MessageInfoResponse->current_gem = response->current_gem();
//		}
//
//		B2MessageInfoResponse->renewal_count = response->renewal_count();
//		B2MessageInfoResponse->next_free_renewal_time = response->next_free_renewal_time();
//		B2MessageInfoResponse->is_free_renewal = response->is_free_renewal();
//		for (auto magicShop : response->magic_shops())
//		{
//			B2MessageInfoResponse->magic_shops.Emplace(ToB2MagicShop(magicShop));
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPurchaseMagicShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PurchaseMagicShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePurchaseMagicShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{	
//			B2MessageInfoResponse->buy_price_type = PbShopBuyPriceTypeToB2ShopBuyPriceType(response->buy_price_type());
//			B2MessageInfoResponse->price = response->price();
//		}
//
//		B2MessageInfoResponse->is_free_renewal = response->is_free_renewal();
//		B2MessageInfoResponse->renewal_count = response->renewal_count();
//		B2MessageInfoResponse->next_free_renewal_time = response->next_free_renewal_time();
//		for (auto magicShop : response->magic_shops())
//		{
//			B2MessageInfoResponse->magic_shops.Emplace(ToB2MagicShop(magicShop));
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetCollectionItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetCollectionItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetCollectionItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto item : response->collection_items())
//			{
//				B2MessageInfoResponse->collection_items.Emplace(ToB2CollectionItem(item));
//			}
//
//			for (auto item : response->reward_collection_set_items())
//			{
//				B2MessageInfoResponse->reward_collection_items.Emplace(ToB2CollectionSetItem(item));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardCollectionItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardCollectionItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardCollectionItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->item_template_id = response->item_template_id();
//			B2MessageInfoResponse->received_reward = ToB2Reward(response->received_reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardCollectionSetItemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardCollectionSetItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardCollectionSetItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->gropup_id = response->group_id();
//			B2MessageInfoResponse->grade = response->grade();
//			B2MessageInfoResponse->character_type = PbCharacter2B2CharacterType(response->character_type());
//			B2MessageInfoResponse->received_reward = ToB2Reward(response->received_reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetMarketProductResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetMarketProductResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetMarketProduct>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mdMarketProduct : response->md_market_products())
//			{
//				B2MessageInfoResponse->md_market_products.Emplace(ToB2mdMarketProduct(mdMarketProduct));
//			}
//
//			for (auto purchaseInfo : response->purchase_infos())
//			{
//				B2MessageInfoResponse->purchase_infos.Emplace(ToB2MarketPurchaseInfo(purchaseInfo));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPreparePurchaseResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PreparePurchaseResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePreparePurchase>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_purchase_info())
//		{
//			B2MessageInfoResponse->purchase_info = ToB2MarketPurchaseInfo(response->purchase_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromVerifyPurchaseResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::VerifyPurchaseResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseVerifyPurchase>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_receive_error_code())
//		{
//			B2MessageInfoResponse->receive_error_code = response->receive_error_code();
//		}
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->product_id = response->product_id();
//			B2MessageInfoResponse->market_product_id = UTF8_TO_TCHAR(response->market_product_id().c_str());
//			B2MessageInfoResponse->developer_payload = UTF8_TO_TCHAR(response->developer_payload().c_str());
//
//			B2MessageInfoResponse->buy_result_info = ToB2BuyShopResultInfo(response->buy_result_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCancelPurchaseResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CancelPurchaseResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCancelPurchase>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_developer_payload())
//		{
//			B2MessageInfoResponse->developer_payload = UTF8_TO_TCHAR(response->developer_payload().c_str());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFinishPurchaseResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FinishPurchaseResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseFinishPurchase>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_developer_payload())
//		{
//			B2MessageInfoResponse->developer_payload = UTF8_TO_TCHAR(response->developer_payload().c_str());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRaidResultResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RaidResultResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRaidResult>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto raidReward : response->raid_rewards()) 
//			{
//				B2MessageInfoResponse->raid_rewards.Emplace(ToB2RaidRankingReward(raidReward));
//			}
//
//			for (auto character : response->light_characters()) {
//				auto b2CharacterExp = LightCharacterPb2B2LightCharacter(character);
//				B2MessageInfoResponse->light_characters.Emplace(b2CharacterExp);
//			}
//
//			for (auto item : response->items())
//			{
//				B2MessageInfoResponse->items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//
//			B2MessageInfoResponse->step_clear_reward = ToB2Reward(response->step_clear_reward());
//
//			if (response->has_main_reward()) 
//			{
//				B2MessageInfoResponse->main_reward = ToB2Reward(response->main_reward());
//			}
//
//			if (response->has_main_bonus_reward()) 
//			{
//				B2MessageInfoResponse->main_bonus_reward = ToB2Reward(response->main_bonus_reward());
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDailyAttendanceResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DailyAttendanceResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDailyAttendance>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->is_attendance = response->is_attendance();
//			B2MessageInfoResponse->attendance_day = response->attendance_day();
//			B2MessageInfoResponse->attendance_type = PbDailyAttendanceTypeToB2DailyAttendanceType(response->attendance_type());
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//			if (response->has_next_attendance_time()) 
//			{
//				B2MessageInfoResponse->next_attendance_time = response->next_attendance_time();
//			}
//		}
//
//		return B2MessageInfoResponse;	
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromJoinGuildBattleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::JoinGuildBattleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseJoinGuildBattle>();
//		b2->SetErrorCode(response->result().error_code());
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildBattleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildBattleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetGuildBattle>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->battle_end_time = response->battle_end_time();
//			b2->my_guild = ToB2GuildBattleGuild(response->my_guild());
//			b2->target_guild = ToB2GuildBattleGuild(response->target_guild());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromStartGuildBattleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::StartGuildBattleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseStartGuildBattle>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->battle_token = ANSI_TO_TCHAR(response->battle_token().c_str());
//			b2->target = AccountInfoPb2B2FindAccount(response->target());
//			b2->target_left_medal = response->target_left_medal();
//			b2->my_info = AccountInfoPb2B2FindAccount(response->my_info());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromFinishGuildBattleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::FinishGuildBattleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseFinishGuildBattle>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->battle_result = response->battle_result();
//			b2->obtain_medal = response->obtain_medal();
//
//			for (auto reward : response->rewards())
//			{
//				b2->rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetGuildBattleEntryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetGuildBattleEntryResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseSetGuildBattleEntry>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->entry = ToB2GuildBattleEntry(response->entry());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildBattleTurnResultResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildBattleTurnResultResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGuildBattleTurnResult>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->mvp_account_id = response->mvp_account_id();
//			b2->battle_result = ToB2GuildBattleResultType(response->battle_result());
//			b2->increased_exp = response->increased_exp();
//			b2->my_guild = ToB2GuildBattleGuild(response->my_guild());
//			b2->target_guild = ToB2GuildBattleGuild(response->target_guild());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildBattleTurnRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildBattleTurnRewardResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGuildBattleTurnReward>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->battle_result = ToB2GuildBattleResultType(response->battle_result());
//			b2->guild_medal = response->guild_medal();
//			b2->reward_grade = response->reward_grade();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildBattleSeasonRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildBattleSeasonRewardResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGuildBattleSeasonReward>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->season_ranking = response->season_ranking();
//			b2->season_ranking_total_count = response->season_ranking_total_count();
//			b2->gem = response->gem();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildBattleHistoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildBattleHistoryResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGuildBattleHistory>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto history : response->battle_histories())
//			{
//				b2->battle_histories.Emplace(ToB2GuildBattleHistory(history));
//			}
//
//			b2->season_record = ToB2GuildBattleRecord(response->season_record());
//			b2->best_season_record = ToB2GuildBattleRecord(response->best_season_record());
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetWingVisibleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetWingVisibleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseSetWingVisible>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->character_type = PbCharacter2B2CharacterType(response->character_type());
//			b2->is_visible = response->is_visible();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetHelmetVisibleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetHelmetVisibleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseSetHelmetVisible>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->character_type = PbCharacter2B2CharacterType(response->character_type());
//			b2->is_visible = response->is_visible();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetCostumeVisibleResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetCostumeVisibleResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseSetCostumeVisible>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->character_type = PbCharacter2B2CharacterType(response->character_type());
//			b2->costume_type = PbCostume2B2CostumeType(response->costume_type());
//			b2->is_visible = response->is_visible();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetRaidResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetRaidResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetRaid>();
//		b2->SetErrorCode(response->result().error_code());
//
//		b2->penalty_remove_time = response->penalty_remove_time();
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRaidResurrectionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RaidResurrectionResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseRaidResurrection>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess()) 
//		{
//			b2->resurrection_count = response->resurrection_count();
//			b2->used_gem = response->used_gem();
//			b2->total_gem = response->total_gem();
//			b2->next_resurrection_gem_cost = response->next_resurrection_gem_cost();
//			b2->selected_count_resurrection_buff1 = response->selected_count_resurrection_buff1();
//			b2->selected_count_resurrection_buff2 = response->selected_count_resurrection_buff2();
//			b2->selected_count_resurrection_buff3 = response->selected_count_resurrection_buff3();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAssaultMatchmakingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AssaultMatchmakingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAssaultMatchmaking>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->token = ANSI_TO_TCHAR(response->token().c_str());
//			B2MessageInfoResponse->room_type = response->room_type();
//			B2MessageInfoResponse->channel_endpoint = EndpointPb2B2Endpoint(response->channel_endpoint());
//
//			for (auto guildSkill : response->guild_skills())
//			{
//				B2MessageInfoResponse->guild_skills.Emplace(ToB2GuildSkill(guildSkill));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCancelAssaultMatchmakingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CancelAssaultMatchmakingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCancelAssaultMatchmaking>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAssaultResultResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AssaultResultResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAssaultResult>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->match_status = MatchStatusPb2B2MatchStatus(response->match_status());
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//			
//			if (response->has_prev_ranker())
//			{
//				B2MessageInfoResponse->prev_ranker = MatchRankerPb2B2MatchRanker(response->prev_ranker());
//			}
//
//			B2MessageInfoResponse->increased_mvp_point = response->increased_mvp_point();
//
//			for (auto pbReward : response->victory_rewards())
//			{
//				B2MessageInfoResponse->victory_rewards.Emplace(ToB2Reward(pbReward));
//			}
//
//			 reward box
//			if (response->has_reward_box_status()) 
//			{
//				B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//			}
//			else 
//			{
//				B2MessageInfoResponse->reward_box_status = nullptr;
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAssaultResurrectionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AssaultResurrectionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAssaultResurrection>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->resurrection_count = response->resurrection_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckPvPMatchPointChargeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckPvPMatchPointChargeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckPvPMatchPointCharge>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->match_point = response->match_point();
//			B2MessageInfoResponse->match_point_charge_lefttime_second = response->match_point_charge_lefttime_second();
//		}
//		return B2MessageInfoResponse;
//
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckTeamMatchPointChargeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckTeamMatchPointChargeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckTeamMatchPointCharge>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->match_point = response->match_point();
//			B2MessageInfoResponse->match_point_charge_lefttime_second = response->match_point_charge_lefttime_second();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAddMailResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AddMailResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAddMail>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMakeRankingResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::MakeRankingResponse>(message);
//
//		auto b2Response = std::make_shared<FB2MessageInfoResponseMakeRanking>();
//		b2Response->SetErrorCode(response->result().error_code());
//		return b2Response;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromForceClearStageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ForceClearStageResponse>(message);
//
//		auto b2Response = std::make_shared<FB2MessageInfoResponseForceClearStage>();
//		b2Response->SetErrorCode(response->result().error_code());
//		return b2Response;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetQuestStateResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetQuestStateResponse>(message);
//
//		auto b2Response = std::make_shared<FB2MessageInfoResponseSetQuestState>();
//		b2Response->SetErrorCode(response->result().error_code());
//		return b2Response;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromIncreaseHeroPieceResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::IncreaseHeroPieceResponse>(message);
//
//		auto b2Response = std::make_shared<FB2MessageInfoResponseIncreaseHeroPiece>();
//		b2Response->SetErrorCode(response->result().error_code());
//		if (b2Response->IsSuccess())
//		{
//			b2Response->current_hero_piece = response->current_hero_piece();
//		}
//		return b2Response;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromForceClearHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ForceClearHeroTowerResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseForceClearHeroTower>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->start_floor = response->start_floor();
//			b2->max_clear_floor = response->max_clear_floor();
//			b2->main_character_type = PbCharacter2B2CharacterType(response->main_character_type());
//			b2->tag_character_type = PbCharacter2B2CharacterType(response->tag_character_type());
//			b2->daily_try_count = response->daily_try_count();
//			b2->daily_max_free_try_count = response->daily_max_free_try_count();
//			b2->daily_left_free_try_count = response->daily_left_free_try_count();
//			b2->current_hero_piece = response->current_hero_piece();
//			b2->is_sweep = response->is_sweep();
//		}
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromForceStartCounterDungeonResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ForceStartCounterDungeonResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseForceStartCounterDungeon>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->play_token = ANSI_TO_TCHAR(response->play_token().c_str());
//			B2MessageInfoResponse->drop_gold = response->drop_gold();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromIncreaseSocialPointResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::IncreaseSocialPointResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseIncreaseSocialPoint>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_social_point = response->current_social_point();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPrepareSweepHeroTowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PrepareSweepHeroTowerResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePrepareSweepHeroTower>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPrepareGuildResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PrepareGuildResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePrepareGuild>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetGuildBattleStateResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetGuildBattleStateResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetGuildBattleState>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		B2MessageInfoResponse->state = ToB2GuildBattleState(response->state());
//		B2MessageInfoResponse->state_message = UTF8_TO_TCHAR(response->state_message().c_str());
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromResetAttendanceTimeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ResetAttendanceTimeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseResetAttendanceTime>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromResetEventAttendanceResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ResetEventAttendanceResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseResetEventAttendance>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromResetDailyPlayTimeRecordResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ResetDailyPlayTimeRecordResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseResetDailyPlayTimeRecord>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->daily_play_time_in_sec = response->daily_play_time_in_sec();
//			B2MessageInfoResponse->daily_play_time_reward_index = response->daily_play_time_reward_index();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromResetTeamMatchPointResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ResetTeamMatchPointResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseResetTeamMatchPoint>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromChangeQuestIdResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ChangeQuestIdResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseChangeQuestId>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromChangeQuestCountResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ChangeQuestIdResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseChangeQuestId>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPrepareFriendResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PrepareFriendResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePrepareFriend>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->new_friend_count = response->new_friend_count();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetGuildLevelExpResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetGuildLevelExpResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetGuildLevelExp>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->guild = ToB2Guild(response->guild());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAddAetherToInventoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AddAetherToInventoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAddAetherToInventory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->aether = AetherPb2B2AetherServerInfo(response->aether());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromForceUpdateAetherResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ForceUpdateAetherResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseForceUpdateAether>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->updated_aether = AetherPb2B2AetherServerInfo(response->updated_aether());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetMatchSeasonRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetMatchSeasonRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetMatchSeasonReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetStageBoostCountResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetStageBoostCountResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetStageBoostCount>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->stage_boost_ticket = response->stage_boost_ticket();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSkipMatchTutorialResponse(MessagePtr message)
//	{
//        auto response = std::static_pointer_cast<b2::protocol::session::SkipMatchTutorialResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSkipMatchTutorial>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//        return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromServerCmdResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ServerCmdResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseServerCmd>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->result_message = UTF8_TO_TCHAR(response->result_message().c_str());
//			B2MessageInfoResponse->need_restart = response->need_restart();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetDebugModeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SetDebugModeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetDebugMode>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetRoomDebugModeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::channel::SetRoomDebugModeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetRoomDebugMode>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromTraceResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::TraceResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseTrace>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->tag = ANSI_TO_TCHAR(response->tag().c_str());
//			b2->client_send_time = response->client_send_time();
//			b2->session_recv_time = response->session_recv_time();
//			b2->app_recv_time = response->app_recv_time();
//			b2->db_request_time = response->db_request_time();
//			b2->db_response_time = response->db_response_time();
//			b2->app_send_time = response->app_send_time();
//			b2->session_send_time = response->session_send_time();
//
//			auto now = std::chrono::steady_clock::now().time_since_epoch();
//			auto ts = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
//		}
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGiveNicknameResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::GiveNicknameResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGiveNickname>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//        return B2MessageInfoResponse;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpdateNicknameResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpdateNicknameResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseUpdateNickname>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			b2->old_nickname = UTF8_TO_TCHAR(response->old_nickname().c_str());
//			b2->new_nickname = UTF8_TO_TCHAR(response->new_nickname().c_str());
//
//			if (response->has_updated_item()) {
//				b2->updated_item = ItemPb2B2ItemServerInfo(response->updated_item());
//			}
//			else {
//				b2->updated_item = nullptr;
//			}
//
//			if (response->has_deleted_item_id()) {
//				b2->deleted_item_id = response->deleted_item_id();
//			}
//			else {
//				b2->deleted_item_id = 0;
//			}
//		}
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetServerVersionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetServerVersionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetServerVersion>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->version = UTF8_TO_TCHAR(response->version().c_str());
//			B2MessageInfoResponse->stage = UTF8_TO_TCHAR(response->stage().c_str());
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGiveCharacterNameResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::GiveCharacterNameResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGiveCharacterName>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromGetChatChannelResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::GetChatChannelResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoGetChatChannelResponse>();
//        B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//            B2MessageInfoResponse->channel_number = response->channel_number();
//        }
//        return B2MessageInfoResponse;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromChangeChatChannelResponse(MessagePtr message)
//    {
//        auto response = std::static_pointer_cast<b2::protocol::session::ChangeChatChannelResponse>(message);
//
//        auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoChangeChatChannelResponse>();
//        if (B2MessageInfoResponse->IsSuccess())
//        {
//        }
//        return B2MessageInfoResponse;
//    }
//
//    FB2MessageInfoPtr FB2MessageInfoConverter::FromChatNotify(MessagePtr message)
//    {
//        auto notify = std::static_pointer_cast<b2::protocol::session::ChatNotify>(message);
//
//        auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoChatNotify>();
//        B2MessageInfoNotify->type = notify->type();
//		B2MessageInfoNotify->from = UTF8_TO_TCHAR(notify->from().c_str());
//        B2MessageInfoNotify->format = notify->format();
//		B2MessageInfoNotify->body = UTF8_TO_TCHAR(notify->body().c_str());
//		B2MessageInfoNotify->guild = ToB2ChatGuild(notify->guild());
//		B2MessageInfoNotify->notice_type = notify->notice_type();
//		for (auto arg : notify->notice_args()) 
//		{
//			B2MessageInfoNotify->notice_args.Emplace(UTF8_TO_TCHAR(arg.c_str()));
//		}
//		B2MessageInfoNotify->account_id = notify->account_id();
//		B2MessageInfoNotify->character_level_max = notify->character_level_max();
//        return B2MessageInfoNotify;
//    }
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromQuestAlarmNotify(MessagePtr message)
//	{
//		auto notify = std::static_pointer_cast<b2::protocol::session::QuestAlarmNotify>(message);
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoQuestAlarmNotify>();
//
//		B2MessageInfoResponse->completed_slot = PbQuestSlotTypeToB2QuestSlotType(notify->completed_slot());
//		if (notify->has_next_quest_info())
//		{
//			B2MessageInfoResponse->next_quest_info = QuestInfoPb2B2QuestInfo(notify->next_quest_info());
//		}
//		for (auto pbReward : notify->received_rewards())
//		{
//			B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(pbReward));
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMissionAlarmNotify(MessagePtr message)
//	{
//		auto notify = std::static_pointer_cast<b2::protocol::session::MissionAlarmNotify>(message);
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoMissionAlarmNotify>();
//		if (notify->has_completed_quest_id())
//		{
//			B2MessageInfoResponse->completed_quest_id = notify->completed_quest_id();
//		}
//
//		if (notify->has_completed_quest_complete_step())
//		{
//			B2MessageInfoResponse->completed_quest_complete_step = notify->completed_quest_complete_step();
//		}
//		
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAssaultBattleStatusResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAssaultBattleStatusResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAssaultBattleStatus>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//			B2MessageInfoResponse->from_ranking = response->from_ranking();
//			B2MessageInfoResponse->total_ranker_count = response->total_ranker_count();
//
//			B2MessageInfoResponse->match_status = MatchStatusPb2B2MatchStatus(response->match_status());
//			B2MessageInfoResponse->ranking = RankingPb2B2Ranking(response->ranking());
//
//			B2MessageInfoResponse->battle_character_type = PbCharacter2B2CharacterType(response->battle_character_type());
//			B2MessageInfoResponse->total_power = response->total_power();
//
//			if (response->has_ranking_reward())
//			{
//				B2MessageInfoResponse->ranking_reward = RankingRewardPb2B2RankingReward(response->ranking_reward());
//			}
//			else 
//			{
//				B2MessageInfoResponse->ranking_reward = nullptr;
//			}
//
//			B2MessageInfoResponse->settlement_time = response->settlement_time();
//			B2MessageInfoResponse->settlement_period_hour = response->settlement_period_hour();
//
//			 daily reward
//			if (response->has_today_reward()) {
//				B2MessageInfoResponse->today_reward = response->today_reward();
//			}
//			else {
//				B2MessageInfoResponse->today_reward = false;
//			}
//			if (response->has_last_reward()) {
//				B2MessageInfoResponse->last_reward = response->last_reward();
//			}
//			else {
//				B2MessageInfoResponse->last_reward = false;
//			}
//			B2MessageInfoResponse->today_participation_count = response->today_participation_count();
//			if (response->has_current_gold()) {
//				B2MessageInfoResponse->current_gold = response->current_gold();
//			}
//			else {
//				B2MessageInfoResponse->current_gold = 0;
//			}
//			if (response->has_current_bladepoint()) {
//				B2MessageInfoResponse->current_bladepoint = response->current_bladepoint();
//			}
//			else {
//				B2MessageInfoResponse->current_bladepoint = 0;
//			}
//			if (response->has_current_gem()) {
//				B2MessageInfoResponse->current_gem = response->current_gem();
//			}
//			else {
//				B2MessageInfoResponse->current_gem = 0;
//			}
//
//			 reward box
//			if (response->has_reward_box_status()) {
//				B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//			}
//			else {
//				B2MessageInfoResponse->reward_box_status = nullptr;
//			}
//
//			if (response->has_mvp_point()) {
//				B2MessageInfoResponse->mvp_point = response->mvp_point();
//			}
//			else {
//				B2MessageInfoResponse->mvp_point = 0;
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetFriendRankingsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetFriendRankingsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetFriendRankings>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		B2MessageInfoResponse->opponent_type = PbMatchInfoType2B2MatchInfoType(response->opponent_type());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(MatchRankerPb2B2MatchRanker(ranker));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetItemRandomOptionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetItemRandomOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetItemRandomOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->item_id = response->item_id();
//			B2MessageInfoResponse->random_option_id1 = response->random_option_id1();
//			B2MessageInfoResponse->random_option_id2 = response->random_option_id2();
//			B2MessageInfoResponse->random_option_id3 = response->random_option_id3();
//			B2MessageInfoResponse->random_option_value1 = response->random_option_value1();
//			B2MessageInfoResponse->random_option_value2 = response->random_option_value2();
//			B2MessageInfoResponse->random_option_value3 = response->random_option_value3();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGenerateItemRandomOptionResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GenerateItemRandomOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGenerateItemRandomOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_dark_crystal = response->current_dark_crystal();
//			B2MessageInfoResponse->current_cash = response->current_cash();
//			B2MessageInfoResponse->random_option_id1 = response->random_option_id1();
//			B2MessageInfoResponse->random_option_id2 = response->random_option_id2();
//			B2MessageInfoResponse->random_option_id3 = response->random_option_id3();
//			B2MessageInfoResponse->random_option_value1 = response->random_option_value1();
//			B2MessageInfoResponse->random_option_value2 = response->random_option_value2();
//			B2MessageInfoResponse->random_option_value3 = response->random_option_value3();
//		}
//
//		return B2MessageInfoResponse;
//	}
//	
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromApplyItemRandomOptionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ApplyItemRandomOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseApplyItemRandomOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO APPLYITEMRANDOMOPTION: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->item_data = ItemPb2B2ItemServerInfo(response->item_data());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDiscardItemRandomOptionResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::DiscardItemRandomOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDiscardItemRandomOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO DISCARDITEMRANDOMOPTION: ADD INTERNAL HANDLER
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetItemRandomQualityResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetItemRandomQualityResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetItemRandomQuality>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{			
//			B2MessageInfoResponse->item_id = response->item_id();
//			B2MessageInfoResponse->quality = response->quality();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGenerateItemRandomQualityResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GenerateItemRandomQualityResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGenerateItemRandomQuality>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->main_item_id = response->main_item_id();
//			B2MessageInfoResponse->material_item_id = response->material_item_id();
//			B2MessageInfoResponse->quality = response->quality();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromApplyItemRandomQualityResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::ApplyItemRandomQualityResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseApplyItemRandomQuality>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->main_item = ItemPb2B2ItemServerInfo(response->main_item());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDiscardItemRandomQualityResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::DiscardItemRandomQualityResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDiscardItemRandomQuality>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO DISCARDITEMRANDOMOPTION: ADD INTERNAL HANDLER
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetFactoryResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetFactoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetFactory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto status : response->factory_periodic_status())
//			{
//				B2MessageInfoResponse->factory_periodic_status.Emplace(FactoryPeriodicStatusPb2B2FactoryPeriodicStatus(status));
//			}
//			
//			for (auto status : response->limit_status())
//			{
//				B2MessageInfoResponse->limit_status.Emplace(FactoryLimitStatusPb2B2FactoryLimitStatus(status));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMakeFactoryItemResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::MakeFactoryItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseMakeFactoryItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->factory_id = response->factory_id();
//			B2MessageInfoResponse->main_item_id = response->main_item_id();
//			B2MessageInfoResponse->material_item_id1 = response->material_item_id1();
//			B2MessageInfoResponse->material_item_id2 = response->material_item_id2();
//			B2MessageInfoResponse->material_item_id3 = response->material_item_id3();
//			B2MessageInfoResponse->material_item_id4 = response->material_item_id4();
//
//			for (auto result : response->make_result()) {
//				B2MessageInfoResponse->make_result.Emplace(FactoryResultPb2B2FactoryResult(result));
//			}
//
//			B2MessageInfoResponse->left_main_item_count = response->left_main_item_count();
//			B2MessageInfoResponse->left_material_item_count1 = response->left_material_item_count1();
//			B2MessageInfoResponse->left_material_item_count2 = response->left_material_item_count2();
//			B2MessageInfoResponse->left_material_item_count3 = response->left_material_item_count3();
//			B2MessageInfoResponse->left_material_item_count4 = response->left_material_item_count4();
//
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->current_dark_crystal = response->current_dark_crystal();
//			B2MessageInfoResponse->cost_gold = response->cost_gold();
//			B2MessageInfoResponse->cost_gem = response->cost_gem();
//			B2MessageInfoResponse->cost_dark_crystal = response->cost_dark_crystal();
//			B2MessageInfoResponse->left_count = response->left_count();
//		}	
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnsealBoxResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnsealBoxResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnsealBox>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->seal_box_id = response->seal_box_id();
//			B2MessageInfoResponse->left_amount = response->left_amount();
//			B2MessageInfoResponse->gold = response->gold();
//			B2MessageInfoResponse->gem = response->gem();
//			B2MessageInfoResponse->blade_point = response->blade_point();
//			B2MessageInfoResponse->social_point = response->social_point();
//			B2MessageInfoResponse->dark_crystal = response->dark_crystal();
//			B2MessageInfoResponse->stage_boost_ticket = response->stage_boost_ticket();
//			B2MessageInfoResponse->stage_auto_clear_ticket = response->stage_auto_clear_ticket();
//			B2MessageInfoResponse->shop_ten_lottery_ticket = response->shop_ten_lottery_ticket();
//			B2MessageInfoResponse->fame_point = response->fame_point();
//			
//			for (auto result : response->rewards()) {
//				B2MessageInfoResponse->rewards.Emplace(UnsealBoxRewardPb2B2UnsealBoxReward(result));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckRedDotResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckRedDotResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckRedDot>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO CheckRedDot: ADD INTERNAL HANDLER
//			 result of hint RED_DOT_EQUIP
//			B2MessageInfoResponse->is_valid_equip = response->has_has_new_equipment();
//			B2MessageInfoResponse->has_new_equipment = response->has_new_equipment();
//			B2MessageInfoResponse->has_new_aether = response->has_new_aether();
//			B2MessageInfoResponse->has_new_costume = response->has_new_costume();
//			B2MessageInfoResponse->has_new_totem = response->has_new_totem();
//
//			 result of hint RED_DOT_COLLECTION
//			B2MessageInfoResponse->is_valid_collection = response->has_has_new_collection();
//			B2MessageInfoResponse->has_new_collection = response->has_new_collection();
//
//			 result of hint RED_DOT_MISSION
//			B2MessageInfoResponse->is_valid_mission = response->has_has_mission_to_reward();
//			B2MessageInfoResponse->has_mission_to_reward = response->has_mission_to_reward();
//			B2MessageInfoResponse->has_mission_mileage_reward = response->has_mission_mileage_reward();
//			B2MessageInfoResponse->has_serial_mission_to_reward = response->has_serial_mission_to_reward();
//
//			 result of hint RED_DOT_FRIEND
//			B2MessageInfoResponse->is_valid_friend = response->has_has_new_friend();
//			B2MessageInfoResponse->has_new_friend = response->has_new_friend();
//			B2MessageInfoResponse->has_friend_request = response->has_friend_request();
//
//			 result of hint RED_DOT_ADVENTURE
//			B2MessageInfoResponse->is_valid_adventure = response->has_has_adventure_mileage_reward();
//			B2MessageInfoResponse->has_adventure_mileage_reward = response->has_adventure_mileage_reward();
//
//			 result of hint RED_DOT_DUNGEON
//			B2MessageInfoResponse->is_valid_dungeon = response->has_counter_dungeon_weekly_clear_count();
//			B2MessageInfoResponse->counter_dungeon_weekly_clear_count = response->counter_dungeon_weekly_clear_count();
//			B2MessageInfoResponse->counter_dungeon_weekly_reward_received = response->counter_dungeon_weekly_reward_received();
//			B2MessageInfoResponse->tickets_for_counter_dungeon = response->tickets_for_counter_dungeon();
//			B2MessageInfoResponse->tickets_for_hero_tower = response->tickets_for_hero_tower();
//			B2MessageInfoResponse->used_tickets_for_hero_tower = response->used_tickets_for_hero_tower();
//			B2MessageInfoResponse->dimension_daily_clear_count = response->dimension_daily_clear_count();
//
//			 result of hint RED_DOT_PVP
//			B2MessageInfoResponse->is_valid_pvp = response->has_has_one_vs_one_reward();
//			B2MessageInfoResponse->has_one_vs_one_reward = response->has_one_vs_one_reward();
//			B2MessageInfoResponse->has_team_battle_reward = response->has_team_battle_reward();
//			B2MessageInfoResponse->has_team_assault_reward = response->has_team_assault_reward();
//			B2MessageInfoResponse->team_assault_mvp_point = response->team_assault_mvp_point();
//
//			 result of hint RED_DOT_GUILD
//			B2MessageInfoResponse->is_valid_guild = response->has_has_guild_battle_reward_box();
//			B2MessageInfoResponse->has_guild_battle_reward_box = response->has_guild_battle_reward_box();
//
//
//			 result of hint RED_DOT_SHOP
//			B2MessageInfoResponse->is_valid_shop = response->has_remaining_free_item_time();
//			B2MessageInfoResponse->remaining_free_item_time = response->remaining_free_item_time();
//			B2MessageInfoResponse->has_shop_item_mileage_reward = response->has_shop_item_mileage_reward();
//			B2MessageInfoResponse->has_shop_cash_mileage_reward = response->has_shop_cash_mileage_reward();
//
//			 result of hint RED_DOT_MAIL
//			B2MessageInfoResponse->is_valid_mail = response->has_unread_mail_count();
//			B2MessageInfoResponse->unread_mail_count = response->unread_mail_count();
//			B2MessageInfoResponse->has_present_mail = response->has_present_mail();
//			B2MessageInfoResponse->has_blade_point_mail = response->has_blade_point_mail();
//			B2MessageInfoResponse->has_social_point_mail = response->has_social_point_mail();
//			B2MessageInfoResponse->has_lottery_mail = response->has_lottery_mail();
//
//			 result of hint RED_DOT_EVENT
//			B2MessageInfoResponse->is_valid_event = response->has_has_event_level_up_reward();
//			B2MessageInfoResponse->has_event_level_up_reward = response->has_event_level_up_reward();
//			B2MessageInfoResponse->has_event_point_reward = response->has_event_point_reward();
//			B2MessageInfoResponse->has_event_season_mission_reward = response->has_event_season_mission_reward();
//			B2MessageInfoResponse->has_event_stage_clear_reward = response->has_event_stage_clear_reward();
//			B2MessageInfoResponse->has_event_point_shop_reward = response->has_event_point_shop_reward();
//
//			 result of hint RED_DOT_SHOP_PACKAGE
//			B2MessageInfoResponse->is_valid_shop_package = response->has_has_shop_package_flat_rate_reward();
//			B2MessageInfoResponse->has_shop_package_flat_rate_reward = response->has_shop_package_flat_rate_reward();
//			B2MessageInfoResponse->has_shop_package_level_up_reward = response->has_shop_package_level_up_reward();
//			B2MessageInfoResponse->has_shop_package_reach_level_reward = response->has_shop_package_reach_level_reward();
//			B2MessageInfoResponse->has_event_gem_purchase_reward = response->has_event_gem_purchase_reward();
//			B2MessageInfoResponse->has_event_ten_lottery_reward = response->has_event_ten_lottery_reward();
//			B2MessageInfoResponse->has_event_spend_gem_reward = response->has_event_spend_gem_reward();
//			B2MessageInfoResponse->has_event_ten_lottery_savings_reward = response->has_event_ten_lottery_savings_reward();
//			B2MessageInfoResponse->has_event_dice_reward = response->has_event_dice_reward();
//
//			 result of hint RED_DOT_MAGIC_SHOP
//			B2MessageInfoResponse->is_valid_magic_shop = response->has_has_magic_shop_free_renewal();
//			B2MessageInfoResponse->has_magic_shop_free_renewal = response->has_magic_shop_free_renewal();
//
//			 result of hint RED_DOT_HALL_OF_FAME
//			B2MessageInfoResponse->is_valid_hall_of_fame = response->has_has_hall_of_fame_praise_count();
//			B2MessageInfoResponse->has_hall_of_fame_praise_count = response->has_hall_of_fame_praise_count();
//
//			 result of hint RED_DOT_FAIRY
//			B2MessageInfoResponse->is_valid_not_apply_bless_fairy = response->has_has_not_apply_bless_fairy();
//			B2MessageInfoResponse->has_not_apply_bless_fairy = response->has_not_apply_bless_fairy();
//
//			 result of hint RED_DOT_DONATION
//			B2MessageInfoResponse->is_valid_donation = response->has_has_donation_point_reward();
//			B2MessageInfoResponse->has_donation_point_reward = response->has_donation_point_reward();
//			B2MessageInfoResponse->has_usable_free_donation_count = response->has_usable_free_donation_count();
//
//			 result of hint RED_DOT_ETC
//			B2MessageInfoResponse->is_valid_etc = response->has_has_item_random_option_change();
//			B2MessageInfoResponse->has_item_random_option_change = response->has_item_random_option_change();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckChargePointsResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckChargePointsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckChargePoints>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			if (response->has_blade_point())
//			{
//				B2MessageInfoResponse->blade_point = response->blade_point();
//				B2MessageInfoResponse->blade_point_max = response->blade_point_max();
//				B2MessageInfoResponse->blade_point_charge_lefttime_second = response->blade_point_charge_lefttime_second();
//			}
//			else
//			{
//				B2MessageInfoResponse->blade_point = -9999;
//			}
//			if (response->has_pvp_match_point()) {
//				B2MessageInfoResponse->pvp_match_point = response->pvp_match_point();
//				B2MessageInfoResponse->pvp_match_point_charge_lefttime_second = response->pvp_match_point_charge_lefttime_second();
//			}
//			else
//			{
//				B2MessageInfoResponse->pvp_match_point = -9999;
//			}
//			if (response->has_team_match_point()) 
//			{
//				B2MessageInfoResponse->team_match_point = response->team_match_point();
//				B2MessageInfoResponse->team_match_point_charge_lefttime_second = response->team_match_point_charge_lefttime_second();
//			}
//			else
//			{
//				B2MessageInfoResponse->team_match_point = -9999;
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromIncreaseDarkCrystalPaidResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::IncreaseDarkCrystalPaidResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseIncreaseDarkCrystalPaid>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO INCREASEDARKCRYSTALPAID: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->dark_crystal_paid = response->dark_crystal_paid();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromModifyItemRandomOptionResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::ModifyItemRandomOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseModifyItemRandomOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO MODIFYITEMRANDOMOPTION: ADD INTERNAL HANDLER
//			 B2MessageInfoResponse->item_id = response->item_id();;
//			 B2MessageInfoResponse->random_option_id1 = response->random_option_id1();;
//			 B2MessageInfoResponse->random_option_id2 = response->random_option_id2();;
//			 B2MessageInfoResponse->random_option_id3 = response->random_option_id3();;
//			 B2MessageInfoResponse->random_option_value1 = response->random_option_value1();;
//			 B2MessageInfoResponse->random_option_value2 = response->random_option_value2();;
//			 B2MessageInfoResponse->random_option_value3 = response->random_option_value3();;
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromModifyItemNewFlagResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ModifyItemNewFlagResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseModifyItemNewFlag>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{	
//			
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromModifyCostumeNewFlagResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ModifyCostumeNewFlagResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseModifyCostumeNewFlag>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromStartRaidResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::StartRaidResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseStartRaid>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{	
//			
//		}
//		else if (B2MessageInfoResponse->GetErrorCode() == B2RESULT_NOT_ENOUGH_INVENTORY_COUNT)
//		{
//			int InventoryCountInfoSize = response->inventory_count_infos_size();
//			for (int i = 0; i < InventoryCountInfoSize; ++i)
//			{
//				auto InventoryCountInfo = InventoryCountInfoPb2B2InventoryCountServerInfo(response->inventory_count_infos(i));
//				if (InventoryCountInfo)
//				{
//					B2MessageInfoResponse->inventory_count_infos.Emplace(InventoryCountInfo);
//				}
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetContentsOpenStatusResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetContentsOpenStatusResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetContentsOpenStatus>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto box : response->contents())
//			{
//				B2MessageInfoResponse->contents.Emplace(ToB2ContentsOpenStatus(box));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetDuelRewardBoxInfoResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetDuelRewardBoxInfoResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetDuelRewardBoxInfo>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_duel_mode())
//			B2MessageInfoResponse->duel_mode = ToB2DuelMode(response->duel_mode());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto box : response->reward_boxes())
//			{
//				B2MessageInfoResponse->reward_boxes.Emplace(ToB2DuelRewardBoxInfoResponseRewardBox(box));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetDuelRewardBoxGainableListResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetDuelRewardBoxGainableListResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetDuelRewardBoxGainableList>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_duel_mode())
//			B2MessageInfoResponse->duel_mode = ToB2DuelMode(response->duel_mode());
//
//		if (response->has_grade())
//			B2MessageInfoResponse->grade = ToB2RewardBoxGrade(response->grade());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto item : response->reward_items())
//			{
//				B2MessageInfoResponse->reward_items.Emplace(ToB2DuelRewardBoxGainableListResponseRewardItem(item));
//			}
//
//			B2MessageInfoResponse->reward_item_count = response->reward_item_count();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromOpenDuelRewardBoxResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::OpenDuelRewardBoxResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseOpenDuelRewardBox>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_duel_mode())
//			B2MessageInfoResponse->duel_mode = ToB2DuelMode(response->duel_mode());
//
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(reward));
//			}
//			B2MessageInfoResponse->reward_box_status = ToB2DuelRewardBoxStatus(response->reward_box_status());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveAssaultMvpRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveAssaultMvpRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveAssaultMvpReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->received_rewards())
//			{
//				B2MessageInfoResponse->received_rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEventAttendanceResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EventAttendanceResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEventAttendance>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->eventId = response->eventid();
//			B2MessageInfoResponse->attendance_day = response->attendance_day();
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//			if (response->has_next_attendance_time())
//				B2MessageInfoResponse->next_attendance_time = response->next_attendance_time();
//			B2MessageInfoResponse->bonus_type = response->has_bonus_type()? 
//				ToB2EventAttanceBonusType(response->bonus_type()) : B2EventAttanceBonusType::NONE;
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBeginnerAttendanceResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BeginnerAttendanceResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBeginnerAttendance>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->attendance_day = response->attendance_day();
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//			if (response->has_next_attendance_time())
//				B2MessageInfoResponse->next_attendance_time = response->next_attendance_time();
//			else
//				B2MessageInfoResponse->next_attendance_time = 0;
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpdateDailyPlayTimeResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UpdateDailyPlayTimeResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUpdateDailyPlayTime>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->daily_play_time_in_sec = response->daily_play_time_in_sec();
//			B2MessageInfoResponse->daily_play_time_reward_index = response->daily_play_time_reward_index();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveDailyPlayTimeRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveDailyPlayTimeRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveDailyPlayTimeReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->daily_play_time_in_sec = response->daily_play_time_in_sec();
//			B2MessageInfoResponse->daily_play_time_reward_index = response->daily_play_time_reward_index();
//
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAdminMessageNotify(MessagePtr message)
//	{
//		auto notify = std::static_pointer_cast<b2::protocol::session::AdminMessageNotify>(message);
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoAdminMessageNotify>();
//
//		for (auto adminMessage : notify->messages())
//		{
//			B2MessageInfoResponse->messages.Emplace(ToB2AdminMessage(adminMessage));
//		}
//		
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAccountAetherResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAccountAetherResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAccountAether>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto aether : response->aethers())
//			{
//				B2MessageInfoResponse->aethers.Emplace(AetherPb2B2AetherServerInfo(aether));
//			}
//
//			for (auto characterSetOptionInfo : response->current_character_set_option_infos())
//			{
//				B2MessageInfoResponse->current_character_set_option_Infos.Emplace(CharacterSetOptionInfoPb2B2CharacterSetOptionInfo(characterSetOptionInfo));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDismantleAethersResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DismantleAethersResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDismantleAethers>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto aetherId : response->dismantle_aether_ids())
//			{
//				B2MessageInfoResponse->dismantle_aether_Ids.Emplace(aetherId);
//			}
//
//			B2MessageInfoResponse->current_aether_piece = ItemPb2B2ItemServerInfo(response->current_aether_piece());
//			B2MessageInfoResponse->obtain_aether_piece_count = response->obtain_aether_piece_count();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromMountAethersResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::MountAethersResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseMountAethers>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mountedAetherId : response->mounted_aether_ids())
//			{
//				B2MessageInfoResponse->mounted_aether_ids.Emplace(mountedAetherId);
//			}
//
//			for (auto unmountedAetherId : response->unmounted_aether_ids())
//			{
//				B2MessageInfoResponse->unmounted_aether_ids.Emplace(unmountedAetherId);
//			}
//
//			B2MessageInfoResponse->selected_offense_set_option_id = response->selected_offense_set_option_id();
//			B2MessageInfoResponse->selected_defense_set_option_id = response->selected_defense_set_option_id();
//			B2MessageInfoResponse->selected_unity_skill_set_option_id = response->selected_unity_skill_set_option_id();
//
//			for (auto defenseSetOptionId : response->defense_set_option_ids())
//			{
//				B2MessageInfoResponse->defense_set_option_ids.Emplace(defenseSetOptionId);
//			}
//
//			for (auto unitySkillSetOptionId : response->unity_skill_set_option_ids())
//			{
//				B2MessageInfoResponse->unity_skill_set_option_ids.Emplace(unitySkillSetOptionId);
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnmountAethersResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnmountAethersResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnmountAethers>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto unmountedAetherId : response->unmounted_aether_ids())
//			{
//				B2MessageInfoResponse->unmounted_aether_ids.Emplace(unmountedAetherId);
//			}
//
//			B2MessageInfoResponse->selected_offense_set_option_id = response->selected_offense_set_option_id();
//			B2MessageInfoResponse->selected_defense_set_option_id = response->selected_defense_set_option_id();
//			B2MessageInfoResponse->selected_unity_skill_set_option_id = response->selected_unity_skill_set_option_id();
//
//			for (auto defenseSetOptionId : response->defense_set_option_ids())
//			{
//				B2MessageInfoResponse->defense_set_option_ids.Emplace(defenseSetOptionId);
//			}
//
//			for (auto unitySkillSetOptionId : response->unity_skill_set_option_ids())
//			{
//				B2MessageInfoResponse->unity_skill_set_option_ids.Emplace(unitySkillSetOptionId);
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLockAethersResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LockAethersResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLockAethers>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto lockedAether : response->locked_aethers())
//			{
//				B2MessageInfoResponse->locked_aethers.Emplace(AetherPb2B2AetherServerInfo(lockedAether));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnlockAethersResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnlockAethersResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnlockAethers>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto unlockedAether : response->unlocked_aethers())
//			{
//				B2MessageInfoResponse->unlocked_aethers.Emplace(AetherPb2B2AetherServerInfo(unlockedAether));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromExtendAetherInventoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ExtendAetherInventoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseExtendAetherInventory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->account = AccountPb2B2AccountInfo(response->account());
//			B2MessageInfoResponse->current_slot_count = response->current_slot_count();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEnhanceAetherResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EnhanceAetherResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEnhanceAether>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->aether = AetherPb2B2AetherServerInfo(response->aether());
//
//			for (auto aetherPiece : response->changed_aether_piece_items())
//			{
//				B2MessageInfoResponse->changed_aether_piece_items.Emplace(ItemPb2B2ItemServerInfo(aetherPiece));
//			}
//
//			for (auto aetherId : response->deleted_aether_piece_ids())
//			{
//				B2MessageInfoResponse->deleted_aether_piece_ids.Emplace(aetherId);
//			}
//
//			B2MessageInfoResponse->is_success = response->is_success();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSellAethersResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SellAethersResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSellAethers>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//			int AetherCount = response->sell_aether_ids_size();
//			for (int i = 0; i < AetherCount; ++i)
//			{
//				B2MessageInfoResponse->sell_aether_ids.Emplace(response->sell_aether_ids(i));
//			}
//			B2MessageInfoResponse->obtain_gold = response->obtain_gold();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromModifyAetherNewFlagResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ModifyAetherNewFlagResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseModifyAetherNewFlag>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSelectAetherDefenseOptionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SelectAetherDefenseOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSelectAetherDefenseOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->selected_defense_set_option_id = response->selected_defense_set_option_id();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSelectAetherUnitySkillOptionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SelectAetherUnitySkillOptionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSelectAetherUnitySkillOption>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->selected_unity_skill_set_option_id = response->selected_unity_skill_set_option_id();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetUnitySkillMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetUnitySkillMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetUnitySkillMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto mission : response->missions())
//			{
//				B2MessageInfoResponse->missions.Emplace(ToB2UnitySkillMission(mission));
//			}
//			B2MessageInfoResponse->all_mission_completed = response->all_mission_completed();
//
//			for (auto awakenMission : response->awaken_missions())
//			{
//				B2MessageInfoResponse->awaken_missions.Emplace(ToB2UnitySkillAwakenMission(awakenMission));
//			}
//			B2MessageInfoResponse->all_awaken_mission_completed = response->all_awaken_mission_completed();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveUnitySkillRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveUnitySkillRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveUnitySkillReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//			B2MessageInfoResponse->next_mission = ToB2UnitySkillMission(response->next_mission());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAwakenUnitySkillMissionResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AwakenUnitySkillMissionResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAwakenUnitySkillMission>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->awaken_mission = ToB2UnitySkillAwakenMission(response->awaken_mission());
//			B2MessageInfoResponse->current_money = response->current_money();
//
//			for (auto itemInfo : response->changed_material_items())
//			{
//				B2MessageInfoResponse->changed_material_items.Emplace(ItemPb2B2ItemServerInfo(itemInfo));
//			}
//
//			for (auto deleteItemId : response->deleted_item_ids())
//			{
//				B2MessageInfoResponse->deleted_item_ids.Emplace(deleteItemId);
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAwakenUnitySkillResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AwakenUnitySkillResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAwakenUnitySkill>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetDuelModeInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetDuelModeInfoResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetDuelModeInfo>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto modeInfo : response->mode_infos())
//			{
//				b2->mode_infos.Emplace(ToB2ContentsModeInfo(modeInfo));
//			}
//
//			for (auto periodInfo : response->assault_open_period_infos())
//			{
//				b2->assault_open_period_infos.Emplace(ToB2MatchPeriodInfo(periodInfo));
//			}
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetChallengeModeInfoResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetChallengeModeInfoResponse>(message);
//
//		auto b2 = std::make_shared<FB2MessageInfoResponseGetChallengeModeInfo>();
//		b2->SetErrorCode(response->result().error_code());
//		if (b2->IsSuccess())
//		{
//			for (auto modeInfo : response->mode_infos())
//			{
//				b2->mode_infos.Emplace(ToB2ContentsModeInfo(modeInfo));
//			}
//
//			for (auto periodInfo : response->raid_open_period_infos())
//			{
//				b2->raid_open_period_infos.Emplace(ToB2MatchPeriodInfo(periodInfo));
//			}
//
//			b2->hero_tower_ticket_count = response->hero_tower_ticket_count();
//			b2->counter_dungeon_left_ticket_count = response->counter_dungeon_left_ticket_count();
//			b2->dimension_daily_clear_count = response->dimension_daily_clear_count();
//		}
//
//		return b2;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveRestRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveRestRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveRestReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//			B2MessageInfoResponse->used_cash = response->used_cash();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckModeOpenResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckModeOpenResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckModeOpen>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto periodInfo : response->assault_open_period_infos())
//			{
//				B2MessageInfoResponse->assault_open_period_infos.Emplace(ToB2MatchPeriodInfo(periodInfo));
//			}
//
//			for (auto periodInfo : response->raid_open_period_infos())
//			{
//				B2MessageInfoResponse->raid_open_period_infos.Emplace(ToB2MatchPeriodInfo(periodInfo));
//			}
//
//			B2MessageInfoResponse->guild_battle_state = ToB2GuildBattleStateType(response->guild_battle_state());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetHallOfFameResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetHallOfFameResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetHallOfFame>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto basic : response->basics())
//			{
//				B2MessageInfoResponse->basics.Emplace(ToB2HallOfFame(basic));
//			}
//
//			for (auto ranker : response->rankers())
//			{
//				B2MessageInfoResponse->rankers.Emplace(ToB2HallOfFameUser(ranker));
//			}
//
//			for (auto reward : response->ranker_rewards())
//			{
//				B2MessageInfoResponse->ranker_rewards.Emplace(ToB2HallOfFameReward(reward));
//			}
//
//			for (auto praise : response->left_praise_counts())
//			{
//				B2MessageInfoResponse->left_praise_counts.Emplace(ToB2HallOfFamePraise(praise));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPraiseTargetRankerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::PraiseTargetRankerResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePraiseTargetRanker>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto basic : response->basics())
//			{
//				B2MessageInfoResponse->basics.Emplace(ToB2HallOfFame(basic));
//			}
//
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//
//			if (response->has_left_praise_count()) 
//			{
//				B2MessageInfoResponse->left_praise_count = ToB2HallOfFamePraise(response->left_praise_count());
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetTutorialProgressResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetTutorialProgressResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetTutorialProgress>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO GETTUTORIALPROGRESS: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->tutorial_id = response->tutorial_id();
//			B2MessageInfoResponse->is_item_prepared = response->is_item_prepared();
//			B2MessageInfoResponse->is_lottery_drawn = response->is_lottery_drawn();
//			B2MessageInfoResponse->is_finished = response->is_finished();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSetTutorialProgressResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::SetTutorialProgressResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSetTutorialProgress>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO SETTUTORIALPROGRESS: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->tutorial_id = response->tutorial_id();
//			B2MessageInfoResponse->is_item_prepared = response->is_item_prepared();
//			B2MessageInfoResponse->is_lottery_drawn = response->is_lottery_drawn();
//			B2MessageInfoResponse->is_finished = response->is_finished();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUpdateTutorialProgressResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::UpdateTutorialProgressResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUpdateTutorialProgress>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO GETTUTORIALPROGRESS: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->tutorial_id = response->tutorial_id();
//			B2MessageInfoResponse->is_item_prepared = response->is_item_prepared();
//			B2MessageInfoResponse->is_lottery_drawn = response->is_lottery_drawn();
//			B2MessageInfoResponse->is_finished = response->is_finished();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSkipTutorialProgressResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::SkipTutorialProgressResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSkipTutorialProgress>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO SKIPTUTORIALPROGRESS: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->tutorial_id = response->tutorial_id();
//			B2MessageInfoResponse->is_item_prepared = response->is_item_prepared();
//			B2MessageInfoResponse->is_lottery_drawn = response->is_lottery_drawn();
//			B2MessageInfoResponse->is_finished = response->is_finished();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDrawTutorialItemResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::DrawTutorialItemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDrawTutorialItem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO DRAWTUTORIALITEM: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->free_lottery = response->free_lottery();
//			B2MessageInfoResponse->shop_ten_lottery_ticket = response->shop_ten_lottery_ticket();
//			B2MessageInfoResponse->gem = response->gem();
//			B2MessageInfoResponse->social_point = response->social_point();
//			B2MessageInfoResponse->mileage = response->mileage();
//			B2MessageInfoResponse->next_free_time = response->next_free_time();
//			B2MessageInfoResponse->daily_lottery_count = response->daily_lottery_count();
//			for (auto item : response->items())
//			{
//				B2MessageInfoResponse->items.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromPrepareTutorialResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::PrepareTutorialResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponsePrepareTutorial>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO PREPARETUTORIAL: ADD INTERNAL HANDLER
//			B2MessageInfoResponse->tutorial_id = response->tutorial_id();
//			B2MessageInfoResponse->current_cash = response->current_cash();
//			B2MessageInfoResponse->current_gold = response->current_gold();
//
//			for (auto item : response->added_item_list())
//			{
//				B2MessageInfoResponse->added_item_list.Emplace(ItemPb2B2ItemServerInfo(item));
//			}
//			for (auto aether : response->added_aether_list())
//			{
//				B2MessageInfoResponse->added_aether_list.Emplace(AetherPb2B2AetherServerInfo(aether));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGiveTutorialRewardResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GiveTutorialRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGiveTutorialReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO GIVETUTORIALREWARD: ADD INTERNAL HANDLER
//			for (auto reward : response->tutorial_rewards())
//			{
//				B2MessageInfoResponse->tutorial_rewards.Emplace(ToB2Reward(reward));
//			}
//
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildChatNotify(MessagePtr message)
//	{
//		auto notify = std::static_pointer_cast<b2::protocol::session::GuildChatNotify>(message);
//
//		auto B2MessageInfoNotify = std::make_shared<FB2MessageInfoGuildChatNotify>();
//		for (auto chatData : notify->chat_list())
//		{
//			B2MessageInfoNotify->chat_list.Emplace(ToB2GuildChatData(chatData));
//		}
//		return B2MessageInfoNotify;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildChatResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildChatResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGuildChat>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetGuildChatResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetGuildChatResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetGuildChat>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto chat_data : response->chat_list())
//			{
//				B2MessageInfoResponse->chat_list.Emplace(ToB2GuildChatData(chat_data));
//			}
//
//			B2MessageInfoResponse->next_ask_support_time = response->next_ask_support_time();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAskGuildSupportResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AskGuildSupportResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAskGuildSupport>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->next_ask_support_time = response->next_ask_support_time();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildSupportResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildSupportResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGuildSupport>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto deleted_item_id : response->deleted_item_ids())
//			{
//				B2MessageInfoResponse->deleted_item_ids.Emplace(deleted_item_id);
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGuildDonationResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GuildDonationResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGuildDonation>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->light_account = LightAccountPb2B2LightAccount(response->light_account());
//			B2MessageInfoResponse->guild_member = ToB2GuildMember(response->guild_member());
//			B2MessageInfoResponse->mercenary = ToB2GuildMercenary(response->mercenary());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromCheckReachPowerResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::CheckReachPowerResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseCheckReachPower>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 TO DO CHECKREACHPOWER: ADD INTERNAL HANDLER
//			 B2MessageInfoResponse->result = CONVERTER(response->result());;
//			 B2MessageInfoResponse->character_power = CONVERTER(response->character_power());;
//
//			 TRACKING TARGET LIST
//			for (auto characterType : response->required_tracking_targets())
//			{
//				B2MessageInfoResponse->required_tracking_targets.Emplace(characterType);
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromTrackingPowerDetailResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::TrackingPowerDetailResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseTrackingPowerDetail>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->different_count = response->different_count();
//
//			 request info
//			B2MessageInfoResponse->character_type = PbCharacter2B2CharacterType(response->character_type());
//			B2MessageInfoResponse->item_preset_id = response->item_preset_id();
//			B2MessageInfoResponse->skill_slot = response->skill_slot();
//
//			 SERVER-SIDE POWER VALUES
//			for (auto specificPower : response->specific_power_values())
//			{
//				B2MessageInfoResponse->specific_power_values.Emplace(ToB2CharacterPowerSpecific(specificPower));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromKakaoMessageProcessingResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::KakaoMessageProcessingResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseKakaoMessageProcessing>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (response->has_message_box_id())
//		{
//			B2MessageInfoResponse->message_box_id = UTF8_TO_TCHAR(response->message_box_id().c_str()); ;
//		}
//		
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->processing_count = response->processing_count();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromKakaoCouponUseResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::KakaoCouponUseResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseKakaoCouponUse>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2RawReward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromKakaoAccountConversionPrepareResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::KakaoAccountConversionPrepareResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseKakaoAccountConversionPrepare>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2RawReward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromKakaoAccountConversionCompleteResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::KakaoAccountConversionCompleteResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseKakaoAccountConversionComplete>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2RawReward(reward));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetEventResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::GetEventResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto status : response->point_event_statuses())
//			{
//				B2MessageInfoResponse->point_event_statuses.Emplace(ToB2PointEventStatus(status));
//			}
//
//			for (auto eventInfo : response->level_up_event_infos())
//			{
//				B2MessageInfoResponse->level_up_event_infos.Emplace(ToB2LevelUpEventInfo(eventInfo));
//			}
//
//			for (auto eventInfo : response->stage_clear_event_statuses())
//			{
//				B2MessageInfoResponse->stage_clear_event_statuses.Emplace(ToB2StageClearEventStatus(eventInfo));
//			}
//
//			for (auto eventInfo : response->point_shop_event_statuses())
//			{
//				B2MessageInfoResponse->point_shop_event_statuses.Emplace(ToB2PointShopEventStatus(eventInfo));
//			}
//
//			for (auto historyInfo : response->point_shop_event_reward_histories())
//			{
//				B2MessageInfoResponse->point_shop_event_reward_histories.Emplace(ToB2PointShopEventRewardHistory(historyInfo));
//			}
//
//			for (auto eventInfo : response->ten_lottery_event_statuses())
//			{
//				B2MessageInfoResponse->ten_lottery_event_statuses.Emplace(ToB2TenLotteryEventStatus(eventInfo));
//			}
//
//			for (auto eventInfo : response->gem_purchase_event_statuses())
//			{
//				B2MessageInfoResponse->gem_purchase_event_statuses.Emplace(ToB2GemPurchaseEventStatus(eventInfo));
//			}
//
//			for (auto eventInfo : response->spend_gem_event_statuses())
//			{
//				B2MessageInfoResponse->spend_gem_event_statuses.Emplace(ToB2SpendGemEvemtStatus(eventInfo));
//			}
//
//			for (auto eventInfo : response->ten_lottery_savings_event_statuses())
//			{
//				B2MessageInfoResponse->ten_lottery_savings_event_statuses.Emplace(ToB2TenLotterySavingsStatus(eventInfo));
//			}
//
//			for (auto eventInfo : response->dice_event_statuses())
//			{
//				B2MessageInfoResponse->dice_event_statuses.Emplace(ToB2DiceEventStatus(eventInfo));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceivePointEventRewardResponse(MessagePtr message)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceivePointEventRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceivePointEventReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveLevelUpEventRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveLevelUpEventRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveLevelUpEventReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveStageClearEventRewardResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveStageClearEventRewardResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveStageClearEventReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuyHotTimeBuffResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuyHotTimeBuffResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBuyHotTimeBuff>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->buff_type = response->buff_type();
//			B2MessageInfoResponse->buff_end_time = response->buff_end_time();
//			B2MessageInfoResponse->buff_ticket = response->buff_ticket();
//			B2MessageInfoResponse->current_gem = response->current_gem();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardFlatRatePackageBonusResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardFlatRatePackageBonusResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardFlatRatePackageBonus>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->received_reward = ToB2Reward(response->received_reward());
//			B2MessageInfoResponse->next_bonus_state = ToB2FlatRatePackageBonusState(response->next_bonus_state());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetPackageShopResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetPackageShopResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetPackageShop>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto state : response->periodic_package_states())
//			{
//				B2MessageInfoResponse->periodic_package_states.Emplace(ToB2PeriodicPackageState(state));
//			}
//
//			B2MessageInfoResponse->flat_rate_package_bonus_state = ToB2FlatRatePackageBonusState(response->flat_rate_package_bonus_state());
//
//			for (auto mission : response->level_up_package_missions())
//			{
//				B2MessageInfoResponse->level_up_package_missions.Emplace(ToB2LevelUpPackageMission(mission));
//			}
//
//			for (auto mission : response->reach_level_package_missions())
//			{
//				B2MessageInfoResponse->reach_level_package_missions.Emplace(ToB2ReachLevelPackageMission(mission));
//			}
//
//			for (auto buyHistory : response->shop_buy_manages())
//			{
//				B2MessageInfoResponse->shop_buy_manages.Emplace(ToB2ShopBuyManage(buyHistory));
//			}
//
//			for (auto mark : response->shop_product_marks())
//			{
//				B2MessageInfoResponse->shop_product_marks.Emplace(ToB2ShopProductMark(mark));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetPeriodPackageStateResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetPeriodPackageStateResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetPeriodPackageState>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto state : response->periodic_package_states())
//			{
//				B2MessageInfoResponse->periodic_package_states.Emplace(ToB2PeriodicPackageState(state));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardLevelUpPackageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardLevelUpPackageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardLevelUpPackage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->received_reward = ToB2Reward(response->received_reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardReachLevelPackageResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardReachLevelPackageResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardReachLevelPackage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->received_reward = ToB2Reward(response->received_reward());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGiveUpQuestResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GiveUpQuestResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGiveUpQuest>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->next_quest_info = QuestInfoPb2B2QuestInfo(response->next_quest_info());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetContentsModeStateResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetContentsModeStateResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetContentsModeState>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto modeInfo : response->mode_infos())
//			{
//				B2MessageInfoResponse->mode_infos.Emplace(ToB2ContentsModeInfo(modeInfo));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardPointShopEventResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardPointShopEventResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardPointShopEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->reward = ToB2Reward(response->reward());
//
//			B2MessageInfoResponse->point_shop_event_status = ToB2PointShopEventStatus(response->point_shop_event_status());
//			B2MessageInfoResponse->index = response->index();
//			B2MessageInfoResponse->total_reward_count = response->total_reward_count();
//			B2MessageInfoResponse->buy_count = response->buy_count();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReportUserResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReportUserResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReportUser>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBlockChatResponse(MessagePtr messageInfo)
//	{
//
//		auto response = std::static_pointer_cast<b2::protocol::session::BlockChatResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBlockChat>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->blocked_user = ToB2BlockChatUser(response->blocked_user());
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnblockChatResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnblockChatResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnblockChat>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetBlockChatListResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetBlockChatListResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetBlockChatList>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto user : response->blocked_users())
//			{
//				B2MessageInfoResponse->blocked_users.Emplace(ToB2BlockChatUser(user));
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromClientStateReportResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ClientStateReportResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseClientStateReport>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardTenLotteryEventResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardTenLotteryEventResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardTenLotteryEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//			
//			B2MessageInfoResponse->reward_index = response->reward_index();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromReceiveGemPurchaseEventRewardResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ReceiveGemPurchaseEventRewardResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseReceiveGemPurchaseEventReward>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//			
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardSpendGemEventResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardSpendGemEventResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardSpendGemEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardTenLotterySavingsEventResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardTenLotterySavingsEventResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardTenLotterySavingsEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//			
//			B2MessageInfoResponse->current_point = response->current_point();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRollDiceEventResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RollDiceEventResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRollDiceEvent>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			if (response->has_cell_reward()) 
//			{
//				B2MessageInfoResponse->cell_reward = ToB2Reward(response->cell_reward());
//			}
//
//			if (response->has_finish_reward()) 
//			{
//				B2MessageInfoResponse->cell_reward = ToB2Reward(response->finish_reward());
//			}
//
//			B2MessageInfoResponse->current_position = response->current_position();
//			B2MessageInfoResponse->dice_number = response->dice_number();
//			B2MessageInfoResponse->dice_cell_type = response->dice_cell_type();
//			B2MessageInfoResponse->is_finish = response->is_finish();
//			B2MessageInfoResponse->free_dice_count = response->free_dice_count();
//			B2MessageInfoResponse->reward_count_times = response->reward_count_times();
//			B2MessageInfoResponse->finish_count = response->finish_count();
//			B2MessageInfoResponse->roll_dice_count = response->roll_dice_count();
//			B2MessageInfoResponse->current_point = response->current_point();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromBuyDiceEventPointResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::BuyDiceEventPointResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseBuyDiceEventPoint>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gem = response->current_gem();
//			B2MessageInfoResponse->buy_count = response->buy_count();
//			B2MessageInfoResponse->point = response->point();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRewardDiceEventMileageResponse(MessagePtr messageInfo)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RewardDiceEventMileageResponse>(messageInfo);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRewardDiceEventMileage>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			for (auto reward : response->rewards())
//			{
//				B2MessageInfoResponse->rewards.Emplace(ToB2Reward(reward));
//			}
//			
//			B2MessageInfoResponse->reward_value = response->reward_value();
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	 totem
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromGetAccountTotemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::GetAccountTotemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseGetAccountTotem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			 totems
//			int totemCount = response->totems_size();
//			for (int i = 0; i < totemCount; ++i)
//			{
//				auto b2Totem = TotemPb2B2TotemServerInfo(response->totems(i));
//				if (b2Totem)
//				{
//					B2MessageInfoResponse->totems.Emplace(b2Totem);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromEquipTotemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::EquipTotemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseEquipTotems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int equipTotemCount = response->equiped_totem_ids_size();
//			for (int i = 0; i < equipTotemCount; ++i)
//			{
//				B2MessageInfoResponse->equiped_totem_ids.Emplace(response->equiped_totem_ids(i));
//			}
//
//			int unequipTotemCount = response->unequiped_totem_ids_size();
//			for (int i = 0; i < unequipTotemCount; ++i)
//			{
//				B2MessageInfoResponse->unequiped_totem_ids.Emplace(response->unequiped_totem_ids(i));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnequipTotemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnequipTotemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnequipTotems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int unequipTotemCount = response->unequiped_totem_ids_size();
//			for (int i = 0; i < unequipTotemCount; ++i)
//			{
//				B2MessageInfoResponse->unequiped_totem_ids.Emplace(response->unequiped_totem_ids(i));
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromLockTotemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::LockTotemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseLockTotems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int totemCount = response->locked_totems_size();
//			for (int i = 0; i < totemCount; ++i)
//			{
//				auto b2Totem = TotemPb2B2TotemServerInfo(response->locked_totems(i));
//				if (b2Totem)
//				{
//					B2MessageInfoResponse->locked_totems.Emplace(b2Totem);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromUnlockTotemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::UnlockTotemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseUnlockTotems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int totemCount = response->unlocked_totems_size();
//			for (int i = 0; i < totemCount; ++i)
//			{
//				auto b2Totem = TotemPb2B2TotemServerInfo(response->unlocked_totems(i));
//				if (b2Totem)
//				{
//					B2MessageInfoResponse->unlocked_totems.Emplace(b2Totem);
//				}
//			}
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromSellTotemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::SellTotemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseSellTotems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//
//			int sellTotemCount = response->sell_totem_ids_size();
//			for (int i = 0; i < sellTotemCount; ++i)
//			{
//				B2MessageInfoResponse->sell_totem_ids.Emplace(response->sell_totem_ids(i));
//			}
//
//			B2MessageInfoResponse->obtain_gold = response->obtain_gold();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromDismantleTotemsResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::DismantleTotemsResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseDismantleTotems>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int dismantleTotemCount = response->dismantle_totem_ids_size();
//			for (int i = 0; i < dismantleTotemCount; ++i)
//			{
//				B2MessageInfoResponse->dismantle_totem_ids.Emplace(response->dismantle_totem_ids(i));
//			}
//
//			B2MessageInfoResponse->current_dimensional_piece = ItemPb2B2ItemServerInfo(response->current_dimensional_piece());
//
//			B2MessageInfoResponse->obtain_dimensional_piece_count = response->obtain_dimensional_piece_count();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromRefineTotemResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::RefineTotemResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseRefineTotem>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			B2MessageInfoResponse->current_gold = response->current_gold();
//
//			B2MessageInfoResponse->refined_totem = TotemPb2B2TotemServerInfo(response->refined_totem());
//
//			int dimensionalPieceItemCount = response->changed_dimensional_piece_items_size();
//			for (int i = 0; i < dimensionalPieceItemCount; ++i)
//			{
//				auto b2Item = ItemPb2B2ItemServerInfo(response->changed_dimensional_piece_items(i));
//				if (b2Item)
//				{
//					B2MessageInfoResponse->changed_dimensional_piece_items.Emplace(b2Item);
//				}
//			}
//
//			int deleteDimensionalPieceCount = response->deleted_dimensional_piece_ids_size();
//			for (int i = 0; i < deleteDimensionalPieceCount; ++i)
//			{
//				B2MessageInfoResponse->deleted_dimensional_piece_ids.Emplace(response->deleted_dimensional_piece_ids(i));
//			}
//
//			B2MessageInfoResponse->is_success = response->is_success();
//		}
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromModifyTotemNewFlagResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::ModifyTotemNewFlagResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseModifyTotemNewFlag>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		
//		return B2MessageInfoResponse;
//	}
//
//	FB2MessageInfoPtr FB2MessageInfoConverter::FromAddTotemToInventoryResponse(MessagePtr message)
//	{
//		auto response = std::static_pointer_cast<b2::protocol::session::AddTotemToInventoryResponse>(message);
//
//		auto B2MessageInfoResponse = std::make_shared<FB2MessageInfoResponseAddTotemToInventory>();
//		B2MessageInfoResponse->SetErrorCode(response->result().error_code());
//		if (B2MessageInfoResponse->IsSuccess())
//		{
//			int totemCount = response->totems_size();
//			for (int i = 0; i < totemCount; ++i)
//			{
//				auto b2Totem = TotemPb2B2TotemServerInfo(response->totems(i));
//				if (b2Totem)
//				{
//					B2MessageInfoResponse->totems.Emplace(b2Totem);
//				}
//			}
//		}
//
//		return B2MessageInfoResponse;
//	}
//
//	 REPLACEMENT POINT: CONVERTER CPP RESPONSE
//	 NEVER DELETE LINE ABOVE
//}
