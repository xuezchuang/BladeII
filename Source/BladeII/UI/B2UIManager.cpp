
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
	// CanUnloadByPolicy 甫 true 肺 父甸绢辑 Unload 妮捞 冈洒档废 窍扁 困秦.
	// 弊矾唱 捞犯霸 窍绊 Unload 甫 秦档 RootSet 捞扼搁 角龙利牢 unload 啊 登瘤绰 臼阑 巴.
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
	ScreenDebugTextWidget = NULL; // Shipping 呼靛俊辑绰 NULL 蔼阑 蜡瘤窍绊 乐绢具.

	ErrorResetPerSecond = 5;
	IsIndicatorVisible = false;
}

UB2UIManager::~UB2UIManager()
{
	// 绢恫 坷宏璃飘啊 绢恫 版肺肺 家戈磊肺 甸绢坷绰瘤 舅 荐 绝澜. 漂洒 橇肺技胶 辆丰矫 Destroy 啊 阂府搁 巩力啊 凳. 
	// 老馆利牢 惑炔俊辑狼 Destroy 绰 ABladeIIGameMode::OnPreLoadMap 俊辑 贸府.
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

	//趣矫 葛福聪鳖 窍困 困连甸狼 牧咆胶飘档 官层霖促
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
		return NULL; // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭
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
		{ // Init 篮 观俊辑 阂矾临 巴.
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
{ // 俊叼磐俊辑 Destroy 啊 力措肺 救等盲 静饭扁蔼 Instance 甫 荤侩窍霸 登绰 版快啊 乐绢辑 霖厚窃.
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
			//FString WarnMsg = FString::Printf(TEXT("促澜 %d 俺狼 WidgetData 俊 StayInViewport 啊 眉农登绢 乐嚼聪促. 捞绰 何累侩苞 滚弊甫 蜡惯且 荐 乐栏聪 林狼秦辑 荤侩秦具 钦聪促.\nUI 傈券 矫狼 喉肺欧阑 肯拳窍扁 困茄 格利捞扼搁 促澜 其捞瘤俊 家俺等 汲沥甸阑 刚历 曼绊秦 林技夸\nhttp://172.168.2.105:8090/pages/viewpage.action?pageId=4827590\n\n"), WidgetNamesWithStayInViewport.Num());
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
	// FB2UIWidgetData 狼 WidgetFName 篮 流立 技泼窍档废 窍瘤 臼绊 夯牢捞 殿废等 甘狼 虐 蔼俊辑 啊廉咳.
	for (TMap<FName, FB2UIWidgetData>::TIterator NewWidgetDataIt(AllWidgetData); NewWidgetDataIt; ++NewWidgetDataIt)
	{
		FB2UIWidgetData& ThisWidgetData = NewWidgetDataIt.Value();
		if (ThisWidgetData.WidgetFName != NewWidgetDataIt.Key())
		{
			ThisWidgetData.WidgetFName = NewWidgetDataIt.Key();
		}
	}
	for (TMap<EUIScene, FB2UISceneData>::TIterator SceneDataIt(AllSceneData); SceneDataIt; ++SceneDataIt)
	{ // SceneData 档 付蛮啊瘤肺 SceneName 阑 Map 狼 虐蔼俊辑 啊廉柯促.
		FB2UISceneData& ThisSceneData = SceneDataIt.Value();
		if (ThisSceneData.SceneName != SceneDataIt.Key())
		{
			ThisSceneData.SceneName = SceneDataIt.Key();
		}
	}

#if !PLATFORM_MAC
	FString NotFoundWidgetNameMsg = TEXT("SceneData 俊 汲沥茄 促澜 WidgetFName 捞 AllWidgetData 俊辑 惯斑登瘤 臼疽嚼聪促.\n\n");
	int32 FoundSomeInvalidCount = 0;
	for (TMap<EUIScene, FB2UISceneData>::TIterator SceneDataIt(AllSceneData); SceneDataIt; ++SceneDataIt)
	{
		FB2UISceneData& ThisSceneData = SceneDataIt.Value();
		for (FName& ThisSceneWidgetFName : ThisSceneData.WidgetFNames)
		{
			// DJLegacy 客狼 楷搬 侩栏肺 歹固 技泼茄 霸 乐栏聪 弊扒 八荤俊辑 力寇.
			if (!IsDJLegacyUIScene(ThisSceneData.SceneName) && !AllWidgetData.Find(ThisSceneWidgetFName))
			{
				NotFoundWidgetNameMsg += FString::Printf(TEXT("SceneData SceneName %d 狼 WidgetFName %s\n"), (int32)ThisSceneData.SceneName, *ThisSceneWidgetFName.ToString());
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

#if 0 // 捞吧 荤侩窍霸 登搁 搬惫 甘 肺爹 矫埃捞 辨绢瘤霸 瞪 巴捞促. 飘饭捞靛坷橇啊 疙犬窍骨肺 窜鉴 CacheOnLoad 甫 窍芭唱 利例茄 瘤痢俊 AsyncLoad 甫 窍芭唱.
void UB2UIManager::SyncPreloadTAssets(const TArray<FName>& InPreloadUIName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_PreloadTAssets);
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::PreloadTAssets"));

	for (const FName ThisPreloadName : InPreloadUIName)
	{
		FB2UIWidgetData* FoundWidgetData = GetWidgetData(ThisPreloadName);
		if (FoundWidgetData)
		{
			FoundWidgetData->bCacheOnLoad = true; // CachedWidgetClasses 俊档 某教秦 初绊 OpenUI 且 锭俊 Unload 窍瘤 臼档废.
			GetWidgetClass(FoundWidgetData); // TAsset 努贰胶 肺爹.
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
		// bAllowUIAsyncLoading 俊 蝶弗 SyncPreload 肺狼 傈券篮 咯扁急 粱 积阿秦 杭 鞘夸啊 乐促.
		// UIManager 狼 Async Load 绰 PreLoad 鸥捞怪狼 钢萍内绢 劝侩捞 格利捞 酒聪扼 牢霸烙 柳青 档吝 归弊扼款靛 肺爹捞 格利烙.
		//SyncPreloadTAssets(InLoadUIs); // 蝶扼辑 咯扁急 鞘夸且 锭 肺爹窍档废 弊成 府畔 せ
		return;
	}

	for (const FName& UIName : InLoadUIs)
	{
		if (FB2UIWidgetData* WidgetData = GetWidgetData(UIName))
		{
			// UIManager::AsyncLoading篮 1 RequestName : 1 UI Asset阑 盔蘑栏肺 茄促. 1 : 促 绰 救凳
			WidgetData->bCacheOnLoad = true; // 肺爹 肯丰 饶 某教阑 秦具 窍绰单 付农甫 蝶肺 救 沁促啊绰 促弗 率俊辑 攫肺爹阑 窍霸 瞪 荐档 乐栏聪 bCacheOnLoad 付农.
			const auto& ClassAsset = WidgetData->GetWidgetClassAsset();
			TryAsyncLoad(UIName.ToString(), TArray<FSoftObjectPath>{ClassAsset.ToSoftObjectPath()});
		}
	}
}

void UB2UIManager::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& CompleteRequestList)
{
	if (CompleteRequestList.IsValidIndex(0))	// 窍唱狼 Request俊 窍唱狼 UI父
	{
		const FSoftObjectPath& LoadedAsset = CompleteRequestList[0];
		if (UClass* LoadedClassAsset = Cast<UClass>(LoadedAsset.ResolveObject()))
		{
			const FName UIName(*CompleteRequest);
			// Async 肺爹 措惑篮 CanUnloadByPolicy 甫 眉农且 鞘夸啊 绝促. 攫力 静老瘤 葛福绰 芭聪 公炼扒 某教秦 狄.
			// 窜 夸没 矫痢俊 bCacheOnLoad 甫 技泼秦 初阑 鞘夸绰 乐促. 救 弊矾搁 促弗 芭 勤甸傅 档吝俊 攫肺爹捞 登绢 滚副 家瘤啊 乐促.
			UClass*& FoundOrAddedCache = CachedWidgetClasses.FindOrAdd(UIName);
			FoundOrAddedCache = LoadedClassAsset;
		}
	}
}

void UB2UIManager::CloseAllUIs() // 荤角惑 PreDestroy 拜栏肺 概 饭骇 攫肺靛 矫俊 阂府况具 窃.
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllUIs);
	CloseAllMsgPopups();

	ChangeUIScene(EUIScene::None);

	for (TMap<FName, FB2UIWidgetData>::TIterator WDMapIt(AllWidgetData); WDMapIt; ++WDMapIt)
	{
		FB2UIWidgetData& ThisWidgetData = WDMapIt.Value();
		if (ThisWidgetData.Widget)
		{
			// bCloseStaticUIs 甫 true 肺 逞败林绢辑 bStayInViewport 牢 巴甸档 葛滴 摧绰促.
			CloseUI(ThisWidgetData.WidgetFName, false, true);
		}
	}
}

void UB2UIManager::CloseAllStandaloneUIs()
{ // UIScene 篮 巢败滴绊 扁鸥 磊肋茄 扑诀捞唱 喊档肺 凯赴 UI 甸父 绝局妨绰 芭.
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
				continue; // 泅犁 Scene 栏肺 凯妨乐绰 扒 摧瘤 臼澜.
			}
			// bCloseStaticUIs 甫 true 肺 逞败林绢辑 bStayInViewport 牢 巴甸档 葛滴 摧绰促.
			CloseUI(ThisWidgetData.WidgetFName, false, true);
		}
	}
}

