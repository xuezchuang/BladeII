

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


FString GB2NetworkIni; // Default 汲沥捞 乐绰 矫胶袍 ini 烙. 荤侩磊 汲沥 历厘侩篮 酒匆 淀.
#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
					   // B2StageEventDirector 狼 漂沥 捞亥飘 厘搁 敲饭捞 惑怕 历厘 侩.
					   // 捞霸 蚕唱 佬绊 静绰 剧捞 腹篮单 瘤陛栏肺急 敲饭捞 咯何俊 蝶扼 够 窍绰 霸 绝栏聪 力芭秦 初绰促.
FString GB2LocalCutSceneSavedStateIni;
#endif
FString GB2NewAcquireStateIni; // 绢恫 辆幅电 秦寸 辆幅狼 酒捞袍阑 货肺 嚼垫矫 new 付农甫 剁况具 窍绰 巴俊 措茄 惑怕 历厘 侩. 辟单 捞力 鞘夸 绝栏妨唱..
FString GB2GeneralSavedStateIni; // 扁鸥 棱促茄, 被捞 喊档 颇老肺 都绢尘 鞘夸 绝绰 肺拿 惑怕 历厘侩 ini
FString GB2PreRenderSavedStateIni; // 府家胶 鸥涝 喊 Pre-render 惑怕 历厘. 嘉捞歹 某浆啊 (酒付档) 登菌绰瘤 咯何俊 措茄 BladeII 霸烙窜狼 high-level state. 林狼 : 捞 惑怕啊 角力 嘉捞歹 某浆 咯何甫 流立利栏肺 馆康窍绰 扒 酒聪促. 窜瘤 咯扁辑 历厘窍绰 惑怕狼 悼累 格利捞 嘉捞歹 某教牢 巴.
FString GB2UserSavedStateIni; //User 辆加利 单捞磐 历厘

bool GbLocalSavedStateCleared = false; // 困 ini 甸 吝 肺拿 惑怕 历厘侩 ini 甸阑 努府绢 茄 惑炔俊辑 难咙. (措眉肺 拌沥昏力 殿狼 惑炔)

void LoadLocalIniStateOnStartup()
{ // GB2UserSavedStateIni 俊辑 佬绢甸捞绰 扒 咯扁俊 初栏搁 救凳.
	//UB2PreRenderer::LoadCompleteState();
}
void LoadLocalIniStateOnNetworkInit()
{// GB2UserSavedStateIni 俊辑 佬绢甸捞绰 巴.
	// 捞扒 ModuleStartup 焊促 词霸.. 肺厚 肺爹登绊 匙飘况农 檬扁拳 等 捞饶俊 阂府霸 登绰单 
	// 弊 傈俊 佬绢甸捞 鞘夸啊 乐促搁 GB2UserSavedStateIni 俊 持绢急 救等促.
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
{// ModuleShutdown 矫痢篮 沥惑 辆丰啊 酒囱 版快 芭摹瘤 臼阑 荐 乐栏骨肺 促弗 镑 (OnPreLoadMap 棺 扁鸥 俺喊利牢 何盒) 俊辑档 历厘阑 且 鞘夸啊 乐促.
 // 肚茄 肺拿 单捞磐甫 努府绢窍绰 惑炔俊辑绰 辆丰窍搁辑 促矫 惑怕蔼阑 历厘窍搁 局结 努府绢 茄 霸 档风宫 登聪 胶诺.
	if (!GbLocalSavedStateCleared)
	{
		SaveLocalIniStateCommon();
	}
}
void SaveLocalIniStateOnPreLoadMap()
{ // 喊档狼 概俺函荐 绝捞 老褒利栏肺 历厘捞 啊瓷茄 巴甸篮 咯扁辑 贸府
	// 官焊鞍瘤父 ~~OnExit 捞尔 度鞍篮 窿..
	SaveLocalIniStateCommon();
}
void SaveLocalIniStateOnAppBecomeInactive()
{
	// 聚 辆丰 勤甸傅捞 肯傈摹 给且 荐 乐绢辑 眠啊凳.
	// 救靛肺捞靛狼 版快 权滚瓢 趣篮 聚 格废 轰肺 傈券 矫 阂赴促.
	// 阂府绰 鸥捞怪篮 敲阀汽 喊 眠啊啊 鞘夸窃.
	SaveLocalIniStateCommon();
}

/** 拌沥 檬扁拳 殿狼 惑炔俊辑 荤侩窍扁 困茄 格利栏肺, 爱啊瘤 惑怕 某教 侩档肺 荤侩茄 肺拿 ini 俊 历厘等 单捞磐甫 朝府绰 扁瓷.
* 辑滚 DB 努府绢 荐霖篮 酒聪瘤父 肋给 荤侩且 版快 购啊 捞惑胶繁 坷悼累捞 唱棵 荐 乐栏骨肺 沥秦柳 镑俊辑父 荤侩. */
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
//		// PreRenderSavedState 绰 咯扁辑 朝府瘤 臼绰促. 咯扁辑 LocalIni 甫 朝府绰 格利篮 霸烙 柳青苞 包访茄 惑怕甫 朝府扁 困窃牢单 Pre-render 绰 聚 汲摹啊 蜡瘤登绰 茄 敲饭捞 拌沥捞 官诧促绊 秦辑 促矫 且 鞘夸啊 绝促.
//		//GConfig->EmptyAll(GB2PreRenderSavedStateIni);
//
//		GConfig->Flush(false);
//
//		GbLocalSavedStateCleared = true;
//	}
}

