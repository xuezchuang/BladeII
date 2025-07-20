#pragma once
//#include "BladeII.h"
#include "EngineMinimal.h"
#include "CommonStruct.h"
#include "Common.h"
#include "B2EtherData.h"
#include "B2TotemData.h"

struct FPCWingContainer // Per PC
{
	FPCWingContainer()
	{
		bHasWing = false;
	}

	bool bHasWing; // WingData must be valid if this is true.
	FB2Wing WingData;

	// For now, set bHasWing by WingData until server explicitly sends it.
	void SetHasWingByWingData();
};

struct FAncientRelic
{
	FAncientRelic(int32 id, int32 lv, int32 grade)
		: nRelicId(id)
		, nRelicLevel(lv)
		, nRelicGrade(grade)
	{
	}

	explicit FAncientRelic()
		: nRelicId(0)
		, nRelicLevel(0)
		, nRelicGrade(0)
	{
	}

	int32 nRelicId;
	int32 nRelicLevel;
	int32 nRelicGrade;
};

using FAncientRelicArray = TArray<FAncientRelic>;

struct FUnitySkillMission
{
	FUnitySkillMission(EPCClass mainc, EPCClass subc, int32 step, int32 state)
		: MainClass(mainc)
		, SubClass(subc)
		, nStep(step)
		, nState(state)
	{
	}

	explicit FUnitySkillMission()
		: MainClass(EPCClass::EPC_End)
		, SubClass(EPCClass::EPC_End)
		, nStep(0)
		, nState(0)
	{
	}

	EPCClass MainClass;
	EPCClass SubClass;
	int32 nStep;
	int32 nState;
};

using FUnitySkillMissionArray = TArray<FUnitySkillMission>;

struct FUnitySkillAwakenMission
{
	FUnitySkillAwakenMission(EPCClass mainc, int32 id, int32 count, bool compelete)
		: MainClass(mainc)
		, nMissionID(id)
		, ntry_count(count)
		, bCompleted(compelete)
	{
	}

	explicit FUnitySkillAwakenMission()
		: MainClass(EPCClass::EPC_End)
		, nMissionID(0)
		, ntry_count(0)
		, bCompleted(false)
	{
	}

	EPCClass MainClass;
	int32 nMissionID;
	int32 ntry_count;
	bool bCompleted;
};

using FUnitySkillAwakenMissionArray = TArray<FUnitySkillAwakenMission>;

struct FUnitySkillState
{
	FUnitySkillState(bool missioncomplete, bool awkencomplete, bool haveawake)
		: AllMissionComplete(missioncomplete)
		, AllAwakenComplete(awkencomplete)
		, HaveAwaken(haveawake)
	{
	}

	explicit FUnitySkillState()
		: AllMissionComplete(false)
		, AllAwakenComplete(false)
		, HaveAwaken(false)
	{
	}

	bool AllMissionComplete;
	bool AllAwakenComplete;
	bool HaveAwaken;
};

struct FSkillOptionData
{
	FSkillOptionData(int32 id, float value, float power = 0) : OptionId(id), OptionValue(value), OptionPower(power) {}
	int32 OptionId;
	float OptionValue;
	float OptionPower;

	bool operator==(const FSkillOptionData& Other) const
	{
		return OptionId == Other.OptionId && OptionValue == Other.OptionValue;
	}
};

struct FSkillMasterData
{
	FSkillMasterData(int32 id, int32 learnlevel, int32 maxlevel, int32 levelUpCostFactor, EPCClass userclass, bool active) :
		SkillId(id),
		LearnLevel(learnlevel),
		MaxSkillLevel(maxlevel),
		LevelUpCostFactor(levelUpCostFactor),
		UserClass(userclass),
		bActiveSkill(active)
	{	}

	int32 SkillId;
	int32 LearnLevel;
	int32 MaxSkillLevel;
	int32 LevelUpCostFactor;
	EPCClass UserClass;
	bool bActiveSkill;

