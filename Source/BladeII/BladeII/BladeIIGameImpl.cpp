
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
///** Ư�� ���൵���� ���缱 ä �⳪�� �ð��� �Ҹ��ϴ� B2 ������ �ε� progresss �� ǥ���ϱ� ���� hack */
//float GB2ContentLoadingProgress = 0.0f; // 0.0 ~ 1.0
//										/** StartupModule �� ��ġ�鼭 �⺻ progress �� �����Ǿ� �ִ� ���� �̰ɷ� B2 ������ �ε� ��Ȳ�� ǥ���Ѵ�. 0 ~ 1 ���̷� */
//void EditorSetB2ContentLoadingProgressSplash(float InProgress)
//{
//	if (GIsEditor)
//	{
//		GB2ContentLoadingProgress = FMath::Clamp(InProgress, 0.0f, 1.0f);
//		// �⺻ splash �ؽ�Ʈ �ڿ��� ���� ��.
//		FString OurProgressText = FString::Printf(TEXT(" (Loading BladeII contents.. %d%%)"), (int32)(GB2ContentLoadingProgress * 100.0f / 1.0f));
//		SetAdditionalStartupProgressSplashText(OurProgressText);
//	}
//}
//void BeginB2ContentLoadingProgressSplash()
//{ // ������ �÷��׸� ������ ����..
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
//		//Editor���� ����������Ʈ�� LocRes�� �ε����� �ʰ� �־ ���ҽ��� FText ã�⿡ ������ �ִ�.
//		//�߰��� �ε��ؾ��� �ʿ䰡 �ִ�.
//		GConfig->SetString(TEXT("Internationalization"), TEXT("NativeGameCulture"), *FInternationalization::Get().GetCurrentCulture().Get().GetName(), GEditorSettingsIni);
//		FInternationalization::Get().OnCultureChanged().Broadcast();
//
//		// TAsset Lazy-loading �� �ϴ� ���µ��� �����Ϳ��� ��� �ε��� ����. �̰� �� �ϸ� Redirector ó���� ������ �߻��ϴ� ��찡 ����
//		// ���⼭ EditorLoadAll �� ���ڸ� �־ ���ο��� progress ����� �ϴ� �ֵ��� Ư���� ��� �ε��� ��� �޸𸮰� ���� �ö���� major InfoAsset ���ε� 
//		// �̰͵��� StaticFind** ���� �ð� �� �޸� �Ҹ� ���� EditorLoadAll ������ ���ٸ� �ð��� ���� �ʴ´ٸ� �ش� asset �� lazy-loading �� ������ �ϴ� �Ŷ�� ������ �� �� �ִ�.
//
//		UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox();
//		if (MobInfoBox)
//		{ // �� �̰� ���� ����Ǹ鼭 ���� �þ�� full load ���� ����.. �̰� full load �� �ص� redirector ó�� ������ �� ���� ��..
//			//MobInfoBox->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.02f, 0.4f);
//		}
//
//		UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox();
//		if (PCInfoBox)
//		{ // �̰� ��ϵ� asset �� � ������ �ʴµ� ���� redirector ó�� �߸��ȴٰ� �ؼ� ū�� ���� ������. LoadEditorData ���� ����
//		  //PCInfoBox->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.25f, 0.45f); EditorLoadAll �ϰ� �Ǹ� progress ���� ����
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
//		// ItemInfo ������ ���̴� ���̺� ����� ���� ���� �ε��ϴ� ���� �׷��� ������ ���� ��.
//		// DataTable ����̶� ������ full-load �� �� �� redirector ó���� ������ �ִ� ������ �ְ� ���۷��� �ϴ� ���ҽ��� ������ ���� �����Ϳ��� full load �ϴ� �� �����ϴ�.
//		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
//		if (ItemInfoTable) {
//			ItemInfoTable->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.2f, 0.8f);
//		}
//
//		B2_CONTENT_LOADING_PROGRESS_SPLASH(0.8f);
//
//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//		if (UIMgrInst)
//		{ // �굵 �ϴ� ������ load �� �� �ص� �� ���� ���� �� ���� ������ ��. ���� ���� ��� �þ�� �ű⵵ �ϰ�..
//		  //UIMgrInst->EditorLoadAll(EditorSetB2ContentLoadingProgressSplash, 0.65f, 0.96f); EditorLoadAll �ϰ� �Ǹ� progress ���� ����
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
//			SomeInfo->EditorLoadAll(); // ������ �߿��� �̰� �� ū ��.
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
//				FString::Printf(TEXT("[Warning] BladeII ��⿡�� �ʿ�� �ϴ� �ʼ� InfoAsset �� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."))
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

/** InfoAsset Async loading On/Off �÷���. ���� ������� On/Off �����ϰ��� �ϴ� �� �ƴϰ� ����� �����ϴٰ� Ȯ�ǽ� �� ������.. ���� ���� �����ߴµ� �̰ɷ� �ǽɵǴ� ũ������ �߻��� ���� ������ */
bool BladeIIGameImpl::bAllowInfoAssetAsyncLoading = true;
bool BladeIIGameImpl::bAllowUIAsyncLoading = true;
bool BladeIIGameImpl::bAllowUIOnDemandAsyncLoading = true;
float BladeIIGameImpl::UIOnDemandAsyncLoadingFlushTimeLimit = 0.5f;

