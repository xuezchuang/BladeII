#include "B2UIQuestNotice.h"

#include "B2UIManager.h"
#include "B2UIQuestSlot.h"
#include "B2UIQuestDialog.h"
#include "QuestManager.h"
#include "QuestDataTable.h"
#include "B2UIQuestNoticeSlot.h"



void UB2UIQuestNotice::Init()
{
	Super::Init();
}
void UB2UIQuestNotice::CacheAssets()
{
	NoticeSlots.AddUninitialized(3);
	GET_SLOT_BYNAME(UB2UIQuestNoticeSlot, NoticeSlots[0], UIP_QuestNoticeSlot1)
		GET_SLOT_BYNAME(UB2UIQuestNoticeSlot, NoticeSlots[1], UIP_QuestNoticeSlot2)
		GET_SLOT_BYNAME(UB2UIQuestNoticeSlot, NoticeSlots[2], UIP_QuestNoticeSlot3)
		for (auto NoticeSlot : NoticeSlots)
		{
			if (NoticeSlot.IsValid())
				NoticeSlot->Init();
		}

	GET_SLOT(UImage, IMG_Character)
		GET_SLOT(UButton, BTN_Close)

		GET_SLOT(UTextBlock, TB_QuestNPC)
		GET_SLOT(UTextBlock, TB_QuestNotice)
		GET_SLOT(UTextBlock, TB_QuestNoticeMent)

		GET_SLOT(UB2Button, BTN_AreaSlot1)
		GET_SLOT(UB2Button, BTN_AreaSlot2)
		GET_SLOT(UB2Button, BTN_AreaSlot3)

}
void UB2UIQuestNotice::UpdateStaticText()
{
	if (TB_QuestNotice.IsValid())
	{
		TB_QuestNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QuestNotice_Title")));
	}

	if (TB_QuestNoticeMent.IsValid())
	{
		TB_QuestNoticeMent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QuestNotice_Ment")));
	}
}

void UB2UIQuestNotice::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIQuestNotice::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_AreaSlot1, &UB2UIQuestNotice::OnClickBtnSlot1);
	BIND_CLICK_FUNC_TO_BTN(BTN_AreaSlot2, &UB2UIQuestNotice::OnClickBtnSlot2);
	BIND_CLICK_FUNC_TO_BTN(BTN_AreaSlot3, &UB2UIQuestNotice::OnClickBtnSlot3);
}

void UB2UIQuestNotice::OpenQuestDialog(int32 SlotIndex)
{
	if (NoticeSlots.IsValidIndex(SlotIndex) && NoticeSlots[SlotIndex].IsValid())
	{
		TWeakObjectPtr<UB2UIQuestSlot> QuestSlot = NoticeSlots[SlotIndex]->GetQuestSlot();
		if (QuestSlot != nullptr && QuestSlot->HasQuestInfo() == true)
		{
			auto* QuestDialogUI = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
			if (QuestDialogUI != nullptr && NoticeSlots[SlotIndex]->GetQuestSlot() != nullptr)
				QuestDialogUI->OpenDialogBySlotIndex(NoticeSlots[SlotIndex]->GetQuestSlot()->GetCurrentQuestSlot());
		}
	}
}

void UB2UIQuestNotice::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::QuestNotice);
}

void UB2UIQuestNotice::OnClickBtnSlot1()
{
	OpenQuestDialog(0);
}

void UB2UIQuestNotice::OnClickBtnSlot2()
{
	OpenQuestDialog(1);
}

void UB2UIQuestNotice::OnClickBtnSlot3()
{
	OpenQuestDialog(2);
}

TWeakObjectPtr<UB2UIQuestNoticeSlot> UB2UIQuestNotice::GetSuitableQuestSlot(int32& Outindex)
{
	int index = 0;
	for (auto SuitableSlot : NoticeSlots)
	{
		if (SuitableSlot.IsValid())
		{
			TWeakObjectPtr<UB2UIQuestSlot> QuestSlot = SuitableSlot->GetQuestSlot();
			if (QuestSlot != nullptr && QuestSlot->HasQuestInfo() == false)
			{
				Outindex = index;
				return SuitableSlot;
			}
		}
		index++;
	}

	return nullptr;
}

