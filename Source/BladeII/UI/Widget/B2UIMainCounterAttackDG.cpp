#include "BladeII.h"
#include "B2UIMainCounterAttackDG.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIMainCounterAttackDG_Slot.h"
#include "B2UIChapterStarRewardBox.h"
#include "B2UIDocHelper.h"
#include "B2UIChapterStarRewardPreview.h"
#include "B2ItemInfo.h"
#include "B2UIRewardEventSet.h"
#include "B2UIMsgPopupReward.h"
#include "Retailer.h"

#include "B2HighlightWidget.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIMainCounterAttackDG::Init()
{
	Super::Init();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("CounterAttack_Title"));

	CAPTURE_UOBJECT(UB2UIMainCounterAttackDG);
	DeliveryCounterDungeonWeeklyRewardTicket = DeliveryCounterDungeonWeeklyRewardClass<FB2CounterDungeonWeeklyReward>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCounterDungeonWeeklyReward, const FB2CounterDungeonWeeklyReward& WeeklyReward)
		Capture->ResponseWeeklyReward(WeeklyReward);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetCounterTicket = DeliveryGetCounterDundeonClass<FB2GetCounterDungeon>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetCounter, const FB2GetCounterDungeon& msg)
		Capture->ResponseGetCounter(msg);
	END_CAPTURE_OBJECT()
		);

	data_trader::Retailer::GetInstance().RequestGetCounterDungeon();
}

void UB2UIMainCounterAttackDG::OnCloseComplete()
{
	DeliveryCounterDungeonWeeklyRewardClass<FB2CounterDungeonWeeklyReward>::GetInstance().Unsubscribe(DeliveryCounterDungeonWeeklyRewardTicket);
	DeliveryGetCounterDundeonClass<FB2GetCounterDungeon>::GetInstance().Unsubscribe(DeliveryGetCounterTicket);

	Super::OnCloseComplete();
}

void UB2UIMainCounterAttackDG::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("CounterAttack_Title"));
}

void UB2UIMainCounterAttackDG::CacheAssets()
{
	GET_SLOT(UB2UIMainCounterAttackDG_Slot, UIP_DungeonSlot1)
	if (UIP_DungeonSlot1.IsValid())
	{
		UIP_DungeonSlot1->Init();
		DungeonSlots.Add(UIP_DungeonSlot1);
	}
	GET_SLOT(UB2UIMainCounterAttackDG_Slot, UIP_DungeonSlot2)
	if (UIP_DungeonSlot2.IsValid())
	{
		UIP_DungeonSlot2->Init();
		DungeonSlots.Add(UIP_DungeonSlot2);
	}
	GET_SLOT(UB2UIMainCounterAttackDG_Slot, UIP_DungeonSlot3)
	if (UIP_DungeonSlot3.IsValid())
	{
		UIP_DungeonSlot3->Init();
		DungeonSlots.Add(UIP_DungeonSlot3);
	}
	GET_SLOT(UB2UIMainCounterAttackDG_Slot, UIP_DungeonSlot4)
	if (UIP_DungeonSlot4.IsValid())
	{
		UIP_DungeonSlot4->Init();
		DungeonSlots.Add(UIP_DungeonSlot4);
	}

	UIP_ChapterStarReward.SetNum(3);
	GET_SLOT_BYNAME(UB2UIChapterStarRewardBox, UIP_ChapterStarReward[0], UIP_ChapterStarRewardBox0)
	GET_SLOT_BYNAME(UB2UIChapterStarRewardBox, UIP_ChapterStarReward[1], UIP_ChapterStarRewardBox1)
	GET_SLOT_BYNAME(UB2UIChapterStarRewardBox, UIP_ChapterStarReward[2], UIP_ChapterStarRewardBox2)

	GET_SLOT(UB2UIChapterStarRewardPreview, UIP_ChapterStarRewardPreview)
	if (UIP_ChapterStarRewardPreview.IsValid())
		UIP_ChapterStarRewardPreview->Init();

	for (auto& Ele : UIP_ChapterStarReward)
	{
		Ele->Init();
		Ele->OnChapterStarRewardBoxClickedDelgate.BindDynamic(this, &UB2UIMainCounterAttackDG::OnClickBtnStarRewardBox);
	}

	//SetRewardInfo();
}

void UB2UIMainCounterAttackDG::UpdateStaticText()
{
}

