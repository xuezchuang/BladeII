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
	// ������ ������ �ִ��� ���ΰ� ���� �������ݿ� ���Ե� ���� �ֱ� �ѵ�.. �ϴ� �̷���.
	bHasWing = (WingData.EvolutionGrade >= MIN_WING_EVOLUTION_GRADE && WingData.EnhanceLevel >= MIN_WING_ENHANCE_LEVEL);
}

FWingMD_EnhanceSingleElem& FWingMD_EnhanceSingleElem:: operator=(const b2network::B2mdWingEnhancePtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetGrade = ServerStruct->grade;
		this->TargetLevel = ServerStruct->level;

		// ������ �̹� �־������ Ȯ�� ��������.. ���� �ǹ��־� ���̴� ���ε� �ٸ��� �ٲ�Ű� �ƴϸ� ��� �߸��� ��
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingEnhanceResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingEnhanceResourceRefID();	// �� �̰ɷ� ����.. ����

		this->RequiredItemCount = ServerStruct->need_item_count;
		this->EnhancePointToAcquirePerTrial = ServerStruct->enhance_point;
		this->RequiredEnhancePoint = ServerStruct->need_enhance_point;
		this->RequiredGoldCost = ServerStruct->gold_cost;

		// option_type �� �ϴ� �� ���� �ɷ�..
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

		// ������ �̹� �־������ Ȯ�� ��������. ���� �ǹ��־� ���̴� ���ε� �ٸ��� �ٲ�Ű� �ƴϸ� ��� �߸��� ��
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingEvolveResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingEvolveResourceRefID(); // �� �̰ɷ� ���� ����

		this->RequiredItemCount = ServerStruct->need_item_count;
		this->RequiredGoldCost = ServerStruct->gold_cost;
		this->SuccessRate = (float)ServerStruct->rate * 0.0001f; // �������� �� rate �� �������̶� �������� ��.
		this->MaxFailPoint = ServerStruct->fail_point_max;

		// option_type �� �ϴ� �� ���� �ɷ�..
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
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // �������� 1���� �����ϴ� ���� �� 
		this->TargetOptionLevel = ServerStruct->level;
		this->OptionType = SvrToCliOptionType(ServerStruct->option_type);
		this->OptionValue = ServerStruct->option_value;

		// ������ �̹� �־������ Ȯ�� ��������.. ���� �ǹ��־� ���̴� ���ε� �ٸ��� �ٲ�Ű� �ƴϸ� ��� �߸��� ��
		ensure(ServerStruct->need_item_template_id <= 0 || ServerStruct->need_item_template_id == FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID());
		this->RequiredItemRefID = FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID(); // �� �̰ɷ� ����.. ����

		this->RequiredItemCount = ServerStruct->need_item_count;
	}

	return *this;
}

FWingMD_PropOptionOpenSingleElem& FWingMD_PropOptionOpenSingleElem::operator = (const b2network::B2mdWingOptionOpenPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // �������� 1���� �����ϴ� ���� ��
		this->RequiredWingGrade = ServerStruct->need_grade;
	}
	return *this;
}

