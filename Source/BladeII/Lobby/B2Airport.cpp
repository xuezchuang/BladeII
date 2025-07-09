// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2Airport.h"
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
	// 날개를 가지고 있는지 여부가 따로 프로토콜에 포함될 수도 있긴 한데.. 일단 이렇게.
	bHasWing = (WingData.EvolutionGrade >= MIN_WING_EVOLUTION_GRADE && WingData.EnhanceLevel >= MIN_WING_ENHANCE_LEVEL);
}

FWingMD_EnhanceSingleElem& FWingMD_EnhanceSingleElem:: operator=(const b2network::B2mdWingEnhancePtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetGrade = ServerStruct->grade;
		this->TargetLevel = ServerStruct->level;

		// 이전에 이미 넣어놨으니 확인 차원에서.. 뭔가 의미있어 보이는 값인데 다르면 바뀐거고 아니면 잠시 잘못된 거
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingEnhanceResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingEnhanceResourceRefID();	// 걍 이걸로 쓴다.. ㄷㄷ

		this->RequiredItemCount = ServerStruct->need_item_count;
		this->EnhancePointToAcquirePerTrial = ServerStruct->enhance_point;
		this->RequiredEnhancePoint = ServerStruct->need_enhance_point;
		this->RequiredGoldCost = ServerStruct->gold_cost;

		// option_type 은 일단 안 보는 걸로..
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

		// 이전에 이미 넣어놨으니 확인 차원에서. 뭔가 의미있어 보이는 값인데 다르면 바뀐거고 아니면 잠시 잘못된 거
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingEvolveResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingEvolveResourceRefID(); // 걍 이걸로 쓴다 ㄷㄷ

		this->RequiredItemCount = ServerStruct->need_item_count;
		this->RequiredGoldCost = ServerStruct->gold_cost;
		this->SuccessRate = (float)ServerStruct->rate * 0.0001f; // 서버에서 온 rate 는 만분율이라 컨버팅해 줌.
		this->MaxFailPoint = ServerStruct->fail_point_max;

		// option_type 은 일단 안 보는 걸로..
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
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // 서버에선 1부터 시작하는 값이 옴 
		this->TargetOptionLevel = ServerStruct->level;
		this->OptionType = SvrToCliOptionType(ServerStruct->option_type);
		this->OptionValue = ServerStruct->option_value;

		// 이전에 이미 넣어놨으니 확인 차원에서.. 뭔가 의미있어 보이는 값인데 다르면 바뀐거고 아니면 잠시 잘못된 거
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID(); // 걍 이걸로 쓴다.. ㄷㄷ

		this->RequiredItemCount = ServerStruct->need_item_count;
	}

	return *this;
}

FWingMD_PropOptionOpenSingleElem& FWingMD_PropOptionOpenSingleElem::operator = (const b2network::B2mdWingOptionOpenPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // 서버에선 1부터 시작하는 값이 옴
		this->RequiredWingGrade = ServerStruct->need_grade;
	}
	return *this;
}

FWingMD_PropOptionMaxLevelSingleElem& FWingMD_PropOptionMaxLevelSingleElem::operator = (const b2network::B2mdWingOptionMaxLevelPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // 서버에선 1부터 시작하는 값이 옴
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
	// 장착 아이템 쪽 수치 얻어오는 함수에서 Wing 도 같이 처리할 것. (UB2LobbyInventory::GetEquippedOptionPoint)
	// 이걸 따로 사용해서 다른 데서 얻은 수치와 합치려면 ECombatOptionApplyType 에 따라 곱하거나 더하거나를 같이 따져보아야 함.

	TArray<FOptionValue> GatheredOptionRawValue; // 실제로는 하나만 들어가 있어야 할 것.

	for (const FWingPropOption& ThisOption : PropOptions)
	{
		if (ThisOption.MyOptionType == InOptionType)
		{
			FOptionValue OptionValue = FOptionValue(ThisOption.RawOptionAmount);
			GatheredOptionRawValue.Add(OptionValue);
		}
	}
	// 기존 Item 과 같은 옵션 체계를 사용..
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

	// 가장 기본적인 서버에서의 날개 정보 받는 함수. 원격 캐릭터 플레이어의 경우는 이걸로 받지 않게 될 것.

	if (InResponseGetWingPtr)
	{
		OwnerPCClass = PCClassToInt(SvrToCliPCClassType(InResponseGetWingPtr->character_type));
		EvolutionGrade = InResponseGetWingPtr->grade;
		EvolutionFailedCount = InResponseGetWingPtr->upgrade_fail_point;
		EnhanceLevel = InResponseGetWingPtr->level;
		EnhancePoint = InResponseGetWingPtr->enhance_point;

		// bShouldBeVisible 은 여기서 세팅하지 않는다.

		// 여기선 옵션 데이터가 다 와야 될 것.. 열려 있든 열려 있지 않던 간에.
		ensure(InResponseGetWingPtr->wing_option_infos.Num() == PropOptions.Num());
		for (int32 POI = 0; POI < InResponseGetWingPtr->wing_option_infos.Num(); ++POI)
		{
			const b2network::B2WingOptionInfoPtr& ThisServerInfo = InResponseGetWingPtr->wing_option_infos[POI];
			UpdateSinglePropOptionByServerInfo(ThisServerInfo, false);
		}
	}

	// 기타 다른 데이터들은 지금까지 세팅된 거 기반으로 마스터 테이블서 가져옴.
	UpdateStats();
}

