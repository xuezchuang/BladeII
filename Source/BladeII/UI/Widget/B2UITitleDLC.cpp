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
// kakao �ӽ�
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

#if PLATFORM_ANDROID  // kakao �α��� delegate ���

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

	// ó�� ���� �� ���� �������� �÷��� �ϴ� ��. ���� Title ������ �÷��̴� ���⼭ ������ ���� �̾ �ϰ� ��.
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
	// ���⼭ �������� ���õ� ������ Ŭ�� �̸� �� �¾� Ű�� �����ؼ� �κ� �ε� ȭ�鿡 ���� ����.
	if (InSelectedSource && inSelectedClass != EPCClass::EPC_End)
	{
		const FString MovieClipName = FPaths::GetCleanFilename(InSelectedSource->GetUrl());
		AB2DLCFrontGameMode* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
		if (LoginGameMode && !MovieClipName.IsEmpty())
		{
			LoginGameMode->SetSelectedFrontTitleMovieInfo(
				FPaths::GetBaseFilename(MovieClipName), // Ȯ���ڰ� ������ �Ѵ�.
				inSelectedClass
			);
		}
	}
}

void UB2UITitleDLC::CloseWidgetDelegate()
{
	B2P_GameExitPopUp("byebye");
}

//[@AKI, 170927][FIXME]Main Thread���� ȣ��Ǵ� ������ �־� �ӽ� �������� ���� ��.
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

	//�������� ������ �����ؾߵ�.
	// PlayTouchAnim �� ���� ����.. �װ� ������ �÷��̵Ǵ� �� ���������� ������ ���� �ɱ��̶� ������ ���� ����. ���� �ɱ�..
	DeferredPlatformServerConnectionHandling(0.1f);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UITitleDLC::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	PlayTouchAnimHandling();
	OnMouseDownHandling();

#if !PLATFORM_IOS // IOS ������ �ߺ� ȣ���� �ȵǾ� �Է� �ϳ��� ����.. �׽�Ʈ �� �ȵ���̵忡���� �ߺ� ȣ��� �ϳ��� �����Ǿ�� ��.
	//�������� ������ �����ؾߵ�.
	// PlayTouchAnim �� ���� ����.. �װ� ������ �÷��̵Ǵ� �� ���������� ������ ���� �ɱ��̶� ������ ���� ����. ���� �ɱ�..
	DeferredPlatformServerConnectionHandling(0.1f);
#endif
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UB2UITitleDLC::PlatformServerConnectionHandling()
{

#if PLATFORM_WINDOWS || defined( NONE_KAKAO_SDK_VERSION ) // ���� īī�� �α����� �Ұ��� PC ���߹��� ��Ʈ. ������ DLCFront ��� ���� ����ڰ� �ƴ� UITitleDLC �� �� �ϵ� ���� ����.
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
	{ // DeferredPlatformServerConnectionHandling ���� �ҷ��� �� �ִ�.
		TheWorld->GetTimerManager().ClearTimer(DeferredPlatformServerConnectTH);
	}
}
void UB2UITitleDLC::DeferredPlatformServerConnectionHandling(float InDeferSecond)
{
	if (InDeferSecond > 0.0f)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld &&
			!DeferredPlatformServerConnectTH.IsValid() // �̹� ��ġ�� ��Ȳ���� �� ��ġ�� �� �ִ�. ����.
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
	// Ÿ�̸Ӹ� ��ġ�ϰų� ���� �ð��� ����ϰų� �ؼ� KakaoLoginDelegate �� ���� �� ���� �ִϸ��̼��� �� �÷��� �� �Ǿ����� ���� ������� �Ѿ�� �� hold �� �� ���� �ִ�.
	// �׷����� UIPatch ���� ������ �ణ �ٲ�� �ϴµ� �����ϰ� ���̰� ���� �ʰ�,
	// �׷��ٰ� PlatformServerConnectionHandling �� ������Ű�� �� �ƴ� �� ����
	// �ܼ��� �÷��� ȣ�⸸ �� ��.

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
	// ���� Dummy �� �ƴ� ��¥ Kakao �α��� �� ��.

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

	// ���� ���Ӿ����尡 �ƴϹǷ� ���� ó���� �̰� �˻��ؼ� ���Ӿ����� Tick ���� ����.
	m_LoginDelegateCalled.AtomicSet(true);
}
#endif

#if (PLATFORM_ANDROID )
void UB2UITitleDLC::KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken)
{
	// ���� Dummy �� �ƴ� ��¥ Kakao �α��� �� ��.

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

	// ���� ���Ӿ����尡 �ƴϹǷ� ���� ó���� �̰� �˻��ؼ� ���Ӿ����� Tick ���� ����.
	m_LoginDelegateCalled.AtomicSet(true);
}

#endif

#if PLATFORM_ANDROID 
void UB2UITitleDLC::KakaoLoginfailDelegate(JNIEnv *env, jobject thiz, jint resultCode)
{
	// ��Ʈ��ũ ����
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

		// ���� ���Ӿ����尡 �ƴϹǷ� ���� ó���� �̰� �˻��ؼ� ���Ӿ����� Tick ���� ����.
		m_LoginDelegateCalled.AtomicSet(true);
	}
	else
	{
#if PLATFORM_IOS
		bool bInitializeFailed = ErrorCode == 3000 || ErrorCode == 3001;
		if (bInitializeFailed && B2P_IsNetworkConnected() == true)
		{
			//��Ʈ��ũ ������ �Ǿ���, �ʱ�ȭ ���и� �ٽ� �õ���.
			KakaotalkAPI::OnInit();
			B2P_KakaoLogin();
			m_firstRequestGetAccount = true;
			return;
		}

		// ��Ʈ��ũ ����
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

	// ���� ������ �Ѿ.
	AB2DLCFrontGameMode* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_Patch, true);
	}

	TurnOffStartMovies();
}

// �̰� ���� �� ���� �ǰ�?
int32 GetFirstCharacterViewState()
{
	int32 RetValue = 0;
	GConfig->GetInt(TEXT("FirstCharacterView"), *FString::Printf(TEXT("isFirst")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}
