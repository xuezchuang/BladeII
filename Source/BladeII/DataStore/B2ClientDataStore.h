#ifndef __B2_CLIENT_DATA_STORE_H__
#define __B2_CLIENT_DATA_STORE_H__
//#include "BladeII.h"
#include "EngineMinimal.h"
#include "B2ModPlayerInfoData.h"
#include "B2RedDotManager.h"
#include "CharacterDataStore.h"
#include "B2UIEnum.h"
#include "BladeIICharacter.h"
#define SvrToCliBalancingScaleFactor(x) x * 0.01f

const int32 MAX_GOLD = 2100000000;

/** To calculate post-enhance item level in client. The final value comes from server, but client need this for some (highly frequent) preview stuff. */
struct FItemLevelupFactorMap
{
	struct FitemOptionValue
	{
		FitemOptionValue() {}
		FitemOptionValue(float value, float power)
		{
			Add_Value = value;
			Power_Factor = power;
		}
		~FitemOptionValue() {}

		float Add_Value;
		float Power_Factor;
	};

	FItemLevelupFactorMap();

	// For all of them, array index is the key.
	TArray<float> TargetItemGradeToFactor;
	TArray<float> IngredItemGradeToFactor;
	TArray<float> IngredItemEquipPosToFactor;
	TArray<float> IngredItemLevelupToFactor;
	TArray<float> IngredItemSurpassCountToFactor;
	//TMap<EItemOption, float> TargetItemOptionIncFactor;
	TMap<EItemOption, FitemOptionValue> TargetItemOptionIncFactor;

	void UpdateByServerSync(const TArray<b2network::B2mdItemLevelupExpFactorPtr>& ServerExpFactor, const TArray<b2network::B2mdItemOptionAddValuePtr>& ServerOptionFactor);
};

struct FItemLevelupFactor
{
	FItemLevelupFactor() : ItemLevel(0), LevelupFactor(0.0) {}

	int32	ItemLevel;
	float	LevelupFactor;
};

struct FItemSurpassLevel
{
	FItemSurpassLevel()
	{
		Grade = 0;
		IncreaseLevel = 0;
		MaxSurpassLevel = 0;
	}

	int32 Grade;
	int32 IncreaseLevel;
	int32 MaxSurpassLevel;
};


/** Another struct for levelup preview - levelup cost preview, */
struct FItemLevelupCostPerLevel
{
private:
	// The structure here follows B2mdItemLevelupCost
	TArray<int32> CostPerGrade_Weapon; // Array index is grade - 1.
	TArray<int32> CostPerGrade_Protection;
	TArray<int32> CostPerGrade_Accessory;
public:
	bool HasDataForInvenType(EItemInvenType InItemInvenType) const;
	int32 GetCostForGrade(EItemInvenType InItemInvenType, int32 InGrade);
	void UpdateByServerSync(const b2network::B2mdItemLevelupCostPtr& ServerLevelupCostStruct);
};

enum class EResurrectGameModeType : uint8	// ServerData Sync.
{
	EResurrectType_OnlyClientMode = 0,		// 클라이언트에서만 부활하는 모드  (점령전)
	EResurrectType_Stage = 1,				// 스테이지 부활모드				 (Default, Stage)
	EResurrectType_Raid = 2,				// 레이드 부활모드 타입			 (Raid)
	EResurrectType_Max,
};

// Resurrect master data without resurrection context label, per resurrect count, and possibly for mode type too.. 
struct FResurrectMasterDataElement
{
	FResurrectMasterDataElement()
	{
		price = 0;
	}
	int32 price;

private:
	TMap<EResurrectBuffType, int32> BuffValueMap;

public:
	void UpdateByServerSync(const b2network::B2mdResurrectionInfoPtr& InThisServerInfo);
	int32 GetBuffValueOfType(EResurrectBuffType InType);
};

struct FModeResurrectMasterDataElement
{
	FModeResurrectMasterDataElement()
	{
		ResurrectElementList.Empty();
	}

	FModeResurrectMasterDataElement(int32 Key, FResurrectMasterDataElement newEle)
	{
		ResurrectElementList.Add(Key, newEle);
	}

	TMap<int32, FResurrectMasterDataElement>	ResurrectElementList;		// Key is Count
};

struct FSingleStageBuffMasterDataElem // For a StageBuffType
{
	FSingleStageBuffMasterDataElem()
	{
		TheAmount = 1.0f;
		RequiredGoldAmount = 0;
	}

	float TheAmount; // Not for all types. The specific usage is different for each type.

	// Some sort of special ticket can be added, which can be used instead of gem or gold.
	int32 RequiredGoldAmount;
};

struct FTeamMatchFormationBuffInfo
{
	int32 Id;
	int32 BuffType1;
	float BuffValue1;
	int32 BuffType2;
	float BuffValue2;
};

struct FTeamMatchFormationLevelInfo
{
	int32 EnhanceGoldCost;
	TMap<int32, FTeamMatchFormationBuffInfo> BuffInfo;
};

struct FTeamMatchFormationTypeInfo
{
	FTeamMatchFormationTypeInfo(int32 Type, int32 InMaxLevel = 0) :MaxLevel(InMaxLevel), FormationType(Type){}

	int32 MaxLevel;
	int32 FormationType;
	TMap<int32, FTeamMatchFormationLevelInfo> LevelInfo;
};

struct FBrevetNodeInfo
{
	FBrevetNodeInfo()
	{
		OpenLevel = 0;
		StatusType = 0;
		StatusValue = 0;
		NeedRankPromotionPoint = 0;
		NeedActiveGold = 0;
		Rate = 0;
	}
	int32 OpenLevel;
	int32 StatusType;
	int32 StatusValue;
	int32 NeedRankPromotionPoint;
	int32 NeedActiveGold;
	int32 Rate;

	int32 TotalAtk;
	int32 TotalDef;
	int32 TotalHp;
};

struct FBrevetRankInfo
{
	FBrevetRankInfo(int32 Rank) : BrevetRank(Rank)
	{
		BrevetRank = 0;
	}

	int32 BrevetRank;
	TMap<int32, FBrevetNodeInfo> BrevetNodeInfo;
};

struct FModReplayInfo
{
    FString Url;
    int32 SizeCompressed;
    int32 SizeUncompressed;
};

struct FModRankerInfo
{
	int32 Rank;
	int32 Grade;
	int32 GradeStar;
	FString UserName;
	TArray<EPCClass> Entry;
	TArray<int32> EntryLevels;
	int32 TotalCombatPower;
	int32 ClearTimeSec;
	int32 BestFloor;
	int32 AccountID;
	float RankingPercentage;
    FModReplayInfo ReplayInfo;
};

struct FModRankerInfoContainer
{
private:
	TArray<FModRankerInfo> RankingList;

public:
	//On Change RankingList 
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRankingListChanged, const TArray<FModRankerInfo>&);
	FOnRankingListChanged	OnRankingListChanged;

	void SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank);
	FORCEINLINE const TArray<FModRankerInfo>* GetRankerList() { return &RankingList; }
	FORCEINLINE void ClearRankerList() { RankingList.Empty(); }
};

struct FRewardItem
{
	FRewardItem(int32 InId, int32 InMinCount, int32 InMaxCount = 0) : Id(InId), Count(InMinCount), MaxCount(InMaxCount) {}
	int32 Id;
	int32 Count;
	int32 MaxCount;
};

struct FCounterDungeon
{
	FServerStageID ServerStageId;
	int32 DungeonType;
	int32 Difficulty;
	int32 DifficultyLevel;
	int32 DifficultyScale;
	int32 RecommendCombat;
	int32 RecommendAttack;
	int32 RecommendDefense;
	TArray<FRewardItem>	RewardId;
};

struct FCounterDungeonPeriod
{
	int32 DungeonType;
	int32 DayOfWeek;
	int32 BeginHour;
	int32 BeginMin;
	int32 EndHour;
	int32 EndMin;
	EPCClass CharacterType;
};

struct FCounterDungeunWeekReward
{
	int32 RequireCount;
	TArray<FRewardItem>	RewardItem;
};

struct FModDailyReward
{
	int32 Match_Type;
	int32 Require_Count;
	int32 Reward_Gold;
	int32 Reward_BladePoint;
	int32 Reward_Gem;
	int32 Reward_ItemId;
};

struct FModWeeklyReward
{
	int32 Grade;
	int32 Ranking_Min;
	int32 Ranking_Max;
	int32 Reward_Gem;

	TArray<b2network::B2RewardPtr> rewardItems;
};

struct FRewardData
{
	FRewardData(ERewardType InType, int32 InRewardId, int32 InRewardPushType)
		:Type(InType), RewardId(InRewardId), RewardPushType(InRewardPushType) {}

	ERewardType Type;
	int32 RewardId;
	int32 RewardPushType; // Mail or Inven
};

struct FGuildInfo
{
	FB2ResponseGuildInfoPtr					GuildInfo;
	TArray<b2network::B2GuildMemberPtr>		GuildMemberInfo;
};

struct FUserGuildInfo
{
	FUserGuildInfo() {}
	FUserGuildInfo(FString name, int32 mark, int32 markcolor, int32 markbg, int32 markbgcolor, int32 rank)
	{
		GuildName = name;
		Mark_Index = mark;
		MarkColor_Index = markcolor;
		MarkBG_Index = markbg;
		MarkBGColor_Index = markbgcolor;
		GuildRank = rank;
	}

	b2network::B2GuildSkillPtr GetGuildSkill(int32 Id)
	{
		if (CurrentGuildSkill.Contains(Id))
			return CurrentGuildSkill[Id];

		return nullptr;
	}

	void SetGuildSkill(b2network::B2GuildSkillPtr InSkill)
	{
		CurrentGuildSkill.Add(InSkill->id, InSkill);
	}
	FString GuildName;
	int32	Mark_Index;
	int32	MarkColor_Index;
	int32	MarkBG_Index;
	int32	MarkBGColor_Index;
	int32	GuildRank;

	TMap<int32, b2network::B2GuildSkillPtr>					CurrentGuildSkill;
};

//////////////////////////////////////////////////////////////////////
// Wing master data client struct

struct FWingMD_EnhanceSingleElem
{
	FWingMD_EnhanceSingleElem()
	{
		TargetGrade = 1;
		TargetLevel = 0;

		RequiredItemRefID = 0;//FItemRefIDHelper::GetWingEnhanceResourceRefID(); // It is just expected one.. kk
		RequiredItemCount = 1;
		EnhancePointToAcquirePerTrial = 10;
		RequiredEnhancePoint = 100;
		RequiredGoldCost = 100;

		AttackPoint = 0.0f;
		DefensePoint = 0.0f;
		HealthPoint = 0.0f;
	}

	FWingMD_EnhanceSingleElem& operator=(const b2network::B2mdWingEnhancePtr& ServerStruct);

	// TargetGrade/Level compose key for whole table
	int32 TargetGrade;
	int32 TargetLevel;

	// Util to generate map key to access single elem of wanted target grade/level.
	FORCEINLINE static int32 GetKey(int32 InGrade, int32 InLevel) { return ((InGrade * 100) + InLevel); }
	FORCEINLINE int32 GetMyKey() { return GetKey(TargetGrade, TargetLevel); }

	// Those are required some stuff for next TargetLevel
	int32 RequiredItemRefID;
	int32 RequiredItemCount;
	int32 EnhancePointToAcquirePerTrial; // You will get this amount of EnhancePoint per single try using enhance item.
	int32 RequiredEnhancePoint;
	int32 RequiredGoldCost;

	// Three prim points expected at this target grade/level.
	float AttackPoint;
	float DefensePoint;
	float HealthPoint;
};

struct FWingMD_EvolveSingleElem
{
	FWingMD_EvolveSingleElem()
	{
		TargetGrade = 1;

		RequiredItemRefID = 0; // FItemRefIDHelper::GetWingEvolveResourceRefID(); The expected one.
		RequiredItemCount = 1;
		RequiredGoldCost = 100;
		SuccessRate = 0.5f;
		MaxFailPoint = 100;

		AttackPoint = 0.0f;
		DefensePoint = 0.0f;
		HealthPoint = 0.0f;
	}

	FWingMD_EvolveSingleElem& operator=(const b2network::B2mdWingUpgradePtr& ServerStruct);

	int32 TargetGrade; // Key for the whole table of this.

	// It just uses Grade for now, but for unified interface.. and who knows?
	FORCEINLINE static int32 GetKey(int32 InGrade) { return InGrade; }
	FORCEINLINE int32 GetMyKey() { return GetKey(TargetGrade); }

	// Required info for evolution (to next grade).
	int32 RequiredItemRefID;
	int32 RequiredItemCount;
	int32 RequiredGoldCost;

	float SuccessRate; // Evolution success rate, 0.0 ~ 1.0 scale.
	int32 MaxFailPoint; // Cannot fail more than this time, it automatically evolve after this times of failure.

						// Three prim points expected at this target grade. additionally applied to the values by Enhance table.
	float AttackPoint;
	float DefensePoint;
	float HealthPoint;
};

struct FWingMD_PropOptionSingleElem
{
	FWingMD_PropOptionSingleElem()
	{
		TargetPCClass = EPCClass::EPC_End;
		TargetOptionIndex = 0;
		TargetOptionLevel = 1;

		OptionType = EItemOption::EIO_End;
		OptionValue = 0.0f;
		RequiredItemRefID = 0; //FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID(); The expected one.
		RequiredItemCount = 1;
	}

	FWingMD_PropOptionSingleElem& operator=(const b2network::B2mdWingOptionValuePtr& ServerStruct);

	// PCClass/OptionIndex/OptionLevel compose key for whole table of this.
	EPCClass TargetPCClass;
	int32 TargetOptionIndex; // It is literally the array index of PropOptions, not necessarily mean option type.
	int32 TargetOptionLevel;

	// Util to generate map key to access single elem of wanted target PCClass/OptionIndex/Level
	FORCEINLINE static int32 GetKey(EPCClass InClass, int32 InOptionIndex, int32 InLevel) { return ((PCClassToInt(InClass) * 10000) + (InOptionIndex * 100) + InLevel); }
	FORCEINLINE int32 GetMyKey() { return GetKey(TargetPCClass, TargetOptionIndex, TargetOptionLevel); }

	EItemOption OptionType;
	float OptionValue;
	// Required info for next level of option enhancement
	int32 RequiredItemRefID;
	int32 RequiredItemCount;
};

struct FWingMD_PropOptionOpenSingleElem
{
	FWingMD_PropOptionOpenSingleElem()
	{
		TargetPCClass = EPCClass::EPC_End;
		TargetOptionIndex = 0;
		RequiredWingGrade = 1;
	}