#include "Developer/MessageLog/Public/MessageLogModule.h"

void BladeIIGameImpl::StartupModule()
{
//#if WITH_EDITOR 
//	BeginB2ContentLoadingProgressSplash(); // Full �ε� �ϴ��� progress �� ǥ���ϸ� ������� ����..
//	if (GIsEditor) {
//		SetManangedChunkIDAssingmentFn(); // ��ŷ ���� Ŀ���� ��� ���� �¾�
//	}
//#endif
//
//	CombatStatEval::InitializeUnitedOptionMapping(); // ������ ���� �ʱ�ȭ ���� �־�� ��.
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
//	// LoadEditorData ������ TAsset ���� �ϰ� �ε��ϴµ� skip �ϸ� redirector ó���� ������ �߻��� �� �ִ�. ��ŵ �ɼ��� ���� �ƴ� ����� ���. �ܺο� �˸��� �ʴ´�.
//	bool bSkipLoadEditorData = (FParse::Param(FCommandLine::Get(), TEXT("UnsafeQuickEditor")) || FParse::Param(FCommandLine::Get(), TEXT("J3M4"))); // J3M4 �� �������� �����ε� ���� ����� ������ �ٸ� ����� �ε��� �̷��� Ī�ߴ� ���� ����. ������ �̰͵� ������� �������̱⵵ �ϰ�.
//	if (!bSkipLoadEditorData)
//	{
//		// ������ progress ǥ�ø� ���� �����ϱ� ���� �̰� ����.
//		// �����Ϳ����� Lazy-loading �� �ϴ� �ֵ��� ��� �ε��� �д�. Redirector ó�� �� ������ ���� �� ��ŷ�� ���ԵǴ� ���� �����ϱ� ����. �׸��� �� �׷� �� �ƴϾ �����ʹ� ��� ���� �̸� �ε��� �δ� �� ���� �鵵.. 
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
//	B2_CONTENT_LOADING_PROGRESS_SPLASH(0.98f); // �������� ��� ���Ŀ��� ���� �� �ȵ� ��..
//
//	GetStageDataStore();	// test
//	
//	LoadLocalIniStateOnStartup(); // ���� ini ���� ���� �� ���� ���������� ���� �͵�
//	LoadIniSystemSettings(); // ���������� ini �ε� ���� ���°� �ƴ� ���� �ý��� ������?
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
//		// Ư�� LoadEditorData �� ���Ե��� ���� InfoAsset ���� ��ŷ�� Ȯ���� ���Խ�Ű���� ���⼭ ������ ���۷����� ��� �� �ʿ䰡 �ִ�.
//		// -> ��, Blueprints/InfoAsset ������ ��ŷ�� ���� ���Խ�Ų�ٸ� �ʼ����� �ƴϴ�. �׷��� ������ġ ������ �����ص� �� ��.
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
//		// Cooking�� TutorialAsset ���Խ�Ű�� ����
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
	// �̰� �Ҹ����� �������� ���� ���� ������ ��ƾ� �Ѵ�. (FPlatformMisc::RequestExit(false)
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
//	// �ȵ���̵� ��� ���� ��ư���� �� ��� ���̰� �� �� �Ҹ�. Ȩ��ư�� ��������.
//	// �̸����� ������ �� �ִ� �Ͱ��� �޸� �̰� �� �� ���Ḧ ���ϴ� �� �ƴ����� ����� ����� ���� ����.
//	// �⺻ �ȵ���̵� �� ���� �������� ��ĥ �� �ִ� ���� �Ϻθ� ����.
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
//	// �̰� �Ҹ����� �������� ���� ���� ������ ��ƾ� �Ѵ�. (FPlatformMisc::RequestExit(false)
//	//
//
//	// �ֿ� ���� �Ŵ����� ���⼭ ����.
//	CloseB2GameSubsystemsOnShutdown();
//
//	SaveLocalIniStateOnExit();  // �÷��������� ��� ������ ���� ShutdownModule �� �� �Ҹ� �� �����Ƿ� �̰� �� �������� ó���� �ʿ䰡 ����.
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
		return NULL; // DLC Front ��� ���ҽ��ε� �ִ��� ����
	}

#if WITH_EDITOR // In formal game, InfoAsset loading must be with BladeIIGameMode (not with StartupModule).
	checkSlow(CanLoadInfoAssetWhileStartupModule() || ABladeIIGameMode::bDevCheckBladeIIGameModeEnteredOnce);
#endif

	/* Engine ������ �� ó�� TransientPackage �� Outer ��Ƽ� */
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
	//	// Engine ������ �� ó�� TransientPackage �� Outer ��Ƽ�
	//	CompositeMeshCacher = NewObject<UB2CompositeMeshCache>(GetTransientPackage(), UB2CompositeMeshCache::StaticClass(), NAME_None);
	//	if (CompositeMeshCacher) {
	//		CompositeMeshCacher->AddToRoot();
	//	}
	//}

	BII_CHECK(GMinimalDLCFrontMode || CompositeMeshCacher); /* MinimalDLCFrontMode �� �̰� �����ص� ��. ������ info �� �ƴ�. */
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
		// Engine ������ �� ó�� TransientPackage �� Outer ��Ƽ�
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
