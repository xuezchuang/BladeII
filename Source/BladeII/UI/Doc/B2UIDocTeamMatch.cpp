#include "B2UIDocTeamMatch.h"
#include "Event.h"
//#include "BladeIIPointCharger.h"


//
// 진형정보 진형에 들어갈 캐릭터정보 등등(팀매치에 들어가는 정보)
//
// 
//

UB2UIDocTeamMatch::UB2UIDocTeamMatch(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsTeamReward = false;
	SetCanViewModRewardBoxConfirm(true);
}

UB2UIDocTeamMatch::~UB2UIDocTeamMatch()
{

}

void UB2UIDocTeamMatch::Init()
{
	TMFormation = ETMFormation::ETM_Normal;
	PCCharacterClass1 = EPCClass::EPC_Gladiator;
	PCCharacterClass2 = EPCClass::EPC_Assassin;
	PCCharacterClass3 = EPCClass::EPC_Wizard;
	OpponentCharacterClass1 = OpponentCharacterClass2 = OpponentCharacterClass3 = EPCClass::EPC_End;
	FormationNormalLevel = 1;
	FormationAttLevel = 1;
	FormationDefLevel = 1;

	//CAPTURE_UOBJECT(UB2UIDocTeamMatch);
	//ChangeTeamMatchPointChargeTimeTicket = ChangeTeamMatchPointChargeTimeClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ChangeTeamMatchPointChargeTime)
	//	Capture->UpdateTeamMatchData();
	//END_CAPTURE_OBJECT()
	//	);

	UpdateTeamMatchData();
}

void UB2UIDocTeamMatch::Destroy()
{
	//ChangeTeamMatchPointChargeTimeClass<>::GetInstance().Unsubscribe(ChangeTeamMatchPointChargeTimeTicket);
}

int32 UB2UIDocTeamMatch::GetCurrentPCFormationLevel()
{
	switch (TMFormation)
	{
	case ETMFormation::ETM_Normal:
		return FormationNormalLevel;
	case ETMFormation::ETM_Attack:
		return FormationAttLevel;
	case ETMFormation::ETM_Defense:
		return FormationDefLevel;	
	}

	return 0;
}


int32 UB2UIDocTeamMatch::GetCurrentPCFormationLevel(ETMFormation Formation)
{
	switch (Formation)
	{
	case ETMFormation::ETM_Normal:
		return FormationNormalLevel;
	case ETMFormation::ETM_Attack:
		return FormationAttLevel;
	case ETMFormation::ETM_Defense:
		return FormationDefLevel;
	}

	return 0;
}

void UB2UIDocTeamMatch::UpdateTeamMatchData()
{
	//SetNextTimeTeamMatchPointGet(FBladeIIChargeBladePointCharger::GetTeamMatchPointCharger().GetLeftChargeTime());
}

#define SET_MATCHED_INDEX_CHARACTER_HP(TeamName, Index, HP, MaxHP)	\
	if(CharIndex + 1 == Index)										\
	{																\
		if(MaxHP > 0.f)	Set##TeamName##MaxHealth##Index(MaxHP);		\
		Set##TeamName##Health##Index(HP);							\
	}

void UB2UIDocTeamMatch::SetCharacterHP(bool bPCTeam, int32 CharIndex, float HealthToSet, float MaxHealthToSet)
{
	if (bPCTeam)
	{
		SET_MATCHED_INDEX_CHARACTER_HP(PC, 1, HealthToSet, MaxHealthToSet);
		SET_MATCHED_INDEX_CHARACTER_HP(PC, 2, HealthToSet, MaxHealthToSet);
		SET_MATCHED_INDEX_CHARACTER_HP(PC, 3, HealthToSet, MaxHealthToSet);
	}
	else
	{
		SET_MATCHED_INDEX_CHARACTER_HP(Opponent, 1, HealthToSet, MaxHealthToSet);
		SET_MATCHED_INDEX_CHARACTER_HP(Opponent, 2, HealthToSet, MaxHealthToSet);
		SET_MATCHED_INDEX_CHARACTER_HP(Opponent, 3, HealthToSet, MaxHealthToSet);
	}
}

