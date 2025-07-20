
#include "B2CombatStatEvaluator.h"
#include "../Common/CommonStructCombatOption.h"
//#include "BladeII.h"
#include "B2LobbyInventory.h"
#include "B2Airport.h"
#include "Algo/Copy.h"
#include "RelicManager.h"
#include "B2UIManager.h"
#include "BladeIIGameImpl.h"
#include "B2EtherManager.h"
#include "BladeIIUtil.h"


namespace CombatStatEval // 傈捧仿 螟沥 臂肺国 蜡瓶
{
	/**
	 * ICharacterDataStore 捞侩秦辑 眉仿 胶泡 掘绢咳.
	 * CharacterData 绰 府葛飘 敲饭捞绢 侩栏肺 狼档茄 巴栏肺 疙矫窍瘤 臼栏搁 肺拿 某腐磐 沥焊甫 荤侩茄促.
	 * SpecifiedLevel 苞 SpecifiedEquipData 甫 扁夯蔼捞 酒囱 烙狼狼 蔼阑 林搁 弊吧 荤侩茄促. 漂沥 饭骇捞唱 厘厚 炼钦狼 橇府轰 殿狼 侩档.
	 */
	float GetPCHealth(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/, bool IsGuildBattle)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 瘤沥窍瘤 臼阑 版快 肺拿 某腐磐 沥焊肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 焊促 农搁 饭骇蔼阑 流立 瘤沥茄 吧肺 荤侩

