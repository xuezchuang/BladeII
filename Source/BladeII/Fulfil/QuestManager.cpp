#include "BladeII.h"
#include "QuestManager.h"
#include "Event.h"
#include "InternalEvents.h"
#include "Stat.h"
#include "BladeIIGameImpl.h"
//#include "B2UIManager.h"
//#include "B2UIQuestDialog.h"
//#include "B2UILobbyMain.h"


QuestManager& QuestManager::GetInstance()
{
	static QuestManager	inst;
	return inst;
}

void QuestManager::Init()
{
	SubscribeEvents();
}

QuestManager::QuestManager()
{
	CachedCompleteMap.Add(b2network::B2QuestSlotType::MAIN1, FQuestManagementDataPtr(new FQuestManagementData(b2network::B2QuestSlotType::MAIN1)));
	CachedCompleteMap.Add(b2network::B2QuestSlotType::MAIN2, FQuestManagementDataPtr(new FQuestManagementData(b2network::B2QuestSlotType::MAIN2)));
}

void QuestManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryMasterDataClass<FB2MasterDatas>::GetInstance().Subscribe2(
		[this](const FB2MasterDatas& data)
	{
		this->BuildUpTables(data);
	}
	));

	Issues.Add(DeliveryGetQuestClass<FB2GetQuest>::GetInstance().Subscribe2(
		[this](const FB2GetQuest& data)
	{
		CachedCompleteMap[b2network::B2QuestSlotType::MAIN1]->RemoveAll();
		CachedCompleteMap[b2network::B2QuestSlotType::MAIN2]->RemoveAll();
		this->UpdateQuests(data);

		DeliveryGetQuestUIClass<FB2GetQuest>::GetInstance().Signal(data);
	}
	));

	Issues.Add(DeliveryAcceptQuestClass<FB2AcceptQuest>::GetInstance().Subscribe2(
		[this](const FB2AcceptQuest& data)
	{
		this->AcceptQuest(data);
	}
	));

	Issues.Add(DeliveryReceiveQuestRewardClass<FB2ReceiveQuestReward>::GetInstance().Subscribe2(
		[this](const FB2ReceiveQuestReward& data)
	{
		this->DoneQuest(data);
	}
	));

	Issues.Add(DeliveryeGiveUpQuestClass<FB2GiveUpQuestPtr>::GetInstance().Subscribe2(
		[this](const FB2GiveUpQuestPtr& data)
	{
		this->GiveUpQuest(data);
	}
	));

	Issues.Add(data_trader::QuestAlarmNotifyClass<int32, b2network::B2QuestInfoPtr, const TArray<b2network::B2RewardPtr>>::GetInstance().Subscribe2(
		[this](int32 slot, b2network::B2QuestInfoPtr info, const TArray<b2network::B2RewardPtr> RewardList)
	{
		if (slot == b2network::B2QuestSlotType::MAIN1)
		{
			this->InternalDoneQuest(slot, info, RewardList);
		}
		else
		{
			QuestPtr CompleteSubQuest = GetQuest(slot);
			if (CompleteSubQuest)
			{
				CachedCompleteSubQuests.Add(CompleteSubQuest); // 바로 Notify 때리는게 우선. Noti 시점을 정해야하므로 Caching도 해야함
				OnQuestCompleteNotifyClass<int32>::GetInstance().Signal(slot);
			}
		}
	}
	));
}

void QuestManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void QuestManager::BuildUpTables(const FB2MasterDatas& data)
{
	//MainQuestTable.Empty();
	//DailyQuestTable.Empty();
	//RewardTable.Empty();

	//TSet<int32> GuardQuestSet;

	//const TArray<b2network::B2mdMainQuestInfoPtr>& MainQuests = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_main_quest_infos_index, data);
	//for (const b2network::B2mdMainQuestInfoPtr& MainQuest : MainQuests)
	//{
	//	if (GuardQuestSet.Contains(MainQuest->id))
	//	{
	//		ensure(!TEXT("############# Check QuestData!!!"));
	//		continue;
	//	}

	//	GuardQuestSet.Add(MainQuest->id);
	//	MainQuestTable.Add(MainQuest);
	//}

	//const TArray<b2network::B2mdDailyQuestInfoPtr>& DailyQuests = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_daily_quest_infos_index, data);
	//for (const b2network::B2mdDailyQuestInfoPtr& DailyQuest : DailyQuests)
	//{
	//	DailyQuestTable.Add(DailyQuest);
	//}

	//const TArray<b2network::B2mdRewardInfoPtr>& Rewards = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_reward_infos_index, data);
	//for (const b2network::B2mdRewardInfoPtr& Reward : Rewards)
	//{
	//	RewardTable.Add(Reward);
	//}
}

bool QuestManager::UpdateQuestState(int32 QuestSlot, int32 QuestState)
{
	if (QuestsInHand.Find(QuestSlot))
	{
		auto QuestElem = QuestsInHand.Find(QuestSlot);
		QuestPtr  Finded = QuestElem ? *QuestElem : nullptr;
		if (Finded)
		{
			auto NewQuest = QuestPtr(new Quest(
				Finded->GetQuestSlot(),
				Finded->GetType(),
				Finded->GetQuestId(),
				Finded->GetProgressCount(),
				QuestState));

			QuestsInHand.FindAndRemoveChecked(QuestSlot);
			QuestsInHand.Add(QuestSlot, NewQuest);
		}
		return true;
	}
	return false;
}

void QuestManager::UpdateQuests(FB2GetQuest const& data)
{
	//QuestsInHand.Empty();

	//const TArray<b2network::B2QuestInfoPtr>&	quests = GET_TUPLE_DATA(FB2ResponseGetQuest::quest_infos_index, data);
	//for (const b2network::B2QuestInfoPtr quest : quests)
	//{
	//	CreateOrUpdateQuest(quest);

	//	if (CachedCompleteMap.Contains(quest->slot) && quest->state == EQS_COMPLETED)
	//		CachedCompleteMap[quest->slot]->AddAutoClearQuest(quest->quest_id);
	//}

	//const auto NewDay = GET_TUPLE_DATA(FB2ResponseGetQuest::daily_quest_initialized_index, data);
	//if (!NewDay)
	//	return;

	//// you get quests which come from yesterday...
	//// you're going to have to show a user that quests...
	//TArray<int32>	QuestFromYesterday;
	//for (const auto MyQuest : QuestsInHand)
	//{
	//	auto quest = MyQuest.Value;
	//	if (quest->GetState() != EQS_PROGRESS)
	//		continue;

	//	QuestFromYesterday.Add(quest->GetQuestSlot());
	//}

	//InformQuestFromYesterdayClass<TArray<int32>>::GetInstance().Signal(QuestFromYesterday);
}

void QuestManager::AcceptQuest(FB2AcceptQuest const& data)
{
	//const b2network::B2QuestInfoPtr	quest = GET_TUPLE_DATA(FB2ResponseAcceptQuest::quest_info_index, data);
	//CreateOrUpdateQuest(quest);
}

void QuestManager::DoneQuest(FB2ReceiveQuestReward const& data)
{
	//const auto	id = GET_TUPLE_DATA(FB2ResponseReceiveQuestReward::received_reward_slot_index, data);
	//const auto	next_quest = GET_TUPLE_DATA(FB2ResponseReceiveQuestReward::next_quest_info_index, data);
	//const auto  rewardList = GET_TUPLE_DATA(FB2ResponseReceiveQuestReward::received_rewards_index, data);

	//InternalDoneQuest(id, next_quest, rewardList);

	//UB2UIManager::GetInstance()->CloseUI(UIFName::QuestNotice);
	//UB2UIManager::GetInstance()->CloseUI(UIFName::QuestDialog);

	////서브퀘 업데이트 해야함
	//UpdateSubQuestScollClass<>::GetInstance().Signal();
}

