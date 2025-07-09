// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIPMIssionCompleteProgress.h"
#include "B2UIChapterStarRewardBox.h"
#include "BladeIILocText.h"
#include "B2UIPMissionPointRewardPreview.h"
#include "B2UIMission.h"


void UB2UIPMIssionCompleteProgress::Init()
{
	Super::Init();

	SubscribeEvent();
}

void UB2UIPMIssionCompleteProgress::DestroySelf(class UB2UIManager* InUIManager)
{
	UnSubscribeEvent();

	if (UIP_RewardBox1.IsValid())
	{
		UIP_RewardBox1->DestroySelf(InUIManager);
	}

	if (UIP_RewardBox2.IsValid())
	{
		UIP_RewardBox2->DestroySelf(InUIManager);
	}

	if (UIP_RewardBox3.IsValid()) 
	{
		UIP_RewardBox3->DestroySelf(InUIManager);
	}

	if (UIP_RewardPreview.IsValid())
	{
		UIP_RewardPreview->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIPMIssionCompleteProgress::UpdateInfo(FMissionTab& MissionTab, EMissionType MissionType)
{
	CP_CompleteProgress->SetVisibility(ESlateVisibility::Collapsed);

	SelectedMissionType = MissionType;

	if (MissionTab.IsCompleteProgress)
	{
		CP_ProgressParts->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CP_CompleteProgress->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (TB_CompleteProgress.IsValid())	// XX 업적 달성률 텍스트 표시
		{
			FText PrevString = BladeIIGetLOCText(B2LOC_CAT_GENERAL, MissionTab.TabTextKeyString);

			TB_CompleteProgress->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mission_Rate")), PrevString));
		}

		const int32 CurMissionPoint = MissionManager::GetInstance().GetMissionPoint(SelectedMissionType);
		int32 MaxMissionPoint = 0;

		MissionPointRewardPtr RewardPtr = nullptr;
		RewardPtr = MissionManager::GetInstance().GetMissionPointRewardInfo(SelectedMissionType, 1);
		UpdateRewardBoxInfo(nullptr, UIP_RewardBox1, RewardPtr);

		if (CurMissionPoint < RewardPtr->RequirePoint && MaxMissionPoint == 0)
		{
			MaxMissionPoint = RewardPtr->RequirePoint;
		}

		RewardPtr = MissionManager::GetInstance().GetMissionPointRewardInfo(SelectedMissionType, 2);
		UpdateRewardBoxInfo(UIP_RewardBox1, UIP_RewardBox2, RewardPtr);

		if (CurMissionPoint < RewardPtr->RequirePoint && MaxMissionPoint == 0)
		{
			MaxMissionPoint = RewardPtr->RequirePoint;
		}

		RewardPtr = MissionManager::GetInstance().GetMissionPointRewardInfo(SelectedMissionType, 3);
		UpdateRewardBoxInfo(UIP_RewardBox2, UIP_RewardBox3, RewardPtr);
		
		if (CurMissionPoint < RewardPtr->RequirePoint && MaxMissionPoint == 0)
		{
			MaxMissionPoint = RewardPtr->RequirePoint;
		}

		if (UIP_RewardBox2.IsValid())
		{
			// 1,3번 ProgressText 표시 안하고 2번에만 표시
			UIP_RewardBox2->SetForceProgressText(CurMissionPoint, MaxMissionPoint);
			UIP_RewardBox2->ShowProgressText(true);
		}

		UpdateInfoGetAllButton(SelectedMissionType);
	}
	
	if (MissionTab.bAllowedGetAllReward == true)
	{
		CP_CompleteProgress->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CP_ProgressParts->SetVisibility(ESlateVisibility::Hidden);
		UpdateInfoGetAllButton(SelectedMissionType);
	}
}


void UB2UIPMIssionCompleteProgress::UpdateInfoGetAllButton(EMissionType MissionType)
{
	if (BTN_GetAllReward.IsValid())
	{
		BTN_GetAllReward->SetVisibility(ESlateVisibility::Visible);
		BTN_GetAllReward->SetIsEnabled(MissionManager::GetInstance().IsReadyforMIssionReaward(MissionType));
	}
}

void UB2UIPMIssionCompleteProgress::OnReceiveMissionPointReward(const FB2ReceiveCompleteMissionReward& CompleteReward)
{
	//int32 RewardBoxIndex = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_reward_index_index, CompleteReward);

	//if (RewardBoxIndex == 1 && UIP_RewardBox1.IsValid())
	//{
	//	UIP_RewardBox1->SetRewarded(true);
	//}
	//else if (RewardBoxIndex == 2 && UIP_RewardBox2.IsValid())
	//{
	//	UIP_RewardBox2->SetRewarded(true);
	//}
	//else if (RewardBoxIndex == 3 && UIP_RewardBox3.IsValid())
	//{
	//	UIP_RewardBox3->SetRewarded(true);
	//}
}

void UB2UIPMIssionCompleteProgress::BindDelegates()
{
	if (UIP_RewardBox1.IsValid())
	{
		UIP_RewardBox1->RewardListDelgate.BindDynamic(this, &UB2UIPMIssionCompleteProgress::OnClickRewardBox1);
	}

	if (UIP_RewardBox2.IsValid())
	{
		UIP_RewardBox2->RewardListDelgate.BindDynamic(this, &UB2UIPMIssionCompleteProgress::OnClickRewardBox2);
	}

	if (UIP_RewardBox3.IsValid())
	{
		UIP_RewardBox3->RewardListDelgate.BindDynamic(this, &UB2UIPMIssionCompleteProgress::OnClickRewardBox3);
	}

	if (BTN_GetAllReward.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(BTN_GetAllReward, &UB2UIPMIssionCompleteProgress::OnClickGetAllReward);
	}
}

void UB2UIPMIssionCompleteProgress::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIChapterStarRewardBox, UIP_RewardBox1);
	if (UIP_RewardBox1.IsValid())
	{
		UIP_RewardBox1->Init();
	}
	
	GET_SLOT(UB2UIChapterStarRewardBox, UIP_RewardBox2);
	if (UIP_RewardBox2.IsValid())
	{
		UIP_RewardBox2->Init();
	}

	GET_SLOT(UB2UIChapterStarRewardBox, UIP_RewardBox3);
	if (UIP_RewardBox3.IsValid())
	{
		UIP_RewardBox3->Init();
	}

	GET_SLOT(UPanelWidget, CP_ProgressParts);
	GET_SLOT(UPanelWidget, CP_CompleteProgress);
	GET_SLOT(UTextBlock, TB_CompleteProgress);
	GET_SLOT(UB2UIPMissionPointRewardPreview, UIP_RewardPreview);
	if (UIP_RewardPreview.IsValid())
	{
		UIP_RewardPreview->Init();
		UIP_RewardPreview->SetPopupVisible(false);
	}

	GET_SLOT(UB2Button, BTN_GetAllReward);
}