void FB2Wing::UpdateByPartialServerInfo(const b2network::B2CharacterServerInfoPtr& InCharServerInfo, const TArray<b2network::B2WingOptionInfoPtr>& InWingOptionInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateByPartialServerInfo"));

	// 로컬 캐릭터의 날개 진화/강화 등의 동작을 제외한 외관 표시나 전투 수치 산정 등에 필요한 정보들은 모두 이걸로 받을 수 있음.
	// 초기 AccountInfo 에 포함된 날개 데이터 및 원격 캐릭터 날개 정보를 이걸로 업데이트.

	if (InCharServerInfo)
	{
		OwnerPCClass = PCClassToInt(SvrToCliPCClassType(InCharServerInfo->character_type));
		EvolutionGrade = InCharServerInfo->wing_grade;
		EnhanceLevel = InCharServerInfo->wing_level;
		bShouldBeVisible = InCharServerInfo->wing_visible;

		// EvolutionFailedCount 랑 EnhancePoint 는 여기서 세팅하지 않는다. 두 값은 전투에서는 필요없는 값임.
	}
	//  아직 한번도 GetWing 을 하지 않은 캐릭터라면 옵션 데이터가 안 올 수 있다. 그게 아니라면 기본 갯수에 맞춰서 오는 걸로..
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
		{ // 서버에선 인덱스를 보내주고 type 은 마스터데이터 룩업으로.
			FWingPropOption& MyOptionInfo = PropOptions[CliOptionIndex];
			MyOptionInfo.OptionLevel = InServerInfoPtr->level;
		}
	}
	if (bUpdateAllImmediate)
	{ // 기타 나머지 데이터는 UpdateStats 를 거치면서 마스터데이터룩업으로 채워짐. 이건 따로 부르게 할 수도.
		UpdateStats();
	}
}

bool FB2Wing::UpdateStats()
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateStats"));

	bool bRetVal = true;

	//// EvolveGrade, EnhanceLevel 등 기본 데이터가 차 있다는 가정으로 기타 나머지 능력치 등을 마스터 데이터에서 룩업해서 채워넣는 것.

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
	//		// 아마도 테스트 기능에서나 이게 필요한 상황이 나올 거 같긴 하지만..
	//		if (ThisPropOption.MaxOptionLevel > 0){ // 그리고 해당 옵션이 닫혀 있는 등급에서는 이게 0 일 수 있는데 0으로 클램프 되면 문제가 되니 그건 놔둠. 
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
	// 정식으로 프로토콜 들어오기 전까지 사용할 거..

	if (InGrade >= MIN_WING_EVOLUTION_GRADE)
	{
		OutWingContainder.WingData.OwnerPCClass = PCClassToInt(InPCClass);
		OutWingContainder.WingData.EvolutionGrade = FMath::Min(InGrade, MAX_WING_EVOLUTION_GRADE_UNOFFICIAL);

		// 나머지 데이터는 적당히 채워넣는데.. 딱히 더 손댈 것도 없을 듯.
		OutWingContainder.WingData.EnhanceLevel = MIN_WING_ENHANCE_LEVEL;
		
		// 여하간 여기서 마스터데이터로 많은 것들을 채워넣을 거니 그나마 Grade 라도 있으면 써먹을만 할 듯.
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
{ // ini 로딩이 있었는데 별도 위치로 옮겨짐..
}

void UB2Airport::StaticOnModuleShutdown()
{
}

void UB2Airport::InitAirportLobby(class AB2LobbyGameMode* InLobbyGM)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::InitAirportLobby"));

	TransientLobbyGM = InLobbyGM;
	//check(TransientLobbyGM && TransientLobbyGM->IsValidObj()); // 객체 생성은 LobbyGameMode 에서만

	SubscribeEvents();
}

