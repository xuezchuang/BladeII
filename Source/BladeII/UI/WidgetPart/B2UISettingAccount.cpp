// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UISettingAccount.h"
#include "B2UIManager.h"

#include "B2UIManager.h"
#include "UnrealEngine.h"

#include "B2UISetting.h"
#include "Retailer.h"

#include "Global.h"
#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"
#include "B2Version.h"
#include "BladeIIGameImpl.h"

#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#include "Android/AndroidJNI.h"
#endif
#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#include "IOSRuntimeSettings.h"
#endif

void UB2UISettingAccount::CacheAssets()
{
//	Super::CacheAssets();
//	GET_SLOT(UB2Button, B2Button_358);
//	GET_SLOT(UB2Image, B2Image_0);
//	GET_SLOT(UB2Button, BTN_Facebook);
//	GET_SLOT(UB2Button, BTN_Google_GameCenter);
//	GET_SLOT(UB2Button, BTN_Logout);
//	GET_SLOT(UB2Button, BTN_DeleteAccount);
//	GET_SLOT(UTextBlock, TXT_UserID);
//	GET_SLOT(UB2Image, IMG_KakaoLogo);
//	GET_SLOT(UB2Button, BTN_KakaoConnect);
//	GET_SLOT(UTextBlock, TXT_Google);
//	GET_SLOT(UTextBlock, TXT_GameVer);
//	GET_SLOT(UTextBlock, TXT_AccLink);
//
//	GET_SLOT(UB2Button, B2Button_Kakaogoogle);
//
//	GET_SLOT(UB2UIDLCFrontPopup, UI_MsgPopupSetting01)
//		if (UI_MsgPopupSetting01.IsValid()) {
//			UI_MsgPopupSetting01->Init();
//			UI_MsgPopupSetting01->SetVisibility(ESlateVisibility::Hidden);
//		}
//
//	if (IMG_KakaoLogo.IsValid() && BTN_KakaoConnect.IsValid())
//	{
//		if (GetIDPCode() == EPlatformIDPCode::Guest)
//		{
//			IMG_KakaoLogo->SetVisibility(ESlateVisibility::Hidden);
//			BTN_KakaoConnect->SetVisibility(ESlateVisibility::Visible);
//			B2Button_Kakaogoogle->SetVisibility(ESlateVisibility::Hidden);
//		}
//		else
//		{
//			IMG_KakaoLogo->SetVisibility(ESlateVisibility::Visible);
//			BTN_KakaoConnect->SetVisibility(ESlateVisibility::Hidden);
//#if !PLATFORM_IOS
//			B2Button_Kakaogoogle->SetVisibility(ESlateVisibility::Visible);
//#endif
//		}
//	}
//
//	if (TXT_Google.IsValid())
//	{
//		if (B2P_IsGoogleLogin())
//		{
//			TXT_Google->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GoogleLogOut")));
//		}
//		else
//		{
//			TXT_Google->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GoogleLogIn")));
//		}
//	}
//
//	SetVersionInfo();
//
//#if PLATFORM_IOS
//	if (B2Button_Kakaogoogle.IsValid())
//	{
//		B2Button_Kakaogoogle->SetVisibility(ESlateVisibility::Hidden);
//	}
//#endif
}

