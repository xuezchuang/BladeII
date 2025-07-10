#include "B2UITitleScreen.h"
#include "B2UIEnum.h"
#include "Event.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "BladeIIScalabilityControl.h"
#include "B2AuthInfo.h"
#include "B2UIDummyDLCFront.h"
#include "B2LobbyGameMode.h"
#include "TutorialManager.h"

// kakao 임시
#include "B2PlatformAPI.h"
#include "B2UITitleDesign.h"
// 최종적으로는 B2UITitleDLC 에 있는 타이틀 화면 기능이 정식 취급이고 그쪽 공통 기능을 여기서 쓰는 게 있음.
#include "B2UITitleDLC.h"
#include "B2DLCFrontGameMode.h"

#include "UnrealEngine.h"

#include "Global.h"
#include "BladeIIUtil.h"
#include "BladeIIBlockToSyncNetwork.h"
#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#endif
#include "BladeIIBlockToSyncNetwork.h"
#include "B2NetworkBridge.h"

//#include "B2IAPGenericPlatform.h"

static bool gbNetworkInitialized = false;

UB2UITitleScreen::UB2UITitleScreen(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DevDummyDLCFront = nullptr;

	// 이걸 true 로 쓰는 건 AccountInfo 로딩 시 그냥 정해진 이미지 쓰는 건데
	// 이 경우 LoadingImageInfo 의 GetLobbyLoginLoadingImageName 에서도 마찬가지 이미지 경로를 리턴하도록 할 필요가 있다.
	// 안 그러면 중간에 잠깐 다른 이미지가 나올 것이다.
	bUseAlternativeLoadingImage = false;
}

void UB2UITitleScreen::Init()
{
	Super::Init();

	CAPTURE_UOBJECT(UB2UITitleScreen);

	DoneToGetAccountTicket = DoneToGetAccountClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DoneToGetAccount)
			Capture->CheckNewTutorial();
		END_CAPTURE_OBJECT()
	);

	FailResponseMasterDataTicket = FailResponseMasterDataClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(FailResponseMasterData)
		Capture->SetFailResponseMasterData();
	END_CAPTURE_OBJECT()
		);

	m_firstRequestGetAccount = true;
	m_LoginDelegateCalled.AtomicSet(false);
	FBladeIIBlockToSyncNetwork::GetInstance().SetIsRequestMasterData(false);

#if PLATFORM_ANDROID // 진짜 kakao 로그인 delegate 등록
	FJavaWrapper::OnKakaoLoginResultDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FJavaWrapper::OnKakaoLoginResultDelegate.AddUObject(this, &UB2UITitleScreen::KakaoLoginDelegate);
#endif
#if PLATFORM_IOS 
	FKakaoTalkPlugin::OnLoggedInDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FKakaoTalkPlugin::OnLoggedInDelegate.AddUObject(this, &UB2UITitleScreen::KakaoLoginDelegate);
#endif

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 && !BUILD_STANDALONE_TEST_APK
	//// 네트워크 초기화가 안 된 상태에서는 DLC 절차를 상기시켜주는 더미를 띄우는데 타 팀 배포에서는 괜히 혼선만 가중될 거 같으니 이걸 스킵하는 실행인자를 준비한다.
	//if (!IsFinalDLCCookRun() 
	//	&& !FBladeIIBlockToSyncNetwork::GetInstance().GetIsNetworkInit()
	//	&& !UB2UIDummyDLCFront::bWentThroughRealDLCFront
	//	&& !(FParse::Param(FCommandLine::Get(), TEXT("NonDLCFrontDummy")))
	//	)
	//{
	//	CreateDummyDLCFront();
	//}
#endif

#if !UE_BUILD_SHIPPING
	if (CheckAndSetupForTitleMovieCaptureMode())
	{
		// 타이틀 동영상 캡쳐 모드로 처리됨. 다른 셋업 패스
	}
	else
