// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIEventAttendance.h"
#include "B2UIConnectReward.h"
#include "B2UIAttendanceMain.h"
#include "B2UIEventNewUser.h"
#include "BladeIIGameMode.h"
#include "Retailer.h"
#include "B2UIAttendance.h"
#include "TutorialManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUICondition.h"
#include "BladeIIUtil.h"
#include "B2LobbyGameMode.h"

#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"

void UB2UIAttendanceMain::OnOpenComplete()
{
 	if (!ShouldShowCBTVersionContent() && BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().bIsAttendance)
	{
		if (UB2UIAttendance* Attendance = Cast<UB2UIAttendance>(m_AttendanceParts[GetIndex(EAttendanceTab::Attendance)].Get()))
		{
			Attendance->ReceiveReward(BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo());
		}
	}
	else if (RedDotCondition_EventNewUser())
	{
		SelectAttendanceTab(EAttendanceTab::EventNewUser, true);
	}
	else if (RedDotCondition_EventAttendance())
	{
		SelectAttendanceTab(EAttendanceTab::EventAttendance, true);
	}
	Super::OnOpenComplete();
}

void UB2UIAttendanceMain::OnCloseComplete()
{
	Super::OnCloseComplete();
}

void UB2UIAttendanceMain::CacheAssets()
{
	Super::CacheAssets();
	
	if (m_AttendanceTabs.Num() == 0)
	{
		m_AttendanceTabs.SetNum(static_cast<int32>(EAttendanceTab::Max));

		GET_SLOT_BYNAME(UPanelWidget, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Attendance)].P_ParentPanel, P_Attendance);
		GET_SLOT_BYNAME(UButton, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Attendance)].Button, BTN_Attendance);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Attendance)].SelectedImage, IMG_Attendance_Unselect);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Attendance)].UnselectedImage, IMG_Attendance_Select);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Attendance)].SelectedText, TXT_Attendance_Unselect);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Attendance)].UnselectedText, TXT_Attendance_Select);

		GET_SLOT_BYNAME(UPanelWidget, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::ConnectReward)].P_ParentPanel, P_ConnectReward);
		GET_SLOT_BYNAME(UButton, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::ConnectReward)].Button, BTN_ConnectReward);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::ConnectReward)].SelectedImage, IMG_ConnectReward_Unselect);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::ConnectReward)].UnselectedImage, IMG_ConnectReward_Select);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::ConnectReward)].SelectedText, TXT_ConnectReward_Unselect);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::ConnectReward)].UnselectedText, TXT_ConnectReward_Select);

		GET_SLOT_BYNAME(UPanelWidget, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventAttendance)].P_ParentPanel, P_EventAttendance);
		GET_SLOT_BYNAME(UButton, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventAttendance)].Button, BTN_EventAttendance);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventAttendance)].SelectedImage, IMG_EventAttendance_Unselect);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventAttendance)].UnselectedImage, IMG_EventAttendance_Select);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventAttendance)].SelectedText, TXT_EventAttendance_Unselect);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventAttendance)].UnselectedText, TXT_EventAttendance_Select);

		GET_SLOT_BYNAME(UPanelWidget, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventNewUser)].P_ParentPanel, P_NewUserEvent);
		GET_SLOT_BYNAME(UButton, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventNewUser)].Button, BTN_NewUserEvent);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventNewUser)].SelectedImage, IMG_NewUserEvent_Unselect);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventNewUser)].UnselectedImage, IMG_NewUserEvent_Select);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventNewUser)].SelectedText, TXT_NewUserEvent_Unselect);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::EventNewUser)].UnselectedText, TXT_NewUserEvent_Select);

		GET_SLOT_BYNAME(UPanelWidget, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Event_2)].P_ParentPanel, P_Event_2);
		GET_SLOT_BYNAME(UButton, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Event_2)].Button, BTN_Event_2);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Event_2)].SelectedImage, IMG_Event_2_Unselect);
		GET_SLOT_BYNAME(UImage, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Event_2)].UnselectedImage, IMG_Event_2_Select);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Event_2)].SelectedText, TXT_Event_2_Unselect);
		GET_SLOT_BYNAME(UTextBlock, m_AttendanceTabs[static_cast<int32>(EAttendanceTab::Event_2)].UnselectedText, TXT_Event_2_Select);
	}

	if (m_AttendanceParts.Num() == 0)
	{
		m_AttendanceParts.SetNum(static_cast<int32>(EAttendanceTab::Max));

		GET_SLOT_BYNAME(UB2UIAttendance, m_AttendanceParts[static_cast<int32>(EAttendanceTab::Attendance)], UIP_Attendance);
		GET_SLOT_BYNAME(UB2UIConnectReward, m_AttendanceParts[static_cast<int32>(EAttendanceTab::ConnectReward)], UIP_ConnectReward);
		GET_SLOT_BYNAME(UB2UIEventAttendance, m_AttendanceParts[static_cast<int32>(EAttendanceTab::EventAttendance)], UIP_EventAttendance);
		GET_SLOT_BYNAME(UB2UIEventNewUser, m_AttendanceParts[static_cast<int32>(EAttendanceTab::EventNewUser)], UIP_EventNewUser);
		GET_SLOT_BYNAME(UB2UIWidget, m_AttendanceParts[static_cast<int32>(EAttendanceTab::Event_2)], UIP_Event_2);
	}
	
	GET_SLOT(UOverlay, O_Tab_EventAttendance);
	GET_SLOT(UB2RichTextBlock, TB_Noti);
	GET_SLOT(UB2Button, BTN_Exit);
	GET_SLOT(UB2RichTextBlock, TB_Title);
}

