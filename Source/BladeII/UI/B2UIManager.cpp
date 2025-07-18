
#include "B2UIManager.h"
#include "EventSubsystem.h"
#include "GameDelegates.h"
#include "B2UIMain.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUICondition.h"
#include "B2UIDocBindable.h"
#include "B2UIManager_InGameHUDChar.h"
#include "B2ErrorCodeInfo.h"
#include "B2UIToolTipTextBox.h"
#include "Retailer.h"
#include "B2UIManager_Lobby.h"
#include "B2UILinkManager.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2DebugTextWidget.h"
#include "BladeIIGameMode.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIMailRewardPopUp.h"
#include "B2AndroidBackManager.h"
#include "B2UIBackWidget.h"
#include "Global.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2GameLoadingProgress.h"
#include "B2GameInstance.h"
#include "B2ServerResultCodeTable.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "B2PlatformAPI.h"
#include "B2UIHallOfFame.h"
#include "B2StageManager.h"
#include "../Common/Event.h"
#include "B2MessageInfoCommonStruct.h"
#include "../BladeII.h"

UB2UIManager* UB2UIManager::Instance = nullptr;

FName FB2UIWidgetData::GetWidgetAssetName()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FB2UIWidgetData_GetWidgetAssetName);
	if (WidgetClass.IsValid())
	{
		FString WidgetPath = WidgetClass.ToString();
		int32 BeginIdx = WidgetPath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (BeginIdx != INDEX_NONE)
		{
			int32 LastIdx = WidgetPath.Find(TEXT("."), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (LastIdx != INDEX_NONE)
			{
				int32 Length = (LastIdx - BeginIdx) - 1;
				FString WidgetName = WidgetPath.Mid(BeginIdx + 1, Length);
				return FName(*WidgetName);
			}
		}
	}

	return FName(TEXT("NoAsset"));
}
void FB2UIWidgetData::ResetAllCacheMarkForUnload()
{ 
	// CanUnloadByPolicy �� true �� ���� Unload ���� �������� �ϱ� ����.
	// �׷��� �̷��� �ϰ� Unload �� �ص� RootSet �̶�� �������� unload �� ������ ���� ��.
	bCacheOnLoad = false;
	bCacheByHistory = false;
}

UB2UIManager::UB2UIManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_ShortagePopupMGR(nullptr)
{
	MainWidget = nullptr;

	CurrUIScene = EUIScene::None;
	ClosingUIScene = EUIScene::None;
	OpeningUIScene = EUIScene::None;
	bDontCloseSameUIAtReservedUIScene = true;
	bClosingUIScene = false;
	bOpeningUIScene = false;
	ChangeUISceneTransitionType = EUIChangeUISceneTransitionType::NO_CANCEL;
	ReservedUISceneOpenData.Key = EUIScene::None;
	ReservedUISceneOpenData.Value = 0;
	UISceneHistory.Empty();
	bOpeningDJLegacyHeroMgmtBySceneHistory = false;
	Instance = nullptr;
	bInit = false;

	bCinematicMode = false;
	bHUDHidingCinematicMode = false;

	MsgPopupCnt = 0;

	FloatNumberOption.MaximumFractionalDigits = 2;
	FloatNumberOption.MinimumFractionalDigits = 1;

	ForceGCOnUISceneCloseInterval = 0;
	UISceneCloseWithoutGCCount = 0;
	UIDataCacheHistoryNum = 5;
	ScreenDebugTextWidget = NULL; // Shipping ���忡���� NULL ���� �����ϰ� �־��.

	ErrorResetPerSecond = 5;
	IsIndicatorVisible = false;
}

UB2UIManager::~UB2UIManager()
{
	// � ������Ʈ�� � ��η� �Ҹ��ڷ� �������� �� �� ����. Ư�� ���μ��� ����� Destroy �� �Ҹ��� ������ ��. 
	// �Ϲ����� ��Ȳ������ Destroy �� ABladeIIGameMode::OnPreLoadMap ���� ó��.
	Destroy();
	/*if (m_ShortagePopupMGR)
	{
		delete m_ShortagePopupMGR;
		m_ShortagePopupMGR = nullptr;
	}*/
}

UWorld* UB2UIManager::GetWorld() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetWorld);
	if (HasAllFlags(RF_ClassDefaultObject))
	{
		// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
		return nullptr;
	}

	// Use the Player Context's world, if a specific player context is given, otherwise fall back to
	// following the outer chain.
	if (PlayerContext.IsValid())
	{
		if (UWorld* World = PlayerContext.GetWorld())
		{
			return World;
		}
	}

	// Could be a GameInstance, could be World, could also be a WidgetTree, so we're just going to follow
	// the outer chain to find the world we're in.
	UObject* Outer = GetOuter();

	while (Outer)
	{
		UWorld* World = Outer->GetWorld();
		if (World)
		{
			return World;
		}

		Outer = Outer->GetOuter();
	}

	return nullptr;
}

void UB2UIManager::SetPlayerContext(const FLocalPlayerContext& InPlayerContext)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_SetPlayerContext);
	PlayerContext = InPlayerContext;

	//Ȥ�� �𸣴ϱ� ���� �������� ���ؽ�Ʈ�� �ٲ��ش�
	for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget)
		{
			ThisWidgetData.Widget->SetPlayerContext(InPlayerContext);
		}
	}
}

UB2UIManager* UB2UIManager::GetInstance()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetInstance);
	if (GMinimalDLCFrontMode) {
		return NULL; // DLC Front ��� ���ҽ��ε� �ִ��� ����
	}

#if WITH_EDITOR // Like other InfoAssets, this must be with BladeIIGameMode (not with StartupModule).
	checkSlow(CanLoadInfoAssetWhileStartupModule() || ABladeIIGameMode::bDevCheckBladeIIGameModeEnteredOnce);
#endif

	if (Instance == nullptr || Instance->GetClass() == nullptr)
	{
		FString UIManagerClassName;
		GConfig->GetString(TEXT("/Script/BladeII.B2UIManager"), TEXT("B2UIManagerClassName"), UIManagerClassName, GGameIni);
		UClass *UIManagerClass = LoadClass<UObject>(NULL, *UIManagerClassName, NULL, LOAD_None, NULL);
		if (UIManagerClass)
		{ // Init �� �ۿ��� �ҷ��� ��.
			Instance = NewObject<UB2UIManager>(GetTransientPackage(), UIManagerClass);
			if (Instance)
			{
				Instance->AddToRoot();
			}
		}
	}
	BII_CHECK(Instance);
	return Instance;
}

#if WITH_EDITOR
UB2UIManager* UB2UIManager::EditorGetNewInstance()
{ // �����Ϳ��� Destroy �� ����� �ȵ�ä �����Ⱚ Instance �� ����ϰ� �Ǵ� ��찡 �־ �غ���.
	check(GIsEditor);
	Instance = nullptr;
	return GetInstance();
}
#endif

void UB2UIManager::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	EditorCheckForWidgetNameIntegrity();
#endif
}

#if WITH_EDITOR
void UB2UIManager::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_AllWidgetData = GET_MEMBER_NAME_CHECKED(UB2UIManager, AllWidgetData);
	const FName Name_WidgetFNames = GET_MEMBER_NAME_CHECKED(FB2UISceneData, WidgetFNames);
	const FName Name_StayInViewport = GET_MEMBER_NAME_CHECKED(FB2UIWidgetData, bStayInViewport);
	const FName Name_AllSceneData = GET_MEMBER_NAME_CHECKED(UB2UIManager, AllSceneData);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_AllWidgetData || PropertyName == Name_WidgetFNames
		|| PropertyName == Name_AllSceneData
		)
	{
		EditorCheckForWidgetNameIntegrity();
	}
	if (PropertyName == Name_StayInViewport)
	{
		TArray<FName> WidgetNamesWithStayInViewport;
		for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
		{
			FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
			if (ThisWidgetData.bStayInViewport)
			{
				WidgetNamesWithStayInViewport.Add(ThisWidgetData.WidgetFName);
			}
		}
		if (WidgetNamesWithStayInViewport.Num() > 0)
		{
#if !PLATFORM_MAC
			//FString WarnMsg = FString::Printf(TEXT("���� %d ���� WidgetData �� StayInViewport �� üũ�Ǿ� �ֽ��ϴ�. �̴� ���ۿ�� ���׸� ������ �� ������ �����ؼ� ����ؾ� �մϴ�.\nUI ��ȯ ���� ���ŷ�� ��ȭ�ϱ� ���� �����̶�� ���� �������� �Ұ��� �������� ���� ������ �ּ���\nhttp://172.168.2.105:8090/pages/viewpage.action?pageId=4827590\n\n"), WidgetNamesWithStayInViewport.Num());
			FString WarnMsg = FString::Printf(TEXT(" %d "), WidgetNamesWithStayInViewport.Num());

			for (int32 WI = 0; WI < WidgetNamesWithStayInViewport.Num(); ++WI)
			{
				WarnMsg += FString::Printf(TEXT("%d. %s\n"), WI + 1, *WidgetNamesWithStayInViewport[WI].ToString());
			}

			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsg));
#endif
		}
	}

}

void UB2UIManager::EditorCheckForWidgetNameIntegrity()
{
	// FB2UIWidgetData �� WidgetFName �� ���� �����ϵ��� ���� �ʰ� ������ ��ϵ� ���� Ű ������ ������.
	for (TMap<FName, FB2UIWidgetData>::TIterator NewWidgetDataIt(AllWidgetData); NewWidgetDataIt; ++NewWidgetDataIt)
	{
		FB2UIWidgetData& ThisWidgetData = NewWidgetDataIt.Value();
		if (ThisWidgetData.WidgetFName != NewWidgetDataIt.Key())
		{
			ThisWidgetData.WidgetFName = NewWidgetDataIt.Key();
		}
	}
	for (TMap<EUIScene, FB2UISceneData>::TIterator SceneDataIt(AllSceneData); SceneDataIt; ++SceneDataIt)
	{ // SceneData �� ���������� SceneName �� Map �� Ű������ �����´�.
		FB2UISceneData& ThisSceneData = SceneDataIt.Value();
		if (ThisSceneData.SceneName != SceneDataIt.Key())
		{
			ThisSceneData.SceneName = SceneDataIt.Key();
		}
	}

#if !PLATFORM_MAC
	FString NotFoundWidgetNameMsg = TEXT("SceneData �� ������ ���� WidgetFName �� AllWidgetData ���� �߰ߵ��� �ʾҽ��ϴ�.\n\n");
	int32 FoundSomeInvalidCount = 0;
	for (TMap<EUIScene, FB2UISceneData>::TIterator SceneDataIt(AllSceneData); SceneDataIt; ++SceneDataIt)
	{
		FB2UISceneData& ThisSceneData = SceneDataIt.Value();
		for (FName& ThisSceneWidgetFName : ThisSceneData.WidgetFNames)
		{
			// DJLegacy ���� ���� ������ ���� ������ �� ������ �װ� �˻翡�� ����.
			if (!IsDJLegacyUIScene(ThisSceneData.SceneName) && !AllWidgetData.Find(ThisSceneWidgetFName))
			{
				NotFoundWidgetNameMsg += FString::Printf(TEXT("SceneData SceneName %d �� WidgetFName %s\n"), (int32)ThisSceneData.SceneName, *ThisSceneWidgetFName.ToString());
				++FoundSomeInvalidCount;
			}
		}
	}
	if (FoundSomeInvalidCount > 0)
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(NotFoundWidgetNameMsg));
	}
#endif
}

void UB2UIManager::EditorPIEEnd()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_EditorPIEEnd);

	for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget)
			ThisWidgetData.Widget->PIEEndDestroy(this);
	}
}

#endif // WITH_EDITOR

#if 0 // �̰� ����ϰ� �Ǹ� �ᱹ �� �ε� �ð��� ������� �� ���̴�. Ʈ���̵������ ��Ȯ�ϹǷ� �ܼ� CacheOnLoad �� �ϰų� ������ ������ AsyncLoad �� �ϰų�.
void UB2UIManager::SyncPreloadTAssets(const TArray<FName>& InPreloadUIName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_PreloadTAssets);
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::PreloadTAssets"));

	for (const FName ThisPreloadName : InPreloadUIName)
	{
		FB2UIWidgetData* FoundWidgetData = GetWidgetData(ThisPreloadName);
		if (FoundWidgetData)
		{
			FoundWidgetData->bCacheOnLoad = true; // CachedWidgetClasses ���� ĳ���� ���� OpenUI �� ���� Unload ���� �ʵ���.
			GetWidgetClass(FoundWidgetData); // TAsset Ŭ���� �ε�.
		}
	}
}
#endif