#endif
//	if (IsFinalDLCCookRun() 
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
//		|| UB2UIDummyDLCFront::bWentThroughRealDLCFront
//#endif
//		)
//	{
//		// DLC 루트에서 여긴 Account Info 요청하고 로비 진입 대기하는 잠시 지나가는 곳이 된다.
//		// DLC Front 에서 Title 을 보여주게 된 후 여기서는 account info 받는 동안의 일시적인 가림막 역할.
//		// 별도 이미지 혹은 시작 flow 일관성을 위해 title 동영상 사용.
//		// Tick 이 돌아가기도 하고, ChangeScene 대기중이라 하더라도 B2GameLoadingScreen 을 사용할만한 환경은 아니다.
//		// 한계가 있지만 여기서 UI 로 띄우는 걸로다 가림막을 삼아야 한다.
//		
//		if (bUseAlternativeLoadingImage)
//		{
//			ShowAlternativeLoadingScreen();
//		}
//		else
//		{ // 이 경우 타이틀 동영상 연장. 동영상이 잘 돌아가는 건 아니고 멈춘 상태로 보일 것.
//			if (IMG_Alternative_LobbyLoading.IsValid())
//			{
//				IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
//			}
//			if(UIP_TitleDesign.IsValid())
//			{
//				// DLCTitle 화면에서 랜덤선택한 클래스로 동영상을 연장 재생.
//				// 여기서 틱을 받는 시간은 짧으므로 그 사이에 뭔가 그럴싸한 외관을 보여주려면 Animation 요소 없이 바로 시작해야.
//				UIP_TitleDesign->StartMovieImmediate(AB2DLCFrontGameMode::GetSelectedFrontTitleMovieClass());
//				UIP_TitleDesign->SetVisibility(ESlateVisibility::HitTestInvisible);
//				// 여기 동영상 캡쳐한 로딩 이미지와 맞추느라 기타 CI 들 안 보이게 처리.
//				UIP_TitleDesign->SetShowOtherCIs(false);
//			}
//			ShowExtraLoadingMessage(true);
//		}		
//		
//		// AccountInfo 응답 받을 때까지 대기를 타긴 하겠지만 이상적인 상황이면 매우 짧으므로 바로 여기서 GameLobbyStartDirect 를 콜하지는 않는다.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{ 		// 적당히 시간 지연 두고 AccountInfo 및 로비 메인 진입 시도
//			// 여기 시간 지연이 은근 민감한데, 시간 지연이 전혀 없으면 동작에 문제는 없지만 화면이 시커매진 채로 대기타게 되고,
//			// 시간 지연이 넘 짧으면 서버 초기화 직후 뭔가와 얽혀서 일시적인 서버 연결 장애 현상이 나타날 수 있다.
//			const float TimeDelayToNetworkInitAndGetAccount = 0.5f;
//			TheWorld->GetTimerManager().SetTimer(DelayedGameLobbyStartTH, this, &UB2UITitleScreen::DelayedGameLobbyStartDirect, 
//				TimeDelayToNetworkInitAndGetAccount, 
//				false);
//		}
//
//		// 적어도 메인쓰레드 틱이 돌아가는 와중에는 로딩 중임을 알 수 있도록 뭐라도 돌린다.
//		ShowMiscLoadingIconClass<bool>::GetInstance().Signal(true);
//		if (TheWorld)
//		{
//			// AccountInfo 오기 전까지 시간이 지나면 BladeIIBlockToSyncNetwork 의 네트워크 대기 indicator 가 뜰 것이므로
//			// 적당히 시간 주고 여긴 닫도록 타이머 설치. 타이밍이 크게 중요한 건 아니니 시간은 여기서 지정.
//			TheWorld->GetTimerManager().SetTimer(CloseMiscLoadingIndicatorTH, this, &UB2UITitleScreen::CloseMiscLoadingIndicator,
//				2.0f,  false);
//		}
//	}
//	else
//	{ 
//		// 이 루트는 개발버전 용 (내지는 혹여나 DLC 배포를 안하게 될 때?) 
//		// TitleDLC 를 통해 띄우지 못한 Title 화면을 대신 여기서라도 띄움.
//		if (UIP_TitleDesign.IsValid())
//		{
//			UIP_TitleDesign->SetVisibility(ESlateVisibility::Visible); // 이 경우 이걸 Visible 로 해 주어야 여기서 필요한 터치 신호를 받을 수 있음.
//			UIP_TitleDesign->StartMovieRandomPick();
//		}
//		if (IMG_Alternative_LobbyLoading.IsValid())
//		{ // 여기선 AlternativeLoadingImage 를 사용하는 경우라도 일단 숨겨둠. 이따가 보임..
//			IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
//		}
//	}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 이 개발 보조용 테스트 플래그의 역할은 여기서 다 하는 걸로 침.
	UB2UIDummyDLCFront::bWentThroughRealDLCFront = false;
#endif

#if !PLATFORM_MAC
	// 인앱결제 초기화
	//B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->Reset();