void UB2UIAttendanceMain::UpdateStaticText()
{
	FText TextTemp;

	for (int32 i = 0; i < m_AttendanceTabs.Num(); i++)
	{
		TextTemp = FText::GetEmpty();
		if (m_AttendanceTabs[i].SelectedText.IsValid()
			&& m_AttendanceTabs[i].UnselectedText.IsValid())
		{
			if (i == static_cast<int32>(EAttendanceTab::Attendance)) {
				TextTemp = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Attend"));
			}
			else if (i == static_cast<int32>(EAttendanceTab::ConnectReward)) {
				TextTemp = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Attendance_Connect_Menu"));
			}
			else if (i == static_cast<int32>(EAttendanceTab::EventAttendance)) {
				TextTemp = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Attendance_Event_Menu"));
			}
			else if (i == static_cast<int32>(EAttendanceTab::EventNewUser)) {
				TextTemp = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Attendance_Beginner_Event_Menu"));
			}
			else if (i == static_cast<int32>(EAttendanceTab::Event_2)) {
				break;
				TextTemp = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Attend"));
			}

			m_AttendanceTabs[i].SelectedText->SetText(TextTemp);
			m_AttendanceTabs[i].UnselectedText->SetText(TextTemp);
		}
	}

	if (TB_Noti.IsValid()) {
		TB_Noti->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Attendance_Menu_Notice")));
	}

	if (TB_Title.IsValid())
	{
		bool IsReturnUser = BladeIIGameImpl::GetClientDataStore().IsReturnUser();
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsReturnUser ? TEXT("Attendance_ComeBackHeadLine") : TEXT("Attendance_HeadLine")));
	}
}

void UB2UIAttendanceMain::SetAttendanceTap()
{
	SelectAttendanceTab(EAttendanceTab::Attendance);
}

