#include "B2UITitleDLC.h"
#include "B2UITitleDesign.h"
#include "B2UIEnum.h"
#include "Event.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "BladeIIScalabilityControl.h"
#include "B2AuthInfo.h"
#include "B2UIDummyDLCFront.h"
#include "TutorialManager.h"
#include "B2DLCFrontGameMode.h"
#include "MediaSource.h"
#include "B2MessageInfoCommonStruct.h"
#include "B2AndroidBackManager.h"
// kakao 烙矫
#include "B2PlatformAPI.h"

#include "UnrealEngine.h"

#include "Global.h"
#include "BladeIIUtil.h"

#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#endif

UB2UITitleDLC::UB2UITitleDLC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UB2UITitleDLC::Init()
{
	Super::Init();

	CAPTURE_UOBJECT(UB2UITitleDLC);

	B2AndroidBackManager::GetInstance();
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	m_firstRequestGetAccount = true;
	m_LoginDelegateCalled.AtomicSet(false);

#if PLATFORM_ANDROID  // kakao 肺弊牢 delegate 殿废

#if defined(NONE_KAKAO_SDK_VERSION)
 
	None_Kakao_Sdk_Login();

#else

	FJavaWrapper::OnKakaoLoginResultDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FJavaWrapper::OnKakaoLoginResultDelegate.AddUObject(this, &UB2UITitleDLC::KakaoLoginDelegate);

	FJavaWrapper::OnKakaoLoginFailResultDelegate.Remove(OnKakaoLoginFailHandler);
	OnKakaoLoginFailHandler = FJavaWrapper::OnKakaoLoginFailResultDelegate.AddUObject(this, &UB2UITitleDLC::KakaoLoginfailDelegate);

#endif

#endif
#if PLATFORM_IOS 
	FKakaoTalkPlugin::OnLoggedInDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FKakaoTalkPlugin::OnLoggedInDelegate.AddUObject(this, &UB2UITitleDLC::KakaoLoginDelegate);
#endif

	// 贸澜 角青 饶 罚待 急琶栏肺 敲饭捞 窍绰 镑. 捞饶 Title 悼康惑 敲饭捞绰 咯扁辑 急琶茄 巴阑 捞绢辑 窍霸 凳.
	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->StartMovieRandomPick();
	}

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA
	FString GameInI;
	FConfigCacheIni::LoadGlobalIniFile(GameInI, TEXT("Game"));

	GConfig->GetString(TEXT("CDN"), TEXT("ServiceType"), ServiceType, GameInI);
#else
	ServiceType = TEXT("KAKAO");
#endif

	if (isLocalConnectIniFileExist())
	{
		FString PreServiceType(ServiceType);
		FString LocalString(TEXT("Local"));
		ServiceType = LocalString.ToUpper();
		UE_LOG(LogClass, Log, TEXT("===================================>UITitleDLC isLocalConnectIniFileExist!! Pre Service Type : [%s] -> Changed Service Type : [%s]"), *PreServiceType, *ServiceType);
	}

	UE_LOG(LogClass, Log, TEXT("===================================> ServiceType : %s"), *ServiceType);
}

void UB2UITitleDLC::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UITitleDesign, UIP_TitleDesign);
	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->Init(this);
	}
}

void UB2UITitleDLC::DestroySelf()
{
	Super::DestroySelf();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->DestroySelf();
	}
}

void UB2UITitleDLC::TurnOffStartMovies()
{
	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->TurnOffStartMovies();
	}
}

void UB2UITitleDLC::SetSelectedMediaSourceInfo(UMediaSource* InSelectedSource, EPCClass inSelectedClass)
{
	// 咯扁辑 公累困肺 急琶等 悼康惑 努赋 捞抚 棺 悸诀 虐甫 历厘秦辑 肺厚 肺爹 拳搁俊 静扁 困窃.
	if (InSelectedSource && inSelectedClass != EPCClass::EPC_End)
	{
		const FString MovieClipName = FPaths::GetCleanFilename(InSelectedSource->GetUrl());
		AB2DLCFrontGameMode* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
		if (LoginGameMode && !MovieClipName.IsEmpty())
		{
			LoginGameMode->SetSelectedFrontTitleMovieInfo(
				FPaths::GetBaseFilename(MovieClipName), // 犬厘磊啊 狐廉具 茄促.
				inSelectedClass
			);
		}
	}
}