		fStatusValue += GetPCBaseHealth(FinalUsedLevel);
		fStatusValue += CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Health_IncHealth, SpecifiedEquipData);

		// SkillOption 俊绰 扁夯 傍/规/眉 歹窍扁绰 绝瘤父 漂喊洒 Health 甫 欺季飘肺 棵府绰 可记捞 乐促.
		const float ExtraIncMaxHPRate = GetUnitedOptionStatusValue(InPCClass, EUnitedCombatOptions::UCO_Health_IncMaxHP, CharacterData);
		fStatusValue *= (1.0f + ExtraIncMaxHPRate);

		fStatusValue = fStatusValue + (fStatusValue * (GetPCHalthGuildBuffValue(CharacterData, IsGuildBattle) * 0.01));		//辨靛 滚橇

		return fStatusValue;
	}


	/**
	 * ICharacterDataStore 捞侩秦辑 傍拜 胶泡 掘绢咳.
	 * CharacterData 绰 府葛飘 敲饭捞绢 侩栏肺 狼档茄 巴栏肺 疙矫窍瘤 臼栏搁 肺拿 某腐磐 沥焊甫 荤侩茄促.
	 * SpecifiedLevel 苞 SpecifiedEquipData 甫 扁夯蔼捞 酒囱 烙狼狼 蔼阑 林搁 弊吧 荤侩茄促. 漂沥 饭骇捞唱 厘厚 炼钦狼 橇府轰 殿狼 侩档.
	 */
	float GetPCAttack(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/, bool IsGuildBattle)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 瘤沥窍瘤 臼阑 版快 肺拿 某腐磐 沥焊肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 焊促 农搁 饭骇蔼阑 流立 瘤沥茄 吧肺 荤侩

		fStatusValue += GetPCBaseAttack(FinalUsedLevel);
		fStatusValue += CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Offense_IncAttack, SpecifiedEquipData);

		fStatusValue = fStatusValue + (fStatusValue * (GetPCAttackGuildBuffValue(CharacterData, IsGuildBattle) * 0.01));		//辨靛 滚橇
		return fStatusValue;
	}


	/**
	 * ICharacterDataStore 捞侩秦辑 规绢 胶泡 掘绢咳.
	 * CharacterData 绰 府葛飘 敲饭捞绢 侩栏肺 狼档茄 巴栏肺 疙矫窍瘤 臼栏搁 肺拿 某腐磐 沥焊甫 荤侩茄促.
	 * SpecifiedLevel 苞 SpecifiedEquipData 甫 扁夯蔼捞 酒囱 烙狼狼 蔼阑 林搁 弊吧 荤侩茄促. 漂沥 饭骇捞唱 厘厚 炼钦狼 橇府轰 殿狼 侩档.
	 */
	float GetPCDefense(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/, bool IsGuildBattle)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 瘤沥窍瘤 臼阑 版快 肺拿 某腐磐 沥焊肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 焊促 农搁 饭骇蔼阑 流立 瘤沥茄 吧肺 荤侩

		fStatusValue += GetPCBaseDefense(FinalUsedLevel);
		fStatusValue += CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Defense_IncDefense, SpecifiedEquipData);

		fStatusValue = fStatusValue + (fStatusValue * (GetPCDefendseGuildBuffValue(CharacterData, IsGuildBattle) * 0.01));		//辨靛 滚橇
		return fStatusValue;
	}



	/**
	 * ICharacterDataStore 捞侩秦辑 某腐磐 辆钦 傈捧仿阑 掘绢咳.
	 * 捞扒 角力 傈捧俊 荤侩登绰 巴篮 酒聪绊 老辆狼 辆钦 乞啊 荐摹狼 狼固肺 荤侩登绰 巴捞促. 蔼捞 臭阑荐废 傈捧俊辑 蜡侩窍变 窍摆瘤.
	 * CharacterData 绰 府葛飘 敲饭捞绢 侩栏肺 狼档茄 巴栏肺 疙矫窍瘤 臼栏搁 肺拿 某腐磐 沥焊甫 荤侩茄促.
	 * SpecifiedLevel 苞 SpecifiedEquipData 甫 扁夯蔼捞 酒囱 烙狼狼 蔼阑 林搁 弊吧 荤侩茄促. 漂沥 饭骇捞唱 厘厚 炼钦狼 橇府轰 殿狼 侩档.
	 */
	float GetPCCombatPower(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		float fStatusValue = 0.0f;

		if (!CharacterData) // CharacterData 甫 瘤沥 救沁栏搁 肺拿 敲饭捞绢 某腐磐 铂肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 焊促 农搁 饭骇蔼阑 流立 瘤沥茄 吧肺 荤侩

		auto& ClientData = BladeIIGameImpl::GetClientDataStore();

		// 扁夯 某腐磐 傍规眉 傈捧仿
		float AttackPower = (static_cast<float>(GetPCBaseAttack(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack));
		float DefensePower = (static_cast<float>(GetPCBaseDefense(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense));
		float HealthPower = (static_cast<float>(GetPCBaseHealth(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth));

		fStatusValue += AttackPower;
		fStatusValue += DefensePower;
		fStatusValue += HealthPower;

		// 厘厚,朝俺 殿殿栏肺 眠啊等 傈捧仿
		fStatusValue += CharacterData->GetCombatStatusValue(InPCClass, SpecifiedEquipData);

		// 辨靛滚橇肺 眠啊等 傈捧仿
		// 醚 傍规眉 * 辨靛滚橇欺季飘 * 傈捧仿 啊吝摹 
		float TotalAtt = static_cast<float>(GetPCBaseAttack(FinalUsedLevel)) + CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Offense_IncAttack, SpecifiedEquipData);
		float TotalDef = static_cast<float>(GetPCBaseDefense(FinalUsedLevel)) + CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Defense_IncDefense, SpecifiedEquipData);
		float TotalHP = static_cast<float>(GetPCBaseHealth(FinalUsedLevel)) + CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Health_IncHealth, SpecifiedEquipData);

		float GuildIncreaseAtt = TotalAtt * (GetPCAttackGuildBuffValue(CharacterData) * 0.01) * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		float GuildIncreaseDef = TotalDef * (GetPCDefendseGuildBuffValue(CharacterData) * 0.01) * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		float GuildIncreaseHP = TotalHP * (GetPCHalthGuildBuffValue(CharacterData) * 0.01) * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);

		float fGuildBuffCombatPower = 0;
		fGuildBuffCombatPower += GuildIncreaseAtt;
		fGuildBuffCombatPower += GuildIncreaseDef;
		fGuildBuffCombatPower += GuildIncreaseHP;

		fStatusValue += fGuildBuffCombatPower;

		return fStatusValue;
	}


	float GetPCAttackByMod(EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 瘤沥窍瘤 臼阑 版快 肺拿 某腐磐 沥焊肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		//Temp Value 瘤陛篮 绊措蜡拱 观俊 救荤侩 窍扁 锭巩. 鞘夸窍搁 龋免 困摹甫(CharacterData->GetOptionStatusValue) 函版窍咯 荤侩窍矫搁 凳.
		fStatusValue = CharacterData->GetOptionStatusValueByMod(InPCClass, ModeType, EItemOption::EIO_Offense_IncAttack);

		return fStatusValue;
	}

	float GetPCDefenseByMod(EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 瘤沥窍瘤 臼阑 版快 肺拿 某腐磐 沥焊肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		//Temp Value 瘤陛篮 绊措蜡拱 观俊 救荤侩 窍扁 锭巩. 鞘夸窍搁 龋免 困摹甫(CharacterData->GetOptionStatusValue) 函版窍咯 荤侩窍矫搁 凳.
		fStatusValue = CharacterData->GetOptionStatusValueByMod(InPCClass, ModeType, EItemOption::EIO_Defense_IncDefense);

		return fStatusValue;
	}

	float GetPCHealthByMod(EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 瘤沥窍瘤 臼阑 版快 肺拿 某腐磐 沥焊肺.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		//Temp Value 瘤陛篮 绊措蜡拱 观俊 救荤侩 窍扁 锭巩. 鞘夸窍搁 龋免 困摹甫(CharacterData->GetOptionStatusValue) 函版窍咯 荤侩窍矫搁 凳.
		fStatusValue = CharacterData->GetOptionStatusValueByMod(InPCClass, ModeType, EItemOption::EIO_Health_IncHealth);

		return fStatusValue;
	}

	//////////////////////////////////////////////////////////////////////////
	float GetReuqestPCCombatPower(EPCClass InPCClass, TArray<b2network::B2CharacterPowerSpecificPtr>& OutCharacterPowerPtr)
	{
		float fStatusValue = 0.0f;

		auto& ClientData = BladeIIGameImpl::GetClientDataStore();
		auto& CharacterData = BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = CharacterData.GetCharacterLevel(InPCClass);

		// 扁夯 某腐磐 傍规眉 傈捧仿
		float AttackPower = (static_cast<float>(GetPCBaseAttack(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack));
		float DefensePower = (static_cast<float>(GetPCBaseDefense(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense));
		float HealthPower = (static_cast<float>(GetPCBaseHealth(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth));

		fStatusValue += AttackPower;
		fStatusValue += DefensePower;
		fStatusValue += HealthPower;

		float fBaseCombatPower = AttackPower + DefensePower + HealthPower;

		/* STAT_POWER */
		auto StatPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		StatPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::STAT_POWER);
		StatPowerSpecific->power = fBaseCombatPower;
		OutCharacterPowerPtr.Add(StatPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Base) : %0.2f"), fBaseCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 厘厚,朝俺 殿殿栏肺 眠啊等 傈捧仿
		fStatusValue += CharacterData.GetRequestCombatStatusValue(InPCClass, OutCharacterPowerPtr);

		/* TOTAL_POWER */
		auto TotalPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		TotalPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::TOTAL_POWER);
		TotalPowerSpecific->power = fStatusValue;
		OutCharacterPowerPtr.Add(TotalPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("Total CombatPower : %0.2f"), fStatusValue), FLinearColor(0.0, 0.5f, 0.5f, 1.0f), 15, 10.0f);

		// 辨靛滚橇肺 眠啊等 傈捧仿
		// 醚 傍规眉 * 辨靛滚橇欺季飘 * 傈捧仿 啊吝摹 
		float TotalAtt = static_cast<float>(GetPCBaseAttack(FinalUsedLevel)) + CharacterData.GetOptionStatusValue(InPCClass, EItemOption::EIO_Offense_IncAttack);
		float TotalDef = static_cast<float>(GetPCBaseDefense(FinalUsedLevel)) + CharacterData.GetOptionStatusValue(InPCClass, EItemOption::EIO_Defense_IncDefense);
		float TotalHP = static_cast<float>(GetPCBaseHealth(FinalUsedLevel)) + CharacterData.GetOptionStatusValue(InPCClass, EItemOption::EIO_Health_IncHealth);

		float GuildIncreaseAtt = TotalAtt * (GetPCAttackGuildBuffValue(&CharacterData) * 0.01) * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		float GuildIncreaseDef = TotalDef * (GetPCDefendseGuildBuffValue(&CharacterData) * 0.01) * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		float GuildIncreaseHP = TotalHP * (GetPCHalthGuildBuffValue(&CharacterData) * 0.01) * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);

		float fGuildBuffCombatPower = 0;
		fGuildBuffCombatPower += GuildIncreaseAtt;
		fGuildBuffCombatPower += GuildIncreaseDef;
		fGuildBuffCombatPower += GuildIncreaseHP;

		fStatusValue += fGuildBuffCombatPower;

		return fStatusValue;
	}
	//////////////////////////////////////////////////////////////////////////


	float GetDefaultCombatCoefficient()
	{
		return 1.0f;
	}

	float GetDefaultPVEProportionalConstant()
	{
		return 0.2f;
	}

	float GetDefaultPVPMaxDamageRatio()
	{
		return 0.2f;
	}

	float GetDefaultPVPDamageScale()
	{
		return 0.05f;
	}

	//	float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems)
	//	{
	//		CombatStatInfoRawDatas CombatRawDatas;
	//		GetOptionStatusRawValues(InPCClass, OptionType, AllEquipped, InWingContainer, BrevetRank, BrevetNodeNum, RelicInfos, Ethers, CombatRawDatas, FairyStatus, Totems);
	//
	//		TArray<FOptionValue>	PrimaryPointRawValues;
	//		CombatRawDatas.GetAllRawValues(PrimaryPointRawValues);
	//
	//		double EnhanceIncreaseFactor = (ConvertItemOptionToItemPrimaryPointType(OptionType) != EItemPrimaryPointType::EIPP_End ? GetEnhanceSetEffectFactor(AllEquipped) : 0.f);
	//		// 碍拳 酒捞袍 技飘 瓤苞甫 绊妨茄 GetEffectiveOptionValue.. 滚傈栏肺.. 
	//		float ResultOptionValue = GetEffectiveOptionValueWithEnhanceSetEffectTempl<EItemOption>(OptionType, PrimaryPointRawValues, EnhanceIncreaseFactor, true);
	//
	//		// 傍规眉 可记狼 版快 某腐磐 扁夯 可记苞 钦媚瘤扁 傈俊 炼沥 苞沥阑 芭模促.
	//		if (IsPrimPointTypeByItemOption(OptionType))
	//		{
	//			ResultOptionValue = FMath::Max(0.0f, ResultOptionValue);
	//		}
	//
	//		return ResultOptionValue;
	//	}
	////
	bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum,
		const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Ether>& Ethers, CombatStatInfoRawDatas& OutCombatStatInfo, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems)
	{
		//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
		//EItemPrimaryPointType CheckPrimaryPointType = ConvertItemOptionToItemPrimaryPointType(OptionType);

		//// 酒捞袍 可记
		//for (auto& Item : AllEquipped)
		//{
		//	// 扁夯瓷仿摹
		//	if (Item.PrimaryPointType == CheckPrimaryPointType)
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_NormalPoint, Item.GetPrimaryPoint());

		//	// 绊蜡可记
		//	for (auto& CurrOption : Item.IntrinsicOptions)
		//	{
		//		if (CurrOption.OptionType == OptionType)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_IntrinsicOptions, CurrOption.RawOptionAmount);
		//	}

		//	// 罚待(老馆)可记
		//	for (auto& CurrOption : Item.RandomOptions)
		//	{
		//		if (CurrOption.OptionType == OptionType)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RandomOptions, CurrOption.RawOptionAmount);
		//	}

		//	//阿己 可记
		//	for (auto& CurrOption : Item.SealOptions)
		//	{
		//		if(CurrOption.OptionInfo.OptionType == OptionType)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_SealOption, CurrOption.OptionInfo.RawOptionAmount);
		//	}
		//}

		//// 朝俺 可记
		//if (InWingContainer.bHasWing)
		//{
		//	const FWingPropOption* ThisPropOption = InWingContainer.WingData.GetPropOptionPtr(OptionType);
		//	if (ThisPropOption && ThisPropOption->bIsOpen)
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_WingAdditionalOption, ThisPropOption->RawOptionAmount);

		//	// 扁夯瓷仿摹
		//	switch (CheckPrimaryPointType)
		//	{
		//	case EItemPrimaryPointType::EIPP_Attack:
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_WingNormalOption, InWingContainer.WingData.GetAttackPoint());
		//		break;
		//	case EItemPrimaryPointType::EIPP_Defense:
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_WingNormalOption, InWingContainer.WingData.GetDefensePoint());
		//		break;
		//	case EItemPrimaryPointType::EIPP_Health:
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_WingNormalOption, InWingContainer.WingData.GetHealthPoint());
		//		break;
		//	default:
		//		break;
		//	}
		//}

		//// 柳鞭 可记(傍 规 眉)
		//if (BrevetRank != 0)
		//{
		//	int32 nAddStatus = ClientDataStore.GetBrevetNodeTotalStatus(OptionType, BrevetRank, BrevetNodeNum);
		//	OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RankPromotion, float(nAddStatus));
		//}
		///////////////////////////////////////////////////////////////////////////////////咯扁焙//////////////////////////
		//// 酒捞袍 技飘瓤苞
		//TMap<int32, int32>	ItemSetMap;
		//TMap<int32, TArray<int32>>	EquipmentGroupIDMap;
		//TMap<int32, TArray<int32>>	AccessoriesGroupIDMap;
		//TMap<int32, int32>	CostumeGroupIDMap;
		//GetSetItemValue(AllEquipped, ItemSetMap);
		//GetSetItemValueByInvenType(AllEquipped, EItemInvenType::EIIVT_Protection, EquipmentGroupIDMap);
		//GetSetItemValueByInvenType(AllEquipped, EItemInvenType::EIIVT_Accessory, AccessoriesGroupIDMap);
		//GetCostumesSetItemValue(AllEquipped, CostumeGroupIDMap );// 泅犁 内胶片廊 规绢备 公扁啊 技飘肺 累悼窃
		//
		//ApplySetItemValueByInvenType(ClientDataStore,ItemSetMap, OptionType, OutCombatStatInfo, EquipmentGroupIDMap);
		//ApplySetItemValueByInvenType(ClientDataStore, ItemSetMap, OptionType, OutCombatStatInfo, AccessoriesGroupIDMap);
		//ApplyCostumeSetItemValue(ClientDataStore, CostumeGroupIDMap,OptionType,OutCombatStatInfo);

		//// 其绢府
		//for (const FB2FairyStatusInfo& Status : FairyStatus)
		//{
		//	EItemOption BlessOption = SvrToCliOptionType(Status.ActivedOptoinId);
		//	if (BlessOption != OptionType)
		//		continue;

		//	FMD_FairyInfo OutData;
		//	if (FairyManager::GetInstance().GetFairyOpendBlessInfo(Status.FairyType, Status.ActivedOptoinId, Status.Level, OutData))
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_FairyOption, OutData.OptionValue);
		//}

		//// 绊措蜡拱
		//RelicManager RelicMGR(GLOBALRELICMANAGER);
		//// if (&RelicMGR) //瘤开俊辑 static栏肺 教臂沛 积己窍绰芭扼 公炼扒 脚汾 秦具 且淀...
		//{
		//	const FMD_AncientRelicGradeElem* gradeInfo;
		//	int32 nRelicId, nGrade;

		//	for (FAncientRelic RelicInfo : RelicInfos)
		//	{
		//		nRelicId = RelicInfo.nRelicId;	nGrade = RelicInfo.nRelicGrade;

		//		gradeInfo = RelicMGR.getRelicGradeInfo(nRelicId, nGrade);

		//		if (gradeInfo)
		//		{
		//			// 扁夯瓷仿摹
		//			switch (CheckPrimaryPointType)
		//			{
		//			case EItemPrimaryPointType::EIPP_Attack:
		//				OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RelicOption, gradeInfo->nOption_Attack.RawOptionAmount);
		//				break;
		//			case EItemPrimaryPointType::EIPP_Defense:
		//				OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RelicOption, gradeInfo->nOption_Defence.RawOptionAmount);
		//				break;
		//			case EItemPrimaryPointType::EIPP_Health:
		//				OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RelicOption, gradeInfo->nOption_Health.RawOptionAmount);
		//				break;
		//			default:
		//				break;
		//			}
		//		}
		//	}
		//}

		//// 俊抛福
		//for (const TPair<int64, FB2Ether>& Elem : Ethers)
		//{
		//	if (Elem.Value.MainOption.OptionType == OptionType)
		//	{
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_EtherOption, Elem.Value.MainOption.RawOptionAmount);
		//	}

		//	for (const FItemOption& ElemOption : Elem.Value.SubOption)
		//	{
		//		if (ElemOption.OptionType == OptionType)
		//		{
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_EtherOption, ElemOption.RawOptionAmount);
		//		}
		//	}
		//}

		//// 配袍
		//for (const TPair<int64, FB2Totem>& Elem : Totems)
		//{
		//	// 厘厚 傍拜仿, 规绢仿, 眉仿
		//	if (Elem.Value.MainOptionDetail.OptionType == OptionType)
		//	{
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_NormalPoint, Elem.Value.MainOptionDetail.RawOptionAmount);
		//	}

		//	// 厘厚 眠啊 可记
		//	for (int SubOptionIndex = 0; SubOptionIndex < Elem.Value.SubOptionDetails.Num(); ++SubOptionIndex)
		//	{
		//		if (Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.OptionType == OptionType)
		//		{
		//			// 皑家 可记捞 啊瓷茄 何盒
		//			EOptionVariation OptionVariation = Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.OptionVariationType;
		//			float OptionAmount = Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.RawOptionAmount;
		//			FOptionValue OptionValue = FOptionValue(OptionVariation, OptionAmount);
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RandomOptions, OptionValue);
		//		}
		//	}

		//	// 力访 焊呈胶 瓤苞
		//	TArray<FItemOption> TotemBonusOptions;
		//	if (BladeIIGameImpl::GetTotemDataStore().GetTotemBonusOption(Elem.Value, TotemBonusOptions))
		//	{
		//		for (FItemOption BonusOption : TotemBonusOptions)
		//		{
		//			if (BonusOption.OptionType == OptionType)
		//			{
		//				OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_TotemSmeltOptions, BonusOption.RawOptionAmount);
		//			}
		//		}
		//	}
		//}

		return (OutCombatStatInfo.IsEmpty() == false);
	}

	bool GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutCombatStatInfo, ICharacterDataStore* InCharacterData /*= NULL*/)
	{
		ICharacterDataStore* FinalUsedCDS = InCharacterData ? InCharacterData : &BladeIIGameImpl::GetLocalCharacterData();
		return FinalUsedCDS->GetOptionStatusRawValues(InPCClass, OptionType, OutCombatStatInfo);
	}

	/* GetCombatStatusValue 官曹锭 酒贰 GetRequestCombatStatusValue 档 度鞍捞 嘎免巴!! */
	float GetCombatStatusValue(EPCClass InPCClass, const TArray<FB2Item>& InEquipmentItem, const TArray<FB2Item>& InEquipmentCostume, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const FUnitySkillMissionArray& UnityInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems)
	{
		//float CharacterCombatPower = 0;
		//auto& ClientData = BladeIIGameImpl::GetClientDataStore();

		//// 酒捞袍 傈捧仿
		//float fItemCombatPower = 0;
		//for (auto& Item : InEquipmentItem)
		//{
		//	fItemCombatPower += Item.Power;
		//}

		//// 内胶片 傈捧仿
		//float fCostumeCombatPower = 0;
		//for (auto& Item : InEquipmentCostume)
		//{
		//	fCostumeCombatPower += Item.Power;
		//}

		//// 技飘 酒捞袍 傈捧仿
		//float fSetItemCombatPower = 0;
		//TMap<int32, int32> ItemSetMap;
		//TMap<int32, TArray<int32>>	EquipmentGroupIDMap;
		//TMap<int32, TArray<int32>>	AccessoriesGroupIDMap;

		//GetSetItemValue(InEquipmentItem, ItemSetMap);
		//GetSetItemValueByInvenType(InEquipmentItem,EItemInvenType::EIIVT_Protection, EquipmentGroupIDMap);
		//GetSetItemValueByInvenType(InEquipmentItem, EItemInvenType::EIIVT_Accessory, AccessoriesGroupIDMap);
		//
		//for (auto& elem : EquipmentGroupIDMap)
		//{
		//	int32 GroupID = elem.Key;
		//	auto SetIDs = elem.Value;

		//	int32 SetItemCount = 0;
		//	int32 LowClass = -1;
		//	int32 HighClass = -1;

		//	for (auto& SetID : SetIDs)
		//	{
		//		SetItemCount += ItemSetMap[SetID];
		//	}

		//	if (SetItemCount < 2)
		//		continue;

		//	int BreakPoint = SetItemCount;

		//	LowClass = SetIDs[0];

		//	for (auto& SetID : SetIDs)
		//	{
		//		if (ItemSetMap[SetID] >= 2)
		//			HighClass = SetID;
		//	}

		//	if (HighClass > 0 && HighClass != LowClass)
		//	{

		//		TArray<FItemSetOptionData> SetItemOptionDatas;
		//		auto* SetItemOption = ClientData.GetItemSetOptionInfo(HighClass);
		//		if (!SetItemOption)
		//			continue;
		//		SetItemOption->GetAllSetItemOption(SetItemOptionDatas, ItemSetMap[HighClass]);

		//		for (auto& OptionData : SetItemOptionDatas)
		//		{
		//			//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData.SetItemOption : %d, OptionData : %d,  optionvalue : %f"),
		//			//	HighClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

		//			if (OptionData.SetItemOptionValue != 0.0f)
		//			{
		//				fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));

		//				//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData : %d,  optionvalue : %f"), HighClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
		//			}

		//		}
		//		BreakPoint -= ItemSetMap[HighClass];
		//	}

		//	auto* SetItemOption2 = ClientData.GetItemSetOptionInfo(LowClass);
		//	if (!SetItemOption2)
		//		continue;
		//	TArray<FItemSetOptionData> SetItemOptionDatas2;
		//	SetItemOption2->GetAllSetItemOption(SetItemOptionDatas2, SetItemCount);
		//	Algo::Reverse(SetItemOptionDatas2);
		//	for (auto& OptionData : SetItemOptionDatas2)
		//	{
		//		if (BreakPoint != 0)
		//		{
		//			BreakPoint--;
		//		}
		//		else
		//			break;

		//		//UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData.SetItemOption  : %d, OptionData.SetItemOption :%d,  optionvalue : %f"),
		//		//	BreakPoint, LowClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

		//		if (OptionData.SetItemOptionValue != 0.0f)
		//		{
		//			fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));
		//		//	UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData : %d,  optionvalue : %f"), BreakPoint, LowClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
		//		}

		//	}

		//}

		//for (auto& elem : AccessoriesGroupIDMap)
		//{
		//	int32 GroupID = elem.Key;
		//	auto SetIDs = elem.Value;

		//	int32 SetItemCount = 0;
		//	int32 LowClass = -1;
		//	int32 HighClass = -1;

		//	for (auto& SetID : SetIDs)
		//	{
		//		SetItemCount += ItemSetMap[SetID];
		//	}

		//	if (SetItemCount < 2)
		//		continue;

		//	int BreakPoint = SetItemCount;

		//	LowClass = SetIDs[0];

		//	for (auto& SetID : SetIDs)
		//	{
		//		if (ItemSetMap[SetID] >= 2)
		//			HighClass = SetID;
		//	}

		//	if (HighClass > 0 && HighClass != LowClass)
		//	{

		//		TArray<FItemSetOptionData> SetItemOptionDatas;
		//		auto* SetItemOption = ClientData.GetItemSetOptionInfo(HighClass);
		//		if (!SetItemOption)
		//			continue;
		//		SetItemOption->GetAllSetItemOption(SetItemOptionDatas, ItemSetMap[HighClass]);

		//		for (auto& OptionData : SetItemOptionDatas)
		//		{
		//			//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData.SetItemOption : %d, OptionData : %d,  optionvalue : %f"),
		//			//	HighClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

		//			if (OptionData.SetItemOptionValue != 0.0f)
		//			{
		//				fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));

		//				//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData : %d,  optionvalue : %f"), HighClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
		//			}

		//		}
		//		BreakPoint -= ItemSetMap[HighClass];
		//	}

		//	auto* SetItemOption2 = ClientData.GetItemSetOptionInfo(LowClass);
		//	if (!SetItemOption2)
		//		continue;
		//	TArray<FItemSetOptionData> SetItemOptionDatas2;
		//	SetItemOption2->GetAllSetItemOption(SetItemOptionDatas2, SetItemCount);
		//	Algo::Reverse(SetItemOptionDatas2);
		//	for (auto& OptionData : SetItemOptionDatas2)
		//	{
		//		if (BreakPoint != 0)
		//		{
		//			BreakPoint--;
		//		}
		//		else
		//			break;

		//		//UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData.SetItemOption  : %d, OptionData.SetItemOption :%d,  optionvalue : %f"),
		//		//	BreakPoint, LowClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

		//		if (OptionData.SetItemOptionValue != 0.0f)
		//		{
		//			fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));
		//			//	UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData : %d,  optionvalue : %f"), BreakPoint, LowClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
		//		}

		//	}

		//}

		//// 技飘 内胶片 傈捧仿
		//float fSetCoustumeCombatPower = 0;
		//TMap<int32, int32> CoustumeSetMap;
		//GetSetItemValue(InEquipmentCostume, CoustumeSetMap);
		//for (auto& ItemSetPair : CoustumeSetMap)
		//{
		//	int32 SetItemId = ItemSetPair.Key;
		//	int32 SetItemCount = ItemSetPair.Value;

		//	if (SetItemCount < 2)
		//		continue;

		//	auto* SetOption = ClientData.GetItemSetOptionInfo(SetItemId);
		//	if (!SetOption)
		//		continue;

		//	TArray<FItemSetOptionData> SetItemOptionDatas;
		//	SetOption->GetAllSetItemOption(SetItemOptionDatas, SetItemCount);

		//	for (auto& OptionData : SetItemOptionDatas)
		//	{
		//		if (OptionData.SetItemOptionValue != 0.0f)
		//			fSetCoustumeCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));
		//	}
		//}

		//// 朝俺 傈捧仿
		//float fWingCombatPower = 0;
		//const TArray<FWingPropOption>& ThisPropOption = InWingContainer.WingData.GetPropOptionArr();
		//fWingCombatPower += InWingContainer.WingData.GetAttackPoint() * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		//fWingCombatPower += InWingContainer.WingData.GetDefensePoint() * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		//fWingCombatPower += InWingContainer.WingData.GetHealthPoint() * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);

		//for (auto& El : ThisPropOption)
		//{
		//	if (El.bIsOpen)
		//		fWingCombatPower += El.RawOptionAmount * ClientData.GetOptionWeight(El.MyOptionType);
		//}

		//// 柳鞭 傈捧仿
		//float fBrevetCombatPower = 0;
		//if (BrevetRank != 0)
		//{
		//	fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Offense_IncAttack, BrevetRank, BrevetNodeNum) *  ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		//	fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Defense_IncDefense, BrevetRank, BrevetNodeNum) *  ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		//	fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Health_IncHealth, BrevetRank, BrevetNodeNum) *  ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);
		//}

		//// 绊措蜡拱 傈捧仿
		//float fRelicCombatPower = 0;
		//for (auto RelicItem : RelicInfos)
		//{
		//	TArray<float> GradeOptionValue = GLOBALRELICMANAGER.GenerateGradeOptionValue(RelicItem.nRelicId, RelicItem.nRelicGrade);
		//	if (GradeOptionValue.Num() > 0)
		//	{
		//		fRelicCombatPower += (int32)GradeOptionValue[0] * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		//		fRelicCombatPower += (int32)GradeOptionValue[1] * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		//		fRelicCombatPower += (int32)GradeOptionValue[2] * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);
		//	}
		//}

		//// 搬加胶懦 傈捧仿
		//float fUnitySkillCombatPower = 0;
		//for (auto UnityItem : UnityInfos)
		//{
		//	fUnitySkillCombatPower += GLOBALUNITYSKILLMANAGER.GetUnitySkillPower(UnityItem);
		//}

		//// 俊抛福
		//float fEtherCombatPower = 0;
		//for (auto Elem : Ethers)
		//{
		//	fEtherCombatPower += Elem.Value.GetCombat();
		//}

		//// 其绢府
		//float fFairyCombatPower = 0;
		//for (const FB2FairyStatusInfo& Status : FairyStatus)
		//{
		//	FMD_FairyInfo FairyData;

		//	if (FairyManager::GetInstance().GetFairyOpendBlessInfo(Status.FairyType, Status.ActivedOptoinId, Status.Level, FairyData))
		//	{
		//		EItemOption BlessOption = SvrToCliOptionType(Status.ActivedOptoinId);
		//		fFairyCombatPower += Status.OptionValue * ClientData.GetOptionWeight(BlessOption);
		//	}
		//}
		//
		//// 配袍
		//float fTotemCombatPower = 0;
		//for (auto Elem : Totems)
		//{
		//	fTotemCombatPower += Elem.Value.Power;
		//}

		//CharacterCombatPower = fItemCombatPower + fCostumeCombatPower + fSetItemCombatPower + fSetCoustumeCombatPower
		//	+ fWingCombatPower + fBrevetCombatPower + fRelicCombatPower
		//	+ fUnitySkillCombatPower + fEtherCombatPower
		//	+ fFairyCombatPower + fTotemCombatPower;

		//return CharacterCombatPower;
		return 0.0;
	}

	//////////////////////////////////////////////////////////////////////////
	float GetRequestCombatStatusValue(EPCClass InPCClass, const TArray<FB2Item>& InEquipmentItem, const TArray<FB2Item>& InEquipmentCostume, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const FUnitySkillMissionArray& UnityInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems, TArray<b2network::B2CharacterPowerSpecificPtr>& OutCharacterPowerPtr)
	{
		float CharacterCombatPower = 0;
		auto& ClientData = BladeIIGameImpl::GetClientDataStore();

		// 酒捞袍 傈捧仿
		float fItemCombatPower = 0;
		for (auto& Item : InEquipmentItem)
		{
			fItemCombatPower += Item.Power;
		}

		/* ITEM_POWER */
		auto ItemPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		ItemPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::ITEM_POWER);
		ItemPowerSpecific->power = fItemCombatPower;
		OutCharacterPowerPtr.Add(ItemPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Item) : %0.2f"), fItemCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 内胶片 傈捧仿
		float fCostumeCombatPower = 0;
		for (auto& Item : InEquipmentCostume)
		{
			fCostumeCombatPower += Item.Power;
		}

		/* COSTUME_POWER */
		auto CostumePowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		CostumePowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::COSTUME_POWER);
		CostumePowerSpecific->power = fCostumeCombatPower;
		OutCharacterPowerPtr.Add(CostumePowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Costume) : %0.2f"), fCostumeCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 技飘 酒捞袍 傈捧仿
		float fSetItemCombatPower = 0;
		TMap<int32, int32> ItemSetMap;
		TMap<int32, TArray<int32>>	EquipmentGroupIDMap;
		TMap<int32, TArray<int32>>	AccessoriesGroupIDMap;

		GetSetItemValue(InEquipmentItem, ItemSetMap);
		GetSetItemValueByInvenType(InEquipmentItem, EItemInvenType::EIIVT_Protection, EquipmentGroupIDMap);
		GetSetItemValueByInvenType(InEquipmentItem, EItemInvenType::EIIVT_Accessory, AccessoriesGroupIDMap);

		for (auto& elem : EquipmentGroupIDMap)
		{
			int32 GroupID = elem.Key;
			auto SetIDs = elem.Value;

			int32 SetItemCount = 0;
			int32 LowClass = -1;
			int32 HighClass = -1;

			for (auto& SetID : SetIDs)
			{
				SetItemCount += ItemSetMap[SetID];
			}

			if (SetItemCount < 2)
				continue;

			int BreakPoint = SetItemCount;

			LowClass = SetIDs[0];

			for (auto& SetID : SetIDs)
			{
				if (ItemSetMap[SetID] >= 2)
					HighClass = SetID;
			}

			if (HighClass > 0 && HighClass != LowClass)
			{

				TArray<FItemSetOptionData> SetItemOptionDatas;
				auto* SetItemOption = ClientData.GetItemSetOptionInfo(HighClass);
				if (!SetItemOption)
					continue;
				SetItemOption->GetAllSetItemOption(SetItemOptionDatas, ItemSetMap[HighClass]);

				for (auto& OptionData : SetItemOptionDatas)
				{
					//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData.SetItemOption : %d, OptionData : %d,  optionvalue : %f"),
					//	HighClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

					if (OptionData.SetItemOptionValue != 0.0f)
					{
						fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));

						//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData : %d,  optionvalue : %f"), HighClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
					}

				}
				BreakPoint -= ItemSetMap[HighClass];
			}

			auto* SetItemOption2 = ClientData.GetItemSetOptionInfo(LowClass);
			if (!SetItemOption2)
				continue;
			TArray<FItemSetOptionData> SetItemOptionDatas2;
			SetItemOption2->GetAllSetItemOption(SetItemOptionDatas2, SetItemCount);
			Algo::Reverse(SetItemOptionDatas2);
			for (auto& OptionData : SetItemOptionDatas2)
			{
				if (BreakPoint != 0)
				{
					BreakPoint--;
				}
				else
					break;

				//UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData.SetItemOption  : %d, OptionData.SetItemOption :%d,  optionvalue : %f"),
				//	BreakPoint, LowClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

				if (OptionData.SetItemOptionValue != 0.0f)
				{
					fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));
					//	UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData : %d,  optionvalue : %f"), BreakPoint, LowClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
				}

			}

		}

		for (auto& elem : AccessoriesGroupIDMap)
		{
			int32 GroupID = elem.Key;
			auto SetIDs = elem.Value;

			int32 SetItemCount = 0;
			int32 LowClass = -1;
			int32 HighClass = -1;

			for (auto& SetID : SetIDs)
			{
				SetItemCount += ItemSetMap[SetID];
			}

			if (SetItemCount < 2)
				continue;

			int BreakPoint = SetItemCount;

			LowClass = SetIDs[0];

			for (auto& SetID : SetIDs)
			{
				if (ItemSetMap[SetID] >= 2)
					HighClass = SetID;
			}

			if (HighClass > 0 && HighClass != LowClass)
			{

				TArray<FItemSetOptionData> SetItemOptionDatas;
				auto* SetItemOption = ClientData.GetItemSetOptionInfo(HighClass);
				if (!SetItemOption)
					continue;
				SetItemOption->GetAllSetItemOption(SetItemOptionDatas, ItemSetMap[HighClass]);

				for (auto& OptionData : SetItemOptionDatas)
				{
					//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData.SetItemOption : %d, OptionData : %d,  optionvalue : %f"),
					//	HighClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

					if (OptionData.SetItemOptionValue != 0.0f)
					{
						fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));

						//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData : %d,  optionvalue : %f"), HighClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
					}

				}
				BreakPoint -= ItemSetMap[HighClass];
			}

			auto* SetItemOption2 = ClientData.GetItemSetOptionInfo(LowClass);
			if (!SetItemOption2)
				continue;
			TArray<FItemSetOptionData> SetItemOptionDatas2;
			SetItemOption2->GetAllSetItemOption(SetItemOptionDatas2, SetItemCount);
			Algo::Reverse(SetItemOptionDatas2);
			for (auto& OptionData : SetItemOptionDatas2)
			{
				if (BreakPoint != 0)
				{
					BreakPoint--;
				}
				else
					break;

				//UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData.SetItemOption  : %d, OptionData.SetItemOption :%d,  optionvalue : %f"),
				//	BreakPoint, LowClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

				if (OptionData.SetItemOptionValue != 0.0f)
				{
					fSetItemCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));
					//	UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData : %d,  optionvalue : %f"), BreakPoint, LowClass, (int)(OptionData.SetItemOption), OptionData.SetItemOptionValue);
				}

			}

		}

		/* SET_ITEM_POWER */
		auto SetItemPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		SetItemPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::SET_ITEM_POWER);
		SetItemPowerSpecific->power = fSetItemCombatPower;
		OutCharacterPowerPtr.Add(SetItemPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Set Item) : %0.2f"), fSetItemCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 技飘 内胶片 傈捧仿
		float fSetCoustumeCombatPower = 0;
		TMap<int32, int32> CoustumeSetMap;
		GetSetItemValue(InEquipmentCostume, CoustumeSetMap);
		for (auto& ItemSetPair : CoustumeSetMap)
		{
			int32 SetItemId = ItemSetPair.Key;
			int32 SetItemCount = ItemSetPair.Value;

			if (SetItemCount < 2)
				continue;

			auto* SetOption = ClientData.GetItemSetOptionInfo(SetItemId);
			if (!SetOption)
				continue;

			TArray<FItemSetOptionData> SetItemOptionDatas;
			SetOption->GetAllSetItemOption(SetItemOptionDatas, SetItemCount);

			for (auto& OptionData : SetItemOptionDatas)
			{
				if (OptionData.SetItemOptionValue != 0.0f)
					fSetCoustumeCombatPower += (OptionData.SetItemOptionValue * ClientData.GetOptionWeight(OptionData.SetItemOption));
			}
		}

		/* SET_COSTUME_POWER */
		auto SetCostumePowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		SetCostumePowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::SET_COUSTUME_POWER);
		SetCostumePowerSpecific->power = fSetCoustumeCombatPower;
		OutCharacterPowerPtr.Add(SetCostumePowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Set Costume) : %0.2f"), fSetCoustumeCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 柳鞭 傈捧仿
		float fBrevetCombatPower = 0;
		if (BrevetRank != 0)
		{
			fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Offense_IncAttack, BrevetRank, BrevetNodeNum) * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
			fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Defense_IncDefense, BrevetRank, BrevetNodeNum) * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
			fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Health_IncHealth, BrevetRank, BrevetNodeNum) * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);
		}

		/* RANK_POWER */
		auto BrevetPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		BrevetPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::RANK_POWER);
		BrevetPowerSpecific->power = fBrevetCombatPower;
		OutCharacterPowerPtr.Add(BrevetPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Brevet) : %0.2f"), fBrevetCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 朝俺 傈捧仿
		float fWingCombatPower = 0;
		const TArray<FWingPropOption>& ThisPropOption = InWingContainer.WingData.GetPropOptionArr();
		fWingCombatPower += InWingContainer.WingData.GetAttackPoint() * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		fWingCombatPower += InWingContainer.WingData.GetDefensePoint() * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		fWingCombatPower += InWingContainer.WingData.GetHealthPoint() * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);

		for (auto& El : ThisPropOption)
		{
			if (El.bIsOpen)
				fWingCombatPower += El.RawOptionAmount * ClientData.GetOptionWeight(El.MyOptionType);
		}

		/* WING_POWER */
		auto WingPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		WingPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::WING_POWER);
		WingPowerSpecific->power = fWingCombatPower;
		OutCharacterPowerPtr.Add(WingPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Wing) : %0.2f"), fWingCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 绊措蜡拱 傈捧仿
		float fRelicCombatPower = 0;
		for (auto RelicItem : RelicInfos)
		{
			//TArray<float> GradeOptionValue = GLOBALRELICMANAGER.GenerateGradeOptionValue(RelicItem.nRelicId, RelicItem.nRelicGrade);
			//if (GradeOptionValue.Num() > 0)
			//{
			//	fRelicCombatPower += (int32)GradeOptionValue[0] * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
			//	fRelicCombatPower += (int32)GradeOptionValue[1] * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
			//	fRelicCombatPower += (int32)GradeOptionValue[2] * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);
			//}
		}

		/* RELIC_POWER */
		auto RelicPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		RelicPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::RELIC_POWER);
		RelicPowerSpecific->power = fRelicCombatPower;
		OutCharacterPowerPtr.Add(RelicPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Relic) : %0.2f"), fRelicCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 搬加胶懦 傈捧仿
		float fUnitySkillCombatPower = 0;
		for (auto UnityItem : UnityInfos)
		{
			//fUnitySkillCombatPower += GLOBALUNITYSKILLMANAGER.GetUnitySkillPower(UnityItem);
		}

		/* UNITY_SKILL_POWER */
		auto UnitySkillPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		UnitySkillPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::UNITY_SKILL_POWER);
		UnitySkillPowerSpecific->power = fUnitySkillCombatPower;
		OutCharacterPowerPtr.Add(UnitySkillPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Unity Skill) : %0.2f"), fUnitySkillCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 俊抛福
		float fEtherCombatPower = 0;
		for (auto Elem : Ethers)
		{
			fEtherCombatPower += Elem.Value.GetCombat();
		}

		/* AETHER_POWER */
		auto EtherPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		EtherPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::AETHER_POWER);
		EtherPowerSpecific->power = fEtherCombatPower;
		OutCharacterPowerPtr.Add(EtherPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Ether) : %0.2f"), fEtherCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 其绢府
		float fFairyCombatPower = 0;
		for (const FB2FairyStatusInfo& Status : FairyStatus)
		{
			FMD_FairyInfo FairyData;

			if (FairyManager::GetInstance().GetFairyOpendBlessInfo(Status.FairyType, Status.ActivedOptoinId, Status.Level, FairyData))
			{
				EItemOption BlessOption = SvrToCliOptionType(Status.ActivedOptoinId);
				fFairyCombatPower += Status.OptionValue * ClientData.GetOptionWeight(BlessOption);
			}
		}

		/* FAIRY_POWER */
		auto FairyPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		FairyPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::FAIRY_POWER);
		FairyPowerSpecific->power = fFairyCombatPower;
		OutCharacterPowerPtr.Add(FairyPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Fairy) : %0.2f"), fFairyCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		// 配袍
		float fTotemCombatPower = 0;
		for (auto Elem : Totems)
		{
			fTotemCombatPower += Elem.Value.Power;
		}

		/* TOTEM_POWER */
		auto TotemPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		TotemPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::TOTEM_POWER);
		TotemPowerSpecific->power = fTotemCombatPower;
		OutCharacterPowerPtr.Add(TotemPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Totem) : %0.2f"), fTotemCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

		CharacterCombatPower = fItemCombatPower + fCostumeCombatPower + fSetItemCombatPower + fSetCoustumeCombatPower
			+ fWingCombatPower + fBrevetCombatPower + fRelicCombatPower
			+ fUnitySkillCombatPower + fEtherCombatPower
			+ fFairyCombatPower + fTotemCombatPower;

		return CharacterCombatPower;
	}
	//////////////////////////////////////////////////////////////////////////

	float GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, ICharacterDataStore* InCharacterData)
	{
		// 搬惫 惑窜 GetOptionStatusValue 甫 妮窍霸 瞪 巴捞促. 扁粮 牢磐其捞胶尔 嘎眠妨绊 持霸 等 扒单 鞘夸绝霸 瞪 荐档 乐促.

		// CharacterData 甫 瘤沥 救沁栏搁 肺拿 敲饭捞绢 某腐磐 铂肺.
		ICharacterDataStore* FinalUsedCDS = InCharacterData ? InCharacterData : &BladeIIGameImpl::GetLocalCharacterData();
		return FinalUsedCDS->GetOptionStatusValue(InPCClass, OptionType); // 鞘夸窍搁 Item 单捞磐 蝶肺 瘤沥 啊瓷窍霸.
	}

	/** Get applied SkillOption value of designated CharacterDataStore (LocalCharacterData if NULL) Info.
	 * 漂沥 Active Skill 俊 措秦辑父 累悼窍绰 可记蔼捞 酒囱 老馆利栏肺 利侩登绰 Passive 幅 可记 蔼甸阑 掘绢咳. */
	float GetSkillOptionStatusValue(EPCClass InPCClass, ESkillOption InOptionType, ICharacterDataStore* InCharacterData /*= NULL*/)
	{
		CombatStatInfoRawDatas CombatRawDatas;
		GetSkillOptionStatusRawValues(InPCClass, InOptionType, CombatRawDatas, InCharacterData);

		TArray<FOptionValue> AllFoundRawValues;
		CombatRawDatas.GetAllRawValues(AllFoundRawValues);

		return GetEffectiveOptionValueTempl<ESkillOption>(InOptionType, AllFoundRawValues, true);
	}

	////////////////////////////////////////////////////////////////////////////////
	////////// The master option status value querying function
	/** EItemOption 捞尔 ESkillOption 酒快福扁. SkillOption 吝俊急 passive 父. */
	float GetUnitedOptionStatusValue(EPCClass InPCClass, EUnitedCombatOptions InUnitedOptionType, ICharacterDataStore* InCharacterData /*= NULL*/)
	{
		// InUnitedOptionType 俊 秦寸窍绰 ItemOption 苞 SkillOption 蔼阑 啊廉客辑 茄单 利侩.
		EItemOption MappedItemOption = GetItemOptionOfUnitedOption(InUnitedOptionType);
		ESkillOption MappedSkillOption = GetSkillOptionOfUnitedOption(InUnitedOptionType);

		// 利绢档 笛 吝 窍唱绰 蜡瓤秦具. 货肺 甸绢柯 芭扼搁 InitializeUnitedOptionMapping 阑..
		checkSlow(MappedItemOption != EItemOption::EIO_End || MappedSkillOption != ESkillOption::ESO_End);

		// GetEffectiveOptionValueTempl 俊辑 窍绰 老何甫 咯扁辑档 秦具 且 淀.
		// itemOption, SkillOption 笛 促 蜡瓤且 版快 笛 葛滴俊辑 GetCombatOptionApplyType 搬苞绰 老摹秦具 茄促. CheckUnitedOptionMappingValidity 俊辑 眉农
		ECombatOptionApplyType OptionApplyType = (MappedItemOption != EItemOption::EIO_End) ? GetCombatOptionApplyType(MappedItemOption) : GetCombatOptionApplyType(MappedSkillOption);
		// 笛 吝 窍唱绰 绝阑 荐 乐栏骨肺 檬扁蔼捞 力傍登绢具.
		const float InitialValueOfInvalidID = GetInitialValueOfCombatOptionApplyType(OptionApplyType);

		// 滴 矫胶袍狼 搬苞蔼阑 蝶肺 备秦辑 钦眉
		const float ItemOptionStatusValue = (MappedItemOption != EItemOption::EIO_End) ?
			GetOptionStatusValue(InPCClass, MappedItemOption, InCharacterData) : InitialValueOfInvalidID;
		const float SkillOptionStatusValue = (MappedSkillOption != ESkillOption::ESO_End) ?
			GetSkillOptionStatusValue(InPCClass, MappedSkillOption, InCharacterData) : InitialValueOfInvalidID;

		// RawValue 啊 酒囱 ScaledValue 肺 摹绊 OptionApplyType 俊 蝶扼 钦眉
		TArray<FOptionValue> GatheredOptionValue;
		FOptionValue ItemOptionValue = FOptionValue(ItemOptionStatusValue);
		FOptionValue SkillOptionValue = FOptionValue(SkillOptionStatusValue);
		GatheredOptionValue.Add(ItemOptionValue);
		GatheredOptionValue.Add(SkillOptionValue);
		return GetEffectiveOptionValueInternal(OptionApplyType, GatheredOptionValue, false, true);
	}

	bool GetUnitedOptionStatusRawValues(EPCClass InPCClass, EUnitedCombatOptions InUnitedOptionType, CombatStatInfoRawDatas& OutCombatStatInfo, ICharacterDataStore* InCharacterData /*= NULL*/)
	{
		OutCombatStatInfo.Empty();

		// InUnitedOptionType 俊 秦寸窍绰 ItemOption 苞 SkillOption 蔼阑 啊廉客辑 茄单 利侩.
		EItemOption MappedItemOption = GetItemOptionOfUnitedOption(InUnitedOptionType);
		ESkillOption MappedSkillOption = GetSkillOptionOfUnitedOption(InUnitedOptionType);

		// 利绢档 笛 吝 窍唱绰 蜡瓤秦具. 货肺 甸绢柯 芭扼搁 InitializeUnitedOptionMapping 阑..
		checkSlow(MappedItemOption != EItemOption::EIO_End || MappedSkillOption != ESkillOption::ESO_End);

		// GetEffectiveOptionValueTempl 俊辑 窍绰 老何甫 咯扁辑档 秦具 且 淀.
		// itemOption, SkillOption 笛 促 蜡瓤且 版快 笛 葛滴俊辑 GetCombatOptionApplyType 搬苞绰 老摹秦具 茄促. CheckUnitedOptionMappingValidity 俊辑 眉农
		ECombatOptionApplyType OptionApplyType = (MappedItemOption != EItemOption::EIO_End) ? GetCombatOptionApplyType(MappedItemOption) : GetCombatOptionApplyType(MappedSkillOption);
		// 笛 吝 窍唱绰 绝阑 荐 乐栏骨肺 檬扁蔼捞 力傍登绢具.
		const float InitialValueOfInvalidID = GetInitialValueOfCombatOptionApplyType(OptionApplyType);

		//if (MappedItemOption != EItemOption::EIO_End)
			//GetOptionStatusRawValues(InPCClass, MappedItemOption, OutCombatStatInfo, InCharacterData);

		//if (MappedSkillOption != ESkillOption::ESO_End)
		//	GetSkillOptionStatusRawValues(InPCClass, MappedSkillOption, OutCombatStatInfo, InCharacterData);

		return (OutCombatStatInfo.IsEmpty() == false);
	}

	float GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Totem>& Totems)
	{
		CombatStatInfoRawDatas CombatRawDatas;
		GetOptionStatusRawValuesByMod(InPCClass, ModeType, OptionType, AllEquipped, InWingContainer, BrevetRank, BrevetNodeNum, RelicInfos, Totems, CombatRawDatas);

		TArray<FOptionValue> PrimaryPointRawValues;
		CombatRawDatas.GetAllRawValues(PrimaryPointRawValues);

		//老窜 荤侩 救窃. 眠饶俊 鞘夸窍搁 凯绢辑 荤侩窍矫搁 凳.
		//double EnhanceIncreaseFactor = (ConvertItemOptionToItemPrimaryPointType(OptionType) != EItemPrimaryPointType::EIPP_End ? GetEnhanceSetEffectFactor(AllEquipped) : 0.f);
		double EnhanceIncreaseFactor = 0.0;
		// 碍拳 酒捞袍 技飘 瓤苞甫 绊妨茄 GetEffectiveOptionValue.. 滚傈栏肺..
		return  GetEffectiveOptionValueWithEnhanceSetEffectTempl<EItemOption>(OptionType, PrimaryPointRawValues, EnhanceIncreaseFactor, true);
	}

	bool GetOptionStatusRawValuesByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Totem>& Totems, CombatStatInfoRawDatas& OutCombatStatInfo)
	{
		//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
		//EItemPrimaryPointType CheckPrimaryPointType = ConvertItemOptionToItemPrimaryPointType(OptionType);
		//
		//// PVP 绊蜡 可记 眉农
		//EItemOption PVPModeItemOption = ConvertItemPrimaryPointTypeToItemOptionByPVPMode(CheckPrimaryPointType, ModeType);

		//// 酒捞袍 可记
		//for (auto& Item : AllEquipped)
		//{
		//	// 扁夯瓷仿摹
		//	
		//	// 绊蜡可记 (搬捧狼 厘脚备)
		//	if (PVPModeItemOption != EItemOption::EIO_End)
		//	{
		//		for (auto& CurrOption : Item.IntrinsicOptions)
		//		{
		//			if (CurrOption.OptionType == PVPModeItemOption)
		//			{
		//				OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_IntrinsicOptions, CurrOption.RawOptionAmount);
		//			}
		//		}
		//	}

		//	//阿己 可记
		//	for (auto& CurrOption : Item.SealOptions)
		//	{
		//		if (CurrOption.OptionInfo.OptionType == PVPModeItemOption)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_SealOption, CurrOption.OptionInfo.RawOptionAmount);
		//	}

		//	// 罚待(老馆)可记
		//}

		//// 朝俺 可记
		////if (InWingContainer.bHasWing)
		//{
		//	// 扁夯瓷仿摹
		//}

		//// 柳鞭 可记(傍 规 眉)
		////if (BrevetRank != 0)
		//{
		//}

		//// 酒捞袍 技飘瓤苞
		////TMap<int32, int32>	ItemSetMap;
		////GetSetItemValue(AllEquipped, ItemSetMap);
		////酒捞袍 技飘 八荤
		////for (auto& ItemSetPair : ItemSetMap)
		//{
		//}

		////绊措蜡拱
		//RelicManager RelicMGR(GLOBALRELICMANAGER);
		////if (&RelicMGR) //瘤开俊辑 static栏肺 教臂沛 积己窍绰芭扼 公炼扒 脚汾 秦具 且淀...
		//{
		//	int32 nRelicId, nGrade, nLevel;

		//	for (FAncientRelic RelicInfo : RelicInfos)
		//	{
		//		nRelicId = RelicInfo.nRelicId; nGrade = RelicInfo.nRelicGrade;  nLevel = RelicInfo.nRelicLevel;

		//		// 扁夯瓷仿摹
		//		switch (CheckPrimaryPointType)
		//		{
		//		case EItemPrimaryPointType::EIPP_Attack:					
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RelicOption, RelicMGR.GetPCAttackByMode(InPCClass, ModeType, nRelicId, nGrade, nLevel));
		//			break;
		//		case EItemPrimaryPointType::EIPP_Defense:
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RelicOption, RelicMGR.GetPCDefenseByMode(InPCClass, ModeType, nRelicId, nGrade, nLevel));
		//			break;
		//		case EItemPrimaryPointType::EIPP_Health:
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RelicOption, RelicMGR.GetPCHealthByMode(InPCClass, ModeType, nRelicId, nGrade, nLevel));
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//}

		//// 配袍俊 嘿阑 荐 乐绰 Option 格废
		//EItemOption TotemModeItemOption = ConvertItemPrimaryPointTypeToItemOptionByTotemMode(CheckPrimaryPointType, ModeType);

		//for (const TPair<int64, FB2Totem>& Elem : Totems)
		//{
		//	if (TotemModeItemOption != EItemOption::EIO_End)
		//	{
		//		// 厘厚 傍拜仿, 规绢仿, 眉仿
		//		if (Elem.Value.MainOptionDetail.OptionType == TotemModeItemOption)
		//		{
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_NormalPoint, Elem.Value.MainOptionDetail.RawOptionAmount);
		//		}

		//		// 厘厚 眠啊 可记
		//		for (int SubOptionIndex = 0; SubOptionIndex < Elem.Value.SubOptionDetails.Num(); ++SubOptionIndex)
		//		{
		//			if (Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.OptionType == TotemModeItemOption)
		//			{
		//				EOptionVariation OptionVariation = Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.OptionVariationType;
		//				float OptionAmount = Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.RawOptionAmount;
		//				FOptionValue OptionValue = FOptionValue(OptionVariation, OptionAmount);
		//				OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RandomOptions, OptionValue);
		//			}
		//		}

		//		// 力访 焊呈胶 瓤苞
		//		TArray<FItemOption> TotemBonusOptions;
		//		if (BladeIIGameImpl::GetTotemDataStore().GetTotemBonusOption(Elem.Value, TotemBonusOptions))
		//		{
		//			for (FItemOption BonusOption : TotemBonusOptions)
		//			{
		//				if (BonusOption.OptionType == TotemModeItemOption)
		//				{
		//					OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_TotemSmeltOptions, BonusOption.RawOptionAmount);
		//				}
		//			}
		//		}
		//	}
		//}

		//return (OutCombatStatInfo.IsEmpty() == false);
		return true;
	}


	/** UnitedOption 狼 琴缴 沥焊牢 EItemOption 苞 ESkillOption 俊 措茄 甘俏 沥焊. Const 肺 父甸绊 酵菌瘤父 弊犯霸 窍瘤 给茄 lookup 抛捞喉 侩档.
	 * 流立 荤侩窍瘤 臼绊 Get**OptionOfUnitedOption 阑 荤侩. */
	static TMap<EUnitedCombatOptions, FUCOBoundInfo>	UnitedOptionMappingTable;
	static TMap<EItemOption, EUnitedCombatOptions>		ItemOptionToUnitedOptionMappingTable;
	static TMap<ESkillOption, EUnitedCombatOptions>		SkillOptionToUnitedOptionMappingTable;
	static TMap<EMercenarySkillOption, EUnitedCombatOptions>		MercenarySkillOptionToUnitedOptionMappingTable;

