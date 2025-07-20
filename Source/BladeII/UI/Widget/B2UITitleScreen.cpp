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

// kakao 烙矫
#include "B2PlatformAPI.h"
#include "B2UITitleDesign.h"
// 弥辆利栏肺绰 B2UITitleDLC 俊 乐绰 鸥捞撇 拳搁 扁瓷捞 沥侥 秒鞭捞绊 弊率 傍烹 扁瓷阑 咯扁辑 静绰 霸 乐澜.
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

	// 捞吧 true 肺 静绰 扒 AccountInfo 肺爹 矫 弊成 沥秦柳 捞固瘤 静绰 扒单
	// 捞 版快 LoadingImageInfo 狼 GetLobbyLoginLoadingImageName 俊辑档 付蛮啊瘤 捞固瘤 版肺甫 府畔窍档废 且 鞘夸啊 乐促.
	// 救 弊矾搁 吝埃俊 泪瘪 促弗 捞固瘤啊 唱棵 巴捞促.
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

#if PLATFORM_ANDROID // 柳楼 kakao 肺弊牢 delegate 殿废
	FJavaWrapper::OnKakaoLoginResultDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FJavaWrapper::OnKakaoLoginResultDelegate.AddUObject(this, &UB2UITitleScreen::KakaoLoginDelegate);
#endif
#if PLATFORM_IOS 
	FKakaoTalkPlugin::OnLoggedInDelegate.Remove(OnKakaoLoginHandler);
	OnKakaoLoginHandler = FKakaoTalkPlugin::OnLoggedInDelegate.AddUObject(this, &UB2UITitleScreen::KakaoLoginDelegate);
#endif

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 && !BUILD_STANDALONE_TEST_APK
	//// 匙飘况农 檬扁拳啊 救 等 惑怕俊辑绰 DLC 例瞒甫 惑扁矫难林绰 歹固甫 剁快绰单 鸥 评 硅器俊辑绰 宝洒 去急父 啊吝瞪 芭 鞍栏聪 捞吧 胶诺窍绰 角青牢磊甫 霖厚茄促.
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
		// 鸥捞撇 悼康惑 母媚 葛靛肺 贸府凳. 促弗 悸诀 菩胶
	}
	else