	FWingMD_PropOptionOpenSingleElem& operator=(const b2network::B2mdWingOptionOpenPtr& ServerStruct);

	// PCClass/OptionIndex compose key for whole table of this.
	EPCClass TargetPCClass;
	int32 TargetOptionIndex; // It is literally the array index of PropOptions, not necessarily mean option type.

	// Util to generate map key to access single elem of wanted target PCClass/OptionIndex
	FORCEINLINE static int32 GetKey(EPCClass InClass, int32 InOptionIndex) { return ((PCClassToInt(InClass) * 100) + InOptionIndex); }
	FORCEINLINE int32 GetMyKey() { return GetKey(TargetPCClass, TargetOptionIndex); }

	int32 RequiredWingGrade; // Option of this TargetOptionIndex will be open at this grade.
};

struct FWingMD_PropOptionMaxLevelSingleElem
{
	FWingMD_PropOptionMaxLevelSingleElem()
	{
		TargetPCClass = EPCClass::EPC_End;
		TargetOptionIndex = 0;
		TargetWingGrade = 1;
		MaxPossibleLevel = 1;
	}

	FWingMD_PropOptionMaxLevelSingleElem& operator=(const b2network::B2mdWingOptionMaxLevelPtr& ServerStruct);

	// PCClass/OptionIndex/WingGrade compose key for whole table of this.
	EPCClass TargetPCClass;
	int32 TargetOptionIndex; // It is literally the array index of PropOptions, not necessarily mean option type.
	int32 TargetWingGrade;

	// Util to generate map key to access single elem of wanted target PCClass/OptionIndex/Level
	FORCEINLINE static int32 GetKey(EPCClass InClass, int32 InOptionIndex, int32 InWingGrade) { return ((PCClassToInt(InClass) * 10000) + (InOptionIndex * 100) + InWingGrade); }
	FORCEINLINE int32 GetMyKey() { return GetKey(TargetPCClass, TargetOptionIndex, TargetWingGrade); }

	int32 MaxPossibleLevel; // Option of this key can be enhanced up to this level.
};

struct FMD_ItemSingleElem
{
	FMD_ItemSingleElem()
	{
		ItemTemplateID = 0;
		ItemEquipClass = EPCClass::EPC_End;
		SetGroupID = 0;
		ItemInvenType = EItemInvenType::EIIVT_End;
		ItemEquipPlace = EItemEquipPlace::EIEP_End;
		SetOptionID = 0;
		ItemClass = EItemClass::EIC_End;
		MaxLevel = 0;
		PrimaryType = EItemPrimaryPointType::EIPP_End;
		ItemFactorLevel = 0;
		ItemGrade = 0;
		MaxGrade = 0;
		RandomOptCount = 0;
		NextGradeItemTemplateID = 0;
		BuyPrice = 0;
		SellPrice = 0;
		CostumeEquipPlace = ECostumeEquipPlace::ECEP_None;
		SealSlot = 0;
		SealSlotOpen = 0;
		SellAble = false;
	}

	int32			ItemTemplateID;
	EPCClass		ItemEquipClass;
	int32			SetGroupID;
	EItemInvenType	ItemInvenType;
	EItemEquipPlace	ItemEquipPlace;

	ECostumeEquipPlace CostumeEquipPlace;

	int32			SetOptionID;
	EItemClass		ItemClass;
	int32			MaxLevel;

	EItemPrimaryPointType	PrimaryType;
	float					ItemFactorLevel;
	TArray<FItemOption>		UniqueOptions;
	int32					MaxGrade;
	int32					ItemGrade;
	int32					RandomOptCount;
	int32					NextGradeItemTemplateID;
	int32					BuyPrice;
	int32					SellPrice;

	int32					SealSlot;
	int32					SealSlotOpen;
	bool					SellAble;

	FMD_ItemSingleElem& operator=(const b2network::B2mdItemInfoPtr& ItemMasterInfo);

	FMD_ItemSingleElem& InitCostumeItemInfo(const b2network::B2mdCostumeInfoPtr& ItemMasterInfo);
};

struct FMD_SetInfo
{
	FMD_SetInfo() : SetUniqueKey(0),SetGroupID(0),  SetGrade(0), TargetClass(EPCClass::EPC_End) {}

	int32		SetUniqueKey;

	int32		SetGroupID;
	int32		SetGrade;
	EPCClass	TargetClass;

	TArray<int32>	SetOptionIDs;
	TArray<int32>	SetCompositionItems;
};

struct FItemSetOptionData
{
	int32		SetItemCount;
	EItemOption SetItemOption;
	float		SetItemOptionValue;

	FItemSetOptionData()
		:SetItemCount(0), SetItemOption(EItemOption::EIO_End), SetItemOptionValue(0)
	{}

	FItemSetOptionData(int32 InSetItemCount, EItemOption InSetItemOption, float InSetItemOptionValue)
		:SetItemCount(InSetItemCount), SetItemOption(InSetItemOption), SetItemOptionValue(InSetItemOptionValue)
	{}
};

struct FMD_ItemSetOptionElem
{
	int32		SetOptionID;

	TArray<FItemSetOptionData> SetOptions;

	FMD_ItemSetOptionElem();
	FMD_ItemSetOptionElem& operator=(const b2network::B2mdItemSetPtr& SetOptionMasterInfo);

	const bool GetSetItemOption(int32 InSetItemCount, FItemSetOptionData& SetOptionData) const;
	const void GetAllSetItemOption(TArray<FItemSetOptionData>& SetOptionData, int32 SetItemCount = -1) const;
	const bool HaveSetItemOption() const;
};

struct FCollectionItemData
{
	FCollectionItemData() : ID(0), CollectionState(EItemCollectionState::None), bNewItem(false) {}

	int32					ID;
	EItemCollectionState	CollectionState;
	bool					bNewItem;

	FCollectionItemData& operator=(const b2network::B2CollectionItemPtr CollectionItem);
	bool operator==(const FCollectionItemData& Other);
};

struct FClassCollectionItemData
{
	TMap<EItemClass, TArray<FMD_ItemSingleElem>>	ItemMasterInfos;
	TMap<int32, FMD_SetInfo>						SetItemMasterInfos;

	TMap<EItemClass, TArray<FCollectionItemData>>	CollectionItems;
	TMap<int32, TArray<int32>>						CollectedSetCompositionItems;

	void InitClassItemMasterData();
	void AddItem(const FMD_ItemSingleElem& ItemElem);
	void AddSetItemInfo(const FMD_ItemSingleElem& ItemElem);
	void AddCollectionItem(EItemClass ItemClass, const FCollectionItemData& ItemData);
	void AddSetCollectionItem(const FCollectionItemData& ItemData);

	void SortAllItemMasterInfo();
	void UpdateSetCollectionState();

	FMD_ItemSingleElem* GetSingleItemInfo(int32 ItemTemplateID);

	FCollectionItemData* GetCollectionItemInfo(EItemClass ItemClass, int32 ID);
	FCollectionItemData* GetCollectionItemInfo(int32 ItemTemplateID);
	FCollectionItemData* GetSetCollectionItemInfo(int32 SetUniqueID);

	int32 GetSetCompositionItems(int32 SetUniqueKey, TArray<int32>& OutCompositionItems);
	int32 GetSetCollectionCount();
	int32 GetCollectionItemCount(EItemClass ItemClass);
	int32 GetEnhancedCollectionItemCount(EItemClass ItemClass);

	void ClearCollectionItems(ECollectionType CollectionType);
	void ClearAllCollectionItems();
};

struct FItemMasterData
{
	TMap<EPCClass, FClassCollectionItemData>	CharacterItems;
	TMap<int32, FMD_ItemSetOptionElem>			SetItemOptions;
	TMap<int32, FMD_ItemSingleElem>				NonEquipItems;

	TArray<int32>	UniqueGroupIDList;		/* All Item has their own groupID using as Composition of SetItem */
	TArray<int32>	CostumeUniqueGroupIDList;		

	void InitItemMasterData();
	void AddItemMasterData(const FMD_ItemSingleElem& ItemInfo);
};

struct FMD_HeroTowerFloorSingleElem
{
	FMD_HeroTowerFloorSingleElem()
	{
		nFloor = 0;
		nRecommendCombat = 0;
		nClearLimitTime = 0;
		nMapId = 0;
		nDifficultyScale = 0;
		RecommendAttack = 0;
		RecommendDefense = 0;
	}

	FMD_HeroTowerFloorSingleElem& operator=(const b2network::B2mdHeroTowerFloorPtr& ServerStruct);

	int32 nFloor;
	int32 nRecommendCombat;
	int32 RecommendAttack;
	int32 RecommendDefense;
	int32 nClearLimitTime;
	int32 nMapId;
	int32 nDifficultyScale;
	TArray<FRewardItem>	first_rewards;
	TArray<FRewardItem>	rewards; // for sweep
};

// colosseum comment : mdHeroTowerRetryCost
//struct FMD_HeroTowerRetryCostSingleElem
//{
//	FMD_HeroTowerRetryCostSingleElem()
//	{
//		nTryCount = 0;
//		nGemCost = 0;
//	}
//
//	FMD_HeroTowerRetryCostSingleElem& operator=(const b2network::B2mdHeroTowerRetryCostPtr& ServerStruct);
//
//	int32 nTryCount;
//	int32 nGemCost;
//};

struct FRaidMasterData
{
	int32 RaidType;
	int32 RaidStep;
	int32 DifficultyScale;
	FText RaidName;
	FText RaidDesc;
	int32 RecommendCombatPoint;
	int32 RecommendAttack;
	int32 RecommendDefense;
	int32 MainRewardID;
	int32 MainRewardMinCount;
	int32 MainRewardMaxCount;
	int32 StepRewardID;
	int32 StepRewardMinCount;
	int32 StepRewardMaxCount;

	// When "MainRewardID = 0", you can use it.
	// that case reward Accessary.
	int32 RewardEquipItemMinGrade;
	int32 RewardEquipItemMaxGrade;

	void clear()
	{
		RaidType = 0;
		RaidStep = 0;
		DifficultyScale = 0;
		RaidName = FText::GetEmpty();
		RaidDesc = FText::GetEmpty();
		MainRewardID = 0;
		MainRewardMinCount = 0;
		MainRewardMaxCount = 0;
		StepRewardID = 0;
		StepRewardMinCount = 0;
		StepRewardMaxCount = 0;
		RewardEquipItemMinGrade = 0;
		RewardEquipItemMaxGrade = 0;
		RecommendAttack = 0;
		RecommendDefense = 0;
	}
};

struct FSummonItemGradeRange
{
	FSummonItemGradeRange(int32 min, int32 max, int32 fixMin) : MinGrade(min), MaxGrade(max), FixMinGrade(fixMin) {}
	int32 MinGrade;
	int32 MaxGrade;
	int32 FixMinGrade;

	static int32 GetHashKey(uint32 CostType)
	{
		return (CostType << 1) | (CostType ? 1 : 0);
	}
};

struct FMD_FairyInfo
{
	EFairyType FairyType;
	int32 OptionId;
	int32 BlessLevel;
	int32 Level;
	float OptionValue;

	FMD_FairyInfo()
		: FairyType(EFairyType::FAIRY_NONE)
		, OptionId(0)
		, BlessLevel(0)
		, Level(0)
		, OptionValue(0.0f)
	{}

	FMD_FairyInfo(const b2network::B2mdFairyInfoPtr ServerStruct);
	FMD_FairyInfo& operator=(const b2network::B2mdFairyInfoPtr ServerStruct);
};

struct FMD_FairyLevelUpCost
{
	int32 Level;
	int32 MaterialCost;

	FMD_FairyLevelUpCost()
		: Level(0)
		, MaterialCost(0)
	{}

	FMD_FairyLevelUpCost(const b2network::B2mdFairyLevelupCostPtr ServerStruct);
	FMD_FairyLevelUpCost& operator=(const b2network::B2mdFairyLevelupCostPtr ServerStruct);
};

struct FMD_FairyOpenInfo
{
	EFairyType FairyType;
	int32 StageId;

	FMD_FairyOpenInfo()
		: FairyType(EFairyType::FAIRY_NONE)
		, StageId(0)
	{}
	FMD_FairyOpenInfo(const b2network::B2mdFairyOpenInfoPtr ServerStruct);
	FMD_FairyOpenInfo& operator=(const b2network::B2mdFairyOpenInfoPtr ServerStruct);
};

struct FSummonItemLotterySale
{
	FSummonItemLotterySale(int32 DiscountRate_, int64 StartTime_, int64 EndTime_)
		: DiscountRate(DiscountRate_), StartTime(StartTime_), EndTime(EndTime_)
	{

	}

	int32 DiscountRate;
	int64 StartTime;
	int64 EndTime;

	static int32 GetHashKey(uint32 CostType, bool bIsMultiple, bool bIsAgain)
	{
		return (CostType << 2) | (bIsMultiple ? 1 : 0) << 1 | (bIsAgain ? 1 : 0);
	}
};

struct FSummonItemSlotData
{
	FSummonItemSlotData(uint32 InHashKey, int32 InCost)
		: HashKey(InHashKey), Cost(InCost), NextFreeTime(0), BuyCount(0), LimitdBuyCount(0)
	{

	}

	FSummonItemSlotData(uint32 InHashKey, int32 InCost, int32 InCalculatedCost)
		: HashKey(InHashKey), Cost(InCost), CalculatedCost(InCalculatedCost), NextFreeTime(0), BuyCount(0), LimitdBuyCount(0)
	{

	}

	FSummonItemSlotData(uint32 CostType, bool bIsMultiple, bool bIsAgain, int32 InCost)
		: Cost(InCost)
	{
		HashKey = GetHashKey(CostType, bIsMultiple, bIsAgain);
	}

private:

	int32 HashKey;
	int32 Cost;
	int32 CalculatedCost;
	FDateTime NextFreeTime;
	int32 BuyCount;
	int32 LimitdBuyCount;

public:
	static int32 GetHashKey(uint32 CostType, bool bIsMultiple, bool bIsAgain)
	{
		return (CostType << 2) | (bIsMultiple ? 1 : 0) << 1 | (bIsAgain ? 1 : 0);
	}

	int32 GetCostType() const
	{
		return (HashKey & 0x04) >> 2;
	}

	bool IsMultiple() const
	{
		return (HashKey & 0x02) >> 1 == 1;
	}

	bool IsAgain() const
	{
		return (HashKey & 0x01) == 1;
	}

