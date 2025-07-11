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

// kakao �ӽ�
#include "B2PlatformAPI.h"
#include "B2UITitleDesign.h"
// ���������δ� B2UITitleDLC �� �ִ� Ÿ��Ʋ ȭ�� ����� ���� ����̰� ���� ���� ����� ���⼭ ���� �� ����.
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

	// �̰� true �� ���� �� AccountInfo �ε� �� �׳� ������ �̹��� ���� �ǵ�
	// �� ��� LoadingImageInfo �� GetLobbyLoginLoadingImageName ������ �������� �̹��� ��θ� �����ϵ��� �� �ʿ䰡 �ִ�.
	// �� �׷��� �߰��� ��� �ٸ� �̹����� ���� ���̴�.
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

#if PLATFORM_ANDROID // ��¥ kakao �α��� delegate ���
	FJavaWrapper::OnKakaoLoginResultDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FJavaWrapper::OnKakaoLoginResultDelegate.AddUObject(this, &UB2UITitleScreen::KakaoLoginDelegate);
#endif
#if PLATFORM_IOS 
	FKakaoTalkPlugin::OnLoggedInDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FKakaoTalkPlugin::OnLoggedInDelegate.AddUObject(this, &UB2UITitleScreen::KakaoLoginDelegate);
#endif

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 && !BUILD_STANDALONE_TEST_APK
	//// ��Ʈ��ũ �ʱ�ȭ�� �� �� ���¿����� DLC ������ �������ִ� ���̸� ���µ� Ÿ �� ���������� ���� ȥ���� ���ߵ� �� ������ �̰� ��ŵ�ϴ� �������ڸ� �غ��Ѵ�.
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
		// Ÿ��Ʋ ������ ĸ�� ���� ó����. �ٸ� �¾� �н�
	}
	else
#endif
//	if (IsFinalDLCCookRun() 
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
//		|| UB2UIDummyDLCFront::bWentThroughRealDLCFront
//#endif
//		)
//	{
//		// DLC ��Ʈ���� ���� Account Info ��û�ϰ� �κ� ���� ����ϴ� ��� �������� ���� �ȴ�.
//		// DLC Front ���� Title �� �����ְ� �� �� ���⼭�� account info �޴� ������ �Ͻ����� ������ ����.
//		// ���� �̹��� Ȥ�� ���� flow �ϰ����� ���� title ������ ���.
//		// Tick �� ���ư��⵵ �ϰ�, ChangeScene ������̶� �ϴ��� B2GameLoadingScreen �� ����Ҹ��� ȯ���� �ƴϴ�.
//		// �Ѱ谡 ������ ���⼭ UI �� ���� �ɷδ� �������� ��ƾ� �Ѵ�.
//		
//		if (bUseAlternativeLoadingImage)
//		{
//			ShowAlternativeLoadingScreen();
//		}
//		else
//		{ // �� ��� Ÿ��Ʋ ������ ����. �������� �� ���ư��� �� �ƴϰ� ���� ���·� ���� ��.
//			if (IMG_Alternative_LobbyLoading.IsValid())
//			{
//				IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
//			}
//			if(UIP_TitleDesign.IsValid())
//			{
//				// DLCTitle ȭ�鿡�� ���������� Ŭ������ �������� ���� ���.
//				// ���⼭ ƽ�� �޴� �ð��� ª���Ƿ� �� ���̿� ���� �׷����� �ܰ��� �����ַ��� Animation ��� ���� �ٷ� �����ؾ�.
//				UIP_TitleDesign->StartMovieImmediate(AB2DLCFrontGameMode::GetSelectedFrontTitleMovieClass());
//				UIP_TitleDesign->SetVisibility(ESlateVisibility::HitTestInvisible);
//				// ���� ������ ĸ���� �ε� �̹����� ���ߴ��� ��Ÿ CI �� �� ���̰� ó��.
//				UIP_TitleDesign->SetShowOtherCIs(false);
//			}
//			ShowExtraLoadingMessage(true);
//		}		
//		
//		// AccountInfo ���� ���� ������ ��⸦ Ÿ�� �ϰ����� �̻����� ��Ȳ�̸� �ſ� ª���Ƿ� �ٷ� ���⼭ GameLobbyStartDirect �� �������� �ʴ´�.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{ 		// ������ �ð� ���� �ΰ� AccountInfo �� �κ� ���� ���� �õ�
//			// ���� �ð� ������ ���� �ΰ��ѵ�, �ð� ������ ���� ������ ���ۿ� ������ ������ ȭ���� ��Ŀ���� ä�� ���Ÿ�� �ǰ�,
//			// �ð� ������ �� ª���� ���� �ʱ�ȭ ���� ������ ������ �Ͻ����� ���� ���� ��� ������ ��Ÿ�� �� �ִ�.
//			const float TimeDelayToNetworkInitAndGetAccount = 0.5f;
//			TheWorld->GetTimerManager().SetTimer(DelayedGameLobbyStartTH, this, &UB2UITitleScreen::DelayedGameLobbyStartDirect, 
//				TimeDelayToNetworkInitAndGetAccount, 
//				false);
//		}
//
//		// ��� ���ξ����� ƽ�� ���ư��� ���߿��� �ε� ������ �� �� �ֵ��� ���� ������.
//		ShowMiscLoadingIconClass<bool>::GetInstance().Signal(true);
//		if (TheWorld)
//		{
//			// AccountInfo ���� ������ �ð��� ������ BladeIIBlockToSyncNetwork �� ��Ʈ��ũ ��� indicator �� �� ���̹Ƿ�
//			// ������ �ð� �ְ� ���� �ݵ��� Ÿ�̸� ��ġ. Ÿ�̹��� ũ�� �߿��� �� �ƴϴ� �ð��� ���⼭ ����.
//			TheWorld->GetTimerManager().SetTimer(CloseMiscLoadingIndicatorTH, this, &UB2UITitleScreen::CloseMiscLoadingIndicator,
//				2.0f,  false);
//		}
//	}
//	else
//	{ 
//		// �� ��Ʈ�� ���߹��� �� (������ Ȥ���� DLC ������ ���ϰ� �� ��?) 
//		// TitleDLC �� ���� ����� ���� Title ȭ���� ��� ���⼭�� ���.
//		if (UIP_TitleDesign.IsValid())
//		{
//			UIP_TitleDesign->SetVisibility(ESlateVisibility::Visible); // �� ��� �̰� Visible �� �� �־�� ���⼭ �ʿ��� ��ġ ��ȣ�� ���� �� ����.
//			UIP_TitleDesign->StartMovieRandomPick();
//		}
//		if (IMG_Alternative_LobbyLoading.IsValid())
//		{ // ���⼱ AlternativeLoadingImage �� ����ϴ� ���� �ϴ� ���ܵ�. �̵��� ����..
//			IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
//		}
//	}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// �� ���� ������ �׽�Ʈ �÷����� ������ ���⼭ �� �ϴ� �ɷ� ħ.
	UB2UIDummyDLCFront::bWentThroughRealDLCFront = false;
