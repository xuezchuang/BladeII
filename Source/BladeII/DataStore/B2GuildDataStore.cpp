#include "B2GuildDataStore.h"
//#include "BladeII.h"
#include "B2StageDataStore.h"
#include "B2ClientDataStore.h"

#include "B2StageManager.h"
#include "Event.h"
//#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"

FB2GuildSupportItem::FB2GuildSupportItem(b2network::B2mdGuildSupportItemPtr Info)
	: ItemTempleteID(Info->item_template_id)
	, SupportMaxCount(Info->support_max_count)
{
}

FGuildDataStore::FGuildDataStore()
{
	bInitGuildSupportItem = false;
}

void FGuildDataStore::SetMasterData(const FB2MasterDatas& InMasterDatas)
{
	InitGuildSupportData(InMasterDatas);
	InitGuildMasterMercenaryData(InMasterDatas);
}

void FGuildDataStore::SetGuildMercenaryData(TArray<b2network::B2GuildMercenaryPtr> InGuildMercenary)
{
	GuildMercenaryData.Empty();

	for (auto El : InGuildMercenary)
	{
		GuildMercenaryData.Add(El->id, El);
	}
}

void FGuildDataStore::SetGuildMercenaryData(b2network::B2GuildMercenaryPtr InGuildMercenary)
{
	GuildMercenaryData.Add(InGuildMercenary->id, InGuildMercenary);
}

void FGuildDataStore::InitGuildSupportData(const FB2MasterDatas& InMasterDatas)
{
	//CachedSupportItemDatas.Empty();

	//int32 MaxCount = PCClassToInt(EPCClass::EPC_End);
	//for (int32 i = 0; i < MaxCount; ++i)
	//{
	//	CachedSupportItemDatas.Add(IntToPCClass(i), TArray<FB2Item>());
	//}

	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//const TArray<b2network::B2mdGuildSupportItemPtr> &AllmdSupportItemInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_support_items_index, InMasterDatas);
	//for (const b2network::B2mdGuildSupportItemPtr& ThisInfo : AllmdSupportItemInfo)
	//{
	//	CachedMDSupportItems.Add(FB2GuildSupportItem(ThisInfo));
	//}
}

void FGuildDataStore::InitGuildMasterMercenaryData(const FB2MasterDatas& InMasterDatas)
{
	/*GuildMercenaryMasterData.ClearGuildMercenary();

	const TArray<b2network::B2mdGuildMercenaryPtr> &GuildMercenaryInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_mercenaries_index, InMasterDatas);
	for (const b2network::B2mdGuildMercenaryPtr& ThisInfo : GuildMercenaryInfo)
	{
		GuildMercenaryMasterData.AddMercenaries(ThisInfo);
	}

	const TArray<b2network::B2mdGuildMercenarySkillPtr> &GuildMercenarySkillInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_mercenary_skills_index, InMasterDatas);
	for (const b2network::B2mdGuildMercenarySkillPtr& ThisInfo : GuildMercenarySkillInfo)
	{
		GuildMercenaryMasterData.AddMercenarySkill(ThisInfo);
	}

	const TArray<b2network::B2mdGuildMercenaryMasteryPtr> &GuildMercenaryMasteriesInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_mercenary_masteries_index, InMasterDatas);
	for (const b2network::B2mdGuildMercenaryMasteryPtr& ThisInfo : GuildMercenaryMasteriesInfo)
	{
		GuildMercenaryMasterData.AddMercenaryMasteries(ThisInfo);
	}

	const TArray<b2network::B2mdGuildMercenaryDonationPtr> &GuildMercenaryDonationsInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_mercenary_donations_index, InMasterDatas);
	for (const b2network::B2mdGuildMercenaryDonationPtr& ThisInfo : GuildMercenaryDonationsInfo)
	{
		GuildMercenaryMasterData.AddMercenaryDonations(ThisInfo);
	}*/

}

void FGuildDataStore::LazyInitialize()
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	if (bInitGuildSupportItem == false)
	{
		bInitGuildSupportItem = true;
		for (auto &Each : CachedMDSupportItems)
		{
			const FMD_ItemSingleElem* ItemMDElem = CliDataStore.GetItemMasterDetailInfo(Each.ItemTempleteID);
			if (ItemMDElem == nullptr)
				continue;

			FB2Item NewItem = FB2Item(ItemMDElem);

			CachedSupportItemDatas[ItemMDElem->ItemEquipClass].Add(NewItem);
		}
	}
}

bool FGuildDataStore::GetGuildSupportItem(int32 TempleteItemID, FB2Item& OutResult)
{
	LazyInitialize();

	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();

	const FMD_ItemSingleElem* ItemMDElem = CliDataStore.GetItemMasterDetailInfo(TempleteItemID);
	if (ItemMDElem)
	{
		OutResult = FB2Item(ItemMDElem);
	}

	return false;
}

b2network::B2GuildMercenaryPtr FGuildDataStore::GetGuildMercenaryData(int32 NPCId)
{
	if (GuildMercenaryData.Contains(NPCId))
		return GuildMercenaryData[NPCId];

	return nullptr;
}

bool FGuildDataStore::GetGuildSupportItems(const EPCClass InPCClass, TArray<FB2Item>& OutResults)
{
	LazyInitialize();

	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	
	TArray<FB2Item> *FindRow = CachedSupportItemDatas.Find(InPCClass);
	check(FindRow)

	for (const auto &Each : CachedSupportItemDatas[InPCClass])
	{
		OutResults.Add(Each);
	}

	struct FInvItemStarGradeSorter
	{
		bool operator()(const FB2Item& A, const FB2Item& B) const
		{
			return A.StarGrade < B.StarGrade;
		}
	};
	OutResults.StableSort(FInvItemStarGradeSorter());

	struct FConsumablesTypeSorter_RefID
	{
		bool operator()(const FB2Item& A, const FB2Item& B) const
		{
			return (A.ItemRefID < B.ItemRefID);
		}
	};
	OutResults.StableSort(FConsumablesTypeSorter_RefID());  // 이어지는 아래 소팅에서도 이전 소팅을 보존하기 위해 StableSort 로.

	return OutResults.Num() > 0;
}