	int32 GetCost() const
	{
		if (NextFreeTime == 0)
		{
			if (GetCostType() == int32(ESummonItemCost::Friendship))
				return 0;
			else if (!IsMultiple())
				return 0;
		}

		return Cost;
	}

	int32 GetCalculatedCost() const
	{
		return CalculatedCost;
	}

	int32 GetBuyCount() const
	{
		return BuyCount;
	}

	int32 GetLimitdBuyCount() const
	{
		return LimitdBuyCount;
	}

	void SetNextFreeTime(int64 InNextFreeTime)
	{
		NextFreeTime = FDateTime::FromUnixTimestamp(InNextFreeTime / 1000);
	}

	void SetLimitdBuyCount(int32 Count)
	{
		LimitdBuyCount = Count;
	}

	void SetBuyCount(int32 Count)
	{
		BuyCount = Count;
	}

	int64 GetNextFreeTimeLeftSeconds();
};

namespace EStoreCategory	// = B2ShopType
{
	const int32 Package = 0;
	const int32 DarkCrystal = 1;
	const int32 Goods = 2;
	const int32 Fame_Point = 3;

	const int32 MAX = 4;
}

namespace EGoodsCategory
{
	const int32 BLADE_POINT = 0;
	const int32 GEM = 1;
	const int32 GOLD = 2;

	const int32 MAX = 3;
}

namespace EStoreProductType	// = B2ShopProductType
{
	const int32 BLADE_POINT = 0;
	const int32 GEM = 1;
	const int32 GOLD = 2;
	const int32 ITEM = 3;
	const int32 GENERAL_LOTTERY = 4;
	const int32 SELECTIVE_LOTTERY = 5;
	const int32 PACKAGE = 6;
	const int32 DARK_CRYSTAL = 7;
	const int32 GOLD_BUFF_TICKET = 8;
	const int32 EXP_BUFF_TICKET = 9;
	const int32 STAGE_BOOST_TICKET = 10;
	const int32 SHOP_TEN_LOTTERY_TICKET = 11;
	const int32 SELECTIVE_AETHER_LOTTERY = 12;
	const int32 PREFIX_SELECTIVE_LOTTERY = 13;
	const int32 STAGE_AUTO_CLEAR_TICKET = 14;

	const int32 MAX = 15;
};

namespace EStoreCostType	// = B2ShopBuyPriceType
{
	const int32 GEM = 0;
	const int32 SOCIAL_POINT = 1;
	const int32 DARK_CRYSTAL = 2;
	const int32 CASH = 3;
	const int32 GOLD = 4;
	const int32 FAME_POINT = 5;

	const int32 MAX = 6;
}

namespace EStoreRestrictType
{
	const int32 NONE = 0;
	const int32 DAILY = 1;
	const int32 WEEKLY = 2;
	const int32 MONTHLY = 3;
	const int32 NUMBER = 4;

	const int32 MAX = 5;
}

namespace EStoreMark		// = B2ShopMarkType
{
	const int32 NONE = 0;
	const int32 NEW = 1;
	const int32 HOT = 2;
	const int32 HIT = 3;
	const int32 LIMIT = 4;
	const int32 SALE = 5;	// Use only on client

	const int32 MAX = 6;
}

namespace ESetItemOptionCount
{
	const int32 FIRST_COUNT = 2;
	const int32 ACCESSORY_COUNT = 3;
	const int32 LAST_COUNT = 4;
}

namespace EEventBonusType
{
	const int32 NORMAL = 0;
	const int32 DOUBLE = 1;
}

struct FProductData
{
	int32 ProductType;
	int32 ProductId;
	int32 ProductCount;
};

struct FPackageData
{
	int32 PackageCategory;
	int32 PackageTabType;
	int32 PackagePeriod;
	int32 PackageTabOrder;
	int32 PackagePeriodBonusID;
	int32 PackagePeriodBonusCount;
};

struct FPackageProductData
{
	int32 PackageIndex;
	int32 PackageProductId;
	int32 PackageProductType;
	int32 PackageProductCount;
};

struct FStoreProductData
{
	int32 StoreProductId;

	int32 StoreCategory;
	int32 StoreCategoryOrder;
	int32 GoodsCategory;
	int32 GoodsCategoryOrder;

	FProductData Product;
	FProductData Bonus;

	int32 CalculatedBonusCount;
	int32 CalculatedPrice;

	int32 CostType;
	int32 Cost;

	int32 RestrictType;
	int32 RestrictCount;

	int32 PayPeriod;
};

struct FStoreCashBonusData
{
	int32 StoreProductId;
	int32 GoodsCategory;
	int32 IncreaseRate;
	int64 StartTime;
	int64 EndTime;
};

struct FStoreFundsSale
{
	int32 StoreProductId;
	int32 StoreCategory;
	int32 GoodsCategory;
	int32 DiscountRate;
	int64 StartTime;
	int64 EndTime;
};

struct FStoreMileageData
{
	TArray <FRewardItem> RewardIds;
	int32 MaxMileage;

private:
	int32 HashKey;

public:
	FStoreMileageData(int32 InHashKey) : HashKey(InHashKey) {}
	static int32 GetHashkey(uint32 MileageType, uint32 ClassType)
	{
		return (MileageType << 2) | (ClassType);
	}
};

struct FAttendanceDayData
{
	FAttendanceDayData()
	{
		nAttendanceType = nDay = nRewardId = nRewardCount = 0;
	}

	int32 nAttendanceType;
	int32 nDay;
	int32 nRewardId;
	int32 nRewardCount;
};

struct FEventAttendanceState
{
	int32 event_id;
	int32 attendance_day;
	int64 last_attendance_time;
	int64 next_attendance_time;

	FEventAttendanceState()
	{
		event_id = attendance_day = last_attendance_time = next_attendance_time = -1;
	}
};

struct FEventNewUserState
{
	bool IsNewUserAttendance;
	int32 AttendanceDay;
	int32 TotalAttendanceDays;
	int64 LastAttendanceTime;
	int64 NextAttendanceTime;
	int64 FinishTime;

	FEventNewUserState()
	{
		IsNewUserAttendance = false;
		AttendanceDay = 0;
		LastAttendanceTime = 0;
		NextAttendanceTime = 0;
		TotalAttendanceDays = 0;
		FinishTime = 0;
	}
};

struct FPlayTimeState
{
	int64 daily_play_time_in_sec;
	int32 daily_play_time_reward_index;
	int64 daily_play_time_update_time;
};

struct FEventAttendanceReward
{
	int32 event_id;
	int32 day;
	int32 reward_id;
	int32 reward_count;
};

struct FPlayTimeRewrad
{
	int32 reward_index;
	int32 play_time_inmin;
	int32 reward_id;
	int32 reward_count;
};

struct FMonsterInfo
{
	int32 Id;
	int32 NameIndex;
	int32 NpcClass;
	int32 Grade;
	int32 MonsterType;
	FString Desc;

	float HPScale;
	float AttackDefenseLevelScale;
	float MaxArmorOverride;

	FMonsterInfo()
	{
		Id = -1;
		NameIndex = -1;
		NpcClass = -1;
		Grade = -1;
		MonsterType = -1;
		Desc = TEXT("");

		HPScale = 1.f;
		AttackDefenseLevelScale = 1.f;
		MaxArmorOverride = 0.f;
	}
};

struct FB2LotteryTicketInfo
{
	int32			LotteryGrade;
	EPCClass		CharClass;
	int32			EquipCategory;
	int32			GroupID;
	int32			SurpassLevel;
	int32			Quality;

	FB2LotteryTicketInfo(const b2network::B2mdGeneralLotteryPtr& SvrInfoPtr)
		:FB2LotteryTicketInfo(*SvrInfoPtr.get())
	{}

	FB2LotteryTicketInfo(const b2network::B2mdGeneralLottery& SvrInfo)
	{
		LotteryGrade = SvrInfo.grade;
		CharClass = SvrToCliPCClassType(SvrInfo.character_type);
		EquipCategory = SvrInfo.equip_category;
		GroupID = -1;
		SurpassLevel = -1;
		Quality = -1;
	}

	FB2LotteryTicketInfo(const b2network::B2mdSelectiveLotteryPtr& SvrInfoPtr)
		:FB2LotteryTicketInfo(*SvrInfoPtr.get())
	{}

	FB2LotteryTicketInfo(const b2network::B2mdSelectiveLottery& SvrInfo)
	{
		LotteryGrade = SvrInfo.item_grade;
		CharClass = SvrToCliPCClassType(SvrInfo.character_type);
		EquipCategory = SvrInfo.equip_category;
		GroupID = -1;
		SurpassLevel = -1;
		Quality = -1;
	}

	FB2LotteryTicketInfo(const b2network::B2mdPrefixSelectiveLotteryPtr& SvrInfoPtr)
		:FB2LotteryTicketInfo(*SvrInfoPtr.get())
	{}

	FB2LotteryTicketInfo(const b2network::B2mdPrefixSelectiveLottery& SvrInfo)
	{
		LotteryGrade = SvrInfo.item_grade;
		CharClass = SvrToCliPCClassType(SvrInfo.character_type);
		EquipCategory = SvrInfo.equip_category;
		GroupID = SvrInfo.group_id;
		SurpassLevel = SvrInfo.surpass_level;
		Quality = SvrInfo.quality;
	}

	FB2LotteryTicketInfo(const b2network::B2mdFixedGradeLotteryPtr& SvrInfoPtr)
		:FB2LotteryTicketInfo(*SvrInfoPtr.get())
	{}
	FB2LotteryTicketInfo(const b2network::B2mdFixedGradeLottery& SvrInfo)
	{
		LotteryGrade = SvrInfo.item_grade;
		CharClass = SvrToCliPCClassType(SvrInfo.character_type);
		EquipCategory = SvrInfo.equip_category;
		GroupID = -1;
		SurpassLevel = -1;
		Quality = -1;
	}
};


struct FB2EnhanceItemCost
{
	int32 EnhanceLevel;
	int32 Rate;
	int32 ScrollCost;
	int32 ProtectionScrollCost;
	int32 MoneyCost;
	int32 CostDarkCrystalForExchange;
	int32 CostGemCrystalForExchange;

	FB2EnhanceItemCost()
	{
		EnhanceLevel = -1;
		Rate = 0;
		ScrollCost = 0;
		ProtectionScrollCost = 0;
		MoneyCost = 0;
		CostDarkCrystalForExchange = 0;
		CostGemCrystalForExchange = 0;
	}

	void UpdateByServerSync(const b2network::B2mdItemEnhanceCostPtr& ServerLevelupCostStruct);
};

struct FItemEnhanceEffectInfo
{
	double	SetEffect;
	FLinearColor AuraColor;

	void UpdateByServerSync(const b2network::B2mdItemEnhanceEffectInfoPtr& ServerLevelupCostStruct);
};

struct FStageFixedDropsInfo
{
	//int32 stage_idx; //stage_idx는 TMap의 키로 사용중
	int32 Iteminfo_idx;
	int32 Min_Gain;
	int32 Max_Gain;
	int32 Rate;

	int32 idx; //사용 안하고 있음

	FStageFixedDropsInfo() : Iteminfo_idx(0), Min_Gain(0), Max_Gain(0), Rate(0), idx(0) {}

	void UpdateByServerSync(const b2network::B2mdItemEnhanceEffectInfoPtr& ServerLevelupCostStruct);
};

/** Master data on experience point by some match game type and result. */
struct FDuelModeExpMDSingleData
{
	FDuelModeExpMDSingleData() : KeyGameMode(EB2GameMode::Unknown), KeyMatchResult(ENetMatchResult::Draw), ExpectedExp(0) {}

	EB2GameMode KeyGameMode; // GameMode type that matches b2network::B2DuelMode. One key element of this data element.
	ENetMatchResult KeyMatchResult; // Another key element.

	int32 ExpectedExp; // wanted data

	void UpdateByServerSync(const b2network::B2mdDuelModeRewardPtr& InDataElem);

	// Generate combined map key of designated data set
	static int32 GetCombinedIntKey(EB2GameMode InGameMode, ENetMatchResult InResult);
};
/** Master data on raid game, per difficulty */
struct FRaidExpMDSingleData
{
	FRaidExpMDSingleData() : RaidType(0), RaidStep(0), ExpectedExp(0), RaidExpKey(0) {}

	int32 RaidType;
	int32 RaidStep;
	int32 ExpectedExp;

	int32 RaidExpKey;

	void UpdateByServerSync(const b2network::B2mdRaidExpPtr& InDataElem);

	FORCEINLINE static int32 GetKey(int32 type, int32 step) { return ((type * 10000) + step); }
	FORCEINLINE int32 GetMyKey() { return GetKey(RaidType, RaidStep); }
};

struct FGuildSkillHashKey
{
	static int32 GetHashKey(int32 Skillid, int32 SkillLevel)
	{
		return SkillLevel << 3 | Skillid;
	}
};

namespace EEventID
{
	const int32 Point = 1;
	const int32 Levelup = 2;
	const int32 HotTime = 3;
	const int32 Season = 4;
	const int32 StageClear = 5;
	const int32 PointConsume = 6;
	const int32 GemPurchase = 7;
	const int32 TenLottery = 8;
	const int32 GemSpend = 9;
}

struct FEventInfo
{
	int32	event_id;
	int32	event_category;
	FString event_tab_name;
	FString title;
	int64	start_time;
	int64	end_time;
	FString explain;
	FString link_text;
	FString link_url;
	int32	tab_order;
	FString banner_url;

	FEventInfo& operator=(const b2network::B2mdEventInfo& InEventInfo);
};

struct FSeasonEventSlotInfo
{
	b2network::B2mdSeasonMissionEvent	RewardInfo;
	int32								progress_count;
	int32								state;
	FSeasonEventSlotInfo()
	{
		progress_count = 0;
		state = 1;
	}
};

struct FMagicShopProduct
{
	int32 MagicShopID;
	int32 MagicShopGroupType;
	int32 ProductType;
	int32 ProductDetail;
	int32 ProductCount;
	int32 PriceType;
	int32 Price;
};

struct FMagicShopRenewalCost
{
	int32 RenewalCount;
	int32 GemCost;
};

struct FSelectItemOption
{
	int32 OptionId;
	int32 Grade;
	int32 EquipPosition;
	float Prob;
	int32 DecimalPoint;
	float MinValue;
	float MaxValue;

	FORCEINLINE static int32 GetKey(int32 position, int32 stargrade) { return ((position * 10000) + stargrade); }
	FORCEINLINE int32 GetMyKey() { return GetKey(EquipPosition, Grade); }
};

