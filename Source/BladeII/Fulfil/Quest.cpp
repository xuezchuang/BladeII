
#include "Quest.h"

#include "Stat.h"

Quest::~Quest()
{
	if (stat_)
	{
		delete stat_;
		stat_ = nullptr;
	}
}

void Quest::UpdateProgress(int32 progress)
{
	if (progress == progress_count_)
		return;

	progress_count_ = progress;
}

void Quest::UpdateState(int32 state)
{
	if (state == state_)
		return;

	state_ = state;
}

uint32 Quest::GetStatValue() const
{
	if (stat_)
		return stat_->GetValue();

	return 0;
}

int32 Quest::GetStatMonsterType() const
{
	if (stat_)
		return stat_->GetMonsterType();

	return -1;
}

int32 Quest::GetStatMonsterGrade() const
{
	if (stat_)
		return stat_->GetMonsterGrade();

	return -1;
}

int32 Quest::GetStateMonsterId()	const
{
	if (stat_)
		return stat_->GetMonsterId();

	return -1;
}