#define GET_MATCHED_INDEX_CHARACTER_HP(TeamName, Index)		\
	if(CharIndex + 1 == Index)								\
		return Get##TeamName##Health##Index();

float UB2UIDocTeamMatch::GetCharacterHP(bool bPCTeam, int32 CharIndex)
{
	if (bPCTeam)
	{
		GET_MATCHED_INDEX_CHARACTER_HP(PC, 1);
		GET_MATCHED_INDEX_CHARACTER_HP(PC, 2);
		GET_MATCHED_INDEX_CHARACTER_HP(PC, 3);
	}
	else
	{
		GET_MATCHED_INDEX_CHARACTER_HP(Opponent, 1);
		GET_MATCHED_INDEX_CHARACTER_HP(Opponent, 2);
		GET_MATCHED_INDEX_CHARACTER_HP(Opponent, 3);
	}

	return 0.f;
}

#define GET_MATCHED_INDEX_CHARACTER_MAXHP(TeamName, Index)	\
	if(CharIndex + 1 == Index)								\
		return Get##TeamName##MaxHealth##Index();

float UB2UIDocTeamMatch::GetCharacterMaxHP(bool bPCTeam, int32 CharIndex)
{
	if (bPCTeam)
	{
		GET_MATCHED_INDEX_CHARACTER_MAXHP(PC, 1);
		GET_MATCHED_INDEX_CHARACTER_MAXHP(PC, 2);
		GET_MATCHED_INDEX_CHARACTER_MAXHP(PC, 3);
	}
	else
	{
		GET_MATCHED_INDEX_CHARACTER_MAXHP(Opponent, 1);
		GET_MATCHED_INDEX_CHARACTER_MAXHP(Opponent, 2);
		GET_MATCHED_INDEX_CHARACTER_MAXHP(Opponent, 3);
	}

	return 0.f;
}

#define SET_MATCHED_INDEX_CHARACTER_SHIELD(TeamName, Index, shield)	\
	if(CharIndex + 1 == Index)										\
		Set##TeamName##Shield##Index(shield);						

void UB2UIDocTeamMatch::SetCharacterShield(bool bPCTeam, int32 CharIndex, float ShieldToSet)
{
	if (bPCTeam)
	{
		SET_MATCHED_INDEX_CHARACTER_SHIELD(PC, 1, ShieldToSet);
		SET_MATCHED_INDEX_CHARACTER_SHIELD(PC, 2, ShieldToSet);
		SET_MATCHED_INDEX_CHARACTER_SHIELD(PC, 3, ShieldToSet);
	}
	else
	{
		SET_MATCHED_INDEX_CHARACTER_SHIELD(Opponent, 1, ShieldToSet);
		SET_MATCHED_INDEX_CHARACTER_SHIELD(Opponent, 2, ShieldToSet);
		SET_MATCHED_INDEX_CHARACTER_SHIELD(Opponent, 3, ShieldToSet);
	}
}

#define GET_MATCHED_INDEX_CHARACTER_SHIELD(TeamName, Index)		\
	if(CharIndex + 1 == Index)									\
		return Get##TeamName##Shield##Index();
float UB2UIDocTeamMatch::GetCharacterShield(bool bPCTeam, int32 CharIndex)
{
	if (bPCTeam)
	{
		GET_MATCHED_INDEX_CHARACTER_SHIELD(PC, 1);
		GET_MATCHED_INDEX_CHARACTER_SHIELD(PC, 2);
		GET_MATCHED_INDEX_CHARACTER_SHIELD(PC, 3);
	}
	else
	{
		GET_MATCHED_INDEX_CHARACTER_SHIELD(Opponent, 1);
		GET_MATCHED_INDEX_CHARACTER_SHIELD(Opponent, 2);
		GET_MATCHED_INDEX_CHARACTER_SHIELD(Opponent, 3);
	}

	return 0.f;
}

