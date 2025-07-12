// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISeasonEvent.h"

#include "B2UIManager.h"
#include "B2UISeasonEventTab.h"
#include "B2UISeasonEventSlot.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "Retailer.h"
#include "B2RedDotManager.h"
#include "B2UIMailRewardPopUp.h"
#include "B2SeasonEventInfo.h"
#include "RewardEventManager.h"
#include "B2GameInstance.h"
#include "Blueprint/AsyncTaskDownloadImage.h"

const int32 iSEASONEVENT_MAINSLOT = 0;
const int32 iSEASONEVENT_POSSIBLE = 2;
const int32 iSEASONEVENT_COMPLETE = 3;

void UB2UISeasonEvent::Init()
{
	Super::Init();

	SubscribeEvent();

	InitEventIDSeasonEvent();
	InitSeasonEventMasterData();
	InitSeasonEventTabs();
	InitBannerImage();

	int32 iSelectedSeasonEventDay = GetSeasonEventProgressDay();
	UpdateSeasonEventState(iSelectedSeasonEventDay);

	data_trader::Retailer::GetInstance().RequestGetSeasonMission();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_SeasonEventMenu"));
}

void UB2UISeasonEvent::DestroySelf(UB2UIManager* InUIManager)
{
	UnSubscribeEvent();

	Super::DestroySelf(InUIManager);
}

void UB2UISeasonEvent::DoMarkRedDot()
{
	const int32 iSeasonEventTabListMax = ArrSeasonEventTab.Num();
	for (int32 i = 0; i < iSeasonEventTabListMax; ++i)
	{
		bool bCheckTabRedDot = false;

		if (ArrSeasonEventDayInfo.IsValidIndex(i))
		{
			const int32 iArrEventSlotInfoMax = ArrSeasonEventDayInfo[i].ArrSeasonEventSlotInfo.Num();
			for (int32 j = 0; j < iArrEventSlotInfoMax; ++j)
			{
				if (ArrSeasonEventDayInfo[i].ArrSeasonEventSlotInfo.IsValidIndex(j) &&
					ArrSeasonEventDayInfo[i].ArrSeasonEventSlotInfo[j].state == iSEASONEVENT_POSSIBLE)
				{
					bCheckTabRedDot = true;
					break;
				}
			}
		}

		if (ArrSeasonEventTab.IsValidIndex(i))
		{
			ArrSeasonEventTab[i]->SetCheckReddot(bCheckTabRedDot);
			ArrSeasonEventTab[i]->DoMarkRedDot();
		}
	}
}

void UB2UISeasonEvent::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, HB_SeasonEventTabList);

	GET_SLOT(UVerticalBox, VB_SeasonEventMainSlot);

	GET_SLOT(UVerticalBox, VB_SeasonEventSlotList);

	GET_SLOT(UB2Button, BTN_GetFinalReward);

	GET_SLOT(UTextBlock, TB_GetFinalReward);

	GET_SLOT(UTextBlock, TB_CompleteReward);

	GET_SLOT(UProgressBar, PB_Progress);

	GET_SLOT(UTextBlock, TB_ProgressCount);

	GET_SLOT(UTextBlock, TB_EventTitle);
	GET_SLOT(UTextBlock, TB_EventPeriod);

	GET_SLOT(UB2Image, IMG_Banner);
}

void UB2UISeasonEvent::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetFinalReward, &UB2UISeasonEvent::OnClickBTN_GetFinalReward);
}

void UB2UISeasonEvent::UpdateStaticText()
{
	if (TB_GetFinalReward.IsValid())
		TB_GetFinalReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));

	UpdateTitleAndPeriodText();
}

