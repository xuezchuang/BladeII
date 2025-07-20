// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDocControl.h"
//#include "B2UIManager.h"
//#include "B2UIControlBattleSub.h"
//#include "B2UIControlFindMatch.h"


#include "BladeIIGameImpl.h"
//#include "B2GameInstance.h"

UB2UIDocControl::UB2UIDocControl(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetCanViewMvpRewardBoxConfirm(true);
	SetCanViewModRewardBoxConfirm(true);
	IsSubmersion = false;
}

UB2UIDocControl::~UB2UIDocControl()
{

}

void UB2UIDocControl::Init()
{
	Super::Init();

	ResetSerialKillCount();
}

void UB2UIDocControl::AddSerialKillCount()
{
	nSerialKillCount++;

	//// UI俊 钎扁(1懦掳何磐 钎矫牢啊. 2懦掳何磐 钎矫牢啊.)
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//	pUI->SetSerialKillCount(nSerialKillCount);
}

void UB2UIDocControl::ResetSerialKillCount()
{
	nSerialKillCount = 0;	
}

void UB2UIDocControl::ResetMatchRecordInfos()
{
	m_nMyTotalDamage = 0;
	m_fMyContributeTime = 0.0f;

	ControlUserUIInfos.Empty();
}

void UB2UIDocControl::AddKillCount(uint32 NetId)
{
	if (!ControlUserUIInfos.Contains(NetId))
	{
		ControlUserUIInfos.Add(NetId, FControlUserUIInfo());
	}

	ControlUserUIInfos[NetId].KillCount++;

	//RefreshKillInfoUI();
}

void UB2UIDocControl::SetKillCount(uint32 NetId, int32 KillCount)
{
	if (!ControlUserUIInfos.Contains(NetId))
	{
		ControlUserUIInfos.Add(NetId, FControlUserUIInfo());
	}

	ControlUserUIInfos[NetId].KillCount = KillCount;
}

int32 UB2UIDocControl::GetKillCount(int32 NetId)
{
	if (ControlUserUIInfos.Contains(NetId))
	{
		return ControlUserUIInfos[NetId].KillCount;
	}

	return 0;
}

void UB2UIDocControl::RefreshKillInfoUI()
{
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
	//
	//if (pUI)
	//	pUI->SetUserKillInfos(GetUserKillInfos());

}

void UB2UIDocControl::SetAllyPlayerCount(int32 nPlayerCount)
{
	////UI乐栏搁 傈崔.
	//UB2UIControlFindMatch* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlFindMatch>(UIFName::ControlFindMatch);

	//if (pUI)
	//	pUI->SetAllyPlayerCount(nPlayerCount);
}

void UB2UIDocControl::SetFindingEnd()
{
	////UI乐栏搁 傈崔.
	//UB2UIControlFindMatch* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlFindMatch>(UIFName::ControlFindMatch);

	//if (pUI)
	//	pUI->SetFindingEnd();
}

TMap<uint32, int32> UB2UIDocControl::GetUserKillInfos()
{
	TMap<uint32, int32> UserKillInfos;

	for (TPair<uint32, FControlUserUIInfo>& FControlUserUIInfoItem : ControlUserUIInfos)
	{
		UserKillInfos.Add(FControlUserUIInfoItem.Key, FControlUserUIInfoItem.Value.KillCount);
	}

	return UserKillInfos;
}

int32 UB2UIDocControl::GetTeamKillCount(ETeamType TeamType)
{
	int32 nKillCount = 0;

	for (TPair<uint32, FControlUserUIInfo>& FControlUserUIInfoItem : ControlUserUIInfos)
	{
		if (FControlUserUIInfoItem.Value.TeamType == TeamType)
			nKillCount += FControlUserUIInfoItem.Value.KillCount;
	}

	return nKillCount;
}

void UB2UIDocControl::SetGetAssaultPtr(FB2ResponseGetAssaultBattleStatusPtr ResponsePtr)
{
	m_ptrGetAssault = ResponsePtr;
	ControlRankingReward = ResponsePtr->ranking_reward;

	SetRankerList(ResponsePtr->rankers, ResponsePtr->from_ranking);
	SetRewardBoxStatus(ResponsePtr->reward_box_status);
	SetMVPPoint(ResponsePtr->mvp_point);
	SetBeforeGradeInfo(GetGradeInfo());
	SetGradeInfo(ResponsePtr->match_status);
}


// 百百绢戒飘(葛靛牢器)
FB2ResponseGetAssaultBattleStatusPtr UB2UIDocControl::GetGetAssaultPtr()
{
	return m_ptrGetAssault;
}