	TMultiMap<int32, FSkillOptionData> LevelOptionData;
	TMap<int32, int32> LevelUpPointData;
};


class ICharacterDataStore
{
public:
	virtual int32	GetCharacterLevel(EPCClass) const = 0;
	virtual FString	GetUserNickName() const = 0;
	virtual void	GetCharacterRegistredSkills(EPCClass InCharacterClass, TArray<int32>& RegistredSkills, bool bIncludeUnitySkill = true) const = 0;
	virtual bool	IsSkillRegistred(EPCClass InCharacterClass, int32 SkillId) const = 0;
	virtual void	GetCharacterUnregistredSkills(EPCClass InCharacterClass, TArray<int32>& UnregistredSkills) const = 0;
	virtual void	GetCharacterSkills(EPCClass InCharacterClass, TArray<FB2SkillInfo>& Skills) const = 0;
	virtual void	GetCharacterSkills(EPCClass InCharacterClass, TArray<int32>& OutSkillIDs) const = 0; // ID only
	virtual void	GetCharacterPassiveSkills(EPCClass InCharacterClass, TArray<int32>& SkillIds) const = 0;
	virtual int32	GetCharacterSkillLevel(int32 SkillId) const = 0;
	virtual void	GetCharacterSkillOptionsOfID(int32 InSkillId, TArray<FSkillOptionData>& OutFoundOptionData) const = 0;

	virtual void	GetEquippedItems(EPCClass PCClass, TArray<FB2Item>& EquippedItems) const = 0;
	virtual void	GetEquippedItems(EPCClass PCClass, TMap<EItemEquipPlace, FB2Item>& EquippedItems) const = 0;
	virtual void	GetEquippedCostumeItems(EPCClass PCClass, TArray<FB2Item>& EquippedCostumeItems) const = 0;
	virtual void	GetEquippedEthers(EPCClass PCClass, TMap<int64, FB2Ether>& EquippedEthers) const = 0;
	virtual void	GetEquippedTotems(EPCClass PCClass, TMap<int64, FB2Totem>& EquippedTotems) const = 0;

	virtual bool	IsLocalCharacterData() = 0;

	virtual int32	GetUserLevel() const = 0;

	virtual EPCClass	GetMainPlayerClass() const = 0;
	virtual EPCClass	GetSubPlayerClass() const = 0;
	virtual void		SetMainPlayerClass(EPCClass) = 0;
	virtual void		SetSubPlayerClass(EPCClass) = 0;

	virtual bool DoesCharacterHaveWing(EPCClass InPCClass) const { return false; }
	// OutData is valid only when it returns true
	virtual bool GetCharacterWing(EPCClass InPCClass, FB2Wing& OutData) const { return false; }

	virtual bool IsShowHeadItem(EPCClass InCharacterClass) const { return true; }
	virtual bool IsShowCostume(EPCClass InCharacterClass, ECostumeEquipPlace InCostumeEquipPlace) const { return true; }
	
	// You may optionally set FB2Item array to use custom equipment list.
	virtual void GetSetItemValue(EPCClass InPCClass, TMap<int32, int32>& OutSetItemDatas) = 0;	// OutSetItemDatas : Key (SetItemId) / Value (Count)
	virtual bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, TArray<FB2Item>* InItemArray = NULL) = 0;
	virtual float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) = 0;
	virtual float GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) = 0;
	virtual float GetCombatStatusValue(EPCClass InPCClass, TArray<FB2Item>* InItemArray = NULL) const = 0;

	virtual int32 GetCharBrevetRank(EPCClass InCharacterClass) const = 0;
	virtual int32 GetCharBrevetNodeIndex(EPCClass InClass) const = 0;
	virtual int32 GetAppliedCharBrevetRank(EPCClass InCharacterClass) const = 0;
	virtual int32 GetAppliedCharBrevetNodeIndex(EPCClass InClass) const = 0;

	virtual int32 GetEnhanceSetEffectLevel(EPCClass InPCClass) = 0;
	virtual double GetEnhanceSetEffectFactor(EPCClass InClass) = 0;

	virtual FPCWingContainer* GetCharacterWingDataPtr(EPCClass InPCClass) = 0;

	virtual int64 GetAccountId() = 0;
	virtual int32 GetEtherOffenseSetID(EPCClass InPCClass) = 0;
	virtual int32 GetEtherDefenceSetID(EPCClass InPCClass) = 0;

	virtual int32	GetUnitySkillId(EPCClass InCharacterClass) const = 0;

	virtual void GetUnitySkillMissionArray(FUnitySkillMissionArray& UnitySkillMissionArray, EPCClass InCharacterClass) = 0;

	virtual TArray<b2network::B2GuildSkillPtr>& GetGuildSkillInfos() = 0;

	virtual const TArray<int32>& GetPreviewCostumeItems() const = 0 ;