void UB2UIQuestNotice::InitQuestSlotNotice(const TArray<b2network::B2QuestInfoPtr>& QuestList)
{
	for (auto QuestObj : QuestList)
	{
		if (QuestObj != nullptr)
		{
			if (QuestObj->state == EQS_REWARDED)
				continue;

			int32 Index = 0;
			TWeakObjectPtr<UB2UIQuestNoticeSlot> SuitableSlot = GetSuitableQuestSlot(Index);
			if (SuitableSlot != nullptr && QuestObj != nullptr)
			{
				SuitableSlot->InitQuestNoticeSlot(QuestObj->slot, QuestObj->state);
				SuitableSlot->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}



#define MAX_QUESTCOMPLETE_NOTIFY	2

#define GET_SLOT_QUESTNOTIFY(SlotRef, Index)															\
	GET_SLOT_BYNAME(UCanvasPanel,		SlotRef.CP_QuestSlot, CP_QuestSlot##Index);						\
	GET_SLOT_BYNAME(UB2RichTextBlock,	SlotRef.RTB_QuestTitle, RTB_QuestTitle##Index);					\
	GET_SLOT_BYNAME(UTextBlock,			SlotRef.TB_QuestComplete, TB_QuestComplete##Index);				\
	GET_SLOT_BYNAME(UImage,				SlotRef.IMG_QuestCompleteIcon, IMG_QuestCompleteIcon##Index);	\

void UB2UIQuestCompleteNotify::CacheAssets()
{
	QuestCompleteNotiSlots.AddDefaulted(MAX_QUESTCOMPLETE_NOTIFY);
	GET_SLOT_QUESTNOTIFY(QuestCompleteNotiSlots[0], 1);
	GET_SLOT_QUESTNOTIFY(QuestCompleteNotiSlots[1], 2);

	for (auto& NotifySlot : QuestCompleteNotiSlots)
		NotifySlot.SetQuestCompleteNotify(false, 0);
	
}

void UB2UIQuestCompleteNotify::ShowCompleteQuestNotify(int32 QuestSlot)
{
	if (ProgressQuestSlots.IsEmpty() == true)
	{
		VisibleNotify(QuestSlot, true);
	}
	
	ProgressQuestSlots.Enqueue(QuestSlot);
}

void UB2UIQuestCompleteNotify::ActiveCloseTimer(float AutoCloseTime)
{
	if (GetOwningPlayer())
	{
		FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
		if (TimerManager.IsTimerActive(AutoClosetTimerHandle))
			TimerManager.ClearTimer(AutoClosetTimerHandle);

		TimerManager.SetTimer(AutoClosetTimerHandle, this, &UB2UIQuestCompleteNotify::CloseCompleteNotify, AutoCloseTime, false);
	}
}

void UB2UIQuestCompleteNotify::CloseCompleteNotify()
{
	if (GetOwningPlayer())
	{
		FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
		if (TimerManager.IsTimerActive(AutoClosetTimerHandle))
			TimerManager.ClearTimer(AutoClosetTimerHandle);
	}
	
	int32 QuestSlot = 0;
	if (ProgressQuestSlots.Dequeue(QuestSlot))
	{
		VisibleNotify(QuestSlot, false);
	}

	if (ProgressQuestSlots.IsEmpty() == false)
	{
		if (GetOwningPlayer())
		{//타이머
			FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
			if (TimerManager.IsTimerActive(AutoGapTimerHandle))
			{
				TimerManager.ClearTimer(AutoGapTimerHandle);
			}
			TimerManager.SetTimer(AutoGapTimerHandle, this, &UB2UIQuestCompleteNotify::OpenNotify, AutoGapTime, false);
		}
	}
	else
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::QuestCompleteNotify);
		QuestManager::GetInstance().FlushCachedCompleteSubQuests();
	}
}
	

void UB2UIQuestCompleteNotify::OpenNotify()
{
	if (GetOwningPlayer())
	{
		FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
		if (TimerManager.IsTimerActive(AutoGapTimerHandle))
			TimerManager.ClearTimer(AutoGapTimerHandle);
	}
	
	int32 QuestSlot;
	if (ProgressQuestSlots.Peek(QuestSlot))
	{
		VisibleNotify(QuestSlot, true);
	}
}

void UB2UIQuestCompleteNotify::VisibleNotify(int32 QuestSlot, bool IsVisible)
{
	int32 SlotIndex = INDEX_NONE;
	switch (QuestSlot)
	{
	case b2network::B2QuestSlotType::MAIN2: SlotIndex = 0; break;
	case b2network::B2QuestSlotType::SUB1: SlotIndex = 1; break;
	case b2network::B2QuestSlotType::SUB2: SlotIndex = 1; break;
	}

	if (QuestCompleteNotiSlots.IsValidIndex(SlotIndex))
	{
		FQuestNotifySlot& NotifySlot = QuestCompleteNotiSlots[SlotIndex];
		NotifySlot.SetQuestCompleteNotify(IsVisible, QuestSlot);

		if (IsVisible)
		{
			ActiveCloseTimer(NotifyAutoCloseTime);
		}
	}
}

void FQuestNotifySlot::SetQuestCompleteNotify(bool bVisible, int32 QuestSlot)
{
	if (CP_QuestSlot.IsValid())
		CP_QuestSlot->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (bVisible)
	{
		QuestPtr CompleteQuest = QuestManager::GetInstance().GetQuest(QuestSlot);

		UB2QuestInfoTable* QuestTable = StaticFindQuestTable();
		FQuestCommonInfo* QuestInfo = QuestTable != nullptr ? QuestTable->GetQuestInfo(CompleteQuest) : nullptr;
		if (QuestInfo)
		{
			FText OrgQuestText = BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestInfo->OutLine));

			TArray<FText> ParsedTextList;
			ParseTextFromSpecialChar(OrgQuestText, TEXT("/div"), ParsedTextList);

			if (RTB_QuestTitle.IsValid() && ParsedTextList.IsValidIndex(0))
			{
				RTB_QuestTitle->SetText(ParsedTextList[0]);
			}

			if (TB_QuestComplete.IsValid())
			{
				TB_QuestComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_CompleteNotifyDesc")));
			}

			// Icon은 어찌될지 모르므로..
		}
	}
}