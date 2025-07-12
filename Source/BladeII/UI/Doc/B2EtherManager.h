// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "EventSubsystem.h"
#include "../../DataStore/B2EtherData.h"

const int32 MAX_ETHER_GRADE = 6;

class BLADEII_API B2EtherManager
{
public:
	B2EtherManager();
	~B2EtherManager();
	static B2EtherManager&	GetInstance();
	
	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();

	b2network::B2mdAetherSetOptionInfoPtr GetEtherSetOptionInfoPtrBySetId(int32 SetOptionId, int32 Tier);
	b2network::B2mdAetherSetOptionInfoPtr GetEtherSetOptionInfoPtrByType(int32 EtherType, int32 Tier);

	void GetEquipEther(IN int32 EtherSetId, IN const TMap<int64, FB2Ether>& EquippedEthers, OUT TArray<FB2Ether>& Ethers);
	float GetEtherSetOptionCoolDown(int32 EtherSetId, TMap<int64, FB2Ether>& EquippedEthers);
	float GetEtherSetOptionTriggerRate(int32 EtherSetId, TMap<int64, FB2Ether>& EquippedEthers);
	void GetEtherSetOptionValue(IN int32 EtherSetId, OUT float& OutOptionValue1, OUT float& OutOptionValue2);
	FString GetEtherSetOptionDescTextTableKey(int32 EtherSetId);
	FString GetEtherSetOptionUnitTableKey(int32 EtherSetId);
	FText GetEtherSetOptionText(int32 EtherSetId, TMap<int64, FB2Ether>& EquippedEthers);
	FText GetEtherSetOptionText(int32 EtherSetId);
	int32 GetMinApplyCount(int InEtherSetId) const;
	int32 GetEtherTypeFromSetId(int32 InEtherSetId) const;
	int32 GetEnhanceCost(int32 Level, int32 Star);
	float GetEnhanceRate(int32 Level, int32 Star);
	int32 GetEnhanceSubOptionState(int32 Level, int32 Tier); // 0 유지, 1 추가, 2 강화
	float GetEnhanceIncMainValue(int32 Id, int32 Star);
	int32 GetEtherPiece(int32 Star, int32 EnhanceLevel);
	int32 GetEtherPiece(int64 EtherID);
	int32 GetEtherSellPrice(int32 Star, int32 EnhanceLevel);
	int32 GetEtherSellPrice(int64 EtherID);
	FText GetEtherName(int32 Type);
	FText GetEtherName(int32 Type, int32 EnhanceLevel);
	FText GetEtherTierName(int32 Tier);
	int32 GetEtherInventoryCount() { return InventoryCount; }
	void  SetEtherInventoryCount(int32 Value) { InventoryCount = Value; }
	float GetEtherSetEffecttDuration(int32 EtherSetId);
	void GetEtherLotteryInfo(int32 InRefID, int32& OutEtherStar, int32& OutEtherTier);

private:
	void ReceiveEtherMasterData(const FB2MasterDatas& InMasterDatas);

private:
	int32														InventoryCount;
	TArray<b2network::B2mdAetherOptionInfoPtr>					MdEtherOptionInfo;
	TArray<b2network::B2mdAetherSlotInfoPtr>					MdEtherSlotInfo;
	TArray<b2network::B2mdAetherEnhanceInfoPtr>					MdEtherEnhanceInfo;
	TArray<b2network::B2mdAetherEnhanceOptionInfoPtr>			MdEtherEnhanceOptionInfo;
	TArray<b2network::B2mdAetherSetOptionInfoPtr>				MdEtherSetOptionInfo;
	TArray<std::shared_ptr<issue>>								Issues;
};