FString GetPreRenderSavedStateIniName()
{
	FString RetName(TEXT("B2PreRenderState")); // 扁夯 捞抚.
	
#if PLATFORM_ANDROID
	RetName += TEXT("_Android");

	FString AndroidVersionStr = FAndroidMisc::GetAndroidVersion();
	FString AVStr_Left;
	FString AVStr_Right;
	// 皋捞历 滚傈 沥焊父 都绢晨
	if (AndroidVersionStr.Split(TEXT("."), &AVStr_Left, &AVStr_Right))
	{
		RetName += FString::Printf(TEXT("%s"), *AVStr_Left);
	}
	// 坷锹 捞饶 Vulkan 荤侩 咯何啊 函版捞 等促芭唱 OpenGLES <-> Vulkan 胶困莫阑 力傍且 啊瓷己 殿捞 乐栏聪 Vulkan 咯何 钎矫.
	if (FAndroidMisc::ShouldUseVulkan())
	{
		RetName += TEXT("_Vulkan");
	}
	else
	{
		// GL 滚傈阑 持栏搁 焊促 犬角且 巴 鞍篮单.. 巩磊凯捞 粱 歹 汗棱窍促. 	
		FString GlVersionStr = FAndroidMisc::GetGLVersion();
		for (int32 SI = 0; SI < GlVersionStr.Len(); ++SI)
		{
			int32 ConvertedNum = FCString::Atoi(&(GlVersionStr[SI]));
			if (ConvertedNum > 0)
			{  // 贸澜栏肺 唱坷绰 0 焊促 奴 箭磊甫 皋捞历 滚傈栏肺 埃林窍绰 扒单 老馆 巩磊扼搁 0 捞 唱棵 巴. 
				// 弊霸 皋捞历 滚傈捞 酒聪扼绊 秦档 被捞 秦啊 瞪 扒 绝绊..
				// 弊府绊 巩磊 窍唱 窜困肺 窍搁 滴磊府 箭磊 滚傈阑 贸府 给窍绰单 快府 霸烙 辑厚胶登绰 悼救 GL 滚傈捞 弊犯霸 棵扼哎 老篮 绝阑 巴 鞍绊..
				// 咯窍埃 狼档客绰 促弗 沥焊甫 持霸 等促 窍歹扼档 秦啊 瞪 扒 绝澜.
				RetName += FString::Printf(TEXT("_Gl%d"), ConvertedNum);
				break;
			}
		}
	}
#elif PLATFORM_IOS
	// 咯扁档 滚傈喊 包府啊 乐栏搁 亮摆绰单.. 扁夯 嘉捞歹 哪颇老 己瓷捞 亮绊 PreRender 贸府档 利绢辑 Android 焊促绰 鞘夸己捞 撤澜.
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
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 康惑 待橇 鞍篮 惑炔俊辑 静扁 困秦 瘤加利栏肺 匙飘况农 楷搬 公矫窍绰 教臂 葛靛 目盖靛
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
//		checkSlow(CreatedUIs.Contains(UIFName::Title)); /* 瘤陛鳖瘤 啊沥篮 鸥捞撇 拳搁俊辑 荤侩窍绰 芭 */
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
//	InitB2GameSubsystemsOnInitB2Network(); // B2Network 尔 流立利牢 楷包捞 乐瘤绰 臼篮 窍困 概聪历甸 檬扁拳
//
//	//UBannerManager::GetInstance().SubscribeEvents();
//	data_trader::ChannelManager::GetInstance();
//	
//	Utilities::FConnectionState::GetInstance().Initialize();
//
//	//[@AKI, 171220] 荤角 咯扁 困摹啊 亮篮 巴篮 酒聪瘤父 绢蠢 Flow电 咯扁甫 烹苞秦具 窍扁 锭巩俊
//	// 咯扁辑 User Dependency Save File阑 肺靛窍霸 窃
//	// 肚茄 咯扁甫 甸绢 坷扁傈俊 PlatformUserId甫 刚历 Set秦辑 捞镑俊 困摹 矫糯
//	const FString UserID(FB2AuthInfo::GetInstance().GetPlatformUserId());
//	FSHAHash Hash;
//	//Sha1捞 面倒 荤肥啊 乐促绊 窃....https://security.googleblog.com/2017/02/announcing-first-sha1-collision.html 曼绊 且 巴
//	//辟单 面倒捞 惯积秦 好磊 肺拿俊 历厘窍绰 单捞磐绰 蜡历狼 阂鞘夸茄 沥焊 面倒捞聪 宝满阑 巴栏肺 积阿凳
//	FSHA1::HashBuffer(*UserID, UserID.Len(), Hash.Hash);
//	const FString UserID_HashResult(Hash.ToString());
//
//	FConfigCacheIni::LoadGlobalIniFile(GB2UserSavedStateIni, *FString(TEXT("B2UserSavedStateIni_") + UserID_HashResult));
//
//	// GB2UserSavedStateIni 檬扁拳 登绊 唱辑 漓轿霸 ini 历厘茄 芭 肺爹.
//	LoadLocalIniStateOnNetworkInit();
}