void UB2UIManager::Destroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_Destroy);
#if WITH_EDITOR
	if (GIsEditor) {
		CachedWidgetClasses.Empty(); // 俊叼磐搁 Destroy 父 蝶肺 阂赴 惑炔俊辑 犬角洒 厚快档废.
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
		if (!GIsEditor) { // 俊叼磐绰 粱 促抚. DestroySelf 磊眉啊 鞘夸绝绢 焊捞扁档.
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

// Level transition 矫 捞吧肺 clear..
void UB2UIManager::MasterDestroy()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_MasterDestroy);
	if (!GIsEditor)
	{ // 促弗 TAsset 甸篮 ManualUnloadBeforeNewBeginning 俊辑 窍绰 芭.
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
		if (!FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) //吝酶 官牢爹 救登霸
			FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &UB2UIManager::Destroy);
#endif

		MainWidget = Cast<UB2UIMain>(CreateWidget<UUserWidget>(TheWorld, this, UIMainClass));
		if (MainWidget)
		{
			MainWidget->Init();
			MainWidget->AddToViewport(BII_WIDGET_ZORDER_NEW_UI_MAIN); // Z-order 甫 蝶肺 包府窍绰 DJLegacy 客 傍粮窍绰 茄 喊档 z 蔼阑 持绰促.
			PrepareUI();
			bInit = true;
		}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
		CreateScreenDebugWidget();
#endif

		SubscribeEvents();
	}

	InitNotAddUISceneHistoryList();

	//馆措肺 夸扁俊 殿废茄芭父 
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
			//轰器飘俊 亲惑 剁况廉 乐绢具 窍绰 赤籍甸. CreateWidget阑 弊锭弊锭 窍搁辑 积扁绰 喉肺欧阑 规瘤(磊林 荤侩窍绰 赤籍父 荤侩且 巴~)