#endif
}

void UB2UITitleScreen::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UITitleDesign, UIP_TitleDesign);
	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->Init(this);
	}
	GET_SLOT(UImage, IMG_Alternative_LobbyLoading);

	GET_SLOT(UCanvasPanel, PN_DLC_COOK);
	if (PN_DLC_COOK.IsValid())
	{ // 특별히 필요하면 띄운다.. 웬만하면 이걸로 덮으면 안될 꺼 같은데..
		PN_DLC_COOK->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UTextBlock, TB_ExtraLoadingMessage);
}

void UB2UITitleScreen::ShowExtraLoadingMessage(bool bShow)
{
	// DLC 배포 빌드 실행 시 로비 진입 직전의 마지막 관문에서 보는 메시지가 됨.
	if (TB_ExtraLoadingMessage.IsValid())
	{
		TB_ExtraLoadingMessage->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		if (bShow)
		{
			TB_ExtraLoadingMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type02")));
		}
	}
}

void UB2UITitleScreen::ShowAlternativeLoadingScreen()
{
	// 반대로 쓰는 경우는 의도적으로 제공하지 않음. 그건 상황에 따라 좀 달라서.
	if (IMG_Alternative_LobbyLoading.IsValid())
	{
		IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->SetVisibility(ESlateVisibility::Hidden);
	}
	ShowExtraLoadingMessage(true);
}

#if WITH_EDITOR
// 에디터의 경우 TitleScreen 을 여러번 열면서 원치 않게 AccountInfo 를 여러번 요청하다가 꼬이는 경우가 좀 있어서.. 이미 AccountInfo 를 받은 상황이면 TitleScreen 에서 AccountInfo 를 요청하지 않도록.
bool gbAlreadyGotAccountOnce_ForTitleScreen = false;
#endif

//[@AKI, 170927][FIXME]Main Thread에서 호출되는 현상이 있어 임시 방편으로 변경 함.
void UB2UITitleScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_LoginDelegateCalled)
	{
		PostKakaoLoginInit();
	}
}

FReply UB2UITitleScreen::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
#if BUILD_STANDALONE_TEST_APK
	GoToStandaloneTestScene();
#else
	//서버와의 접속을 먼저해야됨.
	PlatformServerConnectionHandling();
#endif

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UITitleScreen::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
#if BUILD_STANDALONE_TEST_APK
	GoToStandaloneTestScene();
#elif !PLATFORM_IOS		// IOS 에서는 중복 호출이 안되어 입력 하나는 막음.. 테스트 후 안드로이드에서도 중복 호출시 하나는 삭제되어야 함.
	//서버와의 접속을 먼저해야됨.
	PlatformServerConnectionHandling();
#endif
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

#if BUILD_STANDALONE_TEST_APK // 터치하면 오프라인 모드로 그냥 로비 메인 진입하려는 거.. 몇가지 깔맞춤 해 두고.
void UB2UITitleScreen::GoToStandaloneTestScene()
{
	// 6성장비 맞춤
	extern int32 gForcedPlayerDummyPartGrade;
	gForcedPlayerDummyPartGrade = 6;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
	}
	GEngine->DeferredCommands.Add(TEXT("CombatStatScale 100000000")); // 웬만한 전투에서 죽지 않게 전투력 뻥튀기
	// 로비 메인 화면 직행. 딴 데선 서버 연결 안되게 하고..
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
	// 여캐 샘플 낄낄
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGM)
	{
		LobbyGM->SetPCSelection(EPCClass::EPC_Assassin, EPCClass::EPC_Wizard);
	}
}
#endif

