
#include "BladeII.h"

#include "BladeIIGameImpl.h"

#include "Global.h"
#include "FB2ErrorMessage.h"
#include "FakeConfigure.h"
#include "BladeIIUtil.h"

//#include "CoreDelegates.h"
//
//1.network
#include "B2NetworkBridge.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "BootDataTrader.h"
//
//2.data
#include "B2BrevetInfo.h"
#include "B2ChapterInfo.h"
#include "B2ChattingFilteringInfo.h"
#include "B2CommonSoundInfo.h"
#include "B2ErrorCodeInfo.h"
#include "B2EtherInfo.h"
#include "B2EtherSetOptionInfo.h"
#include "B2EtherContainer.h"
#include "B2GoodsShortcutInfo.h"
#include "B2GoodsTypeInfo.h"
#include "B2ItemInfo.h"
#include "B2MissionInfo.h"
#include "B2NPCSensitiveInfo.h"
#include "B2RaidInfo.h"
#include "B2RewardInfo.h"
#include "B2ScenarioDialogInfo.h"
#include "B2SomeInfo.h"
#include "B2StageInfo.h"
#include "B2TalkInfo.h"
#include "B2WingInfo.h"
#include "QuestDataTable.h"
#include "B2SkillInfo.h"
#include "B2ItemSortInfo.h"
#include "B2CharacterTagDialogueInfo.h"
#include "B2SeasonEventInfo.h"
#include "B2SelectiveEtherLottery.h"
#include "B2TotemRefineInfo.h"

//3.manager
#include "B2EtherManager.h"
#include "B2StageManager.h"
#include "B2UIDocManager.h"
//#include "B2UIManager.h"
//#include "MissionManager.h"
#include "QuestManager.h"
#include "TutorialManager.h"
//#include "HotTimeBuffManager.h"
//
#include "B2Airport.h"
#include "B2CombatStatEvaluator.h"
#include "B2LobbyInventory.h"
#include "B2NPCClassInfoBox.h"
#include "BladeIIGameMode.h"
//#include "IPFootPrinter.h"
//

#include "BladeIIScalabilityControl.h"
//#include "B2PreRenderer.h"
#include "B2CompositeMeshCache.h"
#include "B2AssetLoader.h"
//
#include "FairyManager.h"
//
//#if WITH_EDITOR
//#include "BladeIIEditor/Source//BladeIIEditor.h"
//#include "Editor.h"
//#endif
//
FClientDataStore BladeIIGameImpl::ClientDataStore;
FStageDataStore BladeIIGameImpl::StageDataStore;
FB2ChatStore BladeIIGameImpl::ChatStore;
RelicManager BladeIIGameImpl::m_RelicManager;
UnitySkillManager BladeIIGameImpl::m_UnitySkillMissionManager;
FDevOnlyStageLoadData BladeIIGameImpl::DevOnlyStageLoadData;
FGuildDataStore BladeIIGameImpl::GuildDataStore;
FCostumeDataStore BladeIIGameImpl::CostumeDataStore;
FTotemDataStore BladeIIGameImpl::TotemDataStore;
//
//#if WITH_EDITOR
///** 특정 진행도에서 멈춰선 채 기나긴 시간을 소모하는 B2 컨텐츠 로딩 progresss 를 표시하기 위한 hack */
//float GB2ContentLoadingProgress = 0.0f; // 0.0 ~ 1.0
//										/** StartupModule 를 거치면서 기본 progress 가 고정되어 있는 동안 이걸로 B2 컨텐츠 로딩 현황을 표시한다. 0 ~ 1 사이로 */
//void EditorSetB2ContentLoadingProgressSplash(float InProgress)
//{
//	if (GIsEditor)
//	{
//		GB2ContentLoadingProgress = FMath::Clamp(InProgress, 0.0f, 1.0f);
//		// 기본 splash 텍스트 뒤에다 붙일 거.
//		FString OurProgressText = FString::Printf(TEXT(" (Loading BladeII contents.. %d%%)"), (int32)(GB2ContentLoadingProgress * 100.0f / 1.0f));
//		SetAdditionalStartupProgressSplashText(OurProgressText);
//	}
//}
//void BeginB2ContentLoadingProgressSplash()
//{ // 모종의 플래그를 세팅할 수도..
//	if (GIsEditor)
//	{
//		EditorSetB2ContentLoadingProgressSplash(0.0f);
//	}
//}
//void EndB2ContentLoadingProgressSplash()
//{
//	if (GIsEditor)
//	{
//		SetAdditionalStartupProgressSplashText(TEXT(""));
//	}
//}
//#define B2_CONTENT_LOADING_PROGRESS_SPLASH(InProgress) EditorSetB2ContentLoadingProgressSplash(InProgress);
//#else
//#define B2_CONTENT_LOADING_PROGRESS_SPLASH(InProgress)
//#endif