struct FItemDismantleInfo
{
	EItemInvenType ItemInvenType;
	int32 Grade;
	TArray<int32> Matrial;
	TArray<int32> MatrialAmount;
};

struct FCostumeEnhanceCostInfo
{
	int32 Grade;
	int32 EnhanceLevel;
	int32 MoneyCost;
	int32 WeaponMaterialId;
	int32 WeaponAmount;
	int32 WeaponRestoreAmount;
	int32 ArmorMaterialId;
	int32 ArmorAmount;
	int32 ArmorRestoreAmount;

	FORCEINLINE static int32 GetKey(int32 gd, int32 lv) { return ((gd * 10000) + lv); }
	FORCEINLINE int32 GetMyKey() { return GetKey(Grade, EnhanceLevel); }
};

struct FConvertTableInfo
{
	int32 tableId;
	int32 beforeConvertItemID;
	int32 beforeConvertItemQuantity;
	int32 afterConvertItemID;
	int32 afterConvertItemQuantity;
};
struct FConvertCostInfo
{
	int32 tableId;
	int32 count;
	int32 goldPrice;
	int32 crystalPrice;
};

struct FLotteryShopRateQualityInfo
{
	EEquipCategoryType equipCategory;
	int32 quality;
	int32 rate;
	float qualityIncLevel;
};

struct FQualityTransRateInfo
{
	int32 templateID;
	int32 equipCategory;
	int32 qualityValue;
	int32 transRate;
	int32 transCondQuality;
};

//아이템 제작소 아이템 정보
struct FItemForgeInfo
{
	int32 forgeID;
	EItemForgeType forgeType;
	int32 forgeTab;
	int32 groupID;
	int32 groupSort;
	int32 itemSort;
	int32 limitID;
	EPCClass PCClassType;

	int32 goldPrice;
	int32 gemPrice;
	int32 darkCrystalPrice;

	int32 successRate;
	int32 failRate;
	int32 hugeSuccessRate;

	int32 hugeSuccessType;
	int32 hugeSuccessAdditionalItemID;
	int32 hugeSuccessAdditionalItemQuantity;

	int32 resultItemID;
	int32 resultItemQuantity;

	int32 mainMaterialItemID;
	int32 mainMaterialItemEnhance;
	int32 mainMaterialItemLevel;
	int32 mainMaterialItemQuantity;
	int32 mainMaterialItemRefundRate;

	int32 material1stItemID;
	int32 material1stItemQuantity;
	int32 material1stItemRefundRate;
	int32 material2ndItemID;
	int32 material2ndItemQuantity;
	int32 material2ndItemRefundRate;
	int32 material3rdItemID;
	int32 material3rdItemQuantity;
	int32 material3rdItemRefundRate;
	int32 material4thItemID;
	int32 material4thItemQuantity;
	int32 material4thItemRefundRate;

	EItemForgeListVisibleType visibleType;

	bool previewOnly=false;
};

struct FItemForgeMaterialTransInfo
{
	int32 forgeID;
	int32 mainMaterialItemID;
	int32 mainMaterialItemQuantity;
	int32 material1stItemID;
	int32 material1stItemQuantity;
	int32 material2ndItemID;
	int32 material2ndItemQuantity;
	int32 material3rdItemID;
	int32 material3rdItemQuantity;
	int32 material4thItemID;
	int32 material4thItemQuantity;
};

struct FItemForgeLimitInfo
{
	int32 limitID;
	EItemForgeLimitType limitType;
	int32 limitQuantity;
	EItemForgePeriodLimitType limitTimeType;
};

struct FItemForgeLimitStatus
{
	int32 limitID;
	int32 limitType;
	int32 limitTimeType;
	int32 leftCount;
};

struct FItemForgePeriodicStatus
{
	int32 forgeID;
	FDateTime previewTime;
	FDateTime startTime;
	FDateTime endTime;
};

struct FItemForgeResult
{
	EItemForgeResultType resultType;
	FB2Item forgedItem;
	FB2Item bonusItem;
	TArray<FB2Item> refundedItem;
};

struct FSealBoxInfo
{
	int32 sealboxRefID=-1;
	int32 rewardSlot1;
	int32 rewardSlot2;
	int32 rewardSlot3;
	int32 rewardSlot4;
	int32 rewardSlot5;
	bool isRateVisible;
};

struct FSealBoxRewardRateInfo
{
	int32 sealboxRewardID;
	int32 itemType;
	int32 itemRefID;
	int32 minQuantity;
	int32 maxQuantity;
	int32 starGrade;
	int32 groupID;
	int32 equipCategory;
	int32 equipPlace;
	int32 quality;
	int32 enhanceLevel;
	int32 surpassCount;
	int32 rewardRate;
};

//////////////////////////////////////////////////////////////////////

/** The main data container here. It is not responsible for its own data handling. Just a simple big fat container of various client data to be sync by server. */
class FClientDataStore
{
public:
	FClientDataStore();

	//Use only reference variable.
	FClientDataStore(FClientDataStore const&) = delete;
	FClientDataStore& operator=(FClientDataStore const&) = delete;

public:
	void InitializeAccountInfo();
	void SubscribeEvents();
	void UnsubscribeEvents();

	void CheckBeginCombatTutorialorEntryMainLobby();
	void SetAccountInfo(const FB2WholeInfo&);
	void SetAccountItems(const FB2AccountItems&);
	void SetMasterDatas(const FB2MasterDatas&);
	void SetGuildAccountID(int64 GuildID);
	void SetMyGuildInfo(FGuildInfo InGuildInfo);
	void SetMyGuildInfo(FB2ResponseGuildInfoPtr InChangedGuildMember);
	void UpdateMyGuildGuildMemberInfo(TArray<b2network::B2GuildMemberPtr> GuildInfo);
	FTimespan GetGuildMemberLastLogoutTime(int64 InMemberID);
	FTimespan GetMasterElectedTime();
	void SetMyGuildMemberOut(int64 AccountID);
	void SetMyGuildSubMaster(bool Type, int64 AccountID);
	void SetOtherGuildInfo(FGuildInfo GuildInfo);
	void SetMyGuildBattleInfo(b2network::B2GuildBattleGuildPtr Guild) {
		MyBattleGuildData = Guild;
	}
	void SetOtherGuildBattleInfo(b2network::B2GuildBattleGuildPtr Guild) {
		OtherBattleGuildData = Guild;
	}
	int64 GetAccountId();

	void OnShutdownModule();
	/** Save/Load some local data which does not have to do with server.. */
	void SaveLocalData();
	void LoadLocalData();
	void LoadLocalData_ResourceBound();
	void AddItems(const FB2AddedItems&);
	void AddCostumeItem(const b2network::B2CostumeServerInfo& CostumeInfo);
	void ChangeCostumeItem(const b2network::B2CostumeServerInfoPtr& CostumeInfo);
	void AddNewAcquiredItems(const TArray<FB2Item>& NewItems);
	void AddNewAcquiredItem(const FB2Item& NewItems);
	void AddChangeItem(const FB2Item& NewItems);
	bool RemoveItem(int64);
	void ResponseExtendInventory(const FB2ExtendInventory& ExtendInventoryInfo);
	void SellItem(const FB2SoldItems&);
	void SellConsumableItem(const FB2SellConsumableItem&);
	void ConvertData(const FB2ConvertData&);
	void ConvertItem(const FB2ConvertItem&);
	void UseNickNameChangeTicket(const FB2UpdateNicknamePtr& updated);

	void DecompositionItem(const FB2DismantleItems&);
	void DecompositionCostumeItem(const FB2ResponseDismantleCostumesPtr&);

	void EquipItems(const FB2EquippedItems&);
	void EquipItems(const int32& PresetID, const TArray<int64>& EquipedItems, const TArray<int64>& UnequipedItems);
	void UnEquipItems(const TArray<int64>& ArrayUnEquipItemID, const int32 iPresetID = -1);
	void LockItems(const FB2LockedItems&);
	void UnLockItems(const FB2UnlockedItems&);
	void LevelupItem(const FB2LevelupItem&);
	void SurpassItem(const FB2SurpassItem&);
	void UpgradeItem(const FB2UpgradeItem&);
	void EnhanceChangeItem(const FB2Item&, const FB2Item&);
	int64 ComposeItem(const FB2ComposeItem&); // Returns new composed item's UID
	int32 EnhanceItem(const FB2EnhanceItem&);
	void ItemOptionChange(const FB2Item& OptionAppliedItem);
	void ItemQualityChange(const FB2Item& OptionAppliedItem);

	// Totem
	void ResetTotem();
	void AddTotem(const b2network::B2TotemServerInfoPtr TotemInfo);
	void EquipTotems(const TArray<FB2Totem> EquippedTotems);
	void EquipTotems(const FB2EquipTotemsPtr EquipTotemsPtr, EPCClass& TargetPCClass, ETotemEquipSlot& TargetEquipSlot);
	void UnequipTotems(const FB2UnequipTotemsPtr UnequipTotemsPtr, EPCClass& TargetPCClass);
	void LockTotems(const FB2LockTotemsPtr LockTotemsPtr);
	void UnlockTotems(const FB2UnlockTotemsPtr UnlockTotemsPtr);
	void SellTotem(const FB2SellTotemsPtr SellTotemsPtr);
	void DecompositionTotem(const FB2DismantleTotemsPtr DismantleTotemsPtr);
	void SmeltingTotem(const FB2RefineTotemPtr DismantleTotemsPtr, const int32& ServerRefineCostType);

	void RequestCollectionItem(EPCClass TargetClass, ECollectionType CollectionType);
	void SetCollectionItemData(FB2CollectionItemPtr);
	void SyncingInventoryData(EPCClass PCClass); // It will request shared consumables if PCClass is EPC_End
	void BladePointCharge(const int32 InPoint, const int32 InPointMax, const int32 InLefttime);

	void StartStage(const FB2StartStageInfo&);
	void ClearStage(const FB2ClearStageInfo&, FServerStageID);
	void ClearStage(const FB2SweepStageInfo& SweepStageInfo);

	FB2OriginCharacterInfoPtr CheckEndGameIncreaseExp(const EPCClass PCClass, const int32 LatestLevel);
	void EndGameIncreaseExp(const FB2LightCharacterPtr info);
	void EndGameIncreaseExp(const FB2OriginCharacterInfoPtr CharInfo);
	void CheckNeedRegistSkill(EPCClass InPCClass);
	// Get factors to calculate post-enhance item level in client side.
	float GetItemLevelupFactor_TargetItemGrade(int32 InGrade);
	float GetItemLevelupFactor_IngredItemGrade(int32 InGrade);
	float GetItemLevelupFactor_IngredItemEquipPlace(EItemEquipPlace InEquipPlace);
	float GetItemLevelupFactor_IngredItemEnhLv(int32 InEnhanceLevel);
	float GetItemLevelupFactor_IngredItemSurpCount(int32 InSurpassCount);
	float GetItemLevelupFactor_PerOptionInc(EItemOption InOption);
	float GetOptionWeight(EItemOption InOption);
	int32 GetItemLevelupCostSingleFactor(int32 InEnhanceLevel, EItemInvenType InInvenType, int32 InStarGrade);

	float GetItemLevelupFactor_PrimaryIncrement(int32 ItemLevel);
	// Data that you need to refer to when you plan to buy inventory slot.
	bool GetInventoryExtendReferenceData(int32& OutCashPerSingleExtend, int32& OutSlotCountPerSingleExtend, int32& OutMaxAllowedSlotCount);

	int32 GetEnhanceItem_NextStepRate(int32 InEnhanceLevel);
	int32 GetEnhanceItem_NextStepScrollCost(int32 InEnhanceLevel);
	int32 GetEnhanceItem_NextStepProtectionScrollCost(int32 InEnhanceLevel);
	int32 GetEnhanceItem_NextStepMoneyCost(int32 InEnhanceLevel);

	bool HasEnhanceEffectAuraColor(int32 InEnhanceLevel);
	FLinearColor GetEnhanceEffectAuraColor(int32 InEnhanceLevel);

	int32 GetMinEnhanceLevelForSetEffect();
	int32 GetMaxEnhanceLevelForSetEffect();
	double GetEnhanceItemSetEffect(int32 InEnhanceLevel);

	// Getting resurrect reference data. They are for "StageGameMode" now..
	int32 GetResurrectionCost(EResurrectGameModeType ModeType, int32 ResurrectCount);
	void GetAppliedResurrectBuff(EResurrectGameModeType ModeType, const TMap<EResurrectBuffType, int32>& InSelectedBuffCounts, TMap<EResurrectBuffType, int32>& OutAppliedBuffValues, bool bGetNextExpectedValues = false);

	FString GetBrevetRankNameKey(int32 BrevetRank);
	UMaterialInterface* GetBrevetRankIconMaterial(int32 BrevetRank, bool isBigSize = false);
	int32 GetBrevetGoodsID(int32 BrevetRank);

	FText GetRewardName(int32 RewardId) const;
	FString GetRewardNameKey(int32 RewardIndex);
	UMaterialInterface* GetRewardIconMaterial(int32 RewardIndex);

public:
	FORCEINLINE bool GetAttendanceOpenDailyOnce() {
		return bAttendanceOpenDailyOnce;
	}
	FORCEINLINE void SetAttendanceOpenDailyOnce(bool bOpen) {
		bAttendanceOpenDailyOnce = bOpen;
	}
	FORCEINLINE FTutorialCharacterData& GetTutorialCharacterData() {
		return TutorialCharacterData;
	}
	FORCEINLINE FLocalCharacterData& GetLocalCharacterData() { return LocalCharacterData; }
	FORCEINLINE FB2RedDotManager& GetRedDotManager() {
		return RedDotManager;
	}

	FORCEINLINE FB2OriginAccountInfoPtr GetAccountInfo() {
		return AccountInfo;
	}
	void SetAccountInfo(const FB2OriginAccountInfoPtr Value) {
		AccountInfo = Value;
	}
	FORCEINLINE int32 GetGoldAmount() {
		return AccountInfo ? AccountInfo->money : 0;
	}
	FORCEINLINE int32 GetGemAmount() {
		return AccountInfo ? AccountInfo->cash : 0;
	}
	FORCEINLINE int32 GetBladeCurPoint() {
		return AccountInfo ? AccountInfo->blade_point : 0;
	};
	FORCEINLINE int32 GetBladeMaxPoint() {
		return AccountInfo ? AccountInfo->blade_point_max : 0;
	};
	FORCEINLINE int32 GetRestPoint() {
		return AccountInfo ? ((AccountInfo->rest_time / 432) > 100 ? 100 : (AccountInfo->rest_time / 432)) : 0;
	};
	FORCEINLINE int32 GetFamePoint() {
		return AccountInfo ? AccountInfo->fame_point : 0;
	};
	FORCEINLINE int32 GetShopTenLotteryTicket() {
		return AccountInfo ? AccountInfo->shop_ten_lottery_ticket : 0;
	};
	FORCEINLINE int32 GetGoldBuffTicket() {
		return AccountInfo ? AccountInfo->gold_buff_ticket : 0;
	};
	FORCEINLINE int32 GetExpBuffTicket() {
		return AccountInfo ? AccountInfo->exp_buff_ticket : 0;
	};
	FORCEINLINE int32 GetStageBoostTicket() {
		return AccountInfo ? AccountInfo->stage_boost_ticket : 0;
	};
	FORCEINLINE int32 GetStageAutoClearTicket() {
		return AccountInfo ? AccountInfo->stage_auto_clear_ticket : 0;
	};