#if 0 // bStayInViewport 甫 被捞 贸澜何磐 凯瘤绰 臼绊 贸澜 楷 捞饶肺 摧瘤 臼绰 吧肺 官槽促.
			if (WidgetData->bStayInViewport)
			{
				UClass* WidgetClass = GetWidgetClass(*WidgetData);
				auto* PreCreateWidget = CreateWidget<UB2UIWidget>(GetWorld(), this, WidgetClass);

				if (PreCreateWidget)
				{
					WidgetData->Widget = PreCreateWidget;
					PreCreateWidget->MyUIFName = WidgetData->WidgetFName;
					PreCreateWidget->Init();

					//亲惑 剁况初绰 UI狼 版快,Open瞪锭 挥 酒聪扼 俩 贸澜 积己瞪锭档 DocBind狼 扁雀啊 林绢柳促. 恐衬搁 亲惑 栋乐栏聪鳖...
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
	// UIWidget 阑 侥喊磊肺 秦辑 茫栏妨搁 漏绝捞 葛滴 iteration 秦具 窍聪 捞扒 漂喊茄 版快俊唱 荤侩阑.. 
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

TSubclassOf<UB2UIWidget> UB2UIManager::GetWidgetClass(const FB2UIWidgetData* WidgetData) // 窜鉴 Get 父捞 酒囱 肺爹阑 窃.
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
			if (!WidgetData->CanUnloadByPolicy()) { // 绢恫 侥栏肺电 Cache 付农啊 乐绰 版快, 攫肺爹 规瘤 某教
				UClass*& FoundOrAddedCache = CachedWidgetClasses.FindOrAdd(WidgetFName);
				FoundOrAddedCache = WidgetClass;
			}

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 绰 皋葛府 快妨肺 牢秦 捞繁 芭 窍瘤 臼绰促..

			if (UB2UIManager::IsRootSetWidgetName(WidgetFName))
			{
				// 捞扒 酒抗 霸烙 辆丰 矫痢鳖瘤 救 郴府摆促绰 焊促 碍仿茄 狼荤 钎疙.
				// 肺爹 矫埃捞 面盒洒 辨搁辑 磊林 静捞绰 巴甸肺 急沥.
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
		// CacheOnLoad 殿狼 付农啊 等 扒 UnloadAllTAssets 鞍篮 单辑 敲贰弊甫 力芭茄 捞饶俊 捞吧 妮 秦具 茄促.
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

	// bStayInViewport 甫 被捞 贸澜何磐 凯瘤绰 臼绊 贸澜 楷 捞饶肺 摧瘤 臼绰 吧肺 官槽促.
	//if (!WidgetData->bStayInViewport)
	{
		// 20180124_YJ
		// 扁夯利栏肺 父靛绰 困连甸篮 MainWidget俊 积己鉴辑措肺 嘿咯持瘤父,
		// 距匙飘况农鞍篮 穿档己 弥惑窜俊 困摹 秦具父窍绰 扑诀狼 版快俊 MainWidget阑 公矫窍绊
		// AddToViewport 狼 Zoder 蔼阑 1000栏肺 技泼窍咯 剁况淋
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

	//坷锹 瞪 锭 磊悼官牢靛 秦具 等促搁
	if (WidgetData->bBindDocAutoOnOpen)
	{
		IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(Widget);
		if (CastDocBindable)
			CastDocBindable->BindDocAuto();
	}

	//坷锹 瞪 锭, 岿靛 喉矾甫 冈咯具 等促搁
	if (WidgetData->bAllowWorldBackgroundBlur)
	{
		Widget->bWorldBackgroundBlurOn = true;
		StartWorldBackgroundBlur(WidgetData->WorldBackgroundBlurScale);
	}

	//扁夯利栏肺绰 Visibility甫 难霖促.
	Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//坷锹 瞪 锭, 傈捧 包访 UI甫 见败具 茄促搁( bHideBattleRelatedWidgets绰 阿 困连俊辑 技泼捞 登绊, bHUDHidingCinematicMode绰 寇何俊辑 技泼捞 等促 )
	if (
		(bCinematicMode && bHUDHidingCinematicMode) ||
		(!bCinematicMode && WidgetData->bHideBattleRelatedWidgets) // 泅犁绰 CinematicMode 汲沥捞 快急烙. CinematicMode 扼搁 bHideBattleRelatedWidgets 汲沥阑 公矫.
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

	//OpenState啊 None捞搁 酒抗 凯府瘤档 臼疽栏聪 摧阑 鞘夸档 绝促.
	if (Widget->GetOpenState() == EUIOpenState::None)return;



	//UI率 楷免捞 肯丰登菌栏搁
	if (Widget->GetOpenState() == EUIOpenState::Closed)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
		Widget->OpenState = EUIOpenState::None;

		// bStayInViewport 牢 巴甸篮 bCloseStaticUIs 啊 甸绢柯 版快俊父 摧绰促. (饭骇 攫肺靛 矫痢)
		if (!WidgetData->bStayInViewport || bCloseStaticUIs)
		{
			Widget->DestroySelf(this);
			WidgetData->Widget = nullptr; // 惑窜 DestroySelf 俊辑 贸府瞪 巴.
		}

		//坷锹 瞪 锭 磊悼官牢靛 沁促搁, 磊悼 秦力
		if (WidgetData->bBindDocAutoOnOpen)
		{
			IB2UIDocBindable* CastDocBindable = Cast<IB2UIDocBindable>(Widget);
			if (CastDocBindable)
				CastDocBindable->UnbindDoc();
		}

		//坷锹 瞪 锭, 岿靛 喉矾甫 冈看促搁 秦力
		if (WidgetData->bAllowWorldBackgroundBlur && Widget->bWorldBackgroundBlurOn)
		{
			Widget->bWorldBackgroundBlurOn = false;
			StopWorldBackgroundBlur();
		}

		//坷锹 瞪 锭, 傈捧 包访 UI甫 见变 巴阑 汗备 秦具 茄促搁.
		// 泅 备己篮 bHideBattleRelatedWidgets 汲沥等 巴捞 咯矾俺 凯妨乐阑 锭 巩力啊 瞪 巴捞促. 弊繁 版快俊 措茄 措贸啊 鞘夸窍促搁 惑怕甫 蝶肺 历厘秦 笛 鞘夸啊 乐促.
		// 弊矾唱 CinematicMode 汲沥档 鞍捞 绊妨且 鞘夸啊 乐阑 巴.
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
{ // UIDataCacheHistory 扁瓷 皋牢
	if (UIDataCacheHistoryNum > 0 && InJustOpenedUIData
		// 风飘悸 贸府甫 窍绰 扒 咯扁俊 器窃矫虐瘤 臼绰促... bCacheOnLoad 付农等 巴档 公矫秦 滚副鳖..
		&& !IsRootSetWidgetName(InJustOpenedUIData->WidgetFName)
		&& !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 俊辑 捞吧 窍妨搁 IsRootSetWidgetName 阑 公矫窍带瘤, 酒丛 弊 扁瓷档 鞍捞 荤侩窍带瘤..
		)
	{
		const FName JustOpenedUIName = InJustOpenedUIData->WidgetFName;
		FName PoppedUIName = NAME_None; // 捞锅 霸 history 俊 眠啊登绰 巴栏肺 牢秦 狐瘤绰 霸 乐促搁 捞抚捞 甸绢啊霸 瞪 巴.

		int32 ExistingIndex = UIDataCacheHistory.Find(JustOpenedUIName);
		if (ExistingIndex == INDEX_NONE)
		{
			// 扁粮俊 府胶飘俊 绝促搁 货肺 眠啊啊 瞪 巴捞聪 弊 傈俊 history 啊 父顶牢 惑怕搁 盖 菊俊 吧 刚历 哗绊..
			if (UIDataCacheHistory.Num() >= UIDataCacheHistoryNum)
			{
				PoppedUIName = UIDataCacheHistory[0];
				UIDataCacheHistory.RemoveAt(0);
			}
		}
		else
		{
			// 扁粮俊 府胶飘俊 乐促搁 老窜 猾促.
			UIDataCacheHistory.RemoveAt(ExistingIndex);
		}

		// 府胶飘狼 力老 第俊 眠啊.
		UIDataCacheHistory.Add(JustOpenedUIName);

		// bCacheByHistory 眉农.
		InJustOpenedUIData->bCacheByHistory = true;

		if (PoppedUIName != NAME_None)
		{
			// 捞锅俊 狐柳 巴俊 措茄 unload 贸府. 
			// ResetAllCacheMarkForUnload 甫 窍绰 霸 酒聪扼 bCacheByHistory 父 府悸茄促.
			// bCacheByHistory 客 喊俺肺 bCacheOnLoad 啊 技泼登绢 乐促搁 咯傈洒 Unload 甫 窍瘤 臼澜.
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
		|| gbHACKInGameUIHiddenByDevCommand // 俺惯侩 目盖靛肺 见变 版快.		
#endif
		)
	{
		// 捞霸 惑炔俊 蝶扼 肯寒洒 见败林瘤绰 臼绰单 gbHACKInGameUIHiddenByDevCommand 甫 曼绊. 
		// 酒付档 WorldBackgroundBlur 甫 静绰 惑炔俊急 弊巴鳖瘤 皑救且 鞘夸啊 绝瘤 臼阑鳖 窃.
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

	// 泅 备泅栏肺绰 咯扁辑 CinematicMode 唱哎 锭 泅犁 凯妨乐绰 Widget 狼 WidgetData 俊 bHideBattleRelatedWidgets 啊 汲沥登绢 乐绢档 促矫 焊捞霸 瞪 巴.
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

// 茄锅 荤侩茄 俊矾内靛绰 5檬悼救 荤侩且 荐 绝澜
// NoCheckErrorCode俊 抗寇 殿废 且 荐 乐澜
void UB2UIManager::CheckErrorTimeStamp(float DeltaTime)
{
	static float CheckTime = 0.0f;
	CheckTime += DeltaTime;

	// 0.5檬俊 茄锅 眉农
	if (CheckTime > 0.5f)
	{
		const int64 ErrorCoolTime = ETimespan::TicksPerSecond * ErrorResetPerSecond; // 5檬付促 府悸
		const int64 CurrentUTCTime = UB2GameInstance::GetUTCNow().GetTicks();
		CheckTime = 0.0f;

		for (int32 i = 0; i < ErrorTimeStamp.Num(); ++i)
		{
			// 扑诀矫埃 + 酿鸥烙 < 泅犁矫埃
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
	//[@AKI, 180111] 菊栏肺 箭磊 持阑锭 酒贰 林家 曼绊窍咯 康巩 持阑 巴
	//http://172.168.2.106/result_codes.html
	//////////////////////////////////////////////////////////////////////////////
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_OpenMsgPopupFromErrorCode);

	if (CheckErrorCodeContains(nErrorCode)) // 眉农窍瘤 臼绰 Errorcode绰 官肺 角青(惑痢 殿)
	{
		if (ErrorCodeContains(nErrorCode)) // 淬败 乐栏搁 return
			return;

		SetErrorTimeStamp(nErrorCode); // 殿废
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
	case B2ResultCode::FAILURE_SOCIALPOINT_UNDERFLOW:	//FAILURE_SOCIALPOINT_UNDERFLOW		SocialPoint is zero.//惑痢俊辑 快沥器牢飘 何练且锭 或阑 版快
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
		// EUIMsgPopup::Simple 捞侩且订 俊矾内靛锅龋档 焊咯淋

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

		//OpenUI肺 凯菌栏搁, CloseUI肺 摧酒具瘤... 唱吝俊 扑诀捞 n俺 般媚辑 栋具 瞪 荐 乐栏聪 OpenUI啊 酒囱 促弗 风飘肺 剁快绰 累诀捞 鞘夸且 淀
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
			// 匙飘亏 谗辫 包访 扑诀篮 瘤快瘤 臼澜
			if (MsgPopup.Value->GetPriority() == InIsPriority)
			{
				return true;
			}
		}
	}
	return false;
}

void UB2UIManager::CloseAllMsgPopups(bool bRightNow)
{ // CloseMsgPopup 俊 持绢临 ID 唱 MsgPopup 饭欺繁胶 荤侩捞 阂啊且 版快 绢驴 荐 绝捞 荤侩. 趣篮 扁鸥 促弗 漂荐茄 版快.
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseAllMsgPopups);

	for (auto MsgPopup : MsgPopupMap)
	{
		if (MsgPopup.Value)
		{
			// 匙飘亏 谗辫 包访 扑诀篮 瘤快瘤 臼澜
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
	//	|| (CheckMailReward && b2network::B2RewardPushType::MAIL != BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardItem->id))) //  皋老窃 or 傍侩窃 眉农
	//	return nullptr;

	//if (RewardItem == nullptr
	//	|| (CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardItem->id))
	//	)//  皋老窃 or 傍侩窃 眉农
	//	return nullptr;

	//return UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);

	return RewardItem ? OpenRewardMailPopUp(RewardItem->raw_reward->id, CheckMailReward) : nullptr;
}

// 焊惑阑 皋老肺 罐栏搁 true, 傍侩窃栏肺 罐栏搁 false
//  BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardItem[0]->id)) 捞 何盒篮 泅犁 扁裙俊辑 咯矾 酒捞袍阑 罐阑锭, 葛滴 傍侩窃 or 葛滴 皋老肺 胶农赋飘 累己阑 窍扁肺沁澜. 2017/07/12
//  弊贰辑 0锅掳 牢郸胶父 八荤
//  饶俊 荐沥等促搁 促 倒搁辑 眉农
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
	if ((CheckMailReward && b2network::B2RewardPushType::MAIL != BladeIIGameImpl::GetClientDataStore().GetRewardPushType(RewardId))) //  皋老窃 or 傍侩窃 眉农
		return nullptr;

	if ((CheckMailReward && !CheckRewardPushType(b2network::B2RewardPushType::MAIL, RewardId))
		)//  皋老窃 or 傍侩窃 眉农
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
				if (ParentWidget == nullptr) // 何葛 绝栏搁 霉锅掳 AlarmWidget阑 官肺 府畔
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

	//捞固 凯妨 乐绰 UIScene栏肺 眉牢瘤绰 救等促.
	if (SceneName == CurrUIScene) return;

	B2AndroidBackManager::GetInstance()->RemoveAllPopup();

	//捞固 ChangeUIScene捞 悼累 吝捞扼绰 舵牢单, 肚 Change啊 阂赴芭促. 老窜 傈饶荤沥 毫林瘤 富绊 悼累吝牢 UIScene篮 傲 摧绰吧肺.
	if (bClosingUIScene || bOpeningUIScene)
	{
		ReservedUISceneOpenData.Key = EUIScene::None;

		if (ClosingUIScene != EUIScene::None)
			CloseUIScene(ClosingUIScene, true, DontCloseSameUIAtReservedUIScene);
		if (OpeningUIScene != EUIScene::None)
			CloseUIScene(OpeningUIScene, true, DontCloseSameUIAtReservedUIScene);

		if (ClosingUIScene == EUIScene::None && OpeningUIScene == EUIScene::None && CurrUIScene != EUIScene::None) //RightNow看带 惑炔. ChangeUIScene 肺流捞 捞惑窍促..
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
	{ // 喊档 矫胶袍牢 DJLagacy 其捞瘤啊 趣 摧洒瘤 臼疽阑 荐 乐澜.
		DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_End);
	}

	//摧扁父 窍绊, Open篮 窍瘤 臼绰促.
	if (SceneName == EUIScene::None)
	{
		CloseUIScene(CurrUIScene, TransType == EUIChangeUISceneTransitionType::CANCEL_CLOSE || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL, false);
		CurrUIScene = EUIScene::None;
		return;
	}

	//泅犁 酒公 UIScene档 酒囱版快, Close绝捞 Open官肺
	if (CurrUIScene == EUIScene::None)
	{
		OpenUIScene(SceneName, UISceneOnOpenDelegate, TransType == EUIChangeUISceneTransitionType::CANCEL_OPEN || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL, false);
		return;
	}

	//瘤陛 UIScene阑 Tick俊辑 酒囱 官肺 摧酒具 登绰啊?
	bool CloseCurUISceneRightNow = (TransType == EUIChangeUISceneTransitionType::CANCEL_CLOSE || TransType == EUIChangeUISceneTransitionType::CANCEL_ALL);

	//1. 傈券 楷免 鸥涝 历厘
	ChangeUISceneTransitionType = TransType;
	//2. 抗距UIScene 瘤沥
	ReservedUISceneOpenData.Key = SceneName;
	ReservedUISceneOpenData.Value = UISceneOnOpenDelegate;
	bDontCloseSameUIAtReservedUIScene = DontCloseSameUIAtReservedUIScene;
	//3. 瘤陛UIScene 摧扁
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
		bOpeningDJLegacyHeroMgmtBySceneHistory = true; // 夸扒 LobbyEnterHeroMgmtModeClass 俊 牢磊肺 持绢林绰 霸 彬阐且 荐档.. 官层具 且 镑捞 粱 腹促.

													   // 捞 UIScene 甸篮 角力 捞 矫胶袍俊辑 荤侩登瘤 臼绰促. 角力肺绰 DJLegacy 矫胶袍捞绊 弊率苞狼 楷搬绊府肺辑 巢败笛 挥.
													   // 芭狼 滴 矫胶袍 荤捞啊 楷搬登绰 归 滚瓢 累悼阑 困窃.
		OpenDJLegacyHeroMgmtPageForUIScene(TargetBackUIScene);

		bOpeningDJLegacyHeroMgmtBySceneHistory = false;
	}
	else
	{
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(TargetBackUIScene);

		// 归滚瓢阑 喘矾 Mail UI 肺 倒酒棵 版快 LinkManager甫 烹秦 倒酒柯 巴捞 酒聪扁 锭巩俊 府胶飘 诀单捞飘啊 捞风绢瘤瘤 臼栏哥 流立 诀单捞飘 秦林绢具 茄促.
		if (TargetBackUIScene == EUIScene::Mail)
		{
			MailTabIndex RequestTab = MailTabIndex::EMailTab_Gift;
			data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(RequestTab));
		}
	}

	// DJLegacy 牢 版快绰 咯扁辑 UISceneHistory 力芭 寇俊 角龙利栏肺 购啊绰 窍瘤绰 臼酒具.
	ChangeUIScene(TargetBackUIScene, TransType, UISceneOnOpenDelegate, DontCloseSameUIAtReservedUIScene, bPassBattleUIScene);
}

EUIScene UB2UIManager::GetPrevUIScene()
{
	return (UISceneHistory.Num() > 1) ? UISceneHistory[UISceneHistory.Num() - 2].Key : EUIScene::None;
}

//UI纠 洒胶配府俊 绵利登绢 乐绰 纠甸吝俊 傈捧俊 包拌等 纠捞 乐栏搁 扒呈 堕锭 荤侩
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
				if (CurSceneData && CurSceneData->IsBattleRelatedScene) // DJLegacy 客狼 龋券阑 困秦 持绢敌 歹固 纠 侥喊磊狼 版快 角力 SceneData 绰 绝阑 荐 乐促.
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
	// Artifical Add 扼绰 巴篮 角力 Open 绝捞 窜瘤 history 瞒盔俊辑 持绰 侩档甫 舵窃.
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

	//抗距 UIScene捞 捞傈 洒胶配府客 鞍促搁, 洒胶配府俊辑 扑矫虐绊 酒聪搁 洒胶配府俊 绵利茄促.
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

		// 洒胶配府 歹窍扁 傈俊.. 康旷包府 葛靛 埃狼 流立利牢 傈券 矫俊绰 付瘤阜 洒胶配府父 蜡瘤窍档废 茄促.
		// 捞扒 DJLegacy 矫胶袍苞狼 楷拌 巩力 锭巩篮 酒聪绊 康旷包府 UI 牢磐其捞胶 惑 漂隆 锭巩牢 吧肺..
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
					// 绊措蜡拱概聪隆其捞瘤俊辑 绊措蜡拱肺 倒酒哎矫俊父 抗寇..
					// 窜鉴 绊措蜡拱概聪隆其捞瘤->绊措蜡拱肺 倒酒哎锭俊绰 碍力 洒胶配府 昏力甫 窍瘤臼绊
					// 鸥其捞瘤->绊措蜡拱概聪隆其捞瘤 老锭绰 绊措蜡拱档 鞍捞 瘤况霖促.
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

	//for (auto& WidgetData : WidgetDatas) 混府霸 登搁 AllWidgetData 肺 背眉
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
				// 捞傈 脚俊乐栏搁 货肺救凯澜.. 父距 巩力瞪芭鞍栏搁 弊成 摧绊货肺凯绢滴 瞪芭鞍澜
				// 摧绰扒 酒贰 CloseUIScene 俊辑
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

	//Doc俊 CurUIScene阑 持绢林绊, UI甸俊辑 CurUIScene俊 吧嘎绰 贸府甫 秦具 等促搁 贸府甫 窍霸 秦霖促.
	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
		DocUICondition->SetCurrUIScene(CurrUIScene);

	OnChangUISceneClass<EUIScene>::GetInstance().Signal(CurrUIScene);
}

void UB2UIManager::CloseUIScene(EUIScene SceneName, bool RightNow, bool DontCloseSameUIAtReservedUIScene)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIManager_CloseUIScene);
	//付欧窍绊 Tick俊辑 贸府
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
			//抗距等 UIScene俊 泅犁 UI啊 咯傈洒 器窃登绢 乐促搁 摧瘤 臼绰促.
			if (DontCloseSameUIAtReservedUIScene &&  ReservedSceneData && (ReservedSceneData->WidgetFNames.Contains(FoundWidgetData->WidgetFName)))
				continue;

			CloseUI(FoundWidgetData->WidgetFName, RightNow);

			bClosedSome = true;
		}
	}

	// UIScene 傈券矫付促 碍力 GC 倒府扁.
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
	// 己瓷俊 奴 康氢阑 固磨 家瘤啊 乐绢辑 捞 内靛绰 厚劝己拳 矫难 初嚼聪促.
	// 趣矫 捞傈俊 荤侩沁带 镑捞 乐阑瘤 隔扼 曼绊侩栏肺 出涤聪促.
	// UI 其捞瘤 咯摧绰 楷免篮 俺喊利栏肺 备泅阑..
#if 0
	//UIScene捞 摧洒绰 吝捞搁
	auto CurSceneData = GetUISceneData(CurrUIScene);
	auto ReservedSceneData = GetUISceneData(ReservedUISceneOpenData.Key);

	if (!CurSceneData) return;

	if (bClosingUIScene)
	{
		bool AllReadyClose = true;

		for (auto& WidgetData : WidgetDatas)  捞芭 促矫 混府霸 登搁 AllWidgetData 甫 荤侩窍档废 秦具.
		{
			if (WidgetData.Widget)
			{
				//抗距等 UIScene俊 UI啊 咯傈洒 器窃登绢 乐促搁 摧瘤 臼绰促.
				if (bDontCloseSameUIAtReservedUIScene && ReservedSceneData && (ReservedSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)))
					continue;

				if ((CurSceneData->WidgetFNames.Contains(WidgetData.WidgetFName)) && WidgetData.Widget->GetOpenState() != EUIOpenState::Closed && WidgetData.Widget->GetOpenState() != EUIOpenState::None)
				{
					AllReadyClose = false;
					break;
				}
			}
		}


		//摧洒绰 楷免捞 场车促 促澜 UIScene阑 凯绢拎.
		if (AllReadyClose)
		{
			if (ChangeUISceneTransitionType != EUIChangeUISceneTransitionType::PREOPEN)
				OpenNextUIScene();
		}
	}

	//UIScene捞 凯府绰 吝捞搁
	if (bOpeningUIScene)
	{
		bool AllReadyOpen = true;

		for (auto& WidgetData : WidgetDatas) 捞芭 促矫 混府霸 登搁 AllWidgetData 甫 荤侩窍档废 秦具.
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

		//凯府绰 楷免捞 场车促.
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
		//Doc俊 CurUIScene阑 持绢林绊, UI甸俊辑 CurUIScene俊 吧嘎绰 贸府甫 秦具 等促搁 贸府甫 窍霸 秦霖促.
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
	//UISceneHistory甫 荤侩茄 第肺啊扁 滚瓢 荤侩 第肺 哎矫 酒捞袍 搬苞芒栏肺 哎 老 例措 绝澜
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

		// Unload 救登霸 技泼沁带 芭 府悸. 溜 CacheOnLoad 唱 CacheByHistory 技泼等 巴甸档 UnloadAll 俊辑绰 攫肺爹 等促. RootSet 牢 版快绰 咯窍埃 康氢阑 罐瘤 臼摆瘤父.
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
	// InfoAsset 俊 殿废登绰 局甸捞 倔付唱 登绰瘤俊 蝶扼 利例洒 沥窍绰 芭. 割俺 element 肺爹付促 茄锅究 诀单捞飘 且 瘤.
	const int32 ProgUpdateInterval = 5;
	const int32 TotalProgUpdateCount = AllWidgetData.Num() / ProgUpdateInterval;
	const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	float CurrProg = InStartProgress;

	int32 CurrLoadingWidgetDataIndex = 0; // 窜瘤 肺爹 柳青档 钎矫甫 困茄 牢郸胶 墨款飘 せ
	for (TMap<FName, FB2UIWidgetData>::TIterator WidgetDataIt(AllWidgetData); WidgetDataIt; ++WidgetDataIt)
	{
		//////////////////////////////
		// 琴缴篮 夸芭绊 唱赣瘤 progress 拌魂篮 窜瘤 荤练.
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
	CachedWidgetClasses.Empty(); // 犬角洒 秦 林绰 霸 亮瘤..
}
void UB2UIManager::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	CleanupOnPreSave();
}
#endif

void UB2UIManager::GetAllCreatedUINames(TArray<FName>& OutUINames)
{ // 叼滚弊 犬牢 侩档 殿..
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
{ // 叼滚弊 犬牢 侩档 殿..
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
			if (WidgetData.WidgetFName == UIFName::Header) continue; // 庆歹力寇
			if (IgnoreWidgetName.Contains(WidgetData.WidgetFName)) continue; // 眠啊 力寇 困连

																			 // 促弗 困连捞 泅犁 焊咯瘤绊 乐阑版快
			if (WidgetData.Widget->GetVisibility() != ESlateVisibility::Hidden && WidgetData.Widget->GetVisibility() != ESlateVisibility::Collapsed) return true;
		}
	}
	return false;
}

