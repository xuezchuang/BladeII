// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationEvent.h"
#include "BladeIIUtil.h"
#include "B2UIDonationRankingScroll.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"
#include "B2GameInstance.h"
#include "B2UIDonationProductCost.h"
#include "Retailer.h"
#include "B2UIDonationPointStepReward.h"
#include "B2UIDonationPointStepRewardPopup.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "B2UIDonationRankingRewardPopup.h"
#include "Global.h"


void UB2UIDonationEvent::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_donation")));
}

void UB2UIDonationEvent::Init()
{
	Super::Init();

	InitUIP();
	InitDonationRemainingTimer();
	UpdateUIInfo();
	InitRankingRewardPopup();
}

void UB2UIDonationEvent::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	Issues.Empty();

	EndDonationTimer();
	EndRefreshTimer();

	if (UIP_RankingScroll.IsValid())
	{
		UIP_RankingScroll->DestroySelf(InUIManager);
	}

	if (UIP_OneSummonCost.IsValid())
	{
		UIP_OneSummonCost->DestroySelf(InUIManager);
	}

	if (UIP_TenSummonCost.IsValid())
	{
		UIP_TenSummonCost->DestroySelf(InUIManager);
	}

	for (int i = 0; i < UIP_StepRewards.Num(); i++)
	{
		if (UIP_StepRewards[i].IsValid())
		{
			UIP_StepRewards[i]->DestroySelf(InUIManager);
		}
	}

	if (UIP_DonationPointStepRewardPopup.IsValid())
	{
		if (UIP_DonationPointStepRewardPopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(UIP_DonationPointStepRewardPopup.Get());
		}
		UIP_DonationPointStepRewardPopup->DestroySelf(InUIManager);
	}

	if (UIP_DonationRankingRewardPopup.IsValid())
	{
		if (UIP_DonationRankingRewardPopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(UIP_DonationRankingRewardPopup.Get());
		}
		UIP_DonationRankingRewardPopup->DestroySelf(InUIManager);
	}
}

void UB2UIDonationEvent::CacheAssets()
{
	Super::CacheAssets();

	UIP_StepRewards.SetNum(9);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[0], UIP_DonationPointStepReward_0);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[1], UIP_DonationPointStepReward_1);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[2], UIP_DonationPointStepReward_2);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[3], UIP_DonationPointStepReward_3);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[4], UIP_DonationPointStepReward_4);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[5], UIP_DonationPointStepReward_5);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[6], UIP_DonationPointStepReward_6);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[7], UIP_DonationPointStepReward_7);
	GET_SLOT_BYNAME(UB2UIDonationPointStepReward, UIP_StepRewards[8], UIP_DonationPointStepReward_8);

	GET_SLOT(UB2RichTextBlock, X_RTB_RemainingText);
	GET_SLOT(UB2RichTextBlock, X_RTB_RemainingTime);
	GET_SLOT(UB2RichTextBlock, X_RTB_MyInfo);
	GET_SLOT(UB2RichTextBlock, X_RTB_MyRankingText);
	GET_SLOT(UB2RichTextBlock, X_RTB_MyRanking);
	GET_SLOT(UB2RichTextBlock, X_RTB_MyPointText);
	GET_SLOT(UB2RichTextBlock, X_RTB_MyPoint);
	GET_SLOT(UB2RichTextBlock, X_RTB_Refresh);
	GET_SLOT(UB2RichTextBlock, X_RTB_RewardList);
	GET_SLOT(UB2UIDonationRankingScroll, UIP_RankingScroll);
	GET_SLOT(UB2UIDonationProductCost, UIP_OneSummonCost);
	GET_SLOT(UB2UIDonationProductCost, UIP_TenSummonCost);
	GET_SLOT(UTextBlock, TB_TenSummon);
	GET_SLOT(UTextBlock, TB_OneSummon);
	GET_SLOT(UTextBlock, TB_GiftFreeGuide);
	GET_SLOT(UB2Button, BTN_TenSummon);
	GET_SLOT(UB2Button, BTN_OneSummon);
	GET_SLOT(UB2Button, BTN_Refresh);
	GET_SLOT(UB2Button, BTN_RewardList);
	GET_SLOT(UB2UIDonationPointStepRewardPopup, UIP_DonationPointStepRewardPopup);
	GET_SLOT(UCanvasPanel, CP_EventEnd);
	GET_SLOT(UB2RichTextBlock, X_RTB_EndEvent);
	GET_SLOT(UB2UIDonationRankingRewardPopup, UIP_DonationRankingRewardPopup);
}

