// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDocHeroTower.h"


#include "BladeIIGameImpl.h"


UB2UIDocHeroTower::UB2UIDocHeroTower(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_bIsClearHeroTower = false;
	m_nCurrentFloorRecordTimeMilliSecForResult = 0;
}

UB2UIDocHeroTower::~UB2UIDocHeroTower()
{
}

void UB2UIDocHeroTower::Init()
{
	Super::Init();
}

void UB2UIDocHeroTower::Destroy()
{
	Super::Destroy();
}

int32 UB2UIDocHeroTower::GetDailyTryCount()
{
	if (m_ptrGetHeroTower)
	{
		return m_ptrGetHeroTower->daily_try_count;
	}

	return 0;
}

int32 UB2UIDocHeroTower::GetStartFloor()
{
	if (m_ptrGetHeroTower)
	{
		return m_ptrGetHeroTower->start_floor;
	}

	return 0;
}

int32 UB2UIDocHeroTower::GetMaxClearFloor()
{
	if (m_ptrGetHeroTower)
	{
		return m_ptrGetHeroTower->max_clear_floor;
	}

	return 0;
}

int32 UB2UIDocHeroTower::GetPossibleTryCount()
{
	if (m_ptrGetHeroTower)
	{
		return m_ptrGetHeroTower->possible_try_count;
	}
	return 0;
}

int32 UB2UIDocHeroTower::GetNextFloorFromFinishInfo()
{
	if (m_ptrFinishHeroTower)
	{
		return m_ptrFinishHeroTower->next_floor;
	}

	return 0;
}

FString UB2UIDocHeroTower::GetPlayTokenFromFinishInfo()
{
	if (m_ptrFinishHeroTower)
	{
		return m_ptrFinishHeroTower->play_token;
	}

	return TEXT("");
}

void UB2UIDocHeroTower::SetGetHeroTowerPtr(FB2ResponseGetHeroTowerPtr ResponsePtr)
{
	m_ptrGetHeroTower = ResponsePtr;

	SetDailyLeftFreeTryCount(ResponsePtr->possible_try_count);
}

void UB2UIDocHeroTower::SetFinishHeroTowerPtr(FB2ResponseFinishHeroTowerPtr ResponsePtr)
{
	m_ptrFinishHeroTower = ResponsePtr;
}

bool UB2UIDocHeroTower::CanSweepToday()
{
	if (m_ptrGetHeroTower)
	{
		return m_ptrGetHeroTower->is_sweep;
	}

	// 沥焊 绝栏搁 家帕 沁促魔
	return true;	
}

void UB2UIDocHeroTower::ForcedSetSweepToday(bool bAlreadySweep)
{
	if (m_ptrGetHeroTower)
	{
		m_ptrGetHeroTower->is_sweep = bAlreadySweep;
	}
}

void UB2UIDocHeroTower::SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank)
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
			RankerCliInfo.EntryLevels.Add(EntryInfo.level);
		}

		RankingList.Add(RankerCliInfo);
	}
}