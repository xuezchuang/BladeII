#pragma once
//#include "BladeII.h"
#include <memory>

class IStat;
class KillMonsterBaseStat;

class Quest
{
public:
	Quest() : stat_(nullptr) {}
	Quest(int32 in_quest_slot, int32 type, int32 quest_id, int32 progress_count, int32 state) :
		quest_slot(in_quest_slot),
		type_(type),
		quest_id_(quest_id),
		progress_count_(progress_count),
		state_(state),
		monster_id_(-1),
		stat_(nullptr)
	{
	}

	virtual ~Quest();

	void UpdateProgress(int32);
	void UpdateState(int32);

	int32 GetQuestSlot()		const { return quest_slot; }
	int32 GetType()				const { return type_; }
	int32 GetQuestId()			const { return quest_id_; }
	int32 GetProgressCount()	const { return progress_count_; }
	int32 GetState()			const { return state_; }

	void AttachStat(KillMonsterBaseStat* stat)
	{
		stat_ = stat;
	}

	uint32 GetStatValue()			const;
	int32  GetStatMonsterType()		const;
	int32  GetStatMonsterGrade()	const;
	int32 GetStateMonsterId()		const;

private:
	int32		quest_slot;
	int32		type_;
	int32		quest_id_;
	int32		progress_count_;
	int32		state_;
	int32		monster_id_;

	KillMonsterBaseStat*		stat_;
};

using QuestPtr = std::shared_ptr<Quest>;