void UB2UIDonationEvent::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (X_RTB_RemainingText.IsValid())
	{
		X_RTB_RemainingText->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_TimeTitle")));
	}

	if (X_RTB_MyInfo.IsValid())
	{
		X_RTB_MyInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserInfoTitle")));
	}

	if (X_RTB_MyRankingText.IsValid())
	{
		X_RTB_MyRankingText->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserRankTitle")));
	}

	if (X_RTB_MyPointText.IsValid())
	{
		X_RTB_MyPointText->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserPointTitle")));
	}

	if (X_RTB_Refresh.IsValid())
	{
		X_RTB_Refresh->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RefreshButton")));
	}

	if (X_RTB_RewardList.IsValid())
	{
		X_RTB_RewardList->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RewardInfoButton")));
	}

	if (TB_TenSummon.IsValid())
	{
		TB_TenSummon->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_TenDonation")));
	}

	if (TB_OneSummon.IsValid())
	{
		TB_OneSummon->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_OneDonation")));
	}

	if (TB_GiftFreeGuide.IsValid())
	{
		TB_GiftFreeGuide->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_GiftFreeGuide")));
	}

	if (X_RTB_EndEvent.IsValid())
	{
		X_RTB_EndEvent->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_CloseMsg")));
	}
}

void UB2UIDonationEvent::BindDelegates()
{
	Super::BindDelegates();

	//Issues.Add(UpdateDonationEventClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->UpdateUIInfo();
	//}
	//));

	//Issues.Add(OpenDonationPointStepRewardInfoPopupClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 InStep)
	//{
	//	this->OpenPopup(InStep);
	//}
	//));

	//Issues.Add(UpdateDonationPointRewardUIClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->UpdateBottomUIInfo();
	//}
	//));

	//BIND_CLICK_FUNC_TO_BTN(BTN_TenSummon, &UB2UIDonationEvent::OnClickTenSummon);
	//BIND_CLICK_FUNC_TO_BTN(BTN_OneSummon, &UB2UIDonationEvent::OnClickOneSummon);
	//BIND_CLICK_FUNC_TO_BTN(BTN_Refresh, &UB2UIDonationEvent::OnClickRefresh);
	//BIND_CLICK_FUNC_TO_BTN(BTN_RewardList, &UB2UIDonationEvent::OnClickRewardList);
}