void UB2UIManager::ReqAsyncLoadAsset(const FName& InLoadUI)
{
	TArray<FName> LoadUIs;
	LoadUIs.Add(InLoadUI);
	ReqAsyncLoadAssets(LoadUIs);
}
void UB2UIManager::ReqAsyncLoadAssets(const TArray<FName>& InLoadUIs)
{
	if (BladeIIGameImpl::bAllowUIAsyncLoading == false)
	{
		// bAllowUIAsyncLoading �� ���� SyncPreload ���� ��ȯ�� ���⼱ �� ������ �� �ʿ䰡 �ִ�.
		// UIManager �� Async Load �� PreLoad Ÿ�̹��� ��Ƽ�ھ� Ȱ���� ������ �ƴ϶� �ΰ��� ���� ���� ��׶��� �ε��� ������.
		//SyncPreloadTAssets(InLoadUIs); // ���� ���⼱ �ʿ��� �� �ε��ϵ��� �׳� ���� ��
		return;
	}

	for (const FName& UIName : InLoadUIs)
	{
		if (FB2UIWidgetData* WidgetData = GetWidgetData(UIName))
		{
			// UIManager::AsyncLoading�� 1 RequestName : 1 UI Asset�� ��Ģ���� �Ѵ�. 1 : �� �� �ȵ�
			WidgetData->bCacheOnLoad = true; // �ε� �Ϸ� �� ĳ���� �ؾ� �ϴµ� ��ũ�� ���� �� �ߴٰ��� �ٸ� �ʿ��� ��ε��� �ϰ� �� ���� ������ bCacheOnLoad ��ũ.
			const auto& ClassAsset = WidgetData->GetWidgetClassAsset();
			TryAsyncLoad(UIName.ToString(), TArray<FSoftObjectPath>{ClassAsset.ToSoftObjectPath()});
		}
	}
}

void UB2UIManager::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& CompleteRequestList)
{
	if (CompleteRequestList.IsValidIndex(0))	// �ϳ��� Request�� �ϳ��� UI��
	{
		const FSoftObjectPath& LoadedAsset = CompleteRequestList[0];
		if (UClass* LoadedClassAsset = Cast<UClass>(LoadedAsset.ResolveObject()))
		{
			const FName UIName(*CompleteRequest);
			// Async �ε� ����� CanUnloadByPolicy �� üũ�� �ʿ䰡 ����. ���� ������ �𸣴� �Ŵ� ������ ĳ���� ��.
			// �� ��û ������ bCacheOnLoad �� ������ ���� �ʿ�� �ִ�. �� �׷��� �ٸ� �� �ڵ鸵 ���߿� ��ε��� �Ǿ� ���� ������ �ִ�.
			UClass*& FoundOrAddedCache = CachedWidgetClasses.FindOrAdd(UIName);
			FoundOrAddedCache = LoadedClassAsset;
		}
	}
}

void UB2UIManager::CloseAllUIs() // ��ǻ� PreDestroy ������ �� ���� ��ε� �ÿ� �Ҹ����� ��.
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllUIs);
	CloseAllMsgPopups();

	ChangeUIScene(EUIScene::None);

	for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget)
		{
			// bCloseStaticUIs �� true �� �Ѱ��־ bStayInViewport �� �͵鵵 ��� �ݴ´�.
			CloseUI(ThisWidgetData.WidgetFName, false, true);
		}
	}
}

void UB2UIManager::CloseAllStandaloneUIs()
{ // UIScene �� ���ܵΰ� ��Ÿ ������ �˾��̳� ������ ���� UI �鸸 ���ַ��� ��.
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllStandaloneUIs);
	CloseAllMsgPopups();

	FB2UISceneData* CurSceneData = GetUISceneData(CurrUIScene);

	for (TMap<FName, FB2UIWidgetData>::TConstIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		const FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget)
		{
			if (CurSceneData && CurSceneData->WidgetFNames.Contains(ThisWidgetData.WidgetFName))
			{
				continue; // ���� Scene ���� �����ִ� �� ���� ����.
			}
			// bCloseStaticUIs �� true �� �Ѱ��־ bStayInViewport �� �͵鵵 ��� �ݴ´�.
			CloseUI(ThisWidgetData.WidgetFName, false, true);
		}
	}
}

void UB2UIManager::Destroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_Destroy);
#if WITH_EDITOR
	if (GIsEditor) {
		CachedWidgetClasses.Empty(); // �����͸� Destroy �� ���� �Ҹ� ��Ȳ���� Ȯ���� ��쵵��.
	}
#endif

	UnsubscribeEvents();

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
	DestroyScreenDebugWidget();
#endif

	if (Instance)
	{
		Instance->ConditionalBeginDestroy();
		Instance = nullptr;
	}

	if (MainWidget)
	{
#if WITH_EDITOR
		if (!GIsEditor) { // �����ʹ� �� �ٸ�. DestroySelf ��ü�� �ʿ���� ���̱⵵.
			MainWidget->DestroySelf(this);
		}
#else
		MainWidget->DestroySelf(this);
#endif
		MainWidget = nullptr;
	}

	bInit = false;

	//GC
	//GetWorld()->ForceGarbageCollection(true);
}

// Level transition �� �̰ɷ� clear..
void UB2UIManager::MasterDestroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_MasterDestroy);
	if (!GIsEditor)
	{ // �ٸ� TAsset ���� ManualUnloadBeforeNewBeginning ���� �ϴ� ��.
		UnloadAllTAssets();
	}

	CloseAllUIs();
	Destroy();
}

void UB2UIManager::Init(const FLocalPlayerContext& InPlayerContext)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_Init);
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::Init"));

	SetPlayerContext(InPlayerContext);

	if (GConfig)
	{
		GConfig->GetInt(TEXT("/Script/BladeII.B2UIManager"), TEXT("ForceGCOnUISceneCloseInterval"), ForceGCOnUISceneCloseInterval, GGameIni);
		GConfig->GetInt(TEXT("/Script/BladeII.B2UIManager"), TEXT("UIDataCacheHistoryNum"), UIDataCacheHistoryNum, GGameIni);
	}
	UISceneCloseWithoutGCCount = 0;

	SetupCacheOnLoadList();
	SetupRootsetWidgetList();
	SetupLoadOnStartupWidgetList();

	UWorld* TheWorld = GetWorld();
	if (TheWorld && !bInit)
	{
#if WITH_EDITOR
		if (!FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) //��ø ���ε� �ȵǰ�
			FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &UB2UIManager::Destroy);
#endif

		MainWidget = Cast<UB2UIMain>(CreateWidget<UUserWidget>(TheWorld, this, UIMainClass));
		if (MainWidget)
		{
			MainWidget->Init();
			MainWidget->AddToViewport(BII_WIDGET_ZORDER_NEW_UI_MAIN); // Z-order �� ���� �����ϴ� DJLegacy �� �����ϴ� �� ���� z ���� �ִ´�.
			PrepareUI();
			bInit = true;
		}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
		CreateScreenDebugWidget();
#endif

		SubscribeEvents();
	}

	InitNotAddUISceneHistoryList();

	//�ݴ�� ��⿡ ����ѰŸ� 
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_INTERNAL_ERROR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_INTERNAL_DATABASE_ERROR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_TRY_AGAIN_ERROR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_MASTER_DATA_ERROR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_INVALID_MARKET);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_MAPPER_PARAMETER_ERROR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_PARAMETERS_CHECKSUM_ERROR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_RANDOM_FAILURE_COUNT_INSERT);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_RANDOM_FAILURE_COUNT_UPDATE);

	/*NoCheckErrorCode.Add(B2ResultCode::FAILURE_CASH_UNDERFLOW);

	NoCheckErrorCode.Add(B2ResultCode::FAILURE_BLADEPOINT_UNDERFLOW);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_SOCIALPOINT_UNDERFLOW);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_SOCIALPOINT_OVERFLOW);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NOT_ENOUGH_INVENTORY_LEFT_SLOT);


	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NAME_INVALID_CHAR);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NAME_LENGTH_TOO_SHORT);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NAME_LENGTH_TOO_LONG);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NAME_INCLUDE_SWEAR_WORD);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NAME_INCLUDE_PROHIBITED_NAME);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_MONEY_UNDERFLOW);

	NoCheckErrorCode.Add(B2ResultCode::FAILURE_TODAY_MERCENARY_DONATION_COUNT_OVERFLOW);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_TRANSACTION_WHILE_INCREASE_MERCENARY_EXP);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_MERCENARY_ALREADY_MAX_LEVEL);

	NoCheckErrorCode.Add(B2ResultCode::FAILURE_DUPLICATED_GUILD_NAME);
	NoCheckErrorCode.Add(B2ResultCode::FAILURE_NEED_MORE_MEMBER_FOR_GUILDBATTLE);*/
}

void UB2UIManager::PrepareUI()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_PrepareUI);
	for (TMap<FName, FB2UIWidgetData>::TConstIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		const FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		{
			//WidgetData->WidgetClassLoaded = WidgetData->GetUserWidgetClass(InfoLoadManager); //Used to be streamable.
			//����Ʈ�� �׻� ����� �־�� �ϴ� �༮��. CreateWidget�� �׶��׶� �ϸ鼭 ����� ���ŷ�� ����(���� ����ϴ� �༮�� ����� ��~)
#if 0 // bStayInViewport �� ���� ó������ ������ �ʰ� ó�� �� ���ķ� ���� �ʴ� �ɷ� �ٲ۴�.
			if (WidgetData->bStayInViewport)
			{
				UClass* WidgetClass = GetWidgetClass(*WidgetData);
				auto* PreCreateWidget = CreateWidget<UB2UIWidget>(GetWorld(), this, WidgetClass);

				if (PreCreateWidget)
				{
					WidgetData->Widget = PreCreateWidget;
					PreCreateWidget->MyUIFName = WidgetData->WidgetFName;
					PreCreateWidget->Init();

					//�׻� ������� UI�� ���,Open�ɶ� �� �ƴ϶� �� ó�� �����ɶ��� DocBind�� ��ȸ�� �־�����. �ֳĸ� �׻� �������ϱ�...
					IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(PreCreateWidget);
					if (CastDocBindable)
						CastDocBindable->BindDocAuto();

					if (MainWidget)
					{
						auto MyParent = MainWidget->GetParentLayer(WidgetData->Layer);
						if (MyParent)
						{
							MyParent->AddChild(PreCreateWidget);
							auto OverlaySlot = Cast<UOverlaySlot>(PreCreateWidget->Slot);
							if (OverlaySlot)
							{
								OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
								OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
							}
						}
						else
							PreCreateWidget->AddToViewport();
					}
					else
						PreCreateWidget->AddToViewport();

					PreCreateWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
#endif
		}
	}
}

#include "B2UIBattlePause.h"
void UB2UIManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(StartPauseMenuClass<EB2GameMode>::GetInstance().Subscribe2(
		[this](EB2GameMode Mod)
		{
			if (auto* pUI = OpenUI<UB2UIBattlePause>(UIFName::BattlePause))
				pUI->SetMod(Mod);
		}
	));

	Issues.Add(StopPauseMenuClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->CloseUI(UIFName::BattlePause);
		}
	));

	Issues.Add(StartDefeatMenuClass<EB2GameMode>::GetInstance().Subscribe2(
		[this](EB2GameMode Mod)
		{
			this->OpenUI(Mod == EB2GameMode::Raid ? UIFName::RaidDefeat : UIFName::Defeat);
		}
	));

	Issues.Add(StopDefeatMenuClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->CloseUI(UIFName::Defeat);
			this->CloseUI(UIFName::RaidDefeat);
		}
	));

	Issues.Add(LobbyConditionalBeginOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Subscribe2(
		[this](const FVector2D& InPressPos)
		{
			if (MainWidget)
			{
				MainWidget->ConditionalPlayOnTouchImpulseRing(InPressPos);
			}
		}
	));

	Issues.Add(LobbyMoveOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Subscribe2(
		[this](const FVector2D& InMovePos)
		{
			if (MainWidget)
			{
				MainWidget->MoveOnTouchImpulseRing(InMovePos);
			}
		}
	));

	Issues.Add(LobbyEndOnTouchImpulseRingClass<>::GetInstance().Subscribe2(
		[this]()
		{
			if (MainWidget)
			{
				MainWidget->PendingStopOnTouchImpulseRing();
			}
		}
	));

	Issues.Add(LobbyShowNetworkLoadingIconClass<bool>::GetInstance().Subscribe2(
		[this](bool bShow)
		{
			if (MainWidget)
			{
				MainWidget->ShowNetworkLoadingIcon(bShow);
				this->IsIndicatorVisible = bShow;
			}
		}
	));

	Issues.Add(ShowMiscLoadingIconClass<bool>::GetInstance().Subscribe2(
		[this](bool bShow)
		{
			if (MainWidget)
			{
				MainWidget->ShowMiscLoadingIcon(bShow);
			}
		}
	));

	Issues.Add(HandleServerErrorGoodsShortageClass<const uint32, const EGoodsButtonType>::GetInstance().Subscribe2(
		[this](const uint32 ItemID, const EGoodsButtonType GoodsButtonType)
		{
			if (auto* pUI = OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup))
				pUI->SetItemInfo(ItemID, GoodsButtonType);
		}
	));

	Issues.Add(HandleServerError4018Class<>::GetInstance().Subscribe2(
		[this]()
		{
			this->ResetRepeatBattle();
			CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
		}
	));

	Issues.Add(DeliveryAppUpdatePopUpClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->OpenAppUpdatePopup();
		}
	));
}

