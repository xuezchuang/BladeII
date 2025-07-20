// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIICharacter.h"
#include "B2UIDocBase.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIDocControl.generated.h"

/**
 * 
 */

struct FControlUserUIInfo
{
	FControlUserUIInfo()
	{
		TeamType = ETeamType::ETT_End;
		KillCount = 0;
		DeadCount = 0;
	}

	ETeamType TeamType;
	int32 KillCount;
	int32 DeadCount;
};

UCLASS()
class BLADEII_API UB2UIDocControl : public UB2UIDocBase
{
	GENERATED_BODY()
	
public:
	UB2UIDocControl(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocControl();
	
	virtual void Init() override;

public:
	void AddSerialKillCount();
	void ResetSerialKillCount();
	void ResetMatchRecordInfos();

	void AddKillCount(uint32 NetId);
	void SetKillCount(uint32 NetId, int32 KillCount);
	int32 GetKillCount(int32 NetId);

	void RefreshKillInfoUI();

	void SetAllyPlayerCount(int32 nPlayerCount);

	void SetFindingEnd();

	TMap<uint32, int32> GetUserKillInfos();

	int32 GetTeamKillCount(ETeamType TeamType);

	void SetGetAssaultPtr(FB2ResponseGetAssaultBattleStatusPtr ResponsePtr);
	FB2ResponseGetAssaultBattleStatusPtr GetGetAssaultPtr();

	void SetResultPtr(FB2ResponseAssaultResultPtr ResponsePtr);
	FB2ResponseAssaultResultPtr GetResultPtr();

	int32 GetBattleGrade();
	int32 GetBattleStarCount();

	bool IsAlwaysOpenControlMatch();
	int32 GetControlMatchRemainTime();

	float GetMyContributeTime();
	void AddMyContributeTime(float fAddTimeSec);

	int32 GetDeadCount(uint32 NetId);
	void AddDeadCount(uint32 NetId, int32 nDeadCount = 1);

	int32 GetMyTotalDamage();
	void AddMyTotalDamage(int32 nDamageAmount);

	int32 GetMyRanking();
	float GetMyRankingPercent();

	int64 GetSettlementTime();
	int32 GetSettlementPeriodHour();

	TMap<int64, int64> GetUserReportUser();
	void SetUserReportUser(TMap<int64, int64> InUserReportUser);

	void SetPeriodInfos(TArray<b2network::B2MatchOpenPeriodInfoPtr>& PeriodInfo);
	TArray<b2network::B2MatchOpenPeriodInfoPtr>& GetPeriodInfos() {	return AssaultOpenPeriodInfo; }

	void SetSubmersion(bool InValue);
	bool GetSubmersion();

	int32 GetControlType();

	// DECLARE_UIDOC_FIELD
	DECLARE_UIDOC_FIELD(b2network::B2DuelRewardBoxStatusPtr, RewardBoxStatus);
	DECLARE_UIDOC_FIELD(int32, MVPPoint);
	DECLARE_UIDOC_FIELD(TArray<b2network::B2mdAssaultMvpRewardInfoPtr>, AssaultMvpRewardInfo);
	DECLARE_UIDOC_FIELD(b2network::B2RankingRewardPtr, ControlRankingReward);
	DECLARE_UIDOC_FIELD(b2network::B2MatchStatusPtr, GradeInfo);
	DECLARE_UIDOC_FIELD(b2network::B2MatchStatusPtr, BeforeGradeInfo);
	
	DECLARE_UIDOC_FIELD(bool, CanViewMvpRewardBoxConfirm);
	DECLARE_UIDOC_FIELD(bool, CanViewModRewardBoxConfirm);
		
private:
	int32 nSerialKillCount;
	TMap<uint32, FControlUserUIInfo> ControlUserUIInfos;

	FB2ResponseGetAssaultBattleStatusPtr m_ptrGetAssault;
	FB2ResponseAssaultResultPtr m_ptrResult;
	TArray<b2network::B2MatchOpenPeriodInfoPtr> AssaultOpenPeriodInfo;

	int32 m_nMyTotalDamage;
	float m_fMyContributeTime;
	TMap<int64, int64> UserReportUser;

	bool IsSubmersion;

public:
	void SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank);
	FORCEINLINE const TArray<FModRankerInfo>* GetRankerList() { return &RankingList; }
private:
	TArray<FModRankerInfo> RankingList;
};
