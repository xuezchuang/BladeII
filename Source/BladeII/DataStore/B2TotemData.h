// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "CommonStruct.h"
#include "B2MessageInfoCommonStruct.h"
#include "CommonStructCombatOption.h"
/**
 * 
 */

#define  MAX_TOTEM_SUB_OPTION 3
#define  MAX_TOTEM_BONUS_OPTION 3

UENUM(BlueprintType)
enum class ETotemSubOptionType : uint8
{
	OPTION_PLUS = 0,
	OPTION_MINUS,
};

UENUM(BlueprintType)
enum class ETotemEquipSlotState : uint8
{
	TOTEM_UNEQUIP = 0,
	TOTEM_EQUIP,
	TOTEM_LOCK,
};

UENUM(BlueprintType)
enum class ETotemEquipSlot : uint8
{
	TOTEM_NONE = 0,
	
	TOTEM_TOP,
	TOTEM_MIDDLE,
	TOTEM_BOTTOM,
};

UENUM(BlueprintType)
enum class ETotemRefineState : uint8
{
	NONE = 0,
	SUCCESS,
	FAIL,
};


/////////////////////////////////////////////////////////////////////////////////////
//
// 付胶磐 单捞磐
//
/////////////////////////////////////////////////////////////////////////////////////

struct FB2TotemInfo
{
	FB2TotemInfo(const b2network::B2mdTotemInfoPtr& Info)
	{
		TemplateID = Info->template_id;
		GroupID = Info->group_id;
		BonusID = Info->bonus_id;
		Grade = Info->grade;
		SellPrice = Info->sell_price;
		RefineLimitCounts.Add(Info->refine_limit_count1);
		RefineLimitCounts.Add(Info->refine_limit_count2);
		RefineLimitCounts.Add(Info->refine_limit_count3);
		EquipSlot = Info->equip_slot;
	}

	explicit FB2TotemInfo()
		: TemplateID(-1), GroupID(-1), BonusID(-1), Grade(-1), SellPrice(-1), EquipSlot(-1)
	{
		RefineLimitCounts.Add(-1);
		RefineLimitCounts.Add(-1);
		RefineLimitCounts.Add(-1);
	}

	int32 TemplateID;
	int32 GroupID;
	int32 BonusID;
	int32 Grade;
	int32 SellPrice;
	int32 EquipSlot;
	TArray<int32, TFixedAllocator<MAX_TOTEM_SUB_OPTION>> RefineLimitCounts;
};

struct FB2SelectTotemOption
{
	FB2SelectTotemOption(const b2network::B2mdSelectTotemOptionPtr& Info)
	{
		OptionID = Info->option_id;
		Grade = Info->grade;
		MinMainValue = Info->min_main_value;
		MaxMainValue = Info->max_main_value;
		MinSubValue = Info->min_sub_value;
		MaxSubValue = Info->max_sub_value;
		DecimalPoint = Info->decimal_point;
	}

	explicit FB2SelectTotemOption()
		: OptionID(-1), Grade(-1), MinMainValue(0.0f), MaxMainValue(0.0f), MinSubValue(0.0f), MaxSubValue(0.0f), DecimalPoint(-1)
	{
	}

	int32 OptionID;
	int32 Grade;
	float MinMainValue;
	float MaxMainValue;
	float MinSubValue;
	float MaxSubValue;
	int32 DecimalPoint;
};

struct FB2TotemRefineInfo
{
	FB2TotemRefineInfo(const b2network::B2mdTotemRefineInfoPtr& Info)
	{
		Grade = Info->grade;
		RefineChance = Info->refine_chance;
		GoldCost = Info->gold_cost;
		DimensionalPieceCost = Info->dimensional_piece_cost;
	}
	explicit FB2TotemRefineInfo()
		: Grade(-1), RefineChance(-1), GoldCost(-1), DimensionalPieceCost(-1)
	{
	}

	int32 Grade;
	int32 RefineChance;
	int32 GoldCost;
	int32 DimensionalPieceCost;
};

struct FB2TotemRefineRateInfo
{
	FB2TotemRefineRateInfo(const b2network::B2mdTotemRefineRateInfoPtr& Info)
	{
		Grade = Info->grade;
		BaseRate = Info->grade;
		GoldDecreaseRate = Info->gold_decrease_rate;
		GoldIncreaseRate = Info->gold_increase_rate;
		PieceDecreaseRate = Info->piece_decrease_rate;
		PieceIncreaseRate = Info->piece_increase_rate;
		MinRate = Info->min_rate;
		MaxRate = Info->max_rate;
	}
	explicit FB2TotemRefineRateInfo()
		: Grade(-1), BaseRate(-1), GoldDecreaseRate(-1), GoldIncreaseRate(-1), PieceDecreaseRate(-1), PieceIncreaseRate(-1), MinRate(-1), MaxRate(-1)
	{
	}

	int32 Grade;
	int32 BaseRate;
	int32 GoldDecreaseRate;
	int32 GoldIncreaseRate;
	int32 PieceDecreaseRate;
	int32 PieceIncreaseRate;
	int32 MinRate;
	int32 MaxRate;
};

struct FB2TotemBonusOptionData
{
	FB2TotemBonusOptionData(int32 InCondition, int32 InBonusOptionID, float InBonusValue)
	{
		Condition = InCondition;
		BonusOptionID = InBonusOptionID;
		BonusValue = InBonusValue;
	}

	explicit FB2TotemBonusOptionData()
		: Condition(-1), BonusOptionID(-1), BonusValue(0.0f)
	{
	}

	int32 Condition;
	int32 BonusOptionID;
	float BonusValue;
};