public:
	static void GetRenderItem(const ICharacterDataStore* CharacterDataStore, EPCClass InCharacterClass, TArray<FB2Item>& EquippedItems, bool IgnoreFilter = false);
}; 

struct FPlayerExpLevelInfo
{
	int32 Level;
	int32 Exp;
	int32 Exp_Max;

	FPlayerExpLevelInfo()
	{
		Level = Exp = Exp_Max = 0;
	}
};

/**
* FB2CharacterInfo and other necessary stuff for client.
* It can be used for display some of BladeIIPlayer stats while it does not really exist.
* Its variables can be simply copied to BladeIIPlayer at the beginning of combat or be directly used consistently in stage.
*/
struct FB2PlayerSharedStats
{
	FB2PlayerSharedStats()
		: BasicInfo(nullptr)
		, SkillSlotID(EItemPresetSlotType::EIPS_First)
	{
	}

	void CheckNeedRegistSkill();
	void CheckNeedRegistSkill(EItemPresetSlotType InSkillSlotID);

	void SetSkillSlotID(EItemPresetSlotType InSkillSlotID) { SkillSlotID = InSkillSlotID; }
	EItemPresetSlotType GetSkillSlotID() const { return SkillSlotID; }

	void UpdateBeforeExpLevelInfo();
	
	FB2OriginCharacterInfoPtr BasicInfo;
	EItemPresetSlotType SkillSlotID;
	FPlayerExpLevelInfo BeforeExpLevelInfo;
};


/** Contains data of a single item equipping slot. */
struct FEquippedItemInfo
{
	FEquippedItemInfo()
	{
		bIsEquipped = false;
	}

	uint32 bIsEquipped : 1; // item information here will be ignored if it is false.

	/** The item information itself */
	FB2Item EquippedItem;
};

struct FItemPresetInfo
{
	FItemPresetInfo(int32 presetId)
	{
		PresetID = presetId;

		for (int i = 0; i < (int32)EItemEquipPlace::EIEP_End; ++i) 
		{
			EquippedItemID[i] = 0;
		}
	}

	bool IsEquippedItem(int64 itemUID)
	{
		for (int i = 0; i < (int32)EItemEquipPlace::EIEP_End; ++i)
		{
			if (EquippedItemID[i] == itemUID)
			{
				return true;
			}
		}
		return false;
	}

	void RemoveItem(int64 itemUID)
	{
		for (int i = 0; i < (int32)EItemEquipPlace::EIEP_End; ++i)
		{
			if (EquippedItemID[i] == itemUID)
			{
				EquippedItemID[i] = 0;
			}
		}
	}

	void AddItem(EItemEquipPlace equipPlace, int64 itemUID)
	{
		const uint32 EquipPlaceUint = static_cast<uint8>(equipPlace);
		if (EquipPlaceUint >= (int32)EItemEquipPlace::EIEP_Weapon && EquipPlaceUint < (int32)EItemEquipPlace::EIEP_End)
		{
			EquippedItemID[EquipPlaceUint] = itemUID;
		}
	}

