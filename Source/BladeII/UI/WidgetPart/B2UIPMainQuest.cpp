// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPMainQuest.h"
#include "QuestManager.h"
#include "B2UIPQuestList.h"
#include "B2ScrollBox.h"
#include "B2UIPMainQuestTab.h"
#include "BladeIIUtil.h"



void UB2UIPMainQuest::Init()
{
	Super::Init();

	UpdateMainQuestScrollByIndexClass<const int32>::GetInstance().Subscribe([this](const int32 InTabType)
	{
		this->UpdateScroll(InTabType);
	});

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	int32 MaxCount = 0;
	if (StageInfoTable)
	{
		MaxCount = StageInfoTable->GetEndAdventureModeChapterNum();
	}

	int32 CurrentActID = GetCurrentActID();

	for (int32 ActID = 1; ActID <= MaxCount; ActID++) // act는 1부터 시작
	{
		UB2UIPMainQuestTab* NewItem;
		NewItem = CreateWidget<UB2UIPMainQuestTab>(GetWorld(), QuestTabClass);
		NewItem->SetVisibility(ESlateVisibility::Visible);
		NewItem->Init();
		NewItem->Update(ActID, ActID > CurrentActID ? true : false, GetQuestID());
		NewItem->SetSelected(false);
		//NewItem->Padding = FMargin(0, 0, 10, 0);
		QuestTabs.Add(ActID, NewItem);
		
		if (SB_TabList.IsValid())
		{
			SB_TabList->AddChild(NewItem);
		}
	}
}

void UB2UIPMainQuest::DestroySelf(class UB2UIManager* InUIManager)
{
	for (TPair<int32, TWeakObjectPtr<UB2UIPMainQuestTab>>& Tab : QuestTabs)
	{
		if (Tab.Value.IsValid())
		{
			Tab.Value->DestroySelf(InUIManager);
		}
	}

	if (UIP_QuestList.IsValid())
	{
		UIP_QuestList->DestroySelf(InUIManager);
	}

	UpdateMainQuestScrollByIndexClass<const int32>::GetInstance().UnsubscribeAll();

	Super::DestroySelf(InUIManager);
}

void UB2UIPMainQuest::CurrentTabUpdateScroll()
{
	UpdateScroll(GetCurrentActID());
}

void UB2UIPMainQuest::UpdateScroll(int32 InTabType)
{
	if (UIP_QuestList.IsValid())
	{
		for (TPair<int32, TWeakObjectPtr<UB2UIPMainQuestTab>>& Tab : QuestTabs)
		{
			Tab.Value->SetSelected(false);
		}

		if (QuestTabs.Contains(InTabType) && QuestTabs[InTabType].IsValid())
		{
			QuestTabs[InTabType]->SetSelected(true);
		}

		TArray<b2network::B2mdMainQuestInfoPtr> QuestDatas;
		QuestManager::GetInstance().GetMainQuestMasterInfos(QuestDatas, InTabType);

		QuestPtr QuestInfoPtr = QuestManager::GetInstance().GetQuest(1);
		if(QuestInfoPtr != nullptr)
			UIP_QuestList->UpdateScroll(QuestDatas, QuestInfoPtr->GetQuestId());
	}
}

void UB2UIPMainQuest::CacheAssets()
{
	GET_SLOT(UB2UIPQuestList, UIP_QuestList);
	if (UIP_QuestList.IsValid())
	{
		UIP_QuestList->Init();
	}

	GET_SLOT(UB2ScrollBox, SB_TabList);
}

int32 UB2UIPMainQuest::GetCurrentActID()
{
	int32 Result = 0;

	QuestPtr QuestInfoPtr = QuestManager::GetInstance().GetQuest(1);
	if (QuestInfoPtr)
	{
		b2network::B2mdMainQuestInfoPtr MainQuestInfoPtr = QuestManager::GetInstance().GetMainQuestMasterInfo(QuestInfoPtr->GetQuestId());
		if (MainQuestInfoPtr)
		{
			Result = MainQuestInfoPtr->act_id;
		}
	}

	return Result;
}

int32 UB2UIPMainQuest::GetQuestID()
{
	int32 Result = 0;

	QuestPtr QuestInfoPtr = QuestManager::GetInstance().GetQuest(1);
	if (QuestInfoPtr)
	{
		Result =QuestInfoPtr->GetQuestId();
	}

	return Result;
}