#endif
//	if (IsFinalDLCCookRun() 
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
//		|| UB2UIDummyDLCFront::bWentThroughRealDLCFront
//#endif
//		)
//	{
//		// DLC 风飘俊辑 咯变 Account Info 夸没窍绊 肺厚 柳涝 措扁窍绰 泪矫 瘤唱啊绰 镑捞 等促.
//		// DLC Front 俊辑 Title 阑 焊咯林霸 等 饶 咯扁辑绰 account info 罐绰 悼救狼 老矫利牢 啊覆阜 开且.
//		// 喊档 捞固瘤 趣篮 矫累 flow 老包己阑 困秦 title 悼康惑 荤侩.
//		// Tick 捞 倒酒啊扁档 窍绊, ChangeScene 措扁吝捞扼 窍歹扼档 B2GameLoadingScreen 阑 荤侩且父茄 券版篮 酒聪促.
//		// 茄拌啊 乐瘤父 咯扁辑 UI 肺 剁快绰 吧肺促 啊覆阜阑 伙酒具 茄促.
//		
//		if (bUseAlternativeLoadingImage)
//		{
//			ShowAlternativeLoadingScreen();
//		}
//		else
//		{ // 捞 版快 鸥捞撇 悼康惑 楷厘. 悼康惑捞 肋 倒酒啊绰 扒 酒聪绊 肛冕 惑怕肺 焊老 巴.
//			if (IMG_Alternative_LobbyLoading.IsValid())
//			{
//				IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
//			}
//			if(UIP_TitleDesign.IsValid())
//			{
//				// DLCTitle 拳搁俊辑 罚待急琶茄 努贰胶肺 悼康惑阑 楷厘 犁积.
//				// 咯扁辑 平阑 罐绰 矫埃篮 陋栏骨肺 弊 荤捞俊 购啊 弊凡轿茄 寇包阑 焊咯林妨搁 Animation 夸家 绝捞 官肺 矫累秦具.
//				UIP_TitleDesign->StartMovieImmediate(AB2DLCFrontGameMode::GetSelectedFrontTitleMovieClass());
//				UIP_TitleDesign->SetVisibility(ESlateVisibility::HitTestInvisible);
//				// 咯扁 悼康惑 母媚茄 肺爹 捞固瘤客 嘎眠蠢扼 扁鸥 CI 甸 救 焊捞霸 贸府.
//				UIP_TitleDesign->SetShowOtherCIs(false);
//			}
//			ShowExtraLoadingMessage(true);
//		}		
//		
//		// AccountInfo 览翠 罐阑 锭鳖瘤 措扁甫 鸥变 窍摆瘤父 捞惑利牢 惑炔捞搁 概快 陋栏骨肺 官肺 咯扁辑 GameLobbyStartDirect 甫 妮窍瘤绰 臼绰促.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{ 		// 利寸洒 矫埃 瘤楷 滴绊 AccountInfo 棺 肺厚 皋牢 柳涝 矫档
//			// 咯扁 矫埃 瘤楷捞 篮辟 刮皑茄单, 矫埃 瘤楷捞 傈囚 绝栏搁 悼累俊 巩力绰 绝瘤父 拳搁捞 矫目概柳 盲肺 措扁鸥霸 登绊,
//			// 矫埃 瘤楷捞 逞 陋栏搁 辑滚 檬扁拳 流饶 购啊客 爵囚辑 老矫利牢 辑滚 楷搬 厘局 泅惑捞 唱鸥朝 荐 乐促.
//			const float TimeDelayToNetworkInitAndGetAccount = 0.5f;
//			TheWorld->GetTimerManager().SetTimer(DelayedGameLobbyStartTH, this, &UB2UITitleScreen::DelayedGameLobbyStartDirect, 
//				TimeDelayToNetworkInitAndGetAccount, 
//				false);
//		}
//
//		// 利绢档 皋牢静饭靛 平捞 倒酒啊绰 客吝俊绰 肺爹 吝烙阑 舅 荐 乐档废 构扼档 倒赴促.
//		ShowMiscLoadingIconClass<bool>::GetInstance().Signal(true);
//		if (TheWorld)
//		{
//			// AccountInfo 坷扁 傈鳖瘤 矫埃捞 瘤唱搁 BladeIIBlockToSyncNetwork 狼 匙飘况农 措扁 indicator 啊 躲 巴捞骨肺
//			// 利寸洒 矫埃 林绊 咯变 摧档废 鸥捞赣 汲摹. 鸥捞怪捞 农霸 吝夸茄 扒 酒聪聪 矫埃篮 咯扁辑 瘤沥.
//			TheWorld->GetTimerManager().SetTimer(CloseMiscLoadingIndicatorTH, this, &UB2UITitleScreen::CloseMiscLoadingIndicator,
//				2.0f,  false);
//		}
//	}
//	else
//	{ 
//		// 捞 风飘绰 俺惯滚傈 侩 (郴瘤绰 趣咯唱 DLC 硅器甫 救窍霸 瞪 锭?) 
//		// TitleDLC 甫 烹秦 剁快瘤 给茄 Title 拳搁阑 措脚 咯扁辑扼档 剁框.
//		if (UIP_TitleDesign.IsValid())
//		{
//			UIP_TitleDesign->SetVisibility(ESlateVisibility::Visible); // 捞 版快 捞吧 Visible 肺 秦 林绢具 咯扁辑 鞘夸茄 磐摹 脚龋甫 罐阑 荐 乐澜.
//			UIP_TitleDesign->StartMovieRandomPick();
//		}
//		if (IMG_Alternative_LobbyLoading.IsValid())
//		{ // 咯扁急 AlternativeLoadingImage 甫 荤侩窍绰 版快扼档 老窜 见败狄. 捞蝶啊 焊烙..
//			IMG_Alternative_LobbyLoading->SetVisibility(ESlateVisibility::Hidden);
//		}
//	}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 捞 俺惯 焊炼侩 抛胶飘 敲贰弊狼 开且篮 咯扁辑 促 窍绰 吧肺 魔.
	UB2UIDummyDLCFront::bWentThroughRealDLCFront = false;
#endif

#if !PLATFORM_MAC
	// 牢聚搬力 檬扁拳
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
	{ // 漂喊洒 鞘夸窍搁 剁款促.. 愧父窍搁 捞吧肺 丹栏搁 救瞪 波 鞍篮单..
		PN_DLC_COOK->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UTextBlock, TB_ExtraLoadingMessage);
}

