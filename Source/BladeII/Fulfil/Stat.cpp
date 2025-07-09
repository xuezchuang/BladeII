#include "BladeII.h"
#include "Stat.h"

#include "BladeIICharacter.h"


#include "BladeIIGameImpl.h"

void KillMonsterStat::SubscribeTrigger()
{
	auto id = trigger_type::GetInstance().Subscribe(
		[this]()
		{
			this->IncreaseValue();
		}
	);

	SetTriggerTicket(id);
}

void KillMonsterStat::UnsubscribeTrigger()
{
	trigger_type::GetInstance().Unsubscribe(GetTriggerTicket());
}

void KillSpecificMonsterStat::SubscribeTrigger()
{
	auto id = trigger_type::GetInstance().Subscribe(
		[this](ABladeIICharacter* dead)
		{
			this->IncreaseValueIfConditionMet(dead);
		}
	);

	SetTriggerTicket(id);
}

void KillSpecificMonsterStat::UnsubscribeTrigger()
{
	trigger_type::GetInstance().Unsubscribe(GetTriggerTicket());
}

void KillSpecificMonsterStat::IncreaseValueIfConditionMet(ABladeIICharacter* dead)
{
	const FMonsterInfo *MonsterInfo = BladeIIGameImpl::GetClientDataStore().GetMonsterInfo(dead->GetNPCClass(), dead->GetNPCClassVariation());
	int32 MonsterId = MonsterInfo ? MonsterInfo->Id : -1;

	if ((MonsterId < 0) || (MonsterId != monster_id_))
		return;

	IncreaseValue();
}