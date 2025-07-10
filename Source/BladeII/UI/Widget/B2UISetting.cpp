#include "B2UISetting.h"
#include "B2UIManager.h"
#include "UnrealEngine.h"
#include "BladeIIPlayer.h"
#include "Retailer.h"
#include "BladeIIGameMode.h"
#include "B2UIDLCFrontPopup.h"
#include "B2UISettingAccount.h"
#include "B2UISettingGame.h"
#include "B2UISettingAddOns.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"

extern void AndroidThunkCpp_SetCharacterInfoScene(bool bSet);
#endif

UB2UISetting::UB2UISetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Created_UIP_Setting_Game = nullptr;
	Created_UIP_Setting_AddOns = nullptr;
	Created_UIP_Setting_Account = nullptr;
	Created_UIP_Setting_Info = nullptr;
}

void UB2UISetting::Init()
{
	Super::Init();
	SetViewVibBtn(false);
#if PLATFORM_ANDROID // kakao 로그인 delegate 등록
	FJavaWrapper::OnKakaoCouponSucessDelegate.Remove(OnKakaoCouponHandler);
	OnKakaoCouponHandler = FJavaWrapper::OnKakaoCouponSucessDelegate.AddUObject(this, &UB2UISetting::KakaoCouponDelegate);
	
#endif
	m_CouponDelegateCalled.AtomicSet(false);
	
	SelectSubMenu(EUISettingSubMenu::subMenu_game);
}

void UB2UISetting::CacheAssets()
{	
	GET_SLOT(UButton, BTN_Exit)

	GET_SLOT(UButton, BTN_Setting_Game)
	GET_SLOT(UButton, BTN_Setting_AddOns)
	GET_SLOT(UButton, BTN_Setting_Account)
	GET_SLOT(UButton, BTN_Setting_Info)

#if PLATFORM_IOS
		GET_SLOT(UPanelWidget, X_Coupon)
		if (X_Coupon.IsValid())
			X_Coupon->SetVisibility(ESlateVisibility::Collapsed);
#endif		

	GET_SLOT(UTextBlock, TB_Setting_Title)

	GET_SLOT(UTextBlock, TXT_Setting_Game)
	GET_SLOT(UTextBlock, TXT_Setting_AddOns)
	GET_SLOT(UTextBlock, TXT_Setting_Account)
	GET_SLOT(UTextBlock, TXT_Setting_Info)

	GET_SLOT(UCanvasPanel, X_CP_UIP_Setting_Game)
	GET_SLOT(UCanvasPanel, X_CP_UIP_Setting_AddOns)
	GET_SLOT(UCanvasPanel, X_CP_UIP_Setting_Account)
	GET_SLOT(UCanvasPanel, X_CP_UIP_Setting_Info)
	
	GET_SLOT(UB2UISettingMsgPopup, UI_MsgPopupSetting_01)
	if (UI_MsgPopupSetting_01.IsValid()) {
		UI_MsgPopupSetting_01->Init();
		UI_MsgPopupSetting_01->SetPopupType(EPopupTypeSetting::EPopupType_KakaoLogin);
		UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
	}
		
	GET_SLOT(UTextBlock, TB_Setting_Game_Unselected)
	GET_SLOT(UTextBlock, TB_Setting_Game_Selected)
	GET_SLOT(UTextBlock, TB_Setting_AddOns_Unselected)
	GET_SLOT(UTextBlock, TB_Setting_AddOns_Selected)
	GET_SLOT(UTextBlock, TB_Setting_Account_Unselected)
	GET_SLOT(UTextBlock, TB_Setting_Account_Selected)
	GET_SLOT(UTextBlock, TB_Setting_Info_Unselected)
	GET_SLOT(UTextBlock, TB_Setting_Info_Selected)

	GET_SLOT(UImage, IMG_Setting_Game_Unselected)
	GET_SLOT(UImage, IMG_Setting_Game_Selected)
	GET_SLOT(UImage, IMG_Setting_AddOns_Unselected)
	GET_SLOT(UImage, IMG_Setting_AddOns_Selected)
	GET_SLOT(UImage, IMG_Setting_Account_Unselected)
	GET_SLOT(UImage, IMG_Setting_Account_Selected)
	GET_SLOT(UImage, IMG_Setting_Info_Unselected)
	GET_SLOT(UImage, IMG_Setting_Info_Selected)


	GET_SLOT(UB2UIWidget, UIP_Secession)
	if (UIP_Secession.IsValid()) {
		UIP_Secession->Init();
		UIP_Secession->SetVisibility(ESlateVisibility::Hidden);
	}
	
	UpdateStaticText();
}

