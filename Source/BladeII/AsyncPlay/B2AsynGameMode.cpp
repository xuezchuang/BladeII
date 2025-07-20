//#include "BladeII.h"
#include "B2AsyncGameMode.h"

namespace AsyncMatchState
{
	const FName RequestMatchMaking = FName(TEXT("RequestMatchMaking"));
	const FName MatchOtherPlayer = FName(TEXT("MatchOtherPlayer"));
	const FName StartResult = FName(TEXT("StartResult"));
	const FName FinishBattle = FName(TEXT("FinishBattle"));	//歹捞惑 傈捧 且荐 绝绰 惑怕(搬苞啊 唱柯 惑炔)客 搬苞免仿芒 荤捞狼 State啊 绝绢辑 眠啊
}

AB2AsyncGameMode::AB2AsyncGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ABladeIIPlayer* AB2AsyncGameMode::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly /*= false*/, bool bSpawnPuppetForDead /*= false*/)
{
	return NULL;
}