void UB2UIManager::UnsubscribeEvents()
{
	Issues.Empty();
}

FB2UIWidgetData* UB2UIManager::GetWidgetData(FName InUIName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetWidgetData);
	FB2UIWidgetData* FoundFromMap = AllWidgetData.Find(InUIName);
	if (FoundFromMap)
	{
		checkSlow(FoundFromMap->WidgetFName == InUIName);
		return FoundFromMap;
	}
	return NULL;
}

FB2UIWidgetData* UB2UIManager::GetWidgetDataByWidget(UB2UIWidget* Widget)
{
	// UIWidget �� �ĺ��ڷ� �ؼ� ã������ ©���� ��� iteration �ؾ� �ϴ� �̰� Ư���� ��쿡�� �����.. 
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetWidgetDataByWidget);
	for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget && ThisWidgetData.Widget == Widget) {
			return &ThisWidgetData;
		}
	}

	return nullptr;
}

FB2UISceneData* UB2UIManager::GetUISceneData(EUIScene UISceneName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetUISceneData);

	FB2UISceneData* FoundSceneData = AllSceneData.Find(UISceneName);
	if (FoundSceneData)
	{
		return FoundSceneData;
	}
	return nullptr;
}

void UB2UIManager::Tick(float DeltaTime)
{
	CheckClosingCurrUISceneAndChangeToReservedUIScene();

	CheckErrorTimeStamp(DeltaTime);
}

bool UB2UIManager::IsTickable() const
{
	return true;
}

TStatId UB2UIManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UB2UIManager, STATGROUP_Tickables);
}

bool UB2UIManager::IsTickableWhenPaused() const
{
	return true;
}

bool UB2UIManager::IsTickableInEditor() const
{
	return true;
}

void UB2UIManager::CloseUI(FName UIName, bool RightNow, bool bCloseStaticUIs)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseUI);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UIManager::CloseUI %s"), *UIName.ToString()));

	auto Widget = GetUI<UB2UIWidget>(UIName);
	if (Widget)
		InternalCloseUI(UIName, Widget, RightNow, bCloseStaticUIs);
}

TSubclassOf<UB2UIWidget> UB2UIManager::GetWidgetClass(const FB2UIWidgetData* WidgetData) // �ܼ� Get ���� �ƴ� �ε��� ��.
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetWidgetClass);

	if (WidgetData == nullptr)
		return nullptr;

	const FName& WidgetFName = WidgetData->WidgetFName;
	if (UClass* CachedWidgetClass = FindCachedWidgetClass(WidgetFName))
		return CachedWidgetClass;

	TSoftClassPtr<UB2UIWidget> WidgetClassAsset = WidgetData->GetWidgetClassAsset();
	if (WidgetClassAsset.IsNull() == false)
	{
		UClass* WidgetClass = LoadSynchronous<UB2UIWidget>(WidgetClassAsset, GENERAL_TASSET_ASYNC_LOAD_TIMEOUT);
		if (WidgetClass != nullptr)
		{
			if (!WidgetData->CanUnloadByPolicy()) { // � �����ε� Cache ��ũ�� �ִ� ���, ��ε� ���� ĳ��
				UClass*& FoundOrAddedCache = CachedWidgetClasses.FindOrAdd(WidgetFName);
				FoundOrAddedCache = WidgetClass;
			}

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� �޸� ����� ���� �̷� �� ���� �ʴ´�..

			if (UB2UIManager::IsRootSetWidgetName(WidgetFName))
			{
				// �̰� �ƿ� ���� ���� �������� �� �����ڴٴ� ���� ������ �ǻ� ǥ��.
				// �ε� �ð��� ����� ��鼭 ���� ���̴� �͵�� ����.
				WidgetClass->AddToRoot();
			}

#endif // end !PLATFORM_IOS

			return WidgetClass;
		}
	}

	return nullptr;
}

UClass* UB2UIManager::FindCachedWidgetClass(const FName& WidgetFName)
{
	if (UClass** CachedClassPtr = CachedWidgetClasses.Find(WidgetFName))
		return *CachedClassPtr;

	return nullptr;
}

void UB2UIManager::Unload(const FB2UIWidgetData* WidgetData)
{
	if (WidgetData != nullptr)
	{
		// CacheOnLoad ���� ��ũ�� �� �� UnloadAllTAssets ���� ���� �÷��׸� ������ ���Ŀ� �̰� �� �ؾ� �Ѵ�.
		checkSlow(WidgetData->CanUnloadByPolicy());

		CachedWidgetClasses.Remove(WidgetData->WidgetFName);

		TSoftClassPtr<UB2UIWidget> WidgetClassAsset = WidgetData->GetWidgetClassAsset();
		UnloadAsset(WidgetClassAsset.ToSoftObjectPath());
	}
}

void UB2UIManager::InternalOpenUI(FName InUIName, UB2UIWidget* Widget, bool bRightNow, bool bMustTop)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_InternalOpenUI);
	auto WidgetData = GetWidgetData(InUIName);
	if (!WidgetData || !Widget) return;

	// bStayInViewport �� ���� ó������ ������ �ʰ� ó�� �� ���ķ� ���� �ʴ� �ɷ� �ٲ۴�.
	//if (!WidgetData->bStayInViewport)
	{
		// 20180124_YJ
		// �⺻������ ����� �������� MainWidget�� ����������� �ٿ�������,
		// ���Ʈ��ũ���� ������ �ֻ�ܿ� ��ġ �ؾ߸��ϴ� �˾��� ��쿡 MainWidget�� �����ϰ�
		// AddToViewport �� Zoder ���� 1000���� �����Ͽ� �����
		if (MainWidget && !bMustTop)
		{
			auto MyParent = MainWidget->GetParentLayer(WidgetData->Layer);
			if (MyParent)
			{
				MyParent->AddChild(Widget);
				auto OverlaySlot = Cast<UOverlaySlot>(Widget->Slot);
				if (OverlaySlot)
				{
					OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}
			}
			else
				Widget->AddToViewport();
		}
		else
			Widget->AddToViewport(bMustTop ? BII_WIDGET_ZORDER_NEW_UI_MustTop : 0);
	}

	//���� �� �� �ڵ����ε� �ؾ� �ȴٸ�
	if (WidgetData->bBindDocAutoOnOpen)
	{
		IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(Widget);
		if (CastDocBindable)
			CastDocBindable->BindDocAuto();
	}

	//���� �� ��, ���� ���� �Կ��� �ȴٸ�
	if (WidgetData->bAllowWorldBackgroundBlur)
	{
		Widget->bWorldBackgroundBlurOn = true;
		StartWorldBackgroundBlur(WidgetData->WorldBackgroundBlurScale);
	}

	//�⺻�����δ� Visibility�� ���ش�.
	Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//���� �� ��, ���� ���� UI�� ���ܾ� �Ѵٸ�( bHideBattleRelatedWidgets�� �� �������� ������ �ǰ�, bHUDHidingCinematicMode�� �ܺο��� ������ �ȴ� )
	if (
		(bCinematicMode && bHUDHidingCinematicMode) ||
		(!bCinematicMode && WidgetData->bHideBattleRelatedWidgets) // ����� CinematicMode ������ �켱��. CinematicMode ��� bHideBattleRelatedWidgets ������ ����.
		)
	{
		HideBattleRelatedWidgets(true);
	}

	if (Cast<IB2UIBackWidget>(Widget))
	{
		B2AndroidBackManager::GetInstance()->AddBackWidget(Widget, InUIName);
	}

	Widget->OnOpen(bRightNow);
}

void UB2UIManager::InternalCloseUI(FName InUIName, class UB2UIWidget* Widget, bool bRightNow, bool bCloseStaticUIs)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_InternalCloseUI);
	auto WidgetData = GetWidgetData(InUIName);
	if (!WidgetData || WidgetData->Widget != Widget)
		return;

	//OpenState�� None�̸� �ƿ� �������� �ʾ����� ���� �ʿ䵵 ����.
	if (Widget->GetOpenState() == EUIOpenState::None)return;



	//UI�� ������ �Ϸ�Ǿ�����
	if (Widget->GetOpenState() == EUIOpenState::Closed)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
		Widget->OpenState = EUIOpenState::None;

		// bStayInViewport �� �͵��� bCloseStaticUIs �� ���� ��쿡�� �ݴ´�. (���� ��ε� ����)
		if (!WidgetData->bStayInViewport || bCloseStaticUIs)
		{
			Widget->DestroySelf(this);
			WidgetData->Widget = nullptr; // ��� DestroySelf ���� ó���� ��.
		}

		//���� �� �� �ڵ����ε� �ߴٸ�, �ڵ� ����
		if (WidgetData->bBindDocAutoOnOpen)
		{
			IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(Widget);
			if (CastDocBindable)
				CastDocBindable->UnbindDoc();
		}

		//���� �� ��, ���� ���� �Կ��ٸ� ����
		if (WidgetData->bAllowWorldBackgroundBlur && Widget->bWorldBackgroundBlurOn)
		{
			Widget->bWorldBackgroundBlurOn = false;
			StopWorldBackgroundBlur();
		}

		//���� �� ��, ���� ���� UI�� ���� ���� ���� �ؾ� �Ѵٸ�.
		// �� ������ bHideBattleRelatedWidgets ������ ���� ������ �������� �� ������ �� ���̴�. �׷� ��쿡 ���� ��ó�� �ʿ��ϴٸ� ���¸� ���� ������ �� �ʿ䰡 �ִ�.
		// �׷��� CinematicMode ������ ���� ����� �ʿ䰡 ���� ��.
		if (
			(bCinematicMode && !bHUDHidingCinematicMode) ||
			(!bCinematicMode && WidgetData->bHideBattleRelatedWidgets)
			)
		{
			HideBattleRelatedWidgets(false);
		}

		if (Cast<IB2UIBackWidget>(Widget))
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(Widget, InUIName);
		}
	}
	else
	{
		Widget->OnClose(bRightNow);
	}
}

void UB2UIManager::UpdateUIDataCacheHistoryOnOpen(FB2UIWidgetData* InJustOpenedUIData)
{ // UIDataCacheHistory ��� ����
	if (UIDataCacheHistoryNum > 0 && InJustOpenedUIData
		// ��Ʈ�� ó���� �ϴ� �� ���⿡ ���Խ�Ű�� �ʴ´�... bCacheOnLoad ��ũ�� �͵� ������ ������..
		&& !IsRootSetWidgetName(InJustOpenedUIData->WidgetFName)
		&& !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS ���� �̰� �Ϸ��� IsRootSetWidgetName �� �����ϴ���, �ƴ� �� ��ɵ� ���� ����ϴ���..
		)
	{
		const FName JustOpenedUIName = InJustOpenedUIData->WidgetFName;
		FName PoppedUIName = NAME_None; // �̹� �� history �� �߰��Ǵ� ������ ���� ������ �� �ִٸ� �̸��� ���� �� ��.

		int32 ExistingIndex = UIDataCacheHistory.Find(JustOpenedUIName);
		if (ExistingIndex == INDEX_NONE)
		{
			// ������ ����Ʈ�� ���ٸ� ���� �߰��� �� ���̴� �� ���� history �� ������ ���¸� �� �տ� �� ���� ����..
			if (UIDataCacheHistory.Num() >= UIDataCacheHistoryNum)
			{
				PoppedUIName = UIDataCacheHistory[0];
				UIDataCacheHistory.RemoveAt(0);
			}
		}
		else
		{
			// ������ ����Ʈ�� �ִٸ� �ϴ� ����.
			UIDataCacheHistory.RemoveAt(ExistingIndex);
		}

		// ����Ʈ�� ���� �ڿ� �߰�.
		UIDataCacheHistory.Add(JustOpenedUIName);

		// bCacheByHistory üũ.
		InJustOpenedUIData->bCacheByHistory = true;

		if (PoppedUIName != NAME_None)
		{
			// �̹��� ���� �Ϳ� ���� unload ó��. 
			// ResetAllCacheMarkForUnload �� �ϴ� �� �ƴ϶� bCacheByHistory �� �����Ѵ�.
			// bCacheByHistory �� ������ bCacheOnLoad �� ���õǾ� �ִٸ� ������ Unload �� ���� ����.
			FB2UIWidgetData* PoppedUIData = GetWidgetData(PoppedUIName);
			if (PoppedUIData)
			{
				PoppedUIData->bCacheByHistory = false;
				if (PoppedUIData->CanUnloadByPolicy())
				{
					Unload(PoppedUIData);
				}
			}
		}
	}
}