	FORCEINLINE void SetGoldAmount(int32 NewAmount) {
		if (AccountInfo) AccountInfo->money = NewAmount;
	}
	FORCEINLINE void SetGemAmount(int32 NewAmount) {
		if (AccountInfo) AccountInfo->cash = NewAmount;
	}
	FORCEINLINE void SetBladeCurPoint(int32 NewAmount) {
		if (AccountInfo) AccountInfo->blade_point = NewAmount;
	};
	FORCEINLINE void SetBladeMaxPoint(int32 NewAmount) {
		if (AccountInfo) AccountInfo->blade_point_max = NewAmount;
	};
	FORCEINLINE void SetRestPoint(int32 NewAmount) {
		if (AccountInfo) AccountInfo->rest_time = NewAmount;
	};
	FORCEINLINE void SetFamePoint(int32 NewAmount) {
		if (AccountInfo) AccountInfo->fame_point = NewAmount;
	};

	FORCEINLINE void SetShopTenLotteryTicket(int32 NewAmount) {
		if (AccountInfo) AccountInfo->shop_ten_lottery_ticket = NewAmount;
	};
	FORCEINLINE void SetGoldBuffTicket(int32 NewAmount) {
		if (AccountInfo) AccountInfo->gold_buff_ticket = NewAmount;
	};
	FORCEINLINE void SetExpBuffTicket(int32 NewAmount) {
		if (AccountInfo) AccountInfo->exp_buff_ticket = NewAmount;
	};
	FORCEINLINE void SetStageBoostTicket(int32 NewAmount) {
		if (AccountInfo) AccountInfo->stage_boost_ticket = NewAmount;
	};
	FORCEINLINE void SetStageAutoClearTicket(int32 NewAmount) {
		if (AccountInfo) AccountInfo->stage_auto_clear_ticket = NewAmount;
	};

	FORCEINLINE void SetGuildSkillInfo(b2network::B2GuildSkillPtr InSkill) {
		UserGuildInfo.SetGuildSkill(InSkill);
	};

	void SetAccountKickedGuildID(const FB2OriginAccountInfoPtr Value);
	void SetUserGuildInfo(FUserGuildInfo GuildInfo);

	FORCEINLINE bool ExistAccountInfo() {
		return AccountInfo != nullptr;
	}
	void AddGold(int32 ToAdd);
	FORCEINLINE void AddGem(int32 ToAdd) {
		if (AccountInfo) AccountInfo->cash += ToAdd;
	}
	FORCEINLINE void AddBladePoint(int32 ToAdd) {
		if (AccountInfo) AccountInfo->blade_point += ToAdd;
	};
	FORCEINLINE void AddDarkCrystal(int32 ToAdd) {
		if (AccountInfo) AccountInfo->dark_crystal += ToAdd;
	}
	FORCEINLINE void AddSocialPoint(int32 ToAdd) {
		if (AccountInfo) AccountInfo->social_point += ToAdd;
	}
	FORCEINLINE void AddFamePoint(int32 ToAdd) {
		if (AccountInfo) AccountInfo->fame_point += ToAdd;
	};

	FORCEINLINE void AddShopTenLotteryTicket(int32 ToAdd) {
		if (AccountInfo) AccountInfo->shop_ten_lottery_ticket += ToAdd;
	};
	FORCEINLINE void AddGoldBuffTicket(int32 ToAdd) {
		if (AccountInfo) AccountInfo->gold_buff_ticket += ToAdd;
	};
	FORCEINLINE void AddExpBuffTicket(int32 ToAdd) {
		if (AccountInfo) AccountInfo->exp_buff_ticket += ToAdd;
	};
	FORCEINLINE void AddStageBoostTicket(int32 ToAdd) {
		if (AccountInfo) AccountInfo->stage_boost_ticket += ToAdd;
	};
	FORCEINLINE void AddStageAutoClearTicket(int32 ToAdd) {
		if (AccountInfo) AccountInfo->stage_auto_clear_ticket += ToAdd;
	};

	FORCEINLINE const TArray<FModDailyReward>& GetModDailyRewardList() {
		return ModDailyData;
	}
	FORCEINLINE const TArray<FModWeeklyReward>& GetPvPWeeklyRewardDataList() {
		return PvPWeeklyRewardData;
	}
	FORCEINLINE const TArray<FModWeeklyReward>& GetTeamMatchWeeklyRewardDataList() {
		return TeamMatchWeeklyRewardData;
	}
	FORCEINLINE const TArray<FModWeeklyReward>& GetControlMatchWeeklyRewardDataList() {
		return ControlWeeklyRewardData;
	}

	FORCEINLINE const TArray<FCounterDungeon>& GetCounterDungeonData() {
		return CounterDungeonData;
	}
	int32									   GetCounterDungeonTypeNumber(int32 InDungeonType);
	FORCEINLINE const TArray<FCounterDungeunWeekReward>& GetCounterDeungeonRewardData() {
		return CounterDungeonRewardData;
	}

	FORCEINLINE FString GetUserNickName() {
		return AccountInfo ? AccountInfo->name : TEXT("");
	}
	FORCEINLINE void SetUserNickName(FString fstr) {
		if (AccountInfo) AccountInfo->name = fstr;
	}
	FORCEINLINE bool HasValidUserNickName() const {
		return (AccountInfo && !AccountInfo->name.IsEmpty());
	}

	const TArray<FRaidMasterData>* GetRaidMasterDataList(int32 RaidId);

	FText GetLocalCharClassName(EPCClass InPCClass);
	FText GetLocalCharClassOrNickName(EPCClass InPCClass); // Per-character class or nick name, different from UserNickName.

	FORCEINLINE const FGuildInfo& GetMyGuildInfo() { return MyGuildData; }
	FORCEINLINE const FGuildInfo& GetOtherGuildInfo() { return OtherGuildData; }
	FORCEINLINE const FUserGuildInfo& GetUserGuildInfo() { return UserGuildInfo; }
	FORCEINLINE const TArray<b2network::B2mdGuildBattleSeasonRewardPtr>& GetGuildSeasonRewardInfo() { return GuildSeasonRewardInfo; }
	FORCEINLINE const b2network::B2mdGuildSkillPtr GetGuildSkillInfo(int32 id, int32 Lv) { return GuildSkillInfoMasterData.Contains(FGuildSkillHashKey::GetHashKey(id, Lv)) ? GuildSkillInfoMasterData[FGuildSkillHashKey::GetHashKey(id, Lv)] : nullptr; }
	FORCEINLINE const b2network::B2GuildSkillPtr GetGuildCurrentSkillInfo(int32 id) { return UserGuildInfo.GetGuildSkill(id); }
	FORCEINLINE const TMap<int32, b2network::B2GuildSkillPtr>& GetGuildCurrentSkillInfoArr() { return UserGuildInfo.CurrentGuildSkill; }
	FORCEINLINE const TMap<int32, b2network::B2mdGuildSkillPtr>&	GetGuildSkillMasterDataArr() { return GuildSkillInfoMasterData; }
	const TMap<int32, b2network::B2mdGuildBattleResultRewardPtr>&  GetGuildBattleResultRewardArr();
	const b2network::B2mdGuildBattleResultRewardPtr  GetGuildBattleResultReward(int32 medalCount);
	const b2network::B2mdGuildBattleResultRewardPtr  GetGuildBattleResultRewardGrade(int32 Grade);
	int32  GetGuildBattleResultRewardMaxGrade();
	FORCEINLINE const TArray<b2network::B2mdGuildBattleScheduleInfoPtr>& GetGuildBattleScheduleInfo() { return GuildBattleSchedule; }
	FORCEINLINE const TArray<b2network::B2mdRestRewardPtr>& GetRestRewardData() { return MD_RestRewardData; }
	FORCEINLINE const TArray<b2network::B2RewardPtr>& GetDuelModeRewardData(int32 ModeNum) { return MD_DuelModeRewardInfo[ModeNum]; }

	FORCEINLINE double GetBladePointChargingLeftTime() { return AccountInfo ? AccountInfo->blade_point_charge_lefttime_second : 0; };

	// RankPromotionPoint is now considered as one of consumables in inventory store. This is old legacy of considering it like gold or gem.
	void AddRankPromotionPoint(int32 ToAdd);
	int32 GetRankPromotionPoint();
	void AddAdvancedRankPromotionPoint(int32 ToAdd);
	int32 GetAdvancedRankPromotionPoint();

	const TArray<FCounterDungeonPeriod>* GetCounterDungeonPeriodData(int32 type);
	FORCEINLINE int32 GetSocialPoint() { return AccountInfo ? AccountInfo->social_point : 0; };
	FORCEINLINE void SetSocialPoint(int32 NewSocialPoint) { if (AccountInfo) AccountInfo->social_point = NewSocialPoint; };

	FORCEINLINE int64 GetUserGuildID() { return AccountInfo ? AccountInfo->guild_id : 0; };

	FORCEINLINE int32 GetDarkCrystal() { return AccountInfo ? AccountInfo->dark_crystal : 0; };
	FORCEINLINE void SetDarkCrystal(int32 NewDarkCrystal) { if (AccountInfo) AccountInfo->dark_crystal = NewDarkCrystal; };

	FORCEINLINE int32 GetAdditionalMatchPointCost(int32 nCount) { if (nCount < 0 || MD_AdditonalMatchCostData.Num() < 10) return 0; return nCount > 9 ? MD_AdditonalMatchCostData[9] : MD_AdditonalMatchCostData[nCount]; }
	FORCEINLINE void SetUsedAdditionalMatchPoint(bool bIsUsed) { bUsedAdditonalMatchCount = bIsUsed; };
	FORCEINLINE bool GetUsedAdditionalMatchPoint() { return bUsedAdditonalMatchCount; };

	const TArray<FB2ShopPackageTabPtr>& GetShopPackageTabOrder() { return MD_Store_PackageOrderData; };

	void ApplyRewardInfoList(const TArray<b2network::B2RewardPtr>& Rewards);
	void ApplyRewardInfo(b2network::B2RewardPtr RewardPtr);
	void ApplyRewardInfo(const FCommonRewardInfo& RewardInfo);
	void AddRewardItemByType(ERewardType RewardType, int32 RewardAmount);

	void AddUserDataWithDoc(EDocUserDataType type, int32 amount = 0);
	void ReplaceUserDataWithDoc(EDocUserDataType type, int32 amount = 0);

	void UseSocialPoint();

	int32 GetSkillLearnLevel(int32 SkillId) const;
	int32 GetMaxSkillLevel(int32 SkillId) const;
	EPCClass GetSkillClass(int32 SkillId) const;
	int32 GetSkillMinLevelAcquiredOption(int32 SkillId, int32 OptionId);
	float GetSkillPower(int32 SkillId, int32 SkillLv) const;
	int32 GetGemResetSkill();
	int32 GetGemsBuySkillPoint();
	int32 GetMaxSkillPointBuyable(EPCClass);
	int32 GetMaxSkillPoint();
	int32 GetMaxRaidCountPerDay();
	int32 GetLimitOpenStageDifficulty();

	int32 GetMaxCounterDungeonTicketCount();
	int32 GetMaxHeroTowerTicketCount();
	int32 GetCurrentHeroTowerTicketCount();

	int32 GetRaidMaxStep(int32 RaidType);
	void GetRaidMainReward(int32 RaidType, int32& RewardID);
	FText GetRaidName(int32 RaidId);

	int32 GetCharacterInvenSlotCount(EPCClass InPCClass, EItemInvenType InInvenType);

	FORCEINLINE int32 GetEtherInvenSlotCount() { return AccountInfo ? AccountInfo->inventory_aether_buy_count : 0; };

	const FSkillMasterData* GetSkillMasterData(int32 SkillId);
	FB2EnhanceItemCost GetEnhanceCostData(int32 InEnhanceLevel);

	FORCEINLINE void SetNextFairyGiftTime(int64 InNextFairyGiftTime) { NextFairyGiftTime = InNextFairyGiftTime; }
	bool GetPossibleFairyGift() const;

	void GetDiffSkillOption(int32 SkillId, int32 OldLevel, int32 NewLevel, TArray<FSkillOptionData>& DiffOptions);
	void GetRandomCharacterData(FB2ModPlayerInfo& ModPlayerInfoData);
	float GetAttackDamageBonusByGuildFormation(ETMFormation Formation, int32 FormationLV, int32 Position = 1) const;
	float GetReduceDamageBonusByGuildFormation(ETMFormation Formation, int32 FormationLV, int32 Position = 1) const;
	int32 GetUpgradeGuildFormationCost(ETMFormation Formation, int32 NextLv);
	float GetAttackDamageBonusByTeamFormation(ETMFormation Formation, int32 FormationLV, int32 Position = 1) const;
	float GetReduceDamageBonusByTeamFormation(ETMFormation Formation, int32 FormationLV, int32 Position = 1) const;
	float GetBuffTeamFormation(ETMFormation Formation, int32 FormationLV, int32 BuffType, int32 Position) const;
	float GetBuffGuildFormation(ETMFormation Formation, int32 FormationLV, int32 BuffType, int32 Position) const;
	bool IsExistFormationLevel(ETMFormation Formation, int32 FormationLV) const;
	int32 GetMaxLevelTeamFormation(ETMFormation Formation) const;
	int32 GetUpgradeFormationCost(ETMFormation Formation, int32 NextFormationLV) const;

