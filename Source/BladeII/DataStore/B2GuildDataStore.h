#pragma once

#include "EngineMinimal.h"
#include "CommonStruct.h"
#include "Common.h"


struct FB2GuildSupportItem
{
	int32 ItemTempleteID;
	int32 SupportMaxCount;

	FB2GuildSupportItem(b2network::B2mdGuildSupportItemPtr Info);
};


struct FB2GuildMercenaryInfo
{
	TMap<int32,b2network::B2mdGuildMercenaryPtr> GuildMercenaries;
	TMap<int32,b2network::B2mdGuildMercenarySkillPtr> GuildMercenarySkill;
	TMap<int32, TMap<int32,b2network::B2mdGuildMercenaryMasteryPtr>> GuildMercenaryMasteries;
	TMap<int32,b2network::B2mdGuildMercenaryDonationPtr> GuildMercenaryDonations;

	void ClearGuildMercenary()
	{
		GuildMercenaries.Empty();
		GuildMercenarySkill.Empty();
		GuildMercenaryMasteries.Empty();
		GuildMercenaryDonations.Empty();
	}

	void AddMercenaries(b2network::B2mdGuildMercenaryPtr InMercenaries)
	{
		GuildMercenaries.Add(InMercenaries->id, InMercenaries);
	}

	void AddMercenarySkill(b2network::B2mdGuildMercenarySkillPtr InMercenaries)
	{
		GuildMercenarySkill.Add(InMercenaries->skill_id, InMercenaries);
	}

	void AddMercenaryMasteries(b2network::B2mdGuildMercenaryMasteryPtr InMercenaries)
	{
		if(GuildMercenaryMasteries.Contains(InMercenaries->id))
			GuildMercenaryMasteries[InMercenaries->id].Add(InMercenaries->level, InMercenaries);
		else
		{
			TMap<int32, b2network::B2mdGuildMercenaryMasteryPtr> InData;
			InData.Add(InMercenaries->level, InMercenaries);
			GuildMercenaryMasteries.Add(InMercenaries->id, InData);
		}
	}

	void AddMercenaryDonations(b2network::B2mdGuildMercenaryDonationPtr InMercenaries)
	{
		//뭐하는애인지 확인해야할듯
		/* B2mdGuildMercenaryDonation 타입 까먹을까봐	donation_type (기부타입 ex 골드보석)
														item_template_id (또다른 타입아이템 혹시나하는 대비)
														donation_amount	(1번 기부할때 드는 개수 ex 보석100개)
														exp_increment	(한번 기부할때 exp 증가량)*/


		GuildMercenaryDonations.Add(InMercenaries->donation_type, InMercenaries);

	}

	b2network::B2mdGuildMercenaryPtr GetMercenaries(int32 InID)
	{
		if (GuildMercenaries.Contains(InID))
			return GuildMercenaries[InID];

		return nullptr;
	}
	b2network::B2mdGuildMercenarySkillPtr GetMercenarySkill(int32 InID)
	{
		if (GuildMercenarySkill.Contains(InID))
			return GuildMercenarySkill[InID];

		return nullptr;
	}
	b2network::B2mdGuildMercenaryMasteryPtr GetMercenaryMasteries(int32 ID,int32 Lv)
	{
		if (GuildMercenaryMasteries.Contains(ID))
		{
			if (GuildMercenaryMasteries[ID].Contains(Lv))
				return GuildMercenaryMasteries[ID][Lv];
		}

		return nullptr;
	}

	int32 GetMercenaryMaxLevel(int32 ID)
	{

		for (auto El : GuildMercenaryMasteries)
		{
			if (El.Key == ID)
				return El.Value.Num();
		}

		return 0;
	}
	b2network::B2mdGuildMercenaryDonationPtr GetMercenaryDonations(int32 InType)
	{
		if (GuildMercenaryDonations.Contains(InType))
			return GuildMercenaryDonations[InType];

		return nullptr;
	}

	const TMap<int32, b2network::B2mdGuildMercenaryPtr>& GetMercenariesArr()
	{
		return GuildMercenaries;
	}

	int32 GetMercenaryMasteriesCount(int32 InNPCID)
	{
		if (GuildMercenaryMasteries.Contains(InNPCID))
		{
			return GuildMercenaryMasteries[InNPCID].Num();
		}

		return 0;
	}
};


class FGuildDataStore
{
public:
	FGuildDataStore();

	//Use only reference variable.
	FGuildDataStore(FGuildDataStore const&) = delete;
	FGuildDataStore& operator=(FGuildDataStore const&) = delete;

	// Interface
public:
	void SetMasterData(const FB2MasterDatas& InMasterDatas);
	void SetGuildMercenaryData(TArray<b2network::B2GuildMercenaryPtr> InGuildMercenary);
	void SetGuildMercenaryData(b2network::B2GuildMercenaryPtr InGuildMercenary);
	void SetCurrentGuildMemberInfo(b2network::B2GuildMemberPtr InCurrentGuildMemberInfo) { CurrentGuildMemberInfo = InCurrentGuildMemberInfo; }

	bool GetGuildSupportItems(const EPCClass InPCClass, TArray<FB2Item>& OutResults);
	bool GetGuildSupportItem(int32 TempleteItemID, FB2Item& OutResult);
	FORCEINLINE int32 GetDonatioCount() { return CurrentGuildMemberInfo ? CurrentGuildMemberInfo->today_donation_count : INDEX_NONE; }
	b2network::B2GuildMemberPtr GetCurrentGuildMemberInfo() { return CurrentGuildMemberInfo; }

	FB2GuildMercenaryInfo& GetGuildMercenaryMasterData() { return GuildMercenaryMasterData; }
	TMap<int32, b2network::B2GuildMercenaryPtr>& GetGuildAllMercenaryData() { return GuildMercenaryData; }
	b2network::B2GuildMercenaryPtr GetGuildMercenaryData(int32 NPCId);
	
private:
	void InitGuildSupportData(const FB2MasterDatas& InMasterDatas);
	void InitGuildMasterMercenaryData(const FB2MasterDatas& InMasterDatas);

	void LazyInitialize();
private:
	bool bInitGuildSupportItem;



	b2network::B2GuildMemberPtr						CurrentGuildMemberInfo;
	TArray<FB2GuildSupportItem>						CachedMDSupportItems;
	TMap<EPCClass, TArray<FB2Item>>					CachedSupportItemDatas;
	FB2GuildMercenaryInfo							GuildMercenaryMasterData;
	TMap<int32, b2network::B2GuildMercenaryPtr>		GuildMercenaryData;
};