void UB2UISettingAccount::ReloadUI()
{
//	if (IMG_KakaoLogo.IsValid() && BTN_KakaoConnect.IsValid())
//	{
//		if (GetIDPCode() == EPlatformIDPCode::Guest)
//		{
//			IMG_KakaoLogo->SetVisibility(ESlateVisibility::Hidden);
//			BTN_KakaoConnect->SetVisibility(ESlateVisibility::Visible);
//			B2Button_Kakaogoogle->SetVisibility(ESlateVisibility::Hidden);
//		}
//		else
//		{
//			IMG_KakaoLogo->SetVisibility(ESlateVisibility::Visible);
//			BTN_KakaoConnect->SetVisibility(ESlateVisibility::Hidden);
//#if !PLATFORM_IOS
//			B2Button_Kakaogoogle->SetVisibility(ESlateVisibility::Visible);
//#endif
//		}
//	}
//
//	if (TXT_Google.IsValid())
//	{
//		if (B2P_IsGoogleLogin())
//		{
//			TXT_Google->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GoogleLogOut")));
//		}
//		else
//		{
//			TXT_Google->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GoogleLogIn")));
//		}
//	}
//
//	if (TXT_UserID.IsValid())
//		TXT_UserID->SetText(FText::FromString(B2P_GetKakaoUserID()));
//
//	SetVersionInfo();
}
void UB2UISettingAccount::SetVersionInfo()
{
	if (TXT_GameVer.IsValid())
	{
		FText DisplayInfo;
		FText TextFotmat;
		FText AppVersion = FText::FromString(TEXT(PREPROCESSOR_TO_STRING(VERSION)));
		FText CLText = FText::FromString(BladeIIGameImpl::GetClientDataStore().GetChangelistNum());
		if(CLText.IsEmpty())
			CLText = FText::FromString(TEXT(PREPROCESSOR_TO_STRING(CL)));
#if PLATFORM_ANDROID
		const UAndroidRuntimeSettings* Settings = GetDefault<UAndroidRuntimeSettings>();
		AppVersion = FText::FromString(Settings->VersionDisplayName);
#elif PLATFORM_IOS
		/* // 捞 蔼篮 给 佬绢 棵芭烙. Engine.ini 乐绰 蔼牢单 捞 蔼 救靖
		//const UIOSRuntimeSettings* Settings = GetDefault<UIOSRuntimeSettings>();
		//AppVersion = Settings->VersionInfo;
		*/
#endif

		TextFotmat = FText::FromString(TEXT("{0}.{1}"));
		DisplayInfo = FText::Format(TextFotmat, AppVersion, CLText);
#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_LIVE // !B2_BUILD_LEVEL_LIVE
		//[@AKI, 181004] 泅犁绰 BuildNumText啊 荤侩窍瘤 臼绰 蔼捞扼 林籍 贸府 窃
		//FText BuildNumText = FText::FromString(TEXT(PREPROCESSOR_TO_STRING(BUILDNUMBER)));
		FText ServerVersion = FText::FromString(BladeIIGameImpl::GetClientDataStore().GetServerVersionInfo());
		if (ServerVersion.IsEmpty())
			BladeIIGameImpl::GetClientDataStore().RequestGetServerVersion();

		TextFotmat = FText::FromString(TEXT("{0}.{1} ({2}) (s{3})"));
		DisplayInfo = FText::Format(TextFotmat, AppVersion, CLText, GetBuildInfo(), ServerVersion);
#endif //B2_BUILD_LEVEL < B2_BUILD_LEVEL_LIVE 

		TXT_GameVer->SetText(DisplayInfo);
	}
}

void UB2UISettingAccount::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(B2Button_358, &UB2UISettingAccount::OnClickB2Button_358);
	BIND_CLICK_FUNC_TO_BTN(BTN_Facebook, &UB2UISettingAccount::OnClickBTN_Facebook);
	BIND_CLICK_FUNC_TO_BTN(BTN_Google_GameCenter, &UB2UISettingAccount::OnClickBTN_Google_GameCenter);
	BIND_CLICK_FUNC_TO_BTN(BTN_Logout, &UB2UISettingAccount::OnClickBTN_Logout);
	BIND_CLICK_FUNC_TO_BTN(BTN_DeleteAccount, &UB2UISettingAccount::OnClickBTN_DeleteAccount);
}

void UB2UISettingAccount::Init()
{
	Super::Init();
	if (TXT_UserID.IsValid())
		TXT_UserID->SetText(FText::FromString(B2P_GetKakaoUserID()));
	bSubscribed = false;
	m_KakaoLinkDelegateCalled.AtomicSet(false);
	SubscribeEvent();
#if PLATFORM_ANDROID // kakao 肺弊牢 delegate 殿废
	FJavaWrapper::OnKakaoLinkCompletedDelegate.Remove(OnKakaoLinkHandler);
	OnKakaoLinkHandler = FJavaWrapper::OnKakaoLinkCompletedDelegate.AddUObject(this, &UB2UISettingAccount::KakaoLinkDelegate);
#elif PLATFORM_IOS
	FKakaoTalkPlugin::OnConnectDelegate.Remove(OnKakaoLinkHandler);
	OnKakaoLinkHandler = FKakaoTalkPlugin::OnConnectDelegate.AddUObject(this, &UB2UISettingAccount::KakaoLinkDelegate);
#endif
}


#if PLATFORM_ANDROID
void UB2UISettingAccount::KakaoLinkDelegate(JNIEnv *env, jobject thiz)
{
	UE_LOG(LogClass, Log, TEXT("####KakaoLinkDelegate !!!!!!!!!!!!!! = [[[]]]"));
	UB2UISettingAccount::ReloadUI();
	MarkKakaoLink();

}
#elif PLATFORM_IOS
void UB2UISettingAccount::KakaoLinkDelegate(bool bIsSuccess)
{
	UE_LOG(LogClass, Log, TEXT("####KakaoLinkDelegate !!!!!!!!!!!!!! = [[[]]]"));
	if (bIsSuccess)
	{
		UB2UISettingAccount::ReloadUI();
		MarkKakaoLink();
	}
}
#endif

