// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMission.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "MissionManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocSome.h"

#include "B2UIPMIssionCompleteProgress.h"
#include "B2UIMissionList.h"
#include "B2UIPMainQuest.h"
#include "B2UIPSubQuest.h"


#define TabUnSelected	0
#define TabSelected		1


void FMissionTab::SetTabSelected(bool bSelected)
{
	if (IMG_Tab[TabUnSelected].IsValid() && IMG_Tab[TabSelected].IsValid())
	{
		IMG_Tab[TabUnSelected]->SetVisibility(bSelected ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		IMG_Tab[TabSelected]->SetVisibility(bSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (TB_Tab[TabUnSelected].IsValid() && TB_Tab[TabSelected].IsValid())
	{
		TB_Tab[TabUnSelected]->SetVisibility(bSelected ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
		TB_Tab[TabSelected]->SetVisibility(bSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

UB2UIMission::UB2UIMission(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DeliveryGetMissionTicket = 0;
	DeliveryReceiveCompleteMissionRewardTicket = 0;
	SelectedTabType = ETabType::None;
}

void UB2UIMission::CacheAssets()
{
	Super::CacheAssets();

	int8 MaxTab = static_cast<int8>(ETabType::Max);

	Tabs.Add(ETabType::Daily);
	GET_SLOT_BYNAME(UButton, Tabs[ETabType::Daily].BTN_Tab, BTN_DailyTab);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Daily].TB_Tab[TabUnSelected], TB_DailyTab_UnSelected);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Daily].TB_Tab[TabSelected], TB_DailyTab_Selected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Daily].IMG_Tab[TabUnSelected], IMG_DailyTab_UnSelected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Daily].IMG_Tab[TabSelected], IMG_DailyTab_Selected);
	GET_SLOT_BYNAME(UB2UIWidgetBase, Tabs[ETabType::Daily].NewIcon, NewIcon_DailyNew);

	Tabs.Add(ETabType::Weekly);
	GET_SLOT_BYNAME(UButton, Tabs[ETabType::Weekly].BTN_Tab, BTN_WeeklyTab);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Weekly].TB_Tab[TabUnSelected], TB_WeeklyTab_UnSelected);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Weekly].TB_Tab[TabSelected], TB_WeeklyTab_Selected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Weekly].IMG_Tab[TabUnSelected], IMG_WeeklyTab_UnSelected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Weekly].IMG_Tab[TabSelected], IMG_WeeklyTab_Selected);
	GET_SLOT_BYNAME(UB2UIWidgetBase, Tabs[ETabType::Weekly].NewIcon, NewIcon_WeeklyNew);

	Tabs.Add(ETabType::Monthly);
	GET_SLOT_BYNAME(UButton, Tabs[ETabType::Monthly].BTN_Tab, BTN_MonthlyTab);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Monthly].TB_Tab[TabUnSelected], TB_MonthlyTab_UnSelected);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Monthly].TB_Tab[TabSelected], TB_MonthlyTab_Selected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Monthly].IMG_Tab[TabUnSelected], IMG_MonthlyTab_UnSelected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Monthly].IMG_Tab[TabSelected], IMG_MonthlyTab_Selected);
	GET_SLOT_BYNAME(UB2UIWidgetBase, Tabs[ETabType::Monthly].NewIcon, NewIcon_MonthlyNew);

	Tabs.Add(ETabType::Serial);
	GET_SLOT_BYNAME(UButton, Tabs[ETabType::Serial].BTN_Tab, BTN_SerialTab);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Serial].TB_Tab[TabUnSelected], TB_SerialTab_UnSelected);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::Serial].TB_Tab[TabSelected], TB_SerialTab_Selected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Serial].IMG_Tab[TabUnSelected], IMG_SerialTab_UnSelected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::Serial].IMG_Tab[TabSelected], IMG_SerialTab_Selected);
	GET_SLOT_BYNAME(UB2UIWidgetBase, Tabs[ETabType::Serial].NewIcon, NewIcon_SerialNew);

	Tabs.Add(ETabType::MainQuest);
	GET_SLOT_BYNAME(UButton, Tabs[ETabType::MainQuest].BTN_Tab, BTN_MainQuestTab);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::MainQuest].TB_Tab[TabUnSelected], TB_MainQuestTab_UnSelected);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::MainQuest].TB_Tab[TabSelected], TB_MainQuestTab_Selected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::MainQuest].IMG_Tab[TabUnSelected], IMG_MainQuestTab_UnSelected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::MainQuest].IMG_Tab[TabSelected], IMG_MainQuestTab_Selected);
	GET_SLOT_BYNAME(UB2UIWidgetBase, Tabs[ETabType::MainQuest].NewIcon, NewIcon_MainQuestNew);

	Tabs.Add(ETabType::SubQuest);
	GET_SLOT_BYNAME(UButton, Tabs[ETabType::SubQuest].BTN_Tab, BTN_SubQuestTab);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::SubQuest].TB_Tab[TabUnSelected], TB_SubQuestTab_UnSelected);
	GET_SLOT_BYNAME(UTextBlock, Tabs[ETabType::SubQuest].TB_Tab[TabSelected], TB_SubQuestTab_Selected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::SubQuest].IMG_Tab[TabUnSelected], IMG_SubQuestTab_UnSelected);
	GET_SLOT_BYNAME(UImage, Tabs[ETabType::SubQuest].IMG_Tab[TabSelected], IMG_SubQuestTab_Selected);
	GET_SLOT_BYNAME(UB2UIWidgetBase, Tabs[ETabType::SubQuest].NewIcon, NewIcon_SubQuestNew);

	for (TPair<ETabType, FMissionTab>& Tab : Tabs)
	{
		Tab.Value.SetMissionTabType(Tab.Key);
		Tab.Value.SetTabVisible(true);
		Tab.Value.SetTabNewIcon(false);
	}

	GET_SLOT(UOverlay, O_EventDeco);
	GET_SLOT(UB2UIMissionList, UIP_MissionList);
	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->Init();
	}

	GET_SLOT(UB2UIPMIssionCompleteProgress, UIP_MissionCompleteProgress);
	if (UIP_MissionCompleteProgress.IsValid())
	{
		UIP_MissionCompleteProgress->Init();
	}

	GET_SLOT(UB2UIPMainQuest, UIP_MainQuest);
	if (UIP_MainQuest.IsValid())
	{
		UIP_MainQuest->Init();
	}

	GET_SLOT(UB2UIPSubQuest, UIP_SubQuest);
	if (UIP_SubQuest.IsValid())
	{
		UIP_SubQuest->Init();
	}
}

