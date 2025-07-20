// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventMain.h"

#include "B2UIManager.h"
#include "B2UIEventTapPart.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "Retailer.h"
#include "B2UIEventPage.h"
#include "B2GameInstance.h"

#include "RewardEventManager.h"

TSoftClassPtr<class UB2UIEventPage>* UB2UIEventPageInfo::FindEventPageClass(const int32 iEventCategory, const int32 iEventSubCategory)
{
	TSoftClassPtr<class UB2UIEventPage>* pEventPageSubClass = nullptr;

	EEVENT_CATEGORY eventcategory = (EEVENT_CATEGORY)iEventCategory;

	const int32 iArrayMax = TArrayEventPageData.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (TArrayEventPageData.IsValidIndex(i) == false)
		{
			continue;
		}

		if (TArrayEventPageData[i].eEventCategory == eventcategory &&
			TArrayEventPageData[i].iEventSubCategory == iEventSubCategory)
		{
			pEventPageSubClass = &TArrayEventPageData[i].EventPageSubClass;
			break;
		}
	}

	return pEventPageSubClass;
}

void UB2UIEventMain::Init()
{
	IsCreatedEventTabs = false;

	Super::Init();

	IsNoEnableEventPage(false);

	SubscribeEvents();

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_EventMenu"));
}

