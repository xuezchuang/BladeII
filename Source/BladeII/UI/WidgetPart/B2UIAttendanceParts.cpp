// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIAttendanceParts.h"
#include "B2UIManager.h"

#include "B2UIAttendanceMain.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIMailRewardPopUp.h"
#include "B2UIAttendanceMsgPopup.h"
#include "BladeIIGameImpl.h"

void UB2UIAttendanceParts::CacheAssets()
{
	GET_SLOT(UB2Image, IMG_ITEM);
	GET_SLOT(UTextBlock, TXT_Day);
	GET_SLOT(UB2RichTextBlock, TXT_ItemName);
	GET_SLOT(UTextBlock, TXT_ItemCount);
	GET_SLOT(UB2Image, IMG_RewardDone);
	GET_SLOT(UPanelWidget, OV_RewardDone);
	GET_SLOT(UPanelWidget, P_BackGroundSpecial);

	Anim_Check = GetAnimation(this, "Anim_Check");
}

void UB2UIAttendanceParts::Init()
{
	Super::Init();

	if (P_BackGroundSpecial.IsValid())
	{
		if (IsSpecialDay)
		{
			P_BackGroundSpecial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		} 
		else
		{
			P_BackGroundSpecial->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIAttendanceParts::SetAttendanceInfo(FAttendanceDayData InAttendanceDayData)
{
	if (TXT_Day.IsValid())
	{
		TXT_Day->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendance_Day")), FText::FromString(FString::FromInt(InAttendanceDayData.nDay))));
	}

	if (TXT_ItemName.IsValid())
	{
		TXT_ItemName->SetText(BladeIIGameImpl::GetClientDataStore().GetRewardName(InAttendanceDayData.nRewardId));
	}

	if (TXT_ItemCount.IsValid())
	{
		TXT_ItemCount->SetText(FText::AsNumber(InAttendanceDayData.nRewardCount));
	}

	int32 IndexFromDay = BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().nAttendanceDay;
	
	if (OV_RewardDone.IsValid())
	{
		if (InAttendanceDayData.nDay < IndexFromDay)
		{
			OV_RewardDone->SetVisibility(ESlateVisibility::Visible);
		}
		else if (InAttendanceDayData.nDay == IndexFromDay)
		{
			if (BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().bIsAttendance)
			{
				OV_RewardDone->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				OV_RewardDone->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			OV_RewardDone->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (IMG_ITEM.IsValid())
	{
		IMG_ITEM->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(InAttendanceDayData.nRewardId));
	}
}

void UB2UIAttendanceParts::ReceiveReward(b2network::B2RewardPtr RewardItem)
{
	if (auto* PopUpReward = UB2UIManager::GetInstance()->OpenUI<UB2UIAttendanceMsgPopup>(UIFName::AttendanceMsgPopup))
	{
		ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(false);
		ReceiveAttendanceExitVisibilityClass<bool>::GetInstance().Signal(false);

		PopUpReward->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->StartRewardAnimation(); }));
	}
}

void UB2UIAttendanceParts::StartRewardAnimation()
{
	if (Anim_Check.IsValid())
	{
		BladeIIGameImpl::GetClientDataStore().SetEnableDailyReceive(false);
		PlayAnimation(Anim_Check.Get());

		if (OV_RewardDone.IsValid())
		{
			OV_RewardDone->SetVisibility(ESlateVisibility::Visible);
		}

		OnStartRewardAnimation_BP();
	}
}

void UB2UIAttendanceParts::FinishRecieveAnimation()
{
	if (auto* AttendanceMain = UB2UIManager::GetInstance()->GetUI<UB2UIAttendanceMain>(UIFName::AttendanceMain))
	{
		if(AttendanceMain->RedDotCondition_EventNewUser())
		{
			AttendanceMain->SetDelayOpenEventTap(EAttendanceTab::EventNewUser);
		}
		else if (AttendanceMain->RedDotCondition_EventAttendance())
		{
			AttendanceMain->SetDelayOpenEventTap(EAttendanceTab::EventAttendance);
		}
		else
		{
			ReceiveAttendanceExitEnableClass<bool>::GetInstance().Signal(true);
			ReceiveAttendanceExitVisibilityClass<bool>::GetInstance().Signal(true);
		}
	}
}
