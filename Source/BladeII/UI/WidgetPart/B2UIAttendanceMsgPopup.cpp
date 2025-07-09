// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIAttendanceMsgPopup.h"
#include "B2UIAttendanceMain.h"


#include "BladeIIGameImpl.h"

void UB2UIAttendanceMsgPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, B2Button_0);
	GET_SLOT(UB2Image, B2Image_0);
	GET_SLOT(UB2Image, B2Image_2);
	GET_SLOT(UB2Image, B2Image_141);
	GET_SLOT(UB2Image, IMG_ITEM);
	GET_SLOT(UTextBlock, TXT_ItemName);
	GET_SLOT(UB2Image, B2Image_3);
	GET_SLOT(UB2Button, B2Button_2);
	GET_SLOT(UTextBlock, TXT_Notice);
	GET_SLOT(UTextBlock, TXT_Ok);

	SetRewardItem();
}

void UB2UIAttendanceMsgPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(B2Button_0, &UB2UIAttendanceMsgPopup::OnClickB2Button_0);
	BIND_CLICK_FUNC_TO_BTN(B2Button_2, &UB2UIAttendanceMsgPopup::OnClickB2Button_2);
	SetRewardItem();
}

void UB2UIAttendanceMsgPopup::CloseWidgetDelegate()
{
	OnClickB2Button_2();
}

void UB2UIAttendanceMsgPopup::Init()
{
	Super::Init();

	this->AddToViewport(BII_WIDGET_ZORDER_NEW_UI_MAIN);
}

void UB2UIAttendanceMsgPopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (ConfirmDelegate.ExecuteIfBound())
		ConfirmDelegate.Unbind();
}

void UB2UIAttendanceMsgPopup::OnClickB2Button_0()
{

}

void UB2UIAttendanceMsgPopup::OnClickB2Button_2()
{
	if (ConfirmDelegate.ExecuteIfBound())
		ConfirmDelegate.Unbind();

	UB2UIManager::GetInstance()->CloseUI(UIFName::AttendanceMsgPopup);
}


void UB2UIAttendanceMsgPopup::SetRewardItem()
{
	int32 nRewardID = BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().nAttendance_Id;
	int32 nRewardCount = BladeIIGameImpl::GetClientDataStore().GetAttendanceInfo().nAttendance_count;
	IMG_ITEM->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(nRewardID));
	TXT_ItemName->SetText(UB2UIAttendanceMsgPopup::GetAttendanceItemName(nRewardID, nRewardCount));

	TXT_Notice->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Attendance_Reward_Notice")));
	TXT_Ok->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("General_Confirm")));
}

FText UB2UIAttendanceMsgPopup::GetAttendanceItemName(int32 nRewardId, int32 nRewardCount)
{
	return FText::Format(FText::FromString(TEXT("{0} x {1}")), BladeIIGameImpl::GetClientDataStore().GetRewardName(nRewardId), FText::AsNumber(nRewardCount));
}

void UB2UIAttendanceMsgPopup::SetConfirmCallback(const FMsgPopupOnClick& Handler)
{
	ConfirmDelegate = Handler;
}