void QuestManager::GiveUpQuest(FB2GiveUpQuestPtr const& InData)
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::QuestNotice);

	//b2network::B2QuestInfoPtr NextQuestInfo = InData->next_quest_info;
	//if (NextQuestInfo)
	//{
	//	int32 QuestSlot = NextQuestInfo->slot;

	//	auto QuestElem = QuestsInHand.Find(QuestSlot);
	//	BII_CHECK(QuestElem);
	//	QuestPtr quest = *QuestElem;
	//	Quest  Finded(quest->GetQuestSlot(), quest->GetType(), quest->GetQuestId(), quest->GetProgressCount(), quest->GetState());

	//	QuestsInHand.FindAndRemoveChecked(QuestSlot);

	//	// slot == 0 && quest_id == 0 -> 다음퀘스트가 존재하지 않음
	//	if (NextQuestInfo->slot != 0 && NextQuestInfo->quest_id != 0)
	//	{
	//		CreateOrUpdateQuest(NextQuestInfo);

	//		if (CachedCompleteMap.Contains(QuestSlot))
	//		{
	//			CachedCompleteMap[QuestSlot]->AddNextQuest(NextQuestInfo);
	//		}
	//	}
	//	else
	//	{
	//		if (CachedCompleteMap.Contains(QuestSlot))
	//		{
	//			CachedCompleteMap[QuestSlot]->AddNextQuest(nullptr);
	//		}
	//	}

	//	if (CachedCompleteMap.Contains(QuestSlot))
	//	{
	//		CachedCompleteMap[QuestSlot]->AddCompleteQuest(Finded);
	//	}

	//	// 캐쉬된 퀘스트를 지워주고, 서버로부터 새로 갱신받는다.
	//	FlushCachedCompleteQuest(QuestSlot);

	//	UB2UIQuestDialog* QuestDialogUI = UB2UIManager::GetInstance()->GetUI<UB2UIQuestDialog>(UIFName::QuestDialog);
	//	if (QuestDialogUI == nullptr)
	//	{
	//		QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
	//	}

	//	QuestPtr QuestPtrInfo = GetQuest(QuestSlot);
	//	if (QuestPtrInfo != nullptr && QuestPtrInfo->GetState() != EQS_REWARDED && QuestDialogUI != nullptr)
	//	{
	//		// 만약 대화형같은 미리 완료로 되있을 경우 Auto Clear 상태로 만들어 주어 대화후 보상받기로 넘어가게 한다.
	//		if (QuestPtrInfo->GetState() == EQS_COMPLETED)
	//		{
	//			QuestPtrInfo->UpdateState(EQS_PROGRESS);
	//			SetAutoClearQuest(QuestPtrInfo->GetQuestSlot(), QuestPtrInfo->GetQuestId());
	//		}

	//		QuestDialogUI->OpenDialogBySlotIndex(QuestSlot);
	//		// 팝업 종료 후 퀘스트 갱신
	//		QuestDialogUI->SetDialogCloseDelegate(FOnQuestDialogClosed::CreateLambda([QuestSlot]() {
	//			UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//			if (LobbyMainWidget != nullptr)
	//			{
	//				LobbyMainWidget->UpdateSingleQuestDisplay(QuestSlot);
	//			}
	//		}));
	//	}
	//	else
	//	{
	//		UB2UILobbyMain* LobbyMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//		if (LobbyMainWidget != nullptr)
	//		{
	//			LobbyMainWidget->UpdateSingleQuestDisplay(QuestSlot);
	//		}
	//		UB2UIManager::GetInstance()->CloseUI(UIFName::QuestDialog);
	//	}
	//}
	//else
	//{
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::QuestDialog);
	//}

	////서브퀘 업데이트 해야함
	//UpdateSubQuestScollClass<>::GetInstance().Signal();
}

void QuestManager::EmptyQuestsInHand()
{
	QuestsInHand.Empty();
}

bool QuestManager::IsEndMainQuest() const
{
	//if (MainQuestTable.Num() > 0)
	//{
	//	auto CurrentMainQuest = GetQuest(b2network::B2QuestSlotType::MAIN1);
	//	auto EndMainQuest = MainQuestTable[MainQuestTable.Num() - 1];

	//	if (CurrentMainQuest && EndMainQuest)
	//	{
	//		if (CurrentMainQuest->GetQuestId() && EndMainQuest->id)
	//		{
	//			return CurrentMainQuest->GetState() == EQS_REWARDED;
	//		}
	//	}
	//}
	return false;
}

