// Fill out your copyright notice in the Description page of Project Settings.
#include "B2Airport.h"
//#include "BladeII.h"

#include "CommonStruct.h"
#include "B2WingInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2PCClassInfo.h"
#include "Retailer.h"
#include "B2UIManager_Lobby.h"
#include "B2UIManager.h"
#include "B2LobbyUI_WingMain.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIUpgradeStatNotify.h"
#include "B2UIShortagePopupManager.h"
#include "RelicManager.h"
#include "B2Inventory.h"

#include "Global.h"
#include "BladeIIGameImpl.h"
#include "B2CodeTable.h"


#define SHOW_WING_ON_PLAYERMESH_INI_KEY TEXT("ShowWingOnPlayerMesh")
#define B2AIRPORT_INI_SECTION TEXT("Airport")

void FPCWingContainer::SetHasWingByWingData()
{
	// 朝俺甫 啊瘤绊 乐绰瘤 咯何啊 蝶肺 橇肺配妮俊 器窃瞪 荐档 乐变 茄单.. 老窜 捞犯霸.
	bHasWing = (WingData.EvolutionGrade >= MIN_WING_EVOLUTION_GRADE && WingData.EnhanceLevel >= MIN_WING_ENHANCE_LEVEL);
}

FWingMD_EnhanceSingleElem& FWingMD_EnhanceSingleElem:: operator=(const b2network::B2mdWingEnhancePtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetGrade = ServerStruct->grade;
		this->TargetLevel = ServerStruct->level;

		// 捞傈俊 捞固 持绢躇栏聪 犬牢 瞒盔俊辑.. 购啊 狼固乐绢 焊捞绰 蔼牢单 促福搁 官诧芭绊 酒聪搁 泪矫 肋给等 芭
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingEnhanceResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingEnhanceResourceRefID();	// 傲 捞吧肺 敬促.. ぇぇ

		this->RequiredItemCount = ServerStruct->need_item_count;
		this->EnhancePointToAcquirePerTrial = ServerStruct->enhance_point;
		this->RequiredEnhancePoint = ServerStruct->need_enhance_point;
		this->RequiredGoldCost = ServerStruct->gold_cost;

		// option_type 篮 老窜 救 焊绰 吧肺..
		this->AttackPoint = ServerStruct->option_value1;
		this->DefensePoint = ServerStruct->option_value2;
		this->HealthPoint = ServerStruct->option_value3;
	}

	return *this;
}

FWingMD_EvolveSingleElem& FWingMD_EvolveSingleElem::operator = (const b2network::B2mdWingUpgradePtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetGrade = ServerStruct->grade;

		// 捞傈俊 捞固 持绢躇栏聪 犬牢 瞒盔俊辑. 购啊 狼固乐绢 焊捞绰 蔼牢单 促福搁 官诧芭绊 酒聪搁 泪矫 肋给等 芭
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingEvolveResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingEvolveResourceRefID(); // 傲 捞吧肺 敬促 ぇぇ

		this->RequiredItemCount = ServerStruct->need_item_count;
		this->RequiredGoldCost = ServerStruct->gold_cost;
		this->SuccessRate = (float)ServerStruct->rate * 0.0001f; // 辑滚俊辑 柯 rate 绰 父盒啦捞扼 牧滚泼秦 淋.
		this->MaxFailPoint = ServerStruct->fail_point_max;

		// option_type 篮 老窜 救 焊绰 吧肺..
		this->AttackPoint = ServerStruct->option_value1;
		this->DefensePoint = ServerStruct->option_value2;
		this->HealthPoint = ServerStruct->option_value3;
	}

	return *this;
}

FWingMD_PropOptionSingleElem& FWingMD_PropOptionSingleElem::operator = (const b2network::B2mdWingOptionValuePtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // 辑滚俊急 1何磐 矫累窍绰 蔼捞 咳 
		this->TargetOptionLevel = ServerStruct->level;
		this->OptionType = SvrToCliOptionType(ServerStruct->option_type);
		this->OptionValue = ServerStruct->option_value;

		// 捞傈俊 捞固 持绢躇栏聪 犬牢 瞒盔俊辑.. 购啊 狼固乐绢 焊捞绰 蔼牢单 促福搁 官诧芭绊 酒聪搁 泪矫 肋给等 芭
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID(); // 傲 捞吧肺 敬促.. ぇぇ

		this->RequiredItemCount = ServerStruct->need_item_count;
	}

	return *this;
}

FWingMD_PropOptionOpenSingleElem& FWingMD_PropOptionOpenSingleElem::operator = (const b2network::B2mdWingOptionOpenPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // 辑滚俊急 1何磐 矫累窍绰 蔼捞 咳
		this->RequiredWingGrade = ServerStruct->need_grade;
	}
	return *this;
}

FWingMD_PropOptionMaxLevelSingleElem& FWingMD_PropOptionMaxLevelSingleElem::operator = (const b2network::B2mdWingOptionMaxLevelPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // 辑滚俊急 1何磐 矫累窍绰 蔼捞 咳
		this->TargetWingGrade = ServerStruct->wing_grade;
		this->MaxPossibleLevel = ServerStruct->max_level;
	}
	return *this;
}

FMD_HeroTowerFloorSingleElem& FMD_HeroTowerFloorSingleElem::operator=(const b2network::B2mdHeroTowerFloorPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->nFloor = ServerStruct->floor;
		this->nRecommendCombat = ServerStruct->recommend_power;
		this->RecommendAttack = ServerStruct->recommend_attack;
		this->RecommendDefense = ServerStruct->recommend_defense;
		this->nClearLimitTime = ServerStruct->clear_limit_time;

		this->nMapId = ServerStruct->map_id;

		this->nDifficultyScale = ServerStruct->difficulty_scale;

		for (b2network::B2RewardPtr& reward : ServerStruct->first_rewards)
		{
			this->first_rewards.Add(FRewardItem(reward->raw_reward->id, reward->raw_reward->count));   
		}

		for (b2network::B2RewardPtr& reward : ServerStruct->rewards)
		{
			this->rewards.Add(FRewardItem(reward->raw_reward->id, reward->raw_reward->count));
		}
	}

	return *this;
} 

//FMD_AncientRelicSingleElem& FMD_AncientRelicSingleElem::operator=(const b2network::B2mdRelicInfoPtr& ServerStruct)
//{
//	if (ServerStruct)
//	{
//		this->nRelicId = ServerStruct->relic_id;
//		this->nOpenStage = ServerStruct->open_stage_id;
//		// coloceum comment
//	}
//	return *this;
//}

//================================================================================
// For FB2Wing

float FB2Wing::GetEffectivePropOptionValue(EItemOption InOptionType) const
{
	// 厘馒 酒捞袍 率 荐摹 掘绢坷绰 窃荐俊辑 Wing 档 鞍捞 贸府且 巴. (UB2LobbyInventory::GetEquippedOptionPoint)
	// 捞吧 蝶肺 荤侩秦辑 促弗 单辑 掘篮 荐摹客 钦摹妨搁 ECombatOptionApplyType 俊 蝶扼 蚌窍芭唱 歹窍芭唱甫 鞍捞 蝶廉焊酒具 窃.

	TArray<FOptionValue> GatheredOptionRawValue; // 角力肺绰 窍唱父 甸绢啊 乐绢具 且 巴.

	for (const FWingPropOption& ThisOption : PropOptions)
	{
		if (ThisOption.MyOptionType == InOptionType)
		{
			FOptionValue OptionValue = FOptionValue(ThisOption.RawOptionAmount);
			GatheredOptionRawValue.Add(OptionValue);
		}
	}
	// 扁粮 Item 苞 鞍篮 可记 眉拌甫 荤侩..
	return GetEffectiveItemOptionValue(InOptionType, GatheredOptionRawValue);
}

float FB2Wing::GetAttackPoint(bool bCountRelevantOption) const
{
	return AttackPoint + (bCountRelevantOption ? GetExtraPrimPointByRelevantOption(EItemPrimaryPointType::EIPP_Attack) : 0.0f);
}
float FB2Wing::GetDefensePoint(bool bCountRelevantOption) const
{
	return DefensePoint + (bCountRelevantOption ? GetExtraPrimPointByRelevantOption(EItemPrimaryPointType::EIPP_Defense) : 0.0f);
}
float FB2Wing::GetHealthPoint(bool bCountRelevantOption) const
{
	return HealthPoint + (bCountRelevantOption ? GetExtraPrimPointByRelevantOption(EItemPrimaryPointType::EIPP_Health) : 0.0f);
}

float FB2Wing::GetExtraPrimPointByRelevantOption(EItemPrimaryPointType InPrimPointType) const
{
	//const EItemOption RelevantOption = CombatStatEval::GetPrimPointIncOption(InPrimPointType);
	//if (RelevantOption != EItemOption::EIO_End)
	//{
	//	return FMath::Max(GetEffectivePropOptionValue(RelevantOption), 0.0f);
	//}
	return 0.0f;
}

const FWingPropOption* FB2Wing::GetPropOptionPtr(EItemOption InOptionType) const
{
	for (const FWingPropOption& ThisOption : PropOptions)
	{
		if (ThisOption.MyOptionType == InOptionType)
		{
			return &ThisOption;
		}
	}
	return NULL;
}

const FWingPropOption* FB2Wing::GetPropOptionPtr(int32 InIndex) const
{
	return PropOptions.IsValidIndex(InIndex) ? &PropOptions[InIndex] : NULL;
}