void UB2UIAttendanceMain::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIAttendanceMain::Init()
{
	Super::Init();

	SubscribeEvents();

	OpenEventAfterDaily = false;
	DelayForOpenEventTap = 0.0f;

	SetExitButtonEnable(true);
	SetExitButtonVisibility(true);
	SetAttandanceTabEnable(true);
	SetEnableTab(EAttendanceTab::Attendance, ShouldShowCBTVersionContent() ? false : true);
	SetEnableTab(EAttendanceTab::ConnectReward, true);

	if (auto* Conect = Cast<UB2UIEventAttendance>(m_AttendanceParts[GetIndex(EAttendanceTab::EventAttendance)].Get()))
	{
		SetEnableTab(EAttendanceTab::EventAttendance, Conect->IsEntirePeriod());
	}

	if (auto* EventNewAttendance = Cast<UB2UIEventNewUser>(m_AttendanceParts[GetIndex(EAttendanceTab::EventNewUser)].Get()))
	{
		/* 기획 요청이 출석을 다 받으면 게임을 재시작 하기전까지 탭을 활성화 시켜달라고 했기 때문에 즉시 닫기 위해서는
		FClientDataStore::SetNewUserAttendanceInfo 에서 IsNewUserAttendance을 수정
		*/
		// 접속 할 때 출석 가능일수가 있었는가(출석 창을 키는 시점이 아니라 접속시점인 이유는 기획의도)
		bool IsOpen = BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().IsNewUserAttendance;
		// 날짜가 지났는가?
		bool IsEndEvent = EventNewAttendance->IsEndEvent();

		SetEnableTab(EAttendanceTab::EventNewUser, IsOpen && !IsEndEvent);
	}

	for (int32 i = 0; i < m_AttendanceTabs.Num(); i++)
	{
		if (m_AttendanceParts[i].IsValid() 
			&& m_AttendanceTabs[i].P_ParentPanel.IsValid() && m_AttendanceTabs[i].P_ParentPanel->GetVisibility() != ESlateVisibility::Collapsed)
		{
			m_AttendanceParts[i].Get()->Init();
		}
	}

	static bool IsFirst = true;

	if (!IsFirst)
	{
		EAttendanceTab SelectedTab = EAttendanceTab::Attendance;
		if (auto DocUICondition = UB2UIDocHelper::GetDocUICondition())
		{
			if (DocUICondition->GetForcedOpenEventAttendance())
			{
				SelectedTab = EAttendanceTab::EventAttendance;
				DocUICondition->SetForcedOpenEventAttendance(false);
			}
		}
		SelectAttendanceTab(SelectedTab, true);
	}
	IsFirst = false;
}

void UB2UIAttendanceMain::DestroySelf(UB2UIManager* InUIManager)
{	
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
	UB2UIManager::GetInstance()->CloseUI(UIFName::RewardMailPopup);
}

void UB2UIAttendanceMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(m_AttendanceTabs[GetIndex(EAttendanceTab::Attendance)].P_ParentPanel.Get()), &UB2UIAttendanceMain::RedDotCondition_DailyAttendance);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(m_AttendanceTabs[GetIndex(EAttendanceTab::ConnectReward)].P_ParentPanel.Get()), &UB2UIAttendanceMain::RedDotCondition_ConnectReward);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(m_AttendanceTabs[GetIndex(EAttendanceTab::EventAttendance)].P_ParentPanel.Get()), &UB2UIAttendanceMain::RedDotCondition_EventAttendance);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(m_AttendanceTabs[GetIndex(EAttendanceTab::EventNewUser)].P_ParentPanel.Get()), &UB2UIAttendanceMain::RedDotCondition_EventNewUser);
}

void UB2UIAttendanceMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OpenEventAfterDaily)
	{
		DelayForOpenEventTap += InDeltaTime;
		if (DelayForOpenEventTap > 0.5f)
		{
			UB2UIManager::GetInstance()->CloseUI(UIFName::RewardMailPopup);

			SelectAttendanceTab(DelayOpenTap, true);
			DelayForOpenEventTap = 0.0f;
			OpenEventAfterDaily = false;
		}
	}
}

bool UB2UIAttendanceMain::RedDotCondition_AttendanceMain()
{
	bool IsNewConnectReward = RedDotCondition_ConnectReward();
	bool IsNewEventAttendance = RedDotCondition_EventAttendance();

	return RedDotCondition_DailyAttendance()
		|| IsNewConnectReward
		|| IsNewEventAttendance;
}

bool UB2UIAttendanceMain::RedDotCondition_DailyAttendance()
{
	return false;
}

bool UB2UIAttendanceMain::RedDotCondition_ConnectReward()
{
	if (auto* Conect = Cast<UB2UIConnectReward>(m_AttendanceParts[GetIndex(EAttendanceTab::ConnectReward)].Get()))
	{
		return Conect->IsNewReward();
	}
	return false;
}