void UB2UITitleScreen::PlatformServerConnectionHandling()
{
	////////////////////////////////////////////
	// 타이틀 화면 터치하면 불리는 기본 루틴.
	// 여기서 조건에 따라 분기를 하지만 최종적으로 PostKakaoLoginInit 으로 수렴한다.
	////////////////////////////////////////////
#ifdef NONE_KAKAO_SDK_VERSION 

	if (!FBladeIIBlockToSyncNetwork::GetInstance().GetIsNetworkInit())
	{
		FB2NetworkBridge::GetInstance()->Shutdown();
		FB2NetworkBridge::GetInstance()->Init();
		FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(true);
	}
	TutorialManager::GetInstance().RequestGetTutorial();


#if FORCE_LOGGING_IN_ANDROID
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> PlatformServerConnectionHandling"));
#endif

	return;
#endif 

//
//	if (IsFinalDLCCookRun() )
//	{
//#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA
//
//		// 개발 테스트 시 이쪽을 타게 하고 외부 버전은 카카오 로그인 없이 바로 마스터 데이터 호출
//		B2P_KakaoLogin();
//#else
//		MarkKakaoLogin();
//#endif
//	}
//	else
//	{
//#if WITH_EDITOR
//		if (GIsEditor && gbAlreadyGotAccountOnce_ForTitleScreen)
//		{
//			// 에디터의 경우 타이틀 화면에 여러번 접근이 가능하므로 이미 AccountInfo 를 받은 상황이라면 다시 요청하지 않고 바로 로비 직행
//			//CheckNewTutorial();
//			//ResponseGetTutorial에서 LastClearTutorialID받아서 CheckNewTutorial()실행
//
//			if (!FBladeIIBlockToSyncNetwork::GetInstance().GetIsNetworkInit())
//			{
//				FB2NetworkBridge::GetInstance()->Shutdown();
//				FB2NetworkBridge::GetInstance()->Init();
//				FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(true);
//			}
//			TutorialManager::GetInstance().RequestGetTutorial();
//		}
//		else
//#endif
//		{
//			// 기타 개발버전 및 테스트 쿠킹에서의 루트
//			BeginFakeKakaoLogin(); // 실질적으로 쉽핑에서 여기로 올 일이 있을지 모르겠는데 일말의 가능성은 남겨둔다.
//		}
//	}
}

void UB2UITitleScreen::DestroySelf(class UB2UIManager* InUIManager)
{
	DoneToGetAccountClass<>::GetInstance().Unsubscribe(DoneToGetAccountTicket);
	FailResponseMasterDataClass<>::GetInstance().Unsubscribe(FailResponseMasterDataTicket);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	DestroyDummyDLCFront();
#endif

	// 상황에 따라 이걸 닫는 타이머가 미처 작동하기 전 로비 메인에 진입하게 될 수도 있음.
	CloseMiscLoadingIndicator();

	if (UIP_TitleDesign.IsValid())
	{
		UIP_TitleDesign->DestroySelf();
	}

	Super::DestroySelf(InUIManager);
}

void UB2UITitleScreen::CheckNewTutorial()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UITitleScreen::CheckNewTutorial"));

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// 설치 후 처음 실행될 PreRender 전용 레벨을 먼저 실행하고 여기로 다시 돌아온 후에 로비 혹은 튜토리얼 진입을 하도록 한다.
	// 단, 정상적인 DLC 루트대로라면 DLCFrontGameMode 에서 PreRender 레벨로 직행하므로 여기서 PreRenderPending 에 걸릴 일은 거의 없을 것이다.
	if (!(LobbyGM && LobbyGM->IsFirstTimePreRenderPending()))
	{	
		// 기존 : 서버와 클라이언트 둘다 체크.. 둘중에 하나라도 클리어 했으면 튜토리얼 안함
		// 변경 : B2BUG-1660 서버 조건만 체크하도록 수정
		int32 bServerCheck = TutorialManager::GetInstance().GetLastClearTutorialID();
		bool bClientCheck = IsClearNewTutorial();
		if(bServerCheck < 0/* && !bClientCheck*/)
		{
			GoToTutorial();
		}
		else
		{
			GoToLobby();
		}
	}
}

void UB2UITitleScreen::GoToTutorial()
{
	GoToTutorialClass<bool>::GetInstance().Signal(false);
}

void UB2UITitleScreen::GoToLobby()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UITitleScreen::GoToLobby"));

	PlayLobbyBgmClass<>::GetInstance().Signal();
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// !! KakaoLoginDelegate !!
//
// 실제 KakaoLogin 을 이제 DLCFront(B2UITitleDLC) 를 통해서만 한다면 여기 기능은 필요가 없다.
// 만일 여기서도 정식 카카오 로그인 기능에 대한 필요가 있다면 B2UITitleDLC 에 구현된 걸 공유하도록 해야 함.
// 지금은 과거의 흔적이 될 듯.
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#if PLATFORM_ANDROID
void UB2UITitleScreen::KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken)
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

	UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
	UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

	// set auth data
	FB2AuthInfo::GetInstance().Init();
	FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::AOS); // 1
	FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
	FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);
	// TODO fill any other value

	MarkKakaoLogin(); // 준비되었음. 네트워크 초기화 개시.
}
#endif

