// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIPopupSettingGlobalSound.h"

#include "B2UIManager.h"
#include "B2UIEtherInfoNotify.h"

/*
const int32 iUNSELECTED = 0;
const int32 iSELECTED	= 1;*/

void UB2UIPopupSettingGlobalSound::Init()
{
	Super::Init();

	//CachedSoundLocType = FGenericPlatformMisc::GetSoundLocType();

	UpdatePopupState(CachedSoundLocType);
}

void UB2UIPopupSettingGlobalSound::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIPopupSettingGlobalSound::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_Korea);
	GET_SLOT(UB2Button, BTN_English);
	GET_SLOT(UTextBlock, TB_TitleGlobalSelect);
	GET_SLOT(UTextBlock, TB_ExplainGlobalSelect);

	GET_SLOT(UTextBlock, TB_UnSelectedKorea);
	GET_SLOT(UTextBlock, TB_SelectedKorea);
	GET_SLOT(UTextBlock, TB_UnSelectedEnglish);
	GET_SLOT(UTextBlock, TB_SelectedEnglish);

	GET_SLOT(UWidgetSwitcher, WS_Korea);
	GET_SLOT(UWidgetSwitcher, WS_English);
}

void UB2UIPopupSettingGlobalSound::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIPopupSettingGlobalSound::OnClickBTN_Cancel);

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIPopupSettingGlobalSound::OnClickBTN_Confirm);

	BIND_CLICK_FUNC_TO_BTN(BTN_Korea, &UB2UIPopupSettingGlobalSound::OnClickBTN_Korea);

	BIND_CLICK_FUNC_TO_BTN(BTN_English, &UB2UIPopupSettingGlobalSound::OnClickBTN_English);
}

void UB2UIPopupSettingGlobalSound::OnCloseComplete()
{
	Super::OnCloseComplete();
}

void UB2UIPopupSettingGlobalSound::UpdateStaticText()
{
	if (TB_TitleGlobalSelect.IsValid())
		TB_TitleGlobalSelect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_VoiceChoiceTitle")));

	if (TB_ExplainGlobalSelect.IsValid())
		TB_ExplainGlobalSelect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_VoiceChoiceText")));

	if (TB_UnSelectedKorea.IsValid())
		TB_UnSelectedKorea->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_Korea")));
	if (TB_SelectedKorea.IsValid())
		TB_SelectedKorea->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_Korea")));

	if (TB_UnSelectedEnglish.IsValid())
		TB_UnSelectedEnglish->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_English")));
	if (TB_SelectedEnglish.IsValid())
		TB_SelectedEnglish->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_English")));
}

void UB2UIPopupSettingGlobalSound::OnClickBTN_Cancel()
{
	CloseMySelf();
}

void UB2UIPopupSettingGlobalSound::OnClickBTN_Confirm()
{
	LocalSaveSoundLocType();

	OpenConfirmNotifyPopup();

	CloseMySelf();
}

void UB2UIPopupSettingGlobalSound::OnClickBTN_Korea()
{
	//CachedSoundLocType = B2SoundLocalizeType::Korea();

	UpdatePopupState(CachedSoundLocType);
}

void UB2UIPopupSettingGlobalSound::OnClickBTN_English()
{
	//CachedSoundLocType = B2SoundLocalizeType::English();

	UpdatePopupState(CachedSoundLocType);
}

void UB2UIPopupSettingGlobalSound::CloseMySelf()
{
	if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
		pUIManagerInstance->CloseUI(UIFName::SettingGlobalSound);
}

void UB2UIPopupSettingGlobalSound::LocalSaveSoundLocType()
{
	//FGenericPlatformMisc::SetSoundLocType(CachedSoundLocType);

	SaveGameSetting_SoundLocType(CachedSoundLocType);
}

void UB2UIPopupSettingGlobalSound::UpdatePopupState(const int32 iSoundLocType)
{
	if (!WS_Korea.IsValid() ||
		!WS_English.IsValid())
		return;

	//if (iSoundLocType == B2SoundLocalizeType::English())
	//{
	//	WS_Korea->SetActiveWidgetIndex(0);
	//	WS_English->SetActiveWidgetIndex(1);
	//}
	//else
	//{
	//	WS_Korea->SetActiveWidgetIndex(1);
	//	WS_English->SetActiveWidgetIndex(0);
	//}
}

void UB2UIPopupSettingGlobalSound::OpenConfirmNotifyPopup()
{
//	if (UB2UIEtherInfoNotify* PopupNotify = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify))
//	{
//		FText ftNation = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_Korea"));
//
//		if (FGenericPlatformMisc::GetSoundLocType() == B2SoundLocalizeType::English())
//			ftNation = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_English"));
//
//		FText ftPopupNotifyText = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("VoiceChange_ConfirmNotice"))), ftNation);
//
//		PopupNotify->SetEndTime(ChangeNotiyEndTime);
//		PopupNotify->SetInfoNotify(ftPopupNotifyText);
//	}
}