	int32 PresetID;
	int64 EquippedItemID[(int32)EItemEquipPlace::EIEP_End];
};

struct FPerCharacterItemContainer
{
	FPerCharacterItemContainer() : bSyncingData(false), CurrentItemPresetID(0) {}
	/** Item list stored in container for each item class. Key is item's InstanceUID */
	TMap<int64, FB2Item> StoredWeapons;
	TMap<int64, FB2Item> StoredProtections;
	TMap<int64, FB2Item> StoredAccessries;

	TMap<int64, FB2Item> StoredCostumeWeapons;
	TMap<int64, FB2Item> StoredCostumeArmors;

	TMap<int32, int32> StoredItemCounts;

	/** Current equipment information for all possible equipment places. */
	FEquippedItemInfo AllEuippedInfo[(int32)EItemEquipPlace::EIEP_End];
	TMap<int32, FEquippedItemInfo> AllEuippedCostumeInfo;
	bool bSyncingData;

	// for item preset
	int32 CurrentItemPresetID;
	TMap<int32, FItemPresetInfo> AllItemPresets;

	bool RemoveItem(int64); // It check equipped list first, then stored list.
	void AddItem(const FB2Item& Item, bool CheckPreset = true); // Add to equipped list if bCurrentlyEquipped is true, or to stored list.
	FB2Item* FindItem(int64);
	void EquipItem(FB2Item*, bool, int32);
	void MakeDirty() { bSyncingData = false; }
	void UpdateItemMasterDataOnMD();  // call after itemMasterData received

	int32 GetContainItemCount(int32 InRefID);

	int32 GetContainsPresetID(int64 itemUID);

	bool IsInCurrentItemPreset(int64 itemUID);
	bool IsEquippedCostume(const FB2Item& Item);
	bool IsInItemPreset(int64 itemUID);
	int32 GetContainedItemPresetNum(int64 itemUID);  // How many item presets this item belongs?
	bool IsInItemPreset(int32 presetID, int64 itemUID);
	bool IsInOtherItemPreset(int64 itemUID);

	void SwitchPresetItem(FB2Item* Item);
	void UpdateItemPreset(int32 presetID, EItemEquipPlace equipPlace, int64 itemUID);
	void RemoveCurrentItemPreset(int64 itemUID);
	void RemoveAllItemPreset(int64 itemUID);
	void SetCurrentItemPresetID(int32 presetID);
	void ResetEquipedInfoPart(EItemEquipPlace EquipPlace);
};

struct FPCItemContainer
{
	FPCItemContainer() : bSyncingConsumables(false) {}

	bool bSyncingConsumables; // Like bSyncingData of FPerCharacterItemContainer
	TArray<FPerCharacterItemContainer> CharacterItemContainers;
	TMap<int64, FB2Item> ConsumableItems; // Consumables are shared among PC classes.

	FB2Item* FindItem(const int64&);
	FB2Item* FindItem(const FB2ItemUID&);
	// Specific type of consumables cannot be identified by UID.. Multiple slot can be found depend on total amount.
	void FindConsumablesOfType(int32 InConsumableTypeRefID, TArray<FB2Item>& OutResults);

	int32 FindItemOfTypeCount(int32 InConsumableTypeRefID);

	bool RemoveItem(const FB2ItemUID&);
	bool RemoveItem(const int64&);
	void RemoveItems(const TArray<FB2Item>&);
	void RemoveItems(const TArray<int64>&);

	/** Decrease (only decrease) the total consuming number of specific type of consumable item, considering slot number too. */
	void DecSharedConsumableAmountOfType(int32 InRefID, int32 InDecNum);
	/** The opposite of DecSharedConsumableAmountOfType */
	void IncSharedConsumableAmountOfType(int32 InRefID, int32 InIncNum);
	
	void AddItem(const FB2Item&);
	void AddItems(const TArray<FB2Item>&);