void UB2UITitleDLC::CloseWidgetDelegate()
{
	B2P_GameExitPopUp("byebye");
}

//[@AKI, 170927][FIXME]Main Thread俊辑 龋免登绰 泅惑捞 乐绢 烙矫 规祈栏肺 函版 窃.
void UB2UITitleDLC::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_LoginDelegateCalled)
	{
		PostKakaoLoginInit();
	}
}

FReply UB2UITitleDLC::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	PlayTouchAnimHandling();

	//辑滚客狼 立加阑 刚历秦具凳.
	// PlayTouchAnim 捞 甸绢皑俊 蝶扼.. 弊霸 场鳖瘤 敲饭捞登绰 吧 焊厘窍瘤绰 臼瘤父 酒林 律陛捞扼档 瘤楷篮 矫难 焊磊. 酒林 律陛..
	DeferredPlatformServerConnectionHandling(0.1f);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UITitleDLC::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	PlayTouchAnimHandling();
	OnMouseDownHandling();

#if !PLATFORM_IOS // IOS 俊辑绰 吝汗 龋免捞 救登绢 涝仿 窍唱绰 阜澜.. 抛胶飘 饶 救靛肺捞靛俊辑档 吝汗 龋免矫 窍唱绰 昏力登绢具 窃.
	//辑滚客狼 立加阑 刚历秦具凳.
	// PlayTouchAnim 捞 甸绢皑俊 蝶扼.. 弊霸 场鳖瘤 敲饭捞登绰 吧 焊厘窍瘤绰 臼瘤父 酒林 律陛捞扼档 瘤楷篮 矫难 焊磊. 酒林 律陛..
	DeferredPlatformServerConnectionHandling(0.1f);
#endif
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UB2UITitleDLC::PlatformServerConnectionHandling()
{

#if PLATFORM_WINDOWS || defined( NONE_KAKAO_SDK_VERSION ) // 角力 墨墨坷 肺弊牢捞 阂啊茄 PC 俺惯滚傈 风飘. 角力肺 DLCFront 葛靛 俺惯 淬寸磊啊 酒囱 UITitleDLC 肺 棵 老档 芭狼 绝促.
	auto* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Patch, true);
	}
	TurnOffStartMovies();
#else
	
	if (m_firstRequestGetAccount)
	{
		B2P_KakaoLogin();
		m_firstRequestGetAccount = false;
	}

#endif

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{ // DeferredPlatformServerConnectionHandling 栏肺 阂啡阑 荐 乐促.
		TheWorld->GetTimerManager().ClearTimer(DeferredPlatformServerConnectTH);
	}
}
void UB2UITitleDLC::DeferredPlatformServerConnectionHandling(float InDeferSecond)
{
	if (InDeferSecond > 0.0f)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld &&
			!DeferredPlatformServerConnectTH.IsValid() // 捞固 磐摹茄 惑炔俊辑 肚 磐摹老 荐 乐促. 公矫.
			)
		{
			TheWorld->GetTimerManager().ClearTimer(DeferredPlatformServerConnectTH);
			TheWorld->GetTimerManager().SetTimer(DeferredPlatformServerConnectTH, this, &UB2UITitleDLC::PlatformServerConnectionHandling, InDeferSecond, false);
		}
	}
	else
	{
		PlatformServerConnectionHandling();
	}
}

void UB2UITitleDLC::PlayTouchAnimHandling()
{
	// 鸥捞赣甫 汲摹窍芭唱 矫累 矫阿阑 扁废窍芭唱 秦辑 KakaoLoginDelegate 啊 吭阑 矫 酒流 局聪皋捞记捞 促 敲饭捞 救 登菌栏搁 促澜 厘搁栏肺 逞绢啊绰 吧 hold 秦 杭 荐档 乐促.
	// 弊凡矾搁 UIPatch 咯绰 备炼甫 距埃 官层具 窍绰单 弊率窍绊 揩捞绊 酵瘤 臼绊,
	// 弊犯促绊 PlatformServerConnectionHandling 阑 瘤楷矫虐绰 扒 酒囱 芭 鞍绊
	// 窜鉴洒 敲饭捞 龋免父 秦 狄.

	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->PlayTouchAnim();
	}
}