TArray<FName> UB2UIManager::WidgetNamesForRootSet;
TArray<FName> UB2UIManager::WidgetNamesForLoadOnStartup;
bool UB2UIManager::IsRootSetWidgetName(FName InUIName)
{
	// WidgetNamesForRootSet 荤捞令啊 目瘤搁 TMap 捞 炒摆瘤父 弊犯霸 目瘤绰 扒 促弗 捞蜡俊辑扼档 帮鄂窍促..
	return (WidgetNamesForRootSet.Contains(InUIName) || WidgetNamesForLoadOnStartup.Contains(InUIName));
}

void UB2UIManager::SetupCacheOnLoadList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 皋葛府 快妨肺 牢秦..
#if WITH_EDITOR
	if (GIsEditor) {
		return; // 俊叼磐档 捞繁 芭 鞘夸绝澜.
	}
#endif

	// 绢痘霸 窍促 焊聪鳖 捞扒 Rootset 府胶飘客绰 崔府 概锅 Init 矫俊 八荤秦辑 敲贰弊俊 付农甫 秦 林绰 侥捞 凳. ぱぱ;;

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
		FB2UIWidgetData* FoundWidgetData = GetWidgetData(ThisCacheOnLoadName); // 角力 UI 单捞磐 肺爹捞 老绢唱绰 芭 酒丛.
		if (FoundWidgetData)
		{
			// 搬惫 夸芭 窍唱 技泼窍磊绊 捞 窿阑 窍绰 扒单 捞霸 恐 UIManager 俊辑 技泼窍绰 UPROPERTY 啊 酒聪绊 ini 俊辑 佬绢甸捞绰 蔼牢瘤 狼酒且 荐 乐促.
			// 捞蜡绰 UIWidgetData 俊 技泼窍绰 吧 疵府绊 酵瘤档 臼疽绊 牧刨明 瞒盔狼 单捞磐扼扁焊促绰 矫胶袍利牢 单捞磐扼 捞繁 巴档 乐绊 殿..
			// 弊府绊 荤角 捞吧肺父 技泼登绰 蔼档 酒聪绊 AsyncLoad 甫 茄 版快俊档 捞霸 技泼捞 等促.
			// 捞芭尔 厚搅茄 烩狼 StayInViewport 绰 贸澜何磐 甸绢啊 乐带 扁瓷捞扼 UPROPERTY 甫 蜡瘤窍绊 乐绰单 荤角 弊霸 歹 困氰窍绊 弊巴档 UIWidgetData 俊辑 哗绊 酵澜.
			FoundWidgetData->bCacheOnLoad = true;
		}
	}
