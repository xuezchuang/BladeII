#include "BladeII.h"
#include "Event.h"
#include "B2UIDocGuild.h"
#include "B2GuildGameMode.h"
//#include "B2GameInstance.h"

#include "BladeIIGameImpl.h"

UB2UIDocGuild::UB2UIDocGuild(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UB2UIDocGuild::~UB2UIDocGuild()
{
}

void UB2UIDocGuild::Init()
{
	Super::Init();

	GuildOutState = false;

	BattleTime = 90;

	LeaveGuildTime = 0;
	KickedGuildId = 0;
	AllyDisplayTeamNum = 0;
	OtherDisplayTeamNum = 0;
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
}

void UB2UIDocGuild::Destroy()
{
	Super::Destroy();
}

void UB2UIDocGuild::SetGuildBuff(const TArray<b2network::B2GuildSkillPtr>& GuildSkillArr)
{

	AttackGuildBuff = DefenseGuildBuff = HealthGuildBuff = 0;

	for (auto GuildSkill : GuildSkillArr)
	{
		auto MasterData = BladeIIGameImpl::GetClientDataStore().GetGuildSkillInfo(GuildSkill->id, GuildSkill->level);

		if (!MasterData)
			return;

		if (SvrToCliOptionType(MasterData->buff_option_type) == EItemOption::EIO_Offense_IncAttack ||
			SvrToCliOptionType(MasterData->buff_option_type) == EItemOption::EIO_Guild_Offense_IncAttack)
		{
			AttackGuildBuff = MasterData->buff_option_value;
		}
		else if (SvrToCliOptionType(MasterData->buff_option_type) == EItemOption::EIO_Defense_IncDefense ||
			SvrToCliOptionType(MasterData->buff_option_type) == EItemOption::EIO_Guild_Defense_IncDefense)
		{
			DefenseGuildBuff = MasterData->buff_option_value;
		}
		else if (SvrToCliOptionType(MasterData->buff_option_type) == EItemOption::EIO_Health_IncHealth ||
			SvrToCliOptionType(MasterData->buff_option_type) == EItemOption::EIO_Guild_Health_IncHealth)
		{
			HealthGuildBuff = MasterData->buff_option_value;
		}

	}
}

void UB2UIDocGuild::AddGuildTeam(ETeamIndex InTeamIndex, uint8 InTeamNum)
{
	if (!FindGuildTeamViewModel(InTeamIndex, InTeamNum))
	{
		FGuildTeamModel GuildTeam;
		GuildTeam.TeamIndex = InTeamIndex;
		GuildTeam.TeamNum = InTeamNum;
		GuildTeams.Add(GuildTeam);
	}
}

void UB2UIDocGuild::AddGuildTeamMember(ETeamIndex InTeamIndex, uint8 InTeamNum, TArray<class ABladeIICharacter*> InPlayer)
{
	FGuildTeamModel * GuildTeam = FindGuildTeamViewModel(InTeamIndex, InTeamNum);
	if (GuildTeam)
	{
		GuildTeam->Characters.Append(InPlayer);
		OnChangeGuildTeam.Broadcast(InTeamIndex, InTeamNum);
	}
}

FGuildTeamModel * UB2UIDocGuild::FindGuildTeamViewModel(ETeamIndex InTeamIndex, uint8 InTeamNum)
{
	return GuildTeams.FindByPredicate([InTeamIndex, InTeamNum] (FGuildTeamModel& GuildTeam)
	{
		return GuildTeam.TeamIndex == InTeamIndex && GuildTeam.TeamNum == InTeamNum;
	});
}

void UB2UIDocGuild::ClearGuildBattle()
{
	GuildTeams.Empty();
	SetBattleTime(0.0f);
	OtherDisplayTeamNum = 0;
	AllyDisplayTeamNum = 0;
}

bool UB2UIDocGuild::IsOverAskSupportTime()
{
	//FDateTime ExpireTime = FDateTime::FromUnixTimestamp(GetNextAskSupportTime() / 1000);
	//int64 RemainTime = (ExpireTime - UB2GameInstance::GetUTCNow()).GetTotalSeconds();
	//return RemainTime < 0.0f;
	return false;
}