void UB2UIMainCounterAttackDG::BindDelegates()
{
}

void UB2UIMainCounterAttackDG::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);
}

void UB2UIMainCounterAttackDG::OnClickBtnStarRewardBox(class UB2UIChapterStarRewardBox* ClickedReward)
{
	auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	const TArray<FCounterDungeunWeekReward>& ClientRewardData = BladeIIGameImpl::GetClientDataStore().GetCounterDeungeonRewardData();

	int32 WeeklyRewardCount = CounterDoc->GetWeeklyClearCount();
	int32 ReceivedWeeklyReward = CounterDoc->GetReceivedWeeklyReward();
	int32 i = 0;
	if (ClientRewardData.Num() < ClickedReward->GetRewardIndex())
		return;

	for (auto& Ele : ClientRewardData[ClickedReward->GetRewardIndex()].RewardItem)
	{
		//[@AKI, 170605] Goods를 설정하기 위해 Icon을 넣는 방식에서 Index를 넣는 방식으로 변경
		UIP_ChapterStarRewardPreview->CreateRewardIconCommon(Ele.Id, FText::AsNumber(Ele.Count));
		//UIP_ChapterStarRewardPreview->CreateRewardIconCommon(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(Ele.Id),
		//	FText::AsNumber(Ele.Count));

		int32 nRequireCount = ClientRewardData[ClickedReward->GetRewardIndex()].RequireCount;
		if (nRequireCount <= WeeklyRewardCount)
			UIP_ChapterStarRewardPreview->JoinShowMe(true, ClickedReward->GetRewardIndex()+1, WeeklyRewardCount, nRequireCount);
		else UIP_ChapterStarRewardPreview->JoinShowMe(false, ClickedReward->GetRewardIndex()+1, WeeklyRewardCount, nRequireCount);
	}
}

void UB2UIMainCounterAttackDG::SetRewardInfo()
{
	//auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	//const TArray<FCounterDungeunWeekReward>& ClientRewardData = BladeIIGameImpl::GetClientDataStore().GetCounterDeungeonRewardData();

	//int32 WeeklyRewardCount = CounterDoc->GetWeeklyClearCount();
	//int32 ReceivedWeeklyReward = CounterDoc->GetReceivedWeeklyReward();

	//int8 i = 0;
	//int32 PrevGoalStarNum = 0;
	//int32 CurNum = 0;
	//for (auto& Ele : ClientRewardData)
	//{
	//	if (!UIP_ChapterStarReward[i].IsValid())
	//		return;

	//	int32 ReceiveBoxNumber = StaticCast<int32>(FMath::Pow(10, i + 1));
	//	UIP_ChapterStarReward[i]->SetJoinMaxNum(Ele.RequireCount);

	//	// 첫번째자리 1이면  첫상자  두번째자리 1이면 두번째상자 세번째자리 1이면 세번째까지 열었다는거 111
	//	if (i == 0)
	//		UIP_ChapterStarReward[i]->SetRewarded(1 == (ReceivedWeeklyReward % 100) % 10);
	//	else if (i == 1)
	//		UIP_ChapterStarReward[i]->SetRewarded(10 <= ReceivedWeeklyReward % 100);
	//	else if (i == 2)
	//		UIP_ChapterStarReward[i]->SetRewarded(1 == static_cast<int32>(ReceivedWeeklyReward / 100));

	//	UIP_ChapterStarReward[i]->SetRewardIndex(i);

	//	if (Ele.RequireCount > WeeklyRewardCount && PrevGoalStarNum <= WeeklyRewardCount)
	//	{
	//		UIP_ChapterStarReward[i]->ShowProgressText(true);
	//		UIP_ChapterStarReward[i]->SetForceProgressText(WeeklyRewardCount, Ele.RequireCount);
	//		CurNum = WeeklyRewardCount;
	//	}
	//	else
	//	{
	//		UIP_ChapterStarReward[i]->ShowProgressText(false);
	//		PrevGoalStarNum > WeeklyRewardCount ? CurNum = 0 : CurNum = Ele.RequireCount;
	//	}

	//	UIP_ChapterStarReward[i]->SetStarCurNum(CurNum);
	//	UIP_ChapterStarReward[i]->SetForceProgressBar(CurNum - PrevGoalStarNum, Ele.RequireCount - PrevGoalStarNum);

	//	PrevGoalStarNum = Ele.RequireCount;
	//	++i;
	//}
}