void UB2UIDocControl::SetResultPtr(FB2ResponseAssaultResultPtr ResponsePtr)
{
	m_ptrResult = ResponsePtr;

	SetBeforeGradeInfo(GetGradeInfo());
	SetGradeInfo(ResponsePtr->match_status);
	
	// 葛靛沥焊尔 搬苞沥焊俊 府况靛冠胶 沥焊 乐绰瘤 犬牢
	if (m_ptrGetAssault && m_ptrGetAssault->reward_box_status
		&& m_ptrResult && m_ptrResult->reward_box_status)
	{
		// 葛靛沥焊绰 焊惑いい绊 搬苞沥焊绰 焊惑しせ搁 傈捧 矫累矫 版绊 扑诀焊扁 府悸
		if (!m_ptrGetAssault->reward_box_status->is_reward_state
			&& m_ptrResult->reward_box_status->is_reward_state)
			SetCanViewModRewardBoxConfirm(true);
	}

	if (m_ptrGetAssault	&& m_ptrResult)
	{
		// 葛靛牢器 器牢飘焊促 搬苞 器牢飘啊 臭绊. 弊霸 焊惑罐阑荐 乐绰 痢荐搁 傈捧 矫累矫 版绊 扑诀焊扁 府悸
		// 搬苞俊 醚 Mvp器牢飘沥焊啊 绝澜.
		//if (m_ptrResult->mvp_point > m_ptrGetAssault->mvp_point&& m_ptrResult->mvp_point >= BladeIIGameImpl::GetClientDataStore().GetAssaultMvpPointToOpenReward())
		//	SetCanViewMvpRewardBoxConfirm(true);
	}

	//for (auto RewardItem : ResponsePtr->victory_rewards)
	//{
	//	if (RewardItem->light_characters.Num() > 0)
	//	{
	//		for (auto ExpInfo : RewardItem->light_characters)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().EndGameIncreaseExp(ExpInfo);
	//		}
	//	}
	//}
}

FB2ResponseAssaultResultPtr UB2UIDocControl::GetResultPtr()
{
	return m_ptrResult;
}

int32 UB2UIDocControl::GetBattleGrade()
{
	if(GetGradeInfo())
		return GetGradeInfo()->battle_grade;
	
	return 10;
}

int32 UB2UIDocControl::GetBattleStarCount()
{
	if (GetGradeInfo())
		return GetGradeInfo()->battle_star_count;
	
	return 0;
}

bool UB2UIDocControl::IsAlwaysOpenControlMatch()
{
	//if (AssaultOpenPeriodInfo.Num() <= 0)
	//	return false;

	//for (auto TimeInfo : AssaultOpenPeriodInfo)
	//{
	//	//Local Time俊 嘎苗 拌魂窍绊 乐澜.
	//	/*
	//	FDateTime DateNow = UB2GameInstance::GetUtcToLocalTime(UB2GameInstance::GetUtcNow());
	//	int32 nUTCDayofweek = static_cast<int32>(DateNow.GetDayOfWeek());
	//	int32 nCurrentDayofweek = (DateNow.GetHour() >= 24) ? nUTCDayofweek + 1 : nUTCDayofweek; // 抗寇惑炔捞 惯积 且 版快甫 措厚茄 内靛
	//	nCurrentDayofweek = (nCurrentDayofweek + 1) % 7 + 1; // 辑滚绰 老夸老捞 1肺 矫累. UTCTime狼 EDayOfWeek绰 岿夸老捞 1
	//	/*/
	//	int32 nCurrentDayofweek = UB2GameInstance::ConvertToSvrDayOfWeek();
	//	//*/

	//	if (TimeInfo->day_of_week != nCurrentDayofweek) continue; // 鞍篮 夸老父 拌魂秦具窃

	//	int32 TimeOpen = (TimeInfo->begin_hour * 60) + TimeInfo->begin_min;
	//	int32 TimeClose = (TimeInfo->end_hour * 60) + TimeInfo->end_min;

	//	// match_open_period_infos俊 朝楼沥焊啊 绝绢辑 0矫0盒 ~ 24矫0盒 捞寇狼 蔼捞 汲沥瞪老篮 绝瘤父. 悼老矫埃档 眉农
	//	if (TimeOpen % (60 * 24) == TimeClose % (60 * 24))
	//		return true;
	//}

	return false;
}

