// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISettingGlobalSoundBtn.h"

#include "B2UIManager.h"
#include "B2UIPopupSettingGlobalSound.h"

void UB2UISettingGlobalSoundBtn::Init()
{
	Super::Init();

	CachedSoundLocType = -1;
}

void UB2UISettingGlobalSoundBtn::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISettingGlobalSoundBtn::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateNationBtnState();
}

void UB2UISettingGlobalSoundBtn::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GlobalSound);

	GET_SLOT(UTextBlock, TB_GlobalSound);

	GET_SLOT(UTextBlock, TB_Nation);

	GET_SLOT(UWidgetSwitcher, WS_NationalFlag);
}

void UB2UISettingGlobalSoundBtn::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GlobalSound, &UB2UISettingGlobalSoundBtn::OnClickBTN_GlobalSound);
}

void UB2UISettingGlobalSoundBtn::UpdateStaticText()
{
	if (TB_GlobalSound.IsValid())
		TB_GlobalSound->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_VoiceChange")));
}

void UB2UISettingGlobalSoundBtn::OnClickBTN_GlobalSound()
{
	if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
	{
		if (UB2UIPopupSettingGlobalSound* UIPopupGlobalSound = pUIManagerInstance->OpenUI<UB2UIPopupSettingGlobalSound>(UIFName::SettingGlobalSound))
		{

		}
	}
}

void UB2UISettingGlobalSoundBtn::UpdateNationBtnState()
{
	//const int32 SoundLocType = FGenericPlatformMisc::GetSoundLocType();

	//if (CachedSoundLocType == SoundLocType)
	//	return;

	//CachedSoundLocType = SoundLocType;

	//if (!TB_Nation.IsValid() || 
	//	!WS_NationalFlag.IsValid())
	//	return;

	//FText ftNation = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_Korea"));;

	//if (SoundLocType == B2SoundLocalizeType::English())
	//	ftNation = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("VoiceChange_Language_English"));

	//TB_Nation->SetText(ftNation);
	//WS_NationalFlag->SetActiveWidgetIndex(SoundLocType);
}