#define SET_MATCHED_INDEX_CHARACTER_CLASS(TeamName, Index, Class)	\
	if(CharIndex + 1 == Index)										\
		Set##TeamName##CharacterClass##Index(Class);

void UB2UIDocTeamMatch::SetCharacterClass(bool bPCTeam, int32 CharIndex, EPCClass Class)
{
	if (bPCTeam)
	{
		SET_MATCHED_INDEX_CHARACTER_CLASS(PC, 1, Class);
		SET_MATCHED_INDEX_CHARACTER_CLASS(PC, 2, Class);
		SET_MATCHED_INDEX_CHARACTER_CLASS(PC, 3, Class);
	}
	else
	{
		SET_MATCHED_INDEX_CHARACTER_CLASS(Opponent, 1, Class);
		SET_MATCHED_INDEX_CHARACTER_CLASS(Opponent, 2, Class);
		SET_MATCHED_INDEX_CHARACTER_CLASS(Opponent, 3, Class);
	}
}

#define GET_MATCHED_INDEX_CHARACTER_CLASS(TeamName, Index)	\
	if(CharIndex + 1 == Index)								\
		return Get##TeamName##CharacterClass##Index();

EPCClass UB2UIDocTeamMatch::GetCharacterClass(bool bPCTeam, int32 CharIndex)
{
	if (bPCTeam)
	{
		GET_MATCHED_INDEX_CHARACTER_CLASS(PC, 1);
		GET_MATCHED_INDEX_CHARACTER_CLASS(PC, 2);
		GET_MATCHED_INDEX_CHARACTER_CLASS(PC, 3);
	}
	else
	{
		GET_MATCHED_INDEX_CHARACTER_CLASS(Opponent, 1);
		GET_MATCHED_INDEX_CHARACTER_CLASS(Opponent, 2);
		GET_MATCHED_INDEX_CHARACTER_CLASS(Opponent, 3);
	}

	return EPCClass::EPC_End;
}

int8 UB2UIDocTeamMatch::GetCharacterFormationNum(int32 CharIndex)
{
	EFormationNUM FormationNum = EFormationNUM::None;
	switch (CharIndex)
	{
	case 0:
		FormationNum = GetFirstCharcterForamtionNUM();
		break;
	case 1:
		FormationNum = GetSecondCharcterForamtionNUM();
		break;
	case 2:
		FormationNum = GetThirdCharcterForamtionNUM();
		break;
	default:
		break;
	}

	return static_cast<int8>(FormationNum);
}

EFormationNUM UB2UIDocTeamMatch::GetFirstCharcterForamtionNUM()
{
	EFormationNUM FormationNumber = EFormationNUM::None;
	if (ETMFormation::ETM_Attack == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Front;
	}
	else if (ETMFormation::ETM_Defense == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Front;
	}
	else if (ETMFormation::ETM_Normal == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Front;
	}
	return FormationNumber;
}

EFormationNUM UB2UIDocTeamMatch::GetSecondCharcterForamtionNUM()
{
	EFormationNUM FormationNumber = EFormationNUM::None;
	if (ETMFormation::ETM_Attack == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Center;
	}
	else if (ETMFormation::ETM_Defense == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Front;
	}
	else if (ETMFormation::ETM_Normal == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Center;
	}
	return FormationNumber;
}

EFormationNUM UB2UIDocTeamMatch::GetThirdCharcterForamtionNUM()
{
	EFormationNUM FormationNumber = EFormationNUM::None;
	if (ETMFormation::ETM_Attack == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Center;
	}
	else if (ETMFormation::ETM_Defense == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Center;
	}
	else if (ETMFormation::ETM_Normal == GetTMFormation())
	{
		FormationNumber = EFormationNUM::Back;
	}
	return FormationNumber;
}