void UB2UITitleScreen::ShowExtraLoadingMessage(bool bShow)
{
	// DLC 硅器 呼靛 角青 矫 肺厚 柳涝 流傈狼 付瘤阜 包巩俊辑 焊绰 皋矫瘤啊 凳.
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
	// 馆措肺 静绰 版快绰 狼档利栏肺 力傍窍瘤 臼澜. 弊扒 惑炔俊 蝶扼 粱 崔扼辑.
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
// 俊叼磐狼 版快 TitleScreen 阑 咯矾锅 凯搁辑 盔摹 臼霸 AccountInfo 甫 咯矾锅 夸没窍促啊 部捞绰 版快啊 粱 乐绢辑.. 捞固 AccountInfo 甫 罐篮 惑炔捞搁 TitleScreen 俊辑 AccountInfo 甫 夸没窍瘤 臼档废.
bool gbAlreadyGotAccountOnce_ForTitleScreen = false;
#endif

//[@AKI, 170927][FIXME]Main Thread俊辑 龋免登绰 泅惑捞 乐绢 烙矫 规祈栏肺 函版 窃.
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
	//辑滚客狼 立加阑 刚历秦具凳.
	PlatformServerConnectionHandling();
#endif

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UITitleScreen::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
#if BUILD_STANDALONE_TEST_APK
	GoToStandaloneTestScene();
#elif !PLATFORM_IOS		// IOS 俊辑绰 吝汗 龋免捞 救登绢 涝仿 窍唱绰 阜澜.. 抛胶飘 饶 救靛肺捞靛俊辑档 吝汗 龋免矫 窍唱绰 昏力登绢具 窃.
	//辑滚客狼 立加阑 刚历秦具凳.
	PlatformServerConnectionHandling();
#endif
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

#if BUILD_STANDALONE_TEST_APK // 磐摹窍搁 坷橇扼牢 葛靛肺 弊成 肺厚 皋牢 柳涝窍妨绰 芭.. 割啊瘤 彬嘎勉 秦 滴绊.
void UB2UITitleScreen::GoToStandaloneTestScene()
{
	// 6己厘厚 嘎勉
	extern int32 gForcedPlayerDummyPartGrade;
	gForcedPlayerDummyPartGrade = 6;
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
	}
	GEngine->DeferredCommands.Add(TEXT("CombatStatScale 100000000")); // 愧父茄 傈捧俊辑 磷瘤 臼霸 傈捧仿 唤脾扁
	// 肺厚 皋牢 拳搁 流青. 谍 单急 辑滚 楷搬 救登霸 窍绊..
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
	// 咯某 基敲 偿偿
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
	// 鸥捞撇 拳搁 磐摹窍搁 阂府绰 扁夯 风凭.
	// 咯扁辑 炼扒俊 蝶扼 盒扁甫 窍瘤父 弥辆利栏肺 PostKakaoLoginInit 栏肺 荐放茄促.
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
//		// 俺惯 抛胶飘 矫 捞率阑 鸥霸 窍绊 寇何 滚傈篮 墨墨坷 肺弊牢 绝捞 官肺 付胶磐 单捞磐 龋免
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
//			// 俊叼磐狼 版快 鸥捞撇 拳搁俊 咯矾锅 立辟捞 啊瓷窍骨肺 捞固 AccountInfo 甫 罐篮 惑炔捞扼搁 促矫 夸没窍瘤 臼绊 官肺 肺厚 流青
//			//CheckNewTutorial();
//			//ResponseGetTutorial俊辑 LastClearTutorialID罐酒辑 CheckNewTutorial()角青
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
//			// 扁鸥 俺惯滚傈 棺 抛胶飘 捻欧俊辑狼 风飘
//			BeginFakeKakaoLogin(); // 角龙利栏肺 奖俏俊辑 咯扁肺 棵 老捞 乐阑瘤 葛福摆绰单 老富狼 啊瓷己篮 巢败敌促.
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

	// 惑炔俊 蝶扼 捞吧 摧绰 鸥捞赣啊 固贸 累悼窍扁 傈 肺厚 皋牢俊 柳涝窍霸 瞪 荐档 乐澜.
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
	// 汲摹 饶 贸澜 角青瞪 PreRender 傈侩 饭骇阑 刚历 角青窍绊 咯扁肺 促矫 倒酒柯 饶俊 肺厚 趣篮 譬配府倔 柳涝阑 窍档废 茄促.
	// 窜, 沥惑利牢 DLC 风飘措肺扼搁 DLCFrontGameMode 俊辑 PreRender 饭骇肺 流青窍骨肺 咯扁辑 PreRenderPending 俊 吧副 老篮 芭狼 绝阑 巴捞促.
	if (!(LobbyGM && LobbyGM->IsFirstTimePreRenderPending()))
	{	
		// 扁粮 : 辑滚客 努扼捞攫飘 笛促 眉农.. 笛吝俊 窍唱扼档 努府绢 沁栏搁 譬配府倔 救窃
		// 函版 : B2BUG-1660 辑滚 炼扒父 眉农窍档废 荐沥
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
// 角力 KakaoLogin 阑 捞力 DLCFront(B2UITitleDLC) 甫 烹秦辑父 茄促搁 咯扁 扁瓷篮 鞘夸啊 绝促.
// 父老 咯扁辑档 沥侥 墨墨坷 肺弊牢 扁瓷俊 措茄 鞘夸啊 乐促搁 B2UITitleDLC 俊 备泅等 吧 傍蜡窍档废 秦具 窃.
// 瘤陛篮 苞芭狼 如利捞 瞪 淀.
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#if PLATFORM_ANDROID
void UB2UITitleScreen::KakaoLoginDelegate(JNIEnv *env, jobject thiz, jstring userID, jstring accessToken)
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

	UE_LOG(LogClass, Log, TEXT("####KakaoUserID !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoUserID);
	UE_LOG(LogClass, Log, TEXT("####KakaoAccessToken !!!!!!!!!!!!!! = [[[%s]]]"), *KakaoAccessToken);

	// set auth data
	FB2AuthInfo::GetInstance().Init();
	FB2AuthInfo::GetInstance().SetPlatformType(b2network::B2OSType::AOS); // 1
	FB2AuthInfo::GetInstance().SetPlatformUserId(KakaoUserID);
	FB2AuthInfo::GetInstance().SetAccessToken(KakaoAccessToken);
	// TODO fill any other value

	MarkKakaoLogin(); // 霖厚登菌澜. 匙飘况农 檬扁拳 俺矫.
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

		MarkKakaoLogin(); // 霖厚登菌澜. 匙飘况农 檬扁拳 俺矫.
	}
	else
	{
		m_LoginDelegateCalled.AtomicSet(false);
#if PLATFORM_IOS
		// 匙飘况农 楷搬
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
	// PostKakaoLoginInit 俊辑狼 匙飘况农 檬扁拳尔 AccountInfo 夸没阑 窍绊磊 窍绰 扒单 窜瘤 促弗 静饭靛俊辑 阂副 快妨啊 乐栏骨肺 茄窜拌甫 歹 芭模促.
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
		InitializeB2NetworkModule(); // 沥侥 B2network 檬扁拳 矫痢.
		FBladeIIBlockToSyncNetwork::GetInstance().SetIsNetworkInit(true);
		//gbNetworkInitialized = true;
	}

	if (m_firstRequestGetAccount
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
		// DummyDLCFront 俊辑绰 FakeUpdate 啊 场唱绊 DummyDLCFront 啊 力芭等 饶俊 夯拜 肺厚 柳涝阑 困茄 GetAccount 甫 窃.
		&& !DevDummyDLCFront
#endif
		)
	{
		data_trader::Retailer::GetInstance().RequestGetMasterData();
		m_firstRequestGetAccount = false;

		// 父老 DLC 风飘啊 酒囱 俺惯滚傈 老馆 肺厚俊辑 矫累茄 芭扼搁 
		// 咯扁辑 AccountInfo 夸没阑 窍霸 登搁 弊锭何磐 AlternativeImage 甫 焊捞档废 茄促. DLC 风飘客 弥措茄 厚搅窍霸.
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
{ // DLCFront 芭媚辑 棵 版快 捞固 Kakao 肺弊牢捞 等 惑怕捞骨肺 捞犯霸 窃.
  // 茄锭绰 Kakao 肺弊牢 扁瓷捞 咯扁俊 乐促啊 DLCFront 肺 官差搁辑 捞犯霸 凳.
	check(IsInGameThread());
	MarkKakaoLogin();
	PostKakaoLoginInit(); // MarkKakaoLogin 饶 霸烙静饭靛 平俊辑 窍绰 芭 流立 妮
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
// 俺惯滚傈捞唱 NonDLCCook 殿狼 惑炔俊辑 弥辆 硅器 滚傈狼 角青 例瞒客 弥措茄 厚搅窍霸 啊档废 窍扁 困秦 父电 歹固 扁瓷
// DLCFront 开且父 模促搁 荤角惑 肺厚啊 肺爹等 惑炔捞扼 肯傈洒 鞍阑 荐绰 绝瘤父.

void UB2UITitleScreen::BeginFakeKakaoLogin()
{
	//check(!IsFinalDLCCookRun());
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 歹固 DLCFront 甫 茄 锅 歹 阂矾客辑 Fake 墨墨坷 肺弊牢 例瞒甫 角青.
	if (DevDummyDLCFront)
	{
		DevDummyDLCFront->BeginFakeKakaoLogin();
	}
	else
#endif
	{
		// 歹固 墨墨坷 肺弊牢 例瞒甫 芭摹瘤 臼绊 官肺 辑滚 立加 贸府. DummyDLCFront 甫 积己窍瘤 臼绰 版快啊 乐澜.
		MarkKakaoLogin();
	}
}

void UB2UITitleScreen::OnCompleteDummyDLCFrontFakeDLCUpdate()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	DestroyDummyDLCFront(); // 鸥捞撇 拳搁 立辟捞 啊瓷窍档废 肯傈洒 拱矾巢. 
#endif
}
void UB2UITitleScreen::OnCompleteDummyDLCFrontFakeKakaoLogin()
{
	// 俺惯滚傈 棺 NonDLCCookRun 狼 KakaoLoginDelegate 俊 秦寸
	// DummyDLCFront 狼 FakeKakaoLogin 贸府俊 蝶扼 购啊 狼固乐绰 悼累捞 甸绢啊搁 咯扁档 鞍捞 购啊 甸绢哎 荐 乐促.

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	MarkKakaoLogin(); // 墨墨坷 肺弊牢 沁促 摹绊 捞饶 辑滚 立加 贸府.

	// 弊府绊 DLCFront 甫 啊沥茄 惑炔俊辑绰 诀单捞飘 葛狼 拳搁阑 矫累.
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
	// 咯扁 肺爹 胶农赴 漂沥 窜拌俊辑 鸥捞撇 悼康惑 敲饭捞窍绰 厘搁阑 母媚秦辑 静绰单 芭扁俊 嘎冕 厘搁阑 力傍窍绰 葛靛. 

	if (FParse::Param(FCommandLine::Get(), TEXT("TitleMovieCaptureMode")))
	{
		// 漂沥 秦惑档俊 弥利栏肺 嘎冕 惑怕肺 母媚茄 捞固瘤甫 荤侩.
		GEngine->DeferredCommands.Add(FString::Printf(TEXT("r.setres %dx%d"), 1480, 720));

		int32 TitleMovieCaptureClassInt = PCClassToInt(EPCClass::EPC_End);
		FParse::Value(FCommandLine::Get(), TEXT("TitleMovieCaptureClass="), TitleMovieCaptureClassInt);
		const EPCClass TitleMovieCaptureClass = IntToPCClass(TitleMovieCaptureClassInt);

		if (UIP_TitleDesign.IsValid())
		{ 
			// 磐摹 脚龋 救 罐霸 窃. 宝洒 肺厚 逞绢啊搁 歹 蓖满 せ
			UIP_TitleDesign->SetVisibility(ESlateVisibility::HitTestInvisible);
			// StartMovieImmediate 甫 秦具 TouchToStart 巩备啊 救 唱咳.
			if (TitleMovieCaptureClass != EPCClass::EPC_End)
			{
				UIP_TitleDesign->StartMovieImmediate(TitleMovieCaptureClass);
			}
			else
			{
				UIP_TitleDesign->StartMovieImmediate(IntToPCClass(FMath::RandRange(0, GetMaxPCClassNum() - 1)));
			}
			// 扁鸥 母媚 捞固瘤俊辑 见扁扁肺 茄 芭.
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