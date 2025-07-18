
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
	// CanUnloadByPolicy 를 true 로 만들어서 Unload 콜이 먹히도록 하기 위해.
	// 그러나 이렇게 하고 Unload 를 해도 RootSet 이라면 실질적인 unload 가 되지는 않을 것.
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
	ScreenDebugTextWidget = NULL; // Shipping 빌드에서는 NULL 값을 유지하고 있어야.

	ErrorResetPerSecond = 5;
	IsIndicatorVisible = false;
}

UB2UIManager::~UB2UIManager()
{
	// 어떤 오브젝트가 어떤 경로로 소멸자로 들어오는지 알 수 없음. 특히 프로세스 종료시 Destroy 가 불리면 문제가 됨. 
	// 일반적인 상황에서의 Destroy 는 ABladeIIGameMode::OnPreLoadMap 에서 처리.
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

	//혹시 모르니까 하위 위젯들의 컨텍스트도 바꿔준다
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
		return NULL; // DLC Front 모드 리소스로딩 최대한 제거
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
		{ // Init 은 밖에서 불러줄 것.
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
{ // 에디터에서 Destroy 가 제대로 안된채 쓰레기값 Instance 를 사용하게 되는 경우가 있어서 준비함.
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
			//FString WarnMsg = FString::Printf(TEXT("다음 %d 개의 WidgetData 에 StayInViewport 가 체크되어 있습니다. 이는 부작용과 버그를 유발할 수 있으니 주의해서 사용해야 합니다.\nUI 전환 시의 블로킹을 완화하기 위한 목적이라면 다음 페이지에 소개된 설정들을 먼저 참고해 주세요\nhttp://172.168.2.105:8090/pages/viewpage.action?pageId=4827590\n\n"), WidgetNamesWithStayInViewport.Num());
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
	// FB2UIWidgetData 의 WidgetFName 은 직접 세팅하도록 하지 않고 본인이 등록된 맵의 키 값에서 가져옴.
	for (TMap<FName, FB2UIWidgetData>::TIterator NewWidgetDataIt(AllWidgetData); NewWidgetDataIt; ++NewWidgetDataIt)
	{
		FB2UIWidgetData& ThisWidgetData = NewWidgetDataIt.Value();
		if (ThisWidgetData.WidgetFName != NewWidgetDataIt.Key())
		{
			ThisWidgetData.WidgetFName = NewWidgetDataIt.Key();
		}
	}
	for (TMap<EUIScene, FB2UISceneData>::TIterator SceneDataIt(AllSceneData); SceneDataIt; ++SceneDataIt)
	{ // SceneData 도 마찬가지로 SceneName 을 Map 의 키값에서 가져온다.
		FB2UISceneData& ThisSceneData = SceneDataIt.Value();
		if (ThisSceneData.SceneName != SceneDataIt.Key())
		{
			ThisSceneData.SceneName = SceneDataIt.Key();
		}
	}

#if !PLATFORM_MAC
	FString NotFoundWidgetNameMsg = TEXT("SceneData 에 설정한 다음 WidgetFName 이 AllWidgetData 에서 발견되지 않았습니다.\n\n");
	int32 FoundSomeInvalidCount = 0;
	for (TMap<EUIScene, FB2UISceneData>::TIterator SceneDataIt(AllSceneData); SceneDataIt; ++SceneDataIt)
	{
		FB2UISceneData& ThisSceneData = SceneDataIt.Value();
		for (FName& ThisSceneWidgetFName : ThisSceneData.WidgetFNames)
		{
			// DJLegacy 와의 연결 용으로 더미 세팅한 게 있으니 그건 검사에서 제외.
			if (!IsDJLegacyUIScene(ThisSceneData.SceneName) && !AllWidgetData.Find(ThisSceneWidgetFName))
			{
				NotFoundWidgetNameMsg += FString::Printf(TEXT("SceneData SceneName %d 의 WidgetFName %s\n"), (int32)ThisSceneData.SceneName, *ThisSceneWidgetFName.ToString());
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

#if 0 // 이걸 사용하게 되면 결국 맵 로딩 시간이 길어지게 될 것이다. 트레이드오프가 명확하므로 단순 CacheOnLoad 를 하거나 적절한 지점에 AsyncLoad 를 하거나.
void UB2UIManager::SyncPreloadTAssets(const TArray<FName>& InPreloadUIName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_PreloadTAssets);
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::PreloadTAssets"));

	for (const FName ThisPreloadName : InPreloadUIName)
	{
		FB2UIWidgetData* FoundWidgetData = GetWidgetData(ThisPreloadName);
		if (FoundWidgetData)
		{
			FoundWidgetData->bCacheOnLoad = true; // CachedWidgetClasses 에도 캐싱해 놓고 OpenUI 할 때에 Unload 하지 않도록.
			GetWidgetClass(FoundWidgetData); // TAsset 클래스 로딩.
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
		// bAllowUIAsyncLoading 에 따른 SyncPreload 로의 전환은 여기선 좀 생각해 볼 필요가 있다.
		// UIManager 의 Async Load 는 PreLoad 타이밍의 멀티코어 활용이 목적이 아니라 인게임 진행 도중 백그라운드 로딩이 목적임.
		//SyncPreloadTAssets(InLoadUIs); // 따라서 여기선 필요할 때 로딩하도록 그냥 리턴 ㅋ
		return;
	}

	for (const FName& UIName : InLoadUIs)
	{
		if (FB2UIWidgetData* WidgetData = GetWidgetData(UIName))
		{
			// UIManager::AsyncLoading은 1 RequestName : 1 UI Asset을 원칙으로 한다. 1 : 다 는 안됨
			WidgetData->bCacheOnLoad = true; // 로딩 완료 후 캐싱을 해야 하는데 마크를 따로 안 했다가는 다른 쪽에서 언로딩을 하게 될 수도 있으니 bCacheOnLoad 마크.
			const auto& ClassAsset = WidgetData->GetWidgetClassAsset();
			TryAsyncLoad(UIName.ToString(), TArray<FSoftObjectPath>{ClassAsset.ToSoftObjectPath()});
		}
	}
}

void UB2UIManager::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& CompleteRequestList)
{
	if (CompleteRequestList.IsValidIndex(0))	// 하나의 Request에 하나의 UI만
	{
		const FSoftObjectPath& LoadedAsset = CompleteRequestList[0];
		if (UClass* LoadedClassAsset = Cast<UClass>(LoadedAsset.ResolveObject()))
		{
			const FName UIName(*CompleteRequest);
			// Async 로딩 대상은 CanUnloadByPolicy 를 체크할 필요가 없다. 언제 쓰일지 모르는 거니 무조건 캐싱해 둠.
			// 단 요청 시점에 bCacheOnLoad 를 세팅해 놓을 필요는 있다. 안 그러면 다른 거 핸들링 도중에 언로딩이 되어 버릴 소지가 있다.
			UClass*& FoundOrAddedCache = CachedWidgetClasses.FindOrAdd(UIName);
			FoundOrAddedCache = LoadedClassAsset;
		}
	}
}

void UB2UIManager::CloseAllUIs() // 사실상 PreDestroy 격으로 매 레벨 언로드 시에 불리워야 함.
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllUIs);
	CloseAllMsgPopups();

	ChangeUIScene(EUIScene::None);

	for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget)
		{
			// bCloseStaticUIs 를 true 로 넘겨주어서 bStayInViewport 인 것들도 모두 닫는다.
			CloseUI(ThisWidgetData.WidgetFName, false, true);
		}
	}
}

void UB2UIManager::CloseAllStandaloneUIs()
{ // UIScene 은 남겨두고 기타 자잘한 팝업이나 별도로 열린 UI 들만 없애려는 거.
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
				continue; // 현재 Scene 으로 열려있는 건 닫지 않음.
			}
			// bCloseStaticUIs 를 true 로 넘겨주어서 bStayInViewport 인 것들도 모두 닫는다.
			CloseUI(ThisWidgetData.WidgetFName, false, true);
		}
	}
}

