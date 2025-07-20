#include "B2UISettingGame.h"
#include "B2UIManager.h"
#include "UnrealEngine.h"
#include "BladeIIGameMode.h"
#include "B2PlatformAPI.h"
#include "B2UISettingGlobalSoundBtn.h"

#include "Global.h"

UB2UISettingGame::UB2UISettingGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_nGraphicLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	m_nFrameLimitLevel = B2FrameLevelToInt(EB2FrameLimitLevel::Mid); // FrameLimit 尔 ResolutionLevel 篮 扁夯蔼阑 Mid 肺 窍绰 霸 亮促.
	m_nSafeResolutionLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
	m_fBGM_Volume = 1.0f;
	m_fFXSound_Volume = 1.0f;
	m_bVibration = true;
	m_bScreenLock = true;
#if !UE_BUILD_SHIPPING
	bLoadSettingGameDataCalledOnce = false;
#endif
}

void UB2UISettingGame::Init()
{
	Super::Init();
	
	OnInitBP();
}

void UB2UISettingGame::CacheAssets()
{	
	GET_SLOT(UB2Button, BTN_BackToSuggested)
	GET_SLOT(UTextBlock, TB_BackToSuggested)

	GET_SLOT(UB2Button, BTN_Graphics_High)
	GET_SLOT(UB2Button, BTN_Graphics_Mid)
	GET_SLOT(UB2Button, BTN_Graphics_Low)

	GET_SLOT(UTextBlock, TB_Graphics_Label)
	{
		GET_SLOT(UImage, IMG_Graphics_High_Select)
		GET_SLOT(UImage, IMG_Graphics_High_Unselect)
		GET_SLOT(UTextBlock, TB_Graphics_High_Select)
		GET_SLOT(UTextBlock, TB_Graphics_High_Unselect)
		// On/Off 惑怕俊 蝶弗 叼磊牢 利侩阑 困秦 悼老 困摹狼 惑怕俊 蝶弗 滚瓢 棺 咆胶飘甸阑 茄单 弓澜.
		Graphics_High_Bundle.IMG_On = IMG_Graphics_High_Select;
		Graphics_High_Bundle.IMG_Off = IMG_Graphics_High_Unselect;
		Graphics_High_Bundle.TB_On = TB_Graphics_High_Select;
		Graphics_High_Bundle.TB_Off = TB_Graphics_High_Unselect;
	}
	{
		GET_SLOT(UImage, IMG_Graphics_Mid_Select)
		GET_SLOT(UImage, IMG_Graphics_Mid_Unselect)
		GET_SLOT(UTextBlock, TB_Graphics_Mid_Select)
		GET_SLOT(UTextBlock, TB_Graphics_Mid_Unselect)
		Graphics_Mid_Bundle.IMG_On = IMG_Graphics_Mid_Select;
		Graphics_Mid_Bundle.IMG_Off = IMG_Graphics_Mid_Unselect;
		Graphics_Mid_Bundle.TB_On = TB_Graphics_Mid_Select;
		Graphics_Mid_Bundle.TB_Off = TB_Graphics_Mid_Unselect;
	}
	{
		GET_SLOT(UImage, IMG_Graphics_Low_Select)
		GET_SLOT(UImage, IMG_Graphics_Low_Unselect)
		GET_SLOT(UTextBlock, TB_Graphics_Low_Select)
		GET_SLOT(UTextBlock, TB_Graphics_Low_Unselect)
		Graphics_Low_Bundle.IMG_On = IMG_Graphics_Low_Select;
		Graphics_Low_Bundle.IMG_Off = IMG_Graphics_Low_Unselect;
		Graphics_Low_Bundle.TB_On = TB_Graphics_Low_Select;
		Graphics_Low_Bundle.TB_Off = TB_Graphics_Low_Unselect;
	}


	GET_SLOT(UB2Button, BTN_FrameLimit_High)
	GET_SLOT(UB2Button, BTN_FrameLimit_Mid)
	GET_SLOT(UB2Button, BTN_FrameLimit_Low)

	GET_SLOT(UTextBlock, TB_FrameLimit_Label)
	{
		GET_SLOT(UImage, IMG_FrameLimit_High_Select)
		GET_SLOT(UImage, IMG_FrameLimit_High_Unselect)
		GET_SLOT(UTextBlock, TB_FrameLimit_High_Select)
		GET_SLOT(UTextBlock, TB_FrameLimit_High_Unselect)
		FrameLimit_High_Bundle.IMG_On = IMG_FrameLimit_High_Select;
		FrameLimit_High_Bundle.IMG_Off = IMG_FrameLimit_High_Unselect;
		FrameLimit_High_Bundle.TB_On = TB_FrameLimit_High_Select;
		FrameLimit_High_Bundle.TB_Off = TB_FrameLimit_High_Unselect;
	}
	{
		GET_SLOT(UImage, IMG_FrameLimit_Mid_Select)
		GET_SLOT(UImage, IMG_FrameLimit_Mid_Unselect)
		GET_SLOT(UTextBlock, TB_FrameLimit_Mid_Select)
		GET_SLOT(UTextBlock, TB_FrameLimit_Mid_Unselect)
		FrameLimit_Mid_Bundle.IMG_On = IMG_FrameLimit_Mid_Select;
		FrameLimit_Mid_Bundle.IMG_Off = IMG_FrameLimit_Mid_Unselect;
		FrameLimit_Mid_Bundle.TB_On = TB_FrameLimit_Mid_Select;
		FrameLimit_Mid_Bundle.TB_Off = TB_FrameLimit_Mid_Unselect;
	}
	{
		GET_SLOT(UImage, IMG_FrameLimit_Low_Select)
		GET_SLOT(UImage, IMG_FrameLimit_Low_Unselect)
		GET_SLOT(UTextBlock, TB_FrameLimit_Low_Select)
		GET_SLOT(UTextBlock, TB_FrameLimit_Low_Unselect)
		FrameLimit_Low_Bundle.IMG_On = IMG_FrameLimit_Low_Select;
		FrameLimit_Low_Bundle.IMG_Off = IMG_FrameLimit_Low_Unselect;
		FrameLimit_Low_Bundle.TB_On = TB_FrameLimit_Low_Select;
		FrameLimit_Low_Bundle.TB_Off = TB_FrameLimit_Low_Unselect;
	}


	GET_SLOT(UB2Button, BTN_ResAA_High)
	GET_SLOT(UB2Button, BTN_ResAA_Mid)
	GET_SLOT(UB2Button, BTN_ResAA_Low)

	GET_SLOT(UTextBlock, TB_ResAA_Label)
	{
		GET_SLOT(UImage, IMG_ResAA_High_Select)
		GET_SLOT(UImage, IMG_ResAA_High_Unselect)
		GET_SLOT(UTextBlock, TB_ResAA_High_Select)
		GET_SLOT(UTextBlock, TB_ResAA_High_Unselect)
		ResAA_High_Bundle.IMG_On = IMG_ResAA_High_Select;
		ResAA_High_Bundle.IMG_Off = IMG_ResAA_High_Unselect;
		ResAA_High_Bundle.TB_On = TB_ResAA_High_Select;
		ResAA_High_Bundle.TB_Off = TB_ResAA_High_Unselect;
	}
	{
		GET_SLOT(UImage, IMG_ResAA_Mid_Select)
		GET_SLOT(UImage, IMG_ResAA_Mid_Unselect)
		GET_SLOT(UTextBlock, TB_ResAA_Mid_Select)
		GET_SLOT(UTextBlock, TB_ResAA_Mid_Unselect)
		ResAA_Mid_Bundle.IMG_On = IMG_ResAA_Mid_Select;
		ResAA_Mid_Bundle.IMG_Off = IMG_ResAA_Mid_Unselect;
		ResAA_Mid_Bundle.TB_On = TB_ResAA_Mid_Select;
		ResAA_Mid_Bundle.TB_Off = TB_ResAA_Mid_Unselect;
	}
	{
		GET_SLOT(UImage, IMG_ResAA_Low_Select)
		GET_SLOT(UImage, IMG_ResAA_Low_Unselect)
		GET_SLOT(UTextBlock, TB_ResAA_Low_Select)
		GET_SLOT(UTextBlock, TB_ResAA_Low_Unselect)
		ResAA_Low_Bundle.IMG_On = IMG_ResAA_Low_Select;
		ResAA_Low_Bundle.IMG_Off = IMG_ResAA_Low_Unselect;
		ResAA_Low_Bundle.TB_On = TB_ResAA_Low_Select;
		ResAA_Low_Bundle.TB_Off = TB_ResAA_Low_Unselect;
	}


	GET_SLOT(UTextBlock, TB_BGM_Label)
	GET_SLOT(UTextBlock, TB_FXSound_Label)

	GET_SLOT(UPanelWidget, P_ScreenLock_Vib_Set)
	if (P_ScreenLock_Vib_Set.IsValid()) { // 抗傈俊 窍窜 汲沥 滚瓢 技飘甫 On/Off 窍绰 炼扒捞 乐菌带 葛剧?
		P_ScreenLock_Vib_Set->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	{
		GET_SLOT(UTextBlock, TB_ScreenLock_Label)
		GET_SLOT(UImage, IMG_ScreenLock_On)
		GET_SLOT(UImage, IMG_ScreenLock_Off)
		GET_SLOT(UTextBlock, TB_ScreenLock_On)
		GET_SLOT(UTextBlock, TB_ScreenLock_Off)
		ScreenLock_Bundle.IMG_On = IMG_ScreenLock_On;
		ScreenLock_Bundle.IMG_Off = IMG_ScreenLock_Off;
		ScreenLock_Bundle.TB_On = TB_ScreenLock_On;
		ScreenLock_Bundle.TB_Off = TB_ScreenLock_Off;
	}
	{
		GET_SLOT(UTextBlock, TB_Vib_Label)
		GET_SLOT(UImage, IMG_Vib_On)
		GET_SLOT(UImage, IMG_Vib_Off)
		GET_SLOT(UTextBlock, TB_Vib_On)
		GET_SLOT(UTextBlock, TB_Vib_Off)
		Vib_Bundle.IMG_On = IMG_Vib_On;
		Vib_Bundle.IMG_Off = IMG_Vib_Off;
		Vib_Bundle.TB_On = TB_Vib_On;
		Vib_Bundle.TB_Off = TB_Vib_Off;
	}

	GET_SLOT(UB2UISettingGlobalSoundBtn, UIP_Setting_GlobalSoundBtn);
	if (UIP_Setting_GlobalSoundBtn.IsValid())
		UIP_Setting_GlobalSoundBtn->Init();

	/*GET_SLOT(UImage, IMG_Voice_On)
	GET_SLOT(UImage, IMG_Voice_Off)
	GET_SLOT(UTextBlock, TB_Voice_On)
	GET_SLOT(UTextBlock, TB_Voice_Off)
	*/
	

	LoadSettingGameData();

	RefreshAllButtonSelectedState();

	// 橇饭烙 府固飘客 秦惑档AA 汲沥 滚瓢 劝己拳 咯何绰 贸澜 茄锅父 诀单捞飘 秦林搁 凳.
	SetFrameLimitButtonEnableState();
	SetResAAButtonEnableState();
}

void UB2UISettingGame::UpdateStaticText()
{
	if (TB_Graphics_Label.IsValid()) {
		TB_Graphics_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GraphicSetUp")));
	}
	if (TB_BackToSuggested.IsValid()) {
		TB_BackToSuggested->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_BackToSuggested")));
	}

	if (TB_Graphics_High_Select.IsValid()) {
		TB_Graphics_High_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_HighQuality")));
	}
	if (TB_Graphics_High_Unselect.IsValid()) {
		TB_Graphics_High_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_HighQuality")));
	}
	if (TB_Graphics_Mid_Select.IsValid()) {
		TB_Graphics_Mid_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_MidQuality")));
	}
	if (TB_Graphics_Mid_Unselect.IsValid()) {
		TB_Graphics_Mid_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_MidQuality")));
	}
	if (TB_Graphics_Low_Select.IsValid()) {
		TB_Graphics_Low_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_LowQuality")));
	}
	if (TB_Graphics_Low_Unselect.IsValid()) {
		TB_Graphics_Low_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_LowQuality")));
	}

	if (TB_FrameLimit_Label.IsValid()) {
		TB_FrameLimit_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimitSetup")));
	}
	if (TB_FrameLimit_High_Select.IsValid()) {
		TB_FrameLimit_High_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimit_High")));
	}
	if (TB_FrameLimit_High_Unselect.IsValid()) {
		TB_FrameLimit_High_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimit_High")));
	}
	if (TB_FrameLimit_Mid_Select.IsValid()) {
		TB_FrameLimit_Mid_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimit_Mid")));
	}
	if (TB_FrameLimit_Mid_Unselect.IsValid()) {
		TB_FrameLimit_Mid_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimit_Mid")));
	}
	if (TB_FrameLimit_Low_Select.IsValid()) {
		TB_FrameLimit_Low_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimit_Low")));
	}
	if (TB_FrameLimit_Low_Unselect.IsValid()) {
		TB_FrameLimit_Low_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_FrameLimit_Low")));
	}
	
	if (TB_ResAA_Label.IsValid()) {
		TB_ResAA_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAASetup")));
	}
	if (TB_ResAA_High_Select.IsValid()) {
		TB_ResAA_High_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAA_High")));
	}
	if (TB_ResAA_High_Unselect.IsValid()) {
		TB_ResAA_High_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAA_High")));
	}
	if (TB_ResAA_Mid_Select.IsValid()) {
		TB_ResAA_Mid_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAA_Mid")));
	}
	if (TB_ResAA_Mid_Unselect.IsValid()) {
		TB_ResAA_Mid_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAA_Mid")));
	}
	if (TB_ResAA_Low_Select.IsValid()) {
		TB_ResAA_Low_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAA_Low")));
	}
	if (TB_ResAA_Low_Unselect.IsValid()) {
		TB_ResAA_Low_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ResolutionAA_Low")));
	}

	if (TB_BGM_Label.IsValid()) {
		TB_BGM_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_BGM")));
	}
	if (TB_FXSound_Label.IsValid()) {
		TB_FXSound_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_SFX")));
	}

	if (TB_ScreenLock_Label.IsValid()) {
		TB_ScreenLock_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_ScreenOff")));
	}
	if (TB_ScreenLock_On.IsValid()) {
		TB_ScreenLock_On->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_On")));
	}
	if (TB_ScreenLock_Off.IsValid()) {
		TB_ScreenLock_Off->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Off")));
	}

	if (TB_Vib_Label.IsValid()) {
		TB_Vib_Label->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Vibration")));
	}
	if (TB_Vib_On.IsValid()) {
		TB_Vib_On->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_On")));
	}
	if (TB_Vib_Off.IsValid()) {
		TB_Vib_Off->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Off")));
	}
}

void UB2UISettingGame::BindDelegates()
{	
	BIND_CLICK_FUNC_TO_BTN(BTN_BackToSuggested, &UB2UISettingGame::OnClick_BackToSuggested);

	BIND_CLICK_FUNC_TO_BTN(BTN_Graphics_High, &UB2UISettingGame::OnClick_Graphic_High);
	BIND_CLICK_FUNC_TO_BTN(BTN_Graphics_Mid, &UB2UISettingGame::OnClick_Graphic_Mid);
	BIND_CLICK_FUNC_TO_BTN(BTN_Graphics_Low, &UB2UISettingGame::OnClick_Graphic_Low);

	BIND_CLICK_FUNC_TO_BTN(BTN_FrameLimit_High, &UB2UISettingGame::OnClick_FrameLimit_High);
	BIND_CLICK_FUNC_TO_BTN(BTN_FrameLimit_Mid, &UB2UISettingGame::OnClick_FrameLimit_Mid);
	BIND_CLICK_FUNC_TO_BTN(BTN_FrameLimit_Low, &UB2UISettingGame::OnClick_FrameLimt_Low);

	BIND_CLICK_FUNC_TO_BTN(BTN_ResAA_High, &UB2UISettingGame::OnClick_ResAA_High);
	BIND_CLICK_FUNC_TO_BTN(BTN_ResAA_Mid, &UB2UISettingGame::OnClick_ResAA_Mid);
	BIND_CLICK_FUNC_TO_BTN(BTN_ResAA_Low, &UB2UISettingGame::OnClick_ResAA_Low);

	// 唱赣瘤 滚瓢 勤甸矾绰 喉风橇赴飘俊辑 官牢爹.
}

void UB2UISettingGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UISettingGame::OnClick_BackToSuggested()
{
	// 弊贰侨 饭骇 挥 酒聪扼 促弗 己瓷 包访茄 巴档 扁夯蔼 倒妨出 焊磊..

	OnClick_Graphic_Common(B2Scalability::GetGraphicsLevelByExpectedScalability());

	OnClick_FrameLimt_Common(B2Scalability::GetDefaultRelativeFrameLimitLevel());

	OnClick_ResAA_Common(B2Scalability::GetResolutionLevelOfRQType(GetDefaultGraphicsRQType()));
}

void UB2UISettingGame::OnClick_Graphic_High()
{
	OnClick_Graphic_Common(EB2GraphicsLevel::GraphicsLevel_HIGH);
}
void UB2UISettingGame::OnClick_Graphic_Mid()
{
	OnClick_Graphic_Common(EB2GraphicsLevel::GraphicsLevel_MID);
}
void UB2UISettingGame::OnClick_Graphic_Low()
{
	OnClick_Graphic_Common(EB2GraphicsLevel::GraphicsLevel_LOW);
}
void UB2UISettingGame::OnClick_Graphic_Common(EB2GraphicsLevel SelectedLevel)
{
	// UISettingGame 捞 肺厚挥 酒聪扼 促弗 霸烙葛靛俊辑档 阂副 荐 乐栏骨肺 霸烙葛靛 喊 汲沥阑 且 荐 乐档废 WorldContextobject 甫 持绢林绰 巴捞 鞘夸窍促.
	B2Scalability::AdjustScalabilityBySelectedLevel(this, SelectedLevel, GetContextDesiredResolutionLevel(false));
	
	m_nGraphicLevel = B2GraphicsLevelToInt(SelectedLevel);
	SaveSettingData();
	
	SetGraphicsButtonSelectedState(SelectedLevel);
}
void UB2UISettingGame::SetGraphicsButtonSelectedState(EB2GraphicsLevel SelectedLevel)
{
	Graphics_High_Bundle.SetOnOff(false);
	Graphics_Mid_Bundle.SetOnOff(false);
	Graphics_Low_Bundle.SetOnOff(false);

	switch (SelectedLevel)
	{
	case EB2GraphicsLevel::GraphicsLevel_HIGH:
		Graphics_High_Bundle.SetOnOff(true);
		break;
	case EB2GraphicsLevel::GraphicsLevel_MID:
		Graphics_Mid_Bundle.SetOnOff(true);
		break;
	case EB2GraphicsLevel::GraphicsLevel_LOW:
		Graphics_Low_Bundle.SetOnOff(true);
		break;
	}
}

void UB2UISettingGame::OnClick_FrameLimit_High()
{
	OnClick_FrameLimt_Common(EB2FrameLimitLevel::High);
}
void UB2UISettingGame::OnClick_FrameLimit_Mid()
{
	OnClick_FrameLimt_Common(EB2FrameLimitLevel::Mid);
}
void UB2UISettingGame::OnClick_FrameLimt_Low()
{
	OnClick_FrameLimt_Common(EB2FrameLimitLevel::Low);
}
void UB2UISettingGame::OnClick_FrameLimt_Common(EB2FrameLimitLevel SelectedLevel)
{
	check(SelectedLevel != EB2FrameLimitLevel::End);

	B2Scalability::SetRelativeFrameLimitLevel(SelectedLevel);

	m_nFrameLimitLevel = B2FrameLevelToInt(SelectedLevel);
	SaveSettingData();
	SetFrameLimitButtonSelectedState(SelectedLevel);
}
void UB2UISettingGame::SetFrameLimitButtonSelectedState(EB2FrameLimitLevel SelectedLevel)
{
	FrameLimit_High_Bundle.SetOnOff(false);
	FrameLimit_Mid_Bundle.SetOnOff(false);
	FrameLimit_Low_Bundle.SetOnOff(false);

	switch (SelectedLevel)
	{
	case EB2FrameLimitLevel::High:
		FrameLimit_High_Bundle.SetOnOff(true);
		break;
	case EB2FrameLimitLevel::Mid:
		FrameLimit_Mid_Bundle.SetOnOff(true);
		break;
	case EB2FrameLimitLevel::Low:
		FrameLimit_Low_Bundle.SetOnOff(true);
		break;
	}
}
void UB2UISettingGame::SetFrameLimitButtonEnableState()
{
	// 眠饶 disable 等 扒 惑困 菩澄 collapse 甫 绊妨..

	if (BTN_FrameLimit_High.IsValid())
	{
		bool bIsEnable = B2Scalability::IsRelativeFrameLimitLevelAvailable(EB2FrameLimitLevel::High);
		BTN_FrameLimit_High->SetIsEnabled(bIsEnable);
		FrameLimit_High_Bundle.SetEnable(bIsEnable);
	}
	if (BTN_FrameLimit_Mid.IsValid())
	{
		bool bIsEnable = B2Scalability::IsRelativeFrameLimitLevelAvailable(EB2FrameLimitLevel::Mid);
		BTN_FrameLimit_Mid->SetIsEnabled(bIsEnable);
		FrameLimit_Mid_Bundle.SetEnable(bIsEnable);
	}
	if (BTN_FrameLimit_Low.IsValid())
	{
		bool bIsEnable = B2Scalability::IsRelativeFrameLimitLevelAvailable(EB2FrameLimitLevel::Low);
		BTN_FrameLimit_Low->SetIsEnabled(bIsEnable);
		FrameLimit_Low_Bundle.SetEnable(bIsEnable);
	}
}

void UB2UISettingGame::OnClick_ResAA_High()
{
	OnClick_ResAA_Common(EB2ResolutionLevel::High);
}
void UB2UISettingGame::OnClick_ResAA_Mid()
{
	OnClick_ResAA_Common(EB2ResolutionLevel::Mid);
}
void UB2UISettingGame::OnClick_ResAA_Low()
{
	OnClick_ResAA_Common(EB2ResolutionLevel::Low);
}
void UB2UISettingGame::OnClick_ResAA_Common(EB2ResolutionLevel SelectedLevel)
{
	//check(SelectedLevel != EB2ResolutionLevel::End);

	//const EB2GraphicsRQType SelectedRQType = B2Scalability::GetSafeMappedRQTypeOfResolutionLevel(SelectedLevel);

	//bool bPopupMsgForReservedApply = false;
	//if (B2Scalability::DoesRQTypeRequiresAppRestart(SelectedRQType))
	//{ // 捞锅 繁鸥烙 胶困莫篮 救 登聪 m_nSafeResolutionLevel 阑 官操瘤绰 富绊 促澜 锅 角青矫 利侩阑 困秦 抗距父 秦 狄.
	//	ReserveResolutionLevelForNextAppStart(SelectedLevel);
	//	bPopupMsgForReservedApply = true;
	//}
	//else
	//{ // 捞傈俊 抗距秦 滴菌带 霸 乐歹扼档 努府绢 窃.
	//	ClearResolutionLevelReserve();
	//	m_nSafeResolutionLevel = B2ResolutionLevelToInt(SelectedLevel);
	//}

	//SaveSettingData();

	//// 菊第肺 FlushRenderingCommands 甫 窍绰 扒 MobileContentScaleFactor 啊 函版登绰 惑炔俊辑 酒付档 坊歹鸥百 犁且寸窍搁辑 悼累捞 阂救沥秦瘤绰 隆饶啊 乐菌扁 锭巩.
	//// 利绢档 捞霸 MobileContentScaleFactor 函版 矫 阂救沥茄 悼累阑 阜酒林绰 扒 芭狼 犬角窃.
	//FlushRenderingCommands();
	//// 扁夯 弊贰侨 汲沥苞 嘎拱妨 倒酒啊绰 汲沥烙
	//B2Scalability::AdjustScalabilityBySelectedLevel(this, IntToB2GraphicsLevel(m_nGraphicLevel), GetContextDesiredResolutionLevel(false));
	//FlushRenderingCommands();

	//SetResAAButtonSelectedState(GetContextDesiredResolutionLevel(true));

	//if (bPopupMsgForReservedApply)
	//{
	//	// 犁矫累 饶 利侩登绰 巴俊 措茄 救郴 扑诀
	//	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GuideOnContentScaleChange")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm,
	//		FMsgPopupOnClick::CreateLambda([]()
	//			{
	//				// Nothing..
	//			}
	//		)
	//		);
	//}
}
void UB2UISettingGame::SetResAAButtonSelectedState(EB2ResolutionLevel SelectedLevel)
{
	ResAA_High_Bundle.SetOnOff(false);
	ResAA_Mid_Bundle.SetOnOff(false);
	ResAA_Low_Bundle.SetOnOff(false);

	switch (SelectedLevel)
	{
	case EB2ResolutionLevel::High:
		ResAA_High_Bundle.SetOnOff(true);
		break;
	case EB2ResolutionLevel::Mid:
		ResAA_Mid_Bundle.SetOnOff(true);
		break;
	case EB2ResolutionLevel::Low:
		ResAA_Low_Bundle.SetOnOff(true);
		break;
	}
}
void UB2UISettingGame::SetResAAButtonEnableState()
{
	// 眠饶 disable 等 扒 collapse 甫 绊妨..

	if (BTN_ResAA_High.IsValid())
	{
		bool bIsEnable = B2Scalability::IsResolutionLevelAvailable(EB2ResolutionLevel::High);
		BTN_ResAA_High->SetIsEnabled(bIsEnable);
		ResAA_High_Bundle.SetEnable(bIsEnable); // 滚瓢 磊眉狼 disable 拿矾肺绰 何练秦辑 磊眉利牢 祸惑 焊沥阑 粱 茄促.
	}
	if (BTN_ResAA_Mid.IsValid())
	{
		bool bIsEnable = B2Scalability::IsResolutionLevelAvailable(EB2ResolutionLevel::Mid);
		BTN_ResAA_Mid->SetIsEnabled(bIsEnable);
		ResAA_Mid_Bundle.SetEnable(bIsEnable);
	}
	if (BTN_ResAA_Low.IsValid())
	{
		bool bIsEnable = B2Scalability::IsResolutionLevelAvailable(EB2ResolutionLevel::Low);
		BTN_ResAA_Low->SetIsEnabled(bIsEnable);
		ResAA_Low_Bundle.SetEnable(bIsEnable);
	}
}

void UB2UISettingGame::RefreshAllButtonSelectedState()
{
#if !UE_BUILD_SHIPPING
	// 角荐肺 鉴辑 部捞瘤 臼霸 眉农. 弥家 茄锅 历厘茄 肺拿 汲沥阑 肺爹茄 饶俊 捞吧 荤侩秦具 茄促.
	check(bLoadSettingGameDataCalledOnce);
#endif

	// 肺爹茄 泅犁 汲沥蔼俊 蝶扼 滚瓢 On/Off 惑怕 技泼.
	SetGraphicsButtonSelectedState(IntToB2GraphicsLevel(m_nGraphicLevel));
	SetFrameLimitButtonSelectedState(IntToB2FrameLimitLevel(m_nFrameLimitLevel));
	SetResAAButtonSelectedState(GetContextDesiredResolutionLevel(true));

	ScreenLock_Bundle.SetOnOff(m_bScreenLock);
	Vib_Bundle.SetOnOff(m_bVibration);
}

EB2GraphicsLevel UB2UISettingGame::OnGetGraphicType()
{
	return IntToB2GraphicsLevel(m_nGraphicLevel);
}

void UB2UISettingGame::OnSetBGM(float fVolume)
{
	//FAudioDevice* audioDevice = GEngine->GetMainAudioDevice();

	//if (!audioDevice)
	//	return;

	//if (fVolume < 0.0f)
	//	fVolume = 0.0f;

	//GlobalSetGameBGMVolume(fVolume);

	//m_fBGM_Volume = FMath::RoundToFloat(fVolume * 10.0f) / 10.0f;
	//SaveSettingData();
}

float UB2UISettingGame::OnGetBGM_Volume()
{
	return m_fBGM_Volume;
}

void UB2UISettingGame::OnSetFXSound(float fVolume)
{
	//FAudioDevice* audioDevice = GEngine->GetMainAudioDevice();

	//if (!audioDevice)
	//	return;

	//if (fVolume < 0.0f)
	//	fVolume = 0.0f;

	//GlobalSetGameEffectVolume(fVolume);

	//m_fFXSound_Volume = FMath::RoundToFloat(fVolume * 10.0f) / 10.0f;
	//SaveSettingData();
}

float UB2UISettingGame::OnGetFXSound_Volume()
{
	return m_fFXSound_Volume;
}

float UB2UISettingGame::OnBGMVolume_Set(bool bUpDown, float fCur_Value)
{
	if (bUpDown)
	{
		fCur_Value += 0.1;
		if (fCur_Value > 1)
			fCur_Value = 1;
	}
	else
	{
		fCur_Value -= 0.1;
		if (fCur_Value < 0)
			fCur_Value = 0;
	}
	return fCur_Value;
}


float UB2UISettingGame::OnFXSoundVolume_Set(bool bUpDown, float fCur_Value)
{
	if (bUpDown)
	{
		fCur_Value += 0.1;
		if (fCur_Value > 1)
			fCur_Value = 1;
	}
	else
	{
		fCur_Value -= 0.1;
		if (fCur_Value < 0)
			fCur_Value = 0;
	}
	return fCur_Value;
}

int32 UB2UISettingGame::OnValue_TextView(float fValue)
{
	if (fValue == -0.0f)
		fValue = 0.0f;

	fValue *= 10;
	return (int32)fValue;
}

void UB2UISettingGame::OnsetVibration()
{
	// 柳悼
	
	m_bVibration = !m_bVibration;
	SetVibration(m_bVibration);
	Vib_Bundle.SetOnOff(m_bVibration);

	SaveSettingData();
}

void UB2UISettingGame::OnsetVoice()
{
	DevShowNotReadyYetMessage();
	
	// 眠饶 扁瓷 持磊
}

void UB2UISettingGame::OnsetScreenLock()
{	
	m_bScreenLock = !m_bScreenLock;
	B2P_KeepScreenOn(m_bScreenLock);
	ScreenLock_Bundle.SetOnOff(m_bScreenLock);

	SaveSettingData();
}

void UB2UISettingGame::SaveSettingData()
{
	SaveGameSetting_Graphics(m_nGraphicLevel);
	SaveGameSetting_FrameLimit(m_nFrameLimitLevel);
	SaveGameSetting_Resolution(B2ResolutionLevelToInt(GetContextDesiredResolutionLevel(false)));
	SaveGameSetting_BGMVolume(m_fBGM_Volume);
	SaveGameSetting_EffectVolume(m_fFXSound_Volume);
	SaveGameSetting_Vibration((int32)m_bVibration);
	SaveGameSetting_KeepScreenOn((int32)m_bScreenLock);

	GConfig->Flush(false, GB2GeneralSavedStateIni);
}

void UB2UISettingGame::LoadSettingGameData()
{
	bool bCouldNotLoadSome = false;
	if (!LoadGameSetting_Graphics_OrByDefault(m_nGraphicLevel)) {
		// 捞扒 肺爹 角菩 矫 扁夯蔼阑 咯扁辑 持阑 鞘夸绰 绝促. LoadGameSetting_Graphics_OrByDefault 俊辑 贸府瞪 巴.
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_FrameLimit_OrByDefault(m_nFrameLimitLevel)) {
		// 捞扒 肺爹 角菩 矫 扁夯蔼阑 咯扁辑 持阑 鞘夸绰 绝促. LoadGameSetting_FrameLimit_OrByDefault 俊辑 贸府瞪 巴.
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_Resolution_OrByDefault(m_nSafeResolutionLevel)) {
		// 捞扒 肺爹 角菩 矫 扁夯蔼阑 咯扁辑 持阑 鞘夸绰 绝促. LoadGameSetting_Resolution_OrByDefault 俊辑 贸府瞪 巴.
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_BGMVolume(m_fBGM_Volume)) {
		m_fBGM_Volume = 0.7f; // 扁夯蔼
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_EffectVolume(m_fFXSound_Volume)) {
		m_fFXSound_Volume = 0.7f; // 扁夯蔼
		bCouldNotLoadSome = true;
	}
	int32 LoadedVibInt = 0;
	if (!LoadGameSetting_Vibration(LoadedVibInt)) {
		LoadedVibInt = 1; // 扁夯蔼
		bCouldNotLoadSome = true;
	}
	m_bVibration = (LoadedVibInt == 0) ? false : true;
	
	int32 LoadedScreenLockInt = 0;
	if (!LoadGameSetting_KeepScreenOn(LoadedScreenLockInt))
	{
		LoadedScreenLockInt = 1; // 扁夯蔼
		bCouldNotLoadSome = true;
	}
	m_bScreenLock = (LoadedScreenLockInt == 0) ? false : true;


	if (bCouldNotLoadSome)
	{
		SaveSettingData();
	}

#if !UE_BUILD_SHIPPING
	bLoadSettingGameDataCalledOnce = true;
#endif

}

EB2ResolutionLevel UB2UISettingGame::GetContextDesiredResolutionLevel(bool bIsForUI)
{
	//// 犁矫累 饶 利侩登绰 纳捞胶甫 困秦 措何盒狼 版快 m_nSafeResolutionLevel 俊 弊成 立辟窍瘤 富绊 捞吧 荤侩秦具 窃.
	//if (bIsForUI) // UI 窜 钎矫狼 版快父 Reserved 甫 绊妨.
	//{		
	//	EB2ResolutionLevel ReservedLevel = LoadReservedResolutionLevelOfPrevAppRun();
	//	if (ReservedLevel != EB2ResolutionLevel::End)
	//	{
	//		return ReservedLevel;
	//	}
	//}
	return IntToB2ResolutionLevel(m_nSafeResolutionLevel); // 利绢档 郴何俊 利侩登绰 扒 公炼扒 捞吧 结具 窃.
}

void UB2UISettingGame::FOnOffBtnBundle::SetOnOff(bool bOn)
{
	if (IMG_On.IsValid()) {
		IMG_On->SetVisibility(bOn ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (TB_On.IsValid()) {
		TB_On->SetVisibility(bOn ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (IMG_Off.IsValid()) {
		IMG_Off->SetVisibility(bOn ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
	if (TB_Off.IsValid()) {
		TB_Off->SetVisibility(bOn ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
}
void UB2UISettingGame::FOnOffBtnBundle::SetEnable(bool bEnable)
{
	// 捞巴鳖柳 橇肺弊贰赣狼 犁樊栏肺..
	static const FLinearColor EnableColor(1.0f, 1.0f, 1.0f, 1.0f);
	static const FLinearColor DisableColor(0.2f, 0.2f, 0.2f, 0.4f);

	static const float EnableAlphaOnly = 1.0f;
	static const float DisableAlphaOnly = 0.4f;

	if (IMG_On.IsValid()) {
		IMG_On->SetColorAndOpacity(bEnable ? EnableColor : DisableColor);
	}
	if (IMG_Off.IsValid()) { // 荤角惑 捞霸 焊捞摆绰单.. 捞吧沸 阂面盒且 淀.
		IMG_Off->SetColorAndOpacity(bEnable ? EnableColor : DisableColor);
	}
	// 咆胶飘绰 磊眉 祸惑捞 乐绢辑 舅颇父.. 泵祸窍焙.
	if (TB_On.IsValid()) {
		TB_On->SetOpacity(bEnable ? EnableAlphaOnly : DisableAlphaOnly);
	}
	if (TB_Off.IsValid()) {
		TB_Off->SetOpacity(bEnable ? EnableAlphaOnly : DisableAlphaOnly);
	}
}