void UB2UIPMIssionCompleteProgress::UpdateStaticText()
{
	if (TB_GetAllReward.IsValid())
	{
		TB_GetAllReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mission_GetAllReward")));
	}
}

void UB2UIPMIssionCompleteProgress::OnClickRewardBox(int32 RewardBoxIndex)
{
	MissionPointRewardPtr RewardPtr = MissionManager::GetInstance().GetMissionPointRewardInfo(SelectedMissionType, RewardBoxIndex);

	if (RewardPtr)
	{
		if (UIP_RewardPreview.IsValid())
		{
			bool bCanRequestReward = RewardPtr->bReached && !RewardPtr->bRewarded;
			UIP_RewardPreview->SetPopupVisible(true);
			UIP_RewardPreview->OpenPreview(RewardBoxIndex, bCanRequestReward, SelectedMissionType, RewardPtr->RequirePoint);
			UIP_RewardPreview->CreateRewardIconCommon(RewardPtr->RewardInfos);
		}
	}
}

void UB2UIPMIssionCompleteProgress::OnClickRewardBox1()
{
	OnClickRewardBox(1);
}

void UB2UIPMIssionCompleteProgress::OnClickRewardBox2()
{
	OnClickRewardBox(2);
}

void UB2UIPMIssionCompleteProgress::OnClickRewardBox3()
{
	OnClickRewardBox(3);
}

void UB2UIPMIssionCompleteProgress::OnClickGetAllReward()
{
	GetAllMIssionQuestRewardClass<>::GetInstance().Signal();
}

void UB2UIPMIssionCompleteProgress::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIPMIssionCompleteProgress);

	DeliveryReceiveCompleteMissionRewardTicket = DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveCompleteMissionReward, const FB2ReceiveCompleteMissionReward& CompleteReward)
		Capture->OnReceiveMissionPointReward(CompleteReward);
	END_CAPTURE_OBJECT()
		);
}

void UB2UIPMIssionCompleteProgress::UnSubscribeEvent()
{
	DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Unsubscribe(DeliveryReceiveCompleteMissionRewardTicket);
	DeliveryReceiveCompleteMissionRewardTicket = 0;
}

void UB2UIPMIssionCompleteProgress::UpdateRewardBoxInfo(TWeakObjectPtr<UB2UIChapterStarRewardBox> PrevWidget, TWeakObjectPtr<UB2UIChapterStarRewardBox> RewardWidget, MissionPointRewardPtr RewardPtr)
{
	if (RewardWidget.IsValid() && RewardPtr != nullptr)
	{
		const int32 CurrentPoint = MissionManager::GetInstance().GetMissionPoint(SelectedMissionType);
		const int32 PrevRequirePoint = (PrevWidget.IsValid()) ? PrevWidget->GetStarMaxNum() : 0;

		RewardWidget->SetRewardIndex(RewardPtr->RewardIndex);
		RewardWidget->SetMissionBox(CurrentPoint, RewardPtr->RequirePoint, RewardPtr->bRewarded);
		RewardWidget->ShowProgressText(false);	// ProgressBar만 보여주는걸로

		if (PrevWidget.IsValid() == false)	// 첫 Box면
		{
			RewardWidget->SetForceProgressBar(CurrentPoint, RewardPtr->RequirePoint);
		}
		else // 2,3번째
		{
			if (PrevWidget->GetGoalReachedState() == EUIChapterStarRewardState::NotReached)
			{
				RewardWidget->SetForceProgressBar(0, RewardPtr->RequirePoint);	// Progress Bar는 0부터로 표시해야함
			}
			else // 이전은 도달하고 현재 자신의 Progress 영역일 경우
			{
				const int32 ProgMaxPoint = FMath::Clamp<int32>(RewardPtr->RequirePoint - PrevRequirePoint, 0, RewardPtr->RequirePoint);
				const int32 ProgCurPoint = FMath::Clamp<int32>(CurrentPoint - PrevRequirePoint, 0, ProgMaxPoint);

				// Bar로 표시하는 백분률과 Text는 서로 다른 값이 들어간다. - 연속된 세개의 Progress Bar를 하나의 통합된 Bar로 보이기 위함
				RewardWidget->SetForceProgressBar(ProgCurPoint, ProgMaxPoint);
			}
		}
	}
}
