#include "BladeII.h"
#include "B2AsyncGameMode.h"

namespace AsyncMatchState
{
	const FName RequestMatchMaking = FName(TEXT("RequestMatchMaking"));
	const FName MatchOtherPlayer = FName(TEXT("MatchOtherPlayer"));
	const FName StartResult = FName(TEXT("StartResult"));
	const FName FinishBattle = FName(TEXT("FinishBattle"));	//���̻� ���� �Ҽ� ���� ����(����� ���� ��Ȳ)�� ������â ������ State�� ��� �߰�
}

AB2AsyncGameMode::AB2AsyncGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ABladeIIPlayer* AB2AsyncGameMode::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly /*= false*/, bool bSpawnPuppetForDead /*= false*/)
{
	return NULL;
}
