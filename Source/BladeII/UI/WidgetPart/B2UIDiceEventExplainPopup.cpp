// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDiceEventExplainPopup.h"

#include "B2AndroidBackManager.h"
#include "RewardEventManager.h"
#include "B2UIEventSlotHB.h"
#include "B2UIDiceEventFinishReward.h"
#include "B2UIDiceEventMission.h"


void UB2UIDiceEventExplainPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UScrollBox, SB_FinishRewardList);

	GET_SLOT(UScrollBox, SB_MissionList);

	GET_SLOT(UTextBlock, TB_FinishRewardTitle);
	GET_SLOT(UTextBlock, TB_MissionListTitle);
}

void UB2UIDiceEventExplainPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIDiceEventExplainPopup::OnClickBTN_Close);
}

void UB2UIDiceEventExplainPopup::UpdateStaticText()
{
	if (TB_FinishRewardTitle.IsValid())
	{
		//TB_FinishRewardTitle->SetText();
	}

	if (TB_MissionListTitle.IsValid())
	{
		//TB_MissionListTitle->SetText();
	}
}

void UB2UIDiceEventExplainPopup::CloseWidgetDelegate()
{
	ClosePopup();
}

void UB2UIDiceEventExplainPopup::OpenPopup()
{
	if (this->GetVisibility() != ESlateVisibility::Visible)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

void UB2UIDiceEventExplainPopup::ClosePopup()
{
	if (this->GetVisibility() != ESlateVisibility::Hidden)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIDiceEventExplainPopup::InitDiceEventExplainPopup(int32 iEventID)
{
	CachedEventID = iEventID;

	this->Init();
	this->AddToViewport(PopupZOrder_BP);

	InitDiceEventMasterData_Mission();
	InitDiceEventMasterData_FinishReward();
}

void UB2UIDiceEventExplainPopup::InitDiceEventMasterData_Mission()
{
	if (SB_MissionList.IsValid() == false)
	{
		return;
	}

	const TArray<b2network::B2mdDiceEventMission>* pArrayDiceMission = RewardEventManager::GetInstance().GetArrayEventDiceInfo_Mission(CachedEventID);

	if (pArrayDiceMission)
	{
		const int32 iArrayMax = pArrayDiceMission->Num();
		for (int32 i = 0; i < iArrayMax; ++i)
		{
			if (pArrayDiceMission->IsValidIndex(i) == false)
			{
				continue;
			}

			auto* pCreateEventMission = CreateWidget<UB2UIDiceEventMission>(GetOwningPlayer(), UIDiceEventMissionClass);
			pCreateEventMission->InitDiceEventMission((*pArrayDiceMission)[i]);
			SB_MissionList->AddChild(pCreateEventMission);
		}
	}
}

void UB2UIDiceEventExplainPopup::InitDiceEventMasterData_FinishReward()
{
	if (SB_FinishRewardList.IsValid() == false)
	{
		return;
	}

	ArrayFinishReward.Empty();

	const TArray<b2network::B2mdDiceEventFinishReward>* pArrayFinishReward = RewardEventManager::GetInstance().GetArrayEventDiceInfo_FinishReward(CachedEventID);

	if (pArrayFinishReward)
	{
		TWeakObjectPtr<UB2UIEventSlotHB> pSlotHB = nullptr;

		const int32 iArrayMax = pArrayFinishReward->Num();
		for (int32 i = 0; i < iArrayMax; ++i)
		{
			if (pArrayFinishReward->IsValidIndex(i) == false)
			{
				continue;
			}

			if (i % MAX_HorizontalFinishReward_BP == 0)
			{
				auto* pCreateSlotHB = CreateWidget<UB2UIEventSlotHB>(GetOwningPlayer(), UISlotHBClass);
				pCreateSlotHB->Init();
				if (UScrollBoxSlot* pScrollBoxSlot = Cast<UScrollBoxSlot>(SB_FinishRewardList->AddChild(pCreateSlotHB)))
				{
					pScrollBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				}
				pSlotHB = pCreateSlotHB;	
			}

			if (pSlotHB.IsValid())
			{
				auto* pCreateFinishReward = CreateWidget<UB2UIDiceEventFinishReward>(GetOwningPlayer(), UIDiceEventFinishRewardClass);
				pCreateFinishReward->InitDiceEventFinishReward((*pArrayFinishReward)[i]);
				pSlotHB->AddChildWidget(pCreateFinishReward);

				ArrayFinishReward.Add(pCreateFinishReward);
			}
		}
	}
}

void UB2UIDiceEventExplainPopup::OnClickBTN_Close()
{
	ClosePopup();
}

void UB2UIDiceEventExplainPopup::UpdateFinishRewardState(int32 iFinishCount)
{
	const int32 iArrayMax = ArrayFinishReward.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayFinishReward.IsValidIndex(i) == false)
		{
			continue;
		}

		ArrayFinishReward[i]->UpdateState(iFinishCount);
	}
}

