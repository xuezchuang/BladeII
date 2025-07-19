
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


namespace CombatStatEval // 전투력 측정 글로벌 유틸
{
	/**
	 * ICharacterDataStore 이용해서 체력 스텟 얻어옴.
	 * CharacterData 는 리모트 플레이어 용으로 의도한 것으로 명시하지 않으면 로컬 캐릭터 정보를 사용한다.
	 * SpecifiedLevel 과 SpecifiedEquipData 를 기본값이 아닌 임의의 값을 주면 그걸 사용한다. 특정 레벨이나 장비 조합의 프리뷰 등의 용도.
	 */
	float GetPCHealth(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/, bool IsGuildBattle)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 지정하지 않을 경우 로컬 캐릭터 정보로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 보다 크면 레벨값을 직접 지정한 걸로 사용

		fStatusValue += GetPCBaseHealth(FinalUsedLevel);
		fStatusValue += CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Health_IncHealth, SpecifiedEquipData);

		// SkillOption 에는 기본 공/방/체 더하기는 없지만 특별히 Health 를 퍼센트로 올리는 옵션이 있다.
		const float ExtraIncMaxHPRate = GetUnitedOptionStatusValue(InPCClass, EUnitedCombatOptions::UCO_Health_IncMaxHP, CharacterData);
		fStatusValue *= (1.0f + ExtraIncMaxHPRate);

		fStatusValue = fStatusValue + (fStatusValue * (GetPCHalthGuildBuffValue(CharacterData, IsGuildBattle) * 0.01));		//길드 버프

		return fStatusValue;
	}


	/**
	 * ICharacterDataStore 이용해서 공격 스텟 얻어옴.
	 * CharacterData 는 리모트 플레이어 용으로 의도한 것으로 명시하지 않으면 로컬 캐릭터 정보를 사용한다.
	 * SpecifiedLevel 과 SpecifiedEquipData 를 기본값이 아닌 임의의 값을 주면 그걸 사용한다. 특정 레벨이나 장비 조합의 프리뷰 등의 용도.
	 */
	float GetPCAttack(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/, bool IsGuildBattle)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 지정하지 않을 경우 로컬 캐릭터 정보로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 보다 크면 레벨값을 직접 지정한 걸로 사용

		fStatusValue += GetPCBaseAttack(FinalUsedLevel);
		fStatusValue += CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Offense_IncAttack, SpecifiedEquipData);

		fStatusValue = fStatusValue + (fStatusValue * (GetPCAttackGuildBuffValue(CharacterData, IsGuildBattle) * 0.01));		//길드 버프
		return fStatusValue;
	}


	/**
	 * ICharacterDataStore 이용해서 방어 스텟 얻어옴.
	 * CharacterData 는 리모트 플레이어 용으로 의도한 것으로 명시하지 않으면 로컬 캐릭터 정보를 사용한다.
	 * SpecifiedLevel 과 SpecifiedEquipData 를 기본값이 아닌 임의의 값을 주면 그걸 사용한다. 특정 레벨이나 장비 조합의 프리뷰 등의 용도.
	 */
	float GetPCDefense(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/, bool IsGuildBattle)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 지정하지 않을 경우 로컬 캐릭터 정보로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 보다 크면 레벨값을 직접 지정한 걸로 사용

		fStatusValue += GetPCBaseDefense(FinalUsedLevel);
		fStatusValue += CharacterData->GetOptionStatusValue(InPCClass, EItemOption::EIO_Defense_IncDefense, SpecifiedEquipData);

		fStatusValue = fStatusValue + (fStatusValue * (GetPCDefendseGuildBuffValue(CharacterData, IsGuildBattle) * 0.01));		//길드 버프
		return fStatusValue;
	}



	/**
	 * ICharacterDataStore 이용해서 캐릭터 종합 전투력을 얻어옴.
	 * 이건 실제 전투에 사용되는 것은 아니고 일종의 종합 평가 수치의 의미로 사용되는 것이다. 값이 높을수록 전투에서 유용하긴 하겠지.
	 * CharacterData 는 리모트 플레이어 용으로 의도한 것으로 명시하지 않으면 로컬 캐릭터 정보를 사용한다.
	 * SpecifiedLevel 과 SpecifiedEquipData 를 기본값이 아닌 임의의 값을 주면 그걸 사용한다. 특정 레벨이나 장비 조합의 프리뷰 등의 용도.
	 */
	float GetPCCombatPower(EPCClass InPCClass, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		float fStatusValue = 0.0f;

		if (!CharacterData) // CharacterData 를 지정 안했으면 로컬 플레이어 캐릭터 껄로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		const int32 FinalUsedLevel = SpecifiedLevel > 0 ? SpecifiedLevel : CharacterData->GetCharacterLevel(InPCClass); // 0 보다 크면 레벨값을 직접 지정한 걸로 사용

		auto& ClientData = BladeIIGameImpl::GetClientDataStore();

		// 기본 캐릭터 공방체 전투력
		float AttackPower = (static_cast<float>(GetPCBaseAttack(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack));
		float DefensePower = (static_cast<float>(GetPCBaseDefense(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense));
		float HealthPower = (static_cast<float>(GetPCBaseHealth(FinalUsedLevel)) * ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth));

		fStatusValue += AttackPower;
		fStatusValue += DefensePower;
		fStatusValue += HealthPower;

		// 장비,날개 등등으로 추가된 전투력
		fStatusValue += CharacterData->GetCombatStatusValue(InPCClass, SpecifiedEquipData);

		// 길드버프로 추가된 전투력
		// 총 공방체 * 길드버프퍼센트 * 전투력 가중치 
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

		if (!CharacterData) // 지정하지 않을 경우 로컬 캐릭터 정보로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		//Temp Value 지금은 고대유물 밖에 안사용 하기 때문. 필요하면 호출 위치를(CharacterData->GetOptionStatusValue) 변경하여 사용하시면 됨.
		fStatusValue = CharacterData->GetOptionStatusValueByMod(InPCClass, ModeType, EItemOption::EIO_Offense_IncAttack);

		return fStatusValue;
	}

	float GetPCDefenseByMod(EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 지정하지 않을 경우 로컬 캐릭터 정보로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		//Temp Value 지금은 고대유물 밖에 안사용 하기 때문. 필요하면 호출 위치를(CharacterData->GetOptionStatusValue) 변경하여 사용하시면 됨.
		fStatusValue = CharacterData->GetOptionStatusValueByMod(InPCClass, ModeType, EItemOption::EIO_Defense_IncDefense);

		return fStatusValue;
	}

	float GetPCHealthByMod(EPCClass InPCClass, EB2GameMode ModeType, ICharacterDataStore* CharacterData /*= NULL*/, int32 SpecifiedLevel /*= -1*/, TArray<FB2Item>* SpecifiedEquipData /*= NULL*/)
	{
		if (InPCClass == EPCClass::EPC_End)
			return 0.0f;

		float fStatusValue = 0.0f;

		if (!CharacterData) // 지정하지 않을 경우 로컬 캐릭터 정보로.
			CharacterData = &BladeIIGameImpl::GetLocalCharacterData();

		//Temp Value 지금은 고대유물 밖에 안사용 하기 때문. 필요하면 호출 위치를(CharacterData->GetOptionStatusValue) 변경하여 사용하시면 됨.
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

		// 기본 캐릭터 공방체 전투력
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

		// 장비,날개 등등으로 추가된 전투력
		fStatusValue += CharacterData.GetRequestCombatStatusValue(InPCClass, OutCharacterPowerPtr);

		/* TOTAL_POWER */
		auto TotalPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
		TotalPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::TOTAL_POWER);
		TotalPowerSpecific->power = fStatusValue;
		OutCharacterPowerPtr.Add(TotalPowerSpecific);
		BII_SCREEN_LOG(FString::Printf(TEXT("Total CombatPower : %0.2f"), fStatusValue), FLinearColor(0.0, 0.5f, 0.5f, 1.0f), 15, 10.0f);

		// 길드버프로 추가된 전투력
		// 총 공방체 * 길드버프퍼센트 * 전투력 가중치 
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
	//		// 강화 아이템 세트 효과를 고려한 GetEffectiveOptionValue.. 버전으로.. 
	//		float ResultOptionValue = GetEffectiveOptionValueWithEnhanceSetEffectTempl<EItemOption>(OptionType, PrimaryPointRawValues, EnhanceIncreaseFactor, true);
	//
	//		// 공방체 옵션의 경우 캐릭터 기본 옵션과 합쳐지기 전에 조정 과정을 거친다.
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

		//// 아이템 옵션
		//for (auto& Item : AllEquipped)
		//{
		//	// 기본능력치
		//	if (Item.PrimaryPointType == CheckPrimaryPointType)
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_NormalPoint, Item.GetPrimaryPoint());

		//	// 고유옵션
		//	for (auto& CurrOption : Item.IntrinsicOptions)
		//	{
		//		if (CurrOption.OptionType == OptionType)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_IntrinsicOptions, CurrOption.RawOptionAmount);
		//	}

		//	// 랜덤(일반)옵션
		//	for (auto& CurrOption : Item.RandomOptions)
		//	{
		//		if (CurrOption.OptionType == OptionType)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RandomOptions, CurrOption.RawOptionAmount);
		//	}

		//	//각성 옵션
		//	for (auto& CurrOption : Item.SealOptions)
		//	{
		//		if(CurrOption.OptionInfo.OptionType == OptionType)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_SealOption, CurrOption.OptionInfo.RawOptionAmount);
		//	}
		//}

		//// 날개 옵션
		//if (InWingContainer.bHasWing)
		//{
		//	const FWingPropOption* ThisPropOption = InWingContainer.WingData.GetPropOptionPtr(OptionType);
		//	if (ThisPropOption && ThisPropOption->bIsOpen)
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_WingAdditionalOption, ThisPropOption->RawOptionAmount);

		//	// 기본능력치
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

		//// 진급 옵션(공 방 체)
		//if (BrevetRank != 0)
		//{
		//	int32 nAddStatus = ClientDataStore.GetBrevetNodeTotalStatus(OptionType, BrevetRank, BrevetNodeNum);
		//	OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RankPromotion, float(nAddStatus));
		//}
		///////////////////////////////////////////////////////////////////////////////////여기군//////////////////////////
		//// 아이템 세트효과
		//TMap<int32, int32>	ItemSetMap;
		//TMap<int32, TArray<int32>>	EquipmentGroupIDMap;
		//TMap<int32, TArray<int32>>	AccessoriesGroupIDMap;
		//TMap<int32, int32>	CostumeGroupIDMap;
		//GetSetItemValue(AllEquipped, ItemSetMap);
		//GetSetItemValueByInvenType(AllEquipped, EItemInvenType::EIIVT_Protection, EquipmentGroupIDMap);
		//GetSetItemValueByInvenType(AllEquipped, EItemInvenType::EIIVT_Accessory, AccessoriesGroupIDMap);
		//GetCostumesSetItemValue(AllEquipped, CostumeGroupIDMap );// 현재 코스튬읜 방어구 무기가 세트로 작동함
		//
		//ApplySetItemValueByInvenType(ClientDataStore,ItemSetMap, OptionType, OutCombatStatInfo, EquipmentGroupIDMap);
		//ApplySetItemValueByInvenType(ClientDataStore, ItemSetMap, OptionType, OutCombatStatInfo, AccessoriesGroupIDMap);
		//ApplyCostumeSetItemValue(ClientDataStore, CostumeGroupIDMap,OptionType,OutCombatStatInfo);

		//// 페어리
		//for (const FB2FairyStatusInfo& Status : FairyStatus)
		//{
		//	EItemOption BlessOption = SvrToCliOptionType(Status.ActivedOptoinId);
		//	if (BlessOption != OptionType)
		//		continue;

		//	FMD_FairyInfo OutData;
		//	if (FairyManager::GetInstance().GetFairyOpendBlessInfo(Status.FairyType, Status.ActivedOptoinId, Status.Level, OutData))
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_FairyOption, OutData.OptionValue);
		//}

		//// 고대유물
		//RelicManager RelicMGR(GLOBALRELICMANAGER);
		//// if (&RelicMGR) //지역에서 static으로 싱글톤 생성하는거라 무조건 신뢰 해야 할듯...
		//{
		//	const FMD_AncientRelicGradeElem* gradeInfo;
		//	int32 nRelicId, nGrade;

		//	for (FAncientRelic RelicInfo : RelicInfos)
		//	{
		//		nRelicId = RelicInfo.nRelicId;	nGrade = RelicInfo.nRelicGrade;

		//		gradeInfo = RelicMGR.getRelicGradeInfo(nRelicId, nGrade);

		//		if (gradeInfo)
		//		{
		//			// 기본능력치
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

		//// 에테르
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

		//// 토템
		//for (const TPair<int64, FB2Totem>& Elem : Totems)
		//{
		//	// 장비 공격력, 방어력, 체력
		//	if (Elem.Value.MainOptionDetail.OptionType == OptionType)
		//	{
		//		OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_NormalPoint, Elem.Value.MainOptionDetail.RawOptionAmount);
		//	}

		//	// 장비 추가 옵션
		//	for (int SubOptionIndex = 0; SubOptionIndex < Elem.Value.SubOptionDetails.Num(); ++SubOptionIndex)
		//	{
		//		if (Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.OptionType == OptionType)
		//		{
		//			// 감소 옵션이 가능한 부분
		//			EOptionVariation OptionVariation = Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.OptionVariationType;
		//			float OptionAmount = Elem.Value.SubOptionDetails[SubOptionIndex].SubOption.RawOptionAmount;
		//			FOptionValue OptionValue = FOptionValue(OptionVariation, OptionAmount);
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_RandomOptions, OptionValue);
		//		}
		//	}

		//	// 제련 보너스 효과
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

	/* GetCombatStatusValue 바꿀때 아래 GetRequestCombatStatusValue 도 똑같이 맞출것!! */
	float GetCombatStatusValue(EPCClass InPCClass, const TArray<FB2Item>& InEquipmentItem, const TArray<FB2Item>& InEquipmentCostume, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const FUnitySkillMissionArray& UnityInfos, const TMap<int64, FB2Ether>& Ethers, const TArray<FB2FairyStatusInfo>& FairyStatus, const TMap<int64, FB2Totem>& Totems)
	{
		//float CharacterCombatPower = 0;
		//auto& ClientData = BladeIIGameImpl::GetClientDataStore();

		//// 아이템 전투력
		//float fItemCombatPower = 0;
		//for (auto& Item : InEquipmentItem)
		//{
		//	fItemCombatPower += Item.Power;
		//}

		//// 코스튬 전투력
		//float fCostumeCombatPower = 0;
		//for (auto& Item : InEquipmentCostume)
		//{
		//	fCostumeCombatPower += Item.Power;
		//}

		//// 세트 아이템 전투력
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

		//// 세트 코스튬 전투력
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

		//// 날개 전투력
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

		//// 진급 전투력
		//float fBrevetCombatPower = 0;
		//if (BrevetRank != 0)
		//{
		//	fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Offense_IncAttack, BrevetRank, BrevetNodeNum) *  ClientData.GetOptionWeight(EItemOption::EIO_Offense_IncAttack);
		//	fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Defense_IncDefense, BrevetRank, BrevetNodeNum) *  ClientData.GetOptionWeight(EItemOption::EIO_Defense_IncDefense);
		//	fBrevetCombatPower += BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalStatus(EItemOption::EIO_Health_IncHealth, BrevetRank, BrevetNodeNum) *  ClientData.GetOptionWeight(EItemOption::EIO_Health_IncHealth);
		//}

		//// 고대유물 전투력
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

		//// 결속스킬 전투력
		//float fUnitySkillCombatPower = 0;
		//for (auto UnityItem : UnityInfos)
		//{
		//	fUnitySkillCombatPower += GLOBALUNITYSKILLMANAGER.GetUnitySkillPower(UnityItem);
		//}

		//// 에테르
		//float fEtherCombatPower = 0;
		//for (auto Elem : Ethers)
		//{
		//	fEtherCombatPower += Elem.Value.GetCombat();
		//}

		//// 페어리
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
		//// 토템
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

		// 아이템 전투력
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

		// 코스튬 전투력
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

		// 세트 아이템 전투력
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

		// 세트 코스튬 전투력
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

		// 진급 전투력
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

		// 날개 전투력
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

		// 고대유물 전투력
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

		// 결속스킬 전투력
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

		// 에테르
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

		// 페어리
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

		// 토템
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
		// 결국 상단 GetOptionStatusValue 를 콜하게 될 것이다. 기존 인터페이스랑 맞추려고 넣게 된 건데 필요없게 될 수도 있다.

		// CharacterData 를 지정 안했으면 로컬 플레이어 캐릭터 껄로.
		ICharacterDataStore* FinalUsedCDS = InCharacterData ? InCharacterData : &BladeIIGameImpl::GetLocalCharacterData();
		return FinalUsedCDS->GetOptionStatusValue(InPCClass, OptionType); // 필요하면 Item 데이터 따로 지정 가능하게.
	}

	/** Get applied SkillOption value of designated CharacterDataStore (LocalCharacterData if NULL) Info.
	 * 특정 Active Skill 에 대해서만 작동하는 옵션값이 아닌 일반적으로 적용되는 Passive 류 옵션 값들을 얻어옴. */
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
	/** EItemOption 이랑 ESkillOption 아우르기. SkillOption 중에선 passive 만. */
	float GetUnitedOptionStatusValue(EPCClass InPCClass, EUnitedCombatOptions InUnitedOptionType, ICharacterDataStore* InCharacterData /*= NULL*/)
	{
		// InUnitedOptionType 에 해당하는 ItemOption 과 SkillOption 값을 가져와서 한데 적용.
		EItemOption MappedItemOption = GetItemOptionOfUnitedOption(InUnitedOptionType);
		ESkillOption MappedSkillOption = GetSkillOptionOfUnitedOption(InUnitedOptionType);

		// 적어도 둘 중 하나는 유효해야. 새로 들어온 거라면 InitializeUnitedOptionMapping 을..
		checkSlow(MappedItemOption != EItemOption::EIO_End || MappedSkillOption != ESkillOption::ESO_End);

		// GetEffectiveOptionValueTempl 에서 하는 일부를 여기서도 해야 할 듯.
		// itemOption, SkillOption 둘 다 유효할 경우 둘 모두에서 GetCombatOptionApplyType 결과는 일치해야 한다. CheckUnitedOptionMappingValidity 에서 체크
		ECombatOptionApplyType OptionApplyType = (MappedItemOption != EItemOption::EIO_End) ? GetCombatOptionApplyType(MappedItemOption) : GetCombatOptionApplyType(MappedSkillOption);
		// 둘 중 하나는 없을 수 있으므로 초기값이 제공되어야.
		const float InitialValueOfInvalidID = GetInitialValueOfCombatOptionApplyType(OptionApplyType);

		// 두 시스템의 결과값을 따로 구해서 합체
		const float ItemOptionStatusValue = (MappedItemOption != EItemOption::EIO_End) ?
			GetOptionStatusValue(InPCClass, MappedItemOption, InCharacterData) : InitialValueOfInvalidID;
		const float SkillOptionStatusValue = (MappedSkillOption != ESkillOption::ESO_End) ?
			GetSkillOptionStatusValue(InPCClass, MappedSkillOption, InCharacterData) : InitialValueOfInvalidID;

		// RawValue 가 아닌 ScaledValue 로 치고 OptionApplyType 에 따라 합체
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

		// InUnitedOptionType 에 해당하는 ItemOption 과 SkillOption 값을 가져와서 한데 적용.
		EItemOption MappedItemOption = GetItemOptionOfUnitedOption(InUnitedOptionType);
		ESkillOption MappedSkillOption = GetSkillOptionOfUnitedOption(InUnitedOptionType);

		// 적어도 둘 중 하나는 유효해야. 새로 들어온 거라면 InitializeUnitedOptionMapping 을..
		checkSlow(MappedItemOption != EItemOption::EIO_End || MappedSkillOption != ESkillOption::ESO_End);

		// GetEffectiveOptionValueTempl 에서 하는 일부를 여기서도 해야 할 듯.
		// itemOption, SkillOption 둘 다 유효할 경우 둘 모두에서 GetCombatOptionApplyType 결과는 일치해야 한다. CheckUnitedOptionMappingValidity 에서 체크
		ECombatOptionApplyType OptionApplyType = (MappedItemOption != EItemOption::EIO_End) ? GetCombatOptionApplyType(MappedItemOption) : GetCombatOptionApplyType(MappedSkillOption);
		// 둘 중 하나는 없을 수 있으므로 초기값이 제공되어야.
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

		//일단 사용 안함. 추후에 필요하면 열어서 사용하시면 됨.
		//double EnhanceIncreaseFactor = (ConvertItemOptionToItemPrimaryPointType(OptionType) != EItemPrimaryPointType::EIPP_End ? GetEnhanceSetEffectFactor(AllEquipped) : 0.f);
		double EnhanceIncreaseFactor = 0.0;
		// 강화 아이템 세트 효과를 고려한 GetEffectiveOptionValue.. 버전으로..
		return  GetEffectiveOptionValueWithEnhanceSetEffectTempl<EItemOption>(OptionType, PrimaryPointRawValues, EnhanceIncreaseFactor, true);
	}

	bool GetOptionStatusRawValuesByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, const TArray<FB2Item>& AllEquipped, const FPCWingContainer& InWingContainer, int32 BrevetRank, int32 BrevetNodeNum, const FAncientRelicArray& RelicInfos, const TMap<int64, FB2Totem>& Totems, CombatStatInfoRawDatas& OutCombatStatInfo)
	{
		//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
		//EItemPrimaryPointType CheckPrimaryPointType = ConvertItemOptionToItemPrimaryPointType(OptionType);
		//
		//// PVP 고유 옵션 체크
		//EItemOption PVPModeItemOption = ConvertItemPrimaryPointTypeToItemOptionByPVPMode(CheckPrimaryPointType, ModeType);

		//// 아이템 옵션
		//for (auto& Item : AllEquipped)
		//{
		//	// 기본능력치
		//	
		//	// 고유옵션 (결투의 장신구)
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

		//	//각성 옵션
		//	for (auto& CurrOption : Item.SealOptions)
		//	{
		//		if (CurrOption.OptionInfo.OptionType == PVPModeItemOption)
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_SealOption, CurrOption.OptionInfo.RawOptionAmount);
		//	}

		//	// 랜덤(일반)옵션
		//}

		//// 날개 옵션
		////if (InWingContainer.bHasWing)
		//{
		//	// 기본능력치
		//}

		//// 진급 옵션(공 방 체)
		////if (BrevetRank != 0)
		//{
		//}

		//// 아이템 세트효과
		////TMap<int32, int32>	ItemSetMap;
		////GetSetItemValue(AllEquipped, ItemSetMap);
		////아이템 세트 검사
		////for (auto& ItemSetPair : ItemSetMap)
		//{
		//}

		////고대유물
		//RelicManager RelicMGR(GLOBALRELICMANAGER);
		////if (&RelicMGR) //지역에서 static으로 싱글톤 생성하는거라 무조건 신뢰 해야 할듯...
		//{
		//	int32 nRelicId, nGrade, nLevel;

		//	for (FAncientRelic RelicInfo : RelicInfos)
		//	{
		//		nRelicId = RelicInfo.nRelicId; nGrade = RelicInfo.nRelicGrade;  nLevel = RelicInfo.nRelicLevel;

		//		// 기본능력치
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

		//// 토템에 붙을 수 있는 Option 목록
		//EItemOption TotemModeItemOption = ConvertItemPrimaryPointTypeToItemOptionByTotemMode(CheckPrimaryPointType, ModeType);

		//for (const TPair<int64, FB2Totem>& Elem : Totems)
		//{
		//	if (TotemModeItemOption != EItemOption::EIO_End)
		//	{
		//		// 장비 공격력, 방어력, 체력
		//		if (Elem.Value.MainOptionDetail.OptionType == TotemModeItemOption)
		//		{
		//			OutCombatStatInfo.AddPrimaryPointValue(ECharStatApplyType::ECSA_NormalPoint, Elem.Value.MainOptionDetail.RawOptionAmount);
		//		}

		//		// 장비 추가 옵션
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

		//		// 제련 보너스 효과
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


	/** UnitedOption 의 핵심 정보인 EItemOption 과 ESkillOption 에 대한 맵핑 정보. Const 로 만들고 싶었지만 그렇게 하지 못한 lookup 테이블 용도.
	 * 직접 사용하지 않고 Get**OptionOfUnitedOption 을 사용. */
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
			//{ // 둘 사이에 CombatOptionApplyType 이 필히 동일해야 계산된 걸 똑같이 적둉하든지 할 거다. 다른 게 있다면 한쪽을 손질
			//	checkSlow(GetCombatOptionApplyType(MappedItemOption) == GetCombatOptionApplyType(MappedSkillOption));
			//}
		}

		checkSlow(FoundItemOptionNum == static_cast<int32>(EItemOption::EIO_End)); // 여긴 다 포함되어야
		// 0 아닌 1 부터 시작해서 -1, 자체 성격상 포함 안되는 거 -4, ActiveSkill 로만 적용되는 거라 포함 안되는 거 -14 (기획에 따라 변경될 수 있음.)
		checkSlow(FoundSkillOptionNum == static_cast<int32>(ESkillOption::ESO_End) - 1 - 4 - 14);
	}
