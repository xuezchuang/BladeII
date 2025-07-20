#pragma once

#include "BladeIIGameMode.h"
#include "B2AsyncGameMode.generated.h"

namespace AsyncMatchState
{
	extern BLADEII_API const FName RequestMatchMaking;
	extern BLADEII_API const FName MatchOtherPlayer;
	extern BLADEII_API const FName StartResult;
	extern BLADEII_API const FName FinishBattle;
}

/**
*
*/
struct FCharInfo
{
	FCharInfo() {} 
	FCharInfo(int8 InSpot, EPCClass InClass, int32 InDocCharIndex, int32 InAggroPriority, int8 FormationNum = 0) : 
		 AIController(nullptr),CharClass(InClass), Spot(InSpot),DocCharIndex(InDocCharIndex),  AggroPriority(InAggroPriority), FormationNumber(FormationNum) {}
	bool operator== (const FCharInfo& Other) const
	{
		return Spot == Other.Spot || CharClass == Other.CharClass;
	}

	class ABladeIIPlayerAIController* AIController;

	EPCClass CharClass;
	int8 Spot;

	int32 DocCharIndex;
	//etc..

	int32 AggroPriority;
	int8 FormationNumber;
};


UCLASS(ABSTRACT)
class BLADEII_API AB2AsyncGameMode : public ABladeIIGameMode
{
	GENERATED_BODY()

public:

	AB2AsyncGameMode(const FObjectInitializer& ObjectInitializer);

	virtual int8 GetMyTeamNum() { return 0; }
	virtual ABladeIIPlayer* GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly = false, bool bSpawnPuppetForDead = false);	
};