void UB2UISeasonEvent::SubscribeEvent()
{
	UnSubscribeEvent();

	//if (bSubscribed == false)
	//{
	//	bSubscribed = true;

	//	CAPTURE_UOBJECT(UB2UISeasonEvent);

	//	Issues.Add(SelectSeasonEventTabClass<int32>::GetInstance().Subscribe2(
	//		[Capture](int32 iSeasonEventDay)
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->UpdateSeasonEventState(iSeasonEventDay);
	//		}
	//	}
	//	));

	//	Issues.Add(SeasonEventGotoLinkSceneClass<int32, int32>::GetInstance().Subscribe2(
	//		[Capture](int32 iSeasonEventDay, int32 iSeasonEventSlotIndex)
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->GoToLinkScene(iSeasonEventDay, iSeasonEventSlotIndex);
	//		}
	//	}
	//	));

	//	Issues.Add(DeliveryGetSeasonMissionClass<FB2ResponseGetSeasonMissionPtr>::GetInstance().Subscribe2(
	//		[Capture](FB2ResponseGetSeasonMissionPtr GetSeasonMission)
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->ResponseGetSeasonMission(GetSeasonMission);
	//		}
	//	}
	//	));

	//	Issues.Add(DeliveryRewardSeasonMissionClass<FB2ResponseRewardSeasonMissionPtr>::GetInstance().Subscribe2(
	//		[Capture](FB2ResponseRewardSeasonMissionPtr RewardSeasonMission)
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->ResponseRewardSeasonMission(RewardSeasonMission);
	//		}
	//	}
	//	));
	//}
}

void UB2UISeasonEvent::UnSubscribeEvent()
{
	bSubscribed = false;

	Issues.Empty();
}

void UB2UISeasonEvent::InitEventIDSeasonEvent()
{
	SeasonEventID = RewardEventManager::GetInstance().GetEnableEventIDFromCategory(b2network::B2EventCategory::SEASON_MISSION);
}

void UB2UISeasonEvent::InitSeasonEventMasterData()
{
	//const TArray<b2network::B2mdSeasonMissionEvent>* pArrayEventRewardInfo = RewardEventManager::GetInstance().GetArrayEventRewardInfo_Season(SeasonEventID);
	//if (pArrayEventRewardInfo)
	//{
	//	int32 iSeasonEventSlotInfoMax = pArrayEventRewardInfo->Num();
	//	for (int32 i = 0; i < iSeasonEventSlotInfoMax; ++i)
	//	{
	//		if (pArrayEventRewardInfo->IsValidIndex(i) == false)
	//		{
	//			continue;
	//		}

	//		if (i == iSEASONEVENT_MAINSLOT)
	//		{
	//			FinalSeasonEventSlotInfo.RewardInfo = (*pArrayEventRewardInfo)[i];
	//		}
	//		else
	//		{
	//			int32 iSeasonEventDayInfoIndex = (*pArrayEventRewardInfo)[i].day - 1;

	//			if (!ArrSeasonEventDayInfo.IsValidIndex(iSeasonEventDayInfoIndex))
	//				ArrSeasonEventDayInfo.Add(tSeasonEventDayInfo());

	//			FSeasonEventSlotInfo EventSlotInfo;
	//			EventSlotInfo.RewardInfo = (*pArrayEventRewardInfo)[i];

	//			ArrSeasonEventDayInfo[iSeasonEventDayInfoIndex].ArrSeasonEventSlotInfo.Add(EventSlotInfo);
	//		}
	//	}
	//}
}

void UB2UISeasonEvent::InitSeasonEventTabs()
{
	if (!HB_SeasonEventTabList.IsValid())
		return;

	HB_SeasonEventTabList->ClearChildren();
	ArrSeasonEventTab.Empty();

	int32 iSeasonEventFinalDay = GetSeasonEventFinalDay();
	int32 iSeasonEventProgressDay = GetSeasonEventProgressDay();
	for (int32 i = 0; i < iSeasonEventFinalDay; ++i)
	{
		int32 iDay = i + 1;

		auto* CreatedSeasonEventTabPart = CreateWidget<UB2UISeasonEventTab>(GetOwningPlayer(), SeasonEventTabPartClass);
		CreatedSeasonEventTabPart->SetSeasonEventDay(iDay);
		CreatedSeasonEventTabPart->SetSeasonEventProgressDay(iSeasonEventProgressDay);
		CreatedSeasonEventTabPart->Init();

		ArrSeasonEventTab.Add(CreatedSeasonEventTabPart);

		if(HB_SeasonEventTabList.IsValid())
			HB_SeasonEventTabList->AddChild(CreatedSeasonEventTabPart);

		if (auto HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(CreatedSeasonEventTabPart->Slot))
			HorizontalBoxSlot->SetSize(FSlateChildSize());
	}
}