void UB2UIMission::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(Tabs[ETabType::Daily].BTN_Tab, &UB2UIMission::OnClickDailyTab);
	BIND_CLICK_FUNC_TO_BTN(Tabs[ETabType::Weekly].BTN_Tab, &UB2UIMission::OnClickWeeklyTab);
	BIND_CLICK_FUNC_TO_BTN(Tabs[ETabType::Monthly].BTN_Tab, &UB2UIMission::OnClickMonthlyTab);
	BIND_CLICK_FUNC_TO_BTN(Tabs[ETabType::Serial].BTN_Tab, &UB2UIMission::OnClickSerialTab);
	BIND_CLICK_FUNC_TO_BTN(Tabs[ETabType::MainQuest].BTN_Tab, &UB2UIMission::OnClickMainQuestTab);
	BIND_CLICK_FUNC_TO_BTN(Tabs[ETabType::SubQuest].BTN_Tab, &UB2UIMission::OnClickSubQuestTab);
}

void UB2UIMission::Init()
{
	Super::Init();

	SubscribeEvent();

	ETabType InitMissionType = ETabType::Daily;
	UB2UIDocSome* SomeDoc = UB2UIDocHelper::GetDocSome();
	if (SomeDoc)
	{
		InitMissionType = ConvertTabType(static_cast<EMissionType>(SomeDoc->GetCompleteMissionType()));
		SomeDoc->SetCompleteMissionType(static_cast<int32>(EMissionType::Daily)); // Daily로 다시 초기화
	}

	ChangeTab(InitMissionType, true); // Default Daily로 초기 셋팅 - Mission 완료 창 바로가기로 왔다면 SomeDoc활용
	RefreshTabs();
}

void UB2UIMission::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Mission"));
}