const TArray<FWingPropOption>& FB2Wing::GetPropOptionArr() const
{
	return PropOptions;
}

void FB2Wing::UpdateByServerGetWingResponse(const b2network::FB2MessageInfoResponseGetWingPtr& InResponseGetWingPtr)
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateByServerGetWingResponse"));

	// 啊厘 扁夯利牢 辑滚俊辑狼 朝俺 沥焊 罐绰 窃荐. 盔拜 某腐磐 敲饭捞绢狼 版快绰 捞吧肺 罐瘤 臼霸 瞪 巴.

	if (InResponseGetWingPtr)
	{
		OwnerPCClass = PCClassToInt(SvrToCliPCClassType(InResponseGetWingPtr->character_type));
		EvolutionGrade = InResponseGetWingPtr->grade;
		EvolutionFailedCount = InResponseGetWingPtr->upgrade_fail_point;
		EnhanceLevel = InResponseGetWingPtr->level;
		EnhancePoint = InResponseGetWingPtr->enhance_point;

		// bShouldBeVisible 篮 咯扁辑 技泼窍瘤 臼绰促.

		// 咯扁急 可记 单捞磐啊 促 客具 瞪 巴.. 凯妨 乐电 凯妨 乐瘤 臼带 埃俊.
		ensure(InResponseGetWingPtr->wing_option_infos.Num() == PropOptions.Num());
		for (int32 POI = 0; POI < InResponseGetWingPtr->wing_option_infos.Num(); ++POI)
		{
			const b2network::B2WingOptionInfoPtr& ThisServerInfo = InResponseGetWingPtr->wing_option_infos[POI];
			UpdateSinglePropOptionByServerInfo(ThisServerInfo, false);
		}
	}

	// 扁鸥 促弗 单捞磐甸篮 瘤陛鳖瘤 技泼等 芭 扁馆栏肺 付胶磐 抛捞喉辑 啊廉咳.
	UpdateStats();
}

void FB2Wing::UpdateByPartialServerInfo(const b2network::B2CharacterServerInfoPtr& InCharServerInfo, const TArray<b2network::B2WingOptionInfoPtr>& InWingOptionInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateByPartialServerInfo"));

	// 肺拿 某腐磐狼 朝俺 柳拳/碍拳 殿狼 悼累阑 力寇茄 寇包 钎矫唱 傈捧 荐摹 魂沥 殿俊 鞘夸茄 沥焊甸篮 葛滴 捞吧肺 罐阑 荐 乐澜.
	// 檬扁 AccountInfo 俊 器窃等 朝俺 单捞磐 棺 盔拜 某腐磐 朝俺 沥焊甫 捞吧肺 诀单捞飘.

	if (InCharServerInfo)
	{
		OwnerPCClass = PCClassToInt(SvrToCliPCClassType(InCharServerInfo->character_type));
		EvolutionGrade = InCharServerInfo->wing_grade;
		EnhanceLevel = InCharServerInfo->wing_level;
		bShouldBeVisible = InCharServerInfo->wing_visible;

		// EvolutionFailedCount 尔 EnhancePoint 绰 咯扁辑 技泼窍瘤 臼绰促. 滴 蔼篮 傈捧俊辑绰 鞘夸绝绰 蔼烙.
	}
	//  酒流 茄锅档 GetWing 阑 窍瘤 臼篮 某腐磐扼搁 可记 单捞磐啊 救 棵 荐 乐促. 弊霸 酒聪扼搁 扁夯 肮荐俊 嘎苗辑 坷绰 吧肺..
	ensure(InWingOptionInfo.Num() == 0 || InWingOptionInfo.Num() == PropOptions.Num());
	for (int32 POI = 0; POI < InWingOptionInfo.Num(); ++POI)
	{
		const b2network::B2WingOptionInfoPtr& ThisServerInfo = InWingOptionInfo[POI];
		UpdateSinglePropOptionByServerInfo(ThisServerInfo, false);
	}

	UpdateStats();
}

void FB2Wing::UpdateSinglePropOptionByServerInfo(const b2network::B2WingOptionInfoPtr& InServerInfoPtr, bool bUpdateAllImmediate)
{
	if (InServerInfoPtr)
	{
		int32 CliOptionIndex = SvrToCliWingPropOptionIndex(InServerInfoPtr->option_index);
		if (PropOptions.IsValidIndex(CliOptionIndex))
		{ // 辑滚俊急 牢郸胶甫 焊郴林绊 type 篮 付胶磐单捞磐 疯诀栏肺.
			FWingPropOption& MyOptionInfo = PropOptions[CliOptionIndex];
			MyOptionInfo.OptionLevel = InServerInfoPtr->level;
		}
	}
	if (bUpdateAllImmediate)
	{ // 扁鸥 唱赣瘤 单捞磐绰 UpdateStats 甫 芭摹搁辑 付胶磐单捞磐疯诀栏肺 盲况咙. 捞扒 蝶肺 何福霸 且 荐档.
		UpdateStats();
	}
}

bool FB2Wing::UpdateStats()
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateStats"));

	bool bRetVal = true;

	//// EvolveGrade, EnhanceLevel 殿 扁夯 单捞磐啊 瞒 乐促绰 啊沥栏肺 扁鸥 唱赣瘤 瓷仿摹 殿阑 付胶磐 单捞磐俊辑 疯诀秦辑 盲况持绰 巴.

	//AttackPoint = 0.0f;
	//DefensePoint = 0.0f;
	//HealthPoint = 0.0f;

	//FClientDataStore& CDS = BladeIIGameImpl::GetClientDataStore();

	//const FWingMD_EnhanceSingleElem* EnhanceMD = CDS.GetWingMD_Enhance(*this);
	//
	//if (EnhanceMD){
	//	AttackPoint += EnhanceMD->AttackPoint;
	//	DefensePoint += EnhanceMD->DefensePoint;
	//	HealthPoint += EnhanceMD->HealthPoint;
	//}
	//else{
	//	bRetVal = false;
	//}

	//const FWingMD_EvolveSingleElem* EvolveMD = CDS.GetWingMD_Evolve(*this);
	//if (EvolveMD){
	//	AttackPoint += EvolveMD->AttackPoint;
	//	DefensePoint += EvolveMD->DefensePoint;
	//	HealthPoint += EvolveMD->HealthPoint;
	//}
	//else{
	//	bRetVal = false;
	//}

	//for (int32 POI = 0; POI < PropOptions.Num(); ++POI)
	//{
	//	FWingPropOption& ThisPropOption = PropOptions[POI];

	//	const FWingMD_PropOptionSingleElem* PropOptionMD = CDS.GetWingMD_PropOption(*this, POI);
	//	if (PropOptionMD){
	//		ThisPropOption.MyOptionType = PropOptionMD->OptionType;
	//		ThisPropOption.RawOptionAmount = PropOptionMD->OptionValue;
	//	}
	//	else{
	//		bRetVal = false;
	//	}

	//	const FWingMD_PropOptionOpenSingleElem* PropOptionOpenMD = CDS.GetWingMD_PropOptionOpenCondition(*this, POI);
	//	if (PropOptionOpenMD){
	//		ThisPropOption.RequiredWingEvolveGrade = PropOptionOpenMD->RequiredWingGrade;
	//		ThisPropOption.bIsOpen = (EvolutionGrade >= ThisPropOption.RequiredWingEvolveGrade);
	//	}
	//	else{
	//		bRetVal = false;
	//	}

	//	const FWingMD_PropOptionMaxLevelSingleElem* PropOptionMaxLvMD = CDS.GetWingMD_PropOptionMaxLevel(*this, POI);
	//	if (PropOptionMaxLvMD){
	//		ThisPropOption.MaxOptionLevel = PropOptionMaxLvMD->MaxPossibleLevel;
	//		// 酒付档 抛胶飘 扁瓷俊辑唱 捞霸 鞘夸茄 惑炔捞 唱棵 芭 鞍变 窍瘤父..
	//		if (ThisPropOption.MaxOptionLevel > 0){ // 弊府绊 秦寸 可记捞 摧囚 乐绰 殿鞭俊辑绰 捞霸 0 老 荐 乐绰单 0栏肺 努伐橇 登搁 巩力啊 登聪 弊扒 出狄. 
	//			ThisPropOption.OptionLevel = FMath::Min(ThisPropOption.OptionLevel, ThisPropOption.MaxOptionLevel);
	//		}
	//	}
	//	else{
	//		bRetVal = false;
	//	}
	//}

	return bRetVal;
}

FText GetLOCTextOfWingName(EPCClass InPCClass, int32 InEvolveGrade)
{
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	FString TextKeyString(TEXT("WingName_"));

	switch (InPCClass)
	{
	case EPCClass::EPC_Gladiator: TextKeyString += TEXT("Gladiator_"); break;
	case EPCClass::EPC_Assassin: TextKeyString += TEXT("Assassin_"); break;
	case EPCClass::EPC_Wizard: TextKeyString += TEXT("Wizard_"); break;
	case EPCClass::EPC_Fighter: TextKeyString += TEXT("Fighter_"); break;
	}

	TextKeyString += FString::FromInt(InEvolveGrade);

	return BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, *TextKeyString);
}

FText GetLOCTextOfWingPropOption(EItemOption InOption, EPCClass InPCClass)
{
	return GetLOCTextOfItemOption(InOption, InPCClass);
}