	/** Use this one when you add any items that just acquired by any means like combat reward, buying, or anything else.
	* Not for adding items that current account already owns. */
	void AddNewAcquiredItems(const TArray<FB2Item>&);
	void AddNewAcquiredItem(FB2Item InItem);

	void ResetAllItems(EPCClass);
	void SwitchPresetItems(const TArray<int64>&, EPCClass, int32);
	void EquipItems(const TArray<int64>&, bool, int32);
	void UnEquipItemsPresets(const TArray<int64>&);
	void LockItems(const TArray<FB2Item>&, bool);
	void ChangeItem(const FB2Item&);
	void ChangeItems(const TArray<FB2Item>&);
	void MakeConsumablesDirty() { bSyncingConsumables = false; }
	void UpdateItemMasterDataOnMD();  // call after itemMasterData received

									  // for item preset
	void UpdateItemPresets(const TArray<FB2ItemPresetInfo>&);
	bool IsInCurrentItemPreset(EPCClass ePCClass, int64 itemUID);
	bool IsEquippedCostume(const FB2Item& Item);
	int32 GetCurrentItemPresetID(EPCClass ePCClass);
	void SetCurrentItemPresetID(EPCClass ePCClass, int32 presetID);
	bool IsInItemPresets(EPCClass ePCClass, int64 itemUID);
	bool IsInOtherItemPresets(EPCClass ePCClass, int64 itemUID);
	int32 GetContainedItemPresetNum(EPCClass ePCClass, int64 itemUID); // How many item presets this item belongs?
	int32 GetContainsItemPresetID(EPCClass ePCClass, int64 itemUID);
};

struct FPCClassOrNickName
{
	FPCClassOrNickName()
	{
	}

	FText ClassName; // Used when NickName is not set (empty)
	FText NickName; // It can be set or be empty

	FText GetClassOrNickName() const;
};

class FLocalCharacterData : public ICharacterDataStore
{
public:
	FLocalCharacterData();
	virtual ~FLocalCharacterData() {}; // This struct has virtual function so need virtual destructor.

	//Use only reference variable.
	FLocalCharacterData(FLocalCharacterData const&) = delete;
	FLocalCharacterData& operator=(FLocalCharacterData const&) = delete;

private:
	/** For all player character classes. int32 key is to be converted to EPCClass. */
	TArray<FB2PlayerSharedStats> PlayerSharedStats;

	/** Where actual client item data reside. Some of their client side management will be done by LobbyInventory. */
	FPCItemContainer UserAllItems;

	TArray<FPCWingContainer> AllWingData;
	TArray<bool> CacheShowPlayerHeads;
	
	TArray<b2network::B2GuildSkillPtr> GuildSkillInfos;

	EPCClass MainPlayerClass;
	EPCClass SubPlayerClass;

	FB2EtherContainer AllEthers;

	/** To manages character nickname (different from UserNickName) int32 key is to be converted to EPCClass. */
	TArray<FPCClassOrNickName> CharNames;
	TMap<int32, bool> PlayedUnlockFX;

	TMap<EPCClass, FAncientRelicArray> RelicInfos;
	TMap<EPCClass, FUnitySkillMissionArray> UnitySkillMissionInfos;
	TMap<EPCClass, FUnitySkillAwakenMissionArray> UnitySkillAwakenMissionInfos;
	TMap<EPCClass, FUnitySkillState> UnitySkillState;

