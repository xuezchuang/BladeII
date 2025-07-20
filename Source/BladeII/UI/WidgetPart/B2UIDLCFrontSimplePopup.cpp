// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDLCFrontSimplePopup.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "../B2UIEnum.h"

UB2UIDLCFrontSimplePopup::UB2UIDLCFrontSimplePopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIDLCFrontSimplePopup::AddHandler(EDLCPopupButtonType ButtonType, const FDLCMsgPopupOnClick& Handler)
{
	auto Found = ClickHandlerMap.Find(ButtonType);
	if (Found)
	{
		*Found = Handler;
		return;
	}

	ClickHandlerMap.Add(ButtonType, Handler);
}

void UB2UIDLCFrontSimplePopup::ShowButton(EDLCPopupButtonType ButtonType, bool bShow)
{
	if (ButtonType == EDLCPopupButtonType::Positive && BTN_Positive.IsValid())
		bShow ? BTN_Positive->SetVisibility(ESlateVisibility::Visible) : BTN_Positive->SetVisibility(ESlateVisibility::Collapsed);
	else if (ButtonType == EDLCPopupButtonType::Negative && BTN_Negative.IsValid())
		bShow ? BTN_Negative->SetVisibility(ESlateVisibility::Visible) : BTN_Negative->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIDLCFrontSimplePopup::SetButtonText(EDLCPopupButtonType ButtonType, const FText& InText)
{
	if (ButtonType == EDLCPopupButtonType::Positive && TB_BtnPositive.IsValid())
		TB_BtnPositive->SetText(InText);
	else if (ButtonType == EDLCPopupButtonType::Negative && TB_BtnNegative.IsValid())
		TB_BtnNegative->SetText(InText);
}

void UB2UIDLCFrontSimplePopup::SetButtonGroup(EUIMsgPopupButtonGroup ButtonType)
{
	switch (ButtonType)
	{
	default:
	case EUIMsgPopupButtonGroup::Yes:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Yes"))));
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	case EUIMsgPopupButtonGroup::No:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_No"))));
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	case EUIMsgPopupButtonGroup::YesOrNo:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Yes"))));
		SetButtonText(EDLCPopupButtonType::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_No"))));
		break;
	case EUIMsgPopupButtonGroup::Confirm:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	case EUIMsgPopupButtonGroup::Cancel:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Cancel"))));
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	case EUIMsgPopupButtonGroup::ConfirmOrCancel:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
		SetButtonText(EDLCPopupButtonType::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Cancel"))));
		break;
	case EUIMsgPopupButtonGroup::AcceptOrReject:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Accept"))));
		SetButtonText(EDLCPopupButtonType::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Reject"))));
		break;
	case EUIMsgPopupButtonGroup::WatchOrExit:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Watch"))));
		SetButtonText(EDLCPopupButtonType::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Quit"))));
		break;
	case EUIMsgPopupButtonGroup::MatchingContinue:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MatchingPopup_Btn_Continue"))));
		SetButtonText(EDLCPopupButtonType::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MatchingPopup_Btn_Cancel"))));
		break;
	case EUIMsgPopupButtonGroup::Reconnect:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_ReconnectButton"))));
		SetButtonText(EDLCPopupButtonType::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Cancel"))));
#if WITH_EDITOR // 秒家 滚瓢捞 绝栏搁 辑滚 谗板阑 锭 肺厚俊 酒抗 甸绢哎 荐啊 绝促. 捞贰历贰 阂祈窍绊 刮盔 刘啊档 抗惑登聪 俊叼磐 茄沥 秒家 啊瓷窍霸..
		if (!GIsEditor)
#endif
		{
			ShowButton(EDLCPopupButtonType::Negative, false);
		}
		break;
	case EUIMsgPopupButtonGroup::ReconnectOrGoToTitle:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_GoToTitleButton"))));
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	case EUIMsgPopupButtonGroup::GotoLastChapter:
		SetButtonText(EDLCPopupButtonType::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Tutorial_ChapterToLastAdventureBtn"))));
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	case EUIMsgPopupButtonGroup::None:
		ShowButton(EDLCPopupButtonType::Positive, false);
		ShowButton(EDLCPopupButtonType::Negative, false);
		break;
	}
}

void UB2UIDLCFrontSimplePopup::SetTitle(const FText& InText)
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(InText);

		if (TB_Title->GetVisibility() == ESlateVisibility::Collapsed)
			TB_Title->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UB2UIDLCFrontSimplePopup::SetSubContent(const FText& InText)
{
	if (TB_SubContent.IsValid())
	{
		TB_SubContent->SetText(InText);

		if (TB_SubContent->GetVisibility() == ESlateVisibility::Collapsed)
			TB_SubContent->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UB2UIDLCFrontSimplePopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UButton, BTN_Positive)
	GET_SLOT(UButton, BTN_Negative)
	GET_SLOT(UTextBlock, TB_BtnPositive)
	GET_SLOT(UTextBlock, TB_BtnNegative)

	GET_SLOT(UTextBlock, TB_Title)

	GET_SLOT(UB2RichTextBlock, TB_SubContent)
}
	
void UB2UIDLCFrontSimplePopup::BindDelegates()
{

}

void UB2UIDLCFrontSimplePopup::OnClickBtnPositive()
{
	auto Found = ClickHandlerMap.Find(EDLCPopupButtonType::Positive);

	if (Found && Found->IsBound())
		Found->Execute();

	DestroySelf();
}

void UB2UIDLCFrontSimplePopup::OnClickBtnNegative()
{
	auto Found = ClickHandlerMap.Find(EDLCPopupButtonType::Negative);

	if (Found && Found->IsBound())
		Found->Execute();

	DestroySelf();
}