#if !UE_BUILD_SHIPPING
void TempSetupWingDataByLimitedData(FPCWingContainer& OutWingContainder, EPCClass InPCClass, int32 InGrade)
{
	// 沥侥栏肺 橇肺配妮 甸绢坷扁 傈鳖瘤 荤侩且 芭..

	if (InGrade >= MIN_WING_EVOLUTION_GRADE)
	{
		OutWingContainder.WingData.OwnerPCClass = PCClassToInt(InPCClass);
		OutWingContainder.WingData.EvolutionGrade = FMath::Min(InGrade, MAX_WING_EVOLUTION_GRADE_UNOFFICIAL);

		// 唱赣瘤 单捞磐绰 利寸洒 盲况持绰单.. 迭洒 歹 颊搭 巴档 绝阑 淀.
		OutWingContainder.WingData.EnhanceLevel = MIN_WING_ENHANCE_LEVEL;
		
		// 咯窍埃 咯扁辑 付胶磐单捞磐肺 腹篮 巴甸阑 盲况持阑 芭聪 弊唱付 Grade 扼档 乐栏搁 结冈阑父 且 淀.
		OutWingContainder.WingData.UpdateStats(); 

		OutWingContainder.bHasWing = true;
	}
	else
	{
		OutWingContainder.bHasWing = false;
	}
}
#endif

//================================================================================
// For UB2Airport

AB2LobbyGameMode* UB2Airport::TransientLobbyGM = NULL;
TMap<EPCClass, bool> UB2Airport::PendingWingVisibilityReq;
bool UB2Airport::bWingUIPageOpenedOnce = false;
UB2Airport::UB2Airport(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bEventSubscribed = false;
	bIsRequestingWingDataForInit = false;
	bIsInAutoEnhancement = false;
	AutoEnhancementPCClass = EPCClass::EPC_End;
	bIsInAutoEvolution = false;
	AutoEvolutionPCClass = EPCClass::EPC_End;
	CachedLastAutoEnhEvolExecTime = 0.0f;

	FMemory::Memzero(FullWingDataReqState, sizeof(FullWingDataReqState));
}

void UB2Airport::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::BeginDestroy"));

	TransientLobbyGM = NULL;

	//UnsubscribeEvents();

	Super::BeginDestroy();
}

void UB2Airport::StaticInit()
{ // ini 肺爹捞 乐菌绰单 喊档 困摹肺 颗败咙..
}

void UB2Airport::StaticOnModuleShutdown()
{
}

void UB2Airport::InitAirportLobby(class AB2LobbyGameMode* InLobbyGM)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::InitAirportLobby"));

	TransientLobbyGM = InLobbyGM;
	//check(TransientLobbyGM && TransientLobbyGM->IsValidObj()); // 按眉 积己篮 LobbyGameMode 俊辑父

	SubscribeEvents();
}

void UB2Airport::ConditionalReqForFullWingData(EPCClass InWingPCClass)
{
	int32 WingPCClassIdx = PCClassToInt(InWingPCClass);
	if (WingPCClassIdx < GetMaxPCClassNum() && !FullWingDataReqState[WingPCClassIdx])
	{ // 捞 版肺甫 烹茄 夸没篮 某腐磐寸 茄锅父 窍搁 凳.
		// 览翠篮 OnReceiveGetWing 栏肺 罐霸 瞪 扒单 FullWingData 扼绊 窍扁俊绰 捞吧肺 技泼窍瘤 臼绰 单捞磐档 乐变 窍瘤父 AccountInfo 甫 烹秦 坷绰 巴俊 厚秦辑绰 full data 俊 啊滨促绊 且 荐 乐摆促..
		data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(InWingPCClass));
	}
}

void UB2Airport::SetWingDataByAccountInfo(const TArray<b2network::B2CharacterServerInfoPtr>& InAllCharInfoArray, const TArray<b2network::B2WingOptionInfoPtr>& InAllWingOptionInfoArray)
{
	// 贸澜俊 AccountInfo 罐栏搁辑 阂府霸 凳. EnhancePoint, EvolutionFailedCount 鞍捞 Wing UI 其捞瘤俊辑父 鞘夸茄 单捞磐绰 咯扁辑 罐瘤 臼霸 瞪 巴.

	for (int32 CI = 0; CI < InAllCharInfoArray.Num(); ++CI)
	{
		const b2network::B2CharacterServerInfoPtr& ThisCharInfo = InAllCharInfoArray[CI];

		// 傈眉 WingOptionInfoArry 吝俊辑 捞锅 某腐磐 巴父 哗咳. 酒流 茄锅档 GetWing 阑 窍瘤 臼篮 某腐磐扼搁 可记 单捞磐啊 救 棵 荐 乐促.
		TArray<b2network::B2WingOptionInfoPtr> WingOptionsForThisChar;
		for (int32 AllInfoIndex = 0; AllInfoIndex < InAllWingOptionInfoArray.Num(); ++AllInfoIndex)
		{
			const b2network::B2WingOptionInfoPtr& ThisOptionInfo = InAllWingOptionInfoArray[AllInfoIndex];
			if (ThisOptionInfo && ThisOptionInfo->character_type == ThisCharInfo->character_type)
			{
				WingOptionsForThisChar.Add(ThisOptionInfo);
			}
		}

		EPCClass ThisWingPCClass = IntToPCClass(CI);
		FPCWingContainer* ThisWingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(ThisWingPCClass);
		if (ThisWingContainder)
		{
			ThisWingContainder->WingData.UpdateByPartialServerInfo(ThisCharInfo, WingOptionsForThisChar);
			ThisWingContainder->SetHasWingByWingData(); // bHasWing 阑 蝶肺 辑滚俊辑 罐酒具 且 芭绰 鞍篮单 老窜 绝栏聪..

			// 捞 矫痢俊辑绰 酒付 鞘荐 付胶磐单捞磐啊 绝阑 芭扼 付胶磐单捞磐 罐篮 饶俊 UpdateStats 甫 促矫 阂矾林绢具 且 巴.
		}
	}
}

void UB2Airport::UpdateWingCombatStatsOnMD()
{
	// UB2Airport::SetWingDataByAccountInfo 啊 阂赴 矫痢俊绰 朝俺 付胶磐单捞磐啊 绝阑 巴捞骨肺 蝶肺 捞犯霸 UpdateStats 阑 阂矾霖促.
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass ThisPCClass = IntToPCClass(PCI);
		FPCWingContainer* ThisWingContainer = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(ThisPCClass);
		if (ThisWingContainer)
		{
			ThisWingContainer->WingData.UpdateStats();
		}
	}
}

void UB2Airport::BeginInitReqForWingData()
{
	// AccountInfo 俊 鞘荐利牢 朝俺 单捞磐啊 绝绰 惑炔俊辑 AccountInfo 尔 MasterData 罐绊 唱辑 檬扁俊 朝俺 单捞磐 老褒 夸没 侩.
	// 瘤陛篮 弊成 for 巩 倒妨辑 葛电 努贰胶俊 措秦 RequestGetWing 朝府搁 瞪 扒单 捞芭 父甸 锭俊绰 夸没阑 茄锅俊 茄锅父 朝妨具 窍绰 力茄捞 乐菌澜.

	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::BeginInitReqForWingData"));

	bIsRequestingWingDataForInit = true;

	// 臂贰叼俊捞磐何磐 秦辑 矫累
	checkSlow((int32)EPCClass::EPC_Gladiator == 0);
	data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(EPCClass::EPC_Gladiator));
}

void UB2Airport::OnReceiveGetWingCheckForContinueReq(const FB2Wing& ReceivedWingData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveGetWingCheckForContinueReq"));

	EPCClass ThisWingPCClass = IntToPCClass(ReceivedWingData.OwnerPCClass);
	EPCClass NextPCClass = IntToPCClass(ReceivedWingData.OwnerPCClass + 1);
		
	if (ThisWingPCClass == EPCClass::EPC_End || NextPCClass == EPCClass::EPC_End)
	{ // 购啊 肋给登菌芭唱 促 罐疽栏聪 咯扁辑 场晨
		bIsRequestingWingDataForInit = false;
	}
	else if (NextPCClass != EPCClass::EPC_End)
	{ // 弊 促澜 req
		data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(NextPCClass));
	}
}

void UB2Airport::SubscribeEvents()
{
	if (bEventSubscribed){
		return;
	}

	CAPTURE_UOBJECT(UB2Airport);

	DeliveryReceiveGetWingTicket = DeliveryReceiveGetWingClass<const FB2Wing&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveGetWing, const FB2Wing& gw)
			Capture->OnReceiveGetWing(gw);
		END_CAPTURE_OBJECT()
		);
	DeliveryReceiveEnhanceWingTicket = DeliveryReceiveEnhanceWingClass<FB2EnhanceWing>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveEnhanceWing, FB2EnhanceWing ew)
			Capture->OnReceiveEnhanceWing(ew);
		END_CAPTURE_OBJECT()
		);
	DeliveryReceiveUpgradeWingTicket = DeliveryReceiveUpgradeWingClass<FB2UpgradeWing>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveUpgradeWing, FB2UpgradeWing uw)
			Capture->OnReceiveUpgradeWing(uw);
		END_CAPTURE_OBJECT()
		);
	DeliveryReceiveEnhanceWingOptionTicket = DeliveryReceiveEnhanceWingOptionClass<FB2EnhanceWingOption>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveEnhanceWingOption, FB2EnhanceWingOption ewo)
			Capture->OnReceiveEnhanceWingOption(ewo);
		END_CAPTURE_OBJECT()
		);
	DeliveryReceiveSetWingVisibleTicket = DeliveryReceiveSetWingVisibleClass<FB2SetWingVisible>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveSetWingVisible, FB2SetWingVisible wv)
			Capture->OnReceiveSetWingVisible(wv);
		END_CAPTURE_OBJECT()
		);
	BeginReqForLobbyInitWingDataTicket = BeginReqForLobbyInitWingDataClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(BeginReqForLobbyInitWingData)
			Capture->BeginInitReqForWingData();
		END_CAPTURE_OBJECT()
		);
	SetLocalWingDataByAccountInfoTicket = SetLocalWingDataByAccountInfoClass<const TArray<b2network::B2CharacterServerInfoPtr>&, const TArray<b2network::B2WingOptionInfoPtr>&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SetLocalWingDataByAccountInfo, const TArray<b2network::B2CharacterServerInfoPtr>& charinfoarray, const TArray<b2network::B2WingOptionInfoPtr>& wingoptioninfoarray)
			Capture->SetWingDataByAccountInfo(charinfoarray, wingoptioninfoarray);
		END_CAPTURE_OBJECT()
		);
	UpdateWingCombatStatsOnMDTicket = UpdateWingCombatStatsOnMDClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UpdateWingCombatStatsOnMD)
			Capture->UpdateWingCombatStatsOnMD();
		END_CAPTURE_OBJECT()
		);

	bEventSubscribed = true;
}

