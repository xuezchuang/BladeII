// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "B2DLCFrontGameMode.h"
#include "BladeIIGameMode.h"
#include "Runtime/Engine/Public/Engine.h"
#if WITH_EDITOR
#include "Editor.h"
#endif
#include "Event.h"
#include "B2UIDLCFrontBase.h"
//#include "B2UIDLCFrontMain.h"
#include "B2PreRenderer.h"
#include "B2PreRenderGameMode.h"
//#include "b2"
#include "B2LobbySceneManager.h"
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
#include "B2UIDummyDLCFront.h"
#endif
#include "BladeIIScalabilityControl.h"
#include "../BladeII/BladeIIUtil.h"

//#include "B2UIDLCFrontSimplePopup.h"

////////////////////////////////////////////////////////////////////////////////
//
// 霸烙 肺厚 柳涝 傈 霸烙 牧刨明甫 (DLC) 促款肺靛 罐绰 橇沸飘 备泅俊 荤侩
// 捞 霸烙葛靛甫 角青窍扁 困茄 葛电 府家胶甸篮 力茄等 农扁狼 窜老 apk 救俊 甸绢啊具 窃.
// 烹惑利牢 InfoAsset 肺爹篮 捞 霸烙葛靛啊 栋 乐绰 悼救 肺爹捞 倾侩登瘤 臼栏哥 捞寇俊档 腹篮 BladeII 扁瓷甸狼 荤侩捞 阂啊瓷且 荐 乐促.
// 弥家茄狼 蜡瓶 沥档父 傍蜡啊 啊瓷且 巴捞促.
// 弊俊 嘎霸 咯扁辑 力傍窍绰 扁瓷 裹困档 力茄凳.
//
////////////////////////////////////////////////////////////////////////////////

FString AB2DLCFrontGameMode::SelectedFrontTitleMovieName; // 角青 饶 霉 鸥捞撇 拳搁俊辑 技泼. DLCFrontGameMode 甫 哈绢抄 捞饶俊档 鞘夸且 巴.
EPCClass AB2DLCFrontGameMode::SelectedFrontTitleMovieClass = EPCClass::EPC_End;

AB2DLCFrontGameMode::AB2DLCFrontGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	bEventsSubscribed = false;

	DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn 阑 扁夯栏肺
	PlayerControllerClass = APlayerController::StaticClass(); // 鞘夸俊 蝶扼 蝶肺 extend 秦辑 荤侩..
	
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &AB2DLCFrontGameMode::OnPreLoadMap);
	}	
}

void AB2DLCFrontGameMode::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	EditorCheckForWidgetNameIntegrity();
#endif
}

void AB2DLCFrontGameMode::StartPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::StartPlay"));
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}
	
	// 酒贰 笛 鉴辑 林狼
	InitializeAllGameSettingData(this);
	B2Scalability::OnGameModeStartPlay(this);

	PreBeginPlay(); // ABladeIIGameMode::PreBeginPlay 客 付蛮啊瘤 鸥捞怪栏肺.

	Super::StartPlay();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 沥侥栏肺 DLCFrontMap 阑 烹苞沁澜阑 付农秦辑 还窿 规瘤. 利绢档 葛官老 硅器俊辑绰 捞 霸烙葛靛甫 烹苞窍绰 惑炔篮 角力 DLC 葛靛老 巴捞促.
	UB2UIDummyDLCFront::bWentThroughRealDLCFront = true;
#endif
}

void AB2DLCFrontGameMode::PreBeginPlay()
{
	GMinimalDLCFrontMode = true; // 角青 档吝俊绰 捞扒 扁夯 true 烙. InfoAsset 肺爹阑 阜澜.

#if WITH_EDITOR // BladeIIGameMode 肺 茄锅捞扼档 敲饭捞 饶俊 累悼 犬牢阑 窍瘤 臼档废 版绊 沥档 瞒盔俊辑父 持绢初澜. 抛胶飘甫 阜阑 鞘夸鳖柳 绝摆瘤父 悼累捞 粱 促甫 荐 乐栏骨肺 版绊绰 茄促.
	ensureMsgf(GIsEditor || !ABladeIIGameMode::bDevCheckBladeIIGameModeEnteredOnce, TEXT("DLCFrontGameMode must be the First GameMode before any BladeIIGameMode is played!"));
#endif


	SubscribeEvents();
}

void AB2DLCFrontGameMode::GmaeModeGotoTitle()
{
	AB2DLCFrontGameMode::LaunchBladeIIGame();
}

void AB2DLCFrontGameMode::OpenUIScene(EDLCFrontUIName InUIName, bool bCloseOtherUIs)
{
	if (bCloseOtherUIs)
	{
		CloseAllUIs();
	}

	OpenUI(InUIName);
}