FWingMD_PropOptionMaxLevelSingleElem& FWingMD_PropOptionMaxLevelSingleElem::operator = (const b2network::B2mdWingOptionMaxLevelPtr& ServerStruct)
{
	if (ServerStruct)
	{
		this->TargetPCClass = SvrToCliPCClassType(ServerStruct->character_type);
		this->TargetOptionIndex = SvrToCliWingPropOptionIndex(ServerStruct->option_index); // �������� 1���� �����ϴ� ���� ��
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
	// ���� ������ �� ��ġ ������ �Լ����� Wing �� ���� ó���� ��. (UB2LobbyInventory::GetEquippedOptionPoint)
	// �̰� ���� ����ؼ� �ٸ� ���� ���� ��ġ�� ��ġ���� ECombatOptionApplyType �� ���� ���ϰų� ���ϰų��� ���� �������ƾ� ��.

	TArray<FOptionValue> GatheredOptionRawValue; // �����δ� �ϳ��� �� �־�� �� ��.

	for (const FWingPropOption& ThisOption : PropOptions)
	{
		if (ThisOption.MyOptionType == InOptionType)
		{
			FOptionValue OptionValue = FOptionValue(ThisOption.RawOptionAmount);
			GatheredOptionRawValue.Add(OptionValue);
		}
	}
	// ���� Item �� ���� �ɼ� ü�踦 ���..
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

	// ���� �⺻���� ���������� ���� ���� �޴� �Լ�. ���� ĳ���� �÷��̾��� ���� �̰ɷ� ���� �ʰ� �� ��.

	if (InResponseGetWingPtr)
	{
		OwnerPCClass = PCClassToInt(SvrToCliPCClassType(InResponseGetWingPtr->character_type));
		EvolutionGrade = InResponseGetWingPtr->grade;
		EvolutionFailedCount = InResponseGetWingPtr->upgrade_fail_point;
		EnhanceLevel = InResponseGetWingPtr->level;
		EnhancePoint = InResponseGetWingPtr->enhance_point;

		// bShouldBeVisible �� ���⼭ �������� �ʴ´�.

		// ���⼱ �ɼ� �����Ͱ� �� �;� �� ��.. ���� �ֵ� ���� ���� �ʴ� ����.
		ensure(InResponseGetWingPtr->wing_option_infos.Num() == PropOptions.Num());
		for (int32 POI = 0; POI < InResponseGetWingPtr->wing_option_infos.Num(); ++POI)
		{
			const b2network::B2WingOptionInfoPtr& ThisServerInfo = InResponseGetWingPtr->wing_option_infos[POI];
			UpdateSinglePropOptionByServerInfo(ThisServerInfo, false);
		}
	}

	// ��Ÿ �ٸ� �����͵��� ���ݱ��� ���õ� �� ������� ������ ���̺� ������.
	UpdateStats();
}

void FB2Wing::UpdateByPartialServerInfo(const b2network::B2CharacterServerInfoPtr& InCharServerInfo, const TArray<b2network::B2WingOptionInfoPtr>& InWingOptionInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateByPartialServerInfo"));

	// ���� ĳ������ ���� ��ȭ/��ȭ ���� ������ ������ �ܰ� ǥ�ó� ���� ��ġ ���� � �ʿ��� �������� ��� �̰ɷ� ���� �� ����.
	// �ʱ� AccountInfo �� ���Ե� ���� ������ �� ���� ĳ���� ���� ������ �̰ɷ� ������Ʈ.

	if (InCharServerInfo)
	{
		OwnerPCClass = PCClassToInt(SvrToCliPCClassType(InCharServerInfo->character_type));
		EvolutionGrade = InCharServerInfo->wing_grade;
		EnhanceLevel = InCharServerInfo->wing_level;
		bShouldBeVisible = InCharServerInfo->wing_visible;

		// EvolutionFailedCount �� EnhancePoint �� ���⼭ �������� �ʴ´�. �� ���� ���������� �ʿ���� ����.
	}
	//  ���� �ѹ��� GetWing �� ���� ���� ĳ���Ͷ�� �ɼ� �����Ͱ� �� �� �� �ִ�. �װ� �ƴ϶�� �⺻ ������ ���缭 ���� �ɷ�..
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
		{ // �������� �ε����� �����ְ� type �� �����͵����� �������.
			FWingPropOption& MyOptionInfo = PropOptions[CliOptionIndex];
			MyOptionInfo.OptionLevel = InServerInfoPtr->level;
		}
	}
	if (bUpdateAllImmediate)
	{ // ��Ÿ ������ �����ʹ� UpdateStats �� ��ġ�鼭 �����͵����ͷ������ ä����. �̰� ���� �θ��� �� ����.
		UpdateStats();
	}
}

bool FB2Wing::UpdateStats()
{
	B2_SCOPED_TRACK_LOG(TEXT("FB2Wing::UpdateStats"));

	bool bRetVal = true;

	//// EvolveGrade, EnhanceLevel �� �⺻ �����Ͱ� �� �ִٴ� �������� ��Ÿ ������ �ɷ�ġ ���� ������ �����Ϳ��� ����ؼ� ä���ִ� ��.

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
	//		// �Ƹ��� �׽�Ʈ ��ɿ����� �̰� �ʿ��� ��Ȳ�� ���� �� ���� ������..
	//		if (ThisPropOption.MaxOptionLevel > 0){ // �׸��� �ش� �ɼ��� ���� �ִ� ��޿����� �̰� 0 �� �� �ִµ� 0���� Ŭ���� �Ǹ� ������ �Ǵ� �װ� ����. 
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
	// �������� �������� ������ ������ ����� ��..

	if (InGrade >= MIN_WING_EVOLUTION_GRADE)
	{
		OutWingContainder.WingData.OwnerPCClass = PCClassToInt(InPCClass);
		OutWingContainder.WingData.EvolutionGrade = FMath::Min(InGrade, MAX_WING_EVOLUTION_GRADE_UNOFFICIAL);

		// ������ �����ʹ� ������ ä���ִµ�.. ���� �� �մ� �͵� ���� ��.
		OutWingContainder.WingData.EnhanceLevel = MIN_WING_ENHANCE_LEVEL;
		
		// ���ϰ� ���⼭ �����͵����ͷ� ���� �͵��� ä������ �Ŵ� �׳��� Grade �� ������ ������� �� ��.
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
{ // ini �ε��� �־��µ� ���� ��ġ�� �Ű���..
}

void UB2Airport::StaticOnModuleShutdown()
{
}

void UB2Airport::InitAirportLobby(class AB2LobbyGameMode* InLobbyGM)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::InitAirportLobby"));

	TransientLobbyGM = InLobbyGM;
	//check(TransientLobbyGM && TransientLobbyGM->IsValidObj()); // ��ü ������ LobbyGameMode ������

	SubscribeEvents();
}

void UB2Airport::ConditionalReqForFullWingData(EPCClass InWingPCClass)
{
	int32 WingPCClassIdx = PCClassToInt(InWingPCClass);
	if (WingPCClassIdx < GetMaxPCClassNum() && !FullWingDataReqState[WingPCClassIdx])
	{ // �� ��θ� ���� ��û�� ĳ���ʹ� �ѹ��� �ϸ� ��.
		// ������ OnReceiveGetWing ���� �ް� �� �ǵ� FullWingData ��� �ϱ⿡�� �̰ɷ� �������� �ʴ� �����͵� �ֱ� ������ AccountInfo �� ���� ���� �Ϳ� ���ؼ��� full data �� �����ٰ� �� �� �ְڴ�..
		data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(InWingPCClass));
	}
}