#if !UE_BUILD_SHIPPING
	void CheckUnitedOptionMappingValidity()
	{	// Check for possible miss..
		checkSlow(UnitedOptionMappingTable.Num() == static_cast<int32>(EUnitedCombatOptions::UCO_End));

		int32 FoundItemOptionNum = 0;
		int32 FoundSkillOptionNum = 0;
		for (int32 UCOI = 0; UCOI < static_cast<int32>(EUnitedCombatOptions::UCO_End); ++UCOI)
		{
			EUnitedCombatOptions ThisUCO = static_cast<EUnitedCombatOptions>(UCOI);
			EItemOption MappedItemOption = GetItemOptionOfUnitedOption(ThisUCO);
			//ESkillOption MappedSkillOption = GetSkillOptionOfUnitedOption(ThisUCO);

			//if (MappedItemOption != EItemOption::EIO_End) {
			//	++FoundItemOptionNum;
			//}
			//if (MappedSkillOption != ESkillOption::ESO_End) {
			//	++FoundSkillOptionNum;
			//}

			//if (MappedItemOption != EItemOption::EIO_End && MappedSkillOption != ESkillOption::ESO_End)
			//{ // 笛 荤捞俊 CombatOptionApplyType 捞 鞘洒 悼老秦具 拌魂等 吧 度鞍捞 利奅窍电瘤 且 芭促. 促弗 霸 乐促搁 茄率阑 颊龙
			//	checkSlow(GetCombatOptionApplyType(MappedItemOption) == GetCombatOptionApplyType(MappedSkillOption));
			//}
		}

		checkSlow(FoundItemOptionNum == static_cast<int32>(EItemOption::EIO_End)); // 咯变 促 器窃登绢具
		// 0 酒囱 1 何磐 矫累秦辑 -1, 磊眉 己拜惑 器窃 救登绰 芭 -4, ActiveSkill 肺父 利侩登绰 芭扼 器窃 救登绰 芭 -14 (扁裙俊 蝶扼 函版瞪 荐 乐澜.)
		checkSlow(FoundSkillOptionNum == static_cast<int32>(ESkillOption::ESO_End) - 1 - 4 - 14);
	}