void UB2UITitleDLC::OnMouseDownHandling()
{
	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->SetConnectingUI();
	}
}

#if PLATFORM_ANDROID 
void UB2UITitleDLC::None_Kakao_Sdk_Login()
{
	// 咯变 Dummy 啊 酒囱 柳楼 Kakao 肺弊牢 等 芭.

	UE_LOG(LogClass, Log, TEXT("####KakaoLoginDelegate !!!!!!!!!!!!!!"));
	FString KakaoUserID;
	FString KakaoAccessToken;

	/*const char* CharsUserID = env->GetStringUTFChars(userID, 0);
	KakaoUserID = FString(UTF8_TO_TCHAR(CharsUserID));
	env->ReleaseStringUTFChars(userID, CharsUserID);

	const char* charsToken = env->GetStringUTFChars(accessToken, 0);
	KakaoAccessToken = FString(UTF8_TO_TCHAR(charsToken));
	env->ReleaseStringUTFChars(accessToken, charsToken);*/


	KakaoUserID = "hshduce";
	KakaoAccessToken = "token";

	if (KakaoUserID == "")
	{
		m_firstRequestGetAccount = true;
		return;
	}

	UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
	UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

	// set auth data
	FB2AuthInfo::GetInstance().Init();
	FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::AOS); // 1
	FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
	FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);
	FB2AuthInfo::GetInstance().SetLangCode(B2P_GetKakaoLanguageCode());
	FB2AuthInfo::GetInstance().SetCountryCode(B2P_GetKakaoCountryCode());

	FB2AuthInfo::GetInstance().SetPlatformType(int32(b2network::B2PlatformCode::FOR_KAKAO));
	FB2AuthInfo::GetInstance().SetOsType(int32(b2network::B2OSType::AOS));
	FB2AuthInfo::GetInstance().SetMarketType(B2P_GetMarKetType());


	FString sServerAddress = "139.150.90.154:11000";
	FString sTempServerAddress;
	FString sTempPort;

	sServerAddress.Split(TEXT(":"), &sTempServerAddress, &sTempPort);

	FB2AuthInfo::GetInstance().SetSesssionAddr(sTempServerAddress);
	FB2AuthInfo::GetInstance().SetSessionPort(FCString::Atoi(*sTempPort));

	SetSessionServerUrl(sTempServerAddress);

#if FORCE_LOGGING_IN_ANDROID
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>KAKAO ServerIP : %s"), *sTempServerAddress);
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>KAKAO Port : %s"), *sTempPort);
#endif

	UE_LOG(LogClass, Log, TEXT("####sTempServerAddress !!!!!!!!!!!!!! = [[[%s]]]"), *sTempServerAddress);
	UE_LOG(LogClass, Log, TEXT("####sTempPort !!!!!!!!!!!!!! = [[[%d]]]"), FCString::Atoi(*sTempPort));

	// TODO fill any other value

	// 咯变 霸烙静饭靛啊 酒聪骨肺 捞饶 贸府绰 捞吧 八荤秦辑 霸烙静饭靛 Tick 俊辑 荐青.
	m_LoginDelegateCalled.AtomicSet(true);
}
#endif

