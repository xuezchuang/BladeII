#pragma once

#include "CoreMinimal.h"
#include <memory>
#include "B2Reflection.h"
#ifdef TUTORIAL
#undef TUTORIAL
#endif
namespace b2network {
	//////////////////////////////////////////////////////////////////////////
	namespace B2PlatformCode
	{
		const int32 FOR_KAKAO = 1;
		const int32 WITH_NAVER = 2;
		const int32 FACEBOOK = 4;
		const int32 ONEACCOUNT = 5;
		const int32 GAMECENTER = 6;
		const int32 GOOGLEPLAY = 7;
		const int32 NONE = 99;			// guest
	};

	namespace B2OSType
	{
		const int32 PC = 0;
		const int32 AOS = 1;
		const int32 IOS = 2;
	};

	namespace B2MarketType
	{
		const int32 NONE = 0;

		const int32 APPSTORE = 11;      // apple app store
		const int32 GOOGLEPLAY = 12;    // google play store
		const int32 ONESTORE = 13;      // one store
		const int32 KAKAOGAMESHOP = 24;	// kakao game shop
		const int32 GALAXYAPPS = 30;	// samsung galaxy apps
	};

	namespace B2AccountType
	{
		const int32 NORMAL = 0;

		const int32 BOT = 9;
		const int32 QA = 10;
		const int32 GM = 20;
		const int32 ADMIN = 30;
	};

	namespace B2AccountStatus
	{
		const int32 STATUS_NORMAL = 0;

		const int32 STATUS_WITHDRAWAL_WAIT = 98;
		const int32 STATUS_WITHDRAWAL = 99;
	};

	namespace B2CharacterType
	{
		const int32 Common			= 0;	// Item 정보, 모든 캐릭터 사용 가능
		const int32 Not_Used		= 0;	// 에테르 정보, 미사용중( 사용중인 캐릭터가 없는 경우 )

		const int32 Gladiator		= 1;
		const int32 Assassin		= 2;
		const int32 Wizard			= 3;
		const int32 Fighter			= 4;

		const int32 None			= 5;
	};

	namespace B2ItemType
	{
		const int32 None = 0;
		const int32 Weapon = 1;
		const int32 Helmet = 2;
		const int32 BodyArmor = 3;
		const int32 Glove = 4;
		const int32 Shoes = 5;
		const int32 Belt = 6;
		const int32 Necklace = 7;
		const int32 Ring = 8;
		const int32 Material = 9;
		const int32 GOODS = 10;
		const int32 UpgradeStone = 11;
		const int32 EnhanceScroll = 12;
		const int32 BlessedEnhanceScroll = 13;
		const int32 ProtectionScroll = 14;
		const int32 BossPiece = 15;
		const int32 WeaponEssence = 16;
		const int32 ArmorEssence = 17;
		const int32 AccessoryEssence = 18;
		const int32 RankPromotionPoint = 19;
		const int32 HeroPiece = 20;
		const int32 AetherPiece = 21;
		const int32 Dismantle_Material = 22;
		const int32 DimensionalPiece = 23;
		const int32 CostumeEnhanceMaterial = 24;
		const int32 WeaponSurpassAnvil = 25;
		const int32 ArmorSurpassAnvil = 26;
		const int32 AccessorySurpassAnvil = 27;
		const int32 SealBox = 28;
		const int32 AdvancedRankPromotionPoint = 29;
		const int32 Gold = 100;
	};

	namespace B2CostumeType
	{
		const int32 CostumeWeapon = 201;
		const int32 CostumeHat = 202;
		const int32 CostumeArmor = 203;
	};

	namespace B2InventoryType
	{
		const int32 Weapon			= 2;
		const int32 Protection		= 3;
		const int32 Accessory		= 4;

		const int32 Wing			= 5;

		const int32 CostumeWeapon	= 202;
		const int32 CostumeArmor	= 203;

		const int32 Consumables		= 1001;
	};

	namespace B2EquipPosition
	{
		const int32 None			= 0;
		const int32 Weapon			= 1;
		const int32 Helmet			= 2;
		const int32 BodyArmor		= 3;
		const int32 Glove			= 4;
		const int32 Shoes			= 5;
		const int32 Belt			= 6;
		const int32 Necklace		= 7;
		const int32 Ring			= 8;
		const int32 Wing			= 9;
		const int32 CostumeWeapon	= 201;
		const int32 CostumeHat		= 202;
		const int32 CostumeArmor	= 203;
	};

	namespace B2StageBuffFlag
	{
		const int32 INCREASE_EXP		= 1;
		const int32 INCREASE_GOLD		= 2;
		const int32 AUTO_SKILL			= 4;
		const int32 BOOST_GAME_SPEED	= 8;
	};

	namespace B2ItemOption
	{
		const int32	BIO_None								= 0;

		const int32	BIO_Offense_IncAttack					= 10;
		const int32	BIO_Offense_IncDamage					= 11;
		const int32	BIO_Offense_IncNormalDamage				= 12;
		const int32	BIO_Offense_IncCriticalDamage			= 13;
		const int32	BIO_Offense_IncCriticalRate				= 14;
		const int32	BIO_Offense_IncBossDamage				= 15;
		const int32 BIO_Offense_IncAttackSpeed				= 16;
		const int32 BIO_Offense_IncAdditionalDamage			= 17;

		const int32	BIO_Skill_IncSkillDamage				= 20;
		const int32	BIO_Skill_DecSkillCooltime				= 21;
		const int32	BIO_Skill_IncSkill1Damage				= 22;
		const int32	BIO_Skill_IncSkill2Damage				= 23;
		const int32	BIO_Skill_IncSkill3Damage				= 24;
		const int32	BIO_Skill_IncSkill5Damage				= 25;

		const int32	BIO_Health_ProbableHPAbsorption			= 30;
		const int32	BIO_Health_ProbableRecoverHP			= 31;
		const int32	BIO_Health_IncHealth					= 32;
		const int32	BIO_Health_PerSecRecoverHP				= 33;

		const int32	BIO_Defense_IncDefense					= 40;
		const int32	BIO_Defense_DecAbnormalStateTime		= 41;
		const int32	BIO_Defense_DecDamage					= 42;
		const int32	BIO_Defense_DecBossDamage				= 43;
		const int32	BIO_Defense_DecMeleeDamage				= 44;
		const int32	BIO_Defense_DecRangedDamage				= 45;
		const int32	BIO_Defense_ResistCritical				= 46;
		const int32	BIO_Defense_DecAdditionalDamage			= 47;

		const int32	BIO_Misc_LastNormalAttackKnockbackRate	= 50;
		const int32	BIO_Misc_AdditionalExp					= 51;
		const int32	BIO_Misc_AdditionalGold					= 52;
		const int32	BIO_Misc_IncMoveSpeed					= 53;
		const int32	BIO_Misc_DecTagCooltime					= 54;

		//////////////////////////////////////////////////////////////////////////
		// mod option
		const int32 BIO_Tag_Offense_IncAttack				= 101;
		const int32 BIO_Tag_Defense_IncDefense				= 102;
		const int32 BIO_Tag_Health_IncHealth				= 103;

		const int32 BIO_HeroTower_Offense_IncAttack			= 201;
		const int32 BIO_HeroTower_Defense_IncDefense		= 202;
		const int32 BIO_HeroTower_Health_IncHealth			= 203;

		const int32 BIO_Assault_Offense_IncAttack			= 301;
		const int32 BIO_Assault_Defense_IncDefense			= 302;
		const int32 BIO_Assault_Health_IncHealth			= 303;

		const int32 BIO_Raid_Offense_IncAttack				= 401;
		const int32 BIO_Raid_Defense_IncDefense				= 402;
		const int32 BIO_Raid_Health_IncHealth				= 403;

		const int32 BIO_TeamMatch_Offense_IncAttack			= 501;
		const int32 BIO_TeamMatch_Defense_IncDefense		= 502;
		const int32 BIO_TeamMatch_Health_IncHealth			= 503;

		const int32 BIO_Guild_Offense_IncAttack				= 601;
		const int32 BIO_Guild_Defense_IncDefense			= 602;
		const int32 BIO_Guild_Health_IncHealth				= 603;
	};

	namespace B2ItemPrimeType
	{
		const int32 BIPT_None		= 0;
		const int32 BIPT_Attack		= 1;
		const int32 BIPT_Defense	= 2;
		const int32 BIPT_Health		= 3;
	}

	namespace B2ResurrectBuffType
	{
		const int32 BRBT_None = 0;
		const int32 BRBT_Attack = 1;
		const int32 BRBT_Defense = 2;
		const int32 BRBT_Health = 3;
	}

    namespace B2RoomType
    {
        const int32 BRT_Town = 0; // Pvp
        const int32 BRT_Raid = 1;
        const int32 BRT_Assault = 2;
    }

	namespace B2BattleFormationType
	{
		const int32 BTBFT_Normal	= 1;
		const int32 BTBFT_Offensive = 2;
		const int32 BTBFT_Defensive = 3;
	}

	namespace B2BattleFormaionBuffType
	{
		const int32 BTBFBT_None = 0;
		const int32 BTBFBT_Attack = 1;
		const int32 BTBFBT_Defense = 2;
	}

	namespace B2GuildBattleResultType
	{
		const int32 BGBRT_LOSE = 1;
		const int32 BGBRT_DRAW = 1000;
		const int32 BGBRT_WIN = 1000000;
	}

	namespace B2GuildBattleStateType
	{
		const int NONE = 0;
		const int INIT = 1;
		const int INITDONE = 2;
		const int READY = 3;
		const int MATCHING = 4;
		const int MATCHINGDONE = 5;
		const int BATTLE = 6;
		const int SETTLEMENT = 7;
		const int SETTLEMENTDONE = 8;
		const int RESULT = 9;
	}

	namespace B2GuildBattleEntryType
	{
		const int CHARACTER = 1;
		const int MERCENARY = 2;
	}

	namespace B2MatchInfoType
	{
		const int32 BMIT_AI = 1;
		const int32 BMIT_PLAYER = 2;
		const int32 BMIT_TEAM = 3;
		const int32 BMIT_RAID = 4;
		const int32 BMIT_ASSAULT = 5;
		const int32 BMIT_HEROTOWER = 6;
		const int32 BMIT_DONATION = 7;
		const int32 BMIT_DIMENSION = 8;
	}

	namespace B2MatchResultType
	{
		const int32 BMRT_WIN = 0;
		const int32 BMRT_LOSE = 1;
		const int32 BMRT_DRAW = 2;
	}

	namespace B2MatchMakingType
	{
		const int32 BMMT_PVP_RANK = 1;
		const int32 BMMT_PVP_FRIENDLY = 2;
        const int32 BMMT_RAID_RANK = 10;
        const int32 BMMT_ASSAULT_RANK = 20;
	}

	namespace B2MailCategory
	{
		const int32 PRESENT = 1;
		const int32 BLADE_POINT = 2;
		const int32 SOCIAL_POINT = 3;
		const int32 LOTTERY = 4;
	}

	namespace B2MailAttachType
	{
		const int32 GOLD = 1;
		const int32 GEM = 2;
		const int32 ITEM = 3;
		const int32 BLADE_POINT = 4;
		const int32 SOCIAL_POINT = 5;
		const int32 GENERAL_LOTTERY = 6;
		const int32 SELECTIVE_LOTTERY = 7;
		const int32 SELECTIVE_ITEM_LOTTERY = 8;
		const int32 HERO_PIECE = 9;
		const int32 RANK_PROMOTION_POINT = 10;
		const int32 DARK_CRYSTAL = 11;
		const int32 FIXED_GRADE_LOTTERY = 12;
		const int32 STAGE_BOOST_TICKET = 13;
		const int32 HOT_TIME_GOLD_BUFF_TICKET = 14;
		const int32 HOT_TIME_EXP_BUFF_TICKET = 15;
		const int32 SHOP_TEN_LOTTERY_TICKET = 16;
		const int32 FAME_POINT = 17;
		const int32 SELECTIVE_AETHER_LOTTERY = 18;
		const int32 PREFIX_SELECTIVE_LOTTERY = 19;
		const int32 STAGE_AUTO_CLEAR_TICKET = 20;
		const int32 TOTEM_PREFIX_LOTTERY = 21;
		const int32 ADVANCED_RANK_PROMOTION_POINT = 22;
	}

	namespace B2MailPresentAttachType
	{
		const int32 MONEY = 1;
		const int32 GEM = 2;
		const int32 ITEM = 3;		
		const int32 HERO_PIECE = 4;
		const int32 RANK_PROMOTION_POINT = 5;
		const int32 DARK_CRYSTAL = 6;
		const int32 STAGE_BOOST_TICKET = 7;
		const int32 HOT_TIME_GOLD_BUFF_TICKET = 8;
		const int32 HOT_TIME_EXP_BUFF_TICKET = 9;
		const int32 SHOP_TEN_LOTTERY_TICKET = 10;
		const int32 FAME_POINT = 11;
		const int32 STAGE_AUTO_CLEAR_TICKET = 12;
		const int32 ADVANCED_RANK_PROMOTION_POINT = 13;
	}

	namespace B2MailSenderType
	{
		const int ARIN = 1;          // 아린
		const int HUNT = 2;          // 헌트
		const int GAIUS = 3;         // 가이우스
		const int BELLA = 4;         // 벨라
		const int LUCHI = 5;         // 루치
		const int RAMIEL = 6;        // 라비엘
		const int ADMIN = 7;         // 운영자
		const int GRATIA = 8;        // 그라티아
		const int BROKK = 9;         // 브로크
	}

	namespace B2MailSourceType
	{
		const int32 ATTENDANCE = 1;                      //출석 보상
		const int32 PLAY_TIME = 2;                       //접속 보상
		const int32 EVENT_ATTENDANCE = 3;                //이벤트 출석 보상
		const int32 DAILY_MISSION = 4;                   //일일 업적 보상
		const int32 WEEKLY_MISSION = 5;                  //주간 업적 보상
		const int32 MONTHLY_MISSION = 6;                 //월간 업적 보상
		const int32 SERIAL_MISSION = 7;                  //일반 업적 보상
		const int32 MISSION_COMPLETE_POINT_REWARD = 8;   //업적 완료 점수 보상
		const int32 DAILY_QUEST = 9;                     //일일 퀘스트
		const int32 MAIN_QUEST = 10;                     //메인 퀘스트
		const int32 SUB_QUEST = 11;                      //서브 퀘스트
		const int32 PVP_RESULT = 12;                     //일대일 대전 보상
		const int32 PVP_REWARD_BOX = 13;                 //일대일 대전 결투 상자 보상
		const int32 PVP_RANKING = 14;                    //일대일 대전 랭킹
		const int32 COUNTER_DUNGEON_WIN = 15;            //반격 던전 승리
		const int32 COUNTER_DUNGEON_WEEKLY = 16;         //반격 던전 주간 참여
		const int32 TEAM_MATCH_RESULT = 17;              //팀 대전 결과 보상
		const int32 TEAM_MATCH_REWARD_BOX = 18;          //팀 대전 결투 상자 보상
		const int32 TEAM_MATCH_RANKING = 19;             //팀 대전 랭킹 보상
		const int32 HERO_TOWER = 20;                     //영웅의 탑 승리 보상
		const int32 SOURCE_RAID = 21;                    //레이드 승리 보상
		const int32 TUTORIAL = 22;                       //튜토리얼
		const int32 ASSAULT_RESULT = 23;                 //점령전 결과 보상
		const int32 ASSAULT_REWARD_BOX = 24;             //점령전 결투 상자 보상
		const int32 ASSAULT_RANKING = 25;                //점령전 랭킹 보상
		const int32 ASSAULT_MVP = 26;                    //점령전 MVP 보상
		const int32 GUILD_BATTLE = 27;                   //길드전 참여 보상
		const int32 GUILD_REWARD_BOX = 28;               //길드전 상자 보상
		const int32 GUILD_SUPPORT = 29;                  //길드 지원
		const int32 FRIEND = 30;                         //친구(우정포인트)
		const int32 COLLECT = 31;                        //도감
		const int32 ACT_CLEAR = 32;                      //액트 클리어 보상
		const int32 BUY_SHOP = 33;                       //상점 구매
		const int32 SHOP_DRAW_MILEAGE = 34;              //장비 상점 마일리지
		const int32 SHOP_BUY_MILEAGE = 35;               //재화 상점 마일리지 보상
		const int32 PUSH = 36;                           //푸시 보상
		const int32 MAINTENANCE = 37;                    //점검 보상
		const int32 EVENT = 38;                          //이벤트 보상
		const int32 ADMIN = 39;                          //운영자 보상
		const int32 UNITY_SKILL_MISSION = 40;			 //결속 스킬
		const int32 HERO_TOWER_SWEEP = 41;			     //영웅의 탑 소탕 보상
		const int32 REST = 42;		     			     //휴식 보상
		const int32 COUPON = 43;		     			 //쿠폰 보상
		const int32 PROMOTION = 44;		     			 //프로모션 보상
		const int32 KAKAO_FRIEND_INVITE = 45;            //카카오 친구 초대 보상
		const int32 SHOP_CASH = 46;                      //상점에서 현금으로 구매
		const int32 SHOP_EVENT = 47;                     //상점에서 이벤트 형식으로 지급
		const int32 SHOP_PACKAGE = 48;                       // 상점에서 패키지 형식으로 지급
		const int32 SHOP_FLAT_RATE_PACKAGE_BONUS = 49;       // 상점에서 정액 상품 패키지 구매 시 보너스 형식으로 지급
		const int32 SHOP_LEVEL_UP_PACKAGE = 50;              // 레벨업 패키지 보상
		const int32 SHOP_REACH_LEVEL_PACKAGE_BUY = 51;       // 레벨 달성 패키지 구매 보상
		const int32 SHOP_REACH_LEVEL_PACKAGE_COMPLETE = 52;  // 레벨 달성 패키지 달성 보상
		const int32 SHOP_PACKAGE_PERIOD_BONUS = 53;          // 패키지 기간 지급 보너스(기간 동안 모두 지급 받으면 받는 보너스)
		const int32 FAME_PRAISE_REWARD = 54;			 //명예 포인트 경배 보낸 보상
		const int32 FAME_RAISED_RANKING_REWARD = 55;     //명예 포인트 경배 받은 횟수 랭킹 보상
		const int32 FAME_RANKER_REWARD = 56;			 //명예 포인트 모드별 랭킹 달성 보상
		const int32 BUY_MAGIC_SHOP = 57;				 //마법상점 구매
		const int32 BEGINNER_ATTENDANCE = 58;			 //신규 유저 출석 보상
		const int32 MAKE_DONATION = 59;                  //기부하기
		const int32 DONATION_POINT_REWARD = 60;          //기부 포인트 별 보상
		const int32 DONATION_POINT_RANKING = 61;         //기부시즌 랭킹 보상
		const int32 DIMENSION_RANKING = 62;				 //차원의 균열 랭킹 보상
		const int32 FAIRY_GIFT = 63;					 //페어리 선물 보상
		const int32 WELCOME = 64;						 //신규 유저 우편 보상
		const int32 GUILD_REFUND = 65;					 //길드 탈퇴 스킬비용 보상
	}

	namespace B2HallOfFameType
	{
		const int32 HIGHEST_GRADE = 1;
		const int32 MOST_WINS = 2;
		const int32 MOST_CONTINUE_WINS = 3;
	}

	namespace B2DayOfWeek
	{
		const int32 SUNDAY = 1;
		const int32 MONDAY = 2;
		const int32 TUESDAY = 3;
		const int32 WEDNESDAY = 4;
		const int32 THURSDAY = 5;
		const int32 FRIDAY = 6;
		const int32 SATURDAY = 7;
	}

	namespace B2CounterDungeon
	{
		namespace TYPE
		{
			const int32 ARMOR = 1;
			const int32 WEAPON = 2;
			const int32 ACCESSORY = 3;
			const int32 GOLD = 4;
		}

		namespace DIFFICULTY
		{
			const int32 EASY = 1;
			const int32 NORMAL = 2;
			const int32 HARD = 3;
		}
	}

	namespace B2RewardType
	{
		const int32 GOLD = 1;
		const int32 GEM = 2;
		const int32 BLADE_POINT = 3;
		const int32 ITEM = 4;
		const int32 GENERAL_LOTTERY = 5;
		const int32 SELECTIVE_LOTTERY = 6;
		const int32 BUFF_TICKET = 7;
		const int32 HERO_PIECE = 8;
		const int32 SELECTIVE_ITEM_LOTTERY = 9;
		const int32 RANK_PROMOTION_POINT = 10;
		const int32 EXP = 11;
		const int32 DARK_CRYSTAL = 12;
		const int32 SOCIAL_POINT = 13;
		const int32 FIXED_GRADE_LOTTERY = 14;
		const int32 STAGE_BOOST_TICKET = 15;
		const int32 HOT_TIME_GOLD_BUFF_TICKET = 16;
		const int32 HOT_TIME_EXP_BUFF_TICKET = 17;
		const int32 SHOP_TEN_LOTTERY_TICKET = 18;
		const int32 FAME_POINT = 19;
		const int32 SELECTIVE_AETHER_LOTTERY = 20;
		const int32 PREFIX_SELECTIVE_LOTTERY = 21;
		const int32 STAGE_AUTO_CLEAR_TICKET = 22;
		const int32 COSTUME = 23;
		const int32 TOTEM = 24;
		const int32 TOTEM_PREFIX_LOTTERY = 25;
		const int32 ADVANCED_RANK_PROMOTION_POINT = 26;
	}

