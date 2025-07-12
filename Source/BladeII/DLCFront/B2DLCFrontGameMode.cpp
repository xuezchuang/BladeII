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
// ���� �κ� ���� �� ���� �������� (DLC) �ٿ�ε� �޴� ����Ʈ ������ ���
// �� ���Ӹ�带 �����ϱ� ���� ��� ���ҽ����� ���ѵ� ũ���� ���� apk �ȿ� ���� ��.
// ������� InfoAsset �ε��� �� ���Ӹ�尡 �� �ִ� ���� �ε��� ������ ������ �̿ܿ��� ���� BladeII ��ɵ��� ����� �Ұ����� �� �ִ�.
// �ּ����� ��ƿ ������ ������ ������ ���̴�.
// �׿� �°� ���⼭ �����ϴ� ��� ������ ���ѵ�.
//
////////////////////////////////////////////////////////////////////////////////

FString AB2DLCFrontGameMode::SelectedFrontTitleMovieName; // ���� �� ù Ÿ��Ʋ ȭ�鿡�� ����. DLCFrontGameMode �� ��� ���Ŀ��� �ʿ��� ��.
EPCClass AB2DLCFrontGameMode::SelectedFrontTitleMovieClass = EPCClass::EPC_End;

AB2DLCFrontGameMode::AB2DLCFrontGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	bEventsSubscribed = false;

	DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn �� �⺻����
	PlayerControllerClass = APlayerController::StaticClass(); // �ʿ信 ���� ���� extend �ؼ� ���..
	
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
	
	// �Ʒ� �� ���� ����
	InitializeAllGameSettingData(this);
	B2Scalability::OnGameModeStartPlay(this);

	PreBeginPlay(); // ABladeIIGameMode::PreBeginPlay �� �������� Ÿ�̹�����.

	Super::StartPlay();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// �������� DLCFrontMap �� ��������� ��ũ�ؼ� ���� ����. ��� ����� ���������� �� ���Ӹ�带 ����ϴ� ��Ȳ�� ���� DLC ����� ���̴�.
	UB2UIDummyDLCFront::bWentThroughRealDLCFront = true;
#endif
}

