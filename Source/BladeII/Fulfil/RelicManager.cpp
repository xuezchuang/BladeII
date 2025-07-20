
#include "RelicManager.h"
#include "Event.h"
#include "InternalEvents.h"
#include "B2LobbyInventory.h"

#include "BladeIIGameImpl.h"

void RelicManager::Init()
{
	SubscribeEvents();
}

void RelicManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryMasterDataClass<FB2MasterDatas>::GetInstance().Subscribe2(
		[this](const FB2MasterDatas& data)
	{
		this->ReceiveRelicMasterData(data);
	}
	));

	Issues.Add(DeliveryGetAccountRelicClass<FB2ResponseGetAccountRelicPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseGetAccountRelicPtr& data)
	{
		if (data->relics.Num() > 0)
			this->SetMyActiveRelicsByClass(SvrToCliPCClassType(data->relics[0]->character_type), data);
	}
	));
}

void RelicManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void RelicManager::ReceiveRelicMasterData(const FB2MasterDatas& MasterData)
{
	//MD_AncientRelic.Empty();

	//const TArray<b2network::B2mdRelicInfoPtr>& RelicInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_relic_infos_index, MasterData);
	//for (const b2network::B2mdRelicInfoPtr& ThisInfo : RelicInfo)
	//{
	//	FMD_AncientRelicSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;

	//	if (!MD_AncientRelic.Find(NewCliMDElem.nRelicId))
	//		MD_AncientRelic.Add(NewCliMDElem.nRelicId, NewCliMDElem);
	//}

	////mdRelicGradeInfo
	//int32 MaxGrade = 0;
	//const TArray<b2network::B2mdRelicGradeInfoPtr>& RelicGradeInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_relic_grade_infos_index, MasterData);
	//for (const b2network::B2mdRelicGradeInfoPtr& ThisInfo : RelicGradeInfo)
	//{
	//	FMD_AncientRelicGradeElem Data(ThisInfo->relic_id, ThisInfo->grade,
	//		FItemOption(SvrToCliOptionType(ThisInfo->option_id_1), static_cast<float>(ThisInfo->option_value_1)),
	//		FItemOption(SvrToCliOptionType(ThisInfo->option_id_2), static_cast<float>(ThisInfo->option_value_2)),
	//		FItemOption(SvrToCliOptionType(ThisInfo->option_id_3), static_cast<float>(ThisInfo->option_value_3)),
	//		ThisInfo->promotion_rate, ThisInfo->promotion_gold, ThisInfo->boss_piece_id, ThisInfo->promotion_boss_piece);

	//	//捞 鉴辑绰 辑滚 单捞磐甫 脚汾 窍扁肺 沁栏聪... 趣矫 酒匆锭甫 措厚窍咯
	//	BII_CHECK(Data.nOption_Attack.OptionType == EItemOption::EIO_Offense_IncAttack);
	//	BII_CHECK(Data.nOption_Defence.OptionType == EItemOption::EIO_Defense_IncDefense);
	//	BII_CHECK(Data.nOption_Health.OptionType == EItemOption::EIO_Health_IncHealth);

	//	if (MD_AncientRelic.Num() < ThisInfo->relic_id)
	//		break;

	//	MD_AncientRelic[ThisInfo->relic_id].MD_AncientRelicGrade.Add(ThisInfo->grade, Data);

	//	MaxGrade = FMath::Max(MaxGrade, ThisInfo->grade);
	//}

	//MAX_RELIC_GRADE = MaxGrade;

	////mdRelicLevelInfo
	//const TArray<b2network::B2mdRelicLevelInfoPtr>& RelicLevelInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_relic_level_infos_index, MasterData);
	//for (const b2network::B2mdRelicLevelInfoPtr& ThisInfo : RelicLevelInfo)
	//{
	//	FMD_AncientRelicLevelElem Data(ThisInfo->relic_id, ThisInfo->enhance_level,
	//		FItemOption(SvrToCliOptionType(ThisInfo->mode_option_id_1), static_cast<float>(ThisInfo->mode_option_value_1)),
	//		FItemOption(SvrToCliOptionType(ThisInfo->mode_option_id_2), static_cast<float>(ThisInfo->mode_option_value_2)),
	//		FItemOption(SvrToCliOptionType(ThisInfo->mode_option_id_3), static_cast<float>(ThisInfo->mode_option_value_3)),
	//		ThisInfo->enhance_rate, ThisInfo->need_gold, ThisInfo->need_hero_piece);

	//	if (MD_AncientRelic.Num() < ThisInfo->relic_id)
	//		break;

	//	MD_AncientRelic[ThisInfo->relic_id].MD_AncientRelicLevel.Add(ThisInfo->enhance_level, Data);
	//}
}