	namespace B2QuestSlotType 
	{
		const int32 MAIN1 = 1;
		const int32 MAIN2 = 2;
		const int32 SUB1 = 10;
		const int32 SUB2 = 11;
	}

	namespace B2GuildJoinOption
	{
		const int32 PUBLIC = 1;
		const int32 PRIVATE = 2;
	}

	namespace B2GuildMemberRank
	{
		const int32 MASTER = 1;
		const int32 SUB_MASTER = 2;

		const int32 NORMAL = 99;
	}

	namespace B2MissionType
	{
		const int32 DAILY = 1;
		const int32 WEEKLY = 2;
		const int32 MONTHLY = 3;
		const int32 SERIAL = 4;
		const int32 FEATURED = 5;
	}

	namespace B2MissionStateType
	{
		const int32 PROGRESS = 1;
		const int32 COMPLETED = 2;
		const int32 REWARDED = 3;
	}

	namespace B2StageRepeatMode
	{
		const int32 NORMAL = 0;
		const int32 REPEATED = 1;
		const int32 CONTINUOUS = 2;
	}

	namespace B2StageDifficulty
	{
		const int32 NORMAL = 1;
		const int32 HARD = 2;
		const int32 VERY_HARD = 3;
		const int32 HELL = 4;
	}

	namespace B2StageClearProperty
	{
		const int32 CLEAR_DEFAULT = 0;
		const int32 CLEAR_IN_TIME = 1;
		const int32 CLEAR_ALL_SURVIVAL = 2;
	}

	namespace B2StageFailReason
	{
		const int32 DEFEAT = 1;
		const int32 GIVEUP = 2;
	}

	namespace B2HeroTowerMapType
	{
		const int32 UNDER = 1;
		const int32 MIDDLE = 2;
		const int32 UPPER = 3;
		const int32 BOSS = 4;
	}

	namespace B2RaidRoomType
	{
		const int32 QUICK_JOIN = 1;
		const int32 CREATE_PUBLIC_ROOM = 2;
		const int32 CREATE_PRIVATE_ROOM = 3;
		const int32 REJOIN = 4;
	}

	namespace B2DimensionResult
	{
		const int32 CLEAR = 0;

		const int32 DEFEAT = 1;
		const int32 GIVEUP = 2;
	}

	namespace B2ShopDrawPriceType
	{
		const int32 GEM = 1;
		const int32 SOCIAL_POINT = 2;
		const int32 SHOP_TEN_LOTTERY_TICKET = 3;
	}

	namespace B2ShopMileageType
	{
		const int32 LOTTERY = 1;
		const int32 BLADE_POINT = 2;
		const int32 GEM = 3;
		const int32 GOLD = 4;
		const int32 DARK_CRYSTAL = 5;
		const int32 PACKAGE = 6;
	}

	namespace B2EquipCategory
	{
		const int32 NONE = 0;
		const int32 WEAPON = 1;
		const int32 ARMOR = 2;
		const int32 ACCESSORY = 3;
		const int32 WEAPON_ESSENCE = 16;
		const int32 ARMOR_ESSENCE = 17;
		const int32 ACCESSORY_ESSENCE = 18;
		const int32 WEAPON_SURPASS_ANVIL = 25;
		const int32 ARMOR_SURPASS_ANVIL = 26;
		const int32 ACCESSORY_SURPASS_ANVIL = 27;
		const int32 COSTUME_WEAPON = 201;
		const int32 COSTUME_HAT = 202;
		const int32 COSTUME_ARMOR = 203;
	}

	namespace B2ShopBuyPriceType
	{
		const int32 GEM = 1;
		const int32 SOCIAL_POINT = 2;
		const int32 DARK_CRYSTAL = 3;
		const int32 CASH = 4;
		const int32 GOLD = 5;
		const int32 FAME_POINT = 6;
	}

	namespace B2ShopProductType
	{
		const int32 BLADE_POINT = 1;
		const int32 GEM = 2;
		const int32 GOLD = 3;
		const int32 ITEM = 4;
		const int32 GENERAL_LOTTERY = 5;
		const int32 SELECTIVE_LOTTERY = 6;
		const int32 PACKAGE = 7;
		const int32 DARK_CRYSTAL = 8;
		const int32 GOLD_BUFF_TICKET = 9;
		const int32 EXP_BUFF_TICKET = 10;
		const int32 STAGE_BOOST_TICKET = 11;
		const int32 SHOP_TEN_LOTTERY_TICKET = 12;
		const int32 SELECTIVE_AETHER_LOTTERY = 13;
		const int32 PREFIX_SELECTIVE_LOTTERY = 14;
		const int32 STAGE_AUTO_CLEAR_TICKET = 15;
		const int32 COSTUME = 16;
		const int32 TOTEM = 17;
		const int32 TOTEM_PREFIX_LOTTERY = 18;
	}

	namespace B2ShopMarkType
	{
		const int32 MARK_NONE = 0;
		const int32 MARK_NEW = 1;
		const int32 MARK_HOT = 2;
		const int32 MARK_HIT = 3;
		const int32 MARK_LIMITED = 4;
	}

	namespace B2ShopType
	{
		const int32 SHOP_PACKAGE = 1;
		const int32 SHOP_DARK_CRYSTAL = 2;
		const int32 SHOP_GOODS = 3;
		const int32 SHOP_FAME_POINT = 4;
	}

	namespace B2ShopRestrictType
	{
		const int32 SHOP_NONE = 0;
		const int32 SHOP_DAY = 1;
		const int32 SHOP_WEEK = 2;
		const int32 SHOP_MONTH = 3;
		const int32 SHOP_NUMBER = 4;
	}
	
	namespace B2CollectionItemState 
	{
		const int32 COLLECTED = 1;
		const int32 ENHANCED = 2;
		const int32 REWARDED = 3;
	}

	namespace B2CollectionCategory
	{
		const int32 SET = 1;
		const int32 WEAPON = 2;
		const int32 ARMOR = 3;
		const int32 ACCESSORY = 4;
		const int32 WING = 5;
	}

	namespace B2RaidDifficulty
	{
		const int32 NORMAL = 1;
		const int32 HARD = 2;
		const int32 VERY_HARD = 3;
	}

	namespace B2DailyAttendanceType
	{
		const int32 GENERAL = 1;
		const int32 RETURNED = 2;
		const int32 BEGINNER = 3;
	}

	namespace B2RoomCloseReason
	{
		const int32 START_PVP = 1;
		const int32 START_RAID = 2;
		const int32 BREAK_RAID = 3;
		const int32 START_ASSAULT = 4;
	}

	namespace B2GuildInviteState
	{
		const int32 NONE = 0;
		const int32 JOINED_GUILD = 1;
		const int32 INVITED = 2;
		const int32 NON_INVITING = 3;
		const int32 INVITABLE = 4;
	}

	namespace B2ItemQuality
	{
		const int32 COMMON = 1;
		const int32 UNCOMMON = 2;
		const int32 RARE = 3;
		const int32 EPIC = 4;
		const int32 LEGEND = 5;
	}

	namespace B2AttributeChange
	{
		const int32 INC_ATTACK = 1;
		const int32 INC_DEFENCE = 2;
		const int32 INC_HEALTH = 3;
	}

	namespace B2GroupPrefix
	{
		const int32 SACRED = 1;
		const int32 MYSTIC = 2;
		const int32 STEEL = 3;
		const int32 SILENT = 4;
		const int32 GROWTH = 5;
		const int32 DUEL = 6;
		const int32 HEAVEN = 7;
		const int32 ETERNAL = 8;
		const int32 COLD = 10;
		const int32 CORRUPT = 11;
		const int32 SUN = 12;
		const int32 BEACH = 13;
		const int32 STUDY = 14;
		const int32 CHRISTMAS = 15;
		const int32 NEW_YEAR = 16;
		const int32 HALLOWEEN = 17;
		const int32 NON_EQUIP = 9999;
	}

	namespace B2ContentsModeState
	{
		const int32 NONE = 0;
		const int32 RUN = 1;
		const int32 BLOCK_ENTER = 2;
		const int32 MODE_FINISH = 3;
		const int32 SETTLE_BEGIN = 4;
		const int32 SETTLE_END = 5;

		const int32 SYSTEM_CHECK = 99;
	}

	namespace B2ContentsMode
	{
		const int32 NONE = 0;
		const int32 STAGE = 1;
		const int32 PVP_DUEL = 2;
		const int32 PVP_TEAM = 3;
		const int32 ASSAULT = 4;
		const int32 GUILD = 5;
		const int32 HERO_TOWER = 6;
		const int32 RAID = 7;
		const int32 PROMOTION = 8;
		const int32 COUNTER = 9;
		const int32 FRIEND = 10;
		const int32 INVENTORY = 11;
		const int32 MISSION = 12;
		const int32 SHOP = 13;
		const int32 DONATION = 14;
		const int32 DIMENSION = 15;
		const int32 COSTUME = 16;
	}

	namespace B2LotteryGrade
	{
		const int32 NORMAL = 1;
		const int32 SPECIAL = 2;
		const int32 PREMIUM = 3;
		const int32 SIGNATURE = 4;
		const int32 PLATINUM = 5;
		const int32 ANVIL1 = 6;
		const int32 ANVIL2 = 7;
		const int32 ANVIL3 = 8;
		const int32 SIGNATURE_1 = 9;
	}


	namespace B2AssaultMatchResult
	{
		const int32 DRAW = 1;
		const int32 WIN = 2;
		const int32 LOSE = 3;
	}

	namespace B2DuelMode
	{
		const int32 DUEL_MODE_NONE = 0;
		const int32 DUEL_MODE_PVP = 1;
		const int32 DUEL_MODE_TEAM = 2;
		const int32 DUEL_MODE_ASSAULT = 3;
	}

	namespace B2RewardBoxGrade
	{
		const int32 REWARD_BOX_NONE = 0;
		const int32 REWARD_BOX_BRONZE = 1;
		const int32 REWARD_BOX_SILVER = 2;
		const int32 REWARD_BOX_GOLD = 3;
		const int32 REWARD_BOX_PLATINUM = 4;
		const int32 REWARD_BOX_DIAMOND = 5;
		const int32 REWARD_BOX_MASTER = 6;
	}

	namespace B2ChatType
	{
		const int32 NOTICE = 0;
		const int32 CHANNEL = 1;
		const int32 GUILD_NOTICE = 2;
		const int32 GUILD = 3;
		const int32 ONE_TO_ONE = 4;
	}

	namespace B2ChatNotifyType
	{
		const int32 GUILD_JOIN = 0;
		const int32 GUILD_LEAVE = 1;
		const int32 GUILD_KICK = 2;
		const int32 GUILD_START_BATTLE = 3;
		const int32 GUILD_END_BATTLE = 4;
		const int32 GUILD_CHANGE_NOTICE = 5;
		const int32 GET_ITEM = 6;
		const int32 RAID_START = 7;
		const int32 RAID_END = 8;
		const int32 ASSAULT_START = 9;
		const int32 ASSAULT_END = 10;
		const int32 GUILD_SUPPORT = 11;
		const int32 GUILD_SUPPORT_COMPLETE = 12;
		const int32 ENHANCE_ITEM = 13;
	}

	namespace B2ItemGetRouteType
	{
		const int32 BUY_SHOP = 1;
		const int32 LOTTERY = 2;
		const int32 UPGRADE_ITEM = 3;
		const int32 COMPOSE_ITEM = 4;
		const int32 DISMANTLE_AETHER = 5;
		const int32 DISMANTLE_ITEM = 6;
		const int32 MAKE_DONATION = 7;
		const int32 DISMANTLE_TOTEM = 8;
		const int32 DISMANTLE_COSTUME = 9;
		const int32 FACTORY_MAKE = 10;
		const int32 FACTORY_UPGRADE = 11;
		const int32 UNSEAL_BOX = 12;
		const int32 ETC = 99;
	}

	namespace B2RewardPushType
	{
		const int32 MAIL = 1;
		const int32 DIRECT = 2;
	}

	namespace B2AetherType
	{
		const int32 ALL = 0;
		const int32 THUNDERSTORM = 1;
		const int32 DESTROY = 2;
		const int32 INSIGHT = 3;
		const int32 RAGE = 4;
		const int32 DESPAIR = 5;
		const int32 RESTRAINT = 6;
		const int32 RESIST = 7;
		const int32 REVENGE = 8;
		const int32 PROTECT = 9;
		const int32 PASSION = 10;
		const int32 OBLIVION = 11;
		const int32 FREEZING = 12;
	}

	namespace B2AetherTierType
	{
		const int32 SEAL = 1;
		const int32 ANTI_DEVIL = 2;
		const int32 EXTERMINATE_DEVIL = 3;
		const int32 ETERNAL_LIFE = 4;
	}

	namespace B2AetherSetOptionType
	{
		const int32 THUNDER_DAMAGE = 1001;
		const int32 IMMOBILIZE = 1002;
		const int32 SHIELD = 1003;
		const int32 FIRE_RANGE = 1004;
		const int32 METEOR_DAMAGE = 1005;
		const int32 SKILL_COOLTIME = 1006;
		const int32 SKILL_GAGE = 1007;
		const int32 STUN = 1008;
		const int32 SUPER_ARMOR = 1009;
		const int32 DAMAGE_REFLECT = 1010;
		const int32 PROHIBIT_BEHAVIOR = 1011;
		const int32 FREEZE_RANGE = 1012;
	}

	namespace B2UnitySkillEventType
	{
		const int32 EQUIP_ITEM = 1;
		const int32 EQUIP_ITEM_All = 2;
		const int32 REACH_LEVEL = 3;
	}

	namespace B2RollingBannerType
	{
		const int32 PACKAGE = 1;
		const int32 EVENT = 2;
		const int32 ATTENDANCE_EVENT = 3;
	}

	namespace B2GameEventType
	{
		const int32 CLEAR_DUNGEON = 1; // 던전 클리어 (condition1 : 던전 아이디, condition1 : 클래스 타입)
		const int32 COMPOSE_EQUIPMENT = 2; // 장비 합성하기(condition1 : 합성 후 아이템 등급)
		const int32 UPGRADE_EQUIPMENT = 3; // 장비 승급하기(condition1 : 승급 후 아이템 등급)
		const int32 LEVEL_UP_EQUIPMENT = 4; // 장비 강화하기 (condition1 : 도달 레벨)
		const int32 SURPASS_EQUIPMENT = 5; // 장비 초월하기
		const int32 COMPLETE_DAILY_MISSION = 6; // 일일 업적 달성하기
		const int32 COMPLETE_WEEKLY_MISSION = 7; // 주간 업적 달성하기
		const int32 GET_ITEM = 8; // 아이템 획득하기(condition1 : 아이템 아이디)
		const int32 GET_QUEST_ITEM = 9; //스테이지에서 퀘스트 아이템 획득
										// (condition1 : 스테이지 아이디; condition2 : 퀘스트 아이템 아이디, condition3 : 클래스 타입)
		const int32 GET_GOLD = 10; // 골드 획득하기
		const int32 REACH_CHARACTER_POWER = 11; //캐릭터 전투력 달성
		const int32 REACH_GOLD = 12; // 누적 골드 달성
		const int32 REACH_LEVEL = 13;  // 연속 : 레벨업 달성(condition1 : 클래스, value 레벨업 후 현재 레벨)
		const int32 KILL_MONSTER = 14;   //몬스터 몇 마리 처리
		const int32 KILL_SPECIFIC_MONSTER_STAGE = 15;   //특정 스테이지에서 특정 몬스터 몇 마리 처리
		const int32 JOIN_MATCH_MAKING = 16; //일대일 대전 참여
		const int32 JOIN_TEAM_MATCH_MAKING = 17;  //팀대전 참여
		const int32 JOIN_DUNGEON_MODE = 18; // 던전 모드 참여
		const int32 JOIN_HERO_TOWER = 19; // 영웅의 탑 참여
		const int32 JOIN_GUILD_BATTLE = 20; // 길드전 참여
		const int32 JOIN_COUNTER_DUNGEON = 21; // 반격 던전 참여
		const int32 JOIN_RAID = 22; //레이드 참여
		const int32 JOIN_ASSAULT = 23; //난투전 참여
		const int32 SEND_SOCIAL_POINT = 24; // 우정 포인트 보내기
		const int32 QUEST_ACCEPTED = 25;     //퀘스트 수락
		const int32 ENHANCE_RANK_NODE = 26; //진급 노드 강화시도
		const int32 UPGRADE_WING = 27;      //날개 승급
		const int32 RESURRECTION = 28;      // 부활하기
		const int32 BUY_SKILL_POINT = 29;    //스킬 포인트 구매
		const int32 SELL_EQUIPMENT = 30;    //장비 판매하기
		const int32 DRAW_EQUIPMENT = 31;    //장비뽑기
		const int32 ENHANCE_EQUIPMENT = 32; //장비 강화
		const int32 JOIN_MATCH_MODE = 33; //대결 모드 참여
	}

	namespace B2GuildMercenaryDonationType
	{
		const int32 GOLD = 1;
		const int32 GEM = 2;
	}

	namespace B2MatchType
	{
		const int AI = 1;
		const int PLAYER = 2;
		const int TEAM = 3;
		const int RAID = 4;
		const int ASSAULT = 5;
		const int HEROTOWER = 6;
		const int DONATION = 7;
	}

	namespace B2RedDotHintType
	{
		const int32 RED_DOT_ALL = 0;
		const int32 RED_DOT_EQUIP = 1;
		const int32 RED_DOT_COLLECTION = 2;
		const int32 RED_DOT_MISSION = 3;
		const int32 RED_DOT_FRIEND = 4;
		const int32 RED_DOT_ADVENTURE = 5;
		const int32 RED_DOT_DUNGEON = 6;
		const int32 RED_DOT_PVP = 7;
		const int32 RED_DOT_GUILD = 8;
		const int32 RED_DOT_SHOP = 9;
		const int32 RED_DOT_MAIL = 10;
		const int32 RED_DOT_OPTION = 11;
		const int32 RED_DOT_EVENT = 12;
		const int32 RED_DOT_SHOP_PACKAGE = 13;
		const int32 RED_DOT_MAGIC_SHOP = 14;
		const int32 RED_DOT_HALL_OF_FAME = 15;
		const int32 RED_DOT_FAIRY = 16;
		const int32 RED_DOT_DONATION = 17;
		const int32 RED_DOT_ETC = 100;
	}

	namespace B2EventCategory
	{
		const int POINT_REWARD = 1;
		const int HOT_TIME_BUFF = 2;
		const int LEVEL_UP = 3;
		const int SEASON_MISSION = 4;
		const int STAGE_CLEAR = 5;
		const int POINT_SHOP = 6;
		const int GEM_PURCHASE = 7;
		const int SHOP_LOTTERY = 8;
		const int SPEND_GEM = 9;
		const int SHOP_LOTTERY_SAVINGS = 10;
		const int DICE = 11;
	}

	namespace B2EventPointShopType
	{	// 포인트 구매(교환) 이벤트 세부 유형
		const int NORMAL = 0;			// 일반
		const int LIMITED = 1;			// 구매제한 유형
	}
	
	namespace B2HotTimeBuffType
	{
		const int32 GOLD = 1;
		const int32 EXP = 2;
	}

	namespace B2FriendInviteRewardType
	{
		const int INVITE_COUNT = 1;   // 누적 초대 보상
		const int JOINER_COUNT = 2;   // 누적 초대 참여 보상
	}

	namespace B2MainQuestType
	{
		const int MAIN = 1;
		const int SUB = 2;
	}

	namespace B2MarketPurchaseState
	{
		const int REGISTERED = 1;		// 구매 정보 등록(시작)
		const int VERIFIED = 2;			// 구매 검증 완료
		const int COMPLETED = 3;		// 지급 완료
	}

	namespace B2MarketPurchaseCancel
	{
		const int CANCEL_CURRENT = 1;	// 유저 취소 처리
		const int CANCEL_PAST = 2;		// 미처리된 이전 정보 취소 처리
		const int ORDER_FAILURE = 3;	// 마켓 주문 실패
	}

	namespace B2ShopPackageCategory
	{
		const int BASIC = 1;             // 일반 패키지
		const int LEVEL_UP = 2;           // 레벨업 패키지
		const int REACH_LEVEL = 3;        // 레벨 달성 패키지
		const int FLAT_RATE = 4;          // 정액 패키지
	}

	namespace B2ShopPackageTabType
	{
		const int BASIC = 1;          //추천 패키지
		const int EQUIPMENT = 2;     //장비 패키지
		const int GROWTH = 3;        //성장 패키지
		const int HERO = 4;          //영웅 패키지
		const int BOOST = 5;         //부스트 패키지
		const int LEVEL_UP = 6;      //레벨업 패키지
		const int FLAT_RATE = 7;      //정액 패키지
		const int COSTUME = 8;		 //코스튬
	}
	
	namespace B2ReportModeType
	{
		const int PVP = 1;
		const int ASSAULT = 2;
		const int RAID = 3;
		const int CHAT = 4;
	}

	namespace B2RaidType
	{
		const int HARANG = 1;
		const int SARGON = 2;
	}

	namespace B2EventAttanceBonusType
	{
		const int NONE				= 0;
		const int FLAT_RATE_ALL		= 1;
	}

	namespace B2DiceEventCellType
	{
		const int START = 1;
		const int REWARD = 2;
		const int TIMES = 3;
	}