void UB2UIManager::StartWorldBackgroundBlur(float WorldBackgroundBlurScale)
{
	BeginGlobalSceneBlurClass<float>::GetInstance().Signal(WorldBackgroundBlurScale);
}

void UB2UIManager::StopWorldBackgroundBlur()
{
	EndGlobalSceneBlurClass<>::GetInstance().Signal();
}

void UB2UIManager::HideBattleRelatedWidgets(bool bHide)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_HideBattleRelatedWidgets);
#if !UE_BUILD_SHIPPING
	extern bool gbHACKInGameUIHiddenByDevCommand;
#endif
	if (bHide
#if !UE_BUILD_SHIPPING
		|| gbHACKInGameUIHiddenByDevCommand // ���߿� Ŀ�ǵ�� ���� ���.		
#endif
		)
	{
		// �̰� ��Ȳ�� ���� �Ϻ��� ���������� �ʴµ� gbHACKInGameUIHiddenByDevCommand �� ����. 
		// �Ƹ��� WorldBackgroundBlur �� ���� ��Ȳ���� �װͱ��� ������ �ʿ䰡 ���� ������ ��.
		for (TObjectIterator<UB2UIManager_InGameHUDChar> WMIT; WMIT; ++WMIT)
		{
			(*WMIT)->HideAll();
		}

		for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
		{
			FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
			if (WidgetData.IsBattleRelatedWidget)
			{
				if (WidgetData.Widget)
					WidgetData.Widget->ForceHide();
			}
		}
	}
	else
	{
		for (TObjectIterator<UB2UIManager_InGameHUDChar> WMIT; WMIT; ++WMIT)
		{
			(*WMIT)->RestoreVisibilityAll();
		}

		for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
		{
			FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
			if (WidgetData.IsBattleRelatedWidget)
			{
				if (WidgetData.Widget)
					WidgetData.Widget->RestoreFromForceHidden();
			}
		}
	}
}

void UB2UIManager::SetHUDHidingCinematicMode(bool bInCinematicMode, bool bIsHUDHiding)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_SetHUDHidingCinematicMode);
	bCinematicMode = bInCinematicMode;
	bHUDHidingCinematicMode = bIsHUDHiding;

	// �� �������δ� ���⼭ CinematicMode ���� �� ���� �����ִ� Widget �� WidgetData �� bHideBattleRelatedWidgets �� �����Ǿ� �־ �ٽ� ���̰� �� ��.
	HideBattleRelatedWidgets(bCinematicMode && bHUDHidingCinematicMode);
}


FString UB2UIManager::FormatSecondsAsTime(int32 Secs, FString Format)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_FormatSecondsAsTime);
	int32 Hr = FPlatformMath::FloorToInt(Secs / 3600.0f);
	int32 Min = FPlatformMath::FloorToInt((Secs - (Hr * 3600)) / 60.0f);
	int32 Sec = FPlatformMath::FloorToInt(Secs - (Hr * 3600) - (Min * 60.0f));

	FString H_Char;
	FString M_Char;
	FString S_Char;

	H_Char = (Hr < 10) ? FString::Printf(TEXT("0%d"), Hr) : *FString::FromInt(Hr);
	M_Char = (Min < 10) ? FString::Printf(TEXT("0%d"), Min) : *FString::FromInt(Min);
	S_Char = (Sec < 10) ? FString::Printf(TEXT("0%d"), Sec) : *FString::FromInt(Sec);

	if (Format.Len() > 0)
	{
		Format = Format.Replace(TEXT("hh"), *H_Char);
		Format = Format.Replace(TEXT("h"), *FString::FromInt(Hr));
		Format = Format.Replace(TEXT("mm"), *M_Char);
		Format = Format.Replace(TEXT("m"), *FString::FromInt(Min));
		Format = Format.Replace(TEXT("ss"), *S_Char);
		Format = Format.Replace(TEXT("s"), *FString::FromInt(Sec));
		return Format;
	}
	else
	{
		return TEXT("00:00:00");
	}
}

FString UB2UIManager::FormatMilliSecondsAsTime(int32 MilliSecs, FString Format)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_FormatMilliSecondsAsTime);
	int32 MilliSec = MilliSecs % 1000;

	FString MS_Char = MilliSec < 10 ? FString::Printf(TEXT("00%d"), MilliSec) :
		MilliSec < 100 ? FString::Printf(TEXT("0%d"), MilliSec) :
		*FString::FromInt(MilliSec);

	if (Format.Len() > 0)
		Format = Format.Replace(TEXT("ms"), *MS_Char);

	return FormatSecondsAsTime(MilliSecs / 1000, Format);
}

int64 UB2UIManager::RemainTimeFunction(int64 InEndTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_RemainTimeFunction);
	FDateTime Time = FDateTime::UtcNow();

	return InEndTime - (Time.ToUnixTimestamp() * 1000);
}

UTexture2D* UB2UIManager::LoadTexture2D(const TCHAR* Name)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_LoadTexture2D);
	UTexture2D* Tex = LoadObject<UTexture2D>(NULL, Name, NULL, LOAD_None, NULL);

	return Tex;
}

void UB2UIManager::SetMainWidgetServerVersionText(FText ftServerInfoText)
{
	if (MainWidget)
		MainWidget->SetServerVersionInfoText(ftServerInfoText);
}

void UB2UIManager::ShowMainWidgetVersion()
{
	if (MainWidget)
		MainWidget->ShowVersionInfo();
}

void UB2UIManager::BlockUIClickEvent(bool bBlock)
{
	static bool bBlockClickEvent = false;
	static TSharedPtr<IInputProcessor> BlockInput = MakeShareable(new B2BlockClickEvent());

	if (bBlockClickEvent != bBlock)
	{
		bBlockClickEvent = bBlock;
		if (bBlockClickEvent)
			FSlateApplication::Get().RegisterInputPreProcessor(BlockInput);
		else
			FSlateApplication::Get().UnregisterInputPreProcessor(BlockInput);
	}
}

bool UB2UIManager::ErrorCodeContains(int32 InErrorCode) const
{
	return ErrorTimeStamp.ContainsByPredicate([&InErrorCode](const FB2ErrorCode& InB2ErrorCode) {
		return InB2ErrorCode.ErrorCodeNum == InErrorCode;
	});
}

bool UB2UIManager::CheckErrorCodeContains(int32 InErrorCode) const
{
	return NoCheckErrorCode.ContainsByPredicate([&InErrorCode](const int32& InB2ErrorCode) {
		return InB2ErrorCode == InErrorCode;
	});
}

void UB2UIManager::SetErrorTimeStamp(int32 InErrorCode)
{
	ErrorTimeStamp.Add(FB2ErrorCode(InErrorCode, UB2GameInstance::GetUTCNow().GetTicks()));
}

// �ѹ� ����� �����ڵ�� 5�ʵ��� ����� �� ����
// NoCheckErrorCode�� ���� ��� �� �� ����
void UB2UIManager::CheckErrorTimeStamp(float DeltaTime)
{
	static float CheckTime = 0.0f;
	CheckTime += DeltaTime;

	// 0.5�ʿ� �ѹ� üũ
	if (CheckTime > 0.5f)
	{
		const int64 ErrorCoolTime = ETimespan::TicksPerSecond * ErrorResetPerSecond; // 5�ʸ��� ����
		const int64 CurrentUTCTime = UB2GameInstance::GetUTCNow().GetTicks();
		CheckTime = 0.0f;

		for (int32 i = 0; i < ErrorTimeStamp.Num(); ++i)
		{
			// �˾��ð� + ��Ÿ�� < ����ð�
			if (ErrorTimeStamp[i].ErrorTimeStamp + ErrorCoolTime < CurrentUTCTime)
			{
				ErrorTimeStamp.RemoveAt(i--);
			}
		}
	}
}