#endif
	void InitializeUnitedOptionMapping() // 이걸 모듈 초기화 등 어딘가에서 한번 이상 콜 해서 맵핑 정보를 채워넣어야 한다.
	{
		// 실제 맵핑 코드는 의도적으로 관련 디파인 모아놓은 특정 헤더 파일에 위치시킴.
		INITIALIZE_UNITED_OPTION_MAPPING_BLOCK(UnitedOptionMappingTable, ItemOptionToUnitedOptionMappingTable, SkillOptionToUnitedOptionMappingTable, MercenarySkillOptionToUnitedOptionMappingTable);
#if !UE_BUILD_SHIPPING
		CheckUnitedOptionMappingValidity(); // Check for possible miss..
#endif
	}
	void ConditionalInitializeUnitedOptionMapping()
	{  // 아마 에디터만 그런 거 같긴 한데 모듈이 아예 다시 뜨는지 한번 Initialize 한 후에 비어버리는 일이 발생해서 이걸 준비.
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
				
			// 1. 장착된것 중에 최소레벨 이상인것들을 리스트화
			TArray<int32> ApplyItemLevelList;
			for (auto& EquipItem : AllEquipped)
			{
				if (EquipItem.EnhanceLevel >= MinEnhanceLevel)
					ApplyItemLevelList.Add(EquipItem.EnhanceLevel);
			}
	
			// 2. 리스트 수가 최소적용 수보다 안될때는 적용안함
			if (ApplyItemLevelList.Num() == 0 || ApplyItemLevelList.Num() < MIN_COUNT_ENHANCE_ITEM_SET_EFFECT)
				return 0;
	
			// 3. 내림차순 정렬후 MinEnhanceLevel 인덱스가 최소레벨
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
		// 아이템, 날개, 진급, 유물 등에서 사용하는 EItemOption 으로 구분되는 옵션말고 ESkillOption 으로 구분되는 거.
		ICharacterDataStore* FinalUsedCDS = InCharacterData ? InCharacterData : &BladeIIGameImpl::GetLocalCharacterData();

		TArray<int32> AllFoundSkillID;
		// 여기서의 사용 의미상 PassiveSkill 만 가져온다. ActiveSkill 의 해당 스킬에 적용되는 옵션 상태값은 별도로 처리.
		FinalUsedCDS->GetCharacterPassiveSkills(InPCClass, AllFoundSkillID);

		// 캐릭터가 가지고 있는 스킬옵션들에서 InOptionType 에 해당하는 값들을 긁어모음.
		for (int32 ThisSkillID : AllFoundSkillID)
		{
			// 해당 스킬 ID 와 레벨에서의 스킬옵션들.
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

	//void GetSetItemValue(const TArray<FB2Item>& AllEquipped, CheckInventoryType ENum option으로 인벤 타입을 넣어서 같은거일 경우만 넣게 해주면 됨 TMap<int32, int32>& OutSetItemDatas,
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

		auto GuildSkillInfo = InCharacterDataStore->GetGuildSkillInfos();		//현재가지고있는 길드스킬 가져온다

		for (auto GuildSkillInfoSlot : GuildSkillInfo)
		{
			if (GuildSkillInfoSlot == nullptr)
				continue;

			//if (GuildSkillInfoSlot->level)				//레벨0이들어올수도 있을수 있다
			//{
			//	auto GuildSkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillInfoSlot->id, GuildSkillInfoSlot->level);	// 스킬에대한 마스터 데이터를 가져온다

			//	if (!GuildSkillMasterData || (!IsGuildBattle && !GuildSkillMasterData->buff_time_sec))
			//		continue;

			//	//길드전이면 시간값 체크 안함
			//	if (!IsGuildBattle)
			//	{
			//		float RemainTime = UB2UIManager::RemainTimeFunction(GuildSkillInfoSlot->buff_end_time);

			//		if (RemainTime <= 0)		//시간이 지났으면 값으 0을 반환
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

		auto GuildSkillInfo = InCharacterDataStore->GetGuildSkillInfos();		//현재가지고있는 길드스킬 가져온다

		for (auto GuildSkillInfoSlot : GuildSkillInfo)
		{
			if (GuildSkillInfoSlot == nullptr)
				continue;

			if (GuildSkillInfoSlot->level)				//레벨0이들어올수도 있을수 있다
			{
				auto GuildSkillMasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkillInfoSlot->id, GuildSkillInfoSlot->level);	// 스킬에대한 마스터 데이터를 가져온다

				//if (!GuildSkillMasterData || !GuildSkillMasterData->buff_time_sec)
				//	continue;

				//float RemainTime = UB2UIManager::RemainTimeFunction(GuildSkillInfoSlot->buff_end_time);

				//if (RemainTime <= 0)		//시간이 지났으면 값으 0을 반환
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


// Core of GetEffectiveOptionValueTempl. SkillOption 이랑 ItemOption 통합 건 완료되면 이거랑 관련 함수들 위치 좀 정리.
float GetEffectiveOptionValueInternal(ECombatOptionApplyType InOptionApplyType,
	TArray<FOptionValue>& InSourceValues, // Raw (displayed) values if bRawValueSource is true, effective scaled values if false
	bool bRawValueSource, /*= true*/ // Designate whether InSourceValues are raw values or effective scaled values.
	bool bClampValue /*= false*/) // 값에 최대 최소값 보정이 들어간다.
{
	// Initial value should be different depend on whether it is add or (1 minus) multiplication.
	float FinalEffectiveValue = GetInitialValueOfCombatOptionApplyType(InOptionApplyType);

	// ECombatOptionApplyType 에 따라 모아진 값들을 합치는 operator 를 제공하는 것이 이 함수의 역할이다. 
	// 합친다는 게 더하기를 의미하지만은 않는다.
	for (FOptionValue SourceValue : InSourceValues)
	{
		// 0.0 ~ 1.0 scaling if SourceValue if raw. Need clamping for multiplication type.
		float EffectiveValue = (bRawValueSource) ? GetEffectiveValueForOptionApplyType(InOptionApplyType, SourceValue) : SourceValue.RawOptionAmount;

		if (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply)
		{ // Now it is about how to apply, as its type name implies.
			FinalEffectiveValue *= EffectiveValue;
		}
		else // Add_Value, Add_Percent, Subtract_Value 의 경우는 실 적용시에는 - 해 주어야..
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

	//  능력치 증감 계산을 끝낸 후 상한에 제한을 건다.
	if (bClampValue)
	{
		if (InOptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply)
		{
			// ex > 보스 몹 데미지 감소 옵션이 역으로 적용되더라도 기존보다 더 아프게 들어오진 않도록 한다.
			FinalEffectiveValue = FMath::Clamp(FinalEffectiveValue, 0.0f, 1.0f);
		}
		else
		{
			// ex > 체력 감소 옵션이 아무리 발려있어도 0보다 낮아지지 않는다.
			FinalEffectiveValue = FMath::Max(FinalEffectiveValue, 0.0f);
		}
	}

	return FinalEffectiveValue;
}