void UB2Airport::ConditionalReqForFullWingData(EPCClass InWingPCClass)
{
	int32 WingPCClassIdx = PCClassToInt(InWingPCClass);
	if (WingPCClassIdx < GetMaxPCClassNum() && !FullWingDataReqState[WingPCClassIdx])
	{ // 이 경로를 통한 요청은 캐릭터당 한번만 하면 됨.
		// 응답은 OnReceiveGetWing 으로 받게 될 건데 FullWingData 라고 하기에는 이걸로 세팅하지 않는 데이터도 있긴 하지만 AccountInfo 를 통해 오는 것에 비해서는 full data 에 가깝다고 할 수 있겠다..
		data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(InWingPCClass));
	}
}

void UB2Airport::SetWingDataByAccountInfo(const TArray<b2network::B2CharacterServerInfoPtr>& InAllCharInfoArray, const TArray<b2network::B2WingOptionInfoPtr>& InAllWingOptionInfoArray)
{
	// 처음에 AccountInfo 받으면서 불리게 됨. EnhancePoint, EvolutionFailedCount 같이 Wing UI 페이지에서만 필요한 데이터는 여기서 받지 않게 될 것.

	for (int32 CI = 0; CI < InAllCharInfoArray.Num(); ++CI)
	{
		const b2network::B2CharacterServerInfoPtr& ThisCharInfo = InAllCharInfoArray[CI];

		// 전체 WingOptionInfoArry 중에서 이번 캐릭터 것만 빼옴. 아직 한번도 GetWing 을 하지 않은 캐릭터라면 옵션 데이터가 안 올 수 있다.
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
			ThisWingContainder->SetHasWingByWingData(); // bHasWing 을 따로 서버에서 받아야 할 거는 같은데 일단 없으니..

			// 이 시점에서는 아마 필수 마스터데이터가 없을 거라 마스터데이터 받은 후에 UpdateStats 를 다시 불러주어야 할 것.
		}
	}
}

void UB2Airport::UpdateWingCombatStatsOnMD()
{
	// UB2Airport::SetWingDataByAccountInfo 가 불린 시점에는 날개 마스터데이터가 없을 것이므로 따로 이렇게 UpdateStats 을 불러준다.
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
	// AccountInfo 에 필수적인 날개 데이터가 없는 상황에서 AccountInfo 랑 MasterData 받고 나서 초기에 날개 데이터 일괄 요청 용.
	// 지금은 그냥 for 문 돌려서 모든 클래스에 대해 RequestGetWing 날리면 될 건데 이거 만들 때에는 요청을 한번에 한번만 날려야 하는 제한이 있었음.

	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::BeginInitReqForWingData"));

	bIsRequestingWingDataForInit = true;

	// 글래디에이터부터 해서 시작
	checkSlow((int32)EPCClass::EPC_Gladiator == 0);
	data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(EPCClass::EPC_Gladiator));
}