void AB2DLCFrontGameMode::PreBeginPlay()
{
	GMinimalDLCFrontMode = true; // ���� ���߿��� �̰� �⺻ true ��. InfoAsset �ε��� ����.

#if WITH_EDITOR // BladeIIGameMode �� �ѹ��̶� �÷��� �Ŀ� �۵� Ȯ���� ���� �ʵ��� ��� ���� ���������� �־����. �׽�Ʈ�� ���� �ʿ���� �������� ������ �� �ٸ� �� �����Ƿ� ���� �Ѵ�.
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
{ // Ÿ��Ʋ ȭ�鿡�� ���� ���õ� �� �̸� ����
	SelectedFrontTitleMovieName = InMovieName;
	check(!SelectedFrontTitleMovieName.EndsWith(TEXT(".mp4"))); // �𸮾� GameMoviePlayer ��ɿ��� ����Ϸ��� Ȯ���ڰ� ���� �־�� ��.

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
{ // ���� ��ɿ��� Ÿ��Ʋ Ŭ���� ������ B2DLCFrontGameMode �� �� �ִ� ���ȿ��� ���ȴ�. ���� ���� �׽�Ʈ������ ��� ���� �� ������ �뷫 ����� ������ �� �־ �����.
	SelectedFrontTitleMovieClass = InClass;
}
#endif

void AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie"));

	//// DLCFront ù Title ���� ���� ���õ� �������� �ε� ȭ�鿡 ������ ��.

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

	//		// �Ϲ����� �������� �������̳� Ŭ��¡ ������� ������ �� �ٸ���. �׳� �ε�ȭ���ε� ���� �������ΰ�..
	//		// �⺻ ����ϰ�� �� �ٸ��Ƿ� �Ķ���͸� �����ؼ�..
	//		TArray<B2MovieSubtitle>* LoadingSubtitle = LoadingScreenModule->StartGameLoadingScreen_Movie(
	//			*SelectedFrontTitleMovieName,
	//			// �Ϲ����� ȭ�� ���������� �������� ȭ�� ������ �÷� ���鼭 ������ ���� ����.
	//			bUseShorterScreenLayout ? ELoadingScreenMovieAlignMode::FitToMin_WideHD : ELoadingScreenMovieAlignMode::FitToMax_WideHD, 
	//			false, true, true, EMoviePlaybackType::MT_Looped, true);
	//		if (LoadingSubtitle)
	//		{
	//			// ���ϰ� MovieSubtitles array �� ���⼭ ä�� ��
	//			B2MovieSubtitle LoadingScreenMsg;
	//			// Subtitle �� �������̽��� ������ ���⼱ �ε� �޽����� ������ ��. Time ���� ����ϰ�.
	//			LoadingScreenMsg.StartTime = 0.0f;
	//			LoadingScreenMsg.EndTime = 100.0f;
	//			LoadingScreenMsg.Color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//			LoadingScreenMsg.Text = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type01"));
	//			LoadingSubtitle->Add(LoadingScreenMsg);
	//		}
	//	}
	//	else {
	//		// �ǵ����� ���� ��찡 �� ���� LoadingImageName ���� null �� �Ѱ��ָ� �ȿ��� fallback �̹����� ������ ����.
	//		// DLC �ƴ� ���߹����̸� �̷��� �� ��.
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
	//	LocalPC->ActivateTouchInterface(nullptr); // Virtual Joystick ���ִ� ��
	//	LocalPC->bShowMouseCursor = true;
	//}


	//// ��� ���� �ڵ�� �ƴϰ�.. �� ������ �䱸 ���׿� ����..
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

	GMinimalDLCFrontMode = false; // ���� ���� ���� ���� �̰� �ٽ� true �� ���ư��� �� ��.
}

void AB2DLCFrontGameMode::OnPreLoadMap(const FString& InString)
{
	//
	// �̸��� PreLoadMap �̶� �̹� ���Ӹ�� ���� �������� ������ �� ������ �����δ� �ڱ� �ڽ��� ������ �غ� �ϴ� �κ�.
	// �� ��ȯ (�κ� ����) ���� DLCFront World GC �Ǳ� ���� ���� �����ϱ� ���� ������.
	//

	B2_SCOPED_TRACK_LOG(TEXT("AB2DLCFrontGameMode::OnPreLoadMap"));

	// �κ�� ���� �ε� ȭ���� ����..
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
	//// �ʿ��� Event �� �ִٸ� Subcribe ���.



	//bEventsSubscribed = true;
}
void AB2DLCFrontGameMode::UnsubscribeEvents()
{
	//if (!bEventsSubscribed)
	//	return;
	//
	//DLCFrontLaunchBladeIIGameClass<>::GetInstance().Unsubscribe(DLCFrontLaunchBladeIIGameTicket);
	//
	//// �ʿ��� Event �� �ִٸ� Subcribe ����. 



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
	//		NewCreated->AddToViewport(FoundUIData->CreateZOrder); // UIManager �ʹ� �޸� Z-order �� widget ���� ���� ���� ���踦 ����. Z-order �� �����ϸ� ���߿� ������ �� ����.
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

	checkSlow(AllCreatedUI.Num() == 0); // Ȯ���� ��� check �� �ƴϰ� checkSlow ��..

	AllCreatedUI.Empty();
}

UB2UIDLCFrontBase* AB2DLCFrontGameMode::GetUI(EDLCFrontUIName InUIName)
{
	UB2UIDLCFrontBase** FoundOne = AllCreatedUI.Find(InUIName);
	return FoundOne ? *FoundOne : NULL;
}

void AB2DLCFrontGameMode::SetupLoadingScreen_Lobby()
{
	TrySetupLoadingScreenWithInitialTitleMovie(this); // �� ������ ����� ���� �־ static ���� �� ���� ��.
}

void AB2DLCFrontGameMode::LaunchBladeIIGame()
{
//	if (UB2PreRenderer::IsPreRenderLevelGoneThrough()
//#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 
//		|| !UB2PreRenderer::ShouldDoPreRenderForIOS_PreRenderGM()
//#endif
//		)
//	{
//		// DLC ������ �ޱ� �Ϸ��ϰ� �κ� ����.		
//		// Ÿ��Ʋ ȭ���� �ƴ� �κ� �������� ���� �ϴµ� �� ���� AccountInfo �� ���ʼ� �޾ƾ� �ؼ� ����ȯ ������� ������ �ʰ� �ϴ� Title �� ������ �Ѵ�.
//		// ���� �ڵ� ��ȯ ó���� UITitle �� �� ��.
//		OpenBladeIILobbyCommon(this);
//	}
//	else
//	{ // ��ġ�ϰ� �� ó�� ������ �� ���� �ֿ� InfoAsset �鿡 ��ϵ� ���̴� �������� �̸� �� ���� PreRender ������ �����Ѵ�.
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
	// UIDate TMap �� ����� Key ���� WidgetName �� �Ѱ���.

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