void UB2Airport::UnsubscribeEvents()
{
	if (!bEventSubscribed){
		return;
	}

	DeliveryReceiveGetWingClass<const FB2Wing&>::GetInstance().Unsubscribe(DeliveryReceiveGetWingTicket);
	DeliveryReceiveEnhanceWingClass<FB2EnhanceWing>::GetInstance().Unsubscribe(DeliveryReceiveEnhanceWingTicket);
	DeliveryReceiveUpgradeWingClass<FB2UpgradeWing>::GetInstance().Unsubscribe(DeliveryReceiveUpgradeWingTicket);
	DeliveryReceiveEnhanceWingOptionClass<FB2EnhanceWingOption>::GetInstance().Unsubscribe(DeliveryReceiveEnhanceWingOptionTicket);
	DeliveryReceiveSetWingVisibleClass<FB2SetWingVisible>::GetInstance().Unsubscribe(DeliveryReceiveSetWingVisibleTicket);
	BeginReqForLobbyInitWingDataClass<>::GetInstance().Unsubscribe(BeginReqForLobbyInitWingDataTicket);
	SetLocalWingDataByAccountInfoClass<const TArray<b2network::B2CharacterServerInfoPtr>&, const TArray<b2network::B2WingOptionInfoPtr>&>::GetInstance().Unsubscribe(SetLocalWingDataByAccountInfoTicket);
	UpdateWingCombatStatsOnMDClass<>::GetInstance().Unsubscribe(UpdateWingCombatStatsOnMDTicket);

	bEventSubscribed = false;
}

void UB2Airport::OnReceiveGetWing(const FB2Wing& GetWingData)
{ // 咯扁浚 Response 单捞磐甫 FB2Wing 栏肺 官层辑 逞败淋.

	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveGetWing"));

	EPCClass WingPCClass = IntToPCClass(GetWingData.OwnerPCClass);

	FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
	if (WingContainder)
	{
		// 夸 橇肺配妮肺绰 Visible 咯何啊 坷瘤 臼绊 措脚 捞傈俊 SetWingDataByAccountInfo 俊 狼秦 Visible 捞 刚历 技泼登菌促 摹绊 弊吧 荤侩窍扁 困秦 归诀.
		const bool bPrevVisible = WingContainder->WingData.bShouldBeVisible; 

		WingContainder->WingData = GetWingData;
		// bHasWing 阑 蝶肺 辑滚俊辑 罐酒具 且 芭绰 鞍篮单 老窜 绝栏聪..
		WingContainder->SetHasWingByWingData();

		// 捞傈俊 罐酒初篮 Visible 咯何肺 汗备.
		WingContainder->WingData.bShouldBeVisible = bPrevVisible;

		int32 WingPCClassIdx = PCClassToInt(WingPCClass);
		FullWingDataReqState[WingPCClassIdx] = true; // 绢恫 版肺甫 烹秦 柯 览翠捞扒 茄锅 罐绊 唱搁 歹 捞惑 捞 版肺肺 罐瘤 臼酒档 登骨肺 钎矫.

		UpdateOnWingDataChange(WingPCClass, true);
	}

	if (bIsRequestingWingDataForInit) // 檬扁 老褒 夸没狼 版快
	{
		OnReceiveGetWingCheckForContinueReq(GetWingData);
	}
}

void UB2Airport::OnReceiveEnhanceWing(const FB2EnhanceWing& EnhanceWingData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveEnhanceWing"));

	//EPCClass WingPCClass = SvrToCliPCClassType(GET_TUPLE_DATA(FB2ResponseEnhanceWing::character_type_index, EnhanceWingData));

	//FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
	//if (WingContainder)
	//{
	//	checkSlow(WingContainder->bHasWing);
	//	const int32 PrevEnhanceLevel = WingContainder->WingData.EnhanceLevel;
	//	WingContainder->WingData.EnhanceLevel = GET_TUPLE_DATA(FB2ResponseEnhanceWing::level_index, EnhanceWingData);
	//	WingContainder->WingData.EnhancePoint = GET_TUPLE_DATA(FB2ResponseEnhanceWing::enhance_point_index, EnhanceWingData);

	//	WingContainder->WingData.UpdateStats(); // 付胶磐 单捞磐俊 扁馆茄 傈捧仿 荐摹 殿 诀单捞飘.

	//	BladeIIGameImpl::GetClientDataStore().OnReceiveEnhanceWing(EnhanceWingData); // ClientDataStore 狼 眠啊 贸府. 榜靛尔 犁丰酒捞袍 诀单捞飘 殿

	//	UpdateOnWingDataChange(WingPCClass, !bIsInAutoEnhancement); // UI 棺 皋浆 诀单捞飘 殿. 磊悼碍拳 档吝俊绰 UI 父 诀单捞飘 登霸.
	//			
	//	// UI 率俊 局聪皋捞记 敲饭捞甫 粱 窍绰单 捞巴鳖瘤 Event 父甸绢辑 朝府变 粱 构窍绊 备侥 UI 矫胶袍捞聪 傲 捞犯霸 せ
	//	// 磊悼碍拳俊辑档 老窜 秦 焊磊.
	//	if (WingContainder->WingData.EnhanceLevel > PrevEnhanceLevel)
	//	{ // WingEnhancedAnim 篮 EnhancePoint 啊 酒囱 角力 瓷仿摹啊 函版登绰 EnhanceLevel 函版 矫俊 敲饭捞. 
	//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (TransientLobbyGM && TransientLobbyGM->IsValidObj()) ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//		if (DJLegacyLobbyUIMgr)
	//		{
	//			DJLegacyLobbyUIMgr->PlayWingEnhancedAnim();
	//			LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
	//		}
	//	}

	//	if (bIsInAutoEnhancement)
	//	{ // 磊悼碍拳狼 版快俊 措茄 贸府.
	//		ensure(AutoEnhancementPCClass == WingPCClass); // 磊悼碍拳 档吝俊 狐弗 努腐栏肺 蝶肺 夸没 焊辰促搁 吧副 荐档.. 磊悼碍拳 锭 蝶肺 努腐 给窍霸 秦具 且 淀.

	//		// 老窜篮 磊悼 碍拳 档吝 促弗 努贰胶 朝俺狼 碍拳 览翠捞 吭促搁 磊悼 碍拳甫 吝窜矫虐绰 吧肺 窍磊. 
	//		if (AutoEnhancementPCClass == WingPCClass){
	//			ReserveOrExecSingleAutoEnhancementTask(); // SingleAutoEnhancementTask 俊辑绰 炼扒捞 登搁 沥惑利栏肺 磊悼 碍拳啊 辆丰凳. 趣篮 瘤加 夸没
	//		}
	//		else{
	//			EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Abnormal); // 咯扁肺 坷搁 厚沥惑利牢 磊悼碍拳 辆丰烙.
	//		}
	//	}

	//	//////////////////////////////////////////////////////////////////
	//	//咀记 瘤钎 肺弊 (某腐磐 朝俺 碍拳)
	//	B2ActionLogSender Sender;
	//	Sender.setCategory(FString("CHARACTER"))
	//		  .setAction(FString("WING_ENHANCE"))
	//		  .setLabel(B2ActionLogType<EPCClass>::GetName(WingPCClass))
	//		  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//	//////////////////////////////////////////////////////////////////
	//}
}