void UB2UIDonationEvent::SetRemainingTime()
{
	if (X_RTB_RemainingTime.IsValid())
	{
		FDateTime DateNow = UB2GameInstance::GetLocalNow();
		FTimespan RemainingTime = EventEndTime - DateNow;

		if (RemainingTime > FTimespan(0))
		{
			FString StrDay = TEXT("-");
			FString StrHour = TEXT("-");
			FString StrMin = TEXT("-");
			FString StrSconds = TEXT("-");

			int32 RemainDay = RemainingTime.GetDays();
			if (RemainDay >= 0)
			{
				StrDay = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), FText::FromString(FString::FromInt(RemainDay))).ToString();
			}

			int32 RemainHour = RemainingTime.GetHours();
			if (RemainHour >= 0)
			{
				StrHour = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), FText::FromString(FString::FromInt(RemainHour))).ToString();
			}

			int32 RemainMinutes = RemainingTime.GetMinutes();
			if (RemainMinutes >= 0)
			{
				StrMin = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::FromString(FString::FromInt(RemainMinutes))).ToString();
			}

			int32 RemainScond = RemainingTime.GetSeconds();
			if (RemainScond >= 0)
			{
				StrSconds = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), FText::FromString(FString::FromInt(RemainScond))).ToString();
			}

			X_RTB_RemainingTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			X_RTB_RemainingTime->SetText(FText::FromString(FString::Printf(TEXT("%s %s %s %s"), *StrDay, *StrHour, *StrMin, *StrSconds)));
		}
		else
		{
			EndDonationTimer();
			UpdateUIInfo();
			X_RTB_RemainingTime->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIDonationEvent::UpdateUIInfo()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		int32 MyDonationPoint = DocDonation->GetDonationPoint();
		int32 MyRanking = DocDonation->GetMyRanking();
		int32 MinimumDonationPoint = DocDonation->GetMinimumDonationPoint();
		EDonationEventState DonationEventState = DocDonation->GetDonationEventState();
		switch (DonationEventState)
		{
		case EDonationEventState::Ongoing:
			if (X_RTB_MyRanking.IsValid())
			{
				if (MyDonationPoint >= MinimumDonationPoint)
				{
					X_RTB_MyRanking->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserRank")), MyRanking));
				}
				else
				{
					X_RTB_MyRanking->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_PointMinimumGuide")), MinimumDonationPoint));
				}
			}

			if (X_RTB_MyPoint.IsValid())
			{
				X_RTB_MyPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_UserPoint")), MyDonationPoint));
			}

			if (CP_EventEnd.IsValid())
			{
				CP_EventEnd->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (BTN_OneSummon.IsValid())
			{
				BTN_OneSummon->SetIsEnabled(true);
			}

			if (BTN_TenSummon.IsValid())
			{
				BTN_TenSummon->SetIsEnabled(true);
			}

			if (BTN_Refresh.IsValid() && IsRefresh() == false)
			{
				BTN_Refresh->SetIsEnabled(true);
			}

			break;
		default:
			if (CP_EventEnd.IsValid())
			{
				CP_EventEnd->SetVisibility(ESlateVisibility::Visible);
			}

			if (X_RTB_MyRanking.IsValid())
			{
				X_RTB_MyRanking->SetText(FText::FromString(TEXT("-")));
			}

			if (X_RTB_MyPoint.IsValid())
			{
				X_RTB_MyPoint->SetText(FText::FromString(TEXT("-")));
			}

			if (BTN_OneSummon.IsValid())
			{
				BTN_OneSummon->SetIsEnabled(false);
			}

			if (BTN_TenSummon.IsValid())
			{
				BTN_TenSummon->SetIsEnabled(false);
			}

			if (BTN_Refresh.IsValid())
			{
				BTN_Refresh->SetIsEnabled(false);
			}
			break;
		}

		if (UIP_RankingScroll.IsValid())
		{
			UIP_RankingScroll->UpdateScroll();
		}

		if (UIP_OneSummonCost.IsValid())
		{
			UIP_OneSummonCost->SetCost(DocDonation->GetOneCost(), DocDonation->GetFreeDonationCount());
		}
	}

	UpdateBottomUIInfo();
}

void UB2UIDonationEvent::UpdateBottomUIInfo()
{
	for (int i = 0; i < UIP_StepRewards.Num(); i++)
	{
		if (UIP_StepRewards[i].IsValid())
		{
			UIP_StepRewards[i]->UpdateInfo();
		}
	}
}

void UB2UIDonationEvent::OpenPopup(int32 InStep)
{
	if (UIP_DonationPointStepRewardPopup.IsValid())
	{
		UIP_DonationPointStepRewardPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_DonationPointStepRewardPopup.Get());
		UIP_DonationPointStepRewardPopup->SetInfo(InStep);
	}
}

void UB2UIDonationEvent::InitDonationRemainingTimer()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		EDonationEventState DonationEventState = DocDonation->GetDonationEventState();

		switch (DonationEventState)
		{
		case EDonationEventState::NotOngoing:
			if (X_RTB_RemainingTime.IsValid())
			{
				X_RTB_RemainingTime->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		case EDonationEventState::Ongoing:
			EventEndTime = DocDonation->GetEndTime();
			SetRemainingTime();
			EndDonationTimer();
			UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(RemainingTimeUpdate, this, &UB2UIDonationEvent::SetRemainingTime, 1.0f, true);
			break;
		default:
			break;
		}
	}
}

void UB2UIDonationEvent::EndDonationTimer()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(RemainingTimeUpdate);
}

void UB2UIDonationEvent::InitRefreshTimer()
{
	EndRefreshTimer();

	RefreshTime = MAXREFRESHTIME;

	if (BTN_Refresh.IsValid())
	{
		BTN_Refresh->SetIsEnabled(false);
	}

	if (X_RTB_Refresh.IsValid())
	{
		X_RTB_Refresh->SetText(FText::AsNumber(RefreshTime));
	}

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(RefreshTimer, this, &UB2UIDonationEvent::SetRefreshTime, 1.0f, true);
}

