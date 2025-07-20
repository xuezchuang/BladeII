// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHotTimeBuffPopup.h"
#include "HotTimeBuffManager.h"
#include "BladeIIUtil.h"
#include "B2UIHotTimeBuffPart.h"

#include "B2GameInstance.h"
#include "B2PlatformAPI.h"
#include "Event.h"

const FB2HotTimeBuffUIData* FB2HotTimeBuffUIData::GetData(const TArray<FB2HotTimeBuffUIData>& InBuffDatas, EHotTimeEventType InEventType, EHotTimeBuffType InBuffType)
{
	return InBuffDatas.FindByPredicate([InEventType, InBuffType](FB2HotTimeBuffUIData& UIData)
	{
		return UIData.EventType == InEventType && UIData.BuffType == InBuffType;
	});
}

void UB2UIHotTimeBuffPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Cancle);
	GET_SLOT(UTextBlock, TB_Title);

	GET_SLOT(UTextBlock, TB_HotTimePeriodTitle);
	GET_SLOT(UTextBlock, TB_HotTimePeriod);
	GET_SLOT(UTextBlock, TB_HotTimeEventEnd);

	GET_SLOT(UButton, BTN_HotTimeLink);
	GET_SLOT(UTextBlock, TB_HotTimeBuffDescription);
	GET_SLOT(UTextBlock, TB_HotTimeLinkDescription);
	GET_SLOT(UHorizontalBox, HB_PartBox)

	GET_SLOT(UB2UIHotTimeBuffPart, UIP_HottimeEventGold);
	GET_SLOT(UB2UIHotTimeBuffPart, UIP_HottimeEventExp);
	GET_SLOT(UB2UIHotTimeBuffPart, UIP_HottimeProductGold);
	GET_SLOT(UB2UIHotTimeBuffPart, UIP_HottimeProductExp);
}

void UB2UIHotTimeBuffPopup::CloseWidgetDelegate()
{
	CloseSelf();
}

void UB2UIHotTimeBuffPopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_PopupTitle")));

	bool ActiveHotTimeEventPeriod = HotTimeBuffManager::GetInstance().IsInHotTimeEventPeriod(UB2GameInstance::GetKoreaTimeNow());

	if (TB_HotTimePeriodTitle.IsValid())
	{
		TB_HotTimePeriodTitle->SetVisibility(ActiveHotTimeEventPeriod ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		TB_HotTimePeriodTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_PeriodTitle")));
	}

	if (TB_HotTimePeriod.IsValid())
	{
		if (ActiveHotTimeEventPeriod)
		{
			TB_HotTimePeriod->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			FEventInfo EventInfo;
			HotTimeBuffManager::GetInstance().GetCurrentEventInfo(UB2GameInstance::GetKoreaTimeNow(), EventInfo);

			FDateTime StartTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(EventInfo.start_time));
			FDateTime EndTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(EventInfo.end_time));

			TB_HotTimePeriod->SetText(FText::Format(FText::FromString(TEXT("{0} ~ {1}")),
				FText::FromString(StartTime.ToString(TEXT("%Y.%m.%d"))),
				FText::FromString(EndTime.ToString(TEXT("%Y.%m.%d"))))
			);
		}
		else
		{
			TB_HotTimePeriod->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (TB_HotTimeEventEnd.IsValid())
	{
		TB_HotTimeEventEnd->SetVisibility(ActiveHotTimeEventPeriod ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		TB_HotTimeEventEnd->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_EndDescription")));
	}

	if (TB_HotTimeBuffDescription.IsValid())
		TB_HotTimeBuffDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_BuffDescription")));

	if (TB_HotTimeLinkDescription.IsValid())
		TB_HotTimeLinkDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_EVENT, TEXT("HotTime_LinkDescription")));
}

void UB2UIHotTimeBuffPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancle, &UB2UIHotTimeBuffPopup::OnCancleBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_HotTimeLink, &UB2UIHotTimeBuffPopup::OnLinkBtnClick);
}