void UB2UISetting::UpdateStaticText()
{
	if (TB_Setting_Title.IsValid())
		TB_Setting_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Setup")));
	if (TXT_Setting_Game.IsValid())
		TXT_Setting_Game->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GameTab")));
	if (TXT_Setting_AddOns.IsValid())
		TXT_Setting_AddOns->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_AdditionalFunction")));
	if (TXT_Setting_Account.IsValid())
		TXT_Setting_Account->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_AccountTab")));
	if (TXT_Setting_Info.IsValid())
		TXT_Setting_Info->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Info")));
	// 위에 TXT_Setting_~ 들은 안 쓰는 건가..?
	if (TB_Setting_Game_Unselected.IsValid())
		TB_Setting_Game_Unselected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GameTab")));
	if (TB_Setting_Game_Selected.IsValid())
		TB_Setting_Game_Selected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GameTab")));
	if (TB_Setting_AddOns_Unselected.IsValid())
		TB_Setting_AddOns_Unselected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_AdditionalFunction")));
	if (TB_Setting_AddOns_Selected.IsValid())
		TB_Setting_AddOns_Selected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_AdditionalFunction")));
	if (TB_Setting_Account_Unselected.IsValid())
		TB_Setting_Account_Unselected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_AccountTab")));
	if (TB_Setting_Account_Selected.IsValid())
		TB_Setting_Account_Selected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_AccountTab")));
	if (TB_Setting_Info_Unselected.IsValid())
		TB_Setting_Info_Unselected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Info")));
	if (TB_Setting_Info_Selected.IsValid())
		TB_Setting_Info_Selected->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_Info")));

	TextBlockWidget.Empty();
	TextBlockWidget_UN.Empty();
	if (TB_Setting_Game_Selected.IsValid())
		TextBlockWidget.Add(TB_Setting_Game_Selected.Get());
	if (TB_Setting_Game_Unselected.IsValid())
		TextBlockWidget_UN.Add(TB_Setting_Game_Unselected.Get());
	if (TB_Setting_AddOns_Selected.IsValid())
		TextBlockWidget.Add(TB_Setting_AddOns_Selected.Get());
	if (TB_Setting_AddOns_Unselected.IsValid())
		TextBlockWidget_UN.Add(TB_Setting_AddOns_Unselected.Get());
	if (TB_Setting_Account_Selected.IsValid())
		TextBlockWidget.Add(TB_Setting_Account_Selected.Get());
	if (TB_Setting_Account_Unselected.IsValid())
		TextBlockWidget_UN.Add(TB_Setting_Account_Unselected.Get());
	if (TB_Setting_Info_Selected.IsValid())
		TextBlockWidget.Add(TB_Setting_Info_Selected.Get());
	if (TB_Setting_Info_Unselected.IsValid())
		TextBlockWidget_UN.Add(TB_Setting_Info_Unselected.Get());

	Img_Widget.Empty();
	Img_Widget_UN.Empty();
	if (IMG_Setting_Game_Selected.IsValid())
		Img_Widget.Add(IMG_Setting_Game_Selected.Get());
	if (IMG_Setting_Game_Unselected.IsValid())
		Img_Widget_UN.Add(IMG_Setting_Game_Unselected.Get());
	if (IMG_Setting_AddOns_Selected.IsValid())
		Img_Widget.Add(IMG_Setting_AddOns_Selected.Get());
	if (IMG_Setting_AddOns_Unselected.IsValid())
		Img_Widget_UN.Add(IMG_Setting_AddOns_Unselected.Get());
	if (IMG_Setting_Account_Selected.IsValid())
		Img_Widget.Add(IMG_Setting_Account_Selected.Get());
	if (IMG_Setting_Account_Unselected.IsValid())
		Img_Widget_UN.Add(IMG_Setting_Account_Unselected.Get());
	if (IMG_Setting_Info_Selected.IsValid())
		Img_Widget.Add(IMG_Setting_Info_Selected.Get());
	if (IMG_Setting_Info_Unselected.IsValid())
		Img_Widget_UN.Add(IMG_Setting_Info_Unselected.Get());
	
}


void UB2UISetting::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Setting_Game, &UB2UISetting::OnclickBtnGame)
	BIND_CLICK_FUNC_TO_BTN(BTN_Setting_AddOns, &UB2UISetting::OnclickBtnAddOns)
	BIND_CLICK_FUNC_TO_BTN(BTN_Setting_Account, &UB2UISetting::OnclickBtnAccount)
	BIND_CLICK_FUNC_TO_BTN(BTN_Setting_Info, &UB2UISetting::OnclickBtnInfo)
		
	BIND_CLICK_FUNC_TO_BTN(BTN_Exit, &UB2UISetting::OnclickBtnExit)

}

