// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISettingAddOns.h"
#include "B2UIManager.h"

#include "B2PlatformAPI.h"

#include "B2UIEtherInfoNotify.h"


#include "Global.h"

int32 m_nVibration_Addon = 100;

UB2UISettingAddOns::UB2UISettingAddOns(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UISettingAddOns::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UButton, BTN_AllPush);
	GET_SLOT(UImage, IMG_Unselect_AllPush);
	GET_SLOT(UImage, IMG_Select_AllPush);
	GET_SLOT(UTextBlock, TXT_AllPush_Off);
	GET_SLOT(UTextBlock, TXT_AllPush_On);

	GET_SLOT(UButton, BTN_NoticePush);
	GET_SLOT(UImage, IMG_Unselect_NoticePush);
	GET_SLOT(UImage, IMG_Select_NoticePush);
	GET_SLOT(UTextBlock, TXT_NoticePush_Off);
	GET_SLOT(UTextBlock, TXT_NoticePush_On);

	GET_SLOT(UButton, BTN_GamePush);
	GET_SLOT(UImage, IMG_Unselect_GamePush);
	GET_SLOT(UImage, IMG_Select_GamePush);
	GET_SLOT(UTextBlock, TXT_GamePush_Off);
	GET_SLOT(UTextBlock, TXT_GamePush_On);

	GET_SLOT(UButton, BTN_NightPush);
	GET_SLOT(UImage, IMG_Unselect_NightPush);
	GET_SLOT(UImage, IMG_Select_NightPush);
	GET_SLOT(UTextBlock, TXT_NightPush_Off);
	GET_SLOT(UTextBlock, TXT_NightPush_On);

	/*
	GET_SLOT(UButton, BTN_Vibration);
	GET_SLOT(UImage, IMG_Unselect_Vibration);
	GET_SLOT(UImage, IMG_Select_Vibration);
	GET_SLOT(UTextBlock, TXT_Vibration_Off);
	GET_SLOT(UTextBlock, TXT_Vibration_On);

	GET_SLOT(UButton, BTN_ScreenN);
	GET_SLOT(UImage, IMG_Unselect_ScreenN);
	GET_SLOT(UImage, IMG_Select_ScreenN);
	GET_SLOT(UTextBlock, TXT_ScreenN_Off);
	GET_SLOT(UTextBlock, TXT_ScreenN_On);
	*/
	SetData();
}

void UB2UISettingAddOns::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_AllPush, &UB2UISettingAddOns::OnClickBTN_AllPush);
	BIND_CLICK_FUNC_TO_BTN(BTN_NoticePush, &UB2UISettingAddOns::OnClickBTN_NoticePush);
	BIND_CLICK_FUNC_TO_BTN(BTN_GamePush, &UB2UISettingAddOns::OnClickBTN_GamePush);
	BIND_CLICK_FUNC_TO_BTN(BTN_NightPush, &UB2UISettingAddOns::OnClickBTN_NightPush);
	//BIND_CLICK_FUNC_TO_BTN(BTN_Vibration, &UB2UISettingAddOns::OnClickBTN_Vibration);
	//BIND_CLICK_FUNC_TO_BTN(BTN_ScreenN, &UB2UISettingAddOns::OnClickBTN_ScreenN);
}

void UB2UISettingAddOns::Init()
{
	Super::Init();
}

void UB2UISettingAddOns::DestroySelf(UB2UIManager* InUIManager)
{
	
	Super::DestroySelf(InUIManager);
}


void UB2UISettingAddOns::OnClickBTN_AllPush()
{
	UB2UISettingAddOns::SelectBtn(EAddonsOption::EAddons_AllPush);
}

void UB2UISettingAddOns::OnClickBTN_NoticePush()
{
	UB2UISettingAddOns::SelectBtn(EAddonsOption::EAddons_NoticePush);
}

void UB2UISettingAddOns::OnClickBTN_GamePush()
{
	UB2UISettingAddOns::SelectBtn(EAddonsOption::EAddons_GamePush);
}

void UB2UISettingAddOns::OnClickBTN_NightPush()
{
	UB2UISettingAddOns::SelectBtn(EAddonsOption::EAddons_NightPush);
}