//void LoadEditorData()
//{
//#if WITH_EDITOR
//	if (CanLoadInfoAssetWhileStartupModule())
//	{
//		//Editor에서 게임프로젝트의 LocRes를 로딩하지 않고 있어서 리소스의 FText 찾기에 문제가 있다.
//		//추가로 로드해야할 필요가 있다.
//		GConfig->SetString(TEXT("Internationalization"), TEXT("NativeGameCulture"), *FInternationalization::Get().GetCurrentCulture().Get().GetName(), GEditorSettingsIni);
//		FInternationalization::Get().OnCultureChanged().Broadcast();
//
//		// TAsset Lazy-loading 을 하는 에셋들을 에디터에선 모두 로딩해 놓음. 이걸 안 하면 Redirector 처리에 문제가 발생하는 경우가 있음
//		// 여기서 EditorLoadAll 에 인자를 넣어서 내부에서 progress 계산을 하는 애들은 특히나 모두 로딩할 경우 메모리가 많이 올라오는 major InfoAsset 들인데 
//		// 이것들이 StaticFind** 에서 시간 및 메모리 소모가 많고 EditorLoadAll 에서는 별다른 시간을 먹지 않는다면 해당 asset 의 lazy-loading 이 오동작 하는 거라고 예상해 볼 수 있다.
//
//		UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox();
//		if (MobInfoBox)
//		{ // 아 이거 개발 진행되면서 점점 늘어나니 full load 하지 말자.. 이건 full load 안 해도 redirector 처리 문제될 거 없을 듯..
//			//MobInfoBox->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.02f, 0.4f);
//		}
//
//		UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox();
//		if (PCInfoBox)
//		{ // 이거 등록된 asset 이 몇개 되지도 않는데 설령 redirector 처리 잘못된다고 해서 큰일 날거 까지야. LoadEditorData 에서 제거
//		  //PCInfoBox->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.25f, 0.45f); EditorLoadAll 하게 되면 progress 범위 조절
//		}
//
//		UB2CommonSoundInfo* CommonSoundInfo = StaticFindCommonSoundInfo();
//		if (CommonSoundInfo) {
//			CommonSoundInfo->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.07f);
//
//		UB2DialogSoundInfoSet* DialogSoundInfo = StaticFindDialogSoundInfoTable();
//		if (DialogSoundInfo) {
//			DialogSoundInfo->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.15f);
//
//		UB2DialogCharacterInfoSet* DialogCharInfoTable = StaticFindDialogCharacterInfoTable();
//		if (DialogCharInfoTable)
//		{
//			DialogCharInfoTable->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.2f);
//		// ItemInfo 겉으로 보이는 테이블 사이즈에 비해 실제 로딩하는 양이 그렇게 많지는 않은 듯.
//		// DataTable 기반이라 실제로 full-load 안 할 시 redirector 처리에 문제가 있던 경험이 있고 레퍼런싱 하는 리소스가 워낙에 많아 에디터에선 full load 하는 게 안전하다.
//		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
//		if (ItemInfoTable) {
//			ItemInfoTable->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.2f, 0.8f);
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.8f);
//
//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//		if (UIMgrInst)
//		{ // 얘도 일단 에디터 load 를 안 해도 별 문제 없는 거 같아 제외해 봄. 워낙 양이 계속 늘어나는 거기도 하고..
//		  //UIMgrInst->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.65f, 0.96f); EditorLoadAll 하게 되면 progress 범위 조절
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.88f);
//
//		UB2WingInfo* WingInfo = StaticFindWingInfo();
//		if (WingInfo)
//		{
//			WingInfo->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.9f);
//
//		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
//		if (SomeInfo)
//		{
//			SomeInfo->EditorLoadAll(); // 나머지 중에선 이게 좀 큰 듯.
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.95f);
//
//		UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
//		if (ItemMiscInfo)
//		{
//			ItemMiscInfo->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.96f);
//
//		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//		if (StageInfoTable)
//		{
//			StageInfoTable->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.97f);
//
//		UB2MissionInfo* MissionInfoTable = StaticFindMissionInfo();
//		if (MissionInfoTable)
//		{
//			MissionInfoTable->EditorLoadAll();
//		}
//
//		UB2RewardInfo* RewardInfoTable = StaticFindRewardInfo();
//		if (RewardInfoTable)
//		{
//			RewardInfoTable->EditorLoadAll();
//		}
//
//		UB2EtherInfo* EtherInfoTable = StaticFindEtherInfoTable();
//		if (EtherInfoTable)
//		{
//			EtherInfoTable->EditorLoadAll();
//		}
//
//		if (UB2CharacterTagDialogueInfo* CharacterTagDialogueInfo = StaticFindCharacterTagDialogueInfo())
//		{
//			CharacterTagDialogueInfo->EditorLoadAll();
//		}
//
//		UB2FairyInfo *FairyInfoTable = StaticFindFairyInfo();
//		if (FairyInfoTable)
//		{
//			FairyInfoTable->EditorLoadAll();
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.98f);
//
//		// Just to pop-up a message if missing. Must able to load these info. 
//		if (!StaticFindMobClassInfoBox() || !StaticFindPCClassInfoBox() || !StaticFindCommonSoundInfo() || !StaticFindDialogCharacterInfoTable() ||
//			!StaticFindWingInfo() || !StaticFindSomeInfo() || !StaticFindItemMiscInfo() || !StaticFindGuildMarkInfo() || !StaticFindLoadingImageInfo() || !StaticFindSkillAnimInfo() ||
//			!StaticFindCharacterTagDialogueInfo() || !StaticFindSeasonEventInfo())
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] BladeII 모듈에서 필요로 하는 필수 InfoAsset 을 찾지 못함. 컴퓨터가 곧 폭발한다."))
//			));
//#endif
//		}
//	}
//#endif
//}