void UB2UIDonationEvent::EndRefreshTimer()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(RefreshTimer);
}

void UB2UIDonationEvent::SetRefreshTime()
{
	RefreshTime--;

	if (X_RTB_Refresh.IsValid())
	{
		X_RTB_Refresh->SetText(FText::AsNumber(RefreshTime));
	}

	if (IsRefresh() == false)
	{
		EndRefreshTimer();

		if (BTN_Refresh.IsValid())
		{
			BTN_Refresh->SetIsEnabled(true);
		}

		if (X_RTB_Refresh.IsValid())
		{
			X_RTB_Refresh->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("DonationEvent_RefreshButton")));
		}
	}
}

bool UB2UIDonationEvent::IsRefresh()
{
	return RefreshTime >= 1;
}

void UB2UIDonationEvent::InitUIP()
{
	if (UIP_RankingScroll.IsValid())
	{
		UIP_RankingScroll->Init();
	}

	if (UIP_OneSummonCost.IsValid())
	{
		UIP_OneSummonCost->Init();
	}

	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		if (UIP_TenSummonCost.IsValid())
		{
			UIP_TenSummonCost->Init();
			UIP_TenSummonCost->SetCost(DocDonation->GetTenCost());
		}
	}

	for (int i = 0; i < UIP_StepRewards.Num(); i++)
	{
		if (UIP_StepRewards[i].IsValid())
		{
			UIP_StepRewards[i]->Init();
		}
	}

	if (UIP_DonationPointStepRewardPopup.IsValid())
	{
		UIP_DonationPointStepRewardPopup->Init();
		UIP_DonationPointStepRewardPopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UIP_DonationRankingRewardPopup.IsValid())
	{
		UIP_DonationRankingRewardPopup->Init();
		UIP_DonationRankingRewardPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIDonationEvent::InitRankingRewardPopup()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		bool IsEnd = DocDonation->GetSettled();
		//捞 蜡历绰 捞亥飘俊 曼咯 沁澜?
		bool IsParticipation = false;
		if (GConfig)
		{
			GConfig->GetBool(TEXT("DonationEvent"), TEXT("Participation"), IsParticipation, GB2UserSavedStateIni);
		}

		if (IsEnd && IsParticipation)
		{
			int32 MyRanking = DocDonation->GetMyRanking();
			if (MyRanking <= DocDonation->GetScrollTotalRankerCount() && MyRanking >= 1)
			{
				if (UIP_DonationRankingRewardPopup.IsValid())
				{
					UIP_DonationRankingRewardPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_DonationRankingRewardPopup.Get());
					UIP_DonationRankingRewardPopup->SetInfo(MyRanking);

					//蜡历啊 捞亥飘 曼咯 茄芭 犬牢 窃
					if (GConfig)
					{
						GConfig->SetBool(TEXT("DonationEvent"), TEXT("Participation"), false, GB2UserSavedStateIni);
						GConfig->Flush(false, GB2UserSavedStateIni);
					}
				}
			}
		}
	}
}

void UB2UIDonationEvent::OnClickTenSummon()
{
	UB2UIDocDonation* DonationDoc = UB2UIDocHelper::GetDocDonation();
	if (DonationDoc)
	{
		int32 DonationID = DonationDoc->GetDonationID();
		data_trader::Retailer::GetInstance().RequestMakeDonation(DonationID, true, false, false, FString());
	}
}

void UB2UIDonationEvent::OnClickOneSummon()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		bool IsFree = DocDonation->GetFreeDonationCount() > 0 ? true : false;
		int32 DonationID = DocDonation->GetDonationID();
		data_trader::Retailer::GetInstance().RequestMakeDonation(DonationID, false, false, IsFree, FString());
	}
}

void UB2UIDonationEvent::OnClickRefresh()
{
	data_trader::Retailer::GetInstance().RequestGetDonationEvent();
	InitRefreshTimer();
}

void UB2UIDonationEvent::OnClickRewardList()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::DonationRewardInfoPopup);
}