void UB2Airport::OnReceiveUpgradeWing(const FB2UpgradeWing& UpgradeWingData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveUpgradeWing"));

//	EPCClass WingPCClass = SvrToCliPCClassType(GET_TUPLE_DATA(FB2ResponseUpgradeWing::character_type_index, UpgradeWingData));
//
//	FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
//	if (WingContainder)
//	{
//		checkSlow(WingContainder->bHasWing);
//
//		const bool bEvolutionSucceeded = GET_TUPLE_DATA(FB2ResponseUpgradeWing::isUpgradeSuccess_index, UpgradeWingData);
//
//#if !UE_BUILD_SHIPPING
//		const int32 EvolGradeBefore = WingContainder->WingData.EvolutionGrade;
//		const int32 FailCountBefore = WingContainder->WingData.EvolutionFailedCount;
//#endif
//		// 己傍茄 版快 柳拳 楷免 搬苞 UI 芒俊 厚背 stat 阑 谎府扁 困秦 诀单捞飘 登扁 傈 单捞磐甫 归诀茄促.
//		if (bEvolutionSucceeded)
//		{
//			TemporaryPreEvolveData = WingContainder->WingData;
//			bPreEvolveDataValid = true; // 柳拳 楷免 唱哎 锭 府悸瞪 巴.
//			
//			//////////////////////////////////////////////////////////////////
//			//咀记 瘤钎 肺弊 (某腐磐 朝俺 铰鞭(柳拳))
//			B2ActionLogSender Sender;
//			Sender.setCategory(FString("CHARACTER"))
//				  .setAction(FString("WING_UPGRADE"))
//				  .setLabel(B2ActionLogType<EPCClass>::GetName(WingPCClass))
//				  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
//			//////////////////////////////////////////////////////////////////
//		}
//
//		WingContainder->WingData.EvolutionGrade = GET_TUPLE_DATA(FB2ResponseUpgradeWing::grade_index, UpgradeWingData);
//		WingContainder->WingData.EnhanceLevel = GET_TUPLE_DATA(FB2ResponseUpgradeWing::level_index, UpgradeWingData);
//		WingContainder->WingData.EvolutionFailedCount = GET_TUPLE_DATA(FB2ResponseUpgradeWing::fail_point_index, UpgradeWingData);
//	
//#if !UE_BUILD_SHIPPING
//		checkSlow( // 抗惑茄 巴苞 老摹窍绰瘤..
//			(bEvolutionSucceeded && (EvolGradeBefore + 1) == WingContainder->WingData.EvolutionGrade) ||
//			(!bEvolutionSucceeded && (FailCountBefore + 1) == WingContainder->WingData.EvolutionFailedCount)
//			);
//#endif
//
//		WingContainder->WingData.UpdateStats(); // 付胶磐 单捞磐俊 扁馆茄 傈捧仿 荐摹 殿 诀单捞飘.
//
//		BladeIIGameImpl::GetClientDataStore().OnReceiveUpgradeWing(UpgradeWingData); // ClientDataStore 狼 眠啊 贸府. 榜靛尔 犁丰酒捞袍 诀单捞飘 殿
//
//		UpdateOnWingDataChange(WingPCClass, bEvolutionSucceeded); // UI 棺 皋浆 诀单捞飘 殿. 皋浆 诀单捞飘绰 己傍 矫俊父
//
//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (TransientLobbyGM && TransientLobbyGM->IsValidObj()) ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
//		if (!bEvolutionSucceeded)
//		{ // 柳拳 角菩 皋矫瘤甫 焊捞绰单 捞巴鳖瘤 Event 父甸绢辑 朝府变 粱 构窍绊 备侥 UI 矫胶袍捞聪 傲 捞犯霸 せ
//			if (DJLegacyLobbyUIMgr)
//			{
//				DJLegacyLobbyUIMgr->BeginShowWingEvolutionFailMessage();
//			}
//		}
//		else
//		{
//			if (DJLegacyLobbyUIMgr)
//			{
//				DJLegacyLobbyUIMgr->BeginShowWingEvolutionSuccessMessage();
//			}
//			// 柳拳 己傍俊 蝶弗 楷免 敲饭捞
//			BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Signal(WingPCClass);
//		}
//
//		if (bIsInAutoEvolution)
//		{ // 磊悼柳拳狼 版快俊 措茄 贸府.
//			ensure(AutoEvolutionPCClass == WingPCClass); // 磊悼柳拳 档吝俊 狐弗 努腐栏肺 蝶肺 夸没 焊辰促搁 吧副 荐档.. 磊悼柳拳 锭 蝶肺 努腐 给窍霸 秦具 且 淀.
//
//			// 老窜篮 磊悼 柳拳 档吝 促弗 努贰胶 朝俺狼 柳拳 览翠捞 吭促搁 磊悼 柳拳甫 吝窜矫虐绰 吧肺 窍磊. 
//			if (AutoEvolutionPCClass == WingPCClass){
//				ReserveOrExecSingleAutoEvolutionTask(bEvolutionSucceeded); // SingleAutoEvolutionTask 俊辑绰 炼扒捞 登搁 沥惑利栏肺 磊悼 柳拳啊 辆丰凳. 趣篮 瘤加 夸没
//			}
//			else{
//				EndAutoEvolution(EWingAutoOpEndReason::WAEER_Abnormal); // 咯扁肺 坷搁 厚沥惑利牢 磊悼柳拳 辆丰烙.
//			}
//		}
//	}
}

void UB2Airport::OnReceiveEnhanceWingOption(const FB2EnhanceWingOption& EnhanceWingOptionData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveEnhanceWingOption"));

	//EPCClass WingPCClass = SvrToCliPCClassType(GET_TUPLE_DATA(FB2ResponseEnhanceWingOption::character_type_index, EnhanceWingOptionData));

	//FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
	//if (WingContainder)
	//{
	//	const b2network::B2WingOptionInfoPtr& InServerInfoPtr = GET_TUPLE_DATA(FB2ResponseEnhanceWingOption::enhanced_wing_option_index, EnhanceWingOptionData);
	//	WingContainder->WingData.UpdateSinglePropOptionByServerInfo(InServerInfoPtr, false);
	//	
	//	const FWingPropOption* EnhancedPropOptionPtr = WingContainder->WingData.GetPropOptionPtr(SvrToCliWingPropOptionIndex(InServerInfoPtr->option_index));
	//	FWingPropOption OptionDataBackupBeforeEnhance; // 捞傈俊 啊瘤绊乐绰 可记 单捞鸥
	//	FWingPropOption OptionDataBackupAfterEnhance; // 函版等 可记单捞鸥

	//	if (EnhancedPropOptionPtr)
	//		OptionDataBackupBeforeEnhance = *EnhancedPropOptionPtr;
	//	
	//	WingContainder->WingData.UpdateStats(); // 付胶磐 单捞磐俊 扁馆茄 傈捧仿 荐摹 殿 诀单捞飘.

	//	if (EnhancedPropOptionPtr)
	//		OptionDataBackupAfterEnhance = *EnhancedPropOptionPtr;

	//	// 趣矫隔扼辑 茄锅 歹 眉农
	//	if (OptionDataBackupAfterEnhance.MyOptionType == OptionDataBackupBeforeEnhance.MyOptionType)
	//	{
	//		// 捞傈蔼苞 函版等蔼狼 瞒捞父怒 扑诀栏肺 免仿
	//		float DiffAmount = OptionDataBackupAfterEnhance.RawOptionAmount - OptionDataBackupBeforeEnhance.RawOptionAmount;
	//		
	//		UB2UIUpgradeStatNotify* UpgradeStatNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify);
	//		FText OptionText = GetLOCTextOfWingPropOption(OptionDataBackupAfterEnhance.MyOptionType, WingPCClass);
	//		FText AmountText = DiffAmount > 0 ? FText::Format(FText::FromString(TEXT("{0} {1}")), FText::FromString("+"), GetAutoFractionalFormattedText(DiffAmount, 2)) : GetAutoFractionalFormattedText(DiffAmount, 2);
	//		if (UpgradeStatNotifyUI)
	//		{
	//			UpgradeStatNotifyUI->ClearList();
	//			UpgradeStatNotifyUI->AddListItem(OptionText, AmountText, DiffAmount > 0);
	//		}

	//		//////////////////////////////////////////////////////////////////
	//		//咀记 瘤钎 肺弊 (某腐磐 朝俺 可记 碍拳)
	//		B2ActionLogSender Sender;
	//		Sender.setCategory(FString("CHARACTER"))
	//			  .setAction(FString("WING_OPTION"))
	//			  .setLabel(B2ActionLogType<EPCClass>::GetName(WingPCClass))
	//			  .setActionAttr1(OptionText.ToString().ToUpper())
	//			  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//		//////////////////////////////////////////////////////////////////
	//	}

	//	BladeIIGameImpl::GetClientDataStore().OnReceiveEnhanceWingOption(EnhanceWingOptionData); // ClientDataStore 狼 眠啊 贸府. 

	//	UpdateOnWingDataChange(WingPCClass, false); // 加己 可记俊 蝶扼 皋浆甫 促矫 炼钦且 鞘夸绰 绝阑 淀..?

	//	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(WingPCClass);
	//}
}

void UB2Airport::OnReceiveSetWingVisible(const FB2SetWingVisible& WingVisibleData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveSetWingVisible"));
	//
	//EPCClass WingPCClass = SvrToCliPCClassType(GET_TUPLE_DATA(FB2ResponseSetWingVisible::character_type_index, WingVisibleData));
	//FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
	//if (WingContainder)
	//{
	//	WingContainder->WingData.bShouldBeVisible = GET_TUPLE_DATA(FB2ResponseSetWingVisible::is_visible_index, WingVisibleData);
	//}
}

void UB2Airport::UpdateOnWingDataChange(EPCClass InChangedWingPCClass, bool bUpdateMeshToo)
{
	if (bUpdateMeshToo)
	{
		if (InChangedWingPCClass == EPCClass::EPC_End)
		{ // 葛电 努贰胶 诀单捞飘
			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
			{
				LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
			}
		}
		// CharacterIntro 俊辑绰 厘厚 犁炼钦阑 阜绰单 老窜 弊 拳搁俊辑绰 沥秦柳 厘厚甫 啊瘤绊 焊咯临 芭扼 朝俺啊 鞘夸绝阑 巴档 鞍绊 (FLobbySceneManager::SetupLobbyCharCompForParts 曼绊)
		// 霸促啊 檬厘俊 夸 矫弊澄捞 矫埃瞒甫 滴绊 坷搁辑 敲饭捞登带 局聪皋捞记捞 谗扁绰 巩力啊 乐扁 锭巩.
		else if (TransientLobbyGM && TransientLobbyGM->GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_CharacterIntro)
		{
			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InChangedWingPCClass);
		}
	}
	LobbyUpdateWingMainUIClass<>::GetInstance().Signal(); // Wing Main UI 其捞瘤 诀单捞飘
	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(InChangedWingPCClass, false);
}

