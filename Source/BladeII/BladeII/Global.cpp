

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


FString GB2NetworkIni; // Default ������ �ִ� �ý��� ini ��. ����� ���� ������� �ƴ� ��.
#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
					   // B2StageEventDirector �� Ư�� �̺�Ʈ ��� �÷��� ���� ���� ��.
					   // �̰� �ϳ� �а� ���� ���� ������ �������μ� �÷��� ���ο� ���� �� �ϴ� �� ������ ������ ���´�.
FString GB2LocalCutSceneSavedStateIni;
#endif
FString GB2NewAcquireStateIni; // � ������ �ش� ������ �������� ���� ����� new ��ũ�� ����� �ϴ� �Ϳ� ���� ���� ���� ��. �ٵ� ���� �ʿ� ��������..
FString GB2GeneralSavedStateIni; // ��Ÿ �����, ���� ���� ���Ϸ� ��� �ʿ� ���� ���� ���� ����� ini
FString GB2PreRenderSavedStateIni; // ���ҽ� Ÿ�� �� Pre-render ���� ����. ���̴� ĳ���� (�Ƹ���) �Ǿ����� ���ο� ���� BladeII ���Ӵ��� high-level state. ���� : �� ���°� ���� ���̴� ĳ�� ���θ� ���������� �ݿ��ϴ� �� �ƴϴ�. ���� ���⼭ �����ϴ� ������ ���� ������ ���̴� ĳ���� ��.
FString GB2UserSavedStateIni; //User ������ ������ ����

bool GbLocalSavedStateCleared = false; // �� ini �� �� ���� ���� ����� ini ���� Ŭ���� �� ��Ȳ���� ����. (��ü�� �������� ���� ��Ȳ)

void LoadLocalIniStateOnStartup()
{ // GB2UserSavedStateIni ���� �о���̴� �� ���⿡ ������ �ȵ�.
	//UB2PreRenderer::LoadCompleteState();
}
void LoadLocalIniStateOnNetworkInit()
{// GB2UserSavedStateIni ���� �о���̴� ��.
	// �̰� ModuleStartup ���� �ʰ�.. �κ� �ε��ǰ� ��Ʈ��ũ �ʱ�ȭ �� ���Ŀ� �Ҹ��� �Ǵµ� 
	// �� ���� �о���� �ʿ䰡 �ִٸ� GB2UserSavedStateIni �� �־ �ȵȴ�.
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
{// ModuleShutdown ������ ���� ���ᰡ �ƴ� ��� ��ġ�� ���� �� �����Ƿ� �ٸ� �� (OnPreLoadMap �� ��Ÿ �������� �κ�) ������ ������ �� �ʿ䰡 �ִ�.
 // ���� ���� �����͸� Ŭ�����ϴ� ��Ȳ������ �����ϸ鼭 �ٽ� ���°��� �����ϸ� �ֽ� Ŭ���� �� �� ���繬 �Ǵ� ��ŵ.
	if (!GbLocalSavedStateCleared)
	{
		SaveLocalIniStateCommon();
	}
}
void SaveLocalIniStateOnPreLoadMap()
{ // ������ �Ű����� ���� �ϰ������� ������ ������ �͵��� ���⼭ ó��
	// �ٺ������� ~~OnExit �̶� �Ȱ��� ��..
	SaveLocalIniStateCommon();
}
void SaveLocalIniStateOnAppBecomeInactive()
{
	// �� ���� �ڵ鸵�� ����ġ ���� �� �־ �߰���.
	// �ȵ���̵��� ��� Ȩ��ư Ȥ�� �� ��� ��� ��ȯ �� �Ҹ���.
	// �Ҹ��� Ÿ�̹��� �÷��� �� �߰��� �ʿ���.
	SaveLocalIniStateCommon();
}

/** ���� �ʱ�ȭ ���� ��Ȳ���� ����ϱ� ���� ��������, ������ ���� ĳ�� �뵵�� ����� ���� ini �� ����� �����͸� ������ ���.
* ���� DB Ŭ���� ������ �ƴ����� �߸� ����� ��� ���� �̻󽺷� �������� ���� �� �����Ƿ� ������ �������� ���. */
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
//		// PreRenderSavedState �� ���⼭ ������ �ʴ´�. ���⼭ LocalIni �� ������ ������ ���� ����� ������ ���¸� ������ �����ε� Pre-render �� �� ��ġ�� �����Ǵ� �� �÷��� ������ �ٲ�ٰ� �ؼ� �ٽ� �� �ʿ䰡 ����.
//		//GConfig->EmptyAll(GB2PreRenderSavedStateIni);
//
//		GConfig->Flush(false);
//
//		GbLocalSavedStateCleared = true;
//	}
}