	namespace B2FairyType
	{
		const int NIECE = 1;			// 숲의 요정 니스
		const int PANG = 2;				// 숲의 요정 팡
		const int RIO = 3;				// 숲의 요정 리오
	}

	namespace B2TotemEquipSlot
	{
		const int NONE = 0;				// 착용정보 없음
		const int TOP = 1;				// 상단
		const int MIDDLE = 2;			// 중단
		const int BOTTOM = 3;			// 하단
	}

	namespace B2TotemGrade
	{
		const int NORMAL = 1;			// 일반
		const int MAGIC = 2;			// 마법
		const int RARE = 3;				// 희귀
		const int HERO = 4;				// 영웅
		const int LEGEND = 5;			// 전설
		const int RELIC = 6;			// 유물
	}

	namespace B2TotemBasePrefix
	{
		const int VALOROUS = 1;			// 용맹의
		const int PATRON = 2;			// 수호의
		const int VIGOROUS = 3;			// 활력의
	}

	namespace B2TotemRefinePrefix
	{
		const int ECLIPSE = 1;			// 빛을 잃은
		const int SHINING = 2;			// 빛을 담은
		const int STRONG = 3;			// 강력한
		const int SACRED = 4;			// 신성한
	}

	namespace B2Refinestatus
	{
		const int NONE = 0;				// 제련이력 없음
		const int SUCCESS = 1;			// 제련 성공
		const int FAIL = 2;				// 제련 실패
	}

	namespace B2TotemRefineCostType
	{
		const int GOLD = 1;				// 골드
		const int DIMENSIONAL_PIECE = 2;// 차원의 조각
	}

	namespace B2FactoryLimitType
	{
		const int TRY = 1;		// 시도
		const int SUCCESS = 2;	// 성공
	}

	namespace B2FactoryLimitTimeType
	{
		const int ONCE = 1;		// 영구
		const int DAY = 2;		// 일간
		const int WEEK = 3;		// 주간
		const int MONTH = 4;	// 월간
		const int NONE = 5;		// 무제한
	}

	namespace B2FactoryResultType
	{
		const int FAIL = 1;			// 실패
		const int SUCCESS = 2;		// 성공
		const int HUGE_SUCCESS = 3;	// 대성공
	}

	namespace B2FactoryVisibleType
	{
		const int CLOSE = 1;	// 비공개
		const int OPEN = 2;		// 공개
		const int PERIOD = 3;	// 기간공개
	}

	namespace B2DonationConsumableType
	{
		const int GEM = 1;
		const int BLADE_POINT = 2;
		const int RANK_PROMOTION_POINT = 3;
		const int HERO_PIECE = 4;
		const int MAGIC_CRYSTAL = 11;
		const int TEARS_WIND = 12;
		const int FEATHER_ARCHANGEL = 13;
		const int WEAPON_UPGRADE_STONE = 21;
		const int ARMOR_UPGRADE_STONE = 22;
		const int ACCESSORY_UPGRADE_STONE = 23;
		const int WEAPON_ENHANCE_SCROLL = 24;
		const int ARMOR_ENHANCE_SCROLL = 25;
		const int ACCESSORY_ENHANCE_SCROLL = 26;
		const int BLESSED_WEAPON_ENHANCE_SCROLL = 27;
		const int BLESSED_ARMOR_ENHANCE_SCROLL = 28;
		const int BLESSED_ACCESSORY_ENHANCE_SCROLL = 29;
		const int RAMIEL_PROTECTION = 30;
		const int AETHER_PIECE = 31;
		const int CALIGULA_PIECE = 32;
		const int LILLIS_PIECE = 33;
		const int BEELZEBUB_PIECE = 34;
		const int AZRAEL_PIECE = 35;
		const int NIDHOGG_PIECE = 36;
		const int APOCALYPSE_PIECE = 37;
		const int LIGHT_LAMP = 41;
		const int ADVANCED_RANK_PROMOTION_POINT = 42;

		const int UNDEFINED = 9999;
	}

	namespace B2DonationCostType
	{
		const int32 GEM = 1;
		const int32 GOLD = 2;
	}

	namespace B2PowerCategory
	{
		const int TOTAL_POWER = 0;        // 캐릭터 전투력 총합

		const int STAT_POWER = 1;         // 캐릭터 스텟에 의한 전투력
		const int ITEM_POWER = 2;         // 장착 아이템에 의한 전투력
		const int COSTUME_POWER = 3;      // 장착 코스츔에 의한 전투력
		const int SET_ITEM_POWER = 4;     // 장착 아이템의 세트효과에 의한 전투력
		const int RANK_POWER = 5;         // 캐릭터 진급에 의한 전투력
		const int WING_POWER = 6;         // 캐릭터 날개에 의한 전투력
		const int RELIC_POWER = 7;        // 캐릭터 유물에 의한 전투력
		const int SKILL_POWER = 8;        // 캐릭터 스킬에 의한 전투력
		const int UNITY_SKILL_POWER = 9;  // 결속 스킬에 의한 전투력
		const int AETHER_POWER = 10;      // 캐릭터의 에테르에 의한 전투력
		const int FAIRY_POWER = 11;       // 페어리 축복 효과에 의한 전투력
		const int TOTEM_POWER = 12;		  // 장착 토템에 의한 전투력
		const int SET_COSTUME_POWER = 13; // 착용 코스튬의 세트효과에 의한 전투력
	};

	namespace B2SealSlotOpenPriceType
	{
		const int32 GEM = 1;
		const int32 GOLD = 2;
	}