#endif
}
void UB2UIManager::SetupRootsetWidgetList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 绰 皋葛府 快妨肺 牢秦 捞繁 芭 窍瘤 臼绰促..
	if (GConfig && WidgetNamesForRootSet.Num() == 0)
	{ // 漂喊洒 急喊等 UIFName 府胶飘.. static 函荐扼 茄锅父 盲框.
		TArray<FString> ParsedWidgetNames;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("RootSetOnLoadUI"), ParsedWidgetNames, GGameIni);
		for (const FString& ThisName : ParsedWidgetNames)
		{
			WidgetNamesForRootSet.Add(FName(*ThisName)); // 老窜 府胶飘 持绢初扁父 窍绊 角力 利侩篮 阿 UI 肺爹 矫 
		}
	}
#endif
}
void UB2UIManager::SetupLoadOnStartupWidgetList()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 绰 皋葛府 快妨肺 牢秦 捞繁 芭 窍瘤 臼绰促..
	return;
#endif


	if (GConfig && WidgetNamesForLoadOnStartup.Num() == 0)
	{
		// 漂喊洒 急喊等 UIFName 府胶飘.. static 函荐扼 茄锅父 盲框.

		B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::SetupLoadOnStartupWidgetList"));
		TArray<FString> ParsedWidgetNames;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("LoadOnStartupUI"), ParsedWidgetNames, GGameIni);
		for (const FString& ThisName : ParsedWidgetNames)
		{
			WidgetNamesForLoadOnStartup.Add(FName(*ThisName));
		}

		// 霉 肺厚 肺爹 progress 俊辑绰 捞吧 拌魂俊 持绢具 茄促.
		if (WidgetNamesForLoadOnStartup.Num() > 0) {
			B2GMLoadingProgCollector::SetStepRate(0.4f / (float)WidgetNamesForLoadOnStartup.Num());
		}

		// 府胶飘 持绢初绊急 梅何磐 肺爹茄促. 贸澜 肺厚 肺爹 矫痢捞 登摆瘤..
		for (FName& ThisName : WidgetNamesForLoadOnStartup)
		{
			const FB2UIWidgetData* DataToLoad = GetWidgetData(ThisName);
			GetWidgetClass(DataToLoad);
			B2GMLoadingProgCollector::Step(); // 肺爹 柳青档 刘啊.
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
	// StageClear 绰 救 摹绊.. 救 剁匡 版快 霸烙 柳青捞 救登绰 扒 力寇茄促. 己瓷 抛胶飘侩
	// AB2StageEventDirector::IsStageEventRelatedUI 客 傍蜡窍绰 何盒捞 乐瘤父 悼老且 捞蜡绰 绝促.
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
