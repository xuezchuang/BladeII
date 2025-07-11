#pragma once

#include "EngineMinimal.h"
#include "CommonStruct.h"
#include "../Common/CommonStructCombatOption.h"
//#include "FairyManager.h"
//#include "BladeIIPlayer.h"
//#include "CharacterDataStore.h"

namespace CombatStatEval
{
	//////////////////////////////////////////////////////////////////////////
	// The most basic utils to get various "raw" stats from character level.
	FORCEINLINE float GetMobBaseHealth(int32 InCharLevel)
	{
		// Could be different for player character
		return (80.f + 10.f * InCharLevel + 0.5f * FMath::Square(InCharLevel));
	}

	FORCEINLINE float GetMobBaseAttack(int32 InCharLevel)
	{
		// Could be different for player character
		return (80.f + 10.f * InCharLevel + 0.5f * FMath::Square(InCharLevel));
	}

	FORCEINLINE float GetMobBaseDefense(int32 InCharLevel)
	{
		// Could be different for player character
		return 3.f * GetMobBaseAttack(InCharLevel);
	}
	
	FORCEINLINE int GetPCBaseHealth(int32 InCharLevel)
	{
		return (80.f + 5.f * InCharLevel + 0.05f * FMath::Square(InCharLevel));
	}

	FORCEINLINE int GetPCBaseAttack(int32 InCharLevel)
	{
		return (6.f + InCharLevel + 0.05f * GetPCBaseHealth(InCharLevel));
	}

	FORCEINLINE int GetPCBaseDefense(int32 InCharLevel)
	{
		return 3.f * GetPCBaseAttack(InCharLevel);
	}
	
	// Convert raw TotalAttack/TotalDefense to some level scale.
	FORCEINLINE float GetADLevel(float ADValue, float CoefficientValue)
	{
		return FMath::Max(0.f, -10.f + FMath::Sqrt(100.f - 2.f * (80.f - (ADValue * CoefficientValue))));
	}

	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	//For player character's total combat stats considering level, equipments, and other stuff that affects combat ability.
	//For local player character's current stat, you just specifies InPCClass and leave other parameters as default.
	//For remote player character's current stat, you specifies InPCClass and CharacterData and leave other parameters as default.
	//SpecifiedLevel and SpecifiedEquipData are required for custom stat calculation (e.g. preview purpose)
	//float GetPCAttack(EPCClass InPCClass, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL, bool IsGuildBattle = false);
	//float GetPCDefense(EPCClass InPCClass, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL, bool IsGuildBattle = false);
	//float GetPCHealth(EPCClass InPCClass, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL, bool IsGuildBattle = false);
	//float GetPCCombatPower(EPCClass InPCClass, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL);
	////////////////////////////////////////////////////////////////////////////
	//// 모드별로 들어가는 공격, 방어, 헬스(ex 고대유물)
	//float GetPCAttackByMod(	EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL);
	//float GetPCDefenseByMod(EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL);
	//float GetPCHealthByMod(	EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData = NULL, int32 SpecifiedLevel = -1, TArray<FB2Item>* SpecifiedEquipData = NULL);
	//////////////////////////////////////////////////////////////////////////

	//float GetReuqestPCCombatPower(EPCClass InPCClass, TArray<b2network::B2CharacterPowerSpecificPtr>& OutCharacterPowerPtr);

	float GetDefaultCombatCoefficient();
	float GetDefaultPVEProportionalConstant();
	float GetDefaultPVPMaxDamageRatio();
	float GetDefaultPVPDamageScale();


	//bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Ether>& Ethers, CombatStatInfoRawDatas& OutRawValues, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems);
	//bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, ICharacterDataStore* InCharacterData = NULL);
	//bool GetSkillOptionStatusRawValues(EPCClass InPCClass, ESkillOption InOptionType, CombatStatInfoRawDatas& OutRawValues, ICharacterDataStore* InCharacterData = NULL);
	//bool GetUnitedOptionStatusRawValues(EPCClass InPCClass, EUnitedCombatOptions InOptionType, CombatStatInfoRawDatas& OutRawValues, ICharacterDataStore* InCharacterData = NULL);
	////모드 별로 들어 가는 것을 계산 하기 위한 함수. 현재는 고대유물만 구현되어 있음
	//float GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Totem>& Totems);
	//bool GetOptionStatusRawValuesByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Totem>& Totems, CombatStatInfoRawDatas& OutRawValues);

	//길드 버프
	//bool IsActiveGuildBuff(ICharacterDataStore* InCharacterDataStore, EItemOption InItemOption);
	//float GetPCGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle, EItemOption InItemOption);
	//float GetPCAttackGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle = false);
	//float GetPCDefendseGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle = false);
	//float GetPCHalthGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle = false);