#endif
	void InitializeUnitedOptionMapping() // 捞吧 葛碘 檬扁拳 殿 绢凋啊俊辑 茄锅 捞惑 妮 秦辑 甘俏 沥焊甫 盲况持绢具 茄促.
	{
		// 角力 甘俏 内靛绰 狼档利栏肺 包访 叼颇牢 葛酒初篮 漂沥 庆歹 颇老俊 困摹矫糯.
		INITIALIZE_UNITED_OPTION_MAPPING_BLOCK(UnitedOptionMappingTable, ItemOptionToUnitedOptionMappingTable, SkillOptionToUnitedOptionMappingTable, MercenarySkillOptionToUnitedOptionMappingTable);
#if !UE_BUILD_SHIPPING
		CheckUnitedOptionMappingValidity(); // Check for possible miss..
#endif
	}
	void ConditionalInitializeUnitedOptionMapping()
	{  // 酒付 俊叼磐父 弊繁 芭 鞍变 茄单 葛碘捞 酒抗 促矫 哆绰瘤 茄锅 Initialize 茄 饶俊 厚绢滚府绰 老捞 惯积秦辑 捞吧 霖厚.
		if (UnitedOptionMappingTable.Num() == 0)
		{
			InitializeUnitedOptionMapping();
		}
	}

	ESkillOption GetSkillOptionOfUnitedOption(EUnitedCombatOptions InUCO)
	{
		ConditionalInitializeUnitedOptionMapping();
		FUCOBoundInfo* FoundBoundInfo = UnitedOptionMappingTable.Find(InUCO);
		checkSlow(FoundBoundInfo || InUCO == EUnitedCombatOptions::UCO_End);
		if (FoundBoundInfo)
		{
			checkSlow(FoundBoundInfo->GetUnitedOptionId() == InUCO);
			return FoundBoundInfo->GetMappedSkillOption();
		}
		return ESkillOption::ESO_End;
	}
	EItemOption GetItemOptionOfUnitedOption(EUnitedCombatOptions InUCO)
	{
		ConditionalInitializeUnitedOptionMapping();
		FUCOBoundInfo* FoundBoundInfo = UnitedOptionMappingTable.Find(InUCO);
		checkSlow(FoundBoundInfo || InUCO == EUnitedCombatOptions::UCO_End);
		if (FoundBoundInfo)
		{
			checkSlow(FoundBoundInfo->GetUnitedOptionId() == InUCO);
			return FoundBoundInfo->GetMappedItemOption();
		}
		return EItemOption::EIO_End;
	}
	EUnitedCombatOptions GetUnitedOptionOfItemOption(EItemOption InItemOption)
	{
		ConditionalInitializeUnitedOptionMapping();

		if (ItemOptionToUnitedOptionMappingTable.Contains(InItemOption) == false)
			return EUnitedCombatOptions::UCO_End;

		return ItemOptionToUnitedOptionMappingTable[InItemOption];
	}
	EUnitedCombatOptions GetUnitedOptionOfSkillOption(ESkillOption InSkillOption)
	{
		ConditionalInitializeUnitedOptionMapping();

		if (SkillOptionToUnitedOptionMappingTable.Contains(InSkillOption) == false)
			return EUnitedCombatOptions::UCO_End;

		return SkillOptionToUnitedOptionMappingTable[InSkillOption];
	}

	EUnitedCombatOptions GetUnitedOptionOfMercenarySkillOption(EMercenarySkillOption InUCO)
	{
		ConditionalInitializeUnitedOptionMapping();

		if (MercenarySkillOptionToUnitedOptionMappingTable.Contains(InUCO) == false)
			return EUnitedCombatOptions::UCO_End;

		return MercenarySkillOptionToUnitedOptionMappingTable[InUCO];
	}

	
		////////////////////////////////////////////////////////////////////////////////
		int32 GetEnhanceSetEffectLevel(const TArray<FB2Item>& AllEquipped)
		{
			const int32 MinEnhanceLevel = BladeIIGameImpl::GetClientDataStore().GetMinEnhanceLevelForSetEffect();
			const int32 MinArrayEnhanceEffectLevel = (MIN_COUNT_ENHANCE_ITEM_SET_EFFECT - 1);
				
			// 1. 厘馒等巴 吝俊 弥家饭骇 捞惑牢巴甸阑 府胶飘拳
			TArray<int32> ApplyItemLevelList;
			for (auto& EquipItem : AllEquipped)
			{
				if (EquipItem.EnhanceLevel >= MinEnhanceLevel)
					ApplyItemLevelList.Add(EquipItem.EnhanceLevel);
			}
	
			// 2. 府胶飘 荐啊 弥家利侩 荐焊促 救瞪锭绰 利侩救窃
			if (ApplyItemLevelList.Num() == 0 || ApplyItemLevelList.Num() < MIN_COUNT_ENHANCE_ITEM_SET_EFFECT)
				return 0;
	
			// 3. 郴覆瞒鉴 沥纺饶 MinEnhanceLevel 牢郸胶啊 弥家饭骇
			ApplyItemLevelList.Sort([](const int32& A, const int32& B) { return (A > B); } );
			
			if (ApplyItemLevelList.IsValidIndex(MinArrayEnhanceEffectLevel) == false)
				return 0;
	
			return ApplyItemLevelList[MinArrayEnhanceEffectLevel];
		}
	
		double GetEnhanceSetEffectFactor(const TArray<FB2Item>& AllEquipped)
		{
			int32 EnhanceSetEffectLevel = GetEnhanceSetEffectLevel(AllEquipped);
			return BladeIIGameImpl::GetClientDataStore().GetEnhanceItemSetEffect(EnhanceSetEffectLevel);
		}
	
		bool IsPrimPointTypeByItemOption(EItemOption InCheckType)
		{
			switch (InCheckType)
			{
			case EItemOption::EIO_Offense_IncAttack:
			case EItemOption::EIO_Defense_IncDefense:
			case EItemOption::EIO_Health_IncHealth:
				return true;
			}
			return false;
		}
	
		EItemOption GetPrimPointIncOption(EItemPrimaryPointType InCheckType)
		{
			switch (InCheckType)
			{
			case EItemPrimaryPointType::EIPP_Attack: return EItemOption::EIO_Offense_IncAttack;
			case EItemPrimaryPointType::EIPP_Defense: return EItemOption::EIO_Defense_IncDefense;
			case EItemPrimaryPointType::EIPP_Health: return EItemOption::EIO_Health_IncHealth;
			}
			return EItemOption::EIO_End;
		}
	
		EItemPrimaryPointType ConvertItemOptionToItemPrimaryPointType(EItemOption InCheckType)
		{
			switch (InCheckType)
			{
			case EItemOption::EIO_Offense_IncAttack: return EItemPrimaryPointType::EIPP_Attack;
			case EItemOption::EIO_Defense_IncDefense: return EItemPrimaryPointType::EIPP_Defense;
			case EItemOption::EIO_Health_IncHealth: return EItemPrimaryPointType::EIPP_Health;
			}
			return EItemPrimaryPointType::EIPP_End;
		}
	
		EItemOption ConvertItemPrimaryPointTypeToItemOptionByPVPMode(EItemPrimaryPointType InCheckType, EB2GameMode ModeType)
		{
			switch (InCheckType)
			{
			case EItemPrimaryPointType::EIPP_Attack:
			{
				switch (ModeType)
				{
				case EB2GameMode::PVP_Tag:
					return EItemOption::EIO_Tag_Offense_IncAttack;
	
				case EB2GameMode::PVP_Team:
					return EItemOption::EIO_TeamMatch_Offense_IncAttack;
	
				case EB2GameMode::Control:
					return EItemOption::EIO_Assault_Offense_IncAttack;
	
				case EB2GameMode::Guild:
					return EItemOption::EIO_Guild_Offense_IncAttack;
	
				default:
					return EItemOption::EIO_End;
				}
			}
			case EItemPrimaryPointType::EIPP_Defense:
			{
				switch (ModeType)
				{
				case EB2GameMode::PVP_Tag:
					return EItemOption::EIO_Tag_Defense_IncDefense;
	
				case EB2GameMode::PVP_Team:
					return EItemOption::EIO_TeamMatch_Defense_IncDefense;
	
				case EB2GameMode::Control:
					return EItemOption::EIO_Assault_Defense_IncDefense;
	
				case EB2GameMode::Guild:
					return EItemOption::EIO_Guild_Defense_IncDefense;
	
				default:
					return EItemOption::EIO_End;
				}
			}
			case EItemPrimaryPointType::EIPP_Health:
			{
				switch (ModeType)
				{
				case EB2GameMode::PVP_Tag:
					return EItemOption::EIO_Tag_Health_IncHealth;
	
				case EB2GameMode::PVP_Team:
					return EItemOption::EIO_TeamMatch_Health_IncHealth;
	
				case EB2GameMode::Control:
					return EItemOption::EIO_Assault_Health_IncHealth;
	
				case EB2GameMode::Guild:
					return EItemOption::EIO_Guild_Health_IncHealth;
	
				default:
					return EItemOption::EIO_End;
				}
			}
			}
	
			return EItemOption::EIO_End;
		}
	
		EItemOption ConvertItemPrimaryPointTypeToItemOptionByTotemMode(EItemPrimaryPointType InCheckType, EB2GameMode ModeType)
		{
			switch (InCheckType)
			{
			case EItemPrimaryPointType::EIPP_Attack:
			{
				switch (ModeType)
				{
				case EB2GameMode::PVP_Tag:
					return EItemOption::EIO_Tag_Offense_IncAttack;
	
				case EB2GameMode::PVP_Team:
					return EItemOption::EIO_TeamMatch_Offense_IncAttack;
	
				case EB2GameMode::Control:
					return EItemOption::EIO_Assault_Offense_IncAttack;
	
				case EB2GameMode::Guild:
					return EItemOption::EIO_Guild_Offense_IncAttack;
	
				case EB2GameMode::HeroTower:
					return EItemOption::EIO_HeroTower_Offense_IncAttack;
	
				case EB2GameMode::Raid:
					return EItemOption::EIO_Raid_Offense_IncAttack;
	
				default:
					return EItemOption::EIO_End;
				}
			}
			case EItemPrimaryPointType::EIPP_Defense:
			{
				switch (ModeType)
				{
				case EB2GameMode::PVP_Tag:
					return EItemOption::EIO_Tag_Defense_IncDefense;
	
				case EB2GameMode::PVP_Team:
					return EItemOption::EIO_TeamMatch_Defense_IncDefense;
	
				case EB2GameMode::Control:
					return EItemOption::EIO_Assault_Defense_IncDefense;
	
				case EB2GameMode::Guild:
					return EItemOption::EIO_Guild_Defense_IncDefense;
	
				case EB2GameMode::HeroTower:
					return EItemOption::EIO_HeroTower_Defense_IncDefense;
	
				case EB2GameMode::Raid:
					return EItemOption::EIO_Raid_Defense_IncDefense;
	
				default:
					return EItemOption::EIO_End;
				}
			}
			case EItemPrimaryPointType::EIPP_Health:
			{
				switch (ModeType)
				{
				case EB2GameMode::PVP_Tag:
					return EItemOption::EIO_Tag_Health_IncHealth;
	
				case EB2GameMode::PVP_Team:
					return EItemOption::EIO_TeamMatch_Health_IncHealth;
	
				case EB2GameMode::Control:
					return EItemOption::EIO_Assault_Health_IncHealth;
	
				case EB2GameMode::Guild:
					return EItemOption::EIO_Guild_Health_IncHealth;
	
				case EB2GameMode::HeroTower:
					return EItemOption::EIO_HeroTower_Health_IncHealth;
	
				case EB2GameMode::Raid:
					return EItemOption::EIO_Raid_Health_IncHealth;
	
				default:
					return EItemOption::EIO_End;
				}
			}
			}
	
			return EItemOption::EIO_End;
		}
	
	void InitCombatStatMap(TMap<EUnitedCombatOptions, float>& InMap)
	{
		InMap.Empty();
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecDamage, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecMeleeDamage, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecRangedDamage, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecBossDamage, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_ResistCritical, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncDamage, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncNormalDamage, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Skill_IncSkillDamage, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncCriticalRate, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncCriticalDamage, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncBossDamage, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_HealHPByCountAttack, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Health_ProbableRecoverHP, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncAttackSpeed, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToGladiator, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToAssassin, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToWizard, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Offense_IncAttackDamageToFighter, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecDamageByGladiator, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecDamageByAssassin, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecDamageByWizard, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Defense_DecDamageByFighter, 1.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Health_PerSecRecoverHP, 0.0f);
		InMap.Add(EUnitedCombatOptions::UCO_Health_IncHealth, 0.0f);
	}

	bool GetSkillOptionStatusRawValues(EPCClass InPCClass, ESkillOption InOptionType, CombatStatInfoRawDatas& OutCombatStatInfo, ICharacterDataStore* InCharacterData /*= NULL*/)
	{
		// 酒捞袍, 朝俺, 柳鞭, 蜡拱 殿俊辑 荤侩窍绰 EItemOption 栏肺 备盒登绰 可记富绊 ESkillOption 栏肺 备盒登绰 芭.
		ICharacterDataStore* FinalUsedCDS = InCharacterData ? InCharacterData : &BladeIIGameImpl::GetLocalCharacterData();

		TArray<int32> AllFoundSkillID;
		// 咯扁辑狼 荤侩 狼固惑 PassiveSkill 父 啊廉柯促. ActiveSkill 狼 秦寸 胶懦俊 利侩登绰 可记 惑怕蔼篮 喊档肺 贸府.
		FinalUsedCDS->GetCharacterPassiveSkills(InPCClass, AllFoundSkillID);

		// 某腐磐啊 啊瘤绊 乐绰 胶懦可记甸俊辑 InOptionType 俊 秦寸窍绰 蔼甸阑 避绢葛澜.
		for (int32 ThisSkillID : AllFoundSkillID)
		{
			// 秦寸 胶懦 ID 客 饭骇俊辑狼 胶懦可记甸.
			TArray<FSkillOptionData> FoundSkillOptions;
			FinalUsedCDS->GetCharacterSkillOptionsOfID(ThisSkillID, FoundSkillOptions);
			for (FSkillOptionData& SkillOptionElem : FoundSkillOptions)
			{
				if (InOptionType == SvrToCliSkillOption(SkillOptionElem.OptionId))
					OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_SkillOption, SkillOptionElem.OptionValue);
			}
		}

		return (OutCombatStatInfo.IsEmpty() == false);
	}

	void GetSetItemValue(const TArray<FB2Item>& AllEquipped, TMap<int32, int32>& OutSetItemDatas)
	{
		for (auto& Item : AllEquipped)
			OutSetItemDatas.Contains(Item.SetID) ? ++OutSetItemDatas[Item.SetID] : OutSetItemDatas.Add(Item.SetID, 1);
	}

	void GetSetItemValue_Version2(const TArray<FB2Item>& AllEquipped, TMap<int32, int32>& OutSetItemDatas)
	{
		TMap<int32, int32> EquipedSetItems;
		TMap<int32, TArray<int32>> ProtectionGroupIDDatas;
		TMap<int32, TArray<int32>> AccessoryGroupIDDatas;
		TMap<int32, int32>			CostumeSetIDs;

		GetSetItemValue(AllEquipped, EquipedSetItems);
		GetSetItemValueByInvenType(AllEquipped, EItemInvenType::EIIVT_Protection, ProtectionGroupIDDatas);
		GetSetItemValueByInvenType(AllEquipped, EItemInvenType::EIIVT_Accessory, AccessoryGroupIDDatas);
		GetCostumesSetItemValue(AllEquipped, CostumeSetIDs);

		for (auto elem : ProtectionGroupIDDatas)
		{
			/*for (auto elem_key : elem.Value)
			{
				if (elem_key == elem.Value[0])
				{
					OutSetItemDatas.Add(elem_key, elem.Value.Num());
				}
				else
				{
					OutSetItemDatas.Add(elem_key, EquipedSetItems[elem_key]);
				}
			}*/
			auto SetIDs = elem.Value;
			int32 SetItemCount = 0;

			for (auto& SetID : SetIDs)
			{
				SetItemCount += EquipedSetItems[SetID];
			}

			auto Iter = elem.Value.CreateConstIterator();
			OutSetItemDatas.Add(*Iter, SetItemCount);
			Iter++;

			for (; Iter; ++Iter)
			{

				OutSetItemDatas.Add(*Iter, EquipedSetItems[*Iter]);

			}
		}

		for (auto elem : AccessoryGroupIDDatas)
		{
			auto SetIDs = elem.Value;
			int32 SetItemCount = 0;

			for (auto& SetID : SetIDs)
			{
				SetItemCount += EquipedSetItems[SetID];
			}

			auto Iter = elem.Value.CreateConstIterator();
			OutSetItemDatas.Add(*Iter, SetItemCount);
			Iter++;

			for (; Iter; ++Iter)
			{

				OutSetItemDatas.Add(*Iter, EquipedSetItems[*Iter]);

			}
		}

		for (auto& SetID : CostumeSetIDs)
		{
			OutSetItemDatas.Contains(SetID.Key) ? ++OutSetItemDatas[SetID.Key] : OutSetItemDatas.Add(SetID.Key, 1);
		}
	}

	void GetCostumesSetItemValue(const TArray<FB2Item>& AllEquipped, TMap<int32, int32>& OutSetItemDatas)
	{
		for (auto& Item : AllEquipped)
		{
			if (Item.InventoryType == EItemInvenType::EIIVT_CostumeArmor || Item.InventoryType == EItemInvenType::EIIVT_CostumeWeapon)
				OutSetItemDatas.Contains(Item.SetID) ? ++OutSetItemDatas[Item.SetID] : OutSetItemDatas.Add(Item.SetID, 1);
		}

	}

	//void GetSetItemValue(const TArray<FB2Item>& AllEquipped, CheckInventoryType ENum option栏肺 牢亥 鸥涝阑 持绢辑 鞍篮芭老 版快父 持霸 秦林搁 凳 TMap<int32, int32>& OutSetItemDatas,
	void GetSetItemValueByInvenType(const TArray<FB2Item>& AllEquipped, EItemInvenType CheckInventoryType,
		TMap<int32, TArray<int32>>& OutGroupIDDatas)
	{
		/*for (auto& Item : AllEquipped)
			OutSetItemDatas.Contains(Item.SetID) ? ++OutSetItemDatas[Item.SetID] : OutSetItemDatas.Add(Item.SetID, 1);*/
		for (auto& Item : AllEquipped)
		{
			int GroupID = Item.ItemRefID % 10;
			// if(Item.InventoryType == CheckInventoryType)
			if (Item.InventoryType == CheckInventoryType)
			{
				if (OutGroupIDDatas.Contains(GroupID))
				{
					if (OutGroupIDDatas[GroupID].Contains(Item.SetID))
						continue;
					else
						OutGroupIDDatas[GroupID].Add(Item.SetID);
				}
				else
				{
					OutGroupIDDatas.Add(GroupID);
					OutGroupIDDatas[GroupID].Add(Item.SetID);
				}
			}
		}

		for (auto& Item : OutGroupIDDatas)
		{
			Item.Value.Sort();
		}


	}

	float GetPCGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle, EItemOption InItemOption)
	{
		if (InCharacterDataStore == nullptr)
			return 0;

		auto GuildSkillInfo = InCharacterDataStore->GetGuildSkillInfos();		//泅犁啊瘤绊乐绰 辨靛胶懦 啊廉柯促

		for (auto GuildSkillInfoSlot : GuildSkillInfo)
		{
			if (GuildSkillInfoSlot == nullptr)
				continue;

			//if (GuildSkillInfoSlot->level)				//饭骇0捞甸绢棵荐档 乐阑荐 乐促
			//{
			//	auto GuildSkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillInfoSlot->id, GuildSkillInfoSlot->level);	// 胶懦俊措茄 付胶磐 单捞磐甫 啊廉柯促

			//	if (!GuildSkillMasterData || (!IsGuildBattle && !GuildSkillMasterData->buff_time_sec))
			//		continue;

			//	//辨靛傈捞搁 矫埃蔼 眉农 救窃
			//	if (!IsGuildBattle)
			//	{
			//		float RemainTime = UB2UIManager::RemainTimeFunction(GuildSkillInfoSlot->buff_end_time);

			//		if (RemainTime <= 0)		//矫埃捞 瘤车栏搁 蔼栏 0阑 馆券
			//			continue;
			//	}

			//	if (SvrToCliOptionType(GuildSkillMasterData->buff_option_type) == InItemOption)
			//	{
			//		return GuildSkillMasterData->buff_option_value;
			//	}
			//}
		}
		return 0;
	}

	bool IsActiveGuildBuff(ICharacterDataStore* InCharacterDataStore, EItemOption InItemOption)
	{
		if (InCharacterDataStore == nullptr)
			return false;

		auto GuildSkillInfo = InCharacterDataStore->GetGuildSkillInfos();		//泅犁啊瘤绊乐绰 辨靛胶懦 啊廉柯促

		for (auto GuildSkillInfoSlot : GuildSkillInfo)
		{
			if (GuildSkillInfoSlot == nullptr)
				continue;

			if (GuildSkillInfoSlot->level)				//饭骇0捞甸绢棵荐档 乐阑荐 乐促
			{
				auto GuildSkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillInfoSlot->id, GuildSkillInfoSlot->level);	// 胶懦俊措茄 付胶磐 单捞磐甫 啊廉柯促

				//if (!GuildSkillMasterData || !GuildSkillMasterData->buff_time_sec)
				//	continue;

				//float RemainTime = UB2UIManager::RemainTimeFunction(GuildSkillInfoSlot->buff_end_time);

				//if (RemainTime <= 0)		//矫埃捞 瘤车栏搁 蔼栏 0阑 馆券
				//	continue;

				//if (SvrToCliOptionType(GuildSkillMasterData->buff_option_type) == InItemOption)
				//	return true;
			}
		}

		return false;
	}


	float GetPCAttackGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle)
	{
		EItemOption AttacktemOption = EItemOption::EIO_Offense_IncAttack;
		if (IsGuildBattle)
			AttacktemOption = EItemOption::EIO_Guild_Offense_IncAttack;

		return GetPCGuildBuffValue(InCharacterDataStore, IsGuildBattle, AttacktemOption);
	}

	float GetPCDefendseGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle)
	{
		EItemOption AttacktemOption = EItemOption::EIO_Defense_IncDefense;
		if (IsGuildBattle)
			AttacktemOption = EItemOption::EIO_Guild_Defense_IncDefense;

		return GetPCGuildBuffValue(InCharacterDataStore, IsGuildBattle, AttacktemOption);
	}

	float GetPCHalthGuildBuffValue(ICharacterDataStore* InCharacterDataStore, bool IsGuildBattle)
	{
		EItemOption AttacktemOption = EItemOption::EIO_Health_IncHealth;
		if (IsGuildBattle)
			AttacktemOption = EItemOption::EIO_Guild_Health_IncHealth;

		return GetPCGuildBuffValue(InCharacterDataStore, IsGuildBattle, AttacktemOption);
	}

	void ApplySetItemValueByInvenType(FClientDataStore& ClientDataStore, TMap<int32, int32> ItemSetMap, EItemOption OptionType,
		CombatStatInfoRawDatas& OutCombatStatInfo, TMap<int32, TArray<int32>> GroupIDDatas)
	{
		for (auto& elem : GroupIDDatas)
		{
			int32 GroupID = elem.Key;
			auto SetIDs = elem.Value;


			int32 SetItemCount = 0;
			int32 LowClass = -1;
			int32 HighClass = -1;

			for (auto& SetID : SetIDs)
			{
				SetItemCount += ItemSetMap[SetID];
			}

			if (SetItemCount < 2)
				continue;

			int BreakPoint = SetItemCount;

			LowClass = SetIDs[0];

			for (auto& SetID : SetIDs)
			{
				if (ItemSetMap[SetID] >= 2)
					HighClass = SetID;
			}

			//UE_LOG(LogTemp, Log, TEXT("SetCount : %d High : %d, Low : %d"), SetItemCount, HighClass, LowClass);

			if (HighClass > 0 && HighClass != LowClass)
			{

				TArray<FItemSetOptionData> SetItemOptionDatas;
				auto* HighClassSetOption = ClientDataStore.GetItemSetOptionInfo(HighClass);
				if (!HighClassSetOption)
					continue;
				HighClassSetOption->GetAllSetItemOption(SetItemOptionDatas, ItemSetMap[HighClass]);

				for (auto& OptionData : SetItemOptionDatas)
				{
					//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData.SetItemOption : %d, OptionData : %d,  optionvalue : %f"), 

					//	HighClass, (int)(OptionData.SetItemOption), (int)OptionType, OptionData.SetItemOptionValue);

					if (OptionData.SetItemOption != OptionType)
						continue;

					if (OptionData.SetItemOptionValue != 0.0f)
					{
						OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_ItemSetEffect, OptionData.SetItemOptionValue);
						//UE_LOG(LogTemp, Log, TEXT("high setid %d,  OptionData : %d,  optionvalue : %f"), HighClass, (int)(OptionData.SetItemOption),OptionData.SetItemOptionValue);
					}

				}
				BreakPoint -= ItemSetMap[HighClass];
			}

			auto* LowClassSetOption = ClientDataStore.GetItemSetOptionInfo(LowClass);
			if (!LowClassSetOption)
				continue;
			TArray<FItemSetOptionData> SetItemOptionDatas2;
			LowClassSetOption->GetAllSetItemOption(SetItemOptionDatas2, SetItemCount);
			Algo::Reverse(SetItemOptionDatas2);
			for (auto& OptionData : SetItemOptionDatas2)
			{
				if (BreakPoint != 0)
				{
					BreakPoint--;
				}
				else
					break;


				//UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData.SetItemOption  : %d, OptionData.SetItemOption :%d,  optionvalue : %f"),
				//	BreakPoint, LowClass, (int)(OptionData.SetItemOption), (int)OptionType,OptionData.SetItemOptionValue);

				if (OptionData.SetItemOption != OptionType)
					continue;

				if (OptionData.SetItemOptionValue != 0.0f)
				{
					OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_ItemSetEffect, OptionData.SetItemOptionValue);
					//	UE_LOG(LogTemp, Log, TEXT("break point : %d,  low setid %d,  OptionData : %d,  optionvalue : %f"), BreakPoint,LowClass, (int)(OptionData.SetItemOption),OptionData.SetItemOptionValue);
				}

			}

		}
	}

	void ApplyCostumeSetItemValue(FClientDataStore& ClientDataStore, TMap<int32, int32> SetItemDatas, EItemOption OptionType,
		CombatStatInfoRawDatas& OutCombatStatInfo)
	{
		for (auto& ItemSetPair : SetItemDatas)
		{
			int32 SetItemId = ItemSetPair.Key;
			int32 SetItemCount = ItemSetPair.Value;
			if (SetItemCount < 2)
				continue;

			auto* SetItemOption = ClientDataStore.GetItemSetOptionInfo(SetItemId);
			if (!SetItemOption)
				continue;
			TArray<FItemSetOptionData> SetItemOptionDatas;
			SetItemOption->GetAllSetItemOption(SetItemOptionDatas, SetItemCount);

			for (auto& OptionData : SetItemOptionDatas)
			{
				if (OptionData.SetItemOption != OptionType)
					continue;

				if (OptionData.SetItemOptionValue != 0.0f)
					OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_ItemSetEffect, OptionData.SetItemOptionValue);
			}
		}
	}
}