void UB2UIManager::OpenMsgPopupFromErrorCode(int32 nErrorCode)
{
	//////////////////////////////////////////////////////////////////////////////
	//[@AKI, 180111] ������ ���� ������ �Ʒ� �ּ� �����Ͽ� ���� ���� ��
	//http://172.168.2.106/result_codes.html
	//////////////////////////////////////////////////////////////////////////////
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenMsgPopupFromErrorCode);

	if (CheckErrorCodeContains(nErrorCode)) // üũ���� �ʴ� Errorcode�� �ٷ� ����(���� ��)
	{
		if (ErrorCodeContains(nErrorCode)) // ��� ������ return
			return;

		SetErrorTimeStamp(nErrorCode); // ���
	}

	FMsgPopupOnClick ErrorMsgPopuoOnClick = nullptr;

	switch (nErrorCode)
	{
	case B2ResultCode::FAILURE_BLADEPOINT_UNDERFLOW:	//FAILURE_BLADEPOINT_UNDERFLOW		BladePoint is zero.
		HandleServerErrorGoodsShortageClass<const uint32, const EGoodsButtonType>::GetInstance().Signal(FItemRefIDHelper::GetGoodsID_BladePoint(), EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
		return;
	case B2ResultCode::FAILURE_NOT_ENOUGH_STAGE_BOOST_TICKET: // Not Enough stage boost ticket
		HandleServerError4018Class<>::GetInstance().Signal();
		break;
	case B2ResultCode::FAILURE_SOCIALPOINT_UNDERFLOW:	//FAILURE_SOCIALPOINT_UNDERFLOW		SocialPoint is zero.//�������� ��������Ʈ �����Ҷ� ���� ���
	{
		UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoFriendForFriendShip();
		return;
	}
	case B2ResultCode::FAILURE_CASH_UNDERFLOW:	//FAILURE_CASH_UNDERFLOW							Cash is zero.
		HandleServerError910Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_MONEY_UNDERFLOW:	//FAILURE_MONEY_UNDERFLOW						Money is zero.
		HandleServerError911Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_DARK_CRYSTAL_UNDERFLOW:	//FAILURE_DARK_CRYSTAL_UNDERFLOW				Not enough dark crystal
		HandleServerError919Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_FAME_POINT_UNDERFLOW:
		HandleServerError932Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_DUPLICATED_NICKNAME:	//FAILURE_DUPLICATED_NICKNAME					Account Nickname already exists.
		HandleServerError101Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_INTERNAL_ERROR:	//FAILURE_INTERNAL_ERROR						Server internal error
	case B2ResultCode::FAILURE_INTERNAL_DATABASE_ERROR:	//FAILURE_INTERNAL_DATABASE_ERROR				Server internal database error.
	case B2ResultCode::FAILURE_MASTER_DATA_ERROR:	//FAILURE_MASTER_DATA_ERROR						Master data not found
	case B2ResultCode::FAILURE_NO_ACCOUNT_NICKNAME:	//FAILURE_NO_ACCOUNT_NICKNAME					Account nickname is empty
		GoToTitlePopup(nErrorCode);
		return;
	case B2ResultCode::FAILURE_NOT_READY_TO_REWARD_QUEST:
		CloseUI(UIFName::QuestNotice);
		CloseUI(UIFName::QuestDialog);
		GoToLobbyPopup(nErrorCode);
		return;
	case B2ResultCode::FAILURE_NICKNAME_LENGTH_NOT_AVAILABLE:	//FAILURE_NICKNAME_LENGTH_NOT_AVAILABLE			Nickname length is not available
		HandleServerError113Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_NOT_EXIST_MD_MARKET_PRODUCT:
		HandleServerInAppPurchaseErrorClass<int32>::GetInstance().Signal(B2ResultCode::FAILURE_NOT_EXIST_MD_MARKET_PRODUCT);
		return;
	case B2ResultCode::FAILURE_HERO_TOWER_HERO_PIECE_UNDERFLOW:	//FAILURE_HERO_TOWER_HERO_PIECE_UNDERFLOW		Hero piece count is underflow.
		HandleServerError4207Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_NOT_ENOUGH_STAGE_SWEEP_TICKET:
		HandleServerError4032Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_OVERFLOW_TARGET_FRIEND_COUNT:
		HandleServerError5513Class<>::GetInstance().Signal();
		return;

	case B2ResultCode::FAILURE_INVALID_COUNT_TO_USE_ADDITIONAL_MATCH:	//FAILURE_INVALID_COUNT_TO_USE_ADDITIONAL_MATCH	Invalid count to use additional match
		HandleServerError6516Class<>::GetInstance().Signal();
		return;

	case B2ResultCode::ERROR_MATCHMAKING_CANCELED:	//ERROR_MATCHMAKING_CANCLED						Matchmaking cancled
		DeliveryCancelMatchmakingClass<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_MATCHING_RAID_PENALTY_TIME:	//FAILURE_MATCHING_RAID_PENALTY_TIME			Fail matching raid penalty time
		HandleServerError7413Class<>::GetInstance().Signal();
		return;
	case B2ResultCode::FAILURE_KAKAO_MESSAGE_EMPTY_MESSAGEBOX:	//FAILURE_KAKAO_MESSAGE_EMPTY_MESSAGEBOX		kakako message box is empty
	case B2ResultCode::FAILURE_KAKAO_MESSAGE_ALREADY_PROCESSING:	//FAILURE_KAKAO_MESSAGE_ALREADY_PROCESSING		kakako message is already processing state
		return;

	case B2ResultCode::FAILURE_EXPIRED_MAIL:	//FAILURE_EXPIRED_MAIL							Expired Mail 
	case B2ResultCode::FAILURE_NOT_EXIST_TO_OPEN_MAIL:	//FAILURE_NOT_EXIST_TO_OPEN_MAIL				fail not exist to open mail 
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() { RequestCurrentTabMailListClass<>::GetInstance().Signal(); });
		break;

	case B2ResultCode::FAILURE_NOT_PASSED_ATTENDANCE_TIME:	//FAILURE_NOT_PASSED_ATTENDANCE_TIME			not passed attendance time
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() { UpdateEventAttendancePartsClass<>::GetInstance().Signal(); });
		break;

	case B2ResultCode::FAILURE_NOT_FREE_LOTTERY_STATE:		//FAILURE_NOT_FREE_LOTTERY_STATE			fail not free lottery state
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() { HandleServerError7237Class<>::GetInstance().Signal(); });
		break;

	case B2ResultCode::ERROR_CHANNEL_SERVER_UNAVAILABLE:	//ERROR_CHANNEL_SERVER_UNAVAILABLE				channel server unavailable
	case 8404:	//B2ResultCode::ERROR_NOT_FOUND								not found
	case B2ResultCode::ERROR_BAD_REQUEST:	//ERROR_BAD_REQUEST								bad request
	case B2ResultCode::FAILURE_TRY_AGAIN_ERROR:	//FAILURE_TRY_AGAIN_ERROR						Try again later
	case B2ResultCode::FAILURE_GUILD_BATTLE_TIME_LIMIT_OVER:	//FAILURE_GUILD_BATTLE_TIME_LIMIT_OVER	GuildBattle TimeOver
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() {GoToVillageClass<>::GetInstance().Signal(); });
		break;

	case B2ResultCode::ERROR_CHANNEL_SERVER_ROOM_CLOSED:	// Room has already begin game
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() {HandleServerError8005Class<>::GetInstance().Signal(); });
		break;

	case B2ResultCode::FAILURE_CONTENTS_SYSTEM_CHECK_STATE:
	case B2ResultCode::FAILURE_CONTENTS_BLOCK_ENTER_STATE:
		SettlePopup(nErrorCode);
		return;

	case B2ResultCode::FAILURE_CONTENTS_MODE_FINISH_STATE:	//FAILURE_CONTENTS_MODE_FINISH_STATE			contents is mode finish state
		if (GetCurrUIScene() == EUIScene::HallOfFame)
		{
			SettlePopup(nErrorCode);
		}
		else
		{
			ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() {GoToVillageClass<>::GetInstance().Signal(); });
		}
		break;

	case B2ResultCode::ERROR_SERVICE_UNAVAILABLE:	//ERROR_SERVICE_UNAVAILABLE						service unavailable
	case B2ResultCode::ERROR_API_SERVER_UNAVAILABLE:	//ERROR_API_SERVER_UNAVAILABLE					api server unavailable
	case B2ResultCode::ERROR_SESSION_SERVER_UNAVAILABLE:	//ERROR_SESSION_SERVER_UNAVAILABLE				session server unavailable
	case B2ResultCode::ERROR_VERSION_MISMATCH:	//ERROR_VERSION_MISMATCH						version mismatch
	case B2ResultCode::ERROR_PROTOCOL_MISMATCH:	//ERROR_PROTOCOL_MISMATCH						protocol mismatch
	case B2ResultCode::ERROR_PROTOCOL_CORRUPTED:	//ERROR_PROTOCOL_CORRUPTED						protocol corrupted
	case B2ResultCode::ERROR_UNAUTHENTICATED:	//ERROR_UNAUTHENTICATED							unauthenticated
	case B2ResultCode::ERROR_INVALID_REQUEST:	//ERROR_INVALID_REQUEST							invalid request
	case B2ResultCode::ERROR_ALREADY_AUTHENTICATED:	//ERROR_ALREADY_AUTHENTICATED					already authenticated
	case B2ResultCode::ERROR_UNAUTHORIZED:	//ERROR_UNAUTHORIZED							unauthorized
	case B2ResultCode::ERROR_INSECURE_SESSION:	//ERROR_INSECURE_SESSION						insecure session
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() {GoToTitleClass<>::GetInstance().Signal(); });
		break;

	case B2ResultCode::FAILURE_NOT_START_STATE_MATCH_INFO:
		if (GetCurrUIScene() == EUIScene::PVPOneOnOneFindMatch)
		{
			ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() {UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPOneOnOne); });
		}
		break;

	case B2ResultCode::FAILURE_THIS_DONATION_EVENT_ALREADY_ENDED:
	case B2ResultCode::FAILURE_NOT_EXIST_DONATION_STATUS:
	case B2ResultCode::FAILURE_INVALID_DONATION_ID:
	case B2ResultCode::FAILURE_ALREADY_REWARDED_DONATION_POINT_REWARD:
	case B2ResultCode::FAILURE_UPDATE_DONATION_POINT_REWARD_STATE:
	case B2ResultCode::FAILURE_DUPLICATE_DONATION_EVENT_IN_PROGRESS:
	case B2ResultCode::FAILURE_NOT_ENOUGH_POINT_RECEIVE_REWARD:
	case B2ResultCode::FAILURE_YET_REMAIN_FREE_DONATION_COUNT:
		UpdateDonationEventClass<>::GetInstance().Signal();
		break;
	}

	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr && UIMgr->GetCurrUIScene() == EUIScene::SummonItemResult)
	{
		ErrorMsgPopuoOnClick = FMsgPopupOnClick::CreateLambda([]() { LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemStore); });
	}

	EUIMsgPopup PopupType = EUIMsgPopup::Simple;

	UB2ErrorCodeInfo* pErrorCodeInfo = StaticFindErrorCodeInfo();
	if (pErrorCodeInfo)
	{
		FSingleErrorCodeInfoData* pErrorCodeData = pErrorCodeInfo->GetInfoData(nErrorCode);

		if (pErrorCodeData)
		{
			PopupType = pErrorCodeData->PopupType;
		}
	}

	FText txtComment = BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(nErrorCode));

	if (PopupType == EUIMsgPopup::Center)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(PopupType,
			txtComment,
			FText::FromString(TEXT("")),
			1.5f,
			true,
			true,
			EUIMsgPopupButtonGroup::Yes,
			ErrorMsgPopuoOnClick
		);
	}
	else
	{
		// EUIMsgPopup::Simple �̿��Ҷ� �����ڵ��ȣ�� ������

		FSingleErrorCodeInfoData* pErrorCodeData = pErrorCodeInfo->GetInfoData(nErrorCode);

		if (!pErrorCodeData || pErrorCodeData->ViewErrorNumber)
		{
			FString strErrorCode = FString::Printf(TEXT("\n(%d)"), nErrorCode);
			txtComment = FText::Format(FText::FromString(TEXT("{0}{1}")), txtComment, FText::FromString(strErrorCode));
		}

		UB2UIMsgPopupSimple* pPopupUI = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			txtComment,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm,
			ErrorMsgPopuoOnClick
			);

		if (pPopupUI && pErrorCodeData && pErrorCodeData->SpecialButtonTextKey.Len() > 0)
		{
			pPopupUI->SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), pErrorCodeData->SpecialButtonTextKey));
		}
	}
}

void UB2UIManager::CloseMsgPopup(int32 MsgPopupId, bool bRightNow)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseMsgPopup);
	auto MsgPopup = MsgPopupMap.FindRef(MsgPopupId);
	if (MsgPopup)
	{
		MsgPopupMap.Remove(MsgPopupId);

		//OpenUI�� ��������, CloseUI�� �ݾƾ���... ���߿� �˾��� n�� ���ļ� ���� �� �� ������ OpenUI�� �ƴ� �ٸ� ��Ʈ�� ���� �۾��� �ʿ��� ��
		CloseUI(MsgPopup->MyUIFName, bRightNow);
		//MsgPopup->DestroySelf();
	}
}

void UB2UIManager::CloseMsgPopup(UB2UIMsgPopupBase* MsgPopup, bool bRightNow)
{
	if (MsgPopup)
		CloseMsgPopup(MsgPopup->GetMsgID(), bRightNow);
}

bool UB2UIManager::IsPopupPriority(EPopUpPriority InIsPriority) const
{
	for (auto MsgPopup : MsgPopupMap)
	{
		if (MsgPopup.Value)
		{
			// ��Ʈ�� ���� ���� �˾��� ������ ����
			if (MsgPopup.Value->GetPriority() == InIsPriority)
			{
				return true;
			}
		}
	}
	return false;
}

void UB2UIManager::CloseAllMsgPopups(bool bRightNow)
{ // CloseMsgPopup �� �־��� ID �� MsgPopup ���۷��� ����� �Ұ��� ��� ��¿ �� ���� ���. Ȥ�� ��Ÿ �ٸ� Ư���� ���.
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllMsgPopups);

	for (auto MsgPopup : MsgPopupMap)
	{
		if (MsgPopup.Value)
		{
			// ��Ʈ�� ���� ���� �˾��� ������ ����
			if (MsgPopup.Value->GetPriority() < EPopUpPriority::Server_Message_GoToLobby)
			{
				CloseUI(MsgPopup.Value->MyUIFName, bRightNow);
				MsgPopupMap.Remove(MsgPopup.Key);
			}
		}
	}
}

bool UB2UIManager::IsMsgPopupOpened(int32 MsgPopupId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_IsMsgPopupOpened);
	auto MsgPopup = MsgPopupMap.Find(MsgPopupId);
	return MsgPopup != nullptr;
}
void UB2UIManager::OpenAppUpdatePopup()
{
	OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("UpdateAppVersion")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([this]()
			{
				FBladeIIBlockToSyncNetwork::GetInstance().CloseBlockToSyncNetworkUI();
				GoToDLCMapClass<>::GetInstance().Signal();
			})
		, 0
		, false
		, EPopUpPriority::Server_Message_DLCMap
	);

}

void UB2UIManager::GoToTitlePopup(int32 nErrorCode)
{
	UB2ErrorCodeInfo* pErrorCodeInfo = StaticFindErrorCodeInfo();
	FSingleErrorCodeInfoData* pErrorCodeData = pErrorCodeInfo->GetInfoData(nErrorCode);

	FText txtComment = BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(nErrorCode));

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		txtComment,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([]()
			{
				UB2UIManager::GetInstance()->CloseUI(UIFName::BattlePause);
				GoToTitleClass<>::GetInstance().Signal();
			})
	);
}

void UB2UIManager::GoToLobbyPopup(int32 nErrorCode)
{
	OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		FText::FromString(TEXT("")),
		BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(nErrorCode)),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([this]()
	{
		GoToVillageClass<>::GetInstance().Signal();
	}),
		0,
		true,
		EPopUpPriority::Server_Message_Default
		);
}