#if PLATFORM_IOS
void UB2UITitleScreen::KakaoLoginDelegate(bool IsSuccess, int32 ErrorCode)
{
	if (IsSuccess)
	{
		UE_LOG(LogClass, Log, TEXT("####KakaoLoginDelegate !!!!!!!!!!!!!!"));

		FString KakaoUserID = KakaotalkAPI::GetKakaoUserID();
		FString KakaoAccessToken = KakaotalkAPI::GetKakaoAccessToken();

		UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
		UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

		FB2AuthInfo::GetInstance().Init();
		FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::IOS); // 2
		FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
		FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);

		MarkKakaoLogin(); // 준비되었음. 네트워크 초기화 개시.
	}
	else
	{
		m_LoginDelegateCalled.AtomicSet(false);
#if PLATFORM_IOS
		// 네트워크 연결
		if (AB2DLCFrontGameMode* DLCGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			DLCGameMode->ShowKakaoErrorPopup(ErrorCode);
		}
#endif
	}
}
#endif

void UB2UITitleScreen::MarkKakaoLogin()
{
	// PostKakaoLoginInit 에서의 네트워크 초기화랑 AccountInfo 요청을 하고자 하는 건데 단지 다른 쓰레드에서 불릴 우려가 있으므로 한단계를 더 거친다.
	m_LoginDelegateCalled.AtomicSet(true);
}

void UB2UITitleScreen::SetFailResponseMasterData()
{
	m_LoginDelegateCalled.AtomicSet(false);
	FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(false);
	m_firstRequestGetAccount = true;
}

void UB2UITitleScreen::PostKakaoLoginInit()
{
	check(m_LoginDelegateCalled && IsInGameThread());
	if (!FBladeIIBlockToSyncNetwork::GetInstance().GetIsNetworkInit())
	{
		FB2NetworkBridge::GetInstance()->Shutdown();
		InitializeB2NetworkModule(); // 정식 B2network 초기화 시점.
		FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(true);
		//gbNetworkInitialized = true;
	}

	if (m_firstRequestGetAccount
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
		// DummyDLCFront 에서는 FakeUpdate 가 끝나고 DummyDLCFront 가 제거된 후에 본격 로비 진입을 위한 GetAccount 를 함.
		&& !DevDummyDLCFront
#endif
		)
	{
		data_trader::Retailer::GetInstance().RequestGetMasterData();
		m_firstRequestGetAccount = false;

		// 만일 DLC 루트가 아닌 개발버전 일반 로비에서 시작한 거라면 
		// 여기서 AccountInfo 요청을 하게 되면 그때부터 AlternativeImage 를 보이도록 한다. DLC 루트와 최대한 비슷하게.
		if (bUseAlternativeLoadingImage)
		{
			ShowAlternativeLoadingScreen();
		}
	}
}

void UB2UITitleScreen::DelayedGameLobbyStartDirect()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(DelayedGameLobbyStartTH);
	}

	GameLobbyStartDirect();
}

void UB2UITitleScreen::GameLobbyStartDirect()
{ // DLCFront 거쳐서 올 경우 이미 Kakao 로그인이 된 상태이므로 이렇게 함.
  // 한때는 Kakao 로그인 기능이 여기에 있다가 DLCFront 로 바뀌면서 이렇게 됨.
	check(IsInGameThread());
	MarkKakaoLogin();
	PostKakaoLoginInit(); // MarkKakaoLogin 후 게임쓰레드 틱에서 하는 거 직접 콜
}
void UB2UITitleScreen::CloseMiscLoadingIndicator()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(CloseMiscLoadingIndicatorTH);
	}
	ShowMiscLoadingIconClass<bool>::GetInstance().Signal(false);
}

//////////////////////////////////////////////////////////////////////////////
// Fake DLCUpdate and Kakao login handling.
// 개발버전이나 NonDLCCook 등의 상황에서 최종 배포 버전의 실행 절차와 최대한 비슷하게 가도록 하기 위해 만든 더미 기능
// DLCFront 역할만 친다면 사실상 로비가 로딩된 상황이라 완전히 같을 수는 없지만.

void UB2UITitleScreen::BeginFakeKakaoLogin()
{
	//check(!IsFinalDLCCookRun());
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 더미 DLCFront 를 한 번 더 불러와서 Fake 카카오 로그인 절차를 실행.
	if (DevDummyDLCFront)
	{
		DevDummyDLCFront->BeginFakeKakaoLogin();
	}
	else
#endif
	{
		// 더미 카카오 로그인 절차를 거치지 않고 바로 서버 접속 처리. DummyDLCFront 를 생성하지 않는 경우가 있음.
		MarkKakaoLogin();
	}
}