void UB2UIMission::UpdateStaticText()
{
	for (TPair<ETabType, FMissionTab>& Tab : Tabs)
	{
		if (Tab.Value.TB_Tab[TabUnSelected].IsValid() && Tab.Value.TB_Tab[TabSelected].IsValid())
		{
			FText TabName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, Tab.Value.TabTextKeyString);

			Tab.Value.TB_Tab[TabUnSelected]->SetText(TabName);
			Tab.Value.TB_Tab[TabUnSelected]->SetVisibility(ESlateVisibility::HitTestInvisible);

			Tab.Value.TB_Tab[TabSelected]->SetText(TabName);
			Tab.Value.TB_Tab[TabSelected]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIMission::OnHistoryOpen(bool IsHistory)
{
	if (IsHistory)
	{
		data_trader::Retailer::GetInstance().RequestGetMission(0);
		data_trader::Retailer::GetInstance().RequestGetQuest();
		ShowMissionDialogClass<>::GetInstance().Signal();
	}
}

void UB2UIMission::DestroySelf(class UB2UIManager* InUIManager)
{
	UnSubscribeEvent();

	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->DestroySelf(InUIManager);
	}

	if (UIP_MissionCompleteProgress.IsValid())
	{
		UIP_MissionCompleteProgress->DestroySelf(InUIManager);
	}

	if (UIP_MainQuest.IsValid())
	{
		UIP_MainQuest->DestroySelf(InUIManager);
	}

	if (UIP_SubQuest.IsValid())
	{
		UIP_SubQuest->DestroySelf(InUIManager);
	}

	MissionManager::GetInstance().FlushCachedCompleteMission(EMissionType::Max);

	Super::DestroySelf(InUIManager);
}

void UB2UIMission::RefreshTabs()
{
	for (TPair<ETabType, FMissionTab>& Tab : Tabs)
	{
		bool bDisplayNew = false;
		
		EMissionType MissionType = ConvertMissionType(Tab.Value.Type);
		if (MissionType !=  EMissionType::None)
		{
			bDisplayNew = MissionManager::GetInstance().IsReadyforReaward(MissionType);
		}

		Tab.Value.SetTabNewIcon(bDisplayNew);	// Complete가 있다면 New표시
	}
}

void UB2UIMission::ChangeTab(ETabType InTabType, bool bScrollReset)
{
	if (Tabs.Contains(SelectedTabType))
	{
		Tabs[SelectedTabType].SetTabSelected(false);
	}
	if (Tabs.Contains(InTabType))
	{
		Tabs[InTabType].SetTabSelected(true);
	}

	SelectedTabType = InTabType;

	if (UIP_SubQuest.IsValid())
	{
		UIP_SubQuest->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UIP_MainQuest.IsValid())
	{
		UIP_MainQuest->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->SetVisibility(ESlateVisibility::Collapsed);
	}

	EMissionType MIssionType = ConvertMissionType(SelectedTabType);
	if (MIssionType != EMissionType::None)
	{
		if (UIP_MissionList.IsValid())
		{
			UIP_MissionList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_MissionList->UpdateMissionSlotList(MIssionType);

			if (bScrollReset)
			{
				UIP_MissionList->SetListScrollOffSet(0.f);
			}
		}

		if (Tabs.Contains(InTabType))
		{
			bool bHasCompleteMission = MissionManager::GetInstance().IsReadyforReaward(MIssionType);
			Tabs[InTabType].SetTabNewIcon(bHasCompleteMission);

			if (UIP_MissionCompleteProgress.IsValid())
			{
				UIP_MissionCompleteProgress->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UIP_MissionCompleteProgress->UpdateInfo(Tabs[InTabType], MIssionType);
			}
		}
	}
	else
	{
		if (UIP_MissionCompleteProgress.IsValid())
		{
			UIP_MissionCompleteProgress->SetVisibility(ESlateVisibility::Hidden);
		}

		if (SelectedTabType == ETabType::MainQuest)
		{
			if (UIP_MainQuest.IsValid())
			{
				UIP_MainQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
		else if (SelectedTabType == ETabType::SubQuest)
		{
			if (UIP_SubQuest.IsValid())
			{
				UIP_SubQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
}

void UB2UIMission::ChangeTapShortcut(EMissionType type)
{
	ETabType TabType = ConvertTabType(type);
	if (TabType != ETabType::None)
	{
		ChangeTab(TabType, true); // Default Daily로 초기 셋팅 - Mission 완료 창 바로가기로 왔다면 SomeDoc활용
		RefreshTabs();
	}
}

void UB2UIMission::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIMission);

	DeliveryGetMissionTicket = DeliveryGetMissionClass<FB2GetMission>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetMission, const FB2GetMission& MissionData)
		Capture->OnReceiveMissionData(MissionData);
		END_CAPTURE_OBJECT()
		);

	DeliveryReceiveCompleteMissionRewardTicket = DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveCompleteMissionReward, const FB2ReceiveCompleteMissionReward& CompleteReward)
		Capture->OnReceiveMissionPointReward(CompleteReward);
	END_CAPTURE_OBJECT()
		);

	MissionManager::GetInstance().OnChangedMissionReward.AddUObject(this, &UB2UIMission::OnChangedMissionReward);

	ChangeMissionTabClass<const EMissionType, const bool>::GetInstance().Subscribe([this](const EMissionType MissionType, const bool bScrollReset)
	{ 
		this->ChangeTab(this->ConvertTabType(MissionType), bScrollReset); 
		this->RefreshTabs();
	});
}