QuestPtr QuestManager::GetQuest(int32 QuestSlot) const
{
	auto QuestElem = QuestsInHand.Find(QuestSlot);
	QuestPtr Finded = QuestElem ? *QuestElem : nullptr;

	if (Finded == nullptr)
		return nullptr;

	return Finded;
}

b2network::B2mdMainQuestInfoPtr QuestManager::GetMainQuestMasterInfo(int32 QuestID) const
{
	for (auto MainQuest : MainQuestTable)
	{
		if (MainQuest && QuestID == MainQuest->id)
			return MainQuest;
	}

	return nullptr;
}

void QuestManager::GetMainQuestMasterInfos(TArray<b2network::B2mdMainQuestInfoPtr>& OutList, int InActID)
{
	OutList.Empty();

	for (b2network::B2mdMainQuestInfoPtr MainQuest : MainQuestTable)
	{
		if (MainQuest &&
			b2network::B2MainQuestType::MAIN == MainQuest->main_quest_type &&
			MainQuest->act_id == InActID)
		{
			OutList.Add(MainQuest);
		}
	}
}

int32 QuestManager::GetTotalNumberMainQuestByAct(int32 ActID)
{
	int32 Result = 0;

	for (b2network::B2mdMainQuestInfoPtr MainQuest : MainQuestTable)
	{
		if (MainQuest &&
			b2network::B2MainQuestType::MAIN == MainQuest->main_quest_type &&
			MainQuest->act_id == ActID)
		{
			Result++;
		}
	}

	return Result;
}

void QuestManager::GetSubQuestMasterInfos(TArray<b2network::B2mdMainQuestInfoPtr>& OutList, int InDifficulty)
{
	OutList.Empty();

	for (b2network::B2mdMainQuestInfoPtr MainQuest : MainQuestTable)
	{
		if (MainQuest &&
			b2network::B2MainQuestType::SUB == MainQuest->main_quest_type &&
			MainQuest->difficulty == InDifficulty)
		{
			OutList.Add(MainQuest);
		}
	}
}

b2network::B2mdDailyQuestInfoPtr QuestManager::GetDailyQuestMasterInfo(int32 QuestID) const
{
	for (auto DailyQuest : DailyQuestTable)
	{
		if (DailyQuest && QuestID == DailyQuest->id)
			return DailyQuest;
	}

	return nullptr;
}

void QuestManager::CreateOrUpdateQuest(b2network::B2QuestInfoPtr quest)
{
	auto QuestElem = QuestsInHand.Find(quest->slot);
	QuestPtr  Finded = QuestElem ? *QuestElem : nullptr;
	if (Finded)
	{
		Finded->UpdateProgress(quest->progress_count);
		Finded->UpdateState(quest->state);

		return;
	}

	auto NewQuest = QuestPtr(new Quest(quest->slot, quest->type, quest->quest_id, quest->progress_count, quest->state));
	auto Stat = CreateStat(quest);

	if (Stat)
		NewQuest->AttachStat(Stat);

	QuestsInHand.Add(quest->slot, NewQuest);
}

KillMonsterBaseStat* QuestManager::CreateStat(b2network::B2QuestInfoPtr quest)
{
	KillMonsterBaseStat*	ret = nullptr;
	int32					event_type = -1;
	int32					monster_id = -1;

	if (quest->type == MAIN_QUEST)
	{
		const auto MainQuest = GetMainQuestMasterInfo(quest->quest_id);

		ensure(MainQuest);
		if (MainQuest)
		{
			event_type = MainQuest->event_type;
			monster_id = MainQuest->condition_2;
		}
	}
	else if (quest->type == DAILY_QUEST)
	{
		const auto DailyQuest = GetDailyQuestMasterInfo(quest->quest_id);

		ensure(DailyQuest);
		if (DailyQuest)
		{
			event_type = DailyQuest->event_type;
			monster_id = DailyQuest->condition_2;
		}
	}

	if (event_type == b2network::B2GameEventType::KILL_MONSTER)
		ret = new KillMonsterStat;
	else if (event_type == b2network::B2GameEventType::KILL_SPECIFIC_MONSTER_STAGE)
	{
		ret = new KillSpecificMonsterStat(monster_id);
	}

	return ret;
}

