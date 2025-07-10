#pragma once
//#include "BladeII.h"
#include "CharacterDataStore.h"

struct FB2ModCharInfo
{
	EPCClass CharClass;
	b2network::B2CharacterServerInfoPtr CharInfo;
	TArray<FB2Item> EquippedItems;
	TArray<FB2Item> EquippedCostumeItems;
	FPCWingContainer WingContainer;
	FAncientRelicArray RelicInfos;
	TMap<int64, FB2Ether> EquippedEthers;
	FUnitySkillMissionArray UnityInfos;
	TMap<int64, FB2Totem> EquippedTotems;
};

class FB2ModPlayerInfo : public ICharacterDataStore
{
public:
	FB2ModPlayerInfo();
	virtual ~FB2ModPlayerInfo();

	FB2ModPlayerInfo(b2network::B2FindAccountInfoPtr InPlayerInfo);
	//Use only reference variable.
	FB2ModPlayerInfo& operator=(b2network::B2FindAccountInfoPtr InPlayerInfo);

	void					SetModPlayerInfo(const FB2FindAccountInfoPtr& InPlayerInfo);
	void					AddModCharInfo(const b2network::B2CharacterServerInfoPtr& InCharInfo, const TArray<FB2Item>& InEquippedItems);

	const FB2FindAccountInfoPtr GetAccountInfoPtr();
	FORCEINLINE void		SetNickName(const FString& InNickName){ NickName = InNickName; }
		
	virtual int32			GetCharacterLevel(EPCClass) const;
	virtual FString			GetUserNickName() const { return NickName; }
	virtual void			GetCharacterRegistredSkills(EPCClass InCharacterClass, TArray<int32>& RegistredSkills, bool bIncludeUnitySkill = true) const;
	virtual bool			IsSkillRegistred(EPCClass InCharacterClass, int32 SkillId) const;
	virtual void			GetCharacterUnregistredSkills(EPCClass InCharacterClass, TArray<int32>& UnregistredSkills) const;
	virtual void			GetCharacterSkills(EPCClass InCharacterClass, TArray<FB2SkillInfo>& Skills) const;
	virtual void			GetCharacterSkills(EPCClass InCharacterClass, TArray<int32>& OutSkillIDs) const; // ID only
	virtual void			GetCharacterPassiveSkills(EPCClass InCharacterClass, TArray<int32>& SkillIds) const;
	virtual int32			GetCharacterSkillLevel(int32 SkillId) const;
	virtual void			GetCharacterSkillOptionsOfID(int32 InSkillId, TArray<FSkillOptionData>& OutFoundOptionData) const;

	virtual void			GetEquippedItems(EPCClass PCClass, TArray<FB2Item>& EquippedItems) const;
	virtual void			GetEquippedItems(EPCClass PCClass, TMap<EItemEquipPlace, FB2Item>& EquippedItems) const;
	virtual void			GetEquippedCostumeItems(EPCClass PCClass, TArray<FB2Item>& EquippedCostumeItems) const;
	virtual void			GetEquippedEthers(EPCClass PCClass, TMap<int64, FB2Ether>& EquippedEthers) const;
	virtual void			GetEquippedTotems(EPCClass PCClass, TMap<int64, FB2Totem>& EquippedTotems) const;
	virtual bool			IsLocalCharacterData() { return false; }
	virtual int32			GetUserLevel() const;

	virtual EPCClass		GetMainPlayerClass() const;
	virtual EPCClass		GetSubPlayerClass() const;
	virtual void			SetMainPlayerClass(EPCClass);
	virtual void			SetSubPlayerClass(EPCClass);

	virtual void			ConstructEquipItems();
	virtual void			SetCharacterEquipItems(FB2ModCharInfo& CharacterInfo);

	virtual void			ConstructEquipCostumeItems();
	virtual void			SetCharacterEquipCostumeItems(FB2ModCharInfo& CharacterInfo);

	virtual void			ConstructEquipTotems();
	virtual void			SetCharacterEquipTotems(FB2ModCharInfo& CharacterInfo);
	
	void					GetEquipItemsByPreset(const EPCClass TargetClass, TArray<int32>& EquipItemIDs);
	int32					GetClientPresetID(EInventoryPresetMode PresetMode, EPCClass TargetClass) const;

	virtual bool DoesCharacterHaveWing(EPCClass InCharacterClass) const override;
	// OutData is valid only when it returns true
	virtual bool GetCharacterWing(EPCClass InCharacterClass, FB2Wing& OutData) const override;

	virtual bool IsShowHeadItem(EPCClass InCharacterClass) const override;
	virtual bool IsShowCostume(EPCClass InCharacterClass, ECostumeEquipPlace InCostumeEquipPlace) const override; 
	
	virtual FPCWingContainer* GetCharacterWingDataPtr(EPCClass InPCClass) override;

	virtual void GetSetItemValue(EPCClass InPCClass, TMap<int32, int32>& OutSetItemDatas) override;
	virtual bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, TArray<FB2Item>* InItemArray = NULL) override;
	virtual float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) override;
	virtual float GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, TArray<FB2Item>* InItemArray = NULL) override;
	virtual float GetCombatStatusValue(EPCClass InPCClass, TArray<FB2Item>* InItemArray = NULL) const override;

	virtual int32 GetCharBrevetRank(EPCClass InCharacterClass) const;
	virtual int32 GetCharBrevetNodeIndex(EPCClass InClass) const override;

	virtual int32 GetAppliedCharBrevetRank(EPCClass InCharacterClass) const override;
	virtual int32 GetAppliedCharBrevetNodeIndex(EPCClass InClass) const override;

	virtual int32 GetEnhanceSetEffectLevel(EPCClass InPCClass) override;
	virtual double GetEnhanceSetEffectFactor(EPCClass InPCClass) override;

	int32 GetCharacterExp(EPCClass InClass);
	int32 GetCharacterExpMax(EPCClass InClass);
	float GetCharacterExpPercent(EPCClass InClass);

	void SetAccountId(int64 id);
	virtual int64 GetAccountId() override;
	virtual int32 GetEtherOffenseSetID(EPCClass InPCClass) override;
	virtual int32 GetEtherDefenceSetID(EPCClass InPCClass) override;

	virtual int32	GetUnitySkillId(EPCClass InCharacterClass) const override;

	virtual void GetUnitySkillMissionArray(FUnitySkillMissionArray& UnitySkillMissionArray, EPCClass InCharacterClass);
	virtual TArray<b2network::B2GuildSkillPtr>& GetGuildSkillInfos() override;

	virtual const TArray<int32>& GetPreviewCostumeItems() const override;

private:
	int32					GetIndex(EPCClass CharClass) const;

	int64 AccountId;

	FString NickName;
	FB2FindAccountInfoPtr	ServerAccountInfo;
	TArray<FB2ModCharInfo>	Characters;

	TArray<b2network::B2GuildSkillPtr> GuildSkillInfos;

	TArray<struct FB2FairyStatusInfo> FairyStatus;

	EPCClass MainPlayerClass;
	EPCClass SubPlayerClass;
};