	TMap<int64, FB2Totem> AllTotems;
	TMap<EPCClass, TMap<int64, FB2Totem>> EquippedTotems;

public:
	virtual int32	GetCharacterLevel(EPCClass) const;
	virtual FString	GetUserNickName() const;
	virtual void	GetCharacterRegistredSkills(EPCClass InCharacterClass, TArray<int32>& RegistredSkills, bool bIncludeUnitySkill = true) const;
	virtual bool	IsSkillRegistred(EPCClass InCharacterClass, int32 SkillId) const;
	virtual void	GetCharacterUnregistredSkills(EPCClass InCharacterClass, TArray<int32>& UnregistredSkills) const;
	void			GetCharacterSkillToLearnAtLevel(TArray<int32>& OutSkillIDs, EPCClass InCharClass, int32 InCharLevel) const; // Simple master data look-up
	virtual void	GetCharacterSkills(EPCClass InCharacterClass, TArray<FB2SkillInfo>& Skills) const;
	virtual void	GetCharacterSkills(EPCClass InCharacterClass, TArray<int32>& OutSkillIDs) const; // ID only
	virtual void	GetCharacterPassiveSkills(EPCClass InCharacterClass, TArray<int32>& SkillIds) const;
	virtual int32	GetCharacterSkillLevel(int32 SkillId) const;
	virtual void	GetCharacterSkillOptionsOfID(int32 InSkillId, TArray<FSkillOptionData>& OutFoundOptionData) const;
	virtual void	GetEquippedItems(EPCClass PCClass, TArray<FB2Item>& EquippedItems) const;
	virtual void	GetEquippedItems(EPCClass PCClass, TMap<EItemEquipPlace, FB2Item>& EquippedItems) const;
	virtual void	GetEquippedCostumeItems(EPCClass PCClass, TArray<FB2Item>& EquippedCostumeItems) const;
	virtual void	GetEquippedEthers(EPCClass PCClass, TMap<int64, FB2Ether>& EquippedEthers) const;
	virtual void	GetEquippedTotems(EPCClass PCClass, TMap<int64, FB2Totem>& OutEquippedTotems) const;

	virtual bool	IsLocalCharacterData() { return true; }
	virtual int32	GetUserLevel() const;

	void			GetRegistredTargetSlotSkills(EPCClass InCharacterClass, EItemPresetSlotType InSlotType, TArray<int32>& OutList) const;
	void			GetUnregistredTargetSlotSkills(EPCClass InCharacterClass, EItemPresetSlotType InSlotType, TArray<int32>& OutList) const;

	virtual EPCClass	GetMainPlayerClass() const;
	virtual EPCClass	GetSubPlayerClass() const;
	virtual void		SetMainPlayerClass(EPCClass);
	virtual void		SetSubPlayerClass(EPCClass);

