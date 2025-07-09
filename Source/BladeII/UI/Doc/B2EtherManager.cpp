// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2EtherManager.h"
#include "Event.h"
#include "Retailer.h"
#include "B2EtherInfo.h"
#include "B2EtherSetOptionInfo.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2EtherSetEffect.h"
#include "B2EtherContainer.h"
#include "B2SelectiveEtherLottery.h"



B2EtherManager::B2EtherManager()
{
}

B2EtherManager::~B2EtherManager()
{
}

B2EtherManager& B2EtherManager::GetInstance()
{
	static B2EtherManager EtherManager;
	return EtherManager;
}

void B2EtherManager::Init()
{
	SubscribeEvents();
}

void B2EtherManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryMasterDataClass<FB2MasterDatas>::GetInstance().Subscribe2(
		[this](const FB2MasterDatas& data)
	{
		this->ReceiveEtherMasterData(data);
	}
	));
	
	Issues.Add(SetEquippedEtherClass<const int32, const TArray<b2network::B2AetherServerInfoPtr>&>::GetInstance().Subscribe2(
		[this](const int32 InvenSlotCount, const TArray<b2network::B2AetherServerInfoPtr>& InEquippedEther)
	{
		this->InventoryCount = InvenSlotCount;
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().SetEquippedEther(InvenSlotCount, InEquippedEther);
	}
	));
}

void B2EtherManager::UnsubscribeEvents()
{
	Issues.Empty();
}

b2network::B2mdAetherSetOptionInfoPtr B2EtherManager::GetEtherSetOptionInfoPtrBySetId(int32 SetOptionId, int32 Tier)
{
	for (b2network::B2mdAetherSetOptionInfoPtr SetOptionInfoPtr : MdEtherSetOptionInfo)
	{
		if (SetOptionInfoPtr->set_option_id == SetOptionId && SetOptionInfoPtr->tier == Tier)
			return SetOptionInfoPtr;
	}

	return nullptr;
}

b2network::B2mdAetherSetOptionInfoPtr B2EtherManager::GetEtherSetOptionInfoPtrByType(int32 EtherType, int32 Tier)
{
	for (b2network::B2mdAetherSetOptionInfoPtr SetOptionInfoPtr : MdEtherSetOptionInfo)
	{
		if (SetOptionInfoPtr->type == EtherType && SetOptionInfoPtr->tier == Tier)
			return SetOptionInfoPtr;
	}

	return nullptr;
}

int32 B2EtherManager::GetMinApplyCount(int InEtherSetId) const
{
	for (b2network::B2mdAetherSetOptionInfoPtr Elem : MdEtherSetOptionInfo)
	{
		if (Elem->set_option_id == InEtherSetId)
		{
			return Elem->min_apply_count;
		}
	}

	return 0;
}

int32 B2EtherManager::GetEtherTypeFromSetId(int32 InEtherSetId) const
{
	for (b2network::B2mdAetherSetOptionInfoPtr Elem : MdEtherSetOptionInfo)
	{
		if (Elem->set_option_id == InEtherSetId)
		{
			return Elem->type;
		}
	}
	return 0;
}

void B2EtherManager::GetEquipEther(IN int32 EtherSetId, IN const TMap<int64, FB2Ether>& EquippedEthers, OUT TArray<FB2Ether>& Ethers)
{
	if (EtherSetId != 0)
	{
		int32 EtherType = GetEtherTypeFromSetId(EtherSetId);

		EquippedEthers.GenerateValueArray(Ethers);

		//타입으로 필터링
		Ethers = Ethers.FilterByPredicate
		(
			[EtherType](FB2Ether CurrEther)
		{
			if (CurrEther.Type == EtherType)
			{
				return true;
			}
			return false;
		}
		);

		//티어순으로 정렬
		Ethers.Sort([](const FB2Ether& A, const FB2Ether& B)
		{
			if (A.Tier > B.Tier)
			{
				return true;
			}
			return false;
		});
	}
}

float B2EtherManager::GetEtherSetOptionCoolDown(int32 EtherSetId, TMap<int64, FB2Ether>& EquippedEthers)
{
	if (EtherSetId == 0)
	{
		return 0.0f;
	}
	
	float CoolDown = 0.0f;
	TArray<FB2Ether> Ethers;
	int MinCount = GetMinApplyCount(EtherSetId);

	GetEquipEther(EtherSetId, EquippedEthers, Ethers);

	if (Ethers.Num() >= MinCount)
	{
		for (int i = 0; i < MinCount; ++i)
		{
			// 인자로받은 세트아이디랑 장착아이템 티어로 세트정보 찾음.
			b2network::B2mdAetherSetOptionInfoPtr EtherSetOptionInfoPtr = GetEtherSetOptionInfoPtrBySetId(EtherSetId, Ethers[i].Tier);
			CoolDown += EtherSetOptionInfoPtr->cooldown_second;
		}
	}
	return CoolDown;
}