#if (PLATFORM_ANDROID )
void UB2UITitleDLC::KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken)
{
	// 咯变 Dummy 啊 酒囱 柳楼 Kakao 肺弊牢 等 芭.

	UE_LOG(LogClass, Log, TEXT("####KakaoLoginDelegate !!!!!!!!!!!!!!"));
	FString KakaoUserID;
	FString KakaoAccessToken;

	const char* CharsUserID = env->GetStringUTFChars(userID, 0);
	KakaoUserID = FString(UTF8_TO_TCHAR(CharsUserID));
	env->ReleaseStringUTFChars(userID, CharsUserID);

	const char* charsToken = env->GetStringUTFChars(accessToken, 0);
	KakaoAccessToken = FString(UTF8_TO_TCHAR(charsToken));
	env->ReleaseStringUTFChars(accessToken, charsToken);

	if (KakaoUserID == "")
	{
		m_firstRequestGetAccount = true;
		return;
	}

	UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
	UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

	// set auth data
	FB2AuthInfo::GetInstance().Init();
	FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::AOS); // 1
	FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
	FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);
	FB2AuthInfo::GetInstance().SetLangCode(B2P_GetKakaoLanguageCode());
	FB2AuthInfo::GetInstance().SetCountryCode(B2P_GetKakaoCountryCode());

	FB2AuthInfo::GetInstance().SetPlatformType(int32(b2network::B2PlatformCode::FOR_KAKAO));
	FB2AuthInfo::GetInstance().SetOsType(int32(b2network::B2OSType::AOS));
	FB2AuthInfo::GetInstance().SetMarketType(B2P_GetMarKetType());

	if (ServiceType == TEXT("KAKAO"))
	{
		FString sServerAddress = B2P_GetGameServerAddress();
		FString sTempServerAddress;
		FString sTempPort;

		sServerAddress.Split(TEXT(":"), &sTempServerAddress, &sTempPort);

		FB2AuthInfo::GetInstance().SetSesssionAddr(sTempServerAddress);
		FB2AuthInfo::GetInstance().SetSessionPort(FCString::Atoi(*sTempPort));

		SetSessionServerUrl(sTempServerAddress);

		UE_LOG(LogClass, Log, TEXT("####sTempServerAddress !!!!!!!!!!!!!! = [[[%s]]]"), *sTempServerAddress);
		UE_LOG(LogClass, Log, TEXT("####sTempPort !!!!!!!!!!!!!! = [[[%d]]]"), FCString::Atoi(*sTempPort));

#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>KAKAO ServerIP : %s"), *sServerAddress);
#endif

	}
	else if (ServiceType == TEXT("LOCAL"))
	{
		LoadLocalIni();
	}
	else
	{
		FB2AuthInfo::GetInstance().SetSesssionAddr("172.168.2.104");
		FB2AuthInfo::GetInstance().SetSessionPort(11000);
	}
	// TODO fill any other value

	// 咯变 霸烙静饭靛啊 酒聪骨肺 捞饶 贸府绰 捞吧 八荤秦辑 霸烙静饭靛 Tick 俊辑 荐青.
	m_LoginDelegateCalled.AtomicSet(true);
}

#endif

#if PLATFORM_ANDROID 
void UB2UITitleDLC::KakaoLoginfailDelegate(JNIEnv *env, jobject thiz, jint resultCode)
{
	// 匙飘况农 楷搬
	if (AB2DLCFrontGameMode* DLCGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		DLCGameMode->ShowKakaoErrorPopup(resultCode);
	}
	m_firstRequestGetAccount = true;
}
#endif

