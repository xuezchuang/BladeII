// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyGameMode.h"
//#include "BladeII.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpectatorPawn.h"
#include "Animation/SkeletalMeshActor.h"

#include "BladeIIGameMode.h"
#include "B2UIManager_Lobby.h"
#include "B2UIManager.h"
#include "BladeIIPlayer.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
#include "B2LobbyInventory.h"
#include "B2NetworkBridge.h"
#include "Event.h"
#include "B2StageManager.h"
#include "Retailer.h"
#include "CommonStruct.h"
#include "B2StageInfo.h"
#include "B2ChapterInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIDocBattleStage.h"
#include "B2CombatStatEvaluator.h"
#include "EngineGlobals.h"
#include "BladeIIPointCharger.h"
////#include "Matinee/MatineeActor.h"
#include "B2StageGameMode.h"
#include "B2UIChatting.h"
#include "B2Airport.h"
#include "B2UIHeroTowerReady.h"
#include "B2UIDocRaid.h"
#include "B2UIMsgPopupSelectPCClass.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "ChannelManager.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIBattleScoreNotify.h"
#include "B2UIDocLobbyMatch.h"
#include "B2SkillRTManager.h"
#include "B2UIAttendanceMain.h"
#include "TutorialManager.h"
#include "B2BGMManager.h"
#include "B2DLCFrontGameMode.h"
#include "B2UICommonInvite.h"
#include "B2UIPVPCharacterSelectPage.h"
#include "B2UIMsgPopupSelectPCClass.h"
#include "B2UIMsgPopupInventoryFull.h"

//#if PLATFORM_ANDROID
//#include "Android/AndroidJNI.h"
//extern bool AndroidThunkCpp_GetCharacterInfoScene();
//#endif
#include "B2UIGuildMapMain.h"
#include "B2GuildCastleActor.h"
#include "B2UIRaidMain.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIModRewardBoxGradeInfo.h"
#include "B2UIModRewardBoxDetailInfo.h"
#include "B2UISummonItemDisplay.h"
#include "B2PreRenderer.h"
#include "B2GameLoadingProgress.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2PlatformAPI.h"
#include "B2GameDefine.h"


bool AB2LobbyGameMode::IsFirstLobby = true;
bool AB2LobbyGameMode::IsFirstLobbyMain = true;

AB2LobbyGameMode::AB2LobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bDefaultMatineePlayedReverse(false)
	, bIsInputLeftControl(false)
{
	//PrimaryActorTick.bCanEverTick = true;

	//DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn 을 기본으로
	//PlayerControllerClass = APlayerController::StaticClass(); // 필요에 따라 따로 extend 해서 사용..

	//DefaultPCClassEnum = EPCClass::EPC_End; // 일단 spawn 하지 않는다.

	//GameModeToPlay = ABladeIIGameMode::StaticClass();

	//bHeroMgmtMode = false;
	//HeroMgmtSubMode = EHeroMgmtSubMode::EHMSM_Inventory;

	//SelectedChapterViewNum = 0;
	//SelectedStageDifficulty = EStageDifficulty::ESD_None;

	//LobbyUIManager = NULL;
	//LobbyInventory = NULL;
	//LobbyAirport = NULL;

	//InputHistory_CheckFlick.InitializeHistory();
	//InputHistory_IgnoreCheck.InitializeHistory();

	//if (!HasAnyFlags(RF_ClassDefaultObject) && GConfig)
	//{
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("ConsiderFlickDeltaTime"), ConsiderFlickDeltaTime, GGameIni);
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("ConsiderFlickScreenDeltaRatio"), ConsiderFlickScreenDeltaRatio, GGameIni);
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("OddInputMinTimeInterval"), OddInputMinTimeInterval, GGameIni);
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("OddInputMinPosDeltaRatio"), OddInputMinPosDeltaRatio, GGameIni);
	//}

	//bLobbyLoadingScreenCompleted = false;
	//bEnableLobbyScreenRotate = false;

	//bWaitPrerequisiteToStartForNonHost = false;
}

void AB2LobbyGameMode::BeginPlay()
{
	//B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::BeginPlay"));

	//Super::BeginPlay();	
	//
	//AB2StageManager::GetCacheStageKeepEssentialData().ClearStageBasisData();

	//// PreRenderGameMode 도입에 따라 첫 로비 로딩 직후 별다른 하는 일 없이 바로 Pre-render 전용 맵을 로딩하는 상황이 발생하게 된다.
	//const bool bMapTransitionWillBeginSoon = IsFirstTimePreRenderPending(); // 아마 이게 true 면 그냥 여기서 return 하는 게 이득일지도 ㅋ

	//if (IsFirstLobby && !bMapTransitionWillBeginSoon)
	//{
	//	// 출석 UI 가 처음 뜰 때 랙이 걸리는 것이 뜨는 상황 때문에 안 좋아 보여서 첫 로딩시간 증가를 감수하고 미리 열었다 닫는거. 여기에는 StayInViewport 체크가 되어 있어서 CloseUI 하는 게 그냥 숨기는 것임.
	//	UB2UIAttendanceMain* atta = UB2UIManager::GetInstance()->OpenUI<UB2UIAttendanceMain>(UIFName::AttendanceMain);
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::AttendanceMain);
	//	B2GMLoadingProgCollector::StepCustom(0.4f);
	//}

	//// 아마 BeginPlay 시점 이전에 필요하지는 않을 것.
	//LobbyUIManager = NewObject<UB2UIManager_Lobby>(this, *FString::Printf(TEXT("UIManager_Lobby_%s"), *this->GetName()), RF_Transient);
	//if (LobbyUIManager)
	//{
	//	// LobbyUIManager 는 DJLegacy 의 일부인데 내부에서 신 UIManager 를 사용하는 부분이 있어서 만에 하나 이걸 제거한다면 내부를 살펴볼 것. (아마 제거할 일이 없을 듯)
	//	if (LobbyUIManager->InitUIManager(this) == false)
	//	{
	//		LobbyUIManager->ConditionalBeginDestroy();
	//		LobbyUIManager = NULL;
	//	}
	//}
	//if (IsFirstLobby) // 처음에 UB2UIManager_Lobby 에서 LoadOnStartup 처리하면서 시간 걸리는 게 좀 있음.
	//{
	//	B2GMLoadingProgCollector::StepCustom(0.4f);
	//}

	//// LobbyUIManager 생성 이후로는 가급적 이쪽을 직접 access 하는 대신 Event 시스템을 이용하여 메시지를 보낸다.
	//SetPCSelection(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	//CreateLobbyInventory(); // 필요에 따라 시점을 좀 조절..
	//CreateLobbyAirport();

	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	//Matinee actor에서 cinematic mode가 on에서 off될 때, 무조건 Virtual joystick visibility를 켜게 되어있음.
	//	//리소스에서 설정해야 할 것 같은데, 실수가 있을 수 있으니 여기서 강제로 cinematic mode로 진입하지 않도록 한다.
	//	//UWorld::GetMatineeActors() 이 함수가 있는데, CurrentLevel에 대해서만 찾아와서 아래처럼 코딩
	//	for (FActorIterator It(World); It; ++It)
	//	{
	//		auto* MatineeActor = Cast<ALevelSequenceActor >(*It);
	//		if (MatineeActor)
	//		{
	//			MatineeActor->bNoCinematicModeControl = true;
	//		}
	//	}
	//}

	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(World);
	//if (PlayerController)
	//	PlayerController->ActivateTouchInterface(nullptr);

	//LobbySceneManager.SetGameMode(this);

	////[@Woongbok, 171222][TODO] 여기서 안받아도 될듯 하나 로직 검증이 필요함.
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{ // 주요 전투력 수치 UI 업데이트. 로비에서 좀 놓치는 거 있더라도 확실하게 업데이트 되는 시점 하나 넣어주는 걸로
	//	OnUpdateCombatPower(IntToPCClass(PCI), false);
	//}
	//	
	//if (IsFirstLobby && !bMapTransitionWillBeginSoon)
	//{ // 첫 로비 진입 마크 리셋
	//	IsFirstLobby = false;
	//}
	//check(bMapTransitionWillBeginSoon || !IsFirstLobby);
}

void AB2LobbyGameMode::PreBeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::PreBeginPlay"));

	Super::PreBeginPlay();
}

void AB2LobbyGameMode::StartPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::StartPlay"));
	
	SetUseCustomWaitForLoadingScreen(true); // 상위단 StartPlay 루틴에서 닫히지 않도록 막아놓고.. 여기서 핸들링할 거.

	if (IsFirstLobby)
	{ // 상위단 쪽에서 Scalability override 적용하기 전에 들어가야..
		InitializeAllGameSettingData(this);

		FString Title;
		if (FParse::Value(FCommandLine::Get(), TEXT("TitleName="), Title))
		{
			UKismetSystemLibrary::SetWindowTitle(FText::FromString(Title));
		}
	}

	Super::StartPlay();

	// 로비 게임모드의 진행도는 기본 StartPlay 의 Preload 에서는 별볼일 없을 꺼고 여기서 남은 DeferredChangeLobbyScene 정도..
	if (LobbySceneManager.HasDeferredChangeLobbyScenes())
	{
		B2GMLoadingProgCollector::StepCustom(0.5f, 0.5f);
		// 실제 ChangeScene 을 동반하는 경우.. 그게 얼마나 될지에 따라 다를 껀데.. 대부분 하나니까.. 큰 스텝을 주자.
		// 그럼 UIScene 하나 열고 거의 꽉 채우겠지.
		B2GMLoadingProgCollector::SetStepRate(0.9f);
	}
	else
	{
		B2GMLoadingProgCollector::StepToMax();
	}

	// 전투맵서 돌아오거나 할 때에는 DeferredChangeLobbyScenes 로 예약해 놓은 open 리스트가 있을 것임. 그게 아닐 경우만 Title 화면을 연다. 
	// 어차피 Title 화면이 열리더라도 예약해 놓은 장면으로 바뀌겠지만 그냥 그렇게 두면 로비로 돌아올 때의 로딩시간 낭비가 됨.
	if (!LobbySceneManager.HasDeferredChangeLobbyScenes())
	{
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_Title);
	}
	else
	{
		//title화면에서는 별개의 bgm사용
		//title화면이 아닐시만 로비 bgm사용
		PlayMainLobbyBgm();
	}

	if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		if (DocChapter->GetShowChapterEventScene())
		{
			LobbySceneManager.ClearDeferredChangeLobbyScenes();
			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
			
			FServerStageID DeferredServerStageID = DocChapter->GetDeferredServerStageID();
			if (DeferredServerStageID != 0)
			{
				FStageDataStore& StageDataStore = BladeIIGameImpl::GetStageDataStore();

				int32 ClientStageID = StageDataStore.GetClientStageId(DeferredServerStageID);
				int32 ActID = StageDataStore.GetActIdByClientStageId(ClientStageID);
				EStageDifficulty StageDifficulty = StageDataStore.GetStageDifficulty(DeferredServerStageID);

				StageDataStore.RequestGetActInfoAndChangeChapter(ActID, StageDifficulty, true, DeferredServerStageID);

				DocChapter->SetShowChapterEventScene(false);
				DocChapter->SetDeferredServerStageID(FServerStageID());
			}
		}
	}

	// 전투맵에서 로비로 돌아온 후에는 이것의 실행으로 실제 원하는 로비 장면으로 가게 될 것. 메인화면이든, 전투맵이든.
	LobbySceneManager.CallDeferredChangeLobbyScenes();

	if (LobbyInventory)
	{ // 전투 후 로비 진입 시점에서의 이전 스테이지 습득 아이템의 장착 권장 체크. 바로 위에서 장착 권장 시 참고할 데이터가 들어갈 수 있으므로 이 시점에서 체크함.
		LobbyInventory->ConditionalCheckForBetterEquipSuggest();
	}

	if (!IsInPreRenderPhase() && !IsFirstTimePreRenderPending()
		&& UB2UIManager::GetInstance()->GetCurrUIScene() != EUIScene::Title)
	{ // Pre-render 와 얽히지 않은 대부분의 경우 (설치 후 최초가 아니라면) 상위 StartPlay 의 WaitForLoadingScreen 대신 여기서..
		WaitForLoadingScreen();
	}

	// Initialize InvitationGame
	ClearInvitationGameInfo(EB2GameMode::PVP_Tag);
	ClearInvitationGameInfo(EB2GameMode::Raid);
}

void AB2LobbyGameMode::OnLoadingScreenFinished()
{
	// 특별히 처리하지 않으면 로딩 동영상이 돌아가는 와중에 뜰 수 있는.. 외부 모듈에 의한 팝업이 있어서 이 타이밍을 기록해 둠.
	bLobbyLoadingScreenCompleted = true;
}

void AB2LobbyGameMode::BeginDestroy()
{
	if(GetWorld() && GetWorldTimerManager().IsTimerActive(WaitInventorySyncTimeHandler))
		GetWorldTimerManager().ClearTimer(WaitInventorySyncTimeHandler);

	Super::BeginDestroy();
}

void AB2LobbyGameMode::BattleStart()
{
	LobbySceneManager.BattleStart();
}
void AB2LobbyGameMode::SetDesiredPCClassEnum()
{
	CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
}

void AB2LobbyGameMode::SubscribeEvents()
{
//	if (bEventsSubscribed)
//		return;
//
//	Super::SubscribeEvents(); // Super 쪽에 필요없는 것들도 있지만 안할 것도 없지.
//
//	Issues.Add(StartGameStageFromLobbyClass<int32, EStageDifficulty, const TArray<EStageBuffType>&, bool>::GetInstance().Subscribe2(
//		[this](int32 i, EStageDifficulty diff, const TArray<EStageBuffType>& sbt, bool bskipstory)
//		{
//			this->RequestStartGameStageWithSelectedPC(i, diff, sbt, bskipstory);
//		}
//	));
//	Issues.Add(StartRepeatBattleStageClass<int32, EStageDifficulty>::GetInstance().Subscribe2(
//		[this](int32 i, EStageDifficulty StageDifficulty)
//		{
//			this->RequestStartRepeatBattleWithSelectedPC(i, StageDifficulty);
//		}
//	));
//	Issues.Add(LobbyLoginAndStartGameClass<const FString&, const FString&>::GetInstance().Subscribe2(
//		[this](const FString& sa, const FString& sb)
//		{
//			this->LoginAndStartGame(sa, sb);
//		}
//	));
//	Issues.Add(LobbyCreateAccountClass<const FString&, const FString&>::GetInstance().Subscribe2(
//		[this](const FString& sa, const FString& sb)
//		{
//			this->CreateAccount(sa, sb);
//		}
//	));
//	Issues.Add(LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass MainChar, EPCClass SubChar)
//		{
//			this->SetPCSelection(MainChar, SubChar);
//		}
//	));
//	Issues.Add(LobbySetMainPCClassClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass MainChar)
//		{
//			this->SetMainPCClass(MainChar);
//		}
//	));
//	Issues.Add(LobbyCharacterIntroSetPCSelectionClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass Char)
//		{
//			this->SetCharacterIntroModePCSelection(Char);
//		}
//	));
//	Issues.Add(LobbyProceedOnCharSelClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ProceedOnCharacterSelection();
//		}
//	));
//	Issues.Add(LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Subscribe2(
//		[this](EHeroMgmtSubMode sm)
//		{
//			this->EnterHeroMgmtMode(sm);
//		}
//	));
//	Issues.Add(LobbyQuitHeroMgmtModeClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->QuitHeroMgmtMode();
//		}
//	));
//	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass c)
//		{
//			this->SetHeroMgmtModePCSelection(c);
//		}
//	));
//	Issues.Add(LobbySetCombatEntryChapterNumClass<int32, EStageDifficulty>::GetInstance().Subscribe2(
//		[this](int32 i, EStageDifficulty d)
//		{
//			this->SetCombatEntryChapterNum(i, d);
//		}
//	));
//	Issues.Add(LobbyStartMatchClass<bool>::GetInstance().Subscribe2(
//		[this](bool IsRequestMatchMaking)
//		{
//			this->StartLobbyMatch(IsRequestMatchMaking);
//		}
//	));
//	Issues.Add(GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Subscribe2(
//		[this](FServerStageID ServerStageId, FRepeatBattleStateSet RepeatBattleSet)
//		{
//			this->GoGameStageInfo(ServerStageId, RepeatBattleSet);
//		}
//	));
//	Issues.Add(GoGameStageInfoLastReachedFromLobbyClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->GoGameStageInfoLastReached();
//		}
//	));
//	Issues.Add(GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Subscribe2(
//		[this](FServerStageID ServerStageId)
//		{
//			this->GameStageCheckForArinConsulting(ServerStageId);
//		}
//	));
//	Issues.Add(LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass pc)
//		{
//			this->UpdateCharacterParts(pc);
//		}
//	));
//	Issues.Add(LobbyChangeSceneClass<ELobbyScene>::GetInstance().Subscribe2(
//		[this](ELobbyScene ToChangeScene)
//		{
//			this->ChangeLobbyScene(ToChangeScene);
//		}
//	));
//	Issues.Add(LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Subscribe2(
//		[this](EUIScene Scene)
//		{
//			this->ChangeLobbyScene(Scene);
//		}
//	));
//	Issues.Add(LobbyChangeToChapterSceneByUISceneClass<int32>::GetInstance().Subscribe2(
//		[this](int32 ChangeChapterNum)
//		{
//			this->ChangeLobbyScene(EUIScene::Chapter, ChangeChapterNum); /* specially for chapter scene.. */
//		}
//	));
//	Issues.Add(LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Subscribe2(
//		[this](ELobbyInvenItemOpMode m)
//		{
//			this->EnterItemOpDirectingView(m);
//		}
//	));
//	Issues.Add(LobbyQuitItemOpDirectingViewClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->QuitItemOpDirectingView();
//		}
//	));
//	Issues.Add(LobbyEnterPVPMainMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->EnterPVPMainMenu();
//		}
//	));
//	Issues.Add(LobbyAnyOtherUpdateOnSetAccountClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->AnyOtherUpdateOnSetAccount();
//		}
//	));
//	Issues.Add(LobbyEnterTeamMatchMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->EnterTeamMatchMenu();
//		}
//	));
//	Issues.Add(LobbyEnterControlMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->EnterControlMenu();
//	}
//	));	
//	Issues.Add(LobbyUpdateChapterDataClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->UpdateChapterData();
//		}
//	));
//	Issues.Add(EnterTMHeroChangeMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->EnterTMChageHeroMod();
//		}
//	));
//	Issues.Add(HandleServerError910Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughGems();
//		}
//	));
//	Issues.Add(HandleServerError911Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughGolds();
//		}
//	));
//	Issues.Add(HandleServerError919Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughDarkCystal();
//		}
//	));
//	Issues.Add(HandleServerError932Class<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorNotEnoughFamePoint();
//	}
//	));
//	Issues.Add(HandleServerError4032Class<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorNotEnoughStageAutoClearTicket();
//	}
//	));
//	Issues.Add(HandleServerError1012Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorWrongCharacterNickname();
//		}
//	));
//	Issues.Add(HandleServerError4207Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughHeroPiece();
//		}
//	));
//	Issues.Add(HandleServerError7413Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorUserInRaidPenalty();
//		}
//	));
//	Issues.Add(HandleServerError4015ByRoomJoinClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorInventoryIsFull_RoomJoin();
//	}
//	));
//	Issues.Add(HandleServerError4015ForSelectCharacterClass<TArray<b2network::B2InventoryCountServerInfoPtr>>::GetInstance().Subscribe2(
//		[this](TArray<b2network::B2InventoryCountServerInfoPtr> InStageInfo)
//	{
//		this->ErrorInventoryIsFull_RoomJoin(InStageInfo);
//	}
//	));
//	Issues.Add(DeliveryRoomJoinErrorClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorRoomJoin();
//	}
//	)); 
//	Issues.Add(DeliveryRaidMatchmakingErrorClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorRaidMatchMaking();
//	}
//	));
//	Issues.Add(HandleServerError8005Class<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorChannelRoomClose();
//	}
//	));
//	Issues.Add(ErrorOpenWebEmptyURLClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorOpenWebEmptyURL();
//	}
//	));
//	Issues.Add(ReceivedNotifyInviteMessageClass<const FString&, const FString&, const FString&, int32, int32>::GetInstance().Subscribe2(
//		[this](const FString& Inviter, const FString& InviteKey, const FString& RoomId, int32 mapId, int32 difficulty)
//		{
//			this->NotifyRoomInviteMessage(Inviter, InviteKey, RoomId, mapId, difficulty);
//		}
//	));
//	Issues.Add(ReceivedKickUserClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->HandleNotifyKickUser();
//		}
//	));
//	Issues.Add(ReceivedNoResponsePrerequisiteToStartClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->HandleNotifyNoResponsePrerequisiteToStart();
//		}
//	));
//	Issues.Add(ReceivedBattleGradeInfoClass<uint32, int32, int32>::GetInstance().Subscribe2(
//		[this](uint8 NetId, int32 BattleGrade, int32 BattleStarCount)
//		{
//			this->NetAckBattleGradeInfo(NetId, BattleGrade, BattleStarCount);
//		}
//	));
//	Issues.Add(ReceivedOtherTokenClass<uint32, const FString&>::GetInstance().Subscribe2(
//		[this](uint32 InNetId, const FString& MatchToken)
//		{
//			this->SetOtherToken(InNetId, MatchToken);
//		}
//	));
//	Issues.Add(FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Subscribe2(
//		[this](int64 AccountID, EPCClass PCClass1 = EPCClass::EPC_End, EPCClass PCClass2 = EPCClass::EPC_End, EPCClass PCClass3 = EPCClass::EPC_End)
//		{
//			this->RequsetFindAccount(AccountID, PCClass1, PCClass2, PCClass3);
//		}
//	));
//	Issues.Add(LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Subscribe2(
//		[this](const int32 Index)
//		{
//			this->HeroTransientEffectBegin(Index);
//		}
//	));
//	Issues.Add(UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Subscribe2(
//		[this](EPCClass EPC, bool PopupShow)
//		{
//			this->OnUpdateCombatPower(EPC, PopupShow);
//		}
//	));
//	Issues.Add(ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass EPC)
//		{
//			this->OnShowBattleScoreNotify(EPC);
//		}
//	));
//	Issues.Add(DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Subscribe2(
//		[this](FB2OpenDuelRewardBoxPtr RewardInfo)
//		{
//			this->ModRewardOpenPopup(RewardInfo);
//		}
//	));
//	Issues.Add(DeliveryGetDuelRewardBoxInfoClass<FB2GetDuelRewardBoxInfoPtr>::GetInstance().Subscribe2(
//		[this](FB2GetDuelRewardBoxInfoPtr BoxGradeInfo)
//		{
//			this->ModRewardBoxListInfo(BoxGradeInfo);
//		}
//	));
//	Issues.Add(DeliveryGetDuelRewardBoxGainableListClass<FB2GetDuelRewardBoxGainableListPtr>::GetInstance().Subscribe2(
//		[this](FB2GetDuelRewardBoxGainableListPtr BoxInfo)
//		{
//			this->ModRewardBoxDetailInfo(BoxInfo);
//		}
//	));
//	Issues.Add(ReceiveRoomPlayerTeamInfoClass<TMap<uint32, uint8>&>::GetInstance().Subscribe2(
//		[this](TMap<uint32, uint8>& TeamInfos)
//	{
//		this->NetReceiveMatchPlayerTeamInfo(TeamInfos);
//	}
//	));
//	Issues.Add(PlayLobbyBgmClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->PlayMainLobbyBgm();
//	}
//	));
//	Issues.Add(OnChangUISceneClass<EUIScene>::GetInstance().Subscribe2(
//		[this](EUIScene UIScene)
//	{
//		this->OnChangeUIScene(UIScene);
//	}
//	));
//	Issues.Add(OnChangUISceneBackClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->OnChangeUISceneBack();
//	}
//	));
//	Issues.Add(DeliveryResponseGetHallOfFameClass<FB2ResponseGetHallOfFamePtr>::GetInstance().Subscribe2(
//		[this](FB2ResponseGetHallOfFamePtr Message)
//	{
//		this->ResponseHallOfFameInfo(Message);
//	}
//	));
//	Issues.Add(DeliveryResponsePraiseTargetRankerClass<FB2ResponsePraiseTargetRankerPtr>::GetInstance().Subscribe2(
//		[this](FB2ResponsePraiseTargetRankerPtr Message)
//	{
//		this->ResponsePraiseTargetRanker(Message);
//	}
//	));
//
//	Issues.Add(EnterCostumePageClass<ECostumePage>::GetInstance().Subscribe2(
//	[this](ECostumePage eCostumePageType)
//	{
//		this->EnterCostumeUIScene(eCostumePageType);
//	}
//	));
//
//    Issues.Add(StartPVPReplayClass<FString, int32, int32>::GetInstance().Subscribe2(
//        [this](FString url, int32 sizeCompressed, int32 sizeUncompressed)
//        {
//            FModReplayInfo ReplayInfo{ url, sizeCompressed, sizeUncompressed };
//            StartBladeIIPVPReplay(this, ReplayInfo);
//        }
//    ));
//
//	Issues.Add(DeliveryCheckReachPowerClass<FB2ResponseCheckReachPowerPtr>::GetInstance().Subscribe2(
//		[this](FB2ResponseCheckReachPowerPtr Message)
//	{
//		this->ResponseCheckReachPower(Message);
//	}
//	));
//	Issues.Add(DeliveryReceiveFairyGiftClass<FB2ReceiveFairyGiftPtr>::GetInstance().Subscribe2(
//		[this](const FB2ReceiveFairyGiftPtr& pReceiveFairyGift)
//	{
//		this->ResponseReceiveFairyGift(pReceiveFairyGift);
//	}
//	));
//	Issues.Add(UpdateFairyGiftStateClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->UpdateFairyGiftState();
//	}
//	));
//
bEventsSubscribed = true;
}

