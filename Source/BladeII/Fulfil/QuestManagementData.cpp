
#include "QuestManagementData.h"
#include "QuestManager.h"

void FQuestManagementData::FlushData()
{
	RemoveCompletedQuestFirst();
}

void FQuestManagementData::PassToCompleteQuest()
{
	// 	if (HasWaitingQuest())
	// 	{
	// 		AddCompleteQuest(GetWaitingQuest());
	// 		RemoveWaitingQuestFirst();
	// 	}
}

void FQuestManagementData::AddAutoClearQuest(const int32 QuestID)
{
	AutoClearQuestSet.Add(QuestID);
}

void FQuestManagementData::AddNextQuest(const b2network::B2QuestInfoPtr &NewQuest)
{
	if (NewQuest == nullptr)
	{
		NextQuest = nullptr;
		return;
	}
	bool bNullNextQuest = NextQuest == nullptr;
	bool bNextNumber = bNullNextQuest == false && (NextQuest->GetQuestId() < NewQuest->quest_id);
	if (bNullNextQuest || bNextNumber)
	{
		NextQuest = QuestPtr(new Quest(NewQuest->slot, NewQuest->type, NewQuest->quest_id, NewQuest->progress_count, NewQuest->state));
	}

	if (NextQuest->GetQuestId() == NewQuest->quest_id)
	{
		NextQuest->UpdateState(NewQuest->state);
	}

	if (NewQuest->state == EQS_COMPLETED)
		AddAutoClearQuest(NewQuest->quest_id);
}

void FQuestManagementData::AddCompleteQuest(const QuestPtr& CompleteQuest)
{
	CompletedQuestList.Add(CompleteQuest);
	CompletedQuestList.Sort([](const QuestPtr& A, const QuestPtr& B)
	{
		return A->GetQuestId() < B->GetQuestId();
	});
}

void FQuestManagementData::AddCompleteQuest(const Quest& CompleteQuest)
{
	QuestPtr  MewQuest(new Quest(CompleteQuest.GetQuestSlot(), CompleteQuest.GetType(), CompleteQuest.GetQuestId(), CompleteQuest.GetProgressCount(), CompleteQuest.GetState()));

	AddCompleteQuest(MewQuest);
}

void FQuestManagementData::AddQuestReward(const int32 QuestID, const TArray<b2network::B2RewardPtr>& Reward)
{
	if (QuestRewardMap.Contains(QuestID))
		QuestRewardMap[QuestID] = Reward;
	else
		QuestRewardMap.Add(QuestID, Reward);
}

bool FQuestManagementData::IsAutoClearQuest(const int32 QuestID)
{
	return AutoClearQuestSet.Contains(QuestID);
}

bool FQuestManagementData::HasAutoClearQuest()
{
	return AutoClearQuestSet.Num() > 0;
}

bool FQuestManagementData::HasCompletedQuest()
{
	return CompletedQuestList.Num() > 0;
}

bool FQuestManagementData::HasQuestReward(const int32 QuestID)
{
	return QuestRewardMap.Contains(QuestID);
}

const QuestPtr FQuestManagementData::GetCurrentCompletedQuest()
{
	if (HasCompletedQuest())
	{
		if (IsAutoClearQuest(CompletedQuestList[0]->GetQuestId()))
			CompletedQuestList[0]->UpdateState(EQS_PROGRESS);
		return CompletedQuestList[0];
	}
	return nullptr;
}

const QuestPtr FQuestManagementData::GetNextQuest()
{
	return NextQuest;
}

const TArray<b2network::B2RewardPtr> FQuestManagementData::GetQuestReward(const int32 QuestID)
{
	if (QuestRewardMap.Contains(QuestID))
		return QuestRewardMap[QuestID];

	return TArray<b2network::B2RewardPtr>();
}

void FQuestManagementData::RemoveAll()
{
	AutoClearQuestSet.Empty();
	CompletedQuestList.Empty();
	QuestRewardMap.Empty();
}

void FQuestManagementData::RemoveAutoClearQuest(const int32 QuestID)
{
	AutoClearQuestSet.Remove(QuestID);
}

void FQuestManagementData::RemoveCompletedQuestFirst()
{
	if (HasCompletedQuest())
	{
		int32 QuestID = CompletedQuestList[0]->GetQuestId();
		RemoveAutoClearQuest(QuestID);
		CompletedQuestList.RemoveAt(0);
	}
}

void FQuestManagementData::RemoveReward(const int32 QuestID)
{
	QuestRewardMap.Remove(QuestID);
}