	int32 GetBrevetNodeNeedHonerPoint(int32 BrevetRank, int32 NodeIndex) const;
	int32 GetBrevetNodeTotalAtk(int32 BrevetRank, int32 NodeIndex) const;
	int32 GetBrevetNodeTotalDef(int32 BrevetRank, int32 NodeIndex) const;
	int32 GetBrevetNodeTotalHp(int32 BrevetRank, int32 NodeIndex) const;
	int32 GetBrevetNodeTotalStatus(EItemOption OptionType, int32 BrevetRank, int32 NodeIndex) const;
	int32 GetBrevetNodeCount(int32 BrevetRank) const;
	int32 GetBrevetRankByOpenLevel(int32 OpenLevelMin, int32 OpenLevelMax) const;
	int32 GetMaxBrevetRank();
	const FBrevetNodeInfo* GetBrevetNodeInfo(int32 BrevetRank, int32 NodeIndex) const;
	const int32 GetBrevetMaxRank() const;

	FORCEINLINE int32 GetMaxCharacterLevel() const { return ConfigData ? ConfigData->character_level_max : 50; }
	FORCEINLINE int32 GetMaxFriendCount() { return ConfigData ? ConfigData->max_friend_count : 0; };
	FORCEINLINE int32 GetMaxSendAskFriend() { return ConfigData ? ConfigData->max_send_ask_friend : 0; };
	FORCEINLINE int32 GetMaxReceiveAskFriend() { return ConfigData ? ConfigData->max_receive_ask_friend : 0; };
	FORCEINLINE int32 GetCooltimeForSendSocialPoint() { return ConfigData ? ConfigData->cooltime_for_send_social_point : 0; };
	FORCEINLINE int32 GetMaxDeleteFriendCountForDay() { return ConfigData ? ConfigData->max_delete_friend_count_for_day : 0; };
	FORCEINLINE int32 GetMaxSocialPointValue() { return ConfigData ? ConfigData->max_social_point_value : 0; };
	FORCEINLINE int32 GetAddSocialPointForSendToFriend() { return ConfigData ? ConfigData->add_social_point_for_send_to_friend : 0; };
	FORCEINLINE int32 GetAddSocialPointForReceiveFromFriend() { return ConfigData ? ConfigData->add_social_point_for_receive_from_friend : 0; };
	FORCEINLINE int32 GetStageBuffIncreaseExpRate() { return ConfigData ? ConfigData->stage_buff_rate_for_increase_exp : 0; };
	FORCEINLINE int32 GetStageBuffIncreaseGoldRate() { return ConfigData ? ConfigData->stage_buff_rate_for_increase_gold : 0; };
	FORCEINLINE int32 GetStageBuffIncreaseGamseSpeedRate() { return ConfigData ? ConfigData->stage_buff_rate_for_boost_game_speed : 0; };
	FORCEINLINE int32 GetGuildSetupGoldCost() { return ConfigData ? ConfigData->guild_set_up_gold_cost : 0; };
	FORCEINLINE int32 GetGuildMaxInviteCount() { return ConfigData ? ConfigData->max_invite_guild_count_for_account : 0; };
	FORCEINLINE int32 GetMaxBoostCount() { return 0;	/*3배 모험 이용권 반영에 따른 임시 수정*/ };
	FORCEINLINE int32 GetMaxPvpMatchPoint() { return ConfigData ? ConfigData->pvp_match_point_max : 0; };
	FORCEINLINE int32 GetMaxTeamMatchPoint() { return ConfigData ? ConfigData->team_match_point_max : 0; };
	FORCEINLINE int32 GetItemOptionChangeDarkCrystalCost() { return ConfigData ? ConfigData->item_random_option_change_cost_dark_crystal : 0; };
	FORCEINLINE int32 GetItemOptionChangeCashCost() { return ConfigData ? ConfigData->item_random_option_change_cost_cash : 0; };
	FORCEINLINE int32 GetItemPresetMaxCount() { return ConfigData ? ConfigData->item_preset_max_count : 1; }
	FORCEINLINE int32 GetSkillPresetMaxCount() { return ConfigData ? ConfigData->skill_quick_slot_max_count : 1; }
	FORCEINLINE int32 GetAssaultMVPMaxCount() { return ConfigData ? ConfigData->assault_mvp_point_to_open_reward : 0; };
	FORCEINLINE int32 GetEtherInvenExtendCost() { return ConfigData ? ConfigData->need_cash_for_extend_inventory : 0; };
	FORCEINLINE int32 GetEtherInvenExtendCount() { return ConfigData ? ConfigData->increase_inventory_slot_per_buy_count : 0; };
	FORCEINLINE int32 GetEtherInvenMaxCount() { return ConfigData ? ConfigData->max_inventory_slot_count : 0; };
	FORCEINLINE int32 GetGuildRejoinTimeMin() { return ConfigData ? ConfigData->guild_rejoin_wait_time_min : 0; };
	FORCEINLINE int32 GetWinCountForBonus() { return ConfigData ? ConfigData->match_continuous_win_count_for_bonus : 0; };
	FORCEINLINE int32 GetGuildUpdateCost() { return ConfigData ? ConfigData->update_guild_gold_cost : 0; };
	FORCEINLINE int32 GetMinFloorforHeroRanking() { return ConfigData ? ConfigData->min_floor_for_hero_tower_ranking : 0; };
	FORCEINLINE int32 GetShowReviewInducementPopup() { return ConfigData ? ConfigData->show_review_inducement_popup : 0; };
	FORCEINLINE int32 GetGuildMercenaryDonatioMaxCount() { return ConfigData ? ConfigData->guild_mercenary_donation_max_count_for_day : 0; };
	FORCEINLINE int32 GetEventDiceFreeCount() { return ConfigData ? ConfigData->event_dice_free_count : 0; };
	FORCEINLINE int32 GetEventDicePointPerOneDice() { return ConfigData ? ConfigData->event_dice_point_per_one_dice : 0; };
	FORCEINLINE int32 GetMaxCostumeInventorySlotCount() { return ConfigData ? ConfigData->max_costume_inventory_slot_count : 0; };
	FORCEINLINE int32 GetGuildMasterChangePeriodDay() { return ConfigData ? ConfigData->guild_master_change_period_day : 0; };
	FORCEINLINE int32 GetGuildMasterLogoutLimitPeriodDay() { return ConfigData ? ConfigData->guild_master_logout_limit_period_day : 0; };

	FORCEINLINE int32 GetMaxGoldConvertCount() { return ConfigData ? ConfigData->exchange_count_gold : 0; };
	FORCEINLINE int32 GetMaxCrystalConvertCount() { return ConfigData ? ConfigData->exchange_count_gem : 0; };

	FORCEINLINE int32 GetMaximumNormalBrevetGade() { return ConfigData ? ConfigData->advanced_rank_level : 0; };

	FORCEINLINE const TMap<int32, FSummonItemSlotData>& GetStoreSummonItemListData() { return MD_Store_SummonItem_ListData; }
	FORCEINLINE const TMap<int32, FSummonItemGradeRange>& GetStoreSummonItemGradeRangeData() { return MD_Store_SummonItem_GradeRange; }
	FORCEINLINE FSummonItemSlotData* GetSummonItemData(int32 HashKey) { return MD_Store_SummonItem_ListData.Find(HashKey); }
	FORCEINLINE void SetStoreSummonItemFreeTime(int32 HashKey, int64 FreeTime)
	{
		auto* Data = MD_Store_SummonItem_ListData.Find(HashKey);
		if (Data)
			Data->SetNextFreeTime(FreeTime);
	}
	FORCEINLINE void SetStoreSummonItemDailyCount(int32 HashKey, int32 Count)
	{
		auto* Data = MD_Store_SummonItem_ListData.Find(HashKey);
		if (Data)
			Data->SetBuyCount(Count);
	}

	b2network::B2GuildBattleGuildPtr	GetMyGuildBattleInfo() {
		return MyBattleGuildData;
	}
	b2network::B2GuildBattleGuildPtr	GetOtherGuildBattleInfo() {
		return OtherBattleGuildData;
	}
	int32								GetGuildExp(int32 Lv) {
		return MD_GuildExps.Contains(Lv) ? MD_GuildExps[Lv]->exp_max : 0;
	}
	int32								GetGuildMaxLev() {
		return MD_GuildExps.Num();
	}

	void GetStoreProductList(int32 Category, int32 GoodsCategory, TArray<FStoreProductData>& OutList);
	void GetStoreCategoryOrder(TMap<int32, int32>& OutList);
	const FStoreProductData* GetStoreProductData(int32 ProductId);
	const FStoreCashBonusData* GetStoreCashBonusData(int32 ProductId);
	bool GetStoreCashBonusGoodsCategory(int32 GoodsCategory);
	const FStoreFundsSale* GetStoreFundsSaleData(int32 ProductId);
	bool GetStoreFundsSaleStoreCategory(int32 StoreCategory);
	bool GetStoreFundsSaleGoodsCategory(int32 GoodsCategory);
	const FSummonItemLotterySale* GetSummonItemLotterySaleData(int32 HashKey);
	void GetPackageList(int32 PackageCategory, TArray<FStoreProductData>& OutList);
	void GetDailyNoticePackageList(TArray<FStoreProductData>& OutList);
	const FPackageData* GetPackageData(int32 PackageID);
	void GetPackageProductList(int32 PackageId, TArray<FPackageProductData>& OutList);
	void GetPackageLevelUpList(int32 PackageCategory, TArray<FB2ShopLevelUpPackagePtr>& OutList);
	void GetPackageReachLevelList(int32 PackageId, TArray<FB2ShopReachLevelPackagePtr>& OutList);
	void GetFlatratePackageBonusList(int32 Index, TArray<FB2ShopFlatRatePackageBonusPtr>& OutList);
	int32 GetItemTemplateIdFromMDProductData(const FProductData& ProductData) const;
	int32 GetItemTemplateIdFromMDProductData(const int32 ProductID, const int32 ProductType) const;
	void ResponseBuyShopProduct(const FB2ResponseBuyShopProductPtr& BuyShopProduct);
	void ResponseBuyShopResultInfo(const FB2ResponseBuyShopResultInfoPtr BuyShopResult);
	void ResponseRewardShopMileage(const FB2ResponseRewardShopMileagePtr& MileageReward);
	void ResponseBuyMagicShopProduct(const FB2ResponsePurchaseMagicShopPtr& MagicShopPtr);

	FORCEINLINE bool GetStoreFundsSaleCount() const { return MD_Store_FundsSaleData.Num() > 0 ? true : false; }
	FORCEINLINE bool GetSummonItemLotterySaleCount() const { return MD_Store_LotterySaleData.Num() > 0 ? true : false; }
	FORCEINLINE const TMap<int32, FStoreMileageData>& GetStoreMileageData() { return MD_Store_Mileage_RewardData; }

	int32 GetRewardID(ERewardType RewardType) const;
	ERewardType GetRewardType(int32 RewardId) const;
	int32 GetRewardItemId(int32 RewardId) const;
	int32 GetRewardPushType(int32 RewardId) const;
	int32 GetRewardIDKeyValue(int32 iItemTemplateId) const;

	FClassCollectionItemData* GetClassCollectionItems(EPCClass TargetClass);

	bool GetSetItemUniqueKeys(EPCClass TargetClass, TArray<int32>& UniqueKeys);
	FMD_SetInfo* GetSetItemInfo(EPCClass TargetClass, int32 SetUniqueKey);
	FORCEINLINE const TArray<int32>& GetItemGroupIDs()	const { return ItemMasterData.UniqueGroupIDList; }
	FORCEINLINE const TArray<int32>& GetCostumeItemGroupIDs()	const { return ItemMasterData.CostumeUniqueGroupIDList; }

	int32 GetTotalItemCount(EPCClass TargetClass, EItemClass ItemClass);
	int32 GetCollectionItemCount(EPCClass TargetClass, EItemClass ItemClass);
	int32 GetEnhancedCollectionItemCount(EPCClass TargetClass, EItemClass ItemClass);
	bool HasEnhancedCollectionItem(EPCClass TargetClass);

	bool GetClassItemTemplateIDs(TArray<int32>& OutTemplateIDs, EPCClass TargetClass, EItemClass TargetItem, int32 OptionalGroupID = 0);
	const FMD_ItemSingleElem* GetItemMasterDetailInfo(int32 ItemTemplateID);
	const FMD_ItemSetOptionElem* GetItemSetOptionInfo(int32 SetOptionID);

	const FMonsterInfo* GetMonsterInfo(const ENPCClass NpcClass, const ENPCClassVariation NpcGrade);
	void GetMonsterGradeFactor(ENPCClass InClass, UB2NPCSingleClassInfo* InClassInfo, float& OutHPScale, float& OutAttackDefenseLevelScale, float& OutMaxArmorOverride);

	void OnReceiveCollectionReward(EPCClass TargetClass, int32 ItemTemplateID, b2network::B2RewardPtr RewardPtr);
	void OnReceiveSetCollectionReward(EPCClass TargetClass, int32 SetUniqueKey, b2network::B2RewardPtr RewardPtr);
	void CollectionRewardActionLog(EPCClass InPCClass, EItemClass InItemClass);

	const FCollectionItemData* GetCollectionItem(EItemClass ItemClass, int32 ID);

	EPCClass GetGeneralLotteryCharClass(int32 LotteryId);
	FB2LotteryTicketInfo* GetGeneralLotteryInfo(int32 LotteryId);
	FB2LotteryTicketInfo* GetSelelctiveLotteryInfo(int32 LotteryId);
	FB2LotteryTicketInfo* GetFixedLotteryInfo(int32 LotteryId);
	FB2LotteryTicketInfo* GetPrefixSelelctiveLotteryInfo(int32 LotteryId);
	int32	GetGeneralLotteryItemTemplateId(int32 iLotteryId);
	int32	GetSelectiveLotteryItemTemplateId(int32 iLotteryId);
	int32	GetFixedGradeLotteryItemTemplateId(int32 iLotteryId);
	int32	GetSelectiveEtherLotteryItemTemplateId(int32 iLotteryId);
	int32	GetPrefixSelectiveLotteryItemTemplateId(int32 iLotteryId);

	int32 GetAssaultMvpPointToOpenReward();

	// mode_type - B2ContentsMode
	// ranking_settle_state - B2ContentsModeState
	void GetDuelModeSettleSchedule(int32 in_mode_type, int32 in_ranking_settle_state, int32& out_day, int32& out_end_hour, int32& out_end_min);

	//==================== OtherUserInfo Function =============================//
	void SetOtherUserInfo(b2network::B2FindAccountInfoPtr InPlayerInfo);
	FB2ModPlayerInfo& GetOtherUserInfo();

	bool GetFairyOpenInfoAll(TArray<FMD_FairyOpenInfo> &OutList);
	bool GetFairyOpenInfo(EFairyType Target, FMD_FairyOpenInfo &OutData);