void UB2UISeasonEvent::InitBannerImage()
{
	if (const b2network::B2mdEventInfo* pEventInfo = RewardEventManager::GetInstance().GetEnableEventInfo(SeasonEventID))
	{
		strCahcedBannerUrl = pEventInfo->banner_url;

		ImageDownloader = NewObject<UAsyncTaskDownloadImage>(this, NAME_None, RF_Transient);

		if (ImageDownloader)
		{
			ImageDownloader->OnSuccess.AddDynamic(this, &UB2UISeasonEvent::OnSucessedDownloadBanner);
			ImageDownloader->OnFail.AddDynamic(this, &UB2UISeasonEvent::OnFailedDownloadBanner);
			ImageDownloader->Start(strCahcedBannerUrl);
		}
	}
}

void UB2UISeasonEvent::InitSeasonEventSlots(int32 iSelectedSeasonEventDay)
{
	if ( (!VB_SeasonEventMainSlot.IsValid()) || (!VB_SeasonEventSlotList.IsValid()) )
		return;

	VB_SeasonEventMainSlot->ClearChildren();
	VB_SeasonEventSlotList->ClearChildren();
	ArrSeasonEventSlot.Empty();

	int32 iCreateSeasonEventListMaxIndex = 1;

	const int32 iEventDayIndex = iSelectedSeasonEventDay - 1;
	if (ArrSeasonEventDayInfo.IsValidIndex(iEventDayIndex))
	{
		int32 iSeasonEventInfoIndex = ArrSeasonEventDayInfo[iEventDayIndex].ArrSeasonEventSlotInfo.Num() - 1;

		if (ArrSeasonEventDayInfo[iEventDayIndex].ArrSeasonEventSlotInfo.IsValidIndex(iSeasonEventInfoIndex))
			iCreateSeasonEventListMaxIndex = ArrSeasonEventDayInfo[iEventDayIndex].ArrSeasonEventSlotInfo[iSeasonEventInfoIndex].RewardInfo.index;
	}

	for (int32 i = 0; i < iCreateSeasonEventListMaxIndex; ++i)
	{
		auto* CreatedSeasonEventSlot = CreateWidget<UB2UISeasonEventSlot>(GetOwningPlayer(), SeasonEventSlotClass);
		CreatedSeasonEventSlot->Init();

		ArrSeasonEventSlot.Add(CreatedSeasonEventSlot);

		if (i == iSEASONEVENT_MAINSLOT && VB_SeasonEventMainSlot.IsValid())
			VB_SeasonEventMainSlot->AddChild(CreatedSeasonEventSlot);
		else if (VB_SeasonEventSlotList.IsValid())
			VB_SeasonEventSlotList->AddChild(CreatedSeasonEventSlot);
	}
}

void UB2UISeasonEvent::OnClickBTN_GetFinalReward()
{
	//if (FinalSeasonEventSlotInfo.state != iSEASONEVENT_POSSIBLE)
	//	return;

	//data_trader::Retailer::GetInstance().RequestRewardSeasonMission(
	//	FinalSeasonEventSlotInfo.RewardInfo.event_id, FinalSeasonEventSlotInfo.RewardInfo.day, FinalSeasonEventSlotInfo.RewardInfo.index);

	//FinalSeasonEventSlotInfo.state = iSEASONEVENT_COMPLETE;
}

bool UB2UISeasonEvent::CheckFinalEventSlot(b2network::B2SeasonMissionInfoPtr SeasonMission)
{
	bool bCheck = false;

	//시즌 이벤트 데이터를 같이 쓰는 관계로 날짜가 0이라면 최종 업적이라는 서버와의 약속
	if (SeasonMission && SeasonMission->day == 0)
		bCheck = true;

	return bCheck;
}

