#include "B2UIDocDimension.h"
#include "CommonStruct.h"

#include "BladeIIGameImpl.h"

UB2UIDocDimension::UB2UIDocDimension(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Difficulty = 1;
	ClientStageID = 1;
	m_ptrGetDimension = nullptr;
	m_ptrFinishDimension = nullptr;
	m_nCurrentRecordTimeMilliSecForResult = 0;
	RankingList.Empty();
	FirstEnterDimension = false;
}

UB2UIDocDimension::~UB2UIDocDimension()
{

}

void UB2UIDocDimension::Init()
{
	Super::Init();
}

void UB2UIDocDimension::Destroy()
{
	Super::Destroy();
}

bool UB2UIDocDimension::GetDimensionOpen()
{
	//FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();
	//int32 ChapterNumber = StageStore.GetActIdByClientStageId(StageStore.GetLastClearClientId(EStageDifficulty::ESD_VeryHard));
	//return (ChapterNumber > 4);
	return true;
}

void UB2UIDocDimension::SetMapID(int32 Diff)
{
	FRandomStream Random;
	Random.GenerateNewSeed();

	MapIndex = Random.RandRange(1, 6);
	Difficulty = Diff;

	GenerateMapLoadID();
	GenerateClientStageID();
}

void UB2UIDocDimension::GenerateMapLoadID()
{
	MapLoadID = 1 + (MapIndex * 10000) + 500000;
}

void UB2UIDocDimension::GenerateClientStageID()
{ 
	ClientStageID = Difficulty + (MapIndex * 10000) + 500000;
}

int32 UB2UIDocDimension::GetWeeklyBestRecordDiff()
{
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->dimension_status->best_difficulty_level;
	}

	return 0;
}

int32 UB2UIDocDimension::GetWeeklyBestRecordTime()
{
	if (m_ptrGetDimension && GetWeeklyBestRecordDiff() > 0)
	{
		return m_ptrGetDimension->dimension_status->best_clear_time_millis;
	}

	return 0;
}

int32 UB2UIDocDimension::GetMaxClearDiff()
{
	// 捞芭 穿利 弥绊 抄捞档 饭骇...
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->dimension_status->top_difficulty_level;
	}

	return 0;
}

int32 UB2UIDocDimension::GetMaxOpenDiff()
{
	return GetMaxClearDiff() > 9 ? GetMaxClearDiff() + 1 : 10;
}

int32 UB2UIDocDimension::GetMinOpenDiff()
{
	return 1;
}

int32 UB2UIDocDimension::GetLastClearDiff()
{
	int32 Diff = Difficulty;

	if (!FirstEnterDimension)
	{
		Diff = GetMaxOpenDiff();

		FirstEnterDimension = true;
	}

	return Diff;
}

int32 UB2UIDocDimension::GetClearCount()
{
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->dimension_status->daily_clear_count;
	}

	return 0;
}

void UB2UIDocDimension::SetClearCount(int32 count)
{
	if (m_ptrGetDimension)
	{
		m_ptrGetDimension->dimension_status->daily_clear_count = count;
		DimensionTicket = FMath::Max(GetDailyTryCount() - count, 0);
	}

}

int32 UB2UIDocDimension::GetDailyTryCount()
{
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->dimension_status->daily_clear_count_max;
	}

	return 1;
}

int64 UB2UIDocDimension::GetSettlement_Time()
{
	// 沥魂 矫埃
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->settlement_time;
	}

	return 0;
}

int32 UB2UIDocDimension::GetSettlement_PeriodHour()
{
	// 沥魂 林扁 : 割矫埃付促 沥魂登蠢衬...?
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->settlement_period_hour;
	}

	return 0;
}

FModRankerInfo UB2UIDocDimension::GetMyRank()
{
	return MyRank;
}

void UB2UIDocDimension::SetDimensionPtr(FB2ResponseGetDimensionPtr ResponsePtr)
{
	m_ptrGetDimension = ResponsePtr;
	SetRankerList(ResponsePtr->rankers, ResponsePtr->from_ranking);
	SetMyRank();

	DimensionTicket = FMath::Max(GetDailyTryCount() - GetClearCount(), 0);
}