void AB2DLCFrontGameMode::SetSelectedFrontTitleMovieInfo(const FString& InMovieName, EPCClass InMovieRepresentingClass)
{ // 鸥捞撇 拳搁俊辑 罚待 急琶等 芭 捞抚 历厘
	SelectedFrontTitleMovieName = InMovieName;
	check(!SelectedFrontTitleMovieName.EndsWith(TEXT(".mp4"))); // 攫府倔 GameMoviePlayer 扁瓷俊辑 荤侩窍妨搁 犬厘磊啊 狐廉 乐绢具 窃.

	SelectedFrontTitleMovieClass = InMovieRepresentingClass;
	check(SelectedFrontTitleMovieClass != EPCClass::EPC_End);
}
void AB2DLCFrontGameMode::ClearSelectedFrontTitleMovieInfo()
{
	SelectedFrontTitleMovieName.Empty();
	SelectedFrontTitleMovieClass = EPCClass::EPC_End;

	UE_LOG(LogBladeII, Log, TEXT("SelectedFrontTitleMovieInfo is cleared"));
}
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void AB2DLCFrontGameMode::DevSetFrontTitleMovieClassOnly(EPCClass InClass)
{ // 沥侥 扁瓷俊辑 鸥捞撇 努贰胶 瘤沥篮 B2DLCFrontGameMode 啊 栋 乐绰 悼救俊父 倾侩等促. 俺惯 滚傈 抛胶飘俊辑档 绢蠢 沥档 甸绢啊 乐栏搁 措帆 扁瓷阑 蝶扼且 荐 乐绢辑 凯绢狄.
	SelectedFrontTitleMovieClass = InClass;
}
#endif

void AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie"));

	//// DLCFront 霉 Title 俊辑 罚待 急琶等 悼康惑阑 肺爹 拳搁俊 剁快妨绰 芭.

	//IB2GameLoadingScreenModule* const LoadingScreenModule = GetB2LoadingScreenModulePtr();
	//if (LoadingScreenModule != nullptr)
	//{
	//	if (!SelectedFrontTitleMovieName.IsEmpty()) 
	//	{
	//		int32 ViewSizeX = 1280, ViewSizeY = 720;
	//		APlayerController* LocalPC = Cast<APlayerController>(UGameplayStatics::GetLocalPlayerController(WorldContextObject));
	//		if (LocalPC)
	//		{
	//			LocalPC->GetViewportSize(ViewSizeX, ViewSizeY);
	//		}
	//		const bool bUseShorterScreenLayout = ShouldUseShorterScreenLoadingImageLayout(FVector2D((float)ViewSizeX, (float)ViewSizeY));

	//		// 老馆利牢 胶抛捞瘤 坷橇醋捞唱 努肺隆 康惑苞绰 己拜捞 粱 促福促. 弊成 肺爹拳搁牢单 风俏 悼康惑牢芭..
	//		// 扁夯 荤侩窍绊绰 粱 促福骨肺 颇扼固磐甫 林狼秦辑..
	//		TArray<B2MovieSubtitle>* LoadingSubtitle = LoadingScreenModule->StartGameLoadingScreen_Movie(
	//			*SelectedFrontTitleMovieName,
	//			// 老馆利牢 拳搁 厚啦俊辑绰 悼康惑阑 拳搁 观栏肺 疵妨 啊搁辑 沥秦柳 厚啦 蜡瘤.
	//			bUseShorterScreenLayout ? ELoadingScreenMovieAlignMode::FitToMin_WideHD : ELoadingScreenMovieAlignMode::FitToMax_WideHD, 
	//			false, true, true, EMoviePlaybackType::MT_Looped, true);
	//		if (LoadingSubtitle)
	//		{
	//			// 府畔蔼 MovieSubtitles array 绰 咯扁辑 盲况 淋
	//			B2MovieSubtitle LoadingScreenMsg;
	//			// Subtitle 侩 牢磐其捞胶甫 静瘤父 咯扁急 肺爹 皋矫瘤肺 静妨绰 芭. Time 裹困 面盒窍霸.
	//			LoadingScreenMsg.StartTime = 0.0f;
	//			LoadingScreenMsg.EndTime = 100.0f;
	//			LoadingScreenMsg.Color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//			LoadingScreenMsg.Text = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type01"));
	//			LoadingSubtitle->Add(LoadingScreenMsg);
	//		}
	//	}
	//	else {
	//		// 狼档窍瘤 臼篮 版快啊 瞪 搏单 LoadingImageName 栏肺 null 阑 逞败林搁 救俊辑 fallback 捞固瘤肺 悼累且 巴烙.
	//		// DLC 酒囱 俺惯滚傈捞搁 捞犯霸 瞪 淀.
	//		LoadingScreenModule->StartGameLoadingScreen_Image(nullptr, true);
	//	}
	//}
}