void UB2UISettingAccount::DestroySelf(UB2UIManager* InUIManager)
{
#if PLATFORM_IOS
	FKakaoTalkPlugin::OnConnectDelegate.Remove(OnKakaoLinkHandler);
#endif
	UnSubscribeEvent();
	Super::DestroySelf(InUIManager);
}

void UB2UISettingAccount::OnClickB2Button_358()
{

}

void UB2UISettingAccount::OnClickBTN_Facebook()
{

}

void UB2UISettingAccount::OnClickBTN_Google_GameCenter()
{

}

void UB2UISettingAccount::OnClickBTN_Logout()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			FText::FromString(TEXT("[Temporary] Logout delete all locally saved state!")), // 措帆 捞繁 郴侩狼 版绊 巩磊啊 甸绢啊摆瘤..
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([]()
		{
			//
			// TODO 肺弊酒眶 扁瓷
			//

			ClearLocalIniSavedState(); // 肺弊酒眶篮 拌沥 昏力唱 聚 犁汲摹客 悼鞭栏肺 摹绊 肺拿 历厘 惑怕甫 朝覆.
#if WITH_EDITOR && !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("俺惯滚傈 皋矫瘤: 肺拿 霸烙 惑怕 历厘 单捞磐绰 朝酒艾澜.")));
#endif
		}
			)
			);
	}
}
void UB2UISettingAccount::SetOwnerUI(UB2UISetting* owner)
{
	owner_ = owner;
}

void UB2UISettingAccount::OnClickBTN_DeleteAccount()
{
	/*UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
	UIMgrInst->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	FText::FromString(TEXT("[Temporary] Deleting account...")), // 购啊 版绊 巩磊啊 甸绢啊摆瘤..
	0.f,
	true,
	true,
	EUIMsgPopupButtonGroup::ConfirmOrCancel,
	FMsgPopupOnClick::CreateLambda([]()
	{
	//
	// TODO 拌沥 昏力
	//

	ClearLocalIniSavedState(); // 咯变 寸楷洒 肺拿 历厘 惑怕甫 朝覆.
	#if WITH_EDITOR && !PLATFORM_MAC
	FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("俺惯滚傈 皋矫瘤: 肺拿 霸烙 惑怕 历厘 单捞磐绰 朝酒艾澜.")));
	#endif
	}
	)
	);
	}*/
	owner_->UIP_Secession->SetVisibility(ESlateVisibility::Visible);
}

void UB2UISettingAccount::KakaoConnect()
{
	owner_->UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
	B2P_KakaoAccountLink();
}

void UB2UISettingAccount::KakaoPopupHidden()
{
	owner_->UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UISettingAccount::KakaoDisConnect()
{
	B2P_KakaoLogout();
}

void UB2UISettingAccount::KakaoGoogleConnect()
{
	if (B2P_IsGoogleLogin())
	{
		B2P_KG_GoogleLogOut();
		TXT_Google->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GoogleLogIn")));
	}
	else
	{
		B2P_KG_GoogleLogin();
		TXT_Google->SetText(BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_GoogleLogOut")));
	}
}

void UB2UISettingAccount::KakaoGoogleDisConnect()
{
	B2P_KG_GoogleLogOut();
}

void UB2UISettingAccount::GameSecessionBtn()
{
	B2P_KakaoUnregister();
	// 咯扁 唱吝俊 呕硼贸府
}

void UB2UISettingAccount::ShowAchievementsView()
{
	B2P_ShowAchivementView();
}

EPlatformIDPCode UB2UISettingAccount::GetIDPCode()
{
	//return B2P_GetKakaoIDPCode();
	return EPlatformIDPCode::None;
}

void UB2UISettingAccount::SetClipBoard()
{
	B2P_SetClipboardString(TXT_UserID->GetText().ToString());
}

void UB2UISettingAccount::OpenKakaoPopup()
{
	FString sIDPCode = TEXT("kakaocapri");

	// 辑滚 率栏肺 快急 八刘阑 困窍咯 焊晨
	data_trader::Retailer::GetInstance().RequestKakaoAccountConversionPrepare(sIDPCode);

}

void UB2UISettingAccount::OnShowCSView()
{
	// 绊按季磐
	B2P_ShowCsView();
}


void UB2UISettingAccount::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_KakaoLinkDelegateCalled)
	{
		FString sIDPCode = TEXT("kakaocapri");

		UE_LOG(LogClass, Log, TEXT("####RequestKakaoAccountConversionComplete sIDPCode!!!!!!!!!!!!!! = [[[%s]]]"), *sIDPCode);
		data_trader::Retailer::GetInstance().RequestKakaoAccountConversionComplete(sIDPCode);
		m_KakaoLinkDelegateCalled.AtomicSet(false);
	}
}