void UB2UIManager::Destroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_Destroy);
#if WITH_EDITOR
	if (GIsEditor) {
		CachedWidgetClasses.Empty(); // 에디터면 Destroy 만 따로 불린 상황에서 확실히 비우도록.
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
		if (!GIsEditor) { // 에디터는 좀 다름. DestroySelf 자체가 필요없어 보이기도.
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

// Level transition 시 이걸로 clear..
void UB2UIManager::MasterDestroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_MasterDestroy);
	if (!GIsEditor)
	{ // 다른 TAsset 들은 ManualUnloadBeforeNewBeginning 에서 하는 거.
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
		if (!FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) //중첩 바인딩 안되게
			FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &UB2UIManager::Destroy);
#endif

		MainWidget = Cast<UB2UIMain>(CreateWidget<UUserWidget>(TheWorld, this, UIMainClass));
		if (MainWidget)
		{
			MainWidget->Init();
			MainWidget->AddToViewport(BII_WIDGET_ZORDER_NEW_UI_MAIN); // Z-order 를 따로 관리하는 DJLegacy 와 공존하는 한 별도 z 값을 넣는다.
			PrepareUI();
			bInit = true;
		}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
		CreateScreenDebugWidget();
#endif

		SubscribeEvents();
	}

	InitNotAddUISceneHistoryList();

	//반대로 요기에 등록한거만 
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
			//뷰포트에 항상 띄워져 있어야 하는 녀석들. CreateWidget을 그때그때 하면서 생기는 블로킹을 방지(자주 사용하는 녀석만 사용할 것~)