void UB2Airport::SetWingDataByAccountInfo(const TArray<b2network::B2CharacterServerInfoPtr>& InAllCharInfoArray, const TArray<b2network::B2WingOptionInfoPtr>& InAllWingOptionInfoArray)
{
	// ó���� AccountInfo �����鼭 �Ҹ��� ��. EnhancePoint, EvolutionFailedCount ���� Wing UI ������������ �ʿ��� �����ʹ� ���⼭ ���� �ʰ� �� ��.

	for (int32 CI = 0; CI < InAllCharInfoArray.Num(); ++CI)
	{
		const b2network::B2CharacterServerInfoPtr& ThisCharInfo = InAllCharInfoArray[CI];

		// ��ü WingOptionInfoArry �߿��� �̹� ĳ���� �͸� ����. ���� �ѹ��� GetWing �� ���� ���� ĳ���Ͷ�� �ɼ� �����Ͱ� �� �� �� �ִ�.
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
			ThisWingContainder->SetHasWingByWingData(); // bHasWing �� ���� �������� �޾ƾ� �� �Ŵ� ������ �ϴ� ������..

			// �� ���������� �Ƹ� �ʼ� �����͵����Ͱ� ���� �Ŷ� �����͵����� ���� �Ŀ� UpdateStats �� �ٽ� �ҷ��־�� �� ��.
		}
	}
}

void UB2Airport::UpdateWingCombatStatsOnMD()
{
	// UB2Airport::SetWingDataByAccountInfo �� �Ҹ� �������� ���� �����͵����Ͱ� ���� ���̹Ƿ� ���� �̷��� UpdateStats �� �ҷ��ش�.
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
	// AccountInfo �� �ʼ����� ���� �����Ͱ� ���� ��Ȳ���� AccountInfo �� MasterData �ް� ���� �ʱ⿡ ���� ������ �ϰ� ��û ��.
	// ������ �׳� for �� ������ ��� Ŭ������ ���� RequestGetWing ������ �� �ǵ� �̰� ���� ������ ��û�� �ѹ��� �ѹ��� ������ �ϴ� ������ �־���.

	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::BeginInitReqForWingData"));

	bIsRequestingWingDataForInit = true;

	// �۷������ͺ��� �ؼ� ����
	checkSlow((int32)EPCClass::EPC_Gladiator == 0);
	data_trader::Retailer::GetInstance().RequestGetWing(CliToSvrPCClassType(EPCClass::EPC_Gladiator));
}