void UB2UIEventMain::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	int32 iEventTabMax = ArrayEventTab.Num();
	for (int32 i = 0; i < iEventTabMax; ++i)
	{
		if (ArrayEventTab.IsValidIndex(i))
		{
			if (ArrayEventTab[i].EventPage.IsValid())
			{
				ArrayEventTab[i].EventPage->DestroySelf(InUIManager);
			}
		}
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIEventMain::DoMarkRedDot()
{
	int32 iEventTabMax = ArrayEventTab.Num();
	for (int32 i = 0; i < iEventTabMax; ++i)
	{
		if (ArrayEventTab.IsValidIndex(i))
			ArrayEventTab[i].EventTabPart->DoMarkRedDot();
	}
}

void UB2UIEventMain::CacheAssets()
{
	Super::CacheAssets();

	//GET_SLOT(UVerticalBox, VB_TabList);
	//GET_SLOT(UCanvasPanel, P_EventPage);
	//GET_SLOT(UB2RichTextBlock, RTB_NoEnableEvent);
}

void UB2UIEventMain::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIEventMain::OnOpenComplete()
{
	Super::OnOpenComplete();

	RewardEventManager::GetInstance().RequestGetEventStatus();
}

void UB2UIEventMain::OnCloseComplete()
{
	Super::OnCloseComplete();
}

void UB2UIEventMain::OnSelectEventTap(int32 iEventID)
{
	SelectEventTap(iEventID);
}

void UB2UIEventMain::SubscribeEvents()
{
	UnsubscribeEvents();

	//if (bSubscribed == false)
	//{
	//	CAPTURE_UOBJECT(UB2UIEventMain);

	//	Issues.Add(SelectEventPageTabClass<int32>::GetInstance().Subscribe2(
	//		[Capture](int32 iEventID)
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->OnSelectEventTap(iEventID);
	//		}
	//	}
	//	));

	//	Issues.Add(GetEventStatusSuccessfulClass<>::GetInstance().Subscribe2(
	//		[Capture]()
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->UpdateEventMainPage();
	//		}
	//	}
	//	));

	//	Issues.Add(StratAnimEventMainTransparencyBlockClass<>::GetInstance().Subscribe2(
	//		[Capture]()
	//	{
	//		if (Capture.IsValid())
	//		{
	//			Capture->StartAnimTransparencyBlock_BP();
	//		}
	//	}
	//	));

	//	bSubscribed = true;
	//}
}

void UB2UIEventMain::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIEventMain::CreateEventTaps()
{
	if (VB_TabList.IsValid())
		VB_TabList->ClearChildren();

	if (P_EventPage.IsValid())
		P_EventPage->ClearChildren();

	const TArray<b2network::B2mdEventInfo> ArrayEnableEventInfo = RewardEventManager::GetInstance().GetArrayEnableEventPageInfo();

	int32 iBeginEventPageTapEventID = -1;

	const int32 iEnableEventInfoMax = ArrayEnableEventInfo.Num();
	for (int32 i = 0; i < iEnableEventInfoMax; ++i)
	{
		if (ArrayEnableEventInfo.IsValidIndex(i))
		{
			tEventTab tEventTab;
	
			auto* CreatedEventTabPart = CreateWidget<UB2UIEventTapPart>(GetOwningPlayer(), EventTapPartClass);
			CreatedEventTabPart->InitEventTap(ArrayEnableEventInfo[i]);
			VB_TabList->AddChild(CreatedEventTabPart);
			tEventTab.EventTabPart = CreatedEventTabPart;

			ArrayEventTab.Add(tEventTab);

			if (iBeginEventPageTapEventID == -1)
				iBeginEventPageTapEventID = ArrayEnableEventInfo[i].event_id;
		}
	}

	if(iBeginEventPageTapEventID != -1)
		SelectEventTap(iBeginEventPageTapEventID);

	IsCreatedEventTabs = true;
}

void UB2UIEventMain::SelectEventTap(int32 iEventID)
{
	RewardEventManager::GetInstance().RequestGetEventStatus();
	SelectedEventTabEventID = iEventID;

	int32 iEventTabMax = ArrayEventTab.Num();
	for (int32 i = 0; i < iEventTabMax; ++i)
	{
		bool bVisible = false;

		if(ArrayEventTab[i].EventTabPart->GetEventID() == iEventID)
			bVisible = true;

		SetVisibeEventTap(ArrayEventTab[i], bVisible);
	}
}

UB2UIEventPage* UB2UIEventMain::CreateDynamicEventPage(const TSoftClassPtr<UB2UIEventPage>* pCreateWidgetClass,
	UB2UIEventMain* InOwnerUserWidget, UCanvasPanel* CreateParentPanel, int32 iEventID)
{
	//if (CreateParentPanel && pCreateWidgetClass)
	//{
	//	UB2UIEventPage* NewCreatedEventPage = DynLoadAndCreateInCanvasPanelFull<UB2UIEventPage>(*pCreateWidgetClass, InOwnerUserWidget, CreateParentPanel);
	//	if (NewCreatedEventPage)
	//	{
	//		NewCreatedEventPage->InitEventPage(iEventID, RewardEventManager::GetInstance().GetCahcedGetEventStatus());
	//		return NewCreatedEventPage;
	//	}
	//}
	return nullptr;
}

void UB2UIEventMain::IsNoEnableEventPage(bool bVisibleNoEnableText)
{
	if(bVisibleNoEnableText)
	{
		if (RTB_NoEnableEvent.IsValid())
		{
			RTB_NoEnableEvent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RTB_NoEnableEvent->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("EventDefaultDesc")));
		}
	}
	else
	{
		if (RTB_NoEnableEvent.IsValid())
		{
			RTB_NoEnableEvent->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEventMain::UpdateEventMainPage()
{
	RewardEventManager& RewardEventMgr = RewardEventManager::GetInstance();

	if(RewardEventMgr.HasEnableEventPage())
	{
		if (IsCreatedEventTabs == false)
		{
			CreateEventTaps();
		}
		else if (IsCreatedEventTabs == true)
		{
			UpdateSelectedEventPage();
		}

		IsNoEnableEventPage(false);
	}
	else
	{
		IsNoEnableEventPage(true);
	}
}

void UB2UIEventMain::UpdateNoEnableEventText_BP()
{
	if (RewardEventManager::GetInstance().HasEnableEventPage())
	{
		IsNoEnableEventPage(false);
	}
	else
	{
		IsNoEnableEventPage(true);
	}
}

void UB2UIEventMain::SetVisibeEventTap(tEventTab& EventTab, bool bVisible)
{
	TWeakObjectPtr<class UB2UIEventTapPart> pEventTabPart = EventTab.EventTabPart;

	if (pEventTabPart.IsValid() == false)
		return;

	pEventTabPart->SetSelected(bVisible);

	if (bVisible == true && EventTab.EventPage == nullptr)
	{
		if (auto* pEventPageInfo = EventPageInfo->GetDefaultObject<UB2UIEventPageInfo>())
		{
			if (const TSoftClassPtr<class UB2UIEventPage>* pEventPageAssetSubClass =
				pEventPageInfo->FindEventPageClass(pEventTabPart->GetEventCategory(), pEventTabPart->GetEventSubCategory()))
			{
				EventTab.EventPage = CreateDynamicEventPage(pEventPageAssetSubClass, this, P_EventPage.Get(), pEventTabPart->GetEventID());
			}
		}
	}

	if (EventTab.EventPage.IsValid())
	{
		EventTab.EventPage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}	

void UB2UIEventMain::UpdateSelectedEventPage()
{
	int32 iEventTabMax = ArrayEventTab.Num();
	for (int32 i = 0; i < iEventTabMax; ++i)
	{
		if (ArrayEventTab[i].EventTabPart->GetEventID() == SelectedEventTabEventID)
		{
			if (ArrayEventTab[i].EventPage.IsValid())
			{
				ArrayEventTab[i].EventPage->UpdateEventPage(RewardEventManager::GetInstance().GetCahcedGetEventStatus());
				break;
			}
		}
	}
}