	// You may optionally set FB2Item array to use custom equipment list.
	virtual void GetSetItemValue(EPCClass InPCClass, TMap<int32, int32>& OutSetItemDatas) override;
	virtual bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, TArray<FB2Item>* InItemArray = NULL) override;
	virtual float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) override;
	virtual float GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) override;
	virtual float GetCombatStatusValue(EPCClass InPCClass, TArray<FB2Item>* InItemArray = NULL) const override;

	float GetRequestCombatStatusValue(EPCClass InPCClass, TArray<b2network::B2CharacterPowerSpecificPtr>& OutCharacterPowerPtr);
	
	void SetEtherSetID(EPCClass InCharacterClass, int32 OffenseID, int32 DefenseID);
	void SetEtherDefenseSetID(EPCClass InCharacterClass, int32 DefenseID);

	int32 GetCharacterMaxLevel() const;
	int32 GetCharacterExp(EPCClass);
	int32 GetCharacterExpMax(EPCClass);
	float GetCharacterExpPercent(EPCClass); // 0 ~ 100

	int32 GetBeforeCharacterLevel(EPCClass);
	float GetBeforeCharacterExpPercent(EPCClass);

	void UpdateBeforeExpLevel(EPCClass);
	
	bool IsJustAddedSkill(int32 SkillId) const;
	void SetPlayedUnlockSkill(int32 SkillId);
	int32 GetCharacterLeftSkillPoint(EPCClass) const;
	void SetCharacterLeftSkillPoint(EPCClass InCharacterClass, int32 LeftPoint);
	int32 GetCharacterUsedSkillPoint(EPCClass) const;	
	
	int32 GetCharacterSkillLevelupGold(int32 SkillId);
	int32 GetCharacterSkillLevelupPoint(int32 SkillId, int32 SkillLevel);

	FORCEINLINE FPCItemContainer& GetUserAllItems() { return UserAllItems; }
	FORCEINLINE FB2EtherContainer& GetAllEthers() { return AllEthers; }

	void GetAllTotems(TMap<int64, FB2Totem>& OutAllTotems);
	int32 GetAllTotemsCount();
	bool GetEquuipedTotemByEquipSlot(EPCClass InClass, ETotemEquipSlot InTotemEquipSlot, FB2Totem& OutTotem);
	bool IsFullTotemInventory();
	bool IsOverTotemInventory();
	void SetAllTotemsNewFlagOff();

	void SetCharClassNames(class UB2PCClassInfoBox* InClassInfoBox); // Calling only once is enough, but after some engine initialization when PCClassInfo is available.
	void SetCharNickName(EPCClass InClass, const FText& InNewNickName); // Receive by FString, assuming this is to be used as network response.

	void SetCharBrevetInfo(EPCClass InClass, int32 RankIndex, int32 BrevetNodeIndex, bool BrevetNodeState);

	virtual int32 GetCharBrevetRank(EPCClass InCharacterClass) const;
	virtual int32 GetCharBrevetNodeIndex(EPCClass InClass) const;
	virtual int32 GetAppliedCharBrevetRank(EPCClass InCharacterClass) const;
	virtual int32 GetAppliedCharBrevetNodeIndex(EPCClass InClass) const;

	bool GetCharBrevetNodeState(EPCClass InClass) const;
	
	void SetRelicInfo(EPCClass nClass, FB2ResponseGetAccountRelicPtr msgPtr);
	void SetRelicInfo(B2RelicPtr RelicInfos);
	FAncientRelicArray* GetRelicInfo(EPCClass InClasss);

	void SetUnitySkillMissionInfo(EPCClass nClass, FB2ResponseGetUnitySkillMissionPtr msgPtr);
	void SetUnitySkillMissionInfo(TArray<B2UnitySkillMissionPtr> missions);
	void UpdateUnitySkillMissionInfo(const FB2ResponseUnitySkillRewardPtr RewardPtr);
	FUnitySkillMissionArray* GetUnitySkillMission(EPCClass InClasss);
	FUnitySkillAwakenMissionArray* GetUnitySkillAwakenMission(EPCClass InClasss);

	void SetUnitySkillAwakeSuccess(EPCClass InClass);
	
	FUnitySkillState* GetUnitySkillState(EPCClass InClass);
	void SetUnitySkillAwakenMission(EPCClass InClass, int32 nMissionID, bool bComplete, int32 ntry_count);
	void SetUnitySkillAwakenMission(TArray<b2network::B2UnitySkillAwakenMissionPtr> missions);
	
	virtual int32 GetEnhanceSetEffectLevel(EPCClass InPCClass) override;
	virtual double GetEnhanceSetEffectFactor(EPCClass InPCClass) override;

	virtual int64 GetAccountId() override;

	int32 GetEtherOffenseSetID(EPCClass InPCClass);
	int32 GetEtherDefenceSetID(EPCClass InPCClass);

	bool GetIsUnityAwaken(EPCClass InPCClass);