void UB2UISeasonEvent::OnSucessedDownloadBanner(class UTexture2DDynamic* pTexture)
{
	if (IMG_Banner.IsValid())
	{
		IMG_Banner->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (pTexture)
		{
			IMG_Banner->SetBrushFromTextureDynamic(pTexture);
		}
	}
}

void UB2UISeasonEvent::OnFailedDownloadBanner(class UTexture2DDynamic* pTexture)
{
	if (iTryCountDownBannerImage > 0)
	{
		iTryCountDownBannerImage -= 1;

		if (ImageDownloader)
		{
			ImageDownloader->Start(strCahcedBannerUrl);
		}
	}
}

void UB2UISeasonEvent::UpdateSeasonEventState(int32 iSelectedSeasonEventDay)
{
	iChacedLastSelectedSeasonEventDay = iSelectedSeasonEventDay;

	UpdateSeasonEventTabs(iSelectedSeasonEventDay);

	UpdateSeasonEventSlots(iSelectedSeasonEventDay);

	UpdateSeasonEventGetFinalReward();
}

void UB2UISeasonEvent::UpdateSeasonEventTabs(int32 iSelectedSeasonEventDay)
{
	const int32 iSeasonEventProgressDay = GetSeasonEventProgressDay();

	int32 iSeasonEventTabListMax = ArrSeasonEventTab.Num();

	for (int32 i = 0; i < iSeasonEventTabListMax; ++i)
	{
		if (ArrSeasonEventTab.IsValidIndex(i))
		{
			ArrSeasonEventTab[i]->SetSeasonEventProgressDay(iSeasonEventProgressDay);
			ArrSeasonEventTab[i]->UpdateSeasonEventTab(iSelectedSeasonEventDay);
		}
	}
}

void UB2UISeasonEvent::UpdateSeasonEventSlots(int32 iSelectedSeasonEventDay)
{
	InitSeasonEventSlots(iSelectedSeasonEventDay);

	int32 iDaySlotIndex = iSelectedSeasonEventDay - 1;

	if (ArrSeasonEventDayInfo.IsValidIndex(iDaySlotIndex))
	{
		const int32 iSeasonEventSlotMax = ArrSeasonEventSlot.Num();
		for (int32 i = 0; i < iSeasonEventSlotMax; ++i)
		{
			if (ArrSeasonEventSlot.IsValidIndex(i) && 
				ArrSeasonEventDayInfo[iDaySlotIndex].ArrSeasonEventSlotInfo.IsValidIndex(i))
			{
				const FSeasonEventSlotInfo& SeasonEventSlotInfo = ArrSeasonEventDayInfo[iDaySlotIndex].ArrSeasonEventSlotInfo[i];

				ArrSeasonEventSlot[i]->UpdateSeasonEventSlotInfo(SeasonEventSlotInfo);

				if (UB2SeasonEventInfo* SeasonEventInfoTable = StaticFindSeasonEventInfo())
				{
					FText fSeasonEventText = SeasonEventInfoTable->GetSeasonEventSlotText(SeasonEventSlotInfo.RewardInfo.day, SeasonEventSlotInfo.RewardInfo.index);
					ArrSeasonEventSlot[i]->SetSeasonEventSlotText(fSeasonEventText);
				}
			}
		}
	}
}