float B2EtherManager::GetEtherSetOptionTriggerRate(int32 EtherSetId, TMap<int64, FB2Ether>& EquippedEthers)
{
	if (EtherSetId == 0)
	{
		return 0;
	}
	// 만분율
	TArray<FB2Ether> Ethers;
	int32 nRate = 0;
	int MinCount = GetMinApplyCount(EtherSetId);

	GetEquipEther(EtherSetId, EquippedEthers, Ethers);

	if (Ethers.Num() >= MinCount)
	{
		for (int i = 0; i < MinCount; ++i)
		{
			// 인자로받은 세트아이디랑 장착아이템 티어로 세트정보 찾음.
			b2network::B2mdAetherSetOptionInfoPtr EtherSetOptionInfoPtr = GetEtherSetOptionInfoPtrBySetId(EtherSetId, Ethers[i].Tier);
			nRate += EtherSetOptionInfoPtr->trigger_rate;
		}
	}

	float Result = 0;
	if (nRate != 0)
	{
		Result = nRate / 10000.0f;
	}

	return Result;
}

void B2EtherManager::GetEtherSetOptionValue(IN int32 EtherSetId, OUT float& OutOptionValue1, OUT float& OutOptionValue2)
{
	OutOptionValue1 = 0.0f;
	OutOptionValue2 = 0.0f;

	TSubclassOf<UB2EtherSetEffect> DefaultClass = StaticFindEtherSetEffectInfo()->GetEtherSetEffectClass(EtherSetId);
	if (UB2EtherSetEffect* DefaultObj = DefaultClass.GetDefaultObject())
	{
		switch (EtherSetId)
		{
		case b2network::B2AetherSetOptionType::THUNDER_DAMAGE:
		{
			UB2Ether_Thunder* SetEffect = Cast<UB2Ether_Thunder>(DefaultObj);
			if (SetEffect)
			{
				SetEffect->GetThunderDamageRate(OutOptionValue1, OutOptionValue2);
				OutOptionValue1 *= 100;
				OutOptionValue2 *= 100;
			}
			break;
		}
		case b2network::B2AetherSetOptionType::SHIELD:
		{
			UB2Ether_Shield* SetEffect = Cast<UB2Ether_Shield>(DefaultObj);
			if (SetEffect)
			{
				OutOptionValue1 = SetEffect->GetShieldRate();
				OutOptionValue1 *= 100;
			}
			break;
		}
		case b2network::B2AetherSetOptionType::METEOR_DAMAGE:
		{
			UB2Ether_Meteor* SetEffect = Cast<UB2Ether_Meteor>(DefaultObj);
			if (SetEffect)
			{
				OutOptionValue1 = SetEffect->GetDamageAmount();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::DAMAGE_REFLECT:
		{
			UB2Ether_Reflect* SetEffect = Cast<UB2Ether_Reflect>(DefaultObj);
			if (SetEffect)
			{
				OutOptionValue1 = SetEffect->GetReflectDamageAmount();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::FIRE_RANGE:
		{
			UB2Ether_FireRange* SetEffect = Cast<UB2Ether_FireRange>(DefaultObj);
			if (SetEffect)
			{
				OutOptionValue1 = SetEffect->GetDamageInfo()->StateDamageAmount;
			}
			break;
		}
		default:
			break;
		}
	}
}

FString B2EtherManager::GetEtherSetOptionDescTextTableKey(int32 EtherSetId)
{
	FString Result = TEXT("");

	switch (EtherSetId)
	{
	case b2network::B2AetherSetOptionType::THUNDER_DAMAGE:
		Result = "Ether_SetOption_Detail4";
		break;
	case b2network::B2AetherSetOptionType::SHIELD:
		Result = "Ether_SetOption_Detail6";
		break;
	case b2network::B2AetherSetOptionType::METEOR_DAMAGE:
		Result = "Ether_SetOption_Detail5";
		break;
	case b2network::B2AetherSetOptionType::DAMAGE_REFLECT:
		Result = "Ether_SetOption_Detail7";
		break;
	case b2network::B2AetherSetOptionType::FIRE_RANGE:
		Result = "Ether_SetOption_Detail8";
		break;
	default:
		break;
	}

	return Result;
}

FString B2EtherManager::GetEtherSetOptionUnitTableKey(int32 EtherSetId)
{
	FString Result = TEXT("");

	switch (EtherSetId)
	{
	case b2network::B2AetherSetOptionType::THUNDER_DAMAGE:
		Result = "Ether_PctDamage_ApplytoBoss";
		break;
	case b2network::B2AetherSetOptionType::SHIELD:
	case b2network::B2AetherSetOptionType::METEOR_DAMAGE:
	case b2network::B2AetherSetOptionType::DAMAGE_REFLECT:
	case b2network::B2AetherSetOptionType::FIRE_RANGE:
		Result = "General_Stat_Percent";
		break;
	default:
		break;
	}

	return Result;
}

FText B2EtherManager::GetEtherSetOptionText(int32 EtherSetId, TMap<int64, FB2Ether>& EquippedEthers)
{
	FText Result = FText::GetEmpty();;

	float Rate = GetEtherSetOptionTriggerRate(EtherSetId, EquippedEthers);
	Rate *= 100; // %이므로 변환
	if (StaticFindEtherSetOptionInfoTable() != NULL)
	{
		Result = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_OptionApply"))),
			StaticFindEtherSetOptionInfoTable()->GetEtherSetOptionName(EtherSetId), FText::AsNumber(Rate));
	}

	return Result;
}

FText B2EtherManager::GetEtherSetOptionText(int32 EtherType)
{
	FText Result = FText::GetEmpty();
	
	if (StaticFindEtherSetOptionInfoTable() != NULL)
	{
		Result = StaticFindEtherSetOptionInfoTable()->GetEtherSetOptionName(EtherType);
	}

	return Result;
}

int32 B2EtherManager::GetEnhanceCost(int32 Level, int32 Star)
{
	int32 Result = 0;
	for (const b2network::B2mdAetherEnhanceInfoPtr Elem : MdEtherEnhanceInfo)
	{
		if (Elem->enhance_level == Level && Elem->grade == Star)
		{
			Result = Elem->aether_piece_cost;
			break;
		}
	}
	return Result;
}

float B2EtherManager::GetEnhanceRate(int32 Level, int32 Star)
{
	float Result = 0;
	for (const b2network::B2mdAetherEnhanceInfoPtr Elem : MdEtherEnhanceInfo)
	{
		if (Elem->enhance_level == Level && Elem->grade == Star)
		{
			Result = Elem->enhance_rate * 0.0001f;
			break;
		}
	}
	return Result;
}

int32 B2EtherManager::GetEnhanceSubOptionState(int32 Level, int32 Tier)
{
	int32 Result = 0;
	for (const b2network::B2mdAetherEnhanceOptionInfoPtr Elem : MdEtherEnhanceOptionInfo)
	{
		if (Elem->enhance_level == Level && Elem->tier == Tier)
		{
			Result = Elem->state_sub_option; 
			break;
		}
	}
	return Result;
}

float B2EtherManager::GetEnhanceIncMainValue(int32 Id, int32 Star)
{
	for (const b2network::B2mdAetherOptionInfoPtr Elem : MdEtherOptionInfo)
	{
		if (Elem->option_id == Id && Elem->grade == Star)
		{
			return Elem->increase_value_per_level;

		}
	}
	return 0.0f;
}

int32 B2EtherManager::GetEtherPiece(int32 Star, int32 EnhanceLevel)
{
	int32 Result = 0;
	
	if (EnhanceLevel == 0)
	{
		EnhanceLevel = 1;
	}

	for (const b2network::B2mdAetherEnhanceInfoPtr Elem : MdEtherEnhanceInfo)
	{
		if (Elem->grade == Star&& Elem->enhance_level == EnhanceLevel)
		{
			Result += Elem->dismantle_amount;
			break;
		}
	}
	return Result;
}

int32 B2EtherManager::GetEtherPiece(int64 EtherID)
{
	const FB2Ether* Ether = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEther(EtherID);
	int32 Result = 0;

	if (Ether != nullptr)
	{
		int Star = Ether->Star;
		int EnhanceLevel = Ether->EnhanceLevel;

		if (EnhanceLevel == 0)
		{
			EnhanceLevel = 1;
		}

		for (const b2network::B2mdAetherEnhanceInfoPtr Elem : MdEtherEnhanceInfo)
		{
			if (Elem->grade == Star && Elem->enhance_level == EnhanceLevel)
			{
				Result += Elem->dismantle_amount;
				break;
			}
		}
		
	}
	return Result;
}

int32 B2EtherManager::GetEtherSellPrice(int32 Star, int32 EnhanceLevel)
{
	int32 Result = 0;

	if (EnhanceLevel == 0)
	{
		EnhanceLevel = 1;
	}

	for (const b2network::B2mdAetherEnhanceInfoPtr Elem : MdEtherEnhanceInfo)
	{
		if (Elem->grade == Star&& Elem->enhance_level == EnhanceLevel)
		{
			Result += Elem->sell_price;
			break;
		}
	}
	return Result;
}

int32 B2EtherManager::GetEtherSellPrice(int64 EtherID)
{
	const FB2Ether* Ether = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEther(EtherID);
	int32 Result = 0;

	if (Ether != nullptr)
	{
		int Star = Ether->Star;
		int EnhanceLevel = Ether->EnhanceLevel;

		if (EnhanceLevel == 0)
		{
			EnhanceLevel = 1;
		}

		for (const b2network::B2mdAetherEnhanceInfoPtr Elem : MdEtherEnhanceInfo)
		{
			if (Elem->grade == Star && Elem->enhance_level == EnhanceLevel)
			{
				Result += Elem->sell_price;
				break;
			}
		}

	}
	return Result;
}

FText B2EtherManager::GetEtherName(int32 Type)
{
	FText Result = FText::GetEmpty();
	if (StaticFindEtherInfoTable() != NULL)
	{
		FString EtherName = StaticFindEtherInfoTable()->GetEtherName(Type);
		Result = FText::FromString(EtherName);
	}
	return Result;
}

FText B2EtherManager::GetEtherName(int32 Type, int32 EnhanceLevel)
{
	FText Result = FText::GetEmpty();
	if (StaticFindEtherInfoTable() != NULL)
	{
		FString EtherName = StaticFindEtherInfoTable()->GetEtherName(Type);
		Result = FText::Format(FText::FromString(TEXT("+{0} {1}")), FText::AsNumber(EnhanceLevel), FText::FromString(EtherName));
	}
	return Result;
}

FText B2EtherManager::GetEtherTierName(int32 Tier)
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString::Printf(TEXT("EtherTier_%d"), Tier));
}

float B2EtherManager::GetEtherSetEffecttDuration(int32 EtherSetId)
{
	float Result = 0.0f;

	TSubclassOf<UB2EtherSetEffect> DefaultClass = StaticFindEtherSetEffectInfo()->GetEtherSetEffectClass(EtherSetId);
	if (UB2EtherSetEffect* DefaultObj = DefaultClass.GetDefaultObject())
	{
		switch (EtherSetId)
		{
		case b2network::B2AetherSetOptionType::IMMOBILIZE:
		{
			UB2Ether_Immobilize* SetEffect = Cast<UB2Ether_Immobilize>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::SHIELD:
		{
			UB2Ether_Shield* SetEffect = Cast<UB2Ether_Shield>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::SUPER_ARMOR:
		{
			UB2Ether_invincible* SetEffect = Cast<UB2Ether_invincible>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::STUN:
		{
			UB2Ether_Stun* SetEffect = Cast<UB2Ether_Stun>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::FIRE_RANGE:
		{
			UB2Ether_FireRange* SetEffect = Cast<UB2Ether_FireRange>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::PROHIBIT_BEHAVIOR:
		{
			UB2Ether_NotTagDefence* SetEffect = Cast<UB2Ether_NotTagDefence>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		case b2network::B2AetherSetOptionType::FREEZE_RANGE:
		{
			UB2Ether_FreezeRange* SetEffect = Cast<UB2Ether_FreezeRange>(DefaultObj);
			if (SetEffect)
			{
				Result = SetEffect->GetDuration();
			}
			break;
		}
		default:
			break;
		}
	}

	return Result;
}

void B2EtherManager::GetEtherLotteryInfo(int32 InRefID, int32& OutEtherStar, int32& OutEtherTier)
{
	if (StaticFindSelectiveEtherLotteryTable() != NULL)
	{
		OutEtherStar = StaticFindSelectiveEtherLotteryTable()->GetSelectiveEtherLotteryGrade(InRefID);
		OutEtherTier = StaticFindSelectiveEtherLotteryTable()->GetSelectiveEtherLotteryTier(InRefID);
	}
}

void B2EtherManager::ReceiveEtherMasterData(const FB2MasterDatas& InMasterDatas)
{
	//MdEtherOptionInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_aether_option_infos_index, InMasterDatas);
	//MdEtherSlotInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_aether_slot_infos_index, InMasterDatas);
	//MdEtherEnhanceInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_aether_enhance_infos_index, InMasterDatas);
	//MdEtherEnhanceOptionInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_aether_enhance_option_infos_index, InMasterDatas);
	//MdEtherSetOptionInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_aether_set_option_infos_index, InMasterDatas);	
}

