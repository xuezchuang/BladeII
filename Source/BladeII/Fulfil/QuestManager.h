#pragma once
//#include "BladeII.h"
#include "B2MessageInfoCommonStruct.h"
#include "Quest.h"
#include "EventSubsystem.h"
#include "QuestManagementData.h"
#include "../Common/CommonStruct.h"

class IStat;

enum class RewardType : uint8
{
	ERT_GOLD,
	ERT_GEM,
	ERT_BLADEPOINT,
	ERT_ITEM,
	ERT_LOTTERY,
	ERT_PICKLOTTERY,
};

const uint8	EQS_NOT_ACCEPTED	=	1;
const uint8 EQS_PROGRESS		=	2;
const uint8	EQS_COMPLETED		=	3;
const uint8	EQS_REWARDED		=	4;

const uint8 MAIN_QUEST			=	1;
const uint8 DAILY_QUEST			=	2;

struct FCompleteQuestData
{
	bool bValid;

	QuestPtr CompleteQuest;
	TArray<b2network::B2RewardPtr> CompleteReward;

	FCompleteQuestData() : bValid(false) {}
	FCompleteQuestData(const QuestPtr& InQuest, const TArray<b2network::B2RewardPtr>& Reward)
		: bValid(true)
	{
		CompleteQuest = InQuest;
		CompleteReward = Reward;
	}
};

class QuestManager
{
public:

	static QuestManager& GetInstance();

	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();
	void BuildUpTables(const FB2MasterDatas&);

	void UpdateQuests(FB2GetQuest const&);
	bool UpdateQuestState(int32 QuestSlot, int32 QuestState);
	void AcceptQuest(FB2AcceptQuest const&);
	void DoneQuest(FB2ReceiveQuestReward const&);
	void GiveUpQuest(FB2GiveUpQuestPtr const& InData);
	void EmptyQuestsInHand();

	bool IsEndMainQuest() const;

	QuestPtr							GetQuest(int32 QuestSlot)				const;
	b2network::B2mdMainQuestInfoPtr		GetMainQuestMasterInfo(int32 QuestID)	const;
	void								GetMainQuestMasterInfos(TArray<b2network::B2mdMainQuestInfoPtr>& OutList, int InActID);
	int32								GetTotalNumberMainQuestByAct(int32 ActID);
	void								GetSubQuestMasterInfos(TArray<b2network::B2mdMainQuestInfoPtr>& OutList, int InDifficulty);
	b2network::B2mdDailyQuestInfoPtr	GetDailyQuestMasterInfo(int32 QuestID)	const;

	uint32									GetTotalKilledMonsters();
	TArray<b2network::B2KillMonsterInfoPtr> GetArrayKilledMonsters();

	uint32 GetQuestGoalCount(int32 QuestID, uint8 QuestType) const;
	
	bool								HasCachedQuest();
	bool								HasCachedQuest(int32 QuestSlot);
	bool								IsAutoClearQuest(int32 QuestSlot, int32 QuestID);
	void								SetAutoClearQuest(int32 QuestSlot, int32 QuestID);
	QuestPtr							GetCachedCurrentQuest();
	const FCompleteQuestData&			GetCachedCompleteQuest();
	void								FlushCachedCompleteQuest();
	bool								FlushCachedCompleteQuest(int32 SlotIndex);
	int32								GetTotalRewardGoldAmount();

	FORCEINLINE const TArray<QuestPtr>&	GetCachedCompleteSubQuests() { return CachedCompleteSubQuests; }
	void								FlushCachedCompleteSubQuests();

private:

	QuestManager();
	QuestManager(QuestManager const&)				= delete;
	QuestManager operator = (QuestManager const&)	= delete;

	void					CreateOrUpdateQuest(b2network::B2QuestInfoPtr);
	KillMonsterBaseStat*	CreateStat(b2network::B2QuestInfoPtr);
	void					InternalDoneQuest(int32, b2network::B2QuestInfoPtr, const TArray<b2network::B2RewardPtr>&);

	TArray<b2network::B2mdMainQuestInfoPtr>		MainQuestTable;
	TArray<b2network::B2mdDailyQuestInfoPtr>	DailyQuestTable;
	TArray<b2network::B2mdRewardInfoPtr>		RewardTable;

	TMap<int32, QuestPtr>						QuestsInHand;
	
	FCompleteQuestData							CachedCompleteData;
	TMap<int32, FQuestManagementDataPtr>		CachedCompleteMap;

	TArray<QuestPtr>							CachedCompleteSubQuests;

	TArray<std::shared_ptr<issue>>				Issues;
};