void UB2Airport::OnReceiveGetWingCheckForContinueReq(const FB2Wing& ReceivedWingData)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveGetWingCheckForContinueReq"));

	EPCClass ThisWingPCClass = IntToPCClass(ReceivedWingData.OwnerPCClass);
	EPCClass NextPCClass = IntToPCClass(ReceivedWingData.OwnerPCClass + 1);
		
	if (ThisWingPCClass == EPCClass::EPC_End || NextPCClass == EPCClass::EPC_End)
	{ // ���� �߸��Ǿ��ų� �� �޾����� ���⼭ ����
		bIsRequestingWingDataForInit = false;
	}
	else if (NextPCClass != EPCClass::EPC_End)
	{ // �� ���� req
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
{ // ���⿣ Response �����͸� FB2Wing ���� �ٲ㼭 �Ѱ���.

	B2_SCOPED_TRACK_LOG(TEXT("UB2Airport::OnReceiveGetWing"));

	EPCClass WingPCClass = IntToPCClass(GetWingData.OwnerPCClass);

	FPCWingContainer* WingContainder = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWingDataPtr(WingPCClass);
	if (WingContainder)
	{
		// �� �������ݷδ� Visible ���ΰ� ���� �ʰ� ��� ������ SetWingDataByAccountInfo �� ���� Visible �� ���� ���õǾ��� ġ�� �װ� ����ϱ� ���� ���.
		const bool bPrevVisible = WingContainder->WingData.bShouldBeVisible; 

		WingContainder->WingData = GetWingData;
		// bHasWing �� ���� �������� �޾ƾ� �� �Ŵ� ������ �ϴ� ������..
		WingContainder->SetHasWingByWingData();

		// ������ �޾Ƴ��� Visible ���η� ����.
		WingContainder->WingData.bShouldBeVisible = bPrevVisible;

		int32 WingPCClassIdx = PCClassToInt(WingPCClass);
		FullWingDataReqState[WingPCClassIdx] = true; // � ��θ� ���� �� �����̰� �ѹ� �ް� ���� �� �̻� �� ��η� ���� �ʾƵ� �ǹǷ� ǥ��.

		UpdateOnWingDataChange(WingPCClass, true);
	}

	if (bIsRequestingWingDataForInit) // �ʱ� �ϰ� ��û�� ���
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

	//	WingContainder->WingData.UpdateStats(); // ������ �����Ϳ� ����� ������ ��ġ �� ������Ʈ.

	//	BladeIIGameImpl::GetClientDataStore().OnReceiveEnhanceWing(EnhanceWingData); // ClientDataStore �� �߰� ó��. ���� �������� ������Ʈ ��

	//	UpdateOnWingDataChange(WingPCClass, !bIsInAutoEnhancement); // UI �� �޽� ������Ʈ ��. �ڵ���ȭ ���߿��� UI �� ������Ʈ �ǰ�.
	//			
	//	// UI �ʿ� �ִϸ��̼� �÷��̸� �� �ϴµ� �̰ͱ��� Event ���� ������ �� ���ϰ� ���� UI �ý����̴� �� �̷��� ��
	//	// �ڵ���ȭ������ �ϴ� �� ����.
	//	if (WingContainder->WingData.EnhanceLevel > PrevEnhanceLevel)
	//	{ // WingEnhancedAnim �� EnhancePoint �� �ƴ� ���� �ɷ�ġ�� ����Ǵ� EnhanceLevel ���� �ÿ� �÷���. 
	//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (TransientLobbyGM && TransientLobbyGM->IsValidObj()) ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//		if (DJLegacyLobbyUIMgr)
	//		{
	//			DJLegacyLobbyUIMgr->PlayWingEnhancedAnim();
	//			LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
	//		}
	//	}

	//	if (bIsInAutoEnhancement)
	//	{ // �ڵ���ȭ�� ��쿡 ���� ó��.
	//		ensure(AutoEnhancementPCClass == WingPCClass); // �ڵ���ȭ ���߿� ���� Ŭ������ ���� ��û �����ٸ� �ɸ� ����.. �ڵ���ȭ �� ���� Ŭ�� ���ϰ� �ؾ� �� ��.

	//		// �ϴ��� �ڵ� ��ȭ ���� �ٸ� Ŭ���� ������ ��ȭ ������ �Դٸ� �ڵ� ��ȭ�� �ߴܽ�Ű�� �ɷ� ����. 
	//		if (AutoEnhancementPCClass == WingPCClass){
	//			ReserveOrExecSingleAutoEnhancementTask(); // SingleAutoEnhancementTask ������ ������ �Ǹ� ���������� �ڵ� ��ȭ�� �����. Ȥ�� ���� ��û
	//		}
	//		else{
	//			EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Abnormal); // ����� ���� ���������� �ڵ���ȭ ������.
	//		}
	//	}

	//	//////////////////////////////////////////////////////////////////
	//	//�׼� ��ǥ �α� (ĳ���� ���� ��ȭ)
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
//		// ������ ��� ��ȭ ���� ��� UI â�� �� stat �� �Ѹ��� ���� ������Ʈ �Ǳ� �� �����͸� ����Ѵ�.
//		if (bEvolutionSucceeded)
//		{
//			TemporaryPreEvolveData = WingContainder->WingData;
//			bPreEvolveDataValid = true; // ��ȭ ���� ���� �� ���µ� ��.
//			
//			//////////////////////////////////////////////////////////////////
//			//�׼� ��ǥ �α� (ĳ���� ���� �±�(��ȭ))
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
//		checkSlow( // ������ �Ͱ� ��ġ�ϴ���..
//			(bEvolutionSucceeded && (EvolGradeBefore + 1) == WingContainder->WingData.EvolutionGrade) ||
//			(!bEvolutionSucceeded && (FailCountBefore + 1) == WingContainder->WingData.EvolutionFailedCount)
//			);
//#endif
//
//		WingContainder->WingData.UpdateStats(); // ������ �����Ϳ� ����� ������ ��ġ �� ������Ʈ.
//
//		BladeIIGameImpl::GetClientDataStore().OnReceiveUpgradeWing(UpgradeWingData); // ClientDataStore �� �߰� ó��. ���� �������� ������Ʈ ��
//
//		UpdateOnWingDataChange(WingPCClass, bEvolutionSucceeded); // UI �� �޽� ������Ʈ ��. �޽� ������Ʈ�� ���� �ÿ���
//
//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (TransientLobbyGM && TransientLobbyGM->IsValidObj()) ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
//		if (!bEvolutionSucceeded)
//		{ // ��ȭ ���� �޽����� ���̴µ� �̰ͱ��� Event ���� ������ �� ���ϰ� ���� UI �ý����̴� �� �̷��� ��
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
//			// ��ȭ ������ ���� ���� �÷���
//			BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Signal(WingPCClass);
//		}
//
//		if (bIsInAutoEvolution)
//		{ // �ڵ���ȭ�� ��쿡 ���� ó��.
//			ensure(AutoEvolutionPCClass == WingPCClass); // �ڵ���ȭ ���߿� ���� Ŭ������ ���� ��û �����ٸ� �ɸ� ����.. �ڵ���ȭ �� ���� Ŭ�� ���ϰ� �ؾ� �� ��.
//
//			// �ϴ��� �ڵ� ��ȭ ���� �ٸ� Ŭ���� ������ ��ȭ ������ �Դٸ� �ڵ� ��ȭ�� �ߴܽ�Ű�� �ɷ� ����. 
//			if (AutoEvolutionPCClass == WingPCClass){
//				ReserveOrExecSingleAutoEvolutionTask(bEvolutionSucceeded); // SingleAutoEvolutionTask ������ ������ �Ǹ� ���������� �ڵ� ��ȭ�� �����. Ȥ�� ���� ��û
//			}
//			else{
//				EndAutoEvolution(EWingAutoOpEndReason::WAEER_Abnormal); // ����� ���� ���������� �ڵ���ȭ ������.
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
	//	FWingPropOption OptionDataBackupBeforeEnhance; // ������ �������ִ� �ɼ� ����Ÿ
	//	FWingPropOption OptionDataBackupAfterEnhance; // ����� �ɼǵ���Ÿ

	//	if (EnhancedPropOptionPtr)
	//		OptionDataBackupBeforeEnhance = *EnhancedPropOptionPtr;
	//	
	//	WingContainder->WingData.UpdateStats(); // ������ �����Ϳ� ����� ������ ��ġ �� ������Ʈ.

	//	if (EnhancedPropOptionPtr)
	//		OptionDataBackupAfterEnhance = *EnhancedPropOptionPtr;

	//	// Ȥ�ø��� �ѹ� �� üũ
	//	if (OptionDataBackupAfterEnhance.MyOptionType == OptionDataBackupBeforeEnhance.MyOptionType)
	//	{
	//		// �������� ����Ȱ��� ���̸�ŭ �˾����� ���
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
	//		//�׼� ��ǥ �α� (ĳ���� ���� �ɼ� ��ȭ)
	//		B2ActionLogSender Sender;
	//		Sender.setCategory(FString("CHARACTER"))
	//			  .setAction(FString("WING_OPTION"))
	//			  .setLabel(B2ActionLogType<EPCClass>::GetName(WingPCClass))
	//			  .setActionAttr1(OptionText.ToString().ToUpper())
	//			  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//		//////////////////////////////////////////////////////////////////
	//	}

	//	BladeIIGameImpl::GetClientDataStore().OnReceiveEnhanceWingOption(EnhanceWingOptionData); // ClientDataStore �� �߰� ó��. 

	//	UpdateOnWingDataChange(WingPCClass, false); // �Ӽ� �ɼǿ� ���� �޽��� �ٽ� ������ �ʿ�� ���� ��..?

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
		{ // ��� Ŭ���� ������Ʈ
			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
			{
				LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
			}
		}
		// CharacterIntro ������ ��� �������� ���µ� �ϴ� �� ȭ�鿡���� ������ ��� ������ ������ �Ŷ� ������ �ʿ���� �͵� ���� (FLobbySceneManager::SetupLobbyCharCompForParts ����)
		// �Դٰ� ���忡 �� �ñ׳��� �ð����� �ΰ� ���鼭 �÷��̵Ǵ� �ִϸ��̼��� ����� ������ �ֱ� ����.
		else if (TransientLobbyGM && TransientLobbyGM->GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_CharacterIntro)
		{
			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InChangedWingPCClass);
		}
	}
	LobbyUpdateWingMainUIClass<>::GetInstance().Signal(); // Wing Main UI ������ ������Ʈ
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
	EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Custom); // Req �� ���� ��Ȳ�̵� �ƴϵ� �̰ɷ� ó���� �� ������ ���..
}