	/** Get applied option value of all character data that can be identified by EItemOption. Data here (Item, Wing, Relic,..) can be acquired from CharacterDataStore. */
	//float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems);
	//float GetCombatStatusValue(EPCClass InPCClass, const TArray<FB2Item>& InEquipmentItem, const TArray<FB2Item>& InEquipmentCostume, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const FUnitySkillMissionArray& UnityInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems);
	//
	//float GetRequestCombatStatusValue(EPCClass InPCClass, const TArray<FB2Item>& InEquipmentItem, const TArray<FB2Item>& InEquipmentCostume, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const FUnitySkillMissionArray& UnityInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems, TArray<b2network::B2CharacterPowerSpecificPtr>& OutCharacterPowerPtr);

	/** Simply replaces data source arguments of GetOptionStatusValue above to CharacterDataStore */
	static float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, ICharacterDataStore* InCharacterData = NULL);

	/** Get applied SkillOption value of designated CharacterDataStore (LocalCharacterData if NULL) Info. 
	 * You gets only passive style skill option values, not options bound to specific active skill. */
	float GetSkillOptionStatusValue(EPCClass InPCClass, ESkillOption InOptionType, ICharacterDataStore* InCharacterData = NULL);

	////////////////////////////////////////////////////////////////////////////////
	////////// The master, united, joint, ultimate, whatever, option status value querying function.
	/** Get applied option value of master united type. It includes option values of EItemOption and ESkillOption (of passive skills only)
	 * Like others here, LocalCharacterData is used if InCharacterData is NULL. */
	float GetUnitedOptionStatusValue(EPCClass InPCClass, EUnitedCombatOptions InUnitedOptionType, ICharacterDataStore* InCharacterData = NULL);
	////////////////////////////////////////////////////////////////////////////////

	 //UnitedCombatOption mapping information look-up interfaces.
	ESkillOption GetSkillOptionOfUnitedOption(EUnitedCombatOptions InUCO);
	EItemOption GetItemOptionOfUnitedOption(EUnitedCombatOptions InUCO);
	EUnitedCombatOptions GetUnitedOptionOfItemOption(EItemOption InUCO);
	EUnitedCombatOptions GetUnitedOptionOfSkillOption(ESkillOption InUCO);
	EUnitedCombatOptions GetUnitedOptionOfMercenarySkillOption(EMercenarySkillOption InUCO);
	void InitializeUnitedOptionMapping(); // It must be called prior to use any UnitedCombatOption functionality.
	
	// Set Item 
	void GetSetItemValue(const TArray<FB2Item>& AllEquipped, TMap<int32, int32>& OutSetItemDatas);
	void GetSetItemValue_Version2(const TArray<FB2Item>& AllEquipped, TMap<int32, int32>& OutSetItemDatas);
	void GetCostumesSetItemValue(const TArray<FB2Item>& AllEquipped, TMap<int32, int32>& OutSetItemDatas);	// OutSetItemDatas : Key (SetItemId) / Value (Count)
	void GetSetItemValueByInvenType(const TArray<FB2Item>& AllEquipped, EItemInvenType CheckInventoryType, TMap<int32, TArray<int32>>& OutGroupIDDatas);	

	//void ApplySetItemValueByInvenType(FClientDataStore& ClientDataStore, TMap<int32, int32> ItemSetMap, EItemOption OptionType,
	//	CombatStatInfoRawDatas& OutCombatStatInfo, TMap<int32, TArray<int32>> GroupIDDatas);
	//void ApplyCostumeSetItemValue(FClientDataStore& ClientDataStore, TMap<int32, int32> SetItemDatas, EItemOption OptionType,
	//	CombatStatInfoRawDatas& OutCombatStatInfo);

	// Enhance Set Item
	int32 GetEnhanceSetEffectLevel(const TArray<FB2Item>& AllEquipped);
	double GetEnhanceSetEffectFactor(const TArray<FB2Item>& AllEquipped);

	bool IsPrimPointTypeByItemOption(EItemOption InCheckType);
	EItemOption GetPrimPointIncOption(EItemPrimaryPointType InCheckType);
	EItemPrimaryPointType ConvertItemOptionToItemPrimaryPointType(EItemOption InCheckType);
	EItemOption ConvertItemPrimaryPointTypeToItemOptionByPVPMode(EItemPrimaryPointType InCheckType, EB2GameMode ModeType);
	// 토템의 경우 가능 옵션이 PVP 외에도(ex> 영웅의탑 등) 가능하기에 별도의 체크 과정을 거친다.
	EItemOption ConvertItemPrimaryPointTypeToItemOptionByTotemMode(EItemPrimaryPointType InCheckType, EB2GameMode ModeType);

	void InitCombatStatMap(TMap<EUnitedCombatOptions, float>& InMap);
}