void UB2Airport::OnReceiveGetWingCheckForContinueReq(const FB2Wing& ReceivedWingData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveGetWingCheckForContinueReq"));

	EPCClass ThisWingPCClass = IntToPCClass(ReceivedWingData.OwnerPCClass);
	EPCClass NextPCClass = IntToPCClass(ReceivedWingData.OwnerPCClass + 1);
		
	if (ThisWingPCClass == EPCClass::EPC_End || NextPCClass == EPCClass::EPC_End)
	{ // 뭔가 잘못되었거나 다 받았으니 여기서 끝냄
		bIsRequestingWingDataForInit = false;
	}
	else if (NextPCClass != EPCClass::EPC_End)
	{ // 그 다음 req
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
{ // 여기엔 Response 데이터를 FB2Wing 으로 바꿔서 넘겨줌.

	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveGetWing"));

	EPCClass WingPCClass = IntToPCClass(GetWingData.OwnerPCClass);

	FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
	if (WingContainder)
	{
		// 요 프로토콜로는 Visible 여부가 오지 않고 대신 이전에 SetWingDataByAccountInfo 에 의해 Visible 이 먼저 세팅되었다 치고 그걸 사용하기 위해 백업.
		const bool bPrevVisible = WingContainder->WingData.bShouldBeVisible; 

		WingContainder->WingData = GetWingData;
		// bHasWing 을 따로 서버에서 받아야 할 거는 같은데 일단 없으니..
		WingContainder->SetHasWingByWingData();

		// 이전에 받아놓은 Visible 여부로 복구.
		WingContainder->WingData.bShouldBeVisible = bPrevVisible;

		int32 WingPCClassIdx = PCClassToInt(WingPCClass);
		FullWingDataReqState[WingPCClassIdx] = true; // 어떤 경로를 통해 온 응답이건 한번 받고 나면 더 이상 이 경로로 받지 않아도 되므로 표시.

		UpdateOnWingDataChange(WingPCClass, true);
	}

	if (bIsRequestingWingDataForInit) // 초기 일괄 요청의 경우
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

	//	WingContainder->WingData.UpdateStats(); // 마스터 데이터에 기반한 전투력 수치 등 업데이트.

	//	BladeIIGameImpl::GetClientDataStore().OnReceiveEnhanceWing(EnhanceWingData); // ClientDataStore 의 추가 처리. 골드랑 재료아이템 업데이트 등

	//	UpdateOnWingDataChange(WingPCClass, !bIsInAutoEnhancement); // UI 및 메쉬 업데이트 등. 자동강화 도중에는 UI 만 업데이트 되게.
	//			
	//	// UI 쪽에 애니메이션 플레이를 좀 하는데 이것까지 Event 만들어서 날리긴 좀 뭐하고 구식 UI 시스템이니 걍 이렇게 ㅋ
	//	// 자동강화에서도 일단 해 보자.
	//	if (WingContainder->WingData.EnhanceLevel > PrevEnhanceLevel)
	//	{ // WingEnhancedAnim 은 EnhancePoint 가 아닌 실제 능력치가 변경되는 EnhanceLevel 변경 시에 플레이. 
	//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (TransientLobbyGM && TransientLobbyGM->IsValidObj()) ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//		if (DJLegacyLobbyUIMgr)
	//		{
	//			DJLegacyLobbyUIMgr->PlayWingEnhancedAnim();
	//			LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
	//		}
	//	}

	//	if (bIsInAutoEnhancement)
	//	{ // 자동강화의 경우에 대한 처리.
	//		ensure(AutoEnhancementPCClass == WingPCClass); // 자동강화 도중에 빠른 클릭으로 따로 요청 보낸다면 걸릴 수도.. 자동강화 때 따로 클릭 못하게 해야 할 듯.

	//		// 일단은 자동 강화 도중 다른 클래스 날개의 강화 응답이 왔다면 자동 강화를 중단시키는 걸로 하자. 
	//		if (AutoEnhancementPCClass == WingPCClass){
	//			ReserveOrExecSingleAutoEnhancementTask(); // SingleAutoEnhancementTask 에서는 조건이 되면 정상적으로 자동 강화가 종료됨. 혹은 지속 요청
	//		}
	//		else{
	//			EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Abnormal); // 여기로 오면 비정상적인 자동강화 종료임.
	//		}
	//	}

	//	//////////////////////////////////////////////////////////////////
	//	//액션 지표 로그 (캐릭터 날개 강화)
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
//		// 성공한 경우 진화 연출 결과 UI 창에 비교 stat 을 뿌리기 위해 업데이트 되기 전 데이터를 백업한다.
//		if (bEvolutionSucceeded)
//		{
//			TemporaryPreEvolveData = WingContainder->WingData;
//			bPreEvolveDataValid = true; // 진화 연출 나갈 때 리셋될 것.
//			
//			//////////////////////////////////////////////////////////////////
//			//액션 지표 로그 (캐릭터 날개 승급(진화))
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
//		checkSlow( // 예상한 것과 일치하는지..
//			(bEvolutionSucceeded && (EvolGradeBefore + 1) == WingContainder->WingData.EvolutionGrade) ||
//			(!bEvolutionSucceeded && (FailCountBefore + 1) == WingContainder->WingData.EvolutionFailedCount)
//			);
//#endif
//
//		WingContainder->WingData.UpdateStats(); // 마스터 데이터에 기반한 전투력 수치 등 업데이트.
//
//		BladeIIGameImpl::GetClientDataStore().OnReceiveUpgradeWing(UpgradeWingData); // ClientDataStore 의 추가 처리. 골드랑 재료아이템 업데이트 등
//
//		UpdateOnWingDataChange(WingPCClass, bEvolutionSucceeded); // UI 및 메쉬 업데이트 등. 메쉬 업데이트는 성공 시에만
//
//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (TransientLobbyGM && TransientLobbyGM->IsValidObj()) ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
//		if (!bEvolutionSucceeded)
//		{ // 진화 실패 메시지를 보이는데 이것까지 Event 만들어서 날리긴 좀 뭐하고 구식 UI 시스템이니 걍 이렇게 ㅋ
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
//			// 진화 성공에 따른 연출 플레이
//			BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Signal(WingPCClass);
//		}
//
//		if (bIsInAutoEvolution)
//		{ // 자동진화의 경우에 대한 처리.
//			ensure(AutoEvolutionPCClass == WingPCClass); // 자동진화 도중에 빠른 클릭으로 따로 요청 보낸다면 걸릴 수도.. 자동진화 때 따로 클릭 못하게 해야 할 듯.
//
//			// 일단은 자동 진화 도중 다른 클래스 날개의 진화 응답이 왔다면 자동 진화를 중단시키는 걸로 하자. 
//			if (AutoEvolutionPCClass == WingPCClass){
//				ReserveOrExecSingleAutoEvolutionTask(bEvolutionSucceeded); // SingleAutoEvolutionTask 에서는 조건이 되면 정상적으로 자동 진화가 종료됨. 혹은 지속 요청
//			}
//			else{
//				EndAutoEvolution(EWingAutoOpEndReason::WAEER_Abnormal); // 여기로 오면 비정상적인 자동진화 종료임.
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
	//	FWingPropOption OptionDataBackupBeforeEnhance; // 이전에 가지고있는 옵션 데이타
	//	FWingPropOption OptionDataBackupAfterEnhance; // 변경된 옵션데이타

	//	if (EnhancedPropOptionPtr)
	//		OptionDataBackupBeforeEnhance = *EnhancedPropOptionPtr;
	//	
	//	WingContainder->WingData.UpdateStats(); // 마스터 데이터에 기반한 전투력 수치 등 업데이트.

	//	if (EnhancedPropOptionPtr)
	//		OptionDataBackupAfterEnhance = *EnhancedPropOptionPtr;

	//	// 혹시몰라서 한번 더 체크
	//	if (OptionDataBackupAfterEnhance.MyOptionType == OptionDataBackupBeforeEnhance.MyOptionType)
	//	{
	//		// 이전값과 변경된값의 차이만큼 팝업으로 출력
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
	//		//액션 지표 로그 (캐릭터 날개 옵션 강화)
	//		B2ActionLogSender Sender;
	//		Sender.setCategory(FString("CHARACTER"))
	//			  .setAction(FString("WING_OPTION"))
	//			  .setLabel(B2ActionLogType<EPCClass>::GetName(WingPCClass))
	//			  .setActionAttr1(OptionText.ToString().ToUpper())
	//			  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//		//////////////////////////////////////////////////////////////////
	//	}

	//	BladeIIGameImpl::GetClientDataStore().OnReceiveEnhanceWingOption(EnhanceWingOptionData); // ClientDataStore 의 추가 처리. 

	//	UpdateOnWingDataChange(WingPCClass, false); // 속성 옵션에 따라 메쉬를 다시 조합할 필요는 없을 듯..?

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
		{ // 모든 클래스 업데이트
			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
			{
				LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
			}
		}
		// CharacterIntro 에서는 장비 재조합을 막는데 일단 그 화면에서는 정해진 장비를 가지고 보여줄 거라 날개가 필요없을 것도 같고 (FLobbySceneManager::SetupLobbyCharCompForParts 참고)
		// 게다가 초장에 요 시그널이 시간차를 두고 오면서 플레이되던 애니메이션이 끊기는 문제가 있기 때문.
		else if (TransientLobbyGM && TransientLobbyGM->GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_CharacterIntro)
		{
			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InChangedWingPCClass);
		}
	}
	LobbyUpdateWingMainUIClass<>::GetInstance().Signal(); // Wing Main UI 페이지 업데이트
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
	EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Custom); // Req 를 보낸 상황이든 아니든 이걸로 처리가 될 것으로 기대..
}