void AB2LobbyGameMode::ErrorNotEnoughGems()
{
	ShortageMGR->PopupGoToShopForGemMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughGolds()
{
	ShortageMGR->PopupGoToShopForGoldMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughHeroPiece()
{
	ShortageMGR->PopupGoToHeroTowerMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughStageAutoClearTicket()
{
	ShortageMGR->PopupGotoShopForAutoClearTicket();
}

void AB2LobbyGameMode::ErrorNotEnoughDarkCystal()
{
	ShortageMGR->PopupGotoShopForDarkCrystalMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughFamePoint()
{
	ShortageMGR->PopupGotoShopForFamePointMessage();
}

void AB2LobbyGameMode::ErrorUserInRaidPenalty()
{
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
}

void AB2LobbyGameMode::ErrorInventoryIsFull_RoomJoin()
{
	EPCClass PCClass = EPCClass::EPC_End;
	
	if (BladeIIGameImpl::GetClientDataStore().GetWaitInventorySync()) return; // 싱크중에 재차 왔을 경우 리턴

	for (int32 i = 0; i < GetMaxPCClassNum(); i++)
	{
		if (UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, IntToPCClass(i)))
		{
			PCClass = IntToPCClass(i);
			break;
		}
	}

	GetWorldTimerManager().ClearTimer(WaitInventorySyncTimeHandler);

	if (PCClass == EPCClass::EPC_End)
	{
		UE_LOG(LogBladeII, Log, TEXT("mismatch server-client inventroy data. need refresh"));

		GetWorldTimerManager().SetTimer(WaitInventorySyncTimeHandler, this, &AB2LobbyGameMode::ReleaseWaitInventorySync, 5.0f);

		BladeIIGameImpl::GetClientDataStore().SetWaitInventorySync(true);

		for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
		{
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(IntToPCClass(i));
		}

		return;
	}

	B2UIUtil::OpenInventoryFullPopup(PCClass, FMsgPopupOnClick::CreateLambda([this, PCClass]() {
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	}));
}

void AB2LobbyGameMode::ErrorInventoryIsFull_RoomJoin(TArray<b2network::B2InventoryCountServerInfoPtr> InInvenInfo)
{
	EPCClass PCClass = EPCClass::EPC_End;

	if (BladeIIGameImpl::GetClientDataStore().GetWaitInventorySync()) return; // 싱크중에 재차 왔을 경우 리턴

	if (InInvenInfo.Num() > 0)
	{
		PCClass = SvrToCliPCClassType(InInvenInfo[0]->character_type);
	}

	if (PCClass == EPCClass::EPC_End)
	{
		UE_LOG(LogBladeII, Log, TEXT("mismatch server-client inventroy data(ErrorInventoryIsFull_RoomJoin)."));
		return;
	}

	auto InventoryPopUp = B2UIUtil::OpenInventoryFullPopup(PCClass);
	if (InventoryPopUp.IsValid() && InInvenInfo.Num() > 0)
	{
		InventoryPopUp->SetInventoryItemCount(InInvenInfo[0]->count, InInvenInfo[0]->max_count);
	}

	//B2UIUtil::OpenInventoryFullPopup(PCClass, FMsgPopupOnClick::CreateLambda([this, PCClass]() {
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//	LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	//}));
}

void AB2LobbyGameMode::ReleaseWaitInventorySync()
{
	BladeIIGameImpl::GetClientDataStore().SetWaitInventorySync(false);
}

void AB2LobbyGameMode::ErrorRoomJoin()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ErrorRoomJoin"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidJoinData();
		DocRaid->ClearRaidBattleData();
		DocRaid->SetIsReceiveInvite(false);
	}

	if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
	{
		UIRaidMain->ButtonFree();
	}
}

void AB2LobbyGameMode::ErrorRaidMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ErrorRaidMatchMaking"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidJoinData();
		DocRaid->ClearRaidBattleData();
		DocRaid->SetIsReceiveInvite(false);
	}

	if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
	{
		UIRaidMain->ButtonFree();
	}
}

void AB2LobbyGameMode::ErrorOpenWebEmptyURL()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ErrorOpenWebEmptyURL")),
		0.f,
		true,
		true
	);
}

void AB2LobbyGameMode::ErrorChannelRoomClose()
{
	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		if (DocRaid->GetIsRequestPartyRematch())
		{
			DocRaid->ClearPartyRetryInfo();
			DocRaid->SetIsRequestPartyRematch(false);
		}
	}

	GoToVillage();
}

void AB2LobbyGameMode::ModRewardOpenPopup(FB2OpenDuelRewardBoxPtr RewardBoxInfo)
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		RewardPopup->AddRewardItems(RewardBoxInfo->received_rewards);
		RewardPopup->ShowRewardPopup(true);
	}
}

void AB2LobbyGameMode::ModRewardBoxListInfo(FB2GetDuelRewardBoxInfoPtr BoxInfo)
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto BoxGradeUI = UIManager->OpenUI<UB2UIModRewardBoxGradeInfo>(UIFName::ModRewardBoxList);
		int32 WinCount = 0;				//누적으로 계산

		if (BoxGradeUI)
		{
			for (auto El : BoxInfo->reward_boxes)
			{
				WinCount += El->win_count;
				BoxGradeUI->SetBoxGrade(BoxInfo->duel_mode,El->grade, WinCount);
			}
		}
	}
}

void AB2LobbyGameMode::ModRewardBoxDetailInfo(FB2GetDuelRewardBoxGainableListPtr BoxInfo)
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto BoxGradeUI = UIManager->OpenUI<UB2UIModRewardBoxDetailInfo>(UIFName::ModRewardBoxDetail);

		if (BoxGradeUI)
		{
			BoxGradeUI->SetPanelClear();
			BoxGradeUI->SetRewardCount(BoxInfo->reward_item_count);
			BoxGradeUI->GradeEvent(BoxInfo->grade);
			for (int32 i = 0 ; i <BoxInfo->reward_items.Num() ; ++i)
			{
				BoxGradeUI->SetRewardIcon(BoxInfo->reward_items[i]->reward_id, BoxInfo->reward_items[i]->min_count, BoxInfo->reward_items[i]->max_count, i);
			}
		}
	}
}

void AB2LobbyGameMode::ErrorWrongCharacterNickname()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditWrongName")),
		0.f,
		true,
		true
		);
}

void AB2LobbyGameMode::CheckAndPopupKakaoPromotion()
{
	// 프로모션 웹뷰. 이건 일반적으로 사용하는 BeginPlay 류의 메인 쓰레드 로딩 타이밍이 아니라 비동기 GMP 로딩 영상이 끝나길 기다렸다가 띄운다. 
	// 엔진에서 바로 띄우는 게 아니므로 메인 쓰레드 블로킹이 안 먹힘.
	if (
		bLobbyLoadingScreenCompleted && 
		!IsViewPromotion() && // 최초 구현 시 만들어둔 조건.. 그냥 static 전역 변수 플래근데.. 
#if PLATFORM_ANDROID && PLATFORM_IOS
		IsInitPurchases() &&	// 인앱 결제 초기화 후 프로모션 뷰 띄워 주도록
#endif
		(UB2UIManager::GetInstance()->GetCurrUIScene() != EUIScene::Title) && // 타이틀 UI 를 잠시 스쳤다가 바로 다른 맵을 로딩하게 되는 경우가 있어서 패스.
		BladeIIGameImpl::GetClientDataStore().HasValidUserNickName() // 로그인되지 않았다면 아마 Title UI 를 넘어가지 못했겠지만 추가 안전 장치로서..
		)
	{
		B2P_ShowKakaoPromotionView();
		SetViewPromotion(true);
		bEnableLobbyScreenRotate = true;
	}
}

void AB2LobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

//	if (IsInPreRenderPhase() || IsFirstTimePreRenderPending())
//	{ // Pre-render 와 관련해서 메인 맵 로딩 화면 닫는 걸 지연시키게 되므로 여기서 처리. 로딩 뺑글이 연장 차원
//		ConditionalWaitForLoadingScreen();
//	}
//
//	TickTimeTouchAction(DeltaSeconds);
//
//	if (LobbyUIManager && !LobbyUIManager->IsPendingKill())
//	{
//		LobbyUIManager->UpdateManual(DeltaSeconds);
//	}
//
//#if !UE_BUILD_SHIPPING
//	extern bool GTest_GenerateDummyChatMessage;
//	if (GTest_GenerateDummyChatMessage)
//	{
//		DevOnlyGenerateDummyChat(DeltaSeconds);
//	}
//#endif
//	CheckAndPopupKakaoPromotion();
}

void AB2LobbyGameMode::NotifyActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	LobbySceneManager.OnLobbyGMActorCustomEvent(OptionalEventName, OptionalNotifyingObject);
}

void AB2LobbyGameMode::SetEnableClickEvents(bool bEnable)
{
	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
	//if (PlayerController)
	//	PlayerController->bEnableClickEvents = PlayerController->bEnableTouchEvents = bEnable;
}

void AB2LobbyGameMode::RealtimeUpdateUIDoc()
{
	Super::RealtimeUpdateUIDoc();

	// 이벤트를 여러 곳에서 캐치해야 한다면 여기다 넣어서 버그 부담을 줄일 수 있지만 연산량이 많은 것은 가급적 넣지 않도록 한다.
	// 버그가 생긴다 해도 UI 표시가 잠시 잘못되는 정도이다.
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	const EPCClass PCClassEnum = IntToPCClass(PCI);
	//	const int32 PCLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(PCClassEnum);
	//	const int32 ExpPercent = (int32)BladeIIGameImpl::GetLocalCharacterData().GetCharacterExpPercent(PCClassEnum);
	//	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
	//	if (DocHero)
	//	{
	//		DocHero->SetCurrentLevel(PCLevel);
	//		DocHero->SetCurrentExpPercent(ExpPercent);
	//	}
	//}

	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->SetGold(BladeIIGameImpl::GetClientDataStore().GetGoldAmount());
		DocUser->SetGem(BladeIIGameImpl::GetClientDataStore().GetGemAmount());
		DocUser->SetSocialPoint(BladeIIGameImpl::GetClientDataStore().GetSocialPoint());
		DocUser->SetFamePoint(BladeIIGameImpl::GetClientDataStore().GetFamePoint());
	}
}

void AB2LobbyGameMode::OnUpdateCombatPower(EPCClass UpdatedClass, bool PopupShow)
{
	if (UpdatedClass == EPCClass::EPC_End)
		return;

	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(int32(UpdatedClass));
	if (DocHero)
	{
		// 여기서 쓰는 CombatStat 얻어오는 함수들은 연산량이 많으므로 이벤트 시점에만 호출하도록
		DocHero->SetAttackPower(CombatStatEval::GetPCAttack(UpdatedClass));
		DocHero->SetDefensePower(CombatStatEval::GetPCDefense(UpdatedClass));
		DocHero->SetHealthPoint(CombatStatEval::GetPCHealth(UpdatedClass));

		// 전투력은 추가적인 팝업 애니메이션 처리
		int32 BeforeCP = DocHero->GetCombatPower();
		int32 AfterCP = CombatStatEval::GetPCCombatPower(UpdatedClass);
		
		DocHero->SetPreCombatPower(BeforeCP);
		DocHero->SetCombatPower(AfterCP);

		if (BeforeCP != AfterCP && BeforeCP != 0)
		{
			DocHero->SetChangedCombatPower(true);
		}

		if (PopupShow)
		{
			OnShowBattleScoreNotify(UpdatedClass);
		}
	}
}

void AB2LobbyGameMode::OnShowBattleScoreNotify(EPCClass EPC)
{ // UI 출력 시점때문에 출력부분만 따로 만들었음.
	if (EPC == EPCClass::EPC_End) return;

	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(int32(EPC));
	if (DocHero)
	{
		int32 BeforeCP = DocHero->GetPreCombatPower();
		int32 AfterCP = DocHero->GetCombatPower();

		if (BeforeCP != AfterCP && BeforeCP != 0)
		{
			// 전투력 변경 팝업 출력
			UB2UIBattleScoreNotify* BattleScoreNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIBattleScoreNotify>(UIFName::BattleScoreNotify);
			if (BattleScoreNotifyUI != nullptr)
			{
				BattleScoreNotifyUI->SetAmount(AfterCP, BeforeCP, EPC);
			}
		}
	}
}

void AB2LobbyGameMode::NotifyRoomInviteMessage(const FString& Inviter, const FString& InviteKey, const FString& RoomId, int32 MapId, int32 Step)
{
	// 참여정보를 갱신하기 위해서 먼저 요청을 한다.
	data_trader::Retailer::GetInstance().RequestGetRaid();

	if (CanReceiveGameInvite(Inviter, EB2GameMode::Raid))
	{
		if(UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			DocRaid->SetRaidInvitationInfo(Inviter, RoomId, MapId, Step, FDateTime::UtcNow());
			OnReceiveInvitation(Inviter, EB2GameMode::Raid);
		}
	}
}

void AB2LobbyGameMode::SetPCSelection(EPCClass InMainPCClass, EPCClass InSubPCClass)
{
//	BII_CHECK(InMainPCClass != InSubPCClass);
	if (InMainPCClass == InSubPCClass && InMainPCClass != EPCClass::EPC_End)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("메인캐릭터와 태그 캐릭터를 동일하게 설정할 수 없습니다. 컴퓨터가 곧 폭발한다."))
			));
#endif 
		return;
	}

	/*BII_CHECK(InMainPCClass != EPCClass::EPC_End);
	BII_CHECK(InSubPCClass != EPCClass::EPC_End);
	if (InMainPCClass == EPCClass::EPC_End || InSubPCClass == EPCClass::EPC_End)
		return;*/

	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetPCSelection"));

	BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(InMainPCClass);
	BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(InSubPCClass);

	//UI도큐먼트에 MainChar와 SubChar를 알려야함
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->SetIsCurPCMain(true); // 로비에서의 선택은 언제나 current 가 main 인 것으로 간주. 인게임에서만 달라질 수 있다.
		DocBattle->SetCurPCClass(PCClassToInt(InMainPCClass));
		DocBattle->SetTagPCClass(PCClassToInt(InSubPCClass));
	}
		
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{ // 전투 입장 화면일 경우 세팅이 필요한 정보
		DocBS->SetBattleDifficulty(BladeIIGameImpl::GetStageDataStore().GetRelativeStageDifficulty(DocBS->GetServerStageId(), InMainPCClass, InSubPCClass)
			);
	}

	// 이게 대체로 UI 쪽에서 불리는 거라 좀 순환되는 삘이긴 한데 여하간 다시 UI 쪽으로 변경 통지.
	LobbyPCSelectionChangedClass<EPCClass>::GetInstance().Signal(InMainPCClass);
	SetHeroMgmtModePCSelection(InMainPCClass);
	LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Signal(InMainPCClass, InSubPCClass);
}

bool AB2LobbyGameMode::SetMainPCClass(EPCClass InMainPCClass)
{
	BII_CHECK(InMainPCClass != EPCClass::EPC_End);
	if (InMainPCClass == EPCClass::EPC_End)
		return false;

	if (InMainPCClass == BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass())
		return false;

	if (InMainPCClass == BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass())
		SetPCSelection(InMainPCClass, BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	else
		SetPCSelection(InMainPCClass, BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	return true;
}

bool AB2LobbyGameMode::SetMainPCClassBP(EPCClass InMainPCClass)
{
	return SetMainPCClass(InMainPCClass);	
}

void AB2LobbyGameMode::SceneForceUpdate()
{
	LobbySceneManager.ForceUpdate();
}

void AB2LobbyGameMode::SetCharacterIntroModePCSelection(EPCClass InPCClass)
{
	LobbyCharIntroSelectClass<EPCClass>::GetInstance().Signal(InPCClass);
}

void AB2LobbyGameMode::RequsetFindAccount(int64 InAccountId, EPCClass PCClass1, EPCClass PCClass2, EPCClass PCClass3)
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();

	if (!SomeDoc)
		return;

	SomeDoc->SetFindUserAccountID(InAccountId);

	data_trader::Retailer::GetInstance().RequestFindAccount(InAccountId, PCClass1, PCClass2, PCClass3);
}

void AB2LobbyGameMode::HeroTransientEffectBegin(const int32 Index)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HeroTransientEffectBegin"));

	LobbySceneManager.HeroTransientEffectBegin(Index);
}

bool AB2LobbyGameMode::HasMatchStarted() const
{
	if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart)
		return false;

	return (GetMatchState() == NetMatchState::RequestMatchMaking || 
		GetMatchState() == NetMatchState::WaitToMeetCondition ||
		GetMatchState() == NetMatchState::PreparePlayers);
}

void AB2LobbyGameMode::SetMatchState(FName NewState)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetMatchState"));
	
	Super::SetMatchState(NewState);
}

void AB2LobbyGameMode::HandleSuccessToMatchmake(data_trader::ChannelPtr ch, uint32 peer_role, const TArray<FB2RoomParticipantPtr>& participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleSuccessToMatchmake"));

	Super::HandleSuccessToMatchmake(ch, peer_role, participants);

	LobbySceneManager.NotifySuccessMatchMake(IsHost());
}

