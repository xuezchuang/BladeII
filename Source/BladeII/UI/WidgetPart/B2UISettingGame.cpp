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
	m_nFrameLimitLevel = B2FrameLevelToInt(EB2FrameLimitLevel::Mid); // FrameLimit 랑 ResolutionLevel 은 기본값을 Mid 로 하는 게 좋다.
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
		// On/Off 상태에 따른 디자인 적용을 위해 동일 위치의 상태에 따른 버튼 및 텍스트들을 한데 묶음.
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
	if (P_ScreenLock_Vib_Set.IsValid()) { // 예전에 하단 설정 버튼 세트를 On/Off 하는 조건이 있었던 모양?
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

	// 프레임 리미트와 해상도AA 설정 버튼 활성화 여부는 처음 한번만 업데이트 해주면 됨.
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

	// 나머지 버튼 핸들러는 블루프린트에서 바인딩.
}

void UB2UISettingGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UISettingGame::OnClick_BackToSuggested()
{
	// 그래픽 레벨 뿐 아니라 다른 성능 관련한 것도 기본값 돌려놔 보자..

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
	// UISettingGame 이 로비뿐 아니라 다른 게임모드에서도 불릴 수 있으므로 게임모드 별 설정을 할 수 있도록 WorldContextobject 를 넣어주는 것이 필요하다.
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
	// 추후 disable 된 건 상위 패널 collapse 를 고려..

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
	//{ // 이번 런타임 스위칭은 안 되니 m_nSafeResolutionLevel 을 바꾸지는 말고 다음 번 실행시 적용을 위해 예약만 해 둠.
	//	ReserveResolutionLevelForNextAppStart(SelectedLevel);
	//	bPopupMsgForReservedApply = true;
	//}
	//else
	//{ // 이전에 예약해 두었던 게 있더라도 클리어 함.
	//	ClearResolutionLevelReserve();
	//	m_nSafeResolutionLevel = B2ResolutionLevelToInt(SelectedLevel);
	//}

	//SaveSettingData();

	//// 앞뒤로 FlushRenderingCommands 를 하는 건 MobileContentScaleFactor 가 변경되는 상황에서 아마도 렌더타겟 재할당하면서 동작이 불안정해지는 징후가 있었기 때문.
	//// 적어도 이게 MobileContentScaleFactor 변경 시 불안정한 동작을 막아주는 건 거의 확실함.
	//FlushRenderingCommands();
	//// 기본 그래픽 설정과 맞물려 돌아가는 설정임
	//B2Scalability::AdjustScalabilityBySelectedLevel(this, IntToB2GraphicsLevel(m_nGraphicLevel), GetContextDesiredResolutionLevel(false));
	//FlushRenderingCommands();

	//SetResAAButtonSelectedState(GetContextDesiredResolutionLevel(true));

	//if (bPopupMsgForReservedApply)
	//{
	//	// 재시작 후 적용되는 것에 대한 안내 팝업
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
	// 추후 disable 된 건 collapse 를 고려..

	if (BTN_ResAA_High.IsValid())
	{
		bool bIsEnable = B2Scalability::IsResolutionLevelAvailable(EB2ResolutionLevel::High);
		BTN_ResAA_High->SetIsEnabled(bIsEnable);
		ResAA_High_Bundle.SetEnable(bIsEnable); // 버튼 자체의 disable 컬러로는 부족해서 자체적인 색상 보정을 좀 한다.
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
	// 실수로 순서 꼬이지 않게 체크. 최소 한번 저장한 로컬 설정을 로딩한 후에 이걸 사용해야 한다.
	check(bLoadSettingGameDataCalledOnce);
#endif

	// 로딩한 현재 설정값에 따라 버튼 On/Off 상태 세팅.
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
	// 진동
	
	m_bVibration = !m_bVibration;
	SetVibration(m_bVibration);
	Vib_Bundle.SetOnOff(m_bVibration);

	SaveSettingData();
}

void UB2UISettingGame::OnsetVoice()
{
	DevShowNotReadyYetMessage();
	
	// 추후 기능 넣자
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
		// 이건 로딩 실패 시 기본값을 여기서 넣을 필요는 없다. LoadGameSetting_Graphics_OrByDefault 에서 처리될 것.
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_FrameLimit_OrByDefault(m_nFrameLimitLevel)) {
		// 이건 로딩 실패 시 기본값을 여기서 넣을 필요는 없다. LoadGameSetting_FrameLimit_OrByDefault 에서 처리될 것.
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_Resolution_OrByDefault(m_nSafeResolutionLevel)) {
		// 이건 로딩 실패 시 기본값을 여기서 넣을 필요는 없다. LoadGameSetting_Resolution_OrByDefault 에서 처리될 것.
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_BGMVolume(m_fBGM_Volume)) {
		m_fBGM_Volume = 0.7f; // 기본값
		bCouldNotLoadSome = true;
	}
	if (!LoadGameSetting_EffectVolume(m_fFXSound_Volume)) {
		m_fFXSound_Volume = 0.7f; // 기본값
		bCouldNotLoadSome = true;
	}
	int32 LoadedVibInt = 0;
	if (!LoadGameSetting_Vibration(LoadedVibInt)) {
		LoadedVibInt = 1; // 기본값
		bCouldNotLoadSome = true;
	}
	m_bVibration = (LoadedVibInt == 0) ? false : true;
	
	int32 LoadedScreenLockInt = 0;
	if (!LoadGameSetting_KeepScreenOn(LoadedScreenLockInt))
	{
		LoadedScreenLockInt = 1; // 기본값
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
	//// 재시작 후 적용되는 케이스를 위해 대부분의 경우 m_nSafeResolutionLevel 에 그냥 접근하지 말고 이걸 사용해야 함.
	//if (bIsForUI) // UI 단 표시의 경우만 Reserved 를 고려.
	//{		
	//	EB2ResolutionLevel ReservedLevel = LoadReservedResolutionLevelOfPrevAppRun();
	//	if (ReservedLevel != EB2ResolutionLevel::End)
	//	{
	//		return ReservedLevel;
	//	}
	//}
	return IntToB2ResolutionLevel(m_nSafeResolutionLevel); // 적어도 내부에 적용되는 건 무조건 이걸 써야 함.
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
	// 이것까진 프로그래머의 재량으로..
	static const FLinearColor EnableColor(1.0f, 1.0f, 1.0f, 1.0f);
	static const FLinearColor DisableColor(0.2f, 0.2f, 0.2f, 0.4f);

	static const float EnableAlphaOnly = 1.0f;
	static const float DisableAlphaOnly = 0.4f;

	if (IMG_On.IsValid()) {
		IMG_On->SetColorAndOpacity(bEnable ? EnableColor : DisableColor);
	}
	if (IMG_Off.IsValid()) { // 사실상 이게 보이겠는데.. 이걸론 불충분할 듯.
		IMG_Off->SetColorAndOpacity(bEnable ? EnableColor : DisableColor);
	}
	// 텍스트는 자체 색상이 있어서 알파만.. 궁색하군.
	if (TB_On.IsValid()) {
		TB_On->SetOpacity(bEnable ? EnableAlphaOnly : DisableAlphaOnly);
	}
	if (TB_Off.IsValid()) {
		TB_Off->SetOpacity(bEnable ? EnableAlphaOnly : DisableAlphaOnly);
	}
}