void UB2UIManager::SettlePopup(int32 nErrorCode)
{
	if (GetCurrUIScene() == EUIScene::HallOfFame)
	{
		if (auto* UIHallOfFame = GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
		{
			UIHallOfFame->SetSettleState();
		}
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ContentsCheck")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::GotoLobby,
			FMsgPopupOnClick::CreateLambda([]() {GoToVillageClass<>::GetInstance().Signal(); }),
			0,
			true,
			EPopUpPriority::Server_Message_GoToLobby
		);
	}
}

void UB2UIManager::ResetRepeatBattle()
{
	AB2StageManager::GetCacheStageKeepEssentialData().SetRepeatBattleStateBoostOn(false);
	if (auto* DocBS = UB2UIDocHelper::GetDocBattleStage())
	{
		DocBS->SetRepeatBattleCurrentOn(false);
		DocBS->SetRepeatBattleCurrentCount(0);
		DocBS->SetRepeatBattleLoopAll(false);
		DocBS->SetRepeatBattleLoopOne(false);
		DocBS->SetRepeatBattleBoostOn(false);
	}
}

FString UB2UIManager::GetPlatformAppStoreURL()
{
	FString URLKey = FString(TEXT("Review_AOS"));

#if PLATFORM_ANDROID
	//if(Google Store)
	
#ifdef OnestoreBuild
	URLKey = FString(TEXT("Review_One"));
#else
	URLKey = FString(TEXT("Review_AOS"));
#endif
	
	//else if(Kakao Store)
	//else if(One Store)
	//else
	//URLKey = FString(TEXT("Review_AOS"));
#elif PLATFORM_IOS
	URLKey = FString(TEXT("Review_IOS"));
#endif	

	return BladeIIGameImpl::GetClientDataStore().GetWebURL(URLKey);
}

bool UB2UIManager::IsOpendAppStoreReview()
{
	return B2P_IsInReview() ||
		IsOpenPopupAppStoreReview() ||
		(BladeIIGameImpl::GetClientDataStore().GetShowReviewInducementPopup() == false);
}

bool UB2UIManager::OpenPopupAppStoreReview()
{
	bool IsOpen = false;

	if (IsOpendAppStoreReview())
	{
		return IsOpen;
	}
	else
	{
		IsOpen = true;
		SetTrueOpenPopupAppStoreReview();
	}

	static FString AppStore_Url = GetPlatformAppStoreURL();

	UB2UIMsgPopupSimple* pPopupUI = OpenMsgPopup<UB2UIMsgPopupSimple>(
		EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PleaseReviewMyGameTitle")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PleaseReviewMyGameInfo")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([]() { B2P_ShowOpenURL(AppStore_Url); })
	);

	if (pPopupUI)
	{
		pPopupUI->SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PleaseReviewMyGameBtn")));
	}

	return IsOpen;
}