void InitB2GameSubsystemsOnStartupModule()
{ 
	// 鸥捞撇 拳搁鳖瘤 啊扁 傈俊 檬扁拳 登绰 巴甸.

	BladeIIGameImpl::GetChatStore().OnModuleStartup();

	// InitializeAllGameSettingData 傈俊 阂妨具 茄促.
	//B2Scalability::OnModuleStartup();

	//// 捞 矫痢俊辑狼 GameSetting 檬扁拳绰 肯傈洒 登瘤绰 臼阑 巴. 弊贰档 割割 蔼甸篮 捞码辑 檬扁拳 秦 狄.
	//InitializeAllGameSettingData(nullptr,
	//	true  // 阂肯傈茄 浚柳 扁瓷俊 措茄 Workaround 敲贰弊.. 措眉肺 俊鼓饭捞磐俊辑 绊秦惑档 镜 锭 霉 肺爹拳搁 绢边唱绰 芭 泪矫 措贸窍绰 瞒盔.
	//);

	//UB2LobbyInventory::StaticInit();
	//UB2Airport::StaticInit();
	//AB2StageManager::GetCacheStageKeepEssentialData().Initialize();
	//UB2AssetLoader::Get(); // 积己捞 茄锅档 救等 盲肺 ShutdownModule 俊辑 Get 窍妨搁 null 捞 唱坷扁 锭巩俊... 固府 积己秦 滴绰霸 亮阑鳖.. 酒丛 促弗 侥栏肺 抗寇贸府甫 窍绰 霸 亮阑鳖..?
}
void InitB2GameSubsystemsOnInitB2Network()
{
	// 霸烙 辑滚俊 阜 嘿篮 矫痢. 辑滚 楷悼苞 流立利栏肺 包访等 巴甸 寇俊 捞 鸥捞怪俊 檬扁拳 窍绰 巴甸.

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
	// InitB2GameSubsystemsOnStartupModule 捞尔 InitB2GameSubsystemsOnInitB2Network 俊辑 檬扁拳 等 巴甸 咯扁辑 沥府.
	// Init 父 乐绰 巴甸捞 乐绢辑 肯傈 1:1 概莫篮 酒聪绊..
		
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