void AB2LobbyGameMode::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleRequestMatchMaking"));

	AllPeerInfos.Empty();
	LobbySceneManager.HandleRequestMatchMaking();

	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		UIDocLobbyMatch->ClearRemotePlayerInfoData();

	Super::HandleRequestMatchMaking();
}

void AB2LobbyGameMode::HandleNotifyLeave(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleNotifyLeave"));

	Super::HandleNotifyLeave(NetId);
	
	if (MatchState == NetMatchState::StartResult ||
		MatchState == MatchState::LeavingMap ||
		MatchState == MatchState::Aborted)
		return;

	if (MatchState != MatchState::InProgress && MatchState != NetMatchState::PreparePlayers)
	{
		RemovePeerPlayer(NetId);
		RemotePlayerInfoData.Remove(NetId);
	}
	else if (MatchState == NetMatchState::PreparePlayers)
	{
		// 180213_YJ 레이드 매칭 카운트일때 탈주 추가 처리
		if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
		{
			SendLeavePlayerOnlyRaid(NetId);
		}
	}

	LobbySceneManager.HandleNotifyLeave(NetId);
}

void AB2LobbyGameMode::SetMyPlayerInfoData(uint32 Netid, const FB2FindAccountInfoPtr& info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetMyPlayerInfoData"));

	Super::SetMyPlayerInfoData(Netid, info);

	SetMatchState(NetMatchState::WaitToMeetCondition);

	LobbySceneManager.SetMyPlayerInfoData(Netid, info);
}

void AB2LobbyGameMode::SetRemotePlayerInfoData(uint32 Netid, int64 AccountId, const FB2FindAccountInfoPtr& info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetRemotePlayerInfoData"));

	// 파티장(호스트)일 경우 현재 접속한 유저에게 현재있는 사람들의 선택된 캐릭터를 넘긴다.
	// 해당 패킷을 전달받은 유저는 접속한 유저들을 셋팅하고, 자신의 선택한 캐릭터를 전체 유저에게 알린다(브로드캐스팅)
	if (GetPeerType() == PEER_SUPER)
	{
		packet::ByteStream	payload;

		// Host
		payload << GetMyNetId();

		// 캐릭터 정보
		payload << static_cast<uint8>(RemotePlayerInfoData.Num() + 1) <<
			GetMyNetId() << (uint8)BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
		for (auto& Elem : RemotePlayerInfoData)
			payload << Elem.Key << (uint8)Elem.Value.GetMainPlayerClass();

		FString encoded_string = packet::FinalizePacket(packet::SEND_ROOM_PLAYERCLASS, GetMyNetId(), 0, packet::ALLBUTME, payload);
		data_trader::ChannelManager::GetInstance().SendMessage(Netid, encoded_string);
	}

	Super::SetRemotePlayerInfoData(Netid, AccountId, info);
	
	EmplacePeerPlayer(Netid);

	LobbySceneManager.SetRemotePlayerInfoData(Netid, info);

	if (AllPeerInfos[Netid].ReservedPCClass != EPCClass::EPC_End)
		SetRemotePlayerClass(Netid, AllPeerInfos[Netid].ReservedPCClass);
}

void AB2LobbyGameMode::SetRoomParticipant(const TArray<FB2RoomParticipantPtr>& Participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetRoomParticipant"));

	//BII_CHECK(Participants.Num() <= GetMaxMatchPlayerNum());

	for (auto& Participant : Participants)
	{
		if (Participant->id == GetMyNetId())
			continue;

		SetRemotePlayerInfoData(Participant->id, Participant->account_id, Participant->account_info);
	}
}

void AB2LobbyGameMode::HandleNotifyKickUser()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleNotifyKickUser"));

	LobbySceneManager.HandleNotifyKickUser();

	LeaveRoomIfJoined();
}

void AB2LobbyGameMode::HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleNotifyRoomClose"));

	Super::HandleNotifyRoomClose(RoomToken, RoomType);

	if (MatchState != MatchState::InProgress)
	{
		// 룸 클로즈하면서 토큰 재설정은 레이드에서만.
		if (RoomType == b2network::B2RoomType::BRT_Raid)
		{
			// 자기자신 셋팅
			SetToken(RoomToken);
		}

		SendPrerequisiteToStart(RoomType);
	}
}

void AB2LobbyGameMode::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::OnHostChanged"));
	
	uint32 BeforeHostId = GetHostId();

	Super::OnHostChanged(RoomType, NewHostId);

	LobbySceneManager.OnHostChanged(RoomType, NewHostId);

	if (HasMatchStarted() && (GetMyNetId() == NewHostId))
	{
		// 내꺼도 추가해줘야함. 호스트가 아니었으면
		// HandleWaitToMeetCondition() 요기서 추가 못했을듯.
		IncreasePeerPlayers(GetMyNetId());

		// 매칭과정에서 Host가 가져야하는 정보를 재설정해준다.
		for (auto PeerInfo : AllPeerInfos)
			IncreasePeerPlayers(PeerInfo.Key);
	}

	if (MatchState == NetMatchState::PreparePlayers)
	{
		// 180213_YJ 레이드 매칭 카운트일때 탈주 추가 처리
		if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
		{
			SendLeavePlayerOnlyRaid(BeforeHostId);
		}
	}

	// 비호스트 유저 룸클로즈이후 시작하기까지 대기중인지 플래그. 이사이 호스트 인계받았으면 방깬다.
	if (bWaitPrerequisiteToStartForNonHost)
	{
		NoResponsePrerequisiteToStart();
	}
}


void AB2LobbyGameMode::HandleWaitToMeetCondition()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleWaitToMeetCondition"));

	BII_CHECK(!RoomWhereIAm.IsEmpty());
	BII_CHECK(GetMyNetId() != 0);

	IncreasePeerPlayers(GetMyNetId());
}

void AB2LobbyGameMode::HandlePreparePlayers()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandlePreparePlayers"));

	bWaitPrerequisiteToStartForNonHost = false;

	LobbySceneManager.HandlePreparePlayers();
}

void AB2LobbyGameMode::LeaveRoomIfJoined()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandlePreparePlayers"));

	LobbySceneManager.LeaveRoomIfJoined();

	// 준비확인 타이머 제거
	ClearWaitPrerequisiteToStartTimeHandler();

	bWaitPrerequisiteToStartForNonHost = false;

	Super::LeaveRoomIfJoined();
}


bool AB2LobbyGameMode::CanRequestRoomCloseReason(ERoomCloseReasonType RoomCloseReason)
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::CanRequestRoomCloseReason"));

	if (RoomCloseReason == ERoomCloseReasonType::NONE)
		return false;

	return (RoomCloseReason == GetStartRoomCloseReason() || RoomCloseReason == GetBreakRoomCloseReason());
}

ERoomCloseReasonType AB2LobbyGameMode::GetStartRoomCloseReason()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::GetStartRoomCloseReason()"));

	return LobbySceneManager.GetStartRoomCloseReason();
}

int32 AB2LobbyGameMode::GetMatchModeMapId()
{
	return LobbySceneManager.GetMatchModeMapId();
}

int32 AB2LobbyGameMode::GetMatchModeDifficulty()
{
	return LobbySceneManager.GetMatchModeDifficulty();
}

void AB2LobbyGameMode::HandleFinishLobbyMatch()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleFinishLobbyMatch"));

	// 매칭 후 레벨스트리밍 으로인해 데이터가 날라가게되므로 DocLobby에 매칭관련 정보를 저장
	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		UIDocLobbyMatch->SetPeerType(GetPeerType());
		UIDocLobbyMatch->SetRoomId(GetRoomId());
		UIDocLobbyMatch->SetHostId(GetHostId());
		UIDocLobbyMatch->SetRoomToken(GetToken());
		UIDocLobbyMatch->SetMyNetId(GetMyNetId());	
		//UIDocLobbyMatch->ClearRemotePlayerInfoData();

		for (FB2NetMatchPeerInfo& PeerInfo : ReservedPlayerClasses)
		{
			if (PeerInfo.NetId == GetMyNetId())
				UIDocLobbyMatch->SetMyStartIndex(PeerInfo.PlayerStartIndex);
		}

		UIDocLobbyMatch->MatchedRemotePlayerSkillAnims.Empty();
		for (auto& Elem : RemotePlayerInfoData)
		{
			uint8 StartIndex = 0;

			for (FB2NetMatchPeerInfo& PeerInfo : ReservedPlayerClasses)
			{
				if (Elem.Key == PeerInfo.NetId)
					StartIndex = PeerInfo.PlayerStartIndex;
			}

			UIDocLobbyMatch->SetRemotePlayerInfoData(Elem.Key, Elem.Value.GetAccountId(), Elem.Value.GetAccountInfoPtr(), Elem.Value.GetMainPlayerClass(), StartIndex);

			//////////////////////////////			
			{ // 맵 로딩 도중 SkillAnim 의 Pre-load 에 쓰기 위한 정보. 해당 Pre-load 시점에 새로 로딩되는 게임모드에서 미처 RemotePlayerInfoData 가 없는 상황이라 여기에 저장해 둔다.
				TArray<ESkillAnimType> SkillAnimTypesOfThisRemote;
				UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, Elem.Value.GetMainPlayerClass(),
					///// 다음 게임모드는 아마도 Raid 나 Control 이 될텐데 뭔지 알 수가 없군. 안에 들어가보면 사실 일부 스킬을 특별하게 사용하는 GameMode 타입에 해당하지만 않으면 문제는 없는데 좀 거슬리긴 하지.
					EB2GameMode::Raid,
					/////
					SkillAnimTypesOfThisRemote, &Elem.Value);
				// 모은 걸로 CombinedID 를 만들어 넣는다.
				for (ESkillAnimType ThisSkillAnimType : SkillAnimTypesOfThisRemote)
				{
					UIDocLobbyMatch->MatchedRemotePlayerSkillAnims.AddUnique(FCombinedPCSkillAnimID(Elem.Value.GetMainPlayerClass(), ThisSkillAnimType));
				}
			}
			//////////////////////////////
		}
	}	

	// 주의 :	이후 GameMode가 변경되면 NotifyRoomLeave가 호출되는데 이때 RoomId가 존재하면 서버에 룸을 나가는 패킷을 보낸다. 
	//			이를 방지하기위해 반드시 룸을 초기화해줘야함
	ClearMatchRoom();

	LobbySceneManager.HandleFinishLobbyMatch();
}

void AB2LobbyGameMode::CheckFullPeerRoom()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CheckFullPeerRoom"));

	if (IsMatchMaxPlayerNumOver())
	{
		NoResponsePrerequisiteToStart();
	}

	return Super::CheckFullPeerRoom();
}

void AB2LobbyGameMode::SendPrerequisiteToStart(int32 RoomType)
{
	// 호스트용 ReservedPlayerClasses 설정
	FB2PeerInfo MyPeerInfo;
	TArray<FB2PeerInfo> PeerInfos;
	
	MyPeerInfo.NetId = GetMyNetId();
	MyPeerInfo.Class = uint8(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	MyPeerInfo.PlayerStartIndex = 1;
	
	//if (AllPeerInfos[MyPeerInfo.NetId].PlayerTeamType != ETeamType::ETT_End)
	//	MyPeerInfo.PlayerStartIndex = GetPlayerStartIndexByNetId(MyPeerInfo.NetId);
	if (RoomType == b2network::B2RoomType::BRT_Assault)
	{
		MyPeerInfo.MatchGrade = UB2UIDocHelper::GetDocControl()->GetBattleGrade();
		MyPeerInfo.MatchStar = UB2UIDocHelper::GetDocControl()->GetBattleStarCount();

		if (IsActiveControlTutorial() && AllPeerInfos.Contains(MyPeerInfo.NetId))
		{
			MyPeerInfo.PlayerTeamType = AllPeerInfos[MyPeerInfo.NetId].PlayerTeamType;
		}
	}

	PeerInfos.Add(MyPeerInfo);

	uint8 RemotePlayerStartIndex = 2;

	for (auto& Elem : RemotePlayerInfoData)
	{
		FB2PeerInfo RemotePeerInfo;
		RemotePeerInfo.NetId = Elem.Key;
		RemotePeerInfo.Class = uint8(RemotePlayerInfoData[Elem.Key].GetMainPlayerClass());
		RemotePeerInfo.PlayerStartIndex = RemotePlayerStartIndex;
		
		//if (AllPeerInfos[RemotePeerInfo.NetId].PlayerTeamType != ETeamType::ETT_End)
		//	RemotePeerInfo.PlayerStartIndex = GetPlayerStartIndexByNetId(RemotePeerInfo.NetId);
		if (RoomType == b2network::B2RoomType::BRT_Assault)
		{
			RemotePeerInfo.MatchGrade = Elem.Value.GetAccountInfoPtr()->assault_match_grade;
			RemotePeerInfo.MatchStar = Elem.Value.GetAccountInfoPtr()->assault_match_star_count;
			if (IsActiveControlTutorial() && AllPeerInfos.Contains(RemotePeerInfo.NetId))
			{
				RemotePeerInfo.PlayerTeamType = AllPeerInfos[RemotePeerInfo.NetId].PlayerTeamType;
			}
		}

		PeerInfos.Add(RemotePeerInfo);
		RemotePlayerStartIndex++;
	}
	
	if (RoomType == b2network::B2RoomType::BRT_Assault)
	{
		if (IsActiveControlTutorial())
		{
			GetTutorialControlPlayerInfo(PeerInfos);
		}
		else
		{// 팀, 위치 세팅
			GetControlPlayerInfo(PeerInfos);
		}
	}

	for (FB2PeerInfo& Elem : PeerInfos)
	{
		FB2NetMatchPeerInfo NetMatchPeerInfo;
		NetMatchPeerInfo.NetId = Elem.NetId;
		NetMatchPeerInfo.Class = Elem.Class;
		NetMatchPeerInfo.PlayerStartIndex = Elem.PlayerStartIndex;
		NetMatchPeerInfo.PlayerTeamType = (uint8)Elem.PlayerTeamType;
		ReservedPlayerClasses.Add(NetMatchPeerInfo);

		if (AllPeerInfos.Contains(Elem.NetId))
		{
			AllPeerInfos[Elem.NetId].PlayerTeamType = Elem.PlayerTeamType;
		}
	}

	// 자신을 제외하고 파티원에게 시작을 알림
	packet::ByteStream	payload;
	FString	strRoomToken(GetToken());

	payload << RoomType << strRoomToken <<
		static_cast<uint8>(ReservedPlayerClasses.Num());

	for (FB2NetMatchPeerInfo& PeerInfo : ReservedPlayerClasses)
	{
		payload << PeerInfo.NetId << PeerInfo.Class << PeerInfo.PlayerStartIndex << (uint8)PeerInfo.PlayerTeamType;
	}

	FString encoded_string = packet::FinalizePacket(packet::SEND_PREREQUISITE_TO_START, GetMyNetId(), 0, packet::ALLBUTME, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);

	bool bStandaloneStart = false;
	const bool IsAlone = ReservedPlayerClasses.Num() == 1;

	if (IsAlone)
	{
#if !UE_BUILD_SHIPPING
		bStandaloneStart |= GetMaxMatchPlayerNum() == 1;
#endif
		bStandaloneStart |= IsForceStartMatch(GetCurrentLobbyScene());
	}

	// 튜토리얼
	if (IsActiveControlTutorial())
		bStandaloneStart = true;


	if (bStandaloneStart)
		SetPreparePlayers();

	else
		GetWorldTimerManager().SetTimer(WaitPrerequisiteToStartTimeHandler, this, &AB2LobbyGameMode::NoResponsePrerequisiteToStart, 3.0f);
}

void AB2LobbyGameMode::StartLobbyMatch(bool IsRequestMatchMaking)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::StartLobbyMatch"));

	ClearMatchRoom();
	SetMatchState(IsRequestMatchMaking ? NetMatchState::RequestMatchMaking : NetMatchState::WaitMatchMaking);
}

void AB2LobbyGameMode::StartFinishLobbyMatch()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::StartFinishLobbyMatch"));

	SetMatchState(NetMatchState::FinishLobbyMatch);
}

void AB2LobbyGameMode::HandleNotifyNoResponsePrerequisiteToStart()
{
	// 채널나가기
	// LeaveRoomIfJoined 로비씬 매니저에서 매칭창 닫아준다.
	LeaveRoomIfJoined();

	// 팝업띄우기용
	LobbySceneManager.ReceiveNoResponsePrerequisiteToStart();
}