int32 RelicManager::GetRelicNum()
{
	return MD_AncientRelic.Num();
}

int32 RelicManager::getRelicOpenStageID(int32 nRelicId) {

	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	FMD_AncientRelicSingleElem* AncientRelicSingleElem = MD_AncientRelic.Find(nRelicId);

	FServerStageID StageID = FServerStageID(AncientRelicSingleElem->nOpenStage);

	return StageDataStore.GetClientStageId(StageID);
}

const FMD_AncientRelicLevelElem* RelicManager::getRelicLevelInfo(int32 nRelicId, int32 nLevel)
{
	FMD_AncientRelicSingleElem* FindElem(MD_AncientRelic.Find(nRelicId));

	if (FindElem)
	{
		FMD_AncientRelicLevelElem* ElemItem = FindElem->MD_AncientRelicLevel.Find(nLevel);

		if (ElemItem)
			return ElemItem;
	}

	FMD_AncientRelicLevelElem* ElemItem = new FMD_AncientRelicLevelElem();
	return ElemItem;
}

const FMD_AncientRelicGradeElem* RelicManager::getRelicGradeInfo(int32 nRelicId, int32 nGrade)
{
	FMD_AncientRelicSingleElem* FindElem = MD_AncientRelic.Find(nRelicId);

	if (FindElem)
	{
		FMD_AncientRelicGradeElem* ElemItem = FindElem->MD_AncientRelicGrade.Find(nGrade);

		if (ElemItem)
			return ElemItem;
	}

	return nullptr;
}

TArray<float> RelicManager::GenerateLevelOptionValue(int32 nRelicID, int32 nGrade, int32 nLevel)
{
	const FMD_AncientRelicLevelElem* MaxLevelElem = getRelicLevelInfo(nRelicID, MAX_RELIC_LEVEL);

	TArray<float> TempStatArray;
	if (MaxLevelElem)
	{
		float nLevelTempStat1 = 0;
		float nLevelTempStat2 = 0;
		float nLevelTempStat3 = 0;

		nLevelTempStat1 = (MaxLevelElem->nModeOption_Attack.RawOptionAmount)*(nGrade - 1); // 秦寸 殿鞭阑 力寇茄 殿鞭狼 饭骇 弥措蔼阑 阿阿 歹窍绊,
		if (nLevel != 0) // 0老订 公矫
			nLevelTempStat1 += (getRelicLevelInfo(nRelicID, nLevel)->nModeOption_Attack.RawOptionAmount); // 秦寸 殿鞭狼 饭骇 蔼阑 歹秦淋.
		TempStatArray.Add(nLevelTempStat1);

		nLevelTempStat2 = (MaxLevelElem->nModeOption_Defence.RawOptionAmount)*(nGrade - 1);
		if (nLevel != 0)
			nLevelTempStat2 += (getRelicLevelInfo(nRelicID, nLevel)->nModeOption_Defence.RawOptionAmount);
		TempStatArray.Add(nLevelTempStat2);

		nLevelTempStat3 = (MaxLevelElem->nModeOption_Health.RawOptionAmount)*(nGrade - 1);
		if (nLevel != 0)
			nLevelTempStat3 += (getRelicLevelInfo(nRelicID, nLevel)->nModeOption_Health.RawOptionAmount);
		TempStatArray.Add(nLevelTempStat3);
	}

	return TempStatArray;
}

TArray<float> RelicManager::GenerateGradeOptionValue(int32 nRelicID, int32 nGrade)
{
	TArray<float> TempStatArray;
	const FMD_AncientRelicGradeElem* TempElem = getRelicGradeInfo(nRelicID, nGrade);

	if (TempElem)
	{
		TempStatArray.Add(TempElem->nOption_Attack.RawOptionAmount);
		TempStatArray.Add(TempElem->nOption_Defence.RawOptionAmount);
		TempStatArray.Add(TempElem->nOption_Health.RawOptionAmount);
	}

	return TempStatArray;
}