void AB2DLCFrontGameMode::BeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::BeginPlay"));
	
	//Super::BeginPlay();


	//APlayerController* LocalPC = UGameplayStatics::GetLocalPlayerController(this);
	//if (LocalPC) {
	//	LocalPC->ActivateTouchInterface(nullptr); // Virtual Joystick 绝局绰 芭
	//	LocalPC->bShowMouseCursor = true;
	//}


	//// 夸扒 沥侥 内靛绰 酒聪绊.. 惑技 备泅篮 夸备 荤亲俊 蝶扼..
	//OpenUI(EDLCFrontUIName::EDLCUI_ActionLogo);
}

void AB2DLCFrontGameMode::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::BeginDestroy"));

	OnDestroyCommon();

	Super::BeginDestroy();
}

void AB2DLCFrontGameMode::Destroyed()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::Destroyed"));

	OnDestroyCommon();

	Super::Destroyed();
}

void AB2DLCFrontGameMode::OnDestroyCommon()
{
	if (bEventsSubscribed)
	{
		UnsubscribeEvents();
	}

	CloseAllUIs();

	GMinimalDLCFrontMode = false; // 沥侥 霸烙 角青 档吝 捞霸 促矫 true 肺 倒酒啊急 救 凳.
}

void AB2DLCFrontGameMode::OnPreLoadMap(const FString& InString)
{
	//
	// 捞抚篮 PreLoadMap 捞扼 捞锅 霸烙葛靛 矫累 流傈栏肺 坷秦且 荐 乐瘤父 角力肺绰 磊扁 磊脚篮 郴妨哎 霖厚甫 窍绰 何盒.
	// 甘 傈券 (肺厚 柳涝) 流傈 DLCFront World GC 登扁 捞傈 公攫啊 沥府窍扁 亮篮 胶铺烙.
	//

	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::OnPreLoadMap"));

	// 肺厚肺 甸绢啊绰 肺爹 拳搁阑 剁款促..
	SetupLoadingScreen_Lobby();
}

void AB2DLCFrontGameMode::SubscribeEvents()
{
	//if (bEventsSubscribed)
	//	return;

	//CAPTURE_UOBJECT(AB2DLCFrontGameMode);
	//
	//DLCFrontLaunchBladeIIGameTicket = DLCFrontLaunchBladeIIGameClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT()
	//		Capture->LaunchBladeIIGame();
	//	END_CAPTURE_OBJECT()
	//	);
	//	
	//// 鞘夸茄 Event 啊 乐促搁 Subcribe 殿废.



	//bEventsSubscribed = true;
}
void AB2DLCFrontGameMode::UnsubscribeEvents()
{
	//if (!bEventsSubscribed)
	//	return;
	//
	//DLCFrontLaunchBladeIIGameClass<>::GetInstance().Unsubscribe(DLCFrontLaunchBladeIIGameTicket);
	//
	//// 鞘夸茄 Event 啊 乐促搁 Subcribe 秦力. 



	//bEventsSubscribed = false;
}


UB2UIDLCFrontBase* AB2DLCFrontGameMode::OpenUI(EDLCFrontUIName InUIName)
{
	if (GetUI(InUIName)) {
		CloseUI(InUIName);
	}
	UB2UIDLCFrontBase* NewCreated = NULL;
	//FDLCFrontUIData* FoundUIData = MyAssetRef.AllUIData.Find(InUIName);
	//APlayerController* LocalPC = UGameplayStatics::GetLocalPlayerController(this);
	//if (FoundUIData && FoundUIData->WidgetClass && LocalPC)
	//{
	//	NewCreated = CreateWidget<UB2UIDLCFrontBase>(LocalPC, FoundUIData->WidgetClass);
	//	if (NewCreated)
	//	{
	//		NewCreated->AddToViewport(FoundUIData->CreateZOrder); // UIManager 客绰 崔府 Z-order 肺 widget 埃狼 惑窍 啊覆 包拌甫 力绢. Z-order 啊 悼老窍搁 唱吝俊 积己等 霸 困肺.
	//		NewCreated->Init();
	//	}
	//}
	//if (NewCreated)
	//{
	//	AllCreatedUI.Add(InUIName, NewCreated);
	//}

	return NewCreated;
}

void AB2DLCFrontGameMode::CloseUI(EDLCFrontUIName InUIName)
{
	UB2UIDLCFrontBase** FoundOne = AllCreatedUI.Find(InUIName);
	if (FoundOne && *FoundOne)
	{
		(*FoundOne)->DestroySelf();
		AllCreatedUI.Remove(InUIName);
	}
}