#if 0 // bStayInViewport 를 굳이 처음부터 열지는 않고 처음 연 이후로 닫지 않는 걸로 바꾼다.
			if (WidgetData->bStayInViewport)
			{
				UClass* WidgetClass = GetWidgetClass(*WidgetData);
				auto* PreCreateWidget = CreateWidget<UB2UIWidget>(GetWorld(), this, WidgetClass);

				if (PreCreateWidget)
				{
					WidgetData->Widget = PreCreateWidget;
					PreCreateWidget->MyUIFName = WidgetData->WidgetFName;
					PreCreateWidget->Init();

					//항상 띄워놓는 UI의 경우,Open될때 뿐 아니라 젤 처음 생성될때도 DocBind의 기회가 주어진다. 왜냐면 항상 떠있으니까...
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
	// UIWidget 을 식별자로 해서 찾으려면 짤없이 모두 iteration 해야 하니 이건 특별한 경우에나 사용을.. 
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

TSubclassOf<UB2UIWidget> UB2UIManager::GetWidgetClass(const FB2UIWidgetData* WidgetData) // 단순 Get 만이 아닌 로딩을 함.
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
			if (!WidgetData->CanUnloadByPolicy()) { // 어떤 식으로든 Cache 마크가 있는 경우, 언로딩 방지 캐싱
				UClass*& FoundOrAddedCache = CachedWidgetClasses.FindOrAdd(WidgetFName);
				FoundOrAddedCache = WidgetClass;
			}

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 메모리 우려로 인해 이런 거 하지 않는다..

			if (UB2UIManager::IsRootSetWidgetName(WidgetFName))
			{
				// 이건 아예 게임 종료 시점까지 안 내리겠다는 보다 강력한 의사 표명.
				// 로딩 시간이 충분히 길면서 자주 쓰이는 것들로 선정.
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
		// CacheOnLoad 등의 마크가 된 건 UnloadAllTAssets 같은 데서 플래그를 제거한 이후에 이걸 콜 해야 한다.
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

	// bStayInViewport 를 굳이 처음부터 열지는 않고 처음 연 이후로 닫지 않는 걸로 바꾼다.
	//if (!WidgetData->bStayInViewport)
	{
		// 20180124_YJ
		// 기본적으로 만드는 위젯들은 MainWidget에 생성순서대로 붙여넣지만,
		// 약네트워크같은 누도성 최상단에 위치 해야만하는 팝업의 경우에 MainWidget을 무시하고
		// AddToViewport 의 Zoder 값을 1000으로 세팅하여 띄워줌
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

	//오픈 될 때 자동바인드 해야 된다면
	if (WidgetData->bBindDocAutoOnOpen)
	{
		IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(Widget);
		if (CastDocBindable)
			CastDocBindable->BindDocAuto();
	}

	//오픈 될 때, 월드 블러를 먹여야 된다면
	if (WidgetData->bAllowWorldBackgroundBlur)
	{
		Widget->bWorldBackgroundBlurOn = true;
		StartWorldBackgroundBlur(WidgetData->WorldBackgroundBlurScale);
	}

	//기본적으로는 Visibility를 켜준다.
	Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//오픈 될 때, 전투 관련 UI를 숨겨야 한다면( bHideBattleRelatedWidgets는 각 위젯에서 세팅이 되고, bHUDHidingCinematicMode는 외부에서 세팅이 된다 )
	if (
		(bCinematicMode && bHUDHidingCinematicMode) ||
		(!bCinematicMode && WidgetData->bHideBattleRelatedWidgets) // 현재는 CinematicMode 설정이 우선임. CinematicMode 라면 bHideBattleRelatedWidgets 설정을 무시.
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

	//OpenState가 None이면 아예 열리지도 않았으니 닫을 필요도 없다.
	if (Widget->GetOpenState() == EUIOpenState::None)return;



	//UI쪽 연출이 완료되었으면
	if (Widget->GetOpenState() == EUIOpenState::Closed)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
		Widget->OpenState = EUIOpenState::None;

		// bStayInViewport 인 것들은 bCloseStaticUIs 가 들어온 경우에만 닫는다. (레벨 언로드 시점)
		if (!WidgetData->bStayInViewport || bCloseStaticUIs)
		{
			Widget->DestroySelf(this);
			WidgetData->Widget = nullptr; // 상단 DestroySelf 에서 처리될 것.
		}

		//오픈 될 때 자동바인드 했다면, 자동 해제
		if (WidgetData->bBindDocAutoOnOpen)
		{
			IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(Widget);
			if (CastDocBindable)
				CastDocBindable->UnbindDoc();
		}

		//오픈 될 때, 월드 블러를 먹였다면 해제
		if (WidgetData->bAllowWorldBackgroundBlur && Widget->bWorldBackgroundBlurOn)
		{
			Widget->bWorldBackgroundBlurOn = false;
			StopWorldBackgroundBlur();
		}

		//오픈 될 때, 전투 관련 UI를 숨긴 것을 복구 해야 한다면.
		// 현 구성은 bHideBattleRelatedWidgets 설정된 것이 여러개 열려있을 때 문제가 될 것이다. 그런 경우에 대한 대처가 필요하다면 상태를 따로 저장해 둘 필요가 있다.
		// 그러나 CinematicMode 설정도 같이 고려할 필요가 있을 것.
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
{ // UIDataCacheHistory 기능 메인
	if (UIDataCacheHistoryNum > 0 && InJustOpenedUIData
		// 루트셋 처리를 하는 건 여기에 포함시키지 않는다... bCacheOnLoad 마크된 것도 무시해 버릴까..
		&& !IsRootSetWidgetName(InJustOpenedUIData->WidgetFName)
		&& !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 에서 이걸 하려면 IsRootSetWidgetName 을 무시하던지, 아님 그 기능도 같이 사용하던지..
		)
	{
		const FName JustOpenedUIName = InJustOpenedUIData->WidgetFName;
		FName PoppedUIName = NAME_None; // 이번 게 history 에 추가되는 것으로 인해 빠지는 게 있다면 이름이 들어가게 될 것.

		int32 ExistingIndex = UIDataCacheHistory.Find(JustOpenedUIName);
		if (ExistingIndex == INDEX_NONE)
		{
			// 기존에 리스트에 없다면 새로 추가가 될 것이니 그 전에 history 가 만땅인 상태면 맨 앞에 걸 먼저 빼고..
			if (UIDataCacheHistory.Num() >= UIDataCacheHistoryNum)
			{
				PoppedUIName = UIDataCacheHistory[0];
				UIDataCacheHistory.RemoveAt(0);
			}
		}
		else
		{
			// 기존에 리스트에 있다면 일단 뺀다.
			UIDataCacheHistory.RemoveAt(ExistingIndex);
		}

		// 리스트의 제일 뒤에 추가.
		UIDataCacheHistory.Add(JustOpenedUIName);

		// bCacheByHistory 체크.
		InJustOpenedUIData->bCacheByHistory = true;

		if (PoppedUIName != NAME_None)
		{
			// 이번에 빠진 것에 대한 unload 처리. 
			// ResetAllCacheMarkForUnload 를 하는 게 아니라 bCacheByHistory 만 리셋한다.
			// bCacheByHistory 와 별개로 bCacheOnLoad 가 세팅되어 있다면 여전히 Unload 를 하지 않음.
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
		|| gbHACKInGameUIHiddenByDevCommand // 개발용 커맨드로 숨긴 경우.		
#endif
		)
	{
		// 이게 상황에 따라 완벽히 숨겨주지는 않는데 gbHACKInGameUIHiddenByDevCommand 를 참고. 
		// 아마도 WorldBackgroundBlur 를 쓰는 상황에선 그것까지 감안할 필요가 없지 않을까 함.
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

	// 현 구현으로는 여기서 CinematicMode 나갈 때 현재 열려있는 Widget 의 WidgetData 에 bHideBattleRelatedWidgets 가 설정되어 있어도 다시 보이게 될 것.
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

// 한번 사용한 에러코드는 5초동안 사용할 수 없음
// NoCheckErrorCode에 예외 등록 할 수 있음
void UB2UIManager::CheckErrorTimeStamp(float DeltaTime)
{
	static float CheckTime = 0.0f;
	CheckTime += DeltaTime;

	// 0.5초에 한번 체크
	if (CheckTime > 0.5f)
	{
		const int64 ErrorCoolTime = ETimespan::TicksPerSecond * ErrorResetPerSecond; // 5초마다 리셋
		const int64 CurrentUTCTime = UB2GameInstance::GetUTCNow().GetTicks();
		CheckTime = 0.0f;

		for (int32 i = 0; i < ErrorTimeStamp.Num(); ++i)
		{
			// 팝업시간 + 쿨타임 < 현재시간
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
	//[@AKI, 180111] 앞으로 숫자 넣을때 아래 주소 참고하여 영문 넣을 것
	//http://172.168.2.106/result_codes.html
	//////////////////////////////////////////////////////////////////////////////
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenMsgPopupFromErrorCode);

	if (CheckErrorCodeContains(nErrorCode)) // 체크하지 않는 Errorcode는 바로 실행(상점 등)
	{
		if (ErrorCodeContains(nErrorCode)) // 담겨 있으면 return
			return;

		SetErrorTimeStamp(nErrorCode); // 등록
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
	case B2ResultCode::FAILURE_SOCIALPOINT_UNDERFLOW:	//FAILURE_SOCIALPOINT_UNDERFLOW		SocialPoint is zero.//상점에서 우정포인트 부족할때 샀을 경우
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
		// EUIMsgPopup::Simple 이용할땐 에러코드번호도 보여줌

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

		//OpenUI로 열었으면, CloseUI로 닫아야지... 나중에 팝업이 n개 겹쳐서 떠야 될 수 있으니 OpenUI가 아닌 다른 루트로 띄우는 작업이 필요할 듯
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
			// 네트웍 끊김 관련 팝업은 지우지 않음
			if (MsgPopup.Value->GetPriority() == InIsPriority)
			{
				return true;
			}
		}
	}
	return false;
}

void UB2UIManager::CloseAllMsgPopups(bool bRightNow)
{ // CloseMsgPopup 에 넣어줄 ID 나 MsgPopup 레퍼런스 사용이 불가할 경우 어쩔 수 없이 사용. 혹은 기타 다른 특수한 경우.
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllMsgPopups);

	for (auto MsgPopup : MsgPopupMap)
	{
		if (MsgPopup.Value)
		{
			// 네트웍 끊김 관련 팝업은 지우지 않음
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
	//	|| (CheckMailReward && b2network::B2RewardPushType::MAIL != BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardItem->id))) //  메일함 or 공용함 체크
	//	return nullptr;

	//if (RewardItem == nullptr
	//	|| (CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardItem->id))
	//	)//  메일함 or 공용함 체크
	//	return nullptr;

	//return UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);

	return RewardItem ? OpenRewardMailPopUp(RewardItem->raw_reward->id, CheckMailReward) : nullptr;
}

// 보상을 메일로 받으면 true, 공용함으로 받으면 false
//  BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardItem[0]->id)) 이 부분은 현재 기획에서 여러 아이템을 받을때, 모두 공용함 or 모두 메일로 스크립트 작성을 하기로했음. 2017/07/12
//  그래서 0번째 인덱스만 검사
//  후에 수정된다면 다 돌면서 체크
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
	if ((CheckMailReward && b2network::B2RewardPushType::MAIL != BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardId))) //  메일함 or 공용함 체크
		return nullptr;

	if ((CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardId))
		)//  메일함 or 공용함 체크
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
				if (ParentWidget == nullptr) // 부모 없으면 첫번째 AlarmWidget을 바로 리턴
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

	//이미 열려 있는 UIScene으로 체인지는 안된다.
	if (SceneName == CurrUIScene) return;

	B2AndroidBackManager::GetInstance()->RemoveAllPopup();

	//이미 ChangeUIScene이 동작 중이라는 뜻인데, 또 Change가 불린거다. 일단 전후사정 봐주지 말고 동작중인 UIScene은 걍 닫는걸로.
	if (bClosingUIScene || bOpeningUIScene)
	{
		ReservedUISceneOpenData.Key = EUIScene::None;

		if (ClosingUIScene != EUIScene::None)
			CloseUIScene(ClosingUIScene, true, DontCloseSameUIAtReservedUIScene);
		if (OpeningUIScene != EUIScene::None)
			CloseUIScene(OpeningUIScene, true, DontCloseSameUIAtReservedUIScene);

		if (ClosingUIScene == EUIScene::None && OpeningUIScene == EUIScene::None && CurrUIScene != EUIScene::None) //RightNow였던 상황. ChangeUIScene 로직이 이상하다..
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
	{ // 별도 시스템인 DJLagacy 페이지가 혹 닫히지 않았을 수 있음.
		DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_End);
	}

	//닫기만 하고, Open은 하지 않는다.
	if (SceneName == EUIScene::None)
	{
		CloseUIScene(CurrUIScene, TransType == EUIChangeUISceneTransitionType::CANCEL_CLOSE || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL, false);
		CurrUIScene = EUIScene::None;
		return;
	}

	//현재 아무 UIScene도 아닌경우, Close없이 Open바로
	if (CurrUIScene == EUIScene::None)
	{
		OpenUIScene(SceneName, UISceneOnOpenDelegate, TransType == EUIChangeUISceneTransitionType::CANCEL_OPEN || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL, false);
		return;
	}

	//지금 UIScene을 Tick에서 아닌 바로 닫아야 되는가?
	bool CloseCurUISceneRightNow = (TransType == EUIChangeUISceneTransitionType::CANCEL_CLOSE || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL);

	//1. 전환 연출 타입 저장
	ChangeUISceneTransitionType = TransType;
	//2. 예약UIScene 지정
	ReservedUISceneOpenData.Key = SceneName;
	ReservedUISceneOpenData.Value = UISceneOnOpenDelegate;
	bDontCloseSameUIAtReservedUIScene = DontCloseSameUIAtReservedUIScene;
	//3. 지금UIScene 닫기
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
		bOpeningDJLegacyHeroMgmtBySceneHistory = true; // 요건 LobbyEnterHeroMgmtModeClass 에 인자로 넣어주는 게 깔끔할 수도.. 바꿔야 할 곳이 좀 많다.

													   // 이 UIScene 들은 실제 이 시스템에서 사용되지 않는다. 실제로는 DJLegacy 시스템이고 그쪽과의 연결고리로서 남겨둘 뿐.
													   // 거의 두 시스템 사이가 연결되는 백 버튼 작동을 위함.
		OpenDJLegacyHeroMgmtPageForUIScene(TargetBackUIScene);

		bOpeningDJLegacyHeroMgmtBySceneHistory = false;
	}
	else
	{
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(TargetBackUIScene);

		// 백버튼을 눌러 Mail UI 로 돌아올 경우 LinkManager를 통해 돌아온 것이 아니기 때문에 리스트 업데이트가 이루어지지 않으며 직접 업데이트 해주어야 한다.
		if (TargetBackUIScene == EUIScene::Mail)
		{
			MailTabIndex RequestTab = MailTabIndex::EMailTab_Gift;
			data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(RequestTab));
		}
	}

	// DJLegacy 인 경우는 여기서 UISceneHistory 제거 외에 실질적으로 뭔가는 하지는 않아야.
	ChangeUIScene(TargetBackUIScene, TransType, UISceneOnOpenDelegate, DontCloseSameUIAtReservedUIScene, bPassBattleUIScene);
}

EUIScene UB2UIManager::GetPrevUIScene()
{
	return (UISceneHistory.Num() > 1) ? UISceneHistory[UISceneHistory.Num() - 2].Key : EUIScene::None;
}

//UI씬 히스토리에 축적되어 있는 씬들중에 전투에 관계된 씬이 있으면 건너 띌때 사용
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
				if (CurSceneData && CurSceneData->IsBattleRelatedScene) // DJLegacy 와의 호환을 위해 넣어둔 더미 씬 식별자의 경우 실제 SceneData 는 없을 수 있다.
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
	// Artifical Add 라는 것은 실제 Open 없이 단지 history 차원에서 넣는 용도를 뜻함.
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

	//예약 UIScene이 이전 히스토리와 같다면, 히스토리에서 팝시키고 아니면 히스토리에 축적한다.
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

		// 히스토리 더하기 전에.. 영웅관리 모드 간의 직접적인 전환 시에는 마지막 히스토리만 유지하도록 한다.
		// 이건 DJLegacy 시스템과의 연계 문제 때문은 아니고 영웅관리 UI 인터페이스 상 특징 때문인 걸로..
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
					// 고대유물매니징페이지에서 고대유물로 돌아갈시에만 예외..
					// 단순 고대유물매니징페이지->고대유물로 돌아갈때에는 강제 히스토리 삭제를 하지않고
					// 타페이지->고대유물매니징페이지 일때는 고대유물도 같이 지워준다.
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

	//for (auto& WidgetData : WidgetDatas) 살리게 되면 AllWidgetData 로 교체
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
				// 이전 신에있으면 새로안열음.. 만약 문제될거같으면 그냥 닫고새로열어두 될거같음
				// 닫는건 아래 CloseUIScene 에서
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

	//Doc에 CurUIScene을 넣어주고, UI들에서 CurUIScene에 걸맞는 처리를 해야 된다면 처리를 하게 해준다.
	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
		DocUICondition->SetCurrUIScene(CurrUIScene);

	OnChangUISceneClass<EUIScene>::GetInstance().Signal(CurrUIScene);
}

void UB2UIManager::CloseUIScene(EUIScene SceneName, bool RightNow, bool DontCloseSameUIAtReservedUIScene)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseUIScene);
	//마킹하고 Tick에서 처리
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
			//예약된 UIScene에 현재 UI가 여전히 포함되어 있다면 닫지 않는다.
			if (DontCloseSameUIAtReservedUIScene &&  ReservedSceneData && (ReservedSceneData->WidgetFNames.Contains(FoundWidgetData->WidgetFName)))
				continue;

			CloseUI(FoundWidgetData->WidgetFName, RightNow);

			bClosedSome = true;
		}
	}

	// UIScene 전환시마다 강제 GC 돌리기.
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
	// 성능에 큰 영향을 미칠 소지가 있어서 이 코드는 비활성화 시켜 놓습니다.
	// 혹시 이전에 사용했던 곳이 있을지 몰라 참고용으로 놔둡니다.
	// UI 페이지 여닫는 연출은 개별적으로 구현을..