bool RelicManager::GetIsReddotRelicByClass(int32 nClass)
{
	FAncientRelicArray* TotalRelic = BladeIIGameImpl::GetLocalCharacterData().GetRelicInfo(IntToPCClass(nClass));

	int32 CurrentGold = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

	for (auto RelicItem : *TotalRelic) // 泅犁 坷锹等 蜡拱狼 沥焊.
	{
		if (RelicItem.nRelicLevel < MAX_RELIC_LEVEL) // 饭骇诀 啊瓷 眉农
		{
			const FMD_AncientRelicLevelElem* LevleUpInfo = getRelicLevelInfo(RelicItem.nRelicId, RelicItem.nRelicLevel + 1); //饭骇诀 鞘夸炼扒 八祸

			if (LevleUpInfo)
			{
				int32 CurrnetHeroPiece = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);

				bool bIsCheckGold = (LevleUpInfo->nEnhance_Gold <= CurrentGold);
				bool bIsCheckHeroPiece = (LevleUpInfo->nNeed_HeroPiece <= CurrnetHeroPiece);

				if (bIsCheckGold && bIsCheckHeroPiece)
					return true;
			}
		}
		else // 殿鞭诀 啊瓷 眉农
		{
			if (RelicItem.nRelicGrade < MAX_RELIC_GRADE)
			{
				const FMD_AncientRelicGradeElem* GradeUpInfo = getRelicGradeInfo(RelicItem.nRelicId, RelicItem.nRelicGrade + 1); //殿疤诀 鞘夸炼扒 八祸

				if (GradeUpInfo)
				{
					int32 CurrnetBossPiece = UB2LobbyInventory::GetSharedConsumableAmountOfType(GradeUpInfo->nPromotion_BossPieceid);

					bool bIsCheckGold = (GradeUpInfo->nPromotion_Gold <= CurrentGold);
					bool bIsCheckBossPiece = (GradeUpInfo->nPromotion_BossPiece <= CurrnetBossPiece);

					if (bIsCheckGold && bIsCheckBossPiece)
						return true;
				}
			}
		}
	}

	return false;
}

bool RelicManager::GetIsReddotByRelicId(EPCClass nClass, int32 nRelicID)
{
	if (nClass == EPCClass::EPC_End) return false;

	FAncientRelicArray* TotalRelic = BladeIIGameImpl::GetLocalCharacterData().GetRelicInfo(nClass);

	int32 CurrentGold = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

	if (!TotalRelic) return false;

	for (auto RelicItem : *TotalRelic) // 泅犁 坷锹等 蜡拱狼 沥焊.
	{
		if (nRelicID == RelicItem.nRelicId)
		{
			if (RelicItem.nRelicLevel < MAX_RELIC_LEVEL) // 饭骇诀 啊瓷 眉农
			{
				const FMD_AncientRelicLevelElem* LevleUpInfo = getRelicLevelInfo(RelicItem.nRelicId, RelicItem.nRelicLevel + 1); //饭骇诀 鞘夸炼扒 八祸

				if (LevleUpInfo)
				{
					int32 CurrnetHeroPiece = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);

					bool bIsCheckGold = (LevleUpInfo->nEnhance_Gold <= CurrentGold);
					bool bIsCheckHeroPiece = (LevleUpInfo->nNeed_HeroPiece <= CurrnetHeroPiece);

					if (bIsCheckGold && bIsCheckHeroPiece)
						return true;
				}
			}
			else // 殿鞭诀 啊瓷 眉农
			{
				if (RelicItem.nRelicGrade < MAX_RELIC_GRADE)
				{
					const FMD_AncientRelicGradeElem* GradeUpInfo = getRelicGradeInfo(RelicItem.nRelicId, RelicItem.nRelicGrade + 1); //殿疤诀 鞘夸炼扒 八祸

					if (GradeUpInfo)
					{
						int32 CurrnetBossPiece = UB2LobbyInventory::GetSharedConsumableAmountOfType(GradeUpInfo->nPromotion_BossPieceid);

						bool bIsCheckGold = (GradeUpInfo->nPromotion_Gold <= CurrentGold);
						bool bIsCheckBossPiece = (GradeUpInfo->nPromotion_BossPiece <= CurrnetBossPiece);

						if (bIsCheckGold && bIsCheckBossPiece)
							return true;
					}
				}
			}
		}
	}

	return false;
}