uint32	QuestManager::GetTotalKilledMonsters()
{
	uint32 total = 0;
	for (auto MyQuest : QuestsInHand)
	{
		auto quest = MyQuest.Value;
		total += quest->GetStatValue();
	}

	return total;
}

TArray<b2network::B2KillMonsterInfoPtr> QuestManager::GetArrayKilledMonsters()
{
	TArray<b2network::B2KillMonsterInfoPtr>	infos;

	for (auto MyQuest : QuestsInHand)
	{
		auto quest = MyQuest.Value;
		auto info = b2network::B2KillMonsterInfoPtr(new b2network::B2KillMonsterInfo);

		if (quest->GetStateMonsterId() != -1)
		{
			info->monster_id = quest->GetStateMonsterId();
			info->monster_grade = quest->GetStatMonsterGrade();
			info->kill_count = quest->GetStatValue();
			infos.Add(info);
		}
	}

	return infos;
}

uint32 QuestManager::GetQuestGoalCount(int32 QuestID, uint8 QuestType) const
{
	if (QuestType == MAIN_QUEST)
	{
		auto MasterData = QuestManager::GetInstance().GetMainQuestMasterInfo(QuestID);
		return (MasterData != nullptr) ? MasterData->goal_count : 1;
	}
	else if (QuestType == DAILY_QUEST)
	{
		auto MasterData = QuestManager::GetInstance().GetDailyQuestMasterInfo(QuestID);
		return (MasterData != nullptr) ? MasterData->goal_count : 1;
	}

	return 0;
}

QuestPtr QuestManager::GetCachedCurrentQuest()
{
	int32 SlotIndexs[] = { b2network::B2QuestSlotType::MAIN1, b2network::B2QuestSlotType::MAIN2 };
	for (int32 i = 0; i < CachedCompleteMap.Num(); ++i)
	{
		auto &Each = CachedCompleteMap[SlotIndexs[i]];
		if (Each->HasCompletedQuest())
		{
			return Each->GetCurrentCompletedQuest();
		}

		if (Each->GetNextQuest() != nullptr)
			return Each->GetNextQuest();
	}

	return nullptr;
}

bool QuestManager::HasCachedQuest(int32 QuestSlot)
{
	int32 SlotIndexs[] = { b2network::B2QuestSlotType::MAIN1, b2network::B2QuestSlotType::MAIN2 };
	for (int32 i = 0; i < CachedCompleteMap.Num(); ++i)
	{
		auto &Each = CachedCompleteMap[SlotIndexs[i]];
		if (Each->HasCompletedQuest() && Each->GetQuestSlot() == QuestSlot)
		{
			return true;
		}
	}

	return false;
}

bool QuestManager::HasCachedQuest()
{
	int32 SlotIndexs[] = { b2network::B2QuestSlotType::MAIN1, b2network::B2QuestSlotType::MAIN2 };
	for (int32 i = 0; i < CachedCompleteMap.Num(); ++i)
	{
		auto &Each = CachedCompleteMap[SlotIndexs[i]];
		if (Each->HasCompletedQuest())
		{
			return true;
		}
	}

	return false;
}

bool QuestManager::IsAutoClearQuest(int32 QuestSlot, int32 QuestID)
{
	if (CachedCompleteMap.Contains(QuestSlot) == false)
		return false;

	return CachedCompleteMap[QuestSlot]->IsAutoClearQuest(QuestID);
}

void QuestManager::SetAutoClearQuest(int32 QuestSlot, int32 QuestID)
{
	if (CachedCompleteMap.Contains(QuestSlot) == false)
		return;

	CachedCompleteMap[QuestSlot]->AddAutoClearQuest(QuestID);
}