void AB2DLCFrontGameMode::CloseAllUIs()
{
	TArray<EDLCFrontUIName> AllList;
	for (TMap<EDLCFrontUIName, UB2UIDLCFrontBase*>::TIterator CreatedUIIt(AllCreatedUI); CreatedUIIt; ++CreatedUIIt)
	{
		AllList.Add(CreatedUIIt.Key());
	}

	for (EDLCFrontUIName ThisUIName : AllList)
	{
		CloseUI(ThisUIName);
	}

	checkSlow(AllCreatedUI.Num() == 0); // 犬脚捞 绝绢辑 check 绰 酒聪绊 checkSlow 肺..

	AllCreatedUI.Empty();
}

UB2UIDLCFrontBase* AB2DLCFrontGameMode::GetUI(EDLCFrontUIName InUIName)
{
	UB2UIDLCFrontBase** FoundOne = AllCreatedUI.Find(InUIName);
	return FoundOne ? *FoundOne : NULL;
}

void AB2DLCFrontGameMode::SetupLoadingScreen_Lobby()
{
	TrySetupLoadingScreenWithInitialTitleMovie(this); // 谍 单辑档 荤侩且 老捞 乐绢辑 static 栏肺 哗 初霸 凳.
}

void AB2DLCFrontGameMode::LaunchBladeIIGame()
{
//	if (UB2PreRenderer::IsPreRenderLevelGoneThrough()
//#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 
//		|| !UB2PreRenderer::ShouldDoPreRenderForIOS_PreRenderGM()
//#endif
//		)
//	{
//		// DLC 牧刨明 罐扁 肯丰窍绊 肺厚 涝厘.		
//		// 鸥捞撇 拳搁捞 酒囱 肺厚 皋牢栏肺 啊具 窍绰单 捞 版快绰 AccountInfo 甫 弊率辑 罐酒具 秦辑 纠傈券 抗距鳖瘤 窍瘤绰 臼绊 老窜 Title 肺 啊档废 茄促.
//		// 捞饶 磊悼 傈券 贸府绰 UITitle 辑 且 巴.
//		OpenBladeIILobbyCommon(this);
//	}
//	else
//	{ // 汲摹窍绊 盖 贸澜 角青捞 瞪 搏单 林夸 InfoAsset 甸俊 殿废等 嘉捞歹 哪颇老阑 固府 秦 初绰 PreRender 饭骇阑 角青茄促.
//		StartBladeIIPreRenderGame(this);
//	}
}


#if WITH_EDITOR
void AB2DLCFrontGameMode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_AllUIData = GET_MEMBER_NAME_CHECKED(FDLCFrontAssetRef, AllUIData);
	const FName Name_WidgetName = GET_MEMBER_NAME_CHECKED(FDLCFrontUIData, WidgetName);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_AllUIData || PropertyName == Name_WidgetName)
	{
		EditorCheckForWidgetNameIntegrity();
	}
}

void AB2DLCFrontGameMode::EditorCheckForWidgetNameIntegrity()
{
	// UIDate TMap 俊 殿废茄 Key 蔼阑 WidgetName 俊 逞败淋.

	for (TMap<EDLCFrontUIName, FDLCFrontUIData>::TIterator UIDataIt(MyAssetRef.AllUIData); UIDataIt; ++UIDataIt)
	{
		FDLCFrontUIData& ThisUIData = UIDataIt.Value();
		if (ThisUIData.WidgetName != UIDataIt.Key())
		{
			ThisUIData.WidgetName = UIDataIt.Key();
		}
	}
}

#endif

void AB2DLCFrontGameMode::ShowDisconnectNetworkPopup()
{
//	UB2UIDLCFrontSimplePopup* MsgPopup = Cast<UB2UIDLCFrontSimplePopup>(OpenUI(EDLCFrontUIName::EDLCUI_MsgPopup));
//	if (MsgPopup)
//	{
//		MsgPopup->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
//		MsgPopup->SetSubContent(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DLC_Popup01_NetworkError")));
//		MsgPopup->SetButtonGroup(EUIMsgPopupButtonGroup::Confirm);
//	}
}

void AB2DLCFrontGameMode::ShowKakaoErrorPopup(int32 ErrorCode)
{
	//UB2UIDLCFrontSimplePopup* MsgPopup = Cast<UB2UIDLCFrontSimplePopup>(OpenUI(EDLCFrontUIName::EDLCUI_MsgPopup));
	//if (MsgPopup)
	//{
	//	FText Message = FText::Format(FText::FromString(TEXT("{0}\nERROR CODE:{1}")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Kakao_ServerError")), 
	//		FText::AsNumber(ErrorCode));

	//	MsgPopup->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	//	MsgPopup->SetSubContent(Message);
	//	MsgPopup->SetButtonGroup(EUIMsgPopupButtonGroup::Confirm);
	//}
}