void UB2UISeasonEvent::UpdateSeasonEventGetFinalReward()
{
	//int32 iSeasonEventFinalDay = GetSeasonEventFinalDay();

	//if (TB_ProgressCount.IsValid())
	//{
	//	int32 DispalyCurrCount = FMath::Clamp<int32>(FinalSeasonEventSlotInfo.progress_count, 0, iSeasonEventFinalDay);
	//	FString ProgressString = FString::Printf(TEXT("(%d / %d)"), DispalyCurrCount, iSeasonEventFinalDay);
	//	TB_ProgressCount->SetText(FText::FromString(ProgressString));
	//	TB_ProgressCount->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//if (PB_Progress.IsValid())
	//{
	//	float ProgressRate = static_cast<float>(FinalSeasonEventSlotInfo.progress_count) / static_cast<float>(iSeasonEventFinalDay);
	//	PB_Progress->SetPercent(ProgressRate);
	//	PB_Progress->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//if (BTN_GetFinalReward.IsValid())
	//	BTN_GetFinalReward->SetVisibility(ESlateVisibility::Collapsed);

	//if (TB_CompleteReward.IsValid())
	//	TB_CompleteReward->SetVisibility(ESlateVisibility::Collapsed);

	//switch (FinalSeasonEventSlotInfo.state)
	//{
	//	case iSEASONEVENT_POSSIBLE:
	//	{
	//		if (BTN_GetFinalReward.IsValid())
	//			BTN_GetFinalReward->SetVisibility(ESlateVisibility::Visible);
	//	}break;
	//	case iSEASONEVENT_COMPLETE:
	//	{
	//		if (TB_CompleteReward.IsValid())
	//			TB_CompleteReward->SetVisibility(ESlateVisibility::Visible);
	//	}break;
	//	default: //progress
	//	{
	//		TB_ProgressCount->SetVisibility(ESlateVisibility::Visible);
	//		PB_Progress->SetVisibility(ESlateVisibility::Visible);
	//	}break;
	//}
}

void UB2UISeasonEvent::UpdateSeasonEventSlotState(b2network::B2SeasonMissionInfoPtr SeasonMission)
{
	//if (SeasonMission == nullptr)
	//	return;

	//int32 iSeasonEventDayInfoIndex = SeasonMission->day - 1;
	//int32 iSeasonEventSlotInfoIndex = SeasonMission->index - 1;

	//if (ArrSeasonEventDayInfo.IsValidIndex(iSeasonEventDayInfoIndex) &&
	//	ArrSeasonEventDayInfo[iSeasonEventDayInfoIndex].ArrSeasonEventSlotInfo.IsValidIndex(iSeasonEventSlotInfoIndex))
	//{
	//	ArrSeasonEventDayInfo[iSeasonEventDayInfoIndex].ArrSeasonEventSlotInfo[iSeasonEventSlotInfoIndex].progress_count = SeasonMission->progress_count;
	//	ArrSeasonEventDayInfo[iSeasonEventDayInfoIndex].ArrSeasonEventSlotInfo[iSeasonEventSlotInfoIndex].state = SeasonMission->state;
	//}

	//if (CheckFinalEventSlot(SeasonMission))
	//{
	//	FinalSeasonEventSlotInfo.state = SeasonMission->state;
	//	FinalSeasonEventSlotInfo.progress_count = SeasonMission->progress_count;
	//}
}

void UB2UISeasonEvent::UpdateTitleAndPeriodText()
{
	/*if (const b2network::B2mdEventInfo* pEventInfo = RewardEventManager::GetInstance().GetEnableEventInfoFromCategory(b2network::B2EventCategory::SEASON_MISSION))
	{
		if (TB_EventTitle.IsValid())
		{
			TB_EventTitle->SetText(FText::FromString(pEventInfo->title));
		}

		if (TB_EventPeriod.IsValid())
		{
			FDateTime StartTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(pEventInfo->start_time));
			FDateTime EndTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(pEventInfo->end_time));

			FText PeriodText = FText::FromString(
				FString::Printf(TEXT("%d.%d.%d ~ %d.%d.%d"),
				StartTime.GetYear(), StartTime.GetMonth(), StartTime.GetDay(),
				EndTime.GetYear(), EndTime.GetMonth(), EndTime.GetDay()
			));

			TB_EventPeriod->SetText(PeriodText);
		}
	}*/
}

void UB2UISeasonEvent::GoToLinkScene(int32 iSeasonEventDay, int32 iSeasonEventSlotIndex)
{
	//if (UB2SeasonEventInfo* SeasonEventInfoTable = StaticFindSeasonEventInfo())
	//	SeasonEventInfoTable->GotoLinkUIScene(iSeasonEventDay, iSeasonEventSlotIndex);
}