const FCompleteQuestData& QuestManager::GetCachedCompleteQuest()
{
	int32 SlotIndexs[] = { b2network::B2QuestSlotType::MAIN1, b2network::B2QuestSlotType::MAIN2 };
	for (int32 i = 0; i < CachedCompleteMap.Num(); ++i)
	{
		auto &Each = CachedCompleteMap[SlotIndexs[i]];
		if (Each->HasCompletedQuest())
		{
			CachedCompleteData = FCompleteQuestData(Each->GetCurrentCompletedQuest(), Each->GetQuestReward(Each->GetCurrentCompletedQuest()->GetQuestId()));
			break;
		}
	}
	return CachedCompleteData;
}

void QuestManager::FlushCachedCompleteQuest()
{
	int32 SlotIndexs[] = { b2network::B2QuestSlotType::MAIN1, b2network::B2QuestSlotType::MAIN2 };
	for (int32 i = 0; i < CachedCompleteMap.Num(); ++i)
	{
		if (FlushCachedCompleteQuest(SlotIndexs[i]) == true)
			return;
	}
}

bool QuestManager::FlushCachedCompleteQuest(int32 SlotIndex)
{
	if (QuestsInHand.Find(SlotIndex))
		QuestsInHand.Remove(SlotIndex);

	auto &Each = CachedCompleteMap[SlotIndex];
	if (Each->HasCompletedQuest())
	{
		Each->RemoveCompletedQuestFirst();

		if (Each->HasCompletedQuest())
		{
			QuestPtr CurQuest = Each->GetCurrentCompletedQuest();

			QuestsInHand.Add(SlotIndex, Each->GetCurrentCompletedQuest());
			return true;
		}
	}

	if (Each->GetNextQuest() && Each->GetNextQuest()->GetQuestId() > 0)
	{
		QuestsInHand.Add(SlotIndex, Each->GetNextQuest());
		return true;
	}

	return false;
}

void QuestManager::InternalDoneQuest(int32 quest_slot, b2network::B2QuestInfoPtr next_quest_info, const TArray<b2network::B2RewardPtr>& rewardList)
{
	auto QuestElem = QuestsInHand.Find(quest_slot);
	BII_CHECK(QuestElem);
	QuestPtr quest = *QuestElem;
	Quest  Finded(quest->GetQuestSlot(), quest->GetType(), quest->GetQuestId(), quest->GetProgressCount(), quest->GetState());

	QuestsInHand.FindAndRemoveChecked(quest_slot);

	// slot == 0 && quest_id == 0 -> 다음퀘스트가 존재하지 않음
	if (next_quest_info && next_quest_info->slot != 0 && next_quest_info->quest_id != 0)
	{
		CreateOrUpdateQuest(next_quest_info);

		if (CachedCompleteMap.Contains(quest_slot))
			CachedCompleteMap[quest_slot]->AddNextQuest(next_quest_info);
	}
	else
	{
		if (CachedCompleteMap.Contains(quest_slot))
			CachedCompleteMap[quest_slot]->AddNextQuest(nullptr);
	}

	if (CachedCompleteMap.Contains(quest_slot))
	{
		CachedCompleteMap[quest_slot]->AddQuestReward(quest->GetQuestId(), rewardList);
		CachedCompleteMap[quest_slot]->AddCompleteQuest(Finded);
	}

	// 메인퀘일땐 보상수량 적용, 나머지는 이벤트 호출(보상확인 팝업 닫고 수량적용함)
	if (Finded.GetType() == DAILY_QUEST || Finded.GetType() == MAIN_QUEST)
	{
		DeliveryQuestRewardClass<Quest, TArray<b2network::B2RewardPtr>>::GetInstance().Signal(Finded, rewardList);
	}
	else
	{
		BladeIIGameImpl::GetClientDataStore().ResponseGetQuestReward(rewardList);
	}
}

void QuestManager::FlushCachedCompleteSubQuests()
{
	CachedCompleteSubQuests.Empty();
}
