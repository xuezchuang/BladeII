// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIDLCFrontPopup.h"
#include "B2UIPatch.h"
#include "B2UIActionLogo.h"
#include "B2UISettingAccount.h"
#include "B2UIRewardEventSet.h"
#include "B2DLCFrontGameMode.h"


UB2UIDLCFrontPopup::UB2UIDLCFrontPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_PopupType = EPopupType::None;
}

void UB2UIDLCFrontPopup::CacheAssets()
{	
	GET_SLOT(UB2Button, BTN_Positive)
	GET_SLOT(UB2Button, BTN_Negative)
	GET_SLOT(UTextBlock, TXT_DownloadFileValue)

	GET_SLOT(UTextBlock, TXT_NetWorkFail)
	GET_SLOT(UTextBlock, TXT_FileError)
	GET_SLOT(UTextBlock, TXT_FileErrorSub)
	GET_SLOT(UTextBlock, TXT_NotEnoughDiskSize)

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
	if (TXT_NotEnoughDiskSize.IsValid())
		TXT_NotEnoughDiskSize->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIDLCFrontPopup::BindDelegates()
{
	Super::BindDelegates();

	/*if(BTN_Positive.IsValid())
		BIND_CLICK_FUNC_TO_BTN(BTN_Positive, &UB2UIDLCFrontPopup::OnClickPositive)

	if (BTN_Positive.IsValid())
		BIND_CLICK_FUNC_TO_BTN(BTN_Negative, &UB2UIDLCFrontPopup::OnClickNegative)
	*/
}

void UB2UIDLCFrontPopup::Init()
{
	Super::Init();
}

void  UB2UIDLCFrontPopup::SetOwnerUI(UB2UIPatch* owner)
{
	owner_ = owner;
}

void UB2UIDLCFrontPopup::SetOwnerLogoUI(class  UB2UIActionLogo* ownerLogo)
{
	ownerLogo_ = ownerLogo;
}

void UB2UIDLCFrontPopup::SetOwnerSettingUI(class  UB2UISettingAccount* ownerSetting)
{
	ownerSetting_ = ownerSetting;
}

void UB2UIDLCFrontPopup::SetPopupType(EPopupType eType)
{
	m_PopupType = eType;

	if (m_PopupType == EPopupType::EPopupType_CheckPermission)
	{
		if (TXT_NetWorkFail.IsValid())
			TXT_NetWorkFail->SetVisibility(ESlateVisibility::Visible);
		if (TXT_FileError.IsValid())
			TXT_FileError->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_FileErrorSub.IsValid())
			TXT_FileErrorSub->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_NotEnoughDiskSize.IsValid())
			TXT_NotEnoughDiskSize->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (m_PopupType == EPopupType::EPopupType_NotEnoughDiskSpace)
	{
		if (TXT_NetWorkFail.IsValid())
			TXT_NetWorkFail->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_FileError.IsValid())
			TXT_FileError->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_FileErrorSub.IsValid())
			TXT_FileErrorSub->SetVisibility(ESlateVisibility::Visible);
		if (TXT_NotEnoughDiskSize.IsValid())
			TXT_NotEnoughDiskSize->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (TXT_NetWorkFail.IsValid())
			TXT_NetWorkFail->SetVisibility(ESlateVisibility::Hidden);
		if (TXT_FileError.IsValid())
			TXT_FileError->SetVisibility(ESlateVisibility::Visible);
		if (TXT_FileErrorSub.IsValid())
			TXT_FileErrorSub->SetVisibility(ESlateVisibility::Visible);
		if (TXT_NotEnoughDiskSize.IsValid())
			TXT_NotEnoughDiskSize->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIDLCFrontPopup::OnClickNegative()
{
	if (m_PopupType == EPopupType::EPopupType_DownloadVolume)
	{
		owner_->PatchStart();
	}
	else if (m_PopupType == EPopupType::EPopupType_CheckPermission)
	{
		owner_->RetryPlatformPermission();
	}
	else if (m_PopupType == EPopupType::EPopupType_NotEnoughDiskSpace)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
#if PLATFORM_IOS
	else if (m_PopupType ==  EPopupType::EPopupType_ErrorRespones)
	{
		owner_->RetryRequest();
	}
#endif
	else
	{
		ownerSetting_->KakaoConnect();
	}
}

void  UB2UIDLCFrontPopup::SetDownLoadVolume(int64 nVolume)
{
	if (TXT_DownloadFileValue.IsValid())
	{
		TXT_DownloadFileValue->SetText(FText::FromString(FString::Printf(TEXT("%dMB"), nVolume)));
	}
}

void  UB2UIDLCFrontPopup::SetResponsCode(int32 nResponsCode)
{	
	UE_LOG(LogClass, Log, TEXT("####agwCheck SetPopupType[[ %d]]!!!!!!!!!!!!!!"), nResponsCode);
	if (TXT_FileErrorSub.IsValid())
	{
		UE_LOG(LogClass, Log, TEXT("####agwCheck SetPopupType[[ %d]]]2222222222222222222222222222"), nResponsCode);
		TXT_FileErrorSub->SetVisibility(ESlateVisibility::Visible);
		TXT_FileErrorSub->SetText(FText::FromString(FString::Printf(TEXT("( ErrorCode : %d )"), nResponsCode)));
	}
}

void UB2UIDLCFrontPopup::SetNeedDiskSize(FString DiskSize)
{
	if (TXT_FileErrorSub.IsValid())
	{
		UE_LOG(LogClass, Log, TEXT("####agwCheck SetPopupType[[ %s]]]2222222222222222222222222222"), *DiskSize);
		TXT_FileErrorSub->SetVisibility(ESlateVisibility::Visible);
		TXT_FileErrorSub->SetText(FText::FromString(DiskSize));
	}
}

void UB2UIDLCFrontPopup::OnClickPositive()
{
	if (m_PopupType == EPopupType::EPopupType_CheckPermission 
		|| m_PopupType == EPopupType::EPopupType_ErrorRespones)
	{
		ownerLogo_->GameExit();
	}
	else
	{

	}
}

void UB2UIDLCFrontPopup::OnClickKakaoExitBtn()
{
	ownerSetting_->KakaoPopupHidden();
}

void UB2UIDLCFrontPopup::SetRewardItem(int32 nitemID, int32 nCnt)
{
	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->UpdateRewardIconByItemIndex(nitemID, FText::AsNumber(nCnt));
	}
}


