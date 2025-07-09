#include "BladeII.h"
#include "B2UISemiSetting.h"
#include "B2UIManager.h"
#include "UnrealEngine.h"
#include "BladeIIGameMode.h"


UB2UISemiSetting::UB2UISemiSetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UISemiSetting::Init()
{
	Super::Init();
	SetViewVibBtn(true);
}

void UB2UISemiSetting::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_SettingTitle)
	GET_SLOT(UButton, BTN_Exit)
	GET_SLOT(UB2UISettingGame, UIP_Setting_Game)
	if (UIP_Setting_Game.IsValid())
	{
		UIP_Setting_Game->Init();
	}
}

void UB2UISemiSetting::UpdateStaticText()
{
	if (TB_SettingTitle.IsValid())
	{
		TB_SettingTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PauseMenu_Configuration")));
	}
}

void UB2UISemiSetting::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Exit, &UB2UISemiSetting::OnclickBtnExit)
}

void UB2UISemiSetting::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UISemiSetting::CloseWidgetDelegate()
{
	OnclickBtnExit();
}

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UISemiSetting::OnclickBtnExit()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::SemiSetting);
}