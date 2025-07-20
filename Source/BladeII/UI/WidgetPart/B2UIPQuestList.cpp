// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPQuestList.h"
#include "B2UIPQuestSlot.h"
#include "QuestManager.h"

void UB2UIPQuestList::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (int i = 0; i < QuestSlots.Num(); i++)
	{
		if (QuestSlots[i].IsValid())
		{
			QuestSlots[i]->DestroySelf(InUIManager);
		}
	}
}

void UB2UIPQuestList::UpdateScroll(TArray<b2network::B2mdMainQuestInfoPtr>& QuestDatas, int32 CurrentQuestID)
{
	if (X_SB_QuestScrollBox.IsValid())
	{
		X_SB_QuestScrollBox->SetScrollOffset(0.0f);
	}

	struct QuestSlotInfo
	{
		b2network::B2mdMainQuestInfoPtr InfoPtr;
		EQuestSlotType SlotType;
		QuestSlotInfo() 
		{
			InfoPtr = nullptr;
			SlotType = EQuestSlotType::Complete;
		}

	};

	TArray<QuestSlotInfo> QuestList;
	TArray<QuestSlotInfo> TempInfo;

	for (b2network::B2mdMainQuestInfoPtr Elem : QuestDatas)
	{
		if (Elem->id == CurrentQuestID)
		{
			QuestSlotInfo SlotInfo;
			SlotInfo.InfoPtr = Elem;
			SlotInfo.SlotType = EQuestSlotType::Ongoing;
			
			QuestPtr QuestPtrInfo = QuestManager::GetInstance().GetQuest(Elem->main_quest_type);
			if (QuestPtrInfo != nullptr)
			{
				if (QuestPtrInfo->GetState() == EQS_COMPLETED)
				{
					SlotInfo.SlotType = EQuestSlotType::Reward;
				}
				else if (QuestPtrInfo->GetState() == EQS_REWARDED)
				{
					SlotInfo.SlotType = EQuestSlotType::Complete;
					TempInfo.Add(SlotInfo);
					continue;;
				}
			}
			
			QuestList.Add(SlotInfo);
			break;
		}
	}

	for (b2network::B2mdMainQuestInfoPtr Elem : QuestDatas)
	{
		if (Elem->id > CurrentQuestID)
		{
			QuestSlotInfo SlotInfo;
			SlotInfo.InfoPtr = Elem;
			SlotInfo.SlotType = EQuestSlotType::Lock;
			QuestList.Add(SlotInfo);
		}
	}

	for (b2network::B2mdMainQuestInfoPtr Elem : QuestDatas)
	{
		if (Elem->id < CurrentQuestID)
		{
			QuestSlotInfo SlotInfo;
			SlotInfo.InfoPtr = Elem;
			SlotInfo.SlotType = EQuestSlotType::Complete;
			QuestList.Add(SlotInfo);
		}
	}

	if (TempInfo.Num() > 0)
	{
		QuestList.Append(TempInfo);
	}

	int MaxCount = QuestList.Num();

	for (int i = 0; i < MaxCount; i++)
	{
		UB2UIPQuestSlot* NewItem;
		if (QuestSlots.IsValidIndex(i))
		{
			NewItem = QuestSlots[i].Get();
		}
		else
		{
			NewItem = CreateWidget<UB2UIPQuestSlot>(GetWorld(), QuestSlotClass);
			QuestSlots.Add(NewItem);
		}

		NewItem->SetVisibility(ESlateVisibility::Visible);
		NewItem->Init();
		NewItem->SetInfo(QuestList[i].InfoPtr, QuestList[i].SlotType);
		if (X_SB_QuestScrollBox.IsValid())
		{
			X_SB_QuestScrollBox->AddChild(NewItem);
		}
	}

	if (QuestSlots.Num() > MaxCount)
	{
		for (int i = MaxCount; i < QuestSlots.Num(); i++)
		{
			QuestSlots[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIPQuestList::CacheAssets()
{
	GET_SLOT(UB2ScrollBox, X_SB_QuestScrollBox);
}