#if 0
	//UIScene이 닫히는 중이면
	auto CurSceneData = GetUISceneData(CurrUIScene);
	auto ReservedSceneData = GetUISceneData(ReservedUISceneOpenData.Key);

	if (!CurSceneData) return;

	if (bClosingUIScene)
	{
		bool AllReadyClose = true;

		for (auto& WidgetData : WidgetDatas)  이거 다시 살리게 되면 AllWidgetData 를 사용하도록 해야.
		{
			if (WidgetData.Widget)
			{
				//예약된 UIScene에 UI가 여전히 포함되어 있다면 닫지 않는다.
				if (bDontCloseSameUIAtReservedUIScene && ReservedSceneData && (ReservedSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)))
					continue;

				if ((CurSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)) && WidgetData.Widget->GetOpenState() != EUIOpenState::Closed && WidgetData.Widget->GetOpenState() != EUIOpenState::None)
				{
					AllReadyClose = false;
					break;
				}
			}
		}


		//닫히는 연출이 끝났다 다음 UIScene을 열어줘.
		if (AllReadyClose)
		{
			if (ChangeUISceneTransitionType != EUIChangeUISceneTransitionType::PREOPEN)
				OpenNextUIScene();
		}
	}

	//UIScene이 열리는 중이면
	if (bOpeningUIScene)
	{
		bool AllReadyOpen = true;

		for (auto& WidgetData : WidgetDatas) 이거 다시 살리게 되면 AllWidgetData 를 사용하도록 해야.
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

		//열리는 연출이 끝났다.
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
		//Doc에 CurUIScene을 넣어주고, UI들에서 CurUIScene에 걸맞는 처리를 해야 된다면 처리를 하게 해준다.
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
	//UISceneHistory를 사용한 뒤로가기 버튼 사용 뒤로 갈시 아이템 결과창으로 갈 일 절대 없음
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

		// Unload 안되게 세팅했던 거 리셋. 즉 CacheOnLoad 나 CacheByHistory 세팅된 것들도 UnloadAll 에서는 언로딩 된다. RootSet 인 경우는 여하간 영향을 받지 않겠지만.
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
	// InfoAsset 에 등록되는 애들이 얼마나 되는지에 따라 적절히 정하는 거. 몇개 element 로딩마다 한번씩 업데이트 할 지.
	const int32 ProgUpdateInterval = 5;
	const int32 TotalProgUpdateCount = AllWidgetData.Num() / ProgUpdateInterval;
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	int32 CurrLoadingWidgetDataIndex = 0; // 단지 로딩 진행도 표시를 위한 인덱스 카운트 ㅋ
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		//////////////////////////////
		// 핵심은 요거고 나머지 progress 계산은 단지 사족.
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
	CachedWidgetClasses.Empty(); // 확실히 해 주는 게 좋지..
}
void UB2UIManager::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif

void UB2UIManager::GetAllCreatedUINames(TArray<FName>& OutUINames)
{ // 디버그 확인 용도 등..
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
{ // 디버그 확인 용도 등..
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
			if (WidgetData.WidgetFName == UIFName::Header) continue; // 헤더제외
			if (IgnoreWidgetName.Contains(WidgetData.WidgetFName)) continue; // 추가 제외 위젯

																			 // 다른 위젯이 현재 보여지고 있을경우
			if (WidgetData.Widget->GetVisibility() != ESlateVisibility::Hidden && WidgetData.Widget->GetVisibility() != ESlateVisibility::Collapsed) return true;
		}
	}
	return false;
}

TArray<FName> UB2UIManager::WidgetNamesForRootSet;
TArray<FName> UB2UIManager::WidgetNamesForLoadOnStartup;
bool UB2UIManager::IsRootSetWidgetName(FName InUIName)
{
	// WidgetNamesForRootSet 사이즈가 커지면 TMap 이 낫겠지만 그렇게 커지는 건 다른 이유에서라도 곤란하다..
	return (WidgetNamesForRootSet.Contains(InUIName) || WidgetNamesForLoadOnStartup.Contains(InUIName));
}

void UB2UIManager::SetupCacheOnLoadList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 메모리 우려로 인해..
#if WITH_EDITOR
	if (GIsEditor) {
		return; // 에디터도 이런 거 필요없음.
	}