BladeIIGameImpl::BladeIIGameImpl()
	: FDefaultGameModuleImpl()
{
	/*StageInfoTable = NULL;
	ChapterInfoTable = NULL;
	ItemInfoTable = NULL;
	DialogInfoTable = NULL;
	DialogSoundInfoTable = NULL;
	NPCSensitiveInfoTable = NULL;
	BrevetInfoTable = NULL;
	ErrorCodeInfoTable = NULL;
	QuestTable = NULL;
	QuestNPCSoundInfoTable = NULL;
	MissionInfoTable = NULL;
	RewardInfoTable = NULL;
	RaidInfoTable = NULL;
	TalkInfoTable = NULL;
	TalkSoundInfoTable = NULL;
	GoodsShortcutInfo = NULL;
	GoodsTypeInfoTable = NULL;
	ChattingFilteringInfoTable = NULL;
	IsNetFeaturekInitialized = false;
	EtherInfoTable = NULL;
	SelectiveEtherLotteryTable = NULL;
	EtherSetOptionInfoTable = NULL;
	EtherSetEffectInfo = NULL;
	TotemRefineInfoTable = NULL;
	ItemSortInfoTable = NULL;
	CharacterTagDialogueInfoTable = NULL;
	SeasonEventInfoTable = NULL;
	CompositeMeshCacher = NULL;
	LobbyBannerCacher = NULL;
	FairyInfoTable = NULL;*/
}

/** InfoAsset Async loading On/Off 플래그. 정식 기능으로 On/Off 제공하고자 하는 건 아니고 기능이 안전하다고 확실시 될 때까지.. 쉽핑 빌드 전달했는데 이걸로 의심되는 크래쉬가 발생할 수도 있으니 */
bool BladeIIGameImpl::bAllowInfoAssetAsyncLoading = true;
bool BladeIIGameImpl::bAllowUIAsyncLoading = true;
bool BladeIIGameImpl::bAllowUIOnDemandAsyncLoading = true;
float BladeIIGameImpl::UIOnDemandAsyncLoadingFlushTimeLimit = 0.5f;

#include "Developer/MessageLog/Public/MessageLogModule.h"

void BladeIIGameImpl::StartupModule()
{
//#if WITH_EDITOR 
//	BeginB2ContentLoadingProgressSplash(); // Full 로딩 하더라도 progress 라도 표시하면 답답하지 않지..
//	if (GIsEditor) {
//		SetManangedChunkIDAssingmentFn(); // 쿠킹 관련 커스텀 기능 동작 셋업
//	}
//#endif
//
//	CombatStatEval::InitializeUnitedOptionMapping(); // 가능한 일찍 초기화 시켜 주어야 함.
//
//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		// Register message log UIs
//		FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
//		{
//			FMessageLogInitializationOptions InitOptions;
//			InitOptions.bShowPages = true;
//			MessageLogModule.RegisterLogListing("BladeII", FText::FromString(TEXT("BladeII Game")), InitOptions);
//		}
//	}
//#endif
//
//	FConfigCacheIni::LoadGlobalIniFile(GB2NetworkIni, TEXT("B2Network"));
//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//	FConfigCacheIni::LoadGlobalIniFile(GB2LocalCutSceneSavedStateIni, TEXT("B2LocalCutSceneSavedState"));
//#endif
//	FConfigCacheIni::LoadGlobalIniFile(GB2NewAcquireStateIni, TEXT("B2NewAcquireState"));
//	FConfigCacheIni::LoadGlobalIniFile(GB2GeneralSavedStateIni, TEXT("B2GeneralSavedState"));
//
//	FConfigCacheIni::LoadGlobalIniFile(GB2PreRenderSavedStateIni, *GetPreRenderSavedStateIniName());
//
//	B2_CONTENT_LOADING_PROGRESS_SPLASH(0.01f);
//
//#if WITH_EDITOR
//	// LoadEditorData 에서는 TAsset 들을 일괄 로딩하는데 skip 하면 redirector 처리에 문제가 발생할 수 있다. 스킵 옵션은 몰래 아는 사람만 사용. 외부에 알리지 않는다.
//	bool bSkipLoadEditorData = (FParse::Param(FCommandLine::Get(), TEXT("UnsafeQuickEditor")) || FParse::Param(FCommandLine::Get(), TEXT("J3M4"))); // J3M4 는 조삼모사의 약자인데 원래 비슷한 목적의 다른 방식의 로딩을 이렇게 칭했던 적이 있음. 실제로 이것도 어느정도 조삼모사이기도 하고.
//	if (!bSkipLoadEditorData)
//	{
//		// 에디터 progress 표시를 쉽게 관리하기 위해 이걸 먼저.
//		// 에디터에서는 Lazy-loading 을 하는 애들을 모두 로딩해 둔다. Redirector 처리 등 에디터 동작 및 쿠킹에 포함되는 것을 보장하기 위해. 그리고 꼭 그런 거 아니어도 에디터는 어느 정도 미리 로딩해 두는 게 좋은 면도.. 
//		LoadEditorData();
//	}
//
//#if WITH_EDITOR
//	if(GIsEditor)
//		FEditorDelegates::PrePIEEnded.AddRaw(this, &BladeIIGameImpl::HandleEditorClosePIE);
//#endif // WITH_EDITOR
//
//#endif
//
//	B2_CONTENT_LOADING_PROGRESS_SPLASH(0.98f); // 에디터의 경우 이후에는 정말 얼마 안될 거..
//
//	GetStageDataStore();	// test
//	
//	LoadLocalIniStateOnStartup(); // 로컬 ini 저장 상태 중 유저 종속적이지 않은 것들
//	LoadIniSystemSettings(); // 마찬가지로 ini 인데 로컬 상태가 아닌 거의 시스템 설정들?
//
//	InitB2GameSubsystemsOnStartupModule();
//
//#if !UE_BUILD_SHIPPING
//	NetFakeConfigure::GetInstance().ParseCommandline();
//#endif
//
//#if WITH_EDITOR
//	if (CanLoadInfoAssetWhileStartupModule())
//	{
//		// 특히 LoadEditorData 에 포함되지 않은 InfoAsset 들은 쿠킹에 확실히 포함시키려면 여기서 한차례 레퍼런스를 얻어 둘 필요가 있다.
//		// -> 단, Blueprints/InfoAsset 폴더를 쿠킹에 강제 포함시킨다면 필수까진 아니다. 그래도 안전장치 정도로 생각해도 될 듯.
//		GetStageInfoTable();
//		GetChapterInfoTable();
//		GetItemInfoTable();
//		GetDialogInfoTable();
//		GetDialogSoundInfoTable();
//		GetNPCSensitiveInfoTable();
//		GetBrevetInfoTable();
//		GetQuestInfoTable();
//		GetQuestNPCSoundInfoTable();
//		GetErrorCodeInfoTable();
//		GetRewardInfoTable();
//		GetMissionInfoTable();
//		GetRaidInfoTable();
//		GetTalkInfoTable();
//		GetGoodsShortcutInfo();
//		GetGoodsTypeInfoTable();
//		GetChattingFilteringInfoTable();
//		GetEtherInfoTable();
//		GetSelectiveEtherLotteryTable();
//		GetEtherSetOptionInfoTable();
//		GetEtherSetEffectInfo();
//		GetTotemRefineInfoTable();
//		GetItemSortInfoTable();
//		GetCharacterTagDialogueInfoTable();
//		GetSeasonEventInfoTable();
//		GetFairyInfoTable();
//		// Cooking에 TutorialAsset 포함시키기 위함
//		TutorialManager::GetInstance().Init();
//	}
//#endif
//
//	FCoreDelegates::OnPreExit.AddRaw(this, &BladeIIGameImpl::HandleCorePreExit);
//
//	FPlatformMisc::ControlScreensaver(FPlatformMisc::EScreenSaverAction::Disable);
//
//#if WITH_EDITOR
//	EndB2ContentLoadingProgressSplash();
//
//	if (GIsEditor)
//	{
//#if !PLATFORM_MAC
//		IPFootPrinter	printer;
//		printer.SendIPFoot();
//#endif
//	}
//#endif
//
//	if (FParse::Param(FCommandLine::Get(), TEXT("WithTutorial")))
//	{
//		TutorialManager::GetInstance().Init();
//		TutorialManager::GetInstance().SetTestTutorialAvailable(true);
//	}
//
//#if PLATFORM_ANDROID
//	FCoreDelegates::OnAndroidDestroyWindow.AddRaw(this, &BladeIIGameImpl::OnAndroidDestroyWindow);
//#endif
}