void UB2UIManager::OpenToolTipTextBox(FText& ContextText, UB2UIWidgetBase* TargetUB2UIWidget, UWidget* Target, float ClockDirection /*= 6.0f*/, float InLifeTime /*= 5.0f*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenToolTipTextBox);
	UB2UIToolTipTextBox* pUI = GetUI<UB2UIToolTipTextBox>(UIFName::ToolTipTextBox);

	if (pUI != NULL)
		CloseUI(UIFName::ToolTipTextBox);
	else
	{
		pUI = OpenUI<UB2UIToolTipTextBox>(UIFName::ToolTipTextBox);
		if (pUI)
		{
			pUI->Init();
			pUI->SetContentText(ContextText);
			pUI->SetTargetInfo(TargetUB2UIWidget, Target);
		}
	}
}

UB2UIMailRewardPopUp* UB2UIManager::OpenRewardMailPopUp(const b2network::B2RewardPtr RewardItem, bool CheckMailReward)
{
	//if (RewardItem == nullptr 
	//	|| (CheckMailReward && b2network::B2RewardPushType::MAIL != BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardItem->id))) //  ������ or ������ üũ
	//	return nullptr;

	//if (RewardItem == nullptr
	//	|| (CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardItem->id))
	//	)//  ������ or ������ üũ
	//	return nullptr;

	//return UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);

	return RewardItem ? OpenRewardMailPopUp(RewardItem->raw_reward->id, CheckMailReward) : nullptr;
}

// ������ ���Ϸ� ������ true, ���������� ������ false
//  BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardItem[0]->id)) �� �κ��� ���� ��ȹ���� ���� �������� ������, ��� ������ or ��� ���Ϸ� ��ũ��Ʈ �ۼ��� �ϱ������. 2017/07/12
//  �׷��� 0��° �ε����� �˻�
//  �Ŀ� �����ȴٸ� �� ���鼭 üũ
UB2UIMailRewardPopUp* UB2UIManager::OpenRewardMailPopUp(const TArray<b2network::B2RewardPtr>& RewardItem, bool CheckMailReward /*= true*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenRewardMailPopUp);
	if (RewardItem.Num() < 1)
		return nullptr;

	if (CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardItem[0]->raw_reward->id))
		return nullptr;

	UB2UIManager::GetInstance()->OpenUI(UIFName::RewardMailPopup);

	return UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);;
}

bool UB2UIManager::CheckRewardPushType(int32 PushType, int32 RewardId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CheckRewardPushType);
	return BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardId) == PushType;
	//return false;
}

UB2UIMailRewardPopUp * UB2UIManager::OpenRewardMailPopUp(const int32 RewardId, bool CheckMailReward)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenRewardMailPopUp);
	if ((CheckMailReward && b2network::B2RewardPushType::MAIL != BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardId))) //  ������ or ������ üũ
		return nullptr;

	if ((CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardId))
		)//  ������ or ������ üũ
		return nullptr;

	return UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);
}

UB2UnitedWidgetBase* UB2UIManager::GetRedDotWidget(UUserWidget* OwnerUserWidget, UWidget* ParentWidget /*= nullptr*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetRedDotWidget);
	if (OwnerUserWidget && OwnerUserWidget->WidgetTree)
	{
		UWidgetTree* WidgetTree = OwnerUserWidget->WidgetTree;

		TArray<UWidget*> AllWidgets;
		WidgetTree->GetAllWidgets(AllWidgets);

		for (UWidget* ChildWidget : AllWidgets)
		{
			if (ChildWidget && ChildWidget->IsA(RedDotWidgetClass))
			{
				UB2UnitedWidgetBase* AlarmWidget = Cast<UB2UnitedWidgetBase>(ChildWidget);
				if (ParentWidget == nullptr) // �θ� ������ ù��° AlarmWidget�� �ٷ� ����
					return AlarmWidget;

				else if (ParentWidget && ChildWidget->IsChildOf(ParentWidget))
					return AlarmWidget;
			}
		}
	}

	return nullptr;
}

FName UB2UIManager::GetUIFNameFromAssetName(const FName& AssetName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetUIFNameFromAssetName);
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
		if (WidgetData.WidgetAssetName == AssetName)
			return WidgetData.WidgetFName;
	}

	return FName(TEXT("NoAsset"));
}

FName UB2UIManager::GetWidgetAssetName(const FName& WidgetInstName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetWidgetAssetName);
	FString WidgetString = WidgetInstName.ToString();
	const int32 SubfixIndex = WidgetString.Find(TEXT("_C"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	if (SubfixIndex != INDEX_NONE)
	{
		return FName(*WidgetString.Left(SubfixIndex));
	}

	return FName(TEXT("None"));
}

B2UIShortagePopupManager* UB2UIManager::GetShortagePopupMGR()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetShortagePopupMGR);
	if (m_ShortagePopupMGR == nullptr)
		m_ShortagePopupMGR = new B2UIShortagePopupManager();

	return m_ShortagePopupMGR;
}

void UB2UIManager::ChangeUIScene(EUIScene SceneName, EUIChangeUISceneTransitionType TransType, const FB2UISceneOnOpen& UISceneOnOpenDelegate, bool DontCloseSameUIAtReservedUIScene, bool bPassBattleUIScene)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_ChangeUIScene);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UIManager::ChangeUIScene %d -> %d"), (int32)CurrUIScene, (int32)SceneName));

	CloseUI(UIFName::ToolTipTextBox);

	//�̹� ���� �ִ� UIScene���� ü������ �ȵȴ�.
	if (SceneName == CurrUIScene) return;

	B2AndroidBackManager::GetInstance()->RemoveAllPopup();

	//�̹� ChangeUIScene�� ���� ���̶�� ���ε�, �� Change�� �Ҹ��Ŵ�. �ϴ� ���Ļ��� ������ ���� �������� UIScene�� �� �ݴ°ɷ�.
	if (bClosingUIScene || bOpeningUIScene)
	{
		ReservedUISceneOpenData.Key = EUIScene::None;

		if (ClosingUIScene != EUIScene::None)
			CloseUIScene(ClosingUIScene, true, DontCloseSameUIAtReservedUIScene);
		if (OpeningUIScene != EUIScene::None)
			CloseUIScene(OpeningUIScene, true, DontCloseSameUIAtReservedUIScene);

		if (ClosingUIScene == EUIScene::None && OpeningUIScene == EUIScene::None && CurrUIScene != EUIScene::None) //RightNow���� ��Ȳ. ChangeUIScene ������ �̻��ϴ�..
			CloseUIScene(CurrUIScene, true, DontCloseSameUIAtReservedUIScene);

		CurrUIScene = EUIScene::None;
		bOpeningUIScene = false;
		bClosingUIScene = false;
		ClosingUIScene = EUIScene::None;
		OpeningUIScene = EUIScene::None;

		if (SceneName == EUIScene::None)
			return;
	}

	if (!IsDJLegacyUIScene(SceneName))
	{ // ���� �ý����� DJLagacy �������� Ȥ ������ �ʾ��� �� ����.
		DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_End);
	}

	//�ݱ⸸ �ϰ�, Open�� ���� �ʴ´�.
	if (SceneName == EUIScene::None)
	{
		CloseUIScene(CurrUIScene, TransType == EUIChangeUISceneTransitionType::CANCEL_CLOSE || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL, false);
		CurrUIScene = EUIScene::None;
		return;
	}

	//���� �ƹ� UIScene�� �ƴѰ��, Close���� Open�ٷ�
	if (CurrUIScene == EUIScene::None)
	{
		OpenUIScene(SceneName, UISceneOnOpenDelegate, TransType == EUIChangeUISceneTransitionType::CANCEL_OPEN || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL, false);
		return;
	}

	//���� UIScene�� Tick���� �ƴ� �ٷ� �ݾƾ� �Ǵ°�?
	bool CloseCurUISceneRightNow = (TransType == EUIChangeUISceneTransitionType::CANCEL_CLOSE || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL);

	//1. ��ȯ ���� Ÿ�� ����
	ChangeUISceneTransitionType = TransType;
	//2. ����UIScene ����
	ReservedUISceneOpenData.Key = SceneName;
	ReservedUISceneOpenData.Value = UISceneOnOpenDelegate;
	bDontCloseSameUIAtReservedUIScene = DontCloseSameUIAtReservedUIScene;
	//3. ����UIScene �ݱ�
	CloseUIScene(CurrUIScene, CloseCurUISceneRightNow, DontCloseSameUIAtReservedUIScene);

	if (CloseCurUISceneRightNow || TransType == EUIChangeUISceneTransitionType::PREOPEN)
		OpenNextUIScene();
	else
		CheckClosingCurrUISceneAndChangeToReservedUIScene();
}

void UB2UIManager::ChangeUISceneBack(EUIChangeUISceneTransitionType TransType, const FB2UISceneOnOpen& UISceneOnOpenDelegate, bool DontCloseSameUIAtReservedUIScene, bool bPassBattleUIScene)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_ChangeUISceneBack);

	OnChangUISceneBackClass<>::GetInstance().Signal();

	auto UIChain = GetUISceneData(CurrUIScene);
	if (UIChain)
	{
		for (FName& CurWidgetFName : UIChain->WidgetFNames)
		{
			FB2UIWidgetData* FoundWidgetData = AllWidgetData.Find(CurWidgetFName);

			if (FoundWidgetData && CurWidgetFName == FoundWidgetData->WidgetFName)
			{
				UB2UIWidget* ThisOpenWidget = GetUI<UB2UIWidget>(FoundWidgetData->WidgetFName);
				if (ThisOpenWidget) {
					if (ThisOpenWidget->OnBackButtonProc())
						return;
				}
			}
		}
	}

	EUIScene TargetBackUIScene = bPassBattleUIScene ? GetPrevUIScenePassedBattle() : GetPrevUIScene();

	if (IsDJLegacyUIScene(TargetBackUIScene))
	{
		bOpeningDJLegacyHeroMgmtBySceneHistory = true; // ��� LobbyEnterHeroMgmtModeClass �� ���ڷ� �־��ִ� �� ����� ����.. �ٲ�� �� ���� �� ����.

													   // �� UIScene ���� ���� �� �ý��ۿ��� ������ �ʴ´�. �����δ� DJLegacy �ý����̰� ���ʰ��� ������μ� ���ܵ� ��.
													   // ���� �� �ý��� ���̰� ����Ǵ� �� ��ư �۵��� ����.
		OpenDJLegacyHeroMgmtPageForUIScene(TargetBackUIScene);

		bOpeningDJLegacyHeroMgmtBySceneHistory = false;
	}
	else
	{
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(TargetBackUIScene);

		// ���ư�� ���� Mail UI �� ���ƿ� ��� LinkManager�� ���� ���ƿ� ���� �ƴϱ� ������ ����Ʈ ������Ʈ�� �̷������ ������ ���� ������Ʈ ���־�� �Ѵ�.
		if (TargetBackUIScene == EUIScene::Mail)
		{
			MailTabIndex RequestTab = MailTabIndex::EMailTab_Gift;
			data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(RequestTab));
		}
	}

	// DJLegacy �� ���� ���⼭ UISceneHistory ���� �ܿ� ���������� ������ ������ �ʾƾ�.
	ChangeUIScene(TargetBackUIScene, TransType, UISceneOnOpenDelegate, DontCloseSameUIAtReservedUIScene, bPassBattleUIScene);
}

EUIScene UB2UIManager::GetPrevUIScene()
{
	return (UISceneHistory.Num() > 1) ? UISceneHistory[UISceneHistory.Num() - 2].Key : EUIScene::None;
}

//UI�� �����丮�� �����Ǿ� �ִ� �����߿� ������ ����� ���� ������ �ǳ� �鶧 ���
EUIScene UB2UIManager::GetPrevUIScenePassedBattle()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetPrevUIScenePassedBattle);
	if (UISceneHistory.Num() > 1)
	{
		int HistoryBackStartIndex = UISceneHistory.Num() - 2;
		if (HistoryBackStartIndex >= 0)
		{
			for (int HistoryCnt = HistoryBackStartIndex; HistoryCnt >= 0; --HistoryCnt)
			{
				FB2UISceneData* CurSceneData = GetUISceneData(UISceneHistory[HistoryCnt].Key);
				if (CurSceneData && CurSceneData->IsBattleRelatedScene) // DJLegacy ���� ȣȯ�� ���� �־�� ���� �� �ĺ����� ��� ���� SceneData �� ���� �� �ִ�.
					continue;

				return UISceneHistory[HistoryCnt].Key;
			}
		}
	}

	return EUIScene::None;
}

void UB2UIManager::RemoveUISceneHistory(EUIScene SceneName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_RemoveUISceneHistory);
	int HistoryLength = UISceneHistory.Num();
	for (int HistoryCnt = HistoryLength - 1; HistoryCnt >= 0; HistoryCnt--)
	{
		if (UISceneHistory[HistoryCnt].Key == SceneName)
			UISceneHistory.RemoveAt(HistoryCnt);
	}
}
void UB2UIManager::ClearUISceneHistory()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_ClearUISceneHistory);
	UISceneHistory.Empty();
}
void UB2UIManager::ArtificialAddUIHistory(EUIScene AddSceneName)
{
	// Artifical Add ��� ���� ���� Open ���� ���� history �������� �ִ� �뵵�� ����.
	TPair<EUIScene, FB2UISceneOnOpen> HistoryDataPair;
	HistoryDataPair.Key = AddSceneName;
	HistoryDataPair.Value = nullptr;
	UISceneHistory.Add(HistoryDataPair);
}

void UB2UIManager::OpenUIScene(EUIScene SceneName, const FB2UISceneOnOpen& UISceneOnOpenDelegate, bool RightNow, bool DontOpenSameUIAtReservedUIScene)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenUIScene);
	bOpeningUIScene = true;
	CurrUIScene = SceneName;

	if (!RightNow)
		OpeningUIScene = SceneName;

	EUIScene PrevUIScene = (UISceneHistory.Num() > 0) ? UISceneHistory[UISceneHistory.Num() - 1].Key : EUIScene::None;
	auto PrevSceneData = GetUISceneData(PrevUIScene);
	auto CurSceneData = GetUISceneData(CurrUIScene);

	//���� UIScene�� ���� �����丮�� ���ٸ�, �����丮���� �˽�Ű�� �ƴϸ� �����丮�� �����Ѵ�.
	int HistoryLength = UISceneHistory.Num();
	bool IsBack = UISceneHistory.Num() > 1 && HasUISceneHistory(SceneName);

	if (IsBack)
	{
		for (int HistoryCnt = HistoryLength - 1; HistoryCnt >= 0; --HistoryCnt)
		{
			if (UISceneHistory[HistoryCnt].Key != SceneName)
				UISceneHistory.RemoveAt(HistoryCnt);
			else
				break;
		}
	}
	else
	{
		TPair<EUIScene, FB2UISceneOnOpen> OpenDataPair;
		OpenDataPair.Key = CurrUIScene;
		OpenDataPair.Value = UISceneOnOpenDelegate;

		// �����丮 ���ϱ� ����.. �������� ��� ���� �������� ��ȯ �ÿ��� ������ �����丮�� �����ϵ��� �Ѵ�.
		// �̰� DJLegacy �ý��۰��� ���� ���� ������ �ƴϰ� �������� UI �������̽� �� Ư¡ ������ �ɷ�..
		if (HistoryLength >= 1 && IsDJLegacyUIScene(PrevUIScene) && IsDJLegacyUIScene(CurrUIScene) && PrevUIScene != CurrUIScene)
		{
			if (PrevUIScene != EUIScene::Inventory)
			{
				if (CurrUIScene != EUIScene::RelicManagement)
				{
					UISceneHistory.RemoveAt(HistoryLength - 1);
				}
				else
				{
					// ��������Ŵ�¡���������� ��������� ���ư��ÿ��� ����..
					// �ܼ� ��������Ŵ�¡������->��������� ���ư������� ���� �����丮 ������ �����ʰ�
					// Ÿ������->��������Ŵ�¡������ �϶��� ��������� ���� �����ش�.
					if (PrevUIScene != EUIScene::Relic)
					{
						if (HistoryLength >= 1 && PrevUIScene != CurrUIScene)
						{
							UISceneHistory.RemoveAt(HistoryLength - 1);
							UISceneHistory.RemoveAt(HistoryLength - 1);
						}
					}
				}
			}
		}

		if (NotAddUISceneHistoryCheck(CurrUIScene) == false)
		{
			UISceneHistory.Add(OpenDataPair);
		}
	}

	//for (auto& WidgetData : WidgetDatas) �츮�� �Ǹ� AllWidgetData �� ��ü
	//{
	//	if (PrevSceneData)
	//	{
	//		if (DontOpenSameUIAtReservedUIScene && (PrevSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)))
	//			continue;
	//	}
	//
	//	if (CurSceneData && CurSceneData->WidgetFNames.Contains(WidgetData.WidgetFName))
	//	{
	//		OpenUI(WidgetData.WidgetFName, RightNow);
	//	}
	//}

	if (!CurSceneData) return;

	TArray<UB2UIWidget*> AllOpenInThisScene; // To call OnSceneOpen

	for (auto& CurWidgetFName : CurSceneData->WidgetFNames)
	{
		FB2UIWidgetData* FoundWidgetData = AllWidgetData.Find(CurWidgetFName);

		if (FoundWidgetData && CurWidgetFName == FoundWidgetData->WidgetFName)
		{
			if (PrevSceneData && DontOpenSameUIAtReservedUIScene && (PrevSceneData->WidgetFNames.Contains(FoundWidgetData->WidgetFName)))
			{
				// ���� �ſ������� ���ξȿ���.. ���� �����ɰŰ����� �׳� �ݰ���ο���� �ɰŰ���
				// �ݴ°� �Ʒ� CloseUIScene ����
			}
			else {
				UB2UIWidget* ThisOpenWidget = OpenUI<UB2UIWidget>(FoundWidgetData->WidgetFName, RightNow);
				if (ThisOpenWidget) {
					AllOpenInThisScene.Add(ThisOpenWidget);
				}
			}
		}
	}

	for (UB2UIWidget* ThisOpened : AllOpenInThisScene) {
		if (ThisOpened) {
			ThisOpened->OnSceneOpen(SceneName);
			ThisOpened->OnHistoryOpen(IsBack);
		}
	}

	//Doc�� CurUIScene�� �־��ְ�, UI�鿡�� CurUIScene�� �ɸ´� ó���� �ؾ� �ȴٸ� ó���� �ϰ� ���ش�.
	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
		DocUICondition->SetCurrUIScene(CurrUIScene);

	OnChangUISceneClass<EUIScene>::GetInstance().Signal(CurrUIScene);
}

void UB2UIManager::CloseUIScene(EUIScene SceneName, bool RightNow, bool DontCloseSameUIAtReservedUIScene)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseUIScene);
	//��ŷ�ϰ� Tick���� ó��
	if (!RightNow)
	{
		bClosingUIScene = true;
		ClosingUIScene = SceneName;
	}

	auto CurSceneData = GetUISceneData(SceneName);
	auto ReservedSceneData = GetUISceneData(ReservedUISceneOpenData.Key);
	if (!CurSceneData) return;

	bool bClosedSome = false;

	for (auto& CurWidgetFName : CurSceneData->WidgetFNames)
	{
		FB2UIWidgetData* FoundWidgetData = AllWidgetData.Find(CurWidgetFName);

		if (FoundWidgetData && FoundWidgetData->Widget)
		{
			//����� UIScene�� ���� UI�� ������ ���ԵǾ� �ִٸ� ���� �ʴ´�.
			if (DontCloseSameUIAtReservedUIScene &&  ReservedSceneData && (ReservedSceneData->WidgetFNames.Contains(FoundWidgetData->WidgetFName)))
				continue;

			CloseUI(FoundWidgetData->WidgetFName, RightNow);

			bClosedSome = true;
		}
	}

	// UIScene ��ȯ�ø��� ���� GC ������.
	if (bClosedSome && ForceGCOnUISceneCloseInterval > 0)
	{
		if (UISceneCloseWithoutGCCount >= ForceGCOnUISceneCloseInterval)
		{
			CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
		}
		else
		{
			++UISceneCloseWithoutGCCount;
		}
	}
}

void UB2UIManager::CheckClosingCurrUISceneAndChangeToReservedUIScene()
{
	// ���ɿ� ū ������ ��ĥ ������ �־ �� �ڵ�� ��Ȱ��ȭ ���� �����ϴ�.
	// Ȥ�� ������ ����ߴ� ���� ������ ���� ��������� ���Ӵϴ�.
	// UI ������ ���ݴ� ������ ���������� ������..
#if 0
	//UIScene�� ������ ���̸�
	auto CurSceneData = GetUISceneData(CurrUIScene);
	auto ReservedSceneData = GetUISceneData(ReservedUISceneOpenData.Key);

	if (!CurSceneData) return;

	if (bClosingUIScene)
	{
		bool AllReadyClose = true;

		for (auto& WidgetData : WidgetDatas)  �̰� �ٽ� �츮�� �Ǹ� AllWidgetData �� ����ϵ��� �ؾ�.
		{
			if (WidgetData.Widget)
			{
				//����� UIScene�� UI�� ������ ���ԵǾ� �ִٸ� ���� �ʴ´�.
				if (bDontCloseSameUIAtReservedUIScene && ReservedSceneData && (ReservedSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)))
					continue;

				if ((CurSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)) && WidgetData.Widget->GetOpenState() != EUIOpenState::Closed && WidgetData.Widget->GetOpenState() != EUIOpenState::None)
				{
					AllReadyClose = false;
					break;
				}
			}
		}


		//������ ������ ������ ���� UIScene�� ������.
		if (AllReadyClose)
		{
			if (ChangeUISceneTransitionType != EUIChangeUISceneTransitionType::PREOPEN)
				OpenNextUIScene();
		}
	}

	//UIScene�� ������ ���̸�
	if (bOpeningUIScene)
	{
		bool AllReadyOpen = true;

		for (auto& WidgetData : WidgetDatas) �̰� �ٽ� �츮�� �Ǹ� AllWidgetData �� ����ϵ��� �ؾ�.
		{
			if (WidgetData.Widget)
			{
				if ((CurSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)) && WidgetData.Widget->GetOpenState() != EUIOpenState::Opened && WidgetData.Widget->GetOpenState() != EUIOpenState::None)
				{
					AllReadyOpen = false;
					break;
				}
			}
		}

		//������ ������ ������.
		if (AllReadyOpen)
		{
			OpeningUIScene = EUIScene::None;
			bOpeningUIScene = false;
		}
	}
#endif
}

void UB2UIManager::OpenNextUIScene()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenNextUIScene);
	bClosingUIScene = false;
	ClosingUIScene = EUIScene::None;

	if (ReservedUISceneOpenData.Key != EUIScene::None)
	{
		OpenUIScene(static_cast<EUIScene>(ReservedUISceneOpenData.Key)
			, ReservedUISceneOpenData.Value
			, ChangeUISceneTransitionType == EUIChangeUISceneTransitionType::CANCEL_OPEN || ChangeUISceneTransitionType == EUIChangeUISceneTransitionType::PREOPEN || ChangeUISceneTransitionType == EUIChangeUISceneTransitionType::CANCEL_ALL
			, bDontCloseSameUIAtReservedUIScene);
		ReservedUISceneOpenData.Key = EUIScene::None;
	}
	else
	{
		CurrUIScene = EUIScene::None;
		//Doc�� CurUIScene�� �־��ְ�, UI�鿡�� CurUIScene�� �ɸ´� ó���� �ؾ� �ȴٸ� ó���� �ϰ� ���ش�.
		auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
		if (DocUICondition)
			DocUICondition->SetCurrUIScene(CurrUIScene);

		OnChangUISceneClass<EUIScene>::GetInstance().Signal(CurrUIScene);
	}
}

bool UB2UIManager::HasUISceneHistory(EUIScene SceneName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_HasUISceneHistory);
	for (auto& OpenOpPair : UISceneHistory)
	{
		if (OpenOpPair.Key == SceneName)
			return true;
	}
	return false;
}

void UB2UIManager::InitNotAddUISceneHistoryList()
{
	//UISceneHistory�� ����� �ڷΰ��� ��ư ��� �ڷ� ���� ������ ���â���� �� �� ���� ����
	NotAddUISceneHistoryList.Add(EUIScene::SummonItemResult);
}

bool UB2UIManager::NotAddUISceneHistoryCheck(EUIScene SceneName)
{
	if (NotAddUISceneHistoryList.Num() <= 0)
	{
		InitNotAddUISceneHistoryList();
	}

	for (auto& NotAddUIScene : NotAddUISceneHistoryList)
	{
		if (NotAddUIScene == SceneName)
		{
			return true;
		}
	}

	return false;
}

void UB2UIManager::UnloadAllTAssets()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_UnloadAllTAssets);
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		FB2UIWidgetData& WidgetData = WidgetDataIt.Value();

		// Unload �ȵǰ� �����ߴ� �� ����. �� CacheOnLoad �� CacheByHistory ���õ� �͵鵵 UnloadAll ������ ��ε� �ȴ�. RootSet �� ���� ���ϰ� ������ ���� �ʰ�����.
		WidgetData.ResetAllCacheMarkForUnload();

		Unload(&WidgetData);
	}
	UIDataCacheHistory.Empty();
	CachedWidgetClasses.Empty();
}

#if WITH_EDITOR
void UB2UIManager::EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr, float InStartProgress, float InEndProgress)
{
	const float TotalProgRange = FMath::Max(0.0f, InEndProgress - InStartProgress);
	// InfoAsset �� ��ϵǴ� �ֵ��� �󸶳� �Ǵ����� ���� ������ ���ϴ� ��. � element �ε����� �ѹ��� ������Ʈ �� ��.
	const int32 ProgUpdateInterval = 5;
	const int32 TotalProgUpdateCount = AllWidgetData.Num() / ProgUpdateInterval;
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	int32 CurrLoadingWidgetDataIndex = 0; // ���� �ε� ���൵ ǥ�ø� ���� �ε��� ī��Ʈ ��
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		//////////////////////////////
		// �ٽ��� ��Ű� ������ progress ����� ���� ����.
		FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
		GetWidgetClass(&WidgetData);
		//////////////////////////////

		if (InSplashProgFnPtr && (CurrLoadingWidgetDataIndex % ProgUpdateInterval == ProgUpdateInterval - 1))
		{
			CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount);
			InSplashProgFnPtr(CurrProg);
		}
		++CurrLoadingWidgetDataIndex;
	}
}
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2UIManager::CleanupOnPreSave()
{
	CachedWidgetClasses.Empty(); // Ȯ���� �� �ִ� �� ����..
}
void UB2UIManager::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif

void UB2UIManager::GetAllCreatedUINames(TArray<FName>& OutUINames)
{ // ����� Ȯ�� �뵵 ��..
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetAllCreatedUINames);
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
		if (WidgetData.Widget)
		{
			OutUINames.Add(WidgetData.WidgetFName);
		}
	}
}

void UB2UIManager::GetAllCreatedUIWidgetDatas(TArray<FB2UIWidgetData>& OutUIWidgetDatas)
{ // ����� Ȯ�� �뵵 ��..
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetAllCreatedUIWidgetDatas);
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
		if (WidgetData.Widget)
		{
			OutUIWidgetDatas.Add(WidgetData);
		}
	}
}

bool UB2UIManager::GetIsOtherWidgetVisible(TArray<FName> IgnoreWidgetName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_GetIsOtherWidgetVisible);
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		FB2UIWidgetData& WidgetData = WidgetDataIt.Value();
		if (WidgetData.Widget)
		{
			if (WidgetData.WidgetFName == UIFName::Header) continue; // �������
			if (IgnoreWidgetName.Contains(WidgetData.WidgetFName)) continue; // �߰� ���� ����

																			 // �ٸ� ������ ���� �������� �������
			if (WidgetData.Widget->GetVisibility() != ESlateVisibility::Hidden && WidgetData.Widget->GetVisibility() != ESlateVisibility::Collapsed) return true;
		}
	}
	return false;
}

TArray<FName> UB2UIManager::WidgetNamesForRootSet;
TArray<FName> UB2UIManager::WidgetNamesForLoadOnStartup;
bool UB2UIManager::IsRootSetWidgetName(FName InUIName)
{
	// WidgetNamesForRootSet ����� Ŀ���� TMap �� �������� �׷��� Ŀ���� �� �ٸ� ���������� ����ϴ�..
	return (WidgetNamesForRootSet.Contains(InUIName) || WidgetNamesForLoadOnStartup.Contains(InUIName));
}

void UB2UIManager::SetupCacheOnLoadList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �޸� ����� ����..
#if WITH_EDITOR
	if (GIsEditor) {
		return; // �����͵� �̷� �� �ʿ����.
	}
#endif

	// ��� �ϴ� ���ϱ� �̰� Rootset ����Ʈ�ʹ� �޸� �Ź� Init �ÿ� �˻��ؼ� �÷��׿� ��ũ�� �� �ִ� ���� ��. �Ѥ�;;

	B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::SetupCacheOnLoadList"));

	TArray<FName> AllCacheOnLoadUINames;
	TArray<FString> ParsedAllCacheOnLoadUIs;
	GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("CacheOnLoadUI"), ParsedAllCacheOnLoadUIs, GGameIni);

	for (const FString& ThisUIName : ParsedAllCacheOnLoadUIs)
	{
		AllCacheOnLoadUINames.AddUnique(FName(*ThisUIName));
	}

	for (const FName ThisCacheOnLoadName : AllCacheOnLoadUINames)
	{
		FB2UIWidgetData* FoundWidgetData = GetWidgetData(ThisCacheOnLoadName); // ���� UI ������ �ε��� �Ͼ�� �� �ƴ�.
		if (FoundWidgetData)
		{
			// �ᱹ ��� �ϳ� �������ڰ� �� ���� �ϴ� �ǵ� �̰� �� UIManager ���� �����ϴ� UPROPERTY �� �ƴϰ� ini ���� �о���̴� ������ �Ǿ��� �� �ִ�.
			// ������ UIWidgetData �� �����ϴ� �� �ø��� ������ �ʾҰ� ������ ������ �����Ͷ�⺸�ٴ� �ý������� �����Ͷ� �̷� �͵� �ְ� ��..
			// �׸��� ��� �̰ɷθ� ���õǴ� ���� �ƴϰ� AsyncLoad �� �� ��쿡�� �̰� ������ �ȴ�.
			// �̰Ŷ� ����� ���� StayInViewport �� ó������ �� �ִ� ����̶� UPROPERTY �� �����ϰ� �ִµ� ��� �װ� �� �����ϰ� �װ͵� UIWidgetData ���� ���� ����.
			FoundWidgetData->bCacheOnLoad = true;
		}
	}
#endif
}
void UB2UIManager::SetupRootsetWidgetList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� �޸� ����� ���� �̷� �� ���� �ʴ´�..
	if (GConfig && WidgetNamesForRootSet.Num() == 0)
	{ // Ư���� ������ UIFName ����Ʈ.. static ������ �ѹ��� ä��.
		TArray<FString> ParsedWidgetNames;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("RootSetOnLoadUI"), ParsedWidgetNames, GGameIni);
		for (const FString& ThisName : ParsedWidgetNames)
		{
			WidgetNamesForRootSet.Add(FName(*ThisName)); // �ϴ� ����Ʈ �־���⸸ �ϰ� ���� ������ �� UI �ε� �� 
		}
	}
#endif
}
void UB2UIManager::SetupLoadOnStartupWidgetList()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� �޸� ����� ���� �̷� �� ���� �ʴ´�..
	return;
#endif


	if (GConfig && WidgetNamesForLoadOnStartup.Num() == 0)
	{
		// Ư���� ������ UIFName ����Ʈ.. static ������ �ѹ��� ä��.

		B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::SetupLoadOnStartupWidgetList"));
		TArray<FString> ParsedWidgetNames;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("LoadOnStartupUI"), ParsedWidgetNames, GGameIni);
		for (const FString& ThisName : ParsedWidgetNames)
		{
			WidgetNamesForLoadOnStartup.Add(FName(*ThisName));
		}

		// ù �κ� �ε� progress ������ �̰� ��꿡 �־�� �Ѵ�.
		if (WidgetNamesForLoadOnStartup.Num() > 0) {
			B2GMLoadingProgCollector::SetStepRate(0.4f / (float)WidgetNamesForLoadOnStartup.Num());
		}

		// ����Ʈ �־���� ÷���� �ε��Ѵ�. ó�� �κ� �ε� ������ �ǰ���..
		for (FName& ThisName : WidgetNamesForLoadOnStartup)
		{
			const FB2UIWidgetData* DataToLoad = GetWidgetData(ThisName);
			GetWidgetClass(DataToLoad);
			B2GMLoadingProgCollector::Step(); // �ε� ���൵ ����.
		}
	}
}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
UB2DebugTextWidget* UB2UIManager::GetScreenDebugTextWidget()
{
	return ScreenDebugTextWidget;
}
void UB2UIManager::CreateScreenDebugWidget()
{
	if (!ScreenDebugTextWidget)
	{
		TSubclassOf<UB2DebugTextWidget> CreateClass = UB2DebugTextWidget::GetUniqueClassForScreenDisplay();
		if (CreateClass)
		{
			ScreenDebugTextWidget = CreateWidget<UB2DebugTextWidget>(GetWorld(), this, CreateClass);
			if (ScreenDebugTextWidget)
			{
				ScreenDebugTextWidget->AddToViewport(UB2DebugTextWidget::GetViewportZOrder());
			}
		}
	}
}
void UB2UIManager::DestroyScreenDebugWidget()
{
	if (ScreenDebugTextWidget)
	{
		ScreenDebugTextWidget->DestroySelf();
	}
	ScreenDebugTextWidget = NULL;
}
#endif

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
bool UB2UIManager::DevIsCombatEventUI(FName InCheckUI) const
{
	// StageClear �� �� ġ��.. �� ��� ��� ���� ������ �ȵǴ� �� �����Ѵ�. ���� �׽�Ʈ��
	// AB2StageEventDirector::IsStageEventRelatedUI �� �����ϴ� �κ��� ������ ������ ������ ����.
	return (
		//InCheckUI == UIFName::StageBegin || 
		//InCheckUI == UIFName::PVPBegin ||
		InCheckUI == UIFName::BossStatus ||
		InCheckUI == UIFName::MobAppearBossArea ||
		InCheckUI == UIFName::MobAppearBossStage ||
		InCheckUI == UIFName::ScenarioDialog
		);
}
#endif