void UB2UIDocDimension::SetMyRank()
{
	MyRank.Entry.Empty();

	MyRank.AccountID = m_ptrGetDimension->ranking->account_id;
	MyRank.Rank = m_ptrGetDimension->ranking->ranking;
	MyRank.RankingPercentage = m_ptrGetDimension->ranking->total_ranker_count > 0 ? 100.f * m_ptrGetDimension->ranking->ranking / (float)m_ptrGetDimension->ranking->total_ranker_count : 100;
	
	
	// 郴 林埃 珐欧 沥焊啊 绝栏搁 diff 啊 0 老巴烙.. 捞锭绰 了促 叼弃飘肺 技泼
	MyRank.TotalCombatPower = GetWeeklyBestRecordDiff() > 0 ? m_ptrGetDimension->dimension_status->best_total_power : 0;
	MyRank.ClearTimeSec = GetWeeklyBestRecordTime();
	MyRank.BestFloor = GetWeeklyBestRecordDiff();	
	MyRank.Entry.Add(SvrToCliPCClassType(GetWeeklyBestRecordDiff() > 0 ? m_ptrGetDimension->dimension_status->best_character_type : 0));

	MyRank.UserName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();
}

void UB2UIDocDimension::SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank)
{
	RankingList.Empty();

	for (auto RankerSvrInfo : RankerList)
	{
		FModRankerInfo RankerCliInfo;
		RankerCliInfo.UserName = RankerSvrInfo->nickname;
		RankerCliInfo.Grade = RankerSvrInfo->battle_grade;
		RankerCliInfo.GradeStar = RankerSvrInfo->battle_star_count;
		RankerCliInfo.Rank = BeginRank++;
		RankerCliInfo.TotalCombatPower = RankerSvrInfo->total_power;			// jwyim power
		RankerCliInfo.ClearTimeSec = RankerSvrInfo->clear_time;
		RankerCliInfo.BestFloor = RankerSvrInfo->best_floor;
		RankerCliInfo.AccountID = RankerSvrInfo->account_id;

		for (auto EntryInfo : RankerSvrInfo->entries)
		{
			RankerCliInfo.Entry.Add(SvrToCliPCClassType(EntryInfo.character_type));
		}

		RankingList.Add(RankerCliInfo);
	}
}

b2network::B2DimensionRankingRewardPtr UB2UIDocDimension::GetWeeklyReward()
{
	if (m_ptrGetDimension)
	{
		return m_ptrGetDimension->ranking_reward;
	}

	return nullptr;
}

void UB2UIDocDimension::ClearWeeklyReward()
{
	if (m_ptrGetDimension)
	{
		m_ptrGetDimension->ranking_reward = nullptr;
	}
}

void UB2UIDocDimension::SetFinishDimensionPtr(FB2ResponseFinishDimensionPtr ResponsePtr)
{
	m_ptrFinishDimension = ResponsePtr;

	if (m_ptrFinishDimension && m_ptrFinishDimension->dimension_status)
		m_ptrFinishDimension->dimension_status = m_ptrFinishDimension->dimension_status; // 哎酒哺淋
}

bool UB2UIDocDimension::GetIsNewRecord()
{
	if (m_ptrFinishDimension)
		return m_ptrFinishDimension->is_new_record;

	return false;
}

TArray<FDimensionReward> UB2UIDocDimension::GetClearRewards()
{
	TArray<FDimensionReward> NewRewardArray;

	if (m_ptrFinishDimension)
	{
		for (auto item : m_ptrFinishDimension->fixed_items)
		{
			FDimensionReward reward;

			reward.Count = item->count;
			reward.ItemIndex = FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE;

			NewRewardArray.Add(reward);
		}

		for (auto item : m_ptrFinishDimension->totems)
		{
			FDimensionReward reward;
			reward.Count = 0;
			reward.ItemIndex = item->template_id;
			
			NewRewardArray.Add(reward);
		}
	}

	return NewRewardArray;
}