void UB2UIHotTimeBuffPopup::SetPartUIData(EHotTimeEventType InEventType, EHotTimeBuffType InBuffType)
{
	TWeakObjectPtr<UB2UIHotTimeBuffPart> CurrentPartUI;

	if (InEventType == EHotTimeEventType::Event && InBuffType == EHotTimeBuffType::Gold)
		CurrentPartUI = UIP_HottimeEventGold;
	if (InEventType == EHotTimeEventType::Event && InBuffType == EHotTimeBuffType::Exp)
		CurrentPartUI = UIP_HottimeEventExp;
	if (InEventType == EHotTimeEventType::Product && InBuffType == EHotTimeBuffType::Gold)
		CurrentPartUI = UIP_HottimeProductGold;
	if (InEventType == EHotTimeEventType::Product && InBuffType == EHotTimeBuffType::Exp)
		CurrentPartUI = UIP_HottimeProductExp;

	if (!CurrentPartUI.IsValid())
		return;

	const FB2HotTimeBuffUIData* UIData = FB2HotTimeBuffUIData::GetData(PartUIDatas, InEventType, InBuffType);
	if (UIData == NULL)
		return;

	CurrentPartUI->Init();
	CurrentPartUI->SetHotTimeData(InEventType, InBuffType, *UIData);
	HotTimeUIParts.Add(CurrentPartUI);
}

void UB2UIHotTimeBuffPopup::DestroySelf(UB2UIManager * InUIManager)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
		TheWorld->GetTimerManager().ClearTimer(UpdateTimeHandle);

	Issues.Empty();

	Super::DestroySelf(InUIManager);
}

void UB2UIHotTimeBuffPopup::Init()
{
	Super::Init();

	HotTimeUIParts.Empty();

	TSet<TPair<EHotTimeEventType, EHotTimeBuffType>> HotTimePartTypes;
	
	HotTimePartTypes.Add(TPair<EHotTimeEventType, EHotTimeBuffType>(EHotTimeEventType::Event, EHotTimeBuffType::Gold));
	HotTimePartTypes.Add(TPair<EHotTimeEventType, EHotTimeBuffType>(EHotTimeEventType::Event, EHotTimeBuffType::Exp));

	//product绰 公炼扒
	HotTimePartTypes.Add(TPair<EHotTimeEventType, EHotTimeBuffType>(EHotTimeEventType::Product, EHotTimeBuffType::Gold));
	HotTimePartTypes.Add(TPair<EHotTimeEventType, EHotTimeBuffType>(EHotTimeEventType::Product, EHotTimeBuffType::Exp));

	for (auto & PartType : HotTimePartTypes)
		SetPartUIData(PartType.Key, PartType.Value);

	//1檬窜困 盎脚 鸥捞赣 悸泼
	GetWorld()->GetTimerManager().SetTimer(UpdateTimeHandle, this, &UB2UIHotTimeBuffPopup::UpdateUIParts, 1.0f, true);


	Issues.Add(RefreshHotTimeUIClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateStaticText();
		this->UpdateUIParts();
	}));
}

void UB2UIHotTimeBuffPopup::OnOpen(bool RightNow)
{
	LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(true);
	Super::OnOpen(RightNow);
}

void UB2UIHotTimeBuffPopup::OnClose(bool RightNow)
{
	LobbyBlockMoveCameraClass<bool>::GetInstance().Signal(false);
	Super::OnClose(RightNow);
}


void UB2UIHotTimeBuffPopup::UpdateUIParts()
{
	for (auto Part : HotTimeUIParts)
	{
		if (Part.IsValid())
			Part->UpdateBuffUI();
	}
}

void UB2UIHotTimeBuffPopup::OnCancleBtnClick()
{
	CloseSelf();
}

void UB2UIHotTimeBuffPopup::OnLinkBtnClick()
{
	FEventInfo EventInfo;
	HotTimeBuffManager::GetInstance().GetCurrentEventInfo(UB2GameInstance::GetKoreaTimeNow(), EventInfo);
	B2P_ShowOpenURL(EventInfo.link_url);
}