	bool GetFairyInfoAll(EFairyType Target, TArray<FMD_FairyInfo> &OutList);
	bool GetFairyInfo(EFairyType Target, int32 OptionId, int32 BlessLevel, FMD_FairyInfo &OutData);

	bool GetFairyLevelUpCostAll(TArray<FMD_FairyLevelUpCost> &OutList);
	bool GetFairyLevelUpCost(int32 TargetLevel, FMD_FairyLevelUpCost &OutData);

	////////////////////////////////////////////////////////////////////////////////
	// Get wing master data element of some type by appropriate key of each type.
	const FWingMD_EnhanceSingleElem* GetWingMD_Enhance(int32 InWingGrade, int32 InWingEnhanceLevel);
	FORCEINLINE const FWingMD_EnhanceSingleElem* GetWingMD_Enhance(const FB2Wing& InWing) { return GetWingMD_Enhance(InWing.EvolutionGrade, InWing.EnhanceLevel); }

	const FWingMD_EvolveSingleElem* GetWingMD_Evolve(int32 InWingGrade);
	FORCEINLINE const FWingMD_EvolveSingleElem* GetWingMD_Evolve(const FB2Wing& InWing) { return GetWingMD_Evolve(InWing.EvolutionGrade); }

	const FWingMD_PropOptionSingleElem* GetWingMD_PropOption(EPCClass InPCClass, int32 InOptionIndex, int32 InOptionLevel);
	const FWingMD_PropOptionSingleElem* GetWingMD_PropOption(const FB2Wing& InWing, int32 InOptionIndex);

	const FWingMD_PropOptionOpenSingleElem* GetWingMD_PropOptionOpenCondition(EPCClass InPCClass, int32 InOptionIndex);
	FORCEINLINE const FWingMD_PropOptionOpenSingleElem* GetWingMD_PropOptionOpenCondition(const FB2Wing& InWing, int32 InOptionIndex) {
		return GetWingMD_PropOptionOpenCondition(IntToPCClass(InWing.OwnerPCClass), InOptionIndex);
	}

	const FWingMD_PropOptionMaxLevelSingleElem* GetWingMD_PropOptionMaxLevel(EPCClass InPCClass, int32 InOptionIndex, int32 InWingGrade);
	FORCEINLINE const FWingMD_PropOptionMaxLevelSingleElem* GetWingMD_PropOptionMaxLevel(const FB2Wing& InWing, int32 InOptionIndex) {
		return GetWingMD_PropOptionMaxLevel(IntToPCClass(InWing.OwnerPCClass), InOptionIndex, InWing.EvolutionGrade);
	}

	// Get reference data for specific task of wing. Out** have valid data if it returns true.
	// The returned values are for next TargetGrade and/or Level of input arguments.
	bool GetWingEvolveRefData(int32 InTargetGrade, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, float& OutSuccessRate, int32& OutMaxFailPoint);
	bool GetWingEvolveRefData(const FB2Wing& InCurrentWing, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, float& OutSuccessRate, int32& OutMaxFailPoint); // For next evolve grade.
	bool GetWingEnhanceRefData(int32 InTargetGrade, int32 InTargetEnhanceLevel, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, int32& OutEnhancePointToAcquire, int32& OutRequiredEnhancePoint);
	bool GetWingEnhanceRefData(const FB2Wing& InCurrentWing, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, int32& OutEnhancePointToAcquire, int32& OutRequiredEnhancePoint); // For next enhance level
	bool GetWingPropOptionRefData(EPCClass InPCClass, int32 InOptionIndex, int32 InTargetOptionLevel, int32& OutRequiredItemCount);
	bool GetWingPropOptionRefData(const FB2Wing& InCurrentWing, int32 InOptionIndex, int32& OutRequiredItemCount);

	////////////////////////////////////////////////////////////////////////////////

	// Get HeroTower MasterData
	int32 GetMaxDailyTryCountHeroTower();
	int32 GetMaxDailyFreeTryCountHeroTower();
	int32 GetHeroTowerRetryCost(int32 TryCount);
	int32 GetHeroTowerMapId(int32 nFloor);
	float GetHeroTowerLevel(int32 nFloor);
	int32 GetHeroTowerLimitedTime(int32 nFloor);
	int32 GetHeroTowerRecommendedCombat(int32 nFloor);
	void GetHeroTowerRecommendAbility(int32 nFloor, int32& OutRecommendAttack, int32& OutRecommendDefense);
	TArray<FRewardItem>* GetHeroTowerRewards(int32 nFloor, bool IsSweep = false);
	int32 GetHeroTowerMaxFloor();
	int32 GetHeroTowerSweepRewardCountPerFloor();

	int32 GetItemLevelUpCost(const EItemInvenType InItemType, const int32 InGrade, const int32 InLevel);
	
	const FCostumeEnhanceCostInfo* GetCostumeEnhanceCost(int32 TargetGrade, int32 TargetLevel);

	const bool GetCurrentModeOpenTimes(TArray<b2network::B2MatchOpenPeriodInfoPtr> AllTimeData, TArray<b2network::B2MatchOpenPeriodInfoPtr>& OutTimeData);
	const bool GetRaidModeOpenTimes(TArray<b2network::B2MatchOpenPeriodInfoPtr> AllTimeData, int32 RaidType, TArray<b2network::B2MatchOpenPeriodInfoPtr>& OutTimeData);
	const bool GetIsRaidBossOpen(TArray<b2network::B2MatchOpenPeriodInfoPtr> AllTimeData, int32 RaidType, TArray<int32>& OpenDayData);

	//void RequestGetChapterClearReward(int32 ChapterId, EStageDifficulty Difficulty, int32 nRewardIndex);
	//void ResponseChapterClearReward(const FB2GetReceiveActClearReward& ResponseRewardInfo);

	//void RequestGetRepeatMissionClearReward(FServerStageID ServerStageId);
	//void ResponseRepeatMissionClearReward(const FB2GetReceiveActRepeatClearReward& ResponseRewardInfo);

	void ResponseGetQuestReward(const TArray<b2network::B2RewardPtr>& RewardInfo);

	// Extra handling after B2Airport response handling
	void OnReceiveEnhanceWing(const FB2EnhanceWing& EnhanceWingData);
	void OnReceiveUpgradeWing(const FB2UpgradeWing& UpgradeWingData);
	void OnReceiveEnhanceWingOption(const FB2EnhanceWingOption& EnhanceWingOptionData);

	/** Once was common part for wing enhance, wing upgrade, and wing option enhance..
	* -> Now somewhat general interface for some post oeration consumable amount handling */
	void OnResponseConsumableAmountDecrease(int32 InConsumableRefID, int32 ConsumedAmount, const TArray<b2network::B2ItemServerInfoPtr>& ChangedConsumableData, const TArray<int64>& DeletedConsumableIDs);
	void OnResponseConsumableAmountIncrease(int32 InConsumableRefID, const b2network::B2ItemServerInfoPtr& ChangedConsumableData);

	void GetKakaoFriendInviteRewardInfo(const int32 RewardType, TArray<b2network::B2mdKakaoFriendInviteRewardPtr> &OutList);

	FString GetWebURL(FString URLKey);
	FString GetWebURL(EUIScene CurScene);

	// Magic Shop
	const FMagicShopProduct* GetMagicShopProductData(int32 ProductID);
	const FMagicShopRenewalCost* GetMagicShopRenewalCostData(int32 RenewalCount);
	FORCEINLINE int32 GetMagicShopRenewalCostDataCount() const { return MD_MagicShopRenewalCost.Num(); }

	FORCEINLINE void SetWaitInventorySync(bool isSync) { bWaitInventorySync = isSync; }
	FORCEINLINE bool GetWaitInventorySync() { return bWaitInventorySync; }

	// SelectItemOption
	const TArray<FSelectItemOption>* GetSelectItemOptionData(int32 EquipType, int32 Grade);
	FItemSurpassLevel GetSurpassLevelData(int32 InGrade);

	// Item Dismantle Info
	bool GetItemDismantleInfoData(EItemInvenType InvenType, int32 Grade, FItemDismantleInfo& OutList);

	// Item Convert Info
	int32 GetConvertTable(int32 beforeConvertItemID, FConvertTableInfo& OutList);
	bool GetConvertCost(int32 tableID, int32 count, FConvertCostInfo& OutList);

	// Item Quality Change
	void GetQualityTable(TArray<FQualityTransRateInfo>& OutList);
	int32 GetMaxQuality(int64 hammerID);
	int32 GetMinQuality(int64 hammerID);

	//ItemForge
	void GetItemForgeTable(EItemForgeType forgeType, EPCClass SelectedClass, TArray<FItemForgeInfo>& OutList, TArray<FItemForgePeriodicStatus>& periodicTable);
	void GetItemForgeTable(EItemForgeType forgeType, EPCClass SelectedClass, TArray<FItemForgeInfo>& OutList, TArray<FItemForgePeriodicStatus>& periodicTable, TArray<EItemInvenType> filterInven, TArray<int32> filterStarGrade);
	void GetItemForgeLimitTable(int32 limitID, FItemForgeLimitInfo& OutList);
	void GetItemForgeMaterialTransInfo(int32 forgeID_, TArray<FItemForgeMaterialTransInfo>& OutList);

	//Seal box
	FSealBoxInfo GetSealBoxInfo(int32 sealboxRefID_);
	void GetSealBoxRewardRateInfo(int32 rewardID, TArray<FSealBoxRewardRateInfo>& OutList);
	bool DoesSealboxHaveEquipment(int32 rewardID);

	//Version Info, CL Number for version info in option pop-up
	/*
	FORCEINLINE void SetServerVersionInfo(const FString& _ServerVersion)
	{
	m_ServerVersionInfo = _ServerVersion;
	};
	*/
	FORCEINLINE void SetChangelistNum(const FString& _ChangelistNum)
	{
		m_ChangelistNum = _ChangelistNum;
	};
	FORCEINLINE FString GetServerVersionInfo() const
	{
		return m_ServerVersionInfo;
	};
	FORCEINLINE FString GetChangelistNum() const
	{
		return m_ChangelistNum;
	};

private:
	void PreSetMasterData();
	void PostSetMasterData();