void AB2LobbyGameMode::BreakRoom(int32 InMapId /*= 1*/, int32 InDifficulty /*= 1*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::BreakRoom"));

	if (GetMatchState() != NetMatchState::StartResult)
	{
		// 매칭과정에서 방을 깰것인지 체크
		if (HasMatchStarted())
		{
			if (GetPeerType() == PEER_SUPER)
			{
				// 서버로 더 이상 들어오지 못하게하는 패킷을 전송
				UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
				data_trader::Retailer::GetInstance().RequestRoomClose(GetRoomId(), InMapId, GetBreakRoomCloseReason()); // 타입 변경해주자.

				// 모든 유저를 강퇴시킨다.
				packet::ByteStream	payload;
				FString encoded_string = packet::FinalizePacket(packet::KICK_USER, GetMyNetId(), 0, packet::ALLBUTME, payload);
				data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);				
			}
		}
	}
}

void AB2LobbyGameMode::IncreasePeerPlayers(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IncreasePeerPlayers"));

	if (GetPeerType() != PEER_SUPER)
	{
		if(GetMyNetId() == NetId)
			EmplacePeerPlayer(NetId);
		return;
	}

	if (MatchState == MatchState::InProgress)
		return;

	EmplacePeerPlayer(NetId);
		
	// 룸이 꽉찾는지 체크
	CheckFullPeerRoom();
}

void AB2LobbyGameMode::NetSendBattleGradeInfo(int32 BattleGrade, int32 BattleStarCount)
{
	// 내꺼 점령전 등급 채널내 방송
	packet::ByteStream	payload;

	payload << BattleGrade << BattleStarCount;

	FString encoded_string = packet::FinalizePacket(packet::BATTLE_GRADE_INFO, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage(encoded_string);
}

void AB2LobbyGameMode::NetSendMatchToken()
{
	packet::ByteStream	payload;
	payload << TokenToPlay;

	FString encoded_string = packet::FinalizePacket(packet::MATCH_TOKEN, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage(encoded_string);
}

void AB2LobbyGameMode::NetAckBattleGradeInfo(uint32 NetId, int32 BattleGrade, int32 BattleStarCount)
{
	UB2UIDocHelper::GetDocLobbyMatch()->SetGradeInfoData(NetId, BattleGrade, BattleStarCount);
}

void AB2LobbyGameMode::SetOtherToken(uint32 InNetId, const FString& MatchToken)
{
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		DocLobbyMatch->SetTokenData(InNetId, MatchToken);
	}
}

void AB2LobbyGameMode::NetSendMatchPlayerTeamInfo()
{
	packet::ByteStream	payload;

	payload << AllPeerInfos.Num();

	// 캐릭터 정보
	for (TPair<uint32, FLobbyMatchPeerInfo>& peerInfo : AllPeerInfos)
		payload << peerInfo.Key << (uint8)peerInfo.Value.PlayerTeamType;

	FString encoded_string = packet::FinalizePacket(packet::ROOM_PLAYER_TEAM_INFO, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2LobbyGameMode::NetReceiveMatchPlayerTeamInfo(TMap<uint32, uint8>& TeamInfos)
{
	for (TPair<uint32, uint8>& TeamInfo : TeamInfos)
	{
		EmplacePeerPlayer(TeamInfo.Key);
	}

	LobbySceneManager.ReceiveMatchPlayerTeamInfo();
}

void AB2LobbyGameMode::OnDeliveryRoomJoin(FB2ResponseRoomJoinPtr Message)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::OnDeliveryRoomJoin"));

	if (Message->room_type == b2network::B2RoomType::BRT_Raid)
	{
		OnDeliveryRoomJoin_Raid(Message);
	}
	else
	{
		Super::OnDeliveryRoomJoin(Message);
	}
}

void AB2LobbyGameMode::OnDeliveryRoomJoin_Raid(FB2ResponseRoomJoinPtr Message)
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	auto* UIManager = UB2UIManager::GetInstance();

	const bool InRaidScene = UIManager->IsUIScene(EUIScene::RaidMain);
	if (InRaidScene)
	{
		if (DocRaid && DocRaid->GetIsReceiveInvite())
		{
			// 레이드 페이지 내에서 초대를 받았음.
			if (auto* UIRaidMain = UIManager->GetUI<UB2UIRaidMain>(UIFName::Raid))
			{
				auto* DocBattle = UB2UIDocHelper::GetDocBattle();
				auto* UIDocRaid = UB2UIDocHelper::GetDocRaid();
				if (DocBattle && UIDocRaid)
				{
					int32 RaidStep = UIDocRaid->GetLastRaidStep();
					int32 RaidType = UIDocRaid->GetLastRaidType();
					EPCClass PcClass = IntToPCClass(DocBattle->GetCurPCClass());
					UIRaidMain->ForcedStartGame(RaidType, RaidStep, ERaidJoinType::MakeRoom, PcClass, false);
				}
			}
		}
	}
	else
	{
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_RaidMain);
	}

	auto EndPoint = std::make_shared<b2network::B2Endpoint>();
	FString NextRoomId(TEXT(""));

	const TArray<b2network::B2RoomPropertyPtr>& Properties = Message->properties;
	for (int32 PropIdx = 0; PropIdx < Properties.Num(); PropIdx++)
	{
		if (auto RoomPropPtr = Properties[PropIdx])
		{
			if (RoomPropPtr->key == TEXT("port"))
				EndPoint->port = FCString::Atoi(*RoomPropPtr->value);

			else if (RoomPropPtr->key == TEXT("address"))
				EndPoint->host = RoomPropPtr->value;

			else if (RoomPropPtr->key == TEXT("nextRoomId"))
				NextRoomId = RoomPropPtr->value;
		}
	}

	if (EndPoint && DocRaid)
		DocRaid->SetPartyRetryInfo(EndPoint, NextRoomId);

	UB2UIManager::GetInstance()->OpenUI(UIFName::RaidJoin);
}

void AB2LobbyGameMode::PlayMainLobbyBgm()
{
	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_MainLobby);
}

void AB2LobbyGameMode::SetRemotePlayerClass(uint32 NetId, EPCClass PCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetRemotePlayerClass"));

	// 아직 서버로 부터 매칭이 되지 않았다면 PCClass를 예약 걸어둔다. (Reserved PCClass)
	if (RemotePlayerInfoData.Contains(NetId) == false)
	{
		EmplacePeerPlayer(NetId, PCClass);
		return;
	}

	Super::SetRemotePlayerClass(NetId, PCClass);

	IncreasePeerPlayers(NetId);
	LobbySceneManager.SetRemotePlayerClass(NetId, PCClass, RemotePlayerInfoData[NetId].GetAccountInfoPtr());
}

void AB2LobbyGameMode::ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const& PeerInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ResponsePrerequisiteToStart"));

	for (FB2NetMatchPeerInfo const& PeerInfo : PeerInfos)
	{
		EmplacePeerPlayer(PeerInfo.NetId, static_cast<ETeamType>(PeerInfo.PlayerTeamType));
	}

	Super::ResponsePrerequisiteToStart(RoomType, RoomToken, PeerInfos);

	bWaitPrerequisiteToStartForNonHost = true;
}

void AB2LobbyGameMode::ResponseAckPrerequisiteToStart(uint32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ResponseAckPrerequisiteToStart"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	EmplacePeerPlayer(InNetId, true);

	Super::ResponseAckPrerequisiteToStart(InNetId);

	if (IsAlreadyStarted())
		ClearWaitPrerequisiteToStartTimeHandler();
}

void AB2LobbyGameMode::ResponseChangeRoomAccess(uint8 AccessKey)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ResponseChangeRoomAccess"));

	Super::ResponseChangeRoomAccess(AccessKey);

	LobbySceneManager.ResponseChangeRoomAccess(AccessKey);
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	if (AllPeerInfos.Contains(InNetId))
		return;

	AllPeerInfos.Emplace(InNetId, FLobbyMatchPeerInfo());
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId, bool IsReady)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	EmplacePeerPlayer(InNetId);
	AllPeerInfos[InNetId].IsReady = IsReady;
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId, EPCClass InReservedPCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	EmplacePeerPlayer(InNetId);
	AllPeerInfos[InNetId].ReservedPCClass = InReservedPCClass;
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId, ETeamType InTeamTpye)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	EmplacePeerPlayer(InNetId);
	AllPeerInfos[InNetId].PlayerTeamType = InTeamTpye;
}

void AB2LobbyGameMode::RemovePeerPlayer(int32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RemovePeerPlayer"));

	if (AllPeerInfos.Contains(InNetId) == false)
		return;

	AllPeerInfos.Remove(InNetId);
}

bool AB2LobbyGameMode::CanRequestRoomClose()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::CanRequestRoomClose"));

	if (IsForceStartMatch(GetCurrentLobbyScene()))
		return true;

	return IsMatchMaxPlayerNum();
}

bool AB2LobbyGameMode::CanStartInGame()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CanStartInGame"));

	if (IsMatchMaxPlayerNum() || IsForceStartMatch(GetCurrentLobbyScene()))
	{
		bool bCanStart = true;

		for (auto PeerInfo : AllPeerInfos)
		{
			// 자기자신은 제외
			if (PeerInfo.Key == GetMyNetId())
				continue;

			if (PeerInfo.Value.IsReady == false)
				bCanStart = false;
		}

		return bCanStart;
	}

	return false;
}

ERoomCloseReasonType AB2LobbyGameMode::GetBreakRoomCloseReason()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::GetBreakRoomCloseReason"));

	return LobbySceneManager.GetBreakRoomCloseReason();
}

int32 AB2LobbyGameMode::GetMatchPlayerNum()
{
	return AllPeerInfos.Num();
}

int32 AB2LobbyGameMode::GetMatchPlayerNum(ETeamType teamType)
{
	int32 nCount = 0;

	for (TPair<uint32, FLobbyMatchPeerInfo>& peerInfo : AllPeerInfos)
	{
		if (peerInfo.Value.PlayerTeamType == teamType)
			nCount++;
	}

	return nCount;
}

int32 AB2LobbyGameMode::GetMaxMatchPlayerNum()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::GetMaxMatchPlayerNum"));
	
	return LobbySceneManager.GetMaxMatchPlayerNum();
}

bool AB2LobbyGameMode::IsPlayBeginAction()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IsMatchMaxPlayerNum"));

	return LobbySceneManager.IsPlayingBeginAnim();
}
void AB2LobbyGameMode::ResetPlayBeginActionState()
{
	LobbySceneManager.SetPlayingBeginAnim(true); // 다시 플레이 가능하게 하는 거. 로비 첨 진입할 때 두번 필요해서.
}

void AB2LobbyGameMode::SendLeavePlayerOnlyRaid(uint32 NetId)
{
	// 180213_YJ 레이드 매칭 카운트일때 탈주 추가 처리
	if (GetPeerType() == PEER_SUPER)
	{
		/*
		#if !UE_BUILD_SHIPPING
		if (NetFakeConfigure::GetInstance().GetHateRaidPenalty()) return; // 커맨드 치면 탈주 제제 무시.
		#endif*/

		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
			DocRaid->SetAlreadyExitUser(true);

		for (auto& Elem : RemotePlayerInfoData)
		{
			if (Elem.Key == NetId)
			{
				data_trader::Retailer::GetInstance().RequestRoomLeaveResult(GetRoomId(), Elem.Value.GetAccountId(), GetToken());
				break;
			}
		}
	}
}

bool AB2LobbyGameMode::IsMatchMaxPlayerNum()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IsMatchMaxPlayerNum"));

	return (GetMatchPlayerNum() == GetMaxMatchPlayerNum());
}

bool AB2LobbyGameMode::IsMatchMaxPlayerNumOver()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IsMatchMaxPlayerNumOver"));

	return (GetMatchPlayerNum() > GetMaxMatchPlayerNum());
}

bool AB2LobbyGameMode::IsForceStartMatch(ELobbyScene LobbySceneType)
{
	if (LobbySceneType == ELobbyScene::ELobbyScene_RaidMain)
	{
		auto* DocRaid = UB2UIDocHelper::GetDocRaid();
		return DocRaid && DocRaid->GetForceStartRaid();
	}

	return false;
}

ETeamType AB2LobbyGameMode::GetMyTeamType()
{
	if (AllPeerInfos.Num() == 0)
		return ETeamType::ETT_End;

	return AllPeerInfos[GetMyNetId()].PlayerTeamType;
}

bool AB2LobbyGameMode::IsCurrentUIScene(EUIScene CheckScene)
{
	return UB2UIManager::GetInstance()->IsUIScene(CheckScene);
}

bool AB2LobbyGameMode::RequestStartGameStageWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty, const TArray<EStageBuffType>& StageBuffTypes, bool bSkipStoryCinematic)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RequestStartGameStageWithSelectedPC"));

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		RepeatBattleRequestState.bRepeatOne = DocBS->GetRepeatBattleLoopOne();
		RepeatBattleRequestState.bRepeatAll = DocBS->GetRepeatBattleLoopAll();
		RepeatBattleRequestState.bBoostOn = DocBS->GetRepeatBattleBoostOn();
		RepeatBattleRequestState.RemainingBoostCount = DocBS->GetRepeatBattleBoostRemainCount();

		if (!RepeatBattleRequestState.bIsOn && (DocBS->GetRepeatBattleLoopOne() || DocBS->GetRepeatBattleLoopAll()))
		{
			RepeatBattleRequestState.bIsOn = true;
			RepeatBattleRequestState.CurrentRepeatCount = 0;
		}
		if (!DocBS->GetRepeatBattleLoopOne() && !DocBS->GetRepeatBattleLoopAll())
		{
			RepeatBattleRequestState.bIsOn = false;
			RepeatBattleRequestState.CurrentRepeatCount = 0;
		}
	}

	int32 MainPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	int32 SubPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	auto& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	StageData.RequestStartStage(ClientStageId,
								StageDifficulty,
								BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(),
								BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass(),
								StageBuffTypes,
								RepeatBattleRequestState,
								RepeatBattleRequestState.bIsOn ? true : bSkipStoryCinematic,
								MainPCPresetId,
								SubPCPresetId); // 추가 요청 있기 전에는 반복전투선 시나리오 연출 아닌 숏버전 연출로. RequestStartRepeatBattleWithSelectedPC 에서도..

	return true;
}

bool AB2LobbyGameMode::RequestStartRepeatBattleWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RequestStartRepeatBattleWithSelectedPC"));

	// 각각의 반복전투 스테이지 진입이 아닌 맨 처음 진입점임.
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// 값을 세팅한 다음 서버에서 응답이 오면 인게임 구조체로 전달할 것임.
	RepeatBattleRequestState.bIsOn = true;
	RepeatBattleRequestState.CurrentRepeatCount = 0;
	if (DocBS)
	{
		RepeatBattleRequestState.bRepeatOne = DocBS->GetRepeatBattleLoopOne();
		RepeatBattleRequestState.bRepeatAll = DocBS->GetRepeatBattleLoopAll();
		RepeatBattleRequestState.bBoostOn = DocBS->GetRepeatBattleBoostOn();
		RepeatBattleRequestState.RemainingBoostCount = DocBS->GetRepeatBattleBoostRemainCount();
	}

#if !UE_BUILD_SHIPPING
	extern int32 GCheat_RepeatBattleCountOverride;
	if (GCheat_RepeatBattleCountOverride > 0)
	{
		RepeatBattleRequestState.CurrentRepeatCount = GCheat_RepeatBattleCountOverride;
		GCheat_RepeatBattleCountOverride = 0;
	}
#endif
	SyncRepeatBattleRequestStateToUIDoc();

	TArray<EStageBuffType> SelectedBuffs; // 반복전투에서도 버프 아이템 선택이 있다면 받아오도록..
	if (DocBS)
	{
		SelectedBuffs = DocBS->GetSelectedStageBuffs();
	}

	return RequestStartGameStageWithSelectedPC(ClientStageId, StageDifficulty, SelectedBuffs, true); // 추가 요청 있기 전에는 반복전투선 시나리오 연출 아닌 숏버전 연출로
}

void AB2LobbyGameMode::CancelOrStopRepeatBattle()
{ // 반복 전투로 입장하려는데 인벤토리가 차거나 뭔가 부족하거나 등의 이유로 못하게 되었을 때..
	// 내지는 스테이지 입장 요청하기 전에는 단순히 이걸로 반복전투 중단이 됨.
	RepeatBattleRequestState.bIsOn = false;
	RepeatBattleRequestState.CurrentRepeatCount = 0;
	RepeatBattleRequestState.bRepeatAll = false;
	RepeatBattleRequestState.bRepeatOne = false;
	//RepeatBattleRequestState.MultiplyingFactor = 1;
	SyncRepeatBattleRequestStateToUIDoc();
}

void AB2LobbyGameMode::SetupForNextRepeatBattleStage(FServerStageID InServerStageId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetupForNextRepeatBattleStage"));

	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	// 각각의 반복전투 스테이지 준비 셋업. 이전에도 반복전투를 치루고 왔고 추가 반복전투도 남아있는 상태
	// 필요한 대부분의 것들은 UI 에서 처리하고.. 여기선 반복전투를 더 이상 진행하지 못하는 경우에 대한 사전 체크를 하도록 하자..		
	const bool bIsRepeatBattlePossible = StageDataStore.IsPossibleToDoRepeatBattle(InServerStageId);
	if (!bIsRepeatBattlePossible && !(StageDataStore.IsRepeatBattleLocked(InServerStageId) && RepeatBattleRequestState.bRepeatOne))
	{
		// 일단 중단부터 하고..
		CancelOrStopRepeatBattle();
		// 메시지라도 뿌려줘야겠지..
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_CannotContinueRepeatBattle")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm,
				FMsgPopupOnClick::CreateLambda([this](){
				}),
				FMsgPopupOnClick::CreateLambda([this](){
				})
			);
		}
	}
}

void AB2LobbyGameMode::SyncRepeatBattleRequestStateToUIDoc()
{
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetRepeatBattleCurrentOn(RepeatBattleRequestState.bIsOn);
		DocBS->SetRepeatBattleCurrentCount(RepeatBattleRequestState.CurrentRepeatCount);
		DocBS->SetRepeatBattleLoopAll(RepeatBattleRequestState.bRepeatAll);
		DocBS->SetRepeatBattleLoopOne(RepeatBattleRequestState.bRepeatOne);
		DocBS->SetRepeatBattleBoostOn(RepeatBattleRequestState.bBoostOn);
	}
}

void AB2LobbyGameMode::DJLegacy_ChangeLobbyUIPage(ELobbyUIPages InPageEnum)
{
	// 결국은 LobbyUISwitcher 의 BeginLobbyUIPage 콜인데, 전후로 이쪽서 상태 변화 등 필요한 것이 있으면 처리.
	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(InPageEnum);
}

void AB2LobbyGameMode::DJLegacy_OpenLobbySubPopup(ELobbySubPopups InPopupEnum, FDJLegacySubpopupAddPanelInfo InOptionalAddPanelInfo)
{
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(InPopupEnum, InOptionalAddPanelInfo);
}

void AB2LobbyGameMode::DJLegacy_CloseLobbySubPopup(ELobbySubPopups InPopupEnum)
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(InPopupEnum);
}

UB2LobbyUserWidget* AB2LobbyGameMode::DJLegacy_FindLobbySubPopup(ELobbySubPopups InUserWidgetEnum)
{
	return LobbyUIManager ? LobbyUIManager->DJLegacy_FindLobbySubPopup(InUserWidgetEnum) : nullptr;
}
UB2LobbyUserWidget* AB2LobbyGameMode::DJLegacy_GetCurrLobbyUIPage()
{
	return LobbyUIManager ? Cast<UB2LobbyUserWidget>(LobbyUIManager->GetCurrActiveUI()) : nullptr;
}
void AB2LobbyGameMode::DJLegacy_AsyncLoadUIPage(ELobbyUIPages InLoadUIPage, bool bBlocking)
{
	if (LobbyUIManager)
		LobbyUIManager->TryAsyncLoadOpenUI(InLoadUIPage, bBlocking);
}

EPCClass AB2LobbyGameMode::GetHeroMgmtModePCClass()
{
	return LobbySceneManager.GetHeroMgmtModePCClass();
}

void AB2LobbyGameMode::EnterHeroMgmtMode(EHeroMgmtSubMode InSubMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterHeroMgmtMode"));

	bHeroMgmtMode = true;

	//////////////////////////////////////////////////
	// DJLegacy 와 신 UI 연동 임시 처리.. 일걸로 생각했으나 영웅관리 모드는 계속 DJLegacy 로 남을 듯.
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	const EUIScene CurrentUIScene = UIMgrInst->GetCurrUIScene();
	if (CurrentUIScene != EUIScene::None && /*!IsDJLegacyUIScene(CurrentUIScene) && */ // UIInvenQuickSlide 와 같은 경우 DJLegacyUIScene 이 연속될 수 있음. 어떻게 보면 그 경우는 히스토리 무시하는 게 나을 수도 있긴 한데.. 어떤 경우는 오동작을 하게 되니..
		InSubMode != EHeroMgmtSubMode::EHMSM_End && !UIMgrInst->IsOpeningDJLegacyHeroMgmtBySceneHistory())
	{
		// Back button 클릭 시 (UB2UIManager::ChangeUISceneBack) 별도 UIManager 시스템의 이전 UIScene 으로 돌아갈 수 있도록 따로 히스토리를 넣어준다.
		// 그러나 Back button 클릭으로 여기에 온 경우는 제외.
		UIMgrInst->ChangeUIScene(DJLegacyHeroMgmtPageToUIScene(InSubMode));
	}
	//////////////////////////////////////////////////

	//HeroMgmtModePCSelection = BladeIIGameImpl::GetClientDataStore().GetMainPlayerClass(); // 기본적으로 로비에서 선택된 PC 를 넘겨줌.
		
	OnQuitHeroMgmtSubModeCommon(HeroMgmtSubMode); // 이전 sub 모드 닫는 처리도. InSubMode 와 같더라도 아직 새로 오픈 전이니 닫는 처리를 해 봄. 문제가 된다면 좀 더 검사를 강화.

	HeroMgmtSubMode = InSubMode;
	ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(HeroMgmtSubMode);
	if (MatchingDjLegacyUIPage != ELobbyUIPages::ELUP_End)
	{
		DJLegacy_ChangeLobbyUIPage(MatchingDjLegacyUIPage);
		LobbyInventoryViewJustOpenClass<>::GetInstance().Signal();

		// 혹여나 각 submode 별로 특수한 동작이 필요하면 부끄러워하지 말고 여기에 if 나 switch 넣어서 처리....
		// 이전에 실제로 그렇게 되어 있었다. 낄낄
	} 

	// 영웅 관리 모드 전용 카메라워크
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroManagement);
}

void AB2LobbyGameMode::OnChangeUIScene(EUIScene UIScene)
{
	if (BGMManager)
		BGMManager->PlayBGMByUIScene(UIScene);
}

void AB2LobbyGameMode::OnChangeUISceneBack()
{
	if (BGMManager)
	{
		BGMManager->PlayBGMByPrevUIScene();
	}
}

void AB2LobbyGameMode::QuitHeroMgmtMode()
{
	// 이게 DJLegacy 가 되다보니 실제 바꾸는 건 다른 경로를 통하고 여기는 부가적인 처리를 하게 되었음.

	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::QuitHeroMgmtMode"));

	bHeroMgmtMode = false;

	DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_End); 
		
	OnQuitHeroMgmtSubModeCommon(HeroMgmtSubMode);

	RequestCombatPower();
}

void AB2LobbyGameMode::OnQuitHeroMgmtSubModeCommon(EHeroMgmtSubMode SubModeToClose)
{
	switch (SubModeToClose)
	{
	case EHeroMgmtSubMode::EHMSM_Inventory:
		LobbyInventoryViewJustCloseClass<>::GetInstance().Signal();
		break;
	case EHeroMgmtSubMode::EHMSM_Brevet:
		break;
	case EHeroMgmtSubMode::EHMSM_Skill:
		break;
	case EHeroMgmtSubMode::EHMSM_Wing:
		break;
	case EHeroMgmtSubMode::EHMSM_Relic:
		break;
	case EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE:
		break;
	}

	UB2Airport::FlushPendingWingVisibilityReq(); // 날개 visibility on/off 한 거 모아서 서버로 요청
	BladeIIGameImpl::GetClientDataStore().RequestSyncToServerShowHead(); //머리 Visible 관련 서버 동기화

	// 장비 장착이나 스킬 강화 등에 따라 아린의 조언도 체크
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
}

void AB2LobbyGameMode::RequestCombatPower()
{
	bool bCheckCombatPower = CheckCombatPower();
	if (bCheckCombatPower)
	{
		TArray<b2network::B2CharacterPowerPtr> CharacterPowerArray;

		for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
		{
			auto* DocHero = UB2UIDocHelper::GetDocHero(i);
			if (DocHero)
			{
				auto CharacterPower = std::make_shared<b2network::B2CharacterPower>();
				CharacterPower->character_type = CliToSvrPCClassType(IntToPCClass(i));
				CharacterPower->total_power = DocHero->GetCombatPower();
				CharacterPowerArray.Add(CharacterPower);

				DocHero->SetChangedCombatPower(false);
			}
		}

		data_trader::Retailer::GetInstance().RequestCheckReachPower(CharacterPowerArray);
	}
}

bool AB2LobbyGameMode::CheckCombatPower()
{
	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		auto* DocHero = UB2UIDocHelper::GetDocHero(i);
		if (DocHero)
		{
			if (DocHero->GetChangedCombatPower())
				return true;
		}
	}

	return false;
}

void AB2LobbyGameMode::SetHeroMgmtModePCSelection(EPCClass InPCClass)
{	
	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(InPCClass);
}

class UB2LobbyInventory* AB2LobbyGameMode::GetLobbyInventory()
{
	// 가급적 CreateLobbyInventory 시점 이전에 GetLobbyInventory 를 하는 일이 없도록..
	checkSlow(LobbyInventory);
	return LobbyInventory;
}

bool AB2LobbyGameMode::CreateLobbyInventory()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateLobbyInventory"));

	// LobbyInventory 의 많은 인터페이스들이 static 이지만 여전히 객체 생성을 좀 한다.
	LobbyInventory = NewObject<UB2LobbyInventory>(this, UB2LobbyInventory::StaticClass(), NAME_None);

	if (LobbyInventory)
	{
		LobbyInventory->InitLobbyInventory(this);
		return true;
	}
	return false;
}

UB2Airport* AB2LobbyGameMode::GetLobbyAirport()
{
	// 가급적 CreateLobbyAirport 시점 이전에 GetLobbyAirport 를 하는 일이 없도록..
	checkSlow(LobbyAirport);
	return LobbyAirport;
}