void RelicManager::SetMyActiveRelicsByClass(EPCClass nClass, FB2ResponseGetAccountRelicPtr msgPtr)
{
	BladeIIGameImpl::GetLocalCharacterData().SetRelicInfo(nClass, msgPtr);// 泅犁 坷锹等 蜡拱狼 沥焊.
}

float RelicManager::GetPCAttackByMode(EPCClass InPCClass, EB2GameMode ModeType, int32 nRelicId, int32 nGrade, int32 nLevel)
{
	if (InPCClass == EPCClass::EPC_End)
		return 0.0f;

	//咯扁俊 葛靛甫 罐酒客辑 眠啊
	float attack = 0.0f;
	int32 castMode = ConvertModeNameToEApplyMode(ModeType);
	if (castMode == 0 || castMode != nRelicId) return attack;//EApplyMode_None篮 秦临 鞘夸 绝澜.

	TArray<float> OptionValues = GenerateLevelOptionValue(nRelicId, nGrade, nLevel);

	if (OptionValues.IsValidIndex(0))
	{
		attack = OptionValues[0];
	}

	return attack;
}

float RelicManager::GetPCDefenseByMode(EPCClass InPCClass, EB2GameMode ModeType, int32 nRelicId, int32 nGrade, int32 nLevel)
{
	if (InPCClass == EPCClass::EPC_End)
		return 0.0f;

	//咯扁俊 葛靛甫 罐酒客辑 眠啊
	float defense = 0.0f;
	int32 castMode = ConvertModeNameToEApplyMode(ModeType);
	if (castMode == 0 || castMode != nRelicId) return defense;//EApplyMode_None篮 秦临 鞘夸 绝澜.

	TArray<float> OptionValues = GenerateLevelOptionValue(nRelicId, nGrade, nLevel);

	if (OptionValues.IsValidIndex(1))
	{
		defense = OptionValues[1];
	}

	return defense;
}

float RelicManager::GetPCHealthByMode(EPCClass InPCClass, EB2GameMode ModeType, int32 nRelicId, int32 nGrade, int32 nLevel)
{
	if (InPCClass == EPCClass::EPC_End)
		return 0.0f;

	//咯扁俊 葛靛甫 罐酒客辑 眠啊
	float health = 0.0f;
	int32 castMode = ConvertModeNameToEApplyMode(ModeType);
	if (castMode == 0 || castMode != nRelicId) return health;//EApplyMode_None篮 秦临 鞘夸 绝澜.
	
	TArray<float> OptionValues = GenerateLevelOptionValue(nRelicId, nGrade, nLevel);

	if (OptionValues.IsValidIndex(2))
	{
		health = OptionValues[2];
	}

	return health;
}

/*
#define ConvertEModeNameToEApplyMode(EnumName)	\
case EB2GameMode::##EnumName:			\
mode = EApplyMode_##EnumName;		\
break;
*/

int32 RelicManager::ConvertModeNameToEApplyMode(EB2GameMode ModeType)
{
	EApplyMode mode = EApplyMode_None;

	switch (ModeType)
	{
		/*
		ConvertEModeNameToEApplyMode(PVP_Tag)
		ConvertEModeNameToEApplyMode(HeroTower)
		ConvertEModeNameToEApplyMode(Control)
		ConvertEModeNameToEApplyMode(Raid)
		ConvertEModeNameToEApplyMode(Guild)
		/*/
	case EB2GameMode::PVP_Tag:
		mode = EApplyMode_PVP_Tag;
		break;

	case EB2GameMode::HeroTower:
		mode = EApplyMode_HeroTower;
		break;

	case EB2GameMode::Control:
		mode = EApplyMode_Control;
		break;

	case EB2GameMode::Raid:
		mode = EApplyMode_Raid;
		break;

	case EB2GameMode::Guild:
		mode = EApplyMode_Guild;
		break;

	case EB2GameMode::PVP_Team:
		mode = EApplyMode_PVP_Team;
		break;
		//*/
		//鞘夸窍搁 眠啊 秦辑 静绰 率栏肺....
	case EB2GameMode::Scenario:
	case EB2GameMode::CounterDungeon:
	case EB2GameMode::Tutorial:
	case EB2GameMode::Lobby:
	case EB2GameMode::Unknown:
	default:
		break;
	}

	return static_cast<int32>(mode);
}

/*
void RelicManager::RequestAllRelicInfo()
{
for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
{
data_trader::Retailer::GetInstance().RequestGetAccountRelic(CliToSvrPCClassType(IntToPCClass(PCI)));
}
}*/