	// Master Data Implement
	void SetMasterDataImpl_ConfigData(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_Skill(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemLevelupExpFactorAndOptionAddValue(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemLevelupCost(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_StageInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ResurrectionInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_TeamBattleFormationInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_RankPromotionInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DailyMatchReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_PVPMatchRankingReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_TeamMatchRankingReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ControlMatchRankingReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_CounterDungeon(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_CounterDungeonPeriod(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_CounterDungeonWeekReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_RewardInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_WingEnhance(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_WingUpgrade(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_WingOptionValue(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_WingOptionOpen(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_WingOptionMaxLevel(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_HeroTowerFloor(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopLotteryPrice(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopLotteryPossibleGradeAndSpecialGrade(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopLotteryDailylimitsMaxCount(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopProduct(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopPackage(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_PackageProduct(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopMileageReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_CollectionReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DailyAttendanceInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GuildBattleResultReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GuildBattleSeasonReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_Raid(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_MonsterInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GeneralLottery(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_SelectiveLottery(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_PrefixSelectiveLottery(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_FixedGradeLotteries(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemEnhancementCost(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemEnhancementEffectInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_AdditionalMatchPointInfos(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_StageFixedDrop(const FB2MasterDatas& InMasterDatas);
	void SetMasterDatalmpl_AssaultMvpRewardInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_EventAttendanceReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_PlayTimeReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GameResultExp(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_Tutorial(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GuildExps(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GuildSkill(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_Guild(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_GuildBattleSchedule(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_RestReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DuelModeReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_Item(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_EventInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_KakaoInviteReward(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_HotTimeEvent(const FB2MasterDatas& InMasterDatas, const FEventInfo& HotTimeEventInfo);
	void SetMasterDataImpl_HotTimeProduct(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopCashBonus(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopFundsSale(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopLotterySale(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopLevelUpPackage(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopFlatratePackageBonus(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopReachLevelPackage(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DuelModeSettleSchedule(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ShopPackageTabOrder(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_WebURL(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_MagicShopProduct(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_MagicShopRenewalCost(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemlevelupFactor(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_SelectItemOption(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_SurpassLevel(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemDismantleInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_FairyInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_FairyLevelupCosts(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_FairyOpenInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_Costume(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DonationPointRankingRewardInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DonationBundleFixedRewardInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_DonationPointStepRewardInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ExtendMessage(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_CostumeEnhanceCostInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ConvertTableInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ConvertTableCost(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_LotteryShopRateQualityInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_QualityTransRateInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemForgeInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemForgeLimitStatus(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_ItemForgeMaterialTransInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_SealBoxInfo(const FB2MasterDatas& InMasterDatas);
	void SetMasterDataImpl_SealBoxRewardRateInfo(const FB2MasterDatas& InMasterDatas);

public:
	void RequestSetNickName(FString);
	void RequestLevelUpSkill(int32 SkillId);
	void RequestResetSkillPreset(EPCClass SelectedClass);
	void RequestBuySkillPoint(TArray<int32>& NumSkillPointToBuy);
	void RequestSetUsingSkill(EPCClass SelectedClass, int32 PresetType, int32 UsingSkill1, int32 UsingSkill2, int32 UsingSkill3);
	void RequestSetCharNickName(EPCClass InCharClass, const FText& InNickName);

	void RequestSyncToServerShowHead();
	void RequestSyncToServerCostumeVisible();

	void RequestGetAccountCostumeAll();
	void RequestGetAccountCostume(const EPCClass& PcClass, const EItemInvenType& ItemInvenType);

	void RequestGetAccountTotem();
private:
	void ResponseSetNickName();
	void ResponseLevelUpSkill(const FB2LevelUpSkillInfo&);
	void ResponseResetSkillPreset(const FB2ResetSkillInfo& ResetSkillInfo);
	void ResponseBuySkillPoint(const FB2BuySkillPointInfo& BuySkillPointInfo);
	void ResponseSetUsingSkillInfo(const FB2SetUsingSkillInfo& SetUsingSkillInfo);
	void ResponseSetCharNickName();
	void ResponseEventAttendance(const FB2ResponseEventAttendancePtr& EventAttendance);
	void ResponseUpdateDailyPlayTimeClass(const FB2ResponseUpdateDailyPlayTimePtr& UpdateDailyPlayTime);
	void ResponseDailyPlayTimeRewardClass(const FB2ResponseReceiveDailyPlayTimeRewardPtr& PlayTimeReward);
	void ResponseSetHelmetVisible(const FB2SetHelmetVisible& SetHelmetVisible);
	void ResponseSetCostumeVisible(const FB2SetCostumeVisiblePtr& CostumeVisible);
	void ResponseSetEnhanceTeamFormation(const FB2EnhanceTeamFormation& EnhanceResult);

	void BridgeRequestJoinRoom(const FString& RoomID, int32 RoomType, int32 RoomAccessType);

	void ResponseGetAccountCostume(const FB2GetAccountCostumePtr& info);
	void ResponseAddCostumeToInventory(const FB2AddCostumeToInventoryPtr& pAddCostume);

	void ResponseGetAccountTotem(const FB2GetAccountTotemPtr& info);

public:
	FAttendanceDayData GetAttendanceDayInfo(int32 InType, int32 InValue);
	int32 GetAttendance_day();
	FORCEINLINE const TArray<FPlayTimeRewrad>& GetPlayTimeRewardData() const { return PlayTimeRewradData; }
	FORCEINLINE const FPlayTimeState& GetPlayTimeStatus() const { return PlayTimeStatusData; }
	FORCEINLINE const FEventAttendanceState&  GetEventAttendanceStatus() const { return EventAttendanceStatusData; }
	FORCEINLINE const FEventNewUserState& GetEventNewUserStatus() const { return EventNewUserStatusData; }
	FORCEINLINE bool GetConnectRewardButtonPush() { return PushConnectRewardButton; }
	bool GetEventAttendanceRewardData(TArray<FEventAttendanceReward>& AttendanceRewards, const int32 eventId);
	void SetConnectRewardButtonPush(bool Push) { PushConnectRewardButton = Push; }

	bool IsReturnUser() const;
	void ResponseCheckModeOpen(const FB2ResponseCheckModeOpenPtr& CheckModeOpen);
	void SetEnableDailyReceive(bool Enable) { m_AttendanceRewardInfo.bIsAttendance = Enable; }
	void SetDailyAttendanceInfo(const FB2ReceiveDailyAttendance& AttendanceInfo);
	void SetNewUserAttendanceInfo(int32 InAttendanceDay, int64 InNextAttendanceTime);
	FORCEINLINE const FAttendanceRewardInfo& GetAttendanceInfo() const { return m_AttendanceRewardInfo; }

	FORCEINLINE const TArray<B2RollingBannerPtr>& GetLobbyBannerData() const { return LobbyBannerData; }

	int32 GetExpectedExpOfRaidGame(int32 RaidType, int32 RaidStep);

	TMap<int32, FItemPresetInfo> const GetAllPresetItem(EPCClass PCClass);

	void RequestGetServerVersion();
	void ResponseGetServerVersion(const FString& strVersion, const FString& strStage);

	void TheInitialSoundLocTypeAndCreateNickName();
	void CheckOpenCreateNickName();

	bool HasUnappliedItemOption();
	bool GetItemOptionUnappliedItem(FB2Item& UnappliedItem);
	void SetUnappliedItemOption(FB2ResponseGetItemRandomOptionPtr UnappliedItemOptions);
	void ClearUnappliedItemOptionInfo();

	FORCEINLINE const TArray<FItemOption>& GetUnappliedItemOptions() const { return UnAppliedOptions; }
	FORCEINLINE int64 GetOptionUnappliedItemUniqueID() const { return UnAppliedItemUniqueInstanceID; }

	bool HasUnappliedItemQuality();
	bool GetItemQualityUnappliedItem(FB2Item& UnappliedItem);
	void SetUnappliedItemQuality(FB2ResponseGetItemRandomQualityPtr UnappliedItemQuality);
	void ClearUnappliedItemQualityInfo();

	//FORCEINLINE const TArray<FItemOption>& GetUnappliedItemOptions() const {
	//	return UnAppliedOptions;
	//}
	//FORCEINLINE int64 GetOptionUnappliedItemUniqueID() const {
	//	return UnAppliedItemUniqueInstanceID;
	//}

	void UpdateCombatStateScale(const float fScale);

private:
	FB2OriginAccountInfoPtr		AccountInfo;

	FTutorialCharacterData		TutorialCharacterData;
	FLocalCharacterData			LocalCharacterData;
	FB2RedDotManager			RedDotManager;

	//======================================================================
	// Master data
	FB2ConfigDataPtr			ConfigData;

	TMap<int32, FSkillMasterData>				SkillMasterData;
	TArray<FModDailyReward>						ModDailyData;
	TArray<FModWeeklyReward>					PvPWeeklyRewardData;
	TArray<FModWeeklyReward>					TeamMatchWeeklyRewardData;
	TArray<FModWeeklyReward>					ControlWeeklyRewardData;
	TArray<FCounterDungeon>						CounterDungeonData;
	TMap<int32, TArray<FCounterDungeonPeriod>>	CounterDungeonPeriodData;
	TArray<FCounterDungeunWeekReward>			CounterDungeonRewardData;
	TMap<int32, TArray<FRaidMasterData>>		RaidMasterDatas;
	TMap<int32, FRewardData>					RewardInfoData;
	FItemMasterData								ItemMasterData;

	TMap<int32, TArray<FAttendanceDayData>>		m_AttendanceDayDatas;
	b2network::B2mdAssaultMvpRewardInfoPtr		AssaultMvpRewardInfo;
	FAttendanceRewardInfo						m_AttendanceRewardInfo;

	TMap<int32, TArray<FEventAttendanceReward>>	EventAttendanceRewardData;
	TArray<FPlayTimeRewrad>						PlayTimeRewradData;
	TArray<B2RollingBannerPtr>					LobbyBannerData;

	/** It is for now ResurrectMasterData for "StageGameMode".
	* If resurrect data is to be added for different game mode, change the format or add another ResurrectMasterData. */
	TMap<EResurrectGameModeType, FModeResurrectMasterDataElement> ResurrectMasterData; // Ket is ModeType  / Key is resurrect count.

	// Team Match
	TMap<int32, FTeamMatchFormationTypeInfo> TeamMatchFormationMasterData;

	// Brevet
	TMap<int32, FBrevetRankInfo> BrevetMasterData;

	// Follows wing master data, having their key as the number getting by GetKey method of each struct.
	TMap<int32, FWingMD_EnhanceSingleElem>				WingMD_Enhance;
	TMap<int32, FWingMD_EvolveSingleElem>				WingMD_Evolve;
	TMap<int32, FWingMD_PropOptionSingleElem>			WingMD_PropOption;
	TMap<int32, FWingMD_PropOptionOpenSingleElem>		WingMD_PropOptionOpenCondition;
	TMap<int32, FWingMD_PropOptionMaxLevelSingleElem>	WingMD_PropOptionMaxLevel;

	// Hero Tower
	TMap<int32, FMD_HeroTowerFloorSingleElem> MD_HeroTowerFloor;

	// Store MD
	TMap<int32, FSummonItemSlotData>		MD_Store_SummonItem_ListData;
	TMap<int32, FSummonItemGradeRange>		MD_Store_SummonItem_GradeRange;
	TMap<int32, FStoreProductData>			MD_Store_GeneralShopProductData;
	TMap<int32, int32>						MD_Store_CategoryOrderData;
	TMultiMap<int32, FPackageData>			MD_Store_PackageData;
	TMap<int32, FStoreCashBonusData>		MD_Store_CashBonusData;
	TMap<int32, FStoreFundsSale>			MD_Store_FundsSaleData;
	TMap<int32, FSummonItemLotterySale>		MD_Store_LotterySaleData;
	TArray<FB2ShopPackageTabPtr>			MD_Store_PackageOrderData;
	TMap<int32, FStoreMileageData>			MD_Store_Mileage_RewardData;
	TMultiMap <int32, FPackageProductData>				MD_Store_PackageProductData;
	TMultiMap <int32, FB2ShopLevelUpPackagePtr>			MD_Store_LevelUpPackageData;
	TMultiMap <int32, FB2ShopReachLevelPackagePtr>		MD_Store_ReachLevelPackageData;
	TMultiMap <int32, FB2ShopFlatRatePackageBonusPtr>	MD_Store_FlatRatePackageBonusData;

	// Lottery
	TMap<int32, FB2LotteryTicketInfo>		MD_GeneralLottery;
	TMap<int32, FB2LotteryTicketInfo>		MD_SelectiveLottery;
	TMap<int32, FB2LotteryTicketInfo>		MD_FixedGradeLottery;
	TMap<int32, FB2LotteryTicketInfo>		MD_PrefixSelectiveLottery;
	TMap<int32, FB2EnhanceItemCost>			MD_EnhanceItemCostData;
	TMap<int32, FItemEnhanceEffectInfo>		MD_EnhanceItemEffectData;
	int32									MD_MinEnhanceLevelForSetEffect;
	int32									MD_MaxEnhanceLevelForSetEffect;

	// Duel Mode
	TArray<b2network::B2mdDuelModeSettleSchedulePtr>	MD_DuelModeSettleSchedules;
	TMap<int32, TArray<b2network::B2RewardPtr>>			MD_DuelModeRewardInfo;

	// Raid Exp
	TMap<int32, FRaidExpMDSingleData> MD_RaidExpData;

	TMap<int32, TArray<b2network::B2mdKakaoFriendInviteRewardPtr>> MD_KakaoFriendInviteRewardInfo;

	// Rest Reward
	TArray<b2network::B2mdRestRewardPtr> MD_RestRewardData;

	// Web
	TMap<FString, FString> MD_WebURL;

	// Monster
	TMap<int32, TMap<int32, FMonsterInfo>>	MD_MonterListData;

	/** United option mapping of EItemOption and ESkillOption */
	static TMap<EUnitedCombatOptions, FUCOBoundInfo>	UnitedOptionMappingTable;
	static TMap<EItemOption, EUnitedCombatOptions>		ItemOptionToUnitedOptionMappingTable;
	static TMap<ESkillOption, EUnitedCombatOptions>		SkillOptionToUnitedOptionMappingTable;

	// Guild Data
	FGuildInfo MyGuildData;
	FGuildInfo OtherGuildData;
	FUserGuildInfo UserGuildInfo;
	b2network::B2GuildBattleGuildPtr						MyBattleGuildData;
	b2network::B2GuildBattleGuildPtr						OtherBattleGuildData;
	TArray<b2network::B2mdGuildBattleSeasonRewardPtr>		GuildSeasonRewardInfo;
	TMap<int32, b2network::B2mdGuildExpPtr>					MD_GuildExps;
	TMap<int32, b2network::B2mdGuildSkillPtr>				GuildSkillInfoMasterData;
	
	TMap<int32, b2network::B2mdGuildBattleResultRewardPtr>	GuildBattleResultRewardMasterData;
	TArray<b2network::B2mdGuildBattleScheduleInfoPtr>		GuildBattleSchedule;

	// Fairy Data
	TMap<EFairyType, TArray<FMD_FairyInfo>>	MD_FairyInfoData;
	TMap<int32, FMD_FairyLevelUpCost>		MD_FairyLevelUpCostData;
	TMap<EFairyType, FMD_FairyOpenInfo>		MD_FairyOpenInfoData;

	// Item Levelup
	FItemLevelupFactorMap					ItemLevelupFactorMap;
	TMap<int32, FItemLevelupCostPerLevel>	ItemLevelupCostMap;
	TArray<FItemLevelupFactor>				MD_ItemLevelupPrimaryFactor;

	// Item Surpass
	TArray<FItemSurpassLevel> MD_SurpassLevelData;

	// Item Option Change
	bool					bHasUnappliedItemOption;
	int64					UnAppliedItemUniqueInstanceID;
	TArray<FItemOption>		UnAppliedOptions;

	// Item Quality Change
	bool					bHasUnappliedItemQuality;
	FB2Item					UnAppliedItem;

	// Other User Info
	FB2ModPlayerInfo OtherUserInfo;

	// Additional Match Point Cost Info
	TArray<int32> MD_AdditonalMatchCostData;
	bool bUsedAdditonalMatchCount = false;

	// Attendance
	FPlayTimeState PlayTimeStatusData;
	FEventAttendanceState EventAttendanceStatusData;
	FEventNewUserState EventNewUserStatusData;
	bool PushConnectRewardButton;
	bool bAttendanceOpenDailyOnce = false;

	// Fairy Gift
	int64 NextFairyGiftTime;

	// Magic Shop
	TMap<int32, FMagicShopProduct>			MD_MagicShopProduct;
	TMap<int32, FMagicShopRenewalCost>		MD_MagicShopRenewalCost;

	// Select Item Option
	TMap<int32, TArray<FSelectItemOption>>	MD_SelectItemOption;

	// Item Dismantle Info
	TMultiMap<EItemInvenType, FItemDismantleInfo>	MD_ItemDismantleInfo;
	
	// Costume Enhance Cost Info
	TMap<int32, FCostumeEnhanceCostInfo>	MD_CustumeEnhanceCostInfo;

public:
	// Convert Table Info
	TArray<FConvertTableInfo>	MD_ExchangeSystemInfo;
	TArray<FConvertCostInfo>	MD_ExchangeSystemCost;

	// Hammer Quality Rate Info (or something. Server developer doesn't know how to speak English so client developer had to re-translate it to understand.)
	TArray<FLotteryShopRateQualityInfo> MD_LotteryShopRateQualityInfo;
	TArray<FQualityTransRateInfo> MD_QualityTransRateInfo;

	// Item Forge Info
	TArray<FItemForgeInfo>				MD_ItemForgeInfo;
	TArray<FItemForgeMaterialTransInfo>	MD_ItemForgeMaterialTransInfo;
	TArray<FItemForgeLimitInfo>			MD_ItemForgeLimitInfo;

	// Sealbox Info
	TArray<FSealBoxInfo>				MD_SealBoxInfo;
	TArray<FSealBoxRewardRateInfo>		MD_SealBoxRewardRateInfo;

private:
	// Version Info, CL Number for version info in option popup
	FString m_ServerVersionInfo;
	FString	m_ChangelistNum;

	FString	RequestedUserNickName;
	EPCClass RequestedCharNickNameSetClass;
	FText RequestedCharNickNameSetNameText;

	bool bReceivedServerMasterData;
	bool bForceReqToServer;

	// InventorySync
	bool bWaitInventorySync;

	// Subscribe
	TArray<std::shared_ptr<class issue>> Issues;
};


#define GetB2ItemArray(index, info)																		\
	TArray<FB2Item>	B2ItemArray;																		\
	TU::CopyVectorHelper<FB2Item, b2network::B2ItemServerInfoPtr>	helper(&B2ItemArray);		\
	helper = GET_TUPLE_DATA(index, info);

#endif // __B2_CLIENT_DATA_STORE_H__