struct FB2TotemBonusOptionInfo
{
	FB2TotemBonusOptionInfo(const b2network::B2mdTotemBonusOptionInfoPtr& Info)
	{
		BonusID = Info->bonus_id;
		OptionDatas.Add(FB2TotemBonusOptionData(Info->condition1, Info->bonus_option_id1, Info->bonus_value1 / 100.0f));
		OptionDatas.Add(FB2TotemBonusOptionData(Info->condition2, Info->bonus_option_id2, Info->bonus_value2 / 100.0f));
		OptionDatas.Add(FB2TotemBonusOptionData(Info->condition3, Info->bonus_option_id3, Info->bonus_value3 / 100.0f));
	}

	explicit FB2TotemBonusOptionInfo()
		: BonusID(-1)
	{
		OptionDatas.Add(FB2TotemBonusOptionData(-1, -1, 0.0f));
		OptionDatas.Add(FB2TotemBonusOptionData(-1, -1, 0.0f));
		OptionDatas.Add(FB2TotemBonusOptionData(-1, -1, 0.0f));
	}

	int32 BonusID;
	TArray<FB2TotemBonusOptionData, TFixedAllocator<MAX_TOTEM_BONUS_OPTION>> OptionDatas;
};

struct FB2DimensionInfo
{
	FB2DimensionInfo(const b2network::B2mdDimensionInfoPtr& Info)
	{
		Difficulty_level = Info->difficulty_level;
		Difficulty_scale = Info->difficulty_scale;
		Fixed_drop_item_id1 = Info->fixed_drop_item_id1;
		Fixed_drop_count1 = Info->fixed_drop_count1;
		Drop_totem_count_min = Info->drop_totem_count_min;
		Drop_totem_count_max = Info->drop_totem_count_max;
		Recommend_power = Info->recommend_power;
		Recommend_attack = Info->recommend_attack;
		Recommend_defense = Info->recommend_defense;
	}
	explicit FB2DimensionInfo()
		: Difficulty_level(-1), Difficulty_scale(-1), Fixed_drop_item_id1(-1),
		Fixed_drop_count1(-1), Drop_totem_count_min(-1), Drop_totem_count_max(-1),
		Recommend_power(-1), Recommend_attack(-1), Recommend_defense(-1)
	{
	}

	int32 Difficulty_level;
	int32 Difficulty_scale;
	int32 Fixed_drop_item_id1;
	int32 Fixed_drop_count1;
	int32 Drop_totem_count_min;
	int32 Drop_totem_count_max;
	int32 Recommend_power;
	int32 Recommend_attack;
	int32 Recommend_defense;
};

struct FB2DimensionWeekRewardInfo
{
	FB2DimensionWeekRewardInfo(const b2network::B2mdDimensionWeekRewardPtr& Info)
	{
		RankIndex = Info->index;
		Ranking_min = Info->ranking_min;
		Ranking_max = Info->ranking_max;

		for (auto el : Info->rewards)
		{
			Rewards.Add(el);
		}
	}
	explicit FB2DimensionWeekRewardInfo()
		: RankIndex(-1), Ranking_min(-1), Ranking_max(-1)
	{

	}

	int32 RankIndex;
	int32 Ranking_min;
	int32 Ranking_max;
	TArray<b2network::B2RawRewardPtr> Rewards;
};

struct FB2TotemDismantleInfo
{
	FB2TotemDismantleInfo(const b2network::B2mdTotemDismantleInfoPtr& Info)
	{
		RefineLevel = Info->refine_level;
		Grade = Info->grade;
		Amount = Info->amount;
	}

	explicit FB2TotemDismantleInfo()
		: RefineLevel(-1), Grade(-1), Amount(-1)
	{
	}

	int32 RefineLevel;
	int32 Grade;
	int32 Amount;
};

/////////////////////////////////////////////////////////////////////////////////////
//
// 匙飘况农 单捞磐
//
/////////////////////////////////////////////////////////////////////////////////////

struct FB2TotemRefineState
{
	FB2TotemRefineState();
	FB2TotemRefineState(int32 InIndex, ETotemRefineState InRefineState);

	int32 Index;
	ETotemRefineState RefineState;
};

struct FB2TotemSubOptionDetail
{
	FB2TotemSubOptionDetail();
	FB2TotemSubOptionDetail(const b2network::B2TotemSubOptionDetailPtr ptr);

	FItemOption SubOption;
	int32 RefineCount;
	TArray<FB2TotemRefineState> RefineStates;
};

#define TOTEM_DECREASE_VARIATION_INDEX 2
struct FB2Totem
{
	FB2Totem();
	FB2Totem(const b2network::B2TotemServerInfoPtr ptr);

	float GetCombat();

	bool IsEquipped() const;
	// 力访 肯丰
	bool IsSmeltingComplete() const;
	// 力访 吝
	bool IsSmelting() const;

	int32 GetMaxSmeltingCount() const;
	int32 GetSubOptionTryCount(int32 OptionIndex) const;

	int64 ID;
	int32 RefID;
	EPCClass OwnerPCClass;
	ETotemEquipSlot EquipSlot;
	ETotemGrade Grade;
	FItemOption MainOptionDetail;
	TArray<FB2TotemSubOptionDetail> SubOptionDetails;
	int32 RefineLevel;
	int32 RefineTryCount;
	int32 GoldSuccessRate;
	int32 PieceSuccessRate;
	float Power;
	bool IsLock;
	bool IsNew;

	// 配袍阑 Item栏肺 函券茄促. 葛电 沥焊甫 淬柳 给窍绊, ID殿阑 傍蜡窍咯 皋抛府倔阑 掘绰 惑炔俊辑 荤侩茄促.
	void GetItemFormat(FB2Item& OutItem);
};