bool AB2LobbyGameMode::CreateLobbyAirport()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateLobbyAirport"));

	LobbyAirport = NewObject<UB2Airport>(this, UB2Airport::StaticClass(), NAME_None);
	if (LobbyAirport)
	{
		LobbyAirport->InitAirportLobby(this);
		return true;
	}
	return false;
}

class UB2PCClassInfo* AB2LobbyGameMode::GetCurrentSelectedPCClassInfo()
{
	UB2PCClassInfoBox* TheBox = StaticFindPCClassInfoBox(this);
	return TheBox ? TheBox->GetSingleClassInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass()) : NULL;
}

void AB2LobbyGameMode::UpdateChapterData()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::UpdateChapterData"));

	// 만일 UI Doc 에 아이템이 들어간다면 여기서 넣지는 말 것. 데이터가 많을 것이므로 LobbyInventory 같은 데서 특수하게 처리.
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	if (SelectedChapterViewNum == 0)
		SelectedChapterViewNum = StageDataStore.GetLastPlayedActId();
	if (SelectedStageDifficulty == EStageDifficulty::ESD_None)
		SelectedStageDifficulty = StageDataStore.GetLastPlayedStageDifficulty();

	if (SelectedChapterViewNum == 0 || SelectedStageDifficulty == EStageDifficulty::ESD_None)
		return;

	UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo();
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	FSingleChapterInfoData* ChapterInfoData = ChapterInfoTable ? ChapterInfoTable->GetInfoData(SelectedChapterViewNum) : NULL;
	UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter();

	if (DocChapter && ChapterInfoTable && StageInfoTable)
	{
		int32 ChapterOpenedNum = StageDataStore.GetActIdByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty));
		int32 ChapterMaxNum = FMath::Min(ChapterInfoTable->GetAllChapterNum(), StageInfoTable->GetChapterNumByClientStageId(StageInfoTable->GetFinalClientStageId()));
		FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)SelectedStageDifficulty));

		DocChapter->SetChapterOpenedNum(ChapterMaxNum); // 자, 현재로선 그냥 최대 챕터까지 스크롤은 할 수 있도록 함. 물론 각 스테이지는 잠겨 있을 것.	// 기존 : DocChapter->SetChapterOpenedNum(ChapterOpenedNum);
		DocChapter->SetChapterMaxNum(ChapterMaxNum);
		DocChapter->SetMaxOpenedStageDifficulty(ChapterMaxNum >= SelectedChapterViewNum ? StageDataStore.GetMaxOpenedStageDifficulty() : StageDataStore.GetMaxOpenedNewStageDifficulty(SelectedChapterViewNum));
		DocChapter->SetCurChapterNum(SelectedChapterViewNum);
		DocChapter->SetCurStageDifficulty(SelectedStageDifficulty);
		DocChapter->SetCurChapterName(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()),
			ChapterInfoTable->GetLocalizedChapterName(SelectedChapterViewNum)));

		// UI상으로 알려주기 위해 Next도 표시
		DocChapter->SetNextChapterNum(StageDataStore.GetActIdByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty)));
		DocChapter->SetNextSubChapterNum(StageInfoTable->GetSubChapterNumByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty)));
	}
}

void AB2LobbyGameMode::SetCombatEntryChapterNum(int32 InChapterNum, EStageDifficulty InDifficulty)
{
	SelectedChapterViewNum = InChapterNum;
	SelectedStageDifficulty = InDifficulty;

	UpdateChapterData();

	// 챕터 변경 시 같은 스트리밍 레벨에 묶는 단위에 따라 레벨 스트리밍을 새로 해 주어야 할 수 있다. 
	ChangeLobbyScene(ELobbyScene::ELobbyScene_Chapter, SelectedChapterViewNum);
	LobbySceneManager.ForceUpdate();
}

void AB2LobbyGameMode::GoGameStageInfo(FServerStageID InServerStageId, FRepeatBattleStateSet& RepeatBattleSet)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GoGameStageInfo"));

	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	// 반복전투 스테이지 완료하고 여전히 남았을 때 여기로 돌아온다. (AB2StageGameMode::OnEndPlayOfSingleRepeatBattleStage)
	// 반복전투를 여전히 진행해야 한다면 남은 반복 카운트가 하나는 있어야 함.
	BII_CHECK(!RepeatBattleSet.bIsOn || RepeatBattleSet.CurrentRepeatCount > 0);
	
	const bool bIsRepeatBattlePossible = StageDataStore.IsPossibleToDoRepeatBattle(InServerStageId);
	if (!bIsRepeatBattlePossible && !(StageDataStore.IsRepeatBattleLocked(InServerStageId) && RepeatBattleSet.bRepeatOne))
	{
		RepeatBattleRequestState = FRepeatBattleStateSet();
	}
	else
	{
		RepeatBattleRequestState = RepeatBattleSet;
	}
	if(!RepeatBattleRequestState.bIsOn)
		RepeatBattleRequestState.CurrentRepeatCount = 0;
	if (RepeatBattleRequestState.bIsOn && RepeatBattleRequestState.CurrentRepeatCount < 0){ // 혹시 몰라서
		RepeatBattleRequestState.CurrentRepeatCount = 0;
	}
	SyncRepeatBattleRequestStateToUIDoc();

	//>>==============선택된 스테이지로 보낸다. 일단 테스트니 여기 두었는데 실제로는 서버로 Request하고 응답왔을때 처리 해야 될듯
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetClientStageId(StageDataStore.GetClientStageId(InServerStageId));
		DocBS->SetStageDifficulty(StageDataStore.GetStageDifficulty(InServerStageId));
		DocBS->SetServerStageId(InServerStageId);
		
		//todo StageAreaName을 테이블로 관리하도록 수정하여 여기서 세팅하도록 - 그 전까지는 빈칸으로
		DocBS->SetLocalizedAreaName(FText::FromString(FString(TEXT(""))));

		//todo 인벤토리를 뒤져서 나오면 넣도록.
		// 			DocBS->SetBuffItemPlayFast(FIntPoint(1000100, 10));
		// 			DocBS->SetBuffItemExpIncrease(FIntPoint(1000101, 0));
		// 			DocBS->SetBuffItemGoldIncrease(FIntPoint(1000102, 5));
		// 			DocBS->SetBuffItemAutoSkill(FIntPoint(1000103, 1));

		TArray<FB2RewardItemPreviewInfo> RewardItemInfo;
		StageDataStore.GetStageRewardPreviewItems(InServerStageId, RewardItemInfo);
		StageDataStore.GetStageRewardPreviewItemsOfSameType(RewardItemInfo);
		DocBS->SetRewardItemList(RewardItemInfo);
		DocBS->SetBossDropItemsList(StageDataStore.GetStageFixedDrops(InServerStageId.Id));

		TArray<FB2AetherItemPreviewInfo> AetherItemInfo;
		StageDataStore.GetStageAetherPreviewItems(InServerStageId, AetherItemInfo);
		DocBS->SetAetherItemList(AetherItemInfo);

		TArray<FB2MaterialItemPreviewInfo> MaterialItemInfo;
		StageDataStore.GetStageMaterialPreviewItems(InServerStageId, MaterialItemInfo);
		DocBS->SetMaterialItemList(MaterialItemInfo);

		DocBS->SetRepeatMissionReward(FIntPoint(FItemRefIDHelper::GetGoodsID_Gold(), 0));

		int32 OutRepeatBattleCostRefId, OutRepeatBattleCostCount, OutRequireStamina;
		StageDataStore.GetStageCostData(InServerStageId, OutRepeatBattleCostRefId, OutRepeatBattleCostCount, OutRequireStamina);

		DocBS->SetRepeatBattleCost(FIntPoint(OutRepeatBattleCostRefId, OutRepeatBattleCostCount));
		DocBS->SetRequiredStaminaNum(OutRequireStamina);
		
		int32 RecommendPower;
		StageDataStore.GetStageSuggestedCombatStats(InServerStageId, RecommendPower);
		DocBS->SetRecommendPower(RecommendPower);
		
		int32 RecommendAttack;
		int32 RecommendDefense;
		StageDataStore.GetStageRecommendAbility(InServerStageId, RecommendAttack, RecommendDefense);
		DocBS->SetRecommendAttack(RecommendAttack);
		DocBS->SetRecommendDefense(RecommendDefense);

		DocBS->SetBattleDifficulty(StageDataStore.GetRelativeStageDifficulty(InServerStageId, ClientDataStore.GetLocalCharacterData().GetMainPlayerClass(), ClientDataStore.GetLocalCharacterData().GetSubPlayerClass()));
	}

	GameStageCheckForArinConsulting(InServerStageId); // BattleStageInfo 페이지 열기 전에 아린 조언 체크도 완료.

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::BattleStageInfo);
	//<<==============선택된 스테이지로 보낸다.

	if (RepeatBattleRequestState.bIsOn) // RepeatBattle 도중 스테이지 완료 시에도 이 루트를 통해 보냄.
	{
		SetupForNextRepeatBattleStage(InServerStageId);
	}
}

void AB2LobbyGameMode::GoGameStageInfoLastReached()
{
	/*int32 ChapterNumber = 0;
	int32 StageNumber = 0;
	EStageDifficulty StageDifficulty = EStageDifficulty::ESD_None;

	BladeIIGameImpl::GetStageDataStore().LoadRecentlyStageInfo(ChapterNumber, StageNumber, StageDifficulty);

	if (StageNumber == 0 || ChapterNumber == 0 || StageDifficulty == EStageDifficulty::ESD_None)
	{
	StageNumber = 1;
	ChapterNumber = 1;
	StageDifficulty = EStageDifficulty::ESD_Normal;
	}*/
	//최근 플레이한 스테이지 정보
	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();
	FServerStageID ServerStageID;
	EStageDifficulty CurDiff = EStageDifficulty::ESD_Normal;
	int32 CurChapter = 1;


	auto DocChapter = Cast<UB2UIDocChapter>(UB2UIDocHelper::GetDocChapter());
	

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	if (StageInfoTable == nullptr)
		return;

	if (DocChapter == nullptr)
		return;
	CurChapter = DocChapter->GetCurChapterNum();
	int32 MaxChapterNum = StageInfoTable ? StageInfoTable->GetStartAdventureModeChapterNum() : 0;

	if (CurChapter <= MaxChapterNum)
	{
		if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Normal)) // 노말 모드를 끝까지 진행함
		{
			if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Hard)) // 하드 모드를 끝까지 진행함
			{
				if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_VeryHard)) // 매우 어려움을 끝까지 진행함
				{
					if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Hell)) // 헬을 끝까지 진행함
					{
						ServerStageID = StageStore.GetLastClearServerStageId(EStageDifficulty::ESD_Hell);
					}
					else
					{
						ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_Hell);
					}
				}
				else
				{
					ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_VeryHard);
				}
			}
			else
			{
				ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_Hard);
			}
		}
		else
		{
			ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_Normal);
		}

		
		if (StageStore.GetClientStageId(ServerStageID) > StageInfoTable->GetFinalClientStageId())
			ServerStageID = StageStore.GetServerStageID(StageInfoTable->GetFinalClientStageId() , EStageDifficulty::ESD_Hell);
		
	}
	else
	{
		ServerStageID = StageStore.GetNewLastClearServerStageIdByLastPlayedStageDifficulty(CurChapter);
	}

	CurDiff = StageStore.GetStageDifficulty(ServerStageID);
	CurChapter = StageStore.GetActIdByServerStageId(ServerStageID);

	// 뒤로가기로 돌아왔을때 해당 챕터로 보여주기 위해 미리 DOC세팅
	
	
	DocChapter->SetCurChapterNum(CurChapter);
	DocChapter->SetCurStageDifficulty(CurDiff);
	

	BladeIIGameImpl::GetStageDataStore().RequestGetStageInfo(ServerStageID, true);
}

void AB2LobbyGameMode::GameStageCheckForArinConsulting(FServerStageID InServerStageId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GameStageCheckForArinConsulting"));

	FClientDataStore& ClientDataStorte = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	
	TArray<EPCClass> CharacterClasses;
	CharacterClasses.Add(ClientDataStorte.GetLocalCharacterData().GetMainPlayerClass());
	CharacterClasses.Add(ClientDataStorte.GetLocalCharacterData().GetSubPlayerClass());

	int32 UnitedSkillPoint = 0;
	float UnitedAttackPower = 0.0f;
	float UnitedDefensePower = 0.0f;

	for (auto CharacterClass : CharacterClasses)
	{
		if(EPCClass::EPC_End == CharacterClass)
			continue;

		UnitedAttackPower += CombatStatEval::GetPCAttack(CharacterClass, NULL);
		UnitedDefensePower += CombatStatEval::GetPCDefense(CharacterClass, NULL);
		UnitedSkillPoint += ClientDataStorte.GetLocalCharacterData().GetCharacterLeftSkillPoint(CharacterClass);
	}

	float StageSuggestedAttack = 0.0f;
	float StageSuggestedDefense = 0.0f;
	StageDataStore.GetStageSuggestedCombatStats(InServerStageId, StageSuggestedAttack, StageSuggestedDefense);

	EArinConsultPreCombat_MainSubject FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_None;
	// 어쩌면 메인/서브 중 특히 누가 떨어지는지 따로 저장해야 할 수도. 아린의 조언에서 인벤토리로 넘어갈 때 클래스 선택에 반영해야 할 수도 있으니..
	if (UnitedAttackPower < StageSuggestedAttack)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Attack;
	}
	else if (UnitedDefensePower < StageSuggestedDefense)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Defense;
	}
	else if (UnitedSkillPoint > 5) // 수치를 몇으로(?)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Skill;
	}
	//else if (MainCharSkillPoint > 3 || SubCharSkillPoint > 3) // 둘이 합쳐 3갠지 둘 중 하나만 3갠지 확인..
	//{
	//	FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Skill;
	//}

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetArinConsultingSubject(FinalArinConsultSubject);
	}
}

void AB2LobbyGameMode::GoRaidGameMode(const FString& InvitedRoomId, EPCClass ChangeMainPCClass, int32 RaidType, int32 Step)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GoRaidGameMode"));

	BII_CHECK(!InvitedRoomId.IsEmpty());

	// 방을 만들었을때나, 빠른매칭때는 데이터를 날려주지만 초대받았을경우엔 날려주는 부분이 없어 추가.
	AllPeerInfos.Empty();
	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		UIDocLobbyMatch->ClearRemotePlayerInfoData();

	if (UB2UIDocRaid* UIDocRaid = UB2UIDocHelper::GetDocRaid())
	{
		UIDocRaid->ClearRaidJoinData();
		UIDocRaid->ClearRaidBattleData();
		
		// 선택된 정보 입력
		UIDocRaid->SetLastRaidType(RaidType);
		UIDocRaid->SetLastRaidStep(Step);
		UIDocRaid->SetIsReceiveInvite(true);
		UIDocRaid->SetOpenedRaidJoinType(ERaidJoinType::MakeRoom);
	}

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		DocBattle->SetCurPCClass(PCClassToInt(ChangeMainPCClass));

	// 데이터를 먼저 세팅하고 Request를 보낸후에 Response 받은 이후 UI변경.
	// (주의 : 초대 받았을 경우 RequestRoomMatchMaking을 하지 않고 RequestJoinRoom으로 바로 룸에 입장!)
	TArray<b2network::B2RoomPropertyPtr> properties;
	TArray<b2network::B2RoomCharacterPtr> characters;
	TArray<b2network::B2PresetNumPtr> presetNums;


	auto& LocalCharData = BladeIIGameImpl::GetLocalCharacterData();

	// temporary test code
	for (int i = (int)EPCClass::EPC_Gladiator; i < GetMaxPCClassNum(); ++i) 
	{
		const EPCClass PCClass = static_cast<EPCClass>(i);

		auto b2PresetNum = std::make_shared<b2network::B2PresetNum>();
		b2PresetNum->character_type = CliToSvrPCClassType(IntToPCClass(i));
		b2PresetNum->item_preset_num = static_cast<int32>(LocalCharData.GetPCClassItemPresetSlotID(PCClass));
		b2PresetNum->skill_preset_num = static_cast<int32>(LocalCharData.GetPCClassSkillSlotID(PCClass));
		presetNums.Emplace(b2PresetNum);
	}

	data_trader::Retailer::GetInstance().RequestRequestJoinRoom(InvitedRoomId, b2network::B2RoomType::BRT_Raid, 0, properties, characters, presetNums);
}

void AB2LobbyGameMode::SetItemOpDirectingSceneObjects(class ACameraActor* InCamActor)
{
	LobbySceneManager.SetItemOpDirectingSceneObjects(InCamActor);
}

void AB2LobbyGameMode::EnterItemOpDirectingView(ELobbyInvenItemOpMode InOpMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterItemOpDirectingView"));

	LobbySceneManager.EnterItemOpDirectingView(InOpMode); // LobbySceneManager 를 통해 준비해 놓은 연출용 카메라 뷰로.
}

void AB2LobbyGameMode::QuitItemOpDirectingView()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::QuitItemOpDirectingView"));

	LobbySceneManager.QuitItemOpDirectingView(); // 기존 인벤토리 카메라 뷰로 복귀
}

bool AB2LobbyGameMode::IsInItemOpDirectingView()
{
	return LobbySceneManager.IsInItemOpDirectingView();
}

void AB2LobbyGameMode::EnterPVPMainMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterPVPMainMenu"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPOneOnOne);
}

void AB2LobbyGameMode::EnterTeamMatchMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterTeamMatchMenu"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamMatchUI);
}

void AB2LobbyGameMode::EnterControlMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterControlMenu"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlMain);
}

void AB2LobbyGameMode::EnterTMChageHeroMod()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterTMChageHeroMod"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamHeroChangeUI);
}

#include "B2UIStore.h"
#include "B2UISummonItem.h"
#include "B2UIMagicStore.h"
void AB2LobbyGameMode::EnterShop(int32 nWhereShop)
{ 
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{ // 자잘한 팝업이 뜬 상태에서 상점 진입하는 경우가 생길 수 있어서 그거 처리.
		UIMgr->CloseAllStandaloneUIs();
	}

	switch (static_cast<EStorePageWhere>(nWhereShop))
	{
	case EStorePageWhere::Store:
	{
		auto* StoreUI = UIMgr->GetUI<UB2UIStore>(UIFName::Store);
		if (StoreUI)
			StoreUI->SetTab();
		else
			data_trader::Retailer::GetInstance().RequestGetGeneralShop();
	}
		break;
	case EStorePageWhere::EquipStore:
	{
		auto* SummonItemUI = UIMgr->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
		if (SummonItemUI)
			SummonItemUI->SetTab();
		else
			data_trader::Retailer::GetInstance().RequestGetLotteryShop();
	}
		break;
	case EStorePageWhere::PackageStore:
	{
		data_trader::Retailer::GetInstance().RequestGetPackageShop();
	}
		break;
	case EStorePageWhere::MagicStore:
	{
		auto* MagicStoreUI = UIMgr->GetUI<UB2UIMagicStore>(UIFName::MagicStore);
		if (MagicStoreUI)
			MagicStoreUI->SetItemList();
		else
			data_trader::Retailer::GetInstance().RequestGetMagicShop();
	}
		break;

	}
}

//////////////////////////////////////////////////
// 대체로 Lobby UI 에서 불리게 되는 서버와의 연계 기능들.

void AB2LobbyGameMode::LoginAndStartGame(const FString& InUserID, const FString& InUserPW)
{
	//DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_CharSel); // 바로 다음 페이지로 이동.
}

void AB2LobbyGameMode::CreateAccount(const FString& InUserID, const FString& InUserPW)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateAccount"));

	//if (InUserID.IsEmpty())
	//{
	//	// TODO : 계정명을 입력해주세요~ 팝업 메세지
	//	UE_LOG(LogBladeII, Log, TEXT("Please enter your nickname!!"));
	//}
	//else
	//{
	//	auto B2MessageInfo = std::make_shared<b2network::FB2MessageInfoRequestSignUp>
	//	(
	//		[this, InUserID, InUserPW](FB2MessageInfoPtr messageInfo)
	//		{
	//			// 계정 생성이 성공했으므로 다시 접속 시도를 하자
	//			LoginAndStartGame(InUserID, InUserPW);
	//		},
	//		[this](FB2MessageInfoPtr messageInfo)
	//		{
	//			b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
	//			// TODO 에러 코드 처리
	//		}
	//	);
	//	B2MessageInfo->name = TCHAR_TO_ANSI(*InUserID);
	//	FB2NetworkBridge::GetInstance()->SendB2Message(B2MessageInfo);
	//}
}

void AB2LobbyGameMode::ProceedOnCharacterSelection()
{ // 캐릭터 인트로 창에서의 선택 이후. 뭘 실제로 선택한 건 아닌데 걍 처음에 캐릭터 인트로가 뭔가 선택하는 삘이라 이름이 이렇게 되었음.
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
}

#include "Engine/LevelStreaming.h"
void AB2LobbyGameMode::AddHallOfFrameCharacter(const TArray<FHallOfFameCharacterInfo>& HallOfFameCharacter)
{
	LobbySceneManager.AddHallOfFrameCharacter(HallOfFameCharacter);
}

void AB2LobbyGameMode::AddHallOfCharacterAnimation(EPCClass PCClass, class UAnimationAsset* PCAnimation)
{
	LobbySceneManager.AddHallOfCharacterAnimation(PCClass, PCAnimation);
}