void UB2UISeasonEvent::ResponseGetSeasonMission(FB2ResponseGetSeasonMissionPtr GetSeasonMission)
{
	//ArrSeasonEventStatus.Empty();

	//for (auto EventStatus : GetSeasonMission->event_statuses)
	//{
	//	ArrSeasonEventStatus.Add(*EventStatus);
	//}

	//for (auto Mission : GetSeasonMission->missions)
	//{
	//	UpdateSeasonEventSlotState(Mission);
	//}

	//int32 iSelectedSeasonEventDay = GetSeasonEventProgressDay();
	//UpdateSeasonEventState(iSelectedSeasonEventDay);

	//FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_EVENT });
}

void UB2UISeasonEvent::ResponseRewardSeasonMission(FB2ResponseRewardSeasonMissionPtr RewardSeasonMission)
{
	//int32 iSelectedSeasonEventDay = 1;

	//if (auto& Mission = RewardSeasonMission->reward_mission)
	//{
	//	UpdateSeasonEventSlotState(Mission);
	//}

	//UpdateSeasonEventState(iChacedLastSelectedSeasonEventDay);

	//FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_EVENT });

	//UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);
}

int32 UB2UISeasonEvent::GetSeasonEventFinalDay()
{
	int32 iSeasonEventFinalDay = 0;

	//const TArray<b2network::B2mdSeasonMissionEvent>* pArrayEventRewardInfo = RewardEventManager::GetInstance().GetArrayEventRewardInfo_Season(SeasonEventID);
	//if (pArrayEventRewardInfo)
	//{
	//	int32 iLastSeasonEventSlotInfoIndex = pArrayEventRewardInfo->Num() - 1;

	//	if (pArrayEventRewardInfo->IsValidIndex(iLastSeasonEventSlotInfoIndex))
	//		iSeasonEventFinalDay = (*pArrayEventRewardInfo)[iLastSeasonEventSlotInfoIndex].day;
	//}

	return iSeasonEventFinalDay;
}

int32 UB2UISeasonEvent::GetSeasonEventSlotMaxIndex()
{
	int32 iSeasonEventSlotMaxIndex = 0;

	//const TArray<b2network::B2mdSeasonMissionEvent>* pArrayEventRewardInfo = RewardEventManager::GetInstance().GetArrayEventRewardInfo_Season(SeasonEventID);
	//if (pArrayEventRewardInfo)
	//{
	//	int32 iLastSeasonEventSlotInfoIndex = pArrayEventRewardInfo->Num() - 1;

	//	if (pArrayEventRewardInfo->IsValidIndex(iLastSeasonEventSlotInfoIndex))
	//		iSeasonEventSlotMaxIndex = (*pArrayEventRewardInfo)[iLastSeasonEventSlotInfoIndex].index;
	//}

	return iSeasonEventSlotMaxIndex;
}

int32 UB2UISeasonEvent::GetSeasonEventProgressDay()
{
	int32 iSeasonEventProgressDay = 0;
	int32 iLastProgressDay = 0;

	////ResponseAttendSeasonMission
	//const TArray<b2network::B2SeasonMissionEventStatus>* pArraySeasonEventStatus = RewardEventManager::GetInstance().GetArrayEventStatusInfo_Season(SeasonEventID);
	//if (pArraySeasonEventStatus)
	//{
	//	iLastProgressDay = pArraySeasonEventStatus->Num() - 1;
	//	if (pArraySeasonEventStatus->IsValidIndex(iLastProgressDay))
	//	{
	//		iSeasonEventProgressDay = (*pArraySeasonEventStatus)[iLastProgressDay].attendance_day;
	//	}
	//}

	////ResponseGetMission
	//iLastProgressDay = ArrSeasonEventStatus.Num() - 1;
	//if (ArrSeasonEventStatus.IsValidIndex(iLastProgressDay))
	//{
	//	if (ArrSeasonEventStatus[iLastProgressDay].attendance_day > iSeasonEventProgressDay)
	//		iSeasonEventProgressDay = ArrSeasonEventStatus[iLastProgressDay].attendance_day;
	//}

	int32 iSeasonFinalDay = GetSeasonEventFinalDay();

	return iSeasonFinalDay < iSeasonEventProgressDay ? iSeasonFinalDay : iSeasonEventProgressDay;
}