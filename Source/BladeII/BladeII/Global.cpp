

#include "Global.h"
#include "B2NetworkBridge.h"
#include "BladeIIGameImpl.h"

//1.network
#include "B2NetworkBridge.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "B2AuthInfo.h"

//2.data

//3.manager
#include "B2EtherManager.h"
#include "ChannelManager.h"
#include "MissionManager.h"
#include "QuestManager.h"
#include "TutorialManager.h"
#include "BannerManager.h"
#include "HotTimeBuffManager.h"
#include "RewardEventManager.h"
#include "B2LobbyInventory.h"
#include "B2Airport.h"
#include "BladeIIGameMode.h"
#include "BootDataTrader.h"
#include "B2PreRenderer.h"
#include "B2UIManager.h"
#include "B2StageManager.h"
#include "FairyManager.h"

#include "B2ClientDataStore.h"
#include "BladeIIUtil.h"
#include "BladeIIScalabilityControl.h"


FString GB2NetworkIni; // Default 설정이 있는 시스템 ini 임. 사용자 설정 저장용은 아닐 듯.
#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
					   // B2StageEventDirector 의 특정 이벤트 장면 플레이 상태 저장 용.
					   // 이게 꽤나 읽고 쓰는 양이 많은데 지금으로선 플레이 여부에 따라 뭘 하는 게 없으니 제거해 놓는다.
FString GB2LocalCutSceneSavedStateIni;
#endif
FString GB2NewAcquireStateIni; // 어떤 종류든 해당 종류의 아이템을 새로 습득시 new 마크를 띄워야 하는 것에 대한 상태 저장 용. 근데 이제 필요 없으려나..
FString GB2GeneralSavedStateIni; // 기타 잡다한, 굳이 별도 파일로 떼어낼 필요 없는 로컬 상태 저장용 ini
FString GB2PreRenderSavedStateIni; // 리소스 타입 별 Pre-render 상태 저장. 셰이더 캐쉬가 (아마도) 되었는지 여부에 대한 BladeII 게임단의 high-level state. 주의 : 이 상태가 실제 셰이더 캐쉬 여부를 직접적으로 반영하는 건 아니다. 단지 여기서 저장하는 상태의 동작 목적이 셰이더 캐싱인 것.
FString GB2UserSavedStateIni; //User 종속적 데이터 저장

bool GbLocalSavedStateCleared = false; // 위 ini 들 중 로컬 상태 저장용 ini 들을 클리어 한 상황에서 켜짐. (대체로 계정삭제 등의 상황)

void LoadLocalIniStateOnStartup()
{ // GB2UserSavedStateIni 에서 읽어들이는 건 여기에 놓으면 안됨.
	//UB2PreRenderer::LoadCompleteState();
}
void LoadLocalIniStateOnNetworkInit()
{// GB2UserSavedStateIni 에서 읽어들이는 것.
	// 이건 ModuleStartup 보다 늦게.. 로비 로딩되고 네트워크 초기화 된 이후에 불리게 되는데 
	// 그 전에 읽어들이 필요가 있다면 GB2UserSavedStateIni 에 넣어선 안된다.
	BladeIIGameImpl::GetClientDataStore().LoadLocalData();
	UB2Airport::LoadLocalData();
}

static void SaveLocalIniStateCommon()
{
	BladeIIGameImpl::GetClientDataStore().SaveLocalData();
	UB2Airport::SaveLocalData();
	//UB2PreRenderer::SaveCompleteState();
}
void SaveLocalIniStateOnExit()
{// ModuleShutdown 시점은 정상 종료가 아닌 경우 거치지 않을 수 있으므로 다른 곳 (OnPreLoadMap 및 기타 개별적인 부분) 에서도 저장을 할 필요가 있다.
 // 또한 로컬 데이터를 클리어하는 상황에서는 종료하면서 다시 상태값을 저장하면 애써 클리어 한 게 도루묵 되니 스킵.
	if (!GbLocalSavedStateCleared)
	{
		SaveLocalIniStateCommon();
	}
}
void SaveLocalIniStateOnPreLoadMap()
{ // 별도의 매개변수 없이 일괄적으로 저장이 가능한 것들은 여기서 처리
	// 바보같지만 ~~OnExit 이랑 똑같은 짓..
	SaveLocalIniStateCommon();
}
void SaveLocalIniStateOnAppBecomeInactive()
{
	// 앱 종료 핸들링이 완전치 못할 수 있어서 추가됨.
	// 안드로이드의 경우 홈버튼 혹은 앱 목록 뷰로 전환 시 불린다.
	// 불리는 타이밍은 플랫폼 별 추가가 필요함.
	SaveLocalIniStateCommon();
}