void AB2LobbyGameMode::AddLevelToLobbyScene(ELobbyLevelType LobbyType, class ULevelStreaming* StreamingLevel)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::AddLevelToLobbyScene"));
	if (StreamingLevel)
	{
		LobbySceneManager.AddLevelLobbyScene(LobbyType, StreamingLevel->GetWorldAssetPackageFName());
	}	
}

void AB2LobbyGameMode::ClearMatchRoom()
{
	Super::ClearMatchRoom();

	AllPeerInfos.Empty();
}

void AB2LobbyGameMode::ClearInvitationGameInfo(EB2GameMode GameModeType)
{
	Super::ClearInvitationGameInfo(GameModeType);

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if(auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			DocPVP->ClearInvitationData();
	}
}

void AB2LobbyGameMode::RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType)
{
	// Request to server
	Super::RequestGameInvite(NaickName, GameModeType);

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (UB2UICommonInvite* InviteUI = UIManager->OpenUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnRequestGameInvite(NaickName, GameModeType);

		UIManager->CloseUI(UIFName::RaidInviteList);
	}

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
		{
			DocPVP->SetIsFriendshipGame(true);
			DocPVP->SetInviterForceCancel(false);
		}
	}
}

void AB2LobbyGameMode::RequestDeclineInvitation(EB2GameMode GameModeType)
{
	Super::RequestDeclineInvitation(GameModeType);

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
		const bool bWaitingAccept = DocLobbyMatch && DocLobbyMatch->GetInvitationState() == EInvitationState::WaitingAccept;

		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			DocPVP->SetInviterForceCancel(true);
	}
}

// CallOwner = 초대(수신)자
void AB2LobbyGameMode::PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType)
{
	// InviteGame UI에서 수락은 했지만 캐릭터 선택 이후 실제 수락 Request를 보냄
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (GameModeType == EB2GameMode::PVP_Tag && DocPVP)
	{
		// CharacterSelectPage 이동 전 Setting되어 있어야 함
		DocPVP->SetIsFriendshipGame(true);
		DocPVP->SetIsInvitee(true);
		DocPVP->SetIsInviterReady(true);
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_PVPCharacterSelectPage);
		
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			if (auto* SelectUI = UIManager->GetUI<UB2UIPVPCharacterSelectPage>(UIFName::PVPCharacterSelectPage))
				SelectUI->OnPrevAcceptInvitation();
		}

		if(DocPVP->GetRewardBoxStatus() == nullptr)
			data_trader::Retailer::GetInstance().RequestGetMatchStatus();

	}

	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (GameModeType == EB2GameMode::Raid && DocRaid)
	{
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			auto* RaidInvitePopupUI = UIManager->OpenMsgPopup<UB2UIMsgPopupSelectPCClass>(EUIMsgPopup::SelectPCClass,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")), FText::GetEmpty(),
				0.0f,
				true,
				true,
				EUIMsgPopupButtonGroup::AcceptOrReject);

			// RequestRaidStart 이름은 레이드 시작이지만, 서버팀에서 프로토콜 재사용으로
			// 레이드 초대시에 어떤 캐릭터를 선택했는지 알리고, 리스폰스를 받은 후에 룸조인.
			if (RaidInvitePopupUI != nullptr)
			{
				RaidInvitePopupUI->SetAutoCloseOnClick(false); // 리스폰스를 처리한 후에 닫아야함
				RaidInvitePopupUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this, RaidInvitePopupUI]() {
					data_trader::Retailer::GetInstance().RequestRaidStart(CliToSvrPCClassType(RaidInvitePopupUI->GetSelectPCClass()));
				}));
			}
		}
	}
}

bool AB2LobbyGameMode::CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const
{
	if(Super::CanReceiveGameInvite(Inviter, GameModeType))
	{
		auto* DocRaid = UB2UIDocHelper::GetDocRaid();
		// 이미 방에 들어가 있다는 의미
		if (DocRaid && DocRaid->HasValidPartyRetryInfo())
			return false;

		return true;
	}

	return false;
}

void AB2LobbyGameMode::OnReceiveInvitation(const FString& Inviter, EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
		ClearInvitationGameInfo(EB2GameMode::Raid);
	else if (GameModeType == EB2GameMode::Raid)
		ClearInvitationGameInfo(EB2GameMode::PVP_Tag);

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (UB2UICommonInvite* InviteUI = UIManager->OpenUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnReceiveInvitation(Inviter, GameModeType);

		// LobbyMain이 Hide 상태라면
		if (UIManager->GetCurrUIScene() == EUIScene::LobbyMain)
		{
			auto* DocUICondition = UB2UIDocHelper::GetDocUICondition();
			if(DocUICondition && DocUICondition->GetHideAllLobbyMainUIs())
				DocUICondition->SetHideAllLobbyMainUIs(false);
		}
	}
}

void AB2LobbyGameMode::OnInvitationSuccess(const FString& Invitee, EB2GameMode GameModeType, const FString& InvitationRef)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
		auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
		auto* UIManager = UB2UIManager::GetInstance();
		if (DocPVP == nullptr || UIManager == nullptr)
			return;

		DocPVP->SetIsInviteeReady(true);
		DocPVP->SetIsInviterReady(true);
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_PVPCharacterSelectPage);

		if (auto* CharSelectUI = UIManager->GetUI<UB2UIPVPCharacterSelectPage>(UIFName::PVPCharacterSelectPage))
			CharSelectUI->OnInvitationSuccess();

		UIManager->CloseUI(UIFName::InviteGame);
		UIManager->CloseUI(UIFName::RaidInviteList);
	}
}

void AB2LobbyGameMode::OnInvitationDeclined(const FString& Invitee, EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		const FString MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();
		const bool IsInvitee = Invitee == MyNickName;

		UB2UIManager* UIManager = UB2UIManager::GetInstance();
		if (UB2UICommonInvite* InviteUI = UIManager->GetUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnReceiveDecline(Invitee, IsInvitee);

		if (IsInvitee)
		{
			if (auto* PVPSelectUI = UIManager->GetUI<UB2UIPVPCharacterSelectPage>(UIFName::PVPCharacterSelectPage))
				PVPSelectUI->OnInvitationDeclineByInvitee();
		}
	}
}

void AB2LobbyGameMode::ChangeLobbyScene(EUIScene ToChangeUIScene, int32 OptionalChapterNum /*= -1*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ChangeLobbyScene"));
	ChangeLobbyScene(LobbySceneManager.GetLobbyScene(ToChangeUIScene), 
		(OptionalChapterNum > 0) ? OptionalChapterNum : SelectedChapterViewNum);
}

void AB2LobbyGameMode::HideActorHasTag(FName Tag, bool bHide)
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		if ((*It)->ActorHasTag(Tag))
			(*It)->SetActorHiddenInGame(bHide);
	}
}

//void AB2LobbyGameMode::SetGuildMapChangeMatinee(class ALevelSequenceActor * Atype, class ALevelSequenceActor * Btype)
//{
//	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_GuildMapUI
//		|| LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_Guild)
//	{
//		LobbySceneManager.SetGuildMapChangeMatinee(Atype, Btype);
//	}
//
//}

void AB2LobbyGameMode::GuildMapChangeMatineeEnd()
{
	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_GuildMapUI
		|| LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_Guild)
	{
		LobbySceneManager.GuildMapChangeMatineeEnd();
	}
}


void AB2LobbyGameMode::ResponseHallOfFameInfo(FB2ResponseGetHallOfFamePtr HallOfFameInfo)
{
	if (LobbySceneManager.IsHallOfFameData(HallOfFameInfo))
	{
		LobbySceneManager.SetHallOfFameInfo(HallOfFameInfo);
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::HallOfFame);
	}
}

void AB2LobbyGameMode::ResponsePraiseTargetRanker(FB2ResponsePraiseTargetRankerPtr PraiseInfo)
{
	LobbySceneManager.SetPraiseInfo(PraiseInfo);
}

void AB2LobbyGameMode::EnterCostumeUIScene(const ECostumePage& eCostumePageType)
{
	FCostumeDataStore& CostumeDataStore = BladeIIGameImpl::GetCostumeDataStore();

	if (CostumeDataStore.GetIsOpenCostume() == false)
	{
		return;
	}

	CostumeDataStore.SetCachedCostumePageType(eCostumePageType);

	ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(EHeroMgmtSubMode::EHMSM_Costume);

	bool bRequetAsyncLoading = BladeIIGameImpl::bAllowUIAsyncLoading && BladeIIGameImpl::bAllowUIOnDemandAsyncLoading;

	DJLegacy_AsyncLoadUIPage(MatchingDjLegacyUIPage, bRequetAsyncLoading);
}

void AB2LobbyGameMode::ResponseCheckReachPower(FB2ResponseCheckReachPowerPtr Message)
{
	if (Message->required_tracking_targets.Num() > 0)
	{
		for (int32 i = 0; i < Message->required_tracking_targets.Num(); ++i)
		{
			EPCClass PCClass = SvrToCliPCClassType(Message->required_tracking_targets[i]);
			
			// Preset
			int32 CurrentItemPresetID = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(PCClass);
			// Skill Slot
			EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(PCClass);
			int32 SkillSlotID = static_cast<int32>(SlotType);

			TArray<b2network::B2CharacterPowerSpecificPtr> CharacterPowerSpecificArray;
			CombatStatEval::GetReuqestPCCombatPower(PCClass, CharacterPowerSpecificArray);

			data_trader::Retailer::GetInstance().RequestTrackingPowerDetail(Message->required_tracking_targets[i], CurrentItemPresetID, SkillSlotID, CharacterPowerSpecificArray);
		}
	}
}

void AB2LobbyGameMode::ResponseReceiveFairyGift(const FB2ReceiveFairyGiftPtr& pReceiveFairyGift)
{
	BladeIIGameImpl::GetClientDataStore().SetNextFairyGiftTime(pReceiveFairyGift->next_fairy_gift_time);

	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager && (pReceiveFairyGift->rewards.Num() > 0))
	{
		auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		if (RewardPopup != nullptr)
		{
			RewardPopup->AddRewardItems(pReceiveFairyGift->rewards);
			RewardPopup->ShowMailDescription(false);
			RewardPopup->SetJackpotReward(pReceiveFairyGift->jackpoted);
			RewardPopup->ShowRewardPopup(true);
		}
		UIManager->CloseUI(UIFName::MsgPopupSelectFairyGift);
	}
	LobbySceneManager.OnLobbyFlyFairy_AttachFX(false);
}

void AB2LobbyGameMode::UpdateFairyGiftState()
{
	const bool IsPossibleFairyGift = BladeIIGameImpl::GetClientDataStore().GetPossibleFairyGift();
	LobbySceneManager.OnLobbyFlyFairy_AttachFX(IsPossibleFairyGift);
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// 맵 로딩 시간 측정하던 거 로비 씬 전환 시간 측정에도 사용
extern void DevCheckLoadingTimeBegin();
extern void DevCheckLoadingTimeEnd();
#endif

void AB2LobbyGameMode::ChangeLobbyScene(ELobbyScene LobbyScene, int32 OptionalChapterNum /*= -1*/)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	DevCheckLoadingTimeBegin();
#endif

	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_HeroManagement ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_FairyMain ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_Guild)
	{
		if (LobbyScene != ELobbyScene::ELobbyScene_HeroManagement)
		{
			QuitHeroMgmtMode();
		}
	}

	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ChangeLobbyScene"));
	SetTimeToIgnoreTouchEnd(-1.f);
	LobbySceneManager.ChangeLobbyScene(LobbyScene, (OptionalChapterNum > 0) ? OptionalChapterNum : SelectedChapterViewNum);	

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal(); // 확인 사살 차원에서

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	DevCheckLoadingTimeEnd();
#endif
}

/** 
 * 성능에 쥐약이 될 수 있어서 강제 업데이트 인자는 제공하지 않는다.
 * 기본적으로 각 캐릭터 클래스 별로 merge 한 메쉬는 장착 상태, 날개 여부에 따라 refresh 를 해야하는지 판별을 함.
 */
void AB2LobbyGameMode::UpdateCharacterParts(EPCClass InPClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::UpdateCharacterParts"));
	LobbySceneManager.UpdateLobbyCharCompForParts(InPClass);
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void AB2LobbyGameMode::DevForceUpdateCharacterParts(EPCClass InPCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::DevForceUpdateCharacterParts"));
	LobbySceneManager.DevForceUpdateLobbyCharCompForParts(InPCClass);
}
#endif

void AB2LobbyGameMode::AnyOtherUpdateOnSetAccount()
{
	LobbySceneManager.ConditionalUpdateLobbyOnHeadDisplayWidget();
	// 기타 서버에서 SetAccount 왔을 때 업데이트 필요한 잡다한 거 있으면 추가
}

bool AB2LobbyGameMode::SelectLobbyMainCharacter(EPCClass SelectedClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SelectLobbyMainCharacter"));
	return LobbySceneManager.SelectLobbyMainCharacter(SelectedClass);
}
//
//void AB2LobbyGameMode::SetDefaultCameraMatinee(class ALevelSequenceActor * DefaultMatinee)
//{
//	LobbySceneManager.SetDefaultCameraMatinee(DefaultMatinee);
//}
//
//void AB2LobbyGameMode::SetChapterViewMatinee(class ALevelSequenceActor * ChapterMatinee)
//{
//	LobbySceneManager.SetChapterViewMatinee(ChapterMatinee);
//}

void AB2LobbyGameMode::UpdateNativeLobbyMatineeRef()
{ // 일반적으로는 스트리밍 레벨 로드 시 BeginPlay 를 할 때에 Matinee 가 세팅됨. 특수한 경우에 직접 업데이트할 때 사용.
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::UpdateNativeLobbyMatineeRef"));
	if (GEngine){
		GEngine->HandleCeCommand(GetWorld(), UPDATE_NATIVE_LOBBY_MATINEE_REF_EVENT, *GLog);
	}	
}

void AB2LobbyGameMode::AddLobbyCharacter(EPCClass PCClass, const FLobbyCharacter& LobbyCharacter)
{
	LobbySceneManager.AddLobbyCharacter(PCClass, LobbyCharacter);
}

void AB2LobbyGameMode::AddLobbyATypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter)
{
	LobbySceneManager.AddLobbyATypeActorInfo(PCClass, LobbyCharacter);
}

void AB2LobbyGameMode::AddLobbyGTypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter)
{
	LobbySceneManager.AddLobbyGTypeActorInfo(PCClass, LobbyCharacter);
}

void AB2LobbyGameMode::AddLobbyNPCCharacter(ELobbyNPCType InType, const FLobbyNPCCharacterInfo& InAddInfo)
{
	LobbySceneManager.AddLobbyNPCCharacter(InType, InAddInfo);
}

void AB2LobbyGameMode::AddLobbyMainScenePointLights(const FLobbyMainScenePointLightInfo& InAddInfo)
{
	LobbySceneManager.AddLobbyMainScenePointLights(InAddInfo);
}

void AB2LobbyGameMode::AddLobbyFairyFlyingActor(EFairyType InType, class AB2LobbyFlyingFairyDoumi* fairyactor)
{
	LobbySceneManager.AddLobbyFairyFlyingActor(InType, fairyactor);
}

//void AB2LobbyGameMode::AddSummonItemDisplayMatinee(bool bIsMultiple, bool bIsOpen,bool bIsFriendShip, class ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.AddSummonItemDisplayMatinee(bIsMultiple, bIsOpen, bIsFriendShip, Matinee);
//}
//
//void AB2LobbyGameMode::AddSummonItemDisplayMatineeResult(class ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.AddSummonItemDisplayMatineeResult(Matinee);
//}

void AB2LobbyGameMode::PlaySummonItemDisplayMatineeResult()
{
	LobbySceneManager.PlaySummonItemDisplayMatineeResult();
}

//void AB2LobbyGameMode::OnFinishedHallOfFameDisplayMatinee(class ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.OnFinishedHallOfFameDisplayMatinee(Matinee);
//}

//void AB2LobbyGameMode::OnFinishedSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.OnFinishedSummonItemDisplayMatinee(Matinee);
//}

void AB2LobbyGameMode::OnFinishedSummonItemOpenState(bool State)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UISummonItemDisplay* SummonItemDisplay = pUIManager->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

		if (SummonItemDisplay)
			return SummonItemDisplay->SetOpenCardState(State);
	}
}


void AB2LobbyGameMode::OnFinishedSummonItemDisplayResultMatinee()
{
	LobbySceneManager.OnFinishedSummonItemDisplayResultMatinee();
}

//void AB2LobbyGameMode::OnChangedToFxSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.OnChangedToFxSummonItemDisplayMatinee(Matinee);
//}

void AB2LobbyGameMode::NotifyOpenCard(int32 InIndex)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UISummonItemDisplay* SummonItemDisplay = pUIManager->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

		if (SummonItemDisplay)
			SummonItemDisplay->NotifyOpenCard(InIndex);

	}
}

int32 AB2LobbyGameMode::OpenRemainCardCount()
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UISummonItemDisplay* SummonItemDisplay = pUIManager->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

		if (SummonItemDisplay)
			return SummonItemDisplay->GetItemsCount();
	}
	return 0;
}

void AB2LobbyGameMode::BindTouchInput(bool bBind)
{
	if (bBind == bBindTouchInput)
		return;

	//bBindTouchInput = bBind;

	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
	//if (PlayerController)
	//{
	//	auto* IC = PlayerController->InputComponent;
	//	if (IC)
	//	{
	//		if (bBindTouchInput)
	//		{
	//			//PC->
	//			IC->BindAxis("MoveRight", this, &AB2LobbyGameMode::SwipeH);
	//			IC->BindAxis("MoveForward", this, &AB2LobbyGameMode::SwipeV);

	//			//IC->BindAxisKey(EKeys::MouseX, this, &AB2LobbyGameMode::CameraMouseRotateYaw);
	//			IC->BindAxisKey(EKeys::MouseX, this, &AB2LobbyGameMode::MouseX);
	//			IC->BindAxisKey(EKeys::MouseY, this, &AB2LobbyGameMode::MouseY);
	//			auto& BindLFControlStart = IC->BindKey(EKeys::LeftControl, IE_Pressed, this, &AB2LobbyGameMode::LeftControlPressed);
	//			auto& BindLFControlEnd = IC->BindKey(EKeys::LeftControl, IE_Released, this, &AB2LobbyGameMode::LeftControlReleased);
	//			BindLFControlStart.bConsumeInput = false;
	//			BindLFControlEnd.bConsumeInput = false;

	//			auto& BindViewportTouchEnventStart = IC->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AB2LobbyGameMode::ViewportPressMouseEventStart);
	//			BindViewportTouchEnventStart.bConsumeInput = false;

	//			auto& BindViewportTouchEnventEnd = IC->BindKey(EKeys::LeftMouseButton, IE_Released, this, &AB2LobbyGameMode::OnReleasedLeftMouseButton);
	//			BindViewportTouchEnventEnd.bConsumeInput = false;
	//			//<- PC

	//			auto& BindTouchStart = IC->BindTouch(EInputEvent::IE_Pressed, this, &AB2LobbyGameMode::TouchStart);
	//			BindTouchStart.bConsumeInput = false;
	//			auto& BindTouchMove = IC->BindTouch(EInputEvent::IE_Repeat, this, &AB2LobbyGameMode::TouchMove);
	//			BindTouchMove.bConsumeInput = false;
	//			auto& BindTouchEnd = IC->BindTouch(EInputEvent::IE_Released, this, &AB2LobbyGameMode::TouchEnd);
	//			BindTouchEnd.bConsumeInput = false;

	//			IC->BindAction("ZoomOut", IE_Pressed, this, &AB2LobbyGameMode::CameraZoomOut);
	//			IC->BindAction("ZoomIn", IE_Pressed, this, &AB2LobbyGameMode::CameraZoomIn);

	//			PressedInputPosition = FVector2D::ZeroVector;
	//		}
	//		else
	//		{
	//			IC->ClearActionBindings();
	//			IC->TouchBindings.Reset();
	//			IC->AxisBindings.Reset();
	//			IC->AxisKeyBindings.Reset();
	//		}			
	//	}		
	//}
}

void AB2LobbyGameMode::ViewportPressMouseEventStart()
{
	FVector2D NewMousePosition;
	
	auto* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
		ViewportClient->GetMousePosition(NewMousePosition);

	ViewportTouchEventStart(false, NewMousePosition);
}

void AB2LobbyGameMode::ViewportTouchEventStart(bool bInputTypeTouch, const FVector2D& InPressedPosition)
{
	bIsInputTypeTouch = bInputTypeTouch;
	PressedInputPosition = InPressedPosition;
	SetTimeToIgnoreTouchEnd(GetTimeToDecideInputClickOrTab());
	
#if 0 //-> 해결하지 못한 문제가 많아서 결국 꺼 버림. 이 문제를 해결하는 자 미씨를 얻을지어다.
	// 몇몇 장면에서 OnTouchImpulseRing 을 보이지 않는 것은 해당 모드에 많이 설치해 놓은 widget 들의 입력 핸들링과 얽혀 문제가 발생하기 때문.
	// 예를 들어 스크롤박스 등.. TouchEnd 처리에 문제가 좀 있음..
	if (!bHeroMgmtMode && LobbySceneManager.GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_Chapter
		&& LobbySceneManager.GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_StageInfo)
	{
		LobbyConditionalBeginOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Signal(InPressedPosition);
	}
#endif
	
	InputHistory_CheckFlick.InitializeHistory(
#if !UE_BUILD_SHIPPING
		TEXT("CheckFlick")
#endif
	);
	InputHistory_CheckFlick.AddCheckPoint(ElapsedTimeTouchAction, InPressedPosition);
	InputHistory_IgnoreCheck.InitializeHistory(
#if !UE_BUILD_SHIPPING
		TEXT("IgnoreCheck")
#endif
	);
	InputHistory_IgnoreCheck.AddCheckPoint(ElapsedTimeTouchAction, InPressedPosition);
}