#endif

	// 어떻게 하다 보니까 이건 Rootset 리스트와는 달리 매번 Init 시에 검사해서 플래그에 마크를 해 주는 식이 됨. ㅡㅡ;;

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
		FB2UIWidgetData* FoundWidgetData = GetWidgetData(ThisCacheOnLoadName); // 실제 UI 데이터 로딩이 일어나는 거 아님.
		if (FoundWidgetData)
		{
			// 결국 요거 하나 세팅하자고 이 짓을 하는 건데 이게 왜 UIManager 에서 세팅하는 UPROPERTY 가 아니고 ini 에서 읽어들이는 값인지 의아할 수 있다.
			// 이유는 UIWidgetData 에 세팅하는 걸 늘리고 싶지도 않았고 컨텐츠 차원의 데이터라기보다는 시스템적인 데이터라 이런 것도 있고 등..
			// 그리고 사실 이걸로만 세팅되는 값도 아니고 AsyncLoad 를 한 경우에도 이게 세팅이 된다.
			// 이거랑 비슷한 삘의 StayInViewport 는 처음부터 들어가 있던 기능이라 UPROPERTY 를 유지하고 있는데 사실 그게 더 위험하고 그것도 UIWidgetData 에서 빼고 싶음.
			FoundWidgetData->bCacheOnLoad = true;
		}
	}