void UB2UITitleScreen::OnCompleteDummyDLCFrontFakeDLCUpdate()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	DestroyDummyDLCFront(); // 타이틀 화면 접근이 가능하도록 완전히 물러남. 
#endif
}
void UB2UITitleScreen::OnCompleteDummyDLCFrontFakeKakaoLogin()
{
	// 개발버전 및 NonDLCCookRun 의 KakaoLoginDelegate 에 해당
	// DummyDLCFront 의 FakeKakaoLogin 처리에 따라 뭔가 의미있는 동작이 들어가면 여기도 같이 뭔가 들어갈 수 있다.

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	MarkKakaoLogin(); // 카카오 로그인 했다 치고 이후 서버 접속 처리.

	// 그리고 DLCFront 를 가정한 상황에서는 업데이트 모의 화면을 시작.
	if (DevDummyDLCFront)
	{
		DevDummyDLCFront->ShowFakeDLCUpdate(true);
	}
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
void UB2UITitleScreen::CreateDummyDLCFront()
{
	if (!DevDummyDLCFront)
	{
		TSubclassOf<UB2UIDummyDLCFront> CreateClass = UB2UIDummyDLCFront::GetUniqueClass();
		if (CreateClass)
		{
			DevDummyDLCFront = CreateWidget<UB2UIDummyDLCFront>(GetOwningPlayer(), CreateClass);
			if (DevDummyDLCFront)
			{
				DevDummyDLCFront->AddToViewport(UB2UIDummyDLCFront::GetViewportZOrder());
			}
		}
		if (DevDummyDLCFront)
		{
			DevDummyDLCFront->OnFakeUpdate.BindDynamic(this, &UB2UITitleScreen::OnCompleteDummyDLCFrontFakeDLCUpdate);
			DevDummyDLCFront->OnFakeKakaoLogin.BindDynamic(this, &UB2UITitleScreen::OnCompleteDummyDLCFrontFakeKakaoLogin);
		}
	}
}
void UB2UITitleScreen::DestroyDummyDLCFront()
{
	if (DevDummyDLCFront)
	{
		DevDummyDLCFront->DestroySelf();
		DevDummyDLCFront = nullptr;
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////

#if !UE_BUILD_SHIPPING
bool UB2UITitleScreen::CheckAndSetupForTitleMovieCaptureMode()
{
	// 여기 로딩 스크린 특정 단계에서 타이틀 동영상 플레이하는 장면을 캡쳐해서 쓰는데 거기에 맞춘 장면을 제공하는 모드. 

	if (FParse::Param(FCommandLine::Get(), TEXT("TitleMovieCaptureMode")))
	{
		// 특정 해상도에 최적으로 맞춘 상태로 캡쳐한 이미지를 사용.
		GEngine->DeferredCommands.Add(FString::Printf(TEXT("r.setres %dx%d"), 1480, 720));

		int32 TitleMovieCaptureClassInt = PCClassToInt(EPCClass::EPC_End);
		FParse::Value(FCommandLine::Get(), TEXT("TitleMovieCaptureClass="), TitleMovieCaptureClassInt);
		const EPCClass TitleMovieCaptureClass = IntToPCClass(TitleMovieCaptureClassInt);

		if (UIP_TitleDesign.IsValid())
		{ 
			// 터치 신호 안 받게 함. 괜히 로비 넘어가면 더 귀찮 ㅋ
			UIP_TitleDesign->SetVisibility(ESlateVisibility::HitTestInvisible);
			// StartMovieImmediate 를 해야 TouchToStart 문구가 안 나옴.
			if (TitleMovieCaptureClass != EPCClass::EPC_End)
			{
				UIP_TitleDesign->StartMovieImmediate(TitleMovieCaptureClass);
			}
			else
			{
				UIP_TitleDesign->StartMovieImmediate(IntToPCClass(FMath::RandRange(0, GetMaxPCClassNum() - 1)));
			}
			// 기타 캡쳐 이미지에서 숨기기로 한 거.
			UIP_TitleDesign->SetShowOtherCIs(false);
		}
		if (IMG_Alternative_LobbyLoading.IsValid())
		{
			IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
		}
		return true;
	}
	return false;
}
#endif