#endif

#if !PLATFORM_MAC
	// �ξ۰��� �ʱ�ȭ
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
	{ // Ư���� �ʿ��ϸ� ����.. �����ϸ� �̰ɷ� ������ �ȵ� �� ������..
		PN_DLC_COOK->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UTextBlock, TB_ExtraLoadingMessage);
}

void UB2UITitleScreen::ShowExtraLoadingMessage(bool bShow)
{
	// DLC ���� ���� ���� �� �κ� ���� ������ ������ �������� ���� �޽����� ��.
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
	// �ݴ�� ���� ���� �ǵ������� �������� ����. �װ� ��Ȳ�� ���� �� �޶�.
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
// �������� ��� TitleScreen �� ������ ���鼭 ��ġ �ʰ� AccountInfo �� ������ ��û�ϴٰ� ���̴� ��찡 �� �־.. �̹� AccountInfo �� ���� ��Ȳ�̸� TitleScreen ���� AccountInfo �� ��û���� �ʵ���.
bool gbAlreadyGotAccountOnce_ForTitleScreen = false;
#endif

//[@AKI, 170927][FIXME]Main Thread���� ȣ��Ǵ� ������ �־� �ӽ� �������� ���� ��.
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
	//�������� ������ �����ؾߵ�.
	PlatformServerConnectionHandling();
#endif

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UITitleScreen::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
#if BUILD_STANDALONE_TEST_APK
	GoToStandaloneTestScene();
#elif !PLATFORM_IOS		// IOS ������ �ߺ� ȣ���� �ȵǾ� �Է� �ϳ��� ����.. �׽�Ʈ �� �ȵ���̵忡���� �ߺ� ȣ��� �ϳ��� �����Ǿ�� ��.
	//�������� ������ �����ؾߵ�.
	PlatformServerConnectionHandling();
#endif
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

#if BUILD_STANDALONE_TEST_APK // ��ġ�ϸ� �������� ���� �׳� �κ� ���� �����Ϸ��� ��.. ��� ����� �� �ΰ�.
void UB2UITitleScreen::GoToStandaloneTestScene()
{
	// 6����� ����
	extern int32 gForcedPlayerDummyPartGrade;
	gForcedPlayerDummyPartGrade = 6;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
	}
	GEngine->DeferredCommands.Add(TEXT("CombatStatScale 100000000")); // ������ �������� ���� �ʰ� ������ ��Ƣ��
	// �κ� ���� ȭ�� ����. �� ���� ���� ���� �ȵǰ� �ϰ�..
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
	// ��ĳ ���� ����
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
	// Ÿ��Ʋ ȭ�� ��ġ�ϸ� �Ҹ��� �⺻ ��ƾ.
	// ���⼭ ���ǿ� ���� �б⸦ ������ ���������� PostKakaoLoginInit ���� �����Ѵ�.
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
//		// ���� �׽�Ʈ �� ������ Ÿ�� �ϰ� �ܺ� ������ īī�� �α��� ���� �ٷ� ������ ������ ȣ��
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
//			// �������� ��� Ÿ��Ʋ ȭ�鿡 ������ ������ �����ϹǷ� �̹� AccountInfo �� ���� ��Ȳ�̶�� �ٽ� ��û���� �ʰ� �ٷ� �κ� ����
//			//CheckNewTutorial();
//			//ResponseGetTutorial���� LastClearTutorialID�޾Ƽ� CheckNewTutorial()����
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
//			// ��Ÿ ���߹��� �� �׽�Ʈ ��ŷ������ ��Ʈ
//			BeginFakeKakaoLogin(); // ���������� ���ο��� ����� �� ���� ������ �𸣰ڴµ� �ϸ��� ���ɼ��� ���ܵд�.
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

	// ��Ȳ�� ���� �̰� �ݴ� Ÿ�̸Ӱ� ��ó �۵��ϱ� �� �κ� ���ο� �����ϰ� �� ���� ����.
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
	// ��ġ �� ó�� ����� PreRender ���� ������ ���� �����ϰ� ����� �ٽ� ���ƿ� �Ŀ� �κ� Ȥ�� Ʃ�丮�� ������ �ϵ��� �Ѵ�.
	// ��, �������� DLC ��Ʈ��ζ�� DLCFrontGameMode ���� PreRender ������ �����ϹǷ� ���⼭ PreRenderPending �� �ɸ� ���� ���� ���� ���̴�.
	if (!(LobbyGM && LobbyGM->IsFirstTimePreRenderPending()))
	{	
		// ���� : ������ Ŭ���̾�Ʈ �Ѵ� üũ.. ���߿� �ϳ��� Ŭ���� ������ Ʃ�丮�� ����
		// ���� : B2BUG-1660 ���� ���Ǹ� üũ�ϵ��� ����
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
// ���� KakaoLogin �� ���� DLCFront(B2UITitleDLC) �� ���ؼ��� �Ѵٸ� ���� ����� �ʿ䰡 ����.
// ���� ���⼭�� ���� īī�� �α��� ��ɿ� ���� �ʿ䰡 �ִٸ� B2UITitleDLC �� ������ �� �����ϵ��� �ؾ� ��.
// ������ ������ ������ �� ��.
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#if PLATFORM_ANDROID
void UB2UITitleScreen::KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken)
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

	UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
	UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

	// set auth data
	FB2AuthInfo::GetInstance().Init();
	FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::AOS); // 1
	FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
	FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);
	// TODO fill any other value

	MarkKakaoLogin(); // �غ�Ǿ���. ��Ʈ��ũ �ʱ�ȭ ����.
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

		MarkKakaoLogin(); // �غ�Ǿ���. ��Ʈ��ũ �ʱ�ȭ ����.
	}
	else
	{
		m_LoginDelegateCalled.AtomicSet(false);
#if PLATFORM_IOS
		// ��Ʈ��ũ ����
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
	// PostKakaoLoginInit ������ ��Ʈ��ũ �ʱ�ȭ�� AccountInfo ��û�� �ϰ��� �ϴ� �ǵ� ���� �ٸ� �����忡�� �Ҹ� ����� �����Ƿ� �Ѵܰ踦 �� ��ģ��.
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
		InitializeB2NetworkModule(); // ���� B2network �ʱ�ȭ ����.
		FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(true);
		//gbNetworkInitialized = true;
	}

	if (m_firstRequestGetAccount
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
		// DummyDLCFront ������ FakeUpdate �� ������ DummyDLCFront �� ���ŵ� �Ŀ� ���� �κ� ������ ���� GetAccount �� ��.
		&& !DevDummyDLCFront
#endif
		)
	{
		data_trader::Retailer::GetInstance().RequestGetMasterData();
		m_firstRequestGetAccount = false;

		// ���� DLC ��Ʈ�� �ƴ� ���߹��� �Ϲ� �κ񿡼� ������ �Ŷ�� 
		// ���⼭ AccountInfo ��û�� �ϰ� �Ǹ� �׶����� AlternativeImage �� ���̵��� �Ѵ�. DLC ��Ʈ�� �ִ��� ����ϰ�.
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
{ // DLCFront ���ļ� �� ��� �̹� Kakao �α����� �� �����̹Ƿ� �̷��� ��.
  // �Ѷ��� Kakao �α��� ����� ���⿡ �ִٰ� DLCFront �� �ٲ�鼭 �̷��� ��.
	check(IsInGameThread());
	MarkKakaoLogin();
	PostKakaoLoginInit(); // MarkKakaoLogin �� ���Ӿ����� ƽ���� �ϴ� �� ���� ��
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
// ���߹����̳� NonDLCCook ���� ��Ȳ���� ���� ���� ������ ���� ������ �ִ��� ����ϰ� ������ �ϱ� ���� ���� ���� ���
// DLCFront ���Ҹ� ģ�ٸ� ��ǻ� �κ� �ε��� ��Ȳ�̶� ������ ���� ���� ������.

void UB2UITitleScreen::BeginFakeKakaoLogin()
{
	//check(!IsFinalDLCCookRun());
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// ���� DLCFront �� �� �� �� �ҷ��ͼ� Fake īī�� �α��� ������ ����.
	if (DevDummyDLCFront)
	{
		DevDummyDLCFront->BeginFakeKakaoLogin();
	}
	else
#endif
	{
		// ���� īī�� �α��� ������ ��ġ�� �ʰ� �ٷ� ���� ���� ó��. DummyDLCFront �� �������� �ʴ� ��찡 ����.
		MarkKakaoLogin();
	}
}

void UB2UITitleScreen::OnCompleteDummyDLCFrontFakeDLCUpdate()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	DestroyDummyDLCFront(); // Ÿ��Ʋ ȭ�� ������ �����ϵ��� ������ ������. 
#endif
}
void UB2UITitleScreen::OnCompleteDummyDLCFrontFakeKakaoLogin()
{
	// ���߹��� �� NonDLCCookRun �� KakaoLoginDelegate �� �ش�
	// DummyDLCFront �� FakeKakaoLogin ó���� ���� ���� �ǹ��ִ� ������ ���� ���⵵ ���� ���� �� �� �ִ�.

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	MarkKakaoLogin(); // īī�� �α��� �ߴ� ġ�� ���� ���� ���� ó��.

	// �׸��� DLCFront �� ������ ��Ȳ������ ������Ʈ ���� ȭ���� ����.
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
	// ���� �ε� ��ũ�� Ư�� �ܰ迡�� Ÿ��Ʋ ������ �÷����ϴ� ����� ĸ���ؼ� ���µ� �ű⿡ ���� ����� �����ϴ� ���. 

	if (FParse::Param(FCommandLine::Get(), TEXT("TitleMovieCaptureMode")))
	{
		// Ư�� �ػ󵵿� �������� ���� ���·� ĸ���� �̹����� ���.
		GEngine->DeferredCommands.Add(FString::Printf(TEXT("r.setres %dx%d"), 1480, 720));

		int32 TitleMovieCaptureClassInt = PCClassToInt(EPCClass::EPC_End);
		FParse::Value(FCommandLine::Get(), TEXT("TitleMovieCaptureClass="), TitleMovieCaptureClassInt);
		const EPCClass TitleMovieCaptureClass = IntToPCClass(TitleMovieCaptureClassInt);

		if (UIP_TitleDesign.IsValid())
		{ 
			// ��ġ ��ȣ �� �ް� ��. ���� �κ� �Ѿ�� �� ���� ��
			UIP_TitleDesign->SetVisibility(ESlateVisibility::HitTestInvisible);
			// StartMovieImmediate �� �ؾ� TouchToStart ������ �� ����.
			if (TitleMovieCaptureClass != EPCClass::EPC_End)
			{
				UIP_TitleDesign->StartMovieImmediate(TitleMovieCaptureClass);
			}
			else
			{
				UIP_TitleDesign->StartMovieImmediate(IntToPCClass(FMath::RandRange(0, GetMaxPCClassNum() - 1)));
			}
			// ��Ÿ ĸ�� �̹������� ������ �� ��.
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