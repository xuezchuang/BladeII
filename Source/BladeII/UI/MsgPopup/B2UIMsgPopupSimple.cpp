#include "B2UIMsgPopupSimple.h"
#include "B2UIManager.h"


UB2UIMsgPopupSimple::UB2UIMsgPopupSimple(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsAutoCloseOnClick = false;
	ButtonState = false;
}

void UB2UIMsgPopupSimple::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Positive)
	GET_SLOT(UButton, BTN_Negative)
	GET_SLOT(UButton, BTN_Close)
	GET_SLOT(UTextBlock, TB_BtnPositive)
	GET_SLOT(UTextBlock, TB_BtnNegative)

	GET_SLOT(UB2RichTextBlock, TB_SubContent)
}

void UB2UIMsgPopupSimple::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupSimple::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIMsgPopupSimple::BindDelegates()
{
	Super::BindDelegates();
	//BIND_CLICK_FUNC_TO_BTN(BTN_Positive, &UB2UIMsgPopupSimple::OnClickBtnPositive)
	//BIND_CLICK_FUNC_TO_BTN(BTN_Negative, &UB2UIMsgPopupSimple::OnClickBtnNegative)
}

void UB2UIMsgPopupSimple::AddHandler(EUIMsgPopupButton ButtonType, const FMsgPopupOnClick& Handler)
{
	auto Found = ClickHandlerMap.Find(ButtonType);
	if (Found)
	{
		*Found = Handler;
		return;
	}

	ClickHandlerMap.Add(ButtonType, Handler);
}

void UB2UIMsgPopupSimple::SetButtonText(EUIMsgPopupButton ButtonType, const FText& InText)
{
	if (ButtonType == EUIMsgPopupButton::Positive && TB_BtnPositive.IsValid())
		TB_BtnPositive->SetText(InText);
	else if (ButtonType == EUIMsgPopupButton::Negative && TB_BtnNegative.IsValid())
		TB_BtnNegative->SetText(InText);
}

void UB2UIMsgPopupSimple::ShowButton(EUIMsgPopupButton ButtonType, bool bShow)
{
	if (ButtonType == EUIMsgPopupButton::Positive && BTN_Positive.IsValid())
		bShow ? BTN_Positive->SetVisibility(ESlateVisibility::Visible) : BTN_Positive->SetVisibility(ESlateVisibility::Collapsed);
	else if (ButtonType == EUIMsgPopupButton::Negative && BTN_Negative.IsValid())
		bShow ? BTN_Negative->SetVisibility(ESlateVisibility::Visible) : BTN_Negative->SetVisibility(ESlateVisibility::Collapsed);
	else if (ButtonType == EUIMsgPopupButton::Close && BTN_Negative.IsValid())
		bShow ? BTN_Close->SetVisibility(ESlateVisibility::Visible) : BTN_Close->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIMsgPopupSimple::SetButtonGroup(EUIMsgPopupButtonGroup ButtonType)
{
	switch (ButtonType)
	{
		default:
		case EUIMsgPopupButtonGroup::Yes:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Yes"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
		case EUIMsgPopupButtonGroup::No:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_No"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
		case EUIMsgPopupButtonGroup::YesOrNo:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Yes"))));
			SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_No"))));
			break;
		case EUIMsgPopupButtonGroup::Confirm:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
		case EUIMsgPopupButtonGroup::Cancel:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Cancel"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
		case EUIMsgPopupButtonGroup::ConfirmOrCancel:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
			SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Cancel"))));
			break;
		case EUIMsgPopupButtonGroup::AcceptOrReject:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Accept"))));
			SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Reject"))));
			break;
		case EUIMsgPopupButtonGroup::WatchOrExit:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Watch"))));
			SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Quit"))));
			break;
		case EUIMsgPopupButtonGroup::MatchingContinue:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MatchingPopup_Btn_Continue"))));
			SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MatchingPopup_Btn_Cancel"))));
			break;
		case EUIMsgPopupButtonGroup::Reconnect:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_ReconnectButton"))));
			SetButtonText(EUIMsgPopupButton::Negative, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Cancel"))));
#if WITH_EDITOR // 秒家 滚瓢捞 绝栏搁 辑滚 谗板阑 锭 肺厚俊 酒抗 甸绢哎 荐啊 绝促. 捞贰历贰 阂祈窍绊 刮盔 刘啊档 抗惑登聪 俊叼磐 茄沥 秒家 啊瓷窍霸..
			if (!GIsEditor)
#endif
			{
				ShowButton(EUIMsgPopupButton::Negative, false);
			}
			break;
		case EUIMsgPopupButtonGroup::ReconnectOrGoToTitle:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_GoToTitleButton"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
		case EUIMsgPopupButtonGroup::GotoLastChapter:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Tutorial_ChapterToLastAdventureBtn"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			ShowButton(EUIMsgPopupButton::Close, true);
			break;
		case EUIMsgPopupButtonGroup::None:
			ShowButton(EUIMsgPopupButton::Positive, false);
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
		case EUIMsgPopupButtonGroup::GotoLobby:
			SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_GotoLobby"))));
			ShowButton(EUIMsgPopupButton::Negative, false);
			break;
	}
}

void UB2UIMsgPopupSimple::SetAutoCloseOnClick(bool bAutoClose)
{
	IsAutoCloseOnClick = bAutoClose;
}

void UB2UIMsgPopupSimple::OnClickBtnPositive()
{
	if (IsAutoCloseOnClick)
		DestroySelf(UB2UIManager::GetInstance());

	auto Found = ClickHandlerMap.Find(EUIMsgPopupButton::Positive);

	if (Found && Found->IsBound())
		Found->Execute();
}

void UB2UIMsgPopupSimple::OnClickBtnNegative()
{
	if (IsAutoCloseOnClick)
		DestroySelf(UB2UIManager::GetInstance());

	auto Found = ClickHandlerMap.Find(EUIMsgPopupButton::Negative);

	if (Found && Found->IsBound())
		Found->Execute();
}

void UB2UIMsgPopupSimple::OnClickDestorySelf()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupSimple::CloseWidgetDelegate()
{
	bool IsPositive = false;
	bool IsNegative = false;
	bool IsCloseBtn = false;

	if(BTN_Positive.IsValid())
		IsPositive = BTN_Positive->GetVisibility() == ESlateVisibility::Visible ? true : false;
	if (BTN_Negative.IsValid())
		IsNegative = BTN_Negative->GetVisibility() == ESlateVisibility::Visible ? true : false;
	if (BTN_Close.IsValid())
		IsCloseBtn = BTN_Close->GetVisibility() == ESlateVisibility::Visible ? true : false;

	if (IsNegative == true || IsCloseBtn == true)
	{
		OnClickBtnNegative();
	}
	else if (IsPositive == true)
	{
		OnClickBtnPositive();
	}
}

void UB2UIMsgPopupSimple::SetSubContent(const FText& InText)
{
	if (TB_SubContent.IsValid())
	{
		TB_SubContent->SetText(InText);

		if (TB_SubContent->GetVisibility() == ESlateVisibility::Collapsed)
			TB_SubContent->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}