void UB2Airport::BeginAutoEnhancement(EPCClass EnhanceWingPCClass)
{
	if (EnhanceWingPCClass != EPCClass::EPC_End && !bIsInAutoEnhancement)
	{
		bIsInAutoEnhancement = true;
		AutoEnhancementPCClass = EnhanceWingPCClass;

		SingleAutoEnhancementTask();
	}
}

void UB2Airport::CustomEndAutoItemEnhancement()
{
	EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Custom); // Req 甫 焊辰 惑炔捞电 酒聪电 捞吧肺 贸府啊 瞪 巴栏肺 扁措..
}

void UB2Airport::EndAutoItemEnhance(EWingAutoOpEndReason InEndReason)
{
	bIsInAutoEnhancement = false;
	
	UpdateOnWingDataChange(AutoEnhancementPCClass, true); // 老馆 碍拳 览翠捞 吭阑 锭档 焊郴绰 UI 殿 诀单捞飘 脚龋
		
	if (IsValid(TransientLobbyGM))
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEnhanceResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);

		// 磊悼 碍拳 场唱绰 巴俊 措茄 眠啊利牢 UI 诀单捞飘
		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = TransientLobbyGM->DJLegacy_GetLobbyUIManager();
		if (DJLegacyLobbyUIMgr)
		{
			DJLegacyLobbyUIMgr->UpdateOnWingAutoEnhanceComplete();
		}
	}

	if (InEndReason != EWingAutoOpEndReason::WAEER_Abnormal)
	{
		// NormalEnd 看促绊 窍歹扼档 捣捞唱 犁丰啊 绝绢辑 辆丰等 巴牢瘤, 碍拳啊 辆丰登绢 捞力 柳拳甫 且 窜拌啊 登绢 辆丰等 巴牢瘤俊 蝶扼 促福霸 馆览.
		// NormalEnd 啊 酒囱 扒 矫胶袍 利栏肺 捞惑茄 惑炔俊辑 辆丰等 巴.

		if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordGold)
		{
			ShortageMGR->PopupGoToShopForGoldMessage();
		}
		else if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordIngredient)
		{
			OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EnhanceIngred);
		}
		else
		{ // 啊厘 沥惑利牢 辆丰. 磊悼 碍拳狼 版快绰 Graceful, Custom 葛滴 捞吧肺.
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst)
			{
				UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_AutoEnhance_Ended")), // 巩备俊绰 眠啊 沥焊啊 鞘夸秦龙 荐档..?
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::Confirm
					);
			}
		}
	}

	AutoEnhancementPCClass = EPCClass::EPC_End;
}

void UB2Airport::SingleAutoEnhancementTask()
{
	if (TransientLobbyGM)
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEnhanceResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);
	}

	// 泅犁 惑怕甫 眉农秦辑 拌加 窜老 碍拳 夸没阑 焊郴芭唱 趣篮 吝窜.
	// AutoEnhancement 档吝俊 窜老 碍拳 夸没俊 措茄 览翠捞 吭阑 锭俊档 捞吧肺

	FB2Wing CurrentAutoEnhanceWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(AutoEnhancementPCClass, CurrentAutoEnhanceWingData);

	const bool bCanAffordGold = HasEnoughGoldForWingEnhanceLocalPlayer(AutoEnhancementPCClass);
	const bool bCanAffordIngred = HasEnoughIngredForWingEnhanceLocalPlayer(AutoEnhancementPCClass);
	// 眠啊肺 捞力 柳拳且 锭啊 等 扒 酒囱瘤档 眉农..
	const bool bEnhanceDoneForNow = bGotValidWingData && (IsItTimeToEvolve(CurrentAutoEnhanceWingData) || IsUltimatelyEvolvedWing(CurrentAutoEnhanceWingData));

	if (bGotValidWingData && bCanAffordGold && bCanAffordIngred && !bEnhanceDoneForNow)
	{
		data_trader::Retailer::GetInstance().RequestEnhanceWing(CliToSvrPCClassType(AutoEnhancementPCClass));
		CachedLastAutoEnhEvolExecTime = (float)FPlatformTime::Seconds();

		// 馆靛矫 Response 啊 柯促绰 焊厘篮 绝栏聪, 弥饶狼 鸥捞赣甫 汲摹
		if (TransientLobbyGM)
		{
			TransientLobbyGM->GetWorldTimerManager().SetTimer(AutoEnhanceResponseWaitTH, this, &UB2Airport::ForceEndAutoItemEnhanceTimerCB, 7.0f, false);
		}
	}
	else
	{
		EndAutoItemEnhance(
			!bGotValidWingData ? EWingAutoOpEndReason::WAEER_Abnormal : 
				(bEnhanceDoneForNow ? EWingAutoOpEndReason::WAEER_Graceful : 
					(!bCanAffordGold ? EWingAutoOpEndReason::WAEER_NormalButCannotAffordGold : 
						EWingAutoOpEndReason::WAEER_NormalButCannotAffordIngredient)
				)
			);
	}
}

void UB2Airport::ReserveOrExecSingleAutoEnhancementTask()
{
	if (IsValid(TransientLobbyGM))
	{
		// 辑滚俊辑 览翠捞 柯 饶俊 促澜 夸没阑 窍霸 登骨肺 览翠捞 棵 锭鳖瘤 吧赴 矫埃篮 皑救秦辑 delay 甫 沥窍档废 窃.
		const float TimeSinceLastExec = (float)FPlatformTime::Seconds() - CachedLastAutoEnhEvolExecTime;
		const float AdditionalTimeToDelay = AutoEnhEvolMinInterval - TimeSinceLastExec;
		if (AdditionalTimeToDelay > KINDA_SMALL_NUMBER){
			TransientLobbyGM->GetWorldTimerManager().SetTimer(DelayedAutoEnhEvolExecTH, this, &UB2Airport::SingleAutoEnhancementTask, AdditionalTimeToDelay, false);
		}
		else{
			SingleAutoEnhancementTask();
		}
	}
	else
	{ // 捞繁 版快啊 乐绢急 救登摆瘤父..
		SingleAutoEnhancementTask();
	}
}

void UB2Airport::ForceEndAutoItemEnhanceTimerCB()
{
	EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Abnormal); // 拱沸 沥惑利牢 辆丰啊 酒丛.
}

void UB2Airport::BeginAutoEvolution(EPCClass EvolveWingPCClass)
{
	if (EvolveWingPCClass != EPCClass::EPC_End && !bIsInAutoEvolution)
	{
		bIsInAutoEvolution = true;
		AutoEvolutionPCClass = EvolveWingPCClass;

		SingleAutoEvolutionTask(false);
	}
}

void UB2Airport::CustomEndAutoEvolution()
{
	EndAutoEvolution(EWingAutoOpEndReason::WAEER_Custom); // Req 甫 焊辰 惑炔捞电 酒聪电 捞吧肺 贸府啊 瞪 巴栏肺 扁措..
}