#endif
}
void UB2UIManager::SetupRootsetWidgetList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 메모리 우려로 인해 이런 거 하지 않는다..
	if (GConfig && WidgetNamesForRootSet.Num() == 0)
	{ // 특별히 선별된 UIFName 리스트.. static 변수라 한번만 채움.
		TArray<FString> ParsedWidgetNames;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("RootSetOnLoadUI"), ParsedWidgetNames, GGameIni);
		for (const FString& ThisName : ParsedWidgetNames)
		{
			WidgetNamesForRootSet.Add(FName(*ThisName)); // 일단 리스트 넣어놓기만 하고 실제 적용은 각 UI 로딩 시 
		}
	}
#endif
}
void UB2UIManager::SetupLoadOnStartupWidgetList()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 메모리 우려로 인해 이런 거 하지 않는다..
	return;
#endif


	if (GConfig && WidgetNamesForLoadOnStartup.Num() == 0)
	{
		// 특별히 선별된 UIFName 리스트.. static 변수라 한번만 채움.

		B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::SetupLoadOnStartupWidgetList"));
		TArray<FString> ParsedWidgetNames;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("LoadOnStartupUI"), ParsedWidgetNames, GGameIni);
		for (const FString& ThisName : ParsedWidgetNames)
		{
			WidgetNamesForLoadOnStartup.Add(FName(*ThisName));
		}

		// 첫 로비 로딩 progress 에서는 이걸 계산에 넣어야 한다.
		if (WidgetNamesForLoadOnStartup.Num() > 0) {
			B2GMLoadingProgCollector::SetStepRate(0.4f / (float)WidgetNamesForLoadOnStartup.Num());
		}

		// 리스트 넣어놓고선 첨부터 로딩한다. 처음 로비 로딩 시점이 되겠지..
		for (FName& ThisName : WidgetNamesForLoadOnStartup)
		{
			const FB2UIWidgetData* DataToLoad = GetWidgetData(ThisName);
			GetWidgetClass(DataToLoad);
			B2GMLoadingProgCollector::Step(); // 로딩 진행도 증가.
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
	// StageClear 는 안 치고.. 안 띄울 경우 게임 진행이 안되는 건 제외한다. 성능 테스트용
	// AB2StageEventDirector::IsStageEventRelatedUI 와 공유하는 부분이 있지만 동일할 이유는 없다.
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
