#pragma once
//#include "BladeII.h"
#include <type_traits>

#include "Trigger.h"

class ABladeIICharacter;

class IStat
{
public:
	IStat() : value_(0)
	{
	}

	virtual ~IStat()
	{
	}

	void SetTriggerTicket(uint32 id)
	{
		trigger_ticket_ = id;
	}

	uint32 GetTriggerTicket() const
	{
		return trigger_ticket_;
	}

	uint32	GetValue() const
	{
		return value_;
	}

	void SetValue(uint32 v)
	{
		value_ = v;
	}

	void IncreaseValue()
	{
		++value_;
	}

private:
	uint32		value_;
	uint32		trigger_ticket_;
};

class KillMonsterBaseStat : public IStat
{
public:
	KillMonsterBaseStat() : IStat()
	{
	}

	virtual ~KillMonsterBaseStat()
	{
	}

	virtual void SubscribeTrigger()		=	0;
	virtual void UnsubscribeTrigger()	=	0;

	virtual int32 GetMonsterType() const
	{
		return -1;
	}

	virtual int32 GetMonsterGrade() const
	{
		return -1;
	}

	virtual int32 GetMonsterId() const
	{
		return -1;
	}
};

class KillMonsterStat : public KillMonsterBaseStat
{
	using trigger_type = KillMonsterTriggerClass<>;
public:
	KillMonsterStat() : KillMonsterBaseStat()
	{
		SubscribeTrigger();
	}

	~KillMonsterStat()
	{
		UnsubscribeTrigger();
	}

	virtual void SubscribeTrigger()		final;
	virtual void UnsubscribeTrigger()	final;
};

class KillSpecificMonsterStat : public KillMonsterBaseStat
{
	using trigger_type = KillSpecificMonsterTriggerClass<ABladeIICharacter*>;
public:
	KillSpecificMonsterStat(int32 monster_id) :
		KillMonsterBaseStat(), 
		monster_id_(monster_id)
	{
		SubscribeTrigger();
	}

	~KillSpecificMonsterStat()
	{
		UnsubscribeTrigger();
	}

	virtual void SubscribeTrigger()		final;
	virtual void UnsubscribeTrigger()	final;

	virtual int32 GetMonsterId() const final
	{
		return monster_id_;
	}

	void IncreaseValueIfConditionMet(ABladeIICharacter*);

private:
	int32	monster_id_;
};