void UB2Airport::EndAutoEvolution(EWingAutoOpEndReason InEndReason)
{
	bIsInAutoEvolution = false;

	UpdateOnWingDataChange(AutoEvolutionPCClass, true); // 老馆 碍拳 览翠捞 吭阑 锭档 焊郴绰 UI 殿 诀单捞飘 脚龋

	if (IsValid(TransientLobbyGM))
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEvolutionResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);

		// 磊悼 柳拳 场唱绰 巴俊 措茄 眠啊利牢 UI 诀单捞飘
		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = TransientLobbyGM->DJLegacy_GetLobbyUIManager();
		if (DJLegacyLobbyUIMgr)
		{
			DJLegacyLobbyUIMgr->UpdateOnWingAutoEvolveComplete();
		}
	}

	if (InEndReason != EWingAutoOpEndReason::WAEER_Abnormal)
	{
		// 捣捞唱 犁丰啊 绝绢辑 辆丰等 巴牢瘤, 绢恫 侥栏肺电 柳拳啊 肯丰等 巴牢瘤俊 蝶扼 促福霸 馆览.

		if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordGold)
		{
			ShortageMGR->PopupGoToShopForGoldMessage();
		}
		else if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordIngredient)
		{
			OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EvolveIngred);
		}
		else if (InEndReason == EWingAutoOpEndReason::WAEER_Custom)
		{ 
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst)
			{
				UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_AutoEvolve_Ended")), // 巩备俊绰 眠啊 沥焊啊 鞘夸秦龙 荐档..?
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::Confirm
					);
			}
		}
		else
		{
			// 咯扁急 公攫啊 且 鞘夸啊 绝绢具.. 柳拳啊 己傍沁促绰 娟扁捞骨肺 柳拳 楷免 矫累 脚龋啊 捞固 焊郴脸电瘤, 梆 促弗 镑俊辑 焊郴电瘤.
		}
	}

	AutoEvolutionPCClass = EPCClass::EPC_End;
}
void UB2Airport::SingleAutoEvolutionTask(bool bInLastEvolutionSucceeded)
{
	if (TransientLobbyGM)
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEvolutionResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);
	}

	if (bInLastEvolutionSucceeded)
	{		
		EndAutoEvolution(EWingAutoOpEndReason::WAEER_Graceful); // 捞扒 傲 己傍沁栏聪 吝窜秦具.
	}
	else
	{
		// 眠啊肺 柳拳 夸没 焊郴绰 霸 啊瓷茄瘤 眉农..
		FB2Wing CurrentAutoEvolveWingData;
		const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(AutoEvolutionPCClass, CurrentAutoEvolveWingData);
		
		const bool bCanAffordGold = HasEnoughGoldForWingEvolveLocalPlayer(AutoEvolutionPCClass);
		const bool bCanAffordIngred = HasEnoughIngredForWingEvolveLocalPlayer(AutoEvolutionPCClass);
		// 柳拳 鸥捞怪捞 嘎绰瘤档 促矫 茄锅 眉农甫
		const bool bIsTimeForEvolution = bGotValidWingData && (IsItTimeToEvolve(CurrentAutoEvolveWingData) && !IsUltimatelyEvolvedWing(CurrentAutoEvolveWingData));
		
		if (bGotValidWingData && bCanAffordGold && bCanAffordIngred && bIsTimeForEvolution)
		{
			data_trader::Retailer::GetInstance().RequestUpgradeWing(CliToSvrPCClassType(AutoEvolutionPCClass));
			CachedLastAutoEnhEvolExecTime = (float)FPlatformTime::Seconds();
			// 馆靛矫 Response 啊 柯促绰 焊厘篮 绝栏聪, 弥饶狼 鸥捞赣甫 汲摹
			if (TransientLobbyGM)
			{
				TransientLobbyGM->GetWorldTimerManager().SetTimer(AutoEvolutionResponseWaitTH, this, &UB2Airport::ForceEndAutoEvolutionTimerCB, 7.0f, false);
			}
		}
		else
		{
			EndAutoEvolution(
				!bGotValidWingData ? EWingAutoOpEndReason::WAEER_Abnormal : 
					(
					// 酒贰 风飘俊辑绰 WAEER_Graceful 捞 乐瘤父 角龙利栏肺 弊繁 版快扼搁 bInLastEvolutionSucceeded 捞 true 咯具 且 巴捞促..
						(bIsTimeForEvolution && !bCanAffordGold) ? EWingAutoOpEndReason::WAEER_NormalButCannotAffordGold : 
							((bIsTimeForEvolution && !bCanAffordIngred) ? EWingAutoOpEndReason::WAEER_NormalButCannotAffordIngredient :
								EWingAutoOpEndReason::WAEER_Graceful)
					)
				);
		}
	}
}

void UB2Airport::ReserveOrExecSingleAutoEvolutionTask(bool bInLastEvolutionSucceeded)
{
	if (IsValid(TransientLobbyGM))
	{
		// 辑滚俊辑 览翠捞 柯 饶俊 促澜 夸没阑 窍霸 登骨肺 览翠捞 棵 锭鳖瘤 吧赴 矫埃篮 皑救秦辑 delay 甫 沥窍档废 窃.
		const float TimeSinceLastExec = (float)FPlatformTime::Seconds() - CachedLastAutoEnhEvolExecTime;
		const float AdditionalTimeToDelay = AutoProEvolMinInterval - TimeSinceLastExec;
		if (AdditionalTimeToDelay > KINDA_SMALL_NUMBER){
			TransientLobbyGM->GetWorldTimerManager().SetTimer(DelayedAutoEnhEvolExecTH, 
				FTimerDelegate::CreateUObject(this, &UB2Airport::SingleAutoEvolutionTask, bInLastEvolutionSucceeded), AdditionalTimeToDelay, false);
		}
		else{
			SingleAutoEvolutionTask(bInLastEvolutionSucceeded);
		}

	}
	else
	{// 捞繁 版快啊 乐绢急 救登摆瘤父..
		SingleAutoEvolutionTask(bInLastEvolutionSucceeded);
	}
}

void UB2Airport::ForceEndAutoEvolutionTimerCB()
{
	EndAutoEvolution(EWingAutoOpEndReason::WAEER_Abnormal); // 拱沸 沥惑利牢 辆丰啊 酒丛.
}

float UB2Airport::GetWingAttackPointLocalPlayer(EPCClass InPCClass)
{
	FPCWingContainer* ThisPCWingContainer = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(InPCClass);
	return ThisPCWingContainer ? ThisPCWingContainer->WingData.GetAttackPoint() : 0.0f;
}

float UB2Airport::GetWingDefensePointLocalPlayer(EPCClass InPCClass)
{
	FPCWingContainer* ThisPCWingContainer = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(InPCClass);
	return ThisPCWingContainer ? ThisPCWingContainer->WingData.GetDefensePoint() : 0.0f;
}

float UB2Airport::GetWingHealthPointLocalPlayer(EPCClass InPCClass)
{
	FPCWingContainer* ThisPCWingContainer = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(InPCClass);
	return ThisPCWingContainer ? ThisPCWingContainer->WingData.GetHealthPoint() : 0.0f;
}

bool UB2Airport::HasEnoughGoldForWingEnhanceLocalPlayer(EPCClass InPCClass)
{
	FB2Wing CharacterWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, CharacterWingData);
	if (bGotValidWingData)
	{
		int32 RequiredItemCount = 0, RequiredGoldCost = 0, EnhancePointToAcquire = 0, RequiredEnhancePoint = 0;
		if (BladeIIGameImpl::GetClientDataStore().GetWingEnhanceRefData(CharacterWingData, RequiredItemCount, RequiredGoldCost, EnhancePointToAcquire, RequiredEnhancePoint))
		{
			return (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= RequiredGoldCost);
		}
	}
	return false;
}
bool UB2Airport::HasEnoughIngredForWingEnhanceLocalPlayer(EPCClass InPCClass)
{
	FB2Wing CharacterWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, CharacterWingData);
	if (bGotValidWingData)
	{
		int32 RequiredItemCount = 0, RequiredGoldCost = 0, EnhancePointToAcquire = 0, RequiredEnhancePoint = 0;
		if (BladeIIGameImpl::GetClientDataStore().GetWingEnhanceRefData(CharacterWingData, RequiredItemCount, RequiredGoldCost, EnhancePointToAcquire, RequiredEnhancePoint))
		{
			//return RequiredItemCount > 0 && (UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEnhanceResourceRefID()) >= RequiredItemCount);
		}
	}
	return false;
}
bool UB2Airport::HasEnoughGoldForWingEvolveLocalPlayer(EPCClass InPCClass)
{
	FB2Wing CharacterWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, CharacterWingData);
	if (bGotValidWingData)
	{
		int32 RequiredItemCount = 0, RequiredGoldCost = 0, MaxAllowedFailPoint = 0;
		float SuccessRate = 0.0f;
		if (BladeIIGameImpl::GetClientDataStore().GetWingEvolveRefData(CharacterWingData, RequiredItemCount, RequiredGoldCost, SuccessRate, MaxAllowedFailPoint))
		{
			return (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= RequiredGoldCost);
		}
	}
	return false;
}
bool UB2Airport::HasEnoughIngredForWingEvolveLocalPlayer(EPCClass InPCClass)
{
	FB2Wing CharacterWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, CharacterWingData);
	if (bGotValidWingData)
	{
		int32 RequiredItemCount = 0, RequiredGoldCost = 0, MaxAllowedFailPoint = 0;
		float SuccessRate = 0.0f;
		if (BladeIIGameImpl::GetClientDataStore().GetWingEvolveRefData(CharacterWingData, RequiredItemCount, RequiredGoldCost, SuccessRate, MaxAllowedFailPoint))
		{
		//	return  (UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEvolveResourceRefID()) >= RequiredItemCount);
		}
	}
	return false;
}
bool UB2Airport::HasEnoughGoldForWingOptionEnhanceLocalPlayer(EPCClass InPCClass, int32 InOptionIndex)
{ // 加己可记 碍拳俊绰 捣捞 救 甸绢皑
	return true;
}
bool UB2Airport::HasEnoughIngredForWingOptionEnhanceLocalPlayer(EPCClass InPCClass, int32 InOptionIndex)
{
	FB2Wing CharacterWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, CharacterWingData);
	if (bGotValidWingData)
	{
		int32 RequiredItemCount = 0;
		if (BladeIIGameImpl::GetClientDataStore().GetWingPropOptionRefData(CharacterWingData, InOptionIndex, RequiredItemCount))
		{
		//	return (UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID()) >= RequiredItemCount);
		}
	}
	return false;
}

void UB2Airport::LoadLocalData()
{
	if (GConfig)
	{
		GConfig->GetBool(TEXT("Airport"), TEXT("WingUIPageOpenedOnce"), bWingUIPageOpenedOnce, GB2UserSavedStateIni);

		// ... 
	}
}
void UB2Airport::SaveLocalData()
{
	if (GConfig)
	{
		GConfig->SetBool(TEXT("Airport"), TEXT("WingUIPageOpenedOnce"), bWingUIPageOpenedOnce, GB2UserSavedStateIni);

		// ... 

		GConfig->Flush(false, GB2UserSavedStateIni);
	}
}