void UB2UISetting::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (m_CouponDelegateCalled)
	{
		data_trader::Retailer::GetInstance().RequestKakaoMessageProcessing("inbox");
		m_CouponDelegateCalled.AtomicSet(false);
	}
}
void UB2UISetting::DestroySelf(class UB2UIManager* InUIManager)
{
	DestroyUIPSettingGame();
	DestroyUIPSettingAddOns();
	DestroyUIPSettingAccount();
	DestroyUIPSettingInfo();

	Super::DestroySelf(InUIManager);
}

void UB2UISetting::SelectSubMenu(EUISettingSubMenu submenu)
{
	ConditionalCreateSubUIP(submenu); // 필요시 생성

	for(int32 SI = 0; SI < GetAllUISettingSubMenuNum(); ++SI)
	{
		EUISettingSubMenu ThisSubMenuEnum = static_cast<EUISettingSubMenu>(SI);

		UB2UIWidget** ThisSubWidgetPtr = subWidgets.Find(ThisSubMenuEnum);
		UB2UIWidget* ThisSubWidget = ThisSubWidgetPtr ? *ThisSubWidgetPtr : nullptr;
		UTextBlock* ThisTextBlockWidget = TextBlockWidget.IsValidIndex(SI) ? TextBlockWidget[SI] : nullptr;
		UTextBlock* ThisTextBlockWidget_UN = TextBlockWidget_UN.IsValidIndex(SI) ? TextBlockWidget_UN[SI] : nullptr;
		UImage* ThisImgWidget = Img_Widget.IsValidIndex(SI) ? Img_Widget[SI] : nullptr;
		UImage* ThisImgWidget_UN = Img_Widget_UN.IsValidIndex(SI) ? Img_Widget_UN[SI] : nullptr;

		if (ThisSubMenuEnum == submenu)
		{
			if (ThisSubWidget) {
				ThisSubWidget->SetVisibility(ESlateVisibility::Visible);
			}
			if (ThisTextBlockWidget) {
				ThisTextBlockWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			if (ThisTextBlockWidget_UN) {
				ThisTextBlockWidget_UN->SetVisibility(ESlateVisibility::Hidden);
			}
			if (ThisImgWidget) {
				ThisImgWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			if (ThisImgWidget_UN) {
				ThisImgWidget_UN->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			if (ThisSubWidget) { // 아직 생성 안된 건 여기서 없을 수 있다.
				ThisSubWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			if (ThisTextBlockWidget) {
				ThisTextBlockWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			if (ThisTextBlockWidget_UN) {
				ThisTextBlockWidget_UN->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			if (ThisImgWidget) {
				ThisImgWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			if (ThisImgWidget_UN) {
				ThisImgWidget_UN->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
}


void UB2UISetting::ShowKakaoCommunityView()
{
	B2P_KakaoCommunityView();
}

void UB2UISetting::ShowKakaoCoupon()
{
	B2P_ShowKakaoCoupon();
}

void UB2UISetting::KakaoLoadMessage()
{
	// B2P_KakaoLoadMessage("inbox");
//	data_trader::Retailer::GetInstance().RequestKakaoMessageProcessing("couponbox");
	//B2P_ShowKakaoPromotionView();

	DevShowNotReadyYetMessage();
}

void UB2UISetting::ShowKakaoNotice()
{
	B2P_ShowKakaoNoticeView(false, "");	
}

void UB2UISetting::ShowCredit()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::Credit);
}

void UB2UISetting::RestartTutorial()
{
	GoToTutorialClass<bool>::GetInstance().Signal(true);
}

void UB2UISetting::ShowGachaView()
{
	// 가챠 확률 공개
	B2P_ShowKakaoGachaOddsView();
}

void UB2UISetting::CloseWidgetDelegate()
{
	OnclickBtnExit();
}

#if PLATFORM_ANDROID
void UB2UISetting::KakaoCouponDelegate(JNIEnv *env, jobject thiz)
{
	m_CouponDelegateCalled.AtomicSet(true);
}
#endif

// 진동
//ABladeIIGameMode* B2GM = GetBladeIIGameMode(this);

//	B2GM->SetUseForceFeedback(true);

//====================================================================================
//									Click Handler
//====================================================================================

void UB2UISetting::OnclickBtnExit()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::Setting);
}

void UB2UISetting::OnclickBtnGame()
{
	UB2UISetting::SelectSubMenu(EUISettingSubMenu::subMenu_game);
}

void UB2UISetting::OnclickBtnAddOns()
{
	UB2UISetting::SelectSubMenu(EUISettingSubMenu::subMenu_addons);
}

void UB2UISetting::OnclickBtnAccount()
{
	UB2UISetting::SelectSubMenu(EUISettingSubMenu::subMenu_account);
}

void UB2UISetting::OnclickBtnInfo()
{
	UB2UISetting::SelectSubMenu(EUISettingSubMenu::subMenu_info);
}


//====================================================================================
//									UIP dynamic load & create
//====================================================================================
void UB2UISetting::ConditionalCreateSubUIP(EUISettingSubMenu InSubMenu)
{
	switch (InSubMenu)
	{
	case EUISettingSubMenu::subMenu_game:
		ConditionalCreateUIPSettingGame();
		break;
	case EUISettingSubMenu::subMenu_addons:
		ConditionalCreateUIPSettingAddOns();
		break;
	case EUISettingSubMenu::subMenu_account:
		ConditionalCreateUIPSettingAccount();
		break;
	case EUISettingSubMenu::subMenu_info:
		ConditionalCreateUIPSettingInfo();
		break;
	}
}
void UB2UISetting::ConditionalCreateUIPSettingGame()
{
	if (!Created_UIP_Setting_Game && X_CP_UIP_Setting_Game.IsValid())
	{
		Created_UIP_Setting_Game = DynLoadAndCreateInCanvasPanelFull<UB2UISettingGame>(UIP_Setting_Game_Class, this, X_CP_UIP_Setting_Game.Get());
		if (Created_UIP_Setting_Game)
		{
			Created_UIP_Setting_Game->Init();
			AddNewCreatedSubMenuUIPtoArray(EUISettingSubMenu::subMenu_game, Created_UIP_Setting_Game);
		}
	}
}
void UB2UISetting::ConditionalCreateUIPSettingAddOns()
{
	if (!Created_UIP_Setting_AddOns && X_CP_UIP_Setting_AddOns.IsValid())
	{
		Created_UIP_Setting_AddOns = DynLoadAndCreateInCanvasPanelFull<UB2UISettingAddOns>(UIP_Setting_AddOns_Class, this, X_CP_UIP_Setting_AddOns.Get());
		if (Created_UIP_Setting_AddOns)
		{
			Created_UIP_Setting_AddOns->Init();
			AddNewCreatedSubMenuUIPtoArray(EUISettingSubMenu::subMenu_addons, Created_UIP_Setting_AddOns);
		}
	}
}
void UB2UISetting::ConditionalCreateUIPSettingAccount()
{
	if (!Created_UIP_Setting_Account && X_CP_UIP_Setting_Account.IsValid())
	{
		Created_UIP_Setting_Account = DynLoadAndCreateInCanvasPanelFull<UB2UISettingAccount>(UIP_Setting_Account_Class, this, X_CP_UIP_Setting_Account.Get());
		if (Created_UIP_Setting_Account)
		{
			Created_UIP_Setting_Account->Init();
			Created_UIP_Setting_Account->SetOwnerUI(this);
			AddNewCreatedSubMenuUIPtoArray(EUISettingSubMenu::subMenu_account, Created_UIP_Setting_Account);
		}
	}
}
void UB2UISetting::ConditionalCreateUIPSettingInfo()
{
	if (!Created_UIP_Setting_Info && X_CP_UIP_Setting_Info.IsValid())
	{
		Created_UIP_Setting_Info = DynLoadAndCreateInCanvasPanelFull<UB2UIWidget>(UIP_Setting_Info_Class, this, X_CP_UIP_Setting_Info.Get());
		if (Created_UIP_Setting_Info)
		{
			Created_UIP_Setting_Info->Init();
			AddNewCreatedSubMenuUIPtoArray(EUISettingSubMenu::subMenu_info, Created_UIP_Setting_Info);
		}
	}
}
void UB2UISetting::AddNewCreatedSubMenuUIPtoArray(EUISettingSubMenu InSubMenu, class UB2UIWidget* NewCreatedWidget)
{
	int32 MySubMenuIndex = GetUISettingSubMenuIndex(InSubMenu);
	if (NewCreatedWidget)
	{
		UB2UIWidget*& FoundOrAddEntry = subWidgets.FindOrAdd(InSubMenu);
		FoundOrAddEntry = NewCreatedWidget;
	}
}
void UB2UISetting::DestroyUIPSettingGame()
{
	if (Created_UIP_Setting_Game)
	{
		Created_UIP_Setting_Game->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_Setting_Game = nullptr;
	}
}
void UB2UISetting::DestroyUIPSettingAddOns()
{
	if (Created_UIP_Setting_AddOns)
	{
		Created_UIP_Setting_AddOns->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_Setting_AddOns = nullptr;
	}
}
void UB2UISetting::DestroyUIPSettingAccount()
{
	if (Created_UIP_Setting_Account)
	{
		Created_UIP_Setting_Account->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_Setting_Account = nullptr;
	}
}
void UB2UISetting::DestroyUIPSettingInfo()
{
	if (Created_UIP_Setting_Info)
	{
		Created_UIP_Setting_Info->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_Setting_Info = nullptr;
	}
}