void AB2LobbyGameMode::CheckAndDoSwipe(const FVector2D& CurrentPos, const FVector2D& PrevPos)
{
	//*	
	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);

	FVector2D divideScreenSize(360.0f / ScreenSize.X, 1 / ScreenSize.Y);
	FVector2D Delta( (CurrentPos - PrevPos) * divideScreenSize); //일단 비율로 먼저 Delta를 정함
	
	if (!FMath::IsNearlyZero(Delta.X))
		SwipeH(Delta.X);
	if (!FMath::IsNearlyZero(Delta.Y))
		SwipeV(Delta.Y);
	/*/
	//Update
	float DeltaX = (CurrentPos.X - PrevPos.X);
	float DeltaY = (CurrentPos.Y - PrevPos.Y);

	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);

	if (!FMath::IsNearlyZero(DeltaX))
		SwipeH(DeltaX / ScreenSize.X * 2.f);
	if (!FMath::IsNearlyZero(DeltaY))
		SwipeV(DeltaY / ScreenSize.Y);
	//*/
}

void AB2LobbyGameMode::CheckAndDoFlick(const FVector2D& CurrentPos)
{
	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);

	float OutDeltaX = 0.f;
	InputHistory_CheckFlick.AddCheckPoint(ElapsedTimeTouchAction, CurrentPos);
	if (InputHistory_CheckFlick.IsSpeedOverDeltaX(ConsiderFlickScreenDeltaRatio * ScreenSize.X, ConsiderFlickDeltaTime, OutDeltaX))
	{
		FlickH(OutDeltaX / ScreenSize.X);

		// 확실하지는 않은데 FlickH 가 지속적으로 불리는 듯한 경우가 있다. InputHistory_CheckFlick 을 초기화하는 곳이 많지 않아 여기서 추가 처리를 해 봄.
		InputHistory_CheckFlick.InitializeHistory(
#if !UE_BUILD_SHIPPING
			TEXT("CheckFlick")
#endif
		);
	}
}

void AB2LobbyGameMode::TouchStart(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!CurrentTouchData.bIsPressed)
	{
		CurrentTouchData.MainFingerIndex = FingerIndex;
		CurrentTouchData.MainFingerPrevLocation = FVector2D(Location);
		CurrentTouchData.bIsPressed = true;

		ViewportTouchEventStart(true, FVector2D(Location));
	}
	else if (CurrentTouchData.MainFingerIndex != FingerIndex && !CurrentTouchData.bIsPinching)
	{
		CurrentTouchData.SubFingerIndex = FingerIndex;
		CurrentTouchData.SubFingerPrevLocation = FVector2D(Location);
		CurrentTouchData.bIsPinching = true;

		ScreenZoomCenter = FVector2D((CurrentTouchData.MainFingerPrevLocation + CurrentTouchData.SubFingerPrevLocation) * 0.5f);
	}
}

void AB2LobbyGameMode::TouchMove(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!CurrentTouchData.bIsPinching)
	{
		if (CurrentTouchData.bIsPressed && FingerIndex == CurrentTouchData.MainFingerIndex)
		{
			InputHistory_IgnoreCheck.AddCheckPoint(ElapsedTimeTouchAction, FVector2D(Location.X, Location.Y)); // TouchMove 동안 입력을 저장하면서 원치 않는 수준의 입력이 들어왔다 판단되면 Swipe 을 실행하지 않도록..

			if (InputHistory_IgnoreCheck.IsLastHistoryOdd(OddInputMinTimeInterval, OddInputMinPosDeltaRatio)) {
				// 정상적으로 기대되는 상황에서 TouchEnd 가 잘 들어오면 이런 처리가 필요없는데 모바일 입력 상황에서 그렇지 않을 때가 종종 있다. 
				// 궁여지책이지만 TouchEnd 없이 급작스런 입력이 들어왔다 싶을 때에 Swipe 을 하지 않도록. InputHistory 들은 모두 초기화.
				UE_LOG(LogBladeII, Log, TEXT("Ignore TouchMove and skip Swipe as Odd input is detected.. InputHistories are being initialized.."));
				InputHistory_IgnoreCheck.InitializeHistory();
				InputHistory_CheckFlick.InitializeHistory();
			}
			else {
				CheckAndDoSwipe(FVector2D(Location.X, Location.Y), CurrentTouchData.MainFingerPrevLocation);
			}
			
			CurrentTouchData.MainFingerPrevLocation = FVector2D(Location);
		}		
	}
	else
	{
		FVector2D CurrentLocation(Location);

		if (CurrentTouchData.MainFingerIndex == FingerIndex)
		{
			float OriginDist = (CurrentTouchData.MainFingerPrevLocation - CurrentTouchData.SubFingerPrevLocation).Size();
			float CurrentDist = (CurrentLocation - CurrentTouchData.SubFingerPrevLocation).Size();
			
			FVector2D ScreenSize(1.f, 1.f);
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
				ViewportClient->GetViewportSize(ScreenSize);

			float Delta = (OriginDist - CurrentDist) / ScreenSize.X;

			CurrentTouchData.MainFingerPrevLocation = CurrentLocation;

			CameraZoom(Delta * 65.f);
		}
		else if (CurrentTouchData.SubFingerIndex == FingerIndex)
		{
			float OriginDist = (CurrentTouchData.MainFingerPrevLocation - CurrentTouchData.SubFingerPrevLocation).Size();
			float CurrentDist = (CurrentTouchData.MainFingerPrevLocation - CurrentLocation).Size();

			FVector2D ScreenSize(1.f, 1.f);
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
				ViewportClient->GetViewportSize(ScreenSize);

			float Delta = (OriginDist - CurrentDist) / ScreenSize.X;

			CurrentTouchData.SubFingerPrevLocation = CurrentLocation;

			CameraZoom(Delta * 65.f);
		}
	}

	LobbyMoveOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Signal(FVector2D(Location));
}

void AB2LobbyGameMode::TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	InputHistory_IgnoreCheck.InitializeHistory(
#if !UE_BUILD_SHIPPING
		TEXT("IgnoreCheck")
#endif
	);

	if (!CurrentTouchData.bIsPressed)
		return;

	if (!CurrentTouchData.bIsPinching)
	{
		if (FingerIndex == CurrentTouchData.MainFingerIndex)
		{
			CurrentTouchData.bIsPressed = false;
			CheckAndDoFlick(FVector2D(Location));
		}
	}
	else
	{
		if (FingerIndex == CurrentTouchData.MainFingerIndex || FingerIndex == CurrentTouchData.SubFingerIndex)
		{
			CurrentTouchData.bIsPinching = false;
			CurrentTouchData.bIsPressed = false;
		}
	}

	if (bPendingSetTouchingRotateAreaInHeroMgnt)
	{
		bIsTouchingRotateAreaInHeroMgnt = bPendingSetTouchingRotateAreaInHeroMgnt = false;
	}

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal();
}

void AB2LobbyGameMode::OnReleasedLeftMouseButton()
{ 
	SetPressedInputInUI(false); 

	FVector2D MousePosition(0.f, 0.f);

	auto* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
		ViewportClient->GetMousePosition(MousePosition);

	CheckAndDoFlick(MousePosition);

	if (bPendingSetTouchingRotateAreaInHeroMgnt)
	{
		bIsTouchingRotateAreaInHeroMgnt = bPendingSetTouchingRotateAreaInHeroMgnt = false;
	}

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal();
}

float AB2LobbyGameMode::GetHeroTowerUITowerPosition() const
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UIHeroTowerReady* pHeroTowerReadyUI = pUIManager->GetUI<UB2UIHeroTowerReady>(UIFName::HeroTowerReady);

		if (pHeroTowerReadyUI)
			return pHeroTowerReadyUI->GetTowerPosition();
	}

	return 0.0f;
}

EHeroTowerReadyUIState AB2LobbyGameMode::GetHeroTowerUIState() const
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UIHeroTowerReady* pHeroTowerReadyUI = pUIManager->GetUI<UB2UIHeroTowerReady>(UIFName::HeroTowerReady);

		if (pHeroTowerReadyUI)
			return pHeroTowerReadyUI->GetHeroTowerUIState();
	}

	return EHeroTowerReadyUIState::None;
}

int32 AB2LobbyGameMode::GetHeroTowerStartFloor() const
{
	return UB2UIDocHelper::GetDocHeroTower()->GetStartFloor();
}

int32 AB2LobbyGameMode::GetHeroTowerMaxFloor() const
{
	return BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor();
}

bool AB2LobbyGameMode::IsHeroTowerMaxFloorCleared() const
{
	return UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor() >= BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor();
}

void AB2LobbyGameMode::GuildMapInitCarmeraPosSetting(const FVector& Camera1, const FVector& Camera2, const FVector& Camera3)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UIGuildMapMain* pGuildMap = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

		if (pGuildMap)
			return pGuildMap->CameraPosSetting(Camera1.Y, Camera2.Y, Camera3.Y);
	}
}

void AB2LobbyGameMode::GuildMapFinishMatinee()
{

	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (!pUIManager)
		return;

	UB2UIGuildMapMain* pGuildMapMain = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	if (!pGuildMapMain)
		return;

	pGuildMapMain->FinishMatineeSetting();
}



void AB2LobbyGameMode::GuildMapMoveScroll(ACameraActor* CameraActor)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (!pUIManager)
		return;

	UB2UIGuildMapMain* pGuildMapMain = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	if (!pGuildMapMain)
		return;

	float ScrollOffSet = pGuildMapMain->ScrollCameraSetting();

	FVector CameraLocation;

	CameraLocation =CameraActor->GetActorLocation();
	CameraActor->SetActorLocation(FVector(CameraLocation.X, ScrollOffSet, CameraLocation.Z));


}

void AB2LobbyGameMode::GuildMapDoneMoveScroll(ACameraActor* CameraActor)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (!pUIManager)
		return;

	UB2UIGuildMapMain* pGuildMapMain = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	if (!pGuildMapMain)
		return;

	float ScrollOffSet = pGuildMapMain->DoneMoveCameraSetting();

	FVector CameraLocation;

	CameraLocation = CameraActor->GetActorLocation();
	CameraActor->SetActorLocation(FVector(CameraLocation.X, ScrollOffSet, CameraLocation.Z));
}

#define SET_GUILDCASTLE_EFFECT(Index , Location)											\
	if (Index == static_cast<int32>(EUIGuildSelectEffect::End)){							\
		for (auto& El : GuildEffects)														\
		{																					\
			El->GetRootComponent()->SetVisibility(false);									\
		}																					\
		return;																				\
	}																						\
	GuildEffects[Index]->SetActorLocation(Location);										\
	GuildEffects[Index]->GetRootComponent()->SetVisibility(true);							\



void AB2LobbyGameMode::SetGuildSelectEffects(AEmitter* Maker_01, AEmitter* Maker_02, AEmitter* Maker_03, AEmitter* Maker_04)
{
	GuildEffects.Empty();

	GuildEffects.SetNum(4);
	GuildEffects[0] = Maker_01;
	GuildEffects[1] = Maker_02;
	GuildEffects[2] = Maker_03;
	GuildEffects[3] = Maker_04;

	SET_GUILDCASTLE_EFFECT(static_cast<int32>(EUIGuildSelectEffect::End), FVector(0, 0, 0));
}
void AB2LobbyGameMode::SetGuildEffectsLocation(int32 Type, FVector EffectLocation)
{
	if (Type == static_cast<int32>(EUIGuildSelectEffect::Rival))
	{
		SET_GUILDCASTLE_EFFECT(0 , EffectLocation)
	}
	else if (Type == static_cast<int32>(EUIGuildSelectEffect::Team))
	{
		SET_GUILDCASTLE_EFFECT(2, EffectLocation)
		GuildEffects[2]->GetRootComponent()->SetVisibility(EffectLocation == GuildEffects[1]->GetActorLocation() ? false : true);
	}
	else if (Type == static_cast<int32>(EUIGuildSelectEffect::MyCastle))
	{
		SET_GUILDCASTLE_EFFECT(1, EffectLocation)
	}
	else
	{
		SET_GUILDCASTLE_EFFECT(static_cast<int32>(EUIGuildSelectEffect::End), EffectLocation)
	}
}

void AB2LobbyGameMode::SelectLobbyFocusPCClass(EPCClass SelectClass)
{
	if (SelectClass == EPCClass::EPC_End)
		LobbySceneManager.ChangeLobbyScene(ELobbyScene::ELobbyScene_LobbyMain);
	
	else
		LobbySceneManager.ChangeLobbyScene(ELobbyScene::ELobbyScene_CharacterObserve);
}

EPCClass AB2LobbyGameMode::GetSuitableFocusPCClass()
{
	int32 SuitablePCClassIdx = GetMaxPCClassNum();
	int32 HighestCombatPower = 0;

	for (int32 ClassIdx = 0; ClassIdx < GetMaxPCClassNum(); ClassIdx++)
	{
		if (UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(ClassIdx))
		{
			const int32 ClassCombatPower = DocHero->GetCombatPower();
			if (ClassCombatPower > HighestCombatPower)
			{
				HighestCombatPower = ClassCombatPower;
				SuitablePCClassIdx = ClassIdx;
			}
		}
	}

	const EPCClass SuitablePCClass = IntToPCClass(SuitablePCClassIdx);
	if (SuitablePCClass == EPCClass::EPC_End)
		return EPCClass::EPC_Assassin;

	return SuitablePCClass;
}

bool AB2LobbyGameMode::IsFirstTimePreRenderPending() const
{ // 게임 설치 후 첫 실행. 내지는 로컬 상태 리셋 후 한 번 있을 일..
	return PreRenderer && PreRenderer->IsPendingForPreRenderGM();
}

void AB2LobbyGameMode::ToggleShowLobbyWing()
{
	bTestShowLobbyWing ^= true;
	LobbySceneManager.SetUnconditionalShowWing(bTestShowLobbyWing);
}

void AB2LobbyGameMode::SetPeerTeam()
{
	if (GetPeerType() != PEER_SUPER)
		return;

	//팀정보 세팅 안함

	// 방송. 호스트껄 믿어라.
	UE_LOG(LogBladeII, Log, TEXT("[SetPeerTeam] RED = %d, Bule = %d"), GetMatchPlayerNum(ETeamType::ETT_Red), GetMatchPlayerNum(ETeamType::ETT_Blue));
	NetSendMatchPlayerTeamInfo();
}

uint8 AB2LobbyGameMode::GetPlayerStartIndexByNetId(uint32 NetId, ETeamType PlayerTeamType)
{
	// 호스트만 이거 써야한다.
	check(GetPeerType() == PEER_SUPER);

	uint8 nStartIndex = 1;

	for (TPair<uint32, FLobbyMatchPeerInfo>& peerInfo : AllPeerInfos)
	{
		if (peerInfo.Key == NetId)
		{
			if (PlayerTeamType == ETeamType::ETT_Red)
				return (nStartIndex * 2) - 1;
			if (PlayerTeamType == ETeamType::ETT_Blue)
				return nStartIndex * 2;
		}

		if (peerInfo.Value.PlayerTeamType == PlayerTeamType)
			nStartIndex++;
	}

	return 0;
}

void AB2LobbyGameMode::GetControlPlayerInfo(TArray<FB2PeerInfo>& InPeerInfos)
{
	InPeerInfos.Sort([](FB2PeerInfo A, FB2PeerInfo B)->bool {
		bool Result = false;
		if (A.MatchGrade < B.MatchGrade)
		{
			Result = true;
		}
		else if (A.MatchGrade == B.MatchGrade)
		{
			if (A.MatchStar > B.MatchStar)
			{
				Result = true;
			}
		}
		return Result;
	});

	if (InPeerInfos.Num() == CONTROL_GAME_MAX_PLAYERS)
	{
		if (FMath::RandBool() == true)
		{
			//[JH][todo] 다른곳(AllPeerInfos)에서 팀세팅을 하는 부분이 있어서 정리가 필요함
			for (int i = 0; i < InPeerInfos.Num(); i++)
			{
				if (InPeerInfos.IsValidIndex(i))
				{
					bool isOdd = (i & 1) == 1; // i % 2 == 1
					if (isOdd)
					{
						InPeerInfos[i].PlayerTeamType = ETeamType::ETT_Red;
						InPeerInfos[i].PlayerStartIndex = i;
					}
					else
					{
						InPeerInfos[i].PlayerTeamType = ETeamType::ETT_Blue;
						InPeerInfos[i].PlayerStartIndex = i + 2;
					}
				}
			}
		}
		else
		{
			InPeerInfos[0].PlayerTeamType = ETeamType::ETT_Blue;
			InPeerInfos[0].PlayerStartIndex = 2;
			InPeerInfos[3].PlayerTeamType = ETeamType::ETT_Blue;
			InPeerInfos[3].PlayerStartIndex = 4;
			InPeerInfos[4].PlayerTeamType = ETeamType::ETT_Blue;
			InPeerInfos[4].PlayerStartIndex = 6;
			
			InPeerInfos[1].PlayerTeamType = ETeamType::ETT_Red;
			InPeerInfos[1].PlayerStartIndex = 1;
			InPeerInfos[2].PlayerTeamType = ETeamType::ETT_Red;
			InPeerInfos[2].PlayerStartIndex = 3;
			InPeerInfos[5].PlayerTeamType = ETeamType::ETT_Red;
			InPeerInfos[5].PlayerStartIndex = 5;
		}
	}
	else
	{
		for (int cnt = 0; cnt < InPeerInfos.Num(); cnt++)
		{
			if (cnt % 2 == 0)
			{
				InPeerInfos[cnt].PlayerTeamType = ETeamType::ETT_Red;
			} 
			else
			{
				InPeerInfos[cnt].PlayerTeamType = ETeamType::ETT_Blue;
			}
		}

		int32 RedIDX = 1;
		int32 BlueIDX = 1;
		for (int cnt = 0; cnt < InPeerInfos.Num(); cnt++)
		{
			if (InPeerInfos[cnt].PlayerTeamType == ETeamType::ETT_Red)
			{
				InPeerInfos[cnt].PlayerStartIndex = (RedIDX * 2) - 1;
				RedIDX++;
			}
			else
			{
				InPeerInfos[cnt].PlayerStartIndex = BlueIDX * 2;
				BlueIDX++;
			}
		}
	}
}

void AB2LobbyGameMode::GetTutorialControlPlayerInfo(TArray<FB2PeerInfo>& InPeerInfos)
{
	int32 RedIDX = 1;
	int32 BleuIDX = 1;
	for (FB2PeerInfo& Elem : InPeerInfos)
	{
		if (Elem.PlayerTeamType == ETeamType::ETT_Red)
		{
			Elem.PlayerStartIndex = (RedIDX * 2) - 1;
			RedIDX++;
		}
		else if (Elem.PlayerTeamType == ETeamType::ETT_Blue)
		{
			Elem.PlayerStartIndex = BleuIDX * 2;
			BleuIDX++;
		}
	}
}

