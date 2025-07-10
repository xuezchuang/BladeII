// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "B2MessageInfoCommonStruct.h"
#include "Quest.h"

class FQuestManagementData
{
public:
	FQuestManagementData(int32 Slot)
		: QuestSlot(Slot)
	{ }

	void FlushData();
	void PassToCompleteQuest();

	void AddNextQuest(const b2network::B2QuestInfoPtr &NextQuest);
	void AddCompleteQuest(const QuestPtr& CompleteQuest);
	void AddCompleteQuest(const Quest& CompleteQuest);
	void AddQuestReward(const int32 QuestID, const TArray<b2network::B2RewardPtr>& Reward);

	bool HasAutoClearQuest();
	bool HasCompletedQuest();
	bool HasQuestReward(const int32 QuestID);
	bool IsAutoClearQuest(const int32 QuestID);

	const QuestPtr GetCurrentCompletedQuest();
	const QuestPtr GetNextQuest();
	const TArray<b2network::B2RewardPtr> GetQuestReward(const int32 QuestID);

	void RemoveAll();
	void RemoveAutoClearQuest(const int32 QuestID);
	void RemoveCompletedQuestFirst();
	void RemoveReward(const int32 QuestID);

	int32 GetQuestSlot() { return QuestSlot; }
	int32 GetCompleteRewardGold();
	void AddAutoClearQuest(const int32 QuestID);

	
protected:

private:


private:
	TMap<int32, TArray<b2network::B2RewardPtr>> QuestRewardMap;

	TSet<int32> AutoClearQuestSet;
	TArray<QuestPtr> CompletedQuestList;
	QuestPtr CurrentQuest;
	QuestPtr NextQuest;
	int32 QuestSlot;
};

using FQuestManagementDataPtr = std::shared_ptr<FQuestManagementData>;