void UB2UIMission::UnSubscribeEvent()
{
	DeliveryGetMissionClass<FB2GetMission>::GetInstance().Unsubscribe(DeliveryGetMissionTicket);
	DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Unsubscribe(DeliveryReceiveCompleteMissionRewardTicket);
	DeliveryGetMissionTicket = 0;
	DeliveryReceiveCompleteMissionRewardTicket = 0;
	ChangeMissionTabClass<const EMissionType, const bool>::GetInstance().UnsubscribeAll();
	MissionManager::GetInstance().OnChangedMissionReward.RemoveAll(this);
}

void UB2UIMission::OnReceiveMissionData(const FB2GetMission& MissionData)
{
	if (SelectedTabType == ETabType::Serial && this->UIP_MissionList.IsValid() && this->UIP_MissionList->IsLoopReceiveSerialRewards())
	{
		// 일반 임무에서 모두받기가 진행 중일 경우 완료 여부 확인
		this->UIP_MissionList->CheckDoneReceiveSerialRewards();
		return;
	}
	ChangeTab(SelectedTabType, false); // 현재 Tab 갱신
}

void UB2UIMission::OnReceiveMissionPointReward(const FB2ReceiveCompleteMissionReward& CompleteReward)
{
	RefreshTabs();	// Tab NewIcon 갱신
}

void UB2UIMission::OnClickDailyTab()
{
	ChangeTab(ETabType::Daily, true);
	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->SetAllRewardButtonDisable(false);
	}
}

void UB2UIMission::OnClickWeeklyTab()
{
	ChangeTab(ETabType::Weekly, true);
	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->SetAllRewardButtonDisable(false);
	}
}

void UB2UIMission::OnClickMonthlyTab()
{
	ChangeTab(ETabType::Monthly, true);
	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->SetAllRewardButtonDisable(false);
	}
}

void UB2UIMission::OnClickSerialTab()
{
	ChangeTab(ETabType::Serial, true);
	if (UIP_MissionList.IsValid())
	{
		UIP_MissionList->SetAllRewardButtonDisable(false);
	}
}

void UB2UIMission::OnClickMainQuestTab()
{
	ChangeTab(ETabType::MainQuest, true);
	if (UIP_MainQuest.IsValid())
	{
		UIP_MainQuest->CurrentTabUpdateScroll();
	}
}

void UB2UIMission::OnClickSubQuestTab()
{
	ChangeTab(ETabType::SubQuest, true);
	if (UIP_SubQuest.IsValid())
	{
		UIP_SubQuest->CurrentTabUpdateScroll();
	}
}

void UB2UIMission::OnChangedMissionReward(const FB2ReceiveMissionReward & ReceiveMissionReward)
{
	ChangeTab(SelectedTabType, false);
}

EMissionType UB2UIMission::ConvertMissionType(ETabType InTabType)
{
	EMissionType Result = EMissionType::None;
	
	switch (InTabType)
	{
	case ETabType::Daily:
		Result = EMissionType::Daily;
		break;
	case ETabType::Weekly:
		Result = EMissionType::Weekly;
		break;
	case ETabType::Monthly:
		Result = EMissionType::Monthly;
		break;
	case ETabType::Serial:
		Result = EMissionType::Serial;
		break;
	default:
		break;
	}

	return Result;
}

ETabType UB2UIMission::ConvertTabType(EMissionType InMissionType)
{
	ETabType Result = ETabType::None;

	switch (InMissionType)
	{
	case EMissionType::Daily:
		Result = ETabType::Daily;
		break;
	case EMissionType::Weekly:
		Result = ETabType::Weekly;
		break;
	case EMissionType::Monthly:
		Result = ETabType::Monthly;
		break;
	case EMissionType::Serial:
		Result = ETabType::Serial;
		break;
	default:
		break;
	}

	return Result;
}
