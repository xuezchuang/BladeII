// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPSubQuest.h"
#include "B2UIPQuestList.h"
#include "B2UIPSubQuestTab.h"
#include "QuestManager.h"
#include "BladeIIGameImpl.h"

void UB2UIPSubQuest::Init()
{
	Super::Init();
	UpdateSubQuestScollClass<>::GetInstance().Subscribe([this]()
	{
		this->UpdateScroll(this->TabType);
	});
}

void UB2UIPSubQuest::DestroySelf(class UB2UIManager* InUIManager)
{
	UpdateSubQuestScollClass<>::GetInstance().UnsubscribeAll();

	for (TPair<int32, TWeakObjectPtr<UB2UIPSubQuestTab>>& Tab : Tabs)
	{
		if (Tab.Value.IsValid())
		{
			Tab.Value->DestroySelf(InUIManager);
		}
	}

	if(UIP_QuestList.IsValid())
	{
		UIP_QuestList->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIPSubQuest::CurrentTabUpdateScroll()
{
	UpdateScroll(GetCurrentDifficulty());
}

void UB2UIPSubQuest::UpdateScroll(int32 InTabType)
{//[TODO] 너무 많이 들어옴 ;;
	TabType = InTabType;

	for (TPair<int32, TWeakObjectPtr<UB2UIPSubQuestTab>>& Tab : Tabs)
	{
		Tab.Value->SetTabSelected(false);
	}

	if (Tabs.Contains(InTabType) && Tabs[InTabType].IsValid())
	{
		Tabs[InTabType]->SetTabSelected(true);
	}

	if (UIP_QuestList.IsValid())
	{
		QuestPtr QuestInfoPtr = QuestManager::GetInstance().GetQuest(2);
		TArray<b2network::B2mdMainQuestInfoPtr> QuestDatas;
		QuestManager::GetInstance().GetSubQuestMasterInfos(QuestDatas, InTabType);

		if(QuestInfoPtr != nullptr)
			UIP_QuestList->UpdateScroll(QuestDatas, QuestInfoPtr->GetQuestId());
	}
}

int32 UB2UIPSubQuest::GetCurrentDifficulty()
{
	int32 Result = 0;

	QuestPtr QuestInfoPtr = QuestManager::GetInstance().GetQuest(2);
	if (QuestInfoPtr)
	{
		b2network::B2mdMainQuestInfoPtr MainQuestInfoPtr = QuestManager::GetInstance().GetMainQuestMasterInfo(QuestInfoPtr->GetQuestId());
		if (MainQuestInfoPtr)
		{
			Result = MainQuestInfoPtr->difficulty;
		}
	}

	return Result;
}

void UB2UIPSubQuest::CacheAssets()
{
	GET_SLOT(UB2UIPQuestList, UIP_QuestList);
	if (UIP_QuestList.IsValid())
	{
		UIP_QuestList->Init();
	}

	Tabs.Add(b2network::B2StageDifficulty::NORMAL);
	GET_SLOT_BYNAME(UB2UIPSubQuestTab, Tabs[b2network::B2StageDifficulty::NORMAL], UIP_MissionTab_Normal);
	if (Tabs.Contains(b2network::B2StageDifficulty::NORMAL) && Tabs[b2network::B2StageDifficulty::NORMAL].IsValid())
	{
		Tabs[b2network::B2StageDifficulty::NORMAL]->Init();
		Tabs[b2network::B2StageDifficulty::NORMAL]->TabButtonDelegateBindLambda([this]() { this->UpdateScroll(b2network::B2StageDifficulty::NORMAL); });
		Tabs[b2network::B2StageDifficulty::NORMAL]->SetIcon(b2network::B2StageDifficulty::NORMAL);
	}

	Tabs.Add(b2network::B2StageDifficulty::HARD);
	GET_SLOT_BYNAME(UB2UIPSubQuestTab, Tabs[b2network::B2StageDifficulty::HARD], UIP_MissionTab_Hard);
	if (Tabs.Contains(b2network::B2StageDifficulty::HARD) && Tabs[b2network::B2StageDifficulty::HARD].IsValid())
	{
		Tabs[b2network::B2StageDifficulty::HARD]->Init();
		Tabs[b2network::B2StageDifficulty::HARD]->TabButtonDelegateBindLambda([this]() { this->UpdateScroll(b2network::B2StageDifficulty::HARD); });
		Tabs[b2network::B2StageDifficulty::HARD]->SetIcon(b2network::B2StageDifficulty::HARD);
	}

	Tabs.Add(b2network::B2StageDifficulty::VERY_HARD);
	GET_SLOT_BYNAME(UB2UIPSubQuestTab, Tabs[b2network::B2StageDifficulty::VERY_HARD], UIP_MissionTab_NIghtmare);
	if (Tabs.Contains(b2network::B2StageDifficulty::VERY_HARD) && Tabs[b2network::B2StageDifficulty::VERY_HARD].IsValid())
	{
		Tabs[b2network::B2StageDifficulty::VERY_HARD]->Init();
		Tabs[b2network::B2StageDifficulty::VERY_HARD]->TabButtonDelegateBindLambda([this]() { this->UpdateScroll(b2network::B2StageDifficulty::VERY_HARD); });
		Tabs[b2network::B2StageDifficulty::VERY_HARD]->SetIcon(b2network::B2StageDifficulty::VERY_HARD);
	}
	
	Tabs.Add(b2network::B2StageDifficulty::HELL);
	GET_SLOT_BYNAME(UB2UIPSubQuestTab, Tabs[b2network::B2StageDifficulty::HELL], UIP_MissionTab_HELL);
	if (Tabs.Contains(b2network::B2StageDifficulty::HELL) && Tabs[b2network::B2StageDifficulty::HELL].IsValid())
	{
		Tabs[b2network::B2StageDifficulty::HELL]->Init();
		Tabs[b2network::B2StageDifficulty::HELL]->TabButtonDelegateBindLambda([this]() { this->UpdateScroll(b2network::B2StageDifficulty::HELL); });
		Tabs[b2network::B2StageDifficulty::HELL]->SetIcon(b2network::B2StageDifficulty::HELL);
	}

	// [180712_YJ] 난이도 On/OFF기능 추가.
	// 튜토리얼 고려 안함.
	for (auto TabItem : Tabs)
	{
		bool bVisible = static_cast<int32>(TabItem.Key) <= BladeIIGameImpl::GetClientDataStore().GetLimitOpenStageDifficulty();
		if (TabItem.Value.IsValid())
			TabItem.Value->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}
