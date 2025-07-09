#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2UIDocCounterAttack.generated.h"

/* This is the place where you can put "SOME" fucking data that you have no idea of where to put shits in. */


UCLASS()
class BLADEII_API UB2UIDocCounterAttack : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocCounterAttack(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocCounterAttack();

	virtual void Init() override;
	virtual void Destroy() override;

	void SetRewardItems(TArray<b2network::B2RewardPtr> items);
	TArray<b2network::B2RewardPtr> GetRewardItems();

	DECLARE_UIDOC_FIELD(EUICounterDayType, DayType)
	DECLARE_UIDOC_FIELD(int32,	DungeonType)

	DECLARE_UIDOC_FIELD(int32,	Difficulty)
	DECLARE_UIDOC_FIELD(int32,					NewDifficulty)

	DECLARE_UIDOC_FIELD(TArray<int32>, OpenedDungeonType)
	DECLARE_UIDOC_FIELD(int32, OpenedDungeonSlot)
	DECLARE_UIDOC_FIELD(int32, LeftTicket)
	DECLARE_UIDOC_FIELD(int32, WeeklyClearCount)
	DECLARE_UIDOC_FIELD(int32, ReceivedWeeklyReward)
	DECLARE_UIDOC_FIELD(int32, NextBuyTicketGemCost)
	DECLARE_UIDOC_FIELD(int32, HighestLevel)

	// Whether local player recently won NetGame (PVP) match, or remote opponent won, or draw.
	//DECLARE_UIDOC_FIELD(ENetMatchResult, LocalPlayerMatchResult)
private:
	TArray<b2network::B2RewardPtr> RewardItems;	
};