public:
	const FB2PlayerSharedStats*				GetPlayerStat(EPCClass InPCClass) const;
	FB2OriginCharacterInfoPtr				GetPlayerStatBasicInfo(EPCClass InPCClass) const;
	b2network::B2SkillQuickSlotPtr			GetPlayerStatBasicSkillInfo(EPCClass InPCClass, int32 SkillPresetIndex = 0)  const;
	const FPlayerExpLevelInfo*				GetBeforePlayerExpLevelInfo(EPCClass InPCClass) const;

	virtual FPCWingContainer* GetCharacterWingDataPtr(EPCClass InPCClass)  override;
	virtual bool DoesCharacterHaveWing(EPCClass InPCClass) const override;
	virtual bool GetCharacterWing(EPCClass InPCClass, FB2Wing& OutData) const override;

	virtual bool IsShowHeadItem(EPCClass InCharacterClass) const override;
	virtual bool IsShowCostume(EPCClass InCharacterClass, ECostumeEquipPlace InCostumeEquipPlace) const override;

	void SetCacheShowHeadItem(EPCClass InCharacterClass, bool ShowHead);
	void SetShowHeadItem(EPCClass InCharacterClass, bool ShowHead);
	void InitShowHeadItemCache();
	void GetSyncShowHeadItemClass(TArray<EPCClass>& SyncClassList);

	void InitShowCostumeCache();
	void GetSyncCostumeVisibleParts(TArray<TPair<EPCClass,ECostumeEquipPlace>>& SyncList);
	bool GetServerCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquipPlace);
	void SetServerCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquipPlace, bool bVisible);

	void SetUpdatePCClassSkillSlotID(EPCClass InPCClass, EItemPresetSlotType InSlotID);
	EItemPresetSlotType GetPCClassSkillSlotID(EPCClass InPCClass) const;

	EItemPresetSlotType GetPCClassItemPresetSlotID(EPCClass InPCClass);

	void SaveLocalData();
	void LoadLocalData();
	void LoadLocalData_ResourceBound();

	virtual int32	GetUnitySkillId(EPCClass InCharacterClass) const override;

	virtual void GetUnitySkillMissionArray(FUnitySkillMissionArray& UnitySkillMissionArray, EPCClass InCharacterClass);
	
	virtual TArray<b2network::B2GuildSkillPtr>& GetGuildSkillInfos() override;

	virtual const TArray<int32>& GetPreviewCostumeItems() const override;

protected:
	friend class FClientDataStore;
};

class FTutorialCharacterData : public FLocalCharacterData
{
public:
	FTutorialCharacterData() {}
	virtual ~FTutorialCharacterData() {}; // This struct has virtual function so need virtual destructor.

	//Use only reference variable.
	FTutorialCharacterData(FTutorialCharacterData const&) = delete;
	FTutorialCharacterData& operator=(FTutorialCharacterData const&) = delete;

public:
	virtual int32	GetCharacterLevel(EPCClass) const { return 1; }
	virtual bool	IsLocalCharacterData() { return false; }
	virtual int32	GetUserLevel() const { return 1; }

	virtual void	GetEquippedItems(EPCClass PCClass, TArray<FB2Item>& EquippedItems) const { EquippedItems.Empty(); }
	virtual void	GetEquippedItems(EPCClass PCClass, TMap<EItemEquipPlace, FB2Item>& EquippedItems) const { EquippedItems.Empty(); }
	virtual void	GetEquippedCostumeItems(EPCClass PCClass, TArray<FB2Item>& EquippedCostumeItems) const { EquippedCostumeItems.Empty(); }

	virtual void GetSetItemValue(EPCClass InPCClass, TMap<int32, int32>& OutSetItemDatas) override {}
	virtual bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, TArray<FB2Item>* InItemArray = NULL) override { return false; };
	virtual float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) override { return 0.0f; }
	virtual int32 GetCharBrevetNodeIndex(EPCClass InClass) const override { return 0; }
	virtual int32 GetAppliedCharBrevetRank(EPCClass InCharacterClass) const override { return 0; }
	virtual int32 GetAppliedCharBrevetNodeIndex(EPCClass InClass) const override { return 0; }
	virtual bool IsShowHeadItem(EPCClass InCharacterClass) const override { return true; }
	virtual bool IsShowCostume(EPCClass InCharacterClass, ECostumeEquipPlace InCostumeEquipPlace) const override { return true; }

	virtual FPCWingContainer* GetCharacterWingDataPtr(EPCClass InPCClass)  override;

	virtual const TArray<int32>& GetPreviewCostumeItems() const override;

protected:

	friend class FClientDataStore;
};