void AB2LobbyGameMode::StartControlTutorial()
{
	ClearMatchRoom();

	// 가짜 플레이어 만들기 나포함 101 ~ 108로 설정될듯. 숫자는 큰의미 없음
	uint32 nFakePlayerNetId = 102;
	uint32 nCurrentFakePlayerNetId = nFakePlayerNetId;

	SetPeerType(PEER_SUPER);

	// 훼이크 넷아이디 바로앞숫자를 내껄로.
	SetMyNetId(nFakePlayerNetId - 1);

	EmplacePeerPlayer(GetMyNetId());

	// 레드로 한번 강제설정
	if (AllPeerInfos.Contains(GetMyNetId()))
	{
		AllPeerInfos[GetMyNetId()].PlayerTeamType = ETeamType::ETT_Red;
	}

	const TArray<FTutorialControlBotInfo>* TutorialControlBotInfos = TutorialManager::GetInstance().GetTutorialControlBotInfos();

	for (FTutorialControlBotInfo TutorialControlBotInfo : *TutorialControlBotInfos)
	{
		//7개 넘으면 안된다.
		if (nCurrentFakePlayerNetId >= nFakePlayerNetId + 7)
		{
			check(0)
			return;
		}

		// 리모트 데이터 하나 추가
		RemotePlayerInfoData.Add(nCurrentFakePlayerNetId);

		b2network::B2FindAccountInfoPtr pAccountInfo = std::make_shared<b2network::B2FindAccountInfo>();
		pAccountInfo->nickname = TutorialControlBotInfo.BotName;
		
		pAccountInfo->characters.Add(std::make_shared<b2network::B2CharacterServerInfo>());
		pAccountInfo->characters[0]->character_type = CliToSvrPCClassType(TutorialControlBotInfo.BotPCClass);
		
		pAccountInfo->characters[0]->skill_quick_slots.Add(std::make_shared<b2network::B2SkillQuickSlot>());
		pAccountInfo->characters[0]->skill_quick_slots[0]->using_skill_id_1 = TutorialControlBotInfo.BotSkillId;
		
		pAccountInfo->characters[0]->skills.Add(std::make_shared<b2network::B2SkillServerInfo>());
		pAccountInfo->characters[0]->skills[0]->skill_id = TutorialControlBotInfo.BotSkillId;
		pAccountInfo->characters[0]->skills[0]->skill_level = 1;

		pAccountInfo->characters[0]->level = TutorialControlBotInfo.BotLevel;

		//아이템 추가
		for (FTutorialControlBotItemInfo TutorialControlBotItemInfo : TutorialControlBotInfo.BotEquipItem)
		{
			b2network::B2ItemServerInfoPtr pItemInfo = std::make_shared<b2network::B2ItemServerInfo>();
			pItemInfo->character_type = pAccountInfo->characters[0]->character_type;
			pItemInfo->template_id = TutorialControlBotItemInfo.ItemTemplateId;

			// 가짜 아이템 아이디(서버)는 템플릿아이디 그대로씀 그냥(중복만 안되면 상관없겠지)
			pItemInfo->id = pItemInfo->template_id;

			pItemInfo->equip_position = TutorialControlBotItemInfo.ItemEquipPos;
			pItemInfo->amount = 1;
			pItemInfo->grade = 1;
			pItemInfo->quality = 1;
			pItemInfo->level = 1;
			// 이걸로 공방체 설정(원래 서버에서 아이템정보 받을때 받음)
			pItemInfo->md_main_attribute_value = TutorialControlBotItemInfo.ItemBaseStat;

			pAccountInfo->equip_items.Add(pItemInfo);

			b2network::B2ItemPresetPtr pItemPreset = std::make_shared<b2network::B2ItemPreset>();
			pItemPreset->character_type = pAccountInfo->characters[0]->character_type;
			pItemPreset->preset_id = 0;
			pItemPreset->equip_position = pItemInfo->equip_position;
			pItemPreset->item_id = pItemInfo->id;
			pAccountInfo->item_presets.Add(pItemPreset);
		}

		// 리모트 플레이어인포에 추가.
		RemotePlayerInfoData[nCurrentFakePlayerNetId].SetModPlayerInfo(pAccountInfo);
		RemotePlayerInfoData[nCurrentFakePlayerNetId].SetMainPlayerClass(TutorialControlBotInfo.BotPCClass);

		AllPeerInfos.Emplace(nCurrentFakePlayerNetId, FLobbyMatchPeerInfo());
		AllPeerInfos[nCurrentFakePlayerNetId].ReservedPCClass = TutorialControlBotInfo.BotPCClass;
		AllPeerInfos[nCurrentFakePlayerNetId].PlayerTeamType = TutorialControlBotInfo.BotTeamType;
		
		// 페이크 넷아이디 올려줌
		nCurrentFakePlayerNetId++;
	}
	// 가짜 플레이어 만들기 종료

	// 룸클로즈 스킵하구 그뒤로 연결
	SendPrerequisiteToStart(b2network::B2RoomType::BRT_Assault);
}

void AB2LobbyGameMode::NoResponsePrerequisiteToStart()
{
	packet::ByteStream	payload;
	FString encoded_string = packet::FinalizePacket(packet::NO_RESPONSE_PREREQUISITE_TO_START, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2LobbyGameMode::ClearWaitPrerequisiteToStartTimeHandler()
{
	GetWorldTimerManager().ClearTimer(WaitPrerequisiteToStartTimeHandler);
}

FVector2D AB2LobbyGameMode::GetTouchingInputCenter()
{
	FVector2D Center(0.f, 0.f);

	auto* ViewportClient = GetWorld()->GetGameViewport();
	BII_CHECK(ViewportClient);
	if (ViewportClient)
	{
		/*
		ViewportClient->GetViewportSize(Center);

		Center.X *= 0.5f;
		Center.Y *= 0.5f;
		*/

		if (CurrentTouchData.bIsPinching)
		{
			Center.Y = ScreenZoomCenter.Y;
		}
		else
		{
			FVector2D MousePosition(0.f, 0.f);
			ViewportClient->GetMousePosition(MousePosition);

			Center.Y = MousePosition.Y;
		}
	}
	
	return Center;
}

//////////////////////////////////////////////////
//
// Camera work in Lobby
//

void AB2LobbyGameMode::CameraZoomIn()
{
	CameraZoom(-1.f);
}

void AB2LobbyGameMode::CameraZoomOut()
{
	CameraZoom(1.f);
}

void AB2LobbyGameMode::CameraZoom(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.Zoom(Value, GetTouchingInputCenter());
	}		
}

void AB2LobbyGameMode::SwipeH(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.SwipeH(-Value);
	}		
}

void AB2LobbyGameMode::SwipeV(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.SwipeV(-Value);
	}		
}

void AB2LobbyGameMode::FlickH(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.FlickH(Value < 0.f);
		
		SwipeH(Value);

		BeginLobbySceneFreeRotate(Value);
	}
}

void AB2LobbyGameMode::BeginLobbySceneFreeRotate(float InLastSwipeValue)
{ // 터치 and swipe 후 감속을 동반한 자유 회전 시작.
	bDeacceleratingCharRotate = true;
	DeaccelerationCharRotate = (InLastSwipeValue < 0.f ? 1.f : -1.f) * LobbySceneManager.GetCharacterCameraRotateDeaccelerationFactor(); // 조작했던 방향과 반대로 deaccelerate
	RotationSpeedCharRotate = InLastSwipeValue * LobbySceneManager.GetCharacterCameraRotateFactor(); // 감속 시작 시의 초기값. FLobbySceneManager::SwipeH 처리에서 동일한 속도로 이어지도록.
	RotationSpeedCharRotate = FMath::Clamp(RotationSpeedCharRotate, -1.0f * LobbySceneManager.GetCharacterCameraFreeRotateMaxSpeed(), LobbySceneManager.GetCharacterCameraFreeRotateMaxSpeed());
	// 카메라 돌리는 거랑 캐릭터 돌리는 거랑 스케일이 좀 달라서 일부 씬에서는 추가 스케일을 주는데
	// 이렇게 씬 이름을 직접 명시하는 식이면 구질구질해서 미스나기 쉽지만 미스가 난다고 해서 큰일 날 것도 아니고 하니 이렇게 가는 거.
	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_HeroManagement ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_CollectBookMain ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_FindAccount)
	{
		RotationSpeedCharRotate *= LobbySceneManager.GetCharacterFreeRotateAdditionalScale();
	}

	UE_LOG(LogBladeII, Log, TEXT("[BeginLobbySceneFreeRotate], DeaccelerationCharRotate %.3f  RotationSpeedCharRotate %.3f"), DeaccelerationCharRotate, RotationSpeedCharRotate);
}

void AB2LobbyGameMode::FlickV(float Value)
{

}

void AB2LobbyGameMode::MouseMoved()
{
	//auto* ViewportClient = GetWorld()->GetGameViewport();

	//if (ViewportClient)
	//{
	//	FVector2D NewMousePosition;
	//	if (ViewportClient->GetMousePosition(NewMousePosition))
	//	{
	//		auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
	//		if (bIsInputLeftControl)
	//		{
	//			FVector2D CurrentLocation(NewMousePosition);

	//			FVector2D ScreenSize(1.f, 1.f);
	//			if (ViewportClient != nullptr)
	//				ViewportClient->GetViewportSize(ScreenSize);

	//			CurrentTouchData.SubFingerPrevLocation = FVector2D(CurrentTouchData.MainFingerPrevLocation.X - ScreenSize.X * 0.5f, CurrentTouchData.MainFingerPrevLocation.Y - ScreenSize.Y * 0.5f);

	//			float OriginDist = (CurrentTouchData.MainFingerPrevLocation - CurrentTouchData.SubFingerPrevLocation).Size();
	//			float CurrentDist = (CurrentLocation - CurrentTouchData.SubFingerPrevLocation).Size();

	//			float Delta = (OriginDist - CurrentDist) / ScreenSize.X;

	//			CurrentTouchData.MainFingerPrevLocation = CurrentLocation;

	//			CameraZoom(Delta * 65.f);

	//		}
	//		else if (bPressedInputInUI || (PlayerController && PlayerController->IsInputKeyDown(EKeys::LeftMouseButton)))
	//		{
	//			CheckAndDoSwipe(NewMousePosition, PrevMousePosition);
	//		}

	//		PrevMousePosition = NewMousePosition;

	//		LobbyMoveOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Signal(NewMousePosition);
	//	}
	//}
}

void AB2LobbyGameMode::MouseX(float Value)
{
	MouseMoved();
}

void AB2LobbyGameMode::MouseY(float Value)
{
	MouseMoved();
}

void AB2LobbyGameMode::LeftControlPressed()
{
	if(!bIsInputLeftControl)
	{
		auto* ViewportClient = GetWorld()->GetGameViewport();
		if (ViewportClient)
		{
			FVector2D NewMousePosition;
			if (ViewportClient->GetMousePosition(NewMousePosition))
			{
				CurrentTouchData.MainFingerPrevLocation = NewMousePosition;
				UE_LOG(LogBladeII, Log, TEXT("LeftControlPressed MainFingerPrevLocation X : %f, Y : %f"), CurrentTouchData.MainFingerPrevLocation.X, CurrentTouchData.MainFingerPrevLocation.Y);
			}
		}
	}

	bIsInputLeftControl = true;
}

void AB2LobbyGameMode::LeftControlReleased()
{
	bIsInputLeftControl = false;
}

FVector GetViewportSpacePostion(const FGeometry& Geometry, const FPointerEvent& Event)
{
	//FPointerEvent의 ScreenSpacePosition은 PC 모니터 공간의 좌표이기 때문에 게임뷰포로의 전환이 필요함.
	return FVector(Geometry.Position + Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition()), 0.f);
}

#include "Input/Events.h"
FReply AB2LobbyGameMode::TouchStartFromUI(const FGeometry& Geometry, const FPointerEvent& Event)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchStartFromUI"));
	TouchStart(static_cast<ETouchIndex::Type>(Event.GetPointerIndex()), GetViewportSpacePostion(Geometry, Event));

	return FReply::Handled();
}

FReply AB2LobbyGameMode::TouchEndFromUI(const FGeometry& Geometry, const FPointerEvent& Event)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchFromFromUI"));
	TouchEnd(static_cast<ETouchIndex::Type>(Event.GetPointerIndex()), GetViewportSpacePostion(Geometry, Event));

	return FReply::Handled();
}

FReply AB2LobbyGameMode::TouchMoveFromUI(const FGeometry& Geometry, const FPointerEvent& Event)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchMoveFromUI"));
	TouchMove(static_cast<ETouchIndex::Type>(Event.GetPointerIndex()), GetViewportSpacePostion(Geometry, Event));

	return FReply::Handled();
}

bool AB2LobbyGameMode::IsIgnoreTouchEnd()
{ 
	if (TimeLimitTouchToClick < 0.f || ElapsedTimeTouchAction < TimeLimitTouchToClick)
	{
		FVector2D CurInputPosition(FLT_MAX, FLT_MAX);

		auto* ViewportClient = GetWorld()->GetGameViewport();
		check(ViewportClient);
		
		if (bIsInputTypeTouch)
		{
			if (CurrentTouchData.bIsPinching)
				return true;

			CurInputPosition = FVector2D(CurrentTouchData.MainFingerPrevLocation);
		}
		else
		{				
			ViewportClient->GetMousePosition(CurInputPosition);
		}

		FVector2D ViewportSize(1.f, 1.f);
		ViewportClient->GetViewportSize(ViewportSize);

		FVector2D OldInputPosition = PressedInputPosition;
		PressedInputPosition = FVector2D::ZeroVector;

		return (CurInputPosition - OldInputPosition).Size() / ViewportSize.X > 0.04f;
	}

	return true; 
}

void AB2LobbyGameMode::SetTimeToIgnoreTouchEnd(float LimitTime)
{
	ElapsedTimeTouchAction = 0.f;
	TimeLimitTouchToClick = LimitTime;
}

void AB2LobbyGameMode::TickTimeTouchAction(float DeltaSeconds)
{
	if (TimeLimitTouchToClick > 0.f)
	{
		ElapsedTimeTouchAction += DeltaSeconds;
		
		InputHistory_CheckFlick.AddCheckPoint(ElapsedTimeTouchAction, bIsInputTypeTouch ? CurrentTouchData.MainFingerPrevLocation : PrevMousePosition);

		if (bDeacceleratingCharRotate)
		{
			RotationSpeedCharRotate += DeaccelerationCharRotate * DeltaSeconds;

			if (RotationSpeedCharRotate * DeaccelerationCharRotate >= 0) // 감속 결과로 방향이 반대가 되면 중단.
			{
				UE_LOG(LogBladeII, Log, TEXT("[TickTimeTouchAction] Stopping Deacceleration.. DeaccelerationCharRotate %.3f, RotationSpeedCharRotate %.3f"),
					DeaccelerationCharRotate, RotationSpeedCharRotate);

				RotationSpeedCharRotate = 0.0f;
				bDeacceleratingCharRotate = false;
				InputHistory_CheckFlick.InitializeHistory();
			}
			else
			{
				SwipeH(RotationSpeedCharRotate);
			}
		}
	}		
}

#if !UE_BUILD_SHIPPING
void AB2LobbyGameMode::DevOnlyGenerateDummyChat(float DeltaSeconds)
{
	//static float AccumulatedTime = 0.0f;

	//AccumulatedTime += DeltaSeconds;
	//if (AccumulatedTime > 0.1f)
	//{
	//	FB2ChatMessage NewMsgInfo;
	//	NewMsgInfo.SendUserNick = (FMath::RandBool() == true) ? FText::FromString(TEXT("DummyChatUser")) : FText::FromString(TEXT("DumbAss"));
	//	NewMsgInfo.MessageText = (FMath::RandBool() == true) ?
	//		FText::FromString(TEXT("Short dummy chat message.")) :
	//		FText::FromString(TEXT("This is dummy chat message.. This is dummy chat message. This is dummy chat message"));

	//	BladeIIGameImpl::GetChatStore().AddChatMessage(EB2ChatType::Channel, NewMsgInfo);

	//	// 한줄 채팅도 테스트 할 수 있게..
	//	NewMsgInfo.MessageType = EB2ChatType::Channel;
	//	UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Signal(NewMsgInfo);

	//	AccumulatedTime = 0.0f;
	//}

	//static float AccumulatedTimeForPublicNoti = 0.0f;
	//AccumulatedTimeForPublicNoti += DeltaSeconds;
	//if (AccumulatedTimeForPublicNoti > 0.1f)
	//{
	//	FB2ChatMessage NewMsgInfo;
	//	NewMsgInfo.SendUserNick = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti"));
	//	NewMsgInfo.MessageText = (FMath::RandBool() == true) ?
	//		FText::FromString(TEXT("Short dummy public announcement message.")) :
	//		FText::FromString(TEXT("This is dummy public announcement message.. This is dummy public announcement message. This is dummy announcement message"));

	//	BladeIIGameImpl::GetChatStore().AddChatMessage(EB2ChatType::Notice, NewMsgInfo);

	//	// 한줄 채팅도 테스트 할 수 있게..
	//	NewMsgInfo.MessageType = EB2ChatType::Notice;
	//	UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Signal(NewMsgInfo);

	//	AccumulatedTimeForPublicNoti = 0.0f;
	//}

	//UpdateChatUIClass<bool>::GetInstance().Signal(true);
}
#endif

void AB2LobbyGameMode::UpdateChatUI(bool bIsScrollEnd)
{
}

void AB2LobbyGameMode::ExtraOnPreLoadMap()
{
	if (LobbyUIManager)
	{ // 맵 내릴 시에 DJLegacy UI 에 로딩된 TAsset 들 Unload 필요..
		LobbyUIManager->OnPreLoadMap();
	}

	// 첫 전투맵 로딩 시 시나리오 동영상 대신 엉뚱하게 타이틀 동영상이 나온다는 제보가 있어서.. 이유를 알 수가 없지만 뭔가 하나의 가능성 제거
	if (!IsFirstLobby)
	{
		AB2DLCFrontGameMode::ClearSelectedFrontTitleMovieInfo();
	}
}

void AB2LobbyGameMode::OnPlatformPause()
{
	// 1. 초대 거절 - 다른 액션들이 필요하면 함수로 빼야함
	const TArray<EB2GameMode> InvGameModeList{ EB2GameMode::PVP_Tag, EB2GameMode::Raid };

	for (const auto GameModeType : InvGameModeList)
	{
		if (HasGameInvitation(GameModeType))
			RequestDeclineInvitation(GameModeType);
	}
}

void AB2LobbyGameMode::FInputHistory::InitializeHistory(
#if !UE_BUILD_SHIPPING
	const FString& OptionalLoggingHistoryName
#endif
)
{
#if !UE_BUILD_SHIPPING
	if (OptionalLoggingHistoryName.Len() > 0) // 겸사겸사 로그 뿌릴지 지정하는 매개변수로도.
	{
		UE_LOG(LogBladeII, Log, TEXT("InputHistory %s Initialize.. InputHistoryLastIndex %d"), *OptionalLoggingHistoryName, InputHistoryLastIndex);
	}
#endif

	InputHistoryLastIndex = INDEX_NONE;
	FMemory::Memzero(&InputPointHistory, sizeof(FInputHistoryElem) * MAX_INPUT_HISTORY_INDEX);
}

void AB2LobbyGameMode::FInputHistory::AddCheckPoint(float Time, const FVector2D& Position)
{
	if (InputPointHistory[InputHistoryLastIndex].CheckPointTime == Time)
		return;

	check(InputHistoryLastIndex + 1 > -1);

	InputHistoryLastIndex = (InputHistoryLastIndex + 1) % MAX_INPUT_HISTORY_INDEX;

	InputPointHistory[InputHistoryLastIndex].CheckPointTime = Time;
	InputPointHistory[InputHistoryLastIndex].CheckPointPosition = Position;	
	InputPointHistory[InputHistoryLastIndex].bInitialized = true;
}

bool AB2LobbyGameMode::FInputHistory::IsSpeedOverDeltaX(float DeltaX, float Time, float& OutDeltaX)
{
	for (int32 i = GetPrevHistoryIndexOf(InputHistoryLastIndex); i != InputHistoryLastIndex  ; i = GetPrevHistoryIndexOf(i))
	{
		if (!InputPointHistory[i].bInitialized)
			return false;

		if (InputPointHistory[InputHistoryLastIndex].CheckPointTime - InputPointHistory[i].CheckPointTime > Time)
		{
			return false;
		}
				
		if (DeltaX < FMath::Abs(InputPointHistory[InputHistoryLastIndex].CheckPointPosition.X - InputPointHistory[i].CheckPointPosition.X))
		{			
			OutDeltaX = InputPointHistory[InputHistoryLastIndex].CheckPointPosition.X - InputPointHistory[(InputHistoryLastIndex + MAX_INPUT_HISTORY_INDEX - 1) % MAX_INPUT_HISTORY_INDEX].CheckPointPosition.X;
			OutDeltaX = OutDeltaX * (1.f - (InputPointHistory[InputHistoryLastIndex].CheckPointTime - InputPointHistory[(InputHistoryLastIndex + MAX_INPUT_HISTORY_INDEX - 1) % MAX_INPUT_HISTORY_INDEX].CheckPointTime));
			return true;
		}			
	}

	return false;
}

bool AB2LobbyGameMode::FInputHistory::IsLastHistoryOdd(float MinOddTimeInterval, float MinOddPosDeltaRatio)
{
	if (InputHistoryLastIndex < 0) {
		return false; // History 가 없음. 판별 불가
	}

	int32 AddedOldHistoryCount = 0;
	FVector2D AddedCheckPointDelta = FVector2D(0.0f,0.0f);
	// 마지막꺼 제외하고 서로간의 delta 를 더함
	for (int32 IA = GetPrevHistoryIndexOf(InputHistoryLastIndex); IA != GetNextHistoryIndexOf(InputHistoryLastIndex); IA = GetPrevHistoryIndexOf(IA))
	{
		const int32 IB = GetPrevHistoryIndexOf(IA);

		const FInputHistoryElem& InputHistoryA = InputPointHistory[IA];
		const FInputHistoryElem& InputHistoryB = InputPointHistory[IB]; // A 에서 한번 더 이전꺼

		if (!InputHistoryA.bInitialized || !InputHistoryB.bInitialized) {
			continue; // 둘 다 유의미한 값이 있어야겠다.
		}
		++AddedOldHistoryCount;

		AddedCheckPointDelta = InputHistoryA.CheckPointPosition - InputHistoryB.CheckPointPosition;
	}

	if (AddedOldHistoryCount >= 2) // 이전 추이와 비교하려는 것이니 아예 없거나 하나만 가지고서는 비교 불가함.
	{
		const FInputHistoryElem& LastHistory = InputPointHistory[InputHistoryLastIndex];
		const FInputHistoryElem& LastLastHistory = InputPointHistory[GetPrevHistoryIndexOf(InputHistoryLastIndex)];
		checkSlow(LastHistory.bInitialized && LastLastHistory.bInitialized);

		FVector2D LastPosDelta = LastHistory.CheckPointPosition - LastLastHistory.CheckPointPosition;
		float LastTimeDelta = FMath::Abs(LastHistory.CheckPointTime - LastLastHistory.CheckPointTime);

		if (LastTimeDelta >= MinOddTimeInterval &&
			// PosDelta 는 각 요소별로 본다. 방향이 반대면서 일정 크기 이상으로 튀었는지.
			(
				(LastPosDelta.X * AddedCheckPointDelta.X < 0.0f && FMath::Abs(LastPosDelta.X) >= FMath::Abs(AddedCheckPointDelta.X * MinOddPosDeltaRatio)) ||
				(LastPosDelta.Y * AddedCheckPointDelta.Y < 0.0f && FMath::Abs(LastPosDelta.Y) >= FMath::Abs(AddedCheckPointDelta.Y * MinOddPosDeltaRatio))
			)
			)
		{
			return true;
		}
	}

	return false;
}
