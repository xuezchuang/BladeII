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
// 게임 로비 진입 전 게임 컨텐츠를 (DLC) 다운로드 받는 프론트 구현에 사용
// 이 게임모드를 실행하기 위한 모든 리소스들은 제한된 크기의 단일 apk 안에 들어가야 함.
// 통상적인 InfoAsset 로딩은 이 게임모드가 떠 있는 동안 로딩이 허용되지 않으며 이외에도 많은 BladeII 기능들의 사용이 불가능할 수 있다.
// 최소한의 유틸 정도만 공유가 가능할 것이다.
// 그에 맞게 여기서 제공하는 기능 범위도 제한됨.
//
////////////////////////////////////////////////////////////////////////////////

FString AB2DLCFrontGameMode::SelectedFrontTitleMovieName; // 실행 후 첫 타이틀 화면에서 세팅. DLCFrontGameMode 를 벗어난 이후에도 필요할 것.
EPCClass AB2DLCFrontGameMode::SelectedFrontTitleMovieClass = EPCClass::EPC_End;

AB2DLCFrontGameMode::AB2DLCFrontGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	bEventsSubscribed = false;

	DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn 을 기본으로
	PlayerControllerClass = APlayerController::StaticClass(); // 필요에 따라 따로 extend 해서 사용..
	
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
	
	// 아래 둘 순서 주의
	InitializeAllGameSettingData(this);
	B2Scalability::OnGameModeStartPlay(this);

	PreBeginPlay(); // ABladeIIGameMode::PreBeginPlay 와 마찬가지 타이밍으로.

	Super::StartPlay();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 정식으로 DLCFrontMap 을 통과했음을 마크해서 뻘짓 방지. 적어도 모바일 배포에서는 이 게임모드를 통과하는 상황은 실제 DLC 모드일 것이다.
	UB2UIDummyDLCFront::bWentThroughRealDLCFront = true;
#endif
}