/*
void UB2UISettingAddOns::OnClickBTN_Vibration()
{
	UB2UISettingAddOns::SelectBtn(EAddonsOption::EAddons_Vibration);
}

void UB2UISettingAddOns::OnClickBTN_ScreenN()
{
	UB2UISettingAddOns::SelectBtn(EAddonsOption::EAddons_ScreenN);
}
*/
void UB2UISettingAddOns::SetData()
{
	m_SettingAddOnsBtn.Empty();

	m_TempSettingAddOns.SelectedImage = IMG_Select_AllPush;
	m_TempSettingAddOns.UnselectedImage = IMG_Unselect_AllPush;
	m_TempSettingAddOns.SelectedText = TXT_AllPush_On;
	m_TempSettingAddOns.UnselectedText = TXT_AllPush_Off;
	m_SettingAddOnsBtn.Add(m_TempSettingAddOns);

	m_TempSettingAddOns.SelectedImage = IMG_Select_NoticePush;
	m_TempSettingAddOns.UnselectedImage = IMG_Unselect_NoticePush;
	m_TempSettingAddOns.SelectedText = TXT_NoticePush_On;
	m_TempSettingAddOns.UnselectedText = TXT_NoticePush_Off;
	m_SettingAddOnsBtn.Add(m_TempSettingAddOns);

	m_TempSettingAddOns.SelectedImage = IMG_Select_GamePush;
	m_TempSettingAddOns.UnselectedImage = IMG_Unselect_GamePush;
	m_TempSettingAddOns.SelectedText = TXT_GamePush_On;
	m_TempSettingAddOns.UnselectedText = TXT_GamePush_Off;
	m_SettingAddOnsBtn.Add(m_TempSettingAddOns);

	m_TempSettingAddOns.SelectedImage = IMG_Select_NightPush;
	m_TempSettingAddOns.UnselectedImage = IMG_Unselect_NightPush;
	m_TempSettingAddOns.SelectedText = TXT_NightPush_On;
	m_TempSettingAddOns.UnselectedText = TXT_NightPush_Off;
	m_SettingAddOnsBtn.Add(m_TempSettingAddOns);

	/*
	m_TempSettingAddOns.SelectedImage = IMG_Select_Vibration;
	m_TempSettingAddOns.UnselectedImage = IMG_Unselect_Vibration;
	m_TempSettingAddOns.SelectedText = TXT_Vibration_On;
	m_TempSettingAddOns.UnselectedText = TXT_Vibration_Off;
	m_SettingAddOnsBtn.Add(m_TempSettingAddOns);

	m_TempSettingAddOns.SelectedImage = IMG_Select_ScreenN;
	m_TempSettingAddOns.UnselectedImage = IMG_Unselect_ScreenN;
	m_TempSettingAddOns.SelectedText = TXT_ScreenN_On;
	m_TempSettingAddOns.UnselectedText = TXT_ScreenN_Off;
	m_SettingAddOnsBtn.Add(m_TempSettingAddOns);
	*/
	for (int i = 0; i < m_SettingAddOnsBtn.Num(); i++)
	{
		m_SettingAddOnsBtn[i].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[i].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[i].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[i].UnselectedText->SetVisibility(ESlateVisibility::Visible);
	}

	//LoadSettingGameData();
	int32 nOptionCnt = 0;
	if (B2P_IsEnableKakaoPush(true))
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedText->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
		nOptionCnt++;
		SaveGameSetting_kakaoDayPush(1);
	}
	else
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
		SaveGameSetting_kakaoDayPush(0);
	}

	if (B2P_IsEnableKakaoPush(false))
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedText->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
		nOptionCnt++;
		SaveGameSetting_KakaoNightPush(1);
	}
	else
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
		SaveGameSetting_KakaoNightPush(0);
	}

	int32 nGamePush = 1;
	LoadGameSetting_GamePush(nGamePush);
	if (nGamePush)
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedText->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
		nOptionCnt++;
	}
	else
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
	}

	if (nOptionCnt > 2)
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedText->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UISettingAddOns::SelectBtn(EAddonsOption option)
{
	int32 nOption = (int)option;
	FText sPopupMessage;
	FString sTime; 
	FDateTime timeElapse = FDateTime::UtcNow();
	sTime = FString::Printf(TEXT(" (%d.%d.%d) "), timeElapse.GetYear(), timeElapse.GetMonth(), timeElapse.GetDay());
			
	if (m_SettingAddOnsBtn[nOption].SelectedImage->IsVisible())
	{
		m_SettingAddOnsBtn[nOption].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[nOption].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[nOption].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[nOption].UnselectedText->SetVisibility(ESlateVisibility::Visible);

	
		if (option == EAddonsOption::EAddons_GamePush)
		{
			SaveGameSetting_GamePush(0);
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushGameDisagree"))), FText::FromString(sTime));
		}
		else if (option == EAddonsOption::EAddons_NightPush)
		{
			B2P_SetKakaoPushEnable(false, false);
			SaveGameSetting_KakaoNightPush(0);
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushNightDisagree"))), FText::FromString(sTime));
		}
		else if (option == EAddonsOption::EAddons_AllPush)
		{
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushAlDisagree"))), FText::FromString(sTime));
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedText->SetVisibility(ESlateVisibility::Visible);

			SaveGameSetting_GamePush(0);
			B2P_SetKakaoPushEnable(false, false);
			SaveGameSetting_KakaoNightPush(0);
			B2P_SetKakaoPushEnable(true, false);
			SaveGameSetting_kakaoDayPush(0);
		}
		else if (option == EAddonsOption::EAddons_NoticePush)
		{
			B2P_SetKakaoPushEnable(true, false);
			SaveGameSetting_kakaoDayPush(0);
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushNotiDisagree"))), FText::FromString(sTime));
		}
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedText->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_SettingAddOnsBtn[nOption].SelectedImage->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[nOption].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
		m_SettingAddOnsBtn[nOption].SelectedText->SetVisibility(ESlateVisibility::Visible);
		m_SettingAddOnsBtn[nOption].UnselectedText->SetVisibility(ESlateVisibility::Hidden);

		if (option == EAddonsOption::EAddons_GamePush)
		{
			SaveGameSetting_GamePush(1);
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushGameAgree"))), FText::FromString(sTime));
		}
		else if (option == EAddonsOption::EAddons_NightPush)
		{
			B2P_SetKakaoPushEnable(false, true);
			SaveGameSetting_KakaoNightPush(1);
			
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushNightAgree"))), FText::FromString(sTime));
		}
		else if (option == EAddonsOption::EAddons_AllPush)
		{
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushAllAgree"))), FText::FromString(sTime));
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].SelectedText->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_GamePush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].SelectedText->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NightPush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].SelectedText->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_NoticePush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
			SaveGameSetting_GamePush(1);
			B2P_SetKakaoPushEnable(false, true);
			SaveGameSetting_KakaoNightPush(1);
			B2P_SetKakaoPushEnable(true, true);
			SaveGameSetting_kakaoDayPush(1);
		}
		else if (option == EAddonsOption::EAddons_NoticePush)
		{
			B2P_SetKakaoPushEnable(true, true);
			SaveGameSetting_kakaoDayPush(1);
			sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_CONFIGURATION), FString(TEXT("Configuration_PushNotiAgree"))), FText::FromString(sTime));
		}

		int32 nOptionCnt = 0;
		int32 nGamePush = 1;
		int32 nKakaoDayPush = 1;
		int32 nKakaoNightPush = 1;
		LoadGameSetting_GamePush(nGamePush);
		LoadGameSetting_KakaoDayPush(nKakaoDayPush);
		LoadGameSetting_KakaoNightPush(nKakaoNightPush);
		if (nGamePush) nOptionCnt++;
		if (nKakaoDayPush)  nOptionCnt++;
		if (nKakaoNightPush)  nOptionCnt++;

		if (nOptionCnt > 2)
		{
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedImage->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedImage->SetVisibility(ESlateVisibility::Hidden);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].SelectedText->SetVisibility(ESlateVisibility::Visible);
			m_SettingAddOnsBtn[(int)EAddonsOption::EAddons_AllPush].UnselectedText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
	if (Popup)
	{
		Popup->SetInfoNotify(sPopupMessage);
	}
	SaveSettingData();
}

void UB2UISettingAddOns::SaveSettingData()
{
	GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameVibrationSet")), m_nVibration_Addon, GB2GeneralSavedStateIni);

	GConfig->Flush(false, GB2GeneralSavedStateIni);
}

void UB2UISettingAddOns::LoadSettingGameData()
{	
	GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameVibrationSet")), m_nVibration_Addon, GB2GeneralSavedStateIni);

	if (m_nVibration_Addon == 100)
	{	
		m_nVibration_Addon = 1;
		SaveSettingData();
	}

}