void UB2UIMainCounterAttackDG::ResponseWeeklyReward(const FB2CounterDungeonWeeklyReward& WeeklyReward)
{
	//int32 CurGold = GET_TUPLE_DATA(FB2ResponseCounterDungeonWeeklyReward::current_gold_index, WeeklyReward);
	//int32 CurGem = GET_TUPLE_DATA(FB2ResponseCounterDungeonWeeklyReward::current_gem_index, WeeklyReward);
	//int32 WeeklyRewardCount = GET_TUPLE_DATA(FB2ResponseCounterDungeonWeeklyReward::received_weekly_reward_index, WeeklyReward);

	//auto RewardItem = GET_TUPLE_DATA(FB2ResponseCounterDungeonWeeklyReward::received_rewards_index, WeeklyReward);

	//auto CounterDoc = UB2UIDocHelper::GetDocCounterAttack();
	//auto UserDoc = UB2UIDocHelper::GetDocUser();
	//UB2UIManager* UIManager = UB2UIManager::GetInstance();
	//if (UIManager != nullptr && UserDoc != nullptr)
	//{
	//	CounterDoc->SetReceivedWeeklyReward(WeeklyRewardCount);
	//	UserDoc->UpdateUserData();

	//	auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	//	if (RewardPopup != nullptr)
	//	{
	//		RewardPopup->AddRewardItems(RewardItem);
	//		RewardPopup->ShowRewardPopup(true);
	//	}

	//	SetRewardInfo();
	//}
}

TWeakObjectPtr<class UB2UIMainCounterAttackDG_Slot> UB2UIMainCounterAttackDG::GetActiveDungeonSlot()
{
	for (TWeakObjectPtr<UB2UIMainCounterAttackDG_Slot> DGSlot : DungeonSlots)
	{
		if (DGSlot.IsValid() && DGSlot->IsOpenDungeon())
		{
			return DGSlot;
		}
	}

	return nullptr;
}

TUTORIAL class UB2UnitedWidgetBase* UB2UIMainCounterAttackDG::GetActiveDungeonWidgetPart()
{
	return Cast<UB2UnitedWidgetBase>(GetActiveDungeonSlot().Get());
}

void UB2UIMainCounterAttackDG::ResponseGetCounter(const FB2GetCounterDungeon& msg)
{
	/*auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	if (!CounterDoc)
		return;

	auto OpenedDungeonType = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::opened_dungeon_types_index, msg);
	CounterDoc->SetOpenedDungeonType(OpenedDungeonType);

	auto LeftTicket = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::left_ticket_index, msg);
	CounterDoc->SetLeftTicket(LeftTicket);

	auto WeeklyClearCount = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::weekly_clear_count_index, msg);
	CounterDoc->SetWeeklyClearCount(WeeklyClearCount);

	auto ReceivedWeeklyReward = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::received_weekly_reward_index, msg);
	CounterDoc->SetReceivedWeeklyReward(ReceivedWeeklyReward);

	auto NextBuyTicketGem = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::next_buy_ticket_gem_cost_index, msg);
	CounterDoc->SetNextBuyTicketGemCost(NextBuyTicketGem);

	UpdateOpenedSlot();*/
}

void UB2UIMainCounterAttackDG::UpdateOpenedSlot()
{
	if (UIP_DungeonSlot1.IsValid())
	{
		UIP_DungeonSlot1->SetDayType(b2network::B2CounterDungeon::TYPE::ARMOR, TEXT("CounterAttack_DungeonName1"));
	}
	if (UIP_DungeonSlot2.IsValid())
	{
		UIP_DungeonSlot2->SetDayType(b2network::B2CounterDungeon::TYPE::WEAPON, TEXT("CounterAttack_DungeonName2"));
	}
	if (UIP_DungeonSlot3.IsValid())
	{
		UIP_DungeonSlot3->SetDayType(b2network::B2CounterDungeon::TYPE::ACCESSORY, TEXT("CounterAttack_DungeonName3"));
	}
	if (UIP_DungeonSlot4.IsValid())
	{
		UIP_DungeonSlot4->SetDayType(b2network::B2CounterDungeon::TYPE::GOLD, TEXT("CounterAttack_DungeonName4"));
	}
	SetRewardInfo();
}