// Core of GetEffectiveOptionValueTempl. SkillOption 捞尔 ItemOption 烹钦 扒 肯丰登搁 捞芭尔 包访 窃荐甸 困摹 粱 沥府.
float GetEffectiveOptionValueInternal(ECombatOptionApplyType InOptionApplyType,
	TArray<FOptionValue>& InSourceValues, // Raw (displayed) values if bRawValueSource is true, effective scaled values if false
	bool bRawValueSource, /*= true*/ // Designate whether InSourceValues are raw values or effective scaled values.
	bool bClampValue /*= false*/) // 蔼俊 弥措 弥家蔼 焊沥捞 甸绢埃促.
{
	// Initial value should be different depend on whether it is add or (1 minus) multiplication.
	float FinalEffectiveValue = GetInitialValueOfCombatOptionApplyType(InOptionApplyType);

	// ECombatOptionApplyType 俊 蝶扼 葛酒柳 蔼甸阑 钦摹绰 operator 甫 力傍窍绰 巴捞 捞 窃荐狼 开且捞促. 
	// 钦模促绰 霸 歹窍扁甫 狼固窍瘤父篮 臼绰促.
	for (FOptionValue SourceValue : InSourceValues)
	{
		// 0.0 ~ 1.0 scaling if SourceValue if raw. Need clamping for multiplication type.
		float EffectiveValue = (bRawValueSource) ? GetEffectiveValueForOptionApplyType(InOptionApplyType, SourceValue) : SourceValue.RawOptionAmount;

		if (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply)
		{ // Now it is about how to apply, as its type name implies.
			FinalEffectiveValue *= EffectiveValue;
		}
		else // Add_Value, Add_Percent, Subtract_Value 狼 版快绰 角 利侩矫俊绰 - 秦 林绢具..
		{
			if (SourceValue.OptionVariationType == EOptionVariation::EOV_Incerease)
			{
				FinalEffectiveValue += EffectiveValue;
			}
			else if (SourceValue.OptionVariationType == EOptionVariation::EOV_Decrease)
			{
				FinalEffectiveValue -= EffectiveValue;
			}
		}
	}

	//  瓷仿摹 刘皑 拌魂阑 场辰 饶 惑茄俊 力茄阑 扒促.
	if (bClampValue)
	{
		if (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply)
		{
			// ex > 焊胶 各 单固瘤 皑家 可记捞 开栏肺 利侩登歹扼档 扁粮焊促 歹 酒橇霸 甸绢坷柳 臼档废 茄促.
			FinalEffectiveValue = FMath::Clamp(FinalEffectiveValue, 0.0f, 1.0f);
		}
		else
		{
			// ex > 眉仿 皑家 可记捞 酒公府 惯妨乐绢档 0焊促 撤酒瘤瘤 臼绰促.
			FinalEffectiveValue = FMath::Max(FinalEffectiveValue, 0.0f);
		}
	}

	return FinalEffectiveValue;
}