void BladeIIGameImpl::HandleCorePreExit()
{
	B2_SCOPED_TRACK_LOG(TEXT("BladeIIGameImpl::HandleCorePreExit"));

	//
	// 이게 불리려면 엔진에서 정식 종료 절차를 밟아야 한다. (FPlatformMisc::RequestExit(false)
	//

	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();

	if (UIDocManager)
	{
		UIDocManager->Shutdown();
	}

	DestroyStageInfoTable();
	DestroyChapterInfoTable();
	DestroyItemInfoTable();
	DestroyDialogInfoTable();
	DestroyDialogSoundInfoTable();
	DestroyNPCSensitiveInfoTable();
	DestroyQuestInfoTable();
	DestroyQuestNPCSoundInfoTable();
	DestroyBrevetInfoTable();
	DestroyErrorCodeInfoTable();
	DestroyMissionInfoTable();
	DestroyRewardInfoTable();
	DestroyRaidInfoTable();
	DestroyTalkInfoTable();
	DestroyTalkSoundInfoTable();
	DestroyGoodsShortcutInfo();
	DestroyGoodsTypeInfoTable();
	DestroyChattingFilteringInfoTable();
	DestroyEtherInfoTable();
	DestroySelectiveEtherLotteryTable();
	DestroyEtherSetOptionInfoTable();
	DestroyTotemRefineInfoTable();
	DestroyItemSortInfoTable();
	DestroyCharacterTagDialogueInfoTable();
	DestroySeasonEventInfoTable();
	DestroyCompositeMeshCacher();
	DestroyLobbyBannerCacher();
	DestroyFairyInfoTable();
}

//#if PLATFORM_ANDROID
//void BladeIIGameImpl::OnAndroidDestroyWindow()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("BladeIIGameImpl::OnAndroidDestroyWindow"));
//
//	// 안드로이드 기기 좌측 버튼으로 앱 목록 보이게 할 때 불림. 홈버튼도 마찬가지.
//	// 이름에서 느껴질 수 있는 것과는 달리 이게 꼭 앱 종료를 뜻하는 건 아니지만 종료로 연결될 수는 있음.
//	// 기본 안드로이드 앱 종료 과정에서 놓칠 수 있는 동작 일부를 실행.
//
//	SaveLocalIniStateOnAppBecomeInactive();
//}
//#endif
//
//#include "ChannelManager.h"
void BladeIIGameImpl::ShutdownModule()
{
	B2_SCOPED_TRACK_LOG(TEXT("BladeIIGameImpl::ShutdownModule"));
//
//	//
//	// 이게 불리려면 엔진에서 정식 종료 절차를 밟아야 한다. (FPlatformMisc::RequestExit(false)
//	//
//
//	// 주요 하위 매니저들 여기서 정리.
//	CloseB2GameSubsystemsOnShutdown();
//
//	SaveLocalIniStateOnExit();  // 플랫폼에서의 기능 구현에 따라 ShutdownModule 이 안 불릴 수 있으므로 이건 딴 곳에서도 처리될 필요가 있음.
//
//	if (!IsRunningCommandlet())
//	{
//		data_trader::ChannelManager::ReleaseInstance();
//		data_trader::Association::Shutdown();
//		//		FNetworkTick::ShutdownNetworkTick();
//		FB2NetworkBridge::GetInstance()->Shutdown();
//		Utilities::FConnectionState::GetInstance().Uninitialize();
//	}
//
//#if WITH_EDITOR
//	if (GIsEditor)
//		FEditorDelegates::PrePIEEnded.RemoveAll(this);
//#endif // WITH_EDITOR
}

