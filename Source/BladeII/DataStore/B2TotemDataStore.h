// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class BLADEII_API FTotemDataStore
{
public:
	FTotemDataStore();
	~FTotemDataStore();

public:
	//Use only reference variable.
	FTotemDataStore(FTotemDataStore const&) = delete;
	FTotemDataStore& operator=(FTotemDataStore const&) = delete;

public:
	void SetMasterData(const FB2MasterDatas& InMasterDatas);

	TArray<FB2TotemRefineInfo>& GetTotemRefineInfos() { return CachedMDTotemRefineInfos; };

	FText GetTotemName(const FB2Totem& InTotem, bool IsMultiLine = true);
	bool GetTotemInfo(const int32& ItemTemplateID, FB2TotemInfo& OutTotemInfo);
	int32 GetTotemSellPrice(const FB2Totem InTotem);
	int32 GetTotemGroupID(const FB2Totem InTotem);

	bool GetTotemBonusOptionInfo(const int32& BonusOptionID, FB2TotemBonusOptionInfo& OutTotemInfo);
	bool GetTotemBonusOption(const FB2Totem& InTotem, TArray<FItemOption>& OutItemOption);
	FText GetLOCTextOfSkillOption(const EItemOption& ItemOption);
	int32 GetDismantlePieceCount(const FB2Totem& InTotem);
	bool GetTotemSubOptionMinMax(const ETotemGrade& Grade, const EItemOption& ItemOption, float& OutMinValue, float& OutMaxValue);
	ETotemEquipSlot GetTotemEquipSlot(const int32& InRefID);

	/////////////////////////////////////////////////////////////
	//  Dimensioninfos
	FB2DimensionInfo* GetDimensionInfo(int32 nDiff);
	TArray<FB2DimensionWeekRewardInfo> GetDimensionWeekRewardInfos();
	
	//////////Seal

	TArray<b2network::B2mdSelectItemSealOptionPtr>	GetSealOptionData(int32 InSlotNumber);
	b2network::B2mdItemSealCostPtr					GetSealCostData(int32 InSlotNumber);


private:
	FText GetTotemRefineText(const int32& RefineLevel);
	FText GetTotemMainOptionText(const ETotemBasePrefix& TotemBasePrefix);

private:
	TArray<FB2TotemInfo>								CachedMDTotemInfos;
	TArray<FB2SelectTotemOption>						CachedMDSelectTotemOptions;
	TArray<FB2TotemRefineInfo>							CachedMDTotemRefineInfos;
	TArray<FB2TotemRefineRateInfo>						CachedMDTotemRefineRateInfos;
	TArray<FB2TotemBonusOptionInfo>						CachedMDTotemBonusOptionInfoes;
	TArray<FB2TotemDismantleInfo>						CachedMDTotemDismantleInfos;

	/////////////////////////////////////////////////////////////
	//  Dimensioninfos
	TMap<int32, FB2DimensionInfo>						CachedMDDimensionInfos;
	TArray<FB2DimensionWeekRewardInfo>					CachedMDDimensionWeekRewardInfoes;


	//Seal Data
	TArray<b2network::B2mdItemSealCostPtr>									CachedMDSealCost;
	TMap<int32, TArray<b2network::B2mdSelectItemSealOptionPtr>>				CachedMDItemSealOptions;
};