/** 계정 초기화 등의 상황에서 사용하기 위한 목적으로, 갖가지 상태 캐싱 용도로 사용한 로컬 ini 에 저장된 데이터를 날리는 기능.
* 서버 DB 클리어 수준은 아니지만 잘못 사용할 경우 뭔가 이상스런 오동작이 나올 수 있으므로 정해진 곳에서만 사용. */
void ClearLocalIniSavedState()
{
//	if (GConfig)
//	{
//		GConfig->EmptyAll(GB2GeneralSavedStateIni);
//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//		GConfig->EmptyAll(GB2LocalCutSceneSavedStateIni);
//#endif
//		GConfig->EmptyAll(GB2NewAcquireStateIni);
//		GConfig->EmptyAll(GB2UserSavedStateIni);
//
//		// PreRenderSavedState 는 여기서 날리지 않는다. 여기서 LocalIni 를 날리는 목적은 게임 진행과 관련한 상태를 날리기 위함인데 Pre-render 는 앱 설치가 유지되는 한 플레이 계정이 바뀐다고 해서 다시 할 필요가 없다.
//		//GConfig->EmptyAll(GB2PreRenderSavedStateIni);
//
//		GConfig->Flush(false);
//
//		GbLocalSavedStateCleared = true;
//	}
}

FString GetPreRenderSavedStateIniName()
{
	FString RetName(TEXT("B2PreRenderState")); // 기본 이름.
	
#if PLATFORM_ANDROID
	RetName += TEXT("_Android");

	FString AndroidVersionStr = FAndroidMisc::GetAndroidVersion();
	FString AVStr_Left;
	FString AVStr_Right;
	// 메이저 버전 정보만 떼어냄
	if (AndroidVersionStr.Split(TEXT("."), &AVStr_Left, &AVStr_Right))
	{
		RetName += FString::Printf(TEXT("%s"), *AVStr_Left);
	}
	// 오픈 이후 Vulkan 사용 여부가 변경이 된다거나 OpenGLES <-> Vulkan 스위칭을 제공할 가능성 등이 있으니 Vulkan 여부 표시.
	if (FAndroidMisc::ShouldUseVulkan())
	{
		RetName += TEXT("_Vulkan");
	}
	else
	{
		// GL 버전을 넣으면 보다 확실할 것 같은데.. 문자열이 좀 더 복잡하다. 	
		FString GlVersionStr = FAndroidMisc::GetGLVersion();
		for (int32 SI = 0; SI < GlVersionStr.Len(); ++SI)
		{
			int32 ConvertedNum = FCString::Atoi(&(GlVersionStr[SI]));
			if (ConvertedNum > 0)
			{  // 처음으로 나오는 0 보다 큰 숫자를 메이저 버전으로 간주하는 건데 일반 문자라면 0 이 나올 것. 
				// 그게 메이저 버전이 아니라고 해도 굳이 해가 될 건 없고..
				// 그리고 문자 하나 단위로 하면 두자리 숫자 버전을 처리 못하는데 우리 게임 서비스되는 동안 GL 버전이 그렇게 올라갈 일은 없을 것 같고..
				// 여하간 의도와는 다른 정보를 넣게 된다 하더라도 해가 될 건 없음.
				RetName += FString::Printf(TEXT("_Gl%d"), ConvertedNum);
				break;
			}
		}
	}
#elif PLATFORM_IOS
	// 여기도 버전별 관리가 있으면 좋겠는데.. 기본 셰이더 컴파일 성능이 좋고 PreRender 처리도 적어서 Android 보다는 필요성이 낮음.
	RetName += TEXT("_IOS");
#endif

	UE_LOG(LogBladeII, Log, TEXT("PreRenderSavedStateIniName %s"), *RetName);

	return RetName;
}

data_trader::WholesalerPtr data_trader::Association::wholesaler = nullptr;

void InitializeB2NetworkModule()
{
	B2_SCOPED_TRACK_LOG(TEXT("InitializeB2NetworkModule"));
//
//#if WITH_EDITOR
//	if (IsRunningCommandlet()) {
//		return;
//	}
//#endif
//#if BUILD_STANDALONE_TEST_APK
//	return;
//#endif
//
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 영상 덤프 같은 상황에서 쓰기 위해 지속적으로 네트워크 연결 무시하는 싱글 모드 커맨드
//	if (FParse::Param(FCommandLine::Get(), TEXT("B2Standalone")))
//	{
//		FBladeIIBlockToSyncNetwork::GetInstance().DevSetStandaloneMode(true);
//		return;
//	}
//#endif
//
//#if UE_BUILD_DEBUG
//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//	if (UIMgrInst)
//	{
//		TArray<FName> CreatedUIs;
//		UIMgrInst->GetAllCreatedUINames(CreatedUIs);
//		checkSlow(CreatedUIs.Contains(UIFName::Title)); /* 지금까지 가정은 타이틀 화면에서 사용하는 거 */
//	}
//#endif
//
//	// Before network initialize, We must create FConnectionState
//	check(FB2NetworkBridge::GetInstance());
//	FB2NetworkBridge::GetInstance()->Init();
//	FBladeIIBlockToSyncNetwork::GetInstance().SetEnableBlockToSync(true);
//
//	//	FNetworkTick::CreateNetworkTick(FB2NetworkBridge::GetInstance());
//	data_trader::Association::BootStrap();
//
//	InitB2GameSubsystemsOnInitB2Network(); // B2Network 랑 직접적인 연관이 있지는 않은 하위 매니저들 초기화
//
//	//UBannerManager::GetInstance().SubscribeEvents();
//	data_trader::ChannelManager::GetInstance();
//	
//	Utilities::FConnectionState::GetInstance().Initialize();
//
//	//[@AKI, 171220] 사실 여기 위치가 좋은 것은 아니지만 어느 Flow든 여기를 통과해야 하기 때문에
//	// 여기서 User Dependency Save File을 로드하게 함
//	// 또한 여기를 들어 오기전에 PlatformUserId를 먼저 Set해서 이곳에 위치 시킴
//	const FString UserID(FB2AuthInfo::GetInstance().GetPlatformUserId());
//	FSHAHash Hash;
//	//Sha1이 충돌 사례가 있다고 함....https://security.googleblog.com/2017/02/announcing-first-sha1-collision.html 참고 할 것
//	//근데 충돌이 발생해 봤자 로컬에 저장하는 데이터는 유저의 불필요한 정보 충돌이니 괜찮을 것으로 생각됨
//	FSHA1::HashBuffer(*UserID, UserID.Len(), Hash.Hash);
//	const FString UserID_HashResult(Hash.ToString());
//
//	FConfigCacheIni::LoadGlobalIniFile(GB2UserSavedStateIni, *FString(TEXT("B2UserSavedStateIni_") + UserID_HashResult));
//
//	// GB2UserSavedStateIni 초기화 되고 나서 잽싸게 ini 저장한 거 로딩.
//	LoadLocalIniStateOnNetworkInit();
}