	namespace B2SealPriceType
	{
		const int32 GEM = 1;
		const int32 GOLD = 2;
	}

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RawReward)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, count)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RewardRandomCount)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, min_count)
		B2_DECLARE_FIELD(2, int32, max_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GlobalAccountInfo)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, FString, nickname)
		B2_DECLARE_FIELD(2, int32, type)						// 계정타입, @see B2AccountType
		B2_DECLARE_FIELD(3, int32, status)						// 계정상태, @see B2AccountStatus
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2AccountInfo)
		B2_DECLARE_FIELD(0, FString, name)
		B2_DECLARE_FIELD(1, int64, account_id)
		B2_DECLARE_FIELD(2, int32, cash)
		B2_DECLARE_FIELD(3, int32, money)
		B2_DECLARE_FIELD(4, int32, blade_point)
		B2_DECLARE_FIELD(5, int32, blade_point_max)
		B2_DECLARE_FIELD(6, int32, blade_point_charge_lefttime_second)
		B2_DECLARE_FIELD(7, int32, inventory_aether_buy_count)
		B2_DECLARE_FIELD(8, int32, inventory_consumable_slot_count)
		B2_DECLARE_FIELD(9, int32, dark_crystal)
		B2_DECLARE_FIELD(10, int32, stage_boost_ticket)
		B2_DECLARE_FIELD(11, int32, shop_ten_lottery_ticket)
		B2_DECLARE_FIELD(12, int64, guild_id)
		B2_DECLARE_FIELD(13, int32, fame_point)
		B2_DECLARE_FIELD(14, int32, social_point)
		B2_DECLARE_FIELD(15, int32, last_play_stage_id)
		B2_DECLARE_FIELD(16, int32, last_clear_stage_id_normal)
		B2_DECLARE_FIELD(17, int32, last_clear_stage_id_hard)
		B2_DECLARE_FIELD(18, int32, last_clear_stage_id_very_hard)
		B2_DECLARE_FIELD(19, int32, last_clear_stage_id_hell)
		B2_DECLARE_FIELD(20, int32, attendance_day)
		B2_DECLARE_FIELD(21, int32, attendance_type)
		B2_DECLARE_FIELD(22, int64, nextAttendance_time)
		B2_DECLARE_FIELD(23, int32, daily_play_time_in_sec)
		B2_DECLARE_FIELD(24, int32, daily_play_time_reward_index)
		B2_DECLARE_FIELD(25, int64, daily_play_time_update_time)
		B2_DECLARE_FIELD(26, int64, rest_time)
		B2_DECLARE_FIELD(27, int64, leave_guild_time)
		B2_DECLARE_FIELD(28, int64, kicked_guild_id)
		B2_DECLARE_FIELD(29, int64, gold_buff_end_time)
		B2_DECLARE_FIELD(30, int64, exp_buff_end_time)
		B2_DECLARE_FIELD(31, int64, gold_buff_ticket)
		B2_DECLARE_FIELD(32, int64, exp_buff_ticket)
		B2_DECLARE_FIELD(33, int32, stage_auto_clear_ticket)
		B2_DECLARE_FIELD(34, int64, next_fairy_gift_time)
	DECLARE_END(35)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2LightAccount)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, int32, cash)
		B2_DECLARE_FIELD(2, int32, money)
		B2_DECLARE_FIELD(3, int32, blade_point)
		B2_DECLARE_FIELD(4, int32, dark_crystal)
		B2_DECLARE_FIELD(5, int32, social_point)
		B2_DECLARE_FIELD(6, int32, stage_boost_ticket)
		B2_DECLARE_FIELD(7, int32, shop_ten_lottery_ticket)
		B2_DECLARE_FIELD(8, int32, fame_point)
		B2_DECLARE_FIELD(9, int32, stage_auto_clear_ticket)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SealSlotState)
		B2_DECLARE_FIELD(0, int32, seal_slot)
		B2_DECLARE_FIELD(1, bool, opened)
		B2_DECLARE_FIELD(2, int32, option_id)
		B2_DECLARE_FIELD(3, float, option_value)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FactoryLimitStatus)
		B2_DECLARE_FIELD(0, int32, limit_id)
		B2_DECLARE_FIELD(1, int32, limit_type)
		B2_DECLARE_FIELD(2, int32, limit_time_type)
		B2_DECLARE_FIELD(3, int32, left_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FactoryPeriodicStatus)
		B2_DECLARE_FIELD(0, int32, factory_id)
		B2_DECLARE_FIELD(1, int64, preview_time)
		B2_DECLARE_FIELD(2, int64, start_time)
		B2_DECLARE_FIELD(3, int64, end_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFactoryInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, factory_type)
		B2_DECLARE_FIELD(2, int32, factory_tab)
		B2_DECLARE_FIELD(3, int32, group_id)
		B2_DECLARE_FIELD(4, int32, group_sort)
		B2_DECLARE_FIELD(5, int32, item_sort)
		B2_DECLARE_FIELD(6, int32, limit_id)
		B2_DECLARE_FIELD(7, int32, character_type)
		B2_DECLARE_FIELD(8, int32, gold_cost)
		B2_DECLARE_FIELD(9, int32, gem_cost)
		B2_DECLARE_FIELD(10, int32, dark_crystal_cost)
		B2_DECLARE_FIELD(11, int32, success_rate)
		B2_DECLARE_FIELD(12, int32, huge_success_rate)
		B2_DECLARE_FIELD(13, int32, fail_rate)
		B2_DECLARE_FIELD(14, int32, huge_success_add_item_template_id)
		B2_DECLARE_FIELD(15, int32, huge_success_add_item_count)
		B2_DECLARE_FIELD(16, int32, result_item_template_id)
		B2_DECLARE_FIELD(17, int32, result_item_count)
		B2_DECLARE_FIELD(18, int32, main_material_template_id)
		B2_DECLARE_FIELD(19, int32, main_material_cond_reinforce)
		B2_DECLARE_FIELD(20, int32, main_material_cond_level)
		B2_DECLARE_FIELD(21, int32, main_material_count)
		B2_DECLARE_FIELD(22, int32, main_material_fail_refund_rate)
		B2_DECLARE_FIELD(23, int32, material_template_id1)
		B2_DECLARE_FIELD(24, int32, material_count1)
		B2_DECLARE_FIELD(25, int32, material_fail_refund_rate1)
		B2_DECLARE_FIELD(26, int32, material_template_id2)
		B2_DECLARE_FIELD(27, int32, material_count2)
		B2_DECLARE_FIELD(28, int32, material_fail_refund_rate2)
		B2_DECLARE_FIELD(29, int32, material_template_id3)
		B2_DECLARE_FIELD(30, int32, material_count3)
		B2_DECLARE_FIELD(31, int32, material_fail_refund_rate3)
		B2_DECLARE_FIELD(32, int32, material_template_id4)
		B2_DECLARE_FIELD(33, int32, material_count4)
		B2_DECLARE_FIELD(34, int32, material_fail_refund_rate4)
		B2_DECLARE_FIELD(35, int32, visible_type)
	DECLARE_END(36)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFactoryMaterialTransInfo)
		B2_DECLARE_FIELD(0, int32, factory_id)
		B2_DECLARE_FIELD(1, int32, trans_main_material_template_id)
		B2_DECLARE_FIELD(2, int32, trans_main_material_count)
		B2_DECLARE_FIELD(3, int32, trans_material_template_id1)
		B2_DECLARE_FIELD(4, int32, trans_material_count1)
		B2_DECLARE_FIELD(5, int32, trans_material_template_id2)
		B2_DECLARE_FIELD(6, int32, trans_material_count2)
		B2_DECLARE_FIELD(7, int32, trans_material_template_id3)
		B2_DECLARE_FIELD(8, int32, trans_material_count3)
		B2_DECLARE_FIELD(9, int32, trans_material_template_id4)
		B2_DECLARE_FIELD(10, int32, trans_material_count4)
	DECLARE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFactoryLimitInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, limit_type)
		B2_DECLARE_FIELD(2, int32, limit_count)
		B2_DECLARE_FIELD(3, int32, limit_time_type)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSealBoxInfo)
		B2_DECLARE_FIELD(0, int32, seal_box_template_id)
		B2_DECLARE_FIELD(1, int32, reward_slot1)
		B2_DECLARE_FIELD(2, int32, reward_slot2)
		B2_DECLARE_FIELD(3, int32, reward_slot3)
		B2_DECLARE_FIELD(4, int32, reward_slot4)
		B2_DECLARE_FIELD(5, int32, reward_slot5)
		B2_DECLARE_FIELD(6, bool, is_rate_visible)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSealBoxRewardInfo)
		B2_DECLARE_FIELD(0, int32, seal_box_reward_id)
		B2_DECLARE_FIELD(1, int32, min_fixed_count)
		B2_DECLARE_FIELD(2, int32, max_fixed_count)
		B2_DECLARE_FIELD(3, bool, is_recycle)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSealBoxRewardRateInfo)
		B2_DECLARE_FIELD(0, int32, seal_box_reward_id)
		B2_DECLARE_FIELD(1, int32, reward_type)
		B2_DECLARE_FIELD(2, int32, template_id)
		B2_DECLARE_FIELD(3, int32, min_amount)
		B2_DECLARE_FIELD(4, int32, max_amount)
		B2_DECLARE_FIELD(5, int32, grade)
		B2_DECLARE_FIELD(6, int32, group_id)
		B2_DECLARE_FIELD(7, int32, equip_category)
		B2_DECLARE_FIELD(8, int32, equip_position)
		B2_DECLARE_FIELD(9, int32, quality)
		B2_DECLARE_FIELD(10, int32, enhance_level)
		B2_DECLARE_FIELD(11, int32, surpass_count)
		B2_DECLARE_FIELD(12, int32, reward_rate)
		B2_DECLARE_FIELD(13, bool, is_fixed_reward)
		B2_DECLARE_FIELD(14, int32, fixed_reward_rate)
		B2_DECLARE_FIELD(15, int32, effect_grade)
	DECLARE_END(16)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ItemServerInfo)
		B2_DECLARE_FIELD(0, int64, id)
		B2_DECLARE_FIELD(1, int32, template_id)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, int32, inventory_type)
		B2_DECLARE_FIELD(4, int32, equip_position)
		B2_DECLARE_FIELD(5, int32, amount)
		B2_DECLARE_FIELD(6, int32, grade)
		B2_DECLARE_FIELD(7, int32, quality)
		B2_DECLARE_FIELD(8, int32, random_option_id1)
		B2_DECLARE_FIELD(9, int32, random_option_id2)
		B2_DECLARE_FIELD(10, int32, random_option_id3)
		B2_DECLARE_FIELD(11, float, random_option_value1)
		B2_DECLARE_FIELD(12, float, random_option_value2)
		B2_DECLARE_FIELD(13, float, random_option_value3)
		B2_DECLARE_FIELD(14, int32, level)						// [1, 40]
		B2_DECLARE_FIELD(15, float, exp)						// [0, 100), decimalpoint is 2
		B2_DECLARE_FIELD(16, int32, enhance_level)
		B2_DECLARE_FIELD(17, int32, surpass_count)				// [0, 5]
		B2_DECLARE_FIELD(18, bool, locked)
		B2_DECLARE_FIELD(19, float, power)
		B2_DECLARE_FIELD(20, bool, is_new)
		B2_DECLARE_FIELD(21, float, md_main_attribute_value)
		B2_DECLARE_FIELD(22, int32, open_seal_slot_count)
		B2_DECLARE_FIELD(23, TArray<B2SealSlotStatePtr>, seal_slot_states)
	DECLARE_END(24)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ItemPreset)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, preset_id)
		B2_DECLARE_FIELD(3, int32, equip_position)
		B2_DECLARE_FIELD(4, int64, item_id)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CostumeServerInfo)
		B2_DECLARE_FIELD(0, int64, id)
		B2_DECLARE_FIELD(1, int32, template_id)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, int32, inventory_type)
		B2_DECLARE_FIELD(4, int32, equip_position)
		B2_DECLARE_FIELD(5, int32, amount)
		B2_DECLARE_FIELD(6, int32, grade)
		B2_DECLARE_FIELD(7, int32, quality)
		B2_DECLARE_FIELD(8, int32, random_option_id1)
		B2_DECLARE_FIELD(9, int32, random_option_id2)
		B2_DECLARE_FIELD(10, int32, random_option_id3)
		B2_DECLARE_FIELD(11, float, random_option_value1)
		B2_DECLARE_FIELD(12, float, random_option_value2)
		B2_DECLARE_FIELD(13, float, random_option_value3)
		B2_DECLARE_FIELD(14, int32, level)						// [1, 40]
		B2_DECLARE_FIELD(15, float, exp)						// [0, 100), decimalpoint is 2
		B2_DECLARE_FIELD(16, int32, enhance_level)
		B2_DECLARE_FIELD(17, int32, surpass_count)				// [0, 5]
		B2_DECLARE_FIELD(18, bool, locked)
		B2_DECLARE_FIELD(19, float, power)
		B2_DECLARE_FIELD(20, bool, is_new)
		B2_DECLARE_FIELD(21, float, md_main_attribute_value)
	DECLARE_END(22)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2AetherServerInfo)
		B2_DECLARE_FIELD(0, int64, id)							// 에테르 고유 아이디
		B2_DECLARE_FIELD(1, int32, type)						// 에테르 타입, @see B2AetherType
		B2_DECLARE_FIELD(2, int32, slot)						// 에테르 슬롯(shape)
		B2_DECLARE_FIELD(3, int32, character_type)				// 현재 사용중인 캐릭터 타입, @see B2CharacterType
		B2_DECLARE_FIELD(4, int32, level)						// 레벨
		B2_DECLARE_FIELD(5, int32, grade)						// 등급
		B2_DECLARE_FIELD(6, int32, tier)						// 생성 시 부여된 티어, @see B2AetherTierType
		B2_DECLARE_FIELD(7, int32, main_option_id)				// 메인 옵션 아이디
		B2_DECLARE_FIELD(8, float, main_option_value)			// 메인 옵션 값
		B2_DECLARE_FIELD(9, int32, sub_option_id1)				// 보조 옵션 종류 1
		B2_DECLARE_FIELD(10, int32, sub_option_id2)			// 보조 옵션 종류 2
		B2_DECLARE_FIELD(11, int32, sub_option_id3)			// 보조 옵션 종류 3
		B2_DECLARE_FIELD(12, float, sub_option_value1)			// 보조 옵션 값 1
		B2_DECLARE_FIELD(13, float, sub_option_value2)			// 보조 옵션 값 2
		B2_DECLARE_FIELD(14, float, sub_option_value3)			// 보조 옵션 값 3
		B2_DECLARE_FIELD(15, int32, sub_option_enhance_count1)	// 보조 옵션 강화 수치 1
		B2_DECLARE_FIELD(16, int32, sub_option_enhance_count2)	// 보조 옵션 강화 수치 2
		B2_DECLARE_FIELD(17, int32, sub_option_enhance_count3)	// 보조 옵션 강화 수치 3
		B2_DECLARE_FIELD(18, bool, locked)						// 잠금 상태 여부
		B2_DECLARE_FIELD(19, bool, is_new)						// 신규 여부
		B2_DECLARE_FIELD(20, float, calc_main_option_value)	// 계산된 최종 메인 옵션 값
		B2_DECLARE_FIELD(21, float, calc_sub_option_value1)	// 계산된 최종 보조 옵션 값 1
		B2_DECLARE_FIELD(22, float, calc_sub_option_value2)	// 계산된 최종 보조 옵션 값 2
		B2_DECLARE_FIELD(23, float, calc_sub_option_value3)	// 계산된 최종 보조 옵션 값 3
		B2_DECLARE_FIELD(24, int32, calc_tier)					// 계산된 최종 티어, @see B2AetherTierType
	DECLARE_END(25)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2TotemRefineState)
		B2_DECLARE_FIELD(0, int32, index)
		B2_DECLARE_FIELD(1, int32, status)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2TotemSubOptionDetail)
		B2_DECLARE_FIELD(0, int32, option_id)
		B2_DECLARE_FIELD(1, float, option_value)
		B2_DECLARE_FIELD(2, int32, refine_count)
		B2_DECLARE_FIELD(3, TArray<B2TotemRefineStatePtr>, refine_states)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2TotemServerInfo)
		B2_DECLARE_FIELD(0, int64, id)
		B2_DECLARE_FIELD(1, int32, template_id)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, int32, equip_slot)
		B2_DECLARE_FIELD(4, int32, grade)
		B2_DECLARE_FIELD(5, int32, main_option_id)
		B2_DECLARE_FIELD(6, float, main_option_value)
		B2_DECLARE_FIELD(7, TArray<B2TotemSubOptionDetailPtr>, sub_option_details)		// 순서대로 1, 2, 3번 옵션
		B2_DECLARE_FIELD(8, int32, refine_level)										// 제련 레벨 (옵션1, 2의 제련횟수 합)
		B2_DECLARE_FIELD(9, int32, refine_try_count)									// 제련 시도 횟수 (각 옵션의 제련 시도 횟수 합)
		B2_DECLARE_FIELD(10, int32, gold_success_rate)
		B2_DECLARE_FIELD(11, int32, piece_success_rate)
		B2_DECLARE_FIELD(12, bool, locked)
		B2_DECLARE_FIELD(13, float, power)
		B2_DECLARE_FIELD(14, bool, is_new)
	DECLARE_END(15)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CharacterSetOptionInfo)
		B2_DECLARE_FIELD(0, int32, selected_offense_set_option_id)
		B2_DECLARE_FIELD(1, int32, selected_defense_set_option_id)
		B2_DECLARE_FIELD(2, TArray<int32>, defense_set_option_ids)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CharacterPower)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, total_power)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SkillServerInfo)
		B2_DECLARE_FIELD(0, int32, skill_id)
		B2_DECLARE_FIELD(1, int32, skill_level)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SkillQuickSlot)
		B2_DECLARE_FIELD(0, int32, slot)
		B2_DECLARE_FIELD(1, int32, left_skill_point)
		B2_DECLARE_FIELD(2, float, skill_total_power)
		B2_DECLARE_FIELD(3, int32, using_skill_id_1)
		B2_DECLARE_FIELD(4, int32, using_skill_id_2)
		B2_DECLARE_FIELD(5, int32, using_skill_id_3)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSkillInfo)
		B2_DECLARE_FIELD(0, int32, skill_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, learn_level)
		B2_DECLARE_FIELD(3, int32, max_skill_level)
		B2_DECLARE_FIELD(4, bool, usable)
		B2_DECLARE_FIELD(5, int32, level_up_cost_factor)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSkillOption)
		B2_DECLARE_FIELD(0, int32, skill_id)
		B2_DECLARE_FIELD(1, int32, skill_level)
		B2_DECLARE_FIELD(2, int32, option_id)
		B2_DECLARE_FIELD(3, float, option_value)
		B2_DECLARE_FIELD(4, float, power)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSkillLevelUp)
		B2_DECLARE_FIELD(0, int32, skill_id)
		B2_DECLARE_FIELD(1, int32, skill_level)
		B2_DECLARE_FIELD(2, int32, req_skill_point)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemLevelupExpFactor)
		B2_DECLARE_FIELD(0, FString, type_name)
		B2_DECLARE_FIELD(1, int32, key_code)
		B2_DECLARE_FIELD(2, double, factor)
	DECLARE_END(3)
	 
	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemOptionAddValue)
		B2_DECLARE_FIELD(0, int32, option_id)
		B2_DECLARE_FIELD(1, float, add_value)
		B2_DECLARE_FIELD(2, float, power_factor)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemLevelupFactor)
		B2_DECLARE_FIELD(0, int32, level)
		B2_DECLARE_FIELD(1, double, factor)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemLevelupCost)
		B2_DECLARE_FIELD(0, int32, inventory_type)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, price_grade1)
		B2_DECLARE_FIELD(3, int32, price_grade2) 
		B2_DECLARE_FIELD(4, int32, price_grade3)
		B2_DECLARE_FIELD(5, int32, price_grade4)
		B2_DECLARE_FIELD(6, int32, price_grade5)
		B2_DECLARE_FIELD(7, int32, price_grade6)
		B2_DECLARE_FIELD(8, int32, price_grade7)
		B2_DECLARE_FIELD(9, int32, price_grade8)
		B2_DECLARE_FIELD(10, int32, price_grade9)
		B2_DECLARE_FIELD(11, int32, price_grade10)
		B2_DECLARE_FIELD(12, int32, price_grade11)
		B2_DECLARE_FIELD(13, int32, price_grade12)
	DECLARE_END(14)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemEnhanceCost)
		B2_DECLARE_FIELD(0, int32, enhance_level)
		B2_DECLARE_FIELD(1, int32, enhance_rate)
		B2_DECLARE_FIELD(2, int32, enhance_scroll_cost)
		B2_DECLARE_FIELD(3, int32, protection_scroll_cost)
		B2_DECLARE_FIELD(4, int32, money_cost)
		B2_DECLARE_FIELD(5, int32, exchange_cost_dark_crystal)
		B2_DECLARE_FIELD(6, int32, exchange_cost_gem)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdCostumeEnhanceCost)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, enhance_level)
		B2_DECLARE_FIELD(2, int32, money_cost)
		B2_DECLARE_FIELD(3, int32, weapon_material_id)
		B2_DECLARE_FIELD(4, int32, weapon_amount)
		B2_DECLARE_FIELD(5, int32, weapon_restore_amount)
		B2_DECLARE_FIELD(6, int32, armor_material_id)
		B2_DECLARE_FIELD(7, int32, armor_amount)
		B2_DECLARE_FIELD(8, int32, armor_restore_amount)
	DECLARE_END(9)



	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdExchangeSystemCost)
		B2_DECLARE_FIELD(0, int32, exchange_id)
		B2_DECLARE_FIELD(1, int32, count)
		B2_DECLARE_FIELD(2, int32, gold_cost)
		B2_DECLARE_FIELD(3, int32, gem_cost)
	DECLARE_END(4)


	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdExchangeSystemInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, template_id)
		B2_DECLARE_FIELD(2, int32, request_count)
		B2_DECLARE_FIELD(3, int32, exchange_template_id)
		B2_DECLARE_FIELD(4, int32, result_count)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdLotteryShopRateQualityInfo)
		B2_DECLARE_FIELD(0, int32, equip_category)
		B2_DECLARE_FIELD(1, int32, quality)
		B2_DECLARE_FIELD(2, int32, rate)
		B2_DECLARE_FIELD(3, double, quality_inc_level)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdQualityTransRateInfo)
		B2_DECLARE_FIELD(0, int32, template_id)
		B2_DECLARE_FIELD(1, int32, equip_category)
		B2_DECLARE_FIELD(2, int32, quality)
		B2_DECLARE_FIELD(3, int32, trans_rate)
		B2_DECLARE_FIELD(4, int32, trans_cond_quality)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemEnhanceEffectInfo)
		B2_DECLARE_FIELD(0, int32, enhance_level)
		B2_DECLARE_FIELD(1, double, set_effect)
		B2_DECLARE_FIELD(2, int32, item_aura)
		B2_DECLARE_FIELD(3, float, channel_alpha)
		B2_DECLARE_FIELD(4, float, channel_red)
		B2_DECLARE_FIELD(5, float, channel_green)
		B2_DECLARE_FIELD(6, float, channel_blue)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemSurpassLevel)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, increase_level)
		B2_DECLARE_FIELD(2, int32, max_surpass_level)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemDismantleInfo)
		B2_DECLARE_FIELD(0, int32, inventory_type)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, material1)
		B2_DECLARE_FIELD(3, int32, material_amount1)
		B2_DECLARE_FIELD(4, int32, material2)
		B2_DECLARE_FIELD(5, int32, material_amount2)
		B2_DECLARE_FIELD(6, int32, material3)
		B2_DECLARE_FIELD(7, int32, material_amount3)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdResurrectionInfo)
		B2_DECLARE_FIELD(0, int32, mode_type)
		B2_DECLARE_FIELD(1, int32, count)
		B2_DECLARE_FIELD(2, int32, price)
		B2_DECLARE_FIELD(3, int32, attribute_change1)
		B2_DECLARE_FIELD(4, int32, change_value1)
		B2_DECLARE_FIELD(5, int32, attribute_change2)
		B2_DECLARE_FIELD(6, int32, change_value2)
		B2_DECLARE_FIELD(7, int32, attribute_change3)
		B2_DECLARE_FIELD(8, int32, change_value3)
	DECLARE_END(9)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CharacterServerInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, style_face)
		B2_DECLARE_FIELD(2, int32, style_hair)
		B2_DECLARE_FIELD(3, int32, style_skin)
		B2_DECLARE_FIELD(4, int32, level)
		B2_DECLARE_FIELD(5, int64, exp)
		B2_DECLARE_FIELD(6, int64, exp_max)
		B2_DECLARE_FIELD(7, int32, skill_point_buy_count)
		B2_DECLARE_FIELD(8, int32, inventory_item_buy_count)
		B2_DECLARE_FIELD(9, int32, inventory_item_slot_count)
		B2_DECLARE_FIELD(10, int32, rank)
		B2_DECLARE_FIELD(11, int32, rank_node_index)
		B2_DECLARE_FIELD(12, bool, rank_node_state)
		B2_DECLARE_FIELD(13, int32, wing_grade)
		B2_DECLARE_FIELD(14, int32, wing_level)
		B2_DECLARE_FIELD(15, bool, wing_visible)
		B2_DECLARE_FIELD(16, bool, helmet_visible)
		B2_DECLARE_FIELD(17, int32, aether_offense_set_option_id)
		B2_DECLARE_FIELD(18, int32, aether_defense_set_option_id)
		B2_DECLARE_FIELD(19, int32, remain_skill_point)
		B2_DECLARE_FIELD(20, TArray<B2SkillQuickSlotPtr>, skill_quick_slots)
		B2_DECLARE_FIELD(21, FString, name)
		B2_DECLARE_FIELD(22, TArray<B2SkillServerInfoPtr>, skills)
		B2_DECLARE_FIELD(23, bool,  unity_skill)
		B2_DECLARE_FIELD(24, int32, total_power)
		B2_DECLARE_FIELD(25, bool, costume_weapon_visible)
		B2_DECLARE_FIELD(26, bool, costume_hat_visible)
		B2_DECLARE_FIELD(27, bool, costume_armor_visible)
		B2_DECLARE_FIELD(28, bool, unity_skill_awaken)
	DECLARE_END(29)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2LightCharacter)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int64, exp)
		B2_DECLARE_FIELD(3, int64, exp_max)
		B2_DECLARE_FIELD(4, bool, changed_level)
		B2_DECLARE_FIELD(5, int32, remain_skill_point)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Reward)
		B2_DECLARE_FIELD(0, B2RawRewardPtr, raw_reward)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, item)
		B2_DECLARE_FIELD(2, B2CostumeServerInfoPtr, costume)
		B2_DECLARE_FIELD(3, B2AetherServerInfoPtr, aether)
		B2_DECLARE_FIELD(4, B2LightAccountPtr, light_account)
		B2_DECLARE_FIELD(5, TArray<B2LightCharacterPtr>, light_characters)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Relic)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, relic_id)
		B2_DECLARE_FIELD(2, int32, grade)
		B2_DECLARE_FIELD(3, int32, enhance_level)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2WingOptionInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, option_index)
		B2_DECLARE_FIELD(2, int32, level)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FairyStatus)
		B2_DECLARE_FIELD(0, int32, fairy_type)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, enhance_point)
		B2_DECLARE_FIELD(3, int32, activated_bless_option_id)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildSkill)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, bool, enabled)
		B2_DECLARE_FIELD(3, int64, buff_end_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2PresetNum)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, item_preset_num)
		B2_DECLARE_FIELD(2, int32, skill_preset_num)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2UnitySkill)
		B2_DECLARE_FIELD(0, int32, main_character_type)
		B2_DECLARE_FIELD(1, int32, unity_character_type)
		B2_DECLARE_FIELD(2, int32, step)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FindAccountInfo)
		B2_DECLARE_FIELD(0, FString, nickname)
		B2_DECLARE_FIELD(1, TArray<B2CharacterServerInfoPtr>, characters)
		B2_DECLARE_FIELD(2, TArray<B2ItemServerInfoPtr>, equip_items)
		B2_DECLARE_FIELD(3, TArray<B2CostumeServerInfoPtr>, equip_costumes)
		B2_DECLARE_FIELD(4, TArray<B2TotemServerInfoPtr>, equip_totems)
		B2_DECLARE_FIELD(5, TArray<B2AetherServerInfoPtr>, mount_aethers)
		B2_DECLARE_FIELD(6, TArray<B2RelicPtr>, relics)
		B2_DECLARE_FIELD(7, TArray<B2WingOptionInfoPtr>, wing_options)
		B2_DECLARE_FIELD(8, TArray<B2GuildSkillPtr>, guild_skills)
		B2_DECLARE_FIELD(9, TArray<B2ItemPresetPtr>, item_presets)
		B2_DECLARE_FIELD(10, TArray<B2PresetNumPtr>, character_presets)
		B2_DECLARE_FIELD(11, TArray<B2UnitySkillPtr>, unity_skills)
		B2_DECLARE_FIELD(12, TArray<B2FairyStatusPtr>, fairies)
		B2_DECLARE_FIELD(13, int64, account_id)
		B2_DECLARE_FIELD(14, int64, guild_id)
		B2_DECLARE_FIELD(15, int32, assault_match_grade)
		B2_DECLARE_FIELD(16, int32, assault_match_star_count)
	DECLARE_END(17)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FindAccountQuery)
		B2_DECLARE_FIELD(0, FString, nickname)					// one of nickname or account_id must exist.
		B2_DECLARE_FIELD(1, int64, account_id)
		B2_DECLARE_FIELD(2, TArray<int32>, character_types)	// empty means all characters.
		B2_DECLARE_FIELD(3, int32, item_preset_id)
		B2_DECLARE_FIELD(4, int32, skill_slot)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CharacterSummary)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, FString, account_name)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, int32, character_level)
		B2_DECLARE_FIELD(4, int32, power)
		B2_DECLARE_FIELD(5, bool, is_online)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2User)
		B2_DECLARE_FIELD(0, B2CharacterSummaryPtr, character_summary)
		B2_DECLARE_FIELD(1, FString, guild_name)
		B2_DECLARE_FIELD(2, int32, mark_index)
		B2_DECLARE_FIELD(3, int32, mark_color)
		B2_DECLARE_FIELD(4, int32, mark_bg_index)
		B2_DECLARE_FIELD(5, int32, mark_bg_color)
		B2_DECLARE_FIELD(6, int64, last_logout_time)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Friend)
		B2_DECLARE_FIELD(0, B2UserPtr, user)
		B2_DECLARE_FIELD(1, int64, last_social_point_send_time)
		B2_DECLARE_FIELD(2, bool, is_new)
		B2_DECLARE_FIELD(3, int64, raid_penalty_remove_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FriendInviteRewardInfo)
		B2_DECLARE_FIELD(0, int32, platform_type)
		B2_DECLARE_FIELD(1, int32, invite_count)
		B2_DECLARE_FIELD(2, int32, joiner_count)
		B2_DECLARE_FIELD(3, int32, invite_reward_mask)
		B2_DECLARE_FIELD(4, int32, joiner_reward_mask)
	DECLARE_END(5)
	
	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2InventoryCountServerInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, count)
		B2_DECLARE_FIELD(2, int32, max_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2StageClearInfo)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, int32, clear_grade)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ActRewardInfo)
		B2_DECLARE_FIELD(0, int32, difficulty)
		B2_DECLARE_FIELD(1, int32, act_id)
		B2_DECLARE_FIELD(2, int32, reward_index)
		B2_DECLARE_FIELD(3, int32, require_star_count)
		B2_DECLARE_FIELD(4, int32, reward_gold)	// @NOTE 삭제 예정
		B2_DECLARE_FIELD(5, int32, reward_gem)		// @NOTE 삭제 예정
		B2_DECLARE_FIELD(6, TArray<B2RewardPtr>, rewards)
		B2_DECLARE_FIELD(7, bool, received)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ActRepeatClearRewardInfo)
		B2_DECLARE_FIELD(0, int32, difficulty)
		B2_DECLARE_FIELD(1, int32, act_id)
		B2_DECLARE_FIELD(2, int32, clear_count)
		B2_DECLARE_FIELD(3, int32, require_clear_count)
		B2_DECLARE_FIELD(4, int32, reward_gold)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2StageClearRewardItemInfo)
		B2_DECLARE_FIELD(0, int32, group_id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, type)
		B2_DECLARE_FIELD(3, int32, main_attribute)
		B2_DECLARE_FIELD(4, float, main_attribute_value_min)
		B2_DECLARE_FIELD(5, float, main_attribute_value_max)
		B2_DECLARE_FIELD(6, int32, random_option_count)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2StageClearRewardAetherInfo)
		B2_DECLARE_FIELD(0, int32, type)
		B2_DECLARE_FIELD(1, TArray<int32>, tiers)
		B2_DECLARE_FIELD(2, TArray<int32>, grades)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
    DECLARE_BEGIN(B2Endpoint)
        B2_DECLARE_FIELD(0, FString, host)
        B2_DECLARE_FIELD(1, int32, port)
    DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
    DECLARE_BEGIN(B2MatchInfo)
        B2_DECLARE_FIELD(0, FString, match_token)
        B2_DECLARE_FIELD(1, int32, opponent_type)
    DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MatchResult)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, FString, match_token)
		B2_DECLARE_FIELD(2, int32, result)
		B2_DECLARE_FIELD(3, bool, disconnect)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MatchStatus)
		B2_DECLARE_FIELD(0, int32, battle_grade)
		B2_DECLARE_FIELD(1, int32, battle_star_count)
		B2_DECLARE_FIELD(2, int32, continue_win)
		B2_DECLARE_FIELD(3, int32, continue_win_max)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Ranking)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, int32, ranking)
		B2_DECLARE_FIELD(2, int32, total_ranker_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MatchRankerEntry)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, level)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MatchRanker)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, FString, nickname)
		B2_DECLARE_FIELD(2, int32, battle_grade)
		B2_DECLARE_FIELD(3, int32, battle_star_count)
		B2_DECLARE_FIELD(4, int32, best_floor)
		B2_DECLARE_FIELD(5, int32, clear_time)
		B2_DECLARE_FIELD(6, int32, total_power)
		B2_DECLARE_FIELD(7, int32, point)
		B2_DECLARE_FIELD(8, TArray<B2MatchRankerEntry>, entries)
        B2_DECLARE_FIELD(9, FString, pvp_replay_url)
        B2_DECLARE_FIELD(10, int32, replay_size_compressed)
        B2_DECLARE_FIELD(11, int32, replay_size_uncompressed)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2HallOfFame)
		B2_DECLARE_FIELD(0, int32, mode)
		B2_DECLARE_FIELD(1, int32, ranking)
		B2_DECLARE_FIELD(2, int64, account_id)
		B2_DECLARE_FIELD(3, int32, praised_count)
		B2_DECLARE_FIELD(4, int32, match_grade)
		B2_DECLARE_FIELD(5, int32, match_start_count)
		B2_DECLARE_FIELD(6, int32, mvp_count)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2HallOfFameUser)
		B2_DECLARE_FIELD(0, B2UserPtr, user)
		B2_DECLARE_FIELD(1, TArray<B2ItemServerInfoPtr>, equip_items)
		B2_DECLARE_FIELD(2, TArray<B2CostumeServerInfoPtr>, equip_costumes)
		B2_DECLARE_FIELD(3, TArray<B2TotemServerInfoPtr>, equip_totems)
		B2_DECLARE_FIELD(4, int32, wing_grade)
		B2_DECLARE_FIELD(5, int32, wing_level)
		B2_DECLARE_FIELD(6, bool, wing_visible)
		B2_DECLARE_FIELD(7, bool, helmet_visible)
		B2_DECLARE_FIELD(8, bool, costume_weapon_visible)
		B2_DECLARE_FIELD(9, bool, costume_hat_visible)
		B2_DECLARE_FIELD(10, bool, costume_armor_visible)
	DECLARE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2HallOfFameReward)
		B2_DECLARE_FIELD(0, int32, mode)
		B2_DECLARE_FIELD(1, int32, ranking)
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2HallOfFamePraise)
		B2_DECLARE_FIELD(0, int32, mode)
		B2_DECLARE_FIELD(1, int32, praise_count)
		B2_DECLARE_FIELD(2, int32, max_count)
		B2_DECLARE_FIELD(3, int64, target_account_id)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
    DECLARE_BEGIN(B2RoomLocation)
        B2_DECLARE_FIELD(0, B2EndpointPtr, channel_endpoint)
        B2_DECLARE_FIELD(1, FString, room_id)
    DECLARE_END(2)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_BEGIN(B2RoomParticipant)
        B2_DECLARE_FIELD(0, int32, id)
        B2_DECLARE_FIELD(1, int64, account_id)
        B2_DECLARE_FIELD(2, B2FindAccountInfoPtr, account_info)
    DECLARE_END(3)

    //////////////////////////////////////////////////////////////////////////
    DECLARE_BEGIN(B2RoomProperty)
        B2_DECLARE_FIELD(0, FString, key)
        B2_DECLARE_FIELD(1, FString, value)
    DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ConfigData)
		B2_DECLARE_FIELD(0, int32, skill_point_buy_price_gem)
		B2_DECLARE_FIELD(1, int32, max_skill_point_buy_count)
		B2_DECLARE_FIELD(2, int32, skill_point_reset_price_gem)
		B2_DECLARE_FIELD(3, int32, need_cash_for_extend_inventory)
		B2_DECLARE_FIELD(4, int32, increase_inventory_slot_per_buy_count)
		B2_DECLARE_FIELD(5, int32, max_inventory_slot_count)
		B2_DECLARE_FIELD(6, int32, max_friend_count)
		B2_DECLARE_FIELD(7, int32, max_send_ask_friend)
		B2_DECLARE_FIELD(8, int32, max_receive_ask_friend)
		B2_DECLARE_FIELD(9, int32, cooltime_for_send_social_point)
		B2_DECLARE_FIELD(10, int32, max_delete_friend_count_for_day)
		B2_DECLARE_FIELD(11, int32, max_social_point_value)
		B2_DECLARE_FIELD(12, int32, add_social_point_for_send_to_friend)
		B2_DECLARE_FIELD(13, int32, add_social_point_for_receive_from_friend)
		B2_DECLARE_FIELD(14, int32, stage_buff_rate_for_increase_exp)
		B2_DECLARE_FIELD(15, int32, stage_buff_rate_for_increase_gold)
		B2_DECLARE_FIELD(16, int32, stage_buff_rate_for_boost_game_speed)
		B2_DECLARE_FIELD(17, int32, guild_set_up_gold_cost)
		B2_DECLARE_FIELD(18, int32, default_max_guild_member_count)
		B2_DECLARE_FIELD(19, int32, max_invite_guild_count_for_account)
		B2_DECLARE_FIELD(20, int32, invite_guild_expire_period_by_day)
		B2_DECLARE_FIELD(21, int32, max_raid_count_per_day)
		B2_DECLARE_FIELD(22, int32, update_guild_gold_cost)
		B2_DECLARE_FIELD(23, int32, guild_master_logout_limit_period_day)
		B2_DECLARE_FIELD(24, int32, guild_battle_time_limit_sec_client)
		B2_DECLARE_FIELD(25, int32, hero_tower_sweep_reward_count_per_floor)
		B2_DECLARE_FIELD(26, int32, assault_resurrection_max_count)
		B2_DECLARE_FIELD(27, int32, pvp_match_point_max)
		B2_DECLARE_FIELD(28, int32, pvp_match_point_charge_period_second)
		B2_DECLARE_FIELD(29, int32, team_match_point_max)
		B2_DECLARE_FIELD(30, int32, team_match_point_charge_period_second)
		B2_DECLARE_FIELD(31, int32, item_random_option_change_cost_dark_crystal)
		B2_DECLARE_FIELD(32, int32, item_preset_max_count)
		B2_DECLARE_FIELD(33, int32, skill_quick_slot_max_count)
		B2_DECLARE_FIELD(34, int32, assault_mvp_point_to_open_reward)
		B2_DECLARE_FIELD(35, int32, duel_reward_challenge_max_count)
		B2_DECLARE_FIELD(36, int32, guild_support_cool_time_hour)
		B2_DECLARE_FIELD(37, int32, guild_support_max_count_for_day)
		B2_DECLARE_FIELD(38, int32, rest_max_time_second)
		B2_DECLARE_FIELD(39, int32, rest_time_second_per_one_point)
		B2_DECLARE_FIELD(40, int32, guild_mercenary_donation_max_count_for_day)
		B2_DECLARE_FIELD(41, int32, guild_rejoin_wait_time_min)
		B2_DECLARE_FIELD(42, int32, character_level_max)
		B2_DECLARE_FIELD(43, int32, match_continuous_win_count_for_bonus)
		B2_DECLARE_FIELD(44, int32, hero_tower_free_ticket_count_max)
		B2_DECLARE_FIELD(45, int32, counter_dungeon_daily_enter_count_max)
		B2_DECLARE_FIELD(46, int32, max_chat_block_count)
		B2_DECLARE_FIELD(47, int32, min_floor_for_hero_tower_ranking)
		B2_DECLARE_FIELD(48, int32, raid_guild_bonus_count_member_2)
		B2_DECLARE_FIELD(49, int32, raid_guild_bonus_count_member_3)
		B2_DECLARE_FIELD(50, int32, raid_guild_bonus_count_member_4)
		B2_DECLARE_FIELD(51, int32, show_review_inducement_popup)
		B2_DECLARE_FIELD(52, int32, max_open_stage_difficulty)
		B2_DECLARE_FIELD(53, int32, item_random_option_change_cost_cash)
		B2_DECLARE_FIELD(54, int32, event_dice_free_count)
		B2_DECLARE_FIELD(55, int32, event_dice_point_per_one_dice)
		B2_DECLARE_FIELD(56, int32, max_costume_inventory_slot_count)
		B2_DECLARE_FIELD(57, int32, guild_master_change_period_day)
		B2_DECLARE_FIELD(58, int32, exchange_count_gold)
		B2_DECLARE_FIELD(59, int32, exchange_count_gem)
		B2_DECLARE_FIELD(60, int32, lobby_style)
		B2_DECLARE_FIELD(61, int32, advanced_rank_level)
	DECLARE_END(62)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdStageInfo)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, int32, difficulty)
		B2_DECLARE_FIELD(2, int32, difficulty_scale)
		B2_DECLARE_FIELD(3, int32, map_id)
		B2_DECLARE_FIELD(4, int32, clear_time_grade_high)
		B2_DECLARE_FIELD(5, int32, clear_time_grade_medium)
		B2_DECLARE_FIELD(6, int32, recommend_power)
		B2_DECLARE_FIELD(7, int32, recommend_attack)
		B2_DECLARE_FIELD(8, int32, recommend_defense)
		B2_DECLARE_FIELD(9, int32, gold_cost_exp_buff)
		B2_DECLARE_FIELD(10, int32, gold_cost_gold_buff)
		B2_DECLARE_FIELD(11, int32, gold_cost_auto_skill_buff)
		B2_DECLARE_FIELD(12, int32, gold_cost_boost_buff)
		B2_DECLARE_FIELD(13, int32, aether_type)		
	DECLARE_END(14)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BuySkillPointRequestInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, skill_point_buy_count)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BuySkillPointResponseInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, left_skill_point)
		B2_DECLARE_FIELD(2, int32, total_skill_point_buy_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BattleFormation)
		B2_DECLARE_FIELD(0, int32, formation_type)
		B2_DECLARE_FIELD(1, int32, formation_level)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BattleFormationBuff)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, buff_type1)
		B2_DECLARE_FIELD(2, float, buff_value1)
		B2_DECLARE_FIELD(3, int32, buff_type2)
		B2_DECLARE_FIELD(4, float, buff_value2)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2TeamBattleEntry)
		B2_DECLARE_FIELD(0, int32, formation_type)
		B2_DECLARE_FIELD(1, int32, character_slot_1)
		B2_DECLARE_FIELD(2, int32, character_slot_2)
		B2_DECLARE_FIELD(3, int32, character_slot_3)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTeamBattleFormationInfo)
		B2_DECLARE_FIELD(0, int32, formation_type)
		B2_DECLARE_FIELD(1, int32, formation_level)
		B2_DECLARE_FIELD(2, int32, enhance_gold_cost)
		B2_DECLARE_FIELD(3, TArray<B2BattleFormationBuffPtr>, buffs)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Team)
		B2_DECLARE_FIELD(0, int32, debuff_value)
		B2_DECLARE_FIELD(1, B2FindAccountInfoPtr, account)
		B2_DECLARE_FIELD(2, B2BattleFormationPtr, formation)
		B2_DECLARE_FIELD(3, B2TeamBattleEntryPtr, entry)
		B2_DECLARE_FIELD(4, B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(5, FString, guild_name)
		B2_DECLARE_FIELD(6, int32, mark_index)
		B2_DECLARE_FIELD(7, int32, mark_color)
		B2_DECLARE_FIELD(8, int32, mark_bg_index)
		B2_DECLARE_FIELD(9, int32, mark_bg_color)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Matcher)
		B2_DECLARE_FIELD(0, int32, debuff_value)
		B2_DECLARE_FIELD(1, B2FindAccountInfoPtr, account)
		B2_DECLARE_FIELD(2, B2MatchStatusPtr, match_status)
		B2_DECLARE_FIELD(3, FString, guild_name)
		B2_DECLARE_FIELD(4, int32, mark_index)
		B2_DECLARE_FIELD(5, int32, mark_color)
		B2_DECLARE_FIELD(6, int32, mark_bg_index)
		B2_DECLARE_FIELD(7, int32, mark_bg_color)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRankInfo)
		B2_DECLARE_FIELD(0, int32, rank)
		B2_DECLARE_FIELD(1, int32, node_index)
		B2_DECLARE_FIELD(2, int32, open_level)
		B2_DECLARE_FIELD(3, int32, attribute_change)
		B2_DECLARE_FIELD(4, float, change_value)
		B2_DECLARE_FIELD(5, int32, need_rank_promotion_point)
		B2_DECLARE_FIELD(6, int32, need_activate_gold)
		B2_DECLARE_FIELD(7, int32, rate)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDailyMatchReward)
		B2_DECLARE_FIELD(0, int32, match_type)
		B2_DECLARE_FIELD(1, int32, require_count)
		B2_DECLARE_FIELD(2, int32, reward_gold)
		B2_DECLARE_FIELD(3, int32, reward_bladepoint)
		B2_DECLARE_FIELD(4, int32, reward_gem)
		B2_DECLARE_FIELD(5, int32, reward_item_template_id)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMatchRankingReward)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, ranking_min)
		B2_DECLARE_FIELD(2, int32, ranking_max)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RankingReward)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, star_count)
		B2_DECLARE_FIELD(2, int32, ranking)
		B2_DECLARE_FIELD(3, int32, ranker_count)
		B2_DECLARE_FIELD(4, int32, initial_grade)
		B2_DECLARE_FIELD(5, TArray<B2RewardPtr>, rewards)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DimensionRankingReward)
		B2_DECLARE_FIELD(0, int32, best_difficulty_level)
		B2_DECLARE_FIELD(1, int32, best_clear_time_millis)
		B2_DECLARE_FIELD(2, int32, ranking)
		B2_DECLARE_FIELD(3, int32, ranker_count)
		B2_DECLARE_FIELD(4, TArray<B2RewardPtr>, rewards)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMainQuestInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, main_quest_type)
		B2_DECLARE_FIELD(2, int32, act_id)
		B2_DECLARE_FIELD(3, int32, difficulty)
		B2_DECLARE_FIELD(4, int32, event_type)
		B2_DECLARE_FIELD(5, int32, main_view_condition)
		B2_DECLARE_FIELD(6, int32, condition_1)
		B2_DECLARE_FIELD(7, int32, condition_2)
		B2_DECLARE_FIELD(8, int32, condition_3)
		B2_DECLARE_FIELD(9, int32, condition_4)
		B2_DECLARE_FIELD(10, int32, goal_count)
		B2_DECLARE_FIELD(11, TArray<B2RewardPtr>, rewards)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDailyQuestInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, event_type)
		B2_DECLARE_FIELD(2, int32, condition_1)
		B2_DECLARE_FIELD(3, int32, condition_2)
		B2_DECLARE_FIELD(4, int32, condition_3)
		B2_DECLARE_FIELD(5, int32, condition_4)
		B2_DECLARE_FIELD(6, int32, goal_count)
		B2_DECLARE_FIELD(7, TArray<B2RewardPtr>, rewards)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRewardInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, reward_type)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_push_type)
	DECLARE_END(4)	

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RankInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, node_index)
		B2_DECLARE_FIELD(2, int32, honor_point)
		B2_DECLARE_FIELD(3, int32, node_state)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MailItemInfo)
		B2_DECLARE_FIELD(0, int32, item_template_id)
		B2_DECLARE_FIELD(1, int32, item_amount)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, B2ItemServerInfoPtr, item)
		B2_DECLARE_FIELD(4, B2CostumeServerInfoPtr, costume)
		B2_DECLARE_FIELD(5, B2TotemServerInfoPtr, totem)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FactoryResult)
		B2_DECLARE_FIELD(0, int32, factory_result_type)
		B2_DECLARE_FIELD(1, B2ItemServerInfoPtr, make_item)
		B2_DECLARE_FIELD(2, B2ItemServerInfoPtr, bonus_item)
		B2_DECLARE_FIELD(3, TArray<B2ItemServerInfoPtr>, refund_item)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2UnsealBoxReward)
		B2_DECLARE_FIELD(0, int32, reward_type)
		B2_DECLARE_FIELD(1, int32, amount)
		B2_DECLARE_FIELD(2, int32, template_id)
		B2_DECLARE_FIELD(3, int32, effect_grade)
		B2_DECLARE_FIELD(4, B2ItemServerInfoPtr, item)
		B2_DECLARE_FIELD(5, B2TotemServerInfoPtr, totem)
		B2_DECLARE_FIELD(6, B2AetherServerInfoPtr, aether)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MailInfo)
		B2_DECLARE_FIELD(0, int64, id)
		B2_DECLARE_FIELD(1, int32, mail_category)
		B2_DECLARE_FIELD(2, int32, mail_attachType)
		B2_DECLARE_FIELD(3, B2MailItemInfoPtr, mail_item_info)
		B2_DECLARE_FIELD(4, int32, sender_type)
		B2_DECLARE_FIELD(5, int32, source_type)
		B2_DECLARE_FIELD(6, FString, sender_name)
		B2_DECLARE_FIELD(7, int64, expire_time)
		B2_DECLARE_FIELD(8, int32, gold)
		B2_DECLARE_FIELD(9, int32, gem)
		B2_DECLARE_FIELD(10, int32, blade_point)
		B2_DECLARE_FIELD(11, int32, social_point)
		B2_DECLARE_FIELD(12, TArray<B2MailItemInfoPtr>, selective_item_infos)
		B2_DECLARE_FIELD(13, int32, general_lottery_id)
		B2_DECLARE_FIELD(14, int32, fixed_grade_lottery_id)
		B2_DECLARE_FIELD(15, int32, selective_lottery_id)
		B2_DECLARE_FIELD(16, int32, selective_item_lottery_id)
		B2_DECLARE_FIELD(17, int32, hero_piece)
		B2_DECLARE_FIELD(18, int32, rank_promotion_point)
		B2_DECLARE_FIELD(19, int32, dark_crystal)
		B2_DECLARE_FIELD(20, int32, stage_boost_ticket)
		B2_DECLARE_FIELD(21, int32, hot_time_gold_buff_ticket)
		B2_DECLARE_FIELD(22, int32, hot_time_exp_buff_ticket)
		B2_DECLARE_FIELD(23, int32, shop_ten_lottery_ticket)
		B2_DECLARE_FIELD(24, FString, message)
		B2_DECLARE_FIELD(25, int32, package_id)
		B2_DECLARE_FIELD(26, int32, fame_point)
		B2_DECLARE_FIELD(27, int32, selective_aether_lottery_id)
		B2_DECLARE_FIELD(28, int32, prefix_selective_lottery_id)
		B2_DECLARE_FIELD(29, int32, stage_auto_clear_ticket)
		B2_DECLARE_FIELD(30, int32, totem_prefix_lottery_id)
	DECLARE_END(31)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2KillMonsterInfo)
		B2_DECLARE_FIELD(0, int32, monster_id)
		B2_DECLARE_FIELD(1, int32, monster_grade)
		B2_DECLARE_FIELD(2, int32, kill_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2QuestInfo)
		B2_DECLARE_FIELD(0, int32, slot)
		B2_DECLARE_FIELD(1, int32, type)
		B2_DECLARE_FIELD(2, int32, quest_id)
		B2_DECLARE_FIELD(3, int32, progress_count)
		B2_DECLARE_FIELD(4, int32, state)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdCounterDungeon)
		B2_DECLARE_FIELD(0, int32, dungeon_id)
		B2_DECLARE_FIELD(1, int32, dungeon_type)
		B2_DECLARE_FIELD(2, int32, difficulty)
		B2_DECLARE_FIELD(3, int32, difficulty_level)
		B2_DECLARE_FIELD(4, int32, difficulty_scale)
		B2_DECLARE_FIELD(5, int32, recommend_power)
		B2_DECLARE_FIELD(6, int32, recommend_attack)
		B2_DECLARE_FIELD(7, int32, recommend_defense)
		B2_DECLARE_FIELD(8, TArray<B2RewardRandomCountPtr>, rewards)
		B2_DECLARE_FIELD(9, int32, drop_gold_min)
		B2_DECLARE_FIELD(10, int32, drop_gold_max)
	DECLARE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdCounterDungeonPeriod)
		B2_DECLARE_FIELD(0, int32, dungeon_type)
		B2_DECLARE_FIELD(1, int32, day_of_week)
		B2_DECLARE_FIELD(2, int32, begin_hour)
		B2_DECLARE_FIELD(3, int32, begin_min)
		B2_DECLARE_FIELD(4, int32, end_hour)
		B2_DECLARE_FIELD(5, int32, end_min)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdCounterDungeonWeekReward)
		B2_DECLARE_FIELD(0, int32, index)
		B2_DECLARE_FIELD(1, int32, require_count)
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDimensionInfo)
		B2_DECLARE_FIELD(0, int32, difficulty_level)
		B2_DECLARE_FIELD(1, int32, difficulty_scale)
		B2_DECLARE_FIELD(2, int32, fixed_drop_item_id1)
		B2_DECLARE_FIELD(3, int32, fixed_drop_count1)
		B2_DECLARE_FIELD(4, int32, drop_totem_count_min)
		B2_DECLARE_FIELD(5, int32, drop_totem_count_max)
		B2_DECLARE_FIELD(6, int32, recommend_power)
		B2_DECLARE_FIELD(7, int32, recommend_attack)
		B2_DECLARE_FIELD(8, int32, recommend_defense)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDimensionWeekReward)
		B2_DECLARE_FIELD(0, int32, index)
		B2_DECLARE_FIELD(1, int32, ranking_min)
		B2_DECLARE_FIELD(2, int32, ranking_max)
		B2_DECLARE_FIELD(3, TArray<B2RawRewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2Guild)
		B2_DECLARE_FIELD(0, int64, guild_id)
		B2_DECLARE_FIELD(1, FString, guild_name)
		B2_DECLARE_FIELD(2, int64, master_account_id)
		B2_DECLARE_FIELD(3, FString, master_name)
		B2_DECLARE_FIELD(4, int32, max_member_count)
		B2_DECLARE_FIELD(5, FString, notice)
		B2_DECLARE_FIELD(6, int32, level)
		B2_DECLARE_FIELD(7, int32, exp)
		B2_DECLARE_FIELD(8, int32, join_option)
		B2_DECLARE_FIELD(9, int32, mark_index)
		B2_DECLARE_FIELD(10, int32, mark_color)
		B2_DECLARE_FIELD(11, int32, mark_bg_index)
		B2_DECLARE_FIELD(12, int32, mark_bg_color)
		B2_DECLARE_FIELD(13, int32, cur_member_count)
		B2_DECLARE_FIELD(14, int64, create_time)
		B2_DECLARE_FIELD(15, int32, season_obtain_medal)
		B2_DECLARE_FIELD(16, int32, season_ranking)
		B2_DECLARE_FIELD(17, int32, season_ranking_total)
		B2_DECLARE_FIELD(18, int32, members_avg_power)
		B2_DECLARE_FIELD(19, int32, total_result)
		B2_DECLARE_FIELD(20, bool, join_guild_battle)
		B2_DECLARE_FIELD(21, int32, turn_obtain_medal)
		B2_DECLARE_FIELD(22, int64, master_elected_time)
	DECLARE_END(23)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildMember)
		B2_DECLARE_FIELD(0, int64, guild_id)
		B2_DECLARE_FIELD(1, int32, contribution)
		B2_DECLARE_FIELD(2, int32, rank)
		B2_DECLARE_FIELD(3, B2CharacterSummaryPtr, character_summary)
		B2_DECLARE_FIELD(4, int64, last_logout_time)
		B2_DECLARE_FIELD(5, int32, total_support_count)
		B2_DECLARE_FIELD(6, int32, today_support_count)
		B2_DECLARE_FIELD(7, int32, total_donation_count)
		B2_DECLARE_FIELD(8, int32, today_donation_count)
		B2_DECLARE_FIELD(9, int64, raid_penalty_remove_time)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMissionInfo)
		B2_DECLARE_FIELD(0, int32, mission_id) 
		B2_DECLARE_FIELD(1, int32, mission_type)
		B2_DECLARE_FIELD(2, int32, event_type)
		B2_DECLARE_FIELD(3, int32, condition1)
		B2_DECLARE_FIELD(4, int32, condition2)
		B2_DECLARE_FIELD(5, int32, condition3)
		B2_DECLARE_FIELD(6, int32, condition4)
		B2_DECLARE_FIELD(7, int32, goal_count)
		B2_DECLARE_FIELD(8, int32, complete_point)
		B2_DECLARE_FIELD(9, int32, reward_id)
		B2_DECLARE_FIELD(10, int32, reward_count)
	DECLARE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSerialMissionInfo)
		B2_DECLARE_FIELD(0, int32, mission_id)
		B2_DECLARE_FIELD(1, int32, mission_step)
		B2_DECLARE_FIELD(2, int32, event_type)
		B2_DECLARE_FIELD(3, int32, condition1)
		B2_DECLARE_FIELD(4, int32, condition2)
		B2_DECLARE_FIELD(5, int32, condition3)
		B2_DECLARE_FIELD(6, int32, condition4)
		B2_DECLARE_FIELD(7, int32, goal_count)	
		B2_DECLARE_FIELD(8, int32, reward_id)
		B2_DECLARE_FIELD(9, int32, reward_count)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMissionCompletePointInfo)
		B2_DECLARE_FIELD(0, int32, mission_type)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, require_point)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MissionInfo)
		B2_DECLARE_FIELD(0, int32, mission_type)
		B2_DECLARE_FIELD(1, int32, mission_state_type)
		B2_DECLARE_FIELD(2, int32, mission_id)
		B2_DECLARE_FIELD(3, int32, progress_count)
		B2_DECLARE_FIELD(4, int32, completed_step)
		B2_DECLARE_FIELD(5, int32, rewarded_step)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MissionCompletePointInfo)
		B2_DECLARE_FIELD(0, int32, mission_type)
		B2_DECLARE_FIELD(1, int32, completePoint)
		B2_DECLARE_FIELD(2, TArray<int32>, reward_indexs)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdWingEnhance)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, need_item_template_id)
		B2_DECLARE_FIELD(3, int32, need_item_count)
		B2_DECLARE_FIELD(4, int32, enhance_point)
		B2_DECLARE_FIELD(5, int32, need_enhance_point)
		B2_DECLARE_FIELD(6, int32, gold_cost)
		B2_DECLARE_FIELD(7, int32, option_type1)
		B2_DECLARE_FIELD(8, float, option_value1)
		B2_DECLARE_FIELD(9, int32, option_type2)
		B2_DECLARE_FIELD(10, float, option_value2)
		B2_DECLARE_FIELD(11, int32, option_type3)
		B2_DECLARE_FIELD(12, float, option_value3)
	DECLARE_END(13)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdWingUpgrade)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, need_item_template_id)
		B2_DECLARE_FIELD(2, int32, need_item_count)
		B2_DECLARE_FIELD(3, int32, rate)
		B2_DECLARE_FIELD(4, int32, gold_cost)
		B2_DECLARE_FIELD(5, int32, fail_point_max)
		B2_DECLARE_FIELD(6, int32, option_type1)
		B2_DECLARE_FIELD(7, float, option_value1)
		B2_DECLARE_FIELD(8, int32, option_type2)
		B2_DECLARE_FIELD(9, float, option_value2)
		B2_DECLARE_FIELD(10, int32, option_type3)
		B2_DECLARE_FIELD(11, float, option_value3)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdWingOptionValue)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, option_index)
		B2_DECLARE_FIELD(2, int32, level)
		B2_DECLARE_FIELD(3, int32, option_type)
		B2_DECLARE_FIELD(4, float, option_value)
		B2_DECLARE_FIELD(5, int32, need_item_template_id)
		B2_DECLARE_FIELD(6, int32, need_item_count)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdWingOptionOpen)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, option_index)
		B2_DECLARE_FIELD(2, int32, need_grade)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdWingOptionMaxLevel)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, option_index)
		B2_DECLARE_FIELD(2, int32, wing_grade)
		B2_DECLARE_FIELD(3, int32, max_level)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdHeroTowerFloor)
		B2_DECLARE_FIELD(0, int32, floor)
		B2_DECLARE_FIELD(1, int32, recommend_power)
		B2_DECLARE_FIELD(2, int32, recommend_attack)
		B2_DECLARE_FIELD(3, int32, recommend_defense)
		B2_DECLARE_FIELD(4, int32, clear_limit_time)
		B2_DECLARE_FIELD(5, int32, map_id)
		B2_DECLARE_FIELD(6, int32, difficulty_scale)
		B2_DECLARE_FIELD(7, int32, monster_level)
		B2_DECLARE_FIELD(8, TArray<B2RewardPtr>, first_rewards)
		B2_DECLARE_FIELD(9, TArray<B2RewardPtr>, rewards)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRelicInfo)
		B2_DECLARE_FIELD(0, int32, relic_id)
		B2_DECLARE_FIELD(1, int32, open_stage_id)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRelicGradeInfo)
		B2_DECLARE_FIELD(0, int32, relic_id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, option_id_1)
		B2_DECLARE_FIELD(3, int32, option_value_1)
		B2_DECLARE_FIELD(4, int32, option_id_2)
		B2_DECLARE_FIELD(5, int32, option_value_2)
		B2_DECLARE_FIELD(6, int32, option_id_3)
		B2_DECLARE_FIELD(7, int32, option_value_3)
		B2_DECLARE_FIELD(8, int32, promotion_rate)
		B2_DECLARE_FIELD(9, int32, promotion_gold)
		B2_DECLARE_FIELD(10, int32, promotion_boss_piece)
		B2_DECLARE_FIELD(11, int32, boss_piece_id)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRelicLevelInfo)
		B2_DECLARE_FIELD(0, int32, relic_id)
		B2_DECLARE_FIELD(1, int32, enhance_level)
		B2_DECLARE_FIELD(2, int32, mode_option_id_1)
		B2_DECLARE_FIELD(3, int32, mode_option_value_1)
		B2_DECLARE_FIELD(4, int32, mode_option_id_2)
		B2_DECLARE_FIELD(5, int32, mode_option_value_2)
		B2_DECLARE_FIELD(6, int32, mode_option_id_3)
		B2_DECLARE_FIELD(7, int32, mode_option_value_3)
		B2_DECLARE_FIELD(8, int32, enhance_rate)
		B2_DECLARE_FIELD(9, int32, need_gold)
		B2_DECLARE_FIELD(10, int32, need_hero_piece)
	DECLARE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopCashBonus)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, increase_rate)
		B2_DECLARE_FIELD(2, int64, start_time)
		B2_DECLARE_FIELD(3, int64, end_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopFundsSale)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, discount_rate)
		B2_DECLARE_FIELD(2, int64, start_time)
		B2_DECLARE_FIELD(3, int64, end_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MagicShop)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, bool, is_purchased)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ShopLottery)
		B2_DECLARE_FIELD(0, int32, draw_price_type)
		B2_DECLARE_FIELD(1, bool, is_continuous)
		B2_DECLARE_FIELD(2, int64, next_free_time)
		B2_DECLARE_FIELD(3, int32, lottery_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopLotteryPrice)
		B2_DECLARE_FIELD(0, bool, is_continuous)
		B2_DECLARE_FIELD(1, bool, is_again)
		B2_DECLARE_FIELD(2, int32, gem)
		B2_DECLARE_FIELD(3, int32, calculated_gem)
		B2_DECLARE_FIELD(4, int32, social_point)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopLotteryPossibleGrade)
		B2_DECLARE_FIELD(0, int32, draw_price_type)
		B2_DECLARE_FIELD(1, bool, is_continuous)
		B2_DECLARE_FIELD(2, int32, min_grade)
		B2_DECLARE_FIELD(3, int32, max_grade)
	DECLARE_END(4)

	DECLARE_BEGIN(B2mdShopLotteryDailyLimit)
		B2_DECLARE_FIELD(0, int32, draw_price_type)
		B2_DECLARE_FIELD(1, bool, is_continuous)
		B2_DECLARE_FIELD(2, int32, limit_count)
	DECLARE_END(3)

	DECLARE_BEGIN(B2mdShopLotterySale)
		B2_DECLARE_FIELD(0, int32, draw_price_type)
		B2_DECLARE_FIELD(1, bool, is_continuous)
		B2_DECLARE_FIELD(2, int32, discount_rate)
		B2_DECLARE_FIELD(3, int64, start_time)
		B2_DECLARE_FIELD(4, int64, end_time)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopLotterySpecialGrade)
		B2_DECLARE_FIELD(0, int32, min_grade)
		B2_DECLARE_FIELD(1, int32, max_grade)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemInfo)
		B2_DECLARE_FIELD(0, int32, md_template_id)
		B2_DECLARE_FIELD(1, int32, md_group_id)
		B2_DECLARE_FIELD(2, int32, md_set_id)
		B2_DECLARE_FIELD(3, int32, md_type)
		B2_DECLARE_FIELD(4, int32, md_grade)
		B2_DECLARE_FIELD(5, int32, md_inventory_type)
		B2_DECLARE_FIELD(6, int32, md_equip_position)
		B2_DECLARE_FIELD(7, int32, md_max_levelup)
		B2_DECLARE_FIELD(8, int32, md_main_attribute)
		B2_DECLARE_FIELD(9, float, md_level_factor)
		B2_DECLARE_FIELD(10, int32, md_unique_option_id1)
		B2_DECLARE_FIELD(11, float, md_unique_option_value1)
		B2_DECLARE_FIELD(12, int32, md_unique_option_id2)
		B2_DECLARE_FIELD(13, float, md_unique_option_value2)
		B2_DECLARE_FIELD(14, int32, md_unique_option_id3)
		B2_DECLARE_FIELD(15, float, md_unique_option_value3)
		B2_DECLARE_FIELD(16, int32, md_unique_option_id4)
		B2_DECLARE_FIELD(17, float, md_unique_option_value4)
		B2_DECLARE_FIELD(18, int32, md_unique_skill_id)
		B2_DECLARE_FIELD(19, int32, seal_slot)
		B2_DECLARE_FIELD(20, int32, seal_slot_open)
		B2_DECLARE_FIELD(21, int32, md_max_grade)
		B2_DECLARE_FIELD(22, int32, md_random_option_count)
		B2_DECLARE_FIELD(23, int32, md_next_grade_template_id)
		B2_DECLARE_FIELD(24, int32, md_buy_price)
		B2_DECLARE_FIELD(25, int32, md_sell_price)
		B2_DECLARE_FIELD(26, bool, is_stackable)
		B2_DECLARE_FIELD(27, bool, is_saleable)
		B2_DECLARE_FIELD(28, bool, is_equipable)
		B2_DECLARE_FIELD(29, bool, is_levelupable)
		B2_DECLARE_FIELD(30, bool, is_enhanceable)
		B2_DECLARE_FIELD(31, bool, is_upgradable)
		B2_DECLARE_FIELD(32, bool, is_composable)
		B2_DECLARE_FIELD(33, bool, is_surpassable)
		B2_DECLARE_FIELD(34, bool, is_sealable)
	DECLARE_END(35)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdCostumeInfo)
		B2_DECLARE_FIELD(0, int32, md_template_id)
		B2_DECLARE_FIELD(1, int32, md_group_id)
		B2_DECLARE_FIELD(2, int32, md_set_id)
		B2_DECLARE_FIELD(3, int32, md_type)
		B2_DECLARE_FIELD(4, int32, md_grade)
		B2_DECLARE_FIELD(5, int32, md_inventory_type)
		B2_DECLARE_FIELD(6, int32, md_equip_position)
		B2_DECLARE_FIELD(7, int32, md_max_levelup)
		B2_DECLARE_FIELD(8, int32, md_main_attribute)
		B2_DECLARE_FIELD(9, float, md_level_factor)
		B2_DECLARE_FIELD(10, int32, md_unique_option_id1)
		B2_DECLARE_FIELD(11, float, md_unique_option_value1)
		B2_DECLARE_FIELD(12, int32, md_unique_option_id2)
		B2_DECLARE_FIELD(13, float, md_unique_option_value2)
		B2_DECLARE_FIELD(14, int32, md_unique_option_id3)
		B2_DECLARE_FIELD(15, float, md_unique_option_value3)
		B2_DECLARE_FIELD(16, int32, md_unique_option_id4)
		B2_DECLARE_FIELD(17, float, md_unique_option_value4)
		B2_DECLARE_FIELD(18, int32, md_unique_skill_id)
		B2_DECLARE_FIELD(19, int32, md_max_grade)
		B2_DECLARE_FIELD(20, int32, md_random_option_count)
		B2_DECLARE_FIELD(21, int32, md_next_grade_template_id)
		B2_DECLARE_FIELD(22, int32, md_buy_price)
		B2_DECLARE_FIELD(23, int32, md_sell_price)
		B2_DECLARE_FIELD(24, bool, is_stackable)
		B2_DECLARE_FIELD(25, bool, is_saleable)
		B2_DECLARE_FIELD(26, bool, is_equipable)
		B2_DECLARE_FIELD(27, bool, is_levelupable)
		B2_DECLARE_FIELD(28, bool, is_enhanceable)
		B2_DECLARE_FIELD(29, bool, is_upgradable)
		B2_DECLARE_FIELD(30, bool, is_composable)
		B2_DECLARE_FIELD(31, bool, is_surpassable)
	DECLARE_END(32)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ShopProduct)
		B2_DECLARE_FIELD(0, int32, product_type)
		B2_DECLARE_FIELD(1, int32, product_id)
		B2_DECLARE_FIELD(2, int32, product_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ShopMileage)
		B2_DECLARE_FIELD(0, int32, shop_mileage_type)
		B2_DECLARE_FIELD(1, int32, mileage)
		B2_DECLARE_FIELD(2, int32, reward_index)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ShopBuyManage)
		B2_DECLARE_FIELD(0, int32, product_id)
		B2_DECLARE_FIELD(1, int32, buy_count)
		B2_DECLARE_FIELD(2, int64, updated_time)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ShopProductMark)
		B2_DECLARE_FIELD(0, int32, product_id)
		B2_DECLARE_FIELD(1, int32, mark_type)
		B2_DECLARE_FIELD(2, int64, sales_start_time)
		B2_DECLARE_FIELD(3, int64, sales_end_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopProduct)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, shop_type)
		B2_DECLARE_FIELD(2, int32, shop_order)
		B2_DECLARE_FIELD(3, int32, product_type)
		B2_DECLARE_FIELD(4, int32, product_order)
		B2_DECLARE_FIELD(5, int32, product_id)
		B2_DECLARE_FIELD(6, int32, product_count)
		B2_DECLARE_FIELD(7, int32, bonus_product_type)
		B2_DECLARE_FIELD(8, int32, bonus_product_id)
		B2_DECLARE_FIELD(9, int32, bonus_product_count)
		B2_DECLARE_FIELD(10, int32, calculated_bonus_count)
		B2_DECLARE_FIELD(11, int32, buy_price_type)
		B2_DECLARE_FIELD(12, int32, price)
		B2_DECLARE_FIELD(13, int32, calculated_price)
		B2_DECLARE_FIELD(14, int32, restric_type)
		B2_DECLARE_FIELD(15, int32, buy_limit_count)
	DECLARE_END(16)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopPackage)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, category)
		B2_DECLARE_FIELD(2, int32, tab_type)
		B2_DECLARE_FIELD(3, FString, name)
		B2_DECLARE_FIELD(4, int32, pay_period)
		B2_DECLARE_FIELD(5, int32, period_bonus_reward_id)
		B2_DECLARE_FIELD(6, int32, period_bonus_reward_count)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopPackageProduct)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, B2ShopProductPtr, product)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopMileageReward)
		B2_DECLARE_FIELD(0, int32, mileage_type)
		B2_DECLARE_FIELD(1, int32, index)	
		B2_DECLARE_FIELD(2, int32, mileage)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CollectionItem)
		B2_DECLARE_FIELD(0, int32, item_template_id)
		B2_DECLARE_FIELD(1, int32, collect_state)
		B2_DECLARE_FIELD(2, bool, is_new_item)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CollectionSetItem)
		B2_DECLARE_FIELD(0, int32, group_id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, character_type)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemSet)
		B2_DECLARE_FIELD(0, int32, set_id)
		B2_DECLARE_FIELD(1, FString, name)
		B2_DECLARE_FIELD(2, int32, set_option_id_set2)
		B2_DECLARE_FIELD(3, float, set_value_set2)
		B2_DECLARE_FIELD(4, int32, set_option_id_set3)
		B2_DECLARE_FIELD(5, float, set_value_set3)
		B2_DECLARE_FIELD(6, int32, set_option_id_set4)
		B2_DECLARE_FIELD(7, float, set_value_set4)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSelectItemOption)
		B2_DECLARE_FIELD(0, int32, option_id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, equip_position)
		B2_DECLARE_FIELD(3, float, prob)
		B2_DECLARE_FIELD(4, int32, decimal_point)
		B2_DECLARE_FIELD(5, float, min)
		B2_DECLARE_FIELD(6, float, max)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSelectItemSealOption)
		B2_DECLARE_FIELD(0, int32, option_id)
		B2_DECLARE_FIELD(1, int32, slot)
		B2_DECLARE_FIELD(2, float, prob)
		B2_DECLARE_FIELD(3, int32, decimal_point)
		B2_DECLARE_FIELD(4, float, min)
		B2_DECLARE_FIELD(5, float, max)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdItemSealCost)
		B2_DECLARE_FIELD(0, int32, slot)
		B2_DECLARE_FIELD(1, int32, open_price_type)
		B2_DECLARE_FIELD(2, int32, open_price)
		B2_DECLARE_FIELD(3, int32, template_id1)
		B2_DECLARE_FIELD(4, int32, amount1)
		B2_DECLARE_FIELD(5, int32, template_id2)
		B2_DECLARE_FIELD(6, int32, amount2)
		B2_DECLARE_FIELD(7, int32, template_id3)
		B2_DECLARE_FIELD(8, int32, amount3)
		B2_DECLARE_FIELD(9, int32, template_id4)
		B2_DECLARE_FIELD(10, int32, amount4)
		B2_DECLARE_FIELD(11, int32, seal_price_type)
		B2_DECLARE_FIELD(12, int32, seal_price)
	DECLARE_END(13)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdCollectionReward)
		B2_DECLARE_FIELD(0, int32, collection_category)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RaidRankingReward)
		B2_DECLARE_FIELD(0, int32, ranking)
		B2_DECLARE_FIELD(1, int32, item_template_id)
		B2_DECLARE_FIELD(2, int32, item_count)
		B2_DECLARE_FIELD(3, bool, is_bonus)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DimensionStatus)
		B2_DECLARE_FIELD(0, int32, best_difficulty_level)
		B2_DECLARE_FIELD(1, int32, best_clear_time_millis)
		B2_DECLARE_FIELD(2, int32, best_character_type)
		B2_DECLARE_FIELD(3, int32, best_character_level)
		B2_DECLARE_FIELD(4, int32, best_total_power)
		B2_DECLARE_FIELD(5, int32, best_record_time)
		B2_DECLARE_FIELD(6, int32, top_difficulty_level)
		B2_DECLARE_FIELD(7, int32, last_difficulty_level)
		B2_DECLARE_FIELD(8, int32, last_character_type)
		B2_DECLARE_FIELD(9, int32, daily_clear_count)
		B2_DECLARE_FIELD(10, int32, daily_clear_count_max)
		B2_DECLARE_FIELD(11, int32, daily_count_init_time)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDailyAttendanceInfo)
		B2_DECLARE_FIELD(0, int32, attendance_type)	
		B2_DECLARE_FIELD(1, int32, day)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleCharacterSlot)
		B2_DECLARE_FIELD(0, int32, slot_num)
		B2_DECLARE_FIELD(1, int32, entry_code)
		B2_DECLARE_FIELD(2, int32, entry_type)
		B2_DECLARE_FIELD(3, int32, level)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleEntry)
		B2_DECLARE_FIELD(0, TArray<B2GuildBattleCharacterSlotPtr>, slots)
	DECLARE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleMember)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, FString, nickname)
		B2_DECLARE_FIELD(2, int32, power_index)
		B2_DECLARE_FIELD(3, int32, left_attack)
		B2_DECLARE_FIELD(4, int32, left_medal)
		B2_DECLARE_FIELD(5, int32, season_obtain_medal)
		B2_DECLARE_FIELD(6, int32, total_power)
		B2_DECLARE_FIELD(7, B2GuildBattleEntryPtr, entry)
		B2_DECLARE_FIELD(8, bool, is_attacked)
		B2_DECLARE_FIELD(9, TArray<B2CharacterPowerPtr>, character_powers)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleAttack)
		B2_DECLARE_FIELD(0, int64, guild_id)
		B2_DECLARE_FIELD(1, int64, account_id)
		B2_DECLARE_FIELD(2, int32, attack_count)
		B2_DECLARE_FIELD(3, int32, power_index)
		B2_DECLARE_FIELD(4, FString, nickname)
		B2_DECLARE_FIELD(5, FString, target_nickname)
		B2_DECLARE_FIELD(6, int32, target_power_index)
		B2_DECLARE_FIELD(7, int32, obtain_medal)
		B2_DECLARE_FIELD(8, int64, battle_end_time)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleGuild)
		B2_DECLARE_FIELD(0, int64, guild_id)
		B2_DECLARE_FIELD(1, FString, guild_name)
		B2_DECLARE_FIELD(2, int32, mark_index)
		B2_DECLARE_FIELD(3, int32, mark_color)
		B2_DECLARE_FIELD(4, int32, mark_bg_index)
		B2_DECLARE_FIELD(5, int32, mark_bg_color)
		B2_DECLARE_FIELD(6, int32, obtain_medal)
		B2_DECLARE_FIELD(7, int32, attack_count)
		B2_DECLARE_FIELD(8, int32, guild_level)
		B2_DECLARE_FIELD(9, int32, guild_exp)
		B2_DECLARE_FIELD(10, TArray<B2GuildBattleMemberPtr>, members)
		B2_DECLARE_FIELD(11, TArray<B2GuildBattleAttackPtr>, attacks)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildBattleScheduleInfo)
		B2_DECLARE_FIELD(0, int32, turn)
		B2_DECLARE_FIELD(1, int32, state)
		B2_DECLARE_FIELD(2, int32, start_day)
		B2_DECLARE_FIELD(3, int32, start_hour)
		B2_DECLARE_FIELD(4, int32, start_min)
		B2_DECLARE_FIELD(5, int32, end_day)
		B2_DECLARE_FIELD(6, int32, end_hour)
		B2_DECLARE_FIELD(7, int32, end_min)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildBattleResultReward)
		B2_DECLARE_FIELD(0, int32, reward_grade)
		B2_DECLARE_FIELD(1, int32, req_count_min)
		B2_DECLARE_FIELD(2, int32, req_count_max)
		B2_DECLARE_FIELD(3, TArray<B2RewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildBattleSeasonReward)
		B2_DECLARE_FIELD(0, int32, order)
		B2_DECLARE_FIELD(1, int32, ranking_min)
		B2_DECLARE_FIELD(2, int32, ranking_max)
		B2_DECLARE_FIELD(3, float, ranking_rate_min)
		B2_DECLARE_FIELD(4, float, ranking_rate_max)
		B2_DECLARE_FIELD(5, TArray<B2RewardPtr>, rewards)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleState)
		B2_DECLARE_FIELD(0, int32, season_id)
		B2_DECLARE_FIELD(1, int32, turn)
		B2_DECLARE_FIELD(2, int32, current_state)
		B2_DECLARE_FIELD(3, int64, begin_time)
		B2_DECLARE_FIELD(4, int64, end_time)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleHistory)
		B2_DECLARE_FIELD(0, int32, obtain_medal)
		B2_DECLARE_FIELD(1, int32, medal_total_count)
		B2_DECLARE_FIELD(2, int32, result)
		B2_DECLARE_FIELD(3, int64, battle_end_time)
		B2_DECLARE_FIELD(4, int64, target_guild_id)
		B2_DECLARE_FIELD(5, FString, target_guild_name)
		B2_DECLARE_FIELD(6, int32, target_obtain_medal)
		B2_DECLARE_FIELD(7, int32, target_mark)
		B2_DECLARE_FIELD(8, int32, target_mark_color)
		B2_DECLARE_FIELD(9, int32, target_mark_bg)
		B2_DECLARE_FIELD(10, int32, target_mark_bg_color)
		B2_DECLARE_FIELD(11, int32, target_medal_total_count)
		B2_DECLARE_FIELD(12, int32, member_count)
		B2_DECLARE_FIELD(13, int32, attack_count)
		B2_DECLARE_FIELD(14, int32, target_member_count)
		B2_DECLARE_FIELD(15, int32, target_attack_count)
	DECLARE_END(16)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildBattleRecord)
		B2_DECLARE_FIELD(0, int32, win_count)
		B2_DECLARE_FIELD(1, int32, draw_count)
		B2_DECLARE_FIELD(2, int32, lose_count)
		B2_DECLARE_FIELD(3, int32, total_obtain_medal)
		B2_DECLARE_FIELD(4, int32, ranking)
		B2_DECLARE_FIELD(5, int32, ranking_total_count)
		B2_DECLARE_FIELD(6, int32, season_id)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildRanking)
		B2_DECLARE_FIELD(0, int32, ranking)
		B2_DECLARE_FIELD(1, int64, guild_id)
		B2_DECLARE_FIELD(2, FString, guild_name)
		B2_DECLARE_FIELD(3, int32, total_obtain_medal)
		B2_DECLARE_FIELD(4, int32, total_result)
		B2_DECLARE_FIELD(5, int32, members_avg_power)
		B2_DECLARE_FIELD(6, int64, create_time)
		B2_DECLARE_FIELD(7, int32, mark)
		B2_DECLARE_FIELD(8, int32, mark_color)
		B2_DECLARE_FIELD(9, int32, mark_bg)
		B2_DECLARE_FIELD(10, int32, mark_bg_color)
		B2_DECLARE_FIELD(11, FString, gm_nickname)
		B2_DECLARE_FIELD(12, int32, max_member_count)
		B2_DECLARE_FIELD(13, int32, cur_member_count)
		B2_DECLARE_FIELD(14, int32, level)
		B2_DECLARE_FIELD(15, int32, exp)
	DECLARE_END(16)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildSkill)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, upgrade_cost_gold)
		B2_DECLARE_FIELD(3, int32, upgrade_req_guild_level)
		B2_DECLARE_FIELD(4, int32, buy_cost_gold)
		B2_DECLARE_FIELD(5, int32, buff_option_type)
		B2_DECLARE_FIELD(6, float, buff_option_value)
		B2_DECLARE_FIELD(7, int32, buff_time_sec)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildExp)
		B2_DECLARE_FIELD(0, int32, level)
		B2_DECLARE_FIELD(1, int32, exp_max)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRaidInfo)
		B2_DECLARE_FIELD(0, int32, raid_type)
		B2_DECLARE_FIELD(1, int32, step)
		B2_DECLARE_FIELD(2, int32, difficulty_scale)
		B2_DECLARE_FIELD(3, int32, recommend_power)
		B2_DECLARE_FIELD(4, int32, recommend_attack)
		B2_DECLARE_FIELD(5, int32, recommend_defense)
		B2_DECLARE_FIELD(6, int32, reward_id)
		B2_DECLARE_FIELD(7, int32, reward_min_count)
		B2_DECLARE_FIELD(8, int32, reward_max_count)
		B2_DECLARE_FIELD(9, int32, main_reward_id)
		B2_DECLARE_FIELD(10, int32, main_reward_min_count)
		B2_DECLARE_FIELD(11, int32, main_reward_max_count)
		B2_DECLARE_FIELD(12, int32, reward_item_min_grade)
		B2_DECLARE_FIELD(13, int32, reward_item_max_grade)
	DECLARE_END(14)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMonsterInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, FString, name)
		B2_DECLARE_FIELD(2, FString, symbol)
		B2_DECLARE_FIELD(3, FString, grade)
		B2_DECLARE_FIELD(4, FString, type)
		B2_DECLARE_FIELD(5, FString, desc)
		B2_DECLARE_FIELD(6, int32, hp_scale)
		B2_DECLARE_FIELD(7, int32, attack_defense_level_scale)
		B2_DECLARE_FIELD(8, int32, max_armor_override)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildMemberCandidate)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, int32, invite_state)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildInvite)
		B2_DECLARE_FIELD(0, B2GuildPtr, guild)
		B2_DECLARE_FIELD(1, int64, expire_time)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGeneralLottery)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, character_type)
		B2_DECLARE_FIELD(3, int32, equip_category)
	DECLARE_END(4)
	 
	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSelectiveLottery)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, equip_category)
		B2_DECLARE_FIELD(3, int32, item_grade)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFixedGradeLottery)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, character_type);
		B2_DECLARE_FIELD(2, int32, equip_category)
		B2_DECLARE_FIELD(3, int32, item_grade)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdPrefixSelectiveLottery)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, character_type);
		B2_DECLARE_FIELD(2, int32, group_id);
		B2_DECLARE_FIELD(3, int32, equip_category)
		B2_DECLARE_FIELD(4, int32, item_grade)
		B2_DECLARE_FIELD(5, int32, surpass_level)
		B2_DECLARE_FIELD(6, int32, quality)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAdditionalMatchCost)
		B2_DECLARE_FIELD(0, int32, count)
		B2_DECLARE_FIELD(1, int32, cost)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MatchOpenPeriodInfo)
		B2_DECLARE_FIELD(0, int32, day_of_week)
		B2_DECLARE_FIELD(1, int32, begin_hour)
		B2_DECLARE_FIELD(2, int32, begin_min)
		B2_DECLARE_FIELD(3, int32, end_hour)
		B2_DECLARE_FIELD(4, int32, end_min)
		B2_DECLARE_FIELD(5, int32, raid_type)
		B2_DECLARE_FIELD(6, int32, assault_type)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdStageFixedDrop)
		B2_DECLARE_FIELD(0, int32, stage_id)
		B2_DECLARE_FIELD(1, int32, idx)
		B2_DECLARE_FIELD(2, int32, template_id)
		B2_DECLARE_FIELD(3, int32, min)
		B2_DECLARE_FIELD(4, int32, max)
		B2_DECLARE_FIELD(5, int32, rate)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DropData)
		B2_DECLARE_FIELD(0, int32, template_id)
		B2_DECLARE_FIELD(1, int32, count)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DuelRewardBoxStatus)
		B2_DECLARE_FIELD(0, bool, is_reward_state)
		B2_DECLARE_FIELD(1, int32, box_grade)
		B2_DECLARE_FIELD(2, int32, win_count)
		B2_DECLARE_FIELD(3, int32, upgrade_win_count)
		B2_DECLARE_FIELD(4, int32, defeat_count)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DuelRewardBoxInfoResponseRewardBox)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, win_count)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DuelRewardBoxGainableListResponseRewardItem)
		B2_DECLARE_FIELD(0, int32, reward_id)
		B2_DECLARE_FIELD(1, int32, min_count)
		B2_DECLARE_FIELD(2, int32, max_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAssaultVictoryRewardInfo)
		B2_DECLARE_FIELD(0, int32, match_result)
		B2_DECLARE_FIELD(1, int32, reward_id)
		B2_DECLARE_FIELD(2, int32, reward_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAssaultMvpRewardInfo)
		B2_DECLARE_FIELD(0, int32, reward_id)
		B2_DECLARE_FIELD(1, int32, reward_min_count)
		B2_DECLARE_FIELD(2, int32, reward_max_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdEventAttendance)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, FString, title)
		B2_DECLARE_FIELD(2, int32, total_attendance_days)
		B2_DECLARE_FIELD(3, int64, start_time)
		B2_DECLARE_FIELD(4, int64, end_time)
		B2_DECLARE_FIELD(5, FString, explain)
		B2_DECLARE_FIELD(6, FString, event_url)
		B2_DECLARE_FIELD(7, int32, bonus_type)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdEventAttendanceReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, day)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDailyPlayTimeReward)
		B2_DECLARE_FIELD(0, int32, reward_index)
		B2_DECLARE_FIELD(1, int32, play_time_inmin)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2EventAttendanceStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, attendance_day)
		B2_DECLARE_FIELD(2, int64, last_attendance_time)
		B2_DECLARE_FIELD(3, int64, next_attendance_time)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BeginnerAttendanceStatus)
		B2_DECLARE_FIELD(0, int32, total_attendance_days)
		B2_DECLARE_FIELD(1, int32, attendance_day)
		B2_DECLARE_FIELD(2, int64, last_attendance_time)
		B2_DECLARE_FIELD(3, int64, next_attendance_time)
		B2_DECLARE_FIELD(4, int64, finish_time)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ActIdentifier)
		B2_DECLARE_FIELD(0, int32, difficulty)
		B2_DECLARE_FIELD(1, int32, act_id)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildInfo)
		B2_DECLARE_FIELD(0, int64, guild_id)
		B2_DECLARE_FIELD(1, FString, guild_name)
		B2_DECLARE_FIELD(2, int32, mark_index)
		B2_DECLARE_FIELD(3, int32, mark_color)
		B2_DECLARE_FIELD(4, int32, mark_bg_index)
		B2_DECLARE_FIELD(5, int32, mark_bg_color)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2IncreaseCharacterExpInfo)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, exp)
		B2_DECLARE_FIELD(3, int32, exp_max)
		B2_DECLARE_FIELD(4, bool, level_changed)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTutorialInfo)
		B2_DECLARE_FIELD(0, int32, tutorial_id)
		B2_DECLARE_FIELD(1, int32, prev_tutorial_id)
		B2_DECLARE_FIELD(2, int32, character_level)
		B2_DECLARE_FIELD(3, int32, act_cleared)
		B2_DECLARE_FIELD(4, int32, cash)
		B2_DECLARE_FIELD(5, int32, gold)
		B2_DECLARE_FIELD(6, int32, reward_id1)
		B2_DECLARE_FIELD(7, int32, reward_count1)
		B2_DECLARE_FIELD(8, int32, reward_id2)
		B2_DECLARE_FIELD(9, int32, reward_count2)
		B2_DECLARE_FIELD(10, int32, reward_id3)
		B2_DECLARE_FIELD(11, int32, reward_count3)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAetherOptionInfo)
		B2_DECLARE_FIELD(0, int32, option_id)
		B2_DECLARE_FIELD(1, int32, option_group)
		B2_DECLARE_FIELD(2, int32, grade)
		B2_DECLARE_FIELD(3, float, default_value)
		B2_DECLARE_FIELD(4, float, increase_value_per_level)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAetherSlotInfo)
		B2_DECLARE_FIELD(0, int32, slot_id)
		B2_DECLARE_FIELD(1, int32, option_id)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAetherEnhanceInfo)
		B2_DECLARE_FIELD(0, int32, enhance_level)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, aether_piece_cost)
		B2_DECLARE_FIELD(3, int32, enhance_rate)
		B2_DECLARE_FIELD(4, int32, dismantle_amount)
		B2_DECLARE_FIELD(5, int32, sell_price)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAetherEnhanceOptionInfo)
		B2_DECLARE_FIELD(0, int32, enhance_level)
		B2_DECLARE_FIELD(1, int32, tier)
		B2_DECLARE_FIELD(2, int32, state_main_option)
		B2_DECLARE_FIELD(3, int32, state_sub_option)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdAetherSetOptionInfo)
		B2_DECLARE_FIELD(0, int32, type)
		B2_DECLARE_FIELD(1, int32, tier)
		B2_DECLARE_FIELD(2, int32, set_option_id)
		B2_DECLARE_FIELD(3, int32, min_apply_count)
		B2_DECLARE_FIELD(4, int32, trigger_rate)
		B2_DECLARE_FIELD(5, int32, cooldown_second)
		B2_DECLARE_FIELD(6, int32, damage_rate)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDuelModeReward)
		B2_DECLARE_FIELD(0, int32, duel_mode)
		B2_DECLARE_FIELD(1, int32, match_result)
		B2_DECLARE_FIELD(2, TArray<B2RewardPtr>, rewards)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRaidExp)
		B2_DECLARE_FIELD(0, int32, raid_type)
		B2_DECLARE_FIELD(1, int32, step)
		B2_DECLARE_FIELD(2, int32, exp)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildChatData)
		B2_DECLARE_FIELD(0, int64, id)
		B2_DECLARE_FIELD(1, int64, guild_id)
		B2_DECLARE_FIELD(2, int64, account_id)
		B2_DECLARE_FIELD(3, FString, nickname)
		B2_DECLARE_FIELD(4, int64, create_time)
		B2_DECLARE_FIELD(5, int32, chat_type)
		B2_DECLARE_FIELD(6, FString, message)
		B2_DECLARE_FIELD(7, int32, chat_notice_type)
		B2_DECLARE_FIELD(8, TArray<FString>, notice_args)
		B2_DECLARE_FIELD(9, int32, support_item_template_id)
		B2_DECLARE_FIELD(10, int32, support_count)
		B2_DECLARE_FIELD(11, int32, support_max_count)
		B2_DECLARE_FIELD(12, int64, update_time)
		B2_DECLARE_FIELD(13, int64, expire_time)
		B2_DECLARE_FIELD(14, int32, character_level_max)
	DECLARE_END(15)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildSupportItem)
		B2_DECLARE_FIELD(0, int32, item_template_id)
		B2_DECLARE_FIELD(1, int32, support_max_count)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2UnitySkillMission)
		B2_DECLARE_FIELD(0, int32, main_character_type)
		B2_DECLARE_FIELD(1, int32, unity_character_type)
		B2_DECLARE_FIELD(2, int32, step)
		B2_DECLARE_FIELD(3, int32, state)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdUnitySkillMission)
		B2_DECLARE_FIELD(0, int32, main_character_type)
		B2_DECLARE_FIELD(1, int32, unity_character_type)
		B2_DECLARE_FIELD(2, int32, step)
		B2_DECLARE_FIELD(3, int32, index)
		B2_DECLARE_FIELD(4, int32, event_type)
		B2_DECLARE_FIELD(5, int32, condition1)
		B2_DECLARE_FIELD(6, int32, condition2)
		B2_DECLARE_FIELD(7, int32, condition3)
		B2_DECLARE_FIELD(8, int32, condition4)
		B2_DECLARE_FIELD(9, int32, goal_count)
		B2_DECLARE_FIELD(10, int32, reward_id)
		B2_DECLARE_FIELD(11, int32, reward_count)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2UnitySkillAwakenMission)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, mission_id)
		B2_DECLARE_FIELD(2, int32, try_count)
		B2_DECLARE_FIELD(3, bool, completed)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdUnitySkillAwakenMission)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, mission_id)
		B2_DECLARE_FIELD(2, int32, req_item_id_1)
		B2_DECLARE_FIELD(3, int32, req_item_count_1)
		B2_DECLARE_FIELD(4, int32, req_item_id_2)
		B2_DECLARE_FIELD(5, int32, req_item_count_2)
		B2_DECLARE_FIELD(6, int32, req_gold)
		B2_DECLARE_FIELD(7, int32, success_rate)
		B2_DECLARE_FIELD(8, int32, success_rate_add)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdRestReward)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, reward_id)
		B2_DECLARE_FIELD(2, float, reward_count_per_point)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ContentsOpenStatus)
		B2_DECLARE_FIELD(0, int32, mode_id)
		B2_DECLARE_FIELD(1, bool, opened)
		B2_DECLARE_FIELD(2, int32, stage_id)
		B2_DECLARE_FIELD(3, int32, tutorial_id)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GuildMercenary)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, exp)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildMercenary)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, mastery_type)
		B2_DECLARE_FIELD(2, int32, max_armor_override)
		B2_DECLARE_FIELD(3, int32, skill_id_1)
		B2_DECLARE_FIELD(4, int32, skill_id_2)
		B2_DECLARE_FIELD(5, int32, skill_id_3)
		B2_DECLARE_FIELD(6, int32, max_use_count)
		B2_DECLARE_FIELD(7, bool, is_usable)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildMercenarySkill)
		B2_DECLARE_FIELD(0, int32, skill_id)
		B2_DECLARE_FIELD(1, int32, require_mercenary_level)
		B2_DECLARE_FIELD(2, int32, cool_time_sec)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildMercenaryMastery)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, level)
		B2_DECLARE_FIELD(2, int32, max_exp)
		B2_DECLARE_FIELD(3, int32, option_id_1)
		B2_DECLARE_FIELD(4, float, option_value_1)
		B2_DECLARE_FIELD(5, int32, option_id_2)
		B2_DECLARE_FIELD(6, float, option_value_2)
		B2_DECLARE_FIELD(7, int32, option_id_3)
		B2_DECLARE_FIELD(8, float, option_value_3)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGuildMercenaryDonation)
		B2_DECLARE_FIELD(0, int32, donation_type)
		B2_DECLARE_FIELD(1, int32, item_template_id)
		B2_DECLARE_FIELD(2, int32, donation_amount)
		B2_DECLARE_FIELD(3, int32, exp_increment)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2AssaultBattleInfo)
		B2_DECLARE_FIELD(0, int32, kill_count)
		B2_DECLARE_FIELD(1, int32, death_count)
		B2_DECLARE_FIELD(2, int32, damage)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RoomCharacter)
		B2_DECLARE_FIELD(0, int32, character_type)
		B2_DECLARE_FIELD(1, int32, character_power)
		B2_DECLARE_FIELD(2, int32, character_level)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2AdminMessage)
        B2_DECLARE_FIELD(0, FString, message)
        B2_DECLARE_FIELD(1, int32, display_time)
	DECLARE_END(2)
	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2PointEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, point)
		B2_DECLARE_FIELD(2, int32, reward_state_value)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdEventInfo)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, event_category)
		B2_DECLARE_FIELD(2, FString, event_tab_name)
		B2_DECLARE_FIELD(3, FString, title)
		B2_DECLARE_FIELD(4, int64, start_time)
		B2_DECLARE_FIELD(5, int64, end_time)
		B2_DECLARE_FIELD(6, FString, explain)
		B2_DECLARE_FIELD(7, FString, banner_url)
		B2_DECLARE_FIELD(8, FString, link_text)
		B2_DECLARE_FIELD(9, FString, link_url)
		B2_DECLARE_FIELD(10, int32, tab_order)
		B2_DECLARE_FIELD(11, int32, event_sub_category)
	DECLARE_END(12)

	/////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdPointEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, event_type)
		B2_DECLARE_FIELD(3, int32, condition1)
		B2_DECLARE_FIELD(4, int32, condition2)
		B2_DECLARE_FIELD(5, int32, condition3)
		B2_DECLARE_FIELD(6, int32, condition4)
		B2_DECLARE_FIELD(7, int32, point)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdPointEventReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_count)
		B2_DECLARE_FIELD(4, int32, need_point)
		B2_DECLARE_FIELD(5, int32, tab_group_id)
		B2_DECLARE_FIELD(6, int32, buy_limit_count)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2LevelUpEventInfo)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, state)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdLevelUpEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, level)
		B2_DECLARE_FIELD(3, int32, reward_id)
		B2_DECLARE_FIELD(4, int32, reward_count)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdHotTimeEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, day_of_week)
		B2_DECLARE_FIELD(2, int32, index)
		B2_DECLARE_FIELD(3, int32, buff_type)
		B2_DECLARE_FIELD(4, int32, begin_hour)
		B2_DECLARE_FIELD(5, int32, begin_min)
		B2_DECLARE_FIELD(6, int32, end_hour)
		B2_DECLARE_FIELD(7, int32, end_min)
		B2_DECLARE_FIELD(8, int32, increase_rate)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSeasonMissionEvent)
		B2_DECLARE_FIELD(0, int32, event_id) 
		B2_DECLARE_FIELD(1, int32, day)
		B2_DECLARE_FIELD(2, int32, index)
		B2_DECLARE_FIELD(3, int32, event_type)
		B2_DECLARE_FIELD(4, int32, condition1)
		B2_DECLARE_FIELD(5, int32, condition2)
		B2_DECLARE_FIELD(6, int32, condition3)
		B2_DECLARE_FIELD(7, int32, condition4)
		B2_DECLARE_FIELD(8, int32, goal_count)
		B2_DECLARE_FIELD(9, int32, reward_id)
		B2_DECLARE_FIELD(10, int32, reward_count)
		B2_DECLARE_FIELD(11, int32, tutorial_id)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SeasonMissionInfo)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, day)
		B2_DECLARE_FIELD(2, int32, index)
		B2_DECLARE_FIELD(3, int32, progress_count)
		B2_DECLARE_FIELD(4, int32, state)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SeasonMissionEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, attendance_day)
		B2_DECLARE_FIELD(2, int64, next_attendance_time)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2StageClearEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, state)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdStageClearEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, stage)
		B2_DECLARE_FIELD(3, int32, reward_id)
		B2_DECLARE_FIELD(4, int32, reward_count)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdKakaoFriendInviteReward)
		B2_DECLARE_FIELD(0, int32, invite_reward_type)
		B2_DECLARE_FIELD(1, int32, reward_index)
		B2_DECLARE_FIELD(2, int32, goal_count)
		B2_DECLARE_FIELD(3, int32, reward_id)
		B2_DECLARE_FIELD(4, int32, reward_count)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdHotTimeProduct)
		B2_DECLARE_FIELD(0, int32, buff_type)
		B2_DECLARE_FIELD(1, int32, period_hour)
		B2_DECLARE_FIELD(2, int32, price)
		B2_DECLARE_FIELD(3, int32, increaseRate)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMagicShopRenewalCost)
		B2_DECLARE_FIELD(0, int32, renewal_count)
		B2_DECLARE_FIELD(1, int32, gem_cost)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMagicShopProduct)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, magic_shop_group_type)
		B2_DECLARE_FIELD(2, int32, product_type)
		B2_DECLARE_FIELD(3, int32, product_detail)
		B2_DECLARE_FIELD(4, int32, product_count)
		B2_DECLARE_FIELD(5, int32, price_type)
		B2_DECLARE_FIELD(6, int32, price)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2RollingBanner)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, type)
		B2_DECLARE_FIELD(2, int32, sub_type)
		B2_DECLARE_FIELD(3, FString, banner_img_url)
		B2_DECLARE_FIELD(4, int64, start_time)
		B2_DECLARE_FIELD(5, int64, end_time)
		B2_DECLARE_FIELD(6, int32, banner_duration)
		B2_DECLARE_FIELD(7, FString, web_url)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2MarketPurchaseInfo)
		B2_DECLARE_FIELD(0, int32, product_id)
		B2_DECLARE_FIELD(1, FString, market_product_id)
		B2_DECLARE_FIELD(2, FString, developer_payload)
		B2_DECLARE_FIELD(3, int32, purchase_state)
		B2_DECLARE_FIELD(4, int32, bonus_increase_rate)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdMarketProduct)
		B2_DECLARE_FIELD(0, int32, shop_product_id)
		B2_DECLARE_FIELD(1, FString, market_product_id)
		B2_DECLARE_FIELD(2, FString, currency_code)
		B2_DECLARE_FIELD(3, float, price)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2FlatRatePackageBonusState)
		B2_DECLARE_FIELD(0, int32, current_index)
		B2_DECLARE_FIELD(1, bool, weekly_state)
		B2_DECLARE_FIELD(2, bool, monthly_state)
		B2_DECLARE_FIELD(3, bool, reward_state)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDuelModeSettleSchedule)
		B2_DECLARE_FIELD(0, int32, mode_type)
		B2_DECLARE_FIELD(1, int32, ranking_settle_state)
		B2_DECLARE_FIELD(2, int32, day)
		B2_DECLARE_FIELD(3, int32, end_hour)
		B2_DECLARE_FIELD(4, int32, end_min)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2PeriodicPackageState)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int64, end_pay_time)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2LevelUpPackageMission)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, level)
		B2_DECLARE_FIELD(3, int32, state)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ReachLevelPackageMission)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, level)
		B2_DECLARE_FIELD(3, int32, state)
		B2_DECLARE_FIELD(4, int64, expire_time)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopLevelUpPackage)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, level)
		B2_DECLARE_FIELD(3, int32, reward_id)
		B2_DECLARE_FIELD(4, int32, reward_count)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopReachLevelPackage)
		B2_DECLARE_FIELD(0, int32, package_id)
		B2_DECLARE_FIELD(1, int32, character_type)
		B2_DECLARE_FIELD(2, int32, reach_level)
		B2_DECLARE_FIELD(3, int32, limit_level)
		B2_DECLARE_FIELD(4, int32, buy_reward_id)
		B2_DECLARE_FIELD(5, int32, buy_reward_count)
		B2_DECLARE_FIELD(6, int32, complete_reward_id)
		B2_DECLARE_FIELD(7, int32, complete_reward_count)
		B2_DECLARE_FIELD(8, int32, expire_day)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopFlatRatePackageBonus)
		B2_DECLARE_FIELD(0, int32, index)
		B2_DECLARE_FIELD(1, int32, reward_id)
		B2_DECLARE_FIELD(2, int32, reward_count)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdShopPackageTab)
		B2_DECLARE_FIELD(0, int32, tab_type)
		B2_DECLARE_FIELD(1, int32, sort_order)
		B2_DECLARE_FIELD(2, bool, is_open)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdUnitySkillOption)
		B2_DECLARE_FIELD(0, int32, main_character_type)
		B2_DECLARE_FIELD(1, int32, unity_character_type)
		B2_DECLARE_FIELD(2, int32, step)
		B2_DECLARE_FIELD(3, int32, option_id)
		B2_DECLARE_FIELD(4, double, option_value)
		B2_DECLARE_FIELD(5, double, power)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BuyShopResultInfo)
		B2_DECLARE_FIELD(0, TArray<B2ShopProductPtr>, add_products)
		B2_DECLARE_FIELD(1, TArray<B2ShopBuyManagePtr>, shop_buy_manages)
		B2_DECLARE_FIELD(2, int32, buy_product_type)
		B2_DECLARE_FIELD(3, B2PeriodicPackageStatePtr, periodic_package_state)
		B2_DECLARE_FIELD(4, B2FlatRatePackageBonusStatePtr, flat_rate_package_bonus_state)
		B2_DECLARE_FIELD(5, TArray<B2LevelUpPackageMissionPtr>, level_up_package_missions)
		B2_DECLARE_FIELD(6, B2ReachLevelPackageMissionPtr, reach_level_package_mission)
		B2_DECLARE_FIELD(7, int32, mileage_type)
		B2_DECLARE_FIELD(8, int32, current_mileage)
		B2_DECLARE_FIELD(9, TArray<B2CostumeServerInfoPtr>, costumes)
		B2_DECLARE_FIELD(10, TArray<B2TotemServerInfoPtr>, totems)
	DECLARE_END(11)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ContentsModeInfo)
		B2_DECLARE_FIELD(0, int32, mode)
		B2_DECLARE_FIELD(1, int32, state)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdWebUrl)
		B2_DECLARE_FIELD(0, FString, key)
		B2_DECLARE_FIELD(1, FString, url)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2PointShopEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, point)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2PointShopEventRewardHistory)
		B2_DECLARE_FIELD(0, int32, event_id)				// 이벤트 고유 번호
		B2_DECLARE_FIELD(1, int32, index)					// 보상 고유 번호
		B2_DECLARE_FIELD(2, int32, total_reward_count)		// 누적 보상(교환) 횟수
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2BlockChatUser)
		B2_DECLARE_FIELD(0, int64, account_id)
		B2_DECLARE_FIELD(1, FString, nickname)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2TenLotteryEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, lottery_count)
		B2_DECLARE_FIELD(2, TArray<int32>, rewarded_indexs)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTenLotteryEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, goal_lottery_count)
		B2_DECLARE_FIELD(3, TArray<B2RawRewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSpendGemEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, goal_spend_count)
		B2_DECLARE_FIELD(3, TArray<B2RawRewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SpendGemEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, spend_gem_count)
		B2_DECLARE_FIELD(2, int32, reward_state_value)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdGemPurchaseEventReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, goal_count)
		B2_DECLARE_FIELD(3, TArray<B2RawRewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2GemPurchaseEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, gem_count)
		B2_DECLARE_FIELD(2, int32, reward_state_value)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2TenLotterySavingsStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, current_point)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTenLotterySavingsEvent)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, gain_point)
		B2_DECLARE_FIELD(2, int32, need_point)
		B2_DECLARE_FIELD(3, TArray<B2RawRewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDiceEventMission)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, event_type)
		B2_DECLARE_FIELD(2, int32, goal_count)
		B2_DECLARE_FIELD(3, int32, point)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDiceEventCell)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, cell_number)
		B2_DECLARE_FIELD(2, int32, cell_type)
		B2_DECLARE_FIELD(3, int32, reward_id)
		B2_DECLARE_FIELD(4, int32, reward_count)
		B2_DECLARE_FIELD(5, int32, effect_value)
	DECLARE_END(6)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDiceEventFinishReward)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, number)
		B2_DECLARE_FIELD(2, int32, reward_id)
		B2_DECLARE_FIELD(3, int32, reward_count)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DiceEventMission)
		B2_DECLARE_FIELD(0, int32, event_type)
		B2_DECLARE_FIELD(1, int32, progress_count)
		B2_DECLARE_FIELD(2, int32, state)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DiceEventStatus)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, point)
		B2_DECLARE_FIELD(2, int32, free_dice)
		B2_DECLARE_FIELD(3, int32, position)
		B2_DECLARE_FIELD(4, TArray<B2DiceEventMissionPtr>, missions)
		B2_DECLARE_FIELD(5, int32, finish_count)
		B2_DECLARE_FIELD(6, int32, buy_point_count)
		B2_DECLARE_FIELD(7, int32, roll_dice_count)
		B2_DECLARE_FIELD(8, int32, mileage_reward_value)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2ItemSpotSaleOption)
		B2_DECLARE_FIELD(0, TArray<int32>, inventory_types)
		B2_DECLARE_FIELD(1, int32, item_grade)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SoldItemInfo)
		B2_DECLARE_FIELD(0, int32, template_id)
		B2_DECLARE_FIELD(1, int32, quality)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2AetherSpotSaleOption)
		B2_DECLARE_FIELD(0, int32, aether_grade)
	DECLARE_END(1)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2SoldAetherInfo)
		B2_DECLARE_FIELD(0, int32, aether_grade)
		B2_DECLARE_FIELD(1, int32, tier)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDiceEventBuyPoint)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, buy_count)
		B2_DECLARE_FIELD(2, int32, gem)
		B2_DECLARE_FIELD(3, int32, point)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDiceEventMileage)
		B2_DECLARE_FIELD(0, int32, event_id)
		B2_DECLARE_FIELD(1, int32, index)
		B2_DECLARE_FIELD(2, int32, dice_count)
		B2_DECLARE_FIELD(3, TArray<B2RawRewardPtr>, rewards)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DonationPointRewardState)
		B2_DECLARE_FIELD(0, int32, index)
		B2_DECLARE_FIELD(1, bool, rewarded)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2DonationPresent)
		B2_DECLARE_FIELD(0, int32, item_template_id)
		B2_DECLARE_FIELD(1, int32, amount)
		B2_DECLARE_FIELD(2, int32, donation_consumable_type)		// 기부 보상 : 소모품 타입 정보 (UNDEFINED 이면 장비 아이템)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDonationEventInfo)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, int32, cost_type)
		B2_DECLARE_FIELD(2, int32, cost)
		B2_DECLARE_FIELD(3, int32, extra_cost)
		B2_DECLARE_FIELD(4, int32, cut_off_point)
		B2_DECLARE_FIELD(5, int32, obtain_point)
		B2_DECLARE_FIELD(6, int32, bundle_cost)
		B2_DECLARE_FIELD(7, int32, extra_bundle_cost)
		B2_DECLARE_FIELD(8, int32, bundle_obtain_point)
		B2_DECLARE_FIELD(9, int32, serial_donation_count)
		B2_DECLARE_FIELD(10, int64, start_time)
		B2_DECLARE_FIELD(11, int64, end_time)
		B2_DECLARE_FIELD(12, int64, expire_time)
		B2_DECLARE_FIELD(13, FString, title)
		B2_DECLARE_FIELD(14, FString, description)
		B2_DECLARE_FIELD(15, FString, banner_url)
		B2_DECLARE_FIELD(16, FString, link_text)
		B2_DECLARE_FIELD(17, FString, link_url)
		B2_DECLARE_FIELD(18, int32, donation_template_id)
	DECLARE_END(19)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDonationPointRankingRewardInfo)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, int32, rank)
		B2_DECLARE_FIELD(2, int32, reward_id1)
		B2_DECLARE_FIELD(3, int32, reward_count1)
		B2_DECLARE_FIELD(4, int32, reward_id2)
		B2_DECLARE_FIELD(5, int32, reward_count2)
		B2_DECLARE_FIELD(6, int32, reward_id3)
		B2_DECLARE_FIELD(7, int32, reward_count3)
		B2_DECLARE_FIELD(8, int32, reward_id4)
		B2_DECLARE_FIELD(9, int32, reward_count4)
		B2_DECLARE_FIELD(10, int32, reward_id5)
		B2_DECLARE_FIELD(11, int32, reward_count5)
	DECLARE_END(12)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDonationBundleFixedRewardInfo)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, int32, reward_id)
		B2_DECLARE_FIELD(2, int32, min_amount)
		B2_DECLARE_FIELD(3, int32, max_amount)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdDonationPointStepRewardInfo)
		B2_DECLARE_FIELD(0, int32, donation_id)
		B2_DECLARE_FIELD(1, int32, step)
		B2_DECLARE_FIELD(2, int32, point)
		B2_DECLARE_FIELD(3, int32, reward_id1)
		B2_DECLARE_FIELD(4, int32, reward_count1)
		B2_DECLARE_FIELD(5, int32, reward_id2)
		B2_DECLARE_FIELD(6, int32, reward_count2)
		B2_DECLARE_FIELD(7, int32, reward_id3)
		B2_DECLARE_FIELD(8, int32, reward_count3)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFairyInfo)
		B2_DECLARE_FIELD(0, int32, fairy_type)
		B2_DECLARE_FIELD(1, int32, option_id)
		B2_DECLARE_FIELD(2, int32, bless_level)
		B2_DECLARE_FIELD(3, float, option_value)
		B2_DECLARE_FIELD(4, int32, level)
	DECLARE_END(5)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFairyLevelupCost)
		B2_DECLARE_FIELD(0, int32, level)
		B2_DECLARE_FIELD(1, int32, material_cost)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdFairyOpenInfo)
		B2_DECLARE_FIELD(0, int32, fairy_type)
		B2_DECLARE_FIELD(1, int32, stage_id)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2CharacterPowerSpecific)
		B2_DECLARE_FIELD(0, int32, category)
		B2_DECLARE_FIELD(1, int32, power)
	DECLARE_END(2)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTotemInfo)
		B2_DECLARE_FIELD(0, int32, template_id)
		B2_DECLARE_FIELD(1, int32, group_id)
		B2_DECLARE_FIELD(2, int32, bonus_id)
		B2_DECLARE_FIELD(3, int32, grade)
		B2_DECLARE_FIELD(4, int32, sell_price)
		B2_DECLARE_FIELD(5, int32, equip_slot)
		B2_DECLARE_FIELD(6, int32, refine_limit_count1)
		B2_DECLARE_FIELD(7, int32, refine_limit_count2)
		B2_DECLARE_FIELD(8, int32, refine_limit_count3)
	DECLARE_END(9)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdSelectTotemOption)
		B2_DECLARE_FIELD(0, int32, option_id)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, float, min_main_value)
		B2_DECLARE_FIELD(3, float, max_main_value)
		B2_DECLARE_FIELD(4, float, min_sub_value)
		B2_DECLARE_FIELD(5, float, max_sub_value)
		B2_DECLARE_FIELD(6, int32, decimal_point)
	DECLARE_END(7)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTotemRefineInfo)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, refine_chance)
		B2_DECLARE_FIELD(2, int32, gold_cost)
		B2_DECLARE_FIELD(3, int32, dimensional_piece_cost)
	DECLARE_END(4)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTotemRefineRateInfo)
		B2_DECLARE_FIELD(0, int32, grade)
		B2_DECLARE_FIELD(1, int32, base_rate)
		B2_DECLARE_FIELD(2, int32, gold_decrease_rate)
		B2_DECLARE_FIELD(3, int32, gold_increase_rate)
		B2_DECLARE_FIELD(4, int32, piece_decrease_rate)
		B2_DECLARE_FIELD(5, int32, piece_increase_rate)
		B2_DECLARE_FIELD(6, int32, min_rate)
		B2_DECLARE_FIELD(7, int32, max_rate)
	DECLARE_END(8)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTotemDismantleInfo)
		B2_DECLARE_FIELD(0, int32, refine_level)
		B2_DECLARE_FIELD(1, int32, grade)
		B2_DECLARE_FIELD(2, int32, amount)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTotemBonusOptionInfo)
		B2_DECLARE_FIELD(0, int32, bonus_id)
		B2_DECLARE_FIELD(1, int32, condition1)
		B2_DECLARE_FIELD(2, int32, bonus_option_id1)
		B2_DECLARE_FIELD(3, float, bonus_value1)
		B2_DECLARE_FIELD(4, int32, condition2)
		B2_DECLARE_FIELD(5, int32, bonus_option_id2)
		B2_DECLARE_FIELD(6, float, bonus_value2)
		B2_DECLARE_FIELD(7, int32, condition3)
		B2_DECLARE_FIELD(8, int32, bonus_option_id3)
		B2_DECLARE_FIELD(9, float, bonus_value3)
	DECLARE_END(10)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdTotemPrefixLotteryInfo)
		B2_DECLARE_FIELD(0, int32, id)
		B2_DECLARE_FIELD(1, int32, base_prefix)
		B2_DECLARE_FIELD(2, int32, grade)
	DECLARE_END(3)

	//////////////////////////////////////////////////////////////////////////
	DECLARE_BEGIN(B2mdExtendMessage)
		B2_DECLARE_FIELD(0, TArray<B2mdTotemInfoPtr>, md_totem_infos)
		B2_DECLARE_FIELD(1, TArray<B2mdSelectTotemOptionPtr>, md_select_totem_options)
		B2_DECLARE_FIELD(2, TArray<B2mdTotemRefineInfoPtr>, md_totem_refine_infos)
		B2_DECLARE_FIELD(3, TArray<B2mdTotemRefineRateInfoPtr>, md_totem_refine_rate_infos)
		B2_DECLARE_FIELD(4, TArray<B2mdTotemDismantleInfoPtr>, md_totem_dismantle_infos)
		B2_DECLARE_FIELD(5, TArray<B2mdTotemBonusOptionInfoPtr>, md_totem_bonus_option_infos)
		B2_DECLARE_FIELD(6, TArray<B2mdDimensionInfoPtr>, md_dimension_infos)
		B2_DECLARE_FIELD(7, TArray<B2mdDimensionWeekRewardPtr>, md_dimension_week_rewards)
		B2_DECLARE_FIELD(8, TArray<B2mdTotemPrefixLotteryInfoPtr>, md_totem_prefix_lottery_infos)
		B2_DECLARE_FIELD(9, TArray<B2mdItemSealCostPtr>, md_item_seal_cost)
		B2_DECLARE_FIELD(10, TArray<B2mdSelectItemSealOptionPtr>, md_select_item_seal_option)
		B2_DECLARE_FIELD(11, TArray<B2mdCostumeEnhanceCostPtr>, md_costume_enhance_cost)
		B2_DECLARE_FIELD(12, TArray<B2mdExchangeSystemInfoPtr>, md_exchange_system_info)
		B2_DECLARE_FIELD(13, TArray<B2mdExchangeSystemCostPtr>, md_exchange_system_cost)
		B2_DECLARE_FIELD(14, TArray<B2mdQualityTransRateInfoPtr>, md_quality_trans_rate_info)
		B2_DECLARE_FIELD(15, TArray<B2mdLotteryShopRateQualityInfoPtr>, md_lottery_shop_rate_quality_info)
		B2_DECLARE_FIELD(16, TArray<B2mdFactoryInfoPtr>, md_factory_info)
		B2_DECLARE_FIELD(17, TArray<B2mdFactoryMaterialTransInfoPtr>, md_factory_material_trans_info)
		B2_DECLARE_FIELD(18, TArray<B2mdFactoryLimitInfoPtr>, md_factory_limit_info)
		B2_DECLARE_FIELD(19, TArray<B2mdSealBoxInfoPtr>, md_seal_box_info)
		B2_DECLARE_FIELD(20, TArray<B2mdSealBoxRewardInfoPtr>, md_seal_box_reward_info)
		B2_DECLARE_FIELD(21, TArray<B2mdSealBoxRewardRateInfoPtr>, md_seal_box_reward_rate_info)
	DECLARE_END(22)
};