void UB2Airport::EndAutoItemEnhance(EWingAutoOpEndReason InEndReason)
{
	bIsInAutoEnhancement = false;
	
	UpdateOnWingDataChange(AutoEnhancementPCClass, true); // 일반 강화 응답이 왔을 때도 보내는 UI 등 업데이트 신호
		
	if (IsValid(TransientLobbyGM))
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEnhanceResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);

		// 자동 강화 끝나는 것에 대한 추가적인 UI 업데이트
		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = TransientLobbyGM->DJLegacy_GetLobbyUIManager();
		if (DJLegacyLobbyUIMgr)
		{
			DJLegacyLobbyUIMgr->UpdateOnWingAutoEnhanceComplete();
		}
	}

	if (InEndReason != EWingAutoOpEndReason::WAEER_Abnormal)
	{
		// NormalEnd 였다고 하더라도 돈이나 재료가 없어서 종료된 것인지, 강화가 종료되어 이제 진화를 할 단계가 되어 종료된 것인지에 따라 다르게 반응.
		// NormalEnd 가 아닌 건 시스템 적으로 이상한 상황에서 종료된 것.

		if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordGold)
		{
			ShortageMGR->PopupGoToShopForGoldMessage();
		}
		else if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordIngredient)
		{
			OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EnhanceIngred);
		}
		else
		{ // 가장 정상적인 종료. 자동 강화의 경우는 Graceful, Custom 모두 이걸로.
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst)
			{
				UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_AutoEnhance_Ended")), // 문구에는 추가 정보가 필요해질 수도..?
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

	// 현재 상태를 체크해서 계속 단일 강화 요청을 보내거나 혹은 중단.
	// AutoEnhancement 도중에 단일 강화 요청에 대한 응답이 왔을 때에도 이걸로

	FB2Wing CurrentAutoEnhanceWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(AutoEnhancementPCClass, CurrentAutoEnhanceWingData);

	const bool bCanAffordGold = HasEnoughGoldForWingEnhanceLocalPlayer(AutoEnhancementPCClass);
	const bool bCanAffordIngred = HasEnoughIngredForWingEnhanceLocalPlayer(AutoEnhancementPCClass);
	// 추가로 이제 진화할 때가 된 건 아닌지도 체크..
	const bool bEnhanceDoneForNow = bGotValidWingData && (IsItTimeToEvolve(CurrentAutoEnhanceWingData) || IsUltimatelyEvolvedWing(CurrentAutoEnhanceWingData));

	if (bGotValidWingData && bCanAffordGold && bCanAffordIngred && !bEnhanceDoneForNow)
	{
		data_trader::Retailer::GetInstance().RequestEnhanceWing(CliToSvrPCClassType(AutoEnhancementPCClass));
		CachedLastAutoEnhEvolExecTime = (float)FPlatformTime::Seconds();

		// 반드시 Response 가 온다는 보장은 없으니, 최후의 타이머를 설치
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
		// 서버에서 응답이 온 후에 다음 요청을 하게 되므로 응답이 올 때까지 걸린 시간은 감안해서 delay 를 정하도록 함.
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
	{ // 이런 경우가 있어선 안되겠지만..
		SingleAutoEnhancementTask();
	}
}

void UB2Airport::ForceEndAutoItemEnhanceTimerCB()
{
	EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Abnormal); // 물론 정상적인 종료가 아님.
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
	EndAutoEvolution(EWingAutoOpEndReason::WAEER_Custom); // Req 를 보낸 상황이든 아니든 이걸로 처리가 될 것으로 기대..
}

