// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISettingMsgPopup.h"
#include "B2UIManager.h"

#include "B2UISettingAccount.h"
#include "B2UIRewardEventSet.h"
#include "B2UICombineMenu.h"
#include "B2UIMsgPopupGuestWarning.h"

UB2UISettingMsgPopup::UB2UISettingMsgPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_PopupType = EPopupTypeSetting::None;
}

void UB2UISettingMsgPopup::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Positive);
	GET_SLOT(UB2Button, BTN_Negative);
	GET_SLOT(UTextBlock, TXT_DownloadFileValue);

	GET_SLOT(UTextBlock, TXT_NetWorkFail);
	GET_SLOT(UTextBlock, TXT_FileError);
	GET_SLOT(UTextBlock, TXT_FileErrorSub);

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);
	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->Init();
		UIP_RewardIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UIP_RewardIcon->UpdateRewardIconByItemIndex(8000003, FText::AsNumber(100));
	}

	if (TXT_NetWorkFail.IsValid())
		TXT_NetWorkFail->SetVisibility(ESlateVisibility::Visible);
	if (TXT_FileError.IsValid())
		TXT_FileError->SetVisibility(ESlateVisibility::Hidden);
	if (TXT_FileErrorSub.IsValid())
		TXT_FileErrorSub->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UISettingMsgPopup::Init()
{
	Super::Init();
}

void UB2UISettingMsgPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISettingMsgPopup::SetOwnerSettingUI(class UB2UISettingAccount* ownerSetting)
{
	ownerSetting_ = ownerSetting;
}

void UB2UISettingMsgPopup::SetOwnerCombineUI(class UB2UICombineMenu* ownerCombine)
{
	ownerCombineMenu_ = ownerCombine;
}

void UB2UISettingMsgPopup::SetOwnerGuestWarningUI(class UB2UIMsgPopupGuestWarning* ownerGuestWarning)
{
	ownerGuestWarning_ = ownerGuestWarning;
}

void UB2UISettingMsgPopup::SetPopupType(EPopupTypeSetting eType)
{
	m_PopupType = eType;

	if (m_PopupType == EPopupTypeSetting::EPopupType_CheckPermission)
	{
		if (TXT_NetWorkFail.IsValid())
			TXT_NetWorkFail->SetVisibility(ESlateVisibility::Visible);
		if (TXT_FileError.IsValid())
			TXT_FileError->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_FileErrorSub.IsValid())
			TXT_FileErrorSub->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (TXT_NetWorkFail.IsValid())
			TXT_NetWorkFail->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_FileError.IsValid())
			TXT_FileError->SetVisibility(ESlateVisibility::Visible);
		if (TXT_FileErrorSub.IsValid())
			TXT_FileErrorSub->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UISettingMsgPopup::OnClickNegative()
{
	if (m_PopupType == EPopupTypeSetting::EPopupType_CombineKakaoLink)
		ownerCombineMenu_->KakaoConnect();
	else if (m_PopupType == EPopupTypeSetting::EPopupType_GuestWarningLink)
		ownerGuestWarning_->KakaoConnect();
	else
		ownerSetting_->KakaoConnect();
}

void UB2UISettingMsgPopup::OnClickKakaoExitBtn()
{
	if (m_PopupType == EPopupTypeSetting::EPopupType_CombineKakaoLink)
		ownerCombineMenu_->KakaoPopupHidden();
	else if (m_PopupType == EPopupTypeSetting::EPopupType_GuestWarningLink)
		ownerGuestWarning_->KakaoPopupHidden();
	else
		ownerSetting_->KakaoPopupHidden();
}

void UB2UISettingMsgPopup::SetRewardItem(int32 nitemID, int32 nCnt)
{
	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->UpdateRewardIconByItemIndex(nitemID, FText::AsNumber(nCnt));
	}
}
