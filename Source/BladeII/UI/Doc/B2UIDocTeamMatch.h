// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2UIDocTeamMatch.generated.h"


UCLASS()
class BLADEII_API UB2UIDocTeamMatch : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocTeamMatch(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocTeamMatch();

	virtual void Init() override;
	virtual void Destroy() override;

	DECLARE_UIDOC_FIELD(int32, IsAIMe);

	//Local Player
	DECLARE_UIDOC_FIELD(int32, PCGrade);
	DECLARE_UIDOC_FIELD(int32, PCBeforeGrade);
	DECLARE_UIDOC_FIELD(int32, PCGradeStar);
	DECLARE_UIDOC_FIELD(int32, PCBeforeGradeStar);
	DECLARE_UIDOC_FIELD(int32, PCCurrTopContinuousWins);
	DECLARE_UIDOC_FIELD(int32, PCPrevTopContinuousWins);
	DECLARE_UIDOC_FIELD(int32, PCCurrContinuousWins);
	DECLARE_UIDOC_FIELD(int32, PCPrevContinuousWins);
	DECLARE_UIDOC_FIELD(int32, PCRank);
	DECLARE_UIDOC_FIELD(int32, PCRankPercentage);

	DECLARE_UIDOC_FIELD(ETMFormation, TMFormation)
	DECLARE_UIDOC_FIELD(ETMFormation, EnhanceFormation)
	DECLARE_UIDOC_FIELD(int32, FormationNormalLevel)
	DECLARE_UIDOC_FIELD(int32, FormationAttLevel)
	DECLARE_UIDOC_FIELD(int32, FormationDefLevel)

	DECLARE_UIDOC_FIELD(EPCClass, PCCharacterClass1)
	DECLARE_UIDOC_FIELD(EPCClass, PCCharacterClass2)
	DECLARE_UIDOC_FIELD(EPCClass, PCCharacterClass3)

	DECLARE_UIDOC_FIELD(float, PCHealth1);
	DECLARE_UIDOC_FIELD(float, PCHealth2);
	DECLARE_UIDOC_FIELD(float, PCHealth3);
	DECLARE_UIDOC_FIELD(float, PCMaxHealth1);
	DECLARE_UIDOC_FIELD(float, PCMaxHealth2);
	DECLARE_UIDOC_FIELD(float, PCMaxHealth3);
	DECLARE_UIDOC_FIELD(float, PCShield1);
	DECLARE_UIDOC_FIELD(float, PCShield2);
	DECLARE_UIDOC_FIELD(float, PCShield3);

	//Opponent
	DECLARE_UIDOC_FIELD(int32, OpponentGrade);
	DECLARE_UIDOC_FIELD(int32, OpponentGradeStar);
	DECLARE_UIDOC_FIELD(int32, OpponentContinuousWins);

	DECLARE_UIDOC_FIELD(ETMFormation, OpponentTMFormation);
	DECLARE_UIDOC_FIELD(int32, OpponentFormationLevel);

	DECLARE_UIDOC_FIELD(EPCClass, OpponentCharacterClass1);
	DECLARE_UIDOC_FIELD(EPCClass, OpponentCharacterClass2);
	DECLARE_UIDOC_FIELD(EPCClass, OpponentCharacterClass3);

	DECLARE_UIDOC_FIELD(float, OpponentHealth1);
	DECLARE_UIDOC_FIELD(float, OpponentHealth2);
	DECLARE_UIDOC_FIELD(float, OpponentHealth3);
	DECLARE_UIDOC_FIELD(float, OpponentMaxHealth1);
	DECLARE_UIDOC_FIELD(float, OpponentMaxHealth2);
	DECLARE_UIDOC_FIELD(float, OpponentMaxHealth3);
	DECLARE_UIDOC_FIELD(float, OpponentShield1);
	DECLARE_UIDOC_FIELD(float, OpponentShield2);
	DECLARE_UIDOC_FIELD(float, OpponentShield3);

	//Team Ranking Reward
	DECLARE_UIDOC_FIELD(bool, IsTeamReward)
	DECLARE_UIDOC_FIELD(b2network::B2RankingRewardPtr, TeamReward)

	DECLARE_UIDOC_FIELD(int64, TeamRewardTime)
	DECLARE_UIDOC_FIELD(bool, TeamTodayReward)
	DECLARE_UIDOC_FIELD(bool, TeamLastReward)
	DECLARE_UIDOC_FIELD(int32, TeamParticipationCount)
	DECLARE_UIDOC_FIELD(int32, TeamCurrentGold)
	DECLARE_UIDOC_FIELD(int32, TeamCurrentBladepoint)
	DECLARE_UIDOC_FIELD(int32, TeamCurrentGem)

	//RewardBox
	DECLARE_UIDOC_FIELD(b2network::B2DuelRewardBoxStatusPtr, RewardBoxStatus)

	//////////////////////////////////////////////////
	//MatchPoint data
	DECLARE_UIDOC_FIELD(int32, TeamMatchPoint);
	DECLARE_UIDOC_FIELD(int32, TeamAdditionalMatchCount);
	DECLARE_UIDOC_FIELD(int32, NextTimeTeamMatchPointGet);
	DECLARE_UIDOC_FIELD(double, NextChargeSecond);
	DECLARE_UIDOC_FIELD(bool, UsedAdditionalPoint);

	DECLARE_UIDOC_FIELD(bool, CanViewModRewardBoxConfirm)

	DECLARE_UIDOC_FIELD(int32, IncreaseEtherPiece);

public:
	void SetCharacterHP(bool bPCTeam, int32 CharIndex, float HealthToSet, float MaxHealthToSet = 0.f);
	float GetCharacterHP(bool bPCTeam, int32 CharIndex);
	float GetCharacterMaxHP(bool bPCTeam, int32 CharIndex);

	void SetCharacterShield(bool bPCTeam, int32 CharIndex, float ShieldToSet);
	float GetCharacterShield(bool bPCTeam, int32 CharIndex);

	void SetCharacterClass(bool bPCTeam, int32 CharIndex, EPCClass Class);
	EPCClass GetCharacterClass(bool bPCTeam, int32 CharIndex);

	int8 GetCharacterFormationNum(int32 CharIndex);

	int32 GetCurrentPCFormationLevel();
	int32 GetCurrentPCFormationLevel(ETMFormation Formation);

	void UpdateTeamMatchData();
	EFormationNUM GetFirstCharcterForamtionNUM();
	EFormationNUM GetSecondCharcterForamtionNUM();
	EFormationNUM GetThirdCharcterForamtionNUM();
private:
	uint32 ChangeTeamMatchPointChargeTimeTicket;

public:
	FModRankerInfoContainer RankerInfoContainer;
};