void UB2UISettingAccount::SubscribeEvent()
{
	//if (bSubscribed == false)
	//{
	//	CAPTURE_UOBJECT(UB2UISettingAccount);

	//	DeliveryKakaoAccountConversionPrepareTicket = DeliveryKakaoAccountConversionPrepareClass<FB2ReciveKakaoAccountConversionPrepare>::GetInstance().Subscribe(
	//		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryKakaoAccountConversionPrepare, FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing)
	//		Capture->OnReceiveKakaoAccountConversionPrepare(KakaoMessageProcessing);
	//	END_CAPTURE_OBJECT()
	//		);

	//	DeliveryKakaoAccountConversionCompleteTicket = DeliveryKakaoAccountConversionCompleteClass<FB2ReciveKakaoAccountConversionComplete>::GetInstance().Subscribe(
	//		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryKakaoAccountConversionComplete, FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing)
	//		Capture->OnReceiveKakaoAccountConversionComplete(KakaoMessageProcessing);
	//	END_CAPTURE_OBJECT()
	//		);

	//	bSubscribed = true;
	//}
}

void UB2UISettingAccount::UnSubscribeEvent()
{
	//DeliveryKakaoAccountConversionPrepareClass<FB2ReciveKakaoAccountConversionPrepare>::GetInstance().Unsubscribe(DeliveryKakaoAccountConversionPrepareTicket);
	//DeliveryKakaoAccountConversionCompleteClass<FB2ReciveKakaoAccountConversionComplete>::GetInstance().Unsubscribe(DeliveryKakaoAccountConversionCompleteTicket);

	//DeliveryKakaoAccountConversionPrepareTicket = 0;
	//DeliveryKakaoAccountConversionCompleteTicket = 0;
}


void UB2UISettingAccount::OnCloseComplete()
{

	Super::OnCloseComplete();

}

void UB2UISettingAccount::OnReceiveKakaoAccountConversionPrepare(FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing)
{
	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoAccountConversionPrepare RESPONSE SUCESS."));
	//auto RawRewardPtr = GET_TUPLE_DATA(FB2ResponseKakaoAccountConversionPrepare::rewards_index, KakaoMessageProcessing);

	//for (auto RawRewardSet : RawRewardPtr)
	//{
	//	owner_->UI_MsgPopupSetting_01->SetRewardItem(RawRewardSet->id, RawRewardSet->count);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet id: %d"), RawRewardSet->id);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet count.: %d"), RawRewardSet->count);

	//}

	//owner_->UI_MsgPopupSetting_01->SetOwnerSettingUI(this);
	//owner_->UI_MsgPopupSetting_01->SetVisibility(ESlateVisibility::Visible);
}

void UB2UISettingAccount::OnReceiveKakaoAccountConversionComplete(FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing)
{
	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoAccountConversionComplete RESPONSE SUCESS."));

	//TArray<b2network::B2RawRewardPtr> RawRewardPtr = GET_TUPLE_DATA(FB2ResponseKakaoAccountConversionComplete::rewards_index, KakaoMessageProcessing);

	//for (auto RawRewardSet : RawRewardPtr)
	//{
	//	owner_->UI_MsgPopupSetting_01->SetRewardItem(RawRewardSet->id, RawRewardSet->count);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet id: %d"), RawRewardSet->id);
	//	UE_LOG(LogBladeII, Warning, TEXT("OnReceiveKakaoMessageProcessing RESPONSE RawRewardSet count.: %d"), RawRewardSet->count);
	//}
}


void UB2UISettingAccount::MarkKakaoLink()
{
	// PostKakaoLoginInit 俊辑狼 匙飘况农 檬扁拳尔 AccountInfo 夸没阑 窍绊磊 窍绰 扒单 窜瘤 促弗 静饭靛俊辑 阂副 快妨啊 乐栏骨肺 茄窜拌甫 歹 芭模促.
	m_KakaoLinkDelegateCalled.AtomicSet(true);
}
