#include "BladeII.h"
#include "B2AsyncGameMode.h"

namespace AsyncMatchState
{
	const FName RequestMatchMaking = FName(TEXT("RequestMatchMaking"));
	const FName MatchOtherPlayer = FName(TEXT("MatchOtherPlayer"));
	const FName StartResult = FName(TEXT("StartResult"));
	const FName FinishBattle = FName(TEXT("FinishBattle"));	//더이상 전투 할수 없는 상태(결과가 나온 상황)와 결과출력창 사이의 State가 없어서 추가
}

AB2AsyncGameMode::AB2AsyncGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ABladeIIPlayer* AB2AsyncGameMode::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly /*= false*/, bool bSpawnPuppetForDead /*= false*/)
{
	return NULL;
}