void BladeIIGameImpl::LoadIniSystemSettings()
{
	//UB2PreRenderer::LoadSettings();
#if BII_STATS
	FStatisticalPerfStatCollector::LoadSettings();
#endif
	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("AllowInfoAssetAsyncLoading"), bAllowInfoAssetAsyncLoading, GGameIni);
		GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("AllowUIAsyncLoading"), bAllowUIAsyncLoading, GGameIni);
		GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("AllowUIOnDemandAsyncLoading"), bAllowUIOnDemandAsyncLoading, GGameIni);
		GConfig->GetFloat(TEXT("/Script/BladeII"), TEXT("UIOnDemandAsyncLoadingFlushTimeLimit"), UIOnDemandAsyncLoadingFlushTimeLimit, GGameIni);
	}
}

UObject* CreastSingleStaticInfoAssetCommon(UClass* InClass)
{
	if (GMinimalDLCFrontMode) {
		return NULL; // DLC Front 모드 리소스로딩 최대한 제거
	}

#if WITH_EDITOR // In formal game, InfoAsset loading must be with BladeIIGameMode (not with StartupModule).
	checkSlow(CanLoadInfoAssetWhileStartupModule() || ABladeIIGameMode::bDevCheckBladeIIGameModeEnteredOnce);
#endif

	/* Engine 생성할 때 처럼 TransientPackage 를 Outer 삼아서 */
	UObject* RetObject = NewObject<UObject>(GetTransientPackage(), InClass, NAME_None); \
		if (RetObject) {
			RetObject->AddToRoot();
		}
	BII_CHECK(RetObject);
	return RetObject;
}