void UB2Airport::EndAutoEvolution(EWingAutoOpEndReason InEndReason)
{
	bIsInAutoEvolution = false;

	UpdateOnWingDataChange(AutoEvolutionPCClass, true); // 일반 강화 응답이 왔을 때도 보내는 UI 등 업데이트 신호

	if (IsValid(TransientLobbyGM))
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEvolutionResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);

		// 자동 진화 끝나는 것에 대한 추가적인 UI 업데이트
		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = TransientLobbyGM->DJLegacy_GetLobbyUIManager();
		if (DJLegacyLobbyUIMgr)
		{
			DJLegacyLobbyUIMgr->UpdateOnWingAutoEvolveComplete();
		}
	}

	if (InEndReason != EWingAutoOpEndReason::WAEER_Abnormal)
	{
		// 돈이나 재료가 없어서 종료된 것인지, 어떤 식으로든 진화가 완료된 것인지에 따라 다르게 반응.

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
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_AutoEvolve_Ended")), // 문구에는 추가 정보가 필요해질 수도..?
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::Confirm
					);
			}
		}
		else
		{
			// 여기선 무언가 할 필요가 없어야.. 진화가 성공했다는 얘기이므로 진화 연출 시작 신호가 이미 보내졌든지, 곧 다른 곳에서 보내든지.
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
		EndAutoEvolution(EWingAutoOpEndReason::WAEER_Graceful); // 이건 걍 성공했으니 중단해야.
	}
	else
	{
		// 추가로 진화 요청 보내는 게 가능한지 체크..
		FB2Wing CurrentAutoEvolveWingData;
		const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(AutoEvolutionPCClass, CurrentAutoEvolveWingData);
		
		const bool bCanAffordGold = HasEnoughGoldForWingEvolveLocalPlayer(AutoEvolutionPCClass);
		const bool bCanAffordIngred = HasEnoughIngredForWingEvolveLocalPlayer(AutoEvolutionPCClass);
		// 진화 타이밍이 맞는지도 다시 한번 체크를
		const bool bIsTimeForEvolution = bGotValidWingData && (IsItTimeToEvolve(CurrentAutoEvolveWingData) && !IsUltimatelyEvolvedWing(CurrentAutoEvolveWingData));
		
		if (bGotValidWingData && bCanAffordGold && bCanAffordIngred && bIsTimeForEvolution)
		{
			data_trader::Retailer::GetInstance().RequestUpgradeWing(CliToSvrPCClassType(AutoEvolutionPCClass));
			CachedLastAutoEnhEvolExecTime = (float)FPlatformTime::Seconds();
			// 반드시 Response 가 온다는 보장은 없으니, 최후의 타이머를 설치
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
					// 아래 루트에서는 WAEER_Graceful 이 있지만 실질적으로 그런 경우라면 bInLastEvolutionSucceeded 이 true 여야 할 것이다..
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
		// 서버에서 응답이 온 후에 다음 요청을 하게 되므로 응답이 올 때까지 걸린 시간은 감안해서 delay 를 정하도록 함.
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
	{// 이런 경우가 있어선 안되겠지만..
		SingleAutoEvolutionTask(bInLastEvolutionSucceeded);
	}
}

void UB2Airport::ForceEndAutoEvolutionTimerCB()
{
	EndAutoEvolution(EWingAutoOpEndReason::WAEER_Abnormal); // 물론 정상적인 종료가 아님.
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
{ // 속성옵션 강화에는 돈이 안 들어감
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
//		// 일단 로컬 데이터 값 세팅부터 먼저 함. 이거 visibility 를 변경하는 UI 에서 너무 잦은 변경으로 패킷이 왔다갔다 하는 상황을 피하기 위해 request 를 따로 날릴 수 있도록 하였음.
//		// 물론 FlushPendingWingVisibilityReq 를 적절한 시점에 날리지 않으면 다시 실행할 때에 변경한 것이 무용지물이 될 것이다.
//		const bool bChanged = (ThisCharWingContainer->WingData.bShouldBeVisible != bInShow);
//		ThisCharWingContainer->WingData.bShouldBeVisible = bInShow;
//
//		if (bChanged)
//		{// 로비 메쉬 데이터 업데이트
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InPCClass);
//		}
//
//		int32 WingPCClassIdx = PCClassToInt(InPCClass);
//
//		if (bReqToServerImmediately)
//		{
//			// 바로 서버로 요청하고 해당 클래스 요청 쌓인 게 있다면 제거
//			data_trader::Retailer::GetInstance().RequestSetWingVisible(CliToSvrPCClassType(InPCClass), bInShow);
//
//			PendingWingVisibilityReq.Remove(InPCClass);
//		}
//		else
//		{
//			// 따로 요청을 위해 pending 표시를 해 둔다.
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
	// SetWingVisibility 를 통해 쌓인 날개 visibility on/off 요청 서버로.. 하고 비움.
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
		// 날개 페이지를 최초로 열었을 때 기본값으로 날개를 켜도록 서버에 요청. 이후로는 사용자 설정에 따라.
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
	// 날개 메뉴에서 영웅관리 공통 캐릭터 초상화 위에 New 마크 띄우는 조건

	FB2Wing CheckWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCheckChar, CheckWingData);

	if (bGotValidWingData)
	{
		if (
			// 강화가 가능하거나
			(HasEnoughGoldForWingEnhanceLocalPlayer(InCheckChar) && HasEnoughIngredForWingEnhanceLocalPlayer(InCheckChar)) ||
			// 진화가 가능하거나
			(IsItTimeToEvolve(CheckWingData) && HasEnoughGoldForWingEvolveLocalPlayer(InCheckChar) && HasEnoughIngredForWingEvolveLocalPlayer(InCheckChar))
			)
		{
			return true;
		}

		// 아니면 속성옵션 강화라도 가능하거나
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
	// 날개 메뉴에서 속성 옵션 아이콘 위에 New 마크 띄우는 조건

	FB2Wing CheckWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCheckChar, CheckWingData);
	const FWingPropOption* CheckOptionPtr = bGotValidWingData ? CheckWingData.GetPropOptionPtr(InPropOptionIndex) : NULL;

	if (bGotValidWingData && CheckOptionPtr)
	{
		// 더 강화가 가능한 열려있는 옵션이 있고 재화도 충분한지
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
		UE_LOG(LogBladeII, Log, TEXT("Request dummy wing data to server is NOT implemented yet!")); // 인벤토리의 경우 참고..
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
				
				// 진화 메뉴를 테스트해야겠어서 원래보다 높은 확률로 최대 강화레벨을 채워준다..
				if (FMath::RandRange(1, 4) == 1){
					NewWing.EnhanceLevel = MAX_WING_ENHANCE_LEVEL;
					NewWing.EvolutionFailedCount = FMath::RandRange(0, 99); // 실패 횟수도 대략 채워줌.
				}
				else{
					NewWing.EnhanceLevel = FMath::RandRange(MIN_WING_ENHANCE_LEVEL, MAX_WING_ENHANCE_LEVEL - 1);
					NewWing.EnhancePoint = FMath::RandRange(0, 99); // 대략 이 사이일 듯..
				}
								
				for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
				{
					if (NewWing.PropOptions.IsValidIndex(POI))
					{
						FWingPropOption& ThisOption = NewWing.PropOptions[POI];
						ThisOption.MaxOptionLevel = 30; // 대략 이쯤일 것.
						ThisOption.OptionLevel = FMath::RandRange(1, ThisOption.MaxOptionLevel);
						// 여기도 어느 정도 높은 확률로 최대 강화레벨을 채워준다..
						if (FMath::RandRange(1, 6) == 1){
							ThisOption.OptionLevel = ThisOption.MaxOptionLevel;
						}
					}
				}

				if (!NewWing.UpdateStats()) 
				{
					// UpdateStats 는 마스터 데이터가 있을 때만 제대로 작동하므로 실패하면 기타 능력치도 가라로 맞춰줌.
					float BasePointGrade = NewWing.EvolutionGrade * 100.0f;
					NewWing.SetAttackPoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));
					NewWing.SetDefensePoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));
					NewWing.SetHealthPoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));

					for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
					{
						EItemOption MostLikeWingOption = EItemOption::EIO_Defense_DecAbnormalStateTime;
						// 걍 여긴 초기 기획에 따라 대체로 사용할 거 같은 옵션 넣어주는 것임.
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
							// 진화레벨에 따라 속성옵션이 열리는 식임.
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