bool UB2UIAttendanceMain::RedDotCondition_EventAttendance()
{
	if (auto* EventAttendance = Cast<UB2UIEventAttendance>(m_AttendanceParts[GetIndex(EAttendanceTab::EventAttendance)].Get()))
	{
		return EventAttendance->IsPeriod() && EventAttendance->IsNewReward();
	}
	return false;
}

bool UB2UIAttendanceMain::RedDotCondition_EventNewUser()
{
	if (auto* EventAttendance = Cast<UB2UIEventNewUser>(m_AttendanceParts[GetIndex(EAttendanceTab::EventNewUser)].Get()))
	{
		return EventAttendance->IsNewUserEvent();
	}
	return false;
}

bool UB2UIAttendanceMain::RedDotCondition_BySubMode(const EAttendanceTab& EMode)
{
	switch (EMode)
	{
	case EAttendanceTab::Attendance: return RedDotCondition_DailyAttendance();
	case EAttendanceTab::ConnectReward: return RedDotCondition_ConnectReward();
	case EAttendanceTab::EventAttendance: return RedDotCondition_EventAttendance();
	}

	return false;
}

void UB2UIAttendanceMain::SetDelayOpenEventTap(const EAttendanceTab& eIndex)
{
	OpenEventAfterDaily = true;
	DelayOpenTap = eIndex;
}

void UB2UIAttendanceMain::SelectAttendanceTab(EAttendanceTab eIndex, bool bForce /*=false*/)
{
	if (IsEnableAttendanceTab == false && bForce == false)
	{
		return;
	}

	int32 CurrentTab = static_cast<int32>(GetAutoChangeEnableTab(eIndex));

	for (int32 i = 0; i < m_AttendanceTabs.Num(); i++)
	{
		if (i == CurrentTab)
		{
			m_AttendanceTabs[i].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
			m_AttendanceTabs[i].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_AttendanceTabs[i].UnselectedText->SetVisibility(ESlateVisibility::Visible);
			m_AttendanceTabs[i].SelectedText->SetVisibility(ESlateVisibility::Hidden);
			m_AttendanceParts[i].Get()->SetVisibility(ESlateVisibility::Visible);

		}
		else
		{
			m_AttendanceTabs[i].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_AttendanceTabs[i].SelectedImage->SetVisibility(ESlateVisibility::Visible);
			m_AttendanceTabs[i].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
			m_AttendanceTabs[i].SelectedText->SetVisibility(ESlateVisibility::Visible);
			m_AttendanceParts[i].Get()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	UB2GameInstance::RequestUpdateDailyPlayTime(false);
	UB2UIManager::GetInstance()->CloseUI(UIFName::RewardMailPopup);
	SetExitButtonEnable(true);
	SetExitButtonVisibility(true);
	SetAttandanceTabEnable(true);
	DoMarkRedDot();
}

int32 UB2UIAttendanceMain::GetIndex(EAttendanceTab eIndex)
{
	if (eIndex < EAttendanceTab::Attendance || eIndex > EAttendanceTab::Max)
		return 0;
	return static_cast<int32>(eIndex);
}

void UB2UIAttendanceMain::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2UIAttendanceMain);

	ReceiveAttendanceExitEnableTicket = ReceiveAttendanceExitEnableClass<bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(ReceiveAttendanceExitEnable, bool Enabled)
		Capture->SetExitButtonEnable(Enabled);
		Capture->SetAttandanceTabEnable(Enabled);
	END_CAPTURE_OBJECT()
		);

	ReceiveAttendanceExitVisibilityTicket = ReceiveAttendanceExitVisibilityClass<bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(ReceiveAttendanceExitVisibility, bool InVisible)
		Capture->SetExitButtonVisibility(InVisible);
	END_CAPTURE_OBJECT()
		);
}

void UB2UIAttendanceMain::UnsubscribeEvents()
{
	ReceiveAttendanceExitEnableClass<bool>::GetInstance().Unsubscribe(ReceiveAttendanceExitEnableTicket);
	ReceiveAttendanceExitVisibilityClass<bool>::GetInstance().Unsubscribe(ReceiveAttendanceExitVisibilityTicket);
}