UB2StageInfo* BladeIIGameImpl::GetStageInfoTable()
{
	if (!StageInfoTable)
	{
		StageInfoTable = Cast<UB2StageInfo>(CreastSingleStaticInfoAssetCommon(UB2StageInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || StageInfoTable);
	return StageInfoTable;
}

#define BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(AssetRef) \
	if (AssetRef) \
	{ \
		AssetRef->RemoveFromRoot(); \
		AssetRef->ConditionalBeginDestroy(); \
		AssetRef = NULL; \
	}

void BladeIIGameImpl::DestroyStageInfoTable()
{
	if (!StageInfoTable)
		return;

	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(StageInfoTable)
}

UB2ChapterInfo* BladeIIGameImpl::GetChapterInfoTable()
{
	if (!ChapterInfoTable)
	{
		ChapterInfoTable = Cast<UB2ChapterInfo>(CreastSingleStaticInfoAssetCommon(UB2ChapterInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || ChapterInfoTable);
	return ChapterInfoTable;
}

void BladeIIGameImpl::DestroyChapterInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(ChapterInfoTable)
}

UB2ItemInfo* BladeIIGameImpl::GetItemInfoTable()
{
	if (!ItemInfoTable)
	{
		ItemInfoTable = Cast<UB2ItemInfo>(CreastSingleStaticInfoAssetCommon(UB2ItemInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || ItemInfoTable);
	return ItemInfoTable;
}

void BladeIIGameImpl::DestroyItemInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(ItemInfoTable)
}

UB2ScenarioDialogInfo* BladeIIGameImpl::GetDialogInfoTable()
{
	if (!DialogInfoTable)
	{
		DialogInfoTable = Cast<UB2ScenarioDialogInfo>(CreastSingleStaticInfoAssetCommon(UB2ScenarioDialogInfo::StaticClass()));
		if (DialogInfoTable)
		{
			FString ScenarioDialogTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2ScenarioDialogInfo"), TEXT("ScenarioDialogTable"), ScenarioDialogTablePath, GGameIni);
			auto* DataTable = LoadObject<UDataTable>(NULL, *ScenarioDialogTablePath);
			DialogInfoTable->SetDataTable(DataTable);
		}
	}

	BII_CHECK(DialogInfoTable);
	return DialogInfoTable;
}

void BladeIIGameImpl::DestroyDialogInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(DialogInfoTable)
}

UB2DialogSoundInfoSet* BladeIIGameImpl::GetDialogSoundInfoTable()
{
	if (!DialogSoundInfoTable)
	{
		DialogSoundInfoTable = Cast<UB2DialogSoundInfoSet>(CreastSingleStaticInfoAssetCommon(UB2DialogSoundInfoSet::StaticClass()));
		if (DialogSoundInfoTable)
		{
			FString DialogSoundTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2ScenarioDialogInfo"), TEXT("DialogSoundTable"), DialogSoundTablePath, GGameIni);
			UDataTable* DataTable = LoadObject<UDataTable>(NULL, *DialogSoundTablePath);
			DialogSoundInfoTable->SetDataTable(DataTable);
		}
	}

	BII_CHECK(DialogSoundInfoTable);
	return DialogSoundInfoTable;
}

void BladeIIGameImpl::DestroyDialogSoundInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(DialogSoundInfoTable)
}

UB2NPCSensitiveInfo* BladeIIGameImpl::GetNPCSensitiveInfoTable()
{
	if (!NPCSensitiveInfoTable)
	{
		NPCSensitiveInfoTable = Cast<UB2NPCSensitiveInfo>(CreastSingleStaticInfoAssetCommon(UB2NPCSensitiveInfo::StaticClass()));
		if (NPCSensitiveInfoTable)
		{
			FString InfoDataTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2NPCClassInfo"), TEXT("NPCSensitiveInfoDataTable"), InfoDataTablePath, GGameIni);
			UDataTable* FoundDataTable = LoadObject<UDataTable>(NULL, *InfoDataTablePath);
			NPCSensitiveInfoTable->SetDataTable(FoundDataTable);
		}
	}

	BII_CHECK(NPCSensitiveInfoTable);
	return NPCSensitiveInfoTable;
}
void BladeIIGameImpl::DestroyNPCSensitiveInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(NPCSensitiveInfoTable)
}

class UB2BrevetInfo* BladeIIGameImpl::GetBrevetInfoTable()
{
	if (!BrevetInfoTable)
	{
		BrevetInfoTable = Cast<UB2BrevetInfo>(CreastSingleStaticInfoAssetCommon(UB2BrevetInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || BrevetInfoTable);
	return BrevetInfoTable;
}

void BladeIIGameImpl::DestroyBrevetInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(BrevetInfoTable)
}

class UB2ErrorCodeInfo* BladeIIGameImpl::GetErrorCodeInfoTable()
{
	if (!ErrorCodeInfoTable)
	{
		ErrorCodeInfoTable = Cast<UB2ErrorCodeInfo>(CreastSingleStaticInfoAssetCommon(UB2ErrorCodeInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || ErrorCodeInfoTable);
	return ErrorCodeInfoTable;
}

void BladeIIGameImpl::DestroyErrorCodeInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(ErrorCodeInfoTable)
}

class UB2QuestInfoTable* BladeIIGameImpl::GetQuestInfoTable()
{
	if (!QuestTable)
	{
		QuestTable = Cast<UB2QuestInfoTable>(CreastSingleStaticInfoAssetCommon(UB2QuestInfoTable::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || QuestTable);
	return QuestTable;
}

void BladeIIGameImpl::DestroyQuestInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(QuestTable)
}

class UB2QuestNPCSoundInfo* BladeIIGameImpl::GetQuestNPCSoundInfoTable()
{
	if (!QuestNPCSoundInfoTable)
	{
		QuestNPCSoundInfoTable = Cast<UB2QuestNPCSoundInfo>(CreastSingleStaticInfoAssetCommon(UB2QuestNPCSoundInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || QuestNPCSoundInfoTable);
	return QuestNPCSoundInfoTable;

}

void BladeIIGameImpl::DestroyQuestNPCSoundInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(QuestNPCSoundInfoTable)
}

class UB2MissionInfo* BladeIIGameImpl::GetMissionInfoTable()
{
	if (!MissionInfoTable)
	{
		MissionInfoTable = Cast<UB2MissionInfo>(CreastSingleStaticInfoAssetCommon(UB2MissionInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || MissionInfoTable);
	return MissionInfoTable;
}

void BladeIIGameImpl::DestroyMissionInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(MissionInfoTable)
}


class UB2RewardInfo* BladeIIGameImpl::GetRewardInfoTable()
{
	if (!RewardInfoTable)
	{
		RewardInfoTable = Cast<UB2RewardInfo>(CreastSingleStaticInfoAssetCommon(UB2RewardInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || RewardInfoTable);
	return RewardInfoTable;
}

void BladeIIGameImpl::DestroyRewardInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(RewardInfoTable)
}

class UB2RaidInfoTable* BladeIIGameImpl::GetRaidInfoTable()
{
	if (!RaidInfoTable)
	{
		RaidInfoTable = Cast<UB2RaidInfoTable>(CreastSingleStaticInfoAssetCommon(UB2RaidInfoTable::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || RaidInfoTable);
	return RaidInfoTable;
}

void BladeIIGameImpl::DestroyRaidInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(RaidInfoTable)
}

class UB2TalkInfo* BladeIIGameImpl::GetTalkInfoTable()
{
	if (!TalkInfoTable)
	{
		TalkInfoTable = Cast<UB2TalkInfo>(CreastSingleStaticInfoAssetCommon(UB2TalkInfo::StaticClass()));
		if (TalkInfoTable)
		{
			FString TalkInfoTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2TalkInfo"), TEXT("TalkInfoTable"), TalkInfoTablePath, GGameIni);
			auto* DataTable = LoadObject<UDataTable>(NULL, *TalkInfoTablePath);
			TalkInfoTable->SetDataTable(DataTable);
		}
	}
	BII_CHECK(GMinimalDLCFrontMode || TalkInfoTable);
	return TalkInfoTable;
}

void BladeIIGameImpl::DestroyTalkInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(TalkInfoTable)
}

class UB2DialogSoundInfoSet* BladeIIGameImpl::GetTalkSoundInfoTable()
{
	if (!TalkSoundInfoTable)
	{
		TalkSoundInfoTable = Cast<UB2DialogSoundInfoSet>(CreastSingleStaticInfoAssetCommon(UB2DialogSoundInfoSet::StaticClass()));
		if (TalkSoundInfoTable)
		{
			FString SoundTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2TalkInfo"), TEXT("TalkSoundTable"), SoundTablePath, GGameIni);
			UDataTable* DataTable = LoadObject<UDataTable>(NULL, *SoundTablePath);
			TalkSoundInfoTable->SetDataTable(DataTable);
		}
	}

	BII_CHECK(TalkSoundInfoTable);
	return TalkSoundInfoTable;
}

void BladeIIGameImpl::DestroyTalkSoundInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(TalkSoundInfoTable)
}

class UB2FairyInfo* BladeIIGameImpl::GetFairyInfoTable()
{
	if (!FairyInfoTable)
	{
		FairyInfoTable = Cast<UB2FairyInfo>(CreastSingleStaticInfoAssetCommon(UB2FairyInfo::StaticClass()));
		if (FairyInfoTable)
		{
			FString FairyBlessInfoTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2FairyBlessInfo"), TEXT("FairyBlessInfoTable"), FairyBlessInfoTablePath, GGameIni);
			UDataTable* DataTable = LoadObject<UDataTable>(NULL, *FairyBlessInfoTablePath);
			FairyInfoTable->SetDataTable(DataTable);
		}
	}

	BII_CHECK(FairyInfoTable);
	return FairyInfoTable;
}

void BladeIIGameImpl::DestroyFairyInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(FairyInfoTable)
}

class UB2ChattingFilteringInfo* BladeIIGameImpl::GetChattingFilteringInfoTable()
{
	if (!ChattingFilteringInfoTable)
	{
		ChattingFilteringInfoTable = Cast<UB2ChattingFilteringInfo>(CreastSingleStaticInfoAssetCommon(UB2ChattingFilteringInfo::StaticClass()));
		if (ChattingFilteringInfoTable)
		{
			FString ChattingFilteringDataTablePath;
			GConfig->GetString(TEXT("/Script/BladeII.B2ChattingFilteringInfo"), TEXT("ChattingFilteringTable"), ChattingFilteringDataTablePath, GGameIni);
			auto* DataTable = LoadObject<UDataTable>(NULL, *ChattingFilteringDataTablePath);
			ChattingFilteringInfoTable->SetDataTable(DataTable);
		}
	}
	BII_CHECK(GMinimalDLCFrontMode || ChattingFilteringInfoTable);
	return ChattingFilteringInfoTable;
}

void BladeIIGameImpl::DestroyChattingFilteringInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(ChattingFilteringInfoTable)
}

class UB2GoodsTypeInfo* BladeIIGameImpl::GetGoodsTypeInfoTable()
{
	if (!GoodsTypeInfoTable)
	{
		GoodsTypeInfoTable = Cast<UB2GoodsTypeInfo>(CreastSingleStaticInfoAssetCommon(UB2GoodsTypeInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || GoodsTypeInfoTable);
	return GoodsTypeInfoTable;
}
void BladeIIGameImpl::DestroyGoodsTypeInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(GoodsTypeInfoTable)
}

class UB2GoodsShortcutInfo* BladeIIGameImpl::GetGoodsShortcutInfo()
{
	if (!GoodsShortcutInfo)
	{
		GoodsShortcutInfo = Cast<UB2GoodsShortcutInfo>(CreastSingleStaticInfoAssetCommon(UB2GoodsShortcutInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || GoodsShortcutInfo);
	return GoodsShortcutInfo;
}
void BladeIIGameImpl::DestroyGoodsShortcutInfo()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(GoodsShortcutInfo)
}

class UB2EtherInfo* BladeIIGameImpl::GetEtherInfoTable()
{
	if (!EtherInfoTable)
	{
		EtherInfoTable = Cast<UB2EtherInfo>(CreastSingleStaticInfoAssetCommon(UB2EtherInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || EtherInfoTable);
	return EtherInfoTable;
}

void BladeIIGameImpl::DestroyEtherInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(EtherInfoTable)
}

class UB2SelectiveEtherLottery* BladeIIGameImpl::GetSelectiveEtherLotteryTable()
{
	if (!SelectiveEtherLotteryTable)
	{
		SelectiveEtherLotteryTable = Cast<UB2SelectiveEtherLottery>(CreastSingleStaticInfoAssetCommon(UB2SelectiveEtherLottery::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || SelectiveEtherLotteryTable);
	return SelectiveEtherLotteryTable;
}

void BladeIIGameImpl::DestroySelectiveEtherLotteryTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(SelectiveEtherLotteryTable)
}

class UB2EtherSetOptionInfo* BladeIIGameImpl::GetEtherSetOptionInfoTable()
{
	if (!EtherSetOptionInfoTable)
	{
		EtherSetOptionInfoTable = Cast<UB2EtherSetOptionInfo>(CreastSingleStaticInfoAssetCommon(UB2EtherSetOptionInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || EtherSetOptionInfoTable);
	return EtherSetOptionInfoTable;
}

void BladeIIGameImpl::DestroyEtherSetOptionInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(EtherSetOptionInfoTable)
}

class UB2EtherSetInfoTable* BladeIIGameImpl::GetEtherSetEffectInfo()
{
	FString InfoTalbePath;
	GConfig->GetString(TEXT("/Script/BladeII.B2EtherSetEffect"), TEXT("EtherSetInfoTable"), InfoTalbePath, GGameIni);
	UClass* EtherSetEffectClassBP = LoadClass<UB2EtherSetInfoTable>(nullptr, *InfoTalbePath);

	if (EtherSetEffectClassBP && EtherSetEffectInfo == nullptr)
	{
		EtherSetEffectInfo = Cast<UB2EtherSetInfoTable>(CreastSingleStaticInfoAssetCommon(EtherSetEffectClassBP));
	}

	BII_CHECK(GMinimalDLCFrontMode || EtherSetEffectInfo);
	return EtherSetEffectInfo;
}

void BladeIIGameImpl::DestroyEtherSetEffectInfo()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(EtherSetEffectInfo)
}

UB2TotemRefineInfo* BladeIIGameImpl::GetTotemRefineInfoTable()
{
	if (!TotemRefineInfoTable)
	{
		TotemRefineInfoTable = Cast<UB2TotemRefineInfo>(CreastSingleStaticInfoAssetCommon(UB2TotemRefineInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || TotemRefineInfoTable);
	return TotemRefineInfoTable;
}

void BladeIIGameImpl::DestroyTotemRefineInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(TotemRefineInfoTable)
}

UB2ItemSortInfo * BladeIIGameImpl::GetItemSortInfoTable()
{
	if (!ItemSortInfoTable) {
		ItemSortInfoTable = Cast<UB2ItemSortInfo>(CreastSingleStaticInfoAssetCommon(UB2ItemSortInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || ItemSortInfoTable);
	return ItemSortInfoTable;
}

void BladeIIGameImpl::DestroyItemSortInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(ItemSortInfoTable)
}

UB2CharacterTagDialogueInfo* BladeIIGameImpl::GetCharacterTagDialogueInfoTable()
{
	if (!CharacterTagDialogueInfoTable) {
		CharacterTagDialogueInfoTable = Cast<UB2CharacterTagDialogueInfo>(CreastSingleStaticInfoAssetCommon(UB2CharacterTagDialogueInfo::StaticClass()));
	}
	BII_CHECK(GMinimalDLCFrontMode || CharacterTagDialogueInfoTable);
	return CharacterTagDialogueInfoTable;
}

void BladeIIGameImpl::DestroyCharacterTagDialogueInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(CharacterTagDialogueInfoTable)
}

UB2SeasonEventInfo* BladeIIGameImpl::GetSeasonEventInfoTable()
{
	//if (!SeasonEventInfoTable) {
	//	SeasonEventInfoTable = Cast<UB2SeasonEventInfo>(CreastSingleStaticInfoAssetCommon(UB2SeasonEventInfo::StaticClass()));
	//}
	//BII_CHECK(GMinimalDLCFrontMode || SeasonEventInfoTable);
	return SeasonEventInfoTable;
}

void BladeIIGameImpl::DestroySeasonEventInfoTable()
{
	BII_DESTROY_SINGLE_STATICINFOASSET_CODE_BLOCK(SeasonEventInfoTable)
}

UB2CompositeMeshCache* BladeIIGameImpl::GetCompositeMeshCacher()
{
	//if (!CompositeMeshCacher) {
	//	// Engine 생성할 때 처럼 TransientPackage 를 Outer 삼아서
	//	CompositeMeshCacher = NewObject<UB2CompositeMeshCache>(GetTransientPackage(), UB2CompositeMeshCache::StaticClass(), NAME_None);
	//	if (CompositeMeshCacher) {
	//		CompositeMeshCacher->AddToRoot();
	//	}
	//}

	BII_CHECK(GMinimalDLCFrontMode || CompositeMeshCacher); /* MinimalDLCFrontMode 라도 이건 생성해도 됨. 데이터 info 가 아님. */
	return CompositeMeshCacher;
}

void BladeIIGameImpl::DestroyCompositeMeshCacher()
{
	if (CompositeMeshCacher) 
	{ 
		CompositeMeshCacher->RemoveFromRoot();
		CompositeMeshCacher->ConditionalBeginDestroy();
		CompositeMeshCacher = NULL;
	}
}

UBannerManager* BladeIIGameImpl::GetLobbyBannerCacher()
{
	//LobbyBannerCacher = CreastSingleStaticInfoAssetCommon(UObject::StaticClass());
	if (!LobbyBannerCacher) {
		// Engine 생성할 때 처럼 TransientPackage 를 Outer 삼아서
		LobbyBannerCacher = NewObject<UBannerManager>(GetTransientPackage(), UBannerManager::StaticClass(), NAME_None);
		if (LobbyBannerCacher) {
			LobbyBannerCacher->AddToRoot();
		}
	}

	return LobbyBannerCacher;
}

void BladeIIGameImpl::DestroyLobbyBannerCacher()
{
	if (LobbyBannerCacher)
	{
		LobbyBannerCacher->RemoveFromRoot();
		LobbyBannerCacher->ConditionalBeginDestroy();
		LobbyBannerCacher = NULL;
	}
}

FClientDataStore& BladeIIGameImpl::GetClientDataStore()
{
	return ClientDataStore;
}

FStageDataStore& BladeIIGameImpl::GetStageDataStore()
{
	return StageDataStore;
}

RelicManager& BladeIIGameImpl::GetRelicManager()
{
	return m_RelicManager;
}

UnitySkillManager& BladeIIGameImpl::GetUnitySkillMissionManager()
{
	return m_UnitySkillMissionManager;
}

FLocalCharacterData& BladeIIGameImpl::GetLocalCharacterData()
{
	return ClientDataStore.GetLocalCharacterData();
}

FTutorialCharacterData& BladeIIGameImpl::GetTutorialCharacterData()
{
	return ClientDataStore.GetTutorialCharacterData();
}

FB2RedDotManager& BladeIIGameImpl::GetRedDotManager()
{
	return ClientDataStore.GetRedDotManager();
}

FB2ChatStore& BladeIIGameImpl::GetChatStore()
{
	return ChatStore;
}

FGuildDataStore& BladeIIGameImpl::GetGuildDataStore()
{
	return GuildDataStore;
}

FCostumeDataStore& BladeIIGameImpl::GetCostumeDataStore()
{
	return CostumeDataStore;
}

FTotemDataStore& BladeIIGameImpl::GetTotemDataStore()
{
	return TotemDataStore;
}

#if WITH_EDITOR
void BladeIIGameImpl::HandleEditorClosePIE(bool bIsSimulating)
{
	//if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	//{
	//	UIManager->EditorPIEEnd();
	//}
}
#endif
