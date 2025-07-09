#include "BladeII.h"
#include "B2UIDocCounterAttack.h"


UB2UIDocCounterAttack::UB2UIDocCounterAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DayType = EUICounterDayType::Monday;
}

UB2UIDocCounterAttack::~UB2UIDocCounterAttack()
{

}

void UB2UIDocCounterAttack::Init()
{
	Super::Init();
}

void UB2UIDocCounterAttack::Destroy()
{
	Super::Destroy();
}

void UB2UIDocCounterAttack::SetRewardItems(TArray<b2network::B2RewardPtr> items)
{
	RewardItems.Empty();

	for (auto item : items)
	{
		RewardItems.Add(item);
	}
}

TArray<b2network::B2RewardPtr> UB2UIDocCounterAttack::GetRewardItems()
{
	return RewardItems;
}