void UB2UIAttendanceMain::SetEnableTab(EAttendanceTab eIndex, bool Enable)
{
	auto Index = static_cast<int32>(eIndex);

	if (m_AttendanceTabs.IsValidIndex(Index) && m_AttendanceTabs[Index].P_ParentPanel.IsValid())
	{
		m_AttendanceTabs[Index].P_ParentPanel->SetVisibility(Enable ? ESlateVisibility::Visible  : ESlateVisibility::Collapsed);
		m_AttendanceTabs[Index].Button->SetIsEnabled(Enable);
	}

}

EAttendanceTab UB2UIAttendanceMain::GetAvailableTab() const
{
	int32 MaxCount = static_cast<int32>(EAttendanceTab::Max);

	for (int32 i = 0; i < MaxCount; ++i)
	{
		if (IsValidTab(static_cast<EAttendanceTab>(i)))
		{
			return static_cast<EAttendanceTab>(i);
		}
	}
	return EAttendanceTab::None;
}

bool UB2UIAttendanceMain::IsValidTab(EAttendanceTab eIndex) const
{
	auto Index = static_cast<int32>(eIndex);

	if (!m_AttendanceTabs.IsValidIndex(Index) || !m_AttendanceTabs[Index].P_ParentPanel.IsValid()
		|| m_AttendanceTabs[Index].P_ParentPanel->GetVisibility() == ESlateVisibility::Collapsed
		|| m_AttendanceTabs[Index].P_ParentPanel->GetVisibility() == ESlateVisibility::Hidden)
	{
		return false;
	}

	return true;
}

EAttendanceTab UB2UIAttendanceMain::GetAutoChangeEnableTab(EAttendanceTab eIndex) const
{
	// cbt용 조건 // CBT & 일반 출석일때는->이벤트 출석으로(이벤트 출석이 유효할때)
	if (ShouldShowCBTVersionContent() && eIndex == EAttendanceTab::Attendance && IsValidTab(EAttendanceTab::EventAttendance))
	{
		return EAttendanceTab::EventAttendance;
	}
	// eIndex체크 후 유효하지 않을때 현재 유효한탭을 GetAvailableTab()에서 반환
	return IsValidTab(eIndex) ? eIndex : GetAvailableTab();
}

void UB2UIAttendanceMain::SetExitButtonEnable(bool Enable)
{
	if (BTN_Exit.IsValid())
	{
		BTN_Exit->SetIsEnabled(Enable);
	}
}

void UB2UIAttendanceMain::SetExitButtonVisibility(bool InVisible)
{
	if (BTN_Exit.IsValid())
	{
		BTN_Exit->SetVisibility(InVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2UIAttendanceMain::SetAttandanceTabEnable(bool Enable)
{
	IsEnableAttendanceTab = Enable;
}

void UB2UIAttendanceMain::OnClickBTN_Attendance()
{
	SelectAttendanceTab(EAttendanceTab::Attendance);
}

void UB2UIAttendanceMain::OnClickBTN_ConnectReward()
{
	SelectAttendanceTab(EAttendanceTab::ConnectReward);
}

void UB2UIAttendanceMain::OnClickBTN_EventAttendance()
{
	SelectAttendanceTab(EAttendanceTab::EventAttendance);
}

void UB2UIAttendanceMain::OnClickBTN_NewUser()
{
	SelectAttendanceTab(EAttendanceTab::EventNewUser);
}

void UB2UIAttendanceMain::OnClickBTN_Event_2()
{
	SelectAttendanceTab(EAttendanceTab::Event_2);
}

void UB2UIAttendanceMain::OnClickBtn_Exit()
{
	UB2GameInstance::RequestUpdateDailyPlayTime();
	UB2UIManager::GetInstance()->CloseUI(UIFName::AttendanceMain);

	DoMarkCombineRedDotClass<>::GetInstance().Signal();
	LobbyNoticePopUpClass<int32>::GetInstance().Signal(1);

	CahcedCheckAndProgressTutorialClass<>::GetInstance().Signal();

	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
	{
		if(LobbyGM->IsPlayBeginAction())
			LobbyCharPlayMatineeClass<float>::GetInstance().Signal(0.01f);
	}
}


void UB2UIAttendanceMain::CloseWidgetDelegate()
{
	OnClickBtn_Exit();
}