int32 UB2UIDocControl::GetControlMatchRemainTime()
{
	// 窜困 盒

	if (AssaultOpenPeriodInfo.Num() == 0)
		return 0;

	int32 MinRemainTime = 24 * 60;

	for (auto TimeInfo : AssaultOpenPeriodInfo)
	{
		// 泅犁矫埃焊促 奴芭吝俊 俩啊鳖款芭(瞒捞啊 利篮芭) 茫扁

		int32 TimeOpen = (TimeInfo->begin_hour * 60) + TimeInfo->begin_min;
		int32 TimeClose = (TimeInfo->end_hour * 60) + TimeInfo->end_min;

		int32 nTestHour = FDateTime::UtcNow().GetHour();
		int32 nTestMin = FDateTime::UtcNow().GetMinute();
		
		// 烙矫肺 矫埃 嘎苗夯促. 茄惫矫埃栏肺
		// 辑滚俊辑 罐绰霸 UTC啊 登搁 捞吧 力芭窍搁凳
		nTestHour = (nTestHour + 9) % 24;
		int32 TimeNow = (/*FDateTime::UtcNow().GetHour()*/nTestHour * 60) + FDateTime::UtcNow().GetMinute();

		// 坷锹矫埃
		int32 RemainTimeOpen = TimeOpen - TimeNow;
		if (RemainTimeOpen < 0)
			RemainTimeOpen += 24 * 60;

		MinRemainTime = FMath::Min(MinRemainTime, RemainTimeOpen);

		// 辆丰矫埃
		int32 RemainTimeClose = TimeClose - TimeNow;
		if (RemainTimeClose < 0)
			RemainTimeClose += 24 * 60;

		MinRemainTime = FMath::Min(MinRemainTime, RemainTimeClose);
	}

	//俩累篮芭 府畔
	return MinRemainTime;
}

float UB2UIDocControl::GetMyContributeTime()
{
	return m_fMyContributeTime;
}

void UB2UIDocControl::AddMyContributeTime(float fAddTimeSec)
{
	m_fMyContributeTime += fAddTimeSec;
}

int32 UB2UIDocControl::GetDeadCount(uint32 NetId)
{
	if (ControlUserUIInfos.Contains(NetId))
	{
		return ControlUserUIInfos[NetId].DeadCount;
	}

	return 0;
}

void UB2UIDocControl::AddDeadCount(uint32 NetId, int32 nDeadCount /*= 1*/)
{
	if (!ControlUserUIInfos.Contains(NetId))
	{
		ControlUserUIInfos.Add(NetId, FControlUserUIInfo());
	}

	ControlUserUIInfos[NetId].DeadCount++;
}

int32 UB2UIDocControl::GetMyTotalDamage()
{
	return m_nMyTotalDamage;
}

void UB2UIDocControl::AddMyTotalDamage(int32 nDamageAmount)
{
	m_nMyTotalDamage += nDamageAmount;
}

int32 UB2UIDocControl::GetMyRanking()
{
	if (m_ptrGetAssault && m_ptrGetAssault->ranking)
		return m_ptrGetAssault->ranking->ranking;

	return 0;
}

float UB2UIDocControl::GetMyRankingPercent()
{
	if (m_ptrGetAssault && m_ptrGetAssault->ranking && m_ptrGetAssault->ranking->total_ranker_count != 0)
		return (m_ptrGetAssault->ranking->ranking / float(m_ptrGetAssault->ranking->total_ranker_count)) * 100.0f;

	return 100.0f;
}

int64 UB2UIDocControl::GetSettlementTime()
{
	if (m_ptrGetAssault)
		return m_ptrGetAssault->settlement_time;

	return 0;
}

int32 UB2UIDocControl::GetSettlementPeriodHour()
{
	if (m_ptrGetAssault)
		return m_ptrGetAssault->settlement_period_hour;

	return 0;
}

TMap<int64, int64> UB2UIDocControl::GetUserReportUser()
{
	return UserReportUser;
}

void UB2UIDocControl::SetUserReportUser(TMap<int64, int64> InUserReportUser)
{
	UserReportUser = InUserReportUser;
}

void UB2UIDocControl::SetPeriodInfos(TArray<b2network::B2MatchOpenPeriodInfoPtr>& PeriodInfo)
{
	AssaultOpenPeriodInfo = PeriodInfo;
}

void UB2UIDocControl::SetSubmersion(bool InValue)
{
	IsSubmersion = InValue;
}

bool UB2UIDocControl::GetSubmersion()
{
	return IsSubmersion;
}

void UB2UIDocControl::SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank)
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

int32 UB2UIDocControl::GetControlType()
{
	//int32 nCurrentDayofweek = UB2GameInstance::ConvertToSvrDayOfWeek();

	//for (auto TimeInfo : AssaultOpenPeriodInfo)
	//{
	//	if (TimeInfo->day_of_week != nCurrentDayofweek) continue;

	//	return TimeInfo->assault_type;
	//}
	return 0;
}