// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "B2UIDocBase.h"
#include "B2GuildGameMode.h"
#include "B2UIDocGuild.generated.h"

USTRUCT()
struct FGuildTeamModel
{
	GENERATED_BODY()

	ETeamIndex TeamIndex;
	uint8 TeamNum;

	UPROPERTY(Transient)
	TArray<class ABladeIICharacter*> Characters;
};

struct FGuildBattleState
{
	FGuildBattleState()
	{
		season_id = turn = current_state = begin_time = end_time = 0;
	}
	FGuildBattleState(b2network::B2GuildBattleStatePtr InGuildBattleState)
	{
		season_id = InGuildBattleState->season_id;
		turn = InGuildBattleState->turn;
		current_state = InGuildBattleState->current_state;
		begin_time = InGuildBattleState->begin_time;
		end_time = InGuildBattleState->end_time;
	}
	int32 season_id;
	int32 turn;
	int32 current_state;
	int32 begin_time;
	int32 end_time;
};
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeGuildTeam, ETeamIndex, uint8)

UCLASS()
class BLADEII_API UB2UIDocGuild : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocGuild(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocGuild();

	virtual void Init() override;
	virtual void Destroy() override;

	DECLARE_UIDOC_FIELD(int32, GuildMainType)

	DECLARE_UIDOC_FIELD(bool, GuildOutState)
	DECLARE_UIDOC_FIELD(int64, LeaveGuildTime)

	DECLARE_UIDOC_FIELD(int64, GuildBattleEndTime)
	//DECLARE_UIDOC_FIELD(b2network::B2GuildBattleStatePtr, GuildbattleState)
	DECLARE_UIDOC_FIELD(FGuildBattleState, GuildbattleStateNew)
	DECLARE_UIDOC_FIELD(FString, BattleToken)
	DECLARE_UIDOC_FIELD(int32, BattleResultMedal)
	DECLARE_UIDOC_FIELD(bool,	GuildAcceptState)

	DECLARE_UIDOC_FIELD(int32, GuildLv)
	DECLARE_UIDOC_FIELD(int32, GuildExp)
	DECLARE_UIDOC_FIELD(int32, ObtainMedal)
	DECLARE_UIDOC_FIELD(bool, TurnReward)
	DECLARE_UIDOC_FIELD(bool, JoinGuildBattle)
	DECLARE_UIDOC_FIELD(bool, JoinGuildBattlememeber)

	DECLARE_UIDOC_FIELD(TArray<b2network::B2CharacterPowerPtr>, CharacterPower);

	DECLARE_UIDOC_FIELD(float, AttackGuildBuff)
	DECLARE_UIDOC_FIELD(float, DefenseGuildBuff)
	DECLARE_UIDOC_FIELD(float, HealthGuildBuff)

	DECLARE_UIDOC_FIELD(int64, KickedGuildId)

	DECLARE_UIDOC_FIELD(TArray<b2network::B2RewardPtr>, BattleResultReward)

	DECLARE_UIDOC_FIELD(TArray<b2network::B2GuildBattleCharacterSlotPtr> ,GuildBattleEntry);

	//내정보 캐릭터 장비등
	DECLARE_UIDOC_FIELD(b2network::B2FindAccountInfoPtr, AllyGuildMemeberCharInfo)
	//내 정보 엔트리
	DECLARE_UIDOC_FIELD(b2network::B2GuildBattleEntryPtr, AllyGuildMemeberEntry)

	//상대정보 캐릭터 장비등
	DECLARE_UIDOC_FIELD(b2network::B2FindAccountInfoPtr, OtherGuildMemeberCharInfo)
	//상대 정보 엔트리
	DECLARE_UIDOC_FIELD(b2network::B2GuildBattleEntryPtr, OtherGuildMemeberEntry)
	
	//Opponent
	DECLARE_UIDOC_FIELD(int64, OtherGuildMemeberAccountId);
	DECLARE_UIDOC_FIELD(FString, OtherGuildMemeberNickName);


	DECLARE_UIDOC_FIELD(int32, GuildChatBadge)
	DECLARE_UIDOC_FIELD(int64, NextAskSupportTime)

	//인게임 전투 모음
	DECLARE_UIDOC_FIELD(int32, BattleTime)
	
	DECLARE_UIDOC_FIELD(int32, CurrentRound)
	DECLARE_UIDOC_FIELD(ETeamIndex, CurrentRoundWinner)

	DECLARE_UIDOC_FIELD(uint8, AllyDisplayTeamNum)
	DECLARE_UIDOC_FIELD(uint8, OtherDisplayTeamNum)

	//Camera
	DECLARE_UIDOC_FIELD(bool, FixedCamera)
	DECLARE_UIDOC_FIELD(bool, ShortCamera)
	DECLARE_UIDOC_FIELD(ABladeIIPlayer*, ViewTarget)

public:
	FOnChangeGuildTeam OnChangeGuildTeam;

private:
	TArray<FGuildTeamModel> GuildTeams;
		
public:
	void AddGuildTeam(ETeamIndex InTeamIndex, uint8 InTeamNum);
	void AddGuildTeamMember(ETeamIndex InTeamIndex, uint8 InTeamNum, TArray<class ABladeIICharacter*> InPlayer);
	FGuildTeamModel* FindGuildTeamViewModel(ETeamIndex InTeamIndex, uint8 InTeamNum);
	void ClearGuildBattle();


	void SetGuildBuff(const TArray<b2network::B2GuildSkillPtr>& GuildSkillArr);
	bool IsOverAskSupportTime();

};