FString GetPreRenderSavedStateIniName()
{
	FString RetName(TEXT("B2PreRenderState")); // �⺻ �̸�.
	
#if PLATFORM_ANDROID
	RetName += TEXT("_Android");

	FString AndroidVersionStr = FAndroidMisc::GetAndroidVersion();
	FString AVStr_Left;
	FString AVStr_Right;
	// ������ ���� ������ ���
	if (AndroidVersionStr.Split(TEXT("."), &AVStr_Left, &AVStr_Right))
	{
		RetName += FString::Printf(TEXT("%s"), *AVStr_Left);
	}
	// ���� ���� Vulkan ��� ���ΰ� ������ �ȴٰų� OpenGLES <-> Vulkan ����Ī�� ������ ���ɼ� ���� ������ Vulkan ���� ǥ��.
	if (FAndroidMisc::ShouldUseVulkan())
	{
		RetName += TEXT("_Vulkan");
	}
	else
	{
		// GL ������ ������ ���� Ȯ���� �� ������.. ���ڿ��� �� �� �����ϴ�. 	
		FString GlVersionStr = FAndroidMisc::GetGLVersion();
		for (int32 SI = 0; SI < GlVersionStr.Len(); ++SI)
		{
			int32 ConvertedNum = FCString::Atoi(&(GlVersionStr[SI]));
			if (ConvertedNum > 0)
			{  // ó������ ������ 0 ���� ū ���ڸ� ������ �������� �����ϴ� �ǵ� �Ϲ� ���ڶ�� 0 �� ���� ��. 
				// �װ� ������ ������ �ƴ϶�� �ص� ���� �ذ� �� �� ����..
				// �׸��� ���� �ϳ� ������ �ϸ� ���ڸ� ���� ������ ó�� ���ϴµ� �츮 ���� ���񽺵Ǵ� ���� GL ������ �׷��� �ö� ���� ���� �� ����..
				// ���ϰ� �ǵ��ʹ� �ٸ� ������ �ְ� �ȴ� �ϴ��� �ذ� �� �� ����.
				RetName += FString::Printf(TEXT("_Gl%d"), ConvertedNum);
				break;
			}
		}
	}
#elif PLATFORM_IOS
	// ���⵵ ������ ������ ������ ���ڴµ�.. �⺻ ���̴� ������ ������ ���� PreRender ó���� ��� Android ���ٴ� �ʿ伺�� ����.
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
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ���� ���� ���� ��Ȳ���� ���� ���� ���������� ��Ʈ��ũ ���� �����ϴ� �̱� ��� Ŀ�ǵ�
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
//		checkSlow(CreatedUIs.Contains(UIFName::Title)); /* ���ݱ��� ������ Ÿ��Ʋ ȭ�鿡�� ����ϴ� �� */
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
//	InitB2GameSubsystemsOnInitB2Network(); // B2Network �� �������� ������ ������ ���� ���� �Ŵ����� �ʱ�ȭ
//
//	//UBannerManager::GetInstance().SubscribeEvents();
//	data_trader::ChannelManager::GetInstance();
//	
//	Utilities::FConnectionState::GetInstance().Initialize();
//
//	//[@AKI, 171220] ��� ���� ��ġ�� ���� ���� �ƴ����� ��� Flow�� ���⸦ ����ؾ� �ϱ� ������
//	// ���⼭ User Dependency Save File�� �ε��ϰ� ��
//	// ���� ���⸦ ��� �������� PlatformUserId�� ���� Set�ؼ� �̰��� ��ġ ��Ŵ
//	const FString UserID(FB2AuthInfo::GetInstance().GetPlatformUserId());
//	FSHAHash Hash;
//	//Sha1�� �浹 ��ʰ� �ִٰ� ��....https://security.googleblog.com/2017/02/announcing-first-sha1-collision.html ���� �� ��
//	//�ٵ� �浹�� �߻��� ���� ���ÿ� �����ϴ� �����ʹ� ������ ���ʿ��� ���� �浹�̴� ������ ������ ������
//	FSHA1::HashBuffer(*UserID, UserID.Len(), Hash.Hash);
//	const FString UserID_HashResult(Hash.ToString());
//
//	FConfigCacheIni::LoadGlobalIniFile(GB2UserSavedStateIni, *FString(TEXT("B2UserSavedStateIni_") + UserID_HashResult));
//
//	// GB2UserSavedStateIni �ʱ�ȭ �ǰ� ���� ��ΰ� ini ������ �� �ε�.
//	LoadLocalIniStateOnNetworkInit();
}

void InitB2GameSubsystemsOnStartupModule()
{ 
	// Ÿ��Ʋ ȭ����� ���� ���� �ʱ�ȭ �Ǵ� �͵�.

	BladeIIGameImpl::GetChatStore().OnModuleStartup();

	// InitializeAllGameSettingData ���� �ҷ��� �Ѵ�.
	//B2Scalability::OnModuleStartup();

	//// �� ���������� GameSetting �ʱ�ȭ�� ������ ������ ���� ��. �׷��� ��� ������ ���뼭 �ʱ�ȭ �� ��.
	//InitializeAllGameSettingData(nullptr,
	//	true  // �ҿ����� ���� ��ɿ� ���� Workaround �÷���.. ��ü�� ���ķ����Ϳ��� ���ػ� �� �� ù �ε�ȭ�� ��߳��� �� ��� ��ó�ϴ� ����.
	//);

	//UB2LobbyInventory::StaticInit();
	//UB2Airport::StaticInit();
	//AB2StageManager::GetCacheStageKeepEssentialData().Initialize();
	//UB2AssetLoader::Get(); // ������ �ѹ��� �ȵ� ä�� ShutdownModule ���� Get �Ϸ��� null �� ������ ������... �̸� ������ �δ°� ������.. �ƴ� �ٸ� ������ ����ó���� �ϴ� �� ������..?
}
void InitB2GameSubsystemsOnInitB2Network()
{
	// ���� ������ �� ���� ����. ���� ������ ���������� ���õ� �͵� �ܿ� �� Ÿ�ֿ̹� �ʱ�ȭ �ϴ� �͵�.

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
	// InitB2GameSubsystemsOnStartupModule �̶� InitB2GameSubsystemsOnInitB2Network ���� �ʱ�ȭ �� �͵� ���⼭ ����.
	// Init �� �ִ� �͵��� �־ ���� 1:1 ��Ī�� �ƴϰ�..
		
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