void AB2DLCFrontGameMode::PreBeginPlay()
{
	GMinimalDLCFrontMode = true; // 실행 도중에는 이건 기본 true 임. InfoAsset 로딩을 막음.

#if WITH_EDITOR // BladeIIGameMode 로 한번이라도 플레이 후에 작동 확인을 하지 않도록 경고 정도 차원에서만 넣어놓음. 테스트를 막을 필요까진 없겠지만 동작이 좀 다를 수 있으므로 경고는 한다.
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
{ // 타이틀 화면에서 랜덤 선택된 거 이름 저장
	SelectedFrontTitleMovieName = InMovieName;
	check(!SelectedFrontTitleMovieName.EndsWith(TEXT(".mp4"))); // 언리얼 GameMoviePlayer 기능에서 사용하려면 확장자가 빠져 있어야 함.

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
{ // 정식 기능에서 타이틀 클래스 지정은 B2DLCFrontGameMode 가 떠 있는 동안에만 허용된다. 개발 버전 테스트에서도 어느 정도 들어가 있으면 대략 기능을 따라할 수 있어서 열어둠.
	SelectedFrontTitleMovieClass = InClass;
}
#endif

void AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie"));

	//// DLCFront 첫 Title 에서 랜덤 선택된 동영상을 로딩 화면에 띄우려는 거.

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

	//		// 일반적인 스테이지 오프닝이나 클로징 영상과는 성격이 좀 다르다. 그냥 로딩화면인데 루핑 동영상인거..
	//		// 기본 사용하고는 좀 다르므로 파라미터를 주의해서..
	//		TArray<B2MovieSubtitle>* LoadingSubtitle = LoadingScreenModule->StartGameLoadingScreen_Movie(
	//			*SelectedFrontTitleMovieName,
	//			// 일반적인 화면 비율에서는 동영상을 화면 밖으로 늘려 가면서 정해진 비율 유지.
	//			bUseShorterScreenLayout ? ELoadingScreenMovieAlignMode::FitToMin_WideHD : ELoadingScreenMovieAlignMode::FitToMax_WideHD, 
	//			false, true, true, EMoviePlaybackType::MT_Looped, true);
	//		if (LoadingSubtitle)
	//		{
	//			// 리턴값 MovieSubtitles array 는 여기서 채워 줌
	//			B2MovieSubtitle LoadingScreenMsg;
	//			// Subtitle 용 인터페이스를 쓰지만 여기선 로딩 메시지로 쓰려는 거. Time 범위 충분하게.
	//			LoadingScreenMsg.StartTime = 0.0f;
	//			LoadingScreenMsg.EndTime = 100.0f;
	//			LoadingScreenMsg.Color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//			LoadingScreenMsg.Text = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type01"));
	//			LoadingSubtitle->Add(LoadingScreenMsg);
	//		}
	//	}
	//	else {
	//		// 의도하지 않은 경우가 될 껀데 LoadingImageName 으로 null 을 넘겨주면 안에서 fallback 이미지로 동작할 것임.
	//		// DLC 아닌 개발버전이면 이렇게 될 듯.
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
	//	LocalPC->ActivateTouchInterface(nullptr); // Virtual Joystick 없애는 거
	//	LocalPC->bShowMouseCursor = true;
	//}


	//// 요건 정식 코드는 아니고.. 상세 구현은 요구 사항에 따라..
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

	GMinimalDLCFrontMode = false; // 정식 게임 실행 도중 이게 다시 true 로 돌아가선 안 됨.
}

void AB2DLCFrontGameMode::OnPreLoadMap(const FString& InString)
{
	//
	// 이름은 PreLoadMap 이라 이번 게임모드 시작 직전으로 오해할 수 있지만 실제로는 자기 자신은 내려갈 준비를 하는 부분.
	// 맵 전환 (로비 진입) 직전 DLCFront World GC 되기 이전 무언가 정리하기 좋은 스팟임.
	//

	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::OnPreLoadMap"));

	// 로비로 들어가는 로딩 화면을 띄운다..
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
	//// 필요한 Event 가 있다면 Subcribe 등록.



	//bEventsSubscribed = true;
}
void AB2DLCFrontGameMode::UnsubscribeEvents()
{
	//if (!bEventsSubscribed)
	//	return;
	//
	//DLCFrontLaunchBladeIIGameClass<>::GetInstance().Unsubscribe(DLCFrontLaunchBladeIIGameTicket);
	//
	//// 필요한 Event 가 있다면 Subcribe 해제. 



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
	//		NewCreated->AddToViewport(FoundUIData->CreateZOrder); // UIManager 와는 달리 Z-order 로 widget 간의 상하 가림 관계를 제어. Z-order 가 동일하면 나중에 생성된 게 위로.
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

	checkSlow(AllCreatedUI.Num() == 0); // 확신이 없어서 check 는 아니고 checkSlow 로..

	AllCreatedUI.Empty();
}

UB2UIDLCFrontBase* AB2DLCFrontGameMode::GetUI(EDLCFrontUIName InUIName)
{
	UB2UIDLCFrontBase** FoundOne = AllCreatedUI.Find(InUIName);
	return FoundOne ? *FoundOne : NULL;
}

void AB2DLCFrontGameMode::SetupLoadingScreen_Lobby()
{
	TrySetupLoadingScreenWithInitialTitleMovie(this); // 딴 데서도 사용할 일이 있어서 static 으로 빼 놓게 됨.
}

void AB2DLCFrontGameMode::LaunchBladeIIGame()
{
//	if (UB2PreRenderer::IsPreRenderLevelGoneThrough()
//#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 
//		|| !UB2PreRenderer::ShouldDoPreRenderForIOS_PreRenderGM()
//#endif
//		)
//	{
//		// DLC 컨텐츠 받기 완료하고 로비 입장.		
//		// 타이틀 화면이 아닌 로비 메인으로 가야 하는데 이 경우는 AccountInfo 를 그쪽서 받아야 해서 씬전환 예약까지 하지는 않고 일단 Title 로 가도록 한다.
//		// 이후 자동 전환 처리는 UITitle 서 할 것.
//		OpenBladeIILobbyCommon(this);
//	}
//	else
//	{ // 설치하고 맨 처음 실행이 될 껀데 주요 InfoAsset 들에 등록된 셰이더 컴파일을 미리 해 놓는 PreRender 레벨을 실행한다.
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
	// UIDate TMap 에 등록한 Key 값을 WidgetName 에 넘겨줌.

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