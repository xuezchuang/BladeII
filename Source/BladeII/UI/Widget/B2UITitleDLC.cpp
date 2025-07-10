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
// kakao 임시
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

#if PLATFORM_ANDROID  // kakao 로그인 delegate 등록

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

	// 처음 실행 후 랜덤 선택으로 플레이 하는 곳. 이후 Title 동영상 플레이는 여기서 선택한 것을 이어서 하게 됨.
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
	// 여기서 무작위로 선택된 동영상 클립 이름 및 셋업 키를 저장해서 로비 로딩 화면에 쓰기 위함.
	if (InSelectedSource && inSelectedClass != EPCClass::EPC_End)
	{
		const FString MovieClipName = FPaths::GetCleanFilename(InSelectedSource->GetUrl());
		AB2DLCFrontGameMode* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
		if (LoginGameMode && !MovieClipName.IsEmpty())
		{
			LoginGameMode->SetSelectedFrontTitleMovieInfo(
				FPaths::GetBaseFilename(MovieClipName), // 확장자가 빠져야 한다.
				inSelectedClass
			);
		}
	}
}

void UB2UITitleDLC::CloseWidgetDelegate()
{
	B2P_GameExitPopUp("byebye");
}

//[@AKI, 170927][FIXME]Main Thread에서 호출되는 현상이 있어 임시 방편으로 변경 함.
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

	//서버와의 접속을 먼저해야됨.
	// PlayTouchAnim 이 들어감에 따라.. 그게 끝까지 플레이되는 걸 보장하지는 않지만 아주 쪼금이라도 지연은 시켜 보자. 아주 쪼금..
	DeferredPlatformServerConnectionHandling(0.1f);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UITitleDLC::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	PlayTouchAnimHandling();
	OnMouseDownHandling();

#if !PLATFORM_IOS // IOS 에서는 중복 호출이 안되어 입력 하나는 막음.. 테스트 후 안드로이드에서도 중복 호출시 하나는 삭제되어야 함.
	//서버와의 접속을 먼저해야됨.
	// PlayTouchAnim 이 들어감에 따라.. 그게 끝까지 플레이되는 걸 보장하지는 않지만 아주 쪼금이라도 지연은 시켜 보자. 아주 쪼금..
	DeferredPlatformServerConnectionHandling(0.1f);
#endif
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UB2UITitleDLC::PlatformServerConnectionHandling()
{

#if PLATFORM_WINDOWS || defined( NONE_KAKAO_SDK_VERSION ) // 실제 카카오 로그인이 불가한 PC 개발버전 루트. 실제로 DLCFront 모드 개발 담당자가 아닌 UITitleDLC 로 올 일도 거의 없다.
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
	{ // DeferredPlatformServerConnectionHandling 으로 불렸을 수 있다.
		TheWorld->GetTimerManager().ClearTimer(DeferredPlatformServerConnectTH);
	}
}
void UB2UITitleDLC::DeferredPlatformServerConnectionHandling(float InDeferSecond)
{
	if (InDeferSecond > 0.0f)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld &&
			!DeferredPlatformServerConnectTH.IsValid() // 이미 터치한 상황에서 또 터치일 수 있다. 무시.
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
	// 타이머를 설치하거나 시작 시각을 기록하거나 해서 KakaoLoginDelegate 가 왔을 시 아직 애니메이션이 다 플레이 안 되었으면 다음 장면으로 넘어가는 걸 hold 해 볼 수도 있다.
	// 그럴러면 UIPatch 여는 구조를 약간 바꿔야 하는데 그쪽하고 엮이고 싶지 않고,
	// 그렇다고 PlatformServerConnectionHandling 을 지연시키는 건 아닌 거 같고
	// 단순히 플레이 호출만 해 둠.

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
	// 여긴 Dummy 가 아닌 진짜 Kakao 로그인 된 거.

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

	// 여긴 게임쓰레드가 아니므로 이후 처리는 이걸 검사해서 게임쓰레드 Tick 에서 수행.
	m_LoginDelegateCalled.AtomicSet(true);
}
#endif

#if (PLATFORM_ANDROID )
void UB2UITitleDLC::KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken)
{
	// 여긴 Dummy 가 아닌 진짜 Kakao 로그인 된 거.

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

	// 여긴 게임쓰레드가 아니므로 이후 처리는 이걸 검사해서 게임쓰레드 Tick 에서 수행.
	m_LoginDelegateCalled.AtomicSet(true);
}

#endif

#if PLATFORM_ANDROID 
void UB2UITitleDLC::KakaoLoginfailDelegate(JNIEnv *env, jobject thiz, jint resultCode)
{
	// 네트워크 연결
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

		// 여긴 게임쓰레드가 아니므로 이후 처리는 이걸 검사해서 게임쓰레드 Tick 에서 수행.
		m_LoginDelegateCalled.AtomicSet(true);
	}
	else
	{
#if PLATFORM_IOS
		bool bInitializeFailed = ErrorCode == 3000 || ErrorCode == 3001;
		if (bInitializeFailed && B2P_IsNetworkConnected() == true)
		{
			//네트워크 연결이 되었고, 초기화 실패면 다시 시도함.
			KakaotalkAPI::OnInit();
			B2P_KakaoLogin();
			m_firstRequestGetAccount = true;
			return;
		}

		// 네트워크 연결
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

	// 다음 씬으로 넘어감.
	AB2DLCFrontGameMode* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Patch, true);
	}

	TurnOffStartMovies();
}

// 이건 이제 안 쓰는 건가?
int32 GetFirstCharacterViewState()
{
	int32 RetValue = 0;
	GConfig->GetInt(TEXT("FirstCharacterView"), *FString::Printf(TEXT("isFirst")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}