void UB2Airport::EndAutoItemEnhance(EWingAutoOpEndReason InEndReason)
{
	bIsInAutoEnhancement = false;
	
	UpdateOnWingDataChange(AutoEnhancementPCClass, true); // �Ϲ� ��ȭ ������ ���� ���� ������ UI �� ������Ʈ ��ȣ
		
	if (IsValid(TransientLobbyGM))
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEnhanceResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);

		// �ڵ� ��ȭ ������ �Ϳ� ���� �߰����� UI ������Ʈ
		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = TransientLobbyGM->DJLegacy_GetLobbyUIManager();
		if (DJLegacyLobbyUIMgr)
		{
			DJLegacyLobbyUIMgr->UpdateOnWingAutoEnhanceComplete();
		}
	}

	if (InEndReason != EWingAutoOpEndReason::WAEER_Abnormal)
	{
		// NormalEnd ���ٰ� �ϴ��� ���̳� ��ᰡ ��� ����� ������, ��ȭ�� ����Ǿ� ���� ��ȭ�� �� �ܰ谡 �Ǿ� ����� �������� ���� �ٸ��� ����.
		// NormalEnd �� �ƴ� �� �ý��� ������ �̻��� ��Ȳ���� ����� ��.

		if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordGold)
		{
			ShortageMGR->PopupGoToShopForGoldMessage();
		}
		else if (InEndReason == EWingAutoOpEndReason::WAEER_NormalButCannotAffordIngredient)
		{
			OpenWingIngredAcqPathPopup(EWingIngredAcqElemType::WAET_EnhanceIngred);
		}
		else
		{ // ���� �������� ����. �ڵ� ��ȭ�� ���� Graceful, Custom ��� �̰ɷ�.
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst)
			{
				UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_AutoEnhance_Ended")), // �������� �߰� ������ �ʿ����� ����..?
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

	// ���� ���¸� üũ�ؼ� ��� ���� ��ȭ ��û�� �����ų� Ȥ�� �ߴ�.
	// AutoEnhancement ���߿� ���� ��ȭ ��û�� ���� ������ ���� ������ �̰ɷ�

	FB2Wing CurrentAutoEnhanceWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(AutoEnhancementPCClass, CurrentAutoEnhanceWingData);

	const bool bCanAffordGold = HasEnoughGoldForWingEnhanceLocalPlayer(AutoEnhancementPCClass);
	const bool bCanAffordIngred = HasEnoughIngredForWingEnhanceLocalPlayer(AutoEnhancementPCClass);
	// �߰��� ���� ��ȭ�� ���� �� �� �ƴ����� üũ..
	const bool bEnhanceDoneForNow = bGotValidWingData && (IsItTimeToEvolve(CurrentAutoEnhanceWingData) || IsUltimatelyEvolvedWing(CurrentAutoEnhanceWingData));

	if (bGotValidWingData && bCanAffordGold && bCanAffordIngred && !bEnhanceDoneForNow)
	{
		data_trader::Retailer::GetInstance().RequestEnhanceWing(CliToSvrPCClassType(AutoEnhancementPCClass));
		CachedLastAutoEnhEvolExecTime = (float)FPlatformTime::Seconds();

		// �ݵ�� Response �� �´ٴ� ������ ������, ������ Ÿ�̸Ӹ� ��ġ
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
		// �������� ������ �� �Ŀ� ���� ��û�� �ϰ� �ǹǷ� ������ �� ������ �ɸ� �ð��� �����ؼ� delay �� ���ϵ��� ��.
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
	{ // �̷� ��찡 �־ �ȵǰ�����..
		SingleAutoEnhancementTask();
	}
}

void UB2Airport::ForceEndAutoItemEnhanceTimerCB()
{
	EndAutoItemEnhance(EWingAutoOpEndReason::WAEER_Abnormal); // ���� �������� ���ᰡ �ƴ�.
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
	EndAutoEvolution(EWingAutoOpEndReason::WAEER_Custom); // Req �� ���� ��Ȳ�̵� �ƴϵ� �̰ɷ� ó���� �� ������ ���..
}

void UB2Airport::EndAutoEvolution(EWingAutoOpEndReason InEndReason)
{
	bIsInAutoEvolution = false;

	UpdateOnWingDataChange(AutoEvolutionPCClass, true); // �Ϲ� ��ȭ ������ ���� ���� ������ UI �� ������Ʈ ��ȣ

	if (IsValid(TransientLobbyGM))
	{
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoEvolutionResponseWaitTH);
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(DelayedAutoEnhEvolExecTH);

		// �ڵ� ��ȭ ������ �Ϳ� ���� �߰����� UI ������Ʈ
		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = TransientLobbyGM->DJLegacy_GetLobbyUIManager();
		if (DJLegacyLobbyUIMgr)
		{
			DJLegacyLobbyUIMgr->UpdateOnWingAutoEvolveComplete();
		}
	}

	if (InEndReason != EWingAutoOpEndReason::WAEER_Abnormal)
	{
		// ���̳� ��ᰡ ��� ����� ������, � �����ε� ��ȭ�� �Ϸ�� �������� ���� �ٸ��� ����.

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
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_AutoEvolve_Ended")), // �������� �߰� ������ �ʿ����� ����..?
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::Confirm
					);
			}
		}
		else
		{
			// ���⼱ ���� �� �ʿ䰡 �����.. ��ȭ�� �����ߴٴ� ����̹Ƿ� ��ȭ ���� ���� ��ȣ�� �̹� ����������, �� �ٸ� ������ ��������.
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
		EndAutoEvolution(EWingAutoOpEndReason::WAEER_Graceful); // �̰� �� ���������� �ߴ��ؾ�.
	}
	else
	{
		// �߰��� ��ȭ ��û ������ �� �������� üũ..
		FB2Wing CurrentAutoEvolveWingData;
		const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(AutoEvolutionPCClass, CurrentAutoEvolveWingData);
		
		const bool bCanAffordGold = HasEnoughGoldForWingEvolveLocalPlayer(AutoEvolutionPCClass);
		const bool bCanAffordIngred = HasEnoughIngredForWingEvolveLocalPlayer(AutoEvolutionPCClass);
		// ��ȭ Ÿ�̹��� �´����� �ٽ� �ѹ� üũ��
		const bool bIsTimeForEvolution = bGotValidWingData && (IsItTimeToEvolve(CurrentAutoEvolveWingData) && !IsUltimatelyEvolvedWing(CurrentAutoEvolveWingData));
		
		if (bGotValidWingData && bCanAffordGold && bCanAffordIngred && bIsTimeForEvolution)
		{
			data_trader::Retailer::GetInstance().RequestUpgradeWing(CliToSvrPCClassType(AutoEvolutionPCClass));
			CachedLastAutoEnhEvolExecTime = (float)FPlatformTime::Seconds();
			// �ݵ�� Response �� �´ٴ� ������ ������, ������ Ÿ�̸Ӹ� ��ġ
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
					// �Ʒ� ��Ʈ������ WAEER_Graceful �� ������ ���������� �׷� ����� bInLastEvolutionSucceeded �� true ���� �� ���̴�..
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
		// �������� ������ �� �Ŀ� ���� ��û�� �ϰ� �ǹǷ� ������ �� ������ �ɸ� �ð��� �����ؼ� delay �� ���ϵ��� ��.
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
	{// �̷� ��찡 �־ �ȵǰ�����..
		SingleAutoEvolutionTask(bInLastEvolutionSucceeded);
	}
}

void UB2Airport::ForceEndAutoEvolutionTimerCB()
{
	EndAutoEvolution(EWingAutoOpEndReason::WAEER_Abnormal); // ���� �������� ���ᰡ �ƴ�.
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
{ // �Ӽ��ɼ� ��ȭ���� ���� �� ��
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
//		// �ϴ� ���� ������ �� ���ú��� ���� ��. �̰� visibility �� �����ϴ� UI ���� �ʹ� ���� �������� ��Ŷ�� �Դٰ��� �ϴ� ��Ȳ�� ���ϱ� ���� request �� ���� ���� �� �ֵ��� �Ͽ���.
//		// ���� FlushPendingWingVisibilityReq �� ������ ������ ������ ������ �ٽ� ������ ���� ������ ���� ���������� �� ���̴�.
//		const bool bChanged = (ThisCharWingContainer->WingData.bShouldBeVisible != bInShow);
//		ThisCharWingContainer->WingData.bShouldBeVisible = bInShow;
//
//		if (bChanged)
//		{// �κ� �޽� ������ ������Ʈ
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InPCClass);
//		}
//
//		int32 WingPCClassIdx = PCClassToInt(InPCClass);
//
//		if (bReqToServerImmediately)
//		{
//			// �ٷ� ������ ��û�ϰ� �ش� Ŭ���� ��û ���� �� �ִٸ� ����
//			data_trader::Retailer::GetInstance().RequestSetWingVisible(CliToSvrPCClassType(InPCClass), bInShow);
//
//			PendingWingVisibilityReq.Remove(InPCClass);
//		}
//		else
//		{
//			// ���� ��û�� ���� pending ǥ�ø� �� �д�.
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
	// SetWingVisibility �� ���� ���� ���� visibility on/off ��û ������.. �ϰ� ���.
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
		// ���� �������� ���ʷ� ������ �� �⺻������ ������ �ѵ��� ������ ��û. ���ķδ� ����� ������ ����.
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
	// ���� �޴����� �������� ���� ĳ���� �ʻ�ȭ ���� New ��ũ ���� ����

	FB2Wing CheckWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCheckChar, CheckWingData);

	if (bGotValidWingData)
	{
		if (
			// ��ȭ�� �����ϰų�
			(HasEnoughGoldForWingEnhanceLocalPlayer(InCheckChar) && HasEnoughIngredForWingEnhanceLocalPlayer(InCheckChar)) ||
			// ��ȭ�� �����ϰų�
			(IsItTimeToEvolve(CheckWingData) && HasEnoughGoldForWingEvolveLocalPlayer(InCheckChar) && HasEnoughIngredForWingEvolveLocalPlayer(InCheckChar))
			)
		{
			return true;
		}

		// �ƴϸ� �Ӽ��ɼ� ��ȭ�� �����ϰų�
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
	// ���� �޴����� �Ӽ� �ɼ� ������ ���� New ��ũ ���� ����

	FB2Wing CheckWingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCheckChar, CheckWingData);
	const FWingPropOption* CheckOptionPtr = bGotValidWingData ? CheckWingData.GetPropOptionPtr(InPropOptionIndex) : NULL;

	if (bGotValidWingData && CheckOptionPtr)
	{
		// �� ��ȭ�� ������ �����ִ� �ɼ��� �ְ� ��ȭ�� �������
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
		UE_LOG(LogBladeII, Log, TEXT("Request dummy wing data to server is NOT implemented yet!")); // �κ��丮�� ��� ����..
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
				
				// ��ȭ �޴��� �׽�Ʈ�ؾ߰ھ �������� ���� Ȯ���� �ִ� ��ȭ������ ä���ش�..
				if (FMath::RandRange(1, 4) == 1){
					NewWing.EnhanceLevel = MAX_WING_ENHANCE_LEVEL;
					NewWing.EvolutionFailedCount = FMath::RandRange(0, 99); // ���� Ƚ���� �뷫 ä����.
				}
				else{
					NewWing.EnhanceLevel = FMath::RandRange(MIN_WING_ENHANCE_LEVEL, MAX_WING_ENHANCE_LEVEL - 1);
					NewWing.EnhancePoint = FMath::RandRange(0, 99); // �뷫 �� ������ ��..
				}
								
				for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
				{
					if (NewWing.PropOptions.IsValidIndex(POI))
					{
						FWingPropOption& ThisOption = NewWing.PropOptions[POI];
						ThisOption.MaxOptionLevel = 30; // �뷫 ������ ��.
						ThisOption.OptionLevel = FMath::RandRange(1, ThisOption.MaxOptionLevel);
						// ���⵵ ��� ���� ���� Ȯ���� �ִ� ��ȭ������ ä���ش�..
						if (FMath::RandRange(1, 6) == 1){
							ThisOption.OptionLevel = ThisOption.MaxOptionLevel;
						}
					}
				}

				if (!NewWing.UpdateStats()) 
				{
					// UpdateStats �� ������ �����Ͱ� ���� ���� ����� �۵��ϹǷ� �����ϸ� ��Ÿ �ɷ�ġ�� ����� ������.
					float BasePointGrade = NewWing.EvolutionGrade * 100.0f;
					NewWing.SetAttackPoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));
					NewWing.SetDefensePoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));
					NewWing.SetHealthPoint(FMath::RandRange(BasePointGrade * 0.5f, BasePointGrade * 2.0f));

					for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
					{
						EItemOption MostLikeWingOption = EItemOption::EIO_Defense_DecAbnormalStateTime;
						// �� ���� �ʱ� ��ȹ�� ���� ��ü�� ����� �� ���� �ɼ� �־��ִ� ����.
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
							// ��ȭ������ ���� �Ӽ��ɼ��� ������ ����.
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