void InitB2GameSubsystemsOnStartupModule()
{ 
	// 타이틀 화면까지 가기 전에 초기화 되는 것들.

	BladeIIGameImpl::GetChatStore().OnModuleStartup();

	// InitializeAllGameSettingData 전에 불려야 한다.
	//B2Scalability::OnModuleStartup();

	//// 이 시점에서의 GameSetting 초기화는 완전히 되지는 않을 것. 그래도 몇몇 값들은 이쯤서 초기화 해 둠.
	//InitializeAllGameSettingData(nullptr,
	//	true  // 불완전한 엔진 기능에 대한 Workaround 플래그.. 대체로 에뮬레이터에서 고해상도 쓸 때 첫 로딩화면 어긋나는 거 잠시 대처하는 차원.
	//);

	//UB2LobbyInventory::StaticInit();
	//UB2Airport::StaticInit();
	//AB2StageManager::GetCacheStageKeepEssentialData().Initialize();
	//UB2AssetLoader::Get(); // 생성이 한번도 안된 채로 ShutdownModule 에서 Get 하려면 null 이 나오기 때문에... 미리 생성해 두는게 좋을까.. 아님 다른 식으로 예외처리를 하는 게 좋을까..?
}
void InitB2GameSubsystemsOnInitB2Network()
{
	// 게임 서버에 막 붙은 시점. 서버 연동과 직접적으로 관련된 것들 외에 이 타이밍에 초기화 하는 것들.

	QuestManager::GetInstance().Init();
	//TutorialManager::GetInstance().Init();
	//B2EtherManager::GetInstance().Init();
	//MissionManager::GetInstance().Init();
	//HotTimeBuffManager::GetInstance().Init();
	//RewardEventManager::GetInstance().Init();
	//FairyManager::GetInstance().Init();

	//GLOBALRELICMANAGER.Init();
	//GLOBALUNITYSKILLMANAGER.Init();

	//BladeIIGameImpl::GetCostumeDataStore().OnStartUpModule();
}
void CloseB2GameSubsystemsOnShutdown()
{
	// InitB2GameSubsystemsOnStartupModule 이랑 InitB2GameSubsystemsOnInitB2Network 에서 초기화 된 것들 여기서 정리.
	// Init 만 있는 것들이 있어서 완전 1:1 매칭은 아니고..
		
	//QuestManager::GetInstance().UnsubscribeEvents();
	//QuestManager::GetInstance().EmptyQuestsInHand();
	//TutorialManager::GetInstance().UnsubscribeEvents();
	//B2EtherManager::GetInstance().UnsubscribeEvents();
	//MissionManager::GetInstance().UnsubscribeEvents();
	//HotTimeBuffManager::GetInstance().UnsubscribeEvents();
	//GLOBALRELICMANAGER.UnsubscribeEvents();
	//GLOBALUNITYSKILLMANAGER.UnsubscribeEvents();
	//RewardEventManager::GetInstance().UnsubscribeEvents();
	//FairyManager::GetInstance().UnsubscribeEvents();

	//BladeIIGameImpl::GetClientDataStore().OnShutdownModule();
	//BladeIIGameImpl::GetStageDataStore().OnShutdownModule();
	//BladeIIGameImpl::GetChatStore().OnModuleShutdown();
	//BladeIIGameImpl::GetCostumeDataStore().OnShutDownModule();

	//UB2Airport::StaticOnModuleShutdown();
	//UB2AssetLoader::Get()->OnShutdownModule();
}