#if PLATFORM_IOS
void UB2UITitleDLC::KakaoLoginDelegate(bool IsSuccess, int32 ErrorCode)
{
	if (IsSuccess)
	{
		UE_LOG(LogClass, Log, TEXT("####KakaoLoginDelegate !!!!!!!!!!!!!!"));

		FString KakaoUserID = KakaotalkAPI::GetKakaoUserID();
		FString KakaoAccessToken = KakaotalkAPI::GetKakaoAccessToken();

		if (KakaoUserID == "")
		{
			m_firstRequestGetAccount = true;
			return;
		}
		UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
		UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

		FB2AuthInfo::GetInstance().Init();
		FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::IOS); // 2
		FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
		FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);

		FB2AuthInfo::GetInstance().SetLangCode(B2P_GetKakaoLanguageCode());
		FB2AuthInfo::GetInstance().SetCountryCode(B2P_GetKakaoCountryCode());

		FB2AuthInfo::GetInstance().SetPlatformType(int32(b2network::B2PlatformCode::FOR_KAKAO));
		FB2AuthInfo::GetInstance().SetOsType(int32(b2network::B2OSType::IOS));
		FB2AuthInfo::GetInstance().SetMarketType(B2P_GetMarKetType());

		if (ServiceType == TEXT("KAKAO"))
		{
			FString sServerAddress = B2P_GetGameServerAddress();
			FString sTempServerAddress;
			FString sTempPort;

			sServerAddress.Split(TEXT(":"), &sTempServerAddress, &sTempPort);

			FB2AuthInfo::GetInstance().SetSesssionAddr(sTempServerAddress);
			FB2AuthInfo::GetInstance().SetSessionPort(FCString::Atoi(*sTempPort));

			SetSessionServerUrl(sTempServerAddress);

		}
		else if (ServiceType == TEXT("LOCAL"))
		{
			LoadLocalIni();
		}
		else
		{
			FB2AuthInfo::GetInstance().SetSesssionAddr("172.168.2.104");
			FB2AuthInfo::GetInstance().SetSessionPort(11000);
		}

		// 咯变 霸烙静饭靛啊 酒聪骨肺 捞饶 贸府绰 捞吧 八荤秦辑 霸烙静饭靛 Tick 俊辑 荐青.
		m_LoginDelegateCalled.AtomicSet(true);
	}
	else
	{
#if PLATFORM_IOS
		bool bInitializeFailed = ErrorCode == 3000 || ErrorCode == 3001;
		if (bInitializeFailed && B2P_IsNetworkConnected() == true)
		{
			//匙飘况农 楷搬捞 登菌绊, 檬扁拳 角菩搁 促矫 矫档窃.
			KakaotalkAPI::OnInit();
			B2P_KakaoLogin();
			m_firstRequestGetAccount = true;
			return;
		}

		// 匙飘况农 楷搬
		if (AB2DLCFrontGameMode* DLCGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			DLCGameMode->ShowKakaoErrorPopup(ErrorCode);
		}
#endif
		FString KakaoUserID = KakaotalkAPI::GetKakaoUserID();
		if (KakaoUserID == "")
		{
			m_firstRequestGetAccount = true;
			return;
		}
	}
}
#endif

void UB2UITitleDLC::LoadLocalIni()
{

	FString ServerType;
	GConfig->GetString(TEXT("NetworkSettings"), TEXT("ServerType"), ServerType, GB2NetworkIni);

	FString NetworkSettingCategory = TEXT("NetworkSetting_Public");

	if (ServerType == TEXT("Alpha"))
	{
		NetworkSettingCategory = TEXT("NetworkSetting_Alpha");
	}
	else if (ServerType == TEXT("Beta"))
	{
		NetworkSettingCategory = TEXT("NetworkSetting_Beta");
	}
	else if (ServerType == TEXT("Local"))
	{
		NetworkSettingCategory = TEXT("NetworkSetting_Local");
	}

	FString sTempServerAddress;
	int32 sTempPort;

	GConfig->GetString(*NetworkSettingCategory, TEXT("SESSION_IP"), sTempServerAddress, GB2NetworkIni);
	GConfig->GetInt(*NetworkSettingCategory, TEXT("SESSION_PORT"), sTempPort, GB2NetworkIni);

	FB2AuthInfo::GetInstance().SetSesssionAddr(sTempServerAddress);
	FB2AuthInfo::GetInstance().SetSessionPort(sTempPort);

#if FORCE_LOGGING_IN_ANDROID
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>LoadLocalIni ServerIP : %s"), *sTempServerAddress);
#endif

	UE_LOG(LogClass, Log, TEXT("####Local sTempServerAddress !!!!!!!!!!!!!! = [[[%s]]]"), *sTempServerAddress);
	UE_LOG(LogClass, Log, TEXT("####Local sTempPort !!!!!!!!!!!!!! = [[[%d]]]"), sTempPort);
}

void UB2UITitleDLC::PostKakaoLoginInit()
{
	check(m_LoginDelegateCalled && IsInGameThread());

	// 促澜 纠栏肺 逞绢皑.
	AB2DLCFrontGameMode* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Patch, true);
	}

	TurnOffStartMovies();
}

// 捞扒 捞力 救 静绰 扒啊?
int32 GetFirstCharacterViewState()
{
	int32 RetValue = 0;
	GConfig->GetInt(TEXT("FirstCharacterView"), *FString::Printf(TEXT("isFirst")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}