void UB2Airport::SetWingVisibility(EPCClass InPCClass, bool bInShow, bool bReqToServerImmediately)
{
//	FPCWingContainer* ThisCharWingContainer = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(InPCClass);
//	if (ThisCharWingContainer && ThisCharWingContainer->bHasWing)
//	{
//		// 老窜 肺拿 单捞磐 蔼 技泼何磐 刚历 窃. 捞芭 visibility 甫 函版窍绰 UI 俊辑 呈公 梨篮 函版栏肺 菩哦捞 吭促艾促 窍绰 惑炔阑 乔窍扁 困秦 request 甫 蝶肺 朝副 荐 乐档废 窍看澜.
//		// 拱沸 FlushPendingWingVisibilityReq 甫 利例茄 矫痢俊 朝府瘤 臼栏搁 促矫 角青且 锭俊 函版茄 巴捞 公侩瘤拱捞 瞪 巴捞促.
//		const bool bChanged = (ThisCharWingContainer->WingData.bShouldBeVisible != bInShow);
//		ThisCharWingContainer->WingData.bShouldBeVisible = bInShow;
//
//		if (bChanged)
//		{// 肺厚 皋浆 单捞磐 诀单捞飘
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InPCClass);
//		}
//
//		int32 WingPCClassIdx = PCClassToInt(InPCClass);
//
//		if (bReqToServerImmediately)
//		{
//			// 官肺 辑滚肺 夸没窍绊 秦寸 努贰胶 夸没 阶牢 霸 乐促搁 力芭
//			data_trader::Retailer::GetInstance().RequestSetWingVisible(CliToSvrPCClassType(InPCClass), bInShow);
//
//			PendingWingVisibilityReq.Remove(InPCClass);
//		}
//		else
//		{
//			// 蝶肺 夸没阑 困秦 pending 钎矫甫 秦 敌促.
//
//			bool* ExistingPending = PendingWingVisibilityReq.Find(InPCClass);
//			if (ExistingPending){
//				*ExistingPending = bInShow;
//			}
//			else{
//				PendingWingVisibilityReq.Add(InPCClass, bInShow);
//			}
//		}
//	}
}

void UB2Airport::FlushPendingWingVisibilityReq()
{
	// SetWingVisibility 甫 烹秦 阶牢 朝俺 visibility on/off 夸没 辑滚肺.. 窍绊 厚框.
	for (auto& ThisPending : PendingWingVisibilityReq)
	{
		data_trader::Retailer::GetInstance().RequestSetWingVisible(CliToSvrPCClassType(ThisPending.Key), ThisPending.Value);
	}
	PendingWingVisibilityReq.Empty();
}

void UB2Airport::ConditionalSetWingVisibleOnWingPageOpen()
{
	if (!bWingUIPageOpenedOnce)
	{
		// 朝俺 其捞瘤甫 弥檬肺 凯菌阑 锭 扁夯蔼栏肺 朝俺甫 难档废 辑滚俊 夸没. 捞饶肺绰 荤侩磊 汲沥俊 蝶扼.
		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		{
			SetWingVisibility(IntToPCClass(PCI), true, true);
		}

		bWingUIPageOpenedOnce = true;
		SaveLocalData();
	}
}

bool UB2Airport::HasAnyNewStuff_Char(EPCClass InCheckChar)
{
	// 朝俺 皋春俊辑 康旷包府 傍烹 某腐磐 檬惑拳 困俊 New 付农 剁快绰 炼扒

	FB2Wing CheckWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCheckChar, CheckWingData);

	if (bGotValidWingData)
	{
		if (
			// 碍拳啊 啊瓷窍芭唱
			(HasEnoughGoldForWingEnhanceLocalPlayer(InCheckChar) && HasEnoughIngredForWingEnhanceLocalPlayer(InCheckChar)) ||
			// 柳拳啊 啊瓷窍芭唱
			(IsItTimeToEvolve(CheckWingData) && HasEnoughGoldForWingEvolveLocalPlayer(InCheckChar) && HasEnoughIngredForWingEvolveLocalPlayer(InCheckChar))
			)
		{
			return true;
		}

		// 酒聪搁 加己可记 碍拳扼档 啊瓷窍芭唱
		for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
		{
			if (HasAnyNewStuff_WingPropOption(InCheckChar, POI))
			{
				return true;
			}
		}
	}

	return false;
}

bool UB2Airport::HasAnyNewStuff_WingPropOption(EPCClass InCheckChar, int32 InPropOptionIndex)
{
	// 朝俺 皋春俊辑 加己 可记 酒捞能 困俊 New 付农 剁快绰 炼扒

	FB2Wing CheckWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCheckChar, CheckWingData);
	const FWingPropOption* CheckOptionPtr = bGotValidWingData ? CheckWingData.GetPropOptionPtr(InPropOptionIndex) : NULL;

	if (bGotValidWingData && CheckOptionPtr)
	{
		// 歹 碍拳啊 啊瓷茄 凯妨乐绰 可记捞 乐绊 犁拳档 面盒茄瘤
		if (CheckOptionPtr->bIsOpen && CheckOptionPtr->OptionLevel < CheckOptionPtr->MaxOptionLevel &&
			HasEnoughGoldForWingOptionEnhanceLocalPlayer(InCheckChar, InPropOptionIndex) &&
			HasEnoughIngredForWingOptionEnhanceLocalPlayer(InCheckChar, InPropOptionIndex))
		{
			return true;
		}
	}

	return false;
}

bool UB2Airport::GetPreEvolveData(FB2Wing& OutData) const
{
	if (bPreEvolveDataValid)
	{
		OutData = TemporaryPreEvolveData;
		return true;
	}
	return false;
}

#if !UE_BUILD_SHIPPING
void UB2Airport::DevOnlyGenerateDummyWingData(int32 InEvolveGrade, bool bRequestToServer)
{
	if (bRequestToServer)
	{
		UE_LOG(LogBladeII, Log, TEXT("Request dummy wing data to server is NOT implemented yet!")); // 牢亥配府狼 版快 曼绊..
	}
	else
	{
		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		{
			EPCClass ThisPCClass = IntToPCClass(PCI);
			FPCWingContainer* ThisWingContainer = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(ThisPCClass);
			if (ThisWingContainer)
			{
				FB2Wing NewWing;
				NewWing.OwnerPCClass = PCI;
				NewWing.EvolutionGrade = FMath::Clamp(InEvolveGrade, MIN_WING_EVOLUTION_GRADE, MAX_WING_EVOLUTION_GRADE_UNOFFICIAL);
				
				// 柳拳 皋春甫 抛胶飘秦具摆绢辑 盔贰焊促 臭篮 犬伏肺 弥措 碍拳饭骇阑 盲况霖促..
				if (FMath::RandRange(1, 4) == 1){
					NewWing.EnhanceLevel = MAX_WING_ENHANCE_LEVEL;
					NewWing.EvolutionFailedCount = FMath::RandRange(0, 99); // 角菩 冉荐档 措帆 盲况淋.
				}
				else{
					NewWing.EnhanceLevel = FMath::RandRange(MIN_WING_ENHANCE_LEVEL, MAX_WING_ENHANCE_LEVEL - 1);
					NewWing.EnhancePoint = FMath::RandRange(0, 99); // 措帆 捞 荤捞老 淀..
				}
								
				for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
				{
					if (NewWing.PropOptions.IsValidIndex(POI))
					{
						FWingPropOption& ThisOption = NewWing.PropOptions[POI];
						ThisOption.MaxOptionLevel = 30; // 措帆 捞码老 巴.
						ThisOption.OptionLevel = FMath::RandRange(1, ThisOption.MaxOptionLevel);
						// 咯扁档 绢蠢 沥档 臭篮 犬伏肺 弥措 碍拳饭骇阑 盲况霖促..
						if (FMath::RandRange(1, 6) == 1){
							ThisOption.OptionLevel = ThisOption.MaxOptionLevel;
						}
					}
				}

				if (!NewWing.UpdateStats()) 
				{
					// UpdateStats 绰 付胶磐 单捞磐啊 乐阑 锭父 力措肺 累悼窍骨肺 角菩窍搁 扁鸥 瓷仿摹档 啊扼肺 嘎苗淋.
					float BasePointGrade = NewWing.EvolutionGrade * 100.0f;
					NewWing.SetAttackPoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));
					NewWing.SetDefensePoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));
					NewWing.SetHealthPoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));

					for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
					{
						EItemOption MostLikeWingOption = EItemOption::EIO_Defense_DecAbnormalStateTime;
						// 傲 咯变 檬扁 扁裙俊 蝶扼 措眉肺 荤侩且 芭 鞍篮 可记 持绢林绰 巴烙.
						switch (POI)
						{
						case 1: MostLikeWingOption = EItemOption::EIO_Defense_ResistCritical; break;
						case 2: MostLikeWingOption = EItemOption::EIO_Misc_IncMoveSpeed; break;
						case 3: MostLikeWingOption = EItemOption::EIO_Defense_DecDamage; break;
						case 4: MostLikeWingOption = EItemOption::EIO_Offense_IncCriticalRate; break;
						}

						if (NewWing.PropOptions.IsValidIndex(POI))
						{
							FWingPropOption& ThisOption = NewWing.PropOptions[POI];
							ThisOption.MyOptionType = MostLikeWingOption;
							// 柳拳饭骇俊 蝶扼 加己可记捞 凯府绰 侥烙.
							ThisOption.RequiredWingEvolveGrade = POI + 1;
							ThisOption.bIsOpen = (NewWing.EvolutionGrade >= ThisOption.RequiredWingEvolveGrade);
							ThisOption.RawOptionAmount = ((float)ThisOption.OptionLevel * 100.0f) / (float)ThisOption.MaxOptionLevel;
						}
					}
				}

				ThisWingContainer->bHasWing = true;
				ThisWingContainer